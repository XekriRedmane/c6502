// The port of passes/optimization/static_const_fold.py, which replaces
// each read of a `const` scalar static with its value.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Var` operand that an instruction reads and
// that names a foldable `const` scalar static replaced by a `Constant`
// of its value (Python's `fold_static_const_reads`), or `fn` itself
// when none is read. The rules are in the module comment of the .cpp.
// Without a symbol table nothing is folded.
tac_ast::Function fold_static_const_reads(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
