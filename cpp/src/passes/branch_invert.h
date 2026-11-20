// The port of passes/branch_invert.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Collapses each `Branch(cond, L); Jump(T); Label(L)` of the program's
// functions, with `T` a label of the function, into `Branch(!cond, T);
// Label(L)`.
asm_ast::Program apply_branch_invert(asm_ast::Program prog);

}  // namespace c6502::passes
