// objdump.py. Python formats the columns with `str.format`, so a name
// is padded by code points (`ljust`), a number with `X` is in uppercase
// hex and with `x` in lowercase, and the alignment is printed as the
// power of two `2**(align.bit_length() - 1)`.
#include "binutils/objdump.h"

#include <algorithm>
#include <cstdio>
#include <map>

#include "argparse.h"
#include "binutils/disasm.h"
#include "errors.h"
#include "pcpp_text.h"

namespace c6502::binutils::objdump {

namespace {

using disasm::format_hex;
using disasm::ljust;

std::string section_name(const elf::Symbol& s) {
  if (!s.defined()) return "*UND*";
  if (s.is_absolute()) return "*ABS*";
  return s.section->name;
}

std::string flags(const elf::Section& sec) {
  std::vector<std::string> out;
  if (sec.type != elf::SHT_NOBITS) out.push_back("CONTENTS");
  if (sec.flags & elf::SHF_ALLOC) {
    out.push_back("ALLOC");
    if (sec.type != elf::SHT_NOBITS) out.push_back("LOAD");
  }
  if (!sec.relocs.empty()) out.push_back("RELOC");
  if (!(sec.flags & elf::SHF_WRITE) && (sec.flags & elf::SHF_ALLOC)) out.push_back("READONLY");
  if (sec.flags & elf::SHF_EXECINSTR) {
    out.push_back("CODE");
  } else if (sec.flags & elf::SHF_ALLOC) {
    out.push_back("DATA");
  }
  std::string text;
  for (std::size_t k = 0; k < out.size(); ++k) {
    if (k != 0) text += ", ";
    text += out[k];
  }
  return text;
}

// Returns `int.bit_length()` of a positive `v`.
int bit_length(std::int64_t v) {
  int n = 0;
  for (auto u = static_cast<std::uint64_t>(v); u != 0; u >>= 1) ++n;
  return n;
}

// Returns `text.rstrip("\n").splitlines()`.
std::vector<std::string> metadata_lines(const std::string& text) {
  const std::size_t end = text.find_last_not_of('\n');
  return pcpp::splitlines(end == std::string::npos ? "" : text.substr(0, end + 1));
}

void append(std::vector<std::string>& lines, std::vector<std::string> more) {
  for (auto& l : more) lines.push_back(std::move(l));
}

}  // namespace

std::vector<std::string> file_header(const elf::ElfFile& elf, const std::string& path) {
  std::string type;
  if (elf.type == elf::ET_REL) {
    type = "relocatable object";
  } else if (elf.type == elf::ET_EXEC) {
    type = "executable";
  } else {
    type = "type " + std::to_string(elf.type);
  }
  return {
      path + ":     file format elf32-mos",
      "architecture: mos6502, flags 0x" + format_hex(elf.flags, 8) + ":",
      type,
      "start address 0x" + format_hex(elf.entry, 4),
  };
}

std::vector<std::string> section_headers(const elf::ElfFile& elf) {
  // A later segment at the same address replaces an earlier one, as the
  // dict does.
  std::map<std::int64_t, std::int64_t> load;
  for (const auto& seg : elf.segments) load[seg.vaddr] = seg.paddr;
  std::vector<std::string> out{"Sections:", "Idx Name                 Size  VMA   LMA   Algn"};
  for (std::size_t i = 0; i < elf.sections.size(); ++i) {
    const elf::Section& sec = *elf.sections[i];
    const auto it = load.find(sec.addr);
    const std::int64_t lma = it == load.end() ? sec.addr : it->second;
    char index[24];
    std::snprintf(index, sizeof index, "%3zu", i);
    out.push_back(std::string(index) + " " + ljust(sec.name, 20) + " " +
                  format_hex(sec.size(), 4) + "  " + format_hex(sec.addr, 4) + "  " +
                  format_hex(lma, 4) + "  2**" +
                  std::to_string(bit_length(std::max<std::int64_t>(sec.align, 1)) - 1));
    out.push_back(std::string(25, ' ') + flags(sec));
  }
  return out;
}

std::vector<std::string> symbol_table(const elf::ElfFile& elf) {
  std::vector<std::string> out{"SYMBOL TABLE:"};
  for (const auto& s : elf.symbols) {
    const char* bind = s->is_global() ? "g" : "l";
    std::string kind;
    switch (s->type) {
      case elf::STT_NOTYPE:
        kind = "";
        break;
      case elf::STT_OBJECT:
        kind = "O";
        break;
      case elf::STT_FUNC:
        kind = "F";
        break;
      case elf::STT_SECTION:
        kind = "S";
        break;
      case elf::STT_FILE:
        kind = "f";
        break;
      default:
        kind = "?";
        break;
    }
    out.push_back(format_hex(s->value & 0xFFFF, 4) + " " + ljust(bind, 2) + ljust(kind, 2) + " " +
                  ljust(section_name(*s), 12) + " " + format_hex(s->size, 4) + " " + s->name);
  }
  return out;
}

std::vector<std::string> relocations(const elf::ElfFile& elf, const std::set<std::string>* only) {
  std::vector<std::string> out;
  for (const auto& sec : elf.sections) {
    if (sec->relocs.empty() || (only != nullptr && !only->contains(sec->name))) continue;
    std::string owner;
    for (const auto& s : elf.symbols) {
      if (s->section == sec && (s->type == elf::STT_FUNC || s->type == elf::STT_OBJECT)) {
        owner = s->name;
        break;
      }
    }
    out.push_back("RELOCATION RECORDS FOR [" + sec->name + "]" +
                  (owner.empty() ? "" : " (" + owner + ")") + ":");
    out.push_back("OFFSET TYPE              VALUE");
    for (const auto& r : sec->relocs) {
      const char* known = elf::reloc_name(r.type);
      const std::string name = known != nullptr ? known : std::to_string(r.type);
      std::string add;
      if (r.addend != 0) {
        add = r.addend > 0 ? "+" + std::to_string(r.addend) : "-" + std::to_string(-r.addend);
      }
      out.push_back(format_hex(r.offset, 4) + "   " + ljust(name, 17) + " " + r.symbol->name + add);
    }
    out.push_back("");
  }
  return out;
}

std::vector<std::string> full_contents(const elf::ElfFile& elf, const std::set<std::string>* only) {
  std::vector<std::string> out;
  for (const auto& sec : elf.sections) {
    if (sec->type == elf::SHT_NOBITS) continue;
    if (only != nullptr && !only->contains(sec->name)) continue;
    if (only == nullptr && !(sec->flags & elf::SHF_ALLOC) && sec->name != elf::LINK_SECTION) {
      continue;
    }
    out.push_back("Contents of section " + sec->name + ":");
    const std::string& data = sec->data;
    for (std::size_t off = 0; off < data.size(); off += 16) {
      const std::size_t n = std::min<std::size_t>(16, data.size() - off);
      std::string hexes;
      std::string text;
      for (std::size_t k = 0; k < n; ++k) {
        if (k != 0 && k % 4 == 0) hexes += ' ';
        const auto b = static_cast<unsigned char>(data[off + k]);
        hexes += format_hex(b, 2, true);
        text += (b >= 32 && b < 127) ? static_cast<char>(b) : '.';
      }
      out.push_back(" " + format_hex(sec->addr + static_cast<std::int64_t>(off), 4, true) + " " +
                    ljust(hexes, 35) + "  " + text);
    }
  }
  return out;
}

int main(const std::vector<std::string>& argv, const nm::ReadFile& read_file, int columns,
         std::string& out, std::string& err, bool* help) {
  if (help != nullptr) *help = false;
  argparse::Parser ap("objdump.py", "Show what's in c6502 objects and executables.", false);
  ap.add_help({"--help"}, "show this message and exit");
  ap.add_positional("files");
  ap.add_store_true({"-d", "--disassemble"});
  ap.add_store_true({"-D", "--disassemble-all"});
  ap.add_append({"-j", "--section"}, "sections");
  ap.add_store_true({"-f", "--file-headers"});
  ap.add_store_true({"-h", "--section-headers"});
  ap.add_store_true({"-t", "--syms"});
  ap.add_store_true({"-r", "--reloc"});
  ap.add_store_true({"-s", "--full-contents"});
  ap.add_store_true({"-x", "--all-headers"});
  ap.add_store_true({"--link-metadata"});
  ap.add_store_true({"--no-show-raw-insn"});
  ap.add_store_true({"--no-addresses"});
  argparse::Namespace args;
  try {
    args = ap.parse_args(std::vector<std::string>(argv.begin() + 1, argv.end()), columns);
  } catch (const argparse::Exit& e) {
    out += e.out;
    err += e.err;
    if (help != nullptr) *help = !e.out.empty();
    return e.status;
  }
  const bool disassemble = args.flag("disassemble");
  const bool disassemble_all = args.flag("disassemble_all");
  const bool file_headers = args.flag("file_headers");
  const bool section_headers_ = args.flag("section_headers");
  const bool syms = args.flag("syms");
  const bool reloc = args.flag("reloc");
  const bool contents = args.flag("full_contents");
  const bool all_headers = args.flag("all_headers");
  const bool link_metadata = args.flag("link_metadata");
  if (!(disassemble || disassemble_all || file_headers || section_headers_ || syms || reloc ||
        contents || all_headers || link_metadata)) {
    err += "objdump.py: give at least one of -d -D -f -h -t -r -s -x --link-metadata\n";
    return 2;
  }
  std::set<std::string> sections;
  const std::vector<std::string>* listed = args.list("sections");
  if (listed != nullptr) sections.insert(listed->begin(), listed->end());
  const std::set<std::string>* only = listed != nullptr && !listed->empty() ? &sections : nullptr;
  const disasm::Options options{!args.flag("no_show_raw_insn"), !args.flag("no_addresses")};
  int status = 0;
  for (const std::string& path : *args.list("files")) {
    elf::ElfFile elf;
    try {
      // `E.read_file` names the file `str(path)`, which is the command
      // line's string itself, where the OSError names `str(Path(path))`.
      elf = elf::read(read_file(path), path);
    } catch (const nm::FileError& e) {
      err += "objdump.py: " + e.message + "\n";
      status = 1;
      continue;
    } catch (const CompileError& e) {
      err += std::string("objdump.py: ") + e.what() + "\n";
      status = 1;
      continue;
    }
    std::vector<std::string> lines;
    if (file_headers || all_headers) {
      append(lines, file_header(elf, path));
      lines.push_back("");
    }
    if (section_headers_ || all_headers) {
      append(lines, section_headers(elf));
      lines.push_back("");
    }
    if (syms || all_headers) {
      append(lines, symbol_table(elf));
      lines.push_back("");
    }
    if (reloc || all_headers) append(lines, relocations(elf, only));
    if (link_metadata || all_headers) {
      const std::optional<std::string> text = elf.link_text();
      if (text && !text->empty()) {
        append(lines, metadata_lines(*text));
      } else {
        lines.push_back("(no link metadata)");
      }
      lines.push_back("");
    }
    if (contents) {
      append(lines, full_contents(elf, only));
      lines.push_back("");
    }
    if (disassemble || disassemble_all) {
      append(lines, disasm::disassemble(elf, disassemble_all, only, options));
    }
    while (!lines.empty() && lines.back().empty()) lines.pop_back();
    std::string text;
    for (std::size_t k = 0; k < lines.size(); ++k) {
      if (k != 0) text += '\n';
      text += lines[k];
    }
    out += text + "\n";
  }
  return status;
}

}  // namespace c6502::binutils::objdump
