// The tests of tests/test_divide_shapes.py that read the `-S -O` text
// of a shift-and-subtract divide loop, as Zork's `divu16`
// (examples/zork/numbers.c) has, and of a local array read by index.
#include <stdexcept>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns the `-S -O` text of function `name` of `source`, from its
// label to its first `RTS`.
std::string function_text(const std::string& source, const std::string& name,
                          const suite::Options& options) {
  const std::string text = suite::codegen(source, options);
  const std::size_t start = text.find("\n" + name + ":");
  if (start == std::string::npos) throw std::runtime_error("no function " + name);
  const std::size_t end = text.find("\n   RTS", start);
  if (end == std::string::npos) throw std::runtime_error("no RTS after " + name);
  return text.substr(start, end - start);
}

suite::Options optimized(bool inline_calls) {
  suite::Options options;
  options.optimize = true;
  options.inline_calls = inline_calls;
  return options;
}

}  // namespace

TEST_CASE("divide_shapes: the divide loop's compare branches on its one subtract") {
  const std::string body =
      function_text(R"(
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
)", "divide", optimized(false));
  CHECK(suite::count(body, "SBC   __zpabi_divide__divisor_0") == 1);
}

TEST_CASE("divide_shapes: the divide loop rotates its bits in") {
  // The quotient takes `fits` through C, and the remainder takes the
  // quotient's top bit straight from the quotient's last ROL. One LSR
  // is in the loop and one after it.
  const std::string body =
      function_text(R"(
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
)", "divide", optimized(false));
  CHECK_FALSE(suite::contains(body, "ORA"));
  CHECK(suite::count(body, "LSR") == 2);
  CHECK(suite::count(body, "ROL") >= 3);
}

TEST_CASE("divide_shapes: a local array is read by index with no pointer to it") {
  const std::string body =
      function_text(R"(
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
)", "digits", optimized(true));
  CHECK_FALSE(suite::contains(body, "#<__local_digits__d"));
  CHECK(suite::contains(body, "__local_digits__d,Y"));
}
