// The asm CFG of passes/optimization_asm/cfg.py with blocks that own
// their instructions, for the passes that edit blocks in place (SSA
// construction and destruction).
#pragma once

#include <vector>

#include "asm_ast.h"
#include "passes/optimization_asm/cfg.h"

namespace c6502::passes {

// The CFG of a function whose blocks hold copies of their
// instructions, as the Python CFG's do. `graph` gives the blocks'
// ids, edges, and source order, which the dominator functions of
// cfg.h read, and `code[id]` is block `id`'s instructions, which a pass
// may change. `graph`'s instruction ranges and `graph.instructions`
// refer to nothing and aren't to be used. The sentinels' code is
// empty.
struct OwnedCfg {
  Cfg graph;
  std::vector<std::vector<asm_ast::Type_instruction>> code;
};

// Partitions `fn`'s instructions into basic blocks with the rules of
// `build_cfg`, and gives each block a copy of its instructions. Throws
// `InternalError` for a `Jump` or `Branch` to a label the function
// doesn't have, as `build_cfg` does.
OwnedCfg build_owned_cfg(const asm_ast::Function& fn);

// Returns the instructions of the blocks of `cfg.graph.block_order` in
// that order, with the name, linkage, and parameters of `fn`, as
// Python's `cfg_to_function` does.
asm_ast::Function owned_cfg_to_function(const asm_ast::Function& fn, const OwnedCfg& cfg);

}  // namespace c6502::passes
