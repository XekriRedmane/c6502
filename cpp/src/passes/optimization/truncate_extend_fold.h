// The two passes of the TAC optimizer's fixed point that fold a Truncate
// or an extend of an extend into one conversion of the original value.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Truncate(t, u)` whose source an SSA
// `SignExtend`, `ZeroExtend`, or `Truncate` of an SSA-renamed `x`
// defines replaced by a `Copy(x, u)` when u is as wide as x, a
// `Truncate(x, u)` when u is narrower, and the same extend of x to u
// when u is between the two widths.
tac_ast::Function fold_truncate_extend(tac_ast::Function fn, const framework::PassContext& ctx);

// Returns `fn` with each extend of a widening extend of an SSA-renamed
// `x` replaced by one extend of x.
tac_ast::Function fold_extend_extend(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
