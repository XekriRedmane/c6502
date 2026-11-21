// passes/optimization_ast/unroll.py: the full unrolling of the for
// loops marked `[[c6502::unroll]]`, which runs under -O between the
// parser and identifier_resolution.
#pragma once

#include "c99_ast.h"

namespace c6502::passes {

// The most passes of a loop the unroller writes out.
inline constexpr int kMaxUnrollIterations = 256;

// Replaces each for loop of the program's function bodies whose
// `loop_attribute` is "unroll" with a compound of one copy of its body
// per pass, each in a scope of its own, with the induction variable
// replaced by its value in that pass, and returns the program, which it
// takes by value. Throws `CompileError` of type `UnrollError` for a
// marked loop that doesn't have the shape the unroller takes, that never
// ends, or that runs more than `kMaxUnrollIterations` passes.
c99_ast::Program unroll_program(c99_ast::Program prog);

}  // namespace c6502::passes
