// Tests of src/passes/linker_objects.*: Python's message for both raise
// sites of the linker's functions on the objects, which
// tools/capture_binutils.py's records don't reach, and the results of
// the Python functions for small objects. `c6502cpp check-binutils`
// checks the functions on every call the corpus and the suite make of
// them.
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "argparse.h"
#include "binutils/elf.h"
#include "binutils/ld.h"
#include "doctest.h"
#include "errors.h"
#include "passes/linker_objects.h"

namespace {

using c6502::CompileError;
namespace elf = c6502::binutils::elf;
namespace ld = c6502::binutils::ld;
namespace linker = c6502::passes::linker;

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

}  // namespace

TEST_CASE("object_items and reassemble give Python's messages") {
  const elf::SectionPtr s1 = text();
  const elf::ElfFile o = obj_with({sym("f", 0, s1, 0, elf::STT_FUNC)}, {s1});
  c6502::passes::LinkMetadata meta;
  meta.items = {{.name = "zeta", .section = ".text"},
                {.name = "f", .section = ".text"},
                {.name = "alpha", .section = ".text"}};
  CHECK(error_of([&] { linker::object_items(meta, o, "dir/u.o"); }) ==
        "LinkError: dir/u.o: the link metadata lists ['alpha', 'zeta'], which the object doesn't "
        "define");

  // A unit whose text the assembler rejects, which reaches a slot that
  // the program puts above $FF with zero-page addressing.
  const elf::SectionPtr code = text();
  auto src = std::make_shared<elf::Section>();
  src->name = std::string(elf::SOURCE_SECTION);
  src->flags = 0;
  src->data = "    FOO $12\n";
  const elf::SymbolPtr t = sym("__local_f__x");
  code->relocs.push_back(elf::Reloc{1, elf::R_MOS_ADDR8, t, 0});
  ld::Objects objects{
      {"dir/u.o", obj_with({sym("u.s", 0, nullptr, elf::SHN_ABS, elf::STT_FILE, elf::STB_LOCAL), t},
                           {code, src})}};
  REQUIRE(objects[0].second.source_text() == "    FOO $12\n");
  ld::Names placed;
  placed.insert_or_assign("__local_f__x", 0x200);
  CHECK(
      error_of([&] { linker::reassemble(objects, {{}}, placed, {}); }) ==
      "LinkError: dir/u.o: assembling it again with the program's zero-page addresses: u.s:1: FOO "
      "comes before any label");
  // In zero page, the unit is left alone.
  placed.insert_or_assign("__local_f__x", 0xF0);
  linker::reassemble(objects, {{}}, placed, {});
  CHECK(objects[0].second.sections.size() == 2);
}

TEST_CASE("output_symbols names each file by Path(path).name and sorts each table by value") {
  const elf::SectionPtr s1 = text();
  ld::Objects objects{
      {"dir/sub/u.o", obj_with({sym("u.s", 0, nullptr, elf::SHN_ABS, elf::STT_FILE, elf::STB_LOCAL),
                                sym("f", 0, s1, 0, elf::STT_FUNC),
                                sym(".l", 3, s1, 0, elf::STT_NOTYPE, elf::STB_LOCAL),
                                absolute("X", 0x1234), sym("u")},
                               {s1})},
      {"v.o", obj_with({absolute("X", 0x1234)})}};
  objects[0].second.symbols[1]->size = 4;
  std::vector<ld::OutputSection> outputs(1);
  outputs[0].name = ".text";
  outputs[0].addr = outputs[0].load_addr = 0x800;
  outputs[0].size = 3;
  outputs[0].executable = true;
  outputs[0].parts = {ld::Placed{0, s1, 0x800, 3}};
  ld::PlacedBySection addr_of{{s1.get(), outputs[0].parts[0]}};
  ld::Names placed_names;
  placed_names.insert_or_assign("b", 5);
  placed_names.insert_or_assign("a", 3);
  placed_names.insert_or_assign("c", 5);
  c6502::OrderedMap<std::string, std::vector<c6502::passes::ZpObjectMeta>> zp_objects;
  zp_objects.insert_or_assign("a",
                              std::vector<c6502::passes::ZpObjectMeta>{{.name = "a", .size = 2}});
  ld::Names layout_symbols;
  layout_symbols.insert_or_assign("X", 1);
  layout_symbols.insert_or_assign("e", -1);
  ld::Names defsyms;
  defsyms.insert_or_assign("d", 0x12345);
  const std::vector<ld::OutputSymbol> got = linker::output_symbols(
      objects, outputs, addr_of, placed_names, zp_objects, layout_symbols, defsyms);
  using Row = std::tuple<std::string, std::int64_t, std::int64_t, int, int, int, int>;
  std::vector<Row> rows;
  for (const auto& o : got) {
    const elf::Symbol& s = *o.symbol;
    rows.emplace_back(s.name, s.value, s.size, s.bind, s.type, o.output >= 0 ? -1 : s.shndx,
                      s.other);
  }
  // Python gives these, with the output section where the sixth item
  // is -1 here.
  CHECK(rows == std::vector<Row>{{"u.o", 0, 0, 0, 4, 65521, 0},
                                 {"f", 2048, 3, 1, 2, -1, 0},
                                 {"X", 4660, 0, 1, 0, 65521, 0},
                                 {"v.o", 0, 0, 0, 4, 65521, 0},
                                 {"a", 3, 2, 1, 1, 65521, 0},
                                 {"b", 5, 0, 1, 0, 65521, 0},
                                 {"c", 5, 0, 1, 0, 65521, 0},
                                 {"e", 65535, 0, 1, 0, 65521, 0},
                                 {"d", 9029, 0, 1, 0, 65521, 0}});
}

TEST_CASE("path_name is Path(path).name") {
  CHECK(c6502::argparse::path_name("dir/sub/u.o") == "u.o");
  CHECK(c6502::argparse::path_name("a/./") == "a");
  CHECK(c6502::argparse::path_name("") == "");
  CHECK(c6502::argparse::path_name(".") == "");
  CHECK(c6502::argparse::path_name("x/..") == "..");
  CHECK(c6502::argparse::path_name("//s/h/y.o") == "y.o");
#ifdef _WIN32
  CHECK(c6502::argparse::path_name("C:x.o") == "x.o");
  CHECK(c6502::argparse::path_name("a\\b.o") == "b.o");
#else
  CHECK(c6502::argparse::path_name("C:x.o") == "C:x.o");
  CHECK(c6502::argparse::path_name("a\\b.o") == "a\\b.o");
#endif
}

TEST_CASE("contents sorts a local and a global target at one offset, the global first") {
  auto s = std::make_shared<elf::Section>();
  s->name = ".rodata";
  s->flags = elf::SHF_ALLOC;
  s->data = std::string(2, '\0');
  const elf::SymbolPtr ext = sym("ext");
  const elf::SymbolPtr me = sym("T@lo", 0, s, 0, elf::STT_OBJECT, elf::STB_LOCAL);
  const elf::SymbolPtr own = sym("own", 1, text(), 0, elf::STT_FUNC, elf::STB_LOCAL);
  s->relocs.push_back(elf::Reloc{0, elf::R_MOS_ADDR16_LO, own, 0});
  s->relocs.push_back(elf::Reloc{0, elf::R_MOS_ADDR16_LO, ext, 0});
  const std::optional<linker::Contents> c = linker::contents(2, obj_with({me, ext, own}, {s}), me);
  REQUIRE(c.has_value());
  REQUIRE(c->relocs.size() == 2);
  CHECK(c->relocs[0].target == linker::Target{false, 0, "ext"});
  CHECK(c->relocs[1].target == linker::Target{true, 2, "own"});
}

TEST_CASE("merge_identical redefines the later copies' symbols in place") {
  const auto table = [](const std::string& name, const std::string& bytes) {
    auto s = std::make_shared<elf::Section>();
    s->name = ".rodata";
    s->flags = elf::SHF_ALLOC;
    s->data = bytes;
    const elf::SymbolPtr t = sym("ext");
    s->relocs.push_back(elf::Reloc{0, elf::R_MOS_ADDR16_LO, t, 1});
    const elf::SymbolPtr me = sym(name, 0, s, 0, elf::STT_OBJECT, elf::STB_LOCAL);
    return std::make_pair(obj_with({me, t}, {text(), s}), me);
  };
  auto [oa, ma] = table("A@lo", "\x01\x02");
  auto [ob, mb] = table("B@lo", "\x01\x02");
  auto [oc, mc] = table("C@lo", "\x01\x02\x03");
  ob.sections[0]->relocs.push_back(elf::Reloc{1, elf::R_MOS_ADDR16, mb, 1});
  const std::optional<linker::Contents> c = linker::contents(1, ob, mb);
  REQUIRE(c.has_value());
  // Python gives ('.rodata', 1, 2, 1, 2, b'\x01\x02', ((0, 4, 'ext', 1),)).
  CHECK(c->name == ".rodata");
  CHECK(c->type == 1);
  CHECK(c->flags == 2);
  CHECK(c->align == 1);
  CHECK(c->size == 2);
  CHECK(c->relocs.size() == 1);
  CHECK(c->relocs[0].target == linker::Target{false, 0, "ext"});
  std::vector<c6502::passes::LinkMetadata> metas(3);
  const char* names[] = {"A@lo", "B@lo", "C@lo"};
  for (int i = 0; i < 3; ++i)
    metas[i].items = {{.name = names[i], .section = ".rodata", .merge = true}};
  ld::Objects objects{{"a.o", oa}, {"b.o", ob}, {"c.o", oc}};
  linker::merge_identical(objects, metas);
  // Python leaves 2, 1, and 2 sections and the items [['A@lo'], [],
  // ['C@lo']], and B@lo's symbol becomes a global undefined A@lo.
  CHECK(objects[0].second.sections.size() == 2);
  CHECK(objects[1].second.sections.size() == 1);
  CHECK(objects[2].second.sections.size() == 2);
  CHECK(metas[1].items.empty());
  CHECK(ma->bind == elf::STB_GLOBAL);
  CHECK(mb->name == "A@lo");
  CHECK(mb->section.get() == nullptr);
  CHECK(mb->shndx == elf::SHN_UNDEF);
  CHECK(mb->bind == elf::STB_GLOBAL);
  CHECK(mb->type == elf::STT_NOTYPE);
  // The relocation that named B@lo names the same symbol, now A@lo.
  CHECK(objects[1].second.sections[0]->relocs.back().symbol.get() == mb.get());
  CHECK(mc->name == "C@lo");
  CHECK(mc->bind == elf::STB_LOCAL);
}
