// The tests of tests/test_linker_script.py that compile a whole
// program. They check each item's section in the `-S` text, the
// errors of `[[c6502::section("name")]]`, links laid out by a script
// and the errors of a script or an origin that doesn't fit, a
// function that falls into another unit's, and the renaming of
// internal names that two units share.
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "py_regex.h"

namespace {

namespace suite = c6502::suite;

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

// Returns `text` with its first `old` replaced by `replacement`, which
// is what `str.replace` does for a passage the text holds once.
std::string replace(std::string text, const std::string& old, const std::string& replacement) {
  const std::size_t at = text.find(old);
  REQUIRE(at != std::string::npos);
  REQUIRE(text.find(old, at + 1) == std::string::npos);
  return text.replace(at, old.size(), replacement);
}

std::size_t index(const std::string& text, const std::string& needle) {
  const std::size_t at = text.find(needle);
  REQUIRE(at != std::string::npos);
  return at;
}

// Compiles each source with `-O` to an object and links them, with
// `script` as the linker script and `extra` on the link's command
// line. Returns the exit status, the linked program as `linked_text`
// gives it (empty when the link fails), and stderr.
std::tuple<long long, std::string, std::string> link_units(
    const std::vector<std::string>& sources, const std::optional<std::string>& script = std::nullopt,
    const std::vector<std::string>& extra = {}) {
  const suite::TempDir tmp;
  std::vector<std::string> paths;
  for (std::size_t k = 0; k < sources.size(); ++k) {
    const std::string path = tmp.file("u" + std::to_string(k) + ".o");
    REQUIRE(suite::compile_object(sources[k], path) == 0);
    paths.push_back(path);
  }
  std::optional<std::string> ld;
  if (script) {
    ld = tmp.file("prog.ld");
    suite::write_file(*ld, *script);
  }
  const std::string out = tmp.file("out");
  const auto o = suite::link(paths, out, ld, extra);
  const long long rc = o.status.value_or(-1);
  return {rc, rc == 0 ? suite::linked_text(out) : "", o.err()};
}

const std::string kRomScript = R"(
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

// `main` goes first, and `squares` on a page of its own. `seed` is
// .data and `buf` .bss (an array, which promotion leaves alone), and
// `vectors` is at $BFFA.
const std::string kUnitA = R"(
[[c6502::section(".text.boot")]] int main(void);
const unsigned char table[4] = {10, 20, 30, 40};
unsigned char squares [[c6502::section(".page")]][4] = {0, 1, 4, 9};
unsigned char seed = 7;
unsigned char buf[3];
unsigned char pick(unsigned char i);
[[c6502::section(".vectors")]]
int (*const vectors[3])(void) = {main, main, main};
int main(void) {
    return pick(2) + seed + squares[3] + buf[0] + buf[1] + buf[2];
}
)";
const std::string kUnitB = R"(
extern const unsigned char table[4];
static unsigned char hits = 100;
unsigned char pick(unsigned char i) { hits++; return table[i] + hits; }
)";

// Returns the section of each item that the `-O` text's `; item`
// lines name.
std::map<std::string, std::string> items(const std::string& src) {
  std::map<std::string, std::string> out;
  for (const std::string& line : suite::lines(suite::codegen(src, true))) {
    if (!line.starts_with("; item ")) continue;
    std::vector<std::string> parts;
    std::size_t at = 0;
    while (at < line.size()) {
      const std::size_t start = line.find_first_not_of(' ', at);
      if (start == std::string::npos) break;
      const std::size_t end = line.find(' ', start);
      parts.push_back(line.substr(start, end - start));
      at = end;
    }
    REQUIRE(parts.size() >= 3);
    for (std::size_t k = 3; k < parts.size(); ++k) {
      if (parts[k].starts_with("section=")) out[parts[2]] = parts[k].substr(8);
    }
  }
  return out;
}

}  // namespace

TEST_CASE("linker_script: each item's default section") {
  const auto got = items(
      "const unsigned char k[2] = {1, 2};\n"
      "unsigned char z[2];\n"
      "unsigned char d[2] = {3, 4};\n"
      "int main(void) { const char *s = \"hi\"; "
      "return k[0] + z[0] + d[0] + s[0]; }\n");
  CHECK(got.at("main") == ".text");
  CHECK(got.at("k") == ".rodata");
  CHECK(got.at("z") == ".bss");
  CHECK(got.at("d") == ".data");
  CHECK(got.at("str@0") == ".rodata");
}

TEST_CASE("linker_script: the sections that the attribute names") {
  const auto got = items(
      "[[c6502::section(\".text.boot\")]] int main(void);\n"
      "unsigned char t [[c6502::section(\".page\")]][2] = {1, 2};\n"
      "int f(void) { static int n [[c6502::section(\".noinit\")]];"
      " return ++n; }\n"
      "int main(void) { return t[0] + f(); }\n");
  CHECK(got.at("main") == ".text.boot");
  CHECK(got.at("t") == ".page");
  CHECK(got.at("@0.n") == ".noinit");
}

TEST_CASE("linker_script: the errors of the section attribute") {
  const std::string main = "int main(void) { return 0; }\n";
  const std::vector<std::pair<std::string, std::string>> cases = {
      {"int g [[c6502::section(1)]];\n", "ParserError"},
      {"int g [[c6502::section(\"1x\")]];\n", "ParserError"},
      {"int f(void) { int x [[c6502::section(\".d\")]] = 1;"
       " return x; }\n",
       "TypeCheckError"},
      {"unsigned char g [[c6502::zeropage, c6502::section(\".d\")]];\n", "TypeCheckError"},
      {"extern unsigned char g [[c6502::address(0xC000), c6502::"
       "section(\".d\")]];\n",
       "TypeCheckError"},
      {"int g [[c6502::section(\".a\")]];\n"
       "int g [[c6502::section(\".b\")]];\n",
       "TypeCheckError"},
  };
  suite::Options o;
  o.optimize = true;
  for (const auto& [src, error] : cases) {
    INFO(src);
    const suite::Error e = suite::compile_error(suite::Stage::kCodegen, src + main, o);
    CHECK(suite::is_a(e.type, error));
  }
}

TEST_CASE("linker_script: a script lays the sections out") {
  const auto [rc, text, err] = link_units({kUnitA, kUnitB}, kRomScript);
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(suite::contains(text, "   SEG   text\n   ORG   $8000\n"));
  CHECK(suite::contains(text, "   SEG.U bss\n"));
  CHECK(suite::contains(text, "   RORG  $0800\n"));
  CHECK(suite::contains(text, "   ORG   $BFFA\n"));
  // `main`, from .text.boot, comes before `pick`.
  CHECK(index(text, "\nmain:") < index(text, "\npick:"));
}

TEST_CASE("linker_script: a script places a unit's code by its file's name") {
  // The second unit's code goes first, by its file's name.
  const std::string script = replace(kRomScript,
                                     ".text : { *(.text.boot) *(.text .text.*) } > rom",
                                     ".first : { u1.o(.text .text.*) } > rom\n"
                                     "    .text : { *(.text.boot) *(.text .text.*) } > rom");
  const auto [rc, text, err] = link_units({kUnitA, kUnitB}, script);
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(index(text, "\npick:") < index(text, "\nmain:"));
  CHECK(suite::contains(text, "   SEG   first\n   ORG   $8000\n"));
}

TEST_CASE("linker_script: a region that overflows is a link error") {
  const auto [rc, text, err] =
      link_units({kUnitA, kUnitB}, replace(kRomScript, "LENGTH = 16K", "LENGTH = 0x40"));
  CHECK(rc == 1);
  CHECK(py_search(R"(section \.\w+ runs past the end of region rom by \d+ bytes)", err));
}

TEST_CASE("linker_script: an origin outside the 6502's address space is a link error") {
  // The last is too wide for the C++ linker's 64-bit addresses.
  const std::vector<std::pair<std::string, std::string>> cases = {
      {"0x10000", "$10000"}, {"-1", "-$0001"}, {"0x10000000000000000", "$10000000000000000"}};
  for (const auto& [origin, shown] : cases) {
    INFO(origin);
    const auto [rc, text, err] = link_units({kUnitA, kUnitB}, std::nullopt, {"-Ttext=" + origin});
    CHECK(rc == 1);
    CHECK(suite::contains(err, "link error: the origin " + shown +
                                   " is outside the 6502's address space ($0000-$FFFF)"));
  }
}

TEST_CASE("linker_script: a section that runs past the top of memory is a link error") {
  const auto [rc, text, err] = link_units({kUnitA, kUnitB}, std::nullopt, {"-Ttext=0xFFF0"});
  CHECK(rc == 1);
  CHECK(py_search(R"(section \.text runs at \$FFF0 with \d+ bytes, outside )"
                  R"(the 6502's address space \(\$0000-\$FFFF\))",
                  err));
}

TEST_CASE("linker_script: a load address outside the address space is a link error") {
  std::string script = replace(kRomScript, "ORIGIN = 0x8000", "ORIGIN = 0xFFF0");
  script = replace(script, "LENGTH = 16K", "LENGTH = 1K");
  const auto [rc, text, err] = link_units({kUnitA, kUnitB}, script);
  CHECK(rc == 1);
  CHECK(py_search(R"(section \.\w+ (runs|loads) at \$[0-9A-F]+ with \d+ )"
                  R"(bytes, outside the 6502's address space)",
                  err));
}

TEST_CASE("linker_script: a section that no output section takes is a link error") {
  const auto [rc, text, err] =
      link_units({kUnitA, kUnitB}, replace(kRomScript, "*(.page)", "*(.nothing)"));
  CHECK(rc == 1);
  CHECK(suite::contains(err, "no output section in the script takes `squares`"));
}

TEST_CASE("linker_script: an initialized object in a NOLOAD section is a link error") {
  std::string script = replace(kRomScript, "*(.bss .bss.*)", "*(.bss .bss.* .data)");
  script = replace(script, ".data : { *(.data .data.*) }", ".data : { *(.data.*) }");
  const auto [rc, text, err] = link_units({kUnitA, kUnitB}, script);
  CHECK(rc == 1);
  CHECK(suite::contains(err, "NOLOAD"));
}

TEST_CASE("linker_script: a bad script is reported") {
  const auto [rc, text, err] = link_units({kUnitB}, "SECTIONS { .text : { *(.text) } > r }");
  CHECK(rc == 1);
  CHECK(suite::contains(err, "doesn't define"));
}

namespace {

// A function of one unit that ends by calling one of another's falls
// into it once linked.
const std::string kFallMain =
    "unsigned char step(unsigned char x);\n"
    "unsigned char finish(unsigned char x);\n"
    "unsigned char other(void);\n"
    "int main(void) { return finish(1) + step(2) + other(); }\n"
    "unsigned char step(unsigned char x) {\n"
    "    return finish(x + 3);\n"
    "}\n";
// `other` comes between `step` and `finish` in the script's order.
const std::string kFallFinish =
    "unsigned char other(void) { return 5; }\n"
    "unsigned char finish(unsigned char x) {\n"
    "    return x * 2 + 1;\n"
    "}\n";

// The script with `attribute` after the name of `.text`.
std::string fall_script(const std::string& attribute) {
  return "\nMEMORY { ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800 }\n"
         "SECTIONS\n"
         "{\n"
         "    .text" +
         attribute +
         " : { *(.text .text.*) } > ram\n"
         "    .rodata : { *(.rodata .rodata.*) } > ram\n"
         "    .data : { *(.data .data.*) } > ram\n"
         "    .bss : { *(.bss .bss.*) } > ram\n"
         "}\n";
}

}  // namespace

TEST_CASE("linker_script: a function falls into another unit's function it jumps to") {
  const auto [rc, text, err] = link_units({kFallMain, kFallFinish}, fall_script(""));
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK_FALSE(suite::contains(text, "JMP   finish"));
  const std::size_t step = index(text, "\nstep:");
  const std::size_t finish = index(text, "\nfinish:");
  REQUIRE(step < finish);
  CHECK_FALSE(suite::contains(text.substr(step, finish - step), "RTS"));
}

TEST_CASE("linker_script: NOREORDER keeps the jump") {
  const auto [rc, text, err] = link_units({kFallMain, kFallFinish}, fall_script(" (NOREORDER)"));
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(suite::contains(text, "JMP   finish"));
}

TEST_CASE("linker_script: an internal name two units share is renamed in one of them") {
  const auto [rc, text, err] = link_units({
      "static unsigned char hits = 1;\n"
      "unsigned char b(void);\n"
      "int main(void) { const char *s = \"A\"; hits++; "
      "return hits + s[0] + b(); }\n",
      "static unsigned char hits = 10;\n"
      "unsigned char b(void) { const char *s = \"B\"; hits++; "
      "return hits + s[0]; }\n",
  });
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(suite::contains(text, "str@0.1"));
}

TEST_CASE("linker_script: an external name defined in two units is a link error") {
  const auto [rc, text, err] =
      link_units({"int g = 1;\nint main(void) { return g; }\n", "int g = 2;\n"});
  CHECK(rc == 1);
  CHECK(suite::contains(err, "`g` is defined in multiple TUs"));
}
