#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops the `LDA M` of each `DEC M` or `INC M`, `LDA M`, and branch on
// N or Z in the program's functions, when A is dead after the branch.
asm_ast::Program apply_dec_inc_branch_fold(asm_ast::Program prog);

}  // namespace c6502::passes
