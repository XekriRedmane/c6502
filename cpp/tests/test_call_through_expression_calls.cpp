// The calls that the tests of tests/test_call_through_expression.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_a_name_is_a_function_call (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_a_name_is_a_function_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int); int main(void) { return f(1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"f\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_a_name_is_a_function_call (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_a_name_is_a_function_call\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int); int main(void) { return (f)(1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"f\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_constant_callee_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_constant_callee_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return 3(); }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"called object is not a function\"}\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_constant_callee_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_constant_callee_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { A }; int main(void) { return A(); }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"called object is not a function\"}\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_constant_callee_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_constant_callee_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return \\\"f\\\"(); }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"called object is not a function\"}\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int (*t[2])(int); int main(void) { return t[1](3); }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"t\" None Array(Pointer(FunType([Int] Int)) 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(IndirectCall(Subscript(Var(\"t\" None) Constant(ConstInt(1) None) None) [Constant(ConstInt(3) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { int (*g)(int); } s; int main(void) { return s.g(3); }\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"g\" Pointer(FunType([Int] Int)))]))\n"
      "  VarDecl(var_decl(\"s\" None Structure(\"S\") None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(IndirectCall(Dot(Var(\"s\" None) \"g\" None) [Constant(ConstInt(3) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { int (*g)(int); } *p; int main(void) { return p->g(3); }\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"g\" Pointer(FunType([Int] Int)))]))\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Structure(\"S\")) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(IndirectCall(Arrow(Var(\"p\" None) \"g\" None) [Constant(ConstInt(3) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_other_callees_are_indirect_calls\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int (*fp)(int); int main(void) { return (*fp)(3); }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"fp\" None Pointer(FunType([Int] Int)) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(IndirectCall(Dereference(Var(\"fp\" None) None) [Constant(ConstInt(3) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_call_through_expression.py::TestParse::test_returned_pointer_called (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_call_through_expression.py::TestParse::test_returned_pointer_called\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int (*pick(void))(int); int main(void) { return pick()(3); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"pick\" [] None FunType([] Pointer(FunType([Int] Int))) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(IndirectCall(FunctionCall(\"pick\" [] None false) [Constant(ConstInt(3) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}
