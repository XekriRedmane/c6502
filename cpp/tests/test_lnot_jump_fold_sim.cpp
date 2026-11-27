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
