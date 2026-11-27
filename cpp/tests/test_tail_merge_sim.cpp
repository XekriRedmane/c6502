// A function with early returns whose tails src/passes/tail_merge.h
// merges, run in both pipelines, from tests/test_tail_merge.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("tail_merge sim: early returns") {
  const char* src = R"c(unsigned int f(unsigned char a, unsigned char b) {
    if (a < 3) return 0;
    if (b < 3) return 0;
    if (a == b) return 0;
    return a + b;
}
int main(void) {
    return f(1, 9) + f(9, 1) + f(5, 5) + f(4, 6) * 10;
}
)c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 100);
  }
}

}  // namespace c6502::suite
