// Compares that branch on flags another instruction set, or that fold
// into fewer tests, run in the simulator in both pipelines against
// values computed in C++, from tests/test_flag_compares.py.
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

// The values tests/test_flag_compares.py's `_PROGRAM` stores in
// `checks`, computed as the Python test's `_expected` computes them.
std::vector<int> expected_checks() {
  int s = 0;
  for (int v = 0; v < 256; ++v) {
    const int c = v & 0x80 ? v - 256 : v;
    s = (s * 3 + (v < 0x80 ? 1 : 2)) & 0xFFFF;
    s = (s * 3 + (v >= 1 ? 3 : 4)) & 0xFFFF;
    s = (s * 3 + (c < 0 ? 5 : c > 0x1C ? 6 : 7)) & 0xFFFF;
  }
  const auto rows = [](int top, int btm) {
    int n = 0, r = top;
    while (r + 1 < btm) {
      n += 1;
      r = (r + 1) & 0xFF;
    }
    return n & 0xFF;
  };
  const auto pair = [](int e) { return e == 0 || e == 1 ? 8 : 9; };
  const auto hi = [](int e) {
    if (e & 0x8000) return 1;
    return !(e > 0xFF) ? 2 : 3;
  };
  return {rows(0, 0),
          rows(0, 1),
          rows(0, 5),
          rows(3, 255),
          rows(254, 255),
          rows(255, 255),
          rows(250, 2),
          pair(0),
          pair(1),
          pair(2),
          (pair(0x100) + pair(0x101) * 16) & 0xFF,
          (hi(0) + hi(0xFF) * 4 + hi(0x100) * 16) & 0xFF,
          (hi(0x8000) + hi(0xFFFF) * 4 + hi(0x7FFF) * 16) & 0xFF,
          1 * 16 + 3,
          0 * 16 + 4,
          s & 0xFF,
          s >> 8};
}

}  // namespace

TEST_CASE("flag_compares sim: every shape computes the values in both pipelines") {
  const char* src = R"c(
#include <stdint.h>
uint8_t checks[17];
static bool flag;
uint8_t A, B;
uint8_t bit7(uint8_t v) { if (v < 0x80) return 1; return 2; }
uint8_t one(uint8_t v) { if (v >= 1) return 3; return 4; }
uint8_t pos(int8_t c) { if (c < 0) return 5; if (c > 0x1C) return 6; return 7; }
uint8_t rows(uint8_t top, uint8_t btm) {
    uint8_t n = 0;
    for (uint8_t r = top; r + 1 < btm; r++) n++;
    return n;
}
uint8_t pair(uint16_t e) { if (e == 0 || e == 1) return 8; return 9; }
uint8_t hi(uint16_t e) {
    if (e & 0x8000) return 1;
    if (!(e > 0xFF && !(e & 0x8000))) return 2;
    return 3;
}
void nots(void) { A = !flag; B = (flag ? 3 : 2) + 1; }
int main(void) {
    uint16_t sum = 0;
    for (uint16_t v = 0; v < 256; v++) {
        sum = sum * 3 + bit7((uint8_t)v);
        sum = sum * 3 + one((uint8_t)v);
        sum = sum * 3 + pos((int8_t)v);
    }
    checks[0] = rows(0, 0); checks[1] = rows(0, 1); checks[2] = rows(0, 5);
    checks[3] = rows(3, 255); checks[4] = rows(254, 255);
    checks[5] = rows(255, 255); checks[6] = rows(250, 2);
    checks[7] = pair(0); checks[8] = pair(1); checks[9] = pair(2);
    checks[10] = pair(0x100) + pair(0x101) * 16;
    checks[11] = hi(0) + hi(0xFF) * 4 + hi(0x100) * 16;
    checks[12] = hi(0x8000) + hi(0xFFFF) * 4 + hi(0x7FFF) * 16;
    flag = false; nots(); checks[13] = A * 16 + B;
    flag = true; nots(); checks[14] = A * 16 + B;
    checks[15] = (uint8_t)sum; checks[16] = sum >> 8;
    return 0;
}
)c";
  const std::vector<int> checks = expected_checks();
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    const sim::SimResult r = s.run(20'000'000);
    CHECK_FALSE(r.timed_out);
    const std::int64_t base = s.symbol("checks");
    std::vector<int> got;
    for (std::size_t k = 0; k < checks.size(); ++k) got.push_back(r.memory[base + k]);
    CHECK(got == checks);
  }
}

}  // namespace c6502::suite
