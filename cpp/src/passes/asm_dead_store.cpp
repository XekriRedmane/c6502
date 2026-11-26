#include "passes/asm_dead_store.h"

#include <algorithm>
#include <unordered_set>

#include "passes/asm_aliasing.h"
#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes::asm_dead_store {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

constexpr int kPoolLo = 0x80;
constexpr int kPoolHi = 0x100;

bool starts_with(std::string_view s, std::string_view prefix) {
  return s.substr(0, prefix.size()) == prefix;
}

// Returns whether `target` is a byte of DPTR, the scratch pointer that
// a function called directly neither reads on entry nor keeps, so its
// value dies at such a call and at a return. The `icall` trampoline
// jumps through it.
bool is_call_scratch(const ir::Type_operand& target) {
  const auto* d = as<ir::Data>(target);
  return d != nullptr && d->name == "DPTR";
}

bool operands_equal_exact(const ir::Type_operand& a, const ir::Type_operand& b) {
  return isa<ir::ZP, ir::Data>(a) && a == b;
}

}  // namespace

bool is_dead_cfg(Instrs instrs, std::size_t start, const LabelMap& labels,
                 const AsmContext& ctx, const SymbolAddrs* zp_slot_symbols,
                 std::string_view local_prefix,
                 const NameSet& address_taken_names,
                 const ir::Type_operand& target) {
  const bool is_private =
      call_cannot_touch(target, local_prefix, address_taken_names);
  std::unordered_set<std::size_t> visited;
  std::vector<std::size_t> stack = successors(instrs, start, labels);
  while (!stack.empty()) {
    const std::size_t j = stack.back();
    stack.pop_back();
    if (!visited.insert(j).second) {
      continue;
    }
    const ir::Type_instruction& next = instrs[j];
    const auto* call = as<ir::Call>(next);
    if (call != nullptr && call->name != "icall" && is_private) {
      for (const std::size_t s : successors(instrs, j, labels)) {
        stack.push_back(s);
      }
      continue;
    }
    if (call != nullptr && call->name != "icall" && is_call_scratch(target)) {
      continue;
    }
    if (is_opaque(next)) {
      return false;
    }
    if (const auto* jump = as<ir::Jump>(next)) {
      if (!labels.contains(jump->target)) {
        if (!is_dead_at_tail_call(target, &address_taken_names)) {
          return false;
        }
        continue;
      }
    }
    if (isa<ir::Ret, ir::Return>(next)) {
      if (!is_dead_at_exit(target, zp_slot_symbols)) {
        return false;
      }
      continue;
    }
    if (reads(next, target, ctx, zp_slot_symbols, is_private)) {
      return false;
    }
    if (writes_same(next, target)) {
      continue;
    }
    for (const std::size_t s : successors(instrs, j, labels)) {
      stack.push_back(s);
    }
  }
  return true;
}

bool is_opaque(const ir::Type_instruction& instr) {
  return isa<ir::Call, ir::FunctionPrologue, ir::AllocateStack>(instr);
}

NameSet collect_address_taken_names(Instrs instrs) {
  NameSet out;
  for (const auto& instr : instrs) {
    if (const auto* la = as<ir::LoadAddress>(instr)) {
      if (const auto* d = as<ir::Data>(la->src)) {
        out.insert(d->name);
      }
    }
    if (isa<ir::Compare>(instr)) {
      continue;
    }
    for_each_operand<true>(instr, [&](const ir::Type_operand& op) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.insert(hi->name);
      }
    });
  }
  return out;
}

std::vector<std::size_t> successors(Instrs instrs, std::size_t i,
                                    const LabelMap& labels) {
  const ir::Type_instruction& ins = instrs[i];
  if (isa<ir::Ret, ir::Return>(ins)) {
    return {};
  }
  if (const auto* jump = as<ir::Jump>(ins)) {
    const long tgt = labels.find(jump->target);
    if (tgt >= 0) {
      return {static_cast<std::size_t>(tgt)};
    }
    return {};
  }
  if (const auto* branch = as<ir::Branch>(ins)) {
    std::vector<std::size_t> out;
    if (i + 1 < instrs.size()) {
      out.push_back(i + 1);
    }
    const long tgt = labels.find(branch->target);
    if (tgt >= 0) {
      out.push_back(static_cast<std::size_t>(tgt));
    }
    return out;
  }
  if (i + 1 < instrs.size()) {
    return {i + 1};
  }
  return {};
}

bool is_dead_at_exit(const ir::Type_operand& target,
                     const SymbolAddrs* zp_slot_symbols) {
  if (const auto* zp = as<ir::ZP>(target)) {
    const asdl::Int addr = zp->address + zp->offset;
    return kPoolLo <= addr && addr < kPoolHi;
  }
  if (is_call_scratch(target)) {
    return true;
  }
  if (const auto* d = as<ir::Data>(target)) {
    if (starts_with(d->name, "__local_")) {
      return true;
    }
    if (zp_slot_symbols != nullptr) {
      const auto it = zp_slot_symbols->find(d->name);
      if (it != zp_slot_symbols->end()) {
        const asdl::Int addr = it->second + d->offset;
        return kPoolLo <= addr && addr < kPoolHi;
      }
    }
  }
  return false;
}

bool is_dead_at_tail_call(const ir::Type_operand& target,
                          const NameSet* address_taken_names) {
  if (const auto* d = as<ir::Data>(target)) {
    return starts_with(d->name, "__local_") &&
           (address_taken_names == nullptr ||
            address_taken_names->find(d->name) == address_taken_names->end());
  }
  return false;
}

bool mem_dead_after(Instrs instrs, std::size_t idx, const ir::Type_operand& target,
                    const AsmContext& ctx, std::string_view fn_name,
                    const SymbolAddrs* zp_slot_symbols) {
  return mem_dead_after(instrs, LabelMap(instrs), idx, target, ctx, fn_name,
                        zp_slot_symbols);
}

bool mem_dead_after(Instrs instrs, const LabelMap& labels, std::size_t idx,
                    const ir::Type_operand& target, const AsmContext& ctx,
                    std::string_view fn_name, const SymbolAddrs* zp_slot_symbols) {
  const std::string local_prefix = "__local_" + std::string(fn_name) + "__";
  return is_dead_cfg(instrs, idx, labels, ctx, zp_slot_symbols, local_prefix,
                     collect_address_taken_names(instrs), target);
}

bool call_cannot_touch(const ir::Type_operand& target, std::string_view local_prefix,
                       const NameSet& address_taken_names) {
  const auto* d = as<ir::Data>(target);
  return d != nullptr && starts_with(d->name, local_prefix) &&
         address_taken_names.find(d->name) == address_taken_names.end();
}

bool reads(const ir::Type_instruction& instr, const ir::Type_operand& target,
           const AsmContext& ctx, const SymbolAddrs* zp_slot_symbols,
           bool is_private) {
  const MayAliasOptions options{.zp_slot_symbols = zp_slot_symbols};
  for (const auto& op : read_operands(instr)) {
    if (isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(op)) {
      for (const auto& p : ptr_source_reads(op)) {
        if (may_alias(p, target, ctx, options)) {
          return true;
        }
      }
      if (is_private) {
        continue;
      }
    }
    if (may_alias(op, target, ctx, options)) {
      return true;
    }
  }
  return false;
}

bool writes_same(const ir::Type_instruction& instr, const ir::Type_operand& target) {
  if (const auto* d = as<ir::DecimalArith>(instr)) {
    for (const auto& op : d->dst) {
      if (operands_equal_exact(op, target)) {
        return true;
      }
    }
    return false;
  }
  const std::optional<ir::Type_operand> dst = write_operand(instr);
  return dst.has_value() && operands_equal_exact(*dst, target);
}

std::optional<ir::Type_operand> write_operand(const ir::Type_instruction& instr) {
  const ir::Type_operand* dst = nullptr;
  if (const auto* m = as<ir::Mov>(instr)) {
    dst = &m->dst;
  } else if (const auto* i = as<ir::Inc>(instr)) {
    dst = &i->dst;
  } else if (const auto* d = as<ir::Dec>(instr)) {
    dst = &d->dst;
  } else if (const auto* p = as<ir::Pop>(instr)) {
    dst = &p->dst;
  } else if (const auto* la = as<ir::LoadAddress>(instr)) {
    dst = &la->dst;
  }
  if (dst != nullptr && !isa<ir::Reg>(*dst)) {
    return *dst;
  }
  return std::nullopt;
}

std::vector<ir::Type_operand> read_operands(const ir::Type_instruction& instr) {
  std::vector<ir::Type_operand> out;
  const auto add = [&](const ir::Type_operand& op) {
    if (!isa<ir::Reg>(op)) {
      out.push_back(op);
    }
  };
  const auto add_ptr = [&](const ir::Type_operand& op) {
    for (auto& p : ptr_source_reads(op)) {
      out.push_back(std::move(p));
    }
  };
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith: {
      const auto& d = std::get<ir::DecimalArith>(instr);
      out = decimal_arith::reads(d);
      for (const auto& op : decimal_arith::operands(d)) {
        add_ptr(op);
      }
      break;
    }
    case ir::Kind_instruction::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      add(m.src);
      add_ptr(m.dst);
      break;
    }
    case ir::Kind_instruction::Add:
      add(std::get<ir::Add>(instr).src);
      break;
    case ir::Kind_instruction::Sub:
      add(std::get<ir::Sub>(instr).src);
      break;
    case ir::Kind_instruction::And:
      add(std::get<ir::And>(instr).src);
      break;
    case ir::Kind_instruction::Or:
      add(std::get<ir::Or>(instr).src);
      break;
    case ir::Kind_instruction::Xor: {
      const auto& x = std::get<ir::Xor>(instr);
      add(x.src1);
      add(x.src2);
      break;
    }
    case ir::Kind_instruction::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      add(c.left);
      add(c.right);
      break;
    }
    case ir::Kind_instruction::Inc:
      add(std::get<ir::Inc>(instr).dst);
      add_ptr(std::get<ir::Inc>(instr).dst);
      break;
    case ir::Kind_instruction::Dec:
      add(std::get<ir::Dec>(instr).dst);
      add_ptr(std::get<ir::Dec>(instr).dst);
      break;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      add(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      add_ptr(std::get<ir::ArithmeticShiftLeft>(instr).dst);
      break;
    case ir::Kind_instruction::LogicalShiftRight:
      add(std::get<ir::LogicalShiftRight>(instr).dst);
      add_ptr(std::get<ir::LogicalShiftRight>(instr).dst);
      break;
    case ir::Kind_instruction::RotateLeft:
      add(std::get<ir::RotateLeft>(instr).dst);
      add_ptr(std::get<ir::RotateLeft>(instr).dst);
      break;
    case ir::Kind_instruction::RotateRight:
      add(std::get<ir::RotateRight>(instr).dst);
      add_ptr(std::get<ir::RotateRight>(instr).dst);
      break;
    case ir::Kind_instruction::Push:
      add(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::LoadAddress:
      if (isa<ir::Frame>(std::get<ir::LoadAddress>(instr).src)) {
        out.push_back(ir::Data{.name = "FP", .offset = 0});
        out.push_back(ir::Data{.name = "FP", .offset = 1});
      }
      break;
    default:
      break;
  }
  return out;
}

std::vector<ir::Type_operand> ptr_source_reads(const ir::Type_operand& op) {
  if (isa<ir::Indirect, ir::IndirectY>(op)) {
    return {ir::Data{.name = "DPTR", .offset = 0}, ir::Data{.name = "DPTR", .offset = 1}};
  }
  if (const auto* p = as<ir::IndirectZp>(op)) {
    return {ir::ZP{.address = p->address, .offset = 0},
            ir::ZP{.address = p->address + 1, .offset = 0}};
  }
  if (const auto* q = as<ir::IndirectZpY>(op)) {
    return {ir::ZP{.address = q->address, .offset = 0},
            ir::ZP{.address = q->address + 1, .offset = 0}};
  }
  return {};
}

}  // namespace c6502::passes::asm_dead_store

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// What the pass does with a dead candidate (`candidate_kind`).
enum class Candidate {
  kNone,
  // `Mov(Reg, <ZP or Data>)`, a pure store, which goes when it is dead.
  kPureStore,
  // `Mov(<not a Reg>, <ZP or Data>)`, emitted as `LDA src; STA dst`,
  // whose store half goes, keeping the load.
  kMemToMem,
  // An `INC`, `DEC`, or shift of a `__local_*` slot. It goes when it is
  // dead and the flags it sets are dead too.
  kDeadRmw,
};

// Returns the operand a read-modify-write instruction writes in place,
// or null for any other instruction.
const ir::Type_operand* rmw_dst(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::Inc:
      return &std::get<ir::Inc>(instr).dst;
    case K::Dec:
      return &std::get<ir::Dec>(instr).dst;
    case K::ArithmeticShiftLeft:
      return &std::get<ir::ArithmeticShiftLeft>(instr).dst;
    case K::LogicalShiftRight:
      return &std::get<ir::LogicalShiftRight>(instr).dst;
    case K::RotateLeft:
      return &std::get<ir::RotateLeft>(instr).dst;
    case K::RotateRight:
      return &std::get<ir::RotateRight>(instr).dst;
    default:
      return nullptr;
  }
}

Candidate candidate_kind(const ir::Type_instruction& instr) {
  if (const ir::Type_operand* dst = rmw_dst(instr)) {
    const auto* d = as<ir::Data>(*dst);
    if (d != nullptr && d->name.starts_with("__local_")) {
      return Candidate::kDeadRmw;
    }
  }
  const auto* mov = as<ir::Mov>(instr);
  if (mov == nullptr || mov->is_volatile || !isa<ir::ZP, ir::Data>(mov->dst)) {
    return Candidate::kNone;
  }
  return isa<ir::Reg>(mov->src) ? Candidate::kPureStore : Candidate::kMemToMem;
}

void rewrite_function(ir::Function& fn, const AsmContext& ctx,
                      const SymbolAddrs* zp_slot_symbols) {
  const Instrs instrs = fn.instructions;
  if (std::none_of(instrs.begin(), instrs.end(), [](const auto& i) {
        return candidate_kind(i) != Candidate::kNone;
      })) {
    return;
  }
  const LabelMap labels(instrs);
  const NameSet address_taken_names = asm_dead_store::collect_address_taken_names(instrs);
  const std::string local_prefix = "__local_" + fn.name + "__";
  std::vector<ir::Type_instruction> out;
  out.reserve(instrs.size());
  bool changed = false;
  for (std::size_t i = 0; i < instrs.size(); ++i) {
    const ir::Type_instruction& instr = instrs[i];
    const Candidate kind = candidate_kind(instr);
    if (kind == Candidate::kNone) {
      out.push_back(instr);
      continue;
    }
    const ir::Type_operand& target =
        kind == Candidate::kDeadRmw ? *rmw_dst(instr) : std::get<ir::Mov>(instr).dst;
    if (!asm_dead_store::is_dead_cfg(instrs, i, labels, ctx, zp_slot_symbols,
                                     local_prefix, address_taken_names, target)) {
      out.push_back(instr);
      continue;
    }
    switch (kind) {
      case Candidate::kPureStore:
        changed = true;
        break;
      case Candidate::kDeadRmw: {
        const bool flags_dead = isa<ir::Inc, ir::Dec>(instr)
                                    ? flags_dead_at(instrs, labels, i + 1)
                                    : all_flags_dead_at(instrs, labels, i + 1);
        if (flags_dead) {
          changed = true;
        } else {
          out.push_back(instr);
        }
        break;
      }
      case Candidate::kMemToMem:
        out.push_back(ir::Mov{.src = std::get<ir::Mov>(instr).src,
                              .dst = ir::Reg{.reg = ir::A{}}});
        changed = true;
        break;
      case Candidate::kNone:
        break;
    }
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_asm_dead_store(asm_ast::Program prog, const AsmContext& ctx,
                                      const SymbolAddrs* zp_slot_symbols) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn, ctx, zp_slot_symbols);
    }
  }
  return prog;
}

}  // namespace c6502::passes
