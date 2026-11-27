// The `-S -O` text of tests/test_narrow_byte_bitwise.py's program,
// whose compares of a masked byte are byte compares. The simulator's
// runs are in test_narrow_byte_bitwise_sim.cpp.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("narrow_byte_bitwise: a masked byte's compare is a byte compare") {
  const std::string text =
      suite::codegen(R"(#include <stdint.h>
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
)", true);
  const std::size_t start = text.find("\nbelow:");
  const std::size_t end = text.find("\nmain:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  // A byte compare has no overflow test.
  CHECK_FALSE(suite::contains(body, "BVC"));
  CHECK_FALSE(suite::contains(body, "EOR   #$80"));
}
