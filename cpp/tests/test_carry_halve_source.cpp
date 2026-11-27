// The `-S -O` text of tests/test_carry_halve.py's program. Its
// simulator runs stay in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("carry_halve: a halved sum takes bit 8 from the carry, with no high byte") {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(R"(
#include <stdint.h>
uint8_t col;
static uint8_t half(uint8_t a, uint8_t b) { return (a + b) / 2; }
static uint8_t round_up(uint8_t s) { return (s + 1) >> 1; }
static void center(uint8_t s) { col += (s + 1) / 2; }
uint16_t main(void) {
    static const uint8_t v[] = {0, 1, 2, 127, 128, 200, 254, 255};
    uint16_t n = 0;
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++)
            n = n * 7 + half(v[i], v[j]);
        col = 3;
        center(v[i]);
        n = n * 5 + round_up(v[i]) + col;
    }
    return n;
}
)", options);
  const std::size_t start = text.find("\nhalf:");
  const std::size_t end = text.find("\nmain:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  CHECK_FALSE(suite::contains(body, "LSR"));
  CHECK(suite::count(body, "ROR") == 3);
}
