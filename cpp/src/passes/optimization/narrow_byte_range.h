// The port of passes/optimization/narrow_byte_range.py, which does a
// wide divide, remainder, or shift of a value from 0 to 255 on one byte,
// and reads the operands of a wide multiply of two such values as
// zero-extended bytes.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each wide `/`, `%`, `>>`, `<<`, and `*` whose
// operands `value_range::max_value` bounds by 255 done on bytes, by the
// rules of the .cpp's module comment (Python's `NarrowByteRange`), or
// `fn` itself when none is. Outside SSA form, or without a symbol table,
// it returns `fn`. Each rewrite adds temporaries named `<dst>.lo<k>`,
// `<dst>.nb<k>`, and `<dst>.nw<k>` to the symbols and the SSA names.
tac_ast::Function narrow_byte_range(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
