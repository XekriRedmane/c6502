#pragma once

#include "passes/optimization/driver.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns the TAC optimizer's driver, with its lists of passes in the
// order they run.
const framework::PhaseDriver& tac_driver();

// Returns `fn` after the driver: SSA construction, the fixed point, and
// SSA destruction. Without a symbol table (null) the SSA conversion is
// skipped, since the renaming needs the table to give the new names
// their types, and the passes that work in SSA form do nothing.
tac_ast::Function optimize_function(tac_ast::Function fn, SymbolTable* symbols);

// Returns `prog` with `optimize_function` of each `Function`. The
// `StaticVariable`s pass through unchanged.
tac_ast::Program optimize_program(tac_ast::Program prog, SymbolTable* symbols);

}  // namespace c6502::passes
