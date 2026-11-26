#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with each `Binary(Add | Subtract, X, Constant, Y)` moved
// to just after X's last use within its block segment, and then each
// equality jump on X right after such a step, X's last use, rewritten
// as a `JumpIfCmp` of Y against the moved constant. An X that SSA
// didn't rename stays where it is.
// Without SSA names (`ctx.ssa_dsts` null) every X may move, the scan
// stops at every store, and the jumps are left alone.
tac_ast::Function sink_increments(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
