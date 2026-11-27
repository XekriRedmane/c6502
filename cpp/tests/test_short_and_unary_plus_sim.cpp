// The simulator tests of tests/test_short_and_unary_plus.py: programs with
// `short`, `unsigned short`, and unary `+` give the values C does in
// both pipelines.
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

const std::vector<std::tuple<std::string, std::string, int>> kPrograms = {{"unary_plus", R"c(
        unsigned char c = 200;
        signed char s = -5;
        int n = +7;
        double d = +1.5;
        int arr[+4];
        int main(void) {
            int a = +c;
            int b = +s;
            long l = +(long)s;
            switch (a) { case +200: a = a + 1; }
            return a + b + (int)l + n + 3 + (int)sizeof(arr)
                   + (d > 1.0) + sizeof(+c);
        }
    )c",
                                                                           212},
                                                                          {"short", R"c(
        short s = -3;
        unsigned short us = 65535;
        short arr[3] = {1, -2, 30000};
        struct P { short a; unsigned short b; };
        struct P p = {-7, 40000};
        short twice(short x) { return x * 2; }
        unsigned short umax(void) { return us; }
        int main(void) {
            short int a = 100;
            signed short b = -50;
            unsigned short int c = 60000;
            long r = 0;
            r += a + b;
            r += (us + 1 == 0);
            r += (c > 0);
            r += sizeof(short) + sizeof(unsigned short) + sizeof s;
            r += twice(arr[2]) == -5536;
            r += p.a + (p.b == 40000);
            r += (unsigned short)70000;
            r += umax() == 65535;
            r += s;
            for (short i = 0; i < 3; i++) r += arr[i];
            return (int)(r & 0x7FFF);
        }
    )c",
                                                                           1746},
                                                                          {"unsigned_short_compare", R"c(
        unsigned short big = 40000;
        int main(void) { return (big > 30000) + (big > -1) * 2; }
    )c",
                                                                           1}};
TEST_CASE("short_and_unary_plus: asm sim") {
  for (const auto& [name, src, expected] : kPrograms) {
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
