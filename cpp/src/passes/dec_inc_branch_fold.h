// The port of passes/dec_inc_branch_fold.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops the `LDA M` of each `DEC M` or `INC M`, `LDA M`, and branch on
// N or Z in the program's functions, when A is dead after the branch.
// The comment at the top of dec_inc_branch_fold.cpp gives the reasons.
asm_ast::Program apply_dec_inc_branch_fold(asm_ast::Program prog);

}  // namespace c6502::passes
