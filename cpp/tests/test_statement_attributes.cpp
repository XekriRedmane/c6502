// `[[c6502::keep]]` keeps a delay loop in the `--tac -O` text, from
// tests/test_statement_attributes.py.
#include <string>
#include <utility>

#include "compile_case.h"
#include "doctest.h"

namespace suite = c6502::suite;

TEST_CASE("statement_attributes: keep keeps a delay loop") {
  suite::Options options;
  options.optimize = true;
  for (const auto& c : {std::pair<std::string, bool>{"", false},
                        std::pair<std::string, bool>{"[[c6502::keep]] ", true}}) {
    const std::string& attribute = c.first;
    const bool kept = c.second;
    INFO("attribute=" << attribute);
    const std::string tac = suite::run_source(
        suite::Stage::kTac,
        "void f(void) { " + attribute + "for (unsigned char y = 0xB4; --y;) { } }", options);
    CHECK(suite::contains(tac, "_keep_start") == kept);
    CHECK(suite::contains(tac, "Jump(") == kept);
  }
}
