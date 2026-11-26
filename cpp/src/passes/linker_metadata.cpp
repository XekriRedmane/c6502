// Linking is where c6502 does its whole-program optimization. Each unit
// leaves its zero-page slots, its zero-page objects, and the runtime's
// block to the linker, which places them for the program as a whole.
// `link_objects` reads each object's metadata (`parse_metadata`) and
// then calls these functions in this order:
//
//   1. `rename_locals`, which gives an internal name that another unit
//      also uses, or that names something else in the program, the
//      spelling `name.k` in unit `k`, with `rename_meta` for the unit's
//      metadata.
//   2. `merge`, which unions the units' metadata into the program's,
//      and `validate_eligibility`, which runs `hw_stack_cycles` on
//      `synthesize`'s program.
//   3. `drop_unused_locals`, `merge_zp_objects`, link_layout's
//      `startup_lines` under a script, `choose_layout`, and
//      `place_zp_objects`, with `runtime_bytes` for the allocators'
//      reserved bytes.
//   4. `link_shares`.
//   5. `synthesize`, the allocators (`allocate_zp_slots`,
//      `allocate_function_locals`, and `build_local_slot_symbols`), and
//      `bind_extern_params`.
//   6. Later, `init_lines` and `data_lines` for the linker's own code.
//
// The merged metadata and the synthetic program
//
// The linker requires every function that any unit defines to be
// eligible for the private-pool model: no `IndirectCall`, no part in
// any cycle (local or across units, except one whose every call saves
// on the hardware stack), and every callee either defined in some unit
// or declared zp_abi extern. A call of a name that is neither is a call
// of a non-zp_abi external, such as a C library function, and the
// caller can't be linked. The allocators the single-unit compile runs
// then run on a synthetic `tac_ast.Program` of the merged call graph,
// and their output is the global address of every slot symbol. A cycle
// across units, which no unit's compile could see, makes them throw
// their own errors.
//
// Names
//
// An internal name (a `static`, or a string literal `str@N`) that
// another unit also defines, or that names something else in the
// program, is renamed `name.k` in unit `k`, so that every item has a
// name of its own in the executable. A renamed function's zero-page
// slots, which each unit spells after the function, are renamed with
// it, so the allocators place two functions. Two units defining one
// external name is an error.
//
// The runtime's block and the zero-page objects
//
// Every unit names SSP, FP, HARGS, and DPTR only by symbol, and lists
// how many HARGS bytes it uses, so the linker chooses the program's
// layout the way the compiler does for one unit, from every unit's
// fixed zero-page objects and the largest need. Each unit lists its
// zero-page objects in the metadata and refers to them only by name, so
// the linker places them for the whole program: an object declared with
// an address keeps it, and the rest go in the lowest free bytes below
// $80, the declared ones first and then the ones `-O` promoted, densest
// first. Each unit's `__zp_init` is left out in favor of one the linker
// writes (`init_lines`), which stores every object's initial bytes and
// runs the startup code a script needs.
#include "passes/linker_metadata.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <utility>

#include "asm_emit.h"
#include "errors.h"
#include "passes/recursion_saves.h"
#include "passes/runtime_layout.h"
#include "passes/zp_globals.h"
#include "pyrepr.h"

namespace c6502::passes::linker {

namespace {

using asdl::Int;

constexpr Int kAutoHi = 0x80;

constexpr Int kClassicHargsSize = 32;

[[noreturn]] void link_error(const std::string& message) {
  throw CompileError("LinkError", message);
}

std::string repr_list(const std::vector<std::string>& names) {
  std::string out = "[";
  for (std::size_t i = 0; i < names.size(); ++i) {
    if (i != 0) {
      out += ", ";
    }
    out += pyrepr::string(names[i]);
  }
  return out + "]";
}

template <class Names>
std::string repr_sorted(const Names& names) {
  std::vector<std::string> sorted(names.begin(), names.end());
  std::sort(sorted.begin(), sorted.end());
  return repr_list(sorted);
}

const std::string& renamed(const Renames& renames, const std::string& name) {
  const std::string* to = renames.find(name);
  return to != nullptr ? *to : name;
}

void rename_all(const Renames& renames, std::vector<std::string>& names) {
  for (std::string& n : names) {
    n = renamed(renames, n);
  }
}

void apply_renames(elf::ElfFile& obj, const Renames& renames) {
  for (const elf::SymbolPtr& s : obj.symbols) {
    if (const std::string* to = renames.find(s->name); to != nullptr &&
                                                       !(s->is_global() && s->defined())) {
      s->name = *to;
    }
  }
}

std::vector<Int> span(Int at, Int size) {
  std::vector<Int> out;
  for (Int b = at; b < at + size; ++b) {
    out.push_back(b);
  }
  return out;
}

const Int& layout_value(const LayoutDict& layout, std::string_view name) {
  const Int* v = layout.find(name);
  if (v == nullptr) {
    throw InternalError("the runtime layout has no " + std::string(name));
  }
  return *v;
}

}  // namespace

LinkMetadata merge(const std::vector<LinkMetadata>& metas) {
  OrderedMap<std::string, FunctionMeta> defs_by_name;
  std::vector<std::string> duplicate_defs;
  for (const LinkMetadata& m : metas) {
    for (const FunctionMeta& d : m.defs) {
      if (defs_by_name.contains(d.name)) {
        duplicate_defs.push_back(d.name);
      } else {
        defs_by_name.insert_or_assign(d.name, d);
      }
    }
  }
  OrderedMap<std::string, ExternMeta> externs_by_name;
  std::vector<std::string> extern_param_conflicts;
  for (const LinkMetadata& m : metas) {
    for (const ExternMeta& e : m.externs) {
      const ExternMeta* found = externs_by_name.find(e.name);
      const std::optional<ExternMeta> existing =
          found != nullptr ? std::optional<ExternMeta>(*found) : std::nullopt;
      if (!existing) {
        externs_by_name.insert_or_assign(e.name, e);
      } else if (existing->params.size() != e.params.size()) {
        extern_param_conflicts.push_back(e.name);
      }
      const FunctionMeta* def = defs_by_name.find(e.name);
      const auto check = [&](std::size_t param_bytes, const Arrivals& theirs, bool carry) {
        if (param_bytes == e.params.size() && theirs != arrivals(e)) {
          link_error("the units declare `" + e.name +
                     "` with different parameters in registers or at zero-page addresses; "
                     "every declaration has to give the same calling convention");
        }
        if (carry != e.carry) {
          link_error(carry_conflict(e.name));
        }
      };
      if (existing) {
        check(existing->params.size(), arrivals(*existing), existing->carry);
      }
      if (def != nullptr) {
        check(def->params.size(), arrivals(*def), def->carry);
      }
    }
  }
  std::set<std::pair<std::string, std::string>> seen_edges;
  std::set<std::pair<std::string, std::string>> saved_edges;
  for (const LinkMetadata& m : metas) {
    seen_edges.insert(m.calls.begin(), m.calls.end());
    saved_edges.insert(m.saved_calls.begin(), m.saved_calls.end());
  }
  LinkMetadata merged;
  for (const auto& [name, d] : defs_by_name) {
    merged.defs.push_back(d);
  }
  std::sort(merged.defs.begin(), merged.defs.end(),
            [](const FunctionMeta& a, const FunctionMeta& b) { return a.name < b.name; });
  for (const auto& [name, e] : externs_by_name) {
    merged.externs.push_back(e);
  }
  std::sort(merged.externs.begin(), merged.externs.end(),
            [](const ExternMeta& a, const ExternMeta& b) { return a.name < b.name; });
  merged.calls.assign(seen_edges.begin(), seen_edges.end());
  merged.saved_calls.assign(saved_edges.begin(), saved_edges.end());
  for (const LinkMetadata& m : metas) {
    merged.shares.insert(merged.shares.end(), m.shares.begin(), m.shares.end());
  }
  if (!duplicate_defs.empty()) {
    const std::set<std::string> names(duplicate_defs.begin(), duplicate_defs.end());
    link_error("function(s) defined in multiple TUs: " + repr_sorted(names));
  }
  if (!extern_param_conflicts.empty()) {
    const std::set<std::string> names(extern_param_conflicts.begin(),
                                      extern_param_conflicts.end());
    link_error("zp_abi extern(s) declared with conflicting param_bytes across TUs: " +
               repr_sorted(names));
  }
  return merged;
}

std::string carry_conflict(std::string_view name) {
  return "the units disagree on whether `" + std::string(name) +
         "` returns its `bool` in the carry flag. Under -O, a `bool` function with external "
         "linkage does when it's zp_abi, so a unit that takes its address, or one that can't "
         "give it zp_abi, sees it differently from the others. A function that returns "
         "`unsigned char` returns it in A in every unit";
}

Arrivals arrivals(std::size_t param_bytes, const std::vector<std::string>& param_regs,
                  const std::vector<std::optional<Int>>& pinned) {
  const std::size_t n_regs = std::max(param_bytes, param_regs.size());
  const std::size_t n_pinned = std::max(param_bytes, pinned.size());
  Arrivals out;
  for (std::size_t i = 0; i < std::min(n_regs, n_pinned); ++i) {
    out.emplace_back(i < param_regs.size() ? param_regs[i] : std::string(),
                     i < pinned.size() ? pinned[i] : std::nullopt);
  }
  return out;
}

Arrivals arrivals(const FunctionMeta& meta) {
  return arrivals(meta.params.size(), meta.param_regs, meta.pinned);
}

Arrivals arrivals(const ExternMeta& meta) {
  return arrivals(meta.params.size(), meta.param_regs, meta.pinned);
}

void validate_eligibility(const LinkMetadata& merged) {
  NameSet known;
  for (const FunctionMeta& d : merged.defs) {
    known.insert(d.name);
  }
  for (const ExternMeta& e : merged.externs) {
    known.insert(e.name);
  }
  std::vector<std::string> indirect_fns;
  for (const FunctionMeta& d : merged.defs) {
    if (d.indirect) {
      indirect_fns.push_back(d.name);
    }
  }
  if (!indirect_fns.empty()) {
    link_error(
        "function(s) contain IndirectCall and can't be linked under the private-pool model: " +
        repr_sorted(indirect_fns));
  }
  const tac_ast::Program prog = synthesize(merged).prog;
  const Cycles saved_cycles = hw_stack_cycles(prog);
  std::vector<std::string> locally_cyclic;
  for (const FunctionMeta& d : merged.defs) {
    if (d.in_cycle && !saved_cycles.contains(d.name)) {
      locally_cyclic.push_back(d.name);
    }
  }
  if (!locally_cyclic.empty()) {
    link_error(
        "function(s) on a cycle in their own TU's call graph (recursion is supported only "
        "when every call in the cycle is marked `[[c6502::save_hardware_stack]]`): " +
        repr_sorted(locally_cyclic));
  }
  std::vector<std::string> across;
  for (const auto& [name, members] : cycles(call_graph(prog))) {
    if (!saved_cycles.contains(name)) {
      across.push_back(name);
    }
  }
  if (!across.empty()) {
    link_error(
        "function(s) on a cycle of calls between units, which can't have private pools "
        "(recursion is supported only within one unit, when every call in the cycle is marked "
        "`[[c6502::save_hardware_stack]]`): " +
        repr_sorted(across));
  }
  std::map<std::string, std::set<std::string>> bad_callees;
  for (const auto& [caller, callee] : merged.calls) {
    if (!known.contains(callee)) {
      bad_callees[caller].insert(callee);
    }
  }
  if (!bad_callees.empty()) {
    std::string formatted;
    for (const auto& [caller, callees] : bad_callees) {
      if (!formatted.empty()) {
        formatted += "; ";
      }
      formatted += caller + " → " + repr_sorted(callees);
    }
    link_error("function(s) call non-zp_abi externs that aren't defined in any input TU: " +
               formatted);
  }
}

Synthesized synthesize(const LinkMetadata& merged) {
  OrderedMap<std::string, std::vector<std::string>> callees_of;
  for (const FunctionMeta& d : merged.defs) {
    callees_of.insert_or_assign(d.name, {});
  }
  for (const auto& [caller, callee] : merged.calls) {
    if (std::vector<std::string>* list = callees_of.find(caller)) {
      list->push_back(callee);
    }
  }
  const std::set<std::pair<std::string, std::string>> saved(merged.saved_calls.begin(),
                                                            merged.saved_calls.end());
  Synthesized out;
  for (const FunctionMeta& d : merged.defs) {
    tac_ast::Function fn;
    fn.name = d.name;
    fn.is_global = true;
    for (const std::string& callee : callees_of.at(d.name)) {
      tac_ast::FunctionCall call;
      call.name = callee;
      call.hw_stack_save = saved.contains({d.name, callee});
      fn.instructions.emplace_back(std::move(call));
    }
    out.prog.top_level.emplace_back(std::move(fn));
  }
  const auto registers = [](const std::vector<std::string>& param_regs, std::size_t count) {
    std::vector<std::optional<std::string>> regs;
    for (const std::string& r : param_regs) {
      regs.push_back(r.empty() ? std::nullopt : std::optional<std::string>(r));
    }
    while (regs.size() < count) {
      regs.emplace_back(std::nullopt);
    }
    return regs;
  };
  NameSet defined;
  for (const FunctionMeta& d : merged.defs) {
    defined.insert(d.name);
    if (!d.params.empty()) {
      out.abi.insert_or_assign(d.name, ZpLayout{.slot_symbols = d.params,
                                                .addrs = {},
                                                .param_registers =
                                                    registers(d.param_regs, d.params.size()),
                                                .pinned = d.pinned});
    } else {
      out.abi.insert_or_assign(d.name, SoftStackLayout{});
    }
  }
  for (const ExternMeta& e : merged.externs) {
    if (defined.contains(e.name)) {
      continue;
    }
    out.abi.insert_or_assign(e.name,
                             ZpLayout{.slot_symbols = e.params,
                                      .addrs = {},
                                      .param_registers = registers(e.param_regs, e.params.size()),
                                      .pinned = e.pinned});
  }
  for (const FunctionMeta& d : merged.defs) {
    out.local_bytes.insert_or_assign(d.name, static_cast<Int>(d.locals.size()));
  }
  return out;
}

std::vector<Renames> rename_locals(Objects& objects, std::vector<LinkMetadata>& metas,
                                   const NameSet& taken) {
  std::map<std::string, std::size_t, std::less<>> owner;
  std::map<std::string, std::set<std::size_t>, std::less<>> users;
  for (std::size_t k = 0; k < metas.size(); ++k) {
    for (const ItemMeta& it : metas[k].items) {
      users[it.name].insert(k);
      if (!it.local) {
        if (owner.contains(it.name)) {
          link_error("`" + it.name + "` is defined in multiple TUs");
        }
        owner.emplace(it.name, k);
      }
    }
    for (const ZpObjectMeta& z : metas[k].zp_objects) {
      users[z.name].insert(k);
    }
  }
  std::vector<Renames> out;
  const std::size_t n = std::min(objects.size(), metas.size());
  for (std::size_t k = 0; k < n; ++k) {
    LinkMetadata& meta = metas[k];
    std::vector<std::string> local_names;
    for (const ItemMeta& it : meta.items) {
      if (it.local) {
        local_names.push_back(it.name);
      }
    }
    for (const ZpObjectMeta& z : meta.zp_objects) {
      if (z.local) {
        local_names.push_back(z.name);
      }
    }
    Renames renames;
    const std::string suffix = "." + std::to_string(k);
    for (const std::string& name : local_names) {
      if (users.at(name).size() > 1 || taken.contains(name)) {
        renames.insert_or_assign(name, name + suffix);
      }
    }
    for (const FunctionMeta& d : meta.defs) {
      if (renames.contains(d.name)) {
        for (const auto* list : {&d.params, &d.locals}) {
          for (const std::string& sym : *list) {
            renames.insert_or_assign(sym, sym + suffix);
          }
        }
      }
    }
    out.push_back(renames);
    if (renames.empty()) {
      continue;
    }
    apply_renames(objects[k].second, renames);
    rename_meta(meta, renames);
  }
  return out;
}

void rename_meta(LinkMetadata& meta, const Renames& renames) {
  for (ItemMeta& it : meta.items) {
    it.name = renamed(renames, it.name);
  }
  for (ZpObjectMeta& z : meta.zp_objects) {
    z.name = renamed(renames, z.name);
  }
  for (FunctionMeta& d : meta.defs) {
    d.name = renamed(renames, d.name);
    rename_all(renames, d.params);
    rename_all(renames, d.locals);
    rename_all(renames, d.changes);
  }
  for (ExternMeta& e : meta.externs) {
    e.name = renamed(renames, e.name);
    rename_all(renames, e.params);
  }
  for (auto* edges : {&meta.calls, &meta.saved_calls}) {
    for (auto& [a, b] : *edges) {
      a = renamed(renames, a);
      b = renamed(renames, b);
    }
  }
  for (SlotShare& s : meta.shares) {
    for (auto& [f, syms] : s.members) {
      f = renamed(renames, f);
      rename_all(renames, syms);
    }
    rename_all(renames, s.holders);
    for (auto& [f, cs] : s.live) {
      f = renamed(renames, f);
      rename_all(renames, cs);
    }
    for (auto& [a, b] : s.forwards) {
      a = renamed(renames, a);
      b = renamed(renames, b);
    }
    rename_all(renames, s.writers);
  }
}

ZpObjects merge_zp_objects(const std::vector<LinkMetadata>& metas) {
  ZpObjects objects;
  for (const LinkMetadata& meta : metas) {
    for (const ZpObjectMeta& z : meta.zp_objects) {
      objects[z.name].push_back(z);
    }
  }
  for (const auto& [name, records] : objects) {
    std::set<Int> sizes;
    std::set<Int> ats;
    std::size_t defs = 0;
    for (const ZpObjectMeta& r : records) {
      sizes.insert(r.size);
      if (r.at) {
        ats.insert(*r.at);
      }
      defs += r.defined ? 1 : 0;
    }
    if (sizes.size() > 1) {
      link_error("zero-page object `" + name + "` has different sizes in different units");
    }
    if (records.at(0).size < 1) {
      link_error("zero-page object `" + name + "` has " + pyrepr::integer(records.at(0).size) +
                 " bytes, where an object has at least one");
    }
    if (ats.size() > 1) {
      link_error("zero-page object `" + name +
                 "` is declared at different addresses in different units");
    }
    if (defs > 1) {
      link_error("zero-page object `" + name + "` is defined in " + std::to_string(defs) +
                 " units");
    }
    if (defs == 0 && ats.empty()) {
      link_error("zero-page object `" + name +
                 "` is declared `extern` but no unit defines it");
    }
  }
  return objects;
}

// Places an object declared with an address where any unit gives one,
// and otherwise in the lowest free bytes below $80, in the order the
// units list the objects, around the fixed ones and the runtime's block
// (`layout`'s SSP, FP, HARGS, and DPTR, and its "HARGS size"). The
// objects every unit that lists them says `-O` promoted go last,
// densest first, and one that doesn't fit is returned in the second
// list, to go back in the data segment, unless some unit uses it as an
// `(name),Y` pointer (it's in `pointers`), which only works in zero
// page. The objects without an address also keep off `param_bytes`,
// where functions take `zeropage(addr)` parameters.
std::pair<SymbolAddrs, std::vector<std::string>> place_zp_objects(
    const ZpObjects& objects, const LayoutDict& layout, const std::set<Int>& param_bytes,
    const NameSet& pointers) {
  const RuntimeLayout block{.ssp = layout_value(layout, "SSP"),
                            .fp = layout_value(layout, "FP"),
                            .hargs = layout_value(layout, "HARGS"),
                            .hargs_size = layout_value(layout, "HARGS size"),
                            .dptr = layout_value(layout, "DPTR")};
  const std::set<Int> runtime = block.bytes();
  std::map<Int, std::string> used;
  SymbolAddrs addrs;
  const auto claim = [&](const std::string& name, Int addr, Int size) {
    for (Int b : span(addr, size)) {
      if (const auto it = used.find(b); it != used.end()) {
        link_error("zero-page object `" + name + "` at $" + pyrepr::hex(addr, 2) +
                   " overlaps `" + it->second + "` at byte $" + pyrepr::hex(b, 2));
      }
    }
    for (Int b : span(addr, size)) {
      used[b] = name;
    }
    addrs.insert_or_assign(name, addr);
  };
  struct Promoted {
    Int weight;
    std::string name;
    Int size;
  };
  std::vector<std::pair<std::string, Int>> automatic;
  std::vector<Promoted> promoted;
  for (const auto& [name, records] : objects) {
    const Int size = records.at(0).size;
    std::optional<Int> at;
    for (const ZpObjectMeta& r : records) {
      if (r.at) {
        at = r.at;
        break;
      }
    }
    if (!at) {
      const bool all_weighted = std::all_of(records.begin(), records.end(),
                                            [](const ZpObjectMeta& r) { return r.weight; });
      if (all_weighted) {
        Int weight = *records.at(0).weight;
        for (const ZpObjectMeta& r : records) {
          weight = std::max(weight, *r.weight);
        }
        promoted.push_back({weight, name, size});
      } else {
        automatic.emplace_back(name, size);
      }
      continue;
    }
    const std::vector<Int> bytes = span(*at, size);
    if (std::any_of(bytes.begin(), bytes.end(), [&](Int b) { return runtime.contains(b); })) {
      link_error("zero-page object `" + name + "` at $" + pyrepr::hex(*at, 2) +
                 " overlaps the runtime's zero-page block");
    }
    claim(name, *at, size);
  }
  const auto free = [&](Int size) -> std::optional<Int> {
    for (Int a = 0; a < kAutoHi - size + 1; ++a) {
      bool ok = true;
      for (Int b = a; ok && b < a + size; ++b) {
        ok = !used.contains(b) && !runtime.contains(b) && !param_bytes.contains(b);
      }
      if (ok) {
        return a;
      }
    }
    return std::nullopt;
  };
  for (const auto& [name, size] : automatic) {
    const std::optional<Int> addr = free(size);
    if (!addr) {
      link_error("no room for zero-page object `" + name + "` (" + pyrepr::integer(size) +
                 " bytes) below $" + pyrepr::hex(kAutoHi, 2) + " in the linked program");
    }
    claim(name, *addr, size);
  }
  std::sort(promoted.begin(), promoted.end(), [](const Promoted& a, const Promoted& b) {
    const Int left = a.weight * b.size;
    const Int right = b.weight * a.size;
    return left != right ? left > right : a.name < b.name;
  });
  std::vector<std::string> demoted;
  for (const Promoted& p : promoted) {
    if (const std::optional<Int> addr = free(p.size)) {
      claim(p.name, *addr, p.size);
      continue;
    }
    if (pointers.contains(p.name)) {
      link_error("no room for `" + p.name + "` (" + pyrepr::integer(p.size) +
                 " bytes) below $" + pyrepr::hex(kAutoHi, 2) +
                 " in the linked program, and its unit uses it as an `(zp),Y` pointer, which "
                 "has to be in zero page");
    }
    demoted.push_back(p.name);
  }
  return {addrs, demoted};
}

LayoutDict choose_layout(const std::vector<LinkMetadata>& metas, const ZpObjects& objects,
                         Int startup_need, const std::set<Int>& param_bytes) {
  Int need = startup_need;
  for (const LinkMetadata& m : metas) {
    need = std::max(need, m.hargs_need ? *m.hargs_need : kClassicHargsSize);
  }
  std::set<Int> reserved = param_bytes;
  for (const auto& [name, records] : objects) {
    for (const ZpObjectMeta& r : records) {
      if (r.at) {
        for (Int b : span(*r.at, r.size)) {
          reserved.insert(b);
        }
      }
    }
  }
  RuntimeLayout chosen;
  try {
    chosen = ::c6502::passes::choose_layout(reserved, [need] { return need; });
  } catch (const CompileError& e) {
    if (std::string(e.python_type()) != "RuntimeLayoutError") {
      throw;
    }
    link_error(e.what());
  }
  if (chosen.hargs_size < need) {
    link_error("the program needs " + pyrepr::integer(need) +
               " HARGS bytes, and the runtime's block has " + pyrepr::integer(chosen.hargs_size));
  }
  return LayoutDict{{"SSP", chosen.ssp},
                    {"FP", chosen.fp},
                    {"HARGS", chosen.hargs},
                    {"DPTR", chosen.dptr},
                    {"HARGS size", chosen.hargs_size}};
}

std::set<Int> runtime_bytes(const LayoutDict& layout) {
  const Int size = layout_value(layout, "HARGS size");
  std::set<Int> out;
  for (const auto& [name, n] : std::initializer_list<std::pair<std::string_view, Int>>{
           {"SSP", 2}, {"FP", 2}, {"DPTR", 2}, {"HARGS", size}}) {
    for (Int b : span(layout_value(layout, name), n)) {
      out.insert(b);
    }
  }
  return out;
}

std::vector<SlotShare> link_shares(const LinkMetadata& merged,
                                   const std::vector<LinkMetadata>& metas) {
  std::vector<SlotShare> shares;
  for (const LinkMetadata& m : metas) {
    shares.insert(shares.end(), m.shares.begin(), m.shares.end());
  }
  if (shares.empty()) {
    return {};
  }
  std::map<std::string, const FunctionMeta*, std::less<>> defs;
  for (const FunctionMeta& d : merged.defs) {
    defs.emplace(d.name, &d);
  }
  OrderedMap<std::string, std::string> canonical;
  for (const LinkMetadata& m : metas) {
    for (const ExternMeta& e : m.externs) {
      if (const auto it = defs.find(e.name); it != defs.end()) {
        const std::vector<std::string>& params = it->second->params;
        for (std::size_t i = 0; i < std::min(e.params.size(), params.size()); ++i) {
          canonical.insert_or_assign(e.params[i], params[i]);
        }
      }
    }
  }
  try {
    std::vector<SlotShare> out = merge_shares(shares, canonical);
    NameSet defined;
    OrderedMap<std::string, NameSet> changed;
    for (const FunctionMeta& d : merged.defs) {
      defined.insert(d.name);
      changed.insert_or_assign(d.name, NameSet(d.changes.begin(), d.changes.end()));
    }
    check_linked_shares(out, merged.calls, defined, changed);
    return out;
  } catch (const CompileError& e) {
    if (std::string(e.python_type()) != "ValueError") {
      throw;
    }
    link_error(e.what());
  }
}

std::vector<std::string> regs(const std::vector<std::string>& param_regs, std::size_t count) {
  std::vector<std::string> out = param_regs;
  while (out.size() < count) {
    out.emplace_back();
  }
  return out;
}

void bind_extern_params(const LinkMetadata& merged, const std::vector<ExternMeta>& externs,
                        SymbolAddrs& symbols) {
  OrderedMap<std::string, std::pair<std::vector<std::string>, std::vector<std::string>>>
      canonical;
  for (const FunctionMeta& d : merged.defs) {
    canonical.insert_or_assign(d.name, {d.params, regs(d.param_regs, d.params.size())});
  }
  for (const ExternMeta& e : merged.externs) {
    if (!canonical.contains(e.name)) {
      canonical.insert_or_assign(e.name, {e.params, regs(e.param_regs, e.params.size())});
    }
  }
  for (const ExternMeta& e : externs) {
    const auto* found = canonical.find(e.name);
    if (found == nullptr) {
      throw InternalError("KeyError: " + pyrepr::string(e.name));
    }
    const auto& [params, canon_regs] = *found;
    if (params.size() != e.params.size()) {
      link_error("`" + e.name + "` is declared with " + std::to_string(e.params.size()) +
                 " zp_abi parameter byte(s) in one unit and has " +
                 std::to_string(params.size()) + " where it's defined");
    }
    if (regs(e.param_regs, e.params.size()) != canon_regs) {
      link_error("`" + e.name +
                 "` is declared with its parameters in different registers in two units");
    }
    for (std::size_t i = 0; i < e.params.size(); ++i) {
      if (!canon_regs[i].empty()) {
        continue;
      }
      const Int* addr = symbols.find(params[i]);
      if (addr == nullptr) {
        throw InternalError("KeyError: " + pyrepr::string(params[i]));
      }
      const Int value = *addr;
      const Int* bound = symbols.find(e.params[i]);
      if (bound == nullptr) {
        symbols.insert_or_assign(e.params[i], value);
      } else if (*bound != value) {
        link_error("`" + e.params[i] + "` names two different parameter bytes of `" + e.name +
                   "`; give the parameters the same names in its declarations and its "
                   "definition");
      }
    }
  }
}

void drop_unused_locals(LinkMetadata& merged, const Objects& objects) {
  NameSet named;
  NameSet addressed;
  for (const auto& [path, obj] : objects) {
    for (const elf::SectionPtr& s : obj.sections) {
      for (const elf::Reloc& r : s->relocs) {
        named.insert(r.symbol->name);
        if (r.type == elf::R_MOS_ADDR16_LO || r.type == elf::R_MOS_ADDR16_HI) {
          addressed.insert(r.symbol->name);
        }
      }
    }
  }
  for (FunctionMeta& d : merged.defs) {
    if (std::none_of(d.locals.begin(), d.locals.end(),
                     [&](const std::string& n) { return addressed.contains(n); })) {
      std::erase_if(d.locals, [&](const std::string& n) { return !named.contains(n); });
    }
  }
}

std::vector<std::string> init_lines(const std::vector<ZpObjectMeta>& objects,
                                    const std::vector<std::string>& startup) {
  OrderedMap<std::string, std::vector<std::string>> stores;
  for (const ZpObjectMeta& z : objects) {
    for (std::size_t k = 0; k < z.init.size(); ++k) {
      stores[z.init[k]].push_back(k == 0 ? z.name : z.name + "+" + std::to_string(k));
    }
  }
  if (stores.empty() && startup.empty()) {
    return {};
  }
  std::vector<std::string> lines{std::string(kZpInitFunction) + ":",
                                 asm_emit::instr_line("SUBROUTINE"), ""};
  lines.insert(lines.end(), startup.begin(), startup.end());
  for (const bool zero : {true, false}) {
    for (const auto& [value, dests] : stores) {
      if ((value == "$00") != zero) {
        continue;
      }
      lines.push_back(asm_emit::instr_line("LDA", "#" + value));
      for (const std::string& d : dests) {
        lines.push_back(asm_emit::instr_line("STA", d));
      }
    }
  }
  lines.push_back(asm_emit::instr_line("RTS"));
  return lines;
}

std::vector<std::string> data_lines(const ZpObjectMeta& record) {
  const bool all_zero = std::all_of(record.init.begin(), record.init.end(),
                                    [](const std::string& v) { return v == "$00"; });
  if (record.init.empty() || all_zero) {
    return {record.name + ":", asm_emit::instr_line("DS.B", pyrepr::integer(record.size))};
  }
  std::string values;
  for (std::size_t i = 0; i < record.init.size(); ++i) {
    if (i != 0) {
      values += ", ";
    }
    values += record.init[i];
  }
  return {record.name + ":", asm_emit::instr_line("DC.B", values)};
}

}  // namespace c6502::passes::linker
