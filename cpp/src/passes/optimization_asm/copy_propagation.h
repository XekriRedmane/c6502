#pragma once

#include "asm_ast.h"
#include "passes/optimization_asm/ssa_construction.h"

namespace c6502::passes {

// Returns `fn`, in asm SSA form, with every use of the destination of a
// copy `Mov(src, Pseudo dst)` replaced by the copy's source, followed
// through chains of copies. Only an immediate, a label's half, or a
// Pseudo that isn't excluded (`excluded_pseudo_names`) or in `statics`
// counts as a source.
asm_ast::Function copy_propagate(asm_ast::Function fn, const StaticNames& statics);

}  // namespace c6502::passes
