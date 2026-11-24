// The calls that the tests of tests/test_chapter_12.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("tokens: test_chapter_12.py::TestChapter12InvalidLex::test_lex_fails (call 1)") {
  c6502::testing::check_tokens(
      "int main(void) {\n"
      "\n"
      "    return 0uu;\n"
      "}\n",
      "{\"message\": \"3:12: invalid pp-number: 0u'u' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_12.py::TestChapter12InvalidLex::test_lex_fails (call 2)") {
  c6502::testing::check_tokens(
      "int main(void) {\n"
      "\n"
      "    return 0lul;\n"
      "}\n",
      "{\"message\": \"3:12: invalid pp-number: 0lu'l' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("parse: test_chapter_12.py::TestChapter12InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_12.py::TestChapter12InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n    int i = 0;\\n    return (signed unsigned) i;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'signed' and 'unsigned' cannot both appear in a declaration\"}\n");
}

TEST_CASE("parse: test_chapter_12.py::TestChapter12InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_12.py::TestChapter12InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n    unsigned long unsigned i = 0;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'unsigned' specified more than once\"}\n");
}
