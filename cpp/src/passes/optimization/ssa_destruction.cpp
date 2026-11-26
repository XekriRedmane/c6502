#include "passes/optimization/ssa_destruction.h"

#include <algorithm>
#include <cstddef>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "errors.h"
#include "ordered_map.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using K = ir::Kind_instruction;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;
using Names = std::set<std::string, std::less<>>;
using Edge = std::pair<int, int>;

bool is_conditional(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::JumpIfTrue:
    case K::JumpIfFalse:
    case K::JumpIfCmp:
    case K::JumpIfMasked:
      return true;
    default:
      return false;
  }
}

const std::string& conditional_target(const ir::Type_instruction& instr) {
  switch (ir::kind(instr)) {
    case K::JumpIfTrue:
      return std::get<ir::JumpIfTrue>(instr).target;
    case K::JumpIfFalse:
      return std::get<ir::JumpIfFalse>(instr).target;
    case K::JumpIfCmp:
      return std::get<ir::JumpIfCmp>(instr).target;
    case K::JumpIfMasked:
      return std::get<ir::JumpIfMasked>(instr).target;
    default:
      throw InternalError("AttributeError: the instruction has no target");
  }
}

ir::Type_instruction retarget(ir::Type_instruction instr, const std::string& target) {
  switch (ir::kind(instr)) {
    case K::JumpIfTrue:
      std::get<ir::JumpIfTrue>(instr).target = target;
      break;
    case K::JumpIfFalse:
      std::get<ir::JumpIfFalse>(instr).target = target;
      break;
    case K::JumpIfCmp:
      std::get<ir::JumpIfCmp>(instr).target = target;
      break;
    case K::JumpIfMasked:
      std::get<ir::JumpIfMasked>(instr).target = target;
      break;
    default:
      break;
  }
  return instr;
}

bool is_terminator(const ir::Type_instruction& instr) {
  return asdl::isa<ir::Ret, ir::Jump>(instr) || is_conditional(instr);
}

const std::string& var_name(const ir::Type_val& v) {
  const auto* var = asdl::as<ir::Var>(v);
  if (var == nullptr) {
    throw InternalError("AttributeError: 'Constant' object has no attribute 'name'");
  }
  return var->name;
}

std::map<int, Names> live_in(const Cfg& cfg) {
  std::map<std::string, int, std::less<>> label_of;
  for (const auto& [bid, b] : cfg.blocks) {
    if (!b.instructions.empty()) {
      if (const auto* l = asdl::as<ir::Label>(b.instructions.front())) {
        label_of.insert_or_assign(l->name, bid);
      }
    }
  }
  std::map<int, Names> use;
  std::map<int, Names> defs;
  std::map<int, Names> phi_uses;
  for (const auto& [bid, blk] : cfg.blocks) {
    Names u;
    Names d;
    for (const auto& ins : blk.instructions) {
      if (const auto* phi = asdl::as<ir::Phi>(ins)) {
        d.insert(var_name(phi->dst));
        for (const auto& a : phi->args) {
          const auto& arg = std::get<ir::PhiArg>(a);
          const auto pb = label_of.find(arg.pred_label);
          const auto* src = asdl::as<ir::Var>(arg.source);
          if (pb != label_of.end() && src != nullptr) {
            phi_uses[pb->second].insert(src->name);
          }
        }
        continue;
      }
      for (const auto v : uses_in(ins)) {
        if (!d.contains(v)) {
          u.emplace(v);
        }
      }
      for (const auto v : defs_in(ins)) {
        d.emplace(v);
      }
    }
    use[bid] = std::move(u);
    defs[bid] = std::move(d);
  }
  std::map<int, Names> live;
  for (const auto& [bid, blk] : cfg.blocks) {
    live[bid];
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (const auto& [bid, blk] : cfg.blocks) {
      Names out = phi_uses[bid];
      for (const int sid : blk.successors) {
        const auto& in = live[sid];
        out.insert(in.begin(), in.end());
      }
      Names next = use[bid];
      for (const auto& v : out) {
        if (!defs[bid].contains(v)) {
          next.insert(v);
        }
      }
      if (next != live[bid]) {
        live[bid] = std::move(next);
        changed = true;
      }
    }
  }
  return live;
}

bool needs_split(int pid, int sid, const std::vector<ir::Copy>& copies,
                 const ir::Type_instruction* last, const Cfg& cfg,
                 const OrderedMap<Edge, std::vector<ir::Copy>>& edge_copies,
                 const std::map<int, Names>& live) {
  if (last == nullptr || !is_conditional(*last)) {
    return false;
  }
  std::vector<int> others;
  for (const int t : cfg.blocks.find(pid)->successors) {
    if (t != sid) {
      others.push_back(t);
    }
  }
  if (others.empty()) {
    return false;
  }
  Names dsts;
  for (const auto& c : copies) {
    dsts.insert(var_name(c.dst));
  }
  for (const auto v : uses_in(*last)) {
    if (dsts.contains(v)) {
      return true;
    }
  }
  for (const int t : others) {
    if (edge_copies.contains(Edge{pid, t})) {
      return true;
    }
    const auto it = live.find(t);
    if (it != live.end()) {
      for (const auto& d : dsts) {
        if (it->second.contains(d)) {
          return true;
        }
      }
    }
  }
  return false;
}

bool is_var_named(const ir::Type_val& v, const std::string& name) {
  const auto* var = asdl::as<ir::Var>(v);
  return var != nullptr && var->name == name;
}

std::vector<ir::Copy> order_parallel_copies(const std::vector<ir::Copy>& copies,
                                            const std::string& fn_name, int& cycle_counter,
                                            SymbolTable* symbols) {
  if (copies.size() <= 1) {
    return copies;
  }
  std::vector<ir::Copy> pending = copies;
  std::vector<ir::Copy> out;
  while (!pending.empty()) {
    std::optional<std::size_t> ready;
    for (std::size_t i = 0; i < pending.size(); ++i) {
      const auto* d = asdl::as<ir::Var>(pending[i].dst);
      if (d == nullptr) {
        ready = i;
        break;
      }
      bool blocks_other = false;
      for (std::size_t j = 0; j < pending.size(); ++j) {
        if (j != i && is_var_named(pending[j].src, d->name)) {
          blocks_other = true;
          break;
        }
      }
      if (!blocks_other) {
        ready = i;
        break;
      }
    }
    if (ready) {
      out.push_back(std::move(pending[*ready]));
      pending.erase(pending.begin() + static_cast<std::ptrdiff_t>(*ready));
      continue;
    }
    const std::string d = var_name(pending.front().dst);
    ++cycle_counter;
    const std::string tmp_name = "." + fn_name + "@cycle_tmp@" + std::to_string(cycle_counter);
    if (const Symbol* sym = symbols != nullptr ? symbols->find(d) : nullptr) {
      Symbol copy{.type = sym->type, .attrs = LocalAttr{}};
      symbols->insert_or_assign(tmp_name, std::move(copy));
    }
    out.push_back(ir::Copy{.src = ir::Var{.name = d}, .dst = ir::Var{.name = tmp_name}});
    for (std::size_t j = 1; j < pending.size(); ++j) {
      if (is_var_named(pending[j].src, d)) {
        pending[j] = ir::Copy{.src = ir::Var{.name = tmp_name}, .dst = pending[j].dst};
      }
    }
  }
  return out;
}

}  // namespace

ir::Function from_ssa(ir::Function fn, SymbolTable* symbols) {
  Cfg cfg = tac_cfg::build_cfg(fn);
  std::map<std::string, int, std::less<>> label_to_block;
  for (const auto& [bid, b] : cfg.blocks) {
    if (!b.instructions.empty()) {
      if (const auto* l = asdl::as<ir::Label>(b.instructions.front())) {
        label_to_block.insert_or_assign(l->name, b.id);
      }
    }
  }
  int cycle_counter = 0;
  const std::map<int, Names> live = live_in(cfg);

  OrderedMap<Edge, std::vector<ir::Copy>> edge_copies;
  for (const int bid : cfg.block_order) {
    for (const auto& ins : cfg.blocks.find(bid)->instructions) {
      const auto* phi = asdl::as<ir::Phi>(ins);
      if (phi == nullptr) {
        continue;
      }
      for (const auto& a : phi->args) {
        const auto& arg = std::get<ir::PhiArg>(a);
        const auto pred = label_to_block.find(arg.pred_label);
        if (pred == label_to_block.end()) {
          continue;
        }
        edge_copies[Edge{pred->second, bid}].push_back(
            ir::Copy{.src = arg.source, .dst = phi->dst});
      }
    }
  }

  int split_counter = 0;
  std::map<int, std::ptrdiff_t> last_index;
  for (const auto& [bid, blk] : cfg.blocks) {
    last_index[bid] = static_cast<std::ptrdiff_t>(blk.instructions.size()) - 1;
  }
  for (const auto& [edge, copies] : edge_copies) {
    const auto [pid, sid] = edge;
    const std::vector<ir::Copy> ordered =
        order_parallel_copies(copies, fn.name, cycle_counter, symbols);
    const std::ptrdiff_t k = last_index[pid];
    std::optional<ir::Type_instruction> last;
    if (k >= 0) {
      last = cfg.blocks.find(pid)->instructions[static_cast<std::size_t>(k)];
    }
    auto& pred_instrs = cfg.blocks.find(pid)->instructions;
    if (!needs_split(pid, sid, copies, last ? &*last : nullptr, cfg, edge_copies, live)) {
      std::ptrdiff_t insert_pos = k + 1;
      if (last && is_terminator(*last)) {
        insert_pos = k;
        last_index[pid] = k + static_cast<std::ptrdiff_t>(ordered.size());
      }
      pred_instrs.insert(pred_instrs.begin() + insert_pos, ordered.begin(), ordered.end());
      continue;
    }
    const auto& succ_first = cfg.blocks.find(sid)->instructions.front();
    const auto* succ_label_instr = asdl::as<ir::Label>(succ_first);
    if (succ_label_instr == nullptr) {
      throw InternalError("AttributeError: the Phi's block doesn't start with a Label");
    }
    const std::string succ_label = succ_label_instr->name;
    if (conditional_target(*last) != succ_label) {
      pred_instrs.insert(pred_instrs.end(), ordered.begin(), ordered.end());
      continue;
    }
    ++split_counter;
    const std::string split_label = "." + fn.name + "@ssa_split@" + std::to_string(split_counter);
    pred_instrs[static_cast<std::size_t>(k)] = retarget(*last, split_label);
    int new_id = 0;
    for (const auto& [bid, blk] : cfg.blocks) {
      new_id = std::max(new_id, bid);
    }
    ++new_id;
    BasicBlock split{.id = new_id};
    split.instructions.push_back(ir::Label{.name = split_label});
    split.instructions.insert(split.instructions.end(), ordered.begin(), ordered.end());
    split.instructions.push_back(ir::Jump{.target = succ_label});
    cfg.blocks.insert_or_assign(new_id, std::move(split));
    cfg.block_order.push_back(new_id);
  }

  for (const int bid : cfg.block_order) {
    auto& instrs = cfg.blocks.find(bid)->instructions;
    std::erase_if(instrs, [](const ir::Type_instruction& i) { return asdl::isa<ir::Phi>(i); });
  }

  return tac_cfg::cfg_to_function(fn, cfg);
}

}  // namespace c6502::passes
