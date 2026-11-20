// The port of passes/dual_index_promotion.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Promotes, in each function that leaves Y unused, a `Data` byte that
// two or more `LDX` instructions load into Y, loaded once at the
// function's entry, and turns the `abs,X` reads of it into `abs,Y`. At
// most one byte per function is promoted. The comment at the top of
// dual_index_promotion.cpp gives the conditions. The statics and every
// function the pass leaves alone come back unchanged.
asm_ast::Program apply_dual_index_promotion(asm_ast::Program prog);

}  // namespace c6502::passes
