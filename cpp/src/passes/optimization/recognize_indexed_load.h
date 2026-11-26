#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Load` of a 1-byte Var through `C +
// ZeroExtend(i)`, for a 1-byte `i` and a constant `C` from 0 to 0xFF00,
// replaced by `IndexedConstLoad(C, i, dst)`, dropping the Add and the
// extend. A no-op outside SSA form.
tac_ast::Function recognize_indexed_load(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
