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

const std::string* leading_label(const BasicBlock& blk) {
  if (blk.instructions.empty()) {
    return nullptr;
  }
  const auto* label = std::get_if<ir::Label>(&blk.instructions.front());
  return label != nullptr ? &label->name : nullptr;
}

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

bool try_eliminate(Cfg& cfg, int header, const PyIntSet& body, const NameSet& ssa) {
  const std::vector<std::int64_t> body_ids = body.items();
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

  BasicBlock& exit_blk = *cfg.blocks.find(exit_block_id);
  const std::string* exit_label_p = leading_label(exit_blk);
  if (exit_label_p == nullptr) {
    return false;
  }
  const std::string exit_label = *exit_label_p;

  BasicBlock& header_blk = *cfg.blocks.find(header);
  const std::string* header_label_p = leading_label(header_blk);
  if (header_label_p == nullptr) {
    return false;
  }
  const std::string header_label = *header_label_p;

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
