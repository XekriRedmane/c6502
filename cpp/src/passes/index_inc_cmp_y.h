#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Rewrites `LDA m,X; STA tmp; INC tmp; LDA tmp; CMP n` (or `DEC`), and
// `LDA m,X; CLC; ADC #1; CMP n` (or `SEC; SBC #1`), as `LDY m,X;
// INY; CPY n` (or `DEY`) where Y is free and nothing reads A, the
// temporary, or V afterwards.
asm_ast::Program apply_index_inc_cmp_y(asm_ast::Program prog, const AsmContext& ctx);

}  // namespace c6502::passes
