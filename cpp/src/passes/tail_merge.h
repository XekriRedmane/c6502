// The port of passes/tail_merge.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Merges the repeated return tails of the program's functions into one
// copy that branches reach, as the comment at the top of tail_merge.cpp
// describes. The labels it adds are `.tail@N`, with N counted from 0 in
// each call and skipping the names of labels already in the program.
asm_ast::Program apply_tail_merge(asm_ast::Program prog);

}  // namespace c6502::passes
