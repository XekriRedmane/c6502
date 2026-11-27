// Two pointers to one buffer, which the asm-level aliasing predicate
// must take as possibly the same address, run in both pipelines, from
// tests/test_asm_aliasing.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("asm_aliasing sim: two pointers to one buffer") {
  // Under -O `p[i]` and `q[i]` are `(zp),Y` through two bases, and the
  // read through `p` after the store through `q` must see the 9.
  // (1 + 2 + 3 + 4) + 4 * 9 = 46.
  const char* src = R"c(
        unsigned char buf[4];
        unsigned char g(unsigned char *p, unsigned char *q) {
            unsigned char s = 0;
            for (unsigned char i = 0; i < 4; i++) {
                p[i] = i + 1;
                s += q[i];
                q[i] = 9;
                s += p[i];
            }
            return s;
        }
        int main(void) { return g(buf, buf); }
    )c";
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == 46);
  }
}

}  // namespace c6502::suite
