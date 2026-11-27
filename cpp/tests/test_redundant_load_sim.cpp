// The simulator tests of tests/test_redundant_load.py: programs whose
// loads the asm-level redundant-load elimination drops give the values
// C does.
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

namespace index_register_of_indexed_data {
// `tab,X` and `tab,Y` are different bytes unless X equals Y.
TEST_CASE("redundant_load: simulated table chase") {
  std::string src = R"c(
unsigned char tab[8] = {3, 0, 0, 7, 0, 0, 0, 0};
unsigned char g;
unsigned char f(unsigned char i) { unsigned char j = tab[i]; if (j == 0) return 0; g = tab[j]; return j; }
int main(void) { unsigned char r = f(0); return g * 100 + r; }
)c";
  CHECK(build_sim(src, true).run().return_int() == 703);
}

}  // namespace index_register_of_indexed_data

namespace rows_of_two_tables_sim {
// Two 2-D arrays whose tables of row addresses share the low
// bytes, read and written in one row, as Lode Runner's LEVEL_MAP and
// LEVEL_BG are.
const std::string kSrc = R"c(#include <stdint.h>
static uint8_t map[16][28];
static uint8_t bg[16][28];
uint8_t row, col;
void fall(void) {
    uint8_t r = row + 1;
    if (bg[r][col] == 1 && (int8_t)col >= 0) {
        uint8_t above = r - 1;
        if (bg[above][col] == 0) {
            map[above][col] = 7;
            bg[above][col] = 7;
        }
    }
}
int main(void) {
    for (uint8_t i = 0; i < 16; i++)
        for (uint8_t j = 0; j < 28; j++)
            bg[i][j] = (uint8_t)(i + j) & 1;
    row = 4; col = 4;
    fall();
    return map[4][4] + bg[4][4] * 16 + bg[5][4] * 256;
}
)c";
TEST_CASE("redundant_load: runs") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(kSrc, optimize).run();
    CHECK(out.return_int() == 7 + 7 * 16 + 256);
  }
}

}  // namespace rows_of_two_tables_sim

}  // namespace
}  // namespace c6502::suite
