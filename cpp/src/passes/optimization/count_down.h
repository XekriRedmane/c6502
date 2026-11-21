// The port of passes/optimization/count_down.py, the pass after the TAC
// optimizer's fixed point that makes a loop whose byte counter only
// counts hold the passes left and test it for 0.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every loop that meets the module's conditions
// counting down to 0: its counter's `Phi` starts at the bound minus the
// start, its step subtracts 1, and its test compares it with 0 (Python's
// `count_loops_down`). The names stay. It does nothing without a symbol
// table or SSA names.
tac_ast::Function count_loops_down(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
