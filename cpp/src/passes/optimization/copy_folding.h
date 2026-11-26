#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `<producer dst=%t>; Copy(%t, X)` whose `%t` the
// Copy alone reads, and which nothing outside the instructions can read,
// made `<producer dst=X>`, or `fn` itself when no pair fuses.
tac_ast::Function fold_copies_in_fixedpoint(tac_ast::Function fn, const framework::PassContext& ctx);

// The same fusion after SSA destruction.
tac_ast::Function fold_copies_post_destruction(tac_ast::Function fn,
                                               const framework::PassContext& ctx);

}  // namespace c6502::passes
