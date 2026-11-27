// Programs whose address offsets src/passes/optimization/reassoc_offset.h
// reassociates and whose loop-invariant extends licm.h hoists, run in
// both pipelines, from tests/test_reassoc_offset_licm.py.
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

TEST_CASE("reassoc_offset_licm sim: header fields after a skip of variable length") {
  check_both(R"c(unsigned char t[12] = {2, 9, 9, 7, 1, 4, 0, 0, 0, 0, 0, 0};
int f(unsigned char *d) {
    unsigned char n = d[0];
    return d[n + 1] * 100 + d[n + 2] * 10 + d[n + 3];
}
int main(void) { return f(t); }
)c", 714);
}

TEST_CASE("reassoc_offset_licm sim: a pointer advanced by a byte in a loop") {
  // 1 + 4 + ... + 34, twelve terms.
  std::int64_t want = 0;
  for (int k = 0; k < 12; ++k) want += 1 + 3 * k;
  check_both(R"c(unsigned char t[40];
int f(unsigned char *p, unsigned char step, unsigned char n) {
    int sum = 0;
    while (n--) { sum += *p; p += step; }
    return sum;
}
int main(void) {
    for (unsigned char i = 0; i < 40; i++) t[i] = i;
    return f(t + 1, 3, 12);
}
)c", want);
}

}  // namespace c6502::suite
