#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every short-circuit tail and the `JumpIfTrue` or
// `JumpIfFalse` that reads it folded into the chain's jumps, labels
// added as `.<fn>@scfold@<N>` where needed. The symbols aren't read.
tac_ast::Function fold_short_circuit_jump(tac_ast::Function fn);

// Returns `fn` with each pair of adjacent jumps to one target on
// `x == a` and `x == b`, where `a` and `b` differ in one bit `p`,
// replaced by one jump on `(x & ~p) == min(a, b)`, through a new
// temporary `<x>.eqm<k>` added to `symbols`. A no-op without a symbol
// table.
tac_ast::Function merge_equality_jumps(tac_ast::Function fn, SymbolTable* symbols);

// Runs `fold_short_circuit_jump` and then `merge_equality_jumps`.
tac_ast::Function fold_short_circuit_jumps(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
