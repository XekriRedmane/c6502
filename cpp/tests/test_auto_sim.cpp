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

const std::string kProgram = R"c(static auto a = 3.5;
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
)c";
// `u` is an `unsigned char`, so it wraps at 256, and `s` a `long`,
// so the sum of 0 to 299 fits.
TEST_CASE("auto: asm sim") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto result = build_sim(kProgram, optimize).run(4'000'000);
    CHECK_FALSE(result.timed_out);
    CHECK(result.return_int_signed() == 0);
  }
}

}  // namespace
}  // namespace c6502::suite
