// The calls that the tests of tests/test_chapter_18.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("tokens: test_chapter_18.py::TestChapter18InvalidLex::test_lex_fails (call 1)") {
  c6502::testing::check_tokens(
      "struct s {\n"
      "    int a;\n"
      "};\n"
      "\n"
      "int main(void) {\n"
      "    struct s x;\n"
      "\n"
      "\n"
      "    return x.1l;\n"
      "}\n",
      "1:1\011keyword\011struct\n"
      "1:8\011identifier\011s\n"
      "1:10\011symbol\011{\n"
      "2:5\011keyword\011int\n"
      "2:9\011identifier\011a\n"
      "2:10\011symbol\011;\n"
      "3:1\011symbol\011}\n"
      "3:2\011symbol\011;\n"
      "5:1\011keyword\011int\n"
      "5:5\011identifier\011main\n"
      "5:9\011symbol\011(\n"
      "5:10\011keyword\011void\n"
      "5:14\011symbol\011)\n"
      "5:16\011symbol\011{\n"
      "6:5\011keyword\011struct\n"
      "6:12\011identifier\011s\n"
      "6:14\011identifier\011x\n"
      "6:15\011symbol\011;\n"
      "9:5\011keyword\011return\n"
      "9:12\011identifier\011x\n"
      "9:13\011constant\011.1l\n"
      "9:16\011symbol\011;\n"
      "10:1\011symbol\011}\n",
      false);
}

TEST_CASE("tokens: test_chapter_18.py::TestChapter18InvalidLex::test_lex_fails (call 2)") {
  c6502::testing::check_tokens(
      "struct s {\n"
      "    int a;\n"
      "};\n"
      "\n"
      "int main(void) {\n"
      "    struct s x;\n"
      "\n"
      "    return x.0foo;\n"
      "}\n",
      "{\"message\": \"8:13: invalid pp-number: .0f'oo' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}
