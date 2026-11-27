// Programs whose `LoadAddress` writes both bytes of its destination
// while a loop counter is live, run in both pipelines, from
// tests/test_optimize_asm_regalloc.py. The asm SSA versions each byte of
// a Pseudo on its own, so a destination written two bytes at a time is
// kept out of it. Before that, regalloc gave the loop counter and the
// address's high byte the same callee-saved byte, and the loops computed
// wrong offsets.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

void check_both(const char* src, std::int64_t want) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int_signed() == want);
  }
}

}  // namespace

TEST_CASE("optimize_asm_regalloc sim: an int8_t counter subscripting a uint16_t array") {
  // The SignExtend of `i` reads the byte after the loop counter's, which
  // the in-loop LoadAddress's high byte overwrote.
  check_both(R"c(#include <stdint.h>
static uint16_t table[3] = {0xAA, 0xBB, 0xCC};
int main(void) {
    uint16_t sum = 0;
    int8_t i;
    for (i = 0; i < 3; i++) sum += table[i];
    return sum;
}
)c", 0xAA + 0xBB + 0xCC);
}

TEST_CASE("optimize_asm_regalloc sim: an int counter storing into a uint8_t array") {
  check_both(R"c(#include <stdint.h>
static uint8_t out[5];
int main(void) {
    int i;
    for (i = 0; i < 5; i++) out[i] = (uint8_t)(i * 7);
    return out[3];
}
)c", 3 * 7);
}

TEST_CASE("optimize_asm_regalloc sim: a loop that loads the addresses of two arrays") {
  // Neither LoadAddress's bytes may share a slot with the other's or
  // with the loop counter.
  check_both(R"c(#include <stdint.h>
static uint16_t a[3] = {1, 2, 3};
static uint16_t b[3] = {10, 20, 30};
int main(void) {
    uint16_t sum = 0;
    int8_t i;
    for (i = 0; i < 3; i++) sum += a[i] + b[i];
    return sum;
}
)c", (1 + 10) + (2 + 20) + (3 + 30));
}

TEST_CASE("optimize_asm_regalloc sim: the address of a local read in a loop") {
  check_both(R"c(int main(void) {
    int x = 5;
    int *p = &x;
    int sum = 0;
    int i;
    for (i = 0; i < 4; i++) sum += *p;
    return sum;
}
)c", 5 * 4);
}

}  // namespace c6502::suite
