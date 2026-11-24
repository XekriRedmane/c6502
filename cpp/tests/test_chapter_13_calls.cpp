// The calls that the tests of tests/test_chapter_13.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("tokens: test_chapter_13.py::TestChapter13InvalidLex::test_lex_fails (call 1)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "\n"
      "\n"
      "\n"
      "\n"
      "\n"
      "    return 1.ex;\n"
      "}\n",
      "{\"message\": \"8:12: invalid pp-number: 1.'ex' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_13.py::TestChapter13InvalidLex::test_lex_fails (call 2)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    double foo = 1E2x;\n"
      "}\n",
      "{\"message\": \"3:18: invalid pp-number: 1E2'x' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_13.py::TestChapter13InvalidLex::test_lex_fails (call 3)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "\n"
      "\n"
      "\n"
      "\n"
      "\n"
      "    return 2._;\n"
      "}\n",
      "{\"message\": \"8:12: invalid pp-number: 2.'_' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_13.py::TestChapter13InvalidLex::test_lex_fails (call 4)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "\n"
      "\n"
      "\n"
      "\n"
      "\n"
      "    double d = 1.0e10.0;\n"
      "    return 0;\n"
      "}\n",
      "1:1\011keyword\011int\n"
      "1:5\011identifier\011main\n"
      "1:9\011symbol\011(\n"
      "1:10\011keyword\011void\n"
      "1:14\011symbol\011)\n"
      "2:1\011symbol\011{\n"
      "8:5\011keyword\011double\n"
      "8:12\011identifier\011d\n"
      "8:14\011symbol\011=\n"
      "8:16\011constant\011" "1.0e10\n"
      "8:22\011constant\011.0\n"
      "8:24\011symbol\011;\n"
      "9:5\011keyword\011return\n"
      "9:12\011constant\011" "0\n"
      "9:13\011symbol\011;\n"
      "10:1\011symbol\011}\n",
      false);
}

TEST_CASE("tokens: test_chapter_13.py::TestChapter13InvalidLex::test_lex_fails (call 5)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "\n"
      "\n"
      "\n"
      "    double foo = 30.e;\n"
      "    return 4;\n"
      "}\n",
      "{\"message\": \"6:18: invalid pp-number: 30.'e' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_13.py::TestChapter13InvalidLex::test_lex_fails (call 6)") {
  c6502::testing::check_tokens(
      "int main(void) {\n"
      "\n"
      "\n"
      "\n"
      "    double foo = 24e-;\n"
      "}\n",
      "{\"message\": \"5:18: malformed numeric token '24e'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_13.py::TestChapter13InvalidLex::test_lex_fails (call 7)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    return 1.e-10x;\n"
      "}\n",
      "{\"message\": \"3:12: invalid pp-number: 1.e-10'x' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("parse: test_chapter_13.py::TestChapter13InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_13.py::TestChapter13InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n    unsigned double d = 10.0;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"floating type cannot combine with 'int' / 'signed' / 'unsigned' / 'char'\"}\n");
}

TEST_CASE("parse: test_chapter_13.py::TestChapter13InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_13.py::TestChapter13InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n    double double d = 10.0;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'double' specified more than once\"}\n");
}
