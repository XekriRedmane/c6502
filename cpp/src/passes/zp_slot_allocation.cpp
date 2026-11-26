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

std::vector<std::string> topological_order(const Graph& callees_of) {
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
