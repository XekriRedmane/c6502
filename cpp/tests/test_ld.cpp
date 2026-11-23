// Tests of src/binutils/ld.*: Python's message for every raise site of
// binutils/ld.py, with the results of the Python functions for the same
// small objects, most of which tools/capture_binutils.py's records of
// the corpus don't reach. `c6502cpp check-binutils` checks the
// functions on every call the corpus and the suite make of them.
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "binutils/assembler.h"
#include "binutils/elf.h"
#include "binutils/ld.h"
#include "doctest.h"
#include "errors.h"

namespace {

using c6502::CompileError;
namespace elf = c6502::binutils::elf;
namespace ld = c6502::binutils::ld;

elf::SymbolPtr sym(std::string name, std::int64_t value = 0, elf::SectionPtr section = nullptr,
                   std::uint16_t shndx = elf::SHN_UNDEF, std::uint8_t type = elf::STT_NOTYPE,
                   std::uint8_t bind = elf::STB_GLOBAL) {
  auto s = std::make_shared<elf::Symbol>();
  s->name = std::move(name);
  s->value = value;
  s->section = std::move(section);
  s->shndx = shndx;
  s->type = type;
  s->bind = bind;
  return s;
}

elf::SymbolPtr absolute(std::string name, std::int64_t value) {
  return sym(std::move(name), value, nullptr, elf::SHN_ABS);
}

elf::SectionPtr text() {
  auto s = std::make_shared<elf::Section>();
  s->name = ".text";
  s->flags = elf::SHF_ALLOC | elf::SHF_EXECINSTR;
  s->data = std::string(4, '\0');
  return s;
}

elf::ElfFile obj_with(std::vector<elf::SymbolPtr> symbols,
                      std::vector<elf::SectionPtr> sections = {}) {
  elf::ElfFile o;
  o.symbols = std::move(symbols);
  o.sections = std::move(sections);
  return o;
}

template <class F>
std::string error_of(F f) {
  try {
    f();
  } catch (const CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
  return "no error";
}

std::string hex(const std::string& bytes) {
  static constexpr char kDigits[] = "0123456789abcdef";
  std::string out;
  for (const char c : bytes) {
    out += kDigits[static_cast<unsigned char>(c) >> 4];
    out += kDigits[static_cast<unsigned char>(c) & 15];
  }
  return out;
}

// Returns the bytes of a 4-byte `.text` section placed at `addr` with
// one relocation of `type` at `offset` to a symbol `name` the linker
// gives the value `target`, or the error, as the Python `relocate`
// gives them.
std::string relocated(std::uint8_t type, std::int64_t target, std::int64_t addend = 0,
                      std::int64_t offset = 1, std::int64_t addr = 0x800, std::string name = "t",
                      std::int64_t size = 4) {
  elf::SectionPtr s = text();
  elf::SymbolPtr t = sym(name);
  s->relocs.push_back(elf::Reloc{offset, type, t, addend});
  const ld::Objects objects{{"dir/u.o", obj_with({t}, {s})}};
  const ld::Placed p{0, s, addr, size};
  ld::Names extra;
  extra.insert_or_assign(name, target);
  const ld::Resolver resolver(objects, {p}, extra);
  try {
    return hex(ld::relocate(p, resolver, "dir/u.o"));
  } catch (const CompileError& e) {
    return e.python_type() + ": " + e.what();
  }
}

}  // namespace

TEST_CASE("global_definitions gives Python's messages") {
  const auto definitions = [](elf::ElfFile a, elf::ElfFile b) {
    return error_of(
        [&] { ld::global_definitions({{"a.o", std::move(a)}, {"b.o", std::move(b)}}); });
  };
  CHECK(definitions(obj_with({absolute("X", 0x1234)}), obj_with({absolute("X", 0x1235)})) ==
        "LinkError: b.o: 'X' is at $1235 here and at $1234 in a.o");
  CHECK(definitions(obj_with({absolute("X", -5)}), obj_with({absolute("X", 0x10)})) ==
        "LinkError: b.o: 'X' is at $0010 here and at $-005 in a.o");
  const elf::SectionPtr s1 = text();
  const elf::SectionPtr s2 = text();
  CHECK(definitions(obj_with({sym("f", 0, s1, 0, elf::STT_FUNC)}, {s1}),
                    obj_with({sym("f", 0, s2, 0, elf::STT_FUNC)}, {s2})) ==
        "LinkError: `f` is defined in multiple TUs: a.o and b.o");
  CHECK(definitions(obj_with({absolute("X", 0x1234)}),
                    obj_with({sym("X", 0, s2, 0, elf::STT_FUNC)}, {s2})) ==
        "LinkError: `X` is an address in a.o and is defined in b.o");
  // The same absolute value twice is one name.
  const ld::Definitions d = ld::global_definitions(
      {{"a.o", obj_with({absolute("X", 7)})}, {"b.o", obj_with({absolute("X", 7)})}});
  CHECK(d.absolute.size() == 1);
  CHECK(d.defs.size() == 0);
}

TEST_CASE("the Resolver gives Python's values and messages") {
  const elf::SectionPtr s1 = text();
  const elf::SymbolPtr f = sym("f", 2, s1, 0, elf::STT_FUNC);
  const elf::SymbolPtr g = sym("g");
  const elf::SymbolPtr m = sym("mul16");
  const ld::Objects objects{{"a.o", obj_with({f, g, m}, {s1})}};
  const ld::Resolver none(objects, {}, {});
  CHECK(error_of([&] { none.value(*f, 0); }) ==
        "LinkError: a.o: `f` is in a section the link left out");
  CHECK(error_of([&] { none.value(*g, 0); }) == "LinkError: a.o: undefined symbol `g`");
  CHECK(error_of([&] { none.value(*m, 0); }) ==
        "LinkError: a.o: undefined symbol `mul16` (a runtime helper that has no 6502 code in the "
        "runtime library yet, so give its address with --defsym)");
  ld::Names extra;
  extra.insert_or_assign("g", 7);
  const ld::Resolver placed(objects, {ld::Placed{0, s1, 0x800, 4}}, extra);
  CHECK(placed.value(*f, 0) == 2050);
  CHECK(placed.value(*g, 0) == 7);
}

TEST_CASE("relocate gives Python's bytes and messages") {
  CHECK(relocated(elf::R_MOS_ADDR16, 0x10000) ==
        "LinkError: dir/u.o: .text+$0001: address $10000 of `t` is out of range");
  CHECK(relocated(elf::R_MOS_ADDR16, 0, -1) ==
        "LinkError: dir/u.o: .text+$0001: address $-1 of `t` is out of range");
  CHECK(relocated(elf::R_MOS_ADDR16, 0x1234) == "00341200");
  CHECK(relocated(elf::R_MOS_ADDR8, 0x100) ==
        "LinkError: dir/u.o: .text+$0001: `t` is at $0100, and this instruction was assembled with "
        "zero-page addressing for it");
  CHECK(relocated(elf::R_MOS_ADDR8, 0x1FF, 0, 1, 0x800, "__zpabi_f_p0") ==
        "LinkError: dir/u.o: .text+$0001: `__zpabi_f_p0` is at $01FF, and this instruction was "
        "assembled with zero-page addressing for it, since it was in zero page when its unit was "
        "compiled");
  CHECK(relocated(elf::R_MOS_ADDR8, -3, 0, 1, 0x800, "__local_f__t") ==
        "LinkError: dir/u.o: .text+$0001: `__local_f__t` is at $-003, and this instruction was "
        "assembled with zero-page addressing for it, since it was in zero page when its unit was "
        "compiled");
  CHECK(relocated(elf::R_MOS_IMM8, 300, 2) ==
        "LinkError: dir/u.o: .text+$0001: `t` + 2 = 302 doesn't fit in a byte");
  CHECK(relocated(elf::R_MOS_IMM8, -129) ==
        "LinkError: dir/u.o: .text+$0001: `t` + 0 = -129 doesn't fit in a byte");
  CHECK(relocated(elf::R_MOS_IMM8, -128) == "00800000");
  CHECK(relocated(elf::R_MOS_ADDR16_LO, 0x1234) == "00340000");
  CHECK(relocated(elf::R_MOS_ADDR16_HI, 0x1234) == "00120000");
  CHECK(relocated(elf::R_MOS_PCREL_8, 0x900, -1) ==
        "LinkError: dir/u.o: .text+$0001: the branch to `t` is 254 bytes away, out of reach");
  CHECK(relocated(elf::R_MOS_PCREL_8, 0x700, -1) ==
        "LinkError: dir/u.o: .text+$0001: the branch to `t` is -258 bytes away, out of reach");
  CHECK(relocated(elf::R_MOS_PCREL_8, 0x810, -1) == "000e0000");
  CHECK(relocated(9, 0x10) == "LinkError: dir/u.o: .text+$0001: unknown relocation type 9");
  // A relocation in the `JMP` the layout dropped isn't applied.
  CHECK(relocated(elf::R_MOS_ADDR16, 0x10000, 0, 3, 0x800, "t", 3) == "000000");
}

TEST_CASE("relax shortens an absolute operand the linker put in zero page") {
  const elf::ElfFile t = c6502::binutils::assembler::assemble(
      "f:\n    SUBROUTINE\n    LDA g\n    BNE .x\n    STA g,X\n    JMP f\n.x:\n    RTS\n", "r.s");
  elf::SectionPtr sec;
  for (const auto& s : t.sections) {
    if (s->name == ".text") sec = s;
  }
  REQUIRE(sec.get() != nullptr);
  CHECK(hex(sec->data) == "ad0000d0069d00004c000060");
  const std::int64_t saved = ld::relax(sec, t.symbols, [](const elf::SymbolPtr& s) {
    return s->name == "g" ? std::optional<std::int64_t>(0x42) : std::nullopt;
  });
  CHECK(saved == 2);
  CHECK(hex(sec->data) == "a500d00595004c000060");
  std::vector<std::tuple<std::int64_t, int, std::string>> relocs;
  for (const auto& r : sec->relocs) relocs.emplace_back(r.offset, r.type, r.symbol->name);
  CHECK(relocs == std::vector<std::tuple<std::int64_t, int, std::string>>{
                      {1, 2, "g"}, {5, 2, "g"}, {7, 3, "f"}});
  std::vector<std::tuple<std::string, std::int64_t, std::int64_t>> own;
  for (const auto& s : t.symbols) {
    if (s->section == sec) own.emplace_back(s->name, s->value, s->size);
  }
  CHECK(own == std::vector<std::tuple<std::string, std::int64_t, std::int64_t>>{{"f", 0, 10},
                                                                                {".x", 9, 0}});
}
