#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Retargets each `Branch(cond, L)` or `Jump(L)` of the program's
// functions whose label `L` is followed by an unconditional `Jump(T)`
// to a label of the function, and drops the `L: Jump(T)` trampoline
// when nothing else reaches it.
asm_ast::Program apply_branch_through_jump(asm_ast::Program prog);

}  // namespace c6502::passes
