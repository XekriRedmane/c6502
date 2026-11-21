// The port of passes/optimization/licm.py, the TAC optimizer's
// loop-invariant code motion of extends and truncates.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each loop-invariant extend and `Truncate` moved to
// the end of its loop's preheader (Python's `hoist_loop_invariants`),
// or `fn` itself when none moves. Outside SSA form it leaves `fn`
// alone.
tac_ast::Function hoist_loop_invariants(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
