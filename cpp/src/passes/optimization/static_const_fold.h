#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Var` operand that an instruction reads and
// that names a foldable `const` scalar static replaced by a `Constant`
// of its value, or `fn` itself when none is read. The value is the
// integer of the static's `Initial`, which for a floating type holds
// its IEEE 754 bits, and a static whose `Initial` isn't an integer
// isn't folded. Without a symbol table nothing is folded.
tac_ast::Function fold_static_const_reads(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
