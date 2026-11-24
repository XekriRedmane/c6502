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

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int y;\\n};\\n\\nint main(void) {\\n    struct s *ptr = 0;\\n    return ptr->;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 7, column 17.\\nExpected one of: \\n\\t* IDENTIFIER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int y;\\n};\\n\\nstruct s x;\\n\\n\\nint main(void) {\\n    return x.(y);\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('LPAREN', '(') at line 9, column 14.\\nExpected one of: \\n\\t* IDENTIFIER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    return .a;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('DOT', '.') at line 2, column 12.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct pair {\\n    int a;\\n    int b;\\n};\\n\\nint main(void) {\\n\\n\\n\\n    struct pair x.a = 10;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('DOT', '.') at line 10, column 18.\\nExpected one of: \\n\\t* ASSIGN\\n\\t* ATTR_OPEN\\n\\t* COMMA\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nint main(void) {\\n    switch (0) {\\n        case 0:\\n            struct s {\\n                int a;\\n            };\\n            return 0;\\n    }\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRUCT', 'struct') at line 6, column 13.\\nExpected one of: \\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR_CONSTANT\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* SWITCH\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nstruct s {\\n    int default;\\n};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('DEFAULT', 'default') at line 3, column 9.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STRUCT\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nstruct goto { int a; };\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('GOTO', 'goto') at line 2, column 8.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nstruct s {\\n    int i;\\n\\n\\n    foo : int j;\\n};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'foo') at line 7, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RBRACE\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nint main(void) {\\nfoo:\\n    struct s {\\n        int a;\\n    };\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRUCT', 'struct') at line 5, column 5.\\nExpected one of: \\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR_CONSTANT\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* SWITCH\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nunion struct s {\\n    int a;\\n};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRUCT', 'struct') at line 2, column 7.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"union u {\\n    int a;\\n};\\n\\nunion union u x;\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('UNION', 'union') at line 5, column 7.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nunion x long a;\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"struct/union specifier cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nunion a { int a; };\\n\\nint main(void) {\\n    union a int x;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"struct/union specifier cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nunion s {};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RBRACE', '}') at line 3, column 10.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nunion u {\\n    int a;\\n    ;\\n};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 4, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RBRACE\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"union a {\\n\\n    int member = 1;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ASSIGN', '=') at line 3, column 16.\\nExpected one of: \\n\\t* COMMA\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"union s {\\n\\n\\n\\n    int foo(void);\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"struct/union member 'foo' cannot have function type\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nunion u {\\n    int struct;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 4, column 15.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"union u {\\n    int;\\n\\n\\n\\n\\n\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 2, column 8.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STRUCT\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"union u {\\n    a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'a') at line 2, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"union y {\\n\\n    static int a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STATIC', 'static') at line 3, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nunion struct {\\n    int a;\\n};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRUCT', 'struct') at line 2, column 7.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nunion x y {\\n    int a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function definition's declarator must compose to a function type; got Union\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    union 4 foo;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '4') at line 2, column 11.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int y;\\n};\\n\\nint main(void) {\\n\\n    union(s) var;\\n\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('LPAREN', '(') at line 7, column 10.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int a;\\n};\\n\\n\\nstruct static s foo;\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STATIC', 'static') at line 6, column 8.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int a;\\n    ;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 3, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RBRACE\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nstruct s {};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RBRACE', '}') at line 3, column 11.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    ;\\n    int a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 2, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\ns struct x { int a; };\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 's') at line 2, column 1.\\nExpected one of: \\n\\t* $END\\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* ATTR_OPEN\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 31)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int a;\\n}\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"struct/union specifier cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 32)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\nstruct for {\\n    int a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('FOR', 'for') at line 2, column 8.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 33)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct struct s;\\n\\nint main(void) {\\n    return 1;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRUCT', 'struct') at line 1, column 8.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 34)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct a {\\n\\n    int member = 1;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ASSIGN', '=') at line 3, column 16.\\nExpected one of: \\n\\t* COMMA\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 35)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n\\n\\n\\n    int foo(void);\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"struct/union member 'foo' cannot have function type\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 36)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nstruct s {\\n    int return;\\n}\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RETURN', 'return') at line 4, column 9.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STRUCT\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 37)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int;\\n\\n\\n\\n\\n\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('SEMICOLON', ';') at line 2, column 8.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STRUCT\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 38)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int a\\n};\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RBRACE', '}') at line 3, column 1.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* COMMA\\n\\t* LBRACKET\\n\\t* LPAREN\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 39)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'a') at line 2, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 40)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct y {\\n\\n    static int a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STATIC', 'static') at line 3, column 5.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ATTR_OPEN\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STRUCT\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 41)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    struct 4 foo;\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('INTEGER_CONSTANT', '4') at line 2, column 12.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 42)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int y;\\n};\\n\\nint main(void) {\\n\\n    struct(s) var;\\n\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('LPAREN', '(') at line 7, column 11.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 43)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s;\\n\\n\\nstruct s long a;\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"struct/union specifier cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 44)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    struct x;\\n    x y;\\n\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'y') at line 3, column 7.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COLON\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 45)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s {\\n    int a;\\n};\\n\\nstruct struct s x;\\n\\nint main(void) {\\n    return 0;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('STRUCT', 'struct') at line 5, column 8.\\nExpected one of: \\n\\t* ATTR_OPEN\\n\\t* IDENTIFIER\\n\\t* LBRACE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails (call 46)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_18.py::TestChapter18InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nstruct x y {\\n    int a;\\n};\\n\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function definition's declarator must compose to a function type; got Structure\"}\n");
}
