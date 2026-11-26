#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites each `Indirect(0)` operand of the `Mov` or `Compare` right
// after a `Dec(Reg(Y)); Branch(NE, _)` pair as `IndirectY()`, since Y is
// 0 on the fall-through, which drops the `LDY #$00` the operand is
// emitted with.
asm_ast::Program apply_dec_branch_indirect_y_fold(asm_ast::Program prog);

}  // namespace c6502::passes
