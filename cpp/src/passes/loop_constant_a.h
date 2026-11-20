// The port of passes/loop_constant_a.py: a loop's one constant loaded
// into A once, in front of the loop.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Loads each qualifying loop's constant in A once, before the loop, in
// every function of `prog`, and removes the loop's loads of it and the
// `TXA` / `TYA` transfers whose value nothing reads. The comment at
// the top of loop_constant_a.cpp gives the shape and the conditions.
asm_ast::Program apply_loop_constant_a(asm_ast::Program prog);

}  // namespace c6502::passes
