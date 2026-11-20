// The port of passes/carry_recompute.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops each `LDA M; ASL A` (or `ROL A`, `LSR A`, `ROR A`) that sets C
// to the bit of M that C already holds, when A and N/Z are dead after
// it. The comment at the top of carry_recompute.cpp gives the details.
asm_ast::Program apply_carry_recompute_drop(asm_ast::Program prog);

}  // namespace c6502::passes
