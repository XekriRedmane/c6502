// The port of passes/branch_value_thread.py.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"

namespace c6502::passes {

// Retargets each branch and `JMP` of the program's functions whose
// target's tests are decided on its edge, as the comment at the top of
// branch_value_thread.cpp describes. The labels it adds are
// `.vthread@N`, numbered across the program from one more than the
// largest already there. `ctx` is the compile's context, which
// `may_alias` reads.
asm_ast::Program apply_branch_value_thread(asm_ast::Program prog,
                                           const AsmContext& ctx);

}  // namespace c6502::passes
