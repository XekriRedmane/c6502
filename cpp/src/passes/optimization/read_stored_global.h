#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with the one later read of each temporary `T` that a
// `Copy(T, G)` into a global `G` follows made a read of `G`, when `G`
// still holds `T`'s value at that read, or `fn` itself when none is.
// Without a symbol table, or outside SSA form, it leaves `fn` alone.
tac_ast::Function read_stored_globals(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
