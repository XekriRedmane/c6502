#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every loop whose byte counter is read only by its own
// test and its own `+ 1` step counting down to 0: its counter's `Phi`
// starts at the bound minus the start, its step subtracts 1, and its test
// compares it with 0. The names stay. It does nothing without a symbol
// table or SSA names.
tac_ast::Function count_loops_down(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
