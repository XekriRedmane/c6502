// The port of passes/optimization_asm/cfg.py: the basic blocks of an
// asm function, the edges between them, and its dominators.
#pragma once

#include <cstddef>
#include <set>
#include <span>
#include <vector>

#include "asm_ast.h"

namespace c6502::passes {

// The ids of the two sentinel blocks.
inline constexpr int kEntryId = 0;
inline constexpr int kExitId = 1;

// A basic block: the instructions `begin` to `end` (half-open) of its
// CFG's instruction list, and the ids of its predecessors and
// successors in the order the edges were added.
struct BasicBlock {
  int id = 0;
  std::size_t begin = 0;
  std::size_t end = 0;
  std::vector<int> predecessors;
  std::vector<int> successors;
};

// The CFG of a function. `blocks[id]` is the block with that id: the
// two sentinels, then one block per partition numbered from 2 in source
// order, which is also `block_order`. The Python CFG copies each
// block's instructions. This one keeps index ranges into the
// function's instructions, which the caller has to keep alive and
// unchanged while it uses the CFG, followed by `extra`, the
// instructions `build_cfg_tolerant` adds.
struct Cfg {
  std::vector<BasicBlock> blocks;
  std::vector<int> block_order;
  std::span<const asm_ast::Type_instruction> function_instructions;
  std::vector<asm_ast::Type_instruction> extra;

  // Returns the instruction at index `i` of the CFG's instruction list.
  const asm_ast::Type_instruction& instruction(std::size_t i) const;
  // Returns the instructions of block `id`.
  std::span<const asm_ast::Type_instruction> instructions(int id) const;
};

// Partitions the instructions into basic blocks and adds the edges
// from ENTRY, between the blocks, and to EXIT, as the comment at the
// top of cfg.cpp describes. Throws `InternalError` for a `Jump` or
// `Branch` to a label the function doesn't have, where Python raises a
// `KeyError`.
Cfg build_cfg(const asm_ast::Function& fn);
Cfg build_cfg(std::span<const asm_ast::Type_instruction> instrs);
// Builds the CFG of `instrs` followed by `extra`, which the CFG keeps.
Cfg build_cfg(std::span<const asm_ast::Type_instruction> instrs,
              std::vector<asm_ast::Type_instruction> extra);

// Returns the instructions of `fn` laid out in `cfg.block_order`, with
// the name, linkage, and parameters of `fn`.
asm_ast::Function cfg_to_function(const asm_ast::Function& fn, const Cfg& cfg);

// Returns the blocks reachable from ENTRY in reverse postorder of a
// depth-first walk that takes each block's successors in order.
std::vector<int> reverse_postorder(const Cfg& cfg);

// The immediate dominator of each block reachable from ENTRY, with
// ENTRY its own (Cooper, Harvey, and Kennedy, 2006). `idom[b]` is -1
// for a block that isn't reachable. `order` lists the blocks in the
// order the Python dict gains them, which `dominator_tree_children`
// follows.
struct Dominators {
  std::vector<int> idom;
  std::vector<int> order;
};
Dominators immediate_dominators(const Cfg& cfg);

// Returns the children of each block in the dominator tree, by block
// id, in the order Python's inversion of the `idom` dict gives them.
std::vector<std::vector<int>> dominator_tree_children(const Dominators& doms);

// Returns the dominance frontier of each block reachable from ENTRY,
// by block id, by the walk of Cytron et al.: for each block X with
// several predecessors, each predecessor P adds X to the frontier of P,
// idom[P], idom[idom[P]], and so on up to idom[X].
std::vector<std::set<int>> dominance_frontiers(const Cfg& cfg);

}  // namespace c6502::passes
