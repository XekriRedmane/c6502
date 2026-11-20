// The port of passes/split_mem_to_mem.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Splits each `Mov(mem, mem)` of the program's functions into
// `Mov(mem, Reg(A)); Mov(Reg(A), mem)`, and each move between X and Y
// into the two moves through A that `asm_emit` writes for it, and
// drops each move of an operand to itself. The comment at the top of
// split_mem_to_mem.cpp gives the reasons. The statics and every
// function the pass leaves alone come back unchanged.
asm_ast::Program apply_split_mem_to_mem(asm_ast::Program prog);

}  // namespace c6502::passes
