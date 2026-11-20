// The port of passes/long_branches.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Returns the program with each branch that can't reach its target
// rewritten as a branch on the opposite condition over a `JMP` to the
// target (Python's `expand_program`, which compile.py imports as
// `expand_long_branches`). Throws `InternalError` for a branch to a
// label outside its function, where Python raises a `ValueError`.
asm_ast::Program expand_long_branches(asm_ast::Program prog);

namespace long_branches {

// Returns the condition a branch tests for when it isn't taken: BCC
// for BCS, BEQ for BNE, and so on. Python's `_invert`.
asm_ast::Type_condition invert(const asm_ast::Type_condition& cond);

}  // namespace long_branches

}  // namespace c6502::passes
