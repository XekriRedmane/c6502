// The simulator tests of tests/test_narrow_byte_bitwise.py: bitwise
// operations that src/passes/optimization/narrow_byte_bitwise.h narrows
// to a byte give the values C does in both pipelines.
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

const std::string kSrc = R"c(#include <stdint.h>
uint8_t g = 0xC5;
int8_t sg = -3;
uint8_t p[8] = {0, 0, 0, 0, 0, 0, 0x2A, 0};
int below(uint8_t *q, uint8_t lim) { return (q[6] & 0x3F) < lim; }
int main(void) {
    int n = 0;
    if ((g & 0x3F) < 6) n |= 1;
    if ((g | 0x10) == 0xD5) n |= 2;
    if ((g ^ 0x0F) > 0xC9) n |= 4;
    if ((sg & 0xF0) == 0xF0) n |= 8;
    if (below(p, 43)) n |= 16;
    if (below(p, 42)) n |= 32;
    int8_t d = (int8_t)(g - 0xC6);
    if (d > 0) n |= 64;
    if (d > -2) n |= 128;
    return n;
}
)c";
TEST_CASE("narrow_byte_bitwise: runs") {
  // g & 0x3F is 5, g | 0x10 is 0xD5, g ^ 0x0F is 0xCA, sg & 0xF0
  // is 0xF0, p[6] & 0x3F is 42, and d is -1.
  auto expected = 1 | 2 | 4 | 8 | 16 | 128;
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto out = build_sim(kSrc, optimize).run();
    CHECK(out.return_int() == expected);
  }
}

}  // namespace
}  // namespace c6502::suite
