// The port of passes/promote_loop_carried_to_a.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Moves the body-local slot that a function saves an A-resident
// parameter into at entry into A for the length of the loop that
// steps it, when the shape in the comment at the top of
// promote_loop_carried_to_a.cpp matches. A function that doesn't match
// comes back unchanged.
asm_ast::Program apply_promote_loop_carried_to_a(asm_ast::Program prog);

}  // namespace c6502::passes
