// The port of passes/optimization/distribute_constant.py, which
// multiplies a widened byte before adding a constant to it.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each eligible `(x ± c) * k`, where `x` is an SSA
// ZeroExtend of a byte, rewritten to `x * k ± c * k` (Python's
// `distribute_constant`), or `fn` itself when none is. Each rewrite adds
// a temporary `<dst>.dc<k>` to the symbols and the SSA names. Outside SSA
// form, with no SSA names, or without a symbol table, it leaves `fn`
// alone.
tac_ast::Function distribute_constant(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
