// tests/test_binutils.py's tests of the assembler and of relaxation,
// ported by hand, one case per Python test with each of its assertions.
// `image` is the Python module's `_image`: an object's loaded sections
// one after another from $0800 with their relocations applied. Where the
// Python test matched an AssemblerError's message with a regular
// expression, the case checks the whole message Python gives.
#include <cstdint>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "binutils/assembler.h"
#include "binutils/disasm.h"
#include "binutils/elf.h"
#include "binutils/ld.h"
#include "doctest.h"
#include "errors.h"

namespace {

namespace elf = c6502::binutils::elf;
namespace ld = c6502::binutils::ld;
using c6502::binutils::assembler::assemble;
using Bytes = std::vector<int>;

// Returns the text of function `name` with `body`, each line indented
// unless it is a label.
std::string fn(const std::vector<std::string>& body, const std::string& name = "f") {
  std::string out = name + ":\n   SUBROUTINE\n";
  for (const std::string& b : body) {
    out += (b.ends_with(":") ? b : "   " + b) + "\n";
  }
  return out;
}

Bytes bytes_of(const std::string& s) {
  Bytes out;
  for (const char c : s) out.push_back(static_cast<unsigned char>(c));
  return out;
}

// `obj`'s loaded sections one after another from $0800, with their
// relocations applied and the names the object leaves undefined given
// by `symbols` (the Python test's `_image`).
Bytes image(const elf::ElfFile& obj, const std::map<std::string, std::int64_t>& symbols = {}) {
  std::vector<ld::Placed> placed;
  std::int64_t addr = 0x0800;
  for (const auto& s : obj.sections) {
    if ((s->flags & elf::SHF_ALLOC) != 0) {
      placed.push_back(ld::Placed{0, s, addr, s->size()});
      addr += s->size();
    }
  }
  const ld::Objects objects{{"t.asm", obj}};
  ld::Names extra;
  for (const auto& [name, value] : symbols) extra.insert_or_assign(name, value);
  const ld::Resolver resolver(objects, placed, extra);
  Bytes out;
  for (const auto& p : placed) {
    if (p.section->type == elf::SHT_NOBITS) {
      out.insert(out.end(), static_cast<std::size_t>(p.size), 0);
    } else {
      const Bytes b = bytes_of(ld::relocate(p, resolver, "t.asm"));
      out.insert(out.end(), b.begin(), b.end());
    }
  }
  return out;
}

Bytes first(const Bytes& b, std::size_t n) { return Bytes(b.begin(), b.begin() + n); }

// The read-modify-write instructions with their zp,X and abs,X opcodes,
// from the MCS6500 manual's instruction summary.
struct Rmw {
  const char* op;
  int zpx;
  int absx;
};
const Rmw kRmw[] = {{"INC", 0xF6, 0xFE}, {"DEC", 0xD6, 0xDE}, {"ASL", 0x16, 0x1E},
                    {"LSR", 0x56, 0x5E}, {"ROL", 0x36, 0x3E}, {"ROR", 0x76, 0x7E}};

const char* const kEqus = "ZP\tEQU\t$80\nFAR\tEQU\t$1234\n";

}  // namespace

TEST_CASE("binutils: TestAssembler::test_zero_page_chosen_by_value") {
  const std::string text =
      kEqus +
      fn({"LDA   ZP", "LDA   FAR", "LDA   $10,X", "STX   ZP,Y", "LDA   ZP,Y", "LDA   later",
          "LDA   (ZP),Y", "RTS"}) +
      "later:\n   DC.B  $00\n";
  const elf::ElfFile obj = assemble(text);
  CHECK(first(image(obj), 18) == Bytes{
                                     0xA5, 0x80,        // LDA zp
                                     0xAD, 0x34, 0x12,  // LDA abs
                                     0xB5, 0x10,        // LDA zp,X
                                     0x96, 0x80,        // STX zp,Y
                                     0xB9, 0x80, 0x00,  // LDA abs,Y: LDA has no zp,Y
                                     0xAD, 0x12, 0x08,  // a label is never in zero page
                                     0xB1, 0x80,        // LDA (zp),Y
                                     0x60,
                                 });
  std::map<std::pair<std::int64_t, std::string>, int> relocs;
  for (const auto& r : obj.sections[0]->relocs) relocs[{r.offset, r.symbol->name}] = r.type;
  CHECK(relocs.at({1, "ZP"}) == elf::R_MOS_ADDR8);
  CHECK(relocs.at({3, "FAR"}) == elf::R_MOS_ADDR16);
  CHECK(relocs.at({13, "later"}) == elf::R_MOS_ADDR16);
  const auto far = obj.symbol("FAR");
  REQUIRE(far.get() != nullptr);
  CHECK(far->is_absolute());
  CHECK(far->value == 0x1234);
}

TEST_CASE("binutils: TestAssembler::test_read_modify_write_of_an_element") {
  // `INC t,X` and the rest take zp,X for an address in zero page and
  // abs,X otherwise, and the disassembler reads them back.
  std::vector<std::string> body;
  for (const auto& r : kRmw) {
    for (const char* name : {"ZP", "FAR"}) body.push_back(std::string(r.op) + "   " + name + ",X");
  }
  body.push_back("RTS");
  const elf::ElfFile obj = assemble(kEqus + fn(body));
  Bytes expected;
  for (const auto& r : kRmw) {
    expected.insert(expected.end(), {r.zpx, 0x80, r.absx, 0x34, 0x12});
  }
  expected.push_back(0x60);
  CHECK(image(obj) == expected);
  std::string listing;
  for (const auto& line : c6502::binutils::disasm::disassemble(obj)) {
    if (!listing.empty()) listing += "\n";
    listing += line;
  }
  const auto hex2 = [](int v) {
    static constexpr char kDigits[] = "0123456789ABCDEF";
    return std::string{kDigits[v >> 4], kDigits[v & 15]};
  };
  for (const auto& r : kRmw) {
    CAPTURE(r.op);
    CHECK(std::regex_search(listing, std::regex(std::string(r.op) + "   ZP,X +; [0-9A-F]{4}  " +
                                                hex2(r.zpx) + " ")));
    CHECK(std::regex_search(listing, std::regex(std::string(r.op) + "   FAR,X +; [0-9A-F]{4}  " +
                                                hex2(r.absx) + " ")));
  }
}

TEST_CASE("binutils: TestAssembler::test_linker_placed_names_stay_undefined") {
  // A slot, a runtime name, and a zero-page object take their addressing
  // mode from the unit's value, and the linker gives them their
  // addresses.
  const std::string text = std::string(
                               "HARGS\tEQU\t$04\n__local_f__x\tEQU\t$90\n"
                               "; @zp-link-meta-begin\n"
                               "; zp obj size=1 at=auto def=true init=$00\n"
                               "; @zp-link-meta-end\n"
                               "obj\tEQU\t$30\n") +
                           fn({"LDA   HARGS+2", "STA   __local_f__x", "INC   obj", "LDA   SSP",
                               "RTS"});
  const elf::ElfFile obj = assemble(text);
  const Bytes code = bytes_of(obj.sections[0]->data);
  REQUIRE(code.size() >= 7);
  CHECK(Bytes{code[0], code[2], code[4], code[6]} == Bytes{0xA5, 0x85, 0xE6, 0xA5});
  for (const char* name : {"HARGS", "__local_f__x", "obj", "SSP"}) {
    CAPTURE(name);
    const auto sym = obj.symbol(name);
    REQUIRE(sym.get() != nullptr);
    CHECK_FALSE(sym->defined());
    CHECK((sym->other & elf::STO_MOS_ZEROPAGE) != 0);
  }
}

TEST_CASE("binutils: TestAssembler::test_immediates_and_data") {
  const std::string text =
      fn({"LDA   #<(buf+2)", "LDX   #>buf", "LDY   #$FF", "LDA   #-1", "JMP   .t", ".t:",
          "DC.W  .t-1", "DC.B  <buf, $05, \"hi\"", "RTS"}) +
      "buf:\n   DS.B  4\n";
  const elf::ElfFile obj = assemble(text);
  const auto& sec = obj.sections[0];
  std::map<std::int64_t, std::tuple<int, std::string, std::int64_t>> r;
  for (const auto& x : sec->relocs) r[x.offset] = {x.type, x.symbol->name, x.addend};
  using T = std::tuple<int, std::string, std::int64_t>;
  CHECK(r.at(1) == T{elf::R_MOS_ADDR16_LO, "buf", 2});
  CHECK(r.at(3) == T{elf::R_MOS_ADDR16_HI, "buf", 0});
  CHECK(r.at(9) == T{elf::R_MOS_ADDR16, ".t", 0});
  CHECK(r.at(11) == T{elf::R_MOS_ADDR16, ".t", -1});
  CHECK(r.at(13) == T{elf::R_MOS_ADDR16_LO, "buf", 0});
  const Bytes data = bytes_of(sec->data);
  CHECK(Bytes(data.begin() + 4, data.begin() + 8) == Bytes{0xA0, 0xFF, 0xA9, 0xFF});
  CHECK(Bytes(data.begin() + 14, data.begin() + 17) == Bytes{0x05, 'h', 'i'});
  // The table is data between `$d` and `$c`.
  std::set<std::pair<std::int64_t, std::string>> maps;
  for (const auto& s : obj.symbols) {
    if (s->name == "$c" || s->name == "$d") maps.emplace(s->value, s->name);
  }
  CHECK(maps == std::set<std::pair<std::int64_t, std::string>>{{11, "$d"}, {17, "$c"}});
  const auto& bss = obj.sections[1];
  CHECK(bss->name == ".bss");
  CHECK(bss->type == elf::SHT_NOBITS);
  CHECK(bss->size() == 4);
}

TEST_CASE("binutils: TestAssembler::test_branches_and_local_labels") {
  // Each function has its own `.loop`.
  const elf::ElfFile obj = assemble(fn({".loop:", "DEX", "BNE   .loop", "BEQ   g", "RTS"}) +
                                    fn({".loop:", "BNE   .loop", "RTS"}, "g"));
  std::vector<elf::SectionPtr> alloc;
  for (const auto& s : obj.sections) {
    if ((s->flags & elf::SHF_ALLOC) != 0) alloc.push_back(s);
  }
  REQUIRE(alloc.size() == 2);
  const auto& f = alloc[0];
  CHECK(first(bytes_of(f->data), 3) == Bytes{0xCA, 0xD0, 0xFD});
  // A branch to another item is relocated.
  REQUIRE(f->relocs.size() == 1);
  const auto& r = f->relocs[0];
  CHECK(r.offset == 4);
  CHECK(r.type == elf::R_MOS_PCREL_8);
  CHECK(r.symbol->name == "g");
  CHECK(r.addend == -1);
  const Bytes img = image(obj);
  CHECK(Bytes(img.begin() + 3, img.begin() + 5) == Bytes{0xF0, 0x01});
}

TEST_CASE("binutils: TestAssembler::test_errors") {
  std::vector<std::string> nops = {".a:"};
  for (int k = 0; k < 130; ++k) nops.push_back("NOP");
  nops.push_back("BNE   .a");
  const std::vector<std::pair<std::string, std::string>> cases = {
      {fn({"FROB"}), "<input>:3: unknown instruction or directive FROB"},
      {fn({"LDA   ($1234),Y"}), "<input>:3: $1234 isn't in zero page"},
      {fn({"BNE   .nowhere"}), "<input>:3: .nowhere isn't a label in this function"},
      {fn(nops), "<input>:134: branch to .a is -132 bytes away, out of reach"},
      {"   LDA   #$00\n", "<input>:1: LDA comes before any label"},
      {fn({"LDA   #$1FF"}), "<input>:3: 511 doesn't fit in 1 byte(s)"},
      {fn({"STA   #$01"}), "<input>:3: STA has no imm addressing mode"},
      // This raised UnicodeEncodeError, a traceback from compile.py.
      {"x:\n   DC.B  \"\xE2\x82\xAC\"\n",
       "<input>:2: \"\xE2\x82\xAC\" holds a character outside Latin-1, which DC.B can't store in "
       "a byte"},
  };
  for (const auto& [text, message] : cases) {
    CAPTURE(message);
    std::string got = "no error";
    try {
      assemble(text);
    } catch (const c6502::CompileError& e) {
      got = e.python_type() + ": " + e.what();
    }
    CHECK(got == "AssemblerError: " + message);
  }
}

TEST_CASE("binutils: TestAssembler::test_linker_values_override_the_unit") {
  // The linker assembles a unit again with its own addresses: a slot
  // that left zero page gets absolute addressing.
  const std::string text = "__local_f__x\tEQU\t$90\n" + fn({"LDA   __local_f__x", "RTS"});
  CHECK(static_cast<unsigned char>(assemble(text).sections[0]->data[0]) == 0xA5);
  const c6502::binutils::assembler::LinkSymbols far = {{"__local_f__x", 0x0300}};
  CHECK(static_cast<unsigned char>(assemble(text, "<input>", &far).sections[0]->data[0]) == 0xAD);
  const c6502::binutils::assembler::LinkSymbols none = {{"__local_f__x", std::nullopt}};
  CHECK(static_cast<unsigned char>(assemble(text, "<input>", &none).sections[0]->data[0]) ==
        0xAD);
}

TEST_CASE("binutils: TestRelax::test_absolute_reference_to_zero_page_shortened") {
  const elf::ElfFile obj = assemble(
      fn({"LDA   ext", "BNE   .x", "STA   ext+1,X", "JMP   .x", ".x:", "LDA   far", "RTS"}));
  const std::map<std::string, std::int64_t> known = {{"ext", 0x42}, {"far", 0x1234}};
  const std::int64_t saved =
      ld::relax(obj.sections[0], obj.symbols, [&](const elf::SymbolPtr& s) {
        const auto it = known.find(s->name);
        return it == known.end() ? std::nullopt : std::optional<std::int64_t>(it->second);
      });
  CHECK(saved == 2);
  CHECK(image(obj, known) == Bytes{
                                 0xA5, 0x42,        // LDA zp
                                 0xD0, 0x05,        // BNE .x, re-aimed
                                 0x95, 0x43,        // STA zp,X
                                 0x4C, 0x09, 0x08,  // JMP .x, moved with the label
                                 0xAD, 0x34, 0x12,  // LDA abs stays
                                 0x60,
                             });
  elf::SymbolPtr label;
  for (const auto& s : obj.symbols) {
    if (s->name == ".x" && label == nullptr) label = s;
  }
  REQUIRE(label.get() != nullptr);
  CHECK(label->value == 9);
  CHECK(obj.symbol("f")->size == 13);
}

TEST_CASE("binutils: TestRelax::test_element_read_modify_write_shortened") {
  // `DEC t,X` on a name the linker puts in zero page becomes zp,X, as
  // `LDA t,X` does.
  std::vector<std::string> body;
  for (const auto& r : kRmw) body.push_back(std::string(r.op) + "   ext,X");
  body.push_back("RTS");
  const elf::ElfFile obj = assemble(fn(body));
  const std::int64_t saved = ld::relax(obj.sections[0], obj.symbols, [](const elf::SymbolPtr& s) {
    return s->name == "ext" ? std::optional<std::int64_t>(0x42) : std::nullopt;
  });
  CHECK(saved == static_cast<std::int64_t>(std::size(kRmw)));
  Bytes expected;
  for (const auto& r : kRmw) expected.insert(expected.end(), {r.zpx, 0x42});
  expected.push_back(0x60);
  CHECK(image(obj, {{"ext", 0x42}}) == expected);
}

TEST_CASE("binutils: TestRelax::test_jump_table_left_alone") {
  const elf::ElfFile obj = assemble(fn({"RTS", ".t:", "DC.W  ext"}));
  CHECK(ld::relax(obj.sections[0], obj.symbols,
                  [](const elf::SymbolPtr&) { return std::optional<std::int64_t>(0x42); }) == 0);
}
