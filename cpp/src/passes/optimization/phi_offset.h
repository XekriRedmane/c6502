// The port of passes/optimization/phi_offset.py, the pass after the TAC
// optimizer's fixed point that makes a join of `p + k1` and `p + k2` a
// choice of `k1` or `k2` added to `p`.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each eligible Phi of `p + k` values rewritten as a
// Phi of the constants `k`, `<dst>.o`, which is zero-extended into
// `<dst>.e` and added to `p` after the block's last Phi (Python's
// `phi_offset`). It adds the two temporaries to the symbol table and
// the SSA names. A no-op without a symbol table or SSA names, or with
// an empty set of SSA names.
tac_ast::Function phi_offset(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
