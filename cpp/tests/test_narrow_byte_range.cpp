// The tests of tests/test_narrow_byte_range.py that compiled a program:
// the lowerings that value_range, narrow_byte_range, and strength
// reduction's rule for a nonnegative signed dividend lead to in
// tac_to_asm (`inline_byte_divide`, `shift_loop`,
// `inline_byte_product`, and the pairing of a divide with a remainder).
#include <string>
#include <string_view>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns the `-S` text of `fn` from the newline before its label up to
// the end of the line before the next function's label, as the Python
// test's `_body` did.
std::string body(const std::string& text, std::string_view fn) {
  const std::string label = "\n" + std::string(fn) + ":";
  const std::size_t at = text.find(label);
  REQUIRE(at != std::string::npos);
  const std::string rest = text.substr(at);
  const std::size_t next = rest.find(":\n   SUBROUTINE", fn.size() + 3);
  if (next == std::string::npos) return rest;
  return rest.substr(0, rest.rfind('\n', next - 1));
}

}  // namespace

TEST_CASE("narrow_byte_range: a byte divided by a constant is inline") {
  const std::string b = body(suite::codegen("#include <stdint.h>\n"
                                            "uint8_t f(uint8_t x) { return x / 20; }\n",
                                            true),
                             "f");
  CHECK(suite::contains(b, "SBC   #$14"));
  CHECK_FALSE(suite::contains(b, "JSR"));
  CHECK_FALSE(suite::contains(b, "JMP"));
}

TEST_CASE("narrow_byte_range: a byte's / and % share one loop") {
  const std::string b = body(suite::codegen("#include <stdint.h>\n"
                                            "uint16_t f(uint8_t x) { uint8_t c = x % 20;"
                                            " uint8_t r = x / 20; return c | (uint16_t)r << 8; }\n",
                                            true),
                             "f");
  CHECK(suite::count(b, "SBC   #$14") == 1);
  CHECK(suite::contains(b, "ADC   #$14"));
}

TEST_CASE("narrow_byte_range: a word's / and % share one call") {
  const std::string b = body(suite::codegen("#include <stdint.h>\n"
                                            "uint16_t q;\n"
                                            "uint16_t f(uint16_t v, uint16_t p) "
                                            "{ q = v / p; v %= p; return v; }\n",
                                            true),
                             "f");
  CHECK(suite::count(b, "udivmod16") == 1);
}

TEST_CASE("narrow_byte_range: shifts by a variable are loops") {
  const std::string src =
      "#include <stdint.h>\n"
      "unsigned int f(unsigned int x, uint8_t n) { return x << n; }\n"
      "int g(int x, uint8_t n) { return x >> n; }\n"
      "uint8_t h(uint8_t x, uint8_t n) { return x >> n; }\n";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const std::string text = suite::codegen(src, optimize);
    CHECK_FALSE(suite::contains(text, "JSR   asl"));
    CHECK_FALSE(suite::contains(text, "JSR   asr"));
    CHECK_FALSE(suite::contains(text, "JSR   lsr"));
    CHECK(suite::contains(text, "DEX"));
  }
}

TEST_CASE("narrow_byte_range: a shift of a promoted byte is logical") {
  // `(style >> 5)` inside a product had called `asr16`.
  const std::string b = body(suite::codegen("#include <stdint.h>\n"
                                            "uint16_t f(uint8_t p, uint8_t s) "
                                            "{ return p * (((s >> 5) & 3) + 1); }\n",
                                            true),
                             "f");
  CHECK(suite::count(b, "LSR\n") == 5);
  CHECK_FALSE(suite::contains(b, "JSR"));
  CHECK_FALSE(suite::contains(b, "mul16"));
}

TEST_CASE("narrow_byte_range: a product of two bytes is inline") {
  const std::string b = body(suite::codegen("#include <stdint.h>\n"
                                            "uint16_t f(uint8_t a, uint8_t b) { return a * b; }\n",
                                            true),
                             "f");
  CHECK(suite::contains(b, "ROR   HARGS"));
  CHECK_FALSE(suite::contains(b, "mul16"));
}

TEST_CASE("narrow_byte_range: a product of a word and a byte calls mul16") {
  const std::string b = body(suite::codegen("#include <stdint.h>\n"
                                            "uint16_t f(uint16_t a, uint8_t b) { return a * b; }\n",
                                            true),
                             "f");
  CHECK(suite::contains(b, "mul16"));
}
