#pragma once

#include "passes/asm_context.h"
#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every step `T = X op c` whose result only a
// `Copy(T, Z)` on one edge reads dropped and `Z = X op c` put in place
// of that Copy. `ssa_names` holds the names SSA construction renamed.
tac_ast::Function move_steps_onto_edges(tac_ast::Function fn, const NameSet& ssa_names);

// Runs `move_steps_onto_edges` as the driver calls it, with the SSA
// names of `ctx`, and returns `fn` itself when there are none.
tac_ast::Function move_steps_onto_edges(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
