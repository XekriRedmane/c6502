// The port of passes/optimization/step_on_edge.py, the pass after SSA
// destruction that moves a step whose result is copied on one edge
// alone onto that edge, in place of the copy.
#pragma once

#include "passes/asm_context.h"
#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every step `T = X op c` that meets the module's
// conditions dropped and `Z = X op c` put in place of the one `Copy(T,
// Z)` that reads it (Python's `move_steps_onto_edges`). `ssa_names`
// holds the names SSA construction renamed.
tac_ast::Function move_steps_onto_edges(tac_ast::Function fn, const NameSet& ssa_names);

// Runs `move_steps_onto_edges` as the driver calls it, with the SSA
// names of `ctx`, and returns `fn` itself when there are none (Python's
// `MoveStepsOntoEdges.run`).
tac_ast::Function move_steps_onto_edges(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
