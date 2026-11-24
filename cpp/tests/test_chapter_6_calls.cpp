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

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nint main(void) {\\n    int x = 0;\\nlabel:\\n    x = 1;\\nlabel:\\n    return 2;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(LabeledStmt(\"label\" Expression(Assignment(Var(\"x\" None) Constant(ConstInt(1) None) None))))\n"
      "    S(LabeledStmt(\"label\" Return(Constant(ConstInt(2) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    goto label;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"label\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a;\\n    goto a;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Goto(\"a\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\n\\n\\nint main(void) {\\nlbl:\\n    return a;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"lbl\" Return(Var(\"a\" None))))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int x = 0;\\n    a:\\n    x = a;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(LabeledStmt(\"a\" Expression(Assignment(Var(\"x\" None) Var(\"a\" None) None))))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    if (1)\\n        return c;\\n    int c = 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Return(Var(\"c\" None)) None))\n"
      "    D(VarDecl(var_decl(\"c\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 2;\\n    int b = 1;\\n    a > b \077 a = 1 : a = 0;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Conditional(Binary(GreaterThan Var(\"a\" None) Var(\"b\" None) None) Assignment(Var(\"a\" None) Constant(ConstInt(1) None) None) Var(\"a\" None) None) Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_6.py::TestChapter6InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return a > 0 \077 1 : 2;\\n    int a = 5;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Conditional(Binary(GreaterThan Var(\"a\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
