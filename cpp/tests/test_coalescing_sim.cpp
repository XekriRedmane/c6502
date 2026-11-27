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

namespace coalescing_end_to_end {
TEST_CASE("coalescing: uchar loop correct sum") {
  // 0 + 1 + 2 + 3 + 4 = 10.
  std::string src = R"c(#include <stdint.h>
int main(void) {
    uint8_t total = 0;
    for (uint8_t i = 0; i < 5; i++) total = total + i;
    return total;
}
)c";
  auto s = build_sim(src, Pipeline::kOptimized);
  auto result = s.run(5'000'000);
  CHECK_FALSE(result.timed_out);
  CHECK((result.return_int() & 0xFFFF) == 10);
}

}  // namespace coalescing_end_to_end

namespace byte_merge_sees_whole_class {
// fuzz.stmts seed 1921: in `p *= 5` (`(p << 2) + p`) the shift
// temp was put in p's slot, though p is read after the shift. The
// byte-level check looked only at names still in the graph, which
// left out p's byte that the move pairs had already merged.
TEST_CASE("coalescing: shift temp not merged with live operand") {
  std::string src = R"c(int g2 = 0;
signed char g4;
void f(unsigned int p) {
    unsigned char w;
    if (g2 == 0) {
        w = 0;
        do {
            if ((w | (p *= 5))) { }
        } while (++w < 2);
    }
    g4 = (2 ^ (p -= 255));
}
int main(void) { f(6); return g4; }
)c";
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto got = build_sim(src, optimize).run();
    CHECK(got.return_int_signed() == -107);
  }
}

}  // namespace byte_merge_sees_whole_class

}  // namespace
}  // namespace c6502::suite
