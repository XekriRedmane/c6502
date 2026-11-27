// The tests of tests/test_zp_globals.py that compile a whole program.
// They check the placement of `[[c6502::zeropage]]` objects, the code
// that reads them and `__zp_init`, the promotion of busy globals under
// `-O`, and the links that place every unit's objects for the whole
// program.
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "py_regex.h"

namespace {

namespace suite = c6502::suite;
using Equs = std::map<std::string, std::int64_t>;

// Returns the `name EQU $hex` bindings of `asm_text`, the lines that
// `^(\S+)\tEQU\t\$([0-9A-F]+)$` matches with `re.M`.
Equs equ(const std::string& asm_text) {
  Equs out;
  for (const std::string& line : suite::lines(asm_text)) {
    const std::size_t tab = line.find("\tEQU\t$");
    if (tab == std::string::npos || tab == 0) continue;
    const std::string name = line.substr(0, tab);
    const std::string hex = line.substr(tab + 6);
    if (name.find_first_of(" \t") != std::string::npos || hex.empty() ||
        hex.find_first_not_of("0123456789ABCDEF") != std::string::npos) {
      continue;
    }
    out[name] = std::stoll(hex, nullptr, 16);
  }
  return out;
}

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

// Returns `text.split(sep)[1]`, the text between the first `sep` and
// the next.
std::string second_part(const std::string& text, const std::string& sep) {
  const std::size_t at = text.find(sep);
  REQUIRE(at != std::string::npos);
  const std::size_t from = at + sep.size();
  const std::size_t next = text.find(sep, from);
  return text.substr(from, next == std::string::npos ? std::string::npos : next - from);
}

// Returns `text[text.index(sep):]`.
std::string from(const std::string& text, const std::string& sep) {
  const std::size_t at = text.find(sep);
  REQUIRE(at != std::string::npos);
  return text.substr(at);
}

suite::Options optimized() {
  suite::Options o;
  o.optimize = true;
  return o;
}

}  // namespace

TEST_CASE("zp_globals: automatic addresses follow the fixed ones") {
  const std::string asm_text = suite::codegen(
      "unsigned char a [[c6502::zeropage]];\n"
      "unsigned int b [[c6502::zeropage(0x27)]];\n"
      "unsigned char c [[c6502::zeropage]][3];\n"
      "int main(void) { return a + b + c[1]; }\n",
      true);
  const Equs e = equ(asm_text);
  CHECK(e.at("b") == 0x27);
  CHECK(e.at("a") == 0x26);
  CHECK(e.at("c") == 0x29);
  // No storage in the data segment.
  CHECK_FALSE(py_search("(?m)^a:|^b:|^c:", asm_text));
}

TEST_CASE("zp_globals: a zero-page object must have static storage") {
  for (const char* src : {
           "int main(void) { unsigned char a "
           "[[c6502::zeropage]]; a = 1; return a; }\n",
           "int main(void) { for ([[c6502::zeropage]] int i = 0; "
           "i < 2; i++) ; return 0; }\n",
       }) {
    INFO(src);
    const suite::Error e = suite::compile_error(suite::Stage::kCodegen, src, optimized());
    CHECK((suite::is_a(e.type, "TypeCheckError") || suite::is_a(e.type, "ParserError")));
  }
}

TEST_CASE("zp_globals: placements that overlap, pass $FF, or don't fit are errors") {
  const std::vector<std::pair<std::string, std::string>> bad = {
      {"overlap",
       "unsigned int a [[c6502::zeropage(0x40)]];\n"
       "unsigned char b [[c6502::zeropage(0x41)]];\n"},
      {"past $FF", "unsigned int a [[c6502::zeropage(0xFF)]];\n"},
      {"full", "unsigned char a [[c6502::zeropage]][91];\n"},
      {"two addresses",
       "unsigned char a [[c6502::zeropage(0x40)]];\n"
       "unsigned char a [[c6502::zeropage(0x41)]];\n"},
  };
  for (const auto& [what, decls] : bad) {
    INFO(what);
    const suite::Error e = suite::compile_error(
        suite::Stage::kCodegen, decls + "int main(void) { return a; }\n", optimized());
    CHECK(suite::is_a(e.type, "TypeCheckError"));
  }
}

TEST_CASE("zp_globals: an extern without an address is left to the linker") {
  // Another unit defines it, so this one neither places nor
  // initializes it, and lists it for the linker.
  const std::string asm_text = suite::codegen(
      "extern unsigned char a [[c6502::zeropage]];\n"
      "int main(void) { return a; }\n",
      true);
  CHECK(equ(asm_text).count("a") == 0);
  CHECK_FALSE(suite::contains(asm_text, "__zp_init"));
  CHECK(suite::contains(asm_text, "; zp a size=1 at=auto def=false init="));
}

TEST_CASE("zp_globals: a placement error is a ZpGlobalError") {
  const suite::Error e = suite::compile_error(suite::Stage::kCodegen,
                                              "unsigned char a [[c6502::zeropage]][91];\n"
                                              "int main(void) { return a[0]; }\n",
                                              optimized());
  CHECK(suite::is_a(e.type, "ZpGlobalError"));
}

TEST_CASE("zp_globals: an extern with an address is reserved and not initialized") {
  const std::string asm_text = suite::codegen(
      "extern unsigned char k [[c6502::zeropage(0x90)]];\n"
      "int main(void) { return k; }\n",
      true);
  CHECK(equ(asm_text).at("k") == 0x90);
  CHECK_FALSE(suite::contains(asm_text, "__zp_init"));
}

TEST_CASE("zp_globals: a redeclaration adds to the earlier declaration") {
  const std::string asm_text = suite::codegen(
      "extern unsigned char a [[c6502::zeropage(0x50)]];\n"
      "unsigned char a = 4;\n"
      "int main(void) { return a; }\n",
      true);
  CHECK(equ(asm_text).at("a") == 0x50);
  CHECK(suite::contains(asm_text, "__zp_init"));
}

TEST_CASE("zp_globals: a zero-page pointer is the base of (zp),Y") {
  // `p` gets its value from a global, so the compiler doesn't know it
  // holds `buf`'s address (when it does, `p[2]` reads `buf+2`).
  const std::string asm_text = suite::codegen(
      "unsigned char buf[4] = {1, 2, 3, 4};\n"
      "unsigned char *src = buf;\n"
      "unsigned char *p [[c6502::zeropage]];\n"
      "int main(void) { p = src; return p[2]; }\n",
      true);
  CHECK(suite::contains(asm_text, "(p),Y"));
  CHECK_FALSE(suite::contains(second_part(asm_text, "main:"), "DPTR"));
}

TEST_CASE("zp_globals: __zp_init stores each object's initial bytes") {
  const std::string asm_text = suite::codegen(
      "unsigned int w [[c6502::zeropage]] = 0x1234;\n"
      "unsigned char z [[c6502::zeropage]];\n"
      "int main(void) { return w + z; }\n",
      true);
  const std::string init = second_part(asm_text, "__zp_init:");
  CHECK(suite::contains(init, "LDA   #$00\n   STA   z"));
  CHECK(suite::contains(init, "LDA   #$34\n   STA   w\n"));
  CHECK(suite::contains(init, "LDA   #$12\n   STA   w+1"));
}

TEST_CASE("zp_globals: pinned bytes are kept out of the compiler's pools") {
  // Pin $80-$8F, where zp_abi parameters and private pools would
  // start. None of the compiler's slots may land there.
  const Equs e = equ(suite::codegen(
      "unsigned char g [[c6502::zeropage(0x80)]][16];\n"
      "unsigned int f(unsigned int a, unsigned int b) {\n"
      "    unsigned int t = a * 3 + b;\n"
      "    return t ^ (a + 1);\n"
      "}\n"
      "int main(void) { g[3] = 9; return f(g[3], 2) + g[3]; }\n",
      true));
  for (const auto& [name, addr] : e) {
    if (name.starts_with("__zpabi_") || name.starts_with("__local_")) {
      INFO(name << " at " << addr);
      CHECK_FALSE((0x80 <= addr && addr < 0x90));
    }
  }
}

namespace {

// A pointer and a counter used in a loop, a constant, an array, and a
// variable read once.
const char* const kPromote =
    "#include <stdint.h>\n"
    "uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};\n"
    "uint8_t *p = buf;\n"
    "uint16_t total;\n"
    "const uint8_t step = 1;\n"
    "uint8_t once = 9;\n"
    "int main(void) {\n"
    "    for (uint8_t i = 0; i < 8; i += step) total += p[i] + buf[i];\n"
    "    return total + once;\n"
    "}\n";

}  // namespace

TEST_CASE("zp_globals: -O promotes busy scalars into zero page") {
  const std::string asm_text = suite::codegen(kPromote, true);
  const Equs e = equ(asm_text);
  CHECK(e.count("total") == 1);
  CHECK(e.count("p") == 1);
  // The pointer is the `(zp),Y` base directly.
  CHECK(suite::contains(asm_text, "(p),Y"));
  CHECK_FALSE(suite::contains(asm_text, "\np:"));
  CHECK(py_search(R"(; zp total size=2 at=auto def=true init=\$00,\$00 weight=\d+)", asm_text));
  const std::string init = from(asm_text, "__zp_init:");
  CHECK(suite::contains(init, "STA   total+1"));
  CHECK(suite::contains(init, "LDA   #<buf"));
}

TEST_CASE("zp_globals: -O leaves a constant, an array, and a variable read once in place") {
  const Equs e = equ(suite::codegen(kPromote, true));
  for (const char* name : {"step", "buf", "once"}) {
    INFO(name);
    CHECK(e.count(name) == 0);
  }
}

TEST_CASE("zp_globals: declared objects take zero page before promoted ones") {
  const Equs e = equ(suite::codegen(
      std::string("#include <stdint.h>\n"
                  "uint8_t fill [[c6502::zeropage]][89];\n") +
          kPromote,
      true));
  CHECK(e.at("fill") == 0x26);
  // One byte is left, and both candidates are 2 bytes.
  CHECK(e.count("total") == 0);
  CHECK(e.count("p") == 0);
}

TEST_CASE("zp_globals: nothing is promoted without -O") {
  CHECK(equ(suite::codegen(kPromote, false)).count("total") == 0);
}

namespace {

const std::string kHeader =
    "#include <stdint.h>\n"
    "extern uint8_t *src [[c6502::zeropage]];\n"
    "extern uint8_t total [[c6502::zeropage]];\n"
    "extern uint16_t big [[c6502::zeropage(0x90)]];\n"
    "uint8_t sum(uint8_t n);\n";
// Defines `src`, `total`, and `big`, and calls `sum`.
const std::string kUnitA = kHeader +
                           "uint8_t *src [[c6502::zeropage]];\n"
                           "uint8_t total [[c6502::zeropage]] = 5;\n"
                           "uint16_t big [[c6502::zeropage(0x90)]] = 1000;\n"
                           "uint8_t buf[4] = {1, 2, 3, 4};\n"
                           "int main(void) { src = buf; return sum(4) + big; }\n";
// Defines `count`, which it places at $26 on its own, where unit A
// puts `src`, and reads and writes A's objects.
const std::string kUnitB = kHeader +
                           "uint8_t count [[c6502::zeropage]] = 2;\n"
                           "uint8_t sum(uint8_t n) {\n"
                           "    for (uint8_t i = 0; i < n; i++) total += src[i] * count;\n"
                           "    big += total;\n"
                           "    return total;\n"
                           "}\n";

// Compiles each source with `-O` to an object and links them. Returns
// the exit status, the linked program as `linked_text` gives it (empty
// when the link fails), and stderr.
std::tuple<long long, std::string, std::string> link_units(
    const std::vector<std::string>& sources) {
  const suite::TempDir tmp;
  std::vector<std::string> paths;
  for (std::size_t k = 0; k < sources.size(); ++k) {
    const std::string path = tmp.file("u" + std::to_string(k) + ".o");
    REQUIRE(suite::compile_object(sources[k], path) == 0);
    paths.push_back(path);
  }
  const std::string out = tmp.file("out");
  const auto o = suite::link(paths, out);
  const long long rc = o.status.value_or(-1);
  return {rc, rc == 0 ? suite::linked_text(out) : "", o.err()};
}

}  // namespace

TEST_CASE("zp_globals: a link places every unit's objects for the whole program") {
  const auto [rc, text, err] = link_units({kUnitA, kUnitB});
  REQUIRE_MESSAGE(rc == 0, err);
  const Equs e = equ(text);
  CHECK(e.at("src") == 0x26);
  CHECK(e.at("total") == 0x28);
  CHECK(e.at("count") == 0x29);
  CHECK(e.at("big") == 0x90);
  // The slots keep off `big`'s bytes.
  for (const auto& [name, addr] : e) {
    if (name.starts_with("__")) {
      INFO(name);
      CHECK(addr != 0x90);
      CHECK(addr != 0x91);
    }
  }
}

TEST_CASE("zp_globals: a link merges the units' __zp_init routines into one") {
  const auto [rc, text, err] = link_units({kUnitA, kUnitB});
  REQUIRE_MESSAGE(rc == 0, err);
  CHECK(suite::count(text, "__zp_init:") == 1);
  const std::string init = from(text, "__zp_init:");
  for (const char* store : {"STA   src+1", "STA   total", "STA   count", "STA   big+1"}) {
    INFO(store);
    CHECK(suite::contains(init, store));
  }
}

namespace {

// Unit A leaves 4 bytes below $80, and unit B promotes three busy
// 2-byte counters, so one goes back in the data segment.
const char* const kFull =
    "#include <stdint.h>\n"
    "uint8_t fill [[c6502::zeropage]][86];\n"
    "uint16_t count(void);\n"
    "int main(void) { fill[85] = 3; return count() + fill[85]; }\n";
const char* const kCounters =
    "#include <stdint.h>\n"
    "uint16_t a = 1, b = 2, c = 3;\n"
    "uint16_t count(void) {\n"
    "    for (uint8_t i = 0; i < 10; i++) { a += 1; b += a; c += b; }\n"
    "    return a + b + c;\n"
    "}\n";

}  // namespace

TEST_CASE("zp_globals: a promoted object that doesn't fit goes back in the data segment") {
  const auto [rc, text, err] = link_units({kFull, kCounters});
  REQUIRE_MESSAGE(rc == 0, err);
  const Equs e = equ(text);
  std::vector<std::string> placed;
  std::vector<std::string> back;
  for (const char* n : {"a", "b", "c"}) {
    (e.count(n) ? placed : back).push_back(n);
  }
  CHECK(placed.size() == 2);
  REQUIRE(back.size() == 1);
  CHECK(suite::contains(text, "\n" + back[0] + ":\n   DC.B"));
  CHECK_FALSE(suite::contains(from(text, "__zp_init:"), "STA   " + back[0] + "\n"));
}

TEST_CASE("zp_globals: a promoted pointer has to fit in zero page") {
  // Both pointers are `(zp),Y` bases, and only one fits.
  const auto [rc, text, err] = link_units({
      "unsigned char fill [[c6502::zeropage]][88];\n"
      "unsigned char sum(void);\n"
      "int main(void) { return sum(); }\n",
      "unsigned char buf[4] = {1, 2, 3, 4};\n"
      "unsigned char *p = buf, *q = buf;\n"
      "unsigned char sum(void) {\n"
      "    unsigned char s = 0;\n"
      "    for (unsigned char i = 0; i < 4; i++) s += p[i] + q[i];\n"
      "    return s;\n"
      "}\n",
  });
  CHECK(rc == 1);
  CHECK(suite::contains(err, "(zp),Y"));
}

TEST_CASE("zp_globals: the link's errors for objects the units disagree on") {
  const std::string main = "int main(void) { return 0; }\n";
  struct Case {
    const char* what;
    std::vector<std::string> sources;
    const char* message;
  };
  const std::vector<Case> cases = {
      {"declared but not defined",
       {"extern unsigned char g [[c6502::zeropage]];\n"
        "unsigned char f(void) { return g; }\n" +
        main},
       "no unit defines it"},
      {"defined twice",
       {"unsigned char g [[c6502::zeropage]];\n" + main,
        "unsigned char g [[c6502::zeropage]];\n"
        "unsigned char f(void) { return g; }\n"},
       "defined in 2 units"},
      {"sizes differ",
       {"unsigned char g [[c6502::zeropage]];\n" + main,
        "extern unsigned int g [[c6502::zeropage]];\n"
        "unsigned int f(void) { return g; }\n"},
       "different sizes"},
      {"addresses differ",
       {"unsigned char g [[c6502::zeropage(0x40)]];\n" + main,
        "extern unsigned char g [[c6502::zeropage(0x41)]];\n"
        "unsigned char f(void) { return g; }\n"},
       "different addresses"},
      {"fixed objects overlap",
       {"unsigned int g [[c6502::zeropage(0x40)]];\n" + main,
        "unsigned char h [[c6502::zeropage(0x41)]];\n"
        "unsigned char f(void) { return h; }\n"},
       "overlaps"},
  };
  for (const Case& c : cases) {
    INFO(c.what);
    const auto [rc, text, err] = link_units(c.sources);
    CHECK(rc == 1);
    CHECK(suite::contains(err, c.message));
  }
}
