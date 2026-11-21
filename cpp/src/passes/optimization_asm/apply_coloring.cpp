// Applies a `Coloring` to an asm SSA function. Each `Pseudo(name,
// offset)` whose name is in `coloring.assignments` becomes the
// `ZP(address, offset)` it was given, and each name in
// `coloring.hwreg_assignments` becomes `Reg(X)` or `Reg(Y)`.
//
// The optimizer runs this between `byte_dce` and `from_ssa`. Once the
// colored values are lowered to zero page or to a register, the
// ordering of the parallel copies in `from_ssa` can find the cycles
// between moves at the level of the physical slots, which is where the
// hazard lies, where by SSA name alone it would miss some. Take
//
//     Phi(X, [(P, Y)])  ; X := Y
//     Phi(Y, [(P, X)])  ; Y := X
//
// where X and Y get the different colors $A and $B. The two form a
// 2-cycle on the edge from P, since `Mov $B -> $A` overwrites $A before
// `Mov $A -> $B` reads it. With the coloring applied first, the moves
// are `Mov(ZP($B), ZP($A))` and `Mov(ZP($A), ZP($B))`, and the cycle
// check sees `ZP($A)` twice.
//
// Substitution of X and Y
//
// When a pseudo is pinned to X or Y, every reference to it becomes
// `Reg(X)` or `Reg(Y)`. The substitution alone gives correct code that
// is longer than necessary. The setup of the index of an
// `IndexedData`,
//
//     Mov(P, Reg(A)); Mov(Reg(A), Reg(X)); ... IndexedData(..., X)
//
// with P pinned to Y becomes
//
//     Mov(Reg(Y), Reg(A)); Mov(Reg(A), Reg(X)); ... IndexedData(..., X)
//
// which is `TYA; TAX` before each `LDA name,X`. Both transfers are
// redundant, since Y already holds the value X is given and the access
// could be indexed by Y. So a peephole after the substitution finds the
// chain of transfers, drops it, and indexes the `IndexedData` operands
// that follow by Y in place of X, or by X in place of Y in the
// symmetric case ("Redundant transfers" below).
//
// Pseudos in neither part of the coloring (parameters, names whose
// address is taken, statics, spilled values) are left alone, and
// `replace_pseudoregisters_bare_exit` lowers them later.
#include "passes/optimization_asm/apply_coloring.h"

#include <optional>
#include <string>
#include <utility>

#include "errors.h"
#include "passes/decimal_arith.h"
#include "passes/local_slot_names.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;
using Instrs = std::vector<ir::Type_instruction>;
using AddrNames = OrderedMap<asdl::Int, std::string>;

ir::Type_operand hwreg_letter_to_op(const std::string& letter) {
  if (letter == "X") {
    return ir::Reg{.reg = ir::X{}};
  }
  if (letter == "Y") {
    return ir::Reg{.reg = ir::Y{}};
  }
  throw InternalError("unknown HwReg letter: " + pyrepr::string(letter));
}

ir::Type_operand apply_to_op(const ir::Type_operand& op, const Coloring& coloring,
                             const AddrNames& addr_to_name) {
  const auto* p = as<ir::Pseudo>(op);
  if (p == nullptr) {
    return op;
  }
  if (const std::string* letter = coloring.hwreg_assignments.find(p->name)) {
    // A pin to X or Y holds one byte, so the offset must be 0. The
    // eligibility scan and the allocator together make sure of that,
    // and an offset that gets through is a bug of the allocator, which
    // masking it with `& 0` would hide.
    if (p->offset != 0) {
      throw InternalError("HwReg-colored Pseudo " + p->name + " has nonzero offset " +
                          pyrepr::integer(p->offset) +
                          "; eligibility scan should have rejected it");
    }
    return hwreg_letter_to_op(*letter);
  }
  if (const asdl::Int* base = coloring.assignments.find(p->name)) {
    const asdl::Int addr = *base + p->offset;
    if (const std::string* name = addr_to_name.find(addr)) {
      return ir::Data{.name = *name, .offset = 0};
    }
    return ir::ZP{.address = addr, .offset = 0};
  }
  return op;
}

ir::Type_instruction apply_to_instruction(const ir::Type_instruction& instr,
                                          const Coloring& coloring,
                                          const AddrNames& addr_to_name) {
  using K = ir::Kind_instruction;
  auto apply = [&](const ir::Type_operand& op) { return apply_to_op(op, coloring, addr_to_name); };
  switch (ir::kind(instr)) {
    case K::DecimalArith:
      return decimal_arith::map_operands(std::get<ir::DecimalArith>(instr), apply);
    case K::Mov: {
      const auto& m = std::get<ir::Mov>(instr);
      return ir::Mov{.src = apply(m.src), .dst = apply(m.dst), .is_volatile = m.is_volatile};
    }
    case K::Add: {
      const auto& m = std::get<ir::Add>(instr);
      return ir::Add{.src = apply(m.src), .dst = apply(m.dst)};
    }
    case K::Sub: {
      const auto& m = std::get<ir::Sub>(instr);
      return ir::Sub{.src = apply(m.src), .dst = apply(m.dst)};
    }
    case K::And: {
      const auto& m = std::get<ir::And>(instr);
      return ir::And{.src = apply(m.src), .dst = apply(m.dst)};
    }
    case K::Or: {
      const auto& m = std::get<ir::Or>(instr);
      return ir::Or{.src = apply(m.src), .dst = apply(m.dst)};
    }
    case K::Xor: {
      const auto& m = std::get<ir::Xor>(instr);
      return ir::Xor{.src1 = apply(m.src1), .src2 = apply(m.src2), .dst = apply(m.dst)};
    }
    case K::Inc:
      return ir::Inc{.dst = apply(std::get<ir::Inc>(instr).dst)};
    case K::Dec:
      return ir::Dec{.dst = apply(std::get<ir::Dec>(instr).dst)};
    case K::ArithmeticShiftLeft:
      return ir::ArithmeticShiftLeft{.dst = apply(std::get<ir::ArithmeticShiftLeft>(instr).dst)};
    case K::LogicalShiftRight:
      return ir::LogicalShiftRight{.dst = apply(std::get<ir::LogicalShiftRight>(instr).dst)};
    case K::RotateLeft:
      return ir::RotateLeft{.dst = apply(std::get<ir::RotateLeft>(instr).dst)};
    case K::RotateRight:
      return ir::RotateRight{.dst = apply(std::get<ir::RotateRight>(instr).dst)};
    case K::Push:
      return ir::Push{.src = apply(std::get<ir::Push>(instr).src)};
    case K::Pop:
      return ir::Pop{.dst = apply(std::get<ir::Pop>(instr).dst)};
    case K::Compare: {
      const auto& c = std::get<ir::Compare>(instr);
      return ir::Compare{.left = apply(c.left), .right = apply(c.right)};
    }
    case K::LoadAddress: {
      // The source's address is taken, so it is never colored and
      // `apply` leaves it alone. The destination is mapped for
      // completeness.
      const auto& l = std::get<ir::LoadAddress>(instr);
      return ir::LoadAddress{.src = apply(l.src), .dst = apply(l.dst)};
    }
    case K::Phi: {
      const auto& phi = std::get<ir::Phi>(instr);
      ir::Phi out{.dst = apply(phi.dst)};
      out.args.reserve(phi.args.size());
      for (const auto& a : phi.args) {
        const auto& arg = std::get<ir::AsmPhiArg>(a);
        out.args.push_back(ir::AsmPhiArg{.pred_label = arg.pred_label, .source = apply(arg.source)});
      }
      return out;
    }
    default:
      return instr;
  }
}

// --- Redundant transfers ---

// Returns 'X' for X and 'Y' for Y and A, as Python's test of
// `isinstance(op.index, X)` gives for an index register.
char index_letter(const ir::Type_reg& r) { return isa<ir::X>(r) ? 'X' : 'Y'; }

// Returns 'A', 'X', or 'Y' for a `Reg`, and 0 for any other operand.
char reg_letter(const ir::Type_operand& op) {
  const auto* r = as<ir::Reg>(op);
  if (r == nullptr) {
    return 0;
  }
  if (isa<ir::A>(r->reg)) {
    return 'A';
  }
  return isa<ir::X>(r->reg) ? 'X' : 'Y';
}

// Returns whether `instr` is a `Mov` whose `IndexedData` operand is
// indexed by the register its other operand is. The 6502 has no `LDX
// abs,X`, `LDY abs,Y`, `STX abs,X`, or `STY abs,Y`, and both the emitter
// and the simulator's assembler reject them, so the rewrite across
// registers uses this to find an index rewrite from X to Y (or Y to X)
// that would land on one.
bool is_unencodable_indexed_mov(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr) {
    return false;
  }
  const auto* src_ix = as<ir::IndexedData>(m->src);
  const auto* dst_reg = as<ir::Reg>(m->dst);
  if (src_ix != nullptr && dst_reg != nullptr && src_ix->index.index() == dst_reg->reg.index()) {
    return true;
  }
  const auto* dst_ix = as<ir::IndexedData>(m->dst);
  const auto* src_reg = as<ir::Reg>(m->src);
  return dst_ix != nullptr && src_reg != nullptr && dst_ix->index.index() == src_reg->reg.index();
}

// Splits the load `Mov(IndexedData(..., R), Reg(R))` into
// `Mov(IndexedData(..., R), Reg(A))` and `Mov(Reg(A), Reg(R))`, which is
// `LDA abs,R` and then `TAR`. The single `LDR abs,R` doesn't exist, and
// the pair loads the same value in one byte more. Returns nothing for a
// store, which no such split can do, and for any other instruction.
std::optional<Instrs> split_unencodable_indexed_load(const ir::Type_instruction& instr) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr) {
    return std::nullopt;
  }
  const auto* src = as<ir::IndexedData>(m->src);
  const auto* dst = as<ir::Reg>(m->dst);
  if (src == nullptr || dst == nullptr || src->index.index() != dst->reg.index()) {
    return std::nullopt;
  }
  const ir::Type_operand reg_a = ir::Reg{.reg = ir::A{}};
  // The half that reads memory keeps the volatile bit of the original
  // move. The transfer has no effect on memory, so it is never volatile.
  return Instrs{ir::Mov{.src = m->src, .dst = reg_a, .is_volatile = m->is_volatile},
                ir::Mov{.src = reg_a, .dst = m->dst}};
}

// The match of a chain of two transfers, the letters of its source and
// destination registers.
struct TransferChain {
  char src;
  char dst;
};

// Matches the chain `Mov(Reg(R'), A); Mov(A, Reg(R))` at
// `instrs[start]`, where R and R' are X or Y. R == R' is a transfer
// through A and back, which does nothing but is still matched, and the
// caller handles it.
std::optional<TransferChain> match_transfer_chain(const Instrs& instrs, std::size_t start) {
  if (start + 2 > instrs.size()) {
    return std::nullopt;
  }
  const auto* i0 = as<ir::Mov>(instrs[start]);
  const auto* i1 = as<ir::Mov>(instrs[start + 1]);
  if (i0 == nullptr || i1 == nullptr) {
    return std::nullopt;
  }
  const char s0 = reg_letter(i0->src);
  const char d0 = reg_letter(i0->dst);
  const char s1 = reg_letter(i1->src);
  const char d1 = reg_letter(i1->dst);
  if ((s0 != 'X' && s0 != 'Y') || d0 != 'A') {
    return std::nullopt;
  }
  if (s1 != 'A' || (d1 != 'X' && d1 != 'Y')) {
    return std::nullopt;
  }
  return TransferChain{.src = s0, .dst = d1};
}

// Returns `op` indexed by `to` when it is an `IndexedData` indexed by
// `from`, or nothing when it is left as it is.
std::optional<ir::Type_operand> rewrite_index_in_op(const ir::Type_operand& op, char from, char to) {
  const auto* ix = as<ir::IndexedData>(op);
  if (ix == nullptr || index_letter(ix->index) != from) {
    return std::nullopt;
  }
  return ir::IndexedData{.name = ix->name,
                         .offset = ix->offset,
                         .index = to == 'X' ? ir::Type_reg{ir::X{}} : ir::Type_reg{ir::Y{}}};
}

// Returns a copy of `instr` with the index of its `IndexedData` operands
// changed from `from` to `to` when it is a `Mov` with such an operand,
// or nothing when it is left as it is. Python returns the instruction
// itself for that, and its caller tests the identity.
std::optional<ir::Type_instruction> rewrite_index_in_instr(const ir::Type_instruction& instr,
                                                           char from, char to) {
  const auto* m = as<ir::Mov>(instr);
  if (m == nullptr) {
    return std::nullopt;
  }
  auto src = rewrite_index_in_op(m->src, from, to);
  auto dst = rewrite_index_in_op(m->dst, from, to);
  if (!src && !dst) {
    return std::nullopt;
  }
  return ir::Mov{.src = src ? std::move(*src) : m->src,
                 .dst = dst ? std::move(*dst) : m->dst,
                 .is_volatile = m->is_volatile};
}

// Returns the operand `instr` names as its destination, or null. Many
// instructions write A, and this gives only the `dst` field. A
// `Compare`, a `Push`, and the carry instructions have none.
const ir::Type_operand* instr_destination(const ir::Type_instruction& instr) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::Mov:
      return &std::get<ir::Mov>(instr).dst;
    case K::Add:
      return &std::get<ir::Add>(instr).dst;
    case K::Sub:
      return &std::get<ir::Sub>(instr).dst;
    case K::And:
      return &std::get<ir::And>(instr).dst;
    case K::Or:
      return &std::get<ir::Or>(instr).dst;
    case K::Xor:
      return &std::get<ir::Xor>(instr).dst;
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
    case K::Pop:
      return &std::get<ir::Pop>(instr).dst;
    case K::LoadAddress:
      return &std::get<ir::LoadAddress>(instr).dst;
    default:
      return nullptr;
  }
}

// Returns whether `instr` ends the run that may be rewritten after the
// chain of transfers. The run goes on while `instr` writes neither
// `Reg(dst_reg)`, which would define the register being rewritten
// again, nor `Reg(src_reg)`, which would change the value, and isn't a
// call, a label, a branch, a jump, a return, a prologue, an allocation
// of the stack, or a `DecimalArith`.
//
// Python's comment adds writes of A to the list, and its code doesn't
// stop at them, which this follows. A write of A is common, since every
// read of an `IndexedData` goes through A, and the rewrite of an index
// stays sound through the instruction that writes A, which reads the
// index register before anything has changed it.
bool instr_breaks_transfer_chain(const ir::Type_instruction& instr, char dst_reg, char src_reg) {
  using K = ir::Kind_instruction;
  switch (ir::kind(instr)) {
    case K::Call:
    case K::Label:
    case K::Jump:
    case K::Branch:
    case K::Return:
    case K::Ret:
    case K::FunctionPrologue:
    case K::AllocateStack:
    case K::DecimalArith:
      return true;
    default:
      break;
  }
  const ir::Type_operand* dst = instr_destination(instr);
  if (dst != nullptr && isa<ir::Reg>(*dst)) {
    const char letter = reg_letter(*dst);
    return letter == dst_reg || letter == src_reg;
  }
  return false;
}

// Removes the chains `Mov(Reg(R'), Reg(A)); Mov(Reg(A), Reg(R))` that
// the pinning of a pseudo to R' leaves in the setup of an
// `IndexedData`'s index.
//
// A chain with R == R' moves R' through A and back, which leaves R'
// as it was and only overwrites A. Every place that makes this shape is
// the setup of an index, where the next instruction loads A again, so
// both moves are dropped.
//
// A chain with R != R' moves R' to R, and the value wanted in R is
// already in R'. When `IndexedData` accesses indexed by R follow it
// with no write of R, R', or A and no change of control flow between,
// they are indexed by R' instead and the chain is dropped. When no
// access is rewritten, the chain sets R up for some other use and
// stays, since dropping it would leave R undefined there.
Instrs rewrite_redundant_transfers(const Instrs& instrs) {
  Instrs out;
  std::size_t i = 0;
  const std::size_t n = instrs.size();
  while (i < n) {
    const auto chain = match_transfer_chain(instrs, i);
    if (!chain) {
      out.push_back(instrs[i]);
      ++i;
      continue;
    }
    if (chain->src == chain->dst) {
      i += 2;
      continue;
    }
    // Rewrite the `IndexedData` operands from the end of the chain to
    // the end of the run.
    //
    // A rewrite that would make a load the 6502 can't encode
    // (`Mov(IndexedData(..., X), Reg(Y))`, which is `LDY abs,X`, becomes
    // `LDY abs,Y`) is split into `LDA abs,Y; TAY`. That is one byte
    // longer than the `LDY abs,X`, but dropping the transfer chain of
    // two bytes still saves one, and X stays free for later passes such
    // as `loop_counter_to_x`. A store can't be split that way, since
    // `STX abs,X` and `STY abs,Y` don't exist, so a `Mov` with the
    // `IndexedData` as its destination abandons the whole rewrite.
    std::size_t j = i + 2;
    Instrs rewritten_run;
    bool any_rewritten = false;
    bool aborted = false;
    while (j < n) {
      const ir::Type_instruction& instr = instrs[j];
      auto rewritten = rewrite_index_in_instr(instr, chain->dst, chain->src);
      if (rewritten) {
        any_rewritten = true;
        if (auto split = split_unencodable_indexed_load(*rewritten)) {
          for (auto& s : *split) {
            rewritten_run.push_back(std::move(s));
          }
          ++j;
          // The split ends in `Mov(Reg(A), Reg(dst))`, which writes the
          // destination register and so ends the run.
          break;
        }
        if (is_unencodable_indexed_mov(*rewritten)) {
          aborted = true;
          break;
        }
      }
      const ir::Type_instruction& now = rewritten ? *rewritten : instr;
      const bool should_stop = instr_breaks_transfer_chain(now, chain->dst, chain->src);
      rewritten_run.push_back(now);
      ++j;
      if (should_stop) {
        break;
      }
    }
    if (any_rewritten && !aborted) {
      for (auto& r : rewritten_run) {
        out.push_back(std::move(r));
      }
      i = j;
    } else {
      out.push_back(instrs[i]);
      ++i;
    }
  }
  return out;
}

}  // namespace

asm_ast::Function apply_coloring(asm_ast::Function fn, const Coloring& coloring,
                                 const std::vector<asdl::Int>* local_pool) {
  if (coloring.assignments.empty() && coloring.hwreg_assignments.empty()) {
    return fn;
  }
  AddrNames addr_to_name;
  if (local_pool != nullptr && !local_pool->empty()) {
    addr_to_name = local_slot_names(fn.name, coloring, *local_pool);
  }
  // Substitute the pseudo references.
  Instrs instrs;
  instrs.reserve(fn.instructions.size());
  for (const auto& instr : fn.instructions) {
    instrs.push_back(apply_to_instruction(instr, coloring, addr_to_name));
  }
  // Drop the chains of transfers that a pinned pseudo leaves in the
  // setup of an index, until nothing changes, since the rewritten run
  // of a chain across registers may hold a chain through one register
  // that the next sweep finds.
  if (!coloring.hwreg_assignments.empty()) {
    while (true) {
      const std::size_t prev_len = instrs.size();
      instrs = rewrite_redundant_transfers(instrs);
      if (instrs.size() == prev_len) {
        break;
      }
    }
  }
  fn.instructions = std::move(instrs);
  return fn;
}

}  // namespace c6502::passes
