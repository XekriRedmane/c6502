/* TAC pass: send an edge into a join straight to where the join's test
sends the constant that the edge brings.

The shape. An inlined function that returns a constant on some paths,
such as

    static bool find(uint8_t v) {
        ...
        do { if (a[i] == v) { hits++; return true; } } while (--i != 0);
        return false;
    }
    void f(uint8_t v) { if (find(v)) return; hits += 2; }

leaves its result in a `Phi` at the copy's end label, and the caller's
test of the result right after it:

    .f@ssa_block@1:
        hits = hits + 1
        Jump(.i0@end)
    ...
        JumpIfTrue(%23, .i0_loop@0_start)
    .i0_loop@0_break:
    .i0@end:
        %11.3 = Phi(1 from .f@ssa_block@1, 0 from .i0_loop@0_break)
        JumpIfFalse(%11.3, .if_end@1)
        Ret

Each edge into `.i0@end` brings a constant, so the test is decided on
each of them. The pass sends the first edge to the `Ret` (under a new
label) and the second to `.if_end@1`, which leaves `.i0@end` with no
way in, and unreachable-code elimination then removes it. The 6502 code
had loaded the constant and branched on it, `LDA #$00; BNE`, on every
such path. `&&` and `||` whose result is tested leave the same shape,
with a 0 or a 1 from each edge.

The rule. A block B made of its label, one `Phi` with an SSA
destination `t`, and a `JumpIfTrue` or `JumpIfFalse` on `t`, where
nothing else reads `t`, is a candidate. For each argument of the `Phi`
that is an integer constant, from a predecessor P with one edge into B,
the constant decides the jump, and the edge goes to the successor S it
picks (`JumpIfTrue` takes its target when the constant isn't 0). P's
jump to B goes to S instead. When P falls into B without a jump of its
own, a `Jump` to S goes at P's end, and when P ends with a conditional
jump to somewhere else, the `Jump` goes in a new block after it, under
a new label, so that it can be named as a predecessor in S's `Phi`s. S
gets a label when it starts without one, which can only happen when B
falls through to it. The argument leaves B's `Phi`, and each `Phi` in S
that has an argument from B gets the same argument from P (or from the
new block).

When P holds nothing but its label, or its label and a `Jump` to B,
and every way into P is a jump to its label, the jumps into P go to S
instead, and P is left with no way in. `&&` and `||` leave such a P for
the edges their tests short-circuit (`.and_false@N`, `.or_true@N`), and
a `Jump` to S at P's end would only be a second jump on the way. Each
block that jumps into P must then meet the conditions below in P's
place, and start with a label when S's `Phi`s name B, so that they can
name it instead. Otherwise P's own edge goes to S as above.

A block that an earlier step left with no way in still has its edges in
the graph until unreachable-code elimination drops it, so the pass
doesn't count them.

An edge is left alone when

  - S is B itself, or B's two successors are the same block, or S is
    the exit (B is the last block and falls off the end);
  - P has two edges into B, or P is B;
  - P already has an edge to S, since S's `Phi`s would then need two
    arguments from P;
  - S is inside a natural loop, other than as its header, that P is
    outside of. The new edge would enter that loop's body without
    passing its header, and the loop would have two entries.

Why it's sound. The jump in B reads only `t`, and on the edge from P,
`t` is the constant, so control leaves B for S on that edge whatever
else holds. B computes nothing other than `t`, which nothing else
reads, so skipping B skips no effect and no value that a later
instruction reads. An empty P whose jumps go to S skips nothing either.
A `Phi` in S that took a value `v` from B takes the same `v` from the
block whose edge moved. `v` isn't `t`, which only the jump reads, so it
is defined outside B (and outside an empty P), in a block D that
dominates B. A path from the entry to that block, followed by its edge
into B, is a path to B, which passes through D before B, so D dominates
the block too (or is the block), and `v` is defined on every path to
the block's end.

A loop that holds the edge's new source and not S (an exit), or that
holds both, keeps its entries. A loop with S as its header gains an
edge into the header. Any new cycle through a moved edge Q -> S was
already a cycle Q -> (P ->) B -> S -> ... -> Q, whose header dominated
each of its blocks, so the new edge doesn't make the graph irreducible.

Where it runs. In the fixed point, right before unreachable-code
elimination, which drops B once its last edge is gone (and with it B's
`Phi`, left with no arguments) and drops the stale arguments from B in
S's `Phi`s. */
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

// Returns the target field of a jump of any kind, or null.
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

// Returns whether `instr` is a conditional jump.
bool is_conditional(const ir::Type_instruction& instr) {
  return jump_target(instr) != nullptr && !std::holds_alternative<ir::Jump>(instr);
}

// Returns the name of the label `block` starts with, or none.
std::optional<std::string> leading_label(const BasicBlock& block) {
  if (!block.instructions.empty()) {
    if (const auto* label = std::get_if<ir::Label>(&block.instructions.front())) {
      return label->name;
    }
  }
  return std::nullopt;
}

// Returns the value of an integer constant, or none for any other
// operand.
std::optional<asdl::Int> constant_value(const ir::Type_val& v) {
  const auto* c = std::get_if<ir::Constant>(&v);
  if (c != nullptr && constant_folding::is_integer_const(c->const_)) {
    return bit_precise::raw_value(c->const_);
  }
  return std::nullopt;
}

// One edge to send to `target`: the label of the `Phi` argument it
// brings, and the blocks whose jumps change, with the label each jumps
// to now. `sources` is `[(P, join label)]` for the predecessor P itself,
// or `[(Q, P's label), ...]` for the blocks that jump into an empty P.
struct Thread {
  std::string pred_label;
  std::vector<std::pair<int, std::string>> sources;
  int target = 0;
};

// Returns the ids of the blocks a walk forward from ENTRY reaches.
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

// Returns the blocks, each with `pred`'s label, whose jumps into `pred`
// can go straight to `target`, when `pred` holds its label and nothing
// else but a `Jump` to the join and is reached only by jumps, or none.
// Each of them must jump into `pred` once, have no edge to `target`
// already, not enter a loop's body by the new edge, and start with a
// label when `target`'s `Phi`s name the join (`named`), so that they can
// name it too.
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

// Sends `block`'s edge into the block labeled `into` to the block
// labeled `target`, and returns the label that names `block` (or the
// new block that holds its jump) as a predecessor of `target`.
std::optional<std::string> redirect(BasicBlock& block, const std::string& into,
                                    const std::string& target, const FreshLabel& fresh_label) {
  ir::Type_instruction& last = block.instructions.back();
  if (std::string* t = jump_target(last); t != nullptr && *t == into) {
    *t = target;
    return leading_label(block);
  }
  if (is_conditional(last)) {
    // The edge is the fall-through after a conditional jump, so the jump
    // to `target` goes in a block of its own.
    std::string fresh = fresh_label();
    block.instructions.push_back(ir::Label{.name = fresh});
    block.instructions.push_back(ir::Jump{.target = target});
    return fresh;
  }
  block.instructions.push_back(ir::Jump{.target = target});
  return leading_label(block);
}

// Gives `phi` a copy of its argument from `old`, if it has one, coming
// from each label of `labels` as well.
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
        // Python would build a PhiArg with a pred_label of None, which
        // `named` rules out.
        throw InternalError("jump_thread: a Phi argument from a block without a label");
      }
      extra.push_back(ir::PhiArg{.pred_label = *n, .source = arg.source});
    }
  }
  for (auto& e : extra) {
    phi.args.push_back(std::move(e));
  }
}

// Sends each edge of `threads` to its target, with the bookkeeping the
// module comment describes. Python builds new instructions and `Phi`s
// here in place of changing those of the function, since the driver
// compares the result with its input. The C++ blocks own their copies.
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

// Threads the constant edges into the first join that has one, and
// returns the new function, or none when no join has one.
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

  // A block that an earlier threading left with no way in still has
  // its edges in the graph, until unreachable-code elimination drops it.
  // They aren't edges.
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
  // One join at a time, since threading one changes the blocks the next
  // one sees.
  for (;;) {
    std::optional<ir::Function> next = thread_one_join(fn, *ctx.ssa_dsts, fresh_label);
    if (!next) {
      return fn;
    }
    fn = std::move(*next);
  }
}

}  // namespace c6502::passes
