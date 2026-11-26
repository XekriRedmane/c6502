#include "passes/asm_remat.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_aliasing.h"
#include "passes/asm_liveness.h"
#include "passes/asm_operands.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

constexpr std::size_t kMaxDearerReads = 2;

using InstrVec = std::vector<ir::Type_instruction>;
using AddrSet = std::set<long long>;

bool is_reg_a(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  return r != nullptr && isa<ir::A>(r->reg);
}

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

std::vector<ir::Type_operand> read_operands(const ir::Type_instruction& instr) {
  std::vector<ir::Type_operand> out;
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith::reads(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Mov:
      out.push_back(std::get<ir::Mov>(instr).src);
      break;
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      out = {i.src, i.dst};
      break;
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      out = {i.src1, i.src2};
      break;
    }
    case ir::Kind_instruction::Push:
      out.push_back(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      out = {i.left, i.right};
      break;
    }
    case ir::Kind_instruction::BitTest:
      out.push_back(std::get<ir::BitTest>(instr).src);
      break;
    case ir::Kind_instruction::LoadAddress:
      out.push_back(std::get<ir::LoadAddress>(instr).src);
      break;
    default:
      if (const ir::Type_operand* dst = rmw_dst(instr)) {
        out.push_back(*dst);
      }
      break;
  }
  return out;
}

std::vector<ir::Type_operand> dsts_of(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return decimal_arith::writes(std::get<ir::DecimalArith>(instr));
    case ir::Kind_instruction::Mov:
      return {std::get<ir::Mov>(instr).dst};
    case ir::Kind_instruction::Add:
      return {std::get<ir::Add>(instr).dst};
    case ir::Kind_instruction::Sub:
      return {std::get<ir::Sub>(instr).dst};
    case ir::Kind_instruction::And:
      return {std::get<ir::And>(instr).dst};
    case ir::Kind_instruction::Or:
      return {std::get<ir::Or>(instr).dst};
    case ir::Kind_instruction::Xor:
      return {std::get<ir::Xor>(instr).dst};
    case ir::Kind_instruction::Pop:
      return {std::get<ir::Pop>(instr).dst};
    case ir::Kind_instruction::LoadAddress:
      return {std::get<ir::LoadAddress>(instr).dst};
    default:
      if (const ir::Type_operand* dst = rmw_dst(instr)) {
        return {*dst};
      }
      return {};
  }
}

bool is_block_boundary(const ir::Type_instruction& instr) {
  return isa<ir::Label, ir::Jump, ir::Branch, ir::Call, ir::Ret, ir::Return,
             ir::FunctionPrologue, ir::AllocateStack, ir::LoadAddress,
             ir::DecimalArith>(instr);
}

bool is_recomputable_shape(const ir::Type_operand& src) {
  return isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh, ir::Data, ir::IndexedData>(src);
}

bool matches_local(const ir::Type_operand& op, const std::string& name) {
  const auto* d = as<ir::Data>(op);
  return d != nullptr && d->name == name && d->offset == 0;
}

bool writes_local(const ir::Type_instruction& instr, const std::string& name) {
  for (const auto& dst : dsts_of(instr)) {
    if (matches_local(dst, name)) {
      return true;
    }
  }
  return false;
}

// Returns whether `instr`, which isn't a `Mov`, writes A. The caller
// decides a `Mov` itself.
bool non_mov_writes_a(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      return true;
    case K::Add:
      return is_reg_a(std::get<ir::Add>(instr).dst);
    case K::Sub:
      return is_reg_a(std::get<ir::Sub>(instr).dst);
    case K::And:
      return is_reg_a(std::get<ir::And>(instr).dst);
    case K::Or:
      return is_reg_a(std::get<ir::Or>(instr).dst);
    case K::Xor:
      return is_reg_a(std::get<ir::Xor>(instr).dst);
    case K::Pop:
      return is_reg_a(std::get<ir::Pop>(instr).dst);
    default: {
      const ir::Type_operand* dst = rmw_dst(instr);
      return dst != nullptr && !isa<ir::Inc, ir::Dec>(instr) && is_reg_a(*dst);
    }
  }
}

bool writes_reg(const ir::Type_instruction& instr, const ir::Type_reg& reg) {
  if (isa<ir::Y>(reg) && sets_up_y(instr)) {
    return true;
  }
  const ir::Type_operand* dst = nullptr;
  if (const auto* mov = as<ir::Mov>(instr)) {
    dst = &mov->dst;
  } else if (const auto* inc = as<ir::Inc>(instr)) {
    dst = &inc->dst;
  } else if (const auto* dec = as<ir::Dec>(instr)) {
    dst = &dec->dst;
  } else if (const auto* pop = as<ir::Pop>(instr)) {
    dst = &pop->dst;
  }
  if (dst == nullptr) {
    return false;
  }
  const auto* r = as<ir::Reg>(*dst);
  return r != nullptr && r->reg.index() == reg.index();
}

long find_a_producer(const InstrVec& instrs, std::size_t i) {
  for (long k = static_cast<long>(i) - 1; k >= 0; --k) {
    const ir::Type_instruction& instr = instrs[static_cast<std::size_t>(k)];
    if (is_block_boundary(instr)) {
      return -1;
    }
    if (const auto* mov = as<ir::Mov>(instr)) {
      if (is_reg_a(mov->dst)) {
        return k;
      }
      const auto* src_reg = as<ir::Reg>(mov->src);
      if (src_reg != nullptr || isa<ir::Reg>(mov->dst)) {
        // The 6502 has no `STX (zp),Y`, so a store of X or Y to the
        // soft stack or through a pointer is a `TXA` or `TYA` and an
        // `STA`, which writes A.
        if (src_reg != nullptr && !isa<ir::A>(src_reg->reg) &&
            !isa<ir::Data, ir::ZP>(mov->dst)) {
          return -1;
        }
        continue;
      }
      if (mov->src == mov->dst) {
        continue;
      }
      // The 6502 has no memory-to-memory move, so `Mov(#k, M)` and
      // `Mov(M, N)` emit as `LDA src; STA dst`, and A ends up holding
      // `src`.
      return k;
    }
    if (non_mov_writes_a(instr)) {
      return -1;
    }
  }
  return -1;
}

std::optional<std::pair<ir::Type_operand, std::size_t>> classify_stage_def(
    const InstrVec& instrs, std::size_t i) {
  const auto* defn = as<ir::Mov>(instrs[i]);
  if (defn == nullptr || defn->is_volatile) {
    return std::nullopt;
  }
  const auto* dst = as<ir::Data>(defn->dst);
  if (dst == nullptr || !dst->name.starts_with("__local_") || dst->offset != 0) {
    return std::nullopt;
  }
  if (is_recomputable_shape(defn->src)) {
    return std::make_pair(defn->src, i);
  }
  if (is_reg_a(defn->src)) {
    const long prod_idx = find_a_producer(instrs, i);
    if (prod_idx < 0) {
      return std::nullopt;
    }
    const auto* producer = as<ir::Mov>(instrs[static_cast<std::size_t>(prod_idx)]);
    if (producer == nullptr || producer->is_volatile ||
        !is_recomputable_shape(producer->src)) {
      return std::nullopt;
    }
    return std::make_pair(producer->src, static_cast<std::size_t>(prod_idx));
  }
  return std::nullopt;
}

const std::string& operand_name(const ir::Type_operand& op) {
  if (const auto* d = as<ir::Data>(op)) {
    return d->name;
  }
  return std::get<ir::IndexedData>(op).name;
}

class Pass {
 public:
  Pass(const ir::Program& prog, const AsmContext& ctx, const SymbolAddrs* zp_slot_symbols)
      : ctx_(ctx),
        slots_(zp_slot_symbols != nullptr ? *zp_slot_symbols : SymbolAddrs{}),
        runtime_(runtime_zp_addrs(ctx)),
        private_(collect_private_statics(prog)) {}

  void rewrite_function(ir::Function& fn) const;

 private:
  static NameSet collect_private_statics(const ir::Program& prog);

  void rewrite_uses(InstrVec& out, const NameSet& writable) const;
  std::vector<std::size_t> eligible_uses(const InstrVec& out, std::size_t i,
                                         const ir::Type_operand& recomp,
                                         std::size_t range_start,
                                         const std::string& local_name,
                                         const NameSet& writable) const;
  bool can_remat(const ir::Type_operand& recomp, const InstrVec& instrs,
                 std::size_t def_idx, std::size_t use_idx,
                 const NameSet& writable) const;
  bool store_may_reach(const ir::Type_operand& dst,
                       const ir::Type_operand& recomp) const;
  bool is_zero_page_name(const std::string& name) const;
  bool costs_more(const ir::Type_operand& recomp, const std::string& local_name) const;
  bool store_dies(const InstrVec& instrs, const std::vector<std::size_t>& uses,
                  const std::string& local_name) const;
  AddrSet collect_referenced_addrs(const InstrVec& instrs,
                                   const std::vector<bool>* skip) const;
  InstrVec drop_dead_stage_dsts(InstrVec instrs) const;

  const AsmContext& ctx_;
  SymbolAddrs slots_;
  SymbolAddrs runtime_;
  NameSet private_;
};

NameSet Pass::collect_private_statics(const ir::Program& prog) {
  NameSet statics;
  NameSet taken;
  for (const auto& tl : prog.top_level) {
    if (const auto* sv = as<ir::StaticVariable>(tl)) {
      if (!sv->is_global) {
        statics.insert(sv->name);
      }
      for (const auto& init : sv->init) {
        if (const auto* a = as<ir::AddressInit>(init)) {
          taken.insert(a->name);
        } else if (const auto* b = as<ir::AddressByteInit>(init)) {
          taken.insert(b->name);
        }
      }
    } else if (const auto* fn = as<ir::Function>(tl)) {
      for (const auto& instr : fn->instructions) {
        if (const auto* la = as<ir::LoadAddress>(instr)) {
          if (const auto* d = as<ir::Data>(la->src)) {
            taken.insert(d->name);
          }
        }
        const auto visit = [&](const ir::Type_operand& op) {
          if (const auto* lo = as<ir::ImmLabelLow>(op)) {
            taken.insert(lo->name);
          } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
            taken.insert(hi->name);
          }
        };
        if (const auto* phi = as<ir::Phi>(instr)) {
          visit(phi->dst);
        } else {
          for_each_operand<true>(instr, visit);
        }
      }
    }
  }
  NameSet out;
  for (const auto& name : statics) {
    if (taken.find(name) == taken.end()) {
      out.insert(name);
    }
  }
  return out;
}

bool Pass::is_zero_page_name(const std::string& name) const {
  auto it = slots_.find(name);
  if (it != slots_.end()) {
    return it->second < 0x100;
  }
  it = runtime_.find(name);
  if (it != runtime_.end()) {
    return it->second < 0x100;
  }
  return ctx_.is_user_zp_global(name);
}

// Returns whether reading `recomp` takes a byte more than reading the
// slot `local_name`. An absolute `Data` byte and an `abs,X` / `abs,Y`
// element do (3 bytes and a cycle more) when the slot is in zero page
// (2 bytes). An immediate or a zero-page byte doesn't.
bool Pass::costs_more(const ir::Type_operand& recomp, const std::string& local_name) const {
  if (!isa<ir::Data, ir::IndexedData>(recomp)) {
    return false;
  }
  if (is_zero_page_name(operand_name(recomp))) {
    return false;
  }
  return is_zero_page_name(local_name);
}

void add_referenced_addrs(const ir::Type_operand& op, const SymbolAddrs& slots,
                          AddrSet& referenced) {
  if (isa<ir::Data, ir::IndexedData>(op)) {
    const auto it = slots.find(operand_name(op));
    if (it != slots.end()) {
      const asdl::Int offset = isa<ir::Data>(op) ? std::get<ir::Data>(op).offset
                                                 : std::get<ir::IndexedData>(op).offset;
      referenced.insert(static_cast<long long>(it->second + offset));
    }
  } else if (const auto* z = as<ir::ZP>(op)) {
    referenced.insert(static_cast<long long>(z->address + z->offset));
  } else if (const auto* p = as<ir::IndirectZp>(op)) {
    referenced.insert(static_cast<long long>(p->address));
    referenced.insert(static_cast<long long>(p->address + 1));
  } else if (const auto* q = as<ir::IndirectZpY>(op)) {
    referenced.insert(static_cast<long long>(q->address));
    referenced.insert(static_cast<long long>(q->address + 1));
  }
}

AddrSet Pass::collect_referenced_addrs(const InstrVec& instrs,
                                       const std::vector<bool>* skip) const {
  AddrSet out;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (skip != nullptr && (*skip)[k]) {
      continue;
    }
    const ir::Type_instruction& instr = instrs[k];
    for (const auto& src : read_operands(instr)) {
      add_referenced_addrs(src, slots_, out);
    }
    // `STA (ptr),Y` reads the pointer's two bytes to find the address
    // it writes.
    for (const auto& dst : dsts_of(instr)) {
      if (const auto* p = as<ir::IndirectZp>(dst)) {
        out.insert(static_cast<long long>(p->address));
        out.insert(static_cast<long long>(p->address + 1));
      } else if (const auto* q = as<ir::IndirectZpY>(dst)) {
        out.insert(static_cast<long long>(q->address));
        out.insert(static_cast<long long>(q->address + 1));
      } else if (isa<ir::Indirect, ir::IndirectY>(dst)) {
        const auto it = runtime_.find("DPTR");
        if (it != runtime_.end()) {
          out.insert(static_cast<long long>(it->second));
          out.insert(static_cast<long long>(it->second + 1));
        }
      }
    }
  }
  return out;
}

NameSet collect_escaped_names(const InstrVec& instrs, const std::vector<bool>* skip) {
  NameSet out;
  for (std::size_t k = 0; k < instrs.size(); ++k) {
    if (skip != nullptr && (*skip)[k]) {
      continue;
    }
    const ir::Type_instruction& instr = instrs[k];
    if (const auto* la = as<ir::LoadAddress>(instr)) {
      if (const auto* d = as<ir::Data>(la->src)) {
        out.insert(d->name);
      }
    }
    for (const auto& op : read_operands(instr)) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.insert(hi->name);
      }
    }
  }
  return out;
}

bool Pass::store_dies(const InstrVec& instrs, const std::vector<std::size_t>& uses,
                      const std::string& local_name) const {
  if (uses.size() > kMaxDearerReads) {
    return false;
  }
  const auto it = slots_.find(local_name);
  if (it == slots_.end()) {
    return false;
  }
  std::vector<bool> skip(instrs.size(), false);
  for (const std::size_t u : uses) {
    skip[u] = true;
  }
  if (collect_referenced_addrs(instrs, &skip).count(static_cast<long long>(it->second)) != 0) {
    return false;
  }
  const NameSet escaped = collect_escaped_names(instrs, &skip);
  return escaped.find(local_name) == escaped.end();
}

bool Pass::store_may_reach(const ir::Type_operand& dst,
                           const ir::Type_operand& recomp) const {
  const auto* idx = as<ir::IndexedData>(dst);
  if (isa<ir::Indirect, ir::IndirectY, ir::IndirectZp, ir::IndirectZpY>(dst) ||
      (idx != nullptr && idx->name.empty())) {
    const std::string& name = operand_name(recomp);
    return name.empty() || private_.find(name) == private_.end();
  }
  if (isa<ir::Data, ir::IndexedData>(dst)) {
    return false;
  }
  if (isa<ir::Reg, ir::Frame, ir::Stack>(dst)) {
    return false;
  }
  return may_alias(dst, recomp, ctx_);
}

bool Pass::can_remat(const ir::Type_operand& recomp, const InstrVec& instrs,
                     std::size_t def_idx, std::size_t use_idx,
                     const NameSet& writable) const {
  if (isa<ir::Imm, ir::ImmLabelLow, ir::ImmLabelHigh>(recomp)) {
    return true;
  }
  if (!isa<ir::Data, ir::IndexedData>(recomp)) {
    return false;
  }
  if (writable.find(operand_name(recomp)) != writable.end()) {
    return false;
  }
  for (std::size_t k = def_idx + 1; k < use_idx; ++k) {
    for (const auto& dst : dsts_of(instrs[k])) {
      if (store_may_reach(dst, recomp)) {
        return false;
      }
    }
  }
  for (std::size_t k = def_idx + 1; k < use_idx; ++k) {
    if (isa<ir::Call>(instrs[k])) {
      return false;
    }
  }
  const auto* indexed = as<ir::IndexedData>(recomp);
  if (indexed == nullptr) {
    return true;
  }
  if (!isa<ir::X, ir::Y>(indexed->index)) {
    return false;
  }
  for (std::size_t k = def_idx + 1; k < use_idx; ++k) {
    if (writes_reg(instrs[k], indexed->index)) {
      return false;
    }
  }
  return true;
}

std::vector<std::size_t> Pass::eligible_uses(const InstrVec& out, std::size_t i,
                                             const ir::Type_operand& recomp,
                                             std::size_t range_start,
                                             const std::string& local_name,
                                             const NameSet& writable) const {
  std::vector<std::size_t> uses;
  const auto* indexed = as<ir::IndexedData>(recomp);
  for (std::size_t j = i + 1; j < out.size(); ++j) {
    const ir::Type_instruction& cur = out[j];
    if (is_block_boundary(cur) || writes_local(cur, local_name)) {
      break;
    }
    const auto* mov = as<ir::Mov>(cur);
    if (mov == nullptr || mov->is_volatile || !matches_local(mov->src, local_name)) {
      continue;
    }
    // There's no `LDX abs,X` or `LDY abs,Y`, so an indexed read can't
    // load its own index register.
    if (indexed != nullptr) {
      const auto* r = as<ir::Reg>(mov->dst);
      if (r != nullptr && r->reg.index() == indexed->index.index()) {
        continue;
      }
    }
    if (!can_remat(recomp, out, range_start, j, writable)) {
      break;
    }
    uses.push_back(j);
  }
  return uses;
}

void Pass::rewrite_uses(InstrVec& out, const NameSet& writable) const {
  for (std::size_t i = 0; i < out.size(); ++i) {
    const auto stage = classify_stage_def(out, i);
    if (!stage) {
      continue;
    }
    const auto& [recomp, range_start] = *stage;
    const std::string local_name = std::get<ir::Data>(std::get<ir::Mov>(out[i]).dst).name;
    const std::vector<std::size_t> uses =
        eligible_uses(out, i, recomp, range_start, local_name, writable);
    if (uses.empty()) {
      continue;
    }
    if (costs_more(recomp, local_name) && !store_dies(out, uses, local_name)) {
      continue;
    }
    for (const std::size_t j : uses) {
      auto& cur = std::get<ir::Mov>(out[j]);
      out[j] = ir::Mov{.src = recomp, .dst = cur.dst, .is_volatile = cur.is_volatile};
    }
  }
}

InstrVec Pass::drop_dead_stage_dsts(InstrVec instrs) const {
  const AddrSet referenced = collect_referenced_addrs(instrs, nullptr);
  const NameSet escaped = collect_escaped_names(instrs, nullptr);
  InstrVec out;
  out.reserve(instrs.size());
  for (auto& instr : instrs) {
    if (const auto* mov = as<ir::Mov>(instr); mov != nullptr && !mov->is_volatile) {
      const auto* d = as<ir::Data>(mov->dst);
      if (d != nullptr && d->name.starts_with("__local_") && d->offset == 0) {
        const auto it = slots_.find(d->name);
        if (it != slots_.end() &&
            referenced.count(static_cast<long long>(it->second)) == 0 &&
            escaped.find(d->name) == escaped.end()) {
          if (isa<ir::Reg>(mov->src)) {
            continue;
          }
          out.push_back(ir::Mov{.src = mov->src, .dst = ir::Reg{.reg = ir::A{}},
                                .is_volatile = false});
          continue;
        }
      }
    }
    out.push_back(std::move(instr));
  }
  return out;
}

void Pass::rewrite_function(ir::Function& fn) const {
  NameSet writable;
  for (const auto& instr : fn.instructions) {
    for (const auto& dst : dsts_of(instr)) {
      if (isa<ir::Data, ir::IndexedData>(dst)) {
        writable.insert(operand_name(dst));
      }
    }
  }
  InstrVec out = fn.instructions;
  rewrite_uses(out, writable);
  fn.instructions = drop_dead_stage_dsts(std::move(out));
}

}  // namespace

asm_ast::Program apply_remat(asm_ast::Program prog, const AsmContext& ctx,
                             const SymbolAddrs* zp_slot_symbols) {
  const Pass pass(prog, ctx, zp_slot_symbols);
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      pass.rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
