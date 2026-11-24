// Tests of the ELF reader and writer, the opcode table, argparse, nm,
// the disassembler, and objdump (src/binutils/, src/argparse.h).
// `c6502cpp check-binutils` checks them on every file and command line
// tools/capture_binutils.py records, and tests/test_cpp_binutils.py
// compares nm.py and objdump.py with and without the C++ port at
// several terminal widths.
#include <ostream>
#include <string>
#include <vector>

#include "argparse.h"
#include "binutils/assemble_command.h"
#include "binutils/disasm.h"
#include "binutils/elf.h"
#include "binutils/nm.h"
#include "binutils/objdump.h"
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

TEST_CASE("elf: a relocation past the end of its section's bytes is an ElfError") {
  // Python's message, from tests/test_binutils.py's
  // `test_relocation_past_its_section`.
  for (const auto& [type, offset] : std::vector<std::pair<std::uint8_t, std::int64_t>>{
           {elf::R_MOS_ADDR8, 9}, {elf::R_MOS_ADDR16, 3}, {elf::R_MOS_ADDR8, 4}}) {
    elf::ElfFile f = sample();
    f.sections[0]->relocs[0].type = type;
    f.sections[0]->relocs[0].offset = offset;
    const std::string data = elf::write(f);
    std::string message;
    try {
      elf::read(data, "x.o");
    } catch (const CompileError& e) {
      message = e.python_type() + ": " + e.what();
    }
    CHECK(message == "ElfError: x.o: a relocation in .text is past the end of its bytes");
  }
  elf::ElfFile f = sample();
  f.sections[0]->relocs[0].offset = 2;
  CHECK(elf::read(elf::write(f), "x.o").sections[0]->relocs.size() == 1);
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

TEST_CASE("elf: the fields elf.write packs in 32 unsigned bits") {
  // A link from the origin -1 (`compile.py -Ttext=-1`) puts `.text` at
  // -1 and enters there, which Python's `_SHDR.pack` and `_EHDR.pack`
  // reject with struct.error. The C++ writer would keep the low bits.
  const auto raises = [](const elf::ElfFile& f) {
    try {
      check_write_fields(f);
    } catch (const CompileError& e) {
      return e.python_type() == "struct.error" && std::string(e.what()) == "argument out of range";
    }
    return false;
  };
  elf::ElfFile f = sample();
  CHECK(!raises(f));
  f.sections[0]->addr = -1;
  CHECK(raises(f));
  f.sections[0]->addr = 0x800;
  f.entry = 0x100000000;
  CHECK(raises(f));
  f.entry = 0x800;
  f.segments.push_back(elf::Segment{.vaddr = -1, .paddr = -1, .data = "x", .memsz = 1});
  CHECK(raises(f));
  f.segments.back().vaddr = f.segments.back().paddr = 0xFFFF;
  CHECK(!raises(f));
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

namespace {

// An executable whose code section has a non-ASCII name and is loaded
// at $C000 to run at $0800, with a `$d` mapping symbol, a `.bss`
// object, and absolute symbols, one of them twice.
elf::ElfFile sample_exe() {
  const std::string code("\xD0\xFE\xAD\x80\x00\xAD\x01\x03\x6C\x10\x00\x02\xA9\x07", 14);
  elf::ElfFile f;
  f.type = elf::ET_EXEC;
  f.entry = 0x0800;
  auto text = std::make_shared<elf::Section>();
  text->name = ".t\xC3\xA9xt";
  text->flags = elf::SHF_ALLOC | elf::SHF_EXECINSTR;
  text->data = code;
  text->addr = 0x0800;
  text->align = 4;
  auto bss = std::make_shared<elf::Section>();
  bss->name = ".bss";
  bss->type = elf::SHT_NOBITS;
  bss->flags = elf::SHF_ALLOC | elf::SHF_WRITE;
  bss->nobits_size = 4;
  bss->addr = 0x0300;
  f.sections = {text, bss};
  const auto sym = [](elf::Symbol s) { return std::make_shared<elf::Symbol>(std::move(s)); };
  f.symbols = {
      sym({.name = "main", .value = 0x0800, .size = 14, .bind = elf::STB_GLOBAL,
           .type = elf::STT_FUNC, .section = text}),
      sym({.name = "$d", .value = 0x080C, .section = text}),
      sym({.name = "buf", .value = 0x0300, .size = 4, .type = elf::STT_OBJECT, .section = bss}),
      sym({.name = "ROM", .value = 0xE000, .shndx = elf::SHN_ABS}),
      sym({.name = "IO", .value = 0xC0, .shndx = elf::SHN_ABS}),
      sym({.name = "IO", .value = 0xC0, .bind = elf::STB_GLOBAL, .shndx = elf::SHN_ABS}),
      sym({.name = "__text_size", .value = 14, .shndx = elf::SHN_ABS}),
  };
  f.segments.push_back(elf::Segment{.vaddr = 0x0800, .paddr = 0xC000, .data = code, .memsz = 14});
  return f;
}

}  // namespace

// The expected listings below are Python's for the same files.

TEST_CASE("disasm: Python's formatting of numbers and padding") {
  CHECK(disasm::format_hex(0x80, 4) == "0080");
  CHECK(disasm::format_hex(-126, 4) == "-07E");
  CHECK(disasm::format_hex(0x12345, 4) == "12345");
  CHECK(disasm::format_hex(0xAB, 2, true) == "ab");
  // `ljust` counts code points, so the two bytes of an e with an acute
  // accent are one column.
  CHECK(disasm::ljust("\xC3\xA9", 3) == "\xC3\xA9  ");
  CHECK(disasm::ljust("abcd", 2) == "abcd");
}

TEST_CASE("disasm: an object, with a relocation's symbol in its operand") {
  CHECK(disasm::disassemble(sample()) ==
        std::vector<std::string>{
            "; elf32-mos object", "   PROCESSOR 6502", "", "; @zp-link-meta-begin",
            "; @zp-link-meta-end", "", "; Disassembly of section .text", "f:", "   SUBROUTINE",
            "   JSR   g-2                            ; 0000  20 00 00", ".l@1:",
            "   RTS                                  ; 0003  60"});
  // A branch to before the section's start is written as Python formats
  // a negative number.
  elf::ElfFile back;
  auto text = std::make_shared<elf::Section>();
  text->name = ".text";
  text->flags = elf::SHF_ALLOC | elf::SHF_EXECINSTR;
  text->data = std::string("\xD0\x80", 2);
  back.sections = {text};
  CHECK(disasm::disassemble(back).back() ==
        "   BNE   $-07E                          ; 0000  D0 80");
}

TEST_CASE("disasm: an executable, with its EQUs, SEG, ORG, and RORG") {
  const std::vector<std::string> head{
      "; elf32-mos executable", "   PROCESSOR 6502", "", "__text_size\tEQU\t$0E", "IO\tEQU\t$C0",
      "ROM\tEQU\t$E000", "",
      // The non-ASCII letter is one `_` in the SEG's name.
      "; Disassembly of section .t\xC3\xA9xt at $0800", "   SEG   t_xt", "   ORG   $C000",
      "   RORG  $0800", "main:", "   SUBROUTINE"};
  std::vector<std::string> all = head;
  for (const char* l : {"   BNE   main                           ; 0800  D0 FE",
                        "   LDA.w $0080                          ; 0802  AD 80 00",
                        "   LDA   buf+1                          ; 0805  AD 01 03",
                        "   JMP   ($0010)                        ; 0808  6C 10 00",
                        "   DC.B  $02                            ; 080B  02",
                        "   DC.B  $A9, $07                       ; 080C  A9 07", "   REND", "",
                        "; Disassembly of section .bss at $0300", "   SEG.U bss", "   ORG   $0300",
                        "buf:",
                        // Python joins the address and the empty bytes
                        // with two spaces.
                        "   DS.B  4                              ; 0300  "}) {
    all.push_back(l);
  }
  CHECK(disasm::disassemble(sample_exe(), true) == all);
  std::vector<std::string> bare = head;
  for (const char* l : {"   BNE   main", "   LDA.w $0080", "   LDA   buf+1", "   JMP   ($0010)",
                        "   DC.B  $02", "   DC.B  $A9, $07", "   REND"}) {
    bare.push_back(l);
  }
  CHECK(disasm::disassemble(sample_exe(), false, nullptr, {false, false}) == bare);
}

TEST_CASE("objdump: the headers, symbols, relocations, and contents") {
  CHECK(objdump::section_headers(sample()) ==
        std::vector<std::string>{
            "Sections:", "Idx Name                 Size  VMA   LMA   Algn",
            "  0 .text                0004  0000  0000  2**0",
            "                         CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE",
            "  1 .bss                 0003  0000  0000  2**0", "                         ALLOC, DATA",
            "  2 .c6502.link          002A  0000  0000  2**0", "                         CONTENTS"});
  CHECK(objdump::symbol_table(sample()) ==
        std::vector<std::string>{"SYMBOL TABLE:", "0000 l f  *ABS*        0000 t.c",
                                 "0000 g F  .text        0004 f", "0003 l    .text        0000 .l@1",
                                 "0000 l O  .bss         0003 buf", "0000 g    *UND*        0000 g"});
  CHECK(objdump::relocations(sample(), nullptr) ==
        std::vector<std::string>{"RELOCATION RECORDS FOR [.text] (f):",
                                 "OFFSET TYPE              VALUE", "0001   R_MOS_ADDR16      g-2", ""});
  CHECK(objdump::full_contents(sample(), nullptr) ==
        std::vector<std::string>{
            "Contents of section .text:", " 0000 20000060                              ..`",
            "Contents of section .c6502.link:",
            " 0000 3b20407a 702d6c69 6e6b2d6d 6574612d  ; @zp-link-meta-",
            " 0010 62656769 6e0a3b20 407a702d 6c696e6b  begin.; @zp-link",
            " 0020 2d6d6574 612d656e 640a               -meta-end."});
  // The name is padded by code points, and an alignment of 4 is 2**2.
  CHECK(objdump::section_headers(sample_exe()) ==
        std::vector<std::string>{"Sections:", "Idx Name                 Size  VMA   LMA   Algn",
                                 "  0 .t\xC3\xA9xt                000E  0800  C000  2**2",
                                 "                         CONTENTS, ALLOC, LOAD, READONLY, CODE",
                                 "  1 .bss                 0004  0300  0300  2**0",
                                 "                         ALLOC, DATA"});
  CHECK(objdump::file_header(sample_exe(), "x/e") ==
        std::vector<std::string>{"x/e:     file format elf32-mos",
                                 "architecture: mos6502, flags 0x00000001:", "executable",
                                 "start address 0x0800"});
  CHECK(objdump::full_contents(sample_exe(), nullptr) ==
        std::vector<std::string>{"Contents of section .t\xC3\xA9xt:",
                                 " 0800 d0fead80 00ad0103 6c100002 a907      ........l....."});
}

TEST_CASE("objdump: a command line that asks for nothing returns 2") {
  std::string out;
  std::string err;
  const nm::ReadFile none = [](const std::string&) -> std::string { throw nm::FileError{"x"}; };
  CHECK(objdump::main({"objdump.py", "f.o"}, none, 80, out, err) == 2);
  CHECK(out.empty());
  CHECK(err == "objdump.py: give at least one of -d -D -f -h -t -r -s -x --link-metadata\n");
  err.clear();
  CHECK(objdump::main({"objdump.py", "-d", "f.o"}, none, 80, out, err) == 1);
  CHECK(err == "objdump.py: x\n");
  // A damaged file is named as the command line gives it, as nm names
  // one.
  const std::string data = elf::write(sample());
  const nm::ReadFile half = [&](const std::string&) { return data.substr(0, data.size() / 2); };
  err.clear();
  CHECK(objdump::main({"objdump.py", "-d", "./x.o"}, half, 80, out, err) == 1);
  CHECK(err == "objdump.py: ./x.o: the ELF file is truncated or damaged\n");
}
