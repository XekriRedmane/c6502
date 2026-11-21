// The port of passes/optimization/optimizer.py, the TAC optimizer: the
// table of its driver's passes and the functions that run it on a
// function and on a program.
#pragma once

#include "passes/optimization/driver.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns the driver with the passes of Python's `_DRIVER`, in its
// lists and its order. A pass whose port hasn't landed has a null
// `run`, and the groups that port the passes fill them in.
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
