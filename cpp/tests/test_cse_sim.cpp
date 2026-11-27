// Programs whose repeated subscripts and tests go through
// common-subexpression elimination (src/passes/optimization/cse.h),
// run in both pipelines, from tests/test_cse.py.
#include <string>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

void check_both(const std::string& src, std::int64_t want) {
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    CHECK(build_sim(src, optimize).run().return_int() == want);
  }
}

}  // namespace

TEST_CASE("cse sim: repeated subscripts and tests") {
  // cmp(1, 0) is -1, since 1 < 3. cmp(3, 0) is 3 + 7 = 10. cmp(9, 4) is
  // 1, since 9 > 2.
  check_both(R"c(unsigned char t[8] = {9, 3, 3, 7, 1, 2, 5, 4};
unsigned char *p = t;
int cmp(unsigned char k, unsigned char n) {
    if (k < p[n + 1]) return -1;
    if (k == p[n + 1]) return p[n + 2] + p[n + 3];
    return 1;
}
int main(void) {
    return cmp(1, 0) * 100 + cmp(3, 0) + cmp(9, 4) * 1000;
}
)c", -100 + 10 + 1000);
}

TEST_CASE("cse sim: a store between two loads of an element") {
  check_both(R"c(unsigned char t[4] = {1, 2, 3, 4};
int f(unsigned char *p) {
    int a = p[1];
    p[1] = 9;
    return a * 10 + p[1];
}
int main(void) { return f(t); }
)c", 29);
}

TEST_CASE("cse sim: doubles") {
  check_both(R"c(int f(double d) {
    if (d - 1 != 5) return 1;
    if (d - 2 != 4) return 2;
    return 0;
}
int main(void) { return f(6.0); }
)c", 0);
}

}  // namespace c6502::suite
