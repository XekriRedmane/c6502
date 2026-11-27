// The simulator tests of tests/test_divide_shapes.py: a shift-and-subtract
// divide loop, as Zork's `divu16` (examples/zork/numbers.c) has, and a
// local array read by index give the values computed here in both
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

const std::string kDivide = R"c(
#include <stdint.h>
uint16_t remainder;
uint16_t t[7] = {0, 1, 7, 10, 255, 1000, 65535};
static uint16_t divide(uint16_t dividend, uint16_t divisor) {
    uint16_t quotient = dividend;
    uint16_t partial = 0;
    for (uint8_t i = 17; i != 0; i--) {
        uint8_t fits = 0;
        if (partial >= divisor) {
            partial -= divisor;
            fits = 1;
        }
        uint8_t out = (uint8_t)(quotient >> 15);
        quotient = (quotient << 1) | fits;
        partial = (partial << 1) | out;
    }
    remainder = partial >> 1;
    return quotient;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 7; i++)
        for (uint8_t j = 1; j < 7; j++)
            n = (uint16_t)(n * 3u + divide(t[i], t[j]) + remainder);
    return n;
}
)c";
const std::string kDigits = R"c(
#include <stdint.h>
uint8_t out[6];
uint8_t n_out;
static void put(uint8_t c) { out[n_out++] = c; }
static void digits(uint16_t n) {
    uint8_t d[5];
    uint8_t count = 0;
    do {
        d[count] = (uint8_t)(n % 10);
        n /= 10;
        count++;
    } while (n != 0);
    do {
        count--;
        put(d[count] + '0');
    } while (count != 0);
}
int main(void) {
    digits(40321);
    return (out[0] - '0') * 10 + (out[4] - '0');
}
)c";
auto expected_divide() {
  std::vector<int> t = {0, 1, 7, 10, 255, 1000, 65535};
  auto n = 0;
  for (const auto& a : t) {
    for (std::size_t k = 1; k < t.size(); ++k) n = (n * 3 + a / t[k] + a % t[k]) & 0xFFFF;
  }
  return n;
}

namespace divide_loop {
TEST_CASE("divide_shapes: divide loop: runs") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    SimOptions options;
    options.optimize = optimize;
    options.inline_calls = false;
    auto r = build_sim(kDivide, options).run();
    CHECK(r.return_int() == expected_divide());
  }
}

}  // namespace divide_loop

namespace local_array_indexed {
TEST_CASE("divide_shapes: local array indexed: runs") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto r = build_sim(kDigits, optimize).run();
    CHECK(r.return_int() == 41);
  }
}

}  // namespace local_array_indexed

}  // namespace
}  // namespace c6502::suite
