// The port of passes/short_jumps.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Shortens the jumps of every function of the program, as the comment
// at the top of short_jumps.cpp describes: a jump to a stub that only
// tail-calls another function goes there itself, the stubs nothing
// reaches go, and a `JMP` after a load of a constant or a branch
// becomes a branch when its label is in reach.
asm_ast::Program apply_short_jumps(asm_ast::Program prog);

}  // namespace c6502::passes
