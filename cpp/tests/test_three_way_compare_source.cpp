// The `-S -O` text of tests/test_three_way_compare.py's shapes, in which
// a 2-byte unsigned ordering compare and an equality compare of the same
// values split into compares of the high bytes and then the low ones.
// The simulator runs stay in Python.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns the `-S -O` text of `f` in `source`, up to `main`.
std::string f_of(const std::string& source) {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(source, options);
  const std::size_t start = text.find("\nf:");
  REQUIRE(start != std::string::npos);
  const std::size_t end = text.find("\nmain:", start);
  REQUIRE(end != std::string::npos);
  return text.substr(start, end - start);
}

}  // namespace

TEST_CASE("three_way_compare: no shape has a 2-byte subtract") {
  // A 2-byte ordering compare lowers to `SBC`, and a split one to a
  // `CMP` of each byte.
  const std::vector<std::string> programs = {
      R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        if (a < b) return 1;
        if (a == b) return 2;
        return 3;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)",
      R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        if (a < b) return 1;
        if (a != b) return 3;
        return 2;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)",
      R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        if (a > b) return 3;
        if (a == b) return 2;
        return 1;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)",
      R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        if (a < b) goto less;
        if (a != b) goto greater;
        return 2;
    less:
        return 1;
    greater:
        return 3;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)",
      R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        (void)b;
        if (a < 0x1234u) return 1;
        if (a == 0x1234u) return 2;
        return 3;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)",
      R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        uint8_t ka[2], kb[2];
        ka[0] = (uint8_t)a; ka[1] = (uint8_t)(a >> 8);
        kb[0] = (uint8_t)b; kb[1] = (uint8_t)(b >> 8);
        uint16_t x = ((uint16_t)ka[1] << 8) | ka[0];
        uint16_t y = ((uint16_t)kb[1] << 8) | kb[0];
        if (x < y) return 1;
        if (x == y) return 2;
        return 3;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)",
      R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        uint8_t c = (uint8_t)a;
        if (c < b) return 1;
        if (c == b) return 2;
        return 3;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)",
  };
  for (const std::string& program : programs) {
    INFO(program);
    CHECK_FALSE(suite::contains(f_of(program), "SBC"));
  }
}

TEST_CASE("three_way_compare: a signed compare is left alone") {
  CHECK(suite::contains(
      f_of(R"(#include <stdint.h>
uint16_t t[14] = {0x0000u, 0x0001u, 0x00FFu, 0x0100u, 0x0101u, 0x01FFu, 0x1233u, 0x1234u, 0x1235u, 0x1134u, 0x1334u, 0x8000u, 0xFFFEu, 0xFFFFu};
static uint8_t f(uint16_t a, uint16_t b) {
        if ((int)a < (int)b) return 1;
        if (a == b) return 2;
        return 3;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 14; i++)
        for (uint8_t j = 0; j < 14; j++)
            n = (uint16_t)(n * 5u + f(t[i], t[j]));
    return n;
}
)"),
      "SBC"));
}
