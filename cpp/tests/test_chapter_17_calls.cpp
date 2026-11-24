// The calls that the tests of tests/test_chapter_17.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n  unsigned void *v;\\n  return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'void' cannot combine with any other type specifier\"}\n");
}

TEST_CASE("parse: test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void char *x;\\n\\nint main(void) { return 0; }\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'void' cannot combine with any other type specifier\"}\n");
}

TEST_CASE("parse: test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nint main(void) {\\n    return sizeof(char) 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '1') at line 4, column 25.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COMMA\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACE\\n\\t* LE\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_17.py::TestChapter17InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    return sizeof int;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INT', 'int') at line 3, column 19.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}
