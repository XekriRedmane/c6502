// The `-S -O` text of a program with two live locals, from
// tests/test_asm_regalloc.py.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("asm_regalloc: two locals land in the caller-saved zero-page pool") {
  const auto o = suite::compile_main(
      {"compile.py", "-", "-S", "-O"},
      "int f(int p, int q) { int a = p + 1; int b = a + q; return b; }\n"
      "int main(void) { return f(3, 4); }\n");
  REQUIRE(o.status.has_value());
  CHECK(*o.status == 0);
  const std::string out = o.out();
  CHECK(suite::search(R"(LDA\s+__local_\w+__\w+)", out));
  CHECK(suite::search(R"(__local_\w+__\w+\s+EQU\s+\$[89AB][0-9A-F])", out));
}
