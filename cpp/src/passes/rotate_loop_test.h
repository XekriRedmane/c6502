// The port of passes/rotate_loop_test.py: a loop's test moved from its
// top to its bottom.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Moves each eligible loop's test to its bottom, inverted to branch
// back to the body, in every function of `prog`. The comment at the
// top of rotate_loop_test.cpp gives the shape and the conditions.
asm_ast::Program apply_rotate_loop_test(asm_ast::Program prog, const AsmContext& ctx);

}  // namespace c6502::passes
