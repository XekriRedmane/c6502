// tests/test_auto.py's program of C23's `auto` on the TAC interpreter.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("auto: the program returns 0 on the TAC interpreter") {
  // `u` is an `unsigned char`, so it wraps at 256, and `s` a `long`,
  // so the sum of 0 to 299 fits.
  const auto value = suite::run_program(R"(static auto a = 3.5;
auto p = &a;

int count(void) {
    static auto n = 0;
    return ++n;
}

int main(void) {
    double x = 7;
    double y = 9;
    {
        auto y = x * x;
        auto x = y;
        if (x != 49.0) return 1;
    }
    if (*p != 3.5) return 2;
    auto u = (unsigned char)250;
    u += 10;
    if (u != 4) return 3;
    auto s = 0L;
    for (auto i = 0; i < 300; i++)
        s += i;
    if (s != 44850L) return 4;
    count();
    count();
    if (count() != 3) return 5;
    auto int z = 8;
    return z - 8;
}
)");
  REQUIRE(value.has_value());
  CHECK(static_cast<long long>(*value) == 0);
}
