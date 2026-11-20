// The port of passes/branch_thread.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Retargets each branch of the program's functions whose target is a
// branch on the same flag, as the comment at the top of
// branch_thread.cpp describes. The labels it adds are `.thread@N`,
// numbered across the program from one more than the largest already
// there.
asm_ast::Program apply_branch_thread(asm_ast::Program prog);

}  // namespace c6502::passes
