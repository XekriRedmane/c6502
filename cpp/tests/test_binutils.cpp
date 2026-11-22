// Tests of the ELF reader and writer and the opcode table
// (src/binutils/). `c6502cpp check-binutils` checks them on every file
// tools/capture_binutils.py records.
#include <ostream>
#include <string>
#include <vector>

#include "binutils/elf.h"
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
