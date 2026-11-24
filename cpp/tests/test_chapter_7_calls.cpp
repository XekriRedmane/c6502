// The calls that the tests of tests/test_chapter_7.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    if(0){\\n        return 1;\\n    }}\\n    return 2;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RETURN', 'return') at line 5, column 5.\\nExpected one of: \\n\\t* $END\\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* ATTR_OPEN\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* INT\\n\\t* LONG\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    if(0){\\n        return 1;\\n    return 2;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('$END', '') at line 5, column 1.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AMP\\n\\t* ATTR_OPEN\\n\\t* AUTO\\n\\t* BANG\\n\\t* BITINT\\n\\t* BITINT_INTEGER\\n\\t* BOOL\\n\\t* BREAK\\n\\t* CASE\\n\\t* CHAR\\n\\t* CHAR_CONSTANT\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* CONTINUE\\n\\t* DEFAULT\\n\\t* DO\\n\\t* DOUBLE\\n\\t* DOUBLE_CONSTANT\\n\\t* ELSE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FALSE\\n\\t* FLOAT\\n\\t* FLOAT_CONSTANT\\n\\t* FOR\\n\\t* GOTO\\n\\t* IDENTIFIER\\n\\t* IF\\n\\t* INT\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LONG\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RBRACE\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* RETURN\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRING_LITERAL\\n\\t* STRUCT\\n\\t* SWITCH\\n\\t* THREAD_LOCAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* TYPEDEF\\n\\t* TYPEDEF_NAME\\n\\t* TYPEOF\\n\\t* TYPEOF_UNQUAL\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\\t* WHILE\\n\"}\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 4;\\n    {\\n        a = 5;\\n        return a\\n    }\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('RBRACE', '}') at line 6, column 5.\\nExpected one of: \\n\\t* COMMA\\n\\t* SEMICOLON\\n\"}\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidParse::test_parse_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a;\\n    return 1 \077 { a = 2 } : a = 4;\\n}\\n\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('LBRACE', '{') at line 3, column 16.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    {\\n        int a;\\n        int a;\\n    }\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a = 3;\\n    {\\n        a = 5;\\n    }\\n    int a = 2;\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Expression(Assignment(Var(\"a\" None) Constant(ConstInt(5) None) None)))\n"
      "    ])))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n\\nlabel1:;\\n    int a = 10;\\nlabel2:;\\n    int a = 11;\\n    return 1;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"label1\" Null))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    S(LabeledStmt(\"label2\" Null))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(11) None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n\\nint main(void) {\\n    int x = 0;\\n    if (x) {\\n        x = 5;\\n        goto l;\\n        return 0;\\n        l:\\n            return x;\\n    } else {\\n        goto l;\\n        return 0;\\n        l:\\n            return x;\\n    }\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(IfStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(Expression(Assignment(Var(\"x\" None) Constant(ConstInt(5) None) None)))\n"
      "      S(Goto(\"l\"))\n"
      "      S(Return(Constant(ConstInt(0) None)))\n"
      "      S(LabeledStmt(\"l\" Return(Var(\"x\" None))))\n"
      "    ])) Compound(Block([\n"
      "      S(Goto(\"l\"))\n"
      "      S(Return(Constant(ConstInt(0) None)))\n"
      "      S(LabeledStmt(\"l\" Return(Var(\"x\" None))))\n"
      "    ]))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int x = 0;\\n    if (x != 0) {\\n        return_y:\\n        return y;\\n    }\\n    int y = 4;\\n    goto return_y;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(IfStmt(Binary(NotEqual Var(\"x\" None) Constant(ConstInt(0) None) None) Compound(Block([\n"
      "      S(LabeledStmt(\"return_y\" Return(Var(\"y\" None))))\n"
      "    ])) None))\n"
      "    D(VarDecl(var_decl(\"y\" Constant(ConstInt(4) None) Int None None None None false)))\n"
      "    S(Goto(\"return_y\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    {\\n        int a = 2;\\n    }\\n    return a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_chapter_7.py::TestChapter7InvalidSemantics::test_resolve_fails\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int a;\\n    {\\n        b = 10;\\n    }\\n    int b;\\n    return b;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Expression(Assignment(Var(\"b\" None) Constant(ConstInt(10) None) None)))\n"
      "    ])))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    S(Return(Var(\"b\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
