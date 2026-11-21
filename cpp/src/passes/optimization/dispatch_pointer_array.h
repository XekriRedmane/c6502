// The port of passes/optimization/dispatch_pointer_array.py, which
// turns a read through a small table of `const` pointers into a
// compare-and-branch dispatch on the index.
#pragma once

#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Rewrites every eligible `arr[i][j]` chain of each function of `prog`
// (the .cpp's comment) and returns the program. `symbols` gives each
// table's c99 type, which must have `const` elements, and without a
// table (null) no table qualifies.
tac_ast::Program dispatch_const_pointer_arrays(tac_ast::Program prog, const SymbolTable* symbols);

}  // namespace c6502::passes
