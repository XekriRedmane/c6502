#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each repeated pure computation or load replaced by a
// Copy from the destination of its first occurrence, or `fn` itself when
// none repeats. Without a symbol table, or outside SSA form, it leaves
// `fn` alone.
tac_ast::Function eliminate_common_subexpressions(tac_ast::Function fn,
                                                  const framework::PassContext& ctx);

}  // namespace c6502::passes
