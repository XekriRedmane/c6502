// Control-flow graph for an asm function.
//
// A `Cfg` partitions an asm function's instructions into basic blocks,
// adds the edges between blocks from their terminators, and computes
// immediate dominators and dominance frontiers, which SSA construction
// builds on.
//
// The rules of the partition
//
//   - A block starts at the function's first instruction, at every
//     `Label`, and at the instruction right after a terminator (`Jump`,
//     `Branch`, `Return`, `Ret`).
//   - A block ends after its terminator, when it has one, or at the
//     instruction before the next block starts (a fall-through).
//   - `Call` and `AllocateStack` aren't terminators, since control comes
//     back to the next instruction. Their effects on memory (a call
//     clobbers caller-saved zero page, and `AllocateStack` moves SSP) are
//     the business of the passes that use the CFG, which sees them as
//     ordinary instructions in a block.
//
// The successors of each block
//
//   - A block ending in `Return` / `Ret` goes to EXIT.
//   - A block ending in `Jump(L)` goes to the block whose first
//     instruction is `Label(L)`.
//   - A block ending in `Branch(_, L)` goes to the block of `L` (taken)
//     and then to the next block in source order (fall-through).
//   - Any other block goes to the next block in source order.
//   - The last block in source order, when it has no terminator, goes to
//     EXIT. `tac_to_asm` always ends a function with a `Return` / `Ret`,
//     so this only comes up in tests or after a pass drops the exit.
//   - ENTRY goes to the first block, or to EXIT for a function with no
//     instructions.
//
// A block that ENTRY doesn't reach keeps its instructions and its
// edges out. A pass that drops such blocks walks forward from ENTRY and
// drops what it doesn't visit.
//
// `block_order` lists the ids of the blocks in source order, and
// `cfg_to_function` lays the blocks out in that order to give a function
// back.
#include "passes/optimization_asm/cfg.h"

#include <algorithm>
#include <tuple>
#include <unordered_map>

#include "errors.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::asm_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

std::size_t total_size(const Cfg& cfg) {
  return cfg.function_instructions.size() + cfg.extra.size();
}

// Fills in `cfg.blocks` and `cfg.block_order` from the instruction list
// that `cfg.function_instructions` and `cfg.extra` make.
void partition_and_link(Cfg& cfg) {
  cfg.blocks.clear();
  cfg.blocks.push_back(BasicBlock{.id = kEntryId});
  cfg.blocks.push_back(BasicBlock{.id = kExitId});
  cfg.block_order.clear();
  const std::size_t n = total_size(cfg);
  // The blocks of the synthetic instructions never take in an
  // instruction of the function's, since each starts with a `Label`.
  std::size_t start = 0;
  const auto finalize = [&](std::size_t end) {
    if (end == start) {
      return;
    }
    const int id = static_cast<int>(cfg.blocks.size());
    cfg.blocks.push_back(BasicBlock{.id = id, .begin = start, .end = end});
    cfg.block_order.push_back(id);
    start = end;
  };
  for (std::size_t i = 0; i < n; ++i) {
    const ir::Type_instruction& instr = cfg.instruction(i);
    if (isa<ir::Label>(instr)) {
      finalize(i);
    } else if (isa<ir::Jump, ir::Branch, ir::Return, ir::Ret>(instr)) {
      finalize(i + 1);
    }
  }
  finalize(n);

  std::unordered_map<std::string, int> label_to_block;
  for (const int id : cfg.block_order) {
    if (const auto* label = as<ir::Label>(cfg.instruction(cfg.blocks[id].begin))) {
      label_to_block[label->name] = id;
    }
  }
  const auto add_edge = [&](int src, int dst) {
    cfg.blocks[src].successors.push_back(dst);
    cfg.blocks[dst].predecessors.push_back(src);
  };
  const auto block_of = [&](const std::string& target) {
    const auto it = label_to_block.find(target);
    if (it == label_to_block.end()) {
      throw InternalError("build_cfg: a jump or branch to " + target +
                          ", which no instruction of the function labels");
    }
    return it->second;
  };
  if (!cfg.block_order.empty()) {
    add_edge(kEntryId, cfg.block_order[0]);
  } else {
    add_edge(kEntryId, kExitId);
  }
  for (std::size_t i = 0; i < cfg.block_order.size(); ++i) {
    const int id = cfg.block_order[i];
    const ir::Type_instruction& last = cfg.instruction(cfg.blocks[id].end - 1);
    const int next =
        i + 1 < cfg.block_order.size() ? cfg.block_order[i + 1] : kExitId;
    if (isa<ir::Return, ir::Ret>(last)) {
      add_edge(id, kExitId);
    } else if (const auto* jump = as<ir::Jump>(last)) {
      add_edge(id, block_of(jump->target));
    } else if (const auto* branch = as<ir::Branch>(last)) {
      add_edge(id, block_of(branch->target));
      add_edge(id, next);
    } else {
      add_edge(id, next);
    }
  }
}

}  // namespace

const ir::Type_instruction& Cfg::instruction(std::size_t i) const {
  if (i < function_instructions.size()) {
    return function_instructions[i];
  }
  return extra[i - function_instructions.size()];
}

std::span<const ir::Type_instruction> Cfg::instructions(int id) const {
  const BasicBlock& b = blocks[id];
  if (b.begin >= function_instructions.size()) {
    const std::size_t k = function_instructions.size();
    return std::span<const ir::Type_instruction>(extra).subspan(b.begin - k,
                                                                 b.end - b.begin);
  }
  return function_instructions.subspan(b.begin, b.end - b.begin);
}

Cfg build_cfg(std::span<const ir::Type_instruction> instrs) {
  Cfg cfg;
  cfg.function_instructions = instrs;
  partition_and_link(cfg);
  return cfg;
}

Cfg build_cfg(std::span<const ir::Type_instruction> instrs,
              std::vector<ir::Type_instruction> extra) {
  Cfg cfg;
  cfg.function_instructions = instrs;
  cfg.extra = std::move(extra);
  partition_and_link(cfg);
  return cfg;
}

Cfg build_cfg(const ir::Function& fn) { return build_cfg(fn.instructions); }

ir::Function cfg_to_function(const ir::Function& fn, const Cfg& cfg) {
  ir::Function out{.name = fn.name, .is_global = fn.is_global, .params = fn.params};
  for (const int id : cfg.block_order) {
    if (id < static_cast<int>(cfg.blocks.size())) {
      for (const auto& instr : cfg.instructions(id)) {
        out.instructions.push_back(instr);
      }
    }
  }
  return out;
}

std::vector<int> reverse_postorder(const Cfg& cfg) {
  std::vector<bool> visited(cfg.blocks.size(), false);
  visited[kEntryId] = true;
  std::vector<int> postorder;
  std::vector<std::pair<int, std::size_t>> stack{{kEntryId, 0}};
  while (!stack.empty()) {
    auto& [id, i] = stack.back();
    const auto& succs = cfg.blocks[id].successors;
    if (i >= succs.size()) {
      postorder.push_back(id);
      stack.pop_back();
      continue;
    }
    const int next = succs[i++];
    if (!visited[next]) {
      visited[next] = true;
      stack.emplace_back(next, 0);
    }
  }
  std::reverse(postorder.begin(), postorder.end());
  return postorder;
}

Dominators immediate_dominators(const Cfg& cfg) {
  const std::vector<int> rpo = reverse_postorder(cfg);
  std::vector<int> rpo_index(cfg.blocks.size(), -1);
  for (std::size_t i = 0; i < rpo.size(); ++i) {
    rpo_index[rpo[i]] = static_cast<int>(i);
  }
  Dominators doms;
  doms.idom.assign(cfg.blocks.size(), -1);
  doms.idom[kEntryId] = kEntryId;
  doms.order.push_back(kEntryId);
  const auto intersect = [&](int a, int b) {
    while (a != b) {
      while (rpo_index[a] > rpo_index[b]) {
        a = doms.idom[a];
      }
      while (rpo_index[b] > rpo_index[a]) {
        b = doms.idom[b];
      }
    }
    return a;
  };
  bool changed = true;
  while (changed) {
    changed = false;
    for (const int b : rpo) {
      if (b == kEntryId) {
        continue;
      }
      int new_idom = -1;
      for (const int p : cfg.blocks[b].predecessors) {
        if (rpo_index[p] < 0 || doms.idom[p] < 0) {
          continue;
        }
        new_idom = new_idom < 0 ? p : intersect(p, new_idom);
      }
      if (new_idom < 0) {
        continue;
      }
      if (doms.idom[b] != new_idom) {
        if (doms.idom[b] < 0) {
          doms.order.push_back(b);
        }
        doms.idom[b] = new_idom;
        changed = true;
      }
    }
  }
  return doms;
}

std::vector<std::vector<int>> dominator_tree_children(const Dominators& doms) {
  std::vector<std::vector<int>> children(doms.idom.size());
  for (const int b : doms.order) {
    const int p = doms.idom[b];
    if (b != p) {
      children[p].push_back(b);
    }
  }
  return children;
}

std::vector<std::set<int>> dominance_frontiers(const Cfg& cfg) {
  const Dominators doms = immediate_dominators(cfg);
  std::vector<std::set<int>> df(cfg.blocks.size());
  for (const int b : doms.order) {
    if (b == kEntryId) {
      continue;
    }
    std::vector<int> preds;
    for (const int p : cfg.blocks[b].predecessors) {
      if (doms.idom[p] >= 0) {
        preds.push_back(p);
      }
    }
    if (preds.size() < 2) {
      continue;
    }
    for (const int p : preds) {
      int runner = p;
      while (runner != doms.idom[b]) {
        df[runner].insert(b);
        runner = doms.idom[runner];
      }
    }
  }
  return df;
}

}  // namespace c6502::passes
