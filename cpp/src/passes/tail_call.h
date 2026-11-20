// The port of passes/tail_call.py.
#pragma once

#include "asm_ast.h"

namespace c6502::passes {

// Rewrites each `Call(name); Return(_)` of the program's functions as
// `Jump(name)`, and each `Jump` to a label followed by a bare `Return`
// as a copy of that `Return`.
asm_ast::Program apply_tail_call(asm_ast::Program prog);

}  // namespace c6502::passes
