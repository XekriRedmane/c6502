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

namespace remat_pointer_store {
// A store through a pointer between the load and the read may
// write the static, unless the static is internal and its address
// is never formed. `uint8_t t = g; *p = 7; return t;` called with
// `p == &g` returned 7 instead of the old value of `g`.
TEST_CASE("asm_remat: simulated pointer store to global") {
  std::string src = R"c(unsigned char g;
unsigned char f(unsigned char *p) {
    unsigned char t = g;
    *p = 7;
    return t;
}
int main(void) { g = 3; return f(&g); }
)c";
  CHECK(build_sim(src, true).run().return_int() == 3);
}

}  // namespace remat_pointer_store

namespace remat_cost_sim {
// The shape of Lode Runner's `enable_next_level_ladders` is a table
// byte copied into a local and a zero-page global, with the local read
// again after a call.
const std::string kSrc = R"c(
#include <stdint.h>
uint8_t ROWS[6] = {1, 4, 9, 16, 25, 36};
uint8_t COLS[6] = {2, 3, 5, 7, 11, 13};
uint8_t GR [[c6502::zeropage]];
uint8_t GC [[c6502::zeropage]];
uint16_t sum;
void touch(void) { sum += GR + GC; }
int main(void) {
    for (uint8_t i = 5; i != 0; i--) {
        uint8_t col = COLS[i];
        if (col == 5)
            continue;
        uint8_t row = ROWS[i];
        GR = row;
        GC = col;
        touch();
        sum += (uint16_t)(row * 3) + col;
    }
    return sum;
}
)c";
TEST_CASE("asm_remat: runs") {
  std::vector<int> rows = {1, 4, 9, 16, 25, 36};
  std::vector<int> cols = {2, 3, 5, 7, 11, 13};
  auto want = 0;
  for (std::int64_t i = 5; i > 0; i += -1) {
    if (cols[i] == 5) {
      continue;
    }
    want += rows[i] + cols[i] + rows[i] * 3 + cols[i];
  }
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(kSrc, optimize).run();
    CHECK(got.return_int() == want);
  }
}

}  // namespace remat_cost_sim

}  // namespace
}  // namespace c6502::suite
