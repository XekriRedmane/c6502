// The port of passes/sub1_test_zero_peephole.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops the `LDA #0; CMP M` that tests M against zero after an
// `LDA M; SEC; SBC #1; STA dst` in the program's functions, and flips
// the `BCC` / `BCS` after it to read the SBC's carry instead. The
// comment at the top of sub1_test_zero_peephole.cpp gives the rules.
asm_ast::Program apply_sub1_test_zero_peephole(asm_ast::Program prog);

}  // namespace c6502::passes
