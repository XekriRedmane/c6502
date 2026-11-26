#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each eligible scalar `Load`, `Store`,
// `IndirectIndexedLoad`, and `IndirectIndexedStore` through `&obj + k`
// rewritten as an `IndexedLoad` or `IndexedSymbolStore` of `obj` at the
// constant index `k`. A no-op without a symbol table or SSA names, or
// with an empty set of SSA names.
tac_ast::Function direct_member_access(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
