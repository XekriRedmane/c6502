#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Drops the high byte's add and shift of each halved 9-bit sum in every
// function, so the `ROR` takes bit 8 from the carry of the low byte's
// add. `zp_slot_symbols`, which may be null, is passed to
// `mem_dead_after`. docs/optimizations/carry_halve.md gives the
// pattern and the conditions.
asm_ast::Program apply_carry_halve(asm_ast::Program prog, const AsmContext& ctx,
                                   const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
