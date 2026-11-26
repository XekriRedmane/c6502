#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites each eligible in-place subtract of 1 from a byte (the SBC
// chain `LDA M; SEC; SBC #1; STA M`) in the program's functions to
// `DEC M` (or `DEX` / `DEY`), and the 2-byte decrement that `carry_inc`
// leaves to a test of the low byte before its `DEC`.
asm_ast::Program apply_dec_peephole(asm_ast::Program prog);

}  // namespace c6502::passes
