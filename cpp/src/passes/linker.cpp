// Linking is where c6502 does its whole-program optimization. Each unit
// leaves its zero-page slots, its zero-page objects, and the runtime's
// block to the linker, which places them for the program as a whole.
// `link_objects` runs these steps in this order.
//
//  1. For each object, `parse_metadata`, `object_items`, and
//     `without_init`, which leaves out the unit's `__zp_init`.
//  2. `runtime_library`, which adds `runtime.o` after the units, and
//     `global_definitions` for the absolute names.
//  3. `rename_locals`, with `rename_meta` inside it.
//  4. `merge`, `validate_eligibility`, and `drop_unused_locals`.
//  5. `merge_zp_objects`, `startup_lines` under a script,
//     `choose_layout`, and `place_zp_objects`.
//  6. `link_shares`.
//  7. `synthesize`, `allocate_zp_slots`, `allocate_function_locals`,
//     `build_local_slot_symbols`, and `bind_extern_params`.
//  8. `reassemble`, which assembles a unit again from its `.c6502.asm`
//     when a name it reached in zero page lands above $FF.
//  9. `ld::relax` on every section, and `merge_identical`.
// 10. The assembly of the linker's own code (`init_lines`, `__spill`,
//     and `data_lines` for a promoted object put back in the data
//     segment).
// 11. `lay_out`, the `Resolver`, and `output_symbols`.
// 12. `format_metadata` and `ld::executable`.
#include "passes/linker.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <set>
#include <utility>

#include "argparse.h"
#include "asm_emit.h"
#include "binutils/assembler.h"
#include "binutils/nm.h"
#include "errors.h"
#include "passes/link_layout.h"
#include "passes/linker_metadata.h"
#include "passes/linker_objects.h"
#include "passes/optimization/pool.h"
#include "passes/zp_globals.h"
#include "passes/zp_local_allocation.h"
#include "passes/zp_slot_allocation.h"
#include "pyrepr.h"

namespace c6502::passes::linker {

namespace {

using asdl::Int;

CompileError link_error(const std::string& message) { return CompileError("LinkError", message); }

constexpr Int kSpillBase = 0xF000;
constexpr const char* kSpill = "__spill";

bool is_runtime_name(const std::string& name) {
  return name == "SSP" || name == "FP" || name == "HARGS" || name == "DPTR";
}

std::int64_t to_int64(Int v) {
  if (v < Int(INT64_MIN) || v > Int(INT64_MAX)) {
    throw InternalError("a linker address " + asdl::int_to_string(v) +
                        " is past the 64 bits binutils/ld.h holds");
  }
  return static_cast<std::int64_t>(v);
}

ld::Names to_names(const SymbolAddrs& table) {
  ld::Names out;
  for (const auto& [name, v] : table) out.insert_or_assign(name, to_int64(v));
  return out;
}

}  // namespace

std::string origin_message(asdl::Int origin) {
  return "the origin " + link_layout::address_text(origin) +
         " is outside the 6502's address space ($0000-$FFFF)";
}

elf::ElfFile link_objects(ld::Objects inputs, const std::optional<link_script::Script>& script,
                          std::int64_t origin, const ld::Names& defsyms) {
  if (inputs.empty()) throw link_error("linking needs at least one object");
  if (!script && !(0 <= origin && origin < link_layout::kAddressSpace)) {
    throw link_error(origin_message(origin));
  }
  std::vector<LinkMetadata> metas;
  ld::Objects objects;
  for (auto& [path, obj] : inputs) {
    if (obj.type != elf::ET_REL) throw link_error(path + " isn't a relocatable object");
    const std::optional<std::string> text = obj.link_text();
    LinkMetadata meta;
    if (text && !text->empty()) {
      try {
        meta = parse_metadata(*text);
      } catch (const CompileError& e) {
        if (e.python_type() != "ValueError") throw;
        throw link_error(path + ": " + e.what());
      }
    }
    meta.items = object_items(meta, obj, path);
    objects.emplace_back(path, without_init(obj));
    std::erase_if(meta.items, [](const ItemMeta& it) { return it.name == kZpInitFunction; });
    metas.push_back(std::move(meta));
  }
  if (std::optional<Library> library = runtime_library(objects, defsyms)) {
    objects.push_back(std::move(library->object));
    metas.push_back(std::move(library->meta));
  }
  NameSet fixed;
  for (const auto& [name, _] : ld::global_definitions(objects).absolute) fixed.insert(name);
  const std::vector<Renames> renames = rename_locals(objects, metas, fixed);
  LinkMetadata merged = merge(metas);
  validate_eligibility(merged);
  drop_unused_locals(merged, objects);
  const ZpObjects zp_objects = merge_zp_objects(metas);
  std::vector<std::string> startup;
  if (script) startup = link_layout::startup_lines(*script);
  std::set<Int> param_bytes;
  for (const LinkMetadata& m : metas) {
    for (const FunctionMeta& r : m.defs) {
      for (const auto& a : r.pinned) {
        if (a) param_bytes.insert(*a);
      }
    }
    for (const ExternMeta& r : m.externs) {
      for (const auto& a : r.pinned) {
        if (a) param_bytes.insert(*a);
      }
    }
  }
  const LayoutDict layout =
      choose_layout(metas, zp_objects, startup.empty() ? 0 : 4, param_bytes);
  const auto [zp_addrs, demoted] =
      place_zp_objects(zp_objects, layout, param_bytes, pointer_names(objects));
  std::set<Int> zp_bytes = param_bytes;
  for (const auto& [name, addr] : zp_addrs) {
    const Int size = zp_objects.at(name).at(0).size;
    for (Int b = addr; b < addr + size; ++b) zp_bytes.insert(b);
  }
  // MSVC's `stable_sort` can't take an element with a 128-bit member,
  // whose alignment is past `max_align_t`'s, so the positions are sorted.
  const std::vector<std::pair<std::string, Int>> zp_items(zp_addrs.begin(), zp_addrs.end());
  std::vector<std::size_t> by_addr(zp_items.size());
  for (std::size_t i = 0; i < by_addr.size(); ++i) by_addr[i] = i;
  std::stable_sort(by_addr.begin(), by_addr.end(), [&](std::size_t a, std::size_t b) {
    return zp_items[a].second < zp_items[b].second;
  });
  merged.zp_objects.clear();
  for (const std::size_t i : by_addr) {
    const auto& [name, addr] = zp_items[i];
    const std::vector<ZpObjectMeta>& records = zp_objects.at(name);
    ZpObjectMeta record;
    record.name = name;
    record.size = records.at(0).size;
    record.at = addr;
    for (const ZpObjectMeta& r : records) {
      if (r.defined) {
        record.defined = true;
        record.init = r.init;
        break;
      }
    }
    merged.zp_objects.push_back(std::move(record));
  }
  merged.hargs_size = layout.at("HARGS size");
  merged.hargs_need.reset();
  for (const LinkMetadata& m : metas) {
    if (m.hargs_need && (!merged.hargs_need || *m.hargs_need > *merged.hargs_need)) {
      merged.hargs_need = m.hargs_need;
    }
  }
  merged.shares = link_shares(merged, metas);
  const Synthesized synthesized = synthesize(merged);
  ZpAllocationOptions options;
  const Pool pool;
  options.window = Range{pool.start, 0x100, 1};
  options.fallback = Range{0, pool.start, 1};
  options.reserved = zp_bytes;
  for (Int b : runtime_bytes(layout)) options.reserved.insert(b);
  options.shares = merged.shares;
  if (script) {
    options.spill_start = kSpillBase;
    options.spill_end = 0x10000;
  }
  const auto [abi, zp_sym_to_addr] =
      allocate_zp_slots(synthesized.prog, synthesized.abi, options);
  const LocalPools local_pools =
      allocate_function_locals(synthesized.prog, abi, synthesized.local_bytes, options);
  OrderedMap<std::string, std::vector<std::string>> slot_names_by_fn;
  for (const FunctionMeta& d : merged.defs) slot_names_by_fn.insert_or_assign(d.name, d.locals);
  const OrderedMap<std::string, Int> local_sym_to_addr =
      build_local_slot_symbols(local_pools, &slot_names_by_fn);
  SymbolAddrs runtime;
  for (const auto& [name, v] : layout) {
    if (is_runtime_name(name)) runtime.insert_or_assign(name, v);
  }
  SymbolAddrs placed_names;
  for (const SymbolAddrs* table : std::initializer_list<const SymbolAddrs*>{
           &runtime, &zp_sym_to_addr, &local_sym_to_addr, &zp_addrs}) {
    for (const auto& [name, v] : *table) placed_names.insert_or_assign(name, v);
  }
  std::vector<ExternMeta> externs;
  for (const LinkMetadata& m : metas) externs.insert(externs.end(), m.externs.begin(), m.externs.end());
  bind_extern_params(merged, externs, placed_names);
  SymbolAddrs spilled;
  if (script) {
    for (const auto& [name, addr] : placed_names) {
      if (addr >= kSpillBase) spilled.insert_or_assign(name, addr - kSpillBase);
    }
  }
  for (const auto& [name, _] : spilled) placed_names.erase(name);

  NameSet outside(demoted.begin(), demoted.end());
  for (const auto& [name, _] : spilled) outside.insert(name);
  reassemble(objects, renames, to_names(placed_names), outside);

  {
    const ld::Definitions definitions = ld::global_definitions(objects);
    const ld::Names placed = to_names(placed_names);
    const auto known = [&](const elf::SymbolPtr& sym) -> std::optional<std::int64_t> {
      if (sym->defined()) {
        if (sym->is_absolute()) return sym->value;
        return std::nullopt;
      }
      if (definitions.defs.contains(sym->name)) return std::nullopt;
      if (const std::int64_t* v = placed.find(sym->name)) return *v;
      if (const std::int64_t* v = definitions.absolute.find(sym->name)) return *v;
      if (const std::int64_t* v = defsyms.find(sym->name)) return *v;
      return std::nullopt;
    };
    for (auto& [_, obj] : objects) {
      for (const elf::SectionPtr& sec : obj.sections) ld::relax(sec, obj.symbols, known);
    }
  }

  merge_identical(objects, metas);

  std::vector<std::string> own_lines = init_lines(merged.zp_objects, startup);
  Int spill_size = 0;
  for (const auto& [_, offset] : spilled) spill_size = std::max(spill_size, offset + 1);
  if (spill_size != 0) {
    own_lines.push_back("");
    own_lines.push_back(std::string(kSpill) + ":");
    own_lines.push_back(asm_emit::instr_line("DS.B", asdl::int_to_string(spill_size)));
  }
  for (const std::string& name : demoted) {
    const ZpObjectMeta* record = nullptr;
    for (const ZpObjectMeta& r : zp_objects.at(name)) {
      if (r.defined) {
        record = &r;
        break;
      }
    }
    if (record == nullptr) {
      throw InternalError("the demoted object `" + name + "` has no definition");
    }
    own_lines.push_back("");
    for (std::string& line : data_lines(*record)) own_lines.push_back(std::move(line));
  }
  std::vector<link_layout::Unit> units;
  for (std::size_t k = 0; k < objects.size() && k < metas.size(); ++k) {
    units.push_back(unit(static_cast<int>(k), objects[k].first, objects[k].second, metas[k].items));
  }
  if (!own_lines.empty()) {
    std::string text;
    for (const std::string& line : own_lines) text += line + "\n";
    binutils::assembler::LinkSymbols link_symbols;
    {
      SymbolAddrs table = runtime;
      for (const auto& [name, v] : zp_addrs) table.insert_or_assign(name, v);
      for (const auto& [name, v] : table) link_symbols.emplace_back(name, v);
    }
    elf::ElfFile own;
    try {
      own = binutils::assembler::assemble(text, "the linker", &link_symbols);
    } catch (const CompileError& e) {
      if (e.python_type() != "AssemblerError") throw;
      throw link_error(std::string("the linker's own code: ") + e.what());
    }
    const std::vector<ItemMeta> own_items = object_items(LinkMetadata{}, own, "the linker");
    objects.emplace_back("the linker", own);
    units.push_back(unit(static_cast<int>(units.size()), "the linker", own, own_items));
  }

  ld::Names placed = to_names(placed_names);
  link_layout::Layout placement;
  {
    const ld::Definitions definitions = ld::global_definitions(objects);
    const auto lookup = [&](const std::string& name) -> std::optional<std::int64_t> {
      if (const std::int64_t* v = placed.find(name)) return *v;
      if (const std::int64_t* v = definitions.absolute.find(name)) return *v;
      if (const std::int64_t* v = defsyms.find(name)) return *v;
      return std::nullopt;
    };
    try {
      placement = link_layout::lay_out(script ? *script : link_layout::default_script(units, origin),
                                       units, lookup);
    } catch (const CompileError& e) {
      if (e.python_type() != "LayoutError") throw;
      throw link_error(e.what());
    }
  }
  std::vector<ld::Placed> parts;
  ld::PlacedBySection addr_of;
  for (const ld::OutputSection& out : placement.outputs) {
    for (const ld::Placed& p : out.parts) {
      parts.push_back(p);
      addr_of.insert_or_assign(p.section.get(), p);
    }
  }
  if (!spilled.empty()) {
    const elf::SectionPtr* spill_section = units.back().sections.find(kSpill);
    const auto it = spill_section == nullptr ? addr_of.end() : addr_of.find(spill_section->get());
    if (it == addr_of.end()) {
      throw InternalError("the layout placed no `__spill`");
    }
    const std::int64_t base = it->second.addr;
    for (const auto& [name, offset] : spilled) {
      placed.insert_or_assign(name, base + to_int64(offset));
    }
  }
  ld::Names extra = placed;
  for (const ld::Names* table :
       std::initializer_list<const ld::Names*>{&placement.symbols, &defsyms}) {
    for (const auto& [name, v] : *table) extra.insert_or_assign(name, v);
  }
  const ld::Resolver resolver(objects, parts, std::move(extra));
  const std::vector<ld::OutputSymbol> symbols = output_symbols(
      objects, placement.outputs, addr_of, placed, zp_objects, placement.symbols, defsyms);
  std::int64_t entry = 0;
  if (const auto* main = resolver.definitions().defs.find("main")) {
    entry = resolver.value(*main->second, main->first);
  }
  std::string link_text;
  for (const std::string& line : format_metadata(merged)) link_text += line + "\n";
  if (link_text.empty()) link_text = "\n";
  return ld::executable(objects, placement.outputs, resolver, symbols, entry, link_text);
}

void link_files(const std::vector<std::string>& input_paths, const std::string& output_path,
                const std::optional<std::string>& script_path, std::int64_t origin,
                const ld::Names& defsyms) {
  ld::Objects inputs;
  for (const std::string& path : input_paths) {
    std::string data;
    try {
      data = binutils::nm::read_path(path);
    } catch (const binutils::nm::FileError& e) {
      const std::string shown = ": " + pyrepr::string(argparse::path_str(path));
      std::string reason = e.message;
      if (const std::size_t at = reason.find("] "); reason.starts_with("[Errno ") &&
                                                     at != std::string::npos &&
                                                     reason.ends_with(shown)) {
        reason = reason.substr(at + 2, reason.size() - shown.size() - at - 2);
      }
      throw link_error(path + ": " + reason);
    }
    try {
      inputs.emplace_back(path, elf::read(data, path));
    } catch (const CompileError& e) {
      if (e.python_type() != "ElfError") throw;
      throw link_error(e.what());
    }
  }
  std::optional<link_script::Script> script;
  if (script_path) script = link_script::read_script(*script_path);
  const std::string bytes = elf::write(link_objects(std::move(inputs), script, origin, defsyms));
  std::ofstream out(std::filesystem::path(std::u8string(output_path.begin(), output_path.end())),
                    std::ios::binary);
  out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
  if (!out) {
    throw InternalError("the executable couldn't be written to " + output_path);
  }
}

}  // namespace c6502::passes::linker
