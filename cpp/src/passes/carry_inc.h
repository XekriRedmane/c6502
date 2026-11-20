// The port of passes/carry_inc.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites every eligible run of `LDA M; ADC #0; STA M` in the
// program's functions to a `BCC` / `INC` chain, and every eligible
// `LDA M; SBC #0; STA M` to `BCS skip; DEC M; skip:`. The labels it
// adds are numbered across the program, past any `.carry_inc@N` from an
// earlier run. The comment at the top of carry_inc.cpp gives the rules.
asm_ast::Program apply_carry_inc_peephole(asm_ast::Program prog);

}  // namespace c6502::passes
