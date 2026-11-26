#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Copies `CMP #c; RTS` carry-flag return tails into the paths that load
// a constant into A before reaching them.
asm_ast::Program apply_carry_return_tails(asm_ast::Program prog);

// Folds `LDA #k; CMP #c` to `SEC` / `CLC` when only C is read after it,
// and a compare of a register that holds a known constant the same way.
asm_ast::Program apply_carry_const_fold(asm_ast::Program prog);

// Branches on a captured carry flag directly, and drops a capture that
// is turned straight back into C. `zp_slot_symbols` (slot name to
// address, or null) lets it tell that a stored capture's slot is a
// local that's dead at return.
asm_ast::Program apply_carry_branch_fold(asm_ast::Program prog, const AsmContext& ctx,
                                         const SymbolAddrs* zp_slot_symbols = nullptr);

}  // namespace c6502::passes
