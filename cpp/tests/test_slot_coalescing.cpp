// The `-S -O` text of tests/test_slot_coalescing.py's multiply, in
// which a zero-page local holding a copy of a parameter takes the
// parameter's own slot.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("slot_coalescing: the parameters aren't copied") {
  // `mul` takes the starting product first, which arrives in A and X,
  // so that `a` and `b` arrive in their slots, where `a <<= 1` and
  // `b >>= 1` shift them in place.
  suite::Options options;
  options.optimize = true;
  options.inline_calls = false;
  const std::string text = suite::codegen(R"(
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
)", options);
  const std::size_t start = text.find("\nmul:");
  const std::size_t end = text.find("\ndiv:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  CHECK_FALSE(suite::contains(body, "__local_mul__a"));
  CHECK(suite::contains(body, "ASL   __zpabi_mul__a_0"));
  CHECK(suite::contains(text, "changes=__zpabi_mul__a_0"));
}
