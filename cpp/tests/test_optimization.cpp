// The tests of tests/test_optimization.py that ran compile.py: `-O`
// changes how a program is optimized, and `--tac` and `-S` still run
// with it.
#include <string>
#include <vector>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

constexpr const char* kSource = "int main(void) { return 42; }";

}  // namespace

TEST_CASE("optimization: --tac compiles with and without -O") {
  // Constant folding may give `return 42` a different TAC under -O, so
  // only the status is checked.
  for (const std::vector<std::string>& argv :
       {std::vector<std::string>{"c6502", "-", "--tac"},
        std::vector<std::string>{"c6502", "-", "--tac", "-O"}}) {
    INFO(argv.back());
    const auto o = suite::compile_main(argv, kSource);
    CHECK_FALSE(o.exited);
    CHECK(o.status == 0);
  }
}

TEST_CASE("optimization: -S compiles with and without -O") {
  for (const std::vector<std::string>& argv :
       {std::vector<std::string>{"c6502", "-", "-S"},
        std::vector<std::string>{"c6502", "-", "-S", "-O"}}) {
    INFO(argv.back());
    const auto o = suite::compile_main(argv, kSource);
    CHECK_FALSE(o.exited);
    CHECK(o.status == 0);
  }
}
