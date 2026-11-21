// The port of passes/optimization/noreturn_call.py, the pass before SSA
// construction that drops what follows a call to a `[[noreturn]]`
// function.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with a `Ret` of nothing after each call to a function
// whose `FunAttr.noreturn` is set, and without the instructions between
// that call and the next label (Python's `NoreturnCalls`). Without a
// symbol table, and when nothing changes, returns `fn` as it is.
tac_ast::Function noreturn_calls(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
