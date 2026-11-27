// The `-S -O` text of tests/test_narrow_widened_arith.py's byte shifts.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns the `-S -O` text of `source` from `from` to the first `to`.
std::string between(const std::string& source, const std::string& from, const std::string& to) {
  const std::string text = suite::codegen(source, true);
  const std::size_t start = text.find(from);
  const std::size_t end = text.find(to);
  REQUIRE(start != std::string::npos);
  REQUIRE(end != std::string::npos);
  return text.substr(start, end > start ? end - start : 0);
}

}  // namespace

TEST_CASE("narrow_widened_arith: a byte global shifted on two paths shifts one byte") {
  const std::string body = between(
      R"(#include <stdint.h>
uint8_t alive = 0x81, other = 6, key;
void f(void) {
    if (key == 3) {
        other++;
        alive >>= 1;
        return;
    }
    if (key == 4)
        alive >>= 1;
}
int main(void) {
    key = 3; f();
    key = 4; f();
    key = 5; f();
    return alive + (other << 8);
}
)",
      "\nf:", "\nmain:");
  INFO(body);
  CHECK(suite::count(body, "LSR   alive") == 2);
  CHECK_FALSE(suite::contains(body, "ROR"));
}

TEST_CASE("narrow_widened_arith: a byte shifted left by a variable count is a loop of 1-byte shifts") {
  // `v` arrives in A, and the loop shifts it there
  // (`shift_in_a._shift_loop_in_a`), where it had called `asl16`.
  const std::string body =
      between(R"(#include <stdint.h>
uint8_t s(uint8_t v, uint8_t n) { v <<= n; return v; }
int main(void) {
    int bad = 0;
    if (s(3, 2) != 12) bad |= 1;
    if (s(0x81, 1) != 2) bad |= 2;
    if (s(5, 8) != 0) bad |= 4;
    if (s(5, 12) != 0) bad |= 8;
    if (s(5, 0) != 5) bad |= 16;
    return bad;
}
)", "\ns:", "\nmain:");
  CHECK(suite::contains(body, ".shift_loop@0:\n   ASL\n"));
  CHECK_FALSE(suite::contains(body, "ROL"));
  CHECK_FALSE(suite::contains(body, "JSR"));
}
