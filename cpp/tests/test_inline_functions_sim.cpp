#include <cstdint>
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"
#include "sim_case.h"
#include "sim_helpers.h"

namespace c6502::suite {
namespace {

const std::vector<ReturnCase> kCases = {
    // Each argument's side effects happen once, in order, before the
    // body, which reads its parameter twice.
    {"argument_side_effects", R"c(
        unsigned char log[4];
        unsigned char n;
        static unsigned char note(unsigned char v) { log[n++] = v; return v; }
        static int pair(int a, int b) { return a * 10 + b + a; }
        int main(void) {
            int r = pair(note(3), note(5));
            return r * 100 + log[0] * 10 + log[1] + n * 1000;
        }
    )c",
     2000 + 3800 + 30 + 5},
    // Several returns, used in an expression and in tail position.
    {"returns", R"c(
        static int sign(int x) {
            if (x < 0) return -1;
            if (x == 0) return 0;
            return 1;
        }
        static int tail(int x) { return sign(x - 5); }
        int main(void) {
            return (sign(-7) + 2) * 100 + (sign(0) + 2) * 10
                + sign(9) + tail(2) * 1000;
        }
    )c",
     -1000 + 100 + 20 + 1},
    // A void function with an early return, and a pointer result read
    // through, as Ali Baba's next_string is.
    {"void_and_pointer", R"c(
        unsigned char text[] = { 2, 'a', 'b', 3, 'c', 'd', 'e', 1, 'f' };
        unsigned char g;
        static void bump(unsigned char by) { if (by == 0) return; g += by; }
        static const unsigned char *next(const unsigned char *s) {
            return s + s[0] + 1;
        }
        int main(void) {
            const unsigned char *m = text;
            for (unsigned char i = 0; i < 2; i++) m = next(m);
            bump(0);
            bump(4);
            bump(5);
            return m[1] + g * 1000;
        }
    )c",
     'f' + 9000},
    // A copy inside a loop: each pass gets the parameter's new value,
    // and a local of the copy starts from its initializer each time.
    {"in_a_loop", R"c(
        static unsigned int tri(unsigned char k) {
            unsigned int s = 0;
            while (k) s += k--;
            return s;
        }
        int main(void) {
            unsigned int t = 0;
            for (unsigned char i = 1; i <= 4; i++) t += tri(i);
            return t;
        }
    )c",
     1 + 3 + 6 + 10},
    // Volatile reads in the body happen once per call.
    {"volatile", R"c(
        volatile unsigned char port;
        unsigned char reads;
        static unsigned char get(void) { reads++; return port; }
        int main(void) {
            port = 7;
            unsigned char a = get();
            unsigned char b = get();
            return a + b + reads * 100;
        }
    )c",
     214},
    // Nested candidates, with a wide and a floating parameter.
    {"nested", R"c(
        static long sq(long x) { return x * x; }
        static long sum_sq(long a, long b) { return sq(a) + sq(b); }
        static int trunc(double d) { return (int)d; }
        int main(void) {
            return (int)(sum_sq(3, 4) + sum_sq(100, 200) / 1000) + trunc(2.75);
        }
    )c",
     25 + 50 + 2}};
TEST_CASE("inline_functions: programs") {
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
