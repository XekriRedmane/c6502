// A program that uses C23's `bool`, `true`, `false`, and <stdbool.h>,
// run in both pipelines, from tests/test_c23_bool.py.
#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

TEST_CASE("c23_bool sim: the program runs to its result") {
  const char* src = R"c(#include <stdbool.h>

enum { TWO = true + true };
static bool flags[TWO + 1] = { true, false, true };
static int seen = false;
bool global = 7;

[[c6502::return_bool_in_carry]] bool positive(int x) { return x > 0; }

bool odd(unsigned char x) { return x & 1; }

int classify(bool b) {
    switch (b) {
    case true:
        return 10;
    case false:
        return 20;
    }
    return 0;
}

int main(void) {
    bool b = 256;
    int r = 0;
    for (unsigned char i = 0; i < sizeof flags; i++)
        if (flags[i] == true)
            r += 1;
    if (positive(5) && !positive(-5))
        r += 100;
    r += classify(odd(3)) + classify(odd(4));
    r += b + global + seen + TWO + sizeof(true) + (true << 9);
    return r;
}
)c";
  // 2 flags + 100 + 10 + 20 + 1 + 1 + 0 + 2 + 1 + 512.
  const std::int64_t expected = 649;
  for (const bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    const sim::SimResult r = build_sim(src, optimize).run(2'000'000);
    CHECK_FALSE(r.timed_out);
    CHECK(r.return_int_signed() == expected);
  }
}

}  // namespace c6502::suite
