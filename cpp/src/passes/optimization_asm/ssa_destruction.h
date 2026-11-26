#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Returns `fn` with every `Phi` lowered to a `Mov` on each edge into its
// block, after splitting each edge from a block that ends in a `Branch`
// to a block with `Phi`s, so that no move sits between the instruction
// that sets the flags and the branch that reads them. The comment at
// the top of ssa_destruction.cpp gives the rules.
asm_ast::Function from_ssa(const asm_ast::Function& fn);

}  // namespace c6502::passes
