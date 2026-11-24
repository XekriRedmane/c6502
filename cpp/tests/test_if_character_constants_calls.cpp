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
#include "preprocessor_case.h"

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if '\\\\n' == 10\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 2)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if '\\\\t' == 9\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 3)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if '\\\\\\\\' == 92\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 4)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if '\\\\'' == 39\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 5)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if '\\\\x41' == 65\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 6)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if '\\\\101' == 65\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 7)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if '\\\\0' == 0\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 8)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if 'a' == 97\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestCEscapes::test_values (call 9)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int yes;\\n\", \"source\": \"#if L'b' == 98\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestInvalidEscapes::test_named_escape_in_a_wide_constant (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int no;\\n\", \"source\": \"#if L'\\\\N{DIGIT ONE}' == 49\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"t.c:1 error: Could not evaluate expression due to TypeError('ord() expected a character, but string of length 13 found') (passed to evaluator: 'L'\\\\N{DIGIT ONE}' == 49')\\n\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestInvalidEscapes::test_named_escape_is_not_an_escape (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int no;\\n\", \"source\": \"#if '\\\\N{LATIN SMALL LETTER A}' == 97\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"t.c:1 error: Could not evaluate expression due to TypeError('ord() expected a character, but string of length 24 found') (passed to evaluator: ''\\\\N{LATIN SMALL LETTER A}' == 97')\\n\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestInvalidEscapes::test_unknown_escape (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int no;\\n\", \"source\": \"#if '\\\\q' == 113\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"t.c:1 error: Could not evaluate expression due to TypeError('ord() expected a character, but string of length 2 found') (passed to evaluator: ''\\\\q' == 113')\\n\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_if_character_constants.py::TestRewriting::test_failing_escape_is_kept (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": \"t.c\", \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"int no;\\n\", \"source\": \"#if '\\\\N\\\\xzz\\\\N' == 0\\nint yes;\\n#else\\nint no;\\n#endif\\n\", \"stderr\": \"t.c:1 error: Could not evaluate expression due to UnicodeDecodeError('unicodeescape', b'\\\\\\\\xzz', 0, 2, 'truncated \\\\\\\\xXX escape') (passed to evaluator: ''\\\\N\\\\xzz\\\\N' == 0')\\n\", \"warnings\": []}", {});
}

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
