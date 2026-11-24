// The calls that the tests of tests/test_chapter_5.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\nint main(void) {\\n    int a = 0;\\n    a + = 1;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ASSIGN', '=') at line 6, column 9.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int return = 4;\\n    return return + 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RETURN', 'return') at line 2, column 9.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 0;\\n    return a -- 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '1') at line 3, column 17.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 0;\\n    return a ++ 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '1') at line 3, column 17.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a += 0;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('PLUS_ASSIGN', '+=') at line 2, column 11.\\nExpected one of: \\n\\t* ASSIGN\\n\\t* ATTR_OPEN\\n\\t* COMMA\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a++;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('PLUSPLUS', '++') at line 2, column 10.\\nExpected one of: \\n\\t* ASSIGN\\n\\t* ATTR_OPEN\\n\\t* COMMA\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int foo bar = 3;\\n    return bar;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'bar') at line 2, column 13.\\nExpected one of: \\n\\t* ASSIGN\\n\\t* ATTR_OPEN\\n\\t* COMMA\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    ints a = 1;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'a') at line 2, column 10.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COLON\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void)\\n{\\n    int 10 = 0;\\n    return 10;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '10') at line 3, column 9.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\n\\nint main(void) {\\n    int a = 10;\\n    a =/ 1;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SLASH', '/') at line 7, column 8.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\nint main(void) {\\n    int a = 0;\\n    a - -;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 6, column 10.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\nint main(void) {\\n    int a = 0;\\n    a + +;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 6, column 10.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void)\\n{\\n\\n\\n\\n    return 1 < = 2;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ASSIGN', '=') at line 6, column 16.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void)\\n{\\n\\n\\n\\n    return 1 ! = 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('BANG', '!') at line 6, column 14.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 2\\n    a = a + 4;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'a') at line 3, column 5.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void)\\n{\\n    int 10 = return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '10') at line 3, column 9.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    a = 1 + 2;\\n    int a;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"a\" None) Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) None)))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 0;\\n    -a += 1;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Unary(Negate Var(\"a\" None) None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 10;\\n    (a += 1) -= 2;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Subtract CompoundAssignment(Add Var(\"a\" None) Constant(ConstInt(1) None) None None) Constant(ConstInt(2) None) None None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 10;\\n\\n\\n\\n    return a++--;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    S(Return(Postfix(Decrement Postfix(Increment Var(\"a\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 0;\\n    (a = 4)++;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Assignment(Var(\"a\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return --3;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Prefix(Decrement Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 1;\\n    ++(a+1);\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Expression(Prefix(Increment Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void){\\n    return a >> 2;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(RightShift Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    a += 1;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int b = 10;\\n    b *= a;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Multiply Var(\"b\" None) Var(\"a\" None) None None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    a--;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Decrement Var(\"a\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    a++;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"a\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 2;\\n    a + 3 = 4;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Binary(Add Var(\"a\" None) Constant(ConstInt(3) None) None) Constant(ConstInt(4) None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 2;\\n    !a = 3;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Unary(LogicalNot Var(\"a\" None) None) Constant(ConstInt(3) None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 1;\\n    int b = 2;\\n    a = 3 * b = a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Assignment(Binary(Multiply Constant(ConstInt(3) None) Var(\"b\" None) None) Var(\"a\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 1;\\n    int a = 2;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return 0 && a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(LogicalAnd Constant(ConstInt(0) None) Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return a < 5;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(LessThan Var(\"a\" None) Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return -a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(Negate Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_5.py::TestChapter5InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 0;\\n    return a;\\n    int a = 1;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
