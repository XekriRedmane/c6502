// The port of passes/optimization/known_compare_thread.py, the pass of
// the TAC optimizer's fixed point after SSA destruction that sends a
// jump past the equality tests at its target that the jump's own path
// has decided.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each jump sent past the equality tests at its target
// that its path has decided, to a `.<fn>@cthread@<N>` label where there
// is none, each conditional jump that the code before it has decided
// made a `Jump` or removed, the blocks nothing reaches removed, and a
// conditional jump's target block placed right after the `Jump` that
// follows it where that lets `branch_invert` make one branch (Python's
// `thread_known_compares`). Without a symbol table the Vars' types are
// unknown, and `fn` comes back unchanged.
tac_ast::Function thread_known_compares(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
