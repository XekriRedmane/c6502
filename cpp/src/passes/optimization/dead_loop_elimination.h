// The port of passes/optimization/dead_loop_elimination.py, the TAC
// optimizer's elimination of loops that compute nothing read outside
// them.
#pragma once

#include "passes/optimization/framework.h"
#include "tac_ast.h"

namespace c6502::passes {

// Returns `fn` with the header of each natural loop that has no effect
// and whose values nothing outside it reads made a jump past the loop,
// which unreachable-code elimination then prunes (Python's
// `eliminate_dead_loops`), or `fn` itself when no loop qualifies. A
// loop with a label that holds `c99_to_tac::kKeepLoopMark`, the mark of
// `[[c6502::keep]]`, stays. Outside SSA form it leaves `fn` alone.
tac_ast::Function eliminate_dead_loops(tac_ast::Function fn, const framework::PassContext& ctx);

}  // namespace c6502::passes
