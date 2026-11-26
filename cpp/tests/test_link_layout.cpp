// Tests of the layout (src/passes/link_layout.*) and of fnmatch
// (src/fnmatch.*): Python's message for every raise site of
// link_layout.py, `fnmatch.fnmatchcase` and `translate` on each of their
// special characters, and the startup code. `c6502cpp check-binutils`
// checks the layout's functions on every call the corpus and the suite
// make of them.
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "asm_emit.h"
#include "doctest.h"
#include "errors.h"
#include "fnmatch.h"
#include "passes/link_layout.h"
#include "passes/link_script.h"

namespace {

using c6502::CompileError;
using c6502::InternalError;
namespace elf = c6502::binutils::elf;
namespace ll = c6502::passes::link_layout;
namespace ls = c6502::passes::link_script;

// A unit with a 5-byte function `f` in `.text` and a 3-byte object `d`
// in `.data`, named as a Windows path would be.
ll::Unit sample_unit() {
  ll::Unit u;
  u.path = "a/b\\u.o";
  auto code = std::make_shared<elf::Section>();
  code->name = ".text";
  code->flags = elf::SHF_ALLOC | elf::SHF_EXECINSTR;
  code->data = std::string(5, '\0');
  auto data = std::make_shared<elf::Section>();
  data->name = ".data";
  data->flags = elf::SHF_ALLOC | elf::SHF_WRITE;
  data->data = std::string(3, '\0');
  u.items = {{.name = "f", .section = ".text"}, {.name = "d", .section = ".data"}};
  u.sections.insert_or_assign("f", code);
  u.sections.insert_or_assign("d", data);
  return u;
}

std::string layout_error(const ls::Script& script, const std::vector<ll::Unit>& units) {
  try {
    ll::lay_out(script, units);
  } catch (const CompileError& e) {
    if (e.python_type() != "LayoutError") return "a " + e.python_type();
    return e.what();
  }
  return "no error";
}

std::string layout_error(const std::string& text) {
  return layout_error(ls::parse_script(text), {sample_unit()});
}

ls::Expr num(long long v) {
  ls::Expr e;
  e.kind = ls::Expr::Kind::Num;
  e.value = v;
  return e;
}

}  // namespace

TEST_CASE("lay_out gives Python's message at every raise site") {
  // Each script with the message Python 3.11 gives for it, with the
  // sample unit, one for each raise site of link_layout.py in the order
  // of the file but the two below.
  const std::vector<std::pair<std::string, std::string>> cases = {
      {"SECTIONS { .t 0x100 : { x = y; } }", "the script uses `y` before it has a value"},
      {"SECTIONS { x = .; }", "`.` isn't defined here"},
      {"SECTIONS { x = ALIGN(4); }", "`ALIGN(n)` needs `.`"},
      {"SECTIONS { .t ALIGN(0x100, 3) : { } }", "ALIGN takes a power of two written as a number"},
      {"SECTIONS { .t ALIGN(0x100, y) : { } }", "ALIGN takes a power of two written as a number"},
      {"SECTIONS { x = ORIGIN(r); }", "ORIGIN(r) names no region"},
      {"SECTIONS { x = SIZEOF(.t); .t 1 : { } }",
       "SIZEOF(.t) comes before section .t is laid out"},
      {"SECTIONS { x = ORIGIN(1); }", "can't use `ORIGIN(1)` in a linker script expression"},
      {"SECTIONS { x = LOADADDR(a, b); }",
       "can't use `LOADADDR(a, b)` in a linker script expression"},
      {"SECTIONS { x = SIZEOF(-(. + 2)); }",
       "can't use `SIZEOF(-(. + 2))` in a linker script expression"},
      {"SECTIONS { x = 1 / (1 - 1); }", "division by zero in the linker script"},
      {"SECTIONS { x = 1 >> -1; }", "a shift by a negative count in the linker script"},
      {"SECTIONS { .t : { } }", "output section .t has no address and no region"},
      {"SECTIONS { .t 1 : ALIGN(y) { } }", "ALIGN on .t has to be a number"},
      {"SECTIONS { .t 1 : ALIGN(6) { } }", "ALIGN on .t takes a power of two"},
      {"SECTIONS { .t 0x100 : { . = 0x80; } }",
       "`. = ...` in .t moves the location back from $0100 to $0080"},
      // Python's `X` format puts the sign in the width.
      {"SECTIONS { .t 0x100 : { . = -0x80; } }",
       "`. = ...` in .t moves the location back from $0100 to $-080"},
      {"SECTIONS { .t 0x100 (NOLOAD) : { *(.data) } }",
       "`d` (section .data, a/b\\u.o) has initial bytes, so the NOLOAD output section .t can't "
       "hold it"},
      {"MEMORY { r : ORIGIN = 0x10, LENGTH = 4 } SECTIONS { .t : { *(.text) } > r }",
       "section .t runs past the end of region r by 1 bytes"},
      {"MEMORY { r : ORIGIN = 0x10, LENGTH = 4 } SECTIONS { .t 0 : { *(.text) } AT> r }",
       "section .t runs past the end of region r by 1 bytes"},
      {"SECTIONS { .t 0xFFFC : { *(.text) *(.data) } }",
       "section .t runs at $FFFC with 8 bytes, outside the 6502's address space ($0000-$FFFF)"},
      {"SECTIONS { .t -1 : { *(.text) *(.data) } }",
       "section .t runs at -$0001 with 8 bytes, outside the 6502's address space ($0000-$FFFF)"},
      {"SECTIONS { .t 0x100 : AT(0xFFFC) { *(.text) *(.data) } }",
       "section .t loads at $FFFC with 8 bytes, outside the 6502's address space ($0000-$FFFF)"},
      // An empty section may start at $10000, the end of memory.
      {"SECTIONS { .t 0xFFF8 : { *(.text) *(.data) } .u : { } .v 0x10000 : { } }", "no error"},
      {"SECTIONS { .t 0 : { *(.text) } }",
       "no output section in the script takes `d` (section .data, a/b\\u.o)"},
      // The file pattern matches the name after the last `\`.
      {"SECTIONS { .t 0 : { u.o(.text) } }",
       "no output section in the script takes `d` (section .data, a/b\\u.o)"},
  };
  for (const auto& [text, message] : cases) {
    CAPTURE(text);
    CHECK(layout_error(text) == message);
  }
  // `parse_script` rejects both of these, so only a script built by
  // hand reaches them.
  ls::Script dot;
  dot.statements.emplace_back(ls::Assign{".", num(1)});
  CHECK(layout_error(dot, {}) == "`.` can only be set inside a section");
  ls::Script mod;
  ls::Expr e;
  e.kind = ls::Expr::Kind::Binary;
  e.op = "%";
  e.args = {num(1), num(2)};
  mod.statements.emplace_back(ls::Assign{"x", e});
  CHECK(layout_error(mod, {}) == "unknown operator % in the linker script");
}

TEST_CASE("lay_out places sections, falls through, and defines Python's symbols") {
  ll::Unit u = sample_unit();
  // `g` ends with `JMP f`, so it moves in front of `f` and loses the 3
  // bytes of the jump.
  auto g = std::make_shared<elf::Section>();
  g->name = ".text";
  g->flags = elf::SHF_ALLOC | elf::SHF_EXECINSTR;
  g->data = std::string(4, '\0');
  u.items.push_back({.name = "g", .section = ".text"});
  u.sections.insert_or_assign("g", g);
  u.tails.insert_or_assign("g", "f");
  const ls::Script script = ls::parse_script(
      "MEMORY { rom : ORIGIN = 0x8000, LENGTH = 0x100 ram : ORIGIN = 0x800, LENGTH = 0x10 }\n"
      "SECTIONS { .text : { *(.text) } > rom .data : { *(.data) } > ram AT> rom "
      "end = . + LOADADDR(.data); }");
  const ll::Layout layout = ll::lay_out(script, {u});
  REQUIRE(layout.outputs.size() == 2);
  const auto& text = layout.outputs[0];
  CHECK(text.executable);
  REQUIRE(text.parts.size() == 2);
  CHECK((text.parts[0].section == g));
  CHECK(text.parts[0].addr == 0x8000);
  CHECK(text.parts[0].size == 1);
  CHECK(text.parts[1].addr == 0x8001);
  CHECK(text.size == 6);
  const auto& data = layout.outputs[1];
  CHECK(data.addr == 0x800);
  CHECK(data.load_addr == 0x8006);
  std::vector<std::pair<std::string, std::int64_t>> symbols(layout.symbols.begin(),
                                                            layout.symbols.end());
  const std::vector<std::pair<std::string, std::int64_t>> want = {
      {"__text_start", 0x8000}, {"__text_end", 0x8006},     {"__text_size", 6},
      {"__text_load", 0x8000},  {"__text_load_end", 0x8006}, {"__data_start", 0x800},
      {"__data_load", 0x8006},  {"__data_end", 0x803},      {"__data_size", 3},
      {"__data_load_end", 0x8009}, {"end", 0x803 + 0x8006}};
  CHECK((symbols == want));
}

TEST_CASE("lay_out reports an address past 64 bits") {
  // Python gives the symbol 2**70, which the layout's 64-bit values
  // can't hold.
  CHECK_THROWS_AS(ll::lay_out(ls::parse_script("SECTIONS { x = 1 << 70; }"), {}), InternalError);
}

TEST_CASE("fall_through follows Python's rules") {
  const auto entry = [](std::string name, std::optional<std::string> tail, bool function = true) {
    ll::Entry e;
    e.name = std::move(name);
    e.tail = std::move(tail);
    e.function = function;
    return e;
  };
  const auto order = [](const std::vector<std::pair<ll::Entry, bool>>& out) {
    std::string s;
    for (const auto& [e, dropped] : out) s += e.name + (dropped ? "- " : " ");
    return s;
  };
  // The first function jumping to `g` wins, and a cycle keeps one jump.
  CHECK(order(ll::fall_through({entry("a", "g"), entry("b", "g"), entry("g", std::nullopt)})) ==
        "a- g b ");
  CHECK(order(ll::fall_through({entry("a", "b"), entry("b", "a")})) == "a- b ");
  CHECK(order(ll::fall_through({entry("a", "c"), entry("b", "a"), entry("c", "d"),
                                entry("d", std::nullopt)})) == "b- a- c- d ");
  // Data isn't a target, and without reordering only a neighbor is.
  CHECK(order(ll::fall_through({entry("a", "d"), entry("d", std::nullopt, false)})) == "a d ");
  CHECK(order(ll::fall_through({entry("a", "c"), entry("b", "c"), entry("c", std::nullopt)},
                               false)) == "a b- c ");
}

TEST_CASE("tail_jump finds a final JMP as Python does") {
  auto sec = std::make_shared<elf::Section>();
  sec->data = std::string("\xA9\x01\x4C\x00\x00", 5);
  auto g = std::make_shared<elf::Symbol>();
  g->name = "g";
  sec->relocs.push_back({3, elf::R_MOS_ADDR16, g, 0});
  auto f = std::make_shared<elf::Symbol>();
  f->name = "f";
  f->type = elf::STT_FUNC;
  f->section = sec;
  CHECK(ll::tail_jump(sec, {f}).value_or("none") == "g");
  // A label on the operand, and a `$d` mapping symbol before the JMP.
  auto label = std::make_shared<elf::Symbol>();
  label->name = ".x";
  label->value = 3;
  label->section = sec;
  CHECK_FALSE(ll::tail_jump(sec, {f, label}));
  auto d = std::make_shared<elf::Symbol>();
  d->name = "$d";
  d->value = 1;
  d->section = sec;
  CHECK_FALSE(ll::tail_jump(sec, {f, d}));
  sec->relocs[0].addend = 1;
  CHECK_FALSE(ll::tail_jump(sec, {f}));
}

TEST_CASE("default_script lists the standard sections and then the others") {
  ll::Unit u = sample_unit();
  u.items.push_back({.name = "p", .section = ".page"});
  u.items.push_back({.name = "q", .section = ".text.boot"});
  const ls::Script s = ll::default_script({u}, 0x0800);
  const auto secs = s.output_sections();
  REQUIRE(secs.size() == 5);
  CHECK(secs[0]->address->value == 0x0800);
  CHECK(secs[4]->name == ".page");
  CHECK((std::get<ls::Input>(secs[4]->contents[0]).patterns == std::vector<std::string>{".page"}));
  CHECK((std::get<ls::Input>(secs[0]->contents[0]).patterns ==
         std::vector<std::string>{".text", ".text.*"}));
}

TEST_CASE("startup_lines copies AT> sections and clears NOLOAD ones") {
  const ls::Script s = ls::parse_script(
      "MEMORY { r : ORIGIN = 0, LENGTH = 1 } SECTIONS { .a 0 : { } .b 0 : { } AT> r "
      ".c (NOLOAD) : { } > r }");
  const auto lines = ll::startup_lines(s);
  REQUIRE(lines.size() == 60);
  CHECK(lines[0] == "   LDA   #<__b_load");
  CHECK(lines[13] == ".start1:");
  CHECK(lines[22] == ".src1:");
  CHECK(lines[43] == ".start2:");
  CHECK(lines.back() == ".done2:");
  CHECK(lines[0] == c6502::asm_emit::instr_line("lda", "#<__b_load"));
  CHECK(ll::startup_lines(ls::parse_script("SECTIONS { .a 0 : { } }")).empty());
}

TEST_CASE("section_symbol makes one underscore of each code point") {
  CHECK(ll::section_symbol(".text.boot") == "text_boot");
  CHECK(ll::section_symbol("..x-\xC3\xA9") == "x__");
}

TEST_CASE("fnmatchcase matches as Python's fnmatch does") {
  // Each name and pattern with Python 3.11's `fnmatchcase` and
  // `translate`, which cover `*`, `?`, sets with `!`, ranges, a leading
  // `]`, an unclosed `[`, `re.escape`'s characters, and the set
  // operations `&&`, `~~`, and `||` that `translate` escapes.
  const std::vector<std::tuple<std::string, std::string, bool, std::string>> cases = {
      {"main.o", "*", true, R"((?s:.*)\Z)"},
      {"main.o", "*.o", true, R"((?s:.*\.o)\Z)"},
      {"main.o", "*.c", false, R"((?s:.*\.c)\Z)"},
      {"main.o", "m*n.o", true, R"((?s:m.*n\.o)\Z)"},
      {"main.o", "m??n.o", true, R"((?s:m..n\.o)\Z)"},
      {"main.o", "m?n.o", false, R"((?s:m.n\.o)\Z)"},
      {"main.o", "[lm]ain.o", true, R"((?s:[lm]ain\.o)\Z)"},
      {"main.o", "[!m]ain.o", false, R"((?s:[^m]ain\.o)\Z)"},
      {"main.o", "[a-z]ain.o", true, R"((?s:[a-z]ain\.o)\Z)"},
      {"Main.o", "[a-z]ain.o", false, R"((?s:[a-z]ain\.o)\Z)"},
      {"main.o", "[]m]ain.o", true, R"((?s:[]m]ain\.o)\Z)"},
      {"]ain.o", "[]m]ain.o", true, R"((?s:[]m]ain\.o)\Z)"},
      {"[ain.o", "[ain.o", true, R"((?s:\[ain\.o)\Z)"},
      {"main.o", "[!]ain.o", false, R"((?s:\[!\]ain\.o)\Z)"},
      {"!ain.o", "[!]ain.o", false, R"((?s:\[!\]ain\.o)\Z)"},
      {"a-b", "a[-]b", true, R"((?s:a[\-]b)\Z)"},
      {"a-b", "a[x-]b", true, R"((?s:a[x\-]b)\Z)"},
      {"a.b", "a.b", true, R"((?s:a\.b)\Z)"},
      {"axb", "a.b", false, R"((?s:a\.b)\Z)"},
      {"a$b", "a$b", true, R"((?s:a\$b)\Z)"},
      {"a^b", "a[^]b", true, R"((?s:a[\^]b)\Z)"},
      {"a&b", "a[&]b", true, R"((?s:a[\&]b)\Z)"},
      {"a~b", "a[~|]b", true, R"((?s:a[\~\|]b)\Z)"},
      {"a\\b", "a[\\]b", true, R"((?s:a[\\]b)\Z)"},
      {"a\\b", "a\\b", true, R"((?s:a\\b)\Z)"},
      {".text.boot", ".text.*", true, R"((?s:\.text\..*)\Z)"},
      {".text", ".text.*", false, R"((?s:\.text\..*)\Z)"},
      {".textual", ".text*", true, R"((?s:\.text.*)\Z)"},
      {"x", "[z-a]", false, R"((?s:(?!))\Z)"},
      {"b", "[a-c-e]", true, R"((?s:[a-c\-e])\Z)"},
      {"d", "[a-c-e]", false, R"((?s:[a-c\-e])\Z)"},
      {"-", "[a-c-e]", true, R"((?s:[a-c\-e])\Z)"},
      {"ab\ncd", "ab*", true, R"((?s:ab.*)\Z)"},
      {"\xC3\xA9t\xC3\xA9.o", "?t?.o", true, R"((?s:.t.\.o)\Z)"},
      {"a(b)", "a(b)", true, R"((?s:a\(b\))\Z)"},
      {"a+", "a+", true, R"((?s:a\+)\Z)"},
      {"aaa", "*a*a", true, R"((?s:(?>.*?a).*a)\Z)"},
      {"ab", "*a*a", false, R"((?s:(?>.*?a).*a)\Z)"},
      {"x", "", false, R"((?s:)\Z)"},
      {"", "", true, R"((?s:)\Z)"},
      {"", "*", true, R"((?s:.*)\Z)"},
      {"a b", "a b", true, R"((?s:a\ b)\Z)"},
      {"a{2}", "a{2}", true, R"((?s:a\{2\})\Z)"},
      {"ab", "[!a-c]b", false, R"((?s:[^a-c]b)\Z)"},
      {"db", "[!a-c]b", true, R"((?s:[^a-c]b)\Z)"},
      {"a", "[!]", false, R"((?s:\[!\])\Z)"},
      {"!", "[!]", false, R"((?s:\[!\])\Z)"},
      {"z", "[c-a-z]", true, R"((?s:[\-z])\Z)"},
  };
  for (const auto& [name, pat, match, regex] : cases) {
    CAPTURE(name);
    CAPTURE(pat);
    CHECK(c6502::fnmatch::translate(pat) == regex);
    CHECK(c6502::fnmatch::fnmatchcase(name, pat) == match);
  }
}
