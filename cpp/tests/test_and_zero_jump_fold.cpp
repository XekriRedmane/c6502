// The `-S` text of tests/test_and_zero_jump_fold.py's program of masks
// on wide values. The runs of its programs stay in Python.
#include <stdexcept>
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

// Returns `text.split(sep)[n]`, and throws when `text` has fewer
// pieces.
std::string piece(const std::string& text, const std::string& sep, std::size_t n) {
  std::size_t start = 0;
  for (std::size_t k = 0; k < n; ++k) {
    const std::size_t at = text.find(sep, start);
    if (at == std::string::npos) throw std::runtime_error("no piece " + std::to_string(n));
    start = at + sep.size();
  }
  const std::size_t end = text.find(sep, start);
  return text.substr(start, end == std::string::npos ? std::string::npos : end - start);
}

}  // namespace

TEST_CASE("and_zero_jump_fold: a mask whose bits fall in one byte of a wide value tests that byte") {
  const std::string text = suite::codegen(
      R"(
    unsigned int w;
    int s;
    unsigned long l;
    unsigned char hi(void) { return (w & 0x8000) ? 0xFF : 0; }
    unsigned char mid(void) { return (w & 0x0400) != 0; }
    unsigned char neg(void) { return (s & 0x8000) != 0; }
    unsigned char top(void) { return (l & 0x00800000UL) != 0; }
    int main(void) {
        int r = 0;
        w = 0x8400; r += hi() + mid();
        w = 0x7BFF; r += hi() * 3 + mid() * 5;
        s = -2; r += neg() * 7;
        l = 0x00812345UL; r += top() * 11;
        l = 0x007FFFFFUL; r += top() * 13;
        return r;
    }
    )",
      true);
  const std::string body = piece(piece(text, "\nhi:\n", 1), "RTS", 0);
  CHECK(suite::contains(body, "LDA   w+1"));
  CHECK_FALSE(suite::contains(body, "ORA"));
}
