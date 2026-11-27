// tests/test_typeof.py's program of C23's typeof operators on the TAC
// interpreter.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("typeof: the program returns 0 on the TAC interpreter") {
  // The operand isn't evaluated (C23 §6.7.3.6p4), so `x++` and the call
  // of `bump` don't happen. EXAMPLE 4's nested typeof is a cast to
  // `int`.
  const auto value = suite::run_program(R"(int count;
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
)");
  REQUIRE(value.has_value());
  CHECK(static_cast<long long>(*value) == 0);
}
