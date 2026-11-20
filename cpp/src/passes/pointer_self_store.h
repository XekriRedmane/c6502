// The port of passes/pointer_self_store.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Drops each store through a zero-page pointer of the byte read from
// the same place, when nothing in between can have changed it, and
// reads such a byte through the pointer again in place of a copy of it
// that is read once. `zp_symbol_addrs`, which may be null, gives the
// zero-page addresses of the slot symbols and zero-page objects. The
// comment at the top of pointer_self_store.cpp gives the reasons.
asm_ast::Program apply_pointer_self_store_drop(
    asm_ast::Program prog, const AsmContext& ctx,
    const SymbolAddrs* zp_symbol_addrs = nullptr);

}  // namespace c6502::passes
