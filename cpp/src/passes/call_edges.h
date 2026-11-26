#pragma once

#include <vector>

#include "tac_ast.h"

namespace c6502::passes {

// Returns whether `instr` is a direct call to a `[[noreturn]]` function
// that passes no arguments, which isn't an edge of the call graph.
bool abandons_caller(const tac_ast::Type_instruction& instr);

// Returns the direct calls of `fn` that are edges of the call graph,
// every `FunctionCall` but those that abandon `fn`, in order. The
// pointers are into `fn`, which has to outlive them.
std::vector<const tac_ast::FunctionCall*> graph_calls(const tac_ast::Function& fn);

}  // namespace c6502::passes
