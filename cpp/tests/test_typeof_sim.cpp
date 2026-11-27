// The simulator tests of tests/test_typeof.py: a program that checks C23's
// `typeof` and `typeof_unqual` (§6.7.3.6) returns 0 in both pipelines.
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

const std::string kProgram = R"c(int count;
int bump(void) { return ++count; }

int main(void) {
    int x = 0;
    typeof(x++) y = 5;
    typeof(bump()) z = 7;
    if (x != 0 || count != 0) return 1;
    if (y + z != 12) return 2;
    float val = 6.0f;
    int argc = 1;
    if ((typeof(typeof_unqual(typeof(argc))))val != 6) return 3;
    typeof(char[4]) four = { 1, 2, 3, 4 };
    if (sizeof four != 4 || four[3] != 4) return 4;
    return 0;
}
)c";
// The operand isn't evaluated (C23 §6.7.3.6p4), so `x++` and the
// call of `bump` don't happen. EXAMPLE 4's nested typeof is a cast
// to `int`.
TEST_CASE("typeof: asm sim") {
  for (bool optimize : {false, true}) {
    INFO("optimize=" << optimize);
    auto result = build_sim(kProgram, optimize).run(2'000'000);
    CHECK_FALSE(result.timed_out);
    CHECK(result.return_int_signed() == 0);
  }
}

}  // namespace
}  // namespace c6502::suite
