// The port of passes/optimization/direct_member_access.py, the pass of
// the TAC optimizer's fixed point that makes an access through `&obj +
// k` a direct access to byte `k` of `obj`.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each eligible scalar `Load`, `Store`,
// `IndirectIndexedLoad`, and `IndirectIndexedStore` through `&obj + k`
// rewritten as an `IndexedLoad` or `IndexedSymbolStore` of `obj` at the
// constant index `k` (Python's `direct_member_access`). A no-op without
// a symbol table or SSA names, or with an empty set of SSA names.
tac_ast::Function direct_member_access(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
