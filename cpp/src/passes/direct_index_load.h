#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites each `LDA M; TAX` (or `TAY`) of the program's functions as
// `LDX M` (or `LDY M`) when `LDX` / `LDY` can address M and A is dead
// after the transfer. The statics come back unchanged.
asm_ast::Program apply_direct_index_load(asm_ast::Program prog);

}  // namespace c6502::passes
