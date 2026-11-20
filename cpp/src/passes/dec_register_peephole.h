// The port of passes/dec_register_peephole.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Collapses `Mov(Reg(R), A); SetCarry; Sub(Imm(1), A); Mov(A, tmp);
// Mov(tmp, Reg(R)); Branch(NE|EQ, _)`, and the five-instruction form
// without the temporary, into `Dec(Reg(R)); Branch(NE|EQ, _)` when R
// is X or Y and the gates in the comment at the top of
// dec_register_peephole.cpp pass.
asm_ast::Program apply_dec_register_peephole(asm_ast::Program prog);

}  // namespace c6502::passes
