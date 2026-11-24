// The calls that the tests of tests/test_enum.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_enum.py::TestParse::test_array_size (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_array_size\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { N = 3 }; int a[N * 2];\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 6) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_attribute_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_attribute_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum [[c6502::packed]] E { A, B = 255 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its size: `enum e : uint8_t { ... }` is one byte (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_attribute_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_attribute_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E { A }; enum [[c6502::packed]] E x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its size: `enum e : uint8_t { ... }` is one byte (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_attribute_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_attribute_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x [[c6502::packed]];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`[[c6502::packed]]` was removed. C23's fixed underlying type gives an enum its size: `enum e : uint8_t { ... }` is one byte (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_attribute_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_attribute_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum [[c6502::zeropage]] { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zeropage` doesn't apply to an enum\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_attributes_after_list_are_a_syntax_error (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_attributes_after_list_are_a_syntax_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A } [[deprecated]];\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('ATTR_OPEN', '[[') at line 1, column 12.\\nExpected one of: \\n\\t* ALT_BOOL\\n\\t* ALT_THREAD_LOCAL\\n\\t* AUTO\\n\\t* BITINT\\n\\t* BOOL\\n\\t* CHAR\\n\\t* CONST\\n\\t* CONSTEXPR\\n\\t* DOUBLE\\n\\t* ENUM\\n\\t* EXTERN\\n\\t* FLOAT\\n\\t* IDENTIFIER\\n\\t* INT\\n\\t* LONG\\n\\t* LPAREN\\n\\t* REGISTER\\n\\t* RESTRICT\\n\\t* SEMICOLON\\n\\t* SHORT\\n\\t* SIGNED\\n\\t* STAR\\n\\t* STATIC\\n\\t* STRUCT\\n\\t* THREAD_LOCAL\\n\\t* TYPEDEF\\n\\t* UNION\\n\\t* UNSIGNED\\n\\t* VOID\\n\\t* VOLATILE\\n\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_constant_hides_typedef_name (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_constant_hides_typedef_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int main(void) { enum { T = 3 }; return T * 2; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Multiply Constant(ConstInt(3) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_definition_alone_declares_nothing_in_the_ast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_definition_alone_declares_nothing_in_the_ast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum e { A, B, };\"\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E { A }; enum E { B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' is defined twice\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' isn't defined. Only an enum with a fixed underlying type can be declared before its constants, as in `enum E : int;` (C23 \\u00a76.7.3.3)\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A }; int A;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'A' is declared again in the same scope, and one of the declarations is an enumeration constant\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'A' is declared again in the same scope, and one of the declarations is an enumeration constant\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; enum { T };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'T' is declared again in the same scope, and one of the declarations is an enumeration constant\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A = 32768 };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'A' is 32768, which doesn't fit in an int (-32768..32767)\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A = 32767, B };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enumeration constant 'B' is 32768, which doesn't fit in an int (-32768..32767)\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x; enum { A = x };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"the value of enumeration constant 'A' must be an integer constant expression: undeclared identifier 'x'\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { int a; }; enum S { A };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'S' is a struct or union tag in this scope, so it can't be an enum tag too\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum S { A }; struct S *p;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'S' is an enum tag, not a struct tag\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum S { A }; struct S { int x; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'S' is an enum tag in this scope, so it can't be a struct tag too\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E { A }; unsigned enum E x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"enum 'E' cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A }; int f(void) { return A(); }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"called object is not a function\"}\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_inner_tag_hides_outer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_inner_tag_hides_outer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum S { A }; void f(void) { struct S { int x; } s; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int)])))\n"
      "    D(VarDecl(var_decl(\"s\" None Structure(\"S\") None None None None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_inner_tag_hides_outer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_inner_tag_hides_outer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum E { A }; void f(void) { enum E { B }; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n            enum { A = 1 };\\n            int main(void) {\\n                { int A = 5; A; }\\n                { enum { A = 2 }; A; }\\n                return A;\\n            }\\n        \"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"A\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "      S(Expression(Var(\"A\" None)))\n"
      "    ])))\n"
      "    S(Compound(Block([\n"
      "      S(Expression(Constant(ConstInt(2) None)))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_type_is_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_type_is_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum e { A }; enum e x; typedef enum { B } t; t y;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"y\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_values (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };int main(void) { return A; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_values (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };int main(void) { return B; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_values (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };int main(void) { return C; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(10) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_values (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };int main(void) { return D; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(11) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_values (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };int main(void) { return E; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(19) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_values (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };int main(void) { return F; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(65534) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_enum.py::TestParse::test_values (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_enum.py::TestParse::test_values\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A, B, C = 10, D, E = C * 2 - 1, F = -2, G };int main(void) { return G; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(65535) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
