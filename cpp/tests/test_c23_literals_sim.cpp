// The simulator tests of tests/test_c23_literals.py: a program with C23's
// binary constants and digit separators (C23 §6.4.4.2) runs to the same
// result in both pipelines.
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

TEST_CASE("c23_literals: program") {
  std::string src = R"c(
#define K 1'000
#if 0b1 && K == 1000
static unsigned long table[] = { 0b1, 0b10, 0x1'00, 1'000'000 };
#endif
int main(void) {
    unsigned long sum = 0;
    for (unsigned char i = 0; i < 4; i++)
        sum += table[i];
    double d = 1'000.5;
    if (d != 1000.5)
        return -1;
    return (int)(sum - 1'000'000UL) + 0b1000'0000 + K;
}
)c";
  auto want = 1 + 2 + 256 + 128 + 1000;
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto result = build_sim(src, optimize).run();
    CHECK_FALSE(result.timed_out);
    CHECK(result.return_int() == want);
  }
}

}  // namespace
}  // namespace c6502::suite
