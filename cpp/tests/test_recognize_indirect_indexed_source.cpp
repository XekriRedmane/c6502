// The `-S -O` text of tests/test_recognize_indirect_indexed.py's
// accesses through a pointer at a byte index, which are `(p),Y`. The
// simulator runs stay in Python.
#include <stdexcept>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns `text[text.index(from):text.index(to)]`.
std::string between(const std::string& text, const std::string& from, const std::string& to) {
  const std::size_t start = text.find(from);
  const std::size_t end = text.find(to);
  if (start == std::string::npos || end == std::string::npos) {
    throw std::runtime_error("substring not found");
  }
  return end > start ? text.substr(start, end - start) : std::string();
}

}  // namespace

TEST_CASE("recognize_indirect_indexed: a word read at an even index reads with INY") {
  // `p[i + k]` with a byte `i` whose largest value plus `k` fits in a
  // byte is `(p),Y` with Y = `i + k`.
  const std::string text =
      suite::codegen(R"(#include <stdint.h>
static uint8_t data[300];
uint8_t *table;
uint16_t word(uint8_t j) {
    uint8_t i = j << 1;
    return ((uint16_t)table[i] << 8) | table[i + 1];
}
uint16_t odd(uint8_t i) {
    return ((uint16_t)table[i] << 8) | table[i + 1];
}
int main(void) {
    table = data;
    data[254] = 0x12; data[255] = 0x34; data[256] = 0x56;
    return (word(127) == 0x1234) + 2 * (odd(255) == 0x3456);
}
)", true);
  const std::string word = between(text, "\nword:\n", "\nodd:\n");
  CHECK(suite::contains(word, "INY"));
  CHECK_FALSE(suite::contains(word, "ADC"));
}

TEST_CASE("recognize_indirect_indexed: the accesses share the byte index in Y") {
  // `p + i` read by several 1-byte loads and stores, as in `p[i] &= m`,
  // is `(p),Y` with Y = `i` in each, and a reader `(p + i) + k` with `k`
  // outside 0-255 is `(t),Y` with `t = p + k`.
  const std::string text =
      suite::codegen(R"(#include <stdint.h>
static uint8_t screen[0x2100];
static const uint8_t mask[2] = {0xF0, 0x0F};
uint8_t *row;
void close(uint8_t *line, uint8_t x, uint8_t m) {
    line[x] &= mask[m];
}
void open(uint8_t *line, uint8_t x, uint8_t m) {
    const uint8_t *background = line + 0x2000;
    line[x] |= background[x] & mask[m];
}
void open_global(uint8_t x) {
    uint8_t v = row[x];
    row = screen;
    row[x] = v | 1;
}
int main(void) {
    for (uint16_t k = 0; k < 0x100; k++) {
        screen[k] = (uint8_t)k;
        screen[k + 0x2000] = 0xFF;
    }
    close(screen + 0x10, 0xF5, 0);
    open(screen + 0x10, 0x05, 1);
    row = screen + 0x80;
    open_global(0x20);
    return screen[0x105] + screen[0x15] * 256 + screen[0x20];
}
)", true);
  CHECK_FALSE(suite::contains(between(text, "\nclose:\n", "\nopen:\n"), "ADC"));
  const std::string opened = between(text, "\nopen:\n", "\nopen_global:\n");
  INFO(opened);
  // One add, of 0x20 to the pointer's high byte.
  CHECK(suite::count(opened, "ADC") == 1);
}

TEST_CASE("recognize_indirect_indexed: a load through a parameter at a byte index is short") {
  // The pointer arrives in A and X, and `copy` stores it into a pair of
  // its own zero-page bytes at entry, which are the base of `(zp),Y`.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "static volatile uint8_t result;\n"
      "[[c6502::zp_abi]]\n"
      "void copy(uint8_t *src, uint8_t idx) {\n"
      "    result = src[idx];\n"
      "}\n"
      "int main(void) { return 0; }\n",
      true);
  CHECK(suite::contains(text, "LDA   (__local_copy__src),Y"));
  const std::size_t start = text.find("copy:");
  REQUIRE(start != std::string::npos);
  const std::size_t end = text.find("\n\n", start);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end - start);
  // No 16-bit add, and no copy into DPTR.
  CHECK_FALSE(suite::contains(body, "CLC"));
  CHECK_FALSE(suite::contains(body, "STA   DPTR"));
}

TEST_CASE("recognize_indirect_indexed: a global pointer stored a constant on another path isn't constant") {
  // `P = 0` on one path made the recognizer take P for the constant 0
  // everywhere, since the def index gives a global's last store, and
  // `P->look` was read through a computed copy of P.
  const std::string text = suite::codegen(
      "#include <stdint.h>\n"
      "struct mob { uint8_t a, b, c, d, look; };\n"
      "extern struct mob *P [[c6502::zeropage(0xF4)]];\n"
      "void next(void);\n"
      "void walk(uint8_t n) {\n"
      "    if (n == 0) { P = 0; return; }\n"
      "    while (P->look >= 0x15) next();\n"
      "}\n",
      true);
  const std::size_t at = text.find("walk:");
  REQUIRE(at != std::string::npos);
  const std::string body = text.substr(at);
  CHECK(suite::contains(body, "LDY   #$04"));
  CHECK(suite::contains(body, "LDA   (P),Y"));
  CHECK_FALSE(suite::contains(body, "ADC"));
}
