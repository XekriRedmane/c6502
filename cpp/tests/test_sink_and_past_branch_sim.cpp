// The TAC sinker that moves `ZeroExtend; BitwiseAnd; Truncate` past a
// `JumpIfMasked`, run in both pipelines, from
// tests/test_sink_and_past_branch.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("sink_and_past_branch sim: a ZeroExtend that another instruction reads is kept") {
  // The pass dropped such a ZeroExtend with the rest of the trio, so
  // `g16 = w` read a name that nothing wrote. y = 10 - 5 + 3 = 8, and
  // g16 = 3 from the second call.
  const char* src = R"c(#include <stdint.h>
uint8_t bob[8] = {0x85, 3, 0, 0, 0, 0, 0, 0};
uint8_t y = 10;
int g16;
void step(uint8_t i) {
    uint8_t b = bob[i];
    int w = b;
    uint8_t mag = b & 0x7F;
    if (b & 0x80) y -= mag; else y += mag;
    g16 = w;
}
int main(void) { step(0); step(1); return g16 + y * 256; }
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run(200'000).return_int() == 8 * 256 + 3);
  }
}

}  // namespace c6502::suite
