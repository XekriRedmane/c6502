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

void run_optimized(const std::string& src, std::int64_t expected) {
  auto s = build_sim(src, true);
  auto result = s.run(200'000);
  CHECK_FALSE_MESSAGE(result.timed_out, cat("sim timed out at ", result.cycles));
  CHECK(result.return_int_signed() == expected);
}

TEST_CASE("loop_rotate: simple int countdown") {
  // sum 15..0 = 15*16/2 = 120
  std::string src = R"c(int main(void) {
    int sum = 0;
    for (int x = 15; x >= 0; x--) sum += x;
    return sum;
}
)c";
  run_optimized(src, 120);
}

TEST_CASE("loop_rotate: simple int countdown init zero") {
  // Single iteration: body runs once, x == 0.
  std::string src = R"c(int main(void) {
    int sum = 0;
    for (int x = 0; x >= 0; x--) sum += 1;
    return sum;
}
)c";
  run_optimized(src, 1);
}

TEST_CASE("loop_rotate: int8 countdown") {
  // sum 15..0 from int8_t counter. Verifies the SignExtend-
  // in-cond shape rotates and computes correctly.
  std::string src = R"c(#include <stdint.h>
int main(void) {
    int sum = 0;
    for (int8_t x = 15; x >= 0; x--) sum += x;
    return sum;
}
)c";
  run_optimized(src, 120);
}

void both(const auto& src, const auto& expected) {
  for (bool optimize : {false, true}) {
    auto result = build_sim(src, optimize).run(200'000);
    CHECK_FALSE_MESSAGE(result.timed_out, cat("optimize=", optimize));
    CHECK_MESSAGE(result.return_int() == expected, cat("optimize=", optimize));
  }
}

TEST_CASE("loop_rotate: init on conditional path not rotated") {
  // The last write to i before the loop is inside the `if`, so
  // the -1 parameter reaches the test when c is 0, and the
  // first test fails. The rotated loop ran the body once and
  // returned 1.
  std::string src = R"c(#include <stdint.h>
uint8_t count(int8_t i, uint8_t c) {
    uint8_t k = 0;
    if (c) i = 5;
    for (; i >= 0; i--) k++;
    return k;
}
int main(void) { return count(-1, 0) + count(-1, 1) * 2; }
)c";
  both(src, 12);
}

TEST_CASE("loop_rotate: init through conditional temp not rotated") {
  // `c ? -1 : 5` writes its result temp on two paths, and the
  // write in instruction order is the in-range 5.
  std::string src = R"c(#include <stdint.h>
uint8_t count(uint8_t c) {
    uint8_t k = 0;
    for (int8_t i = c ? -1 : 5; i >= 0; i--) k++;
    return k;
}
int main(void) { return count(1) + count(0) * 2; }
)c";
  both(src, 12);
}

TEST_CASE("loop_rotate: address taken counter not rotated") {
  // The store through p writes i after its constant init, and
  // it isn't a def of i in the TAC.
  std::string src = R"c(#include <stdint.h>
uint8_t count(int8_t i) {
    uint8_t k = 0;
    int8_t *p = &i;
    i = 3;
    *p = -1;
    for (; i >= 0; i--) k++;
    return k;
}
int main(void) { return count(0); }
)c";
  both(src, 0);
}

TEST_CASE("loop_rotate: post decrement in test not rotated") {
  // The test `i-- >= 0` decrements i. The rotated loop skipped
  // the first test and so the first decrement, and returned
  // 5565. The body should see 4, 2, and 0, and the result is
  // their digits.
  std::string src = R"c(int main(void) {
    int c = 0;
    for (signed char i = 5; i-- >= 0; i--) c = c * 10 + i;
    return c;
}
)c";
  both(src, 420);
}

TEST_CASE("loop_rotate: side effect in test not rotated") {
  // `(n++, i) >= 0` runs five times, the last time failing. The
  // rotated loop skipped the first one and returned 44.
  std::string src = R"c(int n;
int main(void) {
    int c = 0;
    for (int i = 3; (n++, i) >= 0; i--) c++;
    return n * 10 + c;
}
)c";
  both(src, 54);
}

}  // namespace
}  // namespace c6502::suite
