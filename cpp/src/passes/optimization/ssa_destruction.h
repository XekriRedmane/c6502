// The port of passes/optimization/ssa_destruction.py, which takes a TAC
// function out of SSA form for the TAC optimizer's driver.
#pragma once

#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every Phi lowered to Copies in its predecessors and
// removed. A cycle of copies on one edge is broken with a new
// temporary, whose symbol goes in `symbols`. Without a symbol table
// (null), such a cycle's copies stay in source order, which can
// miscompile.
tac_ast::Function from_ssa(tac_ast::Function fn, SymbolTable* symbols = nullptr);

}  // namespace c6502::passes
