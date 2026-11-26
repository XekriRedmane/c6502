#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Folds each `p + b + 1` pair of 16-bit adds into one add with the
// carry set, in every function. `zp_slot_symbols`, which may be null,
// is passed to `mem_dead_after`. docs/optimizations/carry_in_add.md
// gives the pattern and the conditions.
asm_ast::Program apply_carry_in_add(asm_ast::Program prog, const AsmContext& ctx,
                                    const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
