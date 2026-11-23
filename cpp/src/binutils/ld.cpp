// The parts of linking that know nothing about c6502's link metadata
// (binutils/ld.py): resolving symbols across objects, applying
// relocations, and writing the executable. passes/linker.py decides
// where each section goes (by the linker script, or one after another)
// and which addresses the linker-placed names get, then calls
// `relocate` and `executable`.
//
// A `Placed` section is one input section at its run address. Every
// symbol a relocation names resolves to one of these, tried in order.
//
// - A symbol defined in the same object, at its section's address plus
//   its offset. Local symbols resolve only this way.
// - A global symbol another object defines, or an absolute one.
// - A name the linker itself defines (`extra`), such as a slot address
//   or `__text_start`.
//
// An absolute symbol that several objects define has to have one value.
//
// Python computes with integers that have no bound. The values here
// come from 32-bit fields of the objects and from the linker's 64-bit
// tables, so `relocate` computes S + A in 128 bits and no sum
// overflows.
#include "binutils/ld.h"

#include <algorithm>
#include <set>
#include <string_view>

#include "asdl/runtime.h"
#include "binutils/opcodes.h"
#include "errors.h"
#include "pyrepr.h"

namespace c6502::binutils::ld {

namespace {

CompileError link_error(const std::string& message) { return CompileError("LinkError", message); }

// Returns Python's normalization of the slice bound `i` of a sequence of
// `n` items: a negative bound counts from the end, and the result is
// clamped to [0, n].
std::int64_t slice_bound(std::int64_t i, std::int64_t n) {
  if (i < 0) i += n;
  return std::clamp<std::int64_t>(i, 0, n);
}

// Returns `data[start:stop]`, as Python slices a bytearray.
std::string py_slice(const std::string& data, std::int64_t start, std::int64_t stop) {
  const auto n = static_cast<std::int64_t>(data.size());
  const std::int64_t i = slice_bound(start, n);
  const std::int64_t j = std::max(i, slice_bound(stop, n));
  return data.substr(static_cast<std::size_t>(i), static_cast<std::size_t>(j - i));
}

// Does `data[start:stop] = value`, as Python assigns to a slice of a
// bytearray: the bytes the slice selects are replaced by `value`, which
// grows or shrinks `data` when the lengths differ.
void py_slice_assign(std::string& data, std::int64_t start, std::int64_t stop,
                     const std::string& value) {
  const auto n = static_cast<std::int64_t>(data.size());
  const std::int64_t i = slice_bound(start, n);
  const std::int64_t j = std::max(i, slice_bound(stop, n));
  data.replace(static_cast<std::size_t>(i), static_cast<std::size_t>(j - i), value);
}

// Does `data[index] = byte`, which raises Python's IndexError for an
// index outside `data`. No object c6502 reads has a relocation outside
// its section, since elf.read rejects one.
void set_byte(std::string& data, std::int64_t index, std::uint8_t byte) {
  const auto n = static_cast<std::int64_t>(data.size());
  if (index < -n || index >= n) {
    throw InternalError(
        "a relocation's byte is outside its section, where Python raises IndexError");
  }
  data[static_cast<std::size_t>(index < 0 ? index + n : index)] = static_cast<char>(byte);
}

// The names of the runtime's helpers, which the compiler calls (Python's
// `_HELPER`, which matches `(mul|udivmod|sdivmod|asl|asr|lsr)
// (8|16|32|64)|icall|[fd](add|sub|mul|div|lt|le)|(i|u|l|ul|ll|ull|f|d)
// 2(f|d|i|u|l|ul|ll|ull)` against the whole name). The pattern matches
// a finite set of names, which is listed here in full. The linker adds
// the helpers the runtime library has (passes/runtime_library.py) to a
// program that calls them, so a name here that is still undefined is
// one of the helpers without 6502 code (CLAUDE.md, "Not yet in the
// repo").
const std::set<std::string, std::less<>>& helper_names() {
  static const std::set<std::string, std::less<>> names = [] {
    std::set<std::string, std::less<>> out;
    for (const char* op : {"mul", "udivmod", "sdivmod", "asl", "asr", "lsr"}) {
      for (const char* width : {"8", "16", "32", "64"}) out.insert(std::string(op) + width);
    }
    out.insert("icall");
    for (const char* t : {"f", "d"}) {
      for (const char* op : {"add", "sub", "mul", "div", "lt", "le"})
        out.insert(std::string(t) + op);
    }
    for (const char* from : {"i", "u", "l", "ul", "ll", "ull", "f", "d"}) {
      for (const char* to : {"f", "d", "i", "u", "l", "ul", "ll", "ull"}) {
        out.insert(std::string(from) + "2" + to);
      }
    }
    return out;
  }();
  return names;
}

// Returns a hint to add to the error for an undefined `name`.
std::string undefined_hint(const std::string& name) {
  if (helper_names().contains(name)) {
    return " (a runtime helper that has no 6502 code in the runtime library yet, so give its "
           "address with --defsym)";
  }
  return "";
}

}  // namespace

Definitions global_definitions(const Objects& objects) {
  Definitions out;
  OrderedMap<std::string, std::string> abs_where;
  for (std::size_t k = 0; k < objects.size(); ++k) {
    const auto& [path, obj] = objects[k];
    for (const elf::SymbolPtr& s : obj.symbols) {
      if (!s->is_global() || !s->defined()) continue;
      if (s->is_absolute()) {
        if (const std::int64_t* before = out.absolute.find(s->name);
            before != nullptr && *before != s->value) {
          throw link_error(path + ": " + pyrepr::string(s->name) + " is at $" +
                           pyrepr::hex(s->value, 4) + " here and at $" + pyrepr::hex(*before, 4) +
                           " in " + abs_where.at(s->name));
        }
        out.absolute.insert_or_assign(s->name, s->value);
        abs_where.insert_or_assign(s->name, path);
        continue;
      }
      if (const auto* d = out.defs.find(s->name)) {
        throw link_error("`" + s->name + "` is defined in multiple TUs: " +
                         objects[static_cast<std::size_t>(d->first)].first + " and " + path);
      }
      out.defs.insert_or_assign(s->name, std::pair<int, elf::SymbolPtr>(static_cast<int>(k), s));
    }
  }
  for (const auto& [name, value] : out.absolute) {
    if (const auto* d = out.defs.find(name)) {
      throw link_error("`" + name + "` is an address in " + abs_where.at(name) +
                       " and is defined in " + objects[static_cast<std::size_t>(d->first)].first);
    }
  }
  return out;
}

Resolver::Resolver(const Objects& objects, const std::vector<Placed>& placed, Names extra)
    : objects_(&objects), extra_(std::move(extra)) {
  // Python's `{id(p.section): p.addr for p in placed}`, where a later
  // part of the same section wins.
  for (const Placed& p : placed) addr_[p.section.get()] = p.addr;
  definitions_ = global_definitions(objects);
}

std::optional<std::int64_t> Resolver::section_addr(const elf::Section* sec) const {
  const auto it = addr_.find(sec);
  if (it == addr_.end()) return std::nullopt;
  return it->second;
}

std::int64_t Resolver::value(const elf::Symbol& sym, int unit) const {
  const auto path = [&]() -> const std::string& {
    return objects_->at(static_cast<std::size_t>(unit)).first;
  };
  if (sym.defined()) {
    if (sym.is_absolute()) return sym.value;
    const std::optional<std::int64_t> base = section_addr(sym.section.get());
    if (!base) throw link_error(path() + ": `" + sym.name + "` is in a section the link left out");
    return *base + sym.value;
  }
  if (const auto* d = definitions_.defs.find(sym.name)) return value(*d->second, d->first);
  if (const std::int64_t* v = definitions_.absolute.find(sym.name)) return *v;
  if (const std::int64_t* v = extra_.find(sym.name)) return *v;
  throw link_error(path() + ": undefined symbol `" + sym.name + "`" + undefined_hint(sym.name));
}

// Shortens each absolute instruction in code section `sec` whose
// operand the linker has put in zero page to its zero-page form, and
// returns the number of bytes saved.
//
// A unit reaches a name another unit defines, such as a global that the
// defining unit's `-O` moved into zero page, with absolute addressing,
// since it can't know where the name goes. When `value_of(symbol)`
// gives the name's address (it returns none for names whose address
// isn't known before layout) and the address is below $100, the
// instruction loses its operand's high byte and its relocation becomes
// `R_MOS_ADDR8`. The bytes after it move down: every branch within the
// section is aimed again, and the relocations, the labels and mapping
// symbols (`symbols`, the object's), and the item's size move with them.
// A branch only gets shorter, so it stays in reach. A jump table
// (between `$d` and `$c`) isn't decoded.
std::int64_t relax(const elf::SectionPtr& sec, const std::vector<elf::SymbolPtr>& symbols,
                   const ValueOf& value_of) {
  if (sec->type == elf::SHT_NOBITS || (sec->flags & elf::SHF_EXECINSTR) == 0) return 0;
  const std::string data = sec->data;
  std::vector<elf::SymbolPtr> own;
  for (const elf::SymbolPtr& s : symbols) {
    if (s->section == sec) own.push_back(s);
  }
  std::vector<std::pair<std::int64_t, std::string>> mapping;
  for (const elf::SymbolPtr& s : own) {
    if (s->name == "$c" || s->name == "$d") mapping.emplace_back(s->value, s->name);
  }
  std::sort(mapping.begin(), mapping.end());
  // Python's `{r.offset: r for r in sec.relocs}`, where a later
  // relocation at the same offset wins.
  std::unordered_map<std::int64_t, elf::Reloc*> relocs;
  for (elf::Reloc& r : sec->relocs) relocs[r.offset] = &r;
  const auto reloc_at = [&](std::int64_t off) -> elf::Reloc* {
    const auto it = relocs.find(off);
    return it == relocs.end() ? nullptr : it->second;
  };
  const auto in_data = [&](std::int64_t off) {
    std::string_view state = "$c";
    for (const auto& [at, name] : mapping) {
      if (at > off) break;
      state = name;
    }
    return state == "$d";
  };

  // Decode the instructions and choose the ones to shorten.
  struct Instr {
    std::int64_t off;
    const opcodes::Entry* entry;
  };
  std::vector<Instr> instrs;
  std::set<std::int64_t> shorten;
  const auto len = static_cast<std::int64_t>(data.size());
  std::int64_t off = 0;
  while (off < len) {
    const auto byte = static_cast<std::uint8_t>(data[static_cast<std::size_t>(off)]);
    const opcodes::Entry* entry = in_data(off) ? nullptr : opcodes::decode(byte);
    if (entry == nullptr) {
      ++off;
      continue;
    }
    instrs.push_back({off, entry});
    const elf::Reloc* r = reloc_at(off + 1);
    const std::optional<opcodes::Mode> zp_mode = opcodes::zp_form(entry->mode);
    if (r != nullptr && r->type == elf::R_MOS_ADDR16 && zp_mode &&
        opcodes::opcode(entry->mnemonic, *zp_mode)) {
      const std::optional<std::int64_t> v = value_of(r->symbol);
      if (v && 0 <= *v + r->addend && *v + r->addend <= 0xFF) shorten.insert(off);
    }
    off += opcodes::size(entry->mode);
  }
  if (shorten.empty()) return 0;
  std::vector<std::int64_t> deleted;
  for (std::int64_t at : shorten) deleted.push_back(at + 2);
  const auto moved = [&](std::int64_t old) {
    return old - (std::lower_bound(deleted.begin(), deleted.end(), old) - deleted.begin());
  };
  const auto byte_at = [&](std::int64_t i) {
    if (i < 0 || i >= len) {
      throw InternalError(
          "an instruction runs past the end of its section, where Python raises "
          "IndexError");
    }
    return static_cast<std::uint8_t>(data[static_cast<std::size_t>(i)]);
  };

  std::string out = data;
  // Aim the branches resolved at assembly again, from the old bytes.
  for (const Instr& in : instrs) {
    if (in.entry->mode == opcodes::Mode::kRel && reloc_at(in.off + 1) == nullptr) {
      const std::uint8_t b = byte_at(in.off + 1);
      const std::int64_t disp = b >= 128 ? b - 256 : b;
      const std::int64_t target = in.off + 2 + disp;
      const std::int64_t new_disp = moved(target) - moved(in.off + 2);
      out[static_cast<std::size_t>(in.off + 1)] = static_cast<char>(new_disp & 0xFF);
    }
  }
  for (std::int64_t at : shorten) {
    const opcodes::Entry* entry = opcodes::decode(byte_at(at));
    out[static_cast<std::size_t>(at)] =
        static_cast<char>(*opcodes::opcode(entry->mnemonic, *opcodes::zp_form(entry->mode)));
    reloc_at(at + 1)->type = elf::R_MOS_ADDR8;
  }
  for (auto it = deleted.rbegin(); it != deleted.rend(); ++it) {
    if (*it >= static_cast<std::int64_t>(out.size())) {
      throw InternalError(
          "an instruction runs past the end of its section, where Python raises "
          "IndexError");
    }
    out.erase(static_cast<std::size_t>(*it), 1);
  }
  sec->data = out;
  for (elf::Reloc& r : sec->relocs) r.offset = moved(r.offset);
  for (const elf::SymbolPtr& s : own) {
    if (s->type == elf::STT_FUNC || s->type == elf::STT_OBJECT) {
      s->size = static_cast<std::int64_t>(out.size());
    } else {
      s->value = moved(s->value);
    }
  }
  return static_cast<std::int64_t>(deleted.size());
}

std::string relocate(const Placed& p, const Resolver& resolver, const std::string& path) {
  const elf::Section& sec = *p.section;
  std::string data = sec.type != elf::SHT_NOBITS ? py_slice(sec.data, 0, p.size) : std::string();
  for (const elf::Reloc& r : sec.relocs) {
    if (r.offset >= p.size) {
      // In the dropped `JMP` at the end of the section.
      continue;
    }
    const asdl::Int s = resolver.value(*r.symbol, p.unit);
    const asdl::Int v = s + r.addend;
    const auto where = [&] { return path + ": " + sec.name + "+$" + pyrepr::hex(r.offset, 4); };
    switch (r.type) {
      case elf::R_MOS_ADDR16: {
        if (!(0 <= v && v <= 0xFFFF)) {
          throw link_error(where() + ": address $" + pyrepr::hex(v, 0) + " of `" + r.symbol->name +
                           "` is out of range");
        }
        const auto u = static_cast<unsigned>(v);
        py_slice_assign(data, r.offset, r.offset + 2,
                        std::string{static_cast<char>(u & 0xFF), static_cast<char>(u >> 8)});
        break;
      }
      case elf::R_MOS_ADDR8: {
        if (!(0 <= v && v <= 0xFF)) {
          const std::string& name = r.symbol->name;
          const bool slot = name.starts_with("__zpabi_") || name.starts_with("__local_");
          throw link_error(where() + ": `" + name + "` is at $" + pyrepr::hex(v, 4) +
                           ", and this instruction was assembled with zero-page addressing for "
                           "it" +
                           (slot ? ", since it was in zero page when its unit was compiled" : ""));
        }
        set_byte(data, r.offset, static_cast<std::uint8_t>(v));
        break;
      }
      case elf::R_MOS_IMM8:
        if (!(-128 <= v && v <= 0xFF)) {
          throw link_error(where() + ": `" + r.symbol->name + "` + " + pyrepr::integer(r.addend) +
                           " = " + pyrepr::integer(v) + " doesn't fit in a byte");
        }
        set_byte(data, r.offset, static_cast<std::uint8_t>(v & 0xFF));
        break;
      case elf::R_MOS_ADDR16_LO:
        set_byte(data, r.offset, static_cast<std::uint8_t>(v & 0xFF));
        break;
      case elf::R_MOS_ADDR16_HI:
        set_byte(data, r.offset, static_cast<std::uint8_t>((v >> 8) & 0xFF));
        break;
      case elf::R_MOS_PCREL_8: {
        const asdl::Int disp = v - (asdl::Int(p.addr) + r.offset);
        if (!(-128 <= disp && disp <= 127)) {
          throw link_error(where() + ": the branch to `" + r.symbol->name + "` is " +
                           pyrepr::integer(disp) + " bytes away, out of reach");
        }
        set_byte(data, r.offset, static_cast<std::uint8_t>(disp & 0xFF));
        break;
      }
      default:
        throw link_error(where() + ": unknown relocation type " + std::to_string(r.type));
    }
  }
  return data;
}

elf::ElfFile executable(const Objects& objects, std::vector<OutputSection>& outputs,
                        const Resolver& resolver, const std::vector<OutputSymbol>& symbols,
                        std::int64_t entry, const std::optional<std::string>& link_text) {
  elf::ElfFile exe;
  exe.type = elf::ET_EXEC;
  exe.entry = entry;
  for (OutputSection& out : outputs) {
    std::string data(static_cast<std::size_t>(out.size), '\0');
    for (const Placed& p : out.parts) {
      const std::int64_t off = p.addr - out.addr;
      const std::string bytes =
          p.section->type != elf::SHT_NOBITS
              ? relocate(p, resolver, objects.at(static_cast<std::size_t>(p.unit)).first)
              : std::string(static_cast<std::size_t>(std::max<std::int64_t>(p.size, 0)), '\0');
      py_slice_assign(data, off, off + p.size, bytes);
    }
    const std::uint32_t flags =
        elf::SHF_ALLOC | (out.executable ? elf::SHF_EXECINSTR : elf::SHF_WRITE);
    auto sec = std::make_shared<elf::Section>();
    sec->name = out.name;
    sec->flags = flags;
    sec->addr = out.addr;
    if (out.noload) {
      sec->type = elf::SHT_NOBITS;
      sec->nobits_size = out.size;
    } else {
      sec->data = data;
    }
    out.elf_section = sec;
    exe.sections.push_back(sec);
    if (out.size != 0) {
      exe.segments.push_back(elf::Segment{
          .vaddr = out.addr,
          .paddr = out.load_addr,
          .data = out.noload ? std::string() : data,
          .memsz = out.size,
          .flags = elf::PF_R | (out.executable ? elf::PF_X : elf::PF_W),
      });
    }
  }
  for (const OutputSymbol& s : symbols) {
    if (s.output >= 0) {
      s.symbol->section = outputs.at(static_cast<std::size_t>(s.output)).elf_section;
    }
    exe.symbols.push_back(s.symbol);
  }
  if (link_text) {
    auto sec = std::make_shared<elf::Section>();
    sec->name = std::string(elf::LINK_SECTION);
    sec->flags = 0;
    sec->data = *link_text;
    exe.sections.push_back(sec);
  }
  return exe;
}

}  // namespace c6502::binutils::ld
