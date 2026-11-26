#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each wide `&`, `|`, and `^` of an extended byte whose
// result's high bytes are zero done on the low bytes into a new
// `unsigned char` and zero-extended, or `fn` itself when none is.
// Outside SSA form, or without a symbol table, it returns `fn`. Each
// rewrite adds temporaries named `<dst>.lo<k>` and `<dst>.n<k>` to the
// symbols and the SSA names.
tac_ast::Function narrow_byte_bitwise(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
