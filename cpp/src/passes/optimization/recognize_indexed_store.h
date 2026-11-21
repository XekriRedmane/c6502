// The port of passes/optimization/recognize_indexed_store.py, the pass
// of the TAC optimizer's fixed point that makes a byte store at a
// constant address plus a zero-extended byte an `IndexedStore`
// (absolute,X).
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Store` of a 1-byte value through `C +
// ZeroExtend(i)`, for a 1-byte `i` and a constant `C` from 0 to 0xFF00,
// replaced by `IndexedStore(C, i, src)`, dropping the Add and the
// extend (Python's `RecognizeIndexedStore`). A no-op outside SSA form.
tac_ast::Function recognize_indexed_store(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
