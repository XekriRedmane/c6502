// The pass after SSA destruction that splits a 2-byte ordering compare
// and an equality compare of the same two values into compares of their
// high bytes and then their low bytes.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each 2-byte unsigned ordering `JumpIfCmp` that has
// an equality `JumpIfCmp` of the same values right after it, or at its
// target, rewritten as compares of the high bytes and then the low
// bytes. It adds the byte temporaries it makes, `%<fn>.cmp3.<n>`, to the
// symbol table, even for a pair it then leaves alone. A no-op without a
// symbol table.
tac_ast::Function split_three_way_compares(tac_ast::Function fn,
                                           const framework::PassContext& ctx);

}  // namespace c6502::passes
