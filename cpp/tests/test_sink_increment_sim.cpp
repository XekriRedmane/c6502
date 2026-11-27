// The simulator tests of tests/test_sink_increment.py: programs whose
// `Y = X + c` src/passes/optimization/sink_increment.h moves past the
// last read of `X` give the values C does in both pipelines.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

void both(const auto& src, const auto& expected) {
  for (bool optimize : {false, true}) {
    auto r = build_sim(src, optimize).run(200'000);
    CHECK_FALSE_MESSAGE(r.timed_out, cat("optimize=", optimize));
    CHECK_MESSAGE(r.return_int() == expected, cat("optimize=", optimize));
  }
}

TEST_CASE("sink_increment: static written before last read") {
  // The add of the static `s` moved past `s = 5`, so h got 6.
  std::string src = R"c(static unsigned s;
unsigned g, h;
void f(void) { unsigned t = s + 1; s = 5; g = s; h = t; }
int main(void) { s = 1; f(); return h * 10 + g; }
)c";
  both(src, 25);
}

TEST_CASE("sink_increment: src y postincrement chain") {
  std::string src = R"c(#include <stdint.h>
uint8_t tab[4] = {3, 5, 7, 9};
uint8_t out[3];
void copy3(uint8_t y) {
    out[0] = tab[y++];
    out[1] = tab[y++];
    out[2] = tab[y];
}
int main(void) { copy3(1); return out[0] + out[1] * 16; }
)c";
  both(src, 5 + 7 * 16);
}

TEST_CASE("sink_increment: post decrement tests") {
  // `x-- != 0` tests the decremented value against -1, for each
  // width and signedness, with the count running out at 0 and
  // at a match.
  std::string src = R"c(#include <stdint.h>
uint8_t tab[8] = {9, 8, 7, 6, 5, 4, 3, 2};
uint8_t find(uint8_t c) {
    uint8_t x = 7;
    do { if (tab[x] == c) return x + 10; } while (x-- != 0);
    return 0;
}
int8_t sdown(int8_t n) { int8_t k = 0;
    while (n-- != -3) k++; return k; }
uint16_t wdown(uint16_t n) { uint16_t k = 0;
    while (n--) k += 2; return k; }
int main(void) {
    return find(5) + find(1) * 100 + sdown(4) * 1000
        + (int)wdown(300);
}
)c";
  both(src, 4 + 10 + 0 + 7 * 1000 + 600);
}

}  // namespace
}  // namespace c6502::suite
