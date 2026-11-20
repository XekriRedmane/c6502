// The port of passes/jump_table.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Turns each chain of compares of A with constants that the rules at
// the top of jump_table.cpp allow into a `JumpTable`, in every function
// of the program. The labels it adds are `.jtab@N` and `.jtdef@N`,
// numbered across the program from one more than the largest already
// there.
asm_ast::Program apply_jump_tables(asm_ast::Program prog);

}  // namespace c6502::passes
