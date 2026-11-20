// The port of passes/unreachable_drop.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Removes the instructions after each `Jump`, `Return`, or `Ret` of the
// program's functions up to the next label that something names, as
// the comment at the top of unreachable_drop.cpp describes.
asm_ast::Program apply_unreachable_drop(asm_ast::Program prog);

}  // namespace c6502::passes
