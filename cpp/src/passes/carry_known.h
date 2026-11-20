// The port of passes/carry_known.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops each `SEC` and `CLC` where the carry already has that value or
// nothing reads it, each `ADC #0` / `SBC #0` that leaves A and C as they
// were, and moves a setter that starts a loop in front of its labels.
// The comment at the top of carry_known.cpp describes the dataflow.
asm_ast::Program apply_carry_known(asm_ast::Program prog);

}  // namespace c6502::passes
