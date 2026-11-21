/* TAC elimination of dead pure loops.

A natural loop with no effect that can be seen (no call, no store, no
`Ret`), none of whose values is read outside it, is dead, since all it
does is compute values nothing reads. The pass rewrites the loop's
header to a `Jump` to the loop's exit, which leaves the rest of the
body unreachable for unreachable-code elimination to prune in the next
sweep.

The pass runs in the TAC optimizer's fixed point, so its input is in
SSA form. With one definition per name, the test of what lives past the
loop is a scan of names: a name defined in the loop lives past it
exactly when some instruction outside the body reads it. A definition
in the body of a name that isn't an SSA name (a static, an
address-taken local, an aggregate) keeps the loop, since proving that
it isn't read later would take liveness outside SSA form, and the
shapes that motivated the pass (an empty `do { } while (--counter)` or
`while (--counter)`) write only SSA names anyway.

Besides purity, these gates apply.

  - The loop has exactly one exit edge, from a body block to a block
    outside. A loop with several exits is left alone, since merging the
    exits' Phi arguments into one is safe only when their sources
    agree, which the pass doesn't check.
  - The exit block starts with a `Label`, since the new jump names it.
  - The header starts with a `Label`, which stays, since code outside
    the loop may still jump to it.

When the rewrite fires, the header's instructions become
`[Label(header), Jump(exit)]`. A Phi at the exit whose `pred_label`
named the old source of the exit edge names the header instead, since
the header is now the exit's predecessor on the loop's side. No Phi
argument's source is a value of the loop, which the scan checked, so
the change is to the predecessor's label alone and not to any value. */
#include "passes/optimization/dead_loop_elimination.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "c99_to_tac.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::PassContext;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;
using K = ir::Kind_instruction;

// Returns the name of the label `blk` starts with, or null.
const std::string* leading_label(const BasicBlock& blk) {
  if (blk.instructions.empty()) {
    return nullptr;
  }
  const auto* label = std::get_if<ir::Label>(&blk.instructions.front());
  return label != nullptr ? &label->name : nullptr;
}

// Returns whether a loop that holds `instr` can't be deleted. A call
// may do anything. A store writes memory, perhaps through an alias,
// that is seen past the loop. A `Ret` leaving the loop is a different
// thing from falling out of it, and merging them would change what the
// function returns. A volatile load is an effect (C99 §6.7.3.6), even
// when only the loop reads its destination. Every other instruction is
// pure.
bool has_effect(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::FunctionCall:
    case K::IndirectCall:
    case K::Store:
    case K::IndexedStore:
    case K::IndexedSymbolStore:
    case K::IndirectIndexedStore:
    case K::Ret:
      return true;
    case K::Load:
      return std::get<ir::Load>(instr).is_volatile;
    case K::IndexedLoad:
      return std::get<ir::IndexedLoad>(instr).is_volatile;
    case K::IndexedConstLoad:
      return std::get<ir::IndexedConstLoad>(instr).is_volatile;
    case K::IndirectIndexedLoad:
      return std::get<ir::IndirectIndexedLoad>(instr).is_volatile;
    default:
      return false;
  }
}

// Checks that the loop is pure and that nothing outside reads its
// values, and then rewrites the header to jump past the body. Changes
// `cfg` only when the rewrite fires, and returns whether it did.
bool try_eliminate(Cfg& cfg, int header, const PyIntSet& body, const NameSet& ssa) {
  const std::vector<std::int64_t> body_ids = body.items();
  // A loop marked `[[c6502::keep]]` stays, which a delay loop needs.
  for (const auto bid : body_ids) {
    for (const auto& instr : cfg.blocks.at(static_cast<int>(bid)).instructions) {
      const auto* label = std::get_if<ir::Label>(&instr);
      if (label != nullptr && label->name.find(c99_to_tac::kKeepLoopMark) != std::string::npos) {
        return false;
      }
    }
  }
  for (const auto bid : body_ids) {
    for (const auto& instr : cfg.blocks.at(static_cast<int>(bid)).instructions) {
      if (has_effect(instr)) {
        return false;
      }
    }
  }

  // Every definition in the body must be of an SSA name, since any other
  // would take full liveness to show it isn't read past the loop.
  std::set<std::string, std::less<>> loop_defs;
  for (const auto bid : body_ids) {
    for (const auto& instr : cfg.blocks.at(static_cast<int>(bid)).instructions) {
      for (const std::string_view d : defs_in(instr)) {
        if (!ssa.contains(d)) {
          return false;
        }
        loop_defs.emplace(d);
      }
    }
  }

  // No read outside the loop may name a value of the loop. `uses_in`
  // gives a Phi's sources as reads, so a Phi at the exit with an
  // argument from the loop is caught here.
  for (const auto& [bid, blk] : cfg.blocks) {
    if (body.contains(bid)) {
      continue;
    }
    for (const auto& instr : blk.instructions) {
      for (const std::string_view u : uses_in(instr)) {
        if (loop_defs.contains(u)) {
          return false;
        }
      }
    }
  }

  // Exactly one edge may leave the body.
  std::vector<std::pair<int, int>> exit_edges;
  for (const auto bid : body_ids) {
    for (const int succ : cfg.blocks.at(static_cast<int>(bid)).successors) {
      if (!body.contains(succ)) {
        exit_edges.emplace_back(static_cast<int>(bid), succ);
      }
    }
  }
  if (exit_edges.size() != 1) {
    return false;
  }
  const auto [src_id, exit_block_id] = exit_edges.front();

  // The exit block needs a leading label for the jump to name.
  BasicBlock& exit_blk = *cfg.blocks.find(exit_block_id);
  const std::string* exit_label_p = leading_label(exit_blk);
  if (exit_label_p == nullptr) {
    return false;
  }
  const std::string exit_label = *exit_label_p;

  // The header needs a leading label, which stays for any jump into it
  // from outside.
  BasicBlock& header_blk = *cfg.blocks.find(header);
  const std::string* header_label_p = leading_label(header_blk);
  if (header_label_p == nullptr) {
    return false;
  }
  const std::string header_label = *header_label_p;

  // The exit's predecessor on the loop's side was `src_id`, and is the
  // header now, so each PhiArg at the exit that named the old one names
  // the header. Python builds a new Phi here, since its block holds the
  // instructions of the function the pass was given, which a change in
  // place would change too (unreachable_code_elimination.cpp's
  // `filter_phi_args` has the reason). The C++ blocks own their copies.
  const std::string* src_label = leading_label(cfg.blocks.at(src_id));
  if (src_label != nullptr && *src_label != header_label) {
    const std::string old = *src_label;
    for (auto& instr : exit_blk.instructions) {
      if (auto* phi = std::get_if<ir::Phi>(&instr)) {
        for (auto& a : phi->args) {
          auto& arg = std::get<ir::PhiArg>(a);
          if (arg.pred_label == old) {
            arg.pred_label = header_label;
          }
        }
      }
    }
  }

  // The header keeps its label, so that anything still jumping in lands
  // somewhere valid, and jumps past the body. Unreachable-code
  // elimination prunes the rest of the body in the next sweep, since
  // ENTRY no longer reaches it.
  header_blk.instructions = {ir::Label{.name = header_label}, ir::Jump{.target = exit_label}};
  return true;
}

}  // namespace

tac_ast::Function eliminate_dead_loops(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr) {
    return fn;
  }
  Cfg cfg = tac_cfg::build_cfg(fn);
  const auto loops = tac_cfg::natural_loops(cfg);
  bool changed = false;
  for (const auto& [header, body] : loops) {
    if (try_eliminate(cfg, header, body, *ctx.ssa_dsts)) {
      changed = true;
    }
  }
  if (!changed) {
    return fn;
  }
  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
