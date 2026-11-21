// The port of passes/optimization/dead_store_elimination.py, the TAC
// optimizer's dead-store elimination in SSA form.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each pure definition of an SSA name that nothing
// reads dropped, and the destination of each call whose result nothing
// reads taken off, repeated until nothing more goes (Python's
// `eliminate_dead_stores`). Stores, returns, and control flow stay.
// Outside SSA form it leaves `fn` alone.
tac_ast::Function eliminate_dead_stores(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
