// The simulator tests of tests/test_register_arguments.py: a program with
// the shapes around arguments that arrive in A and X returns 0 in both
// pipelines.
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

const std::string kProgram = R"c(
#include <stdint.h>
unsigned char k;
unsigned int widen(unsigned int y) { return y - k; }
uint16_t pair(uint8_t a, uint8_t b) { return (uint16_t)a << 8 | b; }
uint16_t swap(uint8_t a, uint8_t b) { return pair(b, a); }
uint8_t count(uint8_t d) {
    register uint8_t y = d;
    uint8_t n = 0;
    while (--y != 0) n++;
    return n;
}
int main(void) {
    k = 5;
    if (widen(3) != 0xFFFE) return 1;
    k = 0;
    if (widen(0x1234) != 0x1234) return 2;
    k = 0xFF;
    if (widen(0x100) != 1) return 3;
    if (widen(0xFF) != 0) return 4;
    if (swap(0x12, 0x34) != 0x3412) return 5;
    if (swap(0, 0xFF) != 0xFF00) return 6;
    if (count(1) != 0) return 7;
    if (count(5) != 4) return 8;
    if (count(0) != 255) return 9;
    return 0;
}
)c";
TEST_CASE("register_arguments: both pipelines") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto result = build_sim(kProgram, optimize).run();
    CHECK_FALSE(result.timed_out);
    CHECK(result.return_int() == 0);
  }
}

}  // namespace
}  // namespace c6502::suite
