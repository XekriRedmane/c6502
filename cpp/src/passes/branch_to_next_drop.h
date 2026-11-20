// The port of passes/branch_to_next_drop.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Drops each `Branch` of the program's functions whose target is the
// label right after it, each `Branch` right after a `Branch` on the
// same condition, and each `Jump` to one of the labels right after it,
// as the comment at the top of branch_to_next_drop.cpp describes.
asm_ast::Program apply_branch_to_next_drop(asm_ast::Program prog);

}  // namespace c6502::passes
