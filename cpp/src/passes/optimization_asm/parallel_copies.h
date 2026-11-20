// The ordering of a parallel copy, `_order_parallel_copies` of
// passes/optimization_asm/ssa_destruction.py, which SSA destruction
// uses for the moves on an edge and `tac_to_asm` for the arguments of a
// call that go in the callee's slots.
#pragma once

#include <string>
#include <vector>

#include "asm_ast.h"

namespace c6502::passes {

// Returns `movs` ordered so that no move's destination is read by a
// later move. A cycle of moves is broken by copying one destination's
// value into a new Pseudo, `.<fn_name>@asm_cycle_tmp@<N>`, and reading
// the copy in its place. `cycle_counter` counts the temporaries made,
// across calls that share it, as the Python list of one int does: each
// temporary takes the number after it, which is stored back. Operands
// are compared by the storage they name (`storage_key`), so two
// distinct names that the coloring put at one byte count as one.
std::vector<asm_ast::Mov> order_parallel_copies(std::vector<asm_ast::Mov> movs,
                                                const std::string& fn_name, int& cycle_counter);

}  // namespace c6502::passes
