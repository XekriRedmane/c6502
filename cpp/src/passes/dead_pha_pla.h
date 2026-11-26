#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops each `PHA` / `PLA` pair of the program's functions whose body
// leaves A alone, and rewrites `LDA M; PHA; <register moves>; PLA` as
// the moves followed by `LDA M`. The statics come back unchanged.
asm_ast::Program apply_dead_pha_pla(asm_ast::Program prog);

}  // namespace c6502::passes
