#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Store` of a 1-byte value through `C +
// ZeroExtend(i)`, for a 1-byte `i` and a constant `C` from 0 to 0xFF00,
// replaced by `IndexedStore(C, i, src)`, dropping the Add and the
// extend. A no-op outside SSA form.
tac_ast::Function recognize_indexed_store(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
