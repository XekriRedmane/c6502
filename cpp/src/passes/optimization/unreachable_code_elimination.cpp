/* TAC unreachable-code elimination.

Five steps, run in order on the function's CFG.

1. Drop the unreachable blocks. A walk forward from ENTRY visits every
   block a path of control flow reaches, and every other block goes.
   The code after a `Ret` or a `Jump` up to the next label that a jump
   targets is such a block: the terminator stops the fall-through, and
   the next block is reachable only when some jump names its label. The
   SSA cleanup goes with it: a `PhiArg` in a surviving block whose
   `pred_label` named a dropped block is dropped too.

2. Prune the Phi arguments of dead edges. Constant folding can drop a
   conditional jump (`JumpIfFalse(true)` or `JumpIfTrue(false)` folds
   to nothing), which takes an edge out of the CFG without taking out
   either block. A Phi at the jump's old target with an argument from
   the block that lost its edge has to lose the argument. Each block's
   Phis drop the arguments whose `pred_label` isn't the label of one of
   the block's present predecessors.

3. Fold the Phis with one argument. After steps 1 and 2 (and after the
   driver's earlier passes propagate constants into the arguments), a
   `Phi` left with one argument means `Copy(args[0].source, dst)`, and
   becomes that. A Phi with no arguments, all of whose predecessors
   went, is dropped. That is defensive, since its block is unreachable
   then and step 1 has already dropped it.

4. Drop the jumps that go to the next block. A block other than the
   last whose terminator has the next block in source order as its only
   successor doesn't need the terminator, since falling through gets
   there. That covers `Jump(L)` when L's block is next, and a
   conditional jump to L when L is next, where the taken edge and the
   fall-through coincide. A `Ret` stays, since its successor is EXIT.

5. Drop the labels nothing names. A `Label(L)` at a block's start goes
   when no remaining jump targets L and no `PhiArg.pred_label` is L. The
   block stays reachable by falling through, so control flow doesn't
   change. The Phi labels have to count in SSA form, or dropping a
   label a Phi names would leave SSA destruction unable to find the
   predecessor block.

The order matters. Step 1 comes before step 2, which uses the
predecessor lists after the cleanup. Steps 1 and 2 come before step 3,
which folds the pruned argument lists. Steps 1 and 2 come before steps
4 and 5, since dropping a dead block takes out the jumps in it, and
step 4 comes before step 5, since dropping a jump can leave its
target's label unused.

The pass doesn't iterate to a fixed point of its own. The driver runs
the whole pipeline again until the function stops changing, so a later
pass's opening is taken in a later sweep.

The blocks left empty by steps 3 and 4 aren't removed. Flattening walks
`block_order` and writes each block's instructions, so an empty block
writes nothing. The edges of the CFG go stale in the process, which
nothing outside the pass sees, since every later pass builds its own
CFG. */
#include "passes/optimization/unreachable_code_elimination.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/optimization/cfg.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::PassContext;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;
using K = ir::Kind_instruction;
using Labels = std::set<std::string, std::less<>>;

// Returns the target of a jump of any kind, or null.
const std::string* jump_target(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::Jump:
      return &std::get<ir::Jump>(instr).target;
    case K::JumpIfTrue:
      return &std::get<ir::JumpIfTrue>(instr).target;
    case K::JumpIfFalse:
      return &std::get<ir::JumpIfFalse>(instr).target;
    case K::JumpIfCmp:
      return &std::get<ir::JumpIfCmp>(instr).target;
    case K::JumpIfMasked:
      return &std::get<ir::JumpIfMasked>(instr).target;
    default:
      return nullptr;
  }
}

// Returns the name of the label `blk` starts with, or null.
const std::string* leading_label(const BasicBlock& blk) {
  if (blk.instructions.empty()) {
    return nullptr;
  }
  const auto* label = std::get_if<ir::Label>(&blk.instructions.front());
  return label != nullptr ? &label->name : nullptr;
}

// Replaces each `Phi` of `blk` with one that holds the arguments `keep`
// accepts. Python builds a new Phi here, since its block's instructions
// are those of the function the pass was given, and the driver's fixed
// point compares the function after a sweep with the one before it.
// Pruning the arguments in place changed both, so a sweep whose only
// change was the pruning looked like no change, and the passes that the
// pruning enabled didn't run again. The C++ blocks own their copies.
template <class Keep>
void filter_phi_args(BasicBlock& blk, Keep keep) {
  for (auto& instr : blk.instructions) {
    if (auto* phi = std::get_if<ir::Phi>(&instr)) {
      std::erase_if(phi->args, [&](const ir::Type_phi_arg& a) {
        return !keep(std::get<ir::PhiArg>(a));
      });
    }
  }
}

// Drops every block a forward walk from ENTRY doesn't reach, with the
// edges of the surviving blocks that named one, and the `PhiArg`s in the
// surviving blocks whose `pred_label` named one.
void remove_unreachable_blocks(Cfg& cfg) {
  std::set<int> reachable;
  std::vector<int> stack{tac_cfg::kEntryId};
  while (!stack.empty()) {
    const int bid = stack.back();
    stack.pop_back();
    if (!reachable.insert(bid).second) {
      continue;
    }
    for (const int s : cfg.blocks.at(bid).successors) {
      stack.push_back(s);
    }
  }

  // The labels of the blocks about to go, so that the PhiArgs that
  // name them can go too.
  Labels dropped_labels;
  std::vector<int> dropped;
  for (const auto& [bid, blk] : cfg.blocks) {
    if (reachable.contains(bid)) {
      continue;
    }
    dropped.push_back(bid);
    if (const std::string* label = leading_label(blk)) {
      dropped_labels.insert(*label);
    }
  }
  for (const int bid : dropped) {
    cfg.blocks.erase(bid);
  }
  std::erase_if(cfg.block_order, [&](int b) { return !reachable.contains(b); });
  for (auto& [bid, blk] : cfg.blocks) {
    std::erase_if(blk.predecessors, [&](int p) { return !reachable.contains(p); });
    std::erase_if(blk.successors, [&](int s) { return !reachable.contains(s); });
    if (dropped_labels.empty()) {
      continue;
    }
    filter_phi_args(blk, [&](const ir::PhiArg& a) { return !dropped_labels.contains(a.pred_label); });
  }
}

// For each block with Phis, drops each PhiArg whose `pred_label` isn't
// the label of one of the block's present predecessors. That catches a
// conditional jump that constant folding dropped (`JumpIfFalse(true)`
// folds to nothing), which takes the edge from the predecessor out
// without taking out the predecessor, and leaves the Phi with a stale
// argument from it.
void prune_dead_phi_edges(Cfg& cfg) {
  std::map<int, std::string> leading;
  for (const auto& [bid, blk] : cfg.blocks) {
    if (const std::string* label = leading_label(blk)) {
      leading.emplace(bid, *label);
    }
  }
  for (auto& [bid, blk] : cfg.blocks) {
    const bool has_phi = std::any_of(blk.instructions.begin(), blk.instructions.end(),
                                     [](const ir::Type_instruction& i) { return std::holds_alternative<ir::Phi>(i); });
    if (!has_phi) {
      continue;
    }
    Labels valid;
    for (const int p : blk.predecessors) {
      const auto it = leading.find(p);
      if (it != leading.end()) {
        valid.insert(it->second);
      }
    }
    filter_phi_args(blk, [&](const ir::PhiArg& a) { return valid.contains(a.pred_label); });
  }
}

// Makes each `Phi` with one remaining argument `Copy(args[0].source,
// dst)`, and drops each Phi with none, which is defensive, since its
// block is unreachable in a well-formed CFG.
void fold_singleton_phis(Cfg& cfg) {
  for (auto& [bid, blk] : cfg.blocks) {
    std::vector<ir::Type_instruction> out;
    out.reserve(blk.instructions.size());
    for (auto& instr : blk.instructions) {
      auto* phi = std::get_if<ir::Phi>(&instr);
      if (phi == nullptr) {
        out.push_back(std::move(instr));
        continue;
      }
      if (phi->args.empty()) {
        continue;
      }
      if (phi->args.size() == 1) {
        out.push_back(ir::Copy{.src = std::get<ir::PhiArg>(phi->args.front()).source, .dst = phi->dst});
        continue;
      }
      out.push_back(std::move(instr));
    }
    blk.instructions = std::move(out);
  }
}

// For each real block but the last whose terminator is a jump or a
// conditional jump with the next block in source order as its every
// successor, drops the terminator and merges the duplicate edges.
void remove_useless_jumps(Cfg& cfg) {
  for (std::size_t i = 0; i + 1 < cfg.block_order.size(); ++i) {
    const int bid = cfg.block_order[i];
    const int next_bid = cfg.block_order[i + 1];
    BasicBlock& blk = *cfg.blocks.find(bid);
    if (blk.instructions.empty() || jump_target(blk.instructions.back()) == nullptr) {
      continue;
    }
    if (std::any_of(blk.successors.begin(), blk.successors.end(), [&](int s) { return s != next_bid; })) {
      continue;
    }
    blk.instructions.pop_back();
    // A conditional jump's two successors merge from [next, next] into
    // [next], and an unconditional jump had one already. The next
    // block's predecessor list does the same.
    blk.successors = {next_bid};
    std::vector<int>& next_preds = cfg.blocks.find(next_bid)->predecessors;
    if (std::count(next_preds.begin(), next_preds.end(), bid) > 1) {
      std::erase(next_preds, bid);
      next_preds.push_back(bid);
    }
  }
}

// Collects every jump target and every `PhiArg.pred_label` in the
// function, and drops each leading `Label(L)` whose L isn't among them.
// A Phi's labels count, since SSA destruction finds the predecessor
// block by its leading label later, and dropping a label a Phi names
// would break that.
void remove_useless_labels(Cfg& cfg) {
  Labels targets;
  for (const auto& [bid, blk] : cfg.blocks) {
    for (const auto& instr : blk.instructions) {
      if (const std::string* t = jump_target(instr)) {
        targets.insert(*t);
      } else if (const auto* phi = std::get_if<ir::Phi>(&instr)) {
        for (const auto& a : phi->args) {
          targets.insert(std::get<ir::PhiArg>(a).pred_label);
        }
      }
    }
  }
  for (auto& [bid, blk] : cfg.blocks) {
    const std::string* label = leading_label(blk);
    if (label != nullptr && !targets.contains(*label)) {
      blk.instructions.erase(blk.instructions.begin());
    }
  }
}

}  // namespace

tac_ast::Function eliminate_unreachable_code(tac_ast::Function fn, const PassContext&) {
  Cfg cfg = tac_cfg::build_cfg(fn);
  remove_unreachable_blocks(cfg);
  prune_dead_phi_edges(cfg);
  fold_singleton_phis(cfg);
  remove_useless_jumps(cfg);
  remove_useless_labels(cfg);
  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
