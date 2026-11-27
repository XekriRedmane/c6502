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

auto check(const auto& src, const auto& expected) {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto r = build_sim(src, optimize).run();
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int() == expected);
  }
}

TEST_CASE("dual_index_promotion: store after branch") {
  // `v[d] = a` sits after `if (a != 0)`. The optimized build
  // used to store to `v[k]`.
  check(R"c(
        #include <stdint.h>
        uint8_t t[8], u[8] = {0,0,0,0,0,7,0,0}, v[8], w[8];
        uint8_t o, o2;
        void f(uint8_t k, uint8_t d)
        {
            o = t[k];
            uint8_t a = u[d];
            if (a != 0)
                v[d] = a;
            o = t[k];
            o2 = w[d];
        }
        int main(void) { f(2, 5); return v[5] * 10 + v[2]; }
        )c",
        70);
}

TEST_CASE("dual_index_promotion: pointer read between uses") {
  // `*p` through a zero-page pointer loads Y with 0, so the
  // optimized build used to read `w[0]` where it should read
  // `w[d]`.
  check(R"c(
        #include <stdint.h>
        uint8_t t[8], u[8], w[8] = {0,0,0,0,0,9,0,0};
        uint8_t o, o2, o3, cell;
        void f(uint8_t k, uint8_t d, uint8_t *p)
        {
            o = t[k];
            o2 = u[d];
            o = t[k];
            o3 = *p;
            o2 = w[d];
        }
        int main(void) { f(2, 5, &cell); return o2; }
        )c",
        9);
}

}  // namespace
}  // namespace c6502::suite
