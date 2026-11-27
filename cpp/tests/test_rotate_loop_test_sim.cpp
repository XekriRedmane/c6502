// Loops whose test src/passes/rotate_loop_test.h moves from the top to
// the bottom, run in both pipelines, from tests/test_rotate_loop_test.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("rotate_loop_test sim: the rotated loops compute their values") {
  const char* src = R"c(
    #include <stdint.h>
    uint8_t buf[8] = {5, 6, 7, 0x8D, 1, 2, 3, 4};
    uint8_t zero = 0;
    static uint8_t a_mod_3(register uint8_t a) {
        while (a >= 3) a -= 3;
        return a;
    }
    static uint8_t count(void) {
        uint8_t c = 0;
        while (buf[c] != 0x8D) c++;
        return c;
    }
    static uint8_t sum_skip(void) {
        uint8_t s = 0;
        for (uint8_t i = 0; i != 8; i++) {
            if (i == 2) continue;
            s += buf[i];
        }
        return s;
    }
    static uint8_t none(void) {
        uint8_t s = 1;
        for (uint8_t i = 0; i != zero; i++) s += i;
        return s;
    }
    int main(void) {
        for (uint8_t i = 0; i != 3; i++) buf[4 + i] = i;
        return a_mod_3(14) + 10 * count() + 100 * none()
            + 1000 * (sum_skip() & 0x0F);
    }
    )c";
  // a_mod_3(14) = 2, count() = 3, none() = 1, and sum_skip() is 5 + 6 +
  // 0x8D + 0 + 1 + 2 + 4 = 159, 0x9F, whose low four bits are 15.
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult out = build_sim(src, optimize).run(5'000'000);
    CHECK_FALSE(out.timed_out);
    CHECK(out.return_int() == 2 + 30 + 100 + 15000);
  }
}

}  // namespace c6502::suite
