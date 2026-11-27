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

namespace sim_integration {
auto run(const auto& src) {
  auto s = build_sim(src, true);
  auto r = s.run(200'000);
  CHECK_FALSE(r.timed_out);
  return r.return_int();
}

TEST_CASE("and_zero_jump_fold: high bit set") {
  std::string src = R"c(
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x80) return 1;
            return 0;
        }
        int main(void) { return test(0xFF); }
        )c";
  CHECK(run(src) == 1);
}

TEST_CASE("and_zero_jump_fold: high bit clear") {
  std::string src = R"c(
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x80) return 1;
            return 0;
        }
        int main(void) { return test(0x7F); }
        )c";
  CHECK(run(src) == 0);
}

TEST_CASE("and_zero_jump_fold: arbitrary mask") {
  std::string src = R"c(
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x10) return 7;
            return 3;
        }
        int main(void) { return test(0x10); }
        )c";
  CHECK(run(src) == 7);
}

TEST_CASE("and_zero_jump_fold: store between extend and and") {
  // The ZeroExtend reads g before the store of 0. The fold read
  // g at the jump instead, after the store, and the test never
  // passed.
  std::string src = R"c(
        #include <stdint.h>
        uint8_t g;
        uint8_t hits;
        void check(void) { int t = g; g = 0; if (t & 1) hits = 1; }
        int main(void) { g = 1; check(); return hits; }
        )c";
  auto s = build_sim(src, false);
  CHECK(s.run(200'000).return_int() == 1);
  CHECK(run(src) == 1);
}

}  // namespace sim_integration

namespace wide_mask_in_one_byte {
const std::string kSrc = R"c(
    unsigned int w;
    int s;
    unsigned long l;
    unsigned char hi(void) { return (w & 0x8000) ? 0xFF : 0; }
    unsigned char mid(void) { return (w & 0x0400) != 0; }
    unsigned char neg(void) { return (s & 0x8000) != 0; }
    unsigned char top(void) { return (l & 0x00800000UL) != 0; }
    int main(void) {
        int r = 0;
        w = 0x8400; r += hi() + mid();
        w = 0x7BFF; r += hi() * 3 + mid() * 5;
        s = -2; r += neg() * 7;
        l = 0x00812345UL; r += top() * 11;
        l = 0x007FFFFFUL; r += top() * 13;
        return r;
    }
    )c";
TEST_CASE("and_zero_jump_fold: runs") {
  auto want = 0xFF + 1 + 0 + 0 + 7 + 11;
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(kSrc, optimize).run().return_int();
    CHECK(got == want);
  }
}

}  // namespace wide_mask_in_one_byte

}  // namespace
}  // namespace c6502::suite
