// The port of passes/constant_store_order.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Groups each run of stores of the program's functions by the value
// stored, so that each value is loaded once, when the run's loads and
// stores allow it. The comment at the top of constant_store_order.cpp
// gives the conditions. The statics come back unchanged.
asm_ast::Program apply_constant_store_order(asm_ast::Program prog);

}  // namespace c6502::passes
