// The port of passes/optimization_asm/interference.py, which builds the
// interference graph of an asm function in SSA form for the register
// allocator.
#pragma once

#include "asm_ast.h"
#include "passes/asm_context.h"
#include "passes/optimization/interference.h"
#include "passes/optimization_asm/liveness.h"

namespace c6502::passes {

// Returns the interference graph of `fn` from its `liveness`, leaving
// out the names in `statics`, the parameters, the names whose address
// is taken, and the names a `LoadAddress` takes the address of. Most
// nodes are 1 byte wide. A `LoadAddress` destination is a node of
// width 2, and a name read or written at several byte offsets a node as
// wide as its highest offset plus one, since the allocator has to find
// contiguous bytes for each.
InterferenceGraph build_interference(const asm_ast::Function& fn, const Liveness& liveness,
                                     const NameSet& statics = {});

}  // namespace c6502::passes
