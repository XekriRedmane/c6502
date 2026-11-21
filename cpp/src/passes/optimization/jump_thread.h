// The port of passes/optimization/jump_thread.py, which sends an edge
// into a join straight to where the join's test sends the constant the
// edge brings.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every edge that brings a join a constant deciding
// the join's test sent to the successor the test picks, one join at a
// time (Python's `thread_constant_jumps`), or `fn` itself when no edge
// qualifies. The new labels are `.<fn>@jthread@<k>`. Outside SSA form
// it leaves `fn` alone.
tac_ast::Function thread_constant_jumps(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
