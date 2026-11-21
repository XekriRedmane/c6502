// The port of passes/optimization/reassoc_const.py, which adds the two
// constants of a nested pair of Adds at compile time.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Add(C1, %inner)` whose single-use SSA `%inner`
// is `Add(C2, V)` rewritten to `Add(C1 + C2, V)` and the inner Add
// dropped (Python's `ReassocConstants`), or `fn` itself when none is.
// Outside SSA form it leaves `fn` alone.
tac_ast::Function reassoc_constants(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
