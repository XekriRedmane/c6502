// The `-S -O` text of tests/test_flag_compares.py's compares, which
// branch on flags another instruction set or fold into fewer tests.
#include <algorithm>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

using Lines = std::vector<std::string>;

Lines body_of(const std::string& source, const std::string& name) {
  return suite::function_body(suite::codegen(source, true), name);
}

bool has(const Lines& lines, const std::string& line) {
  return std::find(lines.begin(), lines.end(), line) != lines.end();
}

Lines first(const Lines& lines, std::size_t n) {
  return Lines(lines.begin(), lines.begin() + std::min(n, lines.size()));
}

}  // namespace

TEST_CASE("flag_compares: a compare with 0x80 after a load is BMI") {
  const Lines body = body_of(
      "#include <stdint.h>\n"
      "extern volatile uint8_t KBD [[c6502::address(0xC000)]];"
      "uint8_t k(void) { uint8_t c; if ((c = KBD) < 0x80) return 0;"
      " return c; }",
      "k");
  CHECK(first(body, 2) == Lines{"LDA KBD", "BMI .if_end@0"});
}

TEST_CASE("flag_compares: e == 0 || e == 1 is one test of e & ~1") {
  const Lines body = body_of(
      "#include <stdint.h>\n"
      "uint16_t find(void); void draw(void);"
      "void f(void) { uint16_t e = find();"
      " if (e == 0 || e == 1) return; draw(); }",
      "f");
  REQUIRE(body.size() > 1);
  CHECK(body[1] == "AND #$FE");
  CHECK_FALSE(has(body, "CMP #$01"));
}

TEST_CASE("flag_compares: !b of a bool is b ^ 1, and a choice of constants plus one joins them") {
  const Lines body = body_of(
      "#include <stdint.h>\n"
      "static bool flag;"
      "extern uint8_t A [[c6502::address(0x5A00)]];"
      "extern uint8_t B [[c6502::address(0x5A01)]];"
      "void f(void) { A = !flag; B = (flag ? 3 : 2) + 1; }",
      "f");
  CHECK(first(body, 3) == Lines{"LDA flag", "EOR #$01", "STA A"});
  CHECK(has(body, "LDA #$04"));
  CHECK_FALSE(has(body, "ADC #$01"));
}

TEST_CASE("flag_compares: a signed compare of a byte plus one that can't be negative is unsigned") {
  const Lines body = body_of(
      "#include <stdint.h>\n"
      "extern uint8_t W [[c6502::zeropage(0x23)]];"
      "void copy(uint8_t row);"
      "void f(uint8_t top) { for (uint8_t r = top; r + 1 < W; r++)"
      " copy(r); }",
      "f");
  CHECK_FALSE(has(body, "EOR #$80"));
}

TEST_CASE("flag_compares: an unsigned e > 0xFF tests the high byte") {
  const Lines body = body_of(
      "#include <stdint.h>\n"
      "uint16_t find(void);"
      "uint8_t g(void) { uint16_t e = find();"
      " if (e > 0xFF) return 1; return 2; }",
      "g");
  CHECK(first(body, 3) == Lines{"JSR find", "TXA", "BEQ .if_end@0"});
}
