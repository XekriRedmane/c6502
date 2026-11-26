#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Promotes one loop counter per function of the program to X, with
// `STX` / `LDX` around the calls the counter lives across and the
// transient `LDX` loads of other values moved to Y. `zp_symbol_addrs`,
// which may be null, gives the addresses of the slot and zero-page
// symbols, so that a byte of a `(zp),Y` pointer, which the pointer's
// reads name by address, is never taken for a counter. The comment at
// the top of loop_counter_to_x.cpp gives the rules.
asm_ast::Program apply_loop_counter_to_x(
    asm_ast::Program prog, const SymbolAddrs* zp_symbol_addrs = nullptr);

}  // namespace c6502::passes
