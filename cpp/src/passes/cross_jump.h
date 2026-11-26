#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Shares, in each function of the program, the longest repeated run of
// instructions before two jumps to the same place, until no such pair
// is left. The labels it adds are `.xjump@N`, numbered across the
// program from one more than the largest already there.
asm_ast::Program apply_cross_jump(asm_ast::Program prog);

}  // namespace c6502::passes
