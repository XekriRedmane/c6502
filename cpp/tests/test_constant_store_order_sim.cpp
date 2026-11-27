// Runs of stores grouped by the value stored
// (src/passes/constant_store_order.h), in programs with the shape of
// Lode Runner's `fill_hole`, run in both pipelines, from
// tests/test_constant_store_order.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("constant_store_order sim: bricks copy their row and column") {
  // Each brick whose timer runs out copies its row and column into
  // locals and globals, and the loop counter stays in X. The values
  // include 0, 128, and 255. Bricks 3, 2, and 0 fill, 808 is summed, and
  // then GROW = 3 and GCOL = 9.
  const char* src = R"c(
#include <stdint.h>
const uint8_t ROWS[4] = {3, 0, 255, 7};
const uint8_t COLS[4] = {9, 128, 1, 0};
uint8_t TIMERS[4] = {1, 2, 1, 1};
uint8_t MAP[8][10];
uint8_t GROW, GCOL;
uint16_t sum;

static void fill(uint8_t i) {
    uint8_t row = ROWS[i];
    uint8_t col = COLS[i];
    GROW = row;
    GCOL = col;
    uint8_t r = row & 7;
    uint8_t c = col % 10;
    MAP[r][c] += 1;
    sum += (uint16_t)(row * 3) + col + MAP[r][c];
}

int main(void) {
    for (uint8_t i = 3; (int8_t)i >= 0; i--) {
        if (--TIMERS[i] != 0)
            continue;
        fill(i);
    }
    return sum + GROW + GCOL;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 820);
  }
}

TEST_CASE("constant_store_order sim: a store into the source of a later load") {
  // `T[1] = col` writes a byte that `T[i]` reads on the next iteration of
  // the loop, so the value of `row` can't be loaded again after it. The
  // rows read are 8, 7, 3 (stored by the iteration before), and 5.
  const char* src = R"c(
#include <stdint.h>
uint8_t T[4] = {5, 6, 7, 8};
const uint8_t C[4] = {1, 2, 3, 4};
uint8_t G, H;

int main(void) {
    uint16_t s = 0;
    for (uint8_t i = 3; (int8_t)i >= 0; i--) {
        uint8_t row = T[i];
        uint8_t col = C[i];
        T[1] = col;
        G = row;
        H = col;
        s += (uint16_t)(row * 16) + col + T[i];
    }
    return s + G + H + T[1];
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 407);
  }
}

}  // namespace c6502::suite
