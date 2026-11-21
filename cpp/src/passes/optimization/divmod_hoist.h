// The port of passes/optimization/divmod_hoist.py, the pass after the
// TAC optimizer's fixed point that puts a `/` and a `%` of the same
// operands next to each other, so that `tac_to_asm` lowers them with one
// loop or one helper call.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with the first pair of a `/` and a `%` of the same
// operands that meets the module's conditions computed at one place, a
// `Copy` of each result left where its instruction was, and the same
// done again until no pair is left, at most 64 times (Python's
// `hoist_divmod_pairs`). It adds a `<dst>.dm<k>` temporary to the symbol
// table and the SSA names for each destination SSA didn't rename. It
// does nothing without a symbol table or SSA names.
tac_ast::Function hoist_divmod_pairs(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
