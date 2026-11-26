#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every `for (T x = N; x >= 0; x--)` loop of
// c99_to_tac's shape rotated to test at the bottom, and a SignExtend of
// x in its body made a ZeroExtend when the body doesn't write x.
// Throws `InternalError` without a symbol table once a loop's test
// matches.
tac_ast::Function rotate_signed_countdown_loops(tac_ast::Function fn,
                                                const framework::PassContext& ctx);

}  // namespace c6502::passes
