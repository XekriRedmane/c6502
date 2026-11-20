// The walk of asm-level dead-store elimination, which other passes
// share through `mem_dead_after`.
//
// The Python pass drops any `Mov(Reg, M)` (an STA, STX, or STY) into a
// memory operand `M` whose value no instruction reachable from the
// store reads. The walk goes forward from the store through the
// function's control flow, and the store is dead when every path
// either writes `M` again before reading it, or reaches the function's
// exit with `M` dead there.
//
// Dead at the exit
//
// At a `Ret` / `Return`, a byte is dead when its address is in the
// register allocator's pool ($80 to $FF by default). Bytes there are
// the function's own. The caller-saved slots $80 to $BF are clobbered
// across calls anyway, and the late epilogue restores the callee-saved
// slots $C0 to $FF from the frame, so their values at the body's
// `Return` are never read. The same holds for DPTR, the runtime's caller-saved pointer
// scratch, and for each `__local_*` slot.
//
// The other zero-page bytes (the runtime's block, $00 to $25 in the
// classic layout, notably HARGS at $04 to $23, which holds return
// values wider than an `int`) and `Data` (globals that other functions
// may read) are live at the exit. The walk can still find such a store
// dead when every path loops back and writes it before reading it.
//
// Aliasing and opaque instructions
//
// The walk reads and writes through `may_alias`. A `Call`, a
// `FunctionPrologue`, and an `AllocateStack` may read any byte, so a
// path through one makes the byte live. The exceptions are a call that
// can't reach one of the function's own `__local_*` slots whose address
// it never takes, and a call that ends the life of a DPTR byte.
//
// `Push` / `Pop` aren't opaque. They touch only the hardware stack at
// $0100-$01FF and A, which no operand the walk follows can name, and
// counting them as reads would keep every store inside an indirect-Y
// store's `PHA src; ...; PLA` bracket, which is the shape that
// `apply_memory_value_propagation`'s cleanup of DPTR staging needs
// gone. `LoadAddress` isn't opaque either. It writes two bytes at its
// destination, and a `Frame` source reads FP and FP+1 (`CLC; LDA FP;
// ADC #off; STA dst.lo; LDA FP+1; ADC #0; STA dst.hi`), while a `Data`
// source reads no memory (`LDA #<name; STA dst.lo; LDA #>name; STA
// dst.hi`). Counting it opaque kept a dead `STA DPTR` whenever a
// `LoadAddress` came before the next write of DPTR.
#include "passes/asm_dead_store.h"

#include <unordered_set>

#include "passes/asm_aliasing.h"
#include "passes/decimal_arith.h"

namespace c6502::passes::asm_dead_store {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

// The register allocator's pool, $80 to $FF.
constexpr int kPoolLo = 0x80;
constexpr int kPoolHi = 0x100;

bool starts_with(std::string_view s, std::string_view prefix) {
  return s.substr(0, prefix.size()) == prefix;
}

// Returns each operand in the `src` and `dst` fields of `instr`, or
// every operand of a `DecimalArith`, for the scan of `ImmLabel*`
// operands. Python's `_all_operands`, which reads only those two field
// names.
std::vector<ir::Type_operand> all_operands(const ir::Type_instruction& instr) {
  if (const auto* d = as<ir::DecimalArith>(instr)) {
    return decimal_arith::operands(*d);
  }
  std::vector<ir::Type_operand> ops;
  std::visit(
      [&](const auto& ins) {
        if constexpr (requires { ins.src; }) {
          if constexpr (std::is_same_v<std::remove_cvref_t<decltype(ins.src)>,
                                       ir::Type_operand>) {
            ops.push_back(ins.src);
          }
        }
        if constexpr (requires { ins.dst; }) {
          if constexpr (std::is_same_v<std::remove_cvref_t<decltype(ins.dst)>,
                                       ir::Type_operand>) {
            ops.push_back(ins.dst);
          }
        }
      },
      instr);
  return ops;
}

bool operands_equal_exact(const ir::Type_operand& a, const ir::Type_operand& b) {
  if (const auto* za = as<ir::ZP>(a)) {
    const auto* zb = as<ir::ZP>(b);
    return zb != nullptr && za->address == zb->address && za->offset == zb->offset;
  }
  if (const auto* da = as<ir::Data>(a)) {
    const auto* db = as<ir::Data>(b);
    return db != nullptr && da->name == db->name && da->offset == db->offset;
  }
  return false;
}

// Returns whether the byte `target` written at `instrs[start]` is read
// on no path forward: each path writes the same byte before reading it,
// or reaches an exit where the byte is dead. A path that reads it, or
// that meets an opaque instruction, makes it live.
//
// A direct `Call` is passed over for a `target` the callee can't reach
// (`call_cannot_touch`): a byte of the function's own private pool
// whose address the function never takes. The allocator gives that
// pool bytes apart from every callee's, so no callee can name the byte.
// The `icall` trampoline is excluded, since its eventual target is
// unknown.
bool is_dead_cfg(Instrs instrs, std::size_t start, const LabelMap& labels,
                 const AsmContext& ctx, const SymbolAddrs* zp_slot_symbols,
                 std::string_view local_prefix,
                 const NameSet& address_taken_names,
                 const ir::Type_operand& target) {
  // A byte in this function's private pool whose address is never
  // taken can't be reached through a pointer, so an indirect access
  // reads only its pointer's bytes as far as `target` goes.
  const bool is_private =
      call_cannot_touch(target, local_prefix, address_taken_names);
  const auto* target_data = as<ir::Data>(target);
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
    // DPTR is caller-saved scratch. No callee reads its incoming value,
    // and no pass assumes it survives a call, so a direct call ends a
    // stored DPTR byte's life. The exception is the `icall` trampoline,
    // the JSR target of an indirect call, whose `JMP (DPTR)` reads both
    // bytes. A pointer staged in DPTR before a call, as in a loop that
    // prints through `(ptr),Y` and calls a routine per character, is
    // dead.
    if (call != nullptr && call->name != "icall" && target_data != nullptr &&
        target_data->name == "DPTR") {
      continue;
    }
    if (is_opaque(next)) {
      return false;
    }
    // A tail call, a `Jump` to a label outside the function, leaves for
    // a callee that may read `target`, notably a `__zpabi_<callee>__*`
    // slot just written as an argument. The allocator gives
    // `__local_<fn>__*` slots addresses apart from every callee's, so
    // such a slot dies at the tail call, while the other bytes of the
    // pool don't.
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
    // A write of exactly the same byte ends the path. A write that only
    // may alias doesn't, since it may write another byte.
    if (writes_same(next, target)) {
      continue;
    }
    for (const std::size_t s : successors(instrs, j, labels)) {
      stack.push_back(s);
    }
  }
  return true;
}

}  // namespace

bool is_opaque(const ir::Type_instruction& instr) {
  return isa<ir::Call, ir::FunctionPrologue, ir::AllocateStack>(instr);
}

NameSet collect_address_taken_names(Instrs instrs) {
  NameSet out;
  for (const auto& instr : instrs) {
    // `LoadAddress(src = Data(name), ...)` takes the address of `name`
    // in one atom, before its bytes are split into the
    // `Mov(ImmLabelLow(name), ...)` / `Mov(ImmLabelHigh(name), ...)`
    // pair that the scan below catches.
    if (const auto* la = as<ir::LoadAddress>(instr)) {
      if (const auto* d = as<ir::Data>(la->src)) {
        out.insert(d->name);
      }
    }
    for (const auto& op : all_operands(instr)) {
      if (const auto* lo = as<ir::ImmLabelLow>(op)) {
        out.insert(lo->name);
      } else if (const auto* hi = as<ir::ImmLabelHigh>(op)) {
        out.insert(hi->name);
      }
    }
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
  if (const auto* d = as<ir::Data>(target)) {
    if (d->name == "DPTR") {
      return true;
    }
    // A body-local slot belongs to the function whose code names it,
    // and dies when that function returns, whether the slot is in zero
    // page or spilled to memory above it.
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
  // The callee may read its own `__zpabi_<callee>__*` argument slots,
  // which are in the pool, so the pool's bytes aren't dead here as they
  // are at a return. A local whose address the function takes may have
  // been given to the callee, as `sum((int[]){1, 2, 3, 4}, 4)` gives
  // `sum` a compound literal's.
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
      // An access through a pointer reads the pointer's bytes, which
      // `may_alias` doesn't count: it compares the bytes the access
      // reaches, and a pointer can't reach the zero-page pool its own
      // bytes may be in.
      for (const auto& p : ptr_source_reads(op)) {
        if (may_alias(p, target, ctx, options)) {
          return true;
        }
      }
      if (is_private) {
        // Only the pointer's bytes can be `target`.
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
    // `reads` is asked first, so a chain that reads `target` before
    // writing it has already counted as a read.
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
  // Any `Mov` with a destination that isn't a register writes that
  // byte, including a move from memory to memory, which `asm_emit`
  // writes as `LDA src; STA dst`. A `Pop` into memory is the defensive
  // case, since the IR's pops go into A. A `LoadAddress` writes two
  // bytes, and the low one is given, so a store to that byte counts as
  // overwritten. The walk goes past the high byte, which is sound.
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
      // A store through an indirect-Y mode reads the pointer (DPTR, FP,
      // SSP, or a zero-page base) to find the byte, so the walk doesn't
      // drop a pointer still being staged.
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
    // INC, DEC, and the shifts read their destination.
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
      // The write to the hardware stack isn't a byte the walk follows.
      add(std::get<ir::Push>(instr).src);
      break;
    case ir::Kind_instruction::LoadAddress:
      // A `Data` source is an immediate address. A `Frame` source adds
      // FP and FP+1 to the offset.
      if (isa<ir::Frame>(std::get<ir::LoadAddress>(instr).src)) {
        out.push_back(ir::Data{.name = "FP", .offset = 0});
        out.push_back(ir::Data{.name = "FP", .offset = 1});
      }
      break;
    default:
      // A `Pop` reads nothing the walk follows, and its write is in
      // `write_operand`.
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
