// The port of passes/ret_slot_coalescing.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Returns `prog` with each local pair that holds a copy of a static
// function's zero-page result `F@ret` renamed to `F@ret` where that's
// sound, as the comment at the top of ret_slot_coalescing.cpp
// describes. `slot_symbols` gives each local slot's address, `zp_addrs`
// each zero-page object's, and `eligible` names the functions in the
// private-pool model that aren't recursive and make no
// `save_hardware_stack` call.
asm_ast::Program apply_ret_slot_coalescing(asm_ast::Program prog,
                                           const SymbolAddrs& slot_symbols,
                                           const SymbolAddrs& zp_addrs,
                                           const NameSet& eligible);

}  // namespace c6502::passes
