// The port of passes/cpx_cpy_peephole.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Rewrites each `TXA; CMP R` (or `TYA; CMP R`) of the program's
// functions as `CPX R` (or `CPY R`), and each `LDA M; STA t; CPX t` as
// `CPX M`, when A is dead after the compare and the operands allow it.
// The comment at the top of cpx_cpy_peephole.cpp gives the conditions.
// The statics come back unchanged.
asm_ast::Program apply_cpx_cpy_peephole(asm_ast::Program prog, const AsmContext& ctx);

}  // namespace c6502::passes
