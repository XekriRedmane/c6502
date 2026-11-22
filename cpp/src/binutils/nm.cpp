#include "binutils/nm.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <tuple>

#include "argparse.h"
#include "errors.h"
#include "pyrepr.h"

namespace c6502::binutils::nm {

namespace {

namespace fs = std::filesystem;

std::string letter(const elf::Symbol& s) {
  if (!s.defined()) return "U";
  char c;
  if (s.is_absolute()) {
    c = 'a';
  } else {
    const elf::Section& sec = *s.section;
    if (sec.flags & elf::SHF_EXECINSTR) {
      c = 't';
    } else if (sec.type == elf::SHT_NOBITS) {
      c = 'b';
    } else if (sec.flags & elf::SHF_WRITE) {
      c = 'd';
    } else {
      c = 'r';
    }
  }
  if (s.is_global()) c = static_cast<char>(c - 'a' + 'A');
  return std::string(1, c);
}

bool hidden(const elf::Symbol& s) {
  return s.type == elf::STT_FILE || s.type == elf::STT_SECTION ||
         (!s.name.empty() && s.name[0] == '.') || s.name == "$c" || s.name == "$d";
}

std::string hex4(std::int64_t v) {
  char buf[24];
  std::snprintf(buf, sizeof buf, "%04llX", static_cast<unsigned long long>(v));
  return buf;
}

}  // namespace

std::vector<std::string> symbol_lines(const elf::ElfFile& elf, const Options& o) {
  std::vector<const elf::Symbol*> syms;
  for (const auto& s : elf.symbols) {
    if (o.debug || !hidden(*s)) syms.push_back(s.get());
  }
  const auto keep = [&](auto pred) {
    std::vector<const elf::Symbol*> out;
    for (const auto* s : syms) {
      if (pred(*s)) out.push_back(s);
    }
    syms = std::move(out);
  };
  if (o.extern_only) keep([](const elf::Symbol& s) { return s.is_global(); });
  if (o.undefined_only) keep([](const elf::Symbol& s) { return !s.defined(); });
  if (o.defined_only) keep([](const elf::Symbol& s) { return s.defined(); });
  // Python's sort is stable, and its strings compare by code point,
  // which is the order of their UTF-8 bytes.
  if (o.sort == Sort::kName) {
    std::stable_sort(syms.begin(), syms.end(), [](const elf::Symbol* a, const elf::Symbol* b) {
      return std::tie(a->name, a->value) < std::tie(b->name, b->value);
    });
  } else if (o.sort == Sort::kNumeric) {
    std::stable_sort(syms.begin(), syms.end(), [](const elf::Symbol* a, const elf::Symbol* b) {
      const bool da = a->defined();
      const bool db = b->defined();
      return std::tie(da, a->value, a->name) < std::tie(db, b->value, b->name);
    });
  }
  if (o.reverse) std::reverse(syms.begin(), syms.end());
  std::vector<std::string> out;
  for (const auto* s : syms) {
    std::string l = letter(*s);
    if (o.debug && !s->defined() && (s->other & elf::STO_MOS_ZEROPAGE)) l += "z";
    std::string value;
    if (s->defined()) {
      value = hex4(s->value & 0xFFFF);
      if (o.size) value += " " + hex4(s->size);
    } else {
      value = std::string(o.size ? 9 : 4, ' ');
    }
    out.push_back(o.prefix + value + " " + l + " " + s->name);
  }
  return out;
}

std::string read_path(const std::string& path) {
  // Python opens `str(Path(path))`, so `""` is the current directory.
  const std::string normal = argparse::path_str(path);
  const std::string shown = pyrepr::string(normal);
  std::error_code ec;
  const fs::path p(std::u8string(normal.begin(), normal.end()));
  if (!fs::exists(p, ec)) {
    throw FileError{"[Errno 2] No such file or directory: " + shown};
  }
  if (fs::is_directory(p, ec)) {
#ifdef _WIN32
    throw FileError{"[Errno 13] Permission denied: " + shown};
#else
    throw FileError{"[Errno 21] Is a directory: " + shown};
#endif
  }
  std::ifstream in(p, std::ios::binary);
  if (!in) throw FileError{"[Errno 13] Permission denied: " + shown};
  return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

int main(const std::vector<std::string>& argv, const ReadFile& read_file, int columns,
         std::string& out, std::string& err, bool* help) {
  if (help != nullptr) *help = false;
  argparse::Parser ap("nm.py", "List the symbols of c6502 objects and executables.");
  ap.add_positional("files", "object or executable files");
  ap.add_store_true({"-a", "--debug-syms"}, "list local labels, mapping and file symbols too");
  ap.add_store_true({"-g", "--extern-only"}, "only global symbols");
  ap.add_store_true({"-u", "--undefined-only"}, "only undefined symbols");
  ap.add_store_true({"--defined-only"}, "only defined symbols");
  ap.add_store_true({"-n", "--numeric-sort"}, "sort by value");
  ap.add_store_true({"-p", "--no-sort"}, "keep the file's order");
  ap.add_store_true({"-r", "--reverse-sort"}, "reverse the order");
  ap.add_store_true({"-S", "--print-size"}, "print the size of each defined symbol");
  ap.add_store_true({"-A", "-o", "--print-file-name"}, "start each line with the file's name");
  argparse::Namespace args;
  try {
    args = ap.parse_args(std::vector<std::string>(argv.begin() + 1, argv.end()), columns);
  } catch (const argparse::Exit& e) {
    out += e.out;
    err += e.err;
    if (help != nullptr) *help = !e.out.empty();
    return e.status;
  }
  Options base;
  base.debug = args.flag("debug_syms");
  base.extern_only = args.flag("extern_only");
  base.undefined_only = args.flag("undefined_only");
  base.defined_only = args.flag("defined_only");
  base.sort = args.flag("no_sort") ? Sort::kNone
                                   : (args.flag("numeric_sort") ? Sort::kNumeric : Sort::kName);
  base.reverse = args.flag("reverse_sort");
  base.size = args.flag("print_size");
  const bool print_file_name = args.flag("print_file_name");
  const std::vector<std::string>& files = *args.list("files");
  int status = 0;
  const bool many = files.size() > 1;
  for (const std::string& path : files) {
    elf::ElfFile elf;
    try {
      elf = elf::read(read_file(path), argparse::path_str(path));
    } catch (const FileError& e) {
      err += "nm.py: " + e.message + "\n";
      status = 1;
      continue;
    } catch (const CompileError& e) {
      err += std::string("nm.py: ") + e.what() + "\n";
      status = 1;
      continue;
    }
    std::vector<std::string> lines;
    if (many && !print_file_name) {
      lines.push_back("");
      lines.push_back(path + ":");
    }
    Options o = base;
    o.prefix = print_file_name ? path + ":" : "";
    for (auto& l : symbol_lines(elf, o)) lines.push_back(std::move(l));
    if (!lines.empty()) {
      for (const auto& l : lines) out += l + "\n";
    }
  }
  return status;
}

}  // namespace c6502::binutils::nm
