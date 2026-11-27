// Functions whose divide and remainder of the same operands
// src/passes/optimization/divmod_hoist.h computes together, run in both
// pipelines for each byte value, from tests/test_divmod_hoist.py.
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("divmod_hoist sim: the hoisted divides and remainders give each value") {
  constexpr int sel = 7;
  // The Python module's source with `sel` put in its `%d`.
  const char* src = R"c(
#include <stdint.h>
struct iob { uint8_t pad[4]; uint8_t track, sector; };
struct iob IOB;
struct iob *P = &IOB;
uint8_t V, Q, R, S;
uint8_t OUT[256 * 5];
void g(uint8_t s) { P->sector = s % 13; P->track = 0x11 + s / 13; }
void f(void) { if (V >= 24) Q = V / 24; R = V % 24; }
uint8_t h(uint8_t sel) {
    if (V >= 24)
        S = sel + V / 24;
    return V % 24 * 4 + 0x20;
}
int main(void) {
    for (uint16_t v = 0; v < 256; v++) {
        g((uint8_t)v);
        V = (uint8_t)v;
        Q = 0xEE;
        f();
        S = 0xEE;
        uint8_t x = h(7);
        OUT[v * 5] = IOB.sector;
        OUT[v * 5 + 1] = IOB.track;
        OUT[v * 5 + 2] = Q;
        OUT[v * 5 + 3] = R;
        OUT[v * 5 + 4] = x ^ S;
    }
    return 0;
}
)c";
  std::vector<int> want;
  for (int v = 0; v < 256; ++v) {
    const int q = v >= 24 ? v / 24 : 0xEE;
    const int s = v >= 24 ? (sel + v / 24) & 0xFF : 0xEE;
    const int x = (v % 24 * 4 + 0x20) & 0xFF;
    want.insert(want.end(), {v % 13, (0x11 + v / 13) & 0xFF, q, v % 24, x ^ s});
  }
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    const sim::SimResult result = s.run(20'000'000);
    CHECK_FALSE(result.timed_out);
    const std::int64_t base = s.symbol("OUT");
    std::vector<int> got;
    for (std::size_t k = 0; k < want.size(); ++k) got.push_back(result.memory[base + k]);
    CHECK(got == want);
  }
}

}  // namespace c6502::suite
