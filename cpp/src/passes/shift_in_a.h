#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Moves the shifts of a byte just stored from A onto A
// (`STA M; ASL M; LDA M` as `ASL A; STA M`), rewrites `STA M; INC M;
// LDA M` as `CLC; ADC #1; STA M`, rewrites `LDA M; ASL; STA M` as
// `ASL M`, and moves a loop of one-bit shifts of a byte onto A, in each
// function of the program. The comment at the top of shift_in_a.cpp
// gives the shapes and the conditions. The statics come back unchanged.
asm_ast::Program apply_shift_in_a(asm_ast::Program prog);

}  // namespace c6502::passes
