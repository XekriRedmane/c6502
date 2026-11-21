// Saving the values live across a recursive call on the hardware stack.
//
// A function on a cycle of the call graph normally can't keep its locals
// in a private zero-page pool, as other functions do
// (passes/zp_local_allocation.py), because the call that recurses runs
// the same code and would overwrite them. Such a function uses the
// shared caller-saved and callee-saved split instead, and saves every
// callee-saved byte it uses in its prologue and restores it in its
// epilogue, on every call, recursive or not.
//
// `[[c6502::save_hardware_stack]]` before a statement or a declaration
// marks the calls in it (`tac_ast::FunctionCall::hw_stack_save`), and
// the caller may then save the values it reads after those calls on the
// hardware stack. The compiler can't bound how deep the
// recursion goes, so the user vouches for the stack's 256 bytes.
//
// When every call that stays inside a cycle is marked
// (`hw_stack_cycles`), and the cycle is otherwise eligible, the functions
// on it get private pools like any other function. A function on the
// cycle has its pool to itself except for the calls that recurse, so
// `tac_to_asm` saves around each of those the values live across it
// (`plan_saves`):
//
//     LDA v      ; just after v's last definition in the call's block,
//     PHA        ; or else just before the call
//     JSR f
//     PLA        ; in the reverse order, after the call's result is
//     STA v      ; stored
//
// With nothing live across the call in zero page, nothing has to be
// saved at entry or exit. A value copied from a global only to be saved
// becomes `LDA g; PHA` once the copy is folded away.
#include "passes/recursion_saves.h"

#include <algorithm>
#include <functional>
#include <optional>
#include <tuple>

#include "asdl/runtime.h"
#include "errors.h"
#include "passes/call_edges.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace {

constexpr std::string_view kAttribute = "`[[c6502::save_hardware_stack]]`";

bool is_volatile_qualified(const c99_ast::Type_data_type& t) {
  const c99_ast::Type_data_type* cur = &t;
  while (const auto* c = as<c99_ast::Const>(*cur)) {
    cur = &*c->referenced_type;
  }
  return isa<c99_ast::Volatile>(*cur);
}

// Returns whether `instr` ends a block for the walk of `plan_saves`,
// which keeps its own list (Python's `_ends_block`).
bool ends_block(const tac::Type_instruction& instr) {
  return isa<tac::Ret, tac::Jump, tac::JumpIfTrue, tac::JumpIfFalse, tac::JumpIfCmp,
             tac::JumpIfMasked>(instr);
}

// Returns whether `instr` touches no memory but the variables it names,
// so that a global it doesn't name can be stored before it (Python's
// `_REGISTER_ONLY`).
bool register_only(const tac::Type_instruction& instr) {
  return isa<tac::Copy, tac::Binary, tac::Unary, tac::SignExtend, tac::ZeroExtend,
             tac::Truncate>(instr);
}

// Returns the names `instr` reads or writes.
NameSet names_of(const tac::Type_instruction& instr) {
  NameSet out;
  for (const auto n : uses_in(instr)) {
    out.emplace(n);
  }
  for (const auto n : recursion_saves::defs_in(instr)) {
    out.emplace(n);
  }
  return out;
}

const std::string* var_name(const tac::Type_val& v) {
  const auto* var = std::get_if<tac::Var>(&v);
  return var == nullptr ? nullptr : &var->name;
}

}  // namespace

CallGraph call_graph(const tac::Program& prog) {
  CallGraph graph;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      graph.emplace(fn->name, NameSet{});
    }
  }
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    NameSet& callees = graph.find(fn->name)->second;
    for (const auto* call : graph_calls(*fn)) {
      if (graph.count(call->name) != 0) {
        callees.insert(call->name);
      }
    }
  }
  return graph;
}

Cycles cycles(const CallGraph& graph) {
  // Tarjan's algorithm, visiting the functions and each one's callees in
  // sorted order, as Python does.
  std::map<std::string, std::size_t, std::less<>> index;
  std::map<std::string, std::size_t, std::less<>> lowlink;
  std::vector<std::string> stack;
  NameSet on_stack;
  Cycles out;

  std::function<void(const std::string&)> visit = [&](const std::string& v) {
    const std::size_t n = index.size();
    index[v] = n;
    lowlink[v] = n;
    stack.push_back(v);
    on_stack.insert(v);
    const auto it = graph.find(v);
    if (it != graph.end()) {
      for (const std::string& w : it->second) {
        if (graph.count(w) == 0) {
          continue;
        }
        if (index.count(w) == 0) {
          visit(w);
          lowlink[v] = std::min(lowlink[v], lowlink[w]);
        } else if (on_stack.count(w) != 0) {
          lowlink[v] = std::min(lowlink[v], index[w]);
        }
      }
    }
    if (lowlink[v] == index[v]) {
      std::vector<std::string> component;
      while (true) {
        std::string w = stack.back();
        stack.pop_back();
        on_stack.erase(w);
        component.push_back(w);
        if (w == v) {
          break;
        }
      }
      const bool self_call = it != graph.end() && it->second.count(v) != 0;
      if (component.size() > 1 || self_call) {
        const NameSet members(component.begin(), component.end());
        for (const std::string& w : component) {
          out.insert_or_assign(w, members);
        }
      }
    }
  };

  for (const auto& [n, callees] : graph) {
    if (index.count(n) == 0) {
      visit(n);
    }
  }
  return out;
}

void check_marked_calls(const tac::Program& prog) {
  const Cycles cyc = cycles(call_graph(prog));
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      const auto* call = as<tac::FunctionCall>(instr);
      if (call == nullptr || !call->hw_stack_save) {
        continue;
      }
      const std::string& callee = call->name;
      const std::string& caller = fn->name;
      if (abandons_caller(instr)) {
        throw CompileError(
            "RecursionSaveError",
            std::string(kAttribute) + " is on a call to '" + callee + "' in '" + caller +
                "', but '" + callee +
                "' is declared `[[noreturn]]` and takes no arguments, so nothing of '" + caller +
                "' is needed after the call and nothing has to be saved");
      }
      const NameSet* members = cyc.find(caller);
      if (members == nullptr || members->count(callee) == 0) {
        throw CompileError("RecursionSaveError",
                           std::string(kAttribute) + " is on a call to '" + callee + "' in '" +
                               caller + "', but '" + callee + "' can't call '" + caller +
                               "' again, so the call can't recurse");
      }
    }
  }
}

Cycles hw_stack_cycles(const tac::Program& prog) {
  const Cycles cyc = cycles(call_graph(prog));
  NameSet unmarked;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    const NameSet* members = cyc.find(fn->name);
    if (members == nullptr) {
      continue;
    }
    for (const auto* call : graph_calls(*fn)) {
      if (members->count(call->name) != 0 && !call->hw_stack_save) {
        unmarked.insert(fn->name);
      }
    }
  }
  NameSet bad;
  for (const auto& n : unmarked) {
    const NameSet& members = cyc.at(n);
    bad.insert(members.begin(), members.end());
  }
  Cycles out;
  for (const auto& [n, members] : cyc) {
    if (bad.count(n) == 0) {
      out.insert_or_assign(n, members);
    }
  }
  return out;
}

namespace {

// Returns the index of a `Copy(name, g)` to a global `g` that a pull
// after `call` can do instead: the first instruction after the call to
// name `name`, in the same block, after which `name` is dead, with only
// `register_only` instructions that don't name `g` before it.
template <class Plain>
std::optional<std::size_t> store_to_global(const tac::Function& fn, std::size_t call,
                                           const std::string& name, const NameSet& statics,
                                           const std::vector<NameSet>& live_out, Plain plain) {
  std::vector<const tac::Type_instruction*> between;
  for (std::size_t j = call + 1; j < fn.instructions.size(); ++j) {
    const auto& instr = fn.instructions[j];
    if (names_of(instr).count(name) != 0) {
      const auto* copy = as<tac::Copy>(instr);
      if (copy == nullptr) {
        return std::nullopt;
      }
      const std::string* src = var_name(copy->src);
      const std::string* dst = var_name(copy->dst);
      if (src == nullptr || *src != name || dst == nullptr || statics.count(*dst) == 0 ||
          !plain(copy->dst) || live_out[j].count(name) != 0) {
        return std::nullopt;
      }
      for (const auto* b : between) {
        if (names_of(*b).count(*dst) != 0) {
          return std::nullopt;
        }
      }
      return j;
    }
    if (!register_only(instr)) {
      return std::nullopt;
    }
    between.push_back(&instr);
  }
  return std::nullopt;
}

}  // namespace

SavePlan plan_saves(const tac::Function& fn, const NameSet& members, const SymbolTable& symbols) {
  // A value its last definition copies from somewhere is pushed from
  // there, so `v = g` then `LDA v; PHA` becomes `LDA g; PHA` once the
  // copy is found dead. A value whose first use after the call copies it
  // to a global is pulled straight into the global, when nothing between
  // the call and the copy can see the global.
  NameSet statics;
  for (const auto& [n, sym] : symbols) {
    if (std::holds_alternative<StaticAttr>(sym.attrs)) {
      statics.insert(n);
    }
  }
  NameSet in_memory = statics;
  in_memory.insert(fn.params.begin(), fn.params.end());
  for (const auto& instr : fn.instructions) {
    if (const auto* get = as<tac::GetAddress>(instr)) {
      if (const std::string* n = var_name(get->operand)) {
        in_memory.insert(*n);
      }
    } else if (const auto* load = as<tac::IndexedLoad>(instr)) {
      in_memory.insert(load->name);
    } else if (const auto* store = as<tac::IndexedSymbolStore>(instr)) {
      in_memory.insert(store->name);
    }
  }

  // Returns whether `val` can be read twice, or written early.
  const auto plain = [&](const tac::Type_val& val) {
    const std::string* n = var_name(val);
    if (n == nullptr) {
      return true;
    }
    const Symbol* sym = symbols.find(*n);
    return sym != nullptr && !is_volatile_qualified(sym->type);
  };

  const std::vector<NameSet> live_out = recursion_saves::live_out(fn);
  SavePlan plan;
  std::size_t block_start = 0;
  for (std::size_t i = 0; i < fn.instructions.size(); ++i) {
    const auto& instr = fn.instructions[i];
    if (isa<tac::Label>(instr)) {
      block_start = i;
    }
    const auto* call = as<tac::FunctionCall>(instr);
    if (call == nullptr || members.count(call->name) == 0) {
      if (ends_block(instr)) {
        block_start = i + 1;
      }
      continue;
    }
    NameSet defined;
    for (const auto d : recursion_saves::defs_in(instr)) {
      defined.emplace(d);
    }
    // The set's order is sorted, as Python's `sorted` gives.
    std::vector<std::string> saved;
    for (const auto& n : live_out[i]) {
      if (defined.count(n) == 0 && in_memory.count(n) == 0) {
        saved.push_back(n);
      }
    }
    // Each value's last definition between the block's start (or the
    // previous saved call) and this call.
    std::map<std::string, std::size_t, std::less<>> last_def;
    const NameSet saved_set(saved.begin(), saved.end());
    for (std::size_t k = block_start; k < i; ++k) {
      for (const auto d : recursion_saves::defs_in(fn.instructions[k])) {
        if (saved_set.count(d) != 0) {
          last_def[std::string(d)] = k;
        }
      }
    }
    const auto last_or_call = [&](const std::string& n) {
      const auto it = last_def.find(n);
      return it == last_def.end() ? i : it->second;
    };
    std::vector<std::string> order = saved;
    std::stable_sort(order.begin(), order.end(), [&](const std::string& a, const std::string& b) {
      return std::make_tuple(last_or_call(a), std::cref(a)) <
             std::make_tuple(last_or_call(b), std::cref(b));
    });
    for (const auto& n : order) {
      tac::Type_val src = tac::Var{.name = n};
      const auto it = last_def.find(n);
      if (it == last_def.end()) {
        plan.push_before[static_cast<int>(i)].emplace_back(n, src);
        continue;
      }
      const auto& d = fn.instructions[it->second];
      if (const auto* copy = as<tac::Copy>(d); copy != nullptr && plain(copy->src)) {
        src = copy->src;
      }
      plan.push_after[static_cast<int>(it->second)].emplace_back(n, src);
    }
    std::vector<SavedValue> pops;
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
      const std::string& n = *it;
      const auto j = store_to_global(fn, i, n, statics, live_out, plain);
      if (!j || plan.skip.count(static_cast<int>(*j)) != 0) {
        pops.emplace_back(n, tac::Var{.name = n});
      } else {
        plan.skip.insert(static_cast<int>(*j));
        pops.emplace_back(n, std::get<tac::Copy>(fn.instructions[*j]).dst);
      }
    }
    plan.pop_after.insert_or_assign(static_cast<int>(i), std::move(pops));
    block_start = i + 1;
  }
  return plan;
}

namespace recursion_saves {

std::vector<std::string_view> defs_in(const tac::Type_instruction& instr) {
  if (const auto* load = as<tac::IndirectIndexedLoad>(instr)) {
    if (const std::string* n = var_name(load->dst)) {
      return {*n};
    }
    return {};
  }
  return ::c6502::passes::defs_in(instr);
}

std::vector<NameSet> live_out(const tac::Function& fn) {
  const tac_cfg::Cfg cfg = tac_cfg::build_cfg(fn);
  std::map<int, std::size_t> starts;
  std::size_t pos = 0;
  for (const int bid : cfg.block_order) {
    starts[bid] = pos;
    pos += cfg.blocks.at(bid).instructions.size();
  }
  if (pos != fn.instructions.size()) {
    throw InternalError("live_out: the blocks don't hold every instruction");
  }
  std::map<int, NameSet> use;
  std::map<int, NameSet> kill;
  for (const int bid : cfg.block_order) {
    NameSet u;
    NameSet k;
    for (const auto& instr : cfg.blocks.at(bid).instructions) {
      for (const auto n : uses_in(instr)) {
        if (k.count(n) == 0) {
          u.emplace(n);
        }
      }
      for (const auto n : defs_in(instr)) {
        k.emplace(n);
      }
    }
    use[bid] = std::move(u);
    kill[bid] = std::move(k);
  }
  std::map<int, NameSet> live_in;
  for (const auto& [bid, block] : cfg.blocks) {
    live_in[bid];
  }
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto it = cfg.block_order.rbegin(); it != cfg.block_order.rend(); ++it) {
      const int bid = *it;
      NameSet out;
      for (const int s : cfg.blocks.at(bid).successors) {
        const NameSet& in = live_in[s];
        out.insert(in.begin(), in.end());
      }
      NameSet new_in = use[bid];
      for (const auto& n : out) {
        if (kill[bid].count(n) == 0) {
          new_in.insert(n);
        }
      }
      if (new_in != live_in[bid]) {
        live_in[bid] = std::move(new_in);
        changed = true;
      }
    }
  }
  std::vector<NameSet> result(fn.instructions.size());
  for (const int bid : cfg.block_order) {
    NameSet live;
    for (const int s : cfg.blocks.at(bid).successors) {
      const NameSet& in = live_in[s];
      live.insert(in.begin(), in.end());
    }
    const auto& instrs = cfg.blocks.at(bid).instructions;
    for (std::size_t j = instrs.size(); j-- > 0;) {
      result[starts[bid] + j] = live;
      for (const auto n : defs_in(instrs[j])) {
        const auto found = live.find(n);
        if (found != live.end()) {
          live.erase(found);
        }
      }
      for (const auto n : uses_in(instrs[j])) {
        live.emplace(n);
      }
    }
  }
  return result;
}

}  // namespace recursion_saves

}  // namespace c6502::passes
