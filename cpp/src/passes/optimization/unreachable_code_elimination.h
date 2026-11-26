// The TAC optimizer's unreachable-code elimination.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with its unreachable blocks dropped, the Phi arguments
// from edges that no longer exist pruned, each Phi left with one
// argument made a Copy, each jump to the next block dropped, and each
// label that no jump and no Phi names dropped. It reads nothing of
// `ctx`.
tac_ast::Function eliminate_unreachable_code(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
