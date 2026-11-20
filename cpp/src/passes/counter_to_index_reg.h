// The port of passes/counter_to_index_reg.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Moves each pure count-down counter of the program's functions from
// memory into X, or into Y when the loop uses X. `zp_symbol_addrs`,
// which may be null, gives the addresses of the slot and zero-page
// symbols, so that a byte of a `(zp),Y` pointer, which the pointer's
// reads name by address, is never taken for a counter. The comment at
// the top of counter_to_index_reg.cpp gives the rules.
asm_ast::Program apply_counter_to_index_reg(
    asm_ast::Program prog, const SymbolAddrs* zp_symbol_addrs = nullptr);

}  // namespace c6502::passes
