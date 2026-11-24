// tests/test_if_character_constants.py's TestRewriting, ported by hand to
// `preprocessor::without_named_escapes`, the C++ form of
// preprocessor.py's `_without_named_escapes`. The preprocessor's stderr
// for the constant whose `\x` fails to decode, which the second Python
// test also checked, is a recorded call in
// test_if_character_constants_calls.cpp.
#include <string>
#include <utility>
#include <vector>

#include "doctest.h"
#include "preprocessor.h"

using c6502::preprocessor::without_named_escapes;

TEST_CASE("if_character_constants: TestRewriting::test_spellings") {
  // Each backslash the expansion leaves is doubled, and every other
  // escape is expanded.
  const std::vector<std::pair<std::string, std::string>> cases = {
      {R"('\N{A}')", R"('\\N{A}')"},
      {R"('\N{\x41}')", R"('\\N{A}')"},
      {R"('\N{\N{A}}')", R"('\\N{\\N{A}}')"},
      {R"(L'\N')", R"(L'\\N')"},
      {R"('\\N{A}')", R"('\\N{A}')"},
      {R"('\n\N')", "'\n\\\\N'"},
  };
  for (const auto& [spelling, rewritten] : cases) {
    CAPTURE(spelling);
    CHECK(without_named_escapes(spelling) == rewritten);
  }
}

TEST_CASE("if_character_constants: TestRewriting::test_failing_escape_is_kept") {
  // `\u` has only three characters after it here, so it isn't an
  // escape, and the backslash of `\N` is written out.
  CHECK(without_named_escapes(R"('\u0\N')") == R"('\\u0\\N')");
  // `\x` takes `zz`, which the codec can't decode, so the text from
  // there on is left for pcpp, which reports the same error.
  CHECK(without_named_escapes(R"('\N\xzz\N')") == R"('\\N\xzz\N')");
}
