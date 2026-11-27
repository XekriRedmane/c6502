// The simulator tests of tests/test_slot_coalescing.py: a multiply and
// divide program whose locals src/passes/slot_coalescing.h puts in their
// parameters' slots gives the values computed here in both pipelines.
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

// `mul` takes the starting product first, which arrives in A and X, so
// that `a` and `b` arrive in their slots, where `a <<= 1` and `b >>= 1`
// shift them in place.
const std::string kMulu16 = R"c(
#include <stdint.h>
uint16_t t[6] = {0, 1, 7, 255, 300, 65535};
static uint16_t mul(uint16_t product, uint16_t a, uint16_t b) {
    while (b != 0) {
        if (b & 1)
            product += a;
        a <<= 1;
        b >>= 1;
    }
    return product;
}
static uint16_t div(uint16_t dividend, uint16_t divisor) {
    uint16_t quotient = dividend;
    uint16_t partial = 0;
    for (uint8_t i = 17; i != 0; i--) {
        uint8_t fits = partial >= divisor;
        if (fits)
            partial -= divisor;
        uint8_t out = (quotient & 0x8000) != 0;
        quotient = (quotient << 1) | fits;
        partial = (partial << 1) | out;
    }
    return quotient;
}
int main(void) {
    uint16_t n = 0;
    for (uint8_t i = 0; i < 6; i++)
        for (uint8_t j = 0; j < 6; j++)
            n = (uint16_t)(n * 3u + mul(0, t[i], t[j])
                           + (t[j] ? div(t[i], t[j]) : 0));
    return n;
}
)c";
std::int64_t expected() {
  const std::vector<std::int64_t> t = {0, 1, 7, 255, 300, 65535};
  std::int64_t n = 0;
  for (std::int64_t a : t) {
    for (std::int64_t b : t) {
      const std::int64_t q = b ? a / b : 0;
      n = (n * 3 + a * b + q) & 0xFFFF;
    }
  }
  return n;
}

TEST_CASE("slot_coalescing: multiply and divide") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    SimOptions options;
    options.optimize = optimize;
    options.inline_calls = false;
    auto r = build_sim(kMulu16, options).run();
    CHECK(r.return_int() == expected());
  }
}

}  // namespace
}  // namespace c6502::suite
