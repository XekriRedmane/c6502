#include <cstdint>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("carry_in_add sim: each sum with the carry in gives its value") {
  std::uint32_t n = 4;
  n = (n * 3 + 5) & 0xFFFF;
  for (const std::uint32_t p : {0u, 1u, 0xFFu, 0x1FFu, 0xFFFEu, 0xFFFFu}) {
    for (const std::uint32_t b : {0u, 1u, 0x7Fu, 0xFEu, 0xFFu}) n = (n * 7 + ((p + b + 1) & 0xFFFF)) & 0xFFFF;
  }
  const char* src = R"c(
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
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == n);
  }
}

}  // namespace c6502::suite
