#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each wide `/`, `%`, `>>`, `<<`, and `*` whose
// operands `value_range::max_value` bounds by 255 done on bytes, or
// `fn` itself when none is. Outside SSA form, or without a symbol table,
// it returns `fn`. Each rewrite adds temporaries named `<dst>.lo<k>`,
// `<dst>.nb<k>`, and `<dst>.nw<k>` to the symbols and the SSA names.
tac_ast::Function narrow_byte_range(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
