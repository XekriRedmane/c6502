// The port of passes/optimization/sink_and_past_branch.py, the pass of
// the TAC optimizer's fixed point that copies a `ZeroExtend;
// BitwiseAnd; Truncate` trio from before a `JumpIfMasked` on the same
// byte into each of the jump's two successors.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each sinkable trio before a block's closing
// `JumpIfMasked` copied, under the names `<name>.snk_then@<n>` and
// `<name>.snk_else@<n>`, to the top of the fall-through block and of
// the jump's target, the uses of its result in each renamed, and the
// original dropped except for any part that another instruction reads
// (Python's `SinkAndPastBranch`). It adds the new names to the symbol
// table but not to the SSA names. A no-op without a symbol table.
tac_ast::Function sink_and_past_branch(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
