// The simulator tests of tests/test_return_in_zero_page.py: programs that
// use the pointer a `static` function returns in a zero-page object of
// its own, `F@ret` (src/passes/return_in_zero_page.h), give the values C
// does in both pipelines.
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

const std::vector<std::tuple<std::string, std::string, int>> kCases = {
    // The result read right after the call, kept across a second
    // call (so it has to be copied out of `F@ret`), stored into a
    // static, compared, and returned on from another function.
    {"uses_of_the_result", R"c(
        unsigned char t[16] = { 1, 2, 3, 4, 5, 6, 7, 8,
                                9, 10, 11, 12, 13, 14, 15, 16 };
        unsigned char *saved;
        static unsigned char *at(unsigned char i) { return t + i; }
        static unsigned char *pick(unsigned char i) {
            if (i & 1) return at(i);
            return t;
        }
        unsigned char *outer(unsigned char i) { return at(i + 1); }
        int main(void) {
            unsigned char *a = at(3);
            unsigned char *b = at(5);
            saved = at(7);
            int r = a[0] * 100 + b[0] * 10 + saved[0];
            if (at(2) == t + 2) r += 1000;
            if (pick(2) == t) r += 2000;
            r += pick(9)[0] + outer(4)[0];
            a[1] = 40;
            return r + t[4];
        }
    )c",
     3000 + 400 + 60 + 8 + 10 + 6 + 40},
    // A recursive pointer function: each call's result is read
    // before the caller's own return writes the object.
    {"recursion", R"c(
        unsigned char t[8] = { 5, 6, 7, 8, 9, 10, 11, 12 };
        static unsigned char *nth(unsigned char n) {
            if (n == 0) return t;
            return nth(n - 1) + 1;
        }
        int main(void) { return *nth(5) + *nth(0) * 100; }
    )c",
     10 + 500},
    // Stores through the result, and through a pointer to another
    // element, between reads of it.
    {"stores_between_reads", R"c(
        unsigned char t[4] = { 1, 2, 3, 4 };
        unsigned char *q;
        static unsigned char *first(void) { return t; }
        int main(void) {
            q = t + 3;
            unsigned char *p = first();
            p[1] = 20;
            *q = 30;
            p[2] = p[1] + *q;
            return p[0] + p[1] + p[2] + p[3];
        }
    )c",
     1 + 20 + 50 + 30}};
TEST_CASE("return_in_zero_page: programs") {
  for (const auto& [name, src, expected] : kCases) {
    for (bool optimize : {false, true}) {
      INFO("case=" << name << " optimize=" << optimize);
      auto result = build_sim(src, optimize).run();
      CHECK_FALSE(result.timed_out);
      CHECK(result.return_int_signed() == expected);
    }
  }
}

}  // namespace
}  // namespace c6502::suite
