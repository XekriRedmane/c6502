// The `-S -O` text of tests/test_carry_in_add.py's program. Its
// simulator runs stay in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("carry_in_add: p + b + 1 is one add with the carry set") {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(R"(
#include <stdint.h>
static const uint8_t table[] = {3, 'a', 'b', 'c', 0, 255};
static const uint8_t *after(const uint8_t *s) { return s + s[0] + 1; }
static uint16_t sum(uint16_t p, uint8_t b) { return p + b + 1; }
uint16_t main(void) {
    static const uint16_t ps[] = {0, 1, 0xFF, 0x1FF, 0xFFFE, 0xFFFF};
    static const uint8_t bs[] = {0, 1, 0x7F, 0xFE, 0xFF};
    uint16_t n = (uint16_t)(after(table) - table);
    n = n * 3 + (uint16_t)(after(table + 4) - table);
    for (uint8_t i = 0; i < 6; i++)
        for (uint8_t j = 0; j < 5; j++)
            n = n * 7 + sum(ps[i], bs[j]);
    return n;
}
)", options);
  const std::size_t start = text.find("\nafter:");
  const std::size_t end = text.find("\nsum:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  CHECK(suite::contains(body, "SEC"));
  CHECK_FALSE(suite::contains(body, "#$01"));
}
