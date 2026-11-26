#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every read of an SSA `Copy`'s destination replaced
// by the Copy's source, followed through chains of Copies. A Phi's
// destination isn't propagated. Outside SSA form it leaves `fn` alone.
tac_ast::Function copy_propagate(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
