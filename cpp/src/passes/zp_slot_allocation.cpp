// Zero-page slot allocation for `[[c6502::zp_abi]]` functions.
//
// Background
//
// `select_abi` decides which functions get the zero-page convention, and
// `allocate_zp_slots` decides where each function's parameter bytes
// live. No two functions that can be on the call stack at the same time
// may share a slot byte. If function `A` calls function `B`, directly or
// not, then `A`'s parameter slots must be disjoint from `B`'s, since
// otherwise `B`'s callers' argument writes would overwrite `A`'s
// parameters while they are still live.
//
// It runs after `select_abi` and before `tac_to_asm`. The asm IR refers
// to every slot by its symbol, `Data(<sym>, 0)`, and the output binds
// each symbol with an `EQU` line. dasm picks zero-page or absolute
// addressing from the symbol's value, so when zero page is full and a
// function's slots spill above $FF, the IR doesn't change. The same `LDA
// <sym>` and `STA <sym>` assemble as 3-byte absolute instructions.
//
// Algorithm
//
// 1. Build the call graph: an edge `caller -> callee` between two zp_abi
//    functions when the caller can reach the callee through calls,
//    directly or through functions that aren't zp_abi. A zp_abi function
//    may call a soft-stack function that has a private pool, which may
//    call zp_abi functions in turn, and their slots must stay apart from
//    the caller's. `select_abi` rejects recursion, indirect calls, and
//    taken addresses first, so the graph is a DAG of zp_abi functions.
// 2. Order the DAG topologically (callers before their callees, by
//    Kahn's algorithm) and walk it in that order. Each function's
//    forbidden bytes are the bytes already given to its transitive
//    callers, since every ancestor's parameters are live while it runs.
//    It takes the lowest run of its parameter bytes that avoids them,
//    from the window (the pool's caller-saved half, $80-$BF, unless
//    given), then from the fallback range, then from the spill region
//    ($0200-$FFFF, anywhere in RAM the output can address).
// 3. Sibling functions, neither of which calls the other, may share
//    addresses, since their activations are never on the call stack
//    together. The topological order allows that, since each sibling
//    picks the lowest run free of its own ancestors.
//
// Slots that get no range
//
// A byte passed in a register has no slot: the caller loads the
// register, and `tac_to_asm` copies it into a local of the callee. A
// byte of a `zeropage(addr)` parameter is bound to that address, which
// `reserved` already keeps every other slot off.
//
// Shared slots
//
// `shares` (slot_sharing.h) lists sets of slots that hold one value for
// several functions, so they get one address. A share's bytes are
// allocated at the turn of the last of its members' functions in the
// order, as one contiguous run, and avoid every byte already given to a
// function that can be on the call stack with any function in the share
// (its members' owners and its holders). The functions allocated after
// it avoid it in turn, the same way. Its members' other slots are
// allocated as usual.
//
// Externs
//
// A zp_abi extern declared in this unit is in `abi`, from `select_abi`,
// but has no body to walk. It is a leaf of the local call graph, and its
// slots are allocated like any other zp_abi function's, so that its call
// sites in this unit bind to the addresses of the same `EQU` lines. The
// linker allocates them again for the whole program.
//
// Python keys the shares by `id()`; here a share is keyed by its index
// in `shares`.
#include "passes/zp_slot_allocation.h"

#include <algorithm>
#include <deque>
#include <map>
#include <string>

#include "errors.h"
#include "passes/call_edges.h"
#include "pyrepr.h"

namespace c6502::passes {

namespace {

namespace tac = ::c6502::tac_ast;
using asdl::Int;
using Graph = std::map<std::string, NameSet, std::less<>>;

CompileError allocation_error(const std::string& message) {
  return CompileError("ZpSlotAllocationError", message);
}

// Returns Python's repr of a sorted list of names, `['a', 'b']`.
std::string names_repr(const std::vector<std::string>& names) {
  std::string out = "[";
  for (std::size_t i = 0; i < names.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += pyrepr::string(names[i]);
  }
  return out + "]";
}

// Returns the nodes of `callees_of` in caller-before-callee order, by
// Kahn's algorithm with the ready nodes taken in sorted order. Throws
// when there is a cycle, which `select_abi` rejects first, so that a bug
// there doesn't misallocate without a word.
std::vector<std::string> topological_order(const Graph& callees_of) {
  // The in-degree of a node is its number of callers. Roots have none.
  std::map<std::string, Int, std::less<>> in_degree;
  for (const auto& [n, cs] : callees_of) {
    in_degree.emplace(n, 0);
  }
  for (const auto& [n, cs] : callees_of) {
    for (const auto& c : cs) {
      in_degree[c] += 1;
    }
  }
  std::deque<std::string> queue;
  for (const auto& [n, d] : in_degree) {
    if (d == 0) {
      queue.push_back(n);
    }
  }
  std::vector<std::string> out;
  while (!queue.empty()) {
    std::string n = std::move(queue.front());
    queue.pop_front();
    out.push_back(n);
    if (const auto it = callees_of.find(n); it != callees_of.end()) {
      for (const auto& c : it->second) {
        if (--in_degree[c] == 0) {
          queue.push_back(c);
        }
      }
    }
  }
  if (out.size() != in_degree.size()) {
    std::vector<std::string> remaining;
    for (const auto& [n, d] : in_degree) {
      if (d > 0) {
        remaining.push_back(n);
      }
    }
    throw allocation_error("cycle detected in zp_abi call graph involving " +
                           names_repr(remaining) +
                           " — select_abi should have rejected this; internal error");
  }
  return out;
}

// Returns the zp_abi functions each zp_abi function calls directly or
// through a chain of calls whose functions in between aren't zp_abi, and
// each zp_abi function's transitive callers, itself left out. An extern
// is a leaf, a target of edges and never a source. A call that abandons
// its caller isn't an edge, since nothing reads the caller's parameters,
// or its callers', once it's made.
std::pair<Graph, Graph> build_callgraph(const tac::Program& prog, const Abi& abi) {
  NameSet zp_names;
  for (const auto& [n, layout] : abi) {
    if (std::holds_alternative<ZpLayout>(layout)) {
      zp_names.insert(n);
    }
  }
  Graph direct;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      NameSet callees;
      for (const auto* call : graph_calls(*fn)) {
        callees.insert(call->name);
      }
      direct[fn->name] = std::move(callees);
    }
  }
  Graph callees_of;
  for (const auto& name : zp_names) {
    NameSet& out = callees_of[name];
    NameSet seen;
    std::vector<std::string> stack;
    if (const auto it = direct.find(name); it != direct.end()) {
      stack.assign(it->second.begin(), it->second.end());
    }
    while (!stack.empty()) {
      std::string c = std::move(stack.back());
      stack.pop_back();
      if (!seen.insert(c).second) {
        continue;
      }
      if (zp_names.contains(c)) {
        out.insert(c);
      } else if (const auto it = direct.find(c); it != direct.end()) {
        stack.insert(stack.end(), it->second.begin(), it->second.end());
      }
    }
  }
  Graph callers_of;
  for (const auto& n : zp_names) {
    callers_of[n];
  }
  for (const auto& [caller, cs] : callees_of) {
    for (const auto& callee : cs) {
      callers_of[callee].insert(caller);
    }
  }
  // The transitive closure of the callers, walked in topological order,
  // so that each node's callers are complete when it's reached.
  Graph ancestors_of;
  for (const auto& n : zp_names) {
    ancestors_of[n];
  }
  for (const auto& node : topological_order(callees_of)) {
    NameSet anc;
    for (const auto& caller : callers_of[node]) {
      anc.insert(caller);
      const NameSet& theirs = ancestors_of[caller];
      anc.insert(theirs.begin(), theirs.end());
    }
    ancestors_of[node] = std::move(anc);
  }
  return {std::move(callees_of), std::move(ancestors_of)};
}

}  // namespace

namespace zp_slot_allocation {

std::optional<std::vector<Int>> find_free_range(const std::set<Int>& forbidden, Int n_bytes,
                                                Int lo, Int hi) {
  Int cur = lo;
  while (cur + n_bytes <= hi) {
    // The highest conflict in [cur, cur + n_bytes): no start up to it
    // gives a free run.
    std::optional<Int> conflict_at;
    for (Int k = 0; k < n_bytes; ++k) {
      if (forbidden.contains(cur + k)) {
        conflict_at = cur + k;
      }
    }
    if (!conflict_at) {
      std::vector<Int> out;
      for (Int a = cur; a < cur + n_bytes; ++a) {
        out.push_back(a);
      }
      return out;
    }
    cur = *conflict_at + 1;
  }
  return std::nullopt;
}

}  // namespace zp_slot_allocation

std::pair<Abi, OrderedMap<std::string, Int>> allocate_zp_slots(const tac::Program& prog,
                                                               const Abi& abi,
                                                               const ZpAllocationOptions& options) {
  using zp_slot_allocation::find_free_range;
  const Range zp_window = options.window ? *options.window : options.pool.caller_saved();
  const Int zp_start = zp_window.start;
  const Int zp_end = zp_window.stop;
  const Int spill_start = options.spill_start;
  const Int spill_end = options.spill_end;

  const auto [callgraph, ancestors] = build_callgraph(prog, abi);
  const ReachSets reach = reach_sets(prog);
  const std::vector<std::string> order = topological_order(callgraph);

  const std::vector<SlotShare>& shares = options.shares;
  NameSet shared;
  for (const auto& share : shares) {
    const NameSet s = share.slots();
    shared.insert(s.begin(), s.end());
  }
  // Each share is allocated at the turn of the last of its members'
  // functions, the first of them when several are last.
  std::map<std::string, Int, std::less<>> position;
  for (std::size_t k = 0; k < order.size(); ++k) {
    position.emplace(order[k], static_cast<Int>(k));
  }
  const auto position_of = [&](const std::string& f) -> Int {
    const auto it = position.find(f);
    return it == position.end() ? -1 : it->second;
  };
  std::map<std::string, std::vector<std::size_t>, std::less<>> turn;
  for (std::size_t s = 0; s < shares.size(); ++s) {
    const std::string* last = nullptr;
    for (const auto& [f, syms] : shares[s].members) {
      if (last == nullptr || position_of(f) > position_of(*last)) {
        last = &f;
      }
    }
    turn[*last].push_back(s);
  }
  std::map<std::size_t, std::vector<Int>> share_addrs;

  const auto range_for = [&](const std::string& what, const std::set<Int>& forbidden, Int n) {
    std::optional<std::vector<Int>> addrs = find_free_range(forbidden, n, zp_start, zp_end);
    if (!addrs && options.fallback) {
      addrs = find_free_range(forbidden, n, options.fallback->start, options.fallback->stop);
    }
    if (!addrs) {
      addrs = find_free_range(forbidden, n, spill_start, spill_end);
    }
    if (!addrs) {
      throw allocation_error("can't allocate " + pyrepr::integer(n) + "-byte slot range for " +
                             what + ": no contiguous gap in either the ZP window ($" +
                             pyrepr::hex(zp_start, 2) + "-$" + pyrepr::hex(zp_end - 1, 2) +
                             ") or the spill region ($" + pyrepr::hex(spill_start, 4) + "-$" +
                             pyrepr::hex(spill_end - 1, 4) +
                             ") avoids every already-allocated caller's range");
    }
    return *addrs;
  };

  // Returns the bytes of the allocated shares with a function that can
  // be on the call stack with `fn_name`.
  const auto share_bytes = [&](const std::string& fn_name) {
    std::set<Int> out;
    for (std::size_t s = 0; s < shares.size(); ++s) {
      const auto it = share_addrs.find(s);
      if (it == share_addrs.end()) {
        continue;
      }
      const NameSet fns = shares[s].functions();
      if (std::any_of(fns.begin(), fns.end(),
                      [&](const std::string& f) { return coexist(fn_name, f, reach); })) {
        out.insert(it->second.begin(), it->second.end());
      }
    }
    return out;
  };

  // Each function's own bytes, its slots outside any share, in the order
  // the functions were allocated.
  OrderedMap<std::string, std::vector<Int>> own_addrs;
  OrderedMap<std::string, Int> sym_to_addr;
  std::map<std::string, std::vector<Int>, std::less<>> addrs_by_fn;
  for (const auto& fn_name : order) {
    const ParamLayout* found = abi.find(fn_name);
    const ZpLayout* layout = found == nullptr ? nullptr : std::get_if<ZpLayout>(found);
    if (layout == nullptr) {
      continue;
    }
    const std::size_t n = layout->slot_symbols.size();
    std::vector<std::optional<Int>> pinned = layout->pinned;
    if (pinned.empty()) {
      pinned.resize(n);
    }
    std::vector<std::optional<std::string>> regs = layout->param_registers;
    if (regs.empty()) {
      regs.resize(n);
    }
    std::vector<std::size_t> mine;
    for (std::size_t k = 0; k < n; ++k) {
      if (!shared.contains(layout->slot_symbols[k]) && !pinned.at(k) && !regs.at(k)) {
        mine.push_back(k);
      }
    }
    for (std::size_t k = 0; k < n; ++k) {
      if (pinned.at(k)) {
        sym_to_addr.insert_or_assign(layout->slot_symbols[k], *pinned[k]);
      }
    }
    std::set<Int> forbidden = options.reserved;
    if (const auto it = ancestors.find(fn_name); it != ancestors.end()) {
      for (const auto& anc : it->second) {
        if (const auto a = addrs_by_fn.find(anc); a != addrs_by_fn.end()) {
          forbidden.insert(a->second.begin(), a->second.end());
        }
      }
    }
    for (const Int b : share_bytes(fn_name)) {
      forbidden.insert(b);
    }
    std::vector<Int> addrs;
    if (!mine.empty()) {
      addrs = range_for("function `" + fn_name + "`", forbidden, static_cast<Int>(mine.size()));
    }
    for (std::size_t i = 0; i < mine.size(); ++i) {
      sym_to_addr.insert_or_assign(layout->slot_symbols[mine[i]], addrs[i]);
    }
    std::vector<Int> own = addrs;
    for (const auto& a : pinned) {
      if (a) {
        own.push_back(*a);
      }
    }
    std::vector<Int>& by_fn = addrs_by_fn[fn_name];
    by_fn.insert(by_fn.end(), own.begin(), own.end());
    own_addrs.insert_or_assign(fn_name, std::move(own));
    if (const auto it = turn.find(fn_name); it != turn.end()) {
      for (const std::size_t s : it->second) {
        const SlotShare& share = shares[s];
        const NameSet fns = share.functions();
        std::set<Int> share_forbidden = options.reserved;
        // A lambda can't capture a structured binding in C++20 as Clang
        // 14 compiles it, so the entry is taken whole.
        for (const auto& entry : own_addrs) {
          const std::string& other = entry.first;
          if (std::any_of(fns.begin(), fns.end(),
                          [&](const std::string& f) { return coexist(other, f, reach); })) {
            share_forbidden.insert(entry.second.begin(), entry.second.end());
          }
        }
        for (const auto& f : fns) {
          for (const Int b : share_bytes(f)) {
            share_forbidden.insert(b);
          }
        }
        const std::vector<Int> got =
            range_for("the shared parameter bytes `" + share.members.at(0).second.at(0) + "`",
                      share_forbidden, static_cast<Int>(share.width()));
        share_addrs[s] = got;
        for (const auto& [f, syms] : share.members) {
          for (std::size_t i = 0; i < syms.size() && i < got.size(); ++i) {
            sym_to_addr.insert_or_assign(syms[i], got[i]);
          }
          std::vector<Int>& theirs = addrs_by_fn[f];
          theirs.insert(theirs.end(), got.begin(), got.end());
        }
      }
    }
  }

  Abi updated;
  for (const auto& [fn_name, layout] : abi) {
    const auto* zp = std::get_if<ZpLayout>(&layout);
    if (zp == nullptr) {
      updated.insert_or_assign(fn_name, layout);
      continue;
    }
    std::vector<std::optional<Int>> new_addrs;
    if (own_addrs.contains(fn_name)) {
      for (const auto& s : zp->slot_symbols) {
        const Int* a = sym_to_addr.find(s);
        new_addrs.push_back(a == nullptr ? std::nullopt : std::optional<Int>(*a));
      }
    } else {
      new_addrs = zp->addrs;
      for (std::size_t i = 0; i < zp->slot_symbols.size() && i < new_addrs.size(); ++i) {
        if (new_addrs[i]) {
          sym_to_addr.insert_or_assign(zp->slot_symbols[i], *new_addrs[i]);
        }
      }
    }
    updated.insert_or_assign(fn_name, ZpLayout{.slot_symbols = zp->slot_symbols,
                                               .addrs = std::move(new_addrs),
                                               .param_registers = zp->param_registers,
                                               .pinned = zp->pinned});
  }
  return {std::move(updated), std::move(sym_to_addr)};
}

}  // namespace c6502::passes
