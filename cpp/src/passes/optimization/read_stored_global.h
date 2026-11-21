// The port of passes/optimization/read_stored_global.py, which reads a
// global just stored from a temporary in place of the temporary.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with the one later read of each temporary `T` that a
// `Copy(T, G)` into a global `G` follows made a read of `G`, where the
// module comment's conditions hold (Python's `read_stored_globals`), or
// `fn` itself when none do. Without a symbol table, or outside SSA
// form, it leaves `fn` alone.
tac_ast::Function read_stored_globals(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
