// The calls that the tests of tests/test_chapter_9.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\n\\n\\n\\nint main(void) {\\n    return 1();\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"called object is not a function\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\nint foo(int x, int y} { return x + y; }\\n\\nint main(void) { return 0;}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RBRACE', '}') at line 4, column 21.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* COMMA\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* RPAREN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n    for (int f(void); ; ) {\\n        return 0;\\n    }\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function declarations aren't permitted in a for-loop initializer (C99 \\u00a76.8.5.3)\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\nint foo(int x, int y) {\\n    return x + y;\\n}\\n\\nint main(void) { return foo(1, 2};}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RBRACE', '}') at line 8, column 33.\\nExpected one of: \\n\\t* COMMA\\n\\t* RPAREN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a) {\\n    return 0;\\n}\\n\\nint main(void) {\\n\\n    return foo(int a);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INT', 'int') at line 7, column 16.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RPAREN\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\n\\n\\nint foo(void)(void);\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function declarator cannot specify a function-typed return (C99 \\u00a76.7.5.3.1)\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\n\\n\\nint foo(void) = 3;\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function declaration cannot have an initializer\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a, int b, int c) {\\n    return a + b + c;\\n}\\n\\nint main(void) {\\n\\n    return foo(1, 2, 3,);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RPAREN', ')') at line 7, column 24.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint foo(int a,) {\\n    return a + 1;\\n}\\n\\nint main(void) {\\n    return foo(4);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RPAREN', ')') at line 2, column 15.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* ATTR_OPEN\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ELLIPSIS\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a, int b {\\n    return 0;\\n}\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('LBRACE', '{') at line 1, column 22.\\nExpected one of: \\n\\t* COMMA\\n\\t* RPAREN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_9.py::TestChapter9InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint bad_params(int a = 3) {\\n    return 1;\\n}\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ASSIGN', '=') at line 2, column 22.\\nExpected one of: \\n\\t* COMMA\\n\\t* RPAREN\\n\"}\n");
}
