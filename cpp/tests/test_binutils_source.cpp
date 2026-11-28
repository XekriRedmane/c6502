// The tests of tests/test_binutils.py that run `compile.py -c`, a link,
// nm, and objdump on files and read what they write, the ELF reader's
// checks on an object the assembler made, and the assembler and the
// listings of linked programs checked against dasm, byte for byte.
//
// The dasm cases run only where dasm is on the PATH, and pass with a
// message saying they were skipped where it isn't.
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "binutils/elf.h"
#include "compile_case.h"
#include "doctest.h"
#include "errors.h"
#include "py_regex.h"
#include "sim_case.h"

namespace {

namespace suite = c6502::suite;
namespace elf = c6502::binutils::elf;
namespace fs = std::filesystem;

// Returns whether Python's `re.search(pattern, text)` finds a match,
// for an ASCII `text`.
bool py_search(std::string_view pattern, std::string_view text) {
  const c6502::py_regex::Regex re(pattern);
  const std::u32string t(text.begin(), text.end());
  for (std::size_t pos = 0; pos <= t.size(); ++pos) {
    if (re.match(t, pos)) return true;
  }
  return false;
}

elf::ElfFile read_elf(const std::string& path) {
  return elf::read(suite::read_file(path), path);
}

// Returns the text of the file at `path` as Python's `read_text` gives
// it, with each CR LF made LF.
std::string read_text(const std::string& path) {
  const std::string bytes = suite::read_file(path);
  std::string out;
  for (std::size_t k = 0; k < bytes.size(); ++k) {
    if (bytes[k] == '\r' && k + 1 < bytes.size() && bytes[k + 1] == '\n') continue;
    out += bytes[k];
  }
  return out;
}

// The object `compile.py -c` assembles from `text`, written to a file
// called `name`.
elf::ElfFile assemble(const std::string& text, const std::string& name = "t.asm") {
  const suite::TempDir tmp;
  const std::string src = tmp.file(name);
  const std::string out = tmp.file("t.o");
  suite::write_file(src, text);
  const auto o = suite::compile_main({"c6502", "-c", src, "-o", out});
  REQUIRE_MESSAGE(o.status == 0, o.err());
  return read_elf(out);
}

// `obj`'s loaded sections one after another from `origin`, with their
// relocations applied as the linker applies them, and with the names
// the object leaves undefined given by `symbols`.
std::string image(const elf::ElfFile& obj, const std::map<std::string, std::int64_t>& symbols,
                  std::int64_t origin = 0x0800) {
  std::vector<elf::SectionPtr> alloc;
  for (const auto& s : obj.sections) {
    if (s->flags & elf::SHF_ALLOC) alloc.push_back(s);
  }
  std::map<const elf::Section*, std::int64_t> addrs;
  std::int64_t addr = origin;
  for (const auto& s : alloc) {
    addrs[s.get()] = addr;
    addr += s->size();
  }
  const auto value = [&](const elf::Symbol& sym) -> std::int64_t {
    if (!sym.defined()) {
      const auto it = symbols.find(sym.name);
      REQUIRE_MESSAGE(it != symbols.end(), sym.name);
      return it->second;
    }
    if (sym.is_absolute()) return sym.value;
    return addrs.at(sym.section.get()) + sym.value;
  };
  std::string out;
  for (const auto& s : alloc) {
    if (s->type == elf::SHT_NOBITS) {
      out += std::string(static_cast<std::size_t>(s->size()), '\0');
      continue;
    }
    std::string data = s->data;
    for (const elf::Reloc& r : s->relocs) {
      const std::int64_t v = value(*r.symbol) + r.addend;
      const auto at = static_cast<std::size_t>(r.offset);
      switch (r.type) {
        case elf::R_MOS_ADDR16:
          REQUIRE((0 <= v && v <= 0xFFFF));
          data[at] = static_cast<char>(v & 0xFF);
          data[at + 1] = static_cast<char>(v >> 8);
          break;
        case elf::R_MOS_ADDR8:
        case elf::R_MOS_IMM8:
        case elf::R_MOS_ADDR16_LO:
          data[at] = static_cast<char>(v & 0xFF);
          break;
        case elf::R_MOS_ADDR16_HI:
          data[at] = static_cast<char>((v >> 8) & 0xFF);
          break;
        case elf::R_MOS_PCREL_8:
          data[at] = static_cast<char>((v - (addrs.at(s.get()) + r.offset)) & 0xFF);
          break;
        default:
          FAIL("relocation type " << int(r.type));
      }
    }
    out += data;
  }
  return out;
}

// The read-modify-write instructions, which have zp,X and abs,X forms.
const std::vector<std::string> kRmwOps = {"INC", "DEC", "ASL", "LSR", "ROL", "ROR"};

// A function `name` whose body is `body`, a line each, with the labels
// unindented.
std::string fn(const std::vector<std::string>& body, const std::string& name = "f") {
  std::string out = name + ":\n   SUBROUTINE";
  for (const std::string& b : body) out += "\n" + (b.ends_with(":") ? b : "   " + b);
  return out + "\n";
}

}  // namespace

TEST_CASE("binutils: an assembled object reads back with the same sections and symbols") {
  const std::string text = read_text(suite::repo_path("golden/O/examples/apply_bobble.s"));
  const elf::ElfFile obj = assemble(text, "apply_bobble.asm");
  const std::string data = elf::write(obj);
  const elf::ElfFile back = elf::read(data, "file");
  CHECK(elf::write(back) == data);
  CHECK(back.type == elf::ET_REL);
  using SectionKey = std::tuple<std::string, std::uint32_t, std::string>;
  const auto sections = [](const elf::ElfFile& f) {
    std::vector<SectionKey> out;
    for (const auto& s : f.sections) out.emplace_back(s->name, s->type, s->data);
    return out;
  };
  CHECK(sections(back) == sections(obj));
  using SymbolKey = std::tuple<std::string, std::int64_t, std::uint8_t, std::uint8_t>;
  const auto symbols = [](const elf::ElfFile& f) {
    std::vector<SymbolKey> out;
    for (const auto& s : f.symbols) out.emplace_back(s->name, s->value, s->bind, s->type);
    std::sort(out.begin(), out.end());
    return out;
  };
  CHECK(symbols(back) == symbols(obj));
  bool found = false;
  for (const auto& s : back.sections) {
    for (const elf::Reloc& r : s->relocs) {
      found = found || (r.offset == 1 && r.type == elf::R_MOS_ADDR8 &&
                        r.symbol->name == "__local_apply_bobble__slot" && r.addend == 0);
    }
  }
  CHECK(found);
  CHECK(back.link_text() == obj.link_text());
  CHECK(back.source_text() == std::optional<std::string>(text));
}

TEST_CASE("binutils: an assembled object's header, and a file that isn't ELF") {
  const std::string data = elf::write(assemble(fn({"RTS"})));
  CHECK(data.substr(0, 4) == "\x7f" "ELF");
  CHECK((static_cast<unsigned char>(data[18]) | static_cast<unsigned char>(data[19]) << 8) ==
        elf::EM_MOS);
  try {
    elf::read("not an elf file at all, no", "file");
    FAIL("read it");
  } catch (const c6502::CompileError& e) {
    CHECK(e.python_type() == "ElfError");
  }
}

TEST_CASE("binutils: each damaged copy of an assembled object is an ElfError") {
  // Each of these raised struct.error, IndexError, or ValueError in
  // Python's reader, which nm.py, objdump.py, and compile.py printed as
  // a traceback.
  const std::string text = read_text(suite::repo_path("golden/O/examples/apply_bobble.s"));
  const std::string data = elf::write(assemble(text, "apply_bobble.asm"));
  const auto u32 = [&](std::size_t at) {
    std::uint32_t v = 0;
    for (int k = 3; k >= 0; --k) v = v << 8 | static_cast<unsigned char>(data[at + k]);
    return v;
  };
  const std::size_t shoff = u32(32);
  const std::size_t shstrndx = 50;
  const auto with_bytes = [&](std::size_t at, const std::string& bytes) {
    std::string out = data;
    out.replace(at, bytes.size(), bytes);
    return out;
  };
  const std::size_t meta = data.find("; @zp-link-meta-begin");
  REQUIRE(meta != std::string::npos);
  CHECK(suite::contains(data, std::string(elf::LINK_SECTION)));
  const std::vector<std::pair<std::string, std::string>> damaged = {
      {"truncated", data.substr(0, data.size() / 2)},
      {"the section headers cut short", data.substr(0, data.size() - 1)},
      {"the names' table index past the end", with_bytes(shstrndx, std::string("\xFF\x7F", 2))},
      // The size field of the second section header.
      {"a section's bytes past the end",
       with_bytes(shoff + 40 + 20, std::string("\xFF\xFF\xFF\x00", 4))},
      {"link metadata that isn't UTF-8", with_bytes(meta, "\xFF")},
  };
  for (const auto& [what, bad] : damaged) {
    INFO(what);
    try {
      elf::read(bad, "x.o");
      FAIL("read it");
    } catch (const c6502::CompileError& e) {
      CHECK(e.python_type() == "ElfError");
      CHECK(std::string(e.what()) == "x.o: the ELF file is truncated or damaged");
    }
  }
  const suite::TempDir tmp;
  const std::string path = tmp.file("x.o");
  suite::write_file(path, damaged[0].second);
  const suite::ToolOutcome o = suite::nm_main({"c6502-nm", path});
  CHECK(o.status == 1);
  CHECK(o.err == "c6502-nm: " + path + ": the ELF file is truncated or damaged\n");
}

namespace {

const char* const kUnitA =
    "#include <stdint.h>\n"
    "uint8_t n;\n"
    "void bump(uint8_t k) { for (uint8_t i = 0; i < k; i++) n++; }\n";
const char* const kUnitB =
    "#include <stdint.h>\n"
    "extern uint8_t n;\n"
    "void bump(uint8_t k);\n"
    "static uint8_t twice(uint8_t x) { return x + x; }\n"
    "int main(void) { bump(twice(3)); return n; }\n";

// A new directory holding a.c and b.c, which is the current directory
// until the object goes.
struct Tools {
  suite::TempDir tmp;
  suite::ChangeDir cd{init(tmp)};

  static const fs::path& init(const suite::TempDir& t) {
    suite::write_file(t.file("a.c"), kUnitA);
    suite::write_file(t.file("b.c"), kUnitB);
    return t.path();
  }
};

// Runs `compile.py` with `args`, and returns the exit status and
// stderr.
std::pair<long long, std::string> run(const std::vector<std::string>& args,
                                      bool inline_calls = true) {
  std::vector<std::string> argv{"c6502"};
  argv.insert(argv.end(), args.begin(), args.end());
  suite::MainOptions options;
  options.inline_calls = inline_calls;
  const auto o = suite::compile_main(argv, "", options);
  REQUIRE(o.status.has_value());
  return {*o.status, o.err()};
}

}  // namespace

TEST_CASE("binutils: units compiled with -c, linked, and run") {
  const Tools t;
  REQUIRE(run({"-O", "-c", "a.c", "b.c"}).first == 0);
  REQUIRE(run({"a.o", "b.o"}).first == 0);
  const elf::ElfFile exe = read_elf("a.out");
  CHECK(exe.type == elf::ET_EXEC);
  CHECK(exe.entry == exe.symbol("main")->value);
  CHECK(suite::build_linked_sim("a.out").run().return_int() == 6);
}

TEST_CASE("binutils: c6502 -c names each object after its source") {
  const Tools t;
  REQUIRE(run({"-O", "-c", "a.c", "b.c"}).first == 0);
  for (const char* name : {"a.o", "b.o"}) {
    INFO(name);
    const elf::ElfFile obj = read_elf(name);
    CHECK(obj.type == elf::ET_REL);
    CHECK(obj.link_text().has_value());
  }
}

TEST_CASE("binutils: a reference to another unit's zero-page object is relaxed at the link") {
  // `a.c` promotes `n` into zero page, and `b.c` only declares it, so
  // it reaches `n` with absolute addressing, which the link shortens
  // to zero page.
  const Tools t;
  REQUIRE(run({"-O", "-c", "a.c", "b.c"}).first == 0);
  CHECK(suite::contains(read_elf("a.o").link_text().value_or(""), "weight="));
  const elf::ElfFile b = read_elf("b.o");
  REQUIRE(b.symbol("main") != nullptr);
  CHECK(suite::contains(b.symbol("main")->section->data, "\xAD"));  // LDA abs n
  REQUIRE(run({"a.o", "b.o", "-o", "prog"}).first == 0);
  const elf::ElfFile exe = read_elf("prog");
  const std::int64_t n = exe.symbol("n")->value;
  CHECK(n < 0x100);
  const auto m = exe.symbol("main");
  const elf::Segment* seg = nullptr;
  for (const auto& s : exe.segments) {
    if (s.vaddr <= m->value && m->value < s.vaddr + s.memsz) {
      seg = &s;
      break;
    }
  }
  REQUIRE(seg != nullptr);
  const std::string code = seg->data.substr(static_cast<std::size_t>(m->value - seg->vaddr),
                                            static_cast<std::size_t>(m->size));
  CHECK(suite::contains(code, std::string{'\xA5', static_cast<char>(n)}));
}

TEST_CASE("binutils: -Ttext moves the origin, and --defsym gives an undefined name") {
  const Tools t;
  REQUIRE(run({"-O", "a.c", "b.c", "-Ttext=0x1000", "-o", "prog"}).first == 0);
  CHECK(read_elf("prog").sections.at(0)->addr == 0x1000);
  suite::write_file("c.c", "extern int rom(void);\nint main(void) { return rom(); }\n");
  const auto [rc, err] = run({"-O", "c.c"});
  CHECK(rc == 1);
  CHECK(suite::contains(err, "undefined symbol `rom`"));
  REQUIRE(run({"-O", "c.c", "--defsym", "rom=$E000", "-o", "prog"}).first == 0);
  CHECK(read_elf("prog").symbol("rom")->value == 0xE000);
}

TEST_CASE("binutils: nm lists an object's and an executable's symbols") {
  // With inlining off, `twice` stays a function of its own, a local
  // symbol.
  const Tools t;
  REQUIRE(run({"-O", "-c", "b.c"}, false).first == 0);
  suite::ToolOutcome o = suite::nm_main({"c6502-nm", "b.o"});
  CHECK(o.status == 0);
  const std::vector<std::string> lines = suite::lines(o.out);
  for (const char* line : {"0000 T main", "0000 t twice", "     U bump", "     U n"}) {
    INFO(line);
    CHECK(std::find(lines.begin(), lines.end(), line) != lines.end());
  }
  CHECK_FALSE(suite::contains(o.out, "$c"));
  o = suite::nm_main({"c6502-nm", "-g", "-u", "b.o"});
  CHECK_FALSE(suite::contains(o.out, "main"));
  CHECK(suite::contains(o.out, "U n"));
  REQUIRE(run({"-O", "a.c", "b.c", "-o", "prog"}, false).first == 0);
  o = suite::nm_main({"c6502-nm", "-n", "prog"});
  std::vector<std::string> names;
  for (const std::string& line : suite::lines(o.out)) {
    const std::size_t last = line.find_last_of(' ');
    names.push_back(last == std::string::npos ? line : line.substr(last + 1));
  }
  // a.c's `bump` comes first, as a.o does.
  const auto bump = std::find(names.begin(), names.end(), "bump");
  const auto main = std::find(names.begin(), names.end(), "main");
  REQUIRE(bump != names.end());
  REQUIRE(main != names.end());
  CHECK(bump < main);
  CHECK(py_search("(?m)^00[0-9A-F]{2} A n$", o.out));
}

TEST_CASE("binutils: objdump disassembles an object and an executable") {
  const Tools t;
  REQUIRE(run({"-O", "-c", "b.c"}).first == 0);
  suite::ToolOutcome o = suite::objdump_main({"c6502-objdump", "-d", "b.o"});
  CHECK(o.status == 0);
  CHECK(py_search("(?m)^main:\n   SUBROUTINE$", o.out));
  CHECK(py_search("JSR   bump +; [0-9A-F]{4}  20 00 00", o.out));
  o = suite::objdump_main({"c6502-objdump", "-x", "b.o"});
  for (const char* part : {"file format elf32-mos", "Sections:", "SYMBOL TABLE:",
                           "RELOCATION RECORDS FOR", "R_MOS_ADDR16", "@zp-link-meta-begin"}) {
    INFO(part);
    CHECK(suite::contains(o.out, part));
  }
  REQUIRE(run({"-O", "a.c", "b.c", "-o", "prog"}).first == 0);
  o = suite::objdump_main({"c6502-objdump", "-D", "--no-show-raw-insn", "prog"});
  CHECK(suite::contains(o.out, "   SEG   text\n   ORG   $0800\n"));
  CHECK(py_search(R"((?m)^n\tEQU\t\$[0-9A-F]{2}$)", o.out));
}

namespace {

// Returns the path of dasm on the PATH, as `shutil.which("dasm")`
// finds it, or none.
std::optional<std::string> find_dasm() {
  const char* path = std::getenv("PATH");
  if (path == nullptr) return std::nullopt;
#ifdef _WIN32
  const char sep = ';';
  const std::vector<std::string> names = {"dasm.exe", "dasm"};
#else
  const char sep = ':';
  const std::vector<std::string> names = {"dasm"};
#endif
  const std::string dirs = path;
  std::size_t at = 0;
  for (;;) {
    const std::size_t end = dirs.find(sep, at);
    const std::string dir = dirs.substr(at, end == std::string::npos ? std::string::npos : end - at);
    if (!dir.empty()) {
      for (const std::string& name : names) {
        std::error_code ec;
        const fs::path p = fs::path(dir) / name;
        if (fs::is_regular_file(p, ec)) return p.string();
      }
    }
    if (end == std::string::npos) return std::nullopt;
    at = end + 1;
  }
}

// dasm's bytes for `text`, by address, from its output in format 2:
// records of a 2-byte address, a 2-byte length, and the bytes.
std::map<std::int64_t, unsigned char> dasm(const std::string& dasm_path, const std::string& text) {
  const suite::TempDir tmp;
  const std::string src = tmp.file("in.asm");
  const std::string out = tmp.file("out.bin");
  const std::string log = tmp.file("log.txt");
  suite::write_file(src, text);
  std::string command = "\"" + dasm_path + "\" \"" + src + "\" -f2 \"-o" + out + "\" > \"" + log +
                        "\" 2>&1";
#ifdef _WIN32
  // cmd.exe drops the first and last quotes of a command that starts
  // with one.
  command = "\"" + command + "\"";
#endif
  const int rc = std::system(command.c_str());
  REQUIRE_MESSAGE(rc == 0, suite::read_file(log));
  const std::string binary = suite::read_file(out);
  std::map<std::int64_t, unsigned char> image;
  std::size_t pos = 0;
  const auto byte = [&](std::size_t k) { return static_cast<unsigned char>(binary[k]); };
  while (pos + 4 <= binary.size()) {
    const std::int64_t addr = byte(pos) | byte(pos + 1) << 8;
    const std::size_t size = byte(pos + 2) | byte(pos + 3) << 8;
    for (std::size_t k = 0; k < size; ++k) image[addr + k] = byte(pos + 4 + k);
    pos += 4 + size;
  }
  return image;
}

// Returns the bindings of the lines of `text` that
// `^(\S+)\tEQU\t\$([0-9A-F]+)$` matches.
std::map<std::string, std::int64_t> equs(const std::string& text) {
  std::map<std::string, std::int64_t> out;
  for (const std::string& line : suite::lines(text)) {
    const std::size_t tab = line.find("\tEQU\t$");
    if (tab == std::string::npos || tab == 0) continue;
    const std::string name = line.substr(0, tab);
    const std::string hex = line.substr(tab + 6);
    if (name.find_first_of(" \t") != std::string::npos || hex.empty() ||
        hex.find_first_not_of("0123456789ABCDEF") != std::string::npos) {
      continue;
    }
    out[name] = std::stoll(hex, nullptr, 16);
  }
  return out;
}

std::string hex4(std::int64_t v) {
  static const char digits[] = "0123456789ABCDEF";
  std::string out(4, '0');
  for (int k = 3; k >= 0; --k, v >>= 4) out[k] = digits[v & 0xF];
  return out;
}

}  // namespace

TEST_CASE("binutils: the assembler gives dasm's bytes for every example's -S -O text") {
  const auto dasm_path = find_dasm();
  if (!dasm_path) {
    MESSAGE("skipped: dasm isn't on the PATH");
    return;
  }
  std::vector<fs::path> paths;
  for (const auto& entry :
       fs::recursive_directory_iterator(suite::repo_path("golden/O/examples"))) {
    if (entry.is_regular_file() && entry.path().extension() == ".s") {
      paths.push_back(entry.path());
    }
  }
  std::sort(paths.begin(), paths.end());
  CHECK(!paths.empty());
  for (const fs::path& path : paths) {
    const std::string name = path.filename().string();
    INFO(name);
    const std::string text = read_text(path.string());
    const elf::ElfFile obj = assemble(text, name);
    std::map<std::string, std::int64_t> symbols = equs(text);
    std::set<std::string> undefined;
    for (const auto& s : obj.sections) {
      for (const elf::Reloc& r : s->relocs) {
        if (!r.symbol->defined() && !symbols.count(r.symbol->name)) {
          undefined.insert(r.symbol->name);
        }
      }
    }
    std::string source = "   PROCESSOR 6502\n   ORG $0800\n" + text;
    std::int64_t k = 0;
    for (const std::string& n : undefined) {
      const std::int64_t v = 0x3000 + 16 * k++;
      symbols[n] = v;
      source += "\n" + n + "\tEQU\t$" + hex4(v);
    }
    const std::string mine = image(obj, symbols);
    const auto theirs = dasm(*dasm_path, source);
    std::string expected;
    for (std::size_t j = 0; j < mine.size(); ++j) {
      const auto it = theirs.find(0x0800 + static_cast<std::int64_t>(j));
      expected += static_cast<char>(it == theirs.end() ? 0 : it->second);
    }
    CHECK(mine == expected);
  }
}

TEST_CASE("binutils: dasm picks zp,X or abs,X for a read-modify-write as the assembler does") {
  const auto dasm_path = find_dasm();
  if (!dasm_path) {
    MESSAGE("skipped: dasm isn't on the PATH");
    return;
  }
  std::vector<std::string> body;
  for (const std::string& op : kRmwOps) {
    for (const char* name : {"ZP", "FAR"}) body.push_back(op + "   " + name + "+1,X");
  }
  body.push_back("RTS");
  const std::string text = "ZP\tEQU\t$80\nFAR\tEQU\t$1234\n" + fn(body);
  const std::string mine = image(assemble(text), {});
  const auto theirs = dasm(*dasm_path, "   PROCESSOR 6502\n   ORG $0800\n" + text);
  std::string expected;
  for (std::size_t j = 0; j < mine.size(); ++j) {
    const auto it = theirs.find(0x0800 + static_cast<std::int64_t>(j));
    REQUIRE(it != theirs.end());
    expected += static_cast<char>(it->second);
  }
  CHECK(mine == expected);
}

TEST_CASE("binutils: dasm assembles the Zork interpreter's listing into its executable's bytes") {
  const auto dasm_path = find_dasm();
  if (!dasm_path) {
    MESSAGE("skipped: dasm isn't on the PATH");
    return;
  }
  // The units and the link of tests/test_zork_interpreter.py's
  // `_build`, which compiles each unit to its object as the example's
  // README does.
  const std::vector<std::string> units = {"main",  "decode",  "objects", "numbers",
                                          "input", "pages",   "zstring", "output",
                                          "disk",  "prompts", "save"};
  const suite::TempDir tmp;
  std::vector<std::string> argv{"c6502", "-T", suite::repo_path("examples/zork/interpreter.ld")};
  for (const std::string& unit : units) {
    INFO(unit);
    const std::string obj = tmp.file(unit + ".o");
    const auto o = suite::compile_main(
        {"c6502", suite::repo_path("examples/zork/" + unit + ".c"), "-c", "-O", "-o", obj});
    REQUIRE_MESSAGE(o.status == 0, o.err());
    argv.push_back(obj);
  }
  const std::string exe_path = tmp.file("interpreter");
  argv.push_back("-o");
  argv.push_back(exe_path);
  const auto o = suite::compile_main(argv);
  REQUIRE_MESSAGE(o.status == 0, o.err());
  const elf::ElfFile exe = read_elf(exe_path);
  const auto theirs = dasm(*dasm_path, suite::listing(exe_path));
  CHECK(!exe.segments.empty());
  for (const elf::Segment& seg : exe.segments) {
    std::string expected;
    for (std::size_t k = 0; k < seg.data.size(); ++k) {
      const auto it = theirs.find(seg.paddr + static_cast<std::int64_t>(k));
      expected += static_cast<char>(it == theirs.end() ? 0xFF : it->second);
    }
    CHECK(seg.data == expected);
  }
}
