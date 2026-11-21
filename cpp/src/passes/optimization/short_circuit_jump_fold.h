// The port of passes/optimization/short_circuit_jump_fold.py, the pass
// of the TAC optimizer's fixed point after SSA destruction that turns
// the 0-or-1 tail of `&&` and `||` and the jump that tests it into
// direct conditional jumps, and then merges two equality jumps one bit
// apart.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with every short-circuit tail and the `JumpIfTrue` or
// `JumpIfFalse` that reads it folded into the chain's jumps, labels
// added as `.<fn>@scfold@<N>` where needed (Python's
// `fold_short_circuit_jump`). The symbols aren't read.
tac_ast::Function fold_short_circuit_jump(tac_ast::Function fn);

// Returns `fn` with each pair of adjacent jumps to one target on
// `x == a` and `x == b`, where `a` and `b` differ in one bit `p`,
// replaced by one jump on `(x & ~p) == min(a, b)`, through a new
// temporary `<x>.eqm<k>` added to `symbols` (Python's
// `merge_equality_jumps`). A no-op without a symbol table.
tac_ast::Function merge_equality_jumps(tac_ast::Function fn, SymbolTable* symbols);

// Runs the two in that order, as the driver's `FoldShortCircuitJump`
// does.
tac_ast::Function fold_short_circuit_jumps(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
