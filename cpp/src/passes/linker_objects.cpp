// `link_objects` runs the linker's functions on the objects in this
// order.
//
// 1. For each object, `object_items` lists its items and `without_init`
//    leaves out its `__zp_init`, which the linker replaces with one for
//    the whole program.
// 2. `runtime_library` adds the object of the runtime's helpers the
//    objects call, which no object defines and no `--defsym` gives, as
//    one more unit, `runtime.o`.
// 3. `rename_locals` gives the internal names two units share names of
//    their own, with `apply_renames` on each object's symbols.
// 4. `pointer_names` tells the placement of the zero-page objects which
//    of them a unit reaches as a pointer.
// 5. `reassemble` assembles a unit again from its `.c6502.asm` text
//    when a name it reached with zero-page addressing lands above $FF.
// 6. `ld::relax` runs on every section, and then `merge_identical`
//    keeps one copy of each table of row addresses that several units
//    make with the same bytes.
// 7. `unit` builds the layout's unit of each object, and after the
//    layout `output_symbols` gives the executable's symbol table, which
//    `ld::executable` writes.
#include "passes/linker_objects.h"

#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>

#include "argparse.h"
#include "binutils/assembler.h"
#include "binutils/opcodes.h"
#include "errors.h"
#include "passes/runtime_library.h"
#include "passes/zp_globals.h"
#include "pyrepr.h"

namespace c6502::passes::linker {

namespace {

CompileError link_error(const std::string& message) { return CompileError("LinkError", message); }

bool is_item_label(const elf::Symbol& s) {
  return s.section != nullptr && (s.type == elf::STT_FUNC || s.type == elf::STT_OBJECT);
}

std::string repr_list(const std::vector<std::string>& names) {
  std::string out = "[";
  for (std::size_t i = 0; i < names.size(); ++i) {
    if (i != 0) out += ", ";
    out += pyrepr::string(names[i]);
  }
  return out + "]";
}

bool target_less(const Target& a, const Target& b) {
  if (a.local != b.local) {
    throw InternalError(
        "two relocations at one offset name a local and a global symbol, where "
        "Python's sort raises TypeError");
  }
  if (a.local && a.unit != b.unit) return a.unit < b.unit;
  return a.name < b.name;
}

bool reloc_less(const Contents::Reloc& a, const Contents::Reloc& b) {
  if (a.offset != b.offset) return a.offset < b.offset;
  if (a.type != b.type) return a.type < b.type;
  if (!(a.target == b.target)) return target_less(a.target, b.target);
  return a.addend < b.addend;
}

struct ContentsLess {
  bool operator()(const Contents& a, const Contents& b) const {
    const auto key = [](const Contents& c) {
      return std::tie(c.name, c.type, c.flags, c.align, c.size, c.data);
    };
    if (key(a) != key(b)) return key(a) < key(b);
    const auto rkey = [](const Contents::Reloc& r) {
      return std::tie(r.offset, r.type, r.target.local, r.target.unit, r.target.name, r.addend);
    };
    return std::lexicographical_compare(
        a.relocs.begin(), a.relocs.end(), b.relocs.begin(), b.relocs.end(),
        [&](const Contents::Reloc& x, const Contents::Reloc& y) { return rkey(x) < rkey(y); });
  }
};

}  // namespace

std::optional<Library> runtime_library(const ld::Objects& objects, const ld::Names& defsyms) {
  const ld::Definitions d = ld::global_definitions(objects);
  NameSet defined;
  for (const auto& [name, _] : d.defs) defined.insert(name);
  for (const auto& [name, _] : d.absolute) defined.insert(name);
  for (const auto& [name, _] : defsyms) defined.insert(name);
  const std::vector<std::string> names = needed(objects, defined);
  if (names.empty()) return std::nullopt;
  elf::ElfFile obj = binutils::assembler::assemble(library_text(names), kLibraryName);
  const std::optional<std::string> text = obj.link_text();
  if (!text) throw InternalError("the runtime library's object has no link metadata");
  LinkMetadata meta = parse_metadata(*text);
  meta.items = object_items(meta, obj, kLibraryName);
  return Library{{kLibraryName, std::move(obj)}, std::move(meta)};
}

std::vector<ItemMeta> object_items(const LinkMetadata& meta, const elf::ElfFile& obj,
                                   const std::string& path) {
  OrderedMap<std::string, elf::SymbolPtr> defined;
  for (const elf::SymbolPtr& s : obj.symbols) {
    if (is_item_label(*s)) defined.insert_or_assign(s->name, s);
  }
  if (!meta.items.empty()) {
    std::vector<std::string> missing;
    for (const ItemMeta& it : meta.items) {
      if (!defined.contains(it.name)) missing.push_back(it.name);
    }
    if (!missing.empty()) {
      std::sort(missing.begin(), missing.end());
      throw link_error(path + ": the link metadata lists " + repr_list(missing) +
                       ", which the object doesn't define");
    }
    return meta.items;
  }
  std::unordered_map<const elf::Section*, std::size_t> order;
  for (std::size_t i = 0; i < obj.sections.size(); ++i) order.emplace(obj.sections[i].get(), i);
  std::vector<elf::SymbolPtr> syms;
  for (const auto& [_, s] : defined) {
    if (!order.contains(s->section.get())) {
      throw InternalError(
          "an item's section isn't one of its object's, where Python raises "
          "KeyError");
    }
    syms.push_back(s);
  }
  std::stable_sort(syms.begin(), syms.end(), [&](const elf::SymbolPtr& a, const elf::SymbolPtr& b) {
    return order.at(a->section.get()) < order.at(b->section.get());
  });
  std::vector<ItemMeta> out;
  for (const elf::SymbolPtr& s : syms) {
    out.push_back(ItemMeta{.name = s->name,
                           .section = s->section->name,
                           .local = !s->is_global(),
                           .zero = s->section->type == elf::SHT_NOBITS});
  }
  return out;
}

elf::SymbolPtr item_symbol(const elf::ElfFile& obj, const std::string& name) {
  for (const elf::SymbolPtr& s : obj.symbols) {
    if (s->name == name && is_item_label(*s)) return s;
  }
  throw InternalError("the object defines no item `" + name +
                      "`, where Python's next() raises StopIteration");
}

elf::ElfFile without_init(const elf::ElfFile& obj) {
  elf::SymbolPtr sym;
  for (const elf::SymbolPtr& s : obj.symbols) {
    if (s->name == kZpInitFunction && s->section != nullptr && s->type == elf::STT_FUNC) {
      sym = s;
      break;
    }
  }
  if (sym == nullptr) return obj;
  const elf::SectionPtr sec = sym->section;
  elf::ElfFile out;
  out.type = obj.type;
  out.entry = obj.entry;
  out.flags = obj.flags;
  for (const elf::SectionPtr& s : obj.sections) {
    if (s != sec) out.sections.push_back(s);
  }
  for (const elf::SymbolPtr& s : obj.symbols) {
    if (s->section != sec) out.symbols.push_back(s);
  }
  return out;
}

link_layout::Unit unit(int k, const std::string& path, const elf::ElfFile& obj,
                       const std::vector<ItemMeta>& items) {
  link_layout::Unit u;
  u.path = path;
  u.index = k;
  u.items = items;
  for (const ItemMeta& it : items)
    u.sections.insert_or_assign(it.name, item_symbol(obj, it.name)->section);
  for (const ItemMeta& it : items) {
    const elf::SectionPtr& sec = u.sections.at(it.name);
    if ((sec->flags & elf::SHF_EXECINSTR) != 0) {
      if (std::optional<std::string> target = link_layout::tail_jump(sec, obj.symbols)) {
        u.tails.insert_or_assign(it.name, *target);
      }
    }
  }
  return u;
}

void apply_renames(elf::ElfFile& obj, const Renames& renames) {
  for (const elf::SymbolPtr& s : obj.symbols) {
    if (const std::string* to = renames.find(s->name);
        to != nullptr && !(s->is_global() && s->defined())) {
      s->name = *to;
    }
  }
}

NameSet pointer_names(const ld::Objects& objects) {
  NameSet out;
  for (const auto& [_, obj] : objects) {
    for (const elf::SectionPtr& sec : obj.sections) {
      for (const elf::Reloc& r : sec->relocs) {
        if (r.type != elf::R_MOS_ADDR8 || r.offset == 0) continue;
        const std::int64_t at = r.offset - 1;
        if (at < 0 || at >= static_cast<std::int64_t>(sec->data.size())) {
          throw InternalError(
              "a relocation is outside its section, where Python raises IndexError");
        }
        const binutils::opcodes::Entry* op = binutils::opcodes::decode(
            static_cast<std::uint8_t>(sec->data[static_cast<std::size_t>(at)]));
        if (op != nullptr && (op->mode == binutils::opcodes::Mode::kIndx ||
                              op->mode == binutils::opcodes::Mode::kIndy)) {
          out.insert(r.symbol->name);
        }
      }
    }
  }
  return out;
}

void reassemble(ld::Objects& objects, const std::vector<Renames>& renames, const ld::Names& placed,
                const NameSet& outside) {
  OrderedMap<std::string, std::optional<asdl::Int>> values;
  for (const std::string& name : outside) values.insert_or_assign(name, std::nullopt);
  for (const auto& [name, addr] : placed) values.insert_or_assign(name, asdl::Int(addr));
  for (std::size_t k = 0; k < objects.size(); ++k) {
    const auto& [path, obj] = objects[k];
    bool clash = false;
    for (const elf::SectionPtr& sec : obj.sections) {
      for (const elf::Reloc& r : sec->relocs) {
        if (r.type != elf::R_MOS_ADDR8 || r.symbol->defined()) continue;
        const std::int64_t* at = placed.find(r.symbol->name);
        if (outside.contains(r.symbol->name) ||
            asdl::Int(at != nullptr ? *at : 0) + r.addend > 0xFF) {
          clash = true;
        }
      }
    }
    const std::optional<std::string> text = obj.source_text();
    if (!clash || !text) continue;
    std::map<std::string, std::string> back;
    for (const auto& [old_name, new_name] : renames.at(k)) back[new_name] = old_name;
    OrderedMap<std::string, std::optional<asdl::Int>> unit_values;
    for (const auto& [name, v] : values) {
      const auto it = back.find(name);
      unit_values.insert_or_assign(it != back.end() ? it->second : name, v);
    }
    std::string filename = path;
    for (const elf::SymbolPtr& s : obj.symbols) {
      if (s->type == elf::STT_FILE) {
        filename = s->name;
        break;
      }
    }
    binutils::assembler::LinkSymbols link_symbols(unit_values.begin(), unit_values.end());
    elf::ElfFile fresh;
    try {
      fresh = binutils::assembler::assemble(*text, filename, &link_symbols);
    } catch (const CompileError& e) {
      if (e.python_type() != "AssemblerError") throw;
      throw link_error(path +
                       ": assembling it again with the program's zero-page addresses: " + e.what());
    }
    fresh = without_init(fresh);
    apply_renames(fresh, renames.at(k));
    objects[k].second = std::move(fresh);
  }
}

std::vector<ld::OutputSymbol> output_symbols(
    const ld::Objects& objects, const std::vector<ld::OutputSection>& outputs,
    const ld::PlacedBySection& addr_of, const ld::Names& placed_names,
    const OrderedMap<std::string, std::vector<ZpObjectMeta>>& zp_objects,
    const ld::Names& layout_symbols, const ld::Names& defsyms) {
  std::unordered_map<const elf::Section*, int> output_of;
  for (std::size_t i = 0; i < outputs.size(); ++i) {
    for (const ld::Placed& p : outputs[i].parts) output_of[p.section.get()] = static_cast<int>(i);
  }
  std::vector<ld::OutputSymbol> out;
  std::set<std::string, std::less<>> seen_abs;
  const auto absolute = [](std::string name, std::int64_t value, std::int64_t size,
                           std::uint8_t bind, std::uint8_t type) {
    auto s = std::make_shared<elf::Symbol>();
    s->name = std::move(name);
    s->value = value;
    s->size = size;
    s->bind = bind;
    s->type = type;
    s->shndx = elf::SHN_ABS;
    return ld::OutputSymbol{s, -1};
  };
  for (const auto& [path, obj] : objects) {
    out.push_back(absolute(argparse::path_name(path), 0, 0, elf::STB_LOCAL, elf::STT_FILE));
    for (const elf::SymbolPtr& s : obj.symbols) {
      if (s->type == elf::STT_FILE || !s->defined()) continue;
      if (s->section == nullptr) {
        if (s->is_absolute() && s->is_global() && !seen_abs.contains(s->name)) {
          seen_abs.insert(s->name);
          out.push_back(absolute(s->name, s->value, 0, elf::STB_GLOBAL, elf::STT_NOTYPE));
        }
        continue;
      }
      const auto pit = addr_of.find(s->section.get());
      if (pit == addr_of.end()) continue;
      const ld::Placed& p = pit->second;
      const bool item = s->type == elf::STT_FUNC || s->type == elf::STT_OBJECT;
      if (s->value >= p.size && !item && p.size < s->section->size()) {
        continue;
      }
      const auto oit = output_of.find(s->section.get());
      if (oit == output_of.end()) {
        throw InternalError(
            "a placed section is in no output section, where Python raises KeyError");
      }
      auto sym = std::make_shared<elf::Symbol>();
      sym->name = s->name;
      sym->value = p.addr + s->value;
      sym->size = item ? p.size : s->size;
      sym->bind = s->bind;
      sym->type = s->type;
      sym->other = s->other;
      out.push_back(ld::OutputSymbol{sym, oit->second});
    }
  }
  std::map<std::string, std::int64_t, std::less<>> sizes;
  for (const auto& [name, records] : zp_objects) {
    sizes[name] = static_cast<std::int64_t>(records.at(0).size);
  }
  for (const ld::Names* table : {&placed_names, &layout_symbols, &defsyms}) {
    std::vector<std::pair<std::string, std::int64_t>> items(table->begin(), table->end());
    std::stable_sort(items.begin(), items.end(),
                     [](const auto& a, const auto& b) { return a.second < b.second; });
    for (const auto& [name, value] : items) {
      if (seen_abs.contains(name)) continue;
      seen_abs.insert(name);
      const auto size = sizes.find(name);
      out.push_back(absolute(name, value & 0xFFFF, size != sizes.end() ? size->second : 0,
                             elf::STB_GLOBAL,
                             size != sizes.end() ? elf::STT_OBJECT : elf::STT_NOTYPE));
    }
  }
  return out;
}

void merge_identical(ld::Objects& objects, std::vector<LinkMetadata>& metas) {
  std::map<Contents, elf::SymbolPtr, ContentsLess> kept;
  const std::size_t n = std::min(objects.size(), metas.size());
  for (std::size_t k = 0; k < n; ++k) {
    elf::ElfFile& obj = objects[k].second;
    LinkMetadata& meta = metas[k];
    std::set<std::string, std::less<>> merged;
    for (const ItemMeta& it : meta.items) {
      if (!it.merge) continue;
      const elf::SymbolPtr sym = item_symbol(obj, it.name);
      std::optional<Contents> key = contents(static_cast<int>(k), obj, sym);
      if (!key) continue;
      const elf::SymbolPtr first = kept.try_emplace(std::move(*key), sym).first->second;
      if (first == sym) continue;
      first->bind = elf::STB_GLOBAL;
      const elf::SectionPtr sec = sym->section;
      const auto at = std::find(obj.sections.begin(), obj.sections.end(), sec);
      if (at == obj.sections.end()) {
        throw InternalError(
            "a merged item's section isn't one of its object's, where Python's "
            "list.remove raises ValueError");
      }
      obj.sections.erase(at);
      sym->name = first->name;
      sym->section = nullptr;
      sym->shndx = elf::SHN_UNDEF;
      sym->value = 0;
      sym->size = 0;
      sym->bind = elf::STB_GLOBAL;
      sym->type = elf::STT_NOTYPE;
      merged.insert(it.name);
    }
    if (!merged.empty()) {
      std::erase_if(meta.items, [&](const ItemMeta& it) { return merged.contains(it.name); });
    }
  }
}

std::optional<Contents> contents(int k, const elf::ElfFile& obj, const elf::SymbolPtr& sym) {
  const elf::SectionPtr& sec = sym->section;
  for (const elf::SymbolPtr& s : obj.symbols) {
    if (s->section == sec && s != sym) return std::nullopt;
  }
  Contents out{.name = sec->name,
               .type = sec->type,
               .flags = sec->flags,
               .align = sec->align,
               .size = sec->size(),
               .data = sec->data};
  for (const elf::Reloc& r : sec->relocs) {
    const elf::Symbol& s = *r.symbol;
    const bool local = s.defined() && !s.is_global();
    out.relocs.push_back(Contents::Reloc{.offset = r.offset,
                                         .type = r.type,
                                         .target = Target{local, local ? k : 0, s.name},
                                         .addend = r.addend});
  }
  std::sort(out.relocs.begin(), out.relocs.end(), reloc_less);
  return out;
}

}  // namespace c6502::passes::linker
