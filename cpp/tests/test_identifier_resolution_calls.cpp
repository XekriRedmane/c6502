// The calls that the tests of tests/test_identifier_resolution.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestDeclarations::test_bare_declaration_renames (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestDeclarations::test_bare_declaration_renames\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestDeclarations::test_duplicate_check_is_against_original_name (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestDeclarations::test_duplicate_check_is_against_original_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate declaration of 'a'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestDeclarations::test_duplicate_declaration_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestDeclarations::test_duplicate_declaration_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate declaration of 'a'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestDeclarations::test_initializer_is_resolved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestDeclarations::test_initializer_is_resolved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Var(\"a\" None) Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Var(\"@0.a\" None) Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestDeclarations::test_initializer_using_undeclared_name_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestDeclarations::test_initializer_using_undeclared_name_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Var(\"b\" None) Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'b'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestDeclarations::test_self_initialization_resolves_to_new_binding (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestDeclarations::test_self_initialization_resolves_to_new_binding\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Var(\"a\" None) Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Var(\"@0.a\" None) Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_assignment_on_left_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_assignment_on_left_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"c\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Assignment(Var(\"a\" None) Var(\"b\" None) None) Var(\"c\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in assignment: Assignment(lval=Var(name='a', data_type=None), rval=Var(name='b', data_type=None), data_type=None)\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_assignment_resolves_both_sides (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_assignment_resolves_both_sides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_binary_on_left_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_binary_on_left_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Binary(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in assignment: Binary(op=Add(), left=Constant(const=ConstInt(value=1), data_type=None), right=Constant(const=ConstInt(value=2), data_type=None), data_type=None)\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_binary_recurses_both_sides (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_binary_recurses_both_sides\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_chained_assignment_preserves_right_associativity (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_chained_assignment_preserves_right_associativity\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"c\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Assignment(Var(\"b\" None) Var(\"c\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.c\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Assignment(Var(\"@1.b\" None) Var(\"@2.c\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_chained_assignment_with_var_lvals_still_works (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_chained_assignment_with_var_lvals_still_works\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"c\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Assignment(Var(\"b\" None) Var(\"c\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.c\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Assignment(Var(\"@1.b\" None) Var(\"@2.c\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_constant_is_preserved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_constant_is_preserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(42) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(42) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_constant_on_left_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_constant_on_left_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Constant(ConstInt(5) None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in assignment: Constant(const=ConstInt(value=5), data_type=None)\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_unary_on_left_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_unary_on_left_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Unary(Negate Var(\"a\" None) None) Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in assignment: Unary(op=Negate(), exp=Var(name='a', data_type=None), data_type=None)\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_unary_recurses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_unary_recurses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(Return(Unary(Negate Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" None Int None None None None false)))\n"
      "    S(Return(Unary(Negate Var(\"@0.x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_undeclared_var_reference_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_undeclared_var_reference_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'x'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestExpressionResolution::test_var_reference_is_renamed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestExpressionResolution::test_var_reference_is_renamed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" None Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIfStatementResolution::test_if_resolves_condition_and_then (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_resolves_condition_and_then\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(IfStmt(Var(\"a\" None) Return(Var(\"a\" None)) None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(IfStmt(Var(\"@0.a\" None) Return(Var(\"@0.a\" None)) None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIfStatementResolution::test_if_resolves_else_branch (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_resolves_else_branch\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    S(IfStmt(Var(\"a\" None) Return(Var(\"a\" None)) Return(Var(\"b\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Int None None None None false)))\n"
      "    S(IfStmt(Var(\"@0.a\" None) Return(Var(\"@0.a\" None)) Return(Var(\"@1.b\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_condition_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { if (a) return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Var(\"a\" None) Return(Constant(ConstInt(0) None)) None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_condition_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Var(\"a\" None) Return(Constant(ConstInt(0) None)) None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_else_branch_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_else_branch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { if (1) return 0; else return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(0) None)) Return(Var(\"a\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_else_branch_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_else_branch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(0) None)) Return(Var(\"a\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_then_branch_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_then_branch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { if (1) return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Return(Var(\"a\" None)) None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_then_branch_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIfStatementResolution::test_if_undeclared_in_then_branch_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(IfStmt(Constant(ConstInt(1) None) Return(Var(\"a\" None)) None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestConditionalResolution::test_all_three_subexpressions_resolve (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_all_three_subexpressions_resolve\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"c\" None Int None None None None false)))\n"
      "    S(Return(Conditional(Var(\"a\" None) Var(\"b\" None) Var(\"c\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.c\" None Int None None None None false)))\n"
      "    S(Return(Conditional(Var(\"@0.a\" None) Var(\"@1.b\" None) Var(\"@2.c\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestConditionalResolution::test_conditional_as_lvalue_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_conditional_as_lvalue_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; 1 \077 2 : a = 5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Conditional(Constant(ConstInt(1) None) Constant(ConstInt(2) None) Var(\"a\" None) None) Constant(ConstInt(5) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestConditionalResolution::test_conditional_as_lvalue_is_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_conditional_as_lvalue_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Assignment(Conditional(Constant(ConstInt(1) None) Constant(ConstInt(2) None) Var(\"a\" None) None) Constant(ConstInt(5) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in assignment: Conditional(condition=Constant(const=ConstInt(value=1), data_type=None), true_clause=Constant(const=ConstInt(value=2), data_type=None), false_clause=Var(name='a', data_type=None), data_type=None)\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_condition_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return a \077 1 : 2; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Conditional(Var(\"a\" None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_condition_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Conditional(Var(\"a\" None) Constant(ConstInt(1) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_false_clause_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_false_clause_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1 \077 2 : a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Constant(ConstInt(2) None) Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_false_clause_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_false_clause_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Constant(ConstInt(2) None) Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_true_clause_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_true_clause_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 1 \077 a : 2; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_true_clause_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestConditionalResolution::test_undeclared_in_true_clause_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_goto_passes_through_unchanged (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_goto_passes_through_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { goto foo; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_goto_passes_through_unchanged (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_goto_passes_through_unchanged\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Goto(\"foo\"))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_labeled_statement_label_unchanged_body_resolved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_labeled_statement_label_unchanged_body_resolved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; foo: return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(LabeledStmt(\"foo\" Return(Var(\"a\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_labeled_statement_label_unchanged_body_resolved (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_labeled_statement_label_unchanged_body_resolved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(LabeledStmt(\"foo\" Return(Var(\"a\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(LabeledStmt(\"foo\" Return(Var(\"@0.a\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_undeclared_var_inside_labeled_stmt_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_undeclared_var_inside_labeled_stmt_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { foo: return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Return(Var(\"a\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_undeclared_var_inside_labeled_stmt_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLabeledAndGotoPassthrough::test_undeclared_var_inside_labeled_stmt_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(LabeledStmt(\"foo\" Return(Var(\"a\" None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestStatementPassthrough::test_mixed_block_items_are_handled_in_order (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestStatementPassthrough::test_mixed_block_items_are_handled_in_order\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Null)\n"
      "    S(Expression(Assignment(Var(\"a\" None) Constant(ConstInt(2) None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Null)\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Constant(ConstInt(2) None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestStatementPassthrough::test_null_statement_is_preserved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestStatementPassthrough::test_null_statement_is_preserved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Null)\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Null)\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_doubly_nested_blocks_chain_outer_visibility (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_doubly_nested_blocks_chain_outer_visibility\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Return(Var(\"a\" None)))\n"
      "      ])))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Compound(Block([\n"
      "        S(Return(Var(\"@0.a\" None)))\n"
      "      ])))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_doubly_nested_redeclaration_uses_innermost (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_doubly_nested_redeclaration_uses_innermost\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "      S(Compound(Block([\n"
      "        S(Return(Var(\"a\" None)))\n"
      "      ])))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "      S(Compound(Block([\n"
      "        S(Return(Var(\"@1.a\" None)))\n"
      "      ])))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_inner_block_can_shadow_outer_name (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_inner_block_can_shadow_outer_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_inner_block_sees_outer_name_when_not_shadowed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_inner_block_sees_outer_name_when_not_shadowed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Return(Var(\"a\" None)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      S(Return(Var(\"@0.a\" None)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_inner_block_undeclared_var_still_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_inner_block_undeclared_var_still_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      S(Return(Var(\"b\" None)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'b'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_inner_decl_self_init_reads_inner_uninitialized_name (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_inner_decl_self_init_reads_inner_uninitialized_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Var(\"a\" None) Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.a\" Var(\"@1.a\" None) Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_outer_name_intact_after_inner_block_exits (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_outer_name_intact_after_inner_block_exits\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_redeclaration_in_outer_after_inner_decl_still_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_redeclaration_in_outer_after_inner_decl_still_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    ])))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate declaration of 'a'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_redeclaration_in_same_inner_block_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_redeclaration_in_same_inner_block_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate declaration of 'a'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestNestedScopes::test_two_inner_blocks_can_each_redeclare (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestNestedScopes::test_two_inner_blocks_can_each_redeclare\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    ])))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.a\" None Int None None None None false)))\n"
      "    ])))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@2.a\" None Int None None None None false)))\n"
      "    ])))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_break_continue_pass_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_break_continue_pass_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { while (1) { break; continue; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(BreakStmt(\"\"))\n"
      "      S(ContinueStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_break_continue_pass_through (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_break_continue_pass_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(BreakStmt(\"\"))\n"
      "      S(ContinueStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(BreakStmt(\"\"))\n"
      "      S(ContinueStmt(\"\"))\n"
      "    ])) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_do_while_body_resolves_outer_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_do_while_body_resolves_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; do a = 1; while (a); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(DoWhileStmt(Expression(Assignment(Var(\"a\" None) Constant(ConstInt(1) None) None)) Var(\"a\" None) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_do_while_body_resolves_outer_var (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_do_while_body_resolves_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(DoWhileStmt(Expression(Assignment(Var(\"a\" None) Constant(ConstInt(1) None) None)) Var(\"a\" None) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(DoWhileStmt(Expression(Assignment(Var(\"@0.a\" None) Constant(ConstInt(1) None) None)) Var(\"@0.a\" None) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_do_while_undeclared_in_body_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_do_while_undeclared_in_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { do a = 1; while (1); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(DoWhileStmt(Expression(Assignment(Var(\"a\" None) Constant(ConstInt(1) None) None)) Constant(ConstInt(1) None) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_do_while_undeclared_in_body_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_do_while_undeclared_in_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(DoWhileStmt(Expression(Assignment(Var(\"a\" None) Constant(ConstInt(1) None) None)) Constant(ConstInt(1) None) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_compound_body_can_shadow_for_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_compound_body_can_shadow_for_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (int i = 0;;) { int i = 5; break; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      D(VarDecl(var_decl(\"i\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_compound_body_can_shadow_for_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_compound_body_can_shadow_for_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      D(VarDecl(var_decl(\"i\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_empty_header_body_resolves (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_empty_header_body_resolves\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int i; for (;;) i; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(None) None None Expression(Var(\"i\" None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_empty_header_body_resolves (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_empty_header_body_resolves\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(None) None None Expression(Var(\"i\" None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(None) None None Expression(Var(\"@0.i\" None)) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_binds_in_header_and_body (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_binds_in_header_and_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (int i = 0; i < 10; i++) i; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(10) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_binds_in_header_and_body (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_binds_in_header_and_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(10) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(10) None) None) Postfix(Increment Var(\"@0.i\" None) None) Expression(Var(\"@0.i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_shadows_outer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_shadows_outer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; for (int a = 1; a < 10; a = a + 1) a = a + 5; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)]) Binary(LessThan Var(\"a\" None) Constant(ConstInt(10) None) None) Assignment(Var(\"a\" None) Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None) None) Expression(Assignment(Var(\"a\" None) Binary(Add Var(\"a\" None) Constant(ConstInt(5) None) None) None)) \"\" None))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_shadows_outer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_shadows_outer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)]) Binary(LessThan Var(\"a\" None) Constant(ConstInt(10) None) None) Assignment(Var(\"a\" None) Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None) None) Expression(Assignment(Var(\"a\" None) Binary(Add Var(\"a\" None) Constant(ConstInt(5) None) None) None)) \"\" None))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(ForStmt(InitDecl([var_decl(\"@1.a\" Constant(ConstInt(1) None) Int None None None None false)]) Binary(LessThan Var(\"@1.a\" None) Constant(ConstInt(10) None) None) Assignment(Var(\"@1.a\" None) Binary(Add Var(\"@1.a\" None) Constant(ConstInt(1) None) None) None) Expression(Assignment(Var(\"@1.a\" None) Binary(Add Var(\"@1.a\" None) Constant(ConstInt(5) None) None) None)) \"\" None))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_visible_to_compound_body (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_visible_to_compound_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (int i = 0; ; ) { i; break; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      S(Expression(Var(\"i\" None)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_visible_to_compound_body (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_decl_visible_to_compound_body\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      S(Expression(Var(\"i\" None)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      S(Expression(Var(\"@0.i\" None)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_init_duplicate_with_for_scope_decl_in_body_is_legal (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_duplicate_with_for_scope_decl_in_body_is_legal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (int i = 0;;) { int i = 1; break; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      D(VarDecl(var_decl(\"i\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_init_duplicate_with_for_scope_decl_in_body_is_legal (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_duplicate_with_for_scope_decl_in_body_is_legal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      D(VarDecl(var_decl(\"i\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) None None Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.i\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "      S(BreakStmt(\"\"))\n"
      "    ])) \"\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_init_exp_uses_outer_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_exp_uses_outer_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int i; for (i = 0; i < 5; i++) i; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(Assignment(Var(\"i\" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var(\"i\" None) Constant(ConstInt(5) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_init_exp_uses_outer_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_init_exp_uses_outer_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(Assignment(Var(\"i\" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var(\"i\" None) Constant(ConstInt(5) None) None) Postfix(Increment Var(\"i\" None) None) Expression(Var(\"i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.i\" None Int None None None None false)))\n"
      "    S(ForStmt(InitExp(Assignment(Var(\"@0.i\" None) Constant(ConstInt(0) None) None)) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(5) None) None) Postfix(Increment Var(\"@0.i\" None) None) Expression(Var(\"@0.i\" None)) \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_condition_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (; a < 10;) ; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) Binary(LessThan Var(\"a\" None) Constant(ConstInt(10) None) None) None Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_condition_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) Binary(LessThan Var(\"a\" None) Constant(ConstInt(10) None) None) None Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_post_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_post_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (;;a++) ; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None Postfix(Increment Var(\"a\" None) None) Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_post_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_for_undeclared_in_post_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitExp(None) None Postfix(Increment Var(\"a\" None) None) Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_while_body_resolves_outer_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_while_body_resolves_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; while (a) a = a + 1; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(WhileStmt(Var(\"a\" None) Expression(Assignment(Var(\"a\" None) Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None) None)) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_while_body_resolves_outer_var (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_while_body_resolves_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(WhileStmt(Var(\"a\" None) Expression(Assignment(Var(\"a\" None) Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None) None)) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(WhileStmt(Var(\"@0.a\" None) Expression(Assignment(Var(\"@0.a\" None) Binary(Add Var(\"@0.a\" None) Constant(ConstInt(1) None) None) None)) \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLoopResolution::test_while_undeclared_in_condition_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_while_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { while (a) ; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Var(\"a\" None) Null \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLoopResolution::test_while_undeclared_in_condition_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLoopResolution::test_while_undeclared_in_condition_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(WhileStmt(Var(\"a\" None) Null \"\"))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestSwitchResolution::test_switch_control_resolves_outer_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_switch_control_resolves_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; switch (a) { case 0: return 0; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestSwitchResolution::test_switch_control_resolves_outer_var (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_switch_control_resolves_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_case_value_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_case_value_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 1; switch (x) { case y: return 0; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Var(\"y\" None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_case_value_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_case_value_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Var(\"y\" None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'y'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_switch_control_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_switch_control_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (a) { case 0: return 0; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_switch_control_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_in_switch_control_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_case_body_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_case_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (1) { case 0: return a; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Var(\"a\" None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_case_body_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_case_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Var(\"a\" None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_default_body_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_default_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (1) { default: return a; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(DefaultStmt(Return(Var(\"a\" None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_default_body_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestSwitchResolution::test_undeclared_var_inside_default_body_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(1) None) Compound(Block([\n"
      "      S(DefaultStmt(Return(Var(\"a\" None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestResolveProgram::test_wraps_function_in_program (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestResolveProgram::test_wraps_function_in_program\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" None Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_rejects_non_var_lhs (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_rejects_non_var_lhs\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { 1 += 2; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(CompoundAssignment(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_rejects_non_var_lhs (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_rejects_non_var_lhs\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(CompoundAssignment(Add Constant(ConstInt(1) None) Constant(ConstInt(2) None) None None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in compound assignment: Constant(const=ConstInt(value=1), data_type=None)\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_resolves_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_resolves_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; a += 1; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_resolves_var (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_compound_assignment_resolves_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.a\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_duplicate_decl_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_duplicate_decl_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; int a; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_duplicate_decl_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_duplicate_decl_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate declaration of 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_inner_block_shadows_outer_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_inner_block_shadows_outer_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; { int a = 2; } return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_inner_block_shadows_outer_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_inner_block_shadows_outer_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.a\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_rejects_non_var_operand (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_rejects_non_var_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { 1++; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Increment Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_rejects_non_var_operand (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_rejects_non_var_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Increment Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in postfix: Constant(const=ConstInt(value=1), data_type=None)\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_resolves_operand_to_unique_name (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_resolves_operand_to_unique_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; a++; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"a\" None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_resolves_operand_to_unique_name (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_postfix_resolves_operand_to_unique_name\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"a\" None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.a\" None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_prefix_rejects_non_lvalue_operand (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_prefix_rejects_non_lvalue_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { ++1; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Prefix(Increment Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_prefix_rejects_non_lvalue_operand (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_prefix_rejects_non_lvalue_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Prefix(Increment Constant(ConstInt(1) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"invalid lvalue in prefix increment/decrement: Constant(const=ConstInt(value=1), data_type=None)\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_same_block_redeclaration_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_same_block_redeclaration_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { { int a; int a; } return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_same_block_redeclaration_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_same_block_redeclaration_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "      D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate declaration of 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_simple_program (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_simple_program\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 5; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_simple_program (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_simple_program\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestIntegrationWithParser::test_undeclared_use_from_source (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_undeclared_use_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestIntegrationWithParser::test_undeclared_use_from_source (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestIntegrationWithParser::test_undeclared_use_from_source\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestFunctionLinkage::test_call_args_are_resolved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_call_args_are_resolved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int f(int x); int a; return f(a); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Return(FunctionCall(\"f\" [Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestFunctionLinkage::test_call_args_are_resolved (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_call_args_are_resolved\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    S(Return(FunctionCall(\"f\" [Var(\"a\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"@0.x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" None Int None None None None false)))\n"
      "    S(Return(FunctionCall(\"f\" [Var(\"@1.a\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestFunctionLinkage::test_call_to_top_level_function_from_another (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_call_to_top_level_function_from_another\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(void) { return 1; } int main(void) { return foo(); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestFunctionLinkage::test_call_to_top_level_function_from_another (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_call_to_top_level_function_from_another\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestFunctionLinkage::test_call_to_undeclared_function_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_call_to_undeclared_function_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return foo(); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestFunctionLinkage::test_call_to_undeclared_function_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_call_to_undeclared_function_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'foo'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestFunctionLinkage::test_duplicate_function_decl_is_legal (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_duplicate_function_decl_is_legal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); int foo(void); return foo(); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestFunctionLinkage::test_duplicate_function_decl_is_legal (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_duplicate_function_decl_is_legal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestFunctionLinkage::test_function_decl_name_is_not_renamed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_function_decl_name_is_not_renamed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); foo(); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Expression(FunctionCall(\"foo\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestFunctionLinkage::test_function_decl_name_is_not_renamed (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_function_decl_name_is_not_renamed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Expression(FunctionCall(\"foo\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Expression(FunctionCall(\"foo\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestFunctionLinkage::test_top_level_function_is_visible_for_self_call (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_top_level_function_is_visible_for_self_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return main(); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"main\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestFunctionLinkage::test_top_level_function_is_visible_for_self_call (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_top_level_function_is_visible_for_self_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"main\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"main\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestFunctionLinkage::test_var_resolution_of_arg_inside_call (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_var_resolution_of_arg_inside_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int f(int x); int a = 5; f(a + 1); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Expression(FunctionCall(\"f\" [Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None)] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestFunctionLinkage::test_var_resolution_of_arg_inside_call (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestFunctionLinkage::test_var_resolution_of_arg_inside_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Expression(FunctionCall(\"f\" [Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None)] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"@0.x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@1.a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Expression(FunctionCall(\"f\" [Binary(Add Var(\"@1.a\" None) Constant(ConstInt(1) None) None)] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_body_can_reference_param (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_body_can_reference_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int x) { return x + 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_body_can_reference_param (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_body_can_reference_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_body_decl_collides_with_param (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_body_decl_collides_with_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a) { int a = 3; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_body_decl_collides_with_param (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_body_decl_collides_with_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate declaration of 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_body_inner_block_can_shadow_param (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_body_inner_block_can_shadow_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a) { { int a = 3; return a; } return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "      S(Return(Var(\"a\" None)))\n"
      "    ])))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_body_inner_block_can_shadow_param (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_body_inner_block_can_shadow_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "      S(Return(Var(\"a\" None)))\n"
      "    ])))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "      S(Return(Var(\"@1.a\" None)))\n"
      "    ])))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_decl_duplicate_param_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_duplicate_param_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a, int a); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"a\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_decl_duplicate_param_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_duplicate_param_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"a\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate parameter name 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_does_not_conflict_with_outer_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_does_not_conflict_with_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; int foo(int a); return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_does_not_conflict_with_outer_var (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_does_not_conflict_with_outer_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@1.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_scope_dies_after_decl (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_scope_dies_after_decl\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int x); int x; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_scope_dies_after_decl (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_param_scope_dies_after_decl\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.x\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@1.x\" None Int None None None None false)))\n"
      "    S(Return(Var(\"@1.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_decl_renames_params (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_renames_params\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int x, int y); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"x\" \"y\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_decl_renames_params (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_decl_renames_params\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"x\" \"y\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.x\" \"@1.y\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_definition_duplicate_param_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_definition_duplicate_param_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(int a, int a) { return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"a\" \"a\"] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_definition_duplicate_param_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_definition_duplicate_param_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"a\" \"a\"] Block([\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate parameter name 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestParameterResolution::test_function_definition_renames_params (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_definition_renames_params\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(int x, int y) { return x + y; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"x\" \"y\"] Block([\n"
      "    S(Return(Binary(Add Var(\"x\" None) Var(\"y\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestParameterResolution::test_function_definition_renames_params (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestParameterResolution::test_function_definition_renames_params\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"x\" \"y\"] Block([\n"
      "    S(Return(Binary(Add Var(\"x\" None) Var(\"y\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"@0.x\" \"@1.y\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.x\" None) Var(\"@1.y\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLinkageTracking::test_block_scope_extern_inherits_file_scope_linkage (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_block_scope_extern_inherits_file_scope_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static int x = 5; int main(void) { extern int x; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLinkageTracking::test_block_scope_extern_inherits_file_scope_linkage (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_block_scope_extern_inherits_file_scope_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLinkageTracking::test_changing_linkage_at_file_scope_is_an_error (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_changing_linkage_at_file_scope_is_an_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static int x; int x; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLinkageTracking::test_changing_linkage_at_file_scope_is_an_error (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_changing_linkage_at_file_scope_is_an_error\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"file-scope identifier 'x' declared with external linkage after a prior internal declaration\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLinkageTracking::test_static_at_block_scope_is_none_linkage (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_static_at_block_scope_is_none_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int x = 5; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLinkageTracking::test_static_at_block_scope_is_none_linkage (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_static_at_block_scope_is_none_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestLinkageTracking::test_static_on_block_scope_function_decl_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_static_on_block_scope_function_decl_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int foo(void); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) Static None [] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestLinkageTracking::test_static_on_block_scope_function_decl_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestLinkageTracking::test_static_on_block_scope_function_decl_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) Static None [] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"static is not allowed on a block-scope function declaration: 'foo'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestUnnamedParameters::test_definition_requires_names (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestUnnamedParameters::test_definition_requires_names\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int) { return 0; } int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\"] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestUnnamedParameters::test_definition_requires_names (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestUnnamedParameters::test_definition_requires_names\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\"] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"parameter name omitted in a function definition (C99 \\u00a76.9.1.5)\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestUnnamedParameters::test_duplicate_named_params_still_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestUnnamedParameters::test_duplicate_named_params_still_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int a, int a); int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"a\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestUnnamedParameters::test_duplicate_named_params_still_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestUnnamedParameters::test_duplicate_named_params_still_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"a\" \"a\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"duplicate parameter name 'a'\"}\n");
}

TEST_CASE("parse: test_identifier_resolution.py::TestUnnamedParameters::test_prototype_with_several_unnamed_params (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_identifier_resolution.py::TestUnnamedParameters::test_prototype_with_several_unnamed_params\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int, int); int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\" \"\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_identifier_resolution.py::TestUnnamedParameters::test_prototype_with_several_unnamed_params (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_identifier_resolution.py::TestUnnamedParameters::test_prototype_with_several_unnamed_params\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\" \"\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\" \"\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
