// The part of passes/optimization/compare_subtract.py that code
// generation reads: which subtraction decides an ordering compare.
// The TAC pass itself, which moves the subtract up to the compare, is
// part of the TAC optimizer and isn't ported yet.
#pragma once

#include <utility>

#include "tac_ast.h"

namespace c6502::passes::compare_subtract {

// Returns the `(a, b)` of the subtraction `a - b` whose flags decide the
// ordering compare `src1 op src2`, as `tac_to_asm` lowers it: the
// operands as they are for `<` and `>=`, and swapped for `>` and `<=`.
// The pointers are to `src1` and `src2`.
std::pair<const tac_ast::Type_val*, const tac_ast::Type_val*> compared_difference(
    const tac_ast::Type_binary_operator& op, const tac_ast::Type_val& src1,
    const tac_ast::Type_val& src2);

}  // namespace c6502::passes::compare_subtract
