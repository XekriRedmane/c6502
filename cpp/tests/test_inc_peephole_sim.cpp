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

namespace inc_peephole_correctness {
TEST_CASE("inc_peephole: loop counter 16bit correct sum") {
  // Sum 0..9 = 45. The loop-counter `i++` is the peephole
  // target.
  std::string src = R"c(int sum10(void) {
    int s = 0;
    for (int i = 0; i < 10; i++) s += i;
    return s;
}
int main(void) { return sum10(); }
)c";
  auto s = build_sim(src, Pipeline::kOptimized);
  auto result = s.run(5'000'000);
  CHECK_FALSE(result.timed_out);
  CHECK((result.return_int() & 0xFFFF) == 45);
}

TEST_CASE("inc_peephole: byte overflow propagates to high byte") {
  // Loop runs 300 iterations. After 256 the low byte wraps
  // to 0 and the BNE in the INC chain falls through to the
  // high-byte INC — exercising the carry-propagation path.
  // `s` accumulates `i`, so the final value tests both that
  // the high byte got incremented (i must reach 300, an
  // impossible value if the high byte stayed 0) and that
  // the loop terminates correctly (so the `i < 300`
  // comparison sees the right multi-byte value).
  std::string src = R"c(long sum300(void) {
    long s = 0;
    for (int i = 0; i < 300; i++) s += 1;
    return s;
}
long main(void) { return sum300(); }
)c";
  auto s = build_sim(src, Pipeline::kOptimized);
  auto result = s.run(10'000'000);
  CHECK_FALSE(result.timed_out);
  // 300 iterations of `s += 1` → s == 300.
  CHECK((result.return_long() & 0xFFFFFFFFLL) == 300);
}

}  // namespace inc_peephole_correctness

namespace inc_dec_leaves_a {
// The ADC / SBC form leaves the new value in A and INC / DEC
// don't, so when A is read afterwards the value is loaded. `return
// ++c;` for a static `unsigned char` used to return whatever A held
// before.
auto both(const auto& src, const auto& want) {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run().return_int();
    CHECK(got == want);
  }
}

TEST_CASE("inc_peephole: return preincrement of static char") {
  both(R"c(unsigned char tick(void) {
    static unsigned char c = 10;
    return ++c;
}
int main(void) { tick(); tick(); return tick(); }
)c",
       13);
}

TEST_CASE("inc_peephole: return predecrement of static char") {
  both(R"c(unsigned char tock(void) {
    static unsigned char c = 10;
    return --c;
}
int main(void) { tock(); return tock(); }
)c",
       8);
}

TEST_CASE("inc_peephole: return preincrement of static int") {
  both(R"c(unsigned int tick(void) {
    static unsigned int w = 0x01FF;
    return ++w;
}
int main(void) { tick(); return tick(); }
)c",
       0x0201);
}

}  // namespace inc_dec_leaves_a

}  // namespace
}  // namespace c6502::suite
