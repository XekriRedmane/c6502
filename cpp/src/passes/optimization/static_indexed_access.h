// The port of passes/optimization/static_indexed_access.py, the pass
// after the TAC optimizer's fixed point that makes an access to a
// static object at a byte index plus a constant an `IndexedLoad` or
// `IndexedSymbolStore` with an offset.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Load`, `Store`, and constant-indexed
// `IndirectIndexedLoad` and `IndirectIndexedStore` through
// `GetAddress(o) + ZeroExtend(b)`, plus a constant, for a static `o`
// and a 1-byte `b`, rewritten as an `IndexedLoad` or
// `IndexedSymbolStore` of `o` at the index `b` and that offset, and each
// `(g + z) + k` reassociated as `(g + k) + z`, with `g + k` in a new
// temporary `<dst>.base` (Python's `static_indexed_access`). A no-op
// without a symbol table or SSA names, or with an empty set of SSA
// names.
tac_ast::Function static_indexed_access(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
