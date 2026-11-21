// The port of passes/optimization/recognize_indirect_indexed.py, the
// pass after the TAC optimizer's fixed point that makes a load or store
// through a pointer plus a zero-extended byte, or plus a constant from
// 0 to 255, an `IndirectIndexedLoad` or `IndirectIndexedStore`, which
// `tac_to_asm` lowers to `(zp),Y`.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each eligible `Load` through `ptr + ZeroExtend(i)`
// or `ptr + k` rewritten as `IndirectIndexedLoad(ptr, i or k, dst)` and
// each eligible `Store` as `IndirectIndexedStore`, dropping the address
// computation with the rewrite when nothing else reads it (Python's
// `RecognizeIndirectIndexed`). It may add the temporaries
// `<addr>.base` and `<ptr>.y<k>` to the symbol table and the SSA names.
// A no-op outside SSA form.
tac_ast::Function recognize_indirect_indexed(tac_ast::Function fn,
                                             const framework::PassContext& ctx);

}  // namespace c6502::passes
