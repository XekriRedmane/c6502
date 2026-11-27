#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("branch_through_jump sim: a goto backwards into an if runs") {
  const char* src = R"c(
        int main(void) {
            if (0)
            label:
                return 5;
            goto label;
            return 0;
        }
    )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 5);
  }
}

}  // namespace c6502::suite
