// The simulator tests of tests/test_lnot_jump_fold.py: `if (!x)` on
// sources of 1, 2, and 4 bytes, whose test
// src/passes/optimization/lnot_jump_fold.h turns into a jump of the
// other sense, gives the same values in both pipelines.
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "batch1_case.h"
#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {
namespace {

// Differential opt-vs-unopt for `if (!x)` shapes.
// Checks that `src` returns `want` in both pipelines.
void both_paths(const std::string& src, std::int64_t want) {
  CHECK(run_c_program(src).return_int_signed() == want);
  CHECK(build_sim(src, true).run().return_int_signed() == want);
}

TEST_CASE("lnot_jump_fold: uchar negated") {
  std::string src = R"c(#include <stdint.h>
int f(uint8_t a) { if (!a) return 1; return 2; }
int main(void) { return f(0) + 10 * f(7); }
)c";
  both_paths(src, 21);
}

TEST_CASE("lnot_jump_fold: uint16 negated") {
  std::string src = R"c(#include <stdint.h>
int f(uint16_t a) { if (!a) return 1; return 2; }
int main(void) { return f(0) + 10 * f(7); }
)c";
  both_paths(src, 21);
}

TEST_CASE("lnot_jump_fold: uint32 negated") {
  // Multi-byte src: the JumpIf*'s lowering walks every byte
  // (ORA chain), so the fold must still yield the right answer.
  std::string src = R"c(#include <stdint.h>
int f(uint32_t a) { if (!a) return 1; return 2; }
int main(void) {
    return f(0) + 10 * f(0x00010000UL);
}
)c";
  both_paths(src, 21);
}

TEST_CASE("lnot_jump_fold: negated in ternary") {
  std::string src = R"c(#include <stdint.h>
int f(uint8_t a) { return !a ? 10 : 20; }
int main(void) { return f(0) + f(5); }
)c";
  both_paths(src, 30);
}

}  // namespace
}  // namespace c6502::suite
