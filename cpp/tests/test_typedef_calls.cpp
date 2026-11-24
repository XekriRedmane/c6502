// The calls that the tests of tests/test_typedef.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_anonymous_structs_are_distinct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_anonymous_structs_are_distinct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef struct { int x; } A; typedef struct { int x; } B;A a; B b; union { char c; int i; } u;\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"__anonymous_struct_1\" false [member_decl(\"x\" Int)]))\n"
      "  StructDecl(struct_decl(\"__anonymous_struct_2\" false [member_decl(\"x\" Int)]))\n"
      "  VarDecl(var_decl(\"a\" None Structure(\"__anonymous_struct_1\") None None None None false))\n"
      "  VarDecl(var_decl(\"b\" None Structure(\"__anonymous_struct_2\") None None None None false))\n"
      "  StructDecl(struct_decl(\"__anonymous_union_3\" true [member_decl(\"c\" Char) member_decl(\"i\" Int)]))\n"
      "  VarDecl(var_decl(\"u\" None Union(\"__anonymous_union_3\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_cast_and_sizeof (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_cast_and_sizeof\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef long L; int main(void) { return (L)1 + sizeof(L); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Binary(Add Cast(Long Constant(ConstInt(1) None) None) SizeOfType(Long None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_function_typedef_declares_function (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_function_typedef_declares_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int F(int, long); F f;\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\" \"\"] None FunType([Int Long] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_parameter_of_array_type_is_adjusted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_parameter_of_array_type_is_adjusted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int A[3]; int sum(A a, A);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"sum\" [\"a\" \"\"] None FunType([Pointer(Int) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_parameter_of_function_type_is_adjusted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_parameter_of_function_type_is_adjusted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int F(int); int apply(F f, F);\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"apply\" [\"f\" \"\"] None FunType([Pointer(FunType([Int] Int)) Pointer(FunType([Int] Int))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_pointer_declarator_on_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_pointer_declarator_on_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef long L; L *p, **q;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Long) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Pointer(Pointer(Long)) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_qualified_array_typedef_qualifies_elements (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_qualified_array_typedef_qualifies_elements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int A[3]; const A x = {1, 2, 3}; const int y[3];\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Const(Int) 3) None None None None false))\n"
      "  VarDecl(var_decl(\"y\" None Array(Const(Int) 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_qualifier_already_in_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_qualifier_already_in_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef const int CI; const CI c = 1;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(1) None) Const(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_qualifier_on_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_qualifier_on_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; const T c = 1; T const d = 2;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"c\" Constant(ConstInt(1) None) Const(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"d\" Constant(ConstInt(2) None) Const(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_several_declarators (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_several_declarators\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T, *PT, AT[2]; T a; PT p; AT q;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" None Array(Int 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_struct_member_of_typedef_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_struct_member_of_typedef_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char u8; struct s { u8 a; u8 *b; };\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Pointer(UChar))]))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_struct_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_struct_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef struct node node_t; node_t *p;\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"node\" false []))\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Structure(\"node\")) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_struct_typedef_with_body (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_struct_typedef_with_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef struct pt { int x; } pt_t; pt_t p;\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"pt\" false [member_decl(\"x\" Int)]))\n"
      "  VarDecl(var_decl(\"p\" None Structure(\"pt\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_typedef_leaves_no_declaration (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_typedef_leaves_no_declaration\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; T x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_typedef_name_in_its_own_declaration (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_typedef_name_in_its_own_declaration\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T, *PT[sizeof(T)]; PT x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Array(Pointer(Int) 2) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefDeclarations::test_typedef_of_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefDeclarations::test_typedef_of_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef unsigned char u8; typedef u8 byte; byte b;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" None UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_block_scope_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_block_scope_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { typedef unsigned char B; B b = 1; { typedef long B; B c = 2; } return b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) UChar None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"c\" Constant(ConstInt(2) None) Long None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"b\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_for_initializer_of_typedef_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_for_initializer_of_typedef_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef long I; int main(void) { for (I i = 0; i < 2; i++) ; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Long None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(2) None) None) Postfix(Increment Var(\"i\" None) None) Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_hiding_ends_with_block (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_hiding_ends_with_block\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int main(void) { { long T = 1; T = T * 2; } T y = 0; return y; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"T\" Constant(ConstInt(1) None) Long None None None None false)))\n"
      "      S(Expression(Assignment(Var(\"T\" None) Binary(Multiply Var(\"T\" None) Constant(ConstInt(2) None) None) None)))\n"
      "    ])))\n"
      "    D(VarDecl(var_decl(\"y\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(Return(Var(\"y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_member_named_after_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_member_named_after_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; struct s { T T; }; int main(void) { struct s v; v.T = 1; return v.T; }\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"T\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" None Structure(\"s\") None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"v\" None) \"T\" None) Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Dot(Var(\"v\" None) \"T\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_nested_function_suffix_parameters (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_nested_function_suffix_parameters\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int a; typedef int b; int (*f(int a))(int b) { b x = a; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\"] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Var(\"a\" None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Pointer(FunType([Int] Int))) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_object_named_after_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_object_named_after_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int main(void) { T T = 4; return T; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"T\" Constant(ConstInt(4) None) Int None None None None false)))\n"
      "    S(Return(Var(\"T\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_parameter_hides_only_its_own_function (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_parameter_hides_only_its_own_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int f(int T) { return T; } int g(void) { T x = 1; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"T\"] Block([\n"
      "    S(Return(Var(\"T\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_parameter_hides_typedef_in_body (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_parameter_hides_typedef_in_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int f(int T) { return T * 2; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"T\"] Block([\n"
      "    S(Return(Binary(Multiply Var(\"T\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_star_declares_when_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_star_declares_when_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int main(void) { T * x; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_star_multiplies_when_hidden (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_star_multiplies_when_hidden\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int main(void) { int T = 2, x = 3; T * x; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"T\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Expression(Binary(Multiply Var(\"T\" None) Var(\"x\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_tag_named_after_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_tag_named_after_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; struct T { T a; }; struct T t;\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"T\" false [member_decl(\"a\" Int)]))\n"
      "  VarDecl(var_decl(\"t\" None Structure(\"T\") None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_typedef_name_after_for_that_hides_it (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_typedef_name_after_for_that_hides_it\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int main(void) { int s = 0; for (int T = 0; T < 3; T++) s += T; T y = s; return y; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"T\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"T\" None) Constant(ConstInt(3) None) None) Postfix(Increment Var(\"T\" None) None) Expression(CompoundAssignment(Add Var(\"s\" None) Var(\"T\" None) None None)) \"\" None))\n"
      "    D(VarDecl(var_decl(\"y\" Var(\"s\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefScopes::test_typedef_redeclared_with_same_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefScopes::test_typedef_redeclared_with_same_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; typedef int T; T x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_attribute_on_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_attribute_on_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"[[c6502::zp_abi]] typedef int F(void);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"`c6502::zp_abi` doesn't apply to typedef 'F'\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_conflicting_redefinition (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_conflicting_redefinition\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; typedef long T;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"typedef 'T' is declared again with a different type\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_function_definition_declared_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_function_definition_declared_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int f(void) { return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"storage class 'typedef' isn't permitted on function definition 'f'\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_function_definition_typed_by_typedef (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_function_definition_typed_by_typedef\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int F(void); F f { return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"function definition 'f' has to list its parameters in its declarator; its type can't come from a typedef (C99 \\u00a76.9.1.2)\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_initializer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T = 3;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"typedef 'T' can't have an initializer\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_inner_tag_hides_typedef_tag (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_inner_tag_hides_typedef_tag\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct foo { int a; }; typedef struct foo T; int main(void) { struct foo { long z; }; T t; return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"typedef 'T' refers to the struct or union 'foo' declared in an outer scope, which a declaration of 'foo' in an inner scope hides here; c6502 can't express that type here, so give the inner struct or union another tag\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_parameter_and_typedef_in_body (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_parameter_and_typedef_in_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int a) { typedef int a; return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'a' is declared both as a typedef name and as an object or function in the same scope\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_and_object_in_one_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_and_object_in_one_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int T;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'T' is declared both as a typedef name and as an object or function in the same scope\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_and_object_in_one_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_and_object_in_one_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int T; typedef int T;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'T' is declared both as a typedef name and as an object or function in the same scope\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_and_object_in_one_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_and_object_in_one_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; int T(void) { return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'T' is declared both as a typedef name and as an object or function in the same scope\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_combined_with_type_specifier (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_combined_with_type_specifier\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef int T; T unsigned x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"typedef name 'T' cannot combine with another type specifier\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_in_for_initializer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_in_for_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (typedef int T;;) ; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"a for-loop initializer can't declare a typedef (C99 \\u00a76.8.5.3 \\u2014 only auto / register)\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_name_out_of_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_name_out_of_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { { typedef int T; } T x; return 0; }\"\n"
      "output-error\n"
      "{\"type\": \"UnexpectedToken\", \"message\": \"Unexpected token Token('IDENTIFIER', 'x') at line 1, column 39.\\nExpected one of: \\n\\t* AMP\\n\\t* AMP_ASSIGN\\n\\t* ANDAND\\n\\t* ARROW\\n\\t* ASSIGN\\n\\t* CARET\\n\\t* CARET_ASSIGN\\n\\t* COLON\\n\\t* COMMA\\n\\t* DOT\\n\\t* EQ\\n\\t* GE\\n\\t* GT\\n\\t* LBRACKET\\n\\t* LE\\n\\t* LPAREN\\n\\t* LSHIFT\\n\\t* LSHIFT_ASSIGN\\n\\t* LT\\n\\t* MINUS\\n\\t* MINUSMINUS\\n\\t* MINUS_ASSIGN\\n\\t* NE\\n\\t* OROR\\n\\t* PERCENT\\n\\t* PERCENT_ASSIGN\\n\\t* PIPE\\n\\t* PIPE_ASSIGN\\n\\t* PLUS\\n\\t* PLUSPLUS\\n\\t* PLUS_ASSIGN\\n\\t* QUESTION\\n\\t* RSHIFT\\n\\t* RSHIFT_ASSIGN\\n\\t* SEMICOLON\\n\\t* SLASH\\n\\t* SLASH_ASSIGN\\n\\t* STAR\\n\\t* STAR_ASSIGN\\n\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_parameter (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_parameter\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(typedef int x);\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"storage class 'typedef' isn't permitted on a function parameter (C99 \\u00a76.7.5.3.2 \\u2014 only register)\"}\n");
}

TEST_CASE("parse: test_typedef.py::TestTypedefErrors::test_typedef_without_name (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_typedef.py::TestTypedefErrors::test_typedef_without_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"typedef struct foo;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"typedef declaration declares no name\"}\n");
}
