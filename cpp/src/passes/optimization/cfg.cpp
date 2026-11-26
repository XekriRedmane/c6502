#include "passes/optimization/cfg.h"

#include <map>
#include <set>
#include <string>

#include "asdl/runtime.h"
#include "errors.h"

namespace c6502::passes::tac_cfg {

namespace tac = ::c6502::tac_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

const std::string* jump_target(const tac::Type_instruction& instr) {
  switch (tac::kind(instr)) {
    case tac::Kind_instruction::Jump:
      return &std::get<tac::Jump>(instr).target;
    case tac::Kind_instruction::JumpIfTrue:
      return &std::get<tac::JumpIfTrue>(instr).target;
    case tac::Kind_instruction::JumpIfFalse:
      return &std::get<tac::JumpIfFalse>(instr).target;
    case tac::Kind_instruction::JumpIfCmp:
      return &std::get<tac::JumpIfCmp>(instr).target;
    case tac::Kind_instruction::JumpIfMasked:
      return &std::get<tac::JumpIfMasked>(instr).target;
    default:
      return nullptr;
  }
}

}  // namespace

bool is_terminator(const tac::Type_instruction& instr) {
  return isa<tac::Ret>(instr) || jump_target(instr) != nullptr;
}

Cfg build_cfg(const tac::Function& fn) {
  Cfg cfg;
  cfg.blocks.insert_or_assign(kEntryId, BasicBlock{.id = kEntryId});
  cfg.blocks.insert_or_assign(kExitId, BasicBlock{.id = kExitId});
  int next_id = 2;
  std::vector<tac::Type_instruction> current;

  const auto finalize = [&]() {
    if (current.empty()) {
      return;
    }
    const int bid = next_id++;
    cfg.blocks.insert_or_assign(bid, BasicBlock{.id = bid, .instructions = std::move(current)});
    cfg.block_order.push_back(bid);
    current.clear();
  };

  for (const auto& instr : fn.instructions) {
    if (isa<tac::Label>(instr)) {
      finalize();
      current.push_back(instr);
    } else if (is_terminator(instr)) {
      current.push_back(instr);
      finalize();
    } else {
      current.push_back(instr);
    }
  }
  finalize();

  OrderedMap<std::string, int> label_to_block;
  for (const int bid : cfg.block_order) {
    if (const auto* label = as<tac::Label>(cfg.blocks.at(bid).instructions.front())) {
      label_to_block.insert_or_assign(label->name, bid);
    }
  }
  const auto block_of = [&](const std::string& label) {
    const int* bid = label_to_block.find(label);
    if (bid == nullptr) {
      throw InternalError("KeyError: '" + label + "'");
    }
    return *bid;
  };

  const auto add_edge = [&](int src, int dst) {
    cfg.blocks.find(src)->successors.push_back(dst);
    cfg.blocks.find(dst)->predecessors.push_back(src);
  };

  if (!cfg.block_order.empty()) {
    add_edge(kEntryId, cfg.block_order.front());
  } else {
    add_edge(kEntryId, kExitId);
  }

  for (std::size_t i = 0; i < cfg.block_order.size(); ++i) {
    const int bid = cfg.block_order[i];
    const int next_bid = i + 1 < cfg.block_order.size() ? cfg.block_order[i + 1] : kExitId;
    const auto& last = cfg.blocks.at(bid).instructions.back();
    if (isa<tac::Ret>(last)) {
      add_edge(bid, kExitId);
    } else if (const auto* jump = as<tac::Jump>(last)) {
      add_edge(bid, block_of(jump->target));
    } else if (const std::string* target = jump_target(last)) {
      add_edge(bid, block_of(*target));
      add_edge(bid, next_bid);
    } else {
      add_edge(bid, next_bid);
    }
  }
  return cfg;
}

tac::Function cfg_to_function(const tac::Function& fn, const Cfg& cfg) {
  tac::Function out{.name = fn.name, .is_global = fn.is_global, .params = fn.params};
  for (const int bid : cfg.block_order) {
    if (const BasicBlock* block = cfg.blocks.find(bid)) {
      out.instructions.insert(out.instructions.end(), block->instructions.begin(),
                              block->instructions.end());
    }
  }
  return out;
}

std::vector<int> reverse_postorder(const Cfg& cfg) {
  std::set<int> visited{kEntryId};
  std::vector<int> postorder;
  struct Frame {
    int bid;
    std::vector<int> succs;
    std::size_t i;
  };
  std::vector<Frame> stack;
  stack.push_back({kEntryId, cfg.blocks.at(kEntryId).successors, 0});
  while (!stack.empty()) {
    Frame& top = stack.back();
    if (top.i >= top.succs.size()) {
      postorder.push_back(top.bid);
      stack.pop_back();
      continue;
    }
    const int nxt = top.succs[top.i++];
    if (visited.insert(nxt).second) {
      stack.push_back({nxt, cfg.blocks.at(nxt).successors, 0});
    }
  }
  return {postorder.rbegin(), postorder.rend()};
}

OrderedMap<int, int> immediate_dominators(const Cfg& cfg) {
  const std::vector<int> rpo = reverse_postorder(cfg);
  std::map<int, std::size_t> rpo_index;
  for (std::size_t i = 0; i < rpo.size(); ++i) {
    rpo_index.emplace(rpo[i], i);
  }
  OrderedMap<int, int> idom;
  idom.insert_or_assign(kEntryId, kEntryId);

  const auto intersect = [&](int a, int b) {
    int finger1 = a;
    int finger2 = b;
    while (finger1 != finger2) {
      while (rpo_index.at(finger1) > rpo_index.at(finger2)) {
        finger1 = idom.at(finger1);
      }
      while (rpo_index.at(finger2) > rpo_index.at(finger1)) {
        finger2 = idom.at(finger2);
      }
    }
    return finger1;
  };

  bool changed = true;
  while (changed) {
    changed = false;
    for (const int b : rpo) {
      if (b == kEntryId) {
        continue;
      }
      std::vector<int> processed;
      for (const int p : cfg.blocks.at(b).predecessors) {
        if (rpo_index.count(p) != 0 && idom.contains(p)) {
          processed.push_back(p);
        }
      }
      if (processed.empty()) {
        continue;
      }
      int new_idom = processed.front();
      for (std::size_t k = 1; k < processed.size(); ++k) {
        new_idom = intersect(processed[k], new_idom);
      }
      const int* old = idom.find(b);
      if (old == nullptr || *old != new_idom) {
        idom.insert_or_assign(b, new_idom);
        changed = true;
      }
    }
  }
  return idom;
}

OrderedMap<int, std::vector<int>> dominator_tree_children(const OrderedMap<int, int>& idom) {
  OrderedMap<int, std::vector<int>> children;
  for (const auto& [b, p] : idom) {
    children.insert_or_assign(b, {});
  }
  for (const auto& [b, p] : idom) {
    if (b == p) {
      continue;
    }
    children[p].push_back(b);
  }
  return children;
}

OrderedMap<int, PyIntSet> dominance_frontiers(const Cfg& cfg) {
  const OrderedMap<int, int> idom = immediate_dominators(cfg);
  OrderedMap<int, PyIntSet> df;
  for (const auto& [b, p] : idom) {
    df.insert_or_assign(b, PyIntSet{});
  }
  for (const auto& [b, b_idom] : idom) {
    if (b == kEntryId) {
      continue;
    }
    std::vector<int> preds;
    for (const int p : cfg.blocks.at(b).predecessors) {
      if (idom.contains(p)) {
        preds.push_back(p);
      }
    }
    if (preds.size() < 2) {
      continue;
    }
    for (const int p : preds) {
      int runner = p;
      while (runner != b_idom) {
        df.find(runner)->add(b);
        runner = idom.at(runner);
      }
    }
  }
  return df;
}

std::vector<std::pair<int, PyIntSet>> natural_loops(const Cfg& cfg) {
  const OrderedMap<int, int> idom = immediate_dominators(cfg);
  PyIntSet reachable;
  for (const auto& [b, p] : idom) {
    reachable.add(b);
  }

  const auto dominates = [&](int d, int b) {
    if (d == b) {
      return true;
    }
    int cur = b;
    while (idom.at(cur) != cur) {
      cur = idom.at(cur);
      if (cur == d) {
        return true;
      }
    }
    return false;
  };

  OrderedMap<int, std::vector<int>> back_edges_by_header;
  for (const auto bid64 : reachable.items()) {
    const int bid = static_cast<int>(bid64);
    if (bid == kEntryId) {
      continue;
    }
    for (const int succ : cfg.blocks.at(bid).successors) {
      if (reachable.contains(succ) && dominates(succ, bid)) {
        back_edges_by_header[succ].push_back(bid);
      }
    }
  }

  std::vector<std::pair<int, PyIntSet>> loops;
  for (const auto& [header, tails] : back_edges_by_header) {
    PyIntSet body;
    body.add(header);
    std::vector<int> stack = tails;
    while (!stack.empty()) {
      const int n = stack.back();
      stack.pop_back();
      if (body.contains(n)) {
        continue;
      }
      body.add(n);
      for (const int pred : cfg.blocks.at(n).predecessors) {
        if (reachable.contains(pred) && !body.contains(pred)) {
          stack.push_back(pred);
        }
      }
    }
    loops.emplace_back(header, std::move(body));
  }
  return loops;
}

}  // namespace c6502::passes::tac_cfg
