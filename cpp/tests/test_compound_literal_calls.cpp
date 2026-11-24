// The calls that the tests of tests/test_compound_literal.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_compound_literal.py::TestParse::test_array_literal (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_array_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return (int[]){1, 2, 3}[1]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(CompoundLiteral(\"\" Array(Int 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_compound_literal.py::TestParse::test_cast_still_parses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_cast_still_parses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return (int)5L; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Int Constant(ConstLong(5) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_compound_literal.py::TestParse::test_char_array_literal_keeps_its_string (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_char_array_literal_keeps_its_string\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return (char[]){\\\"ab\\\"}[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(CompoundLiteral(\"\" Array(Char 0) String(\"ab\") None None false) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_compound_literal.py::TestParse::test_rejects_non_object_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_rejects_non_object_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { (void){1}; return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a compound literal's type must be an object type (C99 \\u00a76.5.2.5.1)\"}\n");
}

TEST_CASE("parse: test_compound_literal.py::TestParse::test_rejects_non_object_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_rejects_non_object_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int F(void); int main(void) { (F){0}; return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a compound literal's type must be an object type (C99 \\u00a76.5.2.5.1)\"}\n");
}

TEST_CASE("parse: test_compound_literal.py::TestParse::test_rejects_scalar_with_two_initializers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_rejects_scalar_with_two_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return (int){1, 2}; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_compound_literal.py::TestParse::test_rejects_scalar_with_two_initializers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_compound_literal.py::TestParse::test_rejects_scalar_with_two_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_compound_literal.py::TestParse::test_rejects_scalar_with_two_initializers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_compound_literal.py::TestParse::test_rejects_scalar_with_two_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"@0.compound_literal\" Int InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for scalar variable '(compound literal)': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_compound_literal.py::TestParse::test_scalar_literal_takes_its_expression (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_scalar_literal_takes_its_expression\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return (long){5}; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(CompoundLiteral(\"\" Long Constant(ConstInt(5) None) None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_compound_literal.py::TestParse::test_trailing_comma_and_nesting (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestParse::test_trailing_comma_and_nesting\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct p { int a[2]; int b; };int main(void) { return (struct p){{1, 2}, 3,}.b; }\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"p\" false [member_decl(\"a\" Array(Int 2)) member_decl(\"b\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Dot(CompoundLiteral(\"\" Structure(\"p\") InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Constant(ConstInt(3) None)] None None) None None false) \"b\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_compound_literal.py::TestTypeCheck::test_rejects_non_constant_at_file_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_non_constant_at_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x; int *p = (int[]){x}; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([Var(\"x\" None)] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_compound_literal.py::TestTypeCheck::test_rejects_non_constant_at_file_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_non_constant_at_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([Var(\"x\" None)] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Var(\"x\" None)] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_compound_literal.py::TestTypeCheck::test_rejects_non_constant_at_file_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_non_constant_at_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" CompoundLiteral(\"@0.compound_literal\" Array(Int 0) InitList([Var(\"x\" None)] None None) None None false) Pointer(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object '(compound literal)' is not a constant expression\"}\n");
}

TEST_CASE("parse: test_compound_literal.py::TestTypeCheck::test_rejects_static_address_of_automatic_literal (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_static_address_of_automatic_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int *p = (int[]){1}; return *p; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([Constant(ConstInt(1) None)] None None) None None false) Pointer(Int) Static None None None false)))\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_compound_literal.py::TestTypeCheck::test_rejects_static_address_of_automatic_literal (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_static_address_of_automatic_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 0) InitList([Constant(ConstInt(1) None)] None None) None None false) Pointer(Int) Static None None None false)))\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None)] None None) None None false) Pointer(Int) Static None None None false)))\n"
      "    S(Return(Dereference(Var(\"@0.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_compound_literal.py::TestTypeCheck::test_rejects_static_address_of_automatic_literal (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_static_address_of_automatic_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" CompoundLiteral(\"@1.compound_literal\" Array(Int 0) InitList([Constant(ConstInt(1) None)] None None) None None false) Pointer(Int) Static None None None false)))\n"
      "    S(Return(Dereference(Var(\"@0.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object '@0.p' takes the address of a compound literal inside a function, which doesn't have static storage duration\"}\n");
}

TEST_CASE("parse: test_compound_literal.py::TestTypeCheck::test_rejects_too_many_initializers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_too_many_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int *p = (int[2]){1, 2, 3}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 2) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_compound_literal.py::TestTypeCheck::test_rejects_too_many_initializers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_too_many_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" CompoundLiteral(\"\" Array(Int 2) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" CompoundLiteral(\"@1.compound_literal\" Array(Int 2) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_compound_literal.py::TestTypeCheck::test_rejects_too_many_initializers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_rejects_too_many_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" CompoundLiteral(\"@1.compound_literal\" Array(Int 2) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for array '(compound literal)': 3 given, array has 2 elements\"}\n");
}

TEST_CASE("parse: test_compound_literal.py::TestTypeCheck::test_unknown_size_comes_from_initializer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_unknown_size_comes_from_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return sizeof((long[]){1, 2, 3}); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(SizeOfExp(CompoundLiteral(\"\" Array(Long 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_compound_literal.py::TestTypeCheck::test_unknown_size_comes_from_initializer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_unknown_size_comes_from_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(SizeOfExp(CompoundLiteral(\"\" Array(Long 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(SizeOfExp(CompoundLiteral(\"@0.compound_literal\" Array(Long 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_compound_literal.py::TestTypeCheck::test_unknown_size_comes_from_initializer (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_compound_literal.py::TestTypeCheck::test_unknown_size_comes_from_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int SizeOfExp(CompoundLiteral(\\\"@0.compound_literal\\\" Array(Long 3) InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long) Cast(Long Constant(ConstInt(3) Int) Long)] Array(Long 3) None) Array(Long 3) None false) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(SizeOfExp(CompoundLiteral(\"@0.compound_literal\" Array(Long 0) InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) None None false) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int SizeOfExp(CompoundLiteral(\\\"@0.compound_literal\\\" Array(Long 3) InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long) Cast(Long Constant(ConstInt(3) Int) Long)] Array(Long 3) None) Array(Long 3) None false) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.compound_literal\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}
