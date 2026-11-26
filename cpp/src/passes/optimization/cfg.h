#pragma once

#include <utility>
#include <vector>

#include "ordered_map.h"
#include "py_int_set.h"
#include "tac_ast.h"

namespace c6502::passes::tac_cfg {

// The ids of the two sentinel blocks. Real blocks number from 2.
inline constexpr int kEntryId = 0;
inline constexpr int kExitId = 1;

// A basic block, which owns a copy of its instructions, so that a pass
// may edit them before flattening the graph back into a function.
struct BasicBlock {
  int id = 0;
  std::vector<tac_ast::Type_instruction> instructions;
  std::vector<int> predecessors;
  std::vector<int> successors;
  bool operator==(const BasicBlock&) const = default;
};

// The graph. `blocks` holds ENTRY, EXIT, and the real blocks, and
// `block_order` the real blocks in source order.
struct Cfg {
  OrderedMap<int, BasicBlock> blocks;
  std::vector<int> block_order;
  bool operator==(const Cfg&) const = default;
};

// Returns whether `instr` ends a block: a `Ret`, a `Jump`, or a
// conditional jump.
bool is_terminator(const tac_ast::Type_instruction& instr);

// Partitions `fn`'s instructions into basic blocks and wires the entry,
// the exit, and the edges between blocks. Throws `InternalError` for a
// jump to a label no block starts with.
Cfg build_cfg(const tac_ast::Function& fn);

// Flattens `cfg` back into a function with `fn`'s name, linkage, and
// parameters, each real block's instructions in `block_order`. A block
// removed from `cfg.blocks` or from `block_order` drops out.
tac_ast::Function cfg_to_function(const tac_ast::Function& fn, const Cfg& cfg);

// Returns the reverse of a postorder of the blocks reachable from
// ENTRY, found by a depth-first walk that takes successors in order.
std::vector<int> reverse_postorder(const Cfg& cfg);

// Returns each reachable block's immediate dominator, with ENTRY mapped
// to itself.
OrderedMap<int, int> immediate_dominators(const Cfg& cfg);

// Returns each block of `idom` mapped to its children in the dominator
// tree, leaving out ENTRY's self-edge.
OrderedMap<int, std::vector<int>> dominator_tree_children(const OrderedMap<int, int>& idom);

// Returns the dominance frontier of every reachable block.
OrderedMap<int, PyIntSet> dominance_frontiers(const Cfg& cfg);

// Returns one `(header, body)` pair per natural loop, with the loops
// sharing a header merged.
std::vector<std::pair<int, PyIntSet>> natural_loops(const Cfg& cfg);

}  // namespace c6502::passes::tac_cfg
