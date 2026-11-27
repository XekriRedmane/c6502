#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("cross_jump sim: the joined blocks compute each value") {
  const char* src = R"c(
#include <stdint.h>
uint16_t t[5] = {1, 2, 4, 7, 0x300};
uint16_t r[5];
static uint16_t scale(uint16_t a, uint16_t b) {
    uint16_t p;
    if (b == 2)
        p = a << 1;
    else if (b == 4)
        p = a << 2;
    else
        p = a + b;
    return p;
}
int main(void) {
    for (uint8_t i = 0; i != 5; i++)
        r[i] = scale(0x81, t[i]);
    return 0;
}
)c";
  const std::vector<std::uint64_t> want = {0x82, 0x102, 0x204, 0x88, 0x381};
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    sim::Simulation s = build_sim(src, optimize);
    const sim::SimResult r = s.run();
    std::vector<std::uint64_t> got;
    for (int k = 0; k < 5; ++k) got.push_back(r.word(s.symbol("r") + 2 * k));
    CHECK(got == want);
  }
}

}  // namespace c6502::suite
