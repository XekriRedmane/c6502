// The calls that the tests of tests/test_empty_initializer.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_empty_initializer.py::TestParse::test_braced_scalar_compound_literal_keeps_its_expression (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestParse::test_braced_scalar_compound_literal_keeps_its_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return (int){5,}; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int Constant(ConstInt(5) None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestParse::test_comma_alone_is_a_syntax_error (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestParse::test_comma_alone_is_a_syntax_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = {,};\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('COMMA', ',') at line 1, column 10.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RBRACE\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestParse::test_comma_alone_is_a_syntax_error (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestParse::test_comma_alone_is_a_syntax_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return (int){,}; }\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('COMMA', ',') at line 1, column 28.\\nExpected one of: \\n\\t* AMP\\n\\t* BANG\\n\\t* BITINT_INTEGER\\n\\t* CHAR_CONSTANT\\n\\t* DOT\\n\\t* DOUBLE_CONSTANT\\n\\t* FALSE\\n\\t* FLOAT_CONSTANT\\n\\t* IDENTIFIER\\n\\t* INTEGER_CONSTANT\\n\\t* LBRACE\\n\\t* LBRACKET\\n\\t* LONG_DOUBLE_CONSTANT\\n\\t* LONG_INTEGER\\n\\t* LPAREN\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* NULLPTR\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* RBRACE\\n\\t* SIZEOF\\n\\t* STAR\\n\\t* STRING_LITERAL\\n\\t* TILDE\\n\\t* TRUE\\n\\t* UBITINT_INTEGER\\n\\t* UINT_INTEGER\\n\\t* ULONG_INTEGER\\n\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestParse::test_compound_literals (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestParse::test_compound_literals\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a; };int f(void) { return (int){} + (int[4]){}[0] + (struct s){}.a; }\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Binary(Add Binary(Add CompoundLiteral(\"\" Int InitList([] None None) None None false) Subscript(CompoundLiteral(\"\" Array(Int 4) InitList([] None None) None None false) Constant(ConstInt(0) None) None) None) Dot(CompoundLiteral(\"\" Structure(\"s\") InitList([] None None) None None false) \"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestParse::test_empty_initializer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestParse::test_empty_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = {};\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestParse::test_nested_empty_initializers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestParse::test_nested_empty_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3][2] = { {}, {1, 2}, };\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Array(Array(Int 2) 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_aggregates (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_aggregates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a; char b[3]; long c; };\\nunion u { char c; long l; };\\nstruct s v = {}; union u un = {}; int a[4] = {};struct s w = { {}, {1}, {} };int k[3][2] = { {}, {1, 2} };char str[3] = {};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"un\" InitList([] None None) Union(\"u\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None)] None None) InitList([] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Array(Array(Int 2) 3) None None None None false))\n"
      "  VarDecl(var_decl(\"str\" InitList([] None None) Array(Char 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_aggregates (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_aggregates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"un\" InitList([] None None) Union(\"u\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None)] None None) InitList([] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Array(Array(Int 2) 3) None None None None false))\n"
      "  VarDecl(var_decl(\"str\" InitList([] None None) Array(Char 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"un\" InitList([] None None) Union(\"u\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None)] None None) InitList([] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Array(Array(Int 2) 3) None None None None false))\n"
      "  VarDecl(var_decl(\"str\" InitList([] None None) Array(Char 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_aggregates (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_aggregates\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n  VarDecl(var_decl(\\\"un\\\" InitList([] Union(\\\"u\\\") None) Union(\\\"u\\\") None None None None false))\\n  VarDecl(var_decl(\\\"a\\\" InitList([] Array(Int 4) None) Array(Int 4) None None None None false))\\n  VarDecl(var_decl(\\\"w\\\" InitList([Constant(ConstInt(0) Int) InitList([Cast(Char Constant(ConstInt(1) Int) Char)] Array(Char 3) None) Cast(Long Constant(ConstInt(0) Int) Long)] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n  VarDecl(var_decl(\\\"k\\\" InitList([InitList([] Array(Int 2) None) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None)] Array(Array(Int 2) 3) None) Array(Array(Int 2) 3) None None None None false))\\n  VarDecl(var_decl(\\\"str\\\" InitList([] Array(Char 3) None) Array(Char 3) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"un\" InitList([] None None) Union(\"u\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None)] None None) InitList([] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"k\" InitList([InitList([] None None) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Array(Array(Int 2) 3) None None None None false))\n"
      "  VarDecl(var_decl(\"str\" InitList([] None None) Array(Char 3) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  VarDecl(var_decl(\\\"v\\\" InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n  VarDecl(var_decl(\\\"un\\\" InitList([] Union(\\\"u\\\") None) Union(\\\"u\\\") None None None None false))\\n  VarDecl(var_decl(\\\"a\\\" InitList([] Array(Int 4) None) Array(Int 4) None None None None false))\\n  VarDecl(var_decl(\\\"w\\\" InitList([Constant(ConstInt(0) Int) InitList([Cast(Char Constant(ConstInt(1) Int) Char)] Array(Char 3) None) Cast(Long Constant(ConstInt(0) Int) Long)] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n  VarDecl(var_decl(\\\"k\\\" InitList([InitList([] Array(Int 2) None) InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None)] Array(Array(Int 2) 3) None) Array(Array(Int 2) 3) None None None None false))\\n  VarDecl(var_decl(\\\"str\\\" InitList([] Array(Char 3) None) Array(Char 3) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, {\"tuple\": [0, 0, 0]}, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"un\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 0, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, {\"tuple\": [1, 0, 0]}, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [0, 0]}, {\"tuple\": [1, 2]}, {\"tuple\": [0, 0]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"str\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\":"
      " \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"byte_offset\": 2}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 5}}], \"size\": 9, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_automatic_scalars_get_a_zero_of_their_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_automatic_scalars_get_a_zero_of_their_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { float f = {}; double d = {}; int *p = {}; char c = {}; return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"f\" InitList([] None None) Float None None None None false)))\n"
      "    D(VarDecl(var_decl(\"d\" InitList([] None None) Double None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" InitList([] None None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"c\" InitList([] None None) Char None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_automatic_scalars_get_a_zero_of_their_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_automatic_scalars_get_a_zero_of_their_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"f\" InitList([] None None) Float None None None None false)))\n"
      "    D(VarDecl(var_decl(\"d\" InitList([] None None) Double None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" InitList([] None None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"c\" InitList([] None None) Char None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.f\" InitList([] None None) Float None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.d\" InitList([] None None) Double None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" InitList([] None None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.c\" InitList([] None None) Char None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_automatic_scalars_get_a_zero_of_their_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_automatic_scalars_get_a_zero_of_their_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.f\\\" Constant(ConstFloat(0) Float) Float None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.d\\\" Constant(ConstDouble(0) Double) Double None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.p\\\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.c\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.f\" InitList([] None None) Float None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.d\" InitList([] None None) Double None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" InitList([] None None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.c\" InitList([] None None) Char None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.f\\\" Constant(ConstFloat(0) Float) Float None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.d\\\" Constant(ConstDouble(0) Double) Double None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.p\\\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.c\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_block_scope_statics (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_block_scope_statics\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a; char b[3]; long c; };\\nunion u { char c; long l; };\\nint f(void) { static struct s v = {}; static int i = {}; static int j = {3}; static union u un = {}; static double d = {}; return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"s\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"i\" InitList([] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"j\" InitList([Constant(ConstInt(3) None)] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"un\" InitList([] None None) Union(\"u\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"d\" InitList([] None None) Double Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_block_scope_statics (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_block_scope_statics\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"s\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"i\" InitList([] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"j\" InitList([Constant(ConstInt(3) None)] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"un\" InitList([] None None) Union(\"u\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"d\" InitList([] None None) Double Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([] None None) Structure(\"s\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" InitList([] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.j\" InitList([Constant(ConstInt(3) None)] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.un\" InitList([] None None) Union(\"u\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@4.d\" InitList([] None None) Double Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_block_scope_statics (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_block_scope_statics\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.v\\\" InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.j\\\" Constant(ConstInt(3) Int) Int Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.un\\\" InitList([] Union(\\\"u\\\") None) Union(\\\"u\\\") Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@4.d\\\" Constant(ConstDouble(0) Double) Double Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.v\" InitList([] None None) Structure(\"s\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" InitList([] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.j\" InitList([Constant(ConstInt(3) None)] None None) Int Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.un\" InitList([] None None) Union(\"u\") Static None None None false)))\n"
      "    D(VarDecl(var_decl(\"@4.d\" InitList([] None None) Double Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.v\\\" InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.i\\\" Constant(ConstInt(0) Int) Int Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.j\\\" Constant(ConstInt(3) Int) Int Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.un\\\" InitList([] Union(\\\"u\\\") None) Union(\\\"u\\\") Static None None None false)))\\n    D(VarDecl(var_decl(\\\"@4.d\\\" Constant(ConstDouble(0) Double) Double Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, {\"tuple\": [0, 0, 0]}, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@2.j\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@3.un\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@4.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.T"
      "ypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"byte_offset\": 2}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 5}}], \"size\": 9, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_at_block_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_at_block_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { int x = {5}; for (int i = {2,}; i;) break; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" InitList([Constant(ConstInt(2) None)] None None) Int None None None None false)]) Var(\"i\" None) None BreakStmt(\"\") \"\" None))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_at_block_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_at_block_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" InitList([Constant(ConstInt(2) None)] None None) Int None None None None false)]) Var(\"i\" None) None BreakStmt(\"\") \"\" None))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"@1.i\" InitList([Constant(ConstInt(2) None)] None None) Int None None None None false)]) Var(\"@1.i\" None) None BreakStmt(\"\") \"\" None))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_at_block_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_at_block_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Constant(ConstInt(2) Int) Int None None None None false)]) Var(\\\"@1.i\\\" Int) None BreakStmt(\\\"\\\") \\\"\\\" None))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"@1.i\" InitList([Constant(ConstInt(2) None)] None None) Int None None None None false)]) Var(\"@1.i\" None) None BreakStmt(\"\") \"\" None))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Constant(ConstInt(2) Int) Int None None None None false)]) Var(\\\"@1.i\\\" Int) None BreakStmt(\\\"\\\") \\\"\\\" None))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = {1, 2};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for scalar variable 'x': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { int x = {1, 2,}; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for scalar variable 'x': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { static int x = {1, 2}; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for scalar variable 'x': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { for (int i = {1, 2};;) return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for scalar variable 'i': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = {{5}};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([Constant(ConstInt(5) None)] None None)] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([Constant(ConstInt(5) None)] None None)] None None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([Constant(ConstInt(5) None)] None None)] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([Constant(ConstInt(5) None)] None None)] None None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unexpected nested initializer for scalar variable 'x': a scalar's initializer is one expression, optionally in one pair of braces, or the empty initializer `{}` (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = {{}};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([] None None)] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([] None None)] None None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([] None None)] None None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([InitList([] None None)] None None) Int None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unexpected nested initializer for scalar variable 'x': a scalar's initializer is one expression, optionally in one pair of braces, or the empty initializer `{}` (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct t { int a; }; struct t v = { {1, 2} };\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Structure(\"t\") None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Structure(\"t\") None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Structure(\"t\") None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Int)]))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)] None None) Structure(\"t\") None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for member 'a' of 'v': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[2] = { {{1}} };\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([InitList([Constant(ConstInt(1) None)] None None)] None None)] None None) Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([InitList([Constant(ConstInt(1) None)] None None)] None None)] None None) Array(Int 2) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([InitList([Constant(ConstInt(1) None)] None None)] None None)] None None) Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([InitList([Constant(ConstInt(1) None)] None None)] None None)] None None) Array(Int 2) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unexpected nested initializer for element 0 of 'a': a scalar's initializer is one expression, optionally in one pair of braces, or the empty initializer `{}` (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 25)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return (int){1, 2}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 26)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 27)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for scalar variable '(compound literal)': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 28)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return (int){{1}}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([InitList([Constant(ConstInt(1) None)] None None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 29)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([InitList([Constant(ConstInt(1) None)] None None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([InitList([Constant(ConstInt(1) None)] None None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors (call 30)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalar_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([InitList([Constant(ConstInt(1) None)] None None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"unexpected nested initializer for scalar variable '(compound literal)': a scalar's initializer is one expression, optionally in one pair of braces, or the empty initializer `{}` (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_braced_scalars (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalars\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a; char b[3]; long c; };\\nunion u { char c; long l; };\\nint x = {5}; long y = {-7,}; double d = {1.5};int *p = {0}; struct s v = { {3}, {}, {4} };int a[3] = { {1}, {}, 3 };\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"y\" InitList([Unary(Negate Constant(ConstInt(7) None) None)] None None) Long None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([Constant(ConstDouble(4609434218613702656) None)] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([Constant(ConstInt(0) None)] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(3) None)] None None) InitList([] None None) InitList([Constant(ConstInt(4) None)] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([] None None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_braced_scalars (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalars\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"y\" InitList([Unary(Negate Constant(ConstInt(7) None) None)] None None) Long None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([Constant(ConstDouble(4609434218613702656) None)] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([Constant(ConstInt(0) None)] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(3) None)] None None) InitList([] None None) InitList([Constant(ConstInt(4) None)] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([] None None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"y\" InitList([Unary(Negate Constant(ConstInt(7) None) None)] None None) Long None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([Constant(ConstDouble(4609434218613702656) None)] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([Constant(ConstInt(0) None)] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(3) None)] None None) InitList([] None None) InitList([Constant(ConstInt(4) None)] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([] None None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_braced_scalars (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_braced_scalars\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"y\\\" Cast(Long Unary(Negate Constant(ConstInt(7) Int) Int) Long) Long None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" Constant(ConstDouble(4609434218613702656) Double) Double None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstInt(3) Int) InitList([] Array(Char 3) None) Cast(Long Constant(ConstInt(4) Int) Long)] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(5) None)] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"y\" InitList([Unary(Negate Constant(ConstInt(7) None) None)] None None) Long None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([Constant(ConstDouble(4609434218613702656) None)] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([Constant(ConstInt(0) None)] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"v\" InitList([InitList([Constant(ConstInt(3) None)] None None) InitList([] None None) InitList([Constant(ConstInt(4) None)] None None)] None None) Structure(\"s\") None None None None false))\n"
      "  VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None)] None None) InitList([] None None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"y\\\" Cast(Long Unary(Negate Constant(ConstInt(7) Int) Int) Long) Long None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" Constant(ConstDouble(4609434218613702656) Double) Double None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"v\\\" InitList([Constant(ConstInt(3) Int) InitList([] Array(Char 3) None) Cast(Long Constant(ConstInt(4) Int) Long)] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None None None false))\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(0) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": -7}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4609434218613702656}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [3, {\"tuple\": [0, 0, 0]}, 4]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 0, 3]}}}, \"is_global\": true, \"z"
      "eropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"byte_offset\": 2}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 5}}], \"size\": 9, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_compound_literals (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_compound_literals\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a; char b[3]; long c; };\\nunion u { char c; long l; };\\nstruct s *ps = &(struct s){}; int *pa = (int[4]){};int f(void) { struct s *q = &(struct s){}; int *r = (int[3]){}; return (int){} + (long){7}; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"ps\" AddressOf(CompoundLiteral(\"\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"pa\" CompoundLiteral(\"\" Array(Int 4) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(CompoundLiteral(\"\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"r\" CompoundLiteral(\"\" Array(Int 3) InitList([] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Add CompoundLiteral(\"\" Int InitList([] None None) None None false) CompoundLiteral(\"\" Long Constant(ConstInt(7) None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_compound_literals (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_compound_literals\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"ps\" AddressOf(CompoundLiteral(\"\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"pa\" CompoundLiteral(\"\" Array(Int 4) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(CompoundLiteral(\"\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"r\" CompoundLiteral(\"\" Array(Int 3) InitList([] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Add CompoundLiteral(\"\" Int InitList([] None None) None None false) CompoundLiteral(\"\" Long Constant(ConstInt(7) None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"ps\" AddressOf(CompoundLiteral(\"@0.compound_literal\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"pa\" CompoundLiteral(\"@1.compound_literal\" Array(Int 4) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(CompoundLiteral(\"@3.compound_literal\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@4.r\" CompoundLiteral(\"@5.compound_literal\" Array(Int 3) InitList([] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@6.compound_literal\" Int InitList([] None None) None None false) CompoundLiteral(\"@7.compound_literal\" Long Constant(ConstInt(7) None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_compound_literals (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_compound_literals\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  VarDecl(var_decl(\\\"ps\\\" AddressOf(CompoundLiteral(\\\"@0.compound_literal\\\" Structure(\\\"s\\\") InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None false) Pointer(Structure(\\\"s\\\"))) Pointer(Structure(\\\"s\\\")) None None None None false))\\n  VarDecl(var_decl(\\\"pa\\\" AddressOf(CompoundLiteral(\\\"@1.compound_literal\\\" Array(Int 4) InitList([] Array(Int 4) None) Array(Int 4) None false) Pointer(Int)) Pointer(Int) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(CompoundLiteral(\\\"@3.compound_literal\\\" Structure(\\\"s\\\") InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None false) Pointer(Structure(\\\"s\\\"))) Pointer(Structure(\\\"s\\\")) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@4.r\\\" AddressOf(CompoundLiteral(\\\"@5.compound_literal\\\" Array(Int 3) InitList([] Array(Int 3) None) Array(Int 3) None false) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long CompoundLiteral(\\\"@6.compound_literal\\\" Int Constant(ConstInt(0) Int) Int None false) Long) CompoundLiteral(\\\"@7.compound_literal\\\" Long Cast(Long Constant(ConstInt(7) Int) Long) Long None false) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  VarDecl(var_decl(\"ps\" AddressOf(CompoundLiteral(\"@0.compound_literal\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"pa\" CompoundLiteral(\"@1.compound_literal\" Array(Int 4) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(CompoundLiteral(\"@3.compound_literal\" Structure(\"s\") InitList([] None None) None None false) None) Pointer(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@4.r\" CompoundLiteral(\"@5.compound_literal\" Array(Int 3) InitList([] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Add CompoundLiteral(\"@6.compound_literal\" Int InitList([] None None) None None false) CompoundLiteral(\"@7.compound_literal\" Long Constant(ConstInt(7) None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  VarDecl(var_decl(\\\"ps\\\" AddressOf(CompoundLiteral(\\\"@0.compound_literal\\\" Structure(\\\"s\\\") InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None false) Pointer(Structure(\\\"s\\\"))) Pointer(Structure(\\\"s\\\")) None None None None false))\\n  VarDecl(var_decl(\\\"pa\\\" AddressOf(CompoundLiteral(\\\"@1.compound_literal\\\" Array(Int 4) InitList([] Array(Int 4) None) Array(Int 4) None false) Pointer(Int)) Pointer(Int) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(CompoundLiteral(\\\"@3.compound_literal\\\" Structure(\\\"s\\\") InitList([] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None false) Pointer(Structure(\\\"s\\\"))) Pointer(Structure(\\\"s\\\")) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@4.r\\\" AddressOf(CompoundLiteral(\\\"@5.compound_literal\\\" Array(Int 3) InitList([] Array(Int 3) None) Array(Int 3) None false) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long CompoundLiteral(\\\"@6.compound_literal\\\" Int Constant(ConstInt(0) Int) Int None false) Long) CompoundLiteral(\\\"@7.compound_literal\\\" Long Cast(Long Constant(ConstInt(7) Int) Long) Long None false) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, {\"tuple\": [0, 0, 0]}, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"ps\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"@0.compound_literal\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"pa\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"@1.compound_literal\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"objec"
      "t\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@5.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"byte_offset\": 2}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 5}}], \"size\": 9, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_scalars_are_zero (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_scalars_are_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef typeof_unqual(nullptr) nullptr_t;int i = {}; unsigned long ul = {}; char c = {};signed char sc = {}; long long ll = {}; int *p = {};void (*fp)(void) = {}; float f = {}; double d = {};bool b = {}; _BitInt(12) w = {}; unsigned _BitInt(40) uw = {};nullptr_t n = {}; const volatile int cv = {};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"i\" InitList([] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"ul\" InitList([] None None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([] None None) Char None None None None false))\n"
      "  VarDecl(var_decl(\"sc\" InitList([] None None) SChar None None None None false))\n"
      "  VarDecl(var_decl(\"ll\" InitList([] None None) LongLong None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" InitList([] None None) Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"f\" InitList([] None None) Float None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([] None None) Bool None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([] None None) BitInt(12 true) None None None None false))\n"
      "  VarDecl(var_decl(\"uw\" InitList([] None None) BitInt(40 false) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" InitList([] None None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"cv\" InitList([] None None) Const(Volatile(Int)) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_scalars_are_zero (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_scalars_are_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"i\" InitList([] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"ul\" InitList([] None None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([] None None) Char None None None None false))\n"
      "  VarDecl(var_decl(\"sc\" InitList([] None None) SChar None None None None false))\n"
      "  VarDecl(var_decl(\"ll\" InitList([] None None) LongLong None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" InitList([] None None) Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"f\" InitList([] None None) Float None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([] None None) Bool None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([] None None) BitInt(12 true) None None None None false))\n"
      "  VarDecl(var_decl(\"uw\" InitList([] None None) BitInt(40 false) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" InitList([] None None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"cv\" InitList([] None None) Const(Volatile(Int)) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"i\" InitList([] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"ul\" InitList([] None None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([] None None) Char None None None None false))\n"
      "  VarDecl(var_decl(\"sc\" InitList([] None None) SChar None None None None false))\n"
      "  VarDecl(var_decl(\"ll\" InitList([] None None) LongLong None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" InitList([] None None) Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"f\" InitList([] None None) Float None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([] None None) Bool None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([] None None) BitInt(12 true) None None None None false))\n"
      "  VarDecl(var_decl(\"uw\" InitList([] None None) BitInt(40 false) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" InitList([] None None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"cv\" InitList([] None None) Const(Volatile(Int)) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_scalars_are_zero (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_scalars_are_zero\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"i\\\" Constant(ConstInt(0) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"ul\\\" Cast(ULong Constant(ConstInt(0) Int) ULong) ULong None None None None false))\\n  VarDecl(var_decl(\\\"c\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false))\\n  VarDecl(var_decl(\\\"sc\\\" Cast(SChar Constant(ConstInt(0) Int) SChar) SChar None None None None false))\\n  VarDecl(var_decl(\\\"ll\\\" Cast(LongLong Constant(ConstInt(0) Int) LongLong) LongLong None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" Cast(Pointer(FunType([] Void)) Constant(ConstInt(0) Int) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"f\\\" Constant(ConstFloat(0) Float) Float None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" Constant(ConstDouble(0) Double) Double None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(0) Int) Bool) Bool None None None None false))\\n  VarDecl(var_decl(\\\"w\\\" Cast(BitInt(12 true) Constant(ConstInt(0) Int) BitInt(12 true)) BitInt(12 true) None None None None false))\\n  VarDecl(var_decl(\\\"uw\\\" Cast(BitInt(40 false) Constant(ConstInt(0) Int) BitInt(40 false)) BitInt(40 false) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\\n  VarDecl(var_decl(\\\"cv\\\" Constant(ConstInt(0) Int) Const(Volatile(Int)) None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"i\" InitList([] None None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"ul\" InitList([] None None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"c\" InitList([] None None) Char None None None None false))\n"
      "  VarDecl(var_decl(\"sc\" InitList([] None None) SChar None None None None false))\n"
      "  VarDecl(var_decl(\"ll\" InitList([] None None) LongLong None None None None false))\n"
      "  VarDecl(var_decl(\"p\" InitList([] None None) Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"fp\" InitList([] None None) Pointer(FunType([] Void)) None None None None false))\n"
      "  VarDecl(var_decl(\"f\" InitList([] None None) Float None None None None false))\n"
      "  VarDecl(var_decl(\"d\" InitList([] None None) Double None None None None false))\n"
      "  VarDecl(var_decl(\"b\" InitList([] None None) Bool None None None None false))\n"
      "  VarDecl(var_decl(\"w\" InitList([] None None) BitInt(12 true) None None None None false))\n"
      "  VarDecl(var_decl(\"uw\" InitList([] None None) BitInt(40 false) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" InitList([] None None) NullptrT None None None None false))\n"
      "  VarDecl(var_decl(\"cv\" InitList([] None None) Const(Volatile(Int)) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"i\\\" Constant(ConstInt(0) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"ul\\\" Cast(ULong Constant(ConstInt(0) Int) ULong) ULong None None None None false))\\n  VarDecl(var_decl(\\\"c\\\" Cast(Char Constant(ConstInt(0) Int) Char) Char None None None None false))\\n  VarDecl(var_decl(\\\"sc\\\" Cast(SChar Constant(ConstInt(0) Int) SChar) SChar None None None None false))\\n  VarDecl(var_decl(\\\"ll\\\" Cast(LongLong Constant(ConstInt(0) Int) LongLong) LongLong None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int) None None None None false))\\n  VarDecl(var_decl(\\\"fp\\\" Cast(Pointer(FunType([] Void)) Constant(ConstInt(0) Int) Pointer(FunType([] Void))) Pointer(FunType([] Void)) None None None None false))\\n  VarDecl(var_decl(\\\"f\\\" Constant(ConstFloat(0) Float) Float None None None None false))\\n  VarDecl(var_decl(\\\"d\\\" Constant(ConstDouble(0) Double) Double None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Constant(ConstInt(0) Int) Bool) Bool None None None None false))\\n  VarDecl(var_decl(\\\"w\\\" Cast(BitInt(12 true) Constant(ConstInt(0) Int) BitInt(12 true)) BitInt(12 true) None None None None false))\\n  VarDecl(var_decl(\\\"uw\\\" Cast(BitInt(40 false) Constant(ConstInt(0) Int) BitInt(40 false)) BitInt(40 false) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(NullptrT Constant(ConstInt(0) Int) NullptrT) NullptrT None None None None false))\\n  VarDecl(var_decl(\\\"cv\\\" Constant(ConstInt(0) Int) Const(Volatile(Int)) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"ul\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"sc\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"ll\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_addr"
      "ess\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([] Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"BitInt(12 true)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"uw\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"BitInt(40 false)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"NullptrT\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"cv\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Volati"
      "le(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[] = {};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"array 'a' has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \\u00a76.7.11p4)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { int a[] = {}; return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([] None None) Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([] None None) Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"array 'a' has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \\u00a76.7.11p4)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { static char a[] = {}; return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([] None None) Array(Char 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([] None None) Array(Char 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([] None None) Array(Char 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([] None None) Array(Char 0) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"array 'a' has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \\u00a76.7.11p4)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { for (int a[] = {};;) return 0; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"a\" InitList([] None None) Array(Int 0) None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.a\" InitList([] None None) Array(Int 0) None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.a\" InitList([] None None) Array(Int 0) None None None None false)]) None None Return(Constant(ConstInt(0) None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"array 'a' has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \\u00a76.7.11p4)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return (int[]){}[0]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Subscript(CompoundLiteral(\"\" Array(Int 0) InitList([] None None) None None false) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Subscript(CompoundLiteral(\"\" Array(Int 0) InitList([] None None) None None false) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Subscript(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([] None None) None None false) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Subscript(CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([] None None) None None false) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the compound literal's array has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \\u00a76.7.11p4)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *p = (int[]){};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestTypeCheck::test_unknown_size_array_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([] None None) None None false) Pointer(Int) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"the compound literal's array has unknown size, so it can't be initialized by the empty initializer `{}`, which has no elements to give it a size (C23 \\u00a76.7.11p4)\"}\n");
}

TEST_CASE("parse: test_empty_initializer.py::TestCode::test_large_automatic_struct_is_a_loop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_empty_initializer.py::TestCode::test_large_automatic_struct_is_a_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a; char b[3]; long c; };\\nunion u { char c; long l; };\\nstruct big { struct s s[4]; union u u; };void use(struct big *p);void f(void) { struct big v = {}; use(&v); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  StructDecl(struct_decl(\"big\" false [member_decl(\"s\" Array(Structure(\"s\") 4)) member_decl(\"u\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"use\" [\"p\"] None FunType([Pointer(Structure(\"big\"))] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"big\") None None None None false)))\n"
      "    S(Expression(FunctionCall(\"use\" [AddressOf(Var(\"v\" None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_empty_initializer.py::TestCode::test_large_automatic_struct_is_a_loop (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_empty_initializer.py::TestCode::test_large_automatic_struct_is_a_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  StructDecl(struct_decl(\"big\" false [member_decl(\"s\" Array(Structure(\"s\") 4)) member_decl(\"u\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"use\" [\"p\"] None FunType([Pointer(Structure(\"big\"))] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" InitList([] None None) Structure(\"big\") None None None None false)))\n"
      "    S(Expression(FunctionCall(\"use\" [AddressOf(Var(\"v\" None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  StructDecl(struct_decl(\"big\" false [member_decl(\"s\" Array(Structure(\"s\") 4)) member_decl(\"u\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"use\" [\"@0.p\"] None FunType([Pointer(Structure(\"big\"))] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([] None None) Structure(\"big\") None None None None false)))\n"
      "    S(Expression(FunctionCall(\"use\" [AddressOf(Var(\"@1.v\" None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_empty_initializer.py::TestCode::test_large_automatic_struct_is_a_loop (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_empty_initializer.py::TestCode::test_large_automatic_struct_is_a_loop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  StructDecl(struct_decl(\\\"big\\\" false [member_decl(\\\"s\\\" Array(Structure(\\\"s\\\") 4)) member_decl(\\\"u\\\" Union(\\\"u\\\"))]))\\n  FunctionDecl(function_decl(\\\"use\\\" [\\\"@0.p\\\"] None FunType([Pointer(Structure(\\\"big\\\"))] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.v\\\" InitList([] Structure(\\\"big\\\") None) Structure(\\\"big\\\") None None None None false)))\\n    S(Expression(FunctionCall(\\\"use\\\" [AddressOf(Var(\\\"@1.v\\\" Structure(\\\"big\\\")) Pointer(Structure(\\\"big\\\")))] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Int) member_decl(\"b\" Array(Char 3)) member_decl(\"c\" Long)]))\n"
      "  StructDecl(struct_decl(\"u\" true [member_decl(\"c\" Char) member_decl(\"l\" Long)]))\n"
      "  StructDecl(struct_decl(\"big\" false [member_decl(\"s\" Array(Structure(\"s\") 4)) member_decl(\"u\" Union(\"u\"))]))\n"
      "  FunctionDecl(function_decl(\"use\" [\"@0.p\"] None FunType([Pointer(Structure(\"big\"))] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@1.v\" InitList([] None None) Structure(\"big\") None None None None false)))\n"
      "    S(Expression(FunctionCall(\"use\" [AddressOf(Var(\"@1.v\" None) None)] None false)))\n"
      "  ]) FunType([] Void) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Int) member_decl(\\\"b\\\" Array(Char 3)) member_decl(\\\"c\\\" Long)]))\\n  StructDecl(struct_decl(\\\"u\\\" true [member_decl(\\\"c\\\" Char) member_decl(\\\"l\\\" Long)]))\\n  StructDecl(struct_decl(\\\"big\\\" false [member_decl(\\\"s\\\" Array(Structure(\\\"s\\\") 4)) member_decl(\\\"u\\\" Union(\\\"u\\\"))]))\\n  FunctionDecl(function_decl(\\\"use\\\" [\\\"@0.p\\\"] None FunType([Pointer(Structure(\\\"big\\\"))] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.v\\\" InitList([] Structure(\\\"big\\\") None) Structure(\\\"big\\\") None None None None false)))\\n    S(Expression(FunctionCall(\\\"use\\\" [AddressOf(Var(\\\"@1.v\\\" Structure(\\\"big\\\")) Pointer(Structure(\\\"big\\\")))] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"use\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Structure(\\\"big\\\"))] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"big\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"byte_offset\": 2}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 5}}], \"size\": 9, \"complete\": true}}], [\"u\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"u\", \"is_union\": true, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"l\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"byte_offset\": 0}}], \"size\": 4, \"complete\": true}}], [\"big\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"big\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"s\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Structure(\\\"s\\\") 4)\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"u\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Union(\\\"u\\\")\\n\"}, \"byte_offset\": 36}}], \"size\": 40, \"complete\": true}}]]}}}]}\n");
}
