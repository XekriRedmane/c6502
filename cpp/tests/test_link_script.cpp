// Tests of the linker scripts (src/passes/link_script.*): Python's
// message for every raise site of `parse_script`, Python's text
// handling (Unicode spaces and digits, positions in code points), and
// `read_script`'s file reading in the locale's encoding. `c6502cpp
// check-binutils` checks `parse_script` on every script the corpus and
// the suite give it.
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "doctest.h"
#include "errors.h"
#include "json.h"
#include "passes/link_checks.h"
#include "passes/link_script.h"

namespace {

using c6502::CompileError;
using c6502::InternalError;
namespace ls = c6502::passes::link_script;

// Returns the message of the ScriptError `parse_script(text)` throws,
// or "no error".
std::string error_of(const std::string& text) {
  try {
    ls::parse_script(text);
  } catch (const CompileError& e) {
    if (e.python_type() != "ScriptError") return "a " + e.python_type();
    return e.what();
  }
  return "no error";
}

// Each script with the message Python 3.11 gives for it, one for each
// of the 18 raise sites of link_script.py in the order of the file.
const std::vector<std::pair<std::string, std::string>> kErrors = {
    {"MEMORY { r : ORIGIN = 0, LENGTH = 1 }\n  @oops", "line 2: can't read '@oops'"},
    {"MEMORY {", "unexpected end of script; expected None"},
    {"MEMORY ( r )", "expected '{', found '('"},
    {"ENTRY(main)", "'ENTRY' isn't supported; a script has MEMORY and SECTIONS commands"},
    {"MEMORY { r : START = 0 }", "region 'r': expected ORIGIN or LENGTH, found 'START'"},
    {"MEMORY { r : ORIGIN = 1, o = 2 }", "region 'r' gives ORIGIN twice"},
    {"MEMORY { r : ORIGIN = 0, LENGTH = 1 r : ORIGIN = 2, LENGTH = 1 }",
     "region 'r' is defined twice"},
    {"SECTIONS { . = 0x1000; }",
     "`. =` outside an output section isn't supported; give the output section an address or "
     "a region"},
    {"SECTIONS { .t 0x100 (KEEP) : { *(.a) } }",
     "output section '.t' has type 'KEEP'; the types are NOLOAD, NOREORDER"},
    {"SECTIONS { .text : { (.text) } }",
     "in .text: an input section description starts with the file it takes sections from, such "
     "as `*` or `zstring.asm`, not '('"},
    {"MEMORY { r : ORIGIN = 0800, LENGTH = 1 }",
     "can't read the number '0800': a decimal number other than 0 can't begin with 0"},
    {"SECTIONS { .t : { x = ; } }", "expected an expression, found ';'"},
    {"MEMORY { r : ORIGIN = 0, LENGTH = 8 / (2 - 2) }", "LENGTH of 'r' divides by zero"},
    {"MEMORY { r : ORIGIN = 1 >> -1, LENGTH = 1 }", "ORIGIN of 'r' shifts by a negative count"},
    {"MEMORY { r : ORIGIN = x, LENGTH = 1 }", "ORIGIN of 'r' has to be a number"},
    {"SECTIONS { .t : { *(.a) } .t : { *(.b) } }", "output section '.t' is listed twice"},
    {"SECTIONS { .text : { *(.text) } > nowhere }",
     "output section '.text' names region 'nowhere', which MEMORY doesn't define"},
    {"MEMORY { r : ORIGIN = 0, LENGTH = 1 }\nSECTIONS { .b (NOLOAD) : { *(.bss) } AT> r }",
     "output section '.b' is NOLOAD, so it has no load address for AT to give"},
};

}  // namespace

TEST_CASE("parse_script gives Python's message at every raise site") {
  for (const auto& [text, message] : kErrors) {
    CAPTURE(text);
    CHECK(error_of(text) == message);
  }
}

TEST_CASE("parse_script reads Python's other errors and corner cases") {
  // An unclosed comment lexes as `/` and `*`.
  CHECK(error_of("SECTIONS { .t : { *(.a) } } /* never closed") ==
        "'/' isn't supported; a script has MEMORY and SECTIONS commands");
  CHECK(error_of("SECTIONS { .t 1 AT ( 2 ) : { } AT > r }") == "expected ':', found 'AT'");
  // The repr shows the 20 code points from where the lexer stopped.
  CHECK(error_of("SECTIONS { \xC3\xA9 }") == "line 1: can't read '\xC3\xA9 }'");
  // A decimal number in another script's digits with a leading zero,
  // U+0660 U+0667.
  CHECK(error_of("MEMORY { r : ORIGIN = \xD9\xA0\xD9\xA7, LENGTH = 1 }") ==
        "can't read the number '\xD9\xA0\xD9\xA7': a decimal number other than 0 can't begin "
        "with 0");
}

TEST_CASE("parse_script takes Unicode's digits and spaces as Python's re does") {
  // U+0663 U+0660 is 30, and U+1D7D9, MATHEMATICAL DOUBLE-STRUCK DIGIT
  // ONE, is 1, so its `K` is 1024. U+2003, EM SPACE, is `\s`.
  const ls::Script s = ls::parse_script(
      "MEMORY {\xE2\x80\x83r : ORIGIN = \xD9\xA3\xD9\xA0, LENGTH = \xF0\x9D\x9F\x99K }");
  const ls::Region& r = s.regions.at("r");
  CHECK(r.origin == 30);
  CHECK(r.length == 1024);
}

TEST_CASE("parse_script computes a region's values as Python does") {
  const ls::Script s = ls::parse_script(
      "MEMORY { r : ORIGIN = ~0, LENGTH = -(3 * 4) + (1 << 4) | 2 ^ 1 & 3 "
      "q : org = 0x10K, len = 2M p : o = 0x800 + -1, l = -7 / 2 }");
  CHECK(s.regions.at("r").origin == 0xFFFF);
  CHECK(s.regions.at("r").length == 7);
  CHECK(s.regions.at("q").origin == 16384);
  CHECK(s.regions.at("q").length == 2097152);
  CHECK(s.regions.at("p").origin == 0x7FF);
  // Python's `//` rounds toward negative infinity.
  CHECK(s.regions.at("p").length == -4);
}

TEST_CASE("parse_script's Script makes Python's tagged JSON") {
  const ls::Script s = ls::parse_script(
      "SECTIONS { .t (1) : { . += 4; KEEP(*(.a , .b)) *zstring.asm(.text) x -= 1; "
      "PROVIDE(y = .); } ; z = ALIGN(2); }");
  const std::string want =
      R"({"object": "passes.link_script.Script", "fields": {"regions": {"dict": []}, )"
      R"("statements": [{"object": "passes.link_script.OutputSection", "fields": {"name": ".t", )"
      R"("address": {"object": "passes.link_script.Num", "fields": {"value": 1}}, "noload": false, )"
      R"("reorder": true, "at": null, "align": null, "contents": [{"object": )"
      R"("passes.link_script.Assign", "fields": {"symbol": ".", "expr": {"object": )"
      R"("passes.link_script.Binary", "fields": {"op": "+", "left": {"object": )"
      R"("passes.link_script.Dot", "fields": {}}, "right": {"object": "passes.link_script.Num", )"
      R"("fields": {"value": 4}}}}}}, {"object": "passes.link_script.Input", "fields": )"
      R"({"patterns": [".a", ".b"], "file": "*"}}, {"object": "passes.link_script.Input", )"
      R"("fields": {"patterns": [".text"], "file": "*zstring.asm"}}, {"object": )"
      R"("passes.link_script.Assign", "fields": {"symbol": "x", "expr": {"object": )"
      R"("passes.link_script.Binary", "fields": {"op": "-", "left": {"object": )"
      R"("passes.link_script.Sym", "fields": {"name": "x"}}, "right": {"object": )"
      R"("passes.link_script.Num", "fields": {"value": 1}}}}}}, {"object": )"
      R"("passes.link_script.Assign", "fields": {"symbol": "y", "expr": {"object": )"
      R"("passes.link_script.Dot", "fields": {}}}}], "region": null, "load_region": null}}, )"
      R"({"object": "passes.link_script.Assign", "fields": {"symbol": "z", "expr": {"object": )"
      R"("passes.link_script.Call", "fields": {"fn": "ALIGN", "args": [{"object": )"
      R"("passes.link_script.Num", "fields": {"value": 2}}]}}}}]}})";
  const std::string got = c6502::json::dump(c6502::passes::link_checks::script_json(s));
  CHECK(got == want);
  // The JSON reads back into the same script.
  const ls::Script back =
      c6502::passes::link_checks::script_from_json(c6502::json::parse(want));
  CHECK(c6502::json::dump(c6502::passes::link_checks::script_json(back)) == want);
}

TEST_CASE("parse_script reports a value past the port's 125 bits") {
  // Python takes the number, 2**127, which the 128-bit values can't.
  CHECK_THROWS_AS(
      ls::parse_script("MEMORY { r : ORIGIN = 0x80000000000000000000000000000000, LENGTH = 1 }"),
      InternalError);
  CHECK_THROWS_AS(ls::parse_script("MEMORY { r : ORIGIN = 1 << 200, LENGTH = 1 }"),
                  InternalError);
  // A shift that ends within the bound is computed.
  CHECK(ls::parse_script("MEMORY { r : ORIGIN = 1 << 100 >> 99, LENGTH = 7 >> 300 }")
            .regions.at("r")
            .origin == 2);
}

namespace {

// Writes `bytes` to a file of the test's own and returns its path.
std::string write_temp(const std::string& name, const std::string& bytes) {
  const auto path = std::filesystem::temp_directory_path() / name;
  std::ofstream out(path, std::ios::binary);
  out << bytes;
  return path.string();
}

std::string read_error(const std::string& path) {
  try {
    ls::read_script(path);
  } catch (const CompileError& e) {
    return std::string(e.python_type()) + ": " + e.what();
  }
  return "no error";
}

}  // namespace

TEST_CASE("read_script reads the file in the locale's encoding with universal newlines") {
  const std::string enc = ls::locale_encoding();
#ifdef _WIN32
  // compile.py run normally on this machine reads cp1252, Windows' ANSI
  // code page for English.
  if (std::getenv("PYTHONUTF8") == nullptr) CHECK(enc == "cp1252");
#endif
  // The bytes C3 A9 are "é" in UTF-8 and "Ã©" in cp1252, so the
  // message's repr differs between Linux and Windows. A CR LF and a
  // bare CR each end a line.
  const std::string path =
      write_temp("c6502_link_script_utf8.ld", "MEMORY {\r\n r : ORIGIN = 0, LENGTH = 1\r}\r\n\xC3\xA9");
  if (enc == "utf-8") {
    CHECK(read_error(path) == "LinkError: " + path + ": line 4: can't read '\xC3\xA9'");
  } else if (enc == "cp1252") {
    CHECK(read_error(path) == "LinkError: " + path + ": line 4: can't read '\xC3\x83\xC2\xA9'");
  }
  // 0x81 has no character in cp1252, and 0xE9 alone isn't UTF-8.
  const std::string bad = write_temp("c6502_link_script_bad.ld", "/* \x81\xE9 */");
  if (enc == "utf-8") {
    CHECK(read_error(bad) == "LinkError: " + bad +
                                 ": 'utf-8' codec can't decode byte 0x81 in position 3: invalid "
                                 "start byte");
  } else if (enc == "cp1252") {
    CHECK(read_error(bad) == "LinkError: " + bad +
                                 ": 'charmap' codec can't decode byte 0x81 in position 3: "
                                 "character maps to <undefined>");
  }
  std::filesystem::remove(path);
  std::filesystem::remove(bad);
}

TEST_CASE("read_script reports a missing file and a script error as LinkErrors") {
  CHECK(read_error("no/such/script.ld") == "LinkError: no/such/script.ld: No such file or directory");
  const std::string path = write_temp("c6502_link_script_err.ld", "ENTRY(main)\n");
  CHECK(read_error(path) == "LinkError: " + path +
                                ": 'ENTRY' isn't supported; a script has MEMORY and SECTIONS "
                                "commands");
  std::filesystem::remove(path);
  const ls::Script ok = ls::read_script(
      write_temp("c6502_link_script_ok.ld", "MEMORY { r : ORIGIN = 1, LENGTH = 2 }\n"));
  CHECK(ok.regions.at("r").length == 2);
}
