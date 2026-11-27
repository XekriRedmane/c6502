#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("index_inc_cmp_y sim: a loop and a global keep their values") {
  // The temporary keeps its old value after the rewrite, so it must be
  // compiler-owned scratch that no path reads afterward. `top` is a
  // global that `main` reads after `check`.
  CHECK(build_sim(R"c(
unsigned char ceiling[8] = {1,2,3,4,5,6,7,8};
unsigned char out[8];
unsigned char beam_y, hit, top;
void check(unsigned char i) { top = ceiling[i]; top++; if (top == beam_y) hit = 1; }
void scan(void) {
    unsigned char prev = 0;
    for (unsigned char i = 0; i < 8; i++) {
        out[i] = prev;
        prev = ceiling[i];
        prev++;
        if (prev == beam_y) hit = 1;
    }
}
int main(void) { beam_y = 99; check(2); scan(); return top * 100 + out[3]; }
)c", true).run().return_int() == 404);
}

}  // namespace c6502::suite
