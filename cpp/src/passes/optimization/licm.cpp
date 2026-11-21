/* Loop-invariant code motion for extends and truncates.

An instruction inside a natural loop whose operands are all Constants or
SSA values defined outside the loop computes the same value on every
iteration. It is moved to the end of the loop's preheader, the one
block outside the loop that flows into its header, so that it runs
once. The instructions that become invariant once an earlier one moves
out are moved too.

Only the extends and `Truncate` move. They have no effects, so running
one before a loop that would have skipped it changes only the time
taken, and they lower to byte copies (with a zero or sign byte), so
moving one out never adds work. Moving an add out as well was tried, and
it made `floor_enemy_advance` two instructions longer: `player_col + 9`
is used once, on a conditional path, and holding it across the loop
costs a store and a load where the add in place took none.

The motivating case is a byte that is zero-extended inside a loop to be
added to a pointer, such as `entry += search_size` with `uint8_t
search_size`. Inside the loop the extended copy has to stay live next to
the byte, which costs a copy per iteration.

The preheader must have the header as its only successor, so that the
moved instructions run only on the way into the loop. A loop whose
header has more than one predecessor outside the loop is left alone. */
#include "passes/optimization/licm.h"

#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::PassContext;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;

// Returns the source and destination of an extend or a `Truncate`, the
// instructions that move, or none for any other instruction.
std::optional<std::pair<const ir::Type_val*, const ir::Type_val*>> movable(
    const ir::Type_instruction& instr) {
  if (const auto* z = std::get_if<ir::ZeroExtend>(&instr)) {
    return std::pair{&z->src, &z->dst};
  }
  if (const auto* s = std::get_if<ir::SignExtend>(&instr)) {
    return std::pair{&s->src, &s->dst};
  }
  if (const auto* t = std::get_if<ir::Truncate>(&instr)) {
    return std::pair{&t->src, &t->dst};
  }
  return std::nullopt;
}

}  // namespace

tac_ast::Function hoist_loop_invariants(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr || ctx.ssa_dsts->empty()) {
    return fn;
  }
  const NameSet& ssa = *ctx.ssa_dsts;
  Cfg cfg = tac_cfg::build_cfg(fn);
  bool changed = false;
  for (const auto& [header, body] : tac_cfg::natural_loops(cfg)) {
    std::vector<int> outside;
    for (const int p : cfg.blocks.at(header).predecessors) {
      if (!body.contains(p)) {
        outside.push_back(p);
      }
    }
    if (outside.size() != 1) {
      continue;
    }
    BasicBlock& pre = *cfg.blocks.find(outside.front());
    if (pre.successors != std::vector<int>{header} || pre.instructions.empty()) {
      continue;
    }
    std::set<std::string, std::less<>> defined_in_loop;
    for (const auto bid : body.items()) {
      for (const auto& instr : cfg.blocks.at(static_cast<int>(bid)).instructions) {
        for (const std::string_view v : defs_in(instr)) {
          defined_in_loop.emplace(v);
        }
      }
    }
    const auto invariant = [&](const ir::Type_val& v) {
      if (std::holds_alternative<ir::Constant>(v)) {
        return true;
      }
      const auto& name = std::get<ir::Var>(v).name;
      return ssa.contains(name) && !defined_in_loop.contains(name);
    };

    std::vector<ir::Type_instruction> hoisted;
    bool progress = true;
    while (progress) {
      progress = false;
      for (const auto bid : body.sorted()) {
        BasicBlock& blk = *cfg.blocks.find(static_cast<int>(bid));
        std::vector<ir::Type_instruction> keep;
        for (auto& instr : blk.instructions) {
          const auto m = movable(instr);
          const auto* dst = m ? std::get_if<ir::Var>(m->second) : nullptr;
          if (dst != nullptr && ssa.contains(dst->name) && invariant(*m->first)) {
            defined_in_loop.erase(dst->name);
            hoisted.push_back(std::move(instr));
            progress = true;
          } else {
            keep.push_back(std::move(instr));
          }
        }
        blk.instructions = std::move(keep);
      }
    }
    if (hoisted.empty()) {
      continue;
    }
    auto& instrs = pre.instructions;
    auto at = instrs.end();
    if (tac_cfg::is_terminator(instrs.back())) {
      --at;
    }
    instrs.insert(at, std::make_move_iterator(hoisted.begin()), std::make_move_iterator(hoisted.end()));
    changed = true;
  }
  if (!changed) {
    return fn;
  }
  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
