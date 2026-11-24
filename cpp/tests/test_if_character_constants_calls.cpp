// The calls that the tests of tests/test_if_character_constants.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("tokens: test_if_character_constants.py::TestOutsideIf::test_lex_errors (call 1)") {
  c6502::testing::check_tokens(
      "int x = '\\q';",
      "{\"message\": \"1:9: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_if_character_constants.py::TestOutsideIf::test_lex_errors (call 2)") {
  c6502::testing::check_tokens(
      "int x = '\\N{LATIN SMALL LETTER A}';",
      "{\"message\": \"1:9: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}
