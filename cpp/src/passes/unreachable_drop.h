#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Removes the instructions after each `Jump`, `Return`, or `Ret` of the
// program's functions up to the next label that something names, and
// repeats until no more are removed.
asm_ast::Program apply_unreachable_drop(asm_ast::Program prog);

}  // namespace c6502::passes
