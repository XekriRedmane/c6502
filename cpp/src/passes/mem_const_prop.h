#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Follows, within each basic block, the immediate that each `Data` and
// `ZP` byte is known to hold, and replaces a later read of the byte in
// an operand that takes an immediate (`LDA`, `AND`, `ORA`, `ADC`, `SBC`,
// `CMP`) with the immediate. The comment at the top of
// mem_const_prop.cpp gives the rules.
asm_ast::Program apply_mem_const_prop(asm_ast::Program prog);

}  // namespace c6502::passes
