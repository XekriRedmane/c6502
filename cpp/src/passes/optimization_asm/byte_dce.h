// The port of passes/optimization_asm/byte_dce.py.
#pragma once

#include "asm_ast.h"
#include "passes/optimization_asm/ssa_construction.h"

namespace c6502::passes {

// Returns `fn`, in asm SSA form, without the `Mov`s, `Phi`s, and packed
// BCD chains whose destination Pseudos nothing reads, and without the
// top bytes of a chain that nothing reads, repeated until nothing
// changes. A destination in `statics` or among the excluded names
// (`excluded_pseudo_names`) is never dead.
asm_ast::Function byte_dce(asm_ast::Function fn, const StaticNames& statics);

}  // namespace c6502::passes
