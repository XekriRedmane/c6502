// Folds `--counter` into DEY or DEX where the counter is pinned to X or
// Y, volatile pins included.
//
// When a C local with `[[c6502::reg("X")]]` or `reg("Y")` gets
// `--counter` (or `counter -= 1; counter != 0`), `tac_to_asm` lowers
// the read-modify-write through A:
//
//     Mov(Reg(Y), Reg(A))       ; TYA (volatile if y is pinned volatile)
//     SetCarry
//     Sub(Imm(1), Reg(A))       ; A = Y - 1
//     Mov(Reg(A), <tmp>)        ; STA tmp
//     Mov(<tmp>, Reg(Y))        ; LDY tmp (writes y)
//     Branch(NE|EQ, ...)
//
// That is six instructions for a decrement of one register byte. The
// 6502's DEY (and DEX) does `Y = Y - 1` and sets N and Z from the
// result in 1 byte and 2 cycles. The fold is
//
//     Dec(Reg(Y))
//     Branch(NE|EQ, ...)
//
// Soundness for volatile
//
// `volatile uint8_t y [[c6502::reg("Y")]]` pins y to the Y register,
// so the register is the storage, and a volatile access of y is a read
// or write of Y, which any DEY performs. The original lowering makes
// one volatile read (the TYA) and one volatile write (the LDY that
// brings the decremented value back into Y). DEY does both in one
// instruction, one read-modify-write of Y, which the C abstract
// machine sees as the single read and single write that `--y` requires
// (C99 6.5.2.4).
//
// Soundness for A
//
// The original leaves A holding `y - 1`, and DEY leaves A as it was.
// The fold requires A dead at both successors of the branch, taken and
// fall-through. A short look ahead at the head of each successor is
// enough for the usual inner loop, where every iteration starts by
// overwriting A with a TYA, a TXA, or another move into A. When A
// can't be shown dead, the fold doesn't happen.
//
// Soundness for the spill temporary
//
// The STA and LDY pair routes the new value through a memory cell.
// Dropping both halves drops the cell's write, so any later reader
// would see whatever was there before. The gate is the simplest
// sufficient one: the cell isn't read anywhere after the pattern in
// the rest of the function. In snd_delay_up's inner loop the cell
// `__local_..._0` is used only here, so the write is dead and the fold
// is safe.
//
// Where it runs
//
// In the asm peephole fixed point, after `replace_pseudoregisters` (so
// Pseudos are resolved to Reg, Data, or ZP). The fold only shrinks code
// (six atoms to two), so it is safe in the fixed point.
#include "passes/dec_register_peephole.h"

#include <cstddef>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/asm_liveness.h"
#include "passes/decimal_arith.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

using Instructions = std::vector<ir::Type_instruction>;

// What a match gives: the register (X or Y) and the branch, which is
// kept as it is.
struct Match {
  ir::Type_reg reg;
  ir::Branch branch;
};

// Returns whether `instr` reads Reg(A).
bool reads_reg_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::Mov:
      return is_reg_a(std::get<ir::Mov>(instr).src);
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      return is_reg_a(i.src) || is_reg_a(i.dst);
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      return is_reg_a(i.src1) || is_reg_a(i.src2);
    }
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      return is_reg_a(i.left) || is_reg_a(i.right);
    }
    case ir::Kind_instruction::Push:
      return is_reg_a(std::get<ir::Push>(instr).src);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return is_reg_a(std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return is_reg_a(std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return is_reg_a(std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return is_reg_a(std::get<ir::RotateRight>(instr).dst);
    default:
      return false;
  }
}

// Returns whether `instr` overwrites Reg(A), clobbering its value.
bool writes_reg_a(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith:
      return true;
    case ir::Kind_instruction::Mov:
      return is_reg_a(std::get<ir::Mov>(instr).dst);
    case ir::Kind_instruction::Add:
      return is_reg_a(std::get<ir::Add>(instr).dst);
    case ir::Kind_instruction::Sub:
      return is_reg_a(std::get<ir::Sub>(instr).dst);
    case ir::Kind_instruction::And:
      return is_reg_a(std::get<ir::And>(instr).dst);
    case ir::Kind_instruction::Or:
      return is_reg_a(std::get<ir::Or>(instr).dst);
    case ir::Kind_instruction::Xor:
      return is_reg_a(std::get<ir::Xor>(instr).dst);
    case ir::Kind_instruction::Pop:
      return is_reg_a(std::get<ir::Pop>(instr).dst);
    case ir::Kind_instruction::ArithmeticShiftLeft:
      return is_reg_a(std::get<ir::ArithmeticShiftLeft>(instr).dst);
    case ir::Kind_instruction::LogicalShiftRight:
      return is_reg_a(std::get<ir::LogicalShiftRight>(instr).dst);
    case ir::Kind_instruction::RotateLeft:
      return is_reg_a(std::get<ir::RotateLeft>(instr).dst);
    case ir::Kind_instruction::RotateRight:
      return is_reg_a(std::get<ir::RotateRight>(instr).dst);
    default:
      return false;
  }
}

bool has_reg_arg_a(const std::vector<std::string>& reg_args) {
  for (const auto& r : reg_args) {
    if (r == "A") {
      return true;
    }
  }
  return false;
}

// Walks forward from `start` until A's state is decided. A is dead at
// `start` when the first operation that touches it overwrites it before
// reading it.
bool a_dead_from(const Instructions& instrs, std::size_t start, const LabelMap& labels) {
  std::size_t i = start;
  std::set<std::size_t> seen;
  while (i < instrs.size()) {
    if (seen.contains(i)) {
      // A loop with nothing decided, which is taken to keep A live.
      return false;
    }
    seen.insert(i);
    const auto& instr = instrs[i];
    if (isa<ir::Label>(instr)) {
      ++i;
      continue;
    }
    // A move of an operand to itself does nothing.
    if (const auto* mov = as<ir::Mov>(instr); mov != nullptr && mov->src == mov->dst) {
      ++i;
      continue;
    }
    if (reads_reg_a(instr)) {
      return false;
    }
    if (writes_reg_a(instr)) {
      return true;
    }
    // An unconditional jump moves to its target.
    if (const auto* jump = as<ir::Jump>(instr)) {
      // A tail call that reads A, which the callee takes as a register
      // argument.
      if (has_reg_arg_a(jump->reg_args)) {
        return false;
      }
      const long tgt = labels.find(jump->target);
      if (tgt < 0) {
        // A jump out of the function (a long-branch trampoline or
        // similar), after which the callee owns A.
        return true;
      }
      i = static_cast<std::size_t>(tgt) + 1;
      continue;
    }
    // A conditional branch would need both paths checked, and the walk
    // gives up on it.
    if (isa<ir::Branch>(instr)) {
      return false;
    }
    // A call that reads A as a register argument keeps it live, and
    // any other call clobbers it.
    if (const auto* call = as<ir::Call>(instr)) {
      return !has_reg_arg_a(call->reg_args);
    }
    // A return clobbers everything.
    if (isa<ir::Ret, ir::Return>(instr)) {
      return true;
    }
    // Any other instruction doesn't touch A.
    ++i;
  }
  // The walk fell off the end of the function, where A dies.
  return true;
}

// Returns whether A is dead at both successors of the branch at
// `branch_idx`.
//
// A is taken to be dead when the first meaningful instruction on each
// successor path writes A or is a call, which clobbers everything. The
// walk skips labels and moves of an operand to itself. With no first
// instruction (the end of the function), A is dead at the exit by
// convention, since the soft-stack frame teardown clobbers it anyway.
bool a_dead_at_successors(const Instructions& instrs, std::size_t branch_idx,
                          const LabelMap& labels) {
  const auto& branch = std::get<ir::Branch>(instrs[branch_idx]);
  // The fall-through start.
  if (!a_dead_from(instrs, branch_idx + 1, labels)) {
    return false;
  }
  // The start of the taken branch.
  const long tgt = labels.find(branch.target);
  if (tgt < 0) {
    // A branch out of the function, like a tail call, after which the
    // callee owns A.
    return true;
  }
  return a_dead_from(instrs, static_cast<std::size_t>(tgt) + 1, labels);
}

// Calls `fn(operand, is_dst)` for every operand of `instr`. `is_dst`
// is set for the destination of a `Mov`, a `Pop`, and a `LoadAddress`,
// and for what a `DecimalArith` writes. Every other position counts as
// a read, the destination of a read-modify-write included.
template <class Fn>
void operand_roles(const ir::Type_instruction& instr, Fn&& fn) {
  switch (ir::kind(instr)) {
    case ir::Kind_instruction::DecimalArith: {
      const auto& d = std::get<ir::DecimalArith>(instr);
      for (const auto& s : decimal_arith::reads(d)) {
        fn(s, false);
      }
      for (const auto& w : decimal_arith::writes(d)) {
        fn(w, true);
      }
      break;
    }
    case ir::Kind_instruction::Mov: {
      const auto& i = std::get<ir::Mov>(instr);
      fn(i.src, false);
      fn(i.dst, true);
      break;
    }
    case ir::Kind_instruction::Add: {
      const auto& i = std::get<ir::Add>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Sub: {
      const auto& i = std::get<ir::Sub>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::And: {
      const auto& i = std::get<ir::And>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Or: {
      const auto& i = std::get<ir::Or>(instr);
      fn(i.src, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Xor: {
      const auto& i = std::get<ir::Xor>(instr);
      fn(i.src1, false);
      fn(i.src2, false);
      fn(i.dst, false);
      break;
    }
    case ir::Kind_instruction::Inc:
      fn(std::get<ir::Inc>(instr).dst, false);
      break;
    case ir::Kind_instruction::Dec:
      fn(std::get<ir::Dec>(instr).dst, false);
      break;
    case ir::Kind_instruction::ArithmeticShiftLeft:
      fn(std::get<ir::ArithmeticShiftLeft>(instr).dst, false);
      break;
    case ir::Kind_instruction::LogicalShiftRight:
      fn(std::get<ir::LogicalShiftRight>(instr).dst, false);
      break;
    case ir::Kind_instruction::RotateLeft:
      fn(std::get<ir::RotateLeft>(instr).dst, false);
      break;
    case ir::Kind_instruction::RotateRight:
      fn(std::get<ir::RotateRight>(instr).dst, false);
      break;
    case ir::Kind_instruction::Push:
      fn(std::get<ir::Push>(instr).src, false);
      break;
    case ir::Kind_instruction::Pop:
      fn(std::get<ir::Pop>(instr).dst, true);
      break;
    case ir::Kind_instruction::Compare: {
      const auto& i = std::get<ir::Compare>(instr);
      fn(i.left, false);
      fn(i.right, false);
      break;
    }
    case ir::Kind_instruction::LoadAddress: {
      const auto& i = std::get<ir::LoadAddress>(instr);
      fn(i.src, false);
      fn(i.dst, true);
      break;
    }
    case ir::Kind_instruction::BitTest:
      fn(std::get<ir::BitTest>(instr).src, false);
      break;
    default:
      break;
  }
}

// Returns whether `target` is in a read position of any instruction of
// `instrs[start:]`. When the temporary is never read again, dropping
// the STA and LDY pair that wrote and read it is safe.
bool operand_read_after(const ir::Type_operand& target, const Instructions& instrs,
                        std::size_t start) {
  for (std::size_t k = start; k < instrs.size(); ++k) {
    bool found = false;
    operand_roles(instrs[k], [&](const ir::Type_operand& op, bool is_dst) {
      if (!is_dst && op == target) {
        found = true;
      }
    });
    if (found) {
      return true;
    }
  }
  return false;
}

// Returns the X or Y register that `op` names, when it is `Reg(X)` or
// `Reg(Y)`.
std::optional<ir::Type_reg> index_register(const ir::Type_operand& op) {
  const auto* reg = as<ir::Reg>(op);
  if (reg != nullptr && isa<ir::X, ir::Y>(reg->reg)) {
    return reg->reg;
  }
  return std::nullopt;
}

bool is_sub_one_from_a(const ir::Type_instruction& instr) {
  const auto* sub = as<ir::Sub>(instr);
  if (sub == nullptr) {
    return false;
  }
  const auto* imm = as<ir::Imm>(sub->src);
  return imm != nullptr && imm->value == 1 && is_reg_a(sub->dst);
}

bool is_ne_or_eq_branch(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  return b != nullptr && isa<ir::NE, ir::EQ>(b->cond);
}

// Returns the register and the branch when the six-instruction
// decrement pattern matches at `i`, else nothing.
std::optional<Match> match_dec_register_pattern(const Instructions& instrs, std::size_t i,
                                                const LabelMap& labels) {
  if (i + 5 >= instrs.size()) {
    return std::nullopt;
  }
  // a: Mov(Reg(R), Reg(A)).
  const auto* a = as<ir::Mov>(instrs[i]);
  if (a == nullptr) {
    return std::nullopt;
  }
  const auto reg = index_register(a->src);
  if (!reg || !is_reg_a(a->dst)) {
    return std::nullopt;
  }
  // b: SetCarry.
  if (!isa<ir::SetCarry>(instrs[i + 1])) {
    return std::nullopt;
  }
  // c: Sub(Imm(1), Reg(A)).
  if (!is_sub_one_from_a(instrs[i + 2])) {
    return std::nullopt;
  }
  // d: Mov(Reg(A), tmp).
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || !is_reg_a(d->src)) {
    return std::nullopt;
  }
  const ir::Type_operand& tmp = d->dst;
  // e: Mov(tmp, Reg(R)), with the tmp and the R of `a`.
  const auto* e = as<ir::Mov>(instrs[i + 4]);
  if (e == nullptr || e->src != tmp) {
    return std::nullopt;
  }
  const auto* e_dst = as<ir::Reg>(e->dst);
  if (e_dst == nullptr || e_dst->reg.index() != reg->index()) {
    return std::nullopt;
  }
  // f: Branch(NE|EQ, _).
  if (!is_ne_or_eq_branch(instrs[i + 5])) {
    return std::nullopt;
  }
  // The soundness gates. (1) A is dead at both of the branch's
  // successors, the head of its target and the head of the
  // fall-through.
  if (!a_dead_at_successors(instrs, i + 5, labels)) {
    return std::nullopt;
  }
  // (2) The spill temporary is dead after the read at `e`, which the
  // simplest case gives: no read of `tmp` in the rest of the function.
  if (operand_read_after(tmp, instrs, i + 6)) {
    return std::nullopt;
  }
  return Match{.reg = *reg, .branch = std::get<ir::Branch>(instrs[i + 5])};
}

// Returns the register and the branch when `instrs[i:i+5]` is `TXA;
// SEC; SBC #1; TAX; BNE|BEQ` (or the Y form), else nothing. That's the
// six-instruction pattern once `redundant_load` has made its `STA tmp;
// LDY tmp` a `TAY` and the store has gone.
//
// `DEY` sets Y and N/Z as `TAY` did from `Y - 1`. It leaves A, C, and V
// alone, where the `SBC` wrote all three, so A must be dead at both
// successors, as for the six-instruction pattern, and C and V must be
// dead after the branch (`asm_liveness`, which counts a return of a
// value in A as a read). A volatile transfer reads and writes the
// register the volatile local lives in, once each, as `DEY` does.
std::optional<Match> match_transfer_back(const Instructions& instrs, std::size_t i,
                                         const LabelMap& labels) {
  if (i + 4 >= instrs.size()) {
    return std::nullopt;
  }
  const auto* a = as<ir::Mov>(instrs[i]);
  if (a == nullptr) {
    return std::nullopt;
  }
  const auto reg = index_register(a->src);
  if (!reg || !is_reg_a(a->dst)) {
    return std::nullopt;
  }
  if (!isa<ir::SetCarry>(instrs[i + 1])) {
    return std::nullopt;
  }
  if (!is_sub_one_from_a(instrs[i + 2])) {
    return std::nullopt;
  }
  const auto* d = as<ir::Mov>(instrs[i + 3]);
  if (d == nullptr || !is_reg_a(d->src)) {
    return std::nullopt;
  }
  const auto* d_dst = as<ir::Reg>(d->dst);
  if (d_dst == nullptr || d_dst->reg.index() != reg->index()) {
    return std::nullopt;
  }
  if (!is_ne_or_eq_branch(instrs[i + 4])) {
    return std::nullopt;
  }
  if (!(c_dead_at(instrs, labels, i + 4) && v_dead_at(instrs, labels, i + 4) &&
        a_dead_at(instrs, labels, i + 4))) {
    return std::nullopt;
  }
  return Match{.reg = *reg, .branch = std::get<ir::Branch>(instrs[i + 4])};
}

void rewrite_function(ir::Function& fn) {
  const auto& instrs = fn.instructions;
  const LabelMap labels(instrs);
  Instructions out;
  bool changed = false;
  std::size_t i = 0;
  while (i < instrs.size()) {
    if (auto m = match_transfer_back(instrs, i, labels)) {
      out.push_back(ir::Dec{.dst = ir::Reg{.reg = m->reg}});
      out.push_back(m->branch);
      i += 5;
      changed = true;
      continue;
    }
    if (auto m = match_dec_register_pattern(instrs, i, labels)) {
      out.push_back(ir::Dec{.dst = ir::Reg{.reg = m->reg}});
      out.push_back(m->branch);
      i += 6;
      changed = true;
      continue;
    }
    out.push_back(instrs[i]);
    ++i;
  }
  if (changed) {
    fn.instructions = std::move(out);
  }
}

}  // namespace

asm_ast::Program apply_dec_register_peephole(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
