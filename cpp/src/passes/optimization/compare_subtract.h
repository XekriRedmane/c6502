// The port of passes/optimization/compare_subtract.py: the pass that
// moves a subtract of an ordering compare's operands up to the compare,
// and `compared_difference`, which code generation reads too.
#pragma once

#include <utility>

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

namespace compare_subtract {

// Returns the `(a, b)` of the subtraction `a - b` whose flags decide the
// ordering compare `src1 op src2`, as `tac_to_asm` lowers it: the
// operands as they are for `<` and `>=`, and swapped for `>` and `<=`.
// The pointers are to `src1` and `src2`.
std::pair<const tac_ast::Type_val*, const tac_ast::Type_val*> compared_difference(
    const tac_ast::Type_binary_operator& op, const tac_ast::Type_val& src1,
    const tac_ast::Type_val& src2);

}  // namespace compare_subtract

// Returns `fn` with each `t = a - b` that a successor of an ordering
// `JumpIfCmp` of `a` and `b` computes moved to just before the compare,
// and, where `a` is a value the loop carries round, the other successor
// given `a` again as `t + b` (Python's `hoist_subtract_to_compare`). It
// adds the `<a>.exit` temporaries to the symbol table and the SSA names.
// It does nothing without a symbol table or outside SSA form.
tac_ast::Function hoist_subtract_to_compare(tac_ast::Function fn,
                                            const framework::PassContext& ctx);

}  // namespace c6502::passes
