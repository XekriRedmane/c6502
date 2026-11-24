// The calls that the tests of tests/test_chapter_6.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "lexer_case.h"

TEST_CASE("tokens: test_chapter_6.py::TestChapter6InvalidLex::test_lex_fails (call 1)") {
  c6502::testing::check_tokens(
      "int main(void) {\n"
      "    0invalid_label:\n"
      "        return 0;\n"
      "}\n",
      "{\"message\": \"2:5: invalid pp-number: 0'invalid_label' (numeric constant may not abut an identifier)\", \"type\": \"LexError\"}",
      true);
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    if (5)\\n        int i = 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INT', 'int') at line 3, column 9.\\nExpected one of: \\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR_CONSTANT\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* SWITCH\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    if (0) else return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ELSE', 'else') at line 2, column 12.\\nExpected one of: \\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR_CONSTANT\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* SWITCH\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    goto;\\nlbl:\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 2, column 9.\\nExpected one of: \\n\\t* IDENTIFIER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return: return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('COLON', ':') at line 2, column 11.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\nlabel:\\n    int a = 0;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INT', 'int') at line 4, column 5.\\nExpected one of: \\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR_CONSTANT\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* SWITCH\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    1 && label: 2;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('COLON', ':') at line 2, column 15.\\nExpected one of: \\n\\t* COMMA\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"label:\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'label') at line 1, column 1.\\nExpected one of: \\n\\t* $END\\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* ATTR_OPEN\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n    foo:\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RBRACE', '}') at line 4, column 1.\\nExpected one of: \\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR_CONSTANT\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* SWITCH\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    goto(a);\\na:\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('LPAREN', '(') at line 2, column 9.\\nExpected one of: \\n\\t* IDENTIFIER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int flag = 0;\\n    int a = if (flag)\\n                2;\\n            else\\n                3;\\n    return a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IF', 'if') at line 3, column 13.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    if 0 return 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '0') at line 2, column 8.\\nExpected one of: \\n\\t* LPAREN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return 1 \077 2;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 2, column 17.\\nExpected one of: \\n\\t* COLON\\n\\t* COMMA\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return 1 \077 2 : 3 : 4;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('COLON', ':') at line 2, column 22.\\nExpected one of: \\n\\t* COMMA\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return 1 \077 2 \077 3 : 4;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 2, column 25.\\nExpected one of: \\n\\t* COLON\\n\\t* COMMA\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 0;\\n    if (1)\\n        return 1;\\n    else\\n        return 2;\\n    else\\n        return 3;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ELSE', 'else') at line 7, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* AUTO\\n\\t* BANG\\n\\t* BITINT\\n\\t* BITINT_INTEGER\\n\\t* BOOL\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR\\n\\t* CHAR_CONSTANT\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE\\n\\t* DOUBLE_CONSTANT\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FALSE\\n\\t* FLOAT\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INT\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RBRACE\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRING_LITERAL\\n\\t* STRUCT\\n\\t* SWITCH\\n\\t* THREAD_LOCAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* TYPEDEF\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\n    int x = 10;\\n\\n    return x \077 1 = 2;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 5, column 21.\\nExpected one of: \\n\\t* COLON\\n\\t* COMMA\\n\"}\n");
}
