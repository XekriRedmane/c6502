#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Moves each `TXA` / `TYA` that sits between the instruction that set
// the flags from X or Y and a branch to after the branch, in every
// function, when A is dead at the branch's target and live on the
// fall-through.
asm_ast::Program apply_transfer_past_branch(asm_ast::Program prog);

}  // namespace c6502::passes
