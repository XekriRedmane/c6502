#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Merges the repeated return tails of the program's functions into one
// copy that branches reach. A conditional branch over a tail of 2 to 4
// instructions ending in a `Return` becomes a branch on the opposite
// condition to the function's last copy of that tail, and a branch or
// jump to a label in front of a tail goes to that copy too. A branch is
// rewritten only when its new target is estimated to be in reach. The
// labels it adds are `.tail@N`, with N counted from 0 in each call and
// skipping the names of labels already in the program.
asm_ast::Program apply_tail_merge(asm_ast::Program prog);

}  // namespace c6502::passes
