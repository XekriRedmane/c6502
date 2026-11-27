// The tests of tests/test_object_byte_view.py that read the TAC and the
// `-S` text: `((uint8_t *)&S)[i]` and `*((uint8_t *)&S + i)` on an
// object with static storage are an indexed access of the object's
// symbol, `LDA S,X` and `STA S,X`, or `S+k` for a constant index.
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

const std::string kPrelude =
    "#include <stdint.h>\n"
    "extern uint32_t Z [[c6502::zeropage(0x8E)]];\n"
    "static uint32_t S;\n"
    "#define ZB ((uint8_t *)&Z)\n"
    "#define SB ((uint8_t *)&S)\n";

std::string tac(const std::string& src) { return suite::run_source(suite::Stage::kTac, src); }

std::string strip(const std::string& s) {
  std::size_t b = 0, e = s.size();
  while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
  while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
  return s.substr(b, e - b);
}

// The stripped lines of function `name` in `asm_text`, joined by
// newlines, up to the next label at column 0 other than a local one.
std::string function(const std::string& asm_text, const std::string& name) {
  const std::vector<std::string> lines = suite::lines(asm_text);
  std::size_t start = 0;
  while (start < lines.size() && lines[start] != name + ":") ++start;
  REQUIRE(start < lines.size());
  std::string out;
  for (std::size_t i = start + 1; i < lines.size(); ++i) {
    const std::string& l = lines[i];
    if (!l.empty() && !std::isspace(static_cast<unsigned char>(l[0])) && l[0] != '.') break;
    out += (i == start + 1 ? "" : "\n") + strip(l);
  }
  return out;
}

std::vector<std::string> words(const std::string& text) {
  std::istringstream in(text);
  std::vector<std::string> out;
  for (std::string w; in >> w;) out.push_back(w);
  return out;
}

}  // namespace

TEST_CASE("object_byte_view: the TAC reads through an IndexedLoad") {
  const std::string t = tac(kPrelude + "uint8_t f(uint8_t i) { return ZB[3 - i]; }\n");
  CHECK(suite::contains(t, "IndexedLoad("));
  CHECK(suite::contains(t, "name='Z'"));
  CHECK_FALSE(suite::contains(t, "GetAddress("));
}

TEST_CASE("object_byte_view: the TAC writes through an IndexedSymbolStore") {
  const std::string t = tac(kPrelude + "void f(uint8_t i, uint8_t v) { SB[i] = v; }\n");
  CHECK(suite::contains(t, "IndexedSymbolStore("));
  CHECK_FALSE(suite::contains(t, "GetAddress("));
}

TEST_CASE("object_byte_view: a constant index is a constant byte in the TAC") {
  const std::string t = tac(kPrelude + "uint8_t f(void) { return SB[2]; }\n");
  CHECK(suite::contains(t, "IndexedLoad("));
  CHECK(suite::contains(t, "ConstUChar(\n              value=2"));
}

TEST_CASE("object_byte_view: a variable index uses absolute,X") {
  const std::string src = kPrelude +
                          "uint8_t f(uint8_t i) { return ZB[3 - i]; }\n"
                          "void g(uint8_t i, uint8_t v) { ZB[i] = v; }\n"
                          "uint8_t h(uint8_t i) { return *(ZB + i); }\n";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string asm_text = suite::codegen(src, optimize);
    CHECK(suite::contains(function(asm_text, "f"), "LDA   Z,X"));
    CHECK(suite::contains(function(asm_text, "g"), "STA   Z,X"));
    CHECK(suite::contains(function(asm_text, "h"), "LDA   Z,X"));
    CHECK_FALSE(suite::contains(function(asm_text, "f"), "DPTR"));
    CHECK_FALSE(suite::contains(function(asm_text, "g"), "DPTR"));
  }
}

TEST_CASE("object_byte_view: the optimized read is a byte array's") {
  // `i` arrives in A, and `f` stores it to subtract it from 3.
  const std::string asm_text =
      suite::codegen(kPrelude + "uint8_t f(uint8_t i) { return ZB[3 - i]; }\n", true);
  CHECK(words(function(asm_text, "f")) ==
        words("SUBROUTINE STA __local_f__i LDA #$03 SEC SBC __local_f__i TAX LDA Z,X RTS"));
}

TEST_CASE("object_byte_view: a constant index is absolute") {
  const std::string src = kPrelude +
                          "uint8_t f(void) { return ZB[2]; }\n"
                          "void g(uint8_t v) { *(ZB + 1) = v; }\n"
                          "uint8_t h(void) { return *ZB; }\n";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string asm_text = suite::codegen(src, optimize);
    CHECK(suite::contains(function(asm_text, "f"), "LDA   Z+2"));
    CHECK(suite::contains(function(asm_text, "g"), "STA   Z+1"));
    CHECK_FALSE(suite::contains(function(asm_text, "f"), ",X"));
    CHECK_FALSE(suite::contains(function(asm_text, "g"), "DPTR"));
  }
}

TEST_CASE("object_byte_view: a constant added to the index goes in the offset") {
  const std::string asm_text =
      suite::codegen(kPrelude + "uint8_t f(uint8_t i) { return SB[i + 1]; }\n", true);
  CHECK(suite::contains(function(asm_text, "f"), "LDA   S+1,X"));
}

TEST_CASE("object_byte_view: a compound assignment is indexed") {
  const std::string asm_text = suite::codegen(kPrelude +
                                                  "uint8_t m;\n"
                                                  "void f(uint8_t i) { ZB[i] |= m; }\n"
                                                  "void g(uint8_t i) { ++*(ZB + i); }\n",
                                              true);
  const std::string body = function(asm_text, "f");
  CHECK(suite::contains(body, "LDA   Z,X"));
  CHECK(suite::contains(body, "STA   Z,X"));
  // The increment is in place (`inc_peephole`'s indexed form).
  CHECK(suite::contains(function(asm_text, "g"), "INC   Z,X"));
}

TEST_CASE("object_byte_view: 2-byte elements are indexed") {
  const std::string asm_text =
      suite::codegen(kPrelude + "uint16_t f(uint8_t i) { return ((uint16_t *)&S)[i]; }\n", true);
  const std::string body = function(asm_text, "f");
  CHECK(suite::contains(body, "S,X"));
  CHECK(suite::contains(body, "S+1,X"));
}

TEST_CASE("object_byte_view: an automatic object keeps the pointer access") {
  // An automatic object has no link-time address to index from.
  const std::string t = tac(kPrelude +
                            "uint8_t f(uint8_t i) {\n"
                            "    uint32_t a = S;\n"
                            "    return ((uint8_t *)&a)[i];\n"
                            "}\n");
  CHECK_FALSE(suite::contains(t, "IndexedLoad("));
  CHECK(suite::contains(t, "GetAddress("));
}
