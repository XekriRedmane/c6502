// A signed count-down loop whose `DEC m; LDA m; BPL` the peephole
// `redundant_load_after_rmw` makes `DEC m; BPL`, run under -O, from
// tests/test_redundant_load_after_rmw.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("redundant_load_after_rmw sim: the count-down loop sums its values") {
  const sim::SimResult result = build_sim(R"c(int main(void) {
    int sum = 0;
    for (int x = 15; x >= 0; x--) sum += x;
    return sum;
}
)c", Pipeline::kOptimized).run(200'000);
  CHECK_FALSE(result.timed_out);
  CHECK(result.return_int_signed() == 120);
}

}  // namespace c6502::suite
