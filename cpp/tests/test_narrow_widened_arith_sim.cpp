// Programs whose widened byte arithmetic
// src/passes/optimization/narrow_widened_arith.h narrows, run in both
// pipelines, from tests/test_narrow_widened_arith.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("narrow_widened_arith sim: a global shifted on two paths") {
  const char* src = R"c(#include <stdint.h>
uint8_t alive = 0x81, other = 6, key;
void f(void) {
    if (key == 3) {
        other++;
        alive >>= 1;
        return;
    }
    if (key == 4)
        alive >>= 1;
}
int main(void) {
    key = 3; f();
    key = 4; f();
    key = 5; f();
    return alive + (other << 8);
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 0x0720);
  }
}

TEST_CASE("narrow_widened_arith sim: a byte shifted left by a variable count") {
  // The shift is a 1-byte shift, which `tac_to_asm` lowers as a loop of
  // `ASL`s, where it had called `asl16`. A count of 8 or more gives 0 at
  // either width.
  const char* src = R"c(#include <stdint.h>
uint8_t s(uint8_t v, uint8_t n) { v <<= n; return v; }
int main(void) {
    int bad = 0;
    if (s(3, 2) != 12) bad |= 1;
    if (s(0x81, 1) != 2) bad |= 2;
    if (s(5, 8) != 0) bad |= 4;
    if (s(5, 12) != 0) bad |= 8;
    if (s(5, 0) != 5) bad |= 16;
    return bad;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 0);
  }
}

}  // namespace c6502::suite
