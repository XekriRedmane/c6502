#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Folds the arithmetic on constants in A that
// docs/optimizations/const_arith_fold.md lists: an `AND` or `ORA` of an
// immediate after an immediate load, an identity `AND` or `ORA` after a
// write of A, an add of constants, a label's high byte plus a constant,
// a shift that takes in a 1, and an add of 0 with the carry known.
asm_ast::Program apply_const_arith_fold(asm_ast::Program prog);

}  // namespace c6502::passes
