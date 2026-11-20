// The port of passes/inc_low_byte_test.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Moves the test of a low byte that an INC chain has just incremented
// onto the chain's first `BNE`, in each of the program's functions, as
// the comment at the top of inc_low_byte_test.cpp describes.
asm_ast::Program apply_inc_low_byte_test(asm_ast::Program prog);

}  // namespace c6502::passes
