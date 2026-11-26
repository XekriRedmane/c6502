#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each eligible `Load` through `ptr + ZeroExtend(i)`
// or `ptr + k` rewritten as `IndirectIndexedLoad(ptr, i or k, dst)` and
// each eligible `Store` as `IndirectIndexedStore`, dropping the address
// computation with the rewrite when nothing else reads it. It may add
// the temporaries `<addr>.base` and `<ptr>.y<k>` to the symbol table
// and the SSA names. A no-op outside SSA form.
tac_ast::Function recognize_indirect_indexed(tac_ast::Function fn,
                                             const framework::PassContext& ctx);

}  // namespace c6502::passes
