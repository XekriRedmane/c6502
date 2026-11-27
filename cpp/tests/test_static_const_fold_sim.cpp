// Programs whose `static const` scalars and array elements fold to
// constants, and whose nested constant adds reassociate, run under `-O`,
// from tests/test_static_const_fold.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

sim::SimResult run_optimized(const char* src) {
  const sim::SimResult r = build_sim(src, true).run(5'000'000);
  CHECK_FALSE(r.timed_out);
  return r;
}

}  // namespace

TEST_CASE("static_const_fold sim: a static const int's value") {
  CHECK((run_optimized(R"c(static const int magic = 0x1234;
int main(void) { return magic + 1; }
)c").return_int() & 0xFFFF) == 0x1235);
}

TEST_CASE("static_const_fold sim: a const array element's value") {
  CHECK((run_optimized(R"c(#include <stdint.h>
static const uint16_t arr[3] = {0x1234, 0x5678, 0x9ABC};
int main(void) { return arr[2]; }
)c").return_int() & 0xFFFF) == 0x9ABC);
}

TEST_CASE("static_const_fold sim: a store through a static const pointer at a folded offset") {
  // 0x4000 + 0x200 + 5 = 0x4205.
  CHECK(run_optimized(R"c(#include <stdint.h>
static uint8_t * const buf = (uint8_t * const)0x4000;
static const uint16_t offsets[3] = {0x100, 0x200, 0x300};
int main(void) { buf[offsets[1] + 5] = 0x42; return 0; }
)c").memory[0x4205] == 0x42);
}

TEST_CASE("static_const_fold sim: reassociated constants add up") {
  // main is called with no arguments, and the boot stub leaves 0 in
  // col, so main returns 300.
  CHECK((run_optimized(R"c(static const int a = 100;
static const int b = 200;
int main(int col) { return a + b + col; }
)c").return_int() & 0xFFFF) == 300);
}

}  // namespace c6502::suite
