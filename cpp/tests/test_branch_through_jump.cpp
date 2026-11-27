// The `-S -O` text of tests/test_branch_through_jump.py's backward
// `goto`. Its simulator run stays in Python.
#include <string>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("branch_through_jump: a goto backwards optimizes to straight-line code") {
  const std::string text =
      suite::codegen(R"(
        int main(void) {
            if (0)
            label:
                return 5;
            goto label;
            return 0;
        }
    )", true);
  const std::size_t at = text.find("main:");
  REQUIRE(at != std::string::npos);
  const std::string body = text.substr(at);
  CHECK_FALSE(suite::contains(body, "JMP"));
  CHECK(suite::search(R"(LDX\s+#\$00\s+LDA\s+#\$05\s+RTS)", body));
}
