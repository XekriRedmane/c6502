#pragma once

#include "asm_ast.h"
#include "passes/optimization_asm/ssa_construction.h"

namespace c6502::passes {

// Returns `fn` with each Pseudo P whose one use copies it into memory D
// (`Mov(P, A); Mov(A, D)`, or `Mov(P, D)`) written straight into D by
// its definition `Mov(A, P)`, and the copy dropped, repeated until
// nothing changes. The comment at the top of
// backward_copy_propagation.cpp gives the conditions.
asm_ast::Function backward_copy_propagate(asm_ast::Function fn, const StaticNames& statics);

}  // namespace c6502::passes
