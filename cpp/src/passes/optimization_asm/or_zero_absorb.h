// The port of passes/optimization_asm/or_zero_absorb.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Mov(Imm(0), Reg(A)); Or(X, Reg(A))`, the first
// not volatile, made `Mov(X, Reg(A))`.
asm_ast::Function absorb_zero_load(asm_ast::Function fn);

}  // namespace c6502::passes
