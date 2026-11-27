// The simulator tests of tests/test_zp_local_allocation.py: programs
// whose zero-page pools src/passes/zp_local_allocation.h fills, falls
// back from, or grows give the values C does.
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

const std::string kFullPool = R"c(
#include <stdint.h>
uint8_t pool [[c6502::zeropage(0x80)]][128];
static uint8_t g(uint8_t x) { return x + 1; }
uint8_t f(uint8_t a, uint8_t b) {
    uint8_t s = g(a);
    uint8_t t = g(b);
    return s + t + a + b;
}
int main(void) { pool[0] = f(3, 4); return pool[0]; }
)c";
// `kFullPool` with a byte `d` in X before the parameters of `f` and
// `g`, so that `a`, `b`, and `x` arrive in slots. Without a register
// named in the declaration, `a` and `b` arrive in A and X, and so does
// `x`.
const std::string kFullPoolSlots = R"c(
#include <stdint.h>
uint8_t pool [[c6502::zeropage(0x80)]][128];
static uint8_t g(uint8_t d [[c6502::in_x]], uint8_t x) { return x + d; }
uint8_t f(uint8_t d [[c6502::in_x]], uint8_t a, uint8_t b) {
    uint8_t s = g(d, a);
    uint8_t t = g(d, b);
    return s + t + a + b;
}
int main(void) { pool[0] = f(1, 3, 4); return pool[0]; }
)c";
namespace unit_fallback {
// A unit compiled on its own takes the free bytes below the pool,
// around the runtime's block, before it spills out of zero page, as
// the linker does. The asm passes that follow values through zero page
// then see its slots.
TEST_CASE("zp_local_allocation: unit fallback: runs") {
  for (const auto& src : {kFullPool, kFullPoolSlots}) {
    INFO("slots=" << (src == kFullPoolSlots));
    CHECK(build_sim(src, true).run().return_int() == 16);
  }
}

TEST_CASE("zp_local_allocation: register parameters with a frame") {
  // With the pool full, f's locals are on the soft stack, so its
  // prologue runs, and it does its arithmetic in A, where `a`
  // arrives. The prologue's PHA / PLA was dropped, since
  // `kills_a` didn't count the prologue, and `b` was stored with
  // `TXA; STA (FP),Y`, which no pass saw writing A. f returned 8
  // or 141.
  auto src =
      replace_all(kFullPool, "uint8_t f(uint8_t a, uint8_t b)", "uint8_t f(register uint8_t a, register uint8_t b)");
  CHECK(contains(src, "register uint8_t b"));
  CHECK(build_sim(src, true).run().return_int() == 16);
}

}  // namespace unit_fallback

// Lode Runner's `guard_find_candidate_row_above`. Its preliminary
// register allocation used 6 bytes, and with a pool of 6 the final
// allocation had no two adjacent free bytes left for one of its row
// pointers, so a byte value went to a soft-stack frame.
const std::string kTightPool = R"c(
#include <stdint.h>
#include <stdbool.h>
extern uint8_t BG [[c6502::address(0x6000)]][16][28];
extern uint8_t LIMIT [[c6502::address(0x01)]];
bool is_floor(uint8_t sprite) { return sprite == 1 || sprite == 2; }
uint8_t f(uint8_t row, uint8_t col) {
    for (;;) {
        if (BG[row][col] != 3)
            return row;
        uint8_t ladder_row = row;
        row--;
        if (col != 0 && (is_floor(BG[ladder_row][col - 1]) ||
                         BG[row][col - 1] == 4)) {
            if (row <= LIMIT)
                return row;
        }
        if (col != 27 && (is_floor(BG[ladder_row][col + 1]) ||
                          BG[row][col + 1] == 4)) {
            if (row <= LIMIT)
                return row;
        }
        if (row == 0)
            return row;
    }
}
int main(void) {
    LIMIT = 9;
    for (uint8_t r = 0; r < 16; r++) BG[r][5] = 3;
    BG[9][4] = 4;
    return f(12, 5);
}
)c";
namespace pool_growth {
// A function whose final register allocation spills a value the
// preliminary one didn't gets a larger pool.
TEST_CASE("zp_local_allocation: pool growth: runs") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(kTightPool, optimize).run().return_int() == 9);
  }
}

}  // namespace pool_growth

}  // namespace
}  // namespace c6502::suite
