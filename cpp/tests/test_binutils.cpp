// Tests of the ELF reader and writer, the opcode table, argparse, and
// nm (src/binutils/, src/argparse.h). `c6502cpp check-binutils` checks
// them on every file and command line tools/capture_binutils.py records,
// and tests/test_cpp_binutils.py compares nm.py with and without the C++
// port at several terminal widths.
#include <ostream>
#include <string>
#include <vector>

#include "argparse.h"
#include "binutils/elf.h"
#include "binutils/nm.h"
#include "binutils/opcodes.h"
#include "doctest.h"
#include "errors.h"

namespace {

using namespace c6502::binutils;
using c6502::CompileError;

// An object with a function in `.text` that calls an undefined name,
// a local label, a `.bss` object, and the link metadata.
elf::ElfFile sample() {
  elf::ElfFile f;
  auto text = std::make_shared<elf::Section>();
  text->name = ".text";
  text->flags = elf::SHF_ALLOC | elf::SHF_EXECINSTR;
  text->data = std::string("\x20\x00\x00\x60", 4);
  auto bss = std::make_shared<elf::Section>();
  bss->name = ".bss";
  bss->type = elf::SHT_NOBITS;
  bss->flags = elf::SHF_ALLOC | elf::SHF_WRITE;
  bss->nobits_size = 3;
  auto meta = std::make_shared<elf::Section>();
  meta->name = std::string(elf::LINK_SECTION);
  meta->flags = 0;
  meta->data = "; @zp-link-meta-begin\n; @zp-link-meta-end\n";
  f.sections = {text, bss, meta};
  auto file = std::make_shared<elf::Symbol>(
      elf::Symbol{.name = "t.c", .type = elf::STT_FILE, .shndx = elf::SHN_ABS});
  auto fn = std::make_shared<elf::Symbol>(elf::Symbol{
      .name = "f", .size = 4, .bind = elf::STB_GLOBAL, .type = elf::STT_FUNC, .section = text});
  auto label = std::make_shared<elf::Symbol>(elf::Symbol{.name = ".l@1", .value = 3, .section = text});
  auto buf = std::make_shared<elf::Symbol>(elf::Symbol{
      .name = "buf", .size = 3, .type = elf::STT_OBJECT, .section = bss});
  auto ext = std::make_shared<elf::Symbol>(
      elf::Symbol{.name = "g", .bind = elf::STB_GLOBAL, .other = elf::STO_MOS_ZEROPAGE});
  f.symbols = {file, fn, label, buf, ext};
  text->relocs.push_back(elf::Reloc{.offset = 1, .type = elf::R_MOS_ADDR16, .symbol = ext, .addend = -2});
  return f;
}

}  // namespace

TEST_CASE("elf: an object read back gives the same file and the same bytes") {
  const std::string data = elf::write(sample());
  CHECK(data.substr(0, 4) == "\x7f" "ELF");
  const elf::ElfFile back = elf::read(data, "t.o");
  CHECK(elf::write(back) == data);
  REQUIRE(back.sections.size() == 3);
  CHECK(back.sections[1]->size() == 3);
  REQUIRE(back.symbols.size() == 5);
  // Local symbols come first.
  CHECK(back.symbols[0]->name == "t.c");
  CHECK(back.symbols[2]->name == "buf");
  CHECK(back.symbols[3]->name == "f");
  const auto& r = back.sections[0]->relocs.at(0);
  CHECK(r.symbol->name == "g");
  CHECK(r.addend == -2);
  CHECK(r.symbol.get() == back.symbol("g").get());
  CHECK(back.link_text().value().starts_with("; @zp-link-meta-begin"));
  CHECK(!back.source_text().has_value());
}

TEST_CASE("elf: a damaged file is an ElfError") {
  const std::string data = elf::write(sample());
  const auto message = [](const std::string& bytes) {
    try {
      elf::read(bytes, "x.o");
    } catch (const CompileError& e) {
      CHECK(e.python_type() == "ElfError");
      return std::string(e.what());
    }
    return std::string("read");
  };
  CHECK(message("not an ELF file") == "x.o: not an ELF file");
  std::string wide = data;
  wide[4] = 2;
  CHECK(message(wide) == "x.o: not a little-endian 32-bit ELF file");
  std::string machine = data;
  machine[18] = 62;
  machine[19] = 0;
  CHECK(message(machine) == "x.o: ELF file for machine 62, not the 6502 (6502)");
  const std::string damaged = "x.o: the ELF file is truncated or damaged";
  CHECK(message(data.substr(0, data.size() / 2)) == damaged);
  CHECK(message(data.substr(0, data.size() - 1)) == damaged);
  std::string names = data;
  names[50] = 0x7F;
  CHECK(message(names) == damaged);
  std::string meta = data;
  meta[meta.find("; @zp")] = '\xff';
  CHECK(message(meta) == damaged);
}

TEST_CASE("elf: a relocation of a symbol outside the table can't be written") {
  elf::ElfFile f = sample();
  f.sections[0]->relocs[0].symbol = std::make_shared<elf::Symbol>(elf::Symbol{.name = "lost"});
  try {
    elf::write(f);
    FAIL("wrote it");
  } catch (const CompileError& e) {
    CHECK(std::string(e.what()) == "a relocation names `lost`, which isn't in the symbol table");
  }
}

TEST_CASE("opcodes: the table and its decoding") {
  CHECK(opcodes::table().size() == 151);
  const auto* lda = opcodes::decode(0xB1);
  REQUIRE(lda != nullptr);
  CHECK(lda->mnemonic == "LDA");
  CHECK(lda->mode == opcodes::Mode::kIndy);
  CHECK(opcodes::decode(0x02) == nullptr);
  CHECK(opcodes::opcode("STX", opcodes::Mode::kZpy) == 0x96);
  CHECK(!opcodes::opcode("STX", opcodes::Mode::kAbsy).has_value());
  CHECK(opcodes::zp_form(opcodes::Mode::kAbsx) == opcodes::Mode::kZpx);
  CHECK(opcodes::size(opcodes::Mode::kInd) == 3);
  CHECK(opcodes::is_mnemonic("BIT"));
  CHECK(!opcodes::is_mnemonic("BRA"));
}

TEST_CASE("argparse: nm's usage, errors, and combined flags") {
  c6502::argparse::Parser ap("nm.py", "List things.");
  ap.add_positional("files", "object or executable files");
  for (const char* name : {"-a", "-g", "-u", "-n", "-p", "-r", "-S", "-A"}) {
    ap.add_store_true({name}, "");
  }
  ap.add_store_true({"--defined-only"}, "only defined symbols");
  const auto exit_of = [&](const std::vector<std::string>& args) {
    try {
      ap.parse_args(args, 80);
    } catch (const c6502::argparse::Exit& e) {
      return e;
    }
    return c6502::argparse::Exit{-1, "", ""};
  };
  const std::string usage =
      "usage: nm.py [-h] [-a] [-g] [-u] [-n] [-p] [-r] [-S] [-A] [--defined-only]\n"
      "             files [files ...]\n";
  CHECK(ap.format_usage(80) == usage);
  CHECK(exit_of({}).err == usage + "nm.py: error: the following arguments are required: files\n");
  CHECK(exit_of({"-ax", "f"}).err ==
        usage + "nm.py: error: argument -a: ignored explicit argument 'x'\n");
  CHECK(exit_of({"f", "-a", "g"}).err == usage + "nm.py: error: unrecognized arguments: g\n");
  CHECK(exit_of({"-h"}).status == 0);
  const c6502::argparse::Namespace ns = ap.parse_args({"-agu", "--", "-h"}, 80);
  CHECK(ns.flag("a"));
  CHECK(ns.flag("g"));
  CHECK(ns.flag("u"));
  CHECK(!ns.flag("n"));
  CHECK(*ns.list("files") == std::vector<std::string>{"-h"});
}

TEST_CASE("nm: the listing of an object") {
  const elf::ElfFile f = sample();
  CHECK(nm::symbol_lines(f, {}) ==
        std::vector<std::string>{"0000 b buf", "0000 T f", "     U g"});
  nm::Options all;
  all.debug = true;
  all.sort = nm::Sort::kNumeric;
  all.size = true;
  CHECK(nm::symbol_lines(f, all) ==
        std::vector<std::string>{std::string(9, ' ') + " Uz g", "0000 0003 b buf",
                                 "0000 0004 T f", "0000 0000 a t.c", "0003 0000 t .l@1"});
}

TEST_CASE("nm: a damaged file is named as the command line gives it") {
  // `E.read_file` passes `str(path)` to `read`, which is the argument
  // itself, so `./x.o` keeps its `./` and its `/` on Windows.
  const std::string data = elf::write(sample());
  const nm::ReadFile half = [&](const std::string&) { return data.substr(0, data.size() / 2); };
  std::string out;
  std::string err;
  CHECK(nm::main({"nm.py", "./x.o"}, half, 80, out, err) == 1);
  CHECK(err == "nm.py: ./x.o: the ELF file is truncated or damaged\n");
}
