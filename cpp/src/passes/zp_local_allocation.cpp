// Per-function body-local zero-page pools.
//
// `zp_slot_allocation` hands each zp_abi function a private byte range
// for its parameters, disjoint from every transitive caller's range.
// `allocate_function_locals` extends the same call-graph analysis to
// body locals, the bytes the asm register allocator colors for pseudos
// that aren't parameters, statics, or spilled. Its pools are disjoint
// from every transitive caller's pool and from the parameter slots of
// every zp_abi function that can be on the call stack at the same time.
// Two functions on paths that don't overlap (neither calls the other,
// directly or not) may share addresses, since their activations are
// never on the call stack together.
//
// Eligibility
//
// A function is eligible for the private-pool model when
//
//   1. it's defined in this unit, so that its body shows its callees and
//      its demand for local bytes;
//   2. its body contains no `IndirectCall`, since an indirect callee's
//      writes can't be bounded;
//   3. it isn't on any cycle of the static direct call graph, since a
//      recursive call would overwrite the outer activation's locals that
//      are still live. The exception is a cycle whose every call from
//      one member to another is marked
//      `[[c6502::save_hardware_stack]]` (`hw_stack_cycles`): the caller
//      saves what's live across each of those calls on the hardware
//      stack, so nothing in its pool has to survive them;
//   4. every direct callee defined in this unit is eligible too, since
//      an ineligible callee's writes are unbounded and the caller's pool
//      couldn't be trusted to survive the call;
//   5. every direct callee declared `extern` here is zp_abi. Such an
//      extern is taken as a leaf whose writes are its parameter slots,
//      as its annotation promises, since its body isn't visible. A
//      callee that is neither disqualifies the caller.
//
// A call to a `[[noreturn]]` function that passes no arguments isn't an
// edge of the call graph in rules 3, 4, and 5 (call_edges.h). Once it's
// made, nothing reads the caller's pool or the pools of the functions
// the caller was called from, so the callee may be anything, even a
// function that calls the caller again.
//
// An ineligible function gets no pool, and its register allocation uses
// the conservative caller-saved and callee-saved partition of `Pool`.
//
// Algorithm
//
//   1. Build the static direct call graph of the functions `prog`
//      defines.
//   2. Determine the eligibility by the rules above.
//   3. Order the eligible functions topologically, callers before
//      callees, with each cycle's members together.
//   4. Walk them in that order. A function's forbidden bytes are every
//      transitive caller's pool, which the walk has allocated already;
//      the parameter slots of every zp_abi function that can be on the
//      call stack with it, its callers and its callees alike; and its own
//      parameter slots when it is zp_abi. It takes the lowest run of
//      `local_bytes(fn)` bytes in the window that avoids them, then in
//      the fallback range, then in the spill region above $FF.
//
// The members of one cycle aren't each other's ancestors here, so their
// pools may overlap: every call between them saves the caller's live
// values first.
//
// A pool also avoids the bytes of every `SlotShare` (slot_sharing.h)
// with a function that can be on the call stack with the pool's
// function. A share's holder keeps a value there across calls, so the
// pools of the functions it calls must stay off it even though the
// share's members are their siblings.
//
// A function's callees aren't in its forbidden bytes, since they haven't
// been allocated yet when it is. The topological order makes each callee
// see its callers' pools when its turn comes, so no two functions that
// can be on the call stack together share a byte. The parameter slots of
// the zp_abi callees are read from `abi`, where `allocate_zp_slots` has
// put their addresses already, which saves a second walk.
//
// After this, each eligible function's register allocation draws its
// colors from its pool alone, which by construction no caller, callee,
// or coexisting function uses. The split into caller-saved and
// callee-saved bytes no longer matters for these functions, since none
// of the bytes they write belongs to anyone else's live storage, so the
// prologue saves nothing.
#include "passes/zp_local_allocation.h"

#include <algorithm>
#include <deque>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "asdl/runtime.h"
#include "errors.h"
#include "passes/call_edges.h"
#include "passes/slot_sharing.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace tac = ::c6502::tac_ast;
using ::c6502::asdl::as;
using ::c6502::asdl::isa;

namespace zp_local_allocation {

NameSet determine_eligibility(const tac::Program& prog, const Abi& abi, const CallGraph& callgraph) {
  NameSet has_indirect;
  NameSet has_non_zp_abi_extern_callee;
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto& instr : fn->instructions) {
      if (isa<tac::IndirectCall>(instr)) {
        has_indirect.insert(fn->name);
      } else if (const auto* call = as<tac::FunctionCall>(instr)) {
        if (callgraph.count(call->name) != 0 || abandons_caller(instr)) {
          continue;
        }
        // An external callee, which is acceptable when it's declared
        // zp_abi, since its annotation bounds its writes to its
        // parameter slots.
        const ParamLayout* layout = abi.find(call->name);
        if (layout == nullptr || !std::holds_alternative<ZpLayout>(*layout)) {
          has_non_zp_abi_extern_callee.insert(fn->name);
        }
      }
    }
  }
  NameSet ineligible = has_indirect;
  ineligible.insert(has_non_zp_abi_extern_callee.begin(), has_non_zp_abi_extern_callee.end());
  const Cycles all_cycles = cycles(callgraph);
  const Cycles saved = hw_stack_cycles(prog);
  for (const auto& [n, members] : all_cycles) {
    if (!saved.contains(n)) {
      ineligible.insert(n);
    }
  }
  // A caller of an ineligible function is ineligible, since its private
  // pool can't survive the callee's unbounded writes.
  const CallGraph callers_of = invert(callgraph);
  std::deque<std::string> queue(ineligible.begin(), ineligible.end());
  while (!queue.empty()) {
    const std::string n = queue.front();
    queue.pop_front();
    const auto it = callers_of.find(n);
    if (it == callers_of.end()) {
      continue;
    }
    for (const auto& caller : it->second) {
      if (ineligible.insert(caller).second) {
        queue.push_back(caller);
      }
    }
  }
  NameSet out;
  for (const auto& [n, callees] : callgraph) {
    if (ineligible.count(n) == 0) {
      out.insert(n);
    }
  }
  return out;
}

CallGraph invert(const CallGraph& callgraph) {
  CallGraph out;
  for (const auto& [n, callees] : callgraph) {
    out[n];
  }
  for (const auto& [caller, callees] : callgraph) {
    for (const auto& callee : callees) {
      out[callee].insert(caller);
    }
  }
  return out;
}

}  // namespace zp_local_allocation

NameSet pool_eligible(const tac::Program& prog, const Abi& abi) {
  return zp_local_allocation::determine_eligibility(prog, abi, call_graph(prog));
}

Cycles recursive_pool_functions(const tac::Program& prog, const Abi& abi) {
  const NameSet eligible = zp_local_allocation::determine_eligibility(prog, abi, call_graph(prog));
  Cycles out;
  for (const auto& [n, members] : hw_stack_cycles(prog)) {
    if (eligible.count(n) != 0) {
      out.insert_or_assign(n, members);
    }
  }
  return out;
}


namespace {

using asdl::Int;
using Component = NameSet;

// Returns each defined function's direct callees that this unit doesn't
// define, which resolve to externs at link time. A call that abandons
// its caller isn't one.
CallGraph direct_extern_callees(const tac::Program& prog, const CallGraph& callgraph) {
  CallGraph out;
  for (const auto& [n, callees] : callgraph) {
    out[n];
  }
  for (const auto& tl : prog.top_level) {
    const auto* fn = std::get_if<tac::Function>(&tl);
    if (fn == nullptr) {
      continue;
    }
    for (const auto* call : graph_calls(*fn)) {
      if (!callgraph.contains(call->name)) {
        out[fn->name].insert(call->name);
      }
    }
  }
  return out;
}

bool is_zp(const Abi& abi, std::string_view name) {
  const ParamLayout* layout = abi.find(name);
  return layout != nullptr && std::holds_alternative<ZpLayout>(*layout);
}

// Returns the zp_abi functions whose parameter slots may be live or be
// written while `fn` is on the call stack: its transitive callers that
// are zp_abi, since their parameters sit in their slots while `fn` runs
// deeper; its transitive callees that are zp_abi, since their parameters
// are written when `fn` or anything it calls calls them; and the zp_abi
// externs that `fn` or any of its callees here calls, which have no body
// here but are on the call stack with `fn` whenever the chain calls
// them. `fn` itself is left out, and the caller adds it.
NameSet coexisting_zp_functions(const std::string& fn, const CallGraph& full_callgraph,
                                const CallGraph& extern_callees, const Abi& abi) {
  NameSet out;
  const CallGraph callers_of = zp_local_allocation::invert(full_callgraph);
  NameSet seen_anc;
  std::vector<std::string> stack;
  if (const auto it = callers_of.find(fn); it != callers_of.end()) {
    stack.assign(it->second.begin(), it->second.end());
  }
  while (!stack.empty()) {
    std::string n = std::move(stack.back());
    stack.pop_back();
    if (!seen_anc.insert(n).second) {
      continue;
    }
    if (is_zp(abi, n)) {
      out.insert(n);
    }
    if (const auto it = callers_of.find(n); it != callers_of.end()) {
      stack.insert(stack.end(), it->second.begin(), it->second.end());
    }
  }
  NameSet seen_desc{fn};
  stack = {fn};
  while (!stack.empty()) {
    std::string n = std::move(stack.back());
    stack.pop_back();
    if (const auto it = full_callgraph.find(n); it != full_callgraph.end()) {
      for (const auto& callee : it->second) {
        if (!seen_desc.insert(callee).second) {
          continue;
        }
        if (is_zp(abi, callee)) {
          out.insert(callee);
        }
        stack.push_back(callee);
      }
    }
    // The externs `n` calls have no body here, so they add no callees
    // of their own.
    if (const auto it = extern_callees.find(n); it != extern_callees.end()) {
      for (const auto& ext : it->second) {
        if (is_zp(abi, ext)) {
          out.insert(ext);
        }
      }
    }
  }
  return out;
}

Component component_of(const std::string& n, const Cycles& cyc) {
  if (const NameSet* members = cyc.find(n)) {
    return *members;
  }
  return Component{n};
}

// Returns the functions of `callgraph` in caller-before-callee order, by
// Kahn's algorithm over the graph with each cycle of `cyc` as one node,
// so that a cycle's members come together. Ready nodes are taken in the
// order of their least names.
std::vector<std::string> topological_order(const CallGraph& callgraph, const Cycles& cyc) {
  std::map<Component, std::set<Component>> succs;
  for (const auto& [n, cs] : callgraph) {
    succs[component_of(n, cyc)];
  }
  for (const auto& [n, cs] : callgraph) {
    const Component from = component_of(n, cyc);
    for (const auto& c : cs) {
      if (!callgraph.contains(c)) {
        continue;
      }
      Component to = component_of(c, cyc);
      if (to != from) {
        succs[from].insert(std::move(to));
      }
    }
  }
  std::map<Component, Int> in_degree;
  for (const auto& [c, ss] : succs) {
    in_degree.emplace(c, 0);
  }
  for (const auto& [c, ss] : succs) {
    for (const auto& s : ss) {
      in_degree[s] += 1;
    }
  }
  const auto by_least = [](const Component* a, const Component* b) {
    return *a->begin() < *b->begin();
  };
  std::deque<const Component*> queue;
  {
    std::vector<const Component*> ready;
    for (const auto& [c, d] : in_degree) {
      if (d == 0) {
        ready.push_back(&c);
      }
    }
    std::sort(ready.begin(), ready.end(), by_least);
    queue.assign(ready.begin(), ready.end());
  }
  std::vector<std::string> out;
  while (!queue.empty()) {
    const Component* c = queue.front();
    queue.pop_front();
    out.insert(out.end(), c->begin(), c->end());
    std::vector<const Component*> next;
    for (const auto& s : succs.at(*c)) {
      next.push_back(&in_degree.find(s)->first);
    }
    std::sort(next.begin(), next.end(), by_least);
    for (const Component* s : next) {
      if (--in_degree[*s] == 0) {
        queue.push_back(s);
      }
    }
  }
  return out;
}

// Returns each function's transitive callers in `callgraph`, without
// the members of its own cycle.
CallGraph ancestors(const CallGraph& callgraph, const Cycles& cyc) {
  const CallGraph callers_of = zp_local_allocation::invert(callgraph);
  CallGraph out;
  for (const auto& [n, cs] : callgraph) {
    NameSet seen;
    std::vector<std::string> stack;
    if (const auto it = callers_of.find(n); it != callers_of.end()) {
      stack.assign(it->second.begin(), it->second.end());
    }
    while (!stack.empty()) {
      std::string m = std::move(stack.back());
      stack.pop_back();
      if (!seen.insert(m).second) {
        continue;
      }
      if (const auto it = callers_of.find(m); it != callers_of.end()) {
        stack.insert(stack.end(), it->second.begin(), it->second.end());
      }
    }
    for (const auto& member : component_of(n, cyc)) {
      seen.erase(member);
    }
    out[n] = std::move(seen);
  }
  return out;
}

}  // namespace

LocalPools allocate_function_locals(const tac::Program& prog, const Abi& abi,
                                    const OrderedMap<std::string, Int>& local_bytes,
                                    const ZpAllocationOptions& options) {
  using zp_slot_allocation::find_free_range;
  const Range zp_window = options.window ? *options.window : options.pool.caller_saved();
  const Int zp_start = zp_window.start;
  const Int zp_end = zp_window.stop;

  const CallGraph callgraph = call_graph(prog);
  const CallGraph extern_callees = direct_extern_callees(prog, callgraph);
  const NameSet eligible = zp_local_allocation::determine_eligibility(prog, abi, callgraph);

  // The eligible functions only, both for the topological walk and for
  // the closures of callers and callees.
  CallGraph eligible_cg;
  for (const auto& n : eligible) {
    NameSet& out = eligible_cg[n];
    for (const auto& c : callgraph.at(n)) {
      if (eligible.contains(c)) {
        out.insert(c);
      }
    }
  }
  const Cycles cyc = cycles(eligible_cg);
  const CallGraph ancestors_of = ancestors(eligible_cg, cyc);
  const std::vector<std::string> order = topological_order(eligible_cg, cyc);

  // The parameter slots of every zp_abi function, eligible or not, since
  // an eligible function's pool has to avoid the slots of any zp_abi
  // function that can be on the call stack with it.
  std::map<std::string, std::set<Int>, std::less<>> zp_param_addrs;
  std::map<std::string, Int, std::less<>> slot_addr;
  for (const auto& [n, layout] : abi) {
    const auto* zp = std::get_if<ZpLayout>(&layout);
    if (zp == nullptr) {
      continue;
    }
    auto& addrs = zp_param_addrs[n];
    for (std::size_t i = 0; i < zp->addrs.size(); ++i) {
      if (zp->addrs[i]) {
        addrs.insert(*zp->addrs[i]);
        if (i < zp->slot_symbols.size()) {
          slot_addr.insert_or_assign(zp->slot_symbols[i], *zp->addrs[i]);
        }
      }
    }
  }
  const ReachSets reach = reach_sets(prog);

  LocalPools local_pools;
  for (const auto& fn : order) {
    const Int* wanted = local_bytes.find(fn);
    const Int size = wanted == nullptr ? 0 : *wanted;
    if (size == 0) {
      local_pools.insert_or_assign(fn, {});
      continue;
    }
    std::set<Int> forbidden = options.reserved;
    // The callers' pools, which the walk has allocated already.
    if (const auto it = ancestors_of.find(fn); it != ancestors_of.end()) {
      for (const auto& anc : it->second) {
        if (const auto* pool = local_pools.find(anc)) {
          forbidden.insert(pool->begin(), pool->end());
        }
      }
    }
    // The slots of every zp_abi function whose parameters are written
    // while `fn` is on the call stack, and `fn`'s own.
    for (const auto& other : coexisting_zp_functions(fn, callgraph, extern_callees, abi)) {
      if (const auto it = zp_param_addrs.find(other); it != zp_param_addrs.end()) {
        forbidden.insert(it->second.begin(), it->second.end());
      }
    }
    if (const auto it = zp_param_addrs.find(fn); it != zp_param_addrs.end()) {
      forbidden.insert(it->second.begin(), it->second.end());
    }
    for (const std::size_t s : share_conflicts(fn, options.shares, reach)) {
      for (const auto& sym : options.shares[s].slots()) {
        if (const auto it = slot_addr.find(sym); it != slot_addr.end()) {
          forbidden.insert(it->second);
        }
      }
    }

    std::optional<std::vector<Int>> addrs = find_free_range(forbidden, size, zp_start, zp_end);
    if (!addrs && options.fallback) {
      addrs = find_free_range(forbidden, size, options.fallback->start, options.fallback->stop);
    }
    if (!addrs) {
      addrs = find_free_range(forbidden, size, options.spill_start, options.spill_end);
    }
    if (!addrs) {
      throw CompileError(
          "ZpLocalAllocationError",
          "can't allocate " + pyrepr::integer(size) + "-byte local pool for function `" + fn +
              "`: no contiguous gap in either ZP ($" + pyrepr::hex(zp_start, 2) + "-$" +
              pyrepr::hex(zp_end - 1, 2) + ") or spill region ($" +
              pyrepr::hex(options.spill_start, 4) + "-$" + pyrepr::hex(options.spill_end - 1, 4) +
              ") avoids the " + std::to_string(forbidden.size()) + " forbidden bytes");
    }
    local_pools.insert_or_assign(fn, std::move(*addrs));
  }
  return local_pools;
}

OrderedMap<std::string, Int> build_local_slot_symbols(
    const LocalPools& local_pools,
    const OrderedMap<std::string, std::vector<std::string>>* slot_names_by_fn) {
  OrderedMap<std::string, Int> out;
  for (const auto& [fn_name, pool] : local_pools) {
    const std::vector<std::string>* names =
        slot_names_by_fn == nullptr ? nullptr : slot_names_by_fn->find(fn_name);
    for (std::size_t k = 0; k < pool.size(); ++k) {
      if (names != nullptr) {
        // `zip` stops at the shorter of the two.
        if (k >= names->size()) {
          break;
        }
        out.insert_or_assign((*names)[k], pool[k]);
      } else {
        out.insert_or_assign("__local_" + fn_name + "__" + std::to_string(k), pool[k]);
      }
    }
  }
  return out;
}

}  // namespace c6502::passes
