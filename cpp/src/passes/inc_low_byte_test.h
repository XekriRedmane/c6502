#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Moves the test of a low byte that an INC chain has just incremented
// onto the chain's first `BNE`, in each of the program's functions.
asm_ast::Program apply_inc_low_byte_test(asm_ast::Program prog);

}  // namespace c6502::passes
