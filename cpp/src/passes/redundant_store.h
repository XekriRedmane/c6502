#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Drops, within each basic block, an `LDA S; STA D` pair that copies a
// byte into a byte known to hold a copy of it already, keeping the
// `LDA` when A is read after the pair. The comment at the top of
// redundant_store.cpp gives the rules. `ctx` gives the runtime's
// zero-page layout.
asm_ast::Program apply_redundant_store_elimination(asm_ast::Program prog,
                                                   const AsmContext& ctx);

}  // namespace c6502::passes
