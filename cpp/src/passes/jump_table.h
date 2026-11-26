#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Turns each chain of at least four `CMP #k; BEQ case` on A, with k
// from 0 to 127, into a `JumpTable`, in every function of the program,
// when A, N/Z, C, and the index register are dead at every case and the
// default, and the table is shorter than the chain. The labels it adds
// are `.jtab@N` and `.jtdef@N`, numbered across the program from one
// more than the largest already there.
asm_ast::Program apply_jump_tables(asm_ast::Program prog);

}  // namespace c6502::passes
