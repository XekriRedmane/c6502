// The opposite of each branch condition, from passes/long_branches.py
// (`_invert`), which several asm passes share.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Returns the condition that is true exactly when `cond` is false:
// BCC and BCS, BEQ and BNE, BMI and BPL, and BVC and BVS swap.
asm_ast::Type_condition invert_condition(const asm_ast::Type_condition& cond);

}  // namespace c6502::passes
