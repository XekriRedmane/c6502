#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites each `LDA M` of a function to `TXA`, and each `Mov(M, D)`
// into a `Data` or `ZP` byte to `STX D`, when M is the one memory slot
// the function stores X into and X holds M's value on every path to
// the read. The statics and every function the pass leaves alone
// come back unchanged.
asm_ast::Program apply_x_save_slot_load(asm_ast::Program prog);

}  // namespace c6502::passes
