// binutils/disasm.py. The header gives the listing's form. Python keys
// the labels, the mapping symbols, the relocations, and the symbols of
// an executable by address in dicts that are only looked up, so these
// are `std::map`s. The lists in them keep the order of the symbol
// table, which the sorts below rely on, since Python's sort is stable
// and its strings compare by code point, the order of their UTF-8 bytes.
#include "binutils/disasm.h"

#include <algorithm>
#include <map>
#include <optional>
#include <tuple>

#include "binutils/opcodes.h"
#include "pcpp_text.h"

namespace c6502::binutils::disasm {

namespace {

using opcodes::Mode;

constexpr std::size_t kOpcodeCol = 3;
constexpr std::size_t kOperandCol = 9;
constexpr std::size_t kCommentCol = 40;

bool starts_with_dot(const std::string& name) { return !name.empty() && name[0] == '.'; }

bool is_mapping(const std::string& name) { return name == "$d" || name == "$c"; }

// Returns one line of the listing: the opcode (or directive) in its
// column, the operand in its column, and the comment after ` ; `.
std::string line(std::string_view op, std::string_view operand = "", std::string_view comment = "") {
  std::string text = std::string(kOpcodeCol, ' ') + std::string(op);
  if (!operand.empty()) text = ljust(text, kOperandCol) + std::string(operand);
  if (!comment.empty()) text = ljust(text, kCommentCol - 1) + " ; " + std::string(comment);
  return text;
}

// Returns `$` and the value in `2 * width` hex digits (`_hex`).
std::string hex(std::int64_t v, int width) { return "$" + format_hex(v, 2 * width); }

// Returns `name`, `name+k`, or `name-k` for a symbol and an addend.
std::string sym_expr(const std::string& name, std::int64_t addend) {
  if (addend == 0) return name;
  return addend > 0 ? name + "+" + std::to_string(addend) : name + "-" + std::to_string(-addend);
}

// One section's labels, relocations, and mapping symbols (`_Section`).
struct SectionInfo {
  const elf::Section* sec = nullptr;
  bool exe = false;
  std::int64_t base = 0;
  std::map<std::int64_t, std::vector<const elf::Symbol*>> labels;
  std::map<std::int64_t, std::string> mapping;
  std::map<std::int64_t, const elf::Reloc*> relocs;

  SectionInfo(const elf::ElfFile& elf, const elf::Section& s) : sec(&s) {
    exe = elf.type == elf::ET_EXEC;
    base = exe ? s.addr : 0;
    for (const auto& sym : elf.symbols) {
      if (sym->section.get() != &s || sym->type == elf::STT_FILE ||
          sym->type == elf::STT_SECTION) {
        continue;
      }
      const std::int64_t off = sym->value - base;
      if (is_mapping(sym->name)) {
        mapping[off] = sym->name;
        continue;
      }
      labels[off].push_back(sym.get());
    }
    // A function starts code and an object starts data, since a linker
    // script can put both in one output section.
    for (const auto& [off, syms] : labels) {
      for (const auto* sym : syms) {
        if (sym->type == elf::STT_FUNC) {
          mapping.emplace(off, "$c");
        } else if (sym->type == elf::STT_OBJECT) {
          mapping.emplace(off, "$d");
        }
      }
    }
    for (auto& [off, syms] : labels) {
      // The item's own symbol first, then local labels.
      std::stable_sort(syms.begin(), syms.end(), [](const elf::Symbol* a, const elf::Symbol* b) {
        const bool na = a->type == elf::STT_NOTYPE;
        const bool nb = b->type == elf::STT_NOTYPE;
        return std::tie(na, a->name) < std::tie(nb, b->name);
      });
    }
    // A later relocation at the same offset replaces an earlier one, as
    // the dict comprehension does.
    for (const auto& r : s.relocs) relocs[r.offset] = &r;
  }

  bool has_label(std::int64_t off) const { return labels.contains(off); }
  bool has_mapping(std::int64_t off) const { return mapping.contains(off); }
};

using Synthetic = std::map<std::int64_t, std::string>;

// An instruction's mnemonic, mode, and size.
struct Decoded {
  std::string_view mnemonic;
  Mode mode;
  int n;
};

class Disassembler {
 public:
  Disassembler(const elf::ElfFile& elf, const Options& options)
      : elf_(elf), opt_(options), exe_(elf.type == elf::ET_EXEC) {
    if (!exe_) return;
    for (const auto& s : elf.symbols) {
      if (s->defined() && s->type == elf::STT_OBJECT && s->size > 1) objects_.push_back(s.get());
    }
    // Addresses of symbols in an executable, for naming operands.
    for (const auto& s : elf.symbols) {
      if (!s->defined() || s->type == elf::STT_FILE || s->type == elf::STT_SECTION) continue;
      if (is_mapping(s->name)) continue;
      if (s->name.starts_with("__") && s->name.ends_with("_size")) {
        // A section's size (passes/link_layout.py), which isn't an
        // address.
        continue;
      }
      by_addr_[s->value].push_back(s.get());
    }
  }

  bool exe() const { return exe_; }

  // Returns the operand text of relocation `r`.
  static std::string reloc_text(const elf::Reloc& r) {
    const std::string& name = r.symbol->name;
    if (r.type == elf::R_MOS_ADDR16_LO || r.type == elf::R_MOS_ADDR16_HI) {
      const std::string inner = sym_expr(name, r.addend);
      const char* sign = r.type == elf::R_MOS_ADDR16_LO ? "<" : ">";
      return r.addend == 0 ? sign + inner : std::string(sign) + "(" + inner + ")";
    }
    if (r.type == elf::R_MOS_PCREL_8) return sym_expr(name, r.addend + 1);
    return sym_expr(name, r.addend);
  }

  // Returns the symbol to write for address `addr` in an executable, or
  // none.
  std::optional<std::string> addr_name(std::int64_t addr, const elf::Symbol* function) const {
    const auto it = by_addr_.find(addr);
    if (it == by_addr_.end() || it->second.empty()) {
      // A byte inside an object: `name+k`.
      std::vector<const elf::Symbol*> inside;
      for (const auto* s : objects_) {
        if (s->value < addr && addr < s->value + s->size) inside.push_back(s);
      }
      if (inside.empty()) return std::nullopt;
      std::stable_sort(inside.begin(), inside.end(), [](const elf::Symbol* a, const elf::Symbol* b) {
        const bool la = !a->is_global();
        const bool lb = !b->is_global();
        return std::tie(la, a->name) < std::tie(lb, b->name);
      });
      const elf::Symbol* s = inside.front();
      return s->name + "+" + std::to_string(addr - s->value);
    }
    const std::vector<const elf::Symbol*>& cands = it->second;
    if (function != nullptr) {
      for (const auto* s : cands) {
        if (starts_with_dot(s->name) && s->section == function->section &&
            function->value <= s->value &&
            s->value < function->value + std::max<std::int64_t>(function->size, 1)) {
          return s->name;
        }
      }
    }
    std::vector<const elf::Symbol*> named;
    for (const auto* s : cands) {
      if (!starts_with_dot(s->name)) named.push_back(s);
    }
    if (named.empty()) return std::nullopt;
    std::stable_sort(named.begin(), named.end(), [](const elf::Symbol* a, const elf::Symbol* b) {
      const auto key = [](const elf::Symbol* s) {
        return std::make_tuple(!s->is_global(), s->type != elf::STT_FUNC,
                               s->type != elf::STT_OBJECT);
      };
      return std::tuple_cat(key(a), std::tie(a->name)) < std::tuple_cat(key(b), std::tie(b->name));
    });
    return named.front()->name;
  }

  // Returns the trailing comment of a line at `addr` with bytes `data`.
  std::string comment(std::int64_t addr, std::string_view data) const {
    std::vector<std::string> parts;
    if (opt_.addresses) parts.push_back(format_hex(addr, 4));
    if (opt_.raw) {
      std::string bytes;
      for (std::size_t k = 0; k < data.size(); ++k) {
        if (k != 0) bytes += ' ';
        bytes += format_hex(static_cast<unsigned char>(data[k]), 2);
      }
      parts.push_back(bytes);
    }
    std::string out;
    for (std::size_t k = 0; k < parts.size(); ++k) {
      if (k != 0) out += "  ";
      out += parts[k];
    }
    return out;
  }

  std::vector<std::string> section_lines(const elf::Section& sec) const {
    const SectionInfo info(elf_, sec);
    std::vector<std::string> out;
    const std::int64_t size = sec.size();
    const bool code = (sec.flags & elf::SHF_EXECINSTR) != 0;
    // Branch targets with no label get one.
    Synthetic synthetic;
    if (code && sec.type != elf::SHT_NOBITS) scan_targets(info, synthetic);
    const elf::Symbol* function = nullptr;
    std::int64_t off = 0;
    bool in_data = !code;
    while (off < size || (off == size && info.has_label(off))) {
      if (const auto it = info.labels.find(off); it != info.labels.end()) {
        for (const auto* s : it->second) {
          if (s->type == elf::STT_FUNC) function = s;
          out.push_back(s->name + ":");
          if (s->type == elf::STT_FUNC) out.push_back(line("SUBROUTINE"));
        }
      }
      if (const auto it = synthetic.find(off); it != synthetic.end() && !info.has_label(off)) {
        out.push_back(it->second + ":");
      }
      if (off >= size) break;
      if (code) {
        if (const auto it = info.mapping.find(off); it != info.mapping.end()) {
          in_data = it->second == "$d";
        }
      }
      if (sec.type == elf::SHT_NOBITS) {
        const std::int64_t end = next_stop(info, off, size, synthetic);
        out.push_back(line("DS.B", std::to_string(end - off),
                           opt_.addresses ? comment(info.base + off, "") : ""));
        off = end;
        continue;
      }
      if (in_data) {
        off = data_line(info, off, size, out, synthetic);
      } else {
        off = instr_line(info, off, size, out, function, synthetic);
      }
    }
    return out;
  }

 private:
  static std::int64_t next_stop(const SectionInfo& info, std::int64_t off, std::int64_t size,
                                const Synthetic& synthetic) {
    std::int64_t end = off + 1;
    while (end < size && !info.has_label(end) && !info.has_mapping(end) &&
           !synthetic.contains(end)) {
      ++end;
    }
    return end;
  }

  static unsigned byte_at(const std::string& data, std::int64_t k) {
    return static_cast<unsigned char>(data[static_cast<std::size_t>(k)]);
  }

  // Returns `data[off:off + n]`, cut short at the end as a slice is.
  static std::string_view slice(const std::string& data, std::int64_t off, std::int64_t n) {
    const auto size = static_cast<std::int64_t>(data.size());
    if (off >= size) return {};
    return std::string_view(data).substr(static_cast<std::size_t>(off),
                                         static_cast<std::size_t>(std::min(n, size - off)));
  }

  std::int64_t data_line(const SectionInfo& info, std::int64_t off, std::int64_t size,
                         std::vector<std::string>& out, const Synthetic& synthetic) const {
    const std::string& data = info.sec->data;
    if (const auto it = info.relocs.find(off); it != info.relocs.end()) {
      const elf::Reloc& r = *it->second;
      const int width = elf::reloc_size(r.type).value_or(1);
      const char* op = width == 2 ? "DC.W" : "DC.B";
      out.push_back(line(op, reloc_text(r), comment(info.base + off, slice(data, off, width))));
      return off + width;
    }
    std::int64_t end = off;
    std::string values;
    int count = 0;
    while (end < size && count < 8) {
      if (end != off &&
          (info.has_label(end) || info.has_mapping(end) || synthetic.contains(end))) {
        break;
      }
      if (info.relocs.contains(end)) break;
      if (count != 0) values += ", ";
      values += hex(byte_at(data, end), 1);
      ++count;
      ++end;
    }
    out.push_back(line("DC.B", values, comment(info.base + off, slice(data, off, end - off))));
    return end;
  }

  static std::optional<Decoded> decode(const SectionInfo& info, std::int64_t off,
                                       std::int64_t size) {
    const opcodes::Entry* e = opcodes::decode(static_cast<std::uint8_t>(byte_at(info.sec->data, off)));
    if (e == nullptr) return std::nullopt;
    const int n = opcodes::size(e->mode);
    if (off + n > size) return std::nullopt;
    // An instruction can't run over a label or a mapping symbol.
    for (std::int64_t k = off + 1; k < off + n; ++k) {
      if (info.has_label(k) || info.has_mapping(k)) return std::nullopt;
    }
    return Decoded{e->mnemonic, e->mode, n};
  }

  static void scan_targets(const SectionInfo& info, Synthetic& synthetic) {
    std::int64_t off = 0;
    bool in_data = false;
    const std::string& data = info.sec->data;
    const auto len = static_cast<std::int64_t>(data.size());
    while (off < len) {
      if (const auto it = info.mapping.find(off); it != info.mapping.end()) {
        in_data = it->second == "$d";
      }
      const std::optional<Decoded> d = in_data ? std::nullopt : decode(info, off, len);
      if (!d) {
        ++off;
        continue;
      }
      if (d->mode == Mode::kRel && !info.relocs.contains(off + 1)) {
        const unsigned disp = byte_at(data, off + 1);
        const std::int64_t target =
            off + 2 + (disp >= 128 ? static_cast<std::int64_t>(disp) - 256 : disp);
        if (0 <= target && target <= len && !info.has_label(target)) {
          synthetic.emplace(target, ".L" + format_hex(info.base + target, 4));
        }
      }
      off += d->n;
    }
  }

  std::int64_t instr_line(const SectionInfo& info, std::int64_t off, std::int64_t size,
                          std::vector<std::string>& out, const elf::Symbol* function,
                          const Synthetic& synthetic) const {
    const std::string& data = info.sec->data;
    const std::optional<Decoded> d = decode(info, off, size);
    if (!d) {
      out.push_back(line("DC.B", hex(byte_at(data, off), 1),
                         comment(info.base + off, slice(data, off, 1))));
      return off + 1;
    }
    std::string mnemonic(d->mnemonic);
    const Mode mode = d->mode;
    const int n = d->n;
    const std::string_view raw = slice(data, off, n);
    std::string operand;
    if (mode != Mode::kImp && mode != Mode::kAcc) {
      const int width = n - 1;
      std::int64_t value = 0;
      for (int k = width; k >= 1; --k) {
        value = value * 256 + static_cast<unsigned char>(raw[static_cast<std::size_t>(k)]);
      }
      std::string text;
      if (const auto it = info.relocs.find(off + 1); it != info.relocs.end()) {
        text = reloc_text(*it->second);
      } else if (mode == Mode::kRel) {
        const std::int64_t disp = value >= 128 ? value - 256 : value;
        const std::int64_t target = off + 2 + disp;
        const auto labels = info.labels.find(target);
        if (labels != info.labels.end() && !labels->second.empty()) {
          const elf::Symbol* pick = labels->second.front();
          for (const auto* s : labels->second) {
            if (starts_with_dot(s->name)) {
              pick = s;
              break;
            }
          }
          text = pick->name;
        } else if (const auto named = synthetic.find(target); named != synthetic.end()) {
          text = named->second;
        } else {
          text = hex(info.base + target, 2);
        }
      } else if (mode == Mode::kImm) {
        text = hex(value, 1);
      } else {
        std::optional<std::string> name;
        if (exe_) name = addr_name(value, function);
        if (name) {
          text = *name;
        } else {
          text = hex(value, width);
          if (width == 2 && value < 0x100 && mode != Mode::kInd) mnemonic += ".w";
        }
      }
      switch (mode) {
        case Mode::kImm:
          operand = "#" + text;
          break;
        case Mode::kZpx:
        case Mode::kAbsx:
          operand = text + ",X";
          break;
        case Mode::kZpy:
        case Mode::kAbsy:
          operand = text + ",Y";
          break;
        case Mode::kInd:
          operand = "(" + text + ")";
          break;
        case Mode::kIndx:
          operand = "(" + text + ",X)";
          break;
        case Mode::kIndy:
          operand = "(" + text + "),Y";
          break;
        default:
          operand = text;
          break;
      }
    }
    out.push_back(line(mnemonic, operand, comment(info.base + off, raw)));
    return off + n;
  }

  const elf::ElfFile& elf_;
  Options opt_;
  bool exe_;
  std::map<std::int64_t, std::vector<const elf::Symbol*>> by_addr_;
  std::vector<const elf::Symbol*> objects_;
};

// Returns the name of a section's `SEG`: the name without its leading
// dots, with each code point other than an ASCII letter, digit, or `_`
// replaced by `_`, as `re.sub(r"[^A-Za-z0-9_]", "_", ...)` does, or
// `seg` when nothing is left.
std::string seg_name(const std::string& name) {
  const std::size_t start = name.find_first_not_of('.');
  const std::string_view rest =
      start == std::string::npos ? std::string_view() : std::string_view(name).substr(start);
  std::string out;
  for (std::size_t i = 0; i < rest.size();) {
    std::size_t len = 0;
    const std::uint32_t cp = pcpp::decode_at(rest, i, len);
    const bool keep = (cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z') ||
                      (cp >= '0' && cp <= '9') || cp == '_';
    out += keep ? static_cast<char>(cp) : '_';
    i += len;
  }
  return out.empty() ? "seg" : out;
}

}  // namespace

std::string format_hex(std::int64_t v, int width, bool lower) {
  const char* digits = lower ? "0123456789abcdef" : "0123456789ABCDEF";
  const bool negative = v < 0;
  auto u = negative ? 0 - static_cast<std::uint64_t>(v) : static_cast<std::uint64_t>(v);
  std::string s;
  do {
    s.insert(s.begin(), digits[u % 16]);
    u /= 16;
  } while (u != 0);
  const int pad = width - static_cast<int>(s.size()) - (negative ? 1 : 0);
  if (pad > 0) s.insert(0, static_cast<std::size_t>(pad), '0');
  return negative ? "-" + s : s;
}

std::string ljust(std::string_view s, std::size_t width) {
  std::string out(s);
  const std::size_t n = pcpp::code_point_count(s);
  if (n < width) out.append(width - n, ' ');
  return out;
}

std::vector<std::string> disassemble(const elf::ElfFile& elf, bool all_sections,
                                     const std::set<std::string>* only, const Options& options) {
  const Disassembler d(elf, options);
  std::vector<std::string> out;
  out.push_back(std::string("; elf32-mos ") + (d.exe() ? "executable" : "object"));
  out.push_back("   PROCESSOR 6502");
  const std::optional<std::string> text = elf.link_text();
  if (text && !text->empty() && !d.exe()) {
    out.push_back("");
    const std::size_t end = text->find_last_not_of('\n');
    const std::string body = end == std::string::npos ? "" : text->substr(0, end + 1);
    for (auto& l : pcpp::splitlines(body)) out.push_back(std::move(l));
  }
  if (d.exe()) {
    std::vector<const elf::Symbol*> absolute;
    for (const auto& s : elf.symbols) {
      if (s->is_absolute() && s->type != elf::STT_FILE && !starts_with_dot(s->name)) {
        absolute.push_back(s.get());
      }
    }
    std::stable_sort(absolute.begin(), absolute.end(), [](const elf::Symbol* a, const elf::Symbol* b) {
      return std::tie(a->value, a->name) < std::tie(b->value, b->name);
    });
    if (!absolute.empty()) {
      out.push_back("");
      std::set<std::string> seen;
      for (const auto* s : absolute) {
        if (!seen.insert(s->name).second) continue;
        const int width = s->value < 0x100 ? 1 : 2;
        out.push_back(s->name + "\tEQU\t" + hex(s->value, width));
      }
    }
  }
  // A later segment at the same address replaces an earlier one, as the
  // dict does.
  std::map<std::int64_t, std::int64_t> load;
  for (const auto& seg : elf.segments) load[seg.vaddr] = seg.paddr;
  const auto lma_of = [&](const elf::Section& sec) {
    const auto it = load.find(sec.addr);
    return it == load.end() ? sec.addr : it->second;
  };
  for (const auto& sec : elf.sections) {
    if (!(sec->flags & elf::SHF_ALLOC)) continue;
    if (only != nullptr && !only->contains(sec->name)) continue;
    if (only == nullptr && !all_sections && !(sec->flags & elf::SHF_EXECINSTR)) continue;
    if (sec->size() == 0 &&
        std::none_of(elf.symbols.begin(), elf.symbols.end(),
                     [&](const elf::SymbolPtr& s) { return s->section == sec; })) {
      continue;
    }
    out.push_back("");
    out.push_back("; Disassembly of section " + sec->name +
                  (d.exe() ? " at $" + format_hex(sec->addr, 4) : ""));
    if (d.exe()) {
      out.push_back(line(sec->type == elf::SHT_NOBITS ? "SEG.U" : "SEG", seg_name(sec->name)));
      const std::int64_t lma = lma_of(*sec);
      out.push_back(line("ORG", hex(lma, 2)));
      if (lma != sec->addr) out.push_back(line("RORG", hex(sec->addr, 2)));
    }
    for (auto& l : d.section_lines(*sec)) out.push_back(std::move(l));
    if (d.exe() && lma_of(*sec) != sec->addr) out.push_back(line("REND"));
  }
  return out;
}

}  // namespace c6502::binutils::disasm
