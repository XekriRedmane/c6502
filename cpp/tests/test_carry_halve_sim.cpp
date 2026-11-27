// The simulator tests of tests/test_carry_halve.py: halved 9-bit sums,
// whose bit 8 src/passes/carry_halve.h takes from the carry of the low
// byte's add, give the values computed here in both pipelines.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

const std::string kSrc = R"c(
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
)c";
auto expected() {
  std::vector<int> v = {0, 1, 2, 127, 128, 200, 254, 255};
  auto n = 0;
  for (const auto& a : v) {
    for (const auto& b : v) {
      n = (n * 7 + (a + b) / 2) & 0xFFFF;
    }
    auto col = (3 + (a + 1) / 2) & 0xFF;
    n = (n * 5 + ((a + 1) >> 1) + col) & 0xFFFF;
  }
  return n;
}

TEST_CASE("carry_halve: values") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto r = build_sim(kSrc, optimize).run();
    CHECK(r.return_int() == expected());
  }
}

}  // namespace
}  // namespace c6502::suite
