// The `-S -O` text of tests/test_truncate_extend_fold.py's byte loops
// against `sizeof`. Their simulator runs stay in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("truncate_extend_fold: a byte loop against sizeof compares one byte") {
  // The compare is at the width of `size_t`, `unsigned int`, so `i` is
  // promoted and copied, as it is for `i < 40U`. Copy propagation
  // removes the copy after the compare has become a `JumpIfCmp`, or
  // keeps it when the extend has other readers, as the address of
  // `p[i]` is in the third loop, and the narrowing looks through it.
  const std::string text =
      suite::codegen(R"(#include <stdint.h>
uint8_t a[40];
uint8_t b[40];
uint8_t f(void) {
    for (uint8_t i = 0; i < sizeof a; i++)
        a[i] = b[i] + 1;
    for (uint8_t i = 0; i < 40U; i++)
        b[i] = a[i];
    uint8_t *p = (uint8_t *)a;
    for (uint8_t i = 0; i < sizeof a; i++)
        p[i] += 2;
    return b[39];
}
int main(void) {
    for (uint8_t i = 0; i < 40; i++)
        b[i] = i;
    return f();
}
)", true);
  const std::size_t start = text.find("\nf:");
  const std::size_t end = text.find("\nmain:");
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  const std::string body = text.substr(start, end > start ? end - start : 0);
  INFO(body);
  // The third loop keeps its counter in memory, so its compare is
  // `CMP #$28`.
  CHECK(suite::count(body, "CPX   #$28") + suite::count(body, "CMP   #$28") == 3);
  CHECK_FALSE(suite::contains(body, "SBC"));
  CHECK_FALSE(suite::contains(body, "sx_done"));
}
