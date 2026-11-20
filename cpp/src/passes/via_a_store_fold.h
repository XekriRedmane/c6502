// The part of passes/via_a_store_fold.py that the peephole fixed point
// runs, `apply_via_a_store_fold`. `apply_copy_through_index`, which
// runs once after the fixed points, isn't ported yet.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Folds each `TXA; STA M` into `STX M`, and each `TYA; STA M` into `STY
// M`, in every function, when M is a `Data` or `ZP` byte and A and N/Z
// are dead after the store. The comment at the top of
// via_a_store_fold.cpp gives the reasons.
asm_ast::Program apply_via_a_store_fold(asm_ast::Program prog);

}  // namespace c6502::passes
