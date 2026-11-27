#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("jump_table sim: each case and the default give their values") {
  const char* src = R"c(
#include <stdint.h>
uint8_t r[25];
static uint8_t pick(uint8_t n) {
    switch (n) {
    case 0: return 7;
    case 1: return 11;
    case 2: return 13;
    case 4: return 17;
    case 5: return 19;
    case 6: return 23;
    case 7: return 29;
    case 9: return 31;
    case 10: return 37;
    case 11: return 41;
    default: return 1;
    }
}
static uint8_t pick_high(uint8_t n) {
    switch (n) {
    case 100: return 2;
    case 101: return 3;
    case 102: return 5;
    case 103: return 7;
    case 104: return 11;
    case 105: return 13;
    case 106: return 17;
    case 107: return 19;
    case 108: return 23;
    default: return 0;
    }
}
int main(void) {
    for (uint8_t i = 0; i != 13; i++)
        r[i] = pick(i);
    r[13] = pick(200);
    for (uint8_t i = 0; i != 11; i++)
        r[14 + i] = pick_high((uint8_t)(99 + i));
    return 0;
}
)c";
  const std::vector<int> want = {7, 11, 13, 1, 17, 19, 23, 29, 1, 31, 37, 41, 1,
                                 1, 0,  2,  3, 5,  7,  11, 13, 17, 19, 23, 0};
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    const sim::SimResult r = s.run();
    const std::int64_t base = s.symbol("r");
    std::vector<int> got;
    for (int k = 0; k < 25; ++k) got.push_back(r.memory[base + k]);
    CHECK(got == want);
  }
}

}  // namespace c6502::suite
