// The C++ side of the `tables` and `pseudos` records of
// tools/capture_helpers.py, the helpers that the code generation passes
// share. The encoding of each value is `stage_values`'s and
// `pseudo_values`'s.
#include <exception>
#include <functional>
#include <string>
#include <vector>

#include "helper_checks.h"
#include "passes/optimization/cfg.h"
#include "passes/optimization/var_visit.h"
#include "passes/recursion_saves.h"
#include "passes/replace_pseudoregisters.h"
#include "passes/slot_sharing.h"
#include "passes/zp_slot_naming.h"
#include "tagged_json.h"

namespace c6502::helper_checks {

namespace {

namespace tac = ::c6502::tac_ast;
using namespace ::c6502::passes;

// Joins `fields` with spaces, each empty one written "-", or returns
// "-" for none, as `_fields` does.
std::string fields(const std::vector<std::string>& xs) {
  if (xs.empty()) {
    return "-";
  }
  std::string out;
  for (std::size_t i = 0; i < xs.size(); ++i) {
    if (i != 0) {
      out += ' ';
    }
    out += xs[i].empty() ? "-" : xs[i];
  }
  return out;
}

template <class Seq, class F>
std::string join(const Seq& xs, F text, char sep = ',') {
  std::string out;
  bool first = true;
  for (const auto& x : xs) {
    if (!first) {
      out += sep;
    }
    first = false;
    out += text(x);
  }
  return out;
}

std::string val_text(const tac::Type_val& v) {
  if (const auto* var = std::get_if<tac::Var>(&v)) {
    return var->name;
  }
  std::string d = asdl::dump(v);
  d.pop_back();
  for (char& c : d) {
    if (c == ' ') {
      c = '_';
    }
  }
  return "#" + d;
}

std::string str(std::string_view s) { return std::string(s); }

template <class Seq>
std::string dots(const Seq& xs) {
  return join(xs, [](auto x) { return std::to_string(x); }, '.');
}

// Returns `f()`, or "E" when it throws, as `_try` gives.
template <class F>
std::string attempt(F f) {
  try {
    return f();
  } catch (const std::exception&) {
    return "E";
  }
}

}  // namespace

Values cfg_values(const std::vector<const tac::Function*>& fns,
                  const std::vector<const tac::Type_instruction*>& instrs) {
  namespace cfg = ::c6502::passes::tac_cfg;
  const auto per_fn = [&](auto f) {
    std::vector<std::string> out;
    for (const auto* fn : fns) {
      out.push_back(attempt([&] { return f(cfg::build_cfg(*fn)); }));
    }
    return fields(out);
  };
  const auto graph = [](const cfg::Cfg& c) {
    std::string blocks = join(c.blocks, [](const auto& kv) {
      const auto& [b, blk] = kv;
      return std::to_string(b) + "/" + std::to_string(blk.instructions.size()) + "/" +
             dots(blk.predecessors) + "/" + dots(blk.successors);
    }, ';');
    return blocks + "|" + dots(c.block_order);
  };
  std::vector<std::string> defs;
  for (const auto* i : instrs) {
    defs.push_back(join(recursion_saves::defs_in(*i), str));
  }
  return {
      {"tac_cfg", per_fn(graph)},
      {"reverse_postorder", per_fn([](const cfg::Cfg& c) { return dots(cfg::reverse_postorder(c)); })},
      {"immediate_dominators", per_fn([](const cfg::Cfg& c) {
         return join(cfg::immediate_dominators(c), [](const auto& kv) {
           return std::to_string(kv.first) + ":" + std::to_string(kv.second);
         }, ';');
       })},
      {"dominator_tree_children", per_fn([](const cfg::Cfg& c) {
         return join(cfg::dominator_tree_children(cfg::immediate_dominators(c)),
                     [](const auto& kv) { return std::to_string(kv.first) + ":" + dots(kv.second); },
                     ';');
       })},
      {"dominance_frontiers", per_fn([](const cfg::Cfg& c) {
         return join(cfg::dominance_frontiers(c), [](const auto& kv) {
           return std::to_string(kv.first) + ":" + dots(kv.second.items());
         }, ';');
       })},
      {"natural_loops", per_fn([](const cfg::Cfg& c) {
         return join(cfg::natural_loops(c), [](const auto& loop) {
           return std::to_string(loop.first) + ":" + dots(loop.second.items());
         }, ';');
       })},
      {"recursion_saves_defs_in", fields(defs)},
  };
}

Values stage_values(const json::Value& v) {
  const auto prog = tagged::TacProgramC::decode(*v.get("tac"));
  const auto symbols = tagged::SymbolTableC::decode(*v.get("symbols"));
  const auto types = tagged::TypeTableC::decode(*v.get("types"));
  const auto shares = tagged::ListOf<tagged::SlotShareC>::decode(*v.get("shares"));
  const auto layout_items = tagged::ListOf<tagged::IntC>::decode(*v.get("layout"));
  const RuntimeLayout layout{.ssp = layout_items.at(0),
                             .fp = layout_items.at(1),
                             .hargs = layout_items.at(2),
                             .hargs_size = layout_items.at(3),
                             .dptr = layout_items.at(4)};

  std::vector<const tac::Function*> fns;
  for (const auto& tl : prog.top_level) {
    if (const auto* fn = std::get_if<tac::Function>(&tl)) {
      fns.push_back(fn);
    }
  }
  std::vector<const tac::Type_instruction*> instrs;
  for (const auto* fn : fns) {
    for (const auto& i : fn->instructions) {
      instrs.push_back(&i);
    }
  }

  std::vector<std::string> sizes;
  std::vector<std::string> sizes_untyped;
  std::vector<std::string> spellings;
  for (const auto& [name, sym] : symbols) {
    sizes.push_back(asdl::int_to_string(size_of_name(name, &symbols, &types)));
    sizes_untyped.push_back(asdl::int_to_string(size_of_name(name, &symbols, nullptr)));
    spellings.push_back(source_spelling(name));
  }
  std::vector<std::string> members;
  for (const auto& [tag, layout_] : types) {
    members.push_back(join(layout_.members, [&](const MemberInfo& m) {
      return asdl::int_to_string(sizeof_type(m.type, &types));
    }));
  }
  std::vector<std::string> slots;
  for (const auto* fn : fns) {
    const Symbol* sym = symbols.find(fn->name);
    if (sym == nullptr || !std::holds_alternative<FunAttr>(sym->attrs)) {
      slots.push_back("-");
      continue;
    }
    try {
      const auto& ft = std::get<c99_ast::FunType>(sym->type);
      std::vector<asdl::Int> psizes;
      for (const auto& p : ft.params) {
        psizes.push_back(sizeof_type(p, &types));
      }
      const std::vector<std::optional<std::string>> pnames(fn->params.begin(), fn->params.end());
      slots.push_back(join(param_slot_symbols(fn->name, pnames, psizes), str));
    } catch (const std::exception&) {
      slots.push_back("E");
    }
  }
  std::vector<std::string> vals;
  std::vector<std::string> uses;
  std::vector<std::string> defs;
  for (const auto* i : instrs) {
    vals.push_back(join(vals_in(*i), [](const tac::Type_val* x) { return val_text(*x); }));
    uses.push_back(join(uses_in(*i), str));
    defs.push_back(join(defs_in(*i), str));
  }
  std::vector<std::string> counts;
  for (const auto* fn : fns) {
    counts.push_back(join(count_uses(fn->instructions), [](const auto& kv) {
      return kv.first + ":" + std::to_string(kv.second);
    }));
  }
  const ReachSets reach = reach_sets(prog);
  std::vector<std::string> reaches;
  std::string coexists;
  std::vector<std::string> conflicts;
  for (const auto& [f, callees] : reach) {
    reaches.push_back(f + ":" + join(callees, str));
    for (const auto& [g, unused] : reach) {
      coexists += coexist(f, g, reach) ? '1' : '0';
    }
    conflicts.push_back(join(share_conflicts(f, shares, reach),
                             [](std::size_t k) { return std::to_string(k); }));
  }
  if (coexists.empty()) {
    coexists = "-";
  }
  std::vector<std::string> share_sets;
  for (const auto& s : shares) {
    share_sets.push_back(std::to_string(s.width()) + ":" + join(s.functions(), str) + ":" +
                         join(s.owners(), str) + ":" + join(s.slots(), str));
  }
  const std::string runtime =
      join(layout.bytes(), [](asdl::Int b) { return asdl::int_to_string(b); });
  Values out = cfg_values(fns, instrs);
  const Values rest = {
      {"size_of_name", fields(sizes)},
      {"size_of_name_untyped", fields(sizes_untyped)},
      {"sizeof_members", fields(members)},
      {"source_spelling", fields(spellings)},
      {"param_slot_symbols", fields(slots)},
      {"vals_in", fields(vals)},
      {"uses_in", fields(uses)},
      {"defs_in", fields(defs)},
      {"count_uses", fields(counts)},
      {"reach_sets", fields(reaches)},
      {"coexist", coexists},
      {"share_conflicts", fields(conflicts)},
      {"slot_share_sets", fields(share_sets)},
      {"runtime_bytes", runtime.empty() ? "-" : runtime},
  };
  out.insert(out.end(), rest.begin(), rest.end());
  return out;
}

Values pseudo_values(const json::Value& v) {
  std::vector<std::string> out;
  for (const auto& name : tagged::ListOf<tagged::Str>::decode(*v.get("names"))) {
    const auto [src, byte] = parse_pseudo_name(name);
    out.push_back((src ? *src : std::string("-")) + "|" + asdl::int_to_string(byte));
  }
  return {{"parse_pseudo_name", fields(out)}};
}

}  // namespace c6502::helper_checks
