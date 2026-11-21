// The port of passes/optimization/ssa_construction.py, which puts a TAC
// function in SSA form for the TAC optimizer's driver.
#pragma once

#include <utility>

#include "passes/asm_context.h"
#include "passes/type_checking.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` in SSA form and the names whose only definition this
// call made (`ssa_dsts`), which the passes in SSA form may treat as
// values with one definition that live only in the function. Each new
// name `<orig>.<N>` gets a `LocalAttr` symbol of the original's type in
// `symbols`. The names include each promotable parameter's own
// spelling, its value at the function's entry, and leave out statics,
// address-taken locals, and aggregates, which keep their names.
std::pair<tac_ast::Function, NameSet> to_ssa(tac_ast::Function fn, SymbolTable& symbols);

}  // namespace c6502::passes
