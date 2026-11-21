// The port of passes/return_in_zero_page.py, which returns a `static`
// function's pointer result in a zero-page pointer of its own.
#pragma once

#include <string>

#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns the name of the zero-page pointer that `fn` returns its result
// in, `<fn>@ret`.
std::string return_slot_name(const std::string& fn);

// Rewrites each qualifying function of `tac` and every call of it (the
// .cpp's comment), adds a `StaticVariable` and a symbol for each one's
// `F@ret`, and returns the new program. A program with no qualifying
// function comes back unchanged.
tac_ast::Program return_pointers_in_zero_page(tac_ast::Program tac, SymbolTable& symbols);

}  // namespace c6502::passes
