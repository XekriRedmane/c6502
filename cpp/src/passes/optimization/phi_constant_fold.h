#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with the first operation of a Phi of constants that
// folds moved into the Phi's arguments, or `fn` itself when none does.
// It adds a temporary `<dst>.p<k>` to the symbols and the SSA names
// when the operation's destination isn't an SSA name.
// Without a symbol table or outside SSA form it leaves `fn` alone.
tac_ast::Function fold_into_constant_phis(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
