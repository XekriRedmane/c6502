// The walk over every operand of an asm instruction that regalloc,
// `function_local_sizing`, and the optimizer's driver call.
#pragma once

#include <vector>

#include "asm_ast.h"

namespace c6502::passes::optimizer {

// Returns every operand of `instr`, as pointers into it, in this order:
// the operands of a `DecimalArith` (`left`, `right`, then `dst`), `src`
// and `dst` of a `Mov`, `Add`, `Sub`, `And`, or `Or`, `src1`, `src2`,
// and `dst` of an `Xor`, the one operand of an `Inc`, `Dec`, shift,
// rotate, `Push`, or `Pop`, `left` and `right` of a `Compare`, `src` and
// `dst` of a `LoadAddress`, and a `Phi`'s `dst` and then each argument's
// source. Any other instruction gives none, `BitTest` included.
std::vector<const asm_ast::Type_operand*> all_operands_in(const asm_ast::Type_instruction& instr);

}  // namespace c6502::passes::optimizer
