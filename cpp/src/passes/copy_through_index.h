#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Copies each byte that A is parked for through X or Y instead, in
// every function of the program. `zp_slot_symbols` gives the slots'
// addresses for `mem_dead_after`, and may be null.
asm_ast::Program apply_copy_through_index(asm_ast::Program prog,
                                          const AsmContext& ctx,
                                          const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
