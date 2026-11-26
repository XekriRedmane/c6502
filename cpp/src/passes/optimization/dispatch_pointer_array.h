#pragma once

#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Rewrites every eligible `arr[i][j]` read through a small table `arr`
// of `const` pointers, in each function of `prog`, as a
// compare-and-branch dispatch on `i`, and returns the program. `symbols`
// gives each table's c99 type, which must have `const` elements, and
// without a table (null) no table qualifies.
tac_ast::Program dispatch_const_pointer_arrays(tac_ast::Program prog, const SymbolTable* symbols);

}  // namespace c6502::passes
