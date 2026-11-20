// The port of passes/cmp_sbc_fusion.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Fuses each `LDA M; CMP N; B<cond> L; [Label L2;] [SEC;] SBC N` of the
// program's functions into `LDA M; SEC; SBC N; B<cond> L; [Label L2]`,
// dropping the `CMP`, when A and V are dead at L. The comment at the
// top of cmp_sbc_fusion.cpp gives the conditions and the reasons.
asm_ast::Program apply_cmp_sbc_fusion(asm_ast::Program prog);

}  // namespace c6502::passes
