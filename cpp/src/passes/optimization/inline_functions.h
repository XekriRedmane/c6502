#pragma once

#include "c99_ast.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Inlines the calls of small `static` functions and returns the new
// program, adding a symbol for each renamed local to `symbols`. `prog` is
// the type-checked c99 program, which holds the attributes of each
// declaration. A program with nothing to inline comes back unchanged.
tac_ast::Program inline_functions(tac_ast::Program tac, SymbolTable& symbols,
                                  const c99_ast::Program& prog);

// Returns the size measure of `fn`'s body that the choice to inline its
// calls multiplies by the bytes per unit. It adds, for each instruction
// other than a `Label` and a `Ret` that ends the body, the width in
// bytes of its widest operand, or 1 for an instruction without one,
// such as a `Jump`. A call counts as 3, a `JSR`.
int inline_cost(const tac_ast::Function& fn, const SymbolTable& symbols);

}  // namespace c6502::passes
