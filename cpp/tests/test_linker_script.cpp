// tests/test_linker_script.py's tests of the script parser,
// `fall_through`, `tail_jump`, and `lay_out`, ported by hand, one case
// per Python test with each of its assertions. Where the Python test
// matched a ScriptError's or a LayoutError's message with a regular
// expression, the case checks the whole message Python 3.11 gives. The
// Python module's tests that compile and link programs stay in Python.
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "binutils/assembler.h"
#include "binutils/elf.h"
#include "doctest.h"
#include "errors.h"
#include "passes/link_layout.h"
#include "passes/link_script.h"

namespace {

namespace ll = c6502::passes::link_layout;
namespace ls = c6502::passes::link_script;
using c6502::CompileError;

const char* const kRomScript = R"(
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800
    rom (rx)  : ORIGIN = 0x8000, LENGTH = 16K
}

SECTIONS
{
    .text : { *(.text.boot) *(.text .text.*) } > rom
    .rodata : { *(.rodata .rodata.*) } > rom
    .page ALIGN(256) : { *(.page) } > rom
    .data : { *(.data .data.*) } > ram AT> rom
    .bss (NOLOAD) : { *(.bss .bss.*) } > ram
    .vectors 0xBFFA : { KEEP(*(.vectors)) }
    __free_ram = ADDR(.bss) + SIZEOF(.bss);
}
)";

// Returns the output section of `script` named `name`, which must exist.
const ls::OutputSection& section(const ls::Script& script, const std::string& name) {
  const ls::OutputSection* found = nullptr;
  for (const ls::OutputSection* s : script.output_sections()) {
    if (s->name == name) found = s;
  }
  REQUIRE_MESSAGE(found != nullptr, "no output section ", name);
  return *found;
}

const ls::Input& input(const ls::OutputSection& s, std::size_t i) {
  REQUIRE(i < s.contents.size());
  const auto* in = std::get_if<ls::Input>(&s.contents[i]);
  REQUIRE(in != nullptr);
  return *in;
}

// Returns the message of the error `f` throws, with its Python class.
template <class F>
std::string error_of(F&& f) {
  try {
    f();
  } catch (const CompileError& e) {
    return std::string(e.python_type()) + ": " + e.what();
  }
  return "no error";
}

}  // namespace

TEST_CASE("linker_script: TestScriptParser::test_regions_and_sections") {
  const ls::Script script = ls::parse_script(kRomScript);
  CHECK(script.regions.at("rom").origin == 0x8000);
  CHECK(script.regions.at("rom").length == 0x4000);
  CHECK(section(script, ".data").region == "ram");
  CHECK(section(script, ".data").load_region == "rom");
  CHECK(section(script, ".bss").noload);
  CHECK(input(section(script, ".text"), 0).patterns == std::vector<std::string>{".text.boot"});
}

TEST_CASE("linker_script: TestScriptParser::test_gnu_spellings") {
  const ls::Script script = ls::parse_script(
      "MEMORY { r : org = 0x1000, len = 2K }\n"
      "SECTIONS { .text : { *(.text) } >r }\n");
  CHECK(script.regions.at("r").length == 2048);
}

TEST_CASE("linker_script: TestScriptParser::test_file_patterns") {
  const ls::Script script = ls::parse_script(
      "SECTIONS { .a : { zstring.asm(.text) } "
      ".b : { *disk.asm(.text .data) } .c : { *(.text) } }\n");
  CHECK(input(section(script, ".a"), 0).file == "zstring.asm");
  CHECK(input(section(script, ".b"), 0).file == "*disk.asm");
  CHECK(input(section(script, ".b"), 0).patterns == std::vector<std::string>{".text", ".data"});
  CHECK(input(section(script, ".c"), 0).file == "*");
}

TEST_CASE("linker_script: TestScriptParser::test_noreorder") {
  const ls::Script script = ls::parse_script(
      "SECTIONS { .a (NOREORDER) : { *(.text) } .b : { *(.data) } "
      ".c (0x100) : { *(.rodata) } }\n");
  CHECK_FALSE(section(script, ".a").reorder);
  CHECK_FALSE(section(script, ".a").noload);
  CHECK(section(script, ".b").reorder);
  REQUIRE(section(script, ".c").address.has_value());
  CHECK(section(script, ".c").address->value == 0x100);
}

TEST_CASE("linker_script: TestScriptParser::test_constant_computes_its_operator_alone") {
  // Python computed each operator of a region's value for every
  // `Binary`, so a negative right operand made a `+` raise the `<<`'s
  // ValueError, and a large one took all of memory.
  const ls::Script script =
      ls::parse_script("MEMORY { r : ORIGIN = 0x800 + -1, LENGTH = 1 + 0x10000000000 }");
  CHECK(script.regions.at("r").origin == 0x7FF);
  CHECK(script.regions.at("r").length == 0x10000000001LL);
}

TEST_CASE("linker_script: TestScriptParser::test_errors") {
  const std::vector<std::pair<std::string, std::string>> cases = {
      {"ENTRY(main)", "'ENTRY' isn't supported; a script has MEMORY and SECTIONS commands"},
      {"SECTIONS { .text : { *(.text) } > nowhere }",
       "output section '.text' names region 'nowhere', which MEMORY doesn't define"},
      {"MEMORY { r : ORIGIN = 0, LENGTH = 1 }\n"
       "SECTIONS { .b (NOLOAD) : { *(.bss) } AT> r }",
       "output section '.b' is NOLOAD, so it has no load address for AT to give"},
      {"SECTIONS { . = 0x1000; }",
       "`. =` outside an output section isn't supported; give the output section an address or "
       "a region"},
      {"SECTIONS { .text : { (.text) } }",
       "in .text: an input section description starts with the file it takes sections from, such "
       "as `*` or `zstring.asm`, not '('"},
      {"SECTIONS { .t : { *(.a) } .t : { *(.b) } }", "output section '.t' is listed twice"},
      {"SECTIONS { .t 0x100 (KEEP) : { *(.a) } }",
       "output section '.t' has type 'KEEP'; the types are NOLOAD, NOREORDER"},
      {"MEMORY { r : ORIGIN = 1 << -1, LENGTH = 1 }", "ORIGIN of 'r' shifts by a negative count"},
      {"MEMORY { r : ORIGIN = 0, LENGTH = 8 / (2 - 2) }", "LENGTH of 'r' divides by zero"},
      // These raised ValueError out of compile.py.
      {"MEMORY { r : ORIGIN = 0800, LENGTH = 1 }",
       "can't read the number '0800': a decimal number other than 0 can't begin with 0"},
      {"SECTIONS { .t 007 : { *(.a) } }",
       "can't read the number '007': a decimal number other than 0 can't begin with 0"},
      // This raised KeyError for the missing LENGTH.
      {"MEMORY { r : ORIGIN = 1, o = 2 }", "region 'r' gives ORIGIN twice"},
  };
  for (const auto& [src, message] : cases) {
    CAPTURE(src);
    CHECK(error_of([&] { ls::parse_script(src); }) == "ScriptError: " + message);
  }
}

namespace {

ll::Entry entry(std::string name, std::optional<std::string> tail = std::nullopt,
                bool function = true) {
  ll::Entry e;
  e.name = std::move(name);
  e.function = function;
  e.tail = std::move(tail);
  return e;
}

// The order `fall_through` gives the items, with `-` after the name of
// each one whose final `JMP` is dropped.
std::vector<std::string> run(const std::vector<ll::Entry>& entries, bool reorder = true) {
  std::vector<std::string> out;
  for (const auto& [e, dropped] : ll::fall_through(entries, reorder)) {
    out.push_back(e.name + (dropped ? "-" : ""));
  }
  return out;
}

using Names = std::vector<std::string>;
const ll::Entry kG = entry("g");
const ll::Entry kData = entry("d", std::nullopt, false);

}  // namespace

TEST_CASE("linker_script: TestFallThrough::test_moves_target_after_jump") {
  CHECK(run({entry("f", "g"), kData, kG}) == Names{"f-", "g", "d"});
}

TEST_CASE("linker_script: TestFallThrough::test_first_jumper_wins") {
  CHECK(run({entry("h", "g"), entry("f", "g"), kG}) == Names{"h-", "g", "f"});
}

TEST_CASE("linker_script: TestFallThrough::test_chain") {
  // e falls into f, which falls into g, all at e's place.
  CHECK(run({entry("g"), entry("f", "g"), kData, entry("e", "f")}) ==
        Names{"d", "e-", "f-", "g"});
}

TEST_CASE("linker_script: TestFallThrough::test_cycle_keeps_one_jump") {
  CHECK(run({entry("a", "b"), entry("b", "a")}) == Names{"a-", "b"});
}

TEST_CASE("linker_script: TestFallThrough::test_left_alone") {
  // A jump to an object, to a name outside the items, or to itself.
  CHECK(run({entry("p", "d"), entry("q", "elsewhere"), entry("s", "s"), kData, kG}) ==
        Names{"p", "q", "s", "d", "g"});
}

TEST_CASE("linker_script: TestFallThrough::test_noreorder") {
  const ll::Entry f = entry("f", "g");
  CHECK(run({f, kData, kG}, false) == Names{"f", "d", "g"});
  CHECK(run({f, kG}, false) == Names{"f-", "g"});
}

namespace {

// Assembles the function `f` with `body`, each line indented unless it
// is a local label, and returns `tail_jump` of its section.
std::optional<std::string> tail(const std::vector<std::string>& body) {
  std::string text = "f:\n   SUBROUTINE\n";
  for (const std::string& b : body) {
    text += (b.starts_with(".") ? b : "   " + b) + "\n";
  }
  const auto obj = c6502::binutils::assembler::assemble(text, "t.asm");
  REQUIRE_FALSE(obj.sections.empty());
  return ll::tail_jump(obj.sections[0], obj.symbols);
}

}  // namespace

TEST_CASE("linker_script: TestTailJump::test_final_jump") {
  CHECK(tail({"LDA   #$01", "JMP   g"}) == "g");
}

TEST_CASE("linker_script: TestTailJump::test_label_on_jump_kept") {
  // A label in front of the jump then names the target's first
  // instruction, which is where the jump went.
  CHECK(tail({"BEQ   .x", "RTS", ".x:", "JMP   g"}) == "g");
}

TEST_CASE("linker_script: TestTailJump::test_not_a_final_jump") {
  const std::vector<std::vector<std::string>> bodies = {
      {"JMP   (vector)"},
      {"JMP   g", "RTS"},
      {"JMP   g", ".after:"},
      {"JMP   g+1"},
      {"BEQ   .x", "JMP   .x", ".x:", "RTS"},
  };
  for (const auto& body : bodies) {
    CAPTURE(body.front());
    CHECK_FALSE(tail(body).has_value());
  }
}

TEST_CASE("linker_script: TestTailJump::test_jump_table_at_end") {
  // Bytes that look like `JMP` in a jump table aren't one.
  CHECK_FALSE(tail({"RTS", ".t:", "DC.W  g"}).has_value());
}

TEST_CASE("linker_script: TestLayoutErrors::test_negative_shift") {
  // Python's `<<` and `>>` raise ValueError for a negative count, which
  // ended compile.py with a traceback.
  for (const std::string op : {"<<", ">>"}) {
    CAPTURE(op);
    const ls::Script script = ls::parse_script("SECTIONS { .t 1 " + op + " -1 : { } }");
    CHECK(error_of([&] { ll::lay_out(script, {}); }) ==
          "LayoutError: a shift by a negative count in the linker script");
  }
}

TEST_CASE("linker_script: TestLayoutErrors::test_section_alignment_not_a_power_of_two") {
  // Python's `_align` masked with `~(n - 1)`, which for 3 left 0x1001 at
  // 0x1001, which isn't a multiple of 3.
  const ls::Script three = ls::parse_script("SECTIONS { .t 0x1001 : ALIGN(3) { } }");
  CHECK(error_of([&] { ll::lay_out(three, {}); }) ==
        "LayoutError: ALIGN on .t takes a power of two");
  const ls::Script four = ls::parse_script("SECTIONS { .t 0x1001 : ALIGN(4) { } }");
  const ll::Layout layout = ll::lay_out(four, {});
  REQUIRE_FALSE(layout.outputs.empty());
  CHECK(layout.outputs[0].addr == 0x1004);
}
