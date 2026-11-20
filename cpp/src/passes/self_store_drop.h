// The port of passes/self_store_drop.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops the `STA M` of each adjacent `LDA M; STA M` in the program's
// functions, and each move of a `Data` or `ZP` byte to itself, when
// neither move is volatile. The comment at the top of
// self_store_drop.cpp gives the reasons.
asm_ast::Program apply_self_store_drop(asm_ast::Program prog);

}  // namespace c6502::passes
