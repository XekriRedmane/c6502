// Drops the `LDY #$00` before an `Indirect(0)` operand when Y is already
// 0 on the fall-through of a `Dec(Reg(Y)); Branch(NE, _)`.
//
// The shape that called for it is the exit of the inner loop of
// snd_delay_up:
//
//     .loop@1_continue:
//     DEY
//     BNE .loop@1_continue
//     LDY #$00                  ; Y is already 0
//     CMP (DPTR),Y
//
// The LDY isn't an atom of its own in the asm IR. It is part of the
// operand. `asm_emit` writes `Compare(Reg(A), Indirect(0))` as `LDY
// #$00; CMP (DPTR),Y`, since the `Indirect(off)` operand puts a load of
// Y in front, and the `IndirectY()` operand leaves that out and writes
// `<op> (DPTR),Y` with whatever Y holds.
//
// The rewrite turns each `Indirect(0)` operand of the instruction right
// after a `Dec(Reg(Y)); Branch(NE, _)` pair into `IndirectY()`. Y is 0
// after the BNE falls through, so `Indirect(0)` and `IndirectY()` reach
// the same byte, and the LDY goes.
//
// Soundness
//
//   * DEY decrements Y and sets Z from the result.
//   * BNE branches when Z is 0 (Y isn't 0), so the fall-through has Z
//     set and Y 0.
//   * `Indirect(0)` and `IndirectY()` reach the same byte through DPTR
//     when Y is 0.
//   * The `LDY #$00` that emission would add sets Z and clears N, the
//     values DEY already left when Y reached 0, so leaving it out keeps
//     N/Z as they were at the access.
//
// The instruction with the `Indirect(0)` must come right after the
// branch. A label between them could be the target of another branch,
// from code where Y holds anything, and the rewrite would change what
// that code reads. Saving 2 cycles and 2 bytes doesn't justify a looser
// match.
//
// Where it runs
//
// In the asm peephole fixed point, after `apply_volatile_void_read_cmp`
// and the promotions of loop counters have left their final shapes.
#include "passes/dec_branch_indirect_y_fold.h"

#include <cstddef>

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

bool is_dec_y(const ir::Type_instruction& instr) {
  const auto* d = as<ir::Dec>(instr);
  if (d == nullptr) {
    return false;
  }
  const auto* r = as<ir::Reg>(d->dst);
  return r != nullptr && isa<ir::Y>(r->reg);
}

bool is_branch_ne(const ir::Type_instruction& instr) {
  const auto* b = as<ir::Branch>(instr);
  return b != nullptr && isa<ir::NE>(b->cond);
}

bool is_indirect_zero(const ir::Type_operand& op) {
  const auto* ind = as<ir::Indirect>(op);
  return ind != nullptr && ind->offset == 0;
}

// Replaces `op` with `IndirectY()` when it is `Indirect(0)`.
void fold(ir::Type_operand& op) {
  if (is_indirect_zero(op)) {
    op = ir::IndirectY{};
  }
}

// Replaces each `Indirect(0)` operand of a `Mov` or `Compare` with
// `IndirectY()`. Any other instruction is left as it is.
void rewrite_indirect_zero(ir::Type_instruction& instr) {
  if (auto* m = as<ir::Mov>(instr)) {
    fold(m->src);
    fold(m->dst);
  } else if (auto* c = as<ir::Compare>(instr)) {
    fold(c->left);
    fold(c->right);
  }
}

void rewrite_function(ir::Function& fn) {
  auto& instrs = fn.instructions;
  for (std::size_t i = 0; i + 2 < instrs.size(); ++i) {
    // The instruction at i + 2 runs with Y == 0 on the fall-through.
    if (is_dec_y(instrs[i]) && is_branch_ne(instrs[i + 1])) {
      rewrite_indirect_zero(instrs[i + 2]);
    }
  }
}

}  // namespace

asm_ast::Program apply_dec_branch_indirect_y_fold(asm_ast::Program prog) {
  for (auto& tl : prog.top_level) {
    if (auto* fn = as<ir::Function>(tl)) {
      rewrite_function(*fn);
    }
  }
  return prog;
}

}  // namespace c6502::passes
