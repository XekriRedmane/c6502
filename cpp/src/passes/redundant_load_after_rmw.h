#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops each `LDA M` of the program's functions that comes right after
// an `INC`, `DEC`, `ASL`, `LSR`, `ROL`, or `ROR` of M, when A is dead
// after it, since the read-modify-write already set N and Z from M's
// new value. The comment at the top of redundant_load_after_rmw.cpp
// gives the shapes and the reasons. The statics come back unchanged.
asm_ast::Program apply_redundant_load_after_rmw(asm_ast::Program prog);

}  // namespace c6502::passes
