// The tests of tests/test_copy_folding.py that compiled a program with
// `-O`. `fold_copies` makes a producer write the name a single-use
// temporary was copied to, which leaves an in-place add that the INC
// peephole turns into `INC; BNE; INC`.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace {

namespace suite = c6502::suite;

}  // namespace

TEST_CASE("copy_folding: a static counter's += 1 is an INC chain") {
  // Without the fold, the add goes through a temporary and a copy back.
  const std::string text = suite::codegen(
      "static int counter = 0;\n"
      "int main(void) { counter += 1; return counter; }\n",
      true);
  CHECK(suite::contains(text, "INC   counter"));
  CHECK(suite::contains(text, "BNE   .inc_done@"));
  CHECK(suite::contains(text, "INC   counter+1"));
}

TEST_CASE("copy_folding: a loop counter's ++ is an INC chain") {
  // from_ssa puts a copy on the back edge for `i++`. The
  // `.inc_done@` label is the target of the 16-bit INC's BNE.
  const std::string text = suite::codegen(
      "int main(void) {\n"
      "    int s = 0;\n"
      "    for (int i = 0; i < 10; i++) s += 1;\n"
      "    return s;\n"
      "}\n",
      true);
  CHECK(suite::search(R"(INC\s+__local_\w+__\w+)", text));
  CHECK(suite::contains(text, ".inc_done@"));
}
