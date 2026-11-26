#include "passes/linker_objects_checks.h"

#include <map>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "binutils/ld.h"
#include "errors.h"
#include "passes/linker_objects.h"
#include "passes/stage_registry_sharing.h"
#include "passes/zp_link_metadata.h"

namespace c6502::passes::linker_objects_checks {

namespace {

namespace elf = ::c6502::binutils::elf;
namespace ld = ::c6502::binutils::ld;
using json::Value;

Value null() { return Value{}; }

Value boolean(bool b) {
  Value v;
  v.kind = Value::Kind::kBool;
  v.boolean = b;
  return v;
}

Value integer(asdl::Int i) {
  Value v;
  v.kind = Value::Kind::kInt;
  v.integer = i;
  return v;
}

Value string(std::string s) {
  Value v;
  v.kind = Value::Kind::kString;
  v.string = std::move(s);
  return v;
}

Value array(std::vector<Value> items = {}) {
  Value v;
  v.kind = Value::Kind::kArray;
  v.items = std::move(items);
  return v;
}

Value object(std::vector<std::pair<std::string, Value>> members) {
  Value v;
  v.kind = Value::Kind::kObject;
  for (auto& [k, m] : members) {
    v.keys.push_back(k);
    v.items.push_back(std::move(m));
  }
  return v;
}

const Value& field(const Value& v, std::string_view key) {
  const Value* f = v.get(key);
  if (f == nullptr) throw InternalError("the record has no `" + std::string(key) + "`");
  return *f;
}

std::int64_t to_int(const Value& v) {
  if (v.kind != Value::Kind::kInt) throw InternalError("expected an integer in the record");
  return static_cast<std::int64_t>(v.integer);
}

std::string hex(std::string_view bytes) {
  static constexpr char kDigits[] = "0123456789abcdef";
  std::string out;
  out.reserve(bytes.size() * 2);
  for (const char c : bytes) {
    const auto b = static_cast<unsigned char>(c);
    out += kDigits[b >> 4];
    out += kDigits[b & 15];
  }
  return out;
}

std::string unhex(std::string_view text) {
  const auto digit = [](char c) -> int {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    throw InternalError("bad hex in the record");
  };
  std::string out;
  for (std::size_t i = 0; i + 1 < text.size(); i += 2) {
    out += static_cast<char>(digit(text[i]) * 16 + digit(text[i + 1]));
  }
  return out;
}

Value elf_value(const elf::ElfFile& obj) {
  std::vector<elf::SectionPtr> secs = obj.sections;
  std::unordered_map<const elf::Section*, std::size_t> sec_ix;
  for (std::size_t i = 0; i < secs.size(); ++i) sec_ix[secs[i].get()] = i;
  std::vector<elf::SymbolPtr> syms = obj.symbols;
  std::unordered_map<const elf::Symbol*, std::size_t> sym_ix;
  for (std::size_t i = 0; i < syms.size(); ++i) sym_ix[syms[i].get()] = i;
  const auto sec_ref = [&](const elf::Symbol& s) -> asdl::Int {
    if (s.section == nullptr) return -1 - asdl::Int(s.shndx);
    const auto it = sec_ix.find(s.section.get());
    if (it != sec_ix.end()) return asdl::Int(it->second);
    sec_ix[s.section.get()] = secs.size();
    secs.push_back(s.section);
    return asdl::Int(secs.size() - 1);
  };
  const auto sym_ref = [&](const elf::SymbolPtr& s) -> asdl::Int {
    const auto it = sym_ix.find(s.get());
    if (it != sym_ix.end()) return asdl::Int(it->second);
    sym_ix[s.get()] = syms.size();
    syms.push_back(s);
    return asdl::Int(syms.size() - 1);
  };
  std::vector<Value> out_syms;
  std::vector<Value> out_secs;
  std::size_t i = 0;
  std::size_t j = 0;
  while (i < syms.size() || j < secs.size()) {
    while (i < syms.size()) {
      const elf::SymbolPtr s = syms[i++];
      out_syms.push_back(
          array({string(s->name), integer(s->value), integer(s->size), integer(s->bind),
                 integer(s->type), integer(sec_ref(*s)), integer(s->other)}));
    }
    while (j < secs.size()) {
      const elf::SectionPtr c = secs[j++];
      std::vector<Value> relocs;
      for (const elf::Reloc& r : c->relocs) {
        relocs.push_back(array(
            {integer(r.offset), integer(r.type), integer(sym_ref(r.symbol)), integer(r.addend)}));
      }
      out_secs.push_back(array({string(c->name), integer(c->type), integer(c->flags),
                                string(hex(c->data)), integer(c->nobits_size), integer(c->addr),
                                integer(c->align), array(std::move(relocs))}));
    }
  }
  std::vector<Value> segments;
  for (const elf::Segment& g : obj.segments) {
    segments.push_back(array({integer(g.vaddr), integer(g.paddr), string(hex(g.data)),
                              integer(g.memsz), integer(g.flags)}));
  }
  return object({{"type", integer(obj.type)},
                 {"entry", integer(obj.entry)},
                 {"flags", integer(obj.flags)},
                 {"nsections", integer(asdl::Int(obj.sections.size()))},
                 {"sections", array(std::move(out_secs))},
                 {"nsymbols", integer(asdl::Int(obj.symbols.size()))},
                 {"symbols", array(std::move(out_syms))},
                 {"segments", array(std::move(segments))}});
}

}  // namespace

std::string elf_json(const elf::ElfFile& obj) { return json::dump(elf_value(obj)); }

elf::ElfFile elf_from_json(const Value& v) {
  elf::ElfFile obj;
  obj.type = static_cast<std::uint16_t>(to_int(field(v, "type")));
  obj.entry = to_int(field(v, "entry"));
  obj.flags = static_cast<std::uint32_t>(to_int(field(v, "flags")));
  const auto& sec_items = field(v, "sections").items;
  const auto& sym_items = field(v, "symbols").items;
  std::vector<elf::SectionPtr> secs;
  for (std::size_t i = 0; i < sec_items.size(); ++i)
    secs.push_back(std::make_shared<elf::Section>());
  std::vector<elf::SymbolPtr> syms;
  for (std::size_t i = 0; i < sym_items.size(); ++i)
    syms.push_back(std::make_shared<elf::Symbol>());
  for (std::size_t i = 0; i < sec_items.size(); ++i) {
    const auto& f = sec_items[i].items;
    elf::Section& c = *secs[i];
    c.name = f.at(0).string;
    c.type = static_cast<std::uint32_t>(to_int(f.at(1)));
    c.flags = static_cast<std::uint32_t>(to_int(f.at(2)));
    c.data = unhex(f.at(3).string);
    c.nobits_size = to_int(f.at(4));
    c.addr = to_int(f.at(5));
    c.align = to_int(f.at(6));
    for (const Value& r : f.at(7).items) {
      c.relocs.push_back(
          elf::Reloc{.offset = to_int(r.items.at(0)),
                     .type = static_cast<std::uint8_t>(to_int(r.items.at(1))),
                     .symbol = syms.at(static_cast<std::size_t>(to_int(r.items.at(2)))),
                     .addend = to_int(r.items.at(3))});
    }
  }
  for (std::size_t i = 0; i < sym_items.size(); ++i) {
    const auto& f = sym_items[i].items;
    elf::Symbol& s = *syms[i];
    s.name = f.at(0).string;
    s.value = to_int(f.at(1));
    s.size = to_int(f.at(2));
    s.bind = static_cast<std::uint8_t>(to_int(f.at(3)));
    s.type = static_cast<std::uint8_t>(to_int(f.at(4)));
    const std::int64_t ref = to_int(f.at(5));
    if (ref >= 0) {
      s.section = secs.at(static_cast<std::size_t>(ref));
    } else {
      s.shndx = static_cast<std::uint16_t>(-1 - ref);
    }
    s.other = static_cast<std::uint8_t>(to_int(f.at(6)));
  }
  const auto nsecs = static_cast<std::size_t>(to_int(field(v, "nsections")));
  const auto nsyms = static_cast<std::size_t>(to_int(field(v, "nsymbols")));
  obj.sections.assign(secs.begin(), secs.begin() + static_cast<std::ptrdiff_t>(nsecs));
  obj.symbols.assign(syms.begin(), syms.begin() + static_cast<std::ptrdiff_t>(nsyms));
  for (const Value& g : field(v, "segments").items) {
    obj.segments.push_back(
        elf::Segment{.vaddr = to_int(g.items.at(0)),
                     .paddr = to_int(g.items.at(1)),
                     .data = unhex(g.items.at(2).string),
                     .memsz = to_int(g.items.at(3)),
                     .flags = static_cast<std::uint32_t>(to_int(g.items.at(4)))});
  }
  return obj;
}

namespace {

constexpr std::string_view kKinds[] = {"global_definitions",     "relax",        "executable",
                                       "linker_runtime_library", "object_items", "item_symbol",
                                       "without_init",           "unit",         "apply_renames",
                                       "pointer_names",          "reassemble",   "output_symbols",
                                       "merge_identical",        "contents"};

class Objects {
 public:
  Objects(const Value& list, const BlobReader& blob) : blob_(blob) {
    for (const Value& pair : list.items) {
      objects.emplace_back(pair.items.at(0).string, read(pair.items.at(1).string));
    }
  }

  elf::ElfFile read(const std::string& hash) const {
    return elf_from_json(json::parse(blob_(hash)));
  }

  elf::SectionPtr section(const Value& where) const {
    const auto k = static_cast<std::size_t>(to_int(where.items.at(0)));
    const auto j = static_cast<std::size_t>(to_int(where.items.at(1)));
    return objects.at(k).second.sections.at(j);
  }

  std::string compare(const Value& want) const {
    if (want.items.size() != objects.size()) return "the number of objects differs";
    for (std::size_t k = 0; k < objects.size(); ++k) {
      const auto& pair = want.items[k];
      if (objects[k].first != pair.items.at(0).string)
        return "object " + std::to_string(k) + "'s name differs";
      if (std::string problem = compare_one(objects[k].second, pair.items.at(1).string);
          !problem.empty()) {
        return "object " + std::to_string(k) + ": " + problem;
      }
    }
    return "";
  }

  std::string compare_one(const elf::ElfFile& obj, const std::string& hash) const {
    const std::string got = elf_json(obj);
    const std::string want = blob_(hash);
    if (got == want) return "";
    std::size_t at = 0;
    while (at < got.size() && at < want.size() && got[at] == want[at]) ++at;
    const std::size_t from = at > 60 ? at - 60 : 0;
    return "the object differs at character " + std::to_string(at) +
           "\nC++:    " + got.substr(from, 160) + "\nPython: " + want.substr(from, 160);
  }

  ld::Objects objects;

 private:
  const BlobReader& blob_;
};

std::vector<ItemMeta> items_from(const Value& v) {
  std::vector<ItemMeta> out;
  for (const Value& it : v.items) {
    out.push_back(ItemMeta{.name = it.items.at(0).string,
                           .section = it.items.at(1).string,
                           .local = it.items.at(2).boolean,
                           .zero = it.items.at(3).boolean,
                           .merge = it.items.at(4).boolean});
  }
  return out;
}

Value items_json(const std::vector<ItemMeta>& items) {
  std::vector<Value> out;
  for (const ItemMeta& it : items) {
    out.push_back(array({string(it.name), string(it.section), boolean(it.local), boolean(it.zero),
                         boolean(it.merge)}));
  }
  return array(std::move(out));
}

ld::Names names_from(const Value& v) {
  ld::Names out;
  for (const Value& pair : v.items)
    out.insert_or_assign(pair.items.at(0).string, to_int(pair.items.at(1)));
  return out;
}

linker::Renames renames_from(const Value& v) {
  linker::Renames out;
  for (const Value& pair : v.items)
    out.insert_or_assign(pair.items.at(0).string, pair.items.at(1).string);
  return out;
}

std::size_t index_of(const std::vector<elf::SymbolPtr>& symbols, const elf::SymbolPtr& s) {
  for (std::size_t i = 0; i < symbols.size(); ++i) {
    if (symbols[i] == s) return i;
  }
  throw InternalError("the symbol isn't in its object's table");
}

std::vector<ld::OutputSection> outputs_from(const Value& v, const Objects& objects) {
  std::vector<ld::OutputSection> out;
  for (const Value& o : v.items) {
    ld::OutputSection s;
    s.name = field(o, "name").string;
    s.addr = to_int(field(o, "addr"));
    s.load_addr = to_int(field(o, "load_addr"));
    s.size = to_int(field(o, "size"));
    s.noload = field(o, "noload").boolean;
    s.executable = field(o, "executable").boolean;
    for (const Value& p : field(o, "parts").items) {
      s.parts.push_back(ld::Placed{.unit = static_cast<int>(to_int(p.items.at(0))),
                                   .section = objects.section(p.items.at(1)),
                                   .addr = to_int(p.items.at(2)),
                                   .size = to_int(p.items.at(3))});
    }
    out.push_back(std::move(s));
  }
  return out;
}

Value output_symbols_json(const std::vector<ld::OutputSymbol>& symbols) {
  std::vector<Value> out;
  for (const ld::OutputSymbol& o : symbols) {
    const elf::Symbol& s = *o.symbol;
    Value sec = o.output >= 0 ? array({string("output"), integer(o.output)})
                              : array({string("shndx"), integer(s.shndx)});
    out.push_back(array({string(s.name), integer(s.value), integer(s.size), integer(s.bind),
                         integer(s.type), std::move(sec), integer(s.other)}));
  }
  return array(std::move(out));
}

std::string compare(const Value& got, const Value& want) {
  const std::string g = json::dump(got);
  const std::string w = json::dump(want);
  if (g == w) return "";
  return "the result differs\nC++:    " + g + "\nPython: " + w;
}

}  // namespace

bool handles(std::string_view kind) {
  for (std::string_view k : kKinds) {
    if (k == kind) return true;
  }
  return false;
}

std::string check(std::string_view kind, const Value& args, const Value& input, const Value& output,
                  bool is_error, const BlobReader& blob) {
  Value got;
  std::string extra;
  bool compared = false;
  try {
    if (kind == "global_definitions") {
      const Objects objects(field(input, "objects"), blob);
      const ld::Definitions d = ld::global_definitions(objects.objects);
      std::vector<Value> defs;
      for (const auto& [name, def] : d.defs) {
        defs.push_back(
            array({string(name), integer(def.first),
                   integer(asdl::Int(index_of(
                       objects.objects.at(static_cast<std::size_t>(def.first)).second.symbols,
                       def.second)))}));
      }
      std::vector<Value> absolute;
      for (const auto& [name, value] : d.absolute)
        absolute.push_back(array({string(name), integer(value)}));
      got = object({{"defs", array(std::move(defs))}, {"absolute", array(std::move(absolute))}});
    } else if (kind == "relax") {
      const Value& sec_json = field(input, "section");
      auto sec = std::make_shared<elf::Section>();
      auto elsewhere = std::make_shared<elf::Section>();
      std::vector<elf::SymbolPtr> all;
      for (const Value& s : field(input, "symbols").items) {
        auto sym = std::make_shared<elf::Symbol>();
        sym->name = s.items.at(0).string;
        sym->value = to_int(s.items.at(1));
        sym->size = to_int(s.items.at(2));
        sym->bind = static_cast<std::uint8_t>(to_int(s.items.at(3)));
        sym->type = static_cast<std::uint8_t>(to_int(s.items.at(4)));
        sym->section = s.items.at(5).boolean ? sec : elsewhere;
        sym->other = static_cast<std::uint8_t>(to_int(s.items.at(6)));
        all.push_back(std::move(sym));
      }
      const auto& f = sec_json.items;
      sec->name = f.at(0).string;
      sec->type = static_cast<std::uint32_t>(to_int(f.at(1)));
      sec->flags = static_cast<std::uint32_t>(to_int(f.at(2)));
      sec->data = unhex(f.at(3).string);
      sec->nobits_size = to_int(f.at(4));
      sec->addr = to_int(f.at(5));
      sec->align = to_int(f.at(6));
      std::unordered_map<const elf::Symbol*, std::optional<std::int64_t>> values;
      const auto& recorded = field(input, "values").items;
      for (std::size_t i = 0; i < f.at(7).items.size(); ++i) {
        const Value& r = f.at(7).items[i];
        elf::SymbolPtr sym = all.at(static_cast<std::size_t>(to_int(r.items.at(2))));
        const Value& v = recorded.at(i);
        values[sym.get()] =
            v.kind == Value::Kind::kNull ? std::nullopt : std::optional<std::int64_t>(to_int(v));
        sec->relocs.push_back(elf::Reloc{.offset = to_int(r.items.at(0)),
                                         .type = static_cast<std::uint8_t>(to_int(r.items.at(1))),
                                         .symbol = std::move(sym),
                                         .addend = to_int(r.items.at(3))});
      }
      const auto nsyms = static_cast<std::size_t>(to_int(field(input, "nsymbols")));
      const std::vector<elf::SymbolPtr> symbols(all.begin(),
                                                all.begin() + static_cast<std::ptrdiff_t>(nsyms));
      const ld::ValueOf value_of = [&](const elf::SymbolPtr& s) -> std::optional<std::int64_t> {
        const auto it = values.find(s.get());
        if (it == values.end()) {
          extra = "C++ asked for the value of `" + s->name + "`, which no relocation names";
          return std::nullopt;
        }
        return it->second;
      };
      const std::int64_t saved = ld::relax(sec, symbols, value_of);
      std::vector<Value> relocs;
      for (const elf::Reloc& r : sec->relocs) {
        relocs.push_back(array({integer(r.offset), integer(r.type),
                                integer(asdl::Int(index_of(all, r.symbol))), integer(r.addend)}));
      }
      std::vector<Value> own;
      for (const elf::SymbolPtr& s : symbols) {
        if (s->section == sec) own.push_back(array({integer(s->value), integer(s->size)}));
      }
      got = object({{"saved", integer(saved)},
                    {"data", string(hex(sec->data))},
                    {"relocs", array(std::move(relocs))},
                    {"own", array(std::move(own))}});
    } else if (kind == "executable") {
      Objects objects(field(input, "objects"), blob);
      std::vector<ld::OutputSection> outputs = outputs_from(field(input, "outputs"), objects);
      std::vector<ld::Placed> placed;
      for (const Value& p : field(input, "placed").items) {
        placed.push_back(
            ld::Placed{.section = objects.section(p.items.at(0)), .addr = to_int(p.items.at(1))});
      }
      const ld::Resolver resolver(objects.objects, placed, names_from(field(input, "extra")));
      std::vector<ld::OutputSymbol> symbols;
      for (const Value& s : field(input, "symbols").items) {
        auto sym = std::make_shared<elf::Symbol>();
        sym->name = s.items.at(0).string;
        sym->value = to_int(s.items.at(1));
        sym->size = to_int(s.items.at(2));
        sym->bind = static_cast<std::uint8_t>(to_int(s.items.at(3)));
        sym->type = static_cast<std::uint8_t>(to_int(s.items.at(4)));
        sym->other = static_cast<std::uint8_t>(to_int(s.items.at(6)));
        const Value& sec = s.items.at(5);
        const std::string& how = sec.items.at(0).string;
        int index = -1;
        if (how == "output") {
          index = static_cast<int>(to_int(sec.items.at(1)));
        } else if (how == "section") {
          sym->section = objects.section(array({sec.items.at(1), sec.items.at(2)}));
        } else {
          sym->shndx = static_cast<std::uint16_t>(to_int(sec.items.at(1)));
        }
        symbols.push_back(ld::OutputSymbol{std::move(sym), index});
      }
      const Value& text = field(args, "link_text");
      const elf::ElfFile exe = ld::executable(
          objects.objects, outputs, resolver, symbols, to_int(field(args, "entry")),
          text.kind == Value::Kind::kNull ? std::nullopt : std::optional<std::string>(text.string));
      if (!is_error) {
        compared = true;
        if (elf::write(exe) != blob(field(output, "executable").string))
          extra = "the executable differs";
      }
    } else if (kind == "linker_runtime_library") {
      const Objects objects(field(input, "objects"), blob);
      const std::optional<linker::Library> library =
          linker::runtime_library(objects.objects, names_from(field(args, "defsyms")));
      if (!is_error) {
        compared = true;
        if (!library) {
          if (output.kind != Value::Kind::kNull) extra = "C++ added no library, and Python did";
        } else if (output.kind == Value::Kind::kNull) {
          extra = "C++ added a library, and Python didn't";
        } else if (library->object.first != field(output, "name").string) {
          extra = "the library's name differs";
        } else if (elf::write(library->object.second) != blob(field(output, "object").string)) {
          extra = "the library's object differs";
        } else if (const std::string meta = stage_sharing::link_metadata_json(library->meta);
                   meta != json::dump(field(output, "meta"))) {
          extra = "the library's metadata differs\nC++:    " + meta +
                  "\nPython: " + json::dump(field(output, "meta"));
        }
      }
    } else if (kind == "object_items") {
      const Objects objects(array(), blob);
      LinkMetadata meta;
      meta.items = items_from(field(input, "items"));
      got = items_json(linker::object_items(meta, objects.read(field(input, "elf").string),
                                            field(args, "path").string));
    } else if (kind == "item_symbol") {
      const Objects objects(array(), blob);
      const elf::ElfFile obj = objects.read(field(input, "elf").string);
      got = integer(
          asdl::Int(index_of(obj.symbols, linker::item_symbol(obj, field(args, "name").string))));
    } else if (kind == "without_init") {
      const Objects objects(array(), blob);
      const elf::ElfFile obj = objects.read(field(input, "elf").string);
      const elf::ElfFile out = linker::without_init(obj);
      const bool same = out.sections == obj.sections && out.symbols == obj.symbols;
      if (!is_error) {
        compared = true;
        if (same != field(output, "same").boolean) {
          extra = "C++ and Python differ on whether the object is the same";
        } else {
          extra = objects.compare_one(out, field(output, "elf").string);
        }
      }
    } else if (kind == "unit") {
      const Objects objects(array(), blob);
      const elf::ElfFile obj = objects.read(field(input, "elf").string);
      const link_layout::Unit u =
          linker::unit(static_cast<int>(to_int(field(args, "k"))), field(args, "path").string, obj,
                       items_from(field(input, "items")));
      std::vector<Value> sections;
      for (const auto& [name, sec] : u.sections) {
        std::size_t at = 0;
        while (at < obj.sections.size() && obj.sections[at] != sec) ++at;
        sections.push_back(array({string(name), integer(asdl::Int(at))}));
      }
      std::vector<Value> tails;
      for (const auto& [name, target] : u.tails)
        tails.push_back(array({string(name), string(target)}));
      got = object({{"path", string(u.path)},
                    {"index", integer(u.index)},
                    {"items", items_json(u.items)},
                    {"sections", array(std::move(sections))},
                    {"tails", array(std::move(tails))}});
    } else if (kind == "apply_renames") {
      const Objects objects(array(), blob);
      elf::ElfFile obj = objects.read(field(input, "elf").string);
      linker::apply_renames(obj, renames_from(field(args, "renames")));
      std::vector<Value> names;
      for (const elf::SymbolPtr& s : obj.symbols) names.push_back(string(s->name));
      got = array(std::move(names));
    } else if (kind == "pointer_names") {
      const Objects objects(field(input, "objects"), blob);
      std::vector<Value> names;
      for (const std::string& name : linker::pointer_names(objects.objects))
        names.push_back(string(name));
      got = array(std::move(names));
    } else if (kind == "reassemble") {
      Objects objects(field(input, "objects"), blob);
      std::vector<linker::Renames> renames;
      for (const Value& r : field(args, "renames").items) renames.push_back(renames_from(r));
      NameSet outside;
      for (const Value& name : field(args, "outside").items) outside.insert(name.string);
      linker::reassemble(objects.objects, renames, names_from(field(args, "placed")), outside);
      if (!is_error) {
        compared = true;
        extra = objects.compare(field(output, "objects"));
      }
    } else if (kind == "output_symbols") {
      const Objects objects(field(input, "objects"), blob);
      const std::vector<ld::OutputSection> outputs = outputs_from(field(input, "outputs"), objects);
      ld::PlacedBySection addr_of;
      for (const Value& p : field(input, "addr_of").items) {
        const elf::SectionPtr sec = objects.section(p.items.at(0));
        addr_of[sec.get()] = ld::Placed{.unit = static_cast<int>(to_int(p.items.at(1))),
                                        .section = sec,
                                        .addr = to_int(p.items.at(2)),
                                        .size = to_int(p.items.at(3))};
      }
      OrderedMap<std::string, std::vector<ZpObjectMeta>> zp_objects;
      for (const Value& pair : field(input, "zp_sizes").items) {
        zp_objects.insert_or_assign(
            pair.items.at(0).string,
            std::vector<ZpObjectMeta>{ZpObjectMeta{.size = pair.items.at(1).integer}});
      }
      got = output_symbols_json(linker::output_symbols(
          objects.objects, outputs, addr_of, names_from(field(input, "placed_names")), zp_objects,
          names_from(field(input, "layout_symbols")), names_from(field(input, "defsyms"))));
    } else if (kind == "merge_identical") {
      Objects objects(field(input, "objects"), blob);
      std::vector<LinkMetadata> metas;
      for (const Value& items : field(input, "metas").items) {
        LinkMetadata meta;
        meta.items = items_from(items);
        metas.push_back(std::move(meta));
      }
      linker::merge_identical(objects.objects, metas);
      if (!is_error) {
        compared = true;
        extra = objects.compare(field(output, "objects"));
        std::vector<Value> items;
        for (const LinkMetadata& m : metas) items.push_back(items_json(m.items));
        if (extra.empty()) extra = compare(array(std::move(items)), field(output, "metas"));
      }
    } else if (kind == "contents") {
      const Objects objects(array(), blob);
      const elf::ElfFile obj = objects.read(field(input, "elf").string);
      const int k = static_cast<int>(to_int(field(args, "k")));
      const std::optional<linker::Contents> c = linker::contents(
          k, obj, obj.symbols.at(static_cast<std::size_t>(to_int(field(input, "symbol")))));
      if (!c) {
        got = null();
      } else {
        std::vector<Value> relocs;
        for (const auto& r : c->relocs) {
          Value target = r.target.local ? array({integer(r.target.unit), string(r.target.name)})
                                        : string(r.target.name);
          relocs.push_back(
              array({integer(r.offset), integer(r.type), std::move(target), integer(r.addend)}));
        }
        got = array({string(c->name), integer(c->type), integer(c->flags), integer(c->align),
                     integer(c->size), string(hex(c->data)), array(std::move(relocs))});
      }
    } else {
      return "no check for records of " + std::string(kind);
    }
  } catch (const CompileError& e) {
    if (!is_error) return std::string("C++ raised ") + e.python_type() + ": " + e.what();
    if (e.python_type() != field(output, "type").string ||
        e.what() != field(output, "message").string) {
      return std::string("C++ raised ") + e.python_type() + ": " + e.what() + "\nPython raised " +
             field(output, "type").string + ": " + field(output, "message").string;
    }
    return extra;
  }
  if (is_error) {
    return "C++ gave a result where Python raised " + field(output, "type").string + ": " +
           field(output, "message").string;
  }
  if (!extra.empty() || compared) return extra;
  return compare(got, output);
}

}  // namespace c6502::passes::linker_objects_checks
