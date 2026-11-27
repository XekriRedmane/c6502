// The `-S -O` text of tests/test_compare_subtract.py's functions. Their
// simulator runs stay in Python.
#include <stdexcept>
#include <string>
#include <vector>

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

// Returns how many bytes the operands of the function in `program`
// have.
std::size_t width(const std::string& program) {
  if (suite::contains(program, "long")) return 4;
  return suite::contains(program, "char") ? 1 : 2;
}

}  // namespace

TEST_CASE("compare_subtract: a compare and a subtract of the same operands are one subtract") {
  // Each function with the `main` that calls it first.
  const std::vector<std::string> programs = {
      R"(
     unsigned int f(unsigned int s, unsigned int k) {
         while (s >= k) s -= k;
         return s;
     }
int main(void) { return (int)(f(1000, 16)); }
)",
      R"(
     int f(int s, int k) {
         while (s >= k) s -= k;
         return s;
     }
int main(void) { return (int)(f(1000, 16)); }
)",
      R"(
     unsigned char f(unsigned char s, unsigned char k) {
         while (s >= k) s -= k;
         return s;
     }
int main(void) { return (int)(f(250, 7)); }
)",
      R"(
     int f(int s, int k) {
         if (s >= k) return s - k;
         return 0;
     }
int main(void) { return (int)(f(-30000, 30000)); }
)",
      R"(
     signed char f(signed char s, signed char k) {
         if (s < k) return 0;
         return s - k;
     }
int main(void) { return (int)(f(-100, 100)); }
)",
      R"(
     unsigned long f(unsigned long a, unsigned long b) {
         if (a > b) return b - a;
         return 7;
     }
int main(void) { return (int)(f(5, 3)); }
)",
  };
  for (const std::string& program : programs) {
    // A `signed char` compare is of the operands widened to `int`, and
    // the subtract is narrowed back to a byte, so the two don't match.
    if (suite::contains(program, "signed char")) continue;
    INFO(program);
    const std::string text = suite::codegen(program, true);
    const std::string body = piece(piece(text, "\nf:\n", 1), "\nmain:\n", 0);
    INFO(body);
    CHECK_FALSE(suite::contains(body, "CMP"));
    CHECK(suite::count(body, "SBC") == width(program));
  }
}
