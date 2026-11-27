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

// `find` is inlined into `f`, which tests its result. Each case
// runs both the `return true;` and the `return false;` paths.
const std::string kSource = R"c(
        #include <stdint.h>
        #include <stdbool.h>
        uint8_t a[8], n, hits;
        static bool find(uint8_t v) {
            uint8_t i = n;
            do { if (a[i] == v) { hits++; return true; } } while (--i != 0);
            return false;
        }
        uint8_t f(uint8_t v) { if (find(v)) return 10; hits += 2; return 20; }
        int main(void) {
            n = 7;
            for (uint8_t k = 0; k < 8; k++) a[k] = k * 3;
            uint8_t r = 0;
            r += f(%d);
            return r * 256 + hits;
        }
    )c";
TEST_CASE("jump_thread: found and not found") {
  // 9 is a[3]; 4 isn't in the array; a[0] = 0 is never compared,
  // since the loop stops when i reaches 0; 21 is a[7], the first
  // compared.
  for (const auto& [v, expected] :
       std::vector<std::tuple<int, int>>{{9, 10 * 256 + 1}, {4, 20 * 256 + 2}, {0, 20 * 256 + 2}, {21, 10 * 256 + 1}}) {
    for (bool optimize : {false, true}) {
      INFO("v=" << v << " optimize=" << optimize);
      auto result = build_sim(replace_all(kSource, "%d", std::to_string(v)), optimize).run();
      CHECK_FALSE(result.timed_out);
      CHECK(result.return_int() == expected);
    }
  }
}

TEST_CASE("jump_thread: and or results") {
  std::string src = R"c(
            #include <stdint.h>
            uint8_t x, y, z;
            static uint8_t pick(void) {
                if (x == 0 && (y != 0 || z != 0)) return 3;
                if (x == 5 || y == 6) return 4;
                return 5;
            }
            int main(void) {
                int r = 0;
                static const uint8_t xs[] = {0, 0, 0, 5, 1};
                static const uint8_t ys[] = {0, 1, 0, 0, 6};
                static const uint8_t zs[] = {0, 0, 1, 0, 0};
                for (uint8_t k = 0; k < 5; k++) {
                    x = xs[k]; y = ys[k]; z = zs[k];
                    r = r * 8 + pick();
                }
                return r;
            }
        )c";
  // x, y, z = 0, 0, 0 gives 5; 0, 1, 0 and 0, 0, 1 give 3; and
  // 5, 0, 0 and 1, 6, 0 give 4. One octal digit each.
  auto expected = 053344;
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto result = build_sim(src, optimize).run();
    CHECK_FALSE(result.timed_out);
    CHECK(result.return_int() == expected);
  }
}

}  // namespace
}  // namespace c6502::suite
