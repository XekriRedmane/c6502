#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each eligible `p + (x + k)` rewritten to `(p + x) +
// k`, or `fn` itself when none is.
// Each rewrite registers the temporary `<addr>.ra` with the type of
// `<addr>`, unless it has a symbol, and adds it to the SSA names.
// Outside SSA form, with no SSA names, or without a symbol table, it
// leaves `fn` alone.
tac_ast::Function reassoc_offset_constant(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
