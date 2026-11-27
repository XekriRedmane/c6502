#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::int64_t run_optimized(const char* src) {
  const sim::SimResult r = build_sim(src, Pipeline::kOptimized).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  return r.return_int_signed();
}

}  // namespace

TEST_CASE("indexed_load sim: loads from an unsigned char array") {
  CHECK(run_optimized(R"c(#include <stdint.h>
static uint8_t arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
int main(void) {
    int sum = 0;
    for (int i = 0; i < 8; i = i + 1) sum = sum + arr[i];
    return sum;
}
)c") == 0 + 1 + 2 + 3 + 4 + 5 + 6 + 7);
}

TEST_CASE("indexed_load sim: both bytes of a uint16_t element") {
  CHECK((run_optimized(R"c(#include <stdint.h>
static uint16_t arr[4] = {0x1234, 0x5678, 0x9ABC, 0xDEF0};
int main(void) {
    return arr[2];
}
)c") & 0xFFFF) == 0x9ABC);
}

TEST_CASE("indexed_load sim: the four bytes of a long element") {
  CHECK(run_optimized(R"c(long arr[4] = {1, 2, 3, 4};
int test_arr(void) {
    for (int i = 0; i < 4; i = i + 1) {
        if (arr[i] != i + 1) return 1;
    }
    return 0;
}
int main(void) { return test_arr(); }
)c") == 0);
}

TEST_CASE("indexed_load sim: a negative signed char index from a constant base") {
  // `buf[i]` through a constant base with `int8_t i = -1` reads $3000.
  // `recognize_indexed_load` accepted the SignExtend of i, and
  // `LDA $3001,X` read $3100.
  const char* src = R"c(#include <stdint.h>
static uint8_t * const buf = (uint8_t * const)0x3001;
uint8_t get(int8_t i) { return buf[i]; }
int main(void) {
    *(uint8_t *)0x3000 = 42;
    *(uint8_t *)0x3100 = 7;
    return get(-1);
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run(5'000'000).return_int() == 42);
  }
}

}  // namespace c6502::suite
