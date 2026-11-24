// The calls that the tests of tests/test_chapter_16.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 1)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    return '\\y';\n"
      "}\n",
      "{\"message\": \"3:12: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 2)") {
  c6502::testing::check_tokens(
      "char *s = \"hello\n"
      "    world \";\n"
      "\n"
      "    int\n"
      "    main(void)\n"
      "{\n"
      "    return 0;\n"
      "}\n",
      "{\"message\": \"1:11: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 3)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    char *str = \"foo\\ybar\";\n"
      "    return 0;\n"
      "}\n",
      "{\"message\": \"3:17: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 4)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    return '\\';\n"
      "}\n",
      "{\"message\": \"3:12: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 5)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    char *ptr = \"foo\"bar\";\n"
      "        return 0;\n"
      "}\n",
      "{\"message\": \"3:25: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 6)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    return ''';\n"
      "}\n",
      "{\"message\": \"3:12: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 7)") {
  c6502::testing::check_tokens(
      "int main(void)\n"
      "{\n"
      "    return 'x\n"
      "}\n",
      "{\"message\": \"3:12: unexpected character \\\"'\\\"\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("tokens: test_chapter_16.py::TestChapter16InvalidLex::test_lex_fails (call 8)") {
  c6502::testing::check_tokens(
      "int main(void) {\n"
      "    char *ptr = \"foo\\\";\n"
      "    return 0;\n"
      "}\n",
      "{\"message\": \"2:17: unexpected character '\\\"'\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    goto 'x';\\n    'x';\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('CHAR_CONSTANT', \\\"'x'\\\") at line 3, column 10.\\nExpected one of: \\n\\t* IDENTIFIER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    'x': return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('COLON', ':') at line 3, column 8.\\nExpected one of: \\n\\t* COMMA\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    goto \\\"foo\\\";\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRING_LITERAL', '\\\"foo\\\"') at line 3, column 10.\\nExpected one of: \\n\\t* IDENTIFIER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    \\\"foo\\\": return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('COLON', ':') at line 3, column 10.\\nExpected one of: \\n\\t* COMMA\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void)\\n{\\n\\n\\n    int char x = 10;\\n    return x;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'char' cannot combine with 'int' / 'long' / 'short'\"}\n");
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n\\n    char static long x = 0;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'char' cannot combine with 'int' / 'long' / 'short'\"}\n");
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\nint main(void) {\\n    int a = 3;\\n    return a'1';\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('CHAR_CONSTANT', \\\"'1'\\\") at line 6, column 13.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_16.py::TestChapter16InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nint main(void) {\\n    int \\\"x\\\" = 0;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRING_LITERAL', '\\\"x\\\"') at line 4, column 9.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}
