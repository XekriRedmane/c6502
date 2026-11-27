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

const auto kMaxCycles = 400'000;
std::int64_t run(const std::string& source, bool optimize) {
  const sim::SimResult result = build_sim(source, optimize).run(kMaxCycles);
  REQUIRE_FALSE_MESSAGE(result.timed_out, "the run timed out with optimize=" << optimize);
  return result.return_int();
}

TEST_CASE("hwreg_pin_correctness: indirect store clobbers y resident iv") {
  // `y` is incremented across nested loops, so the optimizer
  // pins it to Reg(Y). Inside the inner loop's body, `base[x] =
  // p[y]` lowers to an IndirectIndexedStore whose lowering
  // writes Y (it stages `x` into Y for the `STA (DPTR),Y`).
  // After that store, the *next* `p[y]` read uses the stale Y
  // (= x), not the live `y`.
  //
  // Expected return: sum of in_buf[0..15] = 1+2+...+16 = 136.
  // Before the fix, the optimized run returned 90 (the inner
  // `p[y]` re-read after the store picked up `p[x]` instead).
  std::string source = R"c(
        #include <stdint.h>
        static uint8_t out_buf[16];
        int main(void) {
            static const uint8_t in_buf[16] = {
                1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
            };
            const uint8_t *p = in_buf;
            uint8_t y = 0;
            uint8_t sum = 0;
            for (uint8_t h = 2; h != 0; h--) {
                uint8_t *base = out_buf;
                for (uint8_t r = 8, x = 0;
                     (r & 0x80) == 0;
                     r--, x++, y++) {
                    if (x < 8) {
                        base[x] = p[y];
                    }
                    sum = sum + p[y];
                }
            }
            return sum;
        }
        )c";
  auto unopt = run(source, false);
  auto opt = run(source, true);
  CHECK_MESSAGE(unopt == 136, "oracle (unopt) sanity check");
  CHECK_MESSAGE(opt == 136,
                "optimized run returned the wrong value — Reg(Y) was pinned to a Pseudo whose live range crosses an "
                "IndirectIndexedStore (which writes Reg(Y) as part of its lowering)");
}

}  // namespace
}  // namespace c6502::suite
