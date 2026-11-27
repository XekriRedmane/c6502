// Short-circuit shapes that src/passes/optimization/
// short_circuit_jump_fold.h folds, run in the simulator, from
// tests/test_short_circuit_jump_fold.py. Under -O they give what the
// unoptimized pipeline gives.
#include <string>
#include <utility>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

std::pair<std::int64_t, std::int64_t> both_paths(const std::string& src) {
  return {run_c_program(src).return_int_signed(), build_sim(src, true).run().return_int_signed()};
}

}  // namespace

TEST_CASE("short_circuit_jump_fold sim: && on unsigned chars") {
  const auto [a, b] = both_paths(R"c(#include <stdint.h>
int f(uint8_t a, uint8_t b) {
    if (a >= 0x40 && a < 0x47) return 1;
    return 2;
}
int main(void) {
    return f(0x44, 0) + 10 * f(0x10, 0) + 100 * f(0x50, 0);
}
)c");
  CHECK(a == b);
  CHECK(a == 1 + 10 * 2 + 100 * 2);
}

TEST_CASE("short_circuit_jump_fold sim: || on unsigned chars, with the sense flipped") {
  const auto [a, b] = both_paths(R"c(#include <stdint.h>
int f(uint8_t a) {
    if (a < 0x40 || a >= 0x50) return 1;
    return 2;
}
int main(void) {
    return f(0x44) + 10 * f(0x10) + 100 * f(0x60);
}
)c");
  CHECK(a == b);
  CHECK(a == 2 + 10 * 1 + 100 * 1);
}

TEST_CASE("short_circuit_jump_fold sim: a nested || of ||") {
  // c99_to_tac nests a three-operand `||` as `(a||b)||c`, so two
  // foldable short-circuits stack.
  const auto [a, b] = both_paths(R"c(#include <stdint.h>
int f(uint8_t a) {
    if (a == 0x63 || a == 0x8B || a == 0xB3) return 1;
    return 2;
}
int main(void) {
    return f(0x63) + 10 * f(0x8B) + 100 * f(0xB3)
        + 1000 * f(0x00);
}
)c");
  CHECK(a == b);
  CHECK(a == 1 + 10 * 1 + 100 * 1 + 1000 * 2);
}

TEST_CASE("short_circuit_jump_fold sim: uint16_t operands") {
  // The chain's compares OR each byte, and the fold doesn't depend on
  // the width.
  const auto [a, b] = both_paths(R"c(#include <stdint.h>
int f(uint16_t a) {
    if (a >= 0x0100 && a < 0x0200) return 1;
    return 2;
}
int main(void) {
    return f(0x0150) + 10 * f(0x0050) + 100 * f(0x0250);
}
)c");
  CHECK(a == b);
  CHECK(a == 1 + 10 * 2 + 100 * 2);
}

TEST_CASE("short_circuit_jump_fold sim: a phi's copy between the tail and the consumer") {
  // buffer_char's case conversion, as SSA destruction leaves
  // `if (c >= 0x60 && c < 0x80) c -= 0x20;`.
  const char* src = R"c(
        unsigned char up(unsigned char c) {
            if (c >= 0x60 && c < 0x80) c -= 0x20;
            return c | 0x80;
        }
        int main(void) {
            return up('a') + up('Z') * 2 + up(0x90) * 4;
        }
        )c";
  const std::int64_t want = ('A' | 0x80) + ('Z' | 0x80) * 2 + 0x90 * 4;
  CHECK(run_c_program(src).return_int() == want);
  CHECK(build_sim(src, true).run().return_int() == want);
}

}  // namespace c6502::suite
