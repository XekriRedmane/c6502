// tests/test_short_and_unary_plus.py's programs on the TAC interpreter.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

struct Program {
  const char* name;
  std::string source;
  long long expected;
};

// Returns the low 16 bits of `main`'s value on the TAC, as an `int`.
long long tac_int(const std::string& source) {
  const auto value = suite::run_program(source);
  REQUIRE(value.has_value());
  long long v = static_cast<long long>(*value & 0xFFFF);
  if (v & 0x8000) v -= 0x10000;
  return v;
}

}  // namespace

TEST_CASE("short_and_unary_plus: the programs return their values on the TAC interpreter") {
  const std::vector<Program> programs = {
      {"unary_plus", R"(
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
    )", 212},
      {"short", R"(
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
    )", 1746},
      {"unsigned_short_compare", R"(
        unsigned short big = 40000;
        int main(void) { return (big > 30000) + (big > -1) * 2; }
    )", 1},
  };
  for (const Program& p : programs) {
    INFO(p.name);
    CHECK(tac_int(p.source) == p.expected);
  }
}
