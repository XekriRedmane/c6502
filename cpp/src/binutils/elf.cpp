#include "binutils/elf.h"

#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>

#include "errors.h"

namespace c6502::binutils::elf {

namespace {

constexpr std::size_t kEhdrSize = 52;
constexpr std::size_t kShdrSize = 40;
constexpr std::size_t kPhdrSize = 32;
constexpr std::size_t kSymSize = 16;
constexpr std::size_t kRelaSize = 12;

[[noreturn]] void elf_error(const std::string& message) {
  throw CompileError("ElfError", message);
}

struct Damaged {};

void put16(std::string& out, std::size_t at, std::uint64_t v) {
  out[at] = static_cast<char>(v & 0xFF);
  out[at + 1] = static_cast<char>((v >> 8) & 0xFF);
}

void put32(std::string& out, std::size_t at, std::uint64_t v) {
  for (int k = 0; k < 4; ++k) out[at + k] = static_cast<char>((v >> (8 * k)) & 0xFF);
}

void append16(std::string& out, std::uint64_t v) {
  out.push_back(static_cast<char>(v & 0xFF));
  out.push_back(static_cast<char>((v >> 8) & 0xFF));
}

void append32(std::string& out, std::int64_t v) {
  const auto u = static_cast<std::uint64_t>(v);
  for (int k = 0; k < 4; ++k) out.push_back(static_cast<char>((u >> (8 * k)) & 0xFF));
}

std::uint32_t get16(std::string_view d, std::size_t at) {
  return static_cast<std::uint8_t>(d[at]) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(d[at + 1])) << 8);
}

std::uint32_t get32(std::string_view d, std::size_t at) {
  std::uint32_t v = 0;
  for (int k = 0; k < 4; ++k) {
    v |= static_cast<std::uint32_t>(static_cast<std::uint8_t>(d[at + k])) << (8 * k);
  }
  return v;
}

// A section header's ten fields, in the order of `Elf32_Shdr`.
struct Shdr {
  std::uint32_t name, type, flags, addr, offset, size, link, info, addralign, entsize;
};

void append_shdr(std::string& out, const Shdr& h) {
  for (std::uint32_t v : {h.name, h.type, h.flags, h.addr, h.offset, h.size, h.link, h.info,
                          h.addralign, h.entsize}) {
    append32(out, v);
  }
}

class StrTab {
 public:
  StrTab() : data_(1, '\0') { index_.emplace("", 0); }

  std::uint32_t add(const std::string& s) {
    const auto [it, added] = index_.emplace(s, static_cast<std::uint32_t>(data_.size()));
    if (added) {
      data_ += s;
      data_.push_back('\0');
    }
    return it->second;
  }

  const std::string& data() const { return data_; }

 private:
  std::string data_;
  std::unordered_map<std::string, std::uint32_t> index_;
};

bool is_utf8(std::string_view s) {
  std::size_t i = 0;
  while (i < s.size()) {
    const auto c = static_cast<unsigned char>(s[i]);
    if (c < 0x80) {
      ++i;
      continue;
    }
    std::size_t n;
    std::uint32_t cp;
    std::uint32_t min;
    if (c >= 0xC2 && c <= 0xDF) {
      n = 1, cp = c & 0x1F, min = 0x80;
    } else if (c >= 0xE0 && c <= 0xEF) {
      n = 2, cp = c & 0x0F, min = 0x800;
    } else if (c >= 0xF0 && c <= 0xF4) {
      n = 3, cp = c & 0x07, min = 0x10000;
    } else {
      return false;
    }
    for (std::size_t k = 1; k <= n; ++k) {
      if (i + k >= s.size()) return false;
      const auto cc = static_cast<unsigned char>(s[i + k]);
      if ((cc & 0xC0) != 0x80) return false;
      cp = (cp << 6) | (cc & 0x3F);
    }
    if (cp < min || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) return false;
    i += n + 1;
  }
  return true;
}

std::string_view part(std::string_view data, std::uint64_t offset, std::uint64_t size) {
  if (offset + size > data.size()) throw Damaged{};
  return data.substr(offset, size);
}

std::string cstr(std::string_view table, std::uint64_t off) {
  if (off > table.size()) throw Damaged{};
  const std::size_t end = table.find('\0', off);
  if (end == std::string_view::npos) throw Damaged{};
  const std::string_view s = table.substr(off, end - off);
  if (!is_utf8(s)) throw Damaged{};
  return std::string(s);
}

ElfFile read_file(std::string_view data, std::string_view name) {
  const std::string who(name);
  if (data.size() < kEhdrSize || data.substr(0, 4) != std::string_view("\x7f" "ELF", 4)) {
    elf_error(who + ": not an ELF file");
  }
  const auto e_type = static_cast<std::uint16_t>(get16(data, 16));
  const std::uint32_t machine = get16(data, 18);
  const std::uint32_t entry = get32(data, 24);
  const std::uint32_t phoff = get32(data, 28);
  const std::uint32_t shoff = get32(data, 32);
  const std::uint32_t flags = get32(data, 36);
  const std::uint32_t phentsize = get16(data, 42);
  const std::uint32_t phnum = get16(data, 44);
  const std::uint32_t shentsize = get16(data, 46);
  const std::uint32_t shnum = get16(data, 48);
  const std::uint32_t shstrndx = get16(data, 50);
  if (static_cast<std::uint8_t>(data[4]) != ELFCLASS32 ||
      static_cast<std::uint8_t>(data[5]) != ELFDATA2LSB) {
    elf_error(who + ": not a little-endian 32-bit ELF file");
  }
  if (machine != EM_MOS) {
    elf_error(who + ": ELF file for machine " + std::to_string(machine) + ", not the 6502 (" +
              std::to_string(EM_MOS) + ")");
  }
  std::vector<Shdr> shdrs;
  for (std::uint32_t i = 0; i < shnum; ++i) {
    const std::uint64_t at = static_cast<std::uint64_t>(shoff) + static_cast<std::uint64_t>(i) * shentsize;
    if (at + kShdrSize > data.size()) throw Damaged{};
    Shdr h;
    std::uint32_t* fields[] = {&h.name, &h.type,  &h.flags,     &h.addr,   &h.offset,
                               &h.size, &h.link,  &h.info,      &h.addralign, &h.entsize};
    for (int k = 0; k < 10; ++k) *fields[k] = get32(data, at + 4 * k);
    shdrs.push_back(h);
  }
  const auto blob = [&](const Shdr& h) { return part(data, h.offset, h.size); };
  std::string_view shstr("\0", 1);
  if (shnum != 0) {
    if (shstrndx >= shdrs.size()) throw Damaged{};
    shstr = blob(shdrs[shstrndx]);
  }
  ElfFile elf;
  elf.type = e_type;
  elf.entry = entry;
  elf.flags = flags;
  std::map<std::size_t, SectionPtr> by_index;
  for (std::size_t i = 0; i < shdrs.size(); ++i) {
    const Shdr& h = shdrs[i];
    if (h.type != SHT_PROGBITS && h.type != SHT_NOBITS) continue;
    auto s = std::make_shared<Section>();
    s->name = cstr(shstr, h.name);
    s->type = h.type;
    s->flags = h.flags;
    s->addr = h.addr;
    s->align = h.addralign;
    if (h.type == SHT_NOBITS) {
      s->nobits_size = h.size;
    } else {
      s->data = std::string(blob(h));
      if ((s->name == LINK_SECTION || s->name == SOURCE_SECTION) && !is_utf8(s->data)) {
        throw Damaged{};
      }
    }
    by_index[i] = s;
    elf.sections.push_back(s);
  }
  std::vector<SymbolPtr> symbols{nullptr};
  const Shdr* symtab = nullptr;
  for (const Shdr& h : shdrs) {
    if (h.type == SHT_SYMTAB) {
      symtab = &h;
      break;
    }
  }
  if (symtab != nullptr) {
    if (symtab->link >= shdrs.size()) throw Damaged{};
    const std::string_view strtab = blob(shdrs[symtab->link]);
    const std::string_view raw = blob(*symtab);
    for (std::size_t off = kSymSize; off < raw.size(); off += kSymSize) {
      if (off + kSymSize > raw.size()) throw Damaged{};
      const std::uint32_t st_name = get32(raw, off);
      const std::uint32_t value = get32(raw, off + 4);
      const std::uint32_t size = get32(raw, off + 8);
      const auto info = static_cast<std::uint8_t>(raw[off + 12]);
      const auto other = static_cast<std::uint8_t>(raw[off + 13]);
      const auto shndx = static_cast<std::uint16_t>(get16(raw, off + 14));
      auto sym = std::make_shared<Symbol>();
      if (shndx == SHN_UNDEF || shndx == SHN_ABS) {
        sym->shndx = shndx;
      } else if (auto it = by_index.find(shndx); it != by_index.end()) {
        sym->section = it->second;
      } else {
        symbols.push_back(nullptr);
        continue;
      }
      sym->name = cstr(strtab, st_name);
      sym->value = value;
      sym->size = size;
      sym->bind = static_cast<std::uint8_t>(info >> 4);
      sym->type = static_cast<std::uint8_t>(info & 0xF);
      sym->other = other;
      symbols.push_back(sym);
      elf.symbols.push_back(sym);
    }
  }
  for (const Shdr& h : shdrs) {
    if (h.type != SHT_RELA) continue;
    const auto it = by_index.find(h.info);
    if (it == by_index.end()) elf_error(who + ": a relocation section names no section");
    Section& target = *it->second;
    const std::string_view raw = blob(h);
    for (std::size_t off = 0; off < raw.size(); off += kRelaSize) {
      if (off + kRelaSize > raw.size()) throw Damaged{};
      const std::uint32_t r_offset = get32(raw, off);
      const std::uint32_t info = get32(raw, off + 4);
      const auto addend = static_cast<std::int32_t>(get32(raw, off + 8));
      const std::size_t index = info >> 8;
      if (index >= symbols.size()) throw Damaged{};
      if (symbols[index] == nullptr) {
        elf_error(who + ": a relocation in " + target.name +
                  " names a symbol c6502 doesn't read");
      }
      const std::optional<int> field = reloc_size(static_cast<std::uint8_t>(info & 0xFF));
      if (static_cast<std::uint64_t>(r_offset) + static_cast<std::uint64_t>(field.value_or(1)) >
          target.data.size()) {
        elf_error(who + ": a relocation in " + target.name + " is past the end of its bytes");
      }
      target.relocs.push_back(Reloc{.offset = r_offset,
                                    .type = static_cast<std::uint8_t>(info & 0xFF),
                                    .symbol = symbols[index],
                                    .addend = addend});
    }
  }
  for (std::uint32_t i = 0; i < phnum; ++i) {
    const std::uint64_t at = static_cast<std::uint64_t>(phoff) + static_cast<std::uint64_t>(i) * phentsize;
    if (at + kPhdrSize > data.size()) throw Damaged{};
    const std::uint32_t p_type = get32(data, at);
    const std::uint32_t p_offset = get32(data, at + 4);
    const std::uint32_t vaddr = get32(data, at + 8);
    const std::uint32_t paddr = get32(data, at + 12);
    const std::uint32_t filesz = get32(data, at + 16);
    const std::uint32_t memsz = get32(data, at + 20);
    const std::uint32_t pflags = get32(data, at + 24);
    if (p_type == PT_LOAD) {
      elf.segments.push_back(Segment{.vaddr = vaddr,
                                     .paddr = paddr,
                                     .data = std::string(part(data, p_offset, filesz)),
                                     .memsz = memsz,
                                     .flags = pflags});
    }
  }
  return elf;
}

}  // namespace

const char* reloc_name(std::uint8_t type) {
  switch (type) {
    case R_MOS_NONE:
      return "R_MOS_NONE";
    case R_MOS_IMM8:
      return "R_MOS_IMM8";
    case R_MOS_ADDR8:
      return "R_MOS_ADDR8";
    case R_MOS_ADDR16:
      return "R_MOS_ADDR16";
    case R_MOS_ADDR16_LO:
      return "R_MOS_ADDR16_LO";
    case R_MOS_ADDR16_HI:
      return "R_MOS_ADDR16_HI";
    case R_MOS_PCREL_8:
      return "R_MOS_PCREL_8";
    default:
      return nullptr;
  }
}

std::optional<int> reloc_size(std::uint8_t type) {
  switch (type) {
    case R_MOS_IMM8:
    case R_MOS_ADDR8:
    case R_MOS_ADDR16_LO:
    case R_MOS_ADDR16_HI:
    case R_MOS_PCREL_8:
      return 1;
    case R_MOS_ADDR16:
      return 2;
    default:
      return std::nullopt;
  }
}

SectionPtr ElfFile::section(std::string_view name) const {
  for (const auto& s : sections) {
    if (s->name == name) return s;
  }
  return nullptr;
}

std::optional<std::string> ElfFile::link_text() const {
  const SectionPtr s = section(LINK_SECTION);
  if (s == nullptr) return std::nullopt;
  return s->data;
}

std::optional<std::string> ElfFile::source_text() const {
  const SectionPtr s = section(SOURCE_SECTION);
  if (s == nullptr) return std::nullopt;
  return s->data;
}

SymbolPtr ElfFile::symbol(std::string_view name) const {
  SymbolPtr first;
  for (const auto& s : symbols) {
    if (s->name != name) continue;
    if (s->is_global()) return s;
    if (first == nullptr) first = s;
  }
  return first;
}

std::string write(const ElfFile& elf) {
  StrTab shstr;
  StrTab strtab;
  std::vector<const Symbol*> symbols;
  for (const auto& s : elf.symbols) {
    if (!s->is_global()) symbols.push_back(s.get());
  }
  for (const auto& s : elf.symbols) {
    if (s->is_global()) symbols.push_back(s.get());
  }
  std::unordered_map<const Symbol*, std::uint32_t> sym_index;
  for (std::size_t i = 0; i < symbols.size(); ++i) {
    sym_index[symbols[i]] = static_cast<std::uint32_t>(i + 1);
  }
  std::unordered_map<const Section*, std::uint32_t> sec_index;
  for (std::size_t i = 0; i < elf.sections.size(); ++i) {
    sec_index[elf.sections[i].get()] = static_cast<std::uint32_t>(i + 1);
  }
  std::vector<const Section*> rela_secs;
  for (const auto& s : elf.sections) {
    if (!s->relocs.empty()) rela_secs.push_back(s.get());
  }
  const auto n_content = static_cast<std::uint32_t>(elf.sections.size());
  const std::uint32_t symtab_ix = 1 + n_content + static_cast<std::uint32_t>(rela_secs.size());
  const std::uint32_t strtab_ix = symtab_ix + 1;
  const std::uint32_t shstrtab_ix = strtab_ix + 1;
  const std::uint32_t shnum = shstrtab_ix + 1;

  for (const auto& s : elf.sections) {
    for (const Reloc& r : s->relocs) {
      if (!sym_index.contains(r.symbol.get())) {
        elf_error("a relocation names `" + r.symbol->name +
                  "`, which isn't in the symbol table");
      }
    }
  }

  const auto phnum = static_cast<std::uint32_t>(elf.segments.size());
  std::string out(kEhdrSize + phnum * kPhdrSize, '\0');
  std::vector<Shdr> headers{Shdr{}};
  const auto place = [&](std::string_view blob) {
    const auto off = static_cast<std::uint32_t>(out.size());
    out.append(blob);
    return off;
  };

  struct Phdr {
    std::uint32_t offset, vaddr, paddr, filesz, memsz, flags;
  };
  std::vector<Phdr> phdrs;
  std::multimap<std::int64_t, std::pair<const Segment*, std::uint32_t>> segments_at;
  for (const Segment& seg : elf.segments) {
    const std::uint32_t off = place(seg.data);
    segments_at.emplace(seg.vaddr, std::make_pair(&seg, off));
    phdrs.push_back(Phdr{off, static_cast<std::uint32_t>(seg.vaddr),
                         static_cast<std::uint32_t>(seg.paddr),
                         static_cast<std::uint32_t>(seg.data.size()),
                         static_cast<std::uint32_t>(seg.memsz), seg.flags});
  }
  const auto segment_offset = [&](const Section& s) -> std::optional<std::uint32_t> {
    const auto [first, last] = segments_at.equal_range(s.addr);
    for (auto it = first; it != last; ++it) {
      if (it->second.first->data == s.data) return it->second.second;
    }
    return std::nullopt;
  };
  for (const auto& s : elf.sections) {
    std::uint32_t off;
    if (s->type == SHT_NOBITS) {
      off = static_cast<std::uint32_t>(out.size());
    } else if (const auto shared = segment_offset(*s)) {
      off = *shared;
    } else {
      off = place(s->data);
    }
    headers.push_back(Shdr{shstr.add(s->name), s->type, s->flags,
                           static_cast<std::uint32_t>(s->addr), off,
                           static_cast<std::uint32_t>(s->size()), 0, 0,
                           static_cast<std::uint32_t>(s->align), 0});
  }
  for (const Section* s : rela_secs) {
    std::string blob;
    for (const Reloc& r : s->relocs) {
      append32(blob, r.offset);
      append32(blob, (static_cast<std::int64_t>(sym_index.at(r.symbol.get())) << 8) | r.type);
      append32(blob, r.addend);
    }
    const std::uint32_t name = shstr.add(".rela" + s->name);
    headers.push_back(Shdr{name, SHT_RELA, SHF_INFO_LINK, 0, place(blob),
                           static_cast<std::uint32_t>(blob.size()), symtab_ix,
                           sec_index.at(s), 4, static_cast<std::uint32_t>(kRelaSize)});
  }
  std::string syms(kSymSize, '\0');
  std::uint32_t locals = 0;
  for (const Symbol* s : symbols) {
    std::uint32_t shndx = s->shndx;
    if (s->section != nullptr) shndx = sec_index.at(s->section.get());
    append32(syms, strtab.add(s->name));
    append32(syms, s->value & 0xFFFFFFFF);
    append32(syms, s->size);
    syms.push_back(static_cast<char>((s->bind << 4) | s->type));
    syms.push_back(static_cast<char>(s->other));
    append16(syms, shndx);
    if (!s->is_global()) ++locals;
  }
  const std::uint32_t first_global = 1 + locals;
  {
    const std::uint32_t name = shstr.add(".symtab");
    headers.push_back(Shdr{name, SHT_SYMTAB, 0, 0, place(syms),
                           static_cast<std::uint32_t>(syms.size()), strtab_ix, first_global, 4,
                           static_cast<std::uint32_t>(kSymSize)});
  }
  {
    const std::uint32_t name = shstr.add(".strtab");
    headers.push_back(Shdr{name, SHT_STRTAB, 0, 0, place(strtab.data()),
                           static_cast<std::uint32_t>(strtab.data().size()), 0, 0, 1, 0});
  }
  {
    const std::uint32_t name = shstr.add(".shstrtab");
    const std::string data = shstr.data();
    headers.push_back(Shdr{name, SHT_STRTAB, 0, 0, place(data),
                           static_cast<std::uint32_t>(data.size()), 0, 0, 1, 0});
  }
  while (out.size() % 4 != 0) out.push_back('\0');
  const auto shoff = static_cast<std::uint32_t>(out.size());
  for (const Shdr& h : headers) append_shdr(out, h);
  if (headers.size() != shnum) throw InternalError("elf.write: wrong number of section headers");
  out[0] = 0x7f;
  out[1] = 'E';
  out[2] = 'L';
  out[3] = 'F';
  out[4] = static_cast<char>(ELFCLASS32);
  out[5] = static_cast<char>(ELFDATA2LSB);
  out[6] = static_cast<char>(EV_CURRENT);
  put16(out, 16, elf.type);
  put16(out, 18, EM_MOS);
  put32(out, 20, EV_CURRENT);
  put32(out, 24, static_cast<std::uint64_t>(elf.entry));
  put32(out, 28, phnum != 0 ? kEhdrSize : 0);
  put32(out, 32, shoff);
  put32(out, 36, elf.flags);
  put16(out, 40, kEhdrSize);
  put16(out, 42, kPhdrSize);
  put16(out, 44, phnum);
  put16(out, 46, kShdrSize);
  put16(out, 48, shnum);
  put16(out, 50, shstrtab_ix);
  for (std::size_t i = 0; i < phdrs.size(); ++i) {
    const std::size_t at = kEhdrSize + i * kPhdrSize;
    const Phdr& p = phdrs[i];
    put32(out, at, PT_LOAD);
    put32(out, at + 4, p.offset);
    put32(out, at + 8, p.vaddr);
    put32(out, at + 12, p.paddr);
    put32(out, at + 16, p.filesz);
    put32(out, at + 20, p.memsz);
    put32(out, at + 24, p.flags);
    put32(out, at + 28, 1);
  }
  return out;
}

ElfFile read(std::string_view data, std::string_view name) {
  try {
    return read_file(data, name);
  } catch (const Damaged&) {
    throw CompileError("ElfError", std::string(name) + ": the ELF file is truncated or damaged");
  }
}

}  // namespace c6502::binutils::elf
