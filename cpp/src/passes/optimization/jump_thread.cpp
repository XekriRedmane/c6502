#include "passes/optimization/jump_thread.h"

#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "bit_precise.h"
#include "errors.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/constant_folding.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace {

namespace ir = ::c6502::tac_ast;
using framework::PassContext;
using tac_cfg::BasicBlock;
using tac_cfg::Cfg;
using K = ir::Kind_instruction;
using Labels = std::set<std::string, std::less<>>;
using FreshLabel = std::function<std::string()>;

std::string* jump_target(ir::Type_instruction& instr) {
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

const std::string* jump_target(const ir::Type_instruction& instr) {
  return jump_target(const_cast<ir::Type_instruction&>(instr));
}

bool is_conditional(const ir::Type_instruction& instr) {
  return jump_target(instr) != nullptr && !std::holds_alternative<ir::Jump>(instr);
}

std::optional<std::string> leading_label(const BasicBlock& block) {
  if (!block.instructions.empty()) {
    if (const auto* label = std::get_if<ir::Label>(&block.instructions.front())) {
      return label->name;
    }
  }
  return std::nullopt;
}

std::optional<asdl::Int> constant_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c != nullptr && constant_folding::is_integer_const(c->const_)) {
    return bit_precise::raw_value(c->const_);
  }
  return std::nullopt;
}

struct Thread {
  std::string pred_label;
  std::vector<std::pair<int, std::string>> sources;
  int target = 0;
};

std::set<int> reachable_blocks(const Cfg& cfg) {
  std::set<int> seen{tac_cfg::kEntryId};
  std::vector<int> stack{tac_cfg::kEntryId};
  while (!stack.empty()) {
    const int bid = stack.back();
    stack.pop_back();
    for (const int s : cfg.blocks.at(bid).successors) {
      if (seen.insert(s).second) {
        stack.push_back(s);
      }
    }
  }
  return seen;
}

bool contains(const std::vector<int>& v, int x) {
  return std::find(v.begin(), v.end(), x) != v.end();
}

template <class EntersLoopBody>
std::optional<std::vector<std::pair<int, std::string>>> jumps_through(
    const Cfg& cfg, int pred, const std::string& join_label, int target, bool named,
    EntersLoopBody& enters_loop_body) {
  const BasicBlock& block = cfg.blocks.at(pred);
  const std::size_t rest = block.instructions.size() - 1;
  if (rest != 0) {
    const auto* jump = std::get_if<ir::Jump>(&block.instructions[1]);
    if (rest != 1 || jump == nullptr || jump->target != join_label) {
      return std::nullopt;
    }
  }
  if (block.predecessors.empty()) {
    return std::nullopt;
  }
  const std::string name = std::get<ir::Label>(block.instructions.front()).name;
  std::vector<std::pair<int, std::string>> sources;
  for (const int q : block.predecessors) {
    const BasicBlock* q_block = cfg.blocks.find(q);
    if (q == pred || q_block == nullptr || q_block->instructions.empty()) {
      return std::nullopt;
    }
    const std::string* last_target = jump_target(q_block->instructions.back());
    if (!(last_target != nullptr && *last_target == name &&
          std::count(q_block->successors.begin(), q_block->successors.end(), pred) == 1 &&
          !contains(q_block->successors, target) && !enters_loop_body(q, target) &&
          (!named || leading_label(*q_block).has_value()))) {
      return std::nullopt;
    }
    sources.emplace_back(q, name);
  }
  return sources;
}

std::optional<std::string> redirect(BasicBlock& block, const std::string& into,
                                    const std::string& target, const FreshLabel& fresh_label) {
  ir::Type_instruction& last = block.instructions.back();
  if (std::string* t = jump_target(last); t != nullptr && *t == into) {
    *t = target;
    return leading_label(block);
  }
  if (is_conditional(last)) {
    std::string fresh = fresh_label();
    block.instructions.push_back(ir::Label{.name = fresh});
    block.instructions.push_back(ir::Jump{.target = target});
    return fresh;
  }
  block.instructions.push_back(ir::Jump{.target = target});
  return leading_label(block);
}

void add_args_from(ir::Phi& phi, const std::string& old,
                   const std::vector<std::optional<std::string>>& labels) {
  std::vector<ir::Type_phi_arg> extra;
  for (const auto& a : phi.args) {
    const auto& arg = std::get<ir::PhiArg>(a);
    if (arg.pred_label != old) {
      continue;
    }
    for (const auto& n : labels) {
      if (!n) {
        throw InternalError("jump_thread: a Phi argument from a block without a label");
      }
      extra.push_back(ir::PhiArg{.pred_label = *n, .source = arg.source});
    }
  }
  for (auto& e : extra) {
    phi.args.push_back(std::move(e));
  }
}

void apply(Cfg& cfg, int bid, const std::vector<Thread>& threads, const FreshLabel& fresh_label) {
  BasicBlock& join = *cfg.blocks.find(bid);
  const std::string join_label = std::get<ir::Label>(join.instructions[0]).name;
  Labels threaded;
  for (const auto& t : threads) {
    threaded.insert(t.pred_label);
  }
  auto& phi = std::get<ir::Phi>(join.instructions[1]);
  std::erase_if(phi.args, [&](const ir::Type_phi_arg& a) {
    return threaded.contains(std::get<ir::PhiArg>(a).pred_label);
  });
  for (const auto& t : threads) {
    BasicBlock& target_block = *cfg.blocks.find(t.target);
    std::optional<std::string> target_label = leading_label(target_block);
    if (!target_label) {
      target_label = fresh_label();
      target_block.instructions.insert(target_block.instructions.begin(),
                                       ir::Label{.name = *target_label});
    }
    std::vector<std::optional<std::string>> from_labels;
    for (const auto& [q, into] : t.sources) {
      from_labels.push_back(redirect(*cfg.blocks.find(q), into, *target_label, fresh_label));
    }
    for (auto& instr : target_block.instructions) {
      if (auto* p = std::get_if<ir::Phi>(&instr)) {
        add_args_from(*p, join_label, from_labels);
      }
    }
  }
}

std::optional<ir::Function> thread_one_join(const ir::Function& fn, const NameSet& ssa,
                                            const FreshLabel& fresh_label) {
  Cfg cfg = tac_cfg::build_cfg(fn);
  const auto uses = count_uses(fn.instructions);
  std::map<std::string, int, std::less<>> label_block;
  for (const auto& [bid, b] : cfg.blocks) {
    if (auto label = leading_label(b)) {
      label_block.insert_or_assign(*label, bid);
    }
  }
  std::optional<std::vector<std::pair<int, PyIntSet>>> loops;
  const auto enters_loop_body = [&](int source, int target) {
    if (!loops) {
      loops = tac_cfg::natural_loops(cfg);
    }
    return std::any_of(loops->begin(), loops->end(), [&](const auto& loop) {
      return loop.second.contains(target) && target != loop.first && !loop.second.contains(source);
    });
  };

  const std::set<int> reachable = reachable_blocks(cfg);
  for (auto& [bid, b] : cfg.blocks) {
    std::erase_if(b.predecessors, [&](int p) { return !reachable.contains(p); });
  }
  for (const int bid : cfg.block_order) {
    const BasicBlock& block = cfg.blocks.at(bid);
    const auto& ins = block.instructions;
    if (ins.size() != 3 || !reachable.contains(bid)) {
      continue;
    }
    const auto* label = std::get_if<ir::Label>(&ins[0]);
    const auto* phi = std::get_if<ir::Phi>(&ins[1]);
    const ir::Type_val* condition = nullptr;
    if (const auto* jt = std::get_if<ir::JumpIfTrue>(&ins[2])) {
      condition = &jt->condition;
    } else if (const auto* jf = std::get_if<ir::JumpIfFalse>(&ins[2])) {
      condition = &jf->condition;
    }
    if (label == nullptr || phi == nullptr || condition == nullptr) {
      continue;
    }
    const auto* dst = std::get_if<ir::Var>(&phi->dst);
    if (dst == nullptr || *condition != phi->dst || !ssa.contains(dst->name)) {
      continue;
    }
    const std::size_t* n_uses = uses.find(dst->name);
    if (n_uses == nullptr || *n_uses != 1) {
      continue;
    }
    const bool jump_if_true = std::holds_alternative<ir::JumpIfTrue>(ins[2]);
    if (block.successors.size() != 2) {
      throw InternalError("jump_thread: a conditional jump's block without two successors");
    }
    const int taken = block.successors[0];
    const int fall = block.successors[1];
    if (taken == fall) {
      continue;
    }
    std::vector<Thread> threads;
    for (const auto& a : phi->args) {
      const auto& arg = std::get<ir::PhiArg>(a);
      const std::optional<asdl::Int> value = constant_value(arg.source);
      const auto pred_it = label_block.find(arg.pred_label);
      if (!value || pred_it == label_block.end() || pred_it->second == bid) {
        continue;
      }
      const int pred = pred_it->second;
      if (std::count(block.predecessors.begin(), block.predecessors.end(), pred) != 1) {
        continue;
      }
      const int target = ((*value != 0) == jump_if_true) ? taken : fall;
      if (target == bid || target == tac_cfg::kExitId) {
        continue;
      }
      bool named = false;
      for (const auto& i : cfg.blocks.at(target).instructions) {
        if (const auto* p = std::get_if<ir::Phi>(&i)) {
          for (const auto& pa : p->args) {
            named = named || std::get<ir::PhiArg>(pa).pred_label == label->name;
          }
        }
      }
      auto through = jumps_through(cfg, pred, label->name, target, named, enters_loop_body);
      if (through) {
        threads.push_back(Thread{arg.pred_label, std::move(*through), target});
        continue;
      }
      if (contains(cfg.blocks.at(pred).successors, target)) {
        continue;
      }
      if (enters_loop_body(pred, target)) {
        continue;
      }
      threads.push_back(Thread{arg.pred_label, {{pred, label->name}}, target});
    }
    if (threads.empty()) {
      continue;
    }
    apply(cfg, bid, threads, fresh_label);
    return tac_cfg::cfg_to_function(fn, cfg);
  }
  return std::nullopt;
}

}  // namespace

tac_ast::Function thread_constant_jumps(tac_ast::Function fn, const PassContext& ctx) {
  if (ctx.ssa_dsts == nullptr) {
    return fn;
  }
  Labels labels;
  for (const auto& i : fn.instructions) {
    if (const auto* label = std::get_if<ir::Label>(&i)) {
      labels.insert(label->name);
    }
  }
  int counter = 0;
  const std::string prefix = "." + fn.name + "@jthread@";
  const FreshLabel fresh_label = [&]() {
    for (;;) {
      std::string name = prefix + std::to_string(counter++);
      if (labels.insert(name).second) {
        return name;
      }
    }
  };
  for (;;) {
    std::optional<ir::Function> next = thread_one_join(fn, *ctx.ssa_dsts, fresh_label);
    if (!next) {
      return fn;
    }
    fn = std::move(*next);
  }
}

}  // namespace c6502::passes
