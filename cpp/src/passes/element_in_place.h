#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Rewrites, in every function, the sites that step a slot holding a
// copy of an array element, and the reads of the slot they reach, to
// step and read the element. `zp_slot_symbols` gives the slots'
// addresses. Without it, or with it empty, the program comes back
// unchanged.
asm_ast::Program apply_element_in_place(asm_ast::Program prog, const AsmContext& ctx,
                                        const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
