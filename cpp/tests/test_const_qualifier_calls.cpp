// The calls that the tests of tests/test_const_qualifier.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_after_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_after_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int const x = 5;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_function_param (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_function_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(const int x) { return x + 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Const(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestParsingConst::test_const_function_param (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_function_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Const(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Const(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestParsingConst::test_const_function_param (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_function_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Const(Int)) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Const(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Const(Int)] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Const(Int)) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Const(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Const(Int)] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_idempotent (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_idempotent\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const const int x = 5;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_in_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_in_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int x = 5;\\n    return (const int)x;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Cast(Const(Int) Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_in_pointer_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_in_pointer_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int x = 5;\\n    int *p = &x;\\n    const int *q = (const int *)p;\\n    return *q;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" Cast(Pointer(Const(Int)) Var(\"p\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Return(Dereference(Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestParsingConst::test_const_in_pointer_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_in_pointer_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" Cast(Pointer(Const(Int)) Var(\"p\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Return(Dereference(Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" Cast(Pointer(Const(Int)) Var(\"@1.p\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Return(Dereference(Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestParsingConst::test_const_in_pointer_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_in_pointer_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" Cast(Pointer(Const(Int)) Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Const(Int))) Pointer(Const(Int)) None None None None false)))\\n    S(Return(Dereference(Var(\\\"@2.q\\\" Pointer(Const(Int))) Const(Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" Cast(Pointer(Const(Int)) Var(\"@1.p\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Return(Dereference(Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" Cast(Pointer(Const(Int)) Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Const(Int))) Pointer(Const(Int)) None None None None false)))\\n    S(Return(Dereference(Var(\\\"@2.q\\\" Pointer(Const(Int))) Const(Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_in_struct_member (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_in_struct_member\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { const int x; int y; };\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Const(Int)) member_decl(\"y\" Int)]))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_on_simple_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_on_simple_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const int x = 5;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_pointer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x; int * const p = &x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Const(Pointer(Int)) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_const_pointer_to_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_const_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x; const int * const p = &x;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Const(Pointer(Const(Int))) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestParsingConst::test_pointer_to_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestParsingConst::test_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const int *p;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Const(Int)) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_assign_through_const_pointer_is_OK (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_assign_through_const_pointer_is_OK\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int x = 5;\\n    int * const p = &x;\\n    *p = 10;\\n    return x;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_assign_through_const_pointer_is_OK (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_assign_through_const_pointer_is_OK\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"@1.p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_assign_through_const_pointer_is_OK (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_assign_through_const_pointer_is_OK\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Const(Pointer(Int)) None None None None false)))\\n    S(Expression(Assignment(Dereference(Var(\\\"@1.p\\\" Const(Pointer(Int))) Int) Constant(ConstInt(10) Int) Int)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"@1.p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.x\\\" Int) Pointer(Int)) Const(Pointer(Int)) None None None None false)))\\n    S(Expression(Assignment(Dereference(Var(\\\"@1.p\\\" Const(Pointer(Int))) Int) Constant(ConstInt(10) Int) Int)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_assign_to_const_var (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_assign_to_const_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const int x = 5;\\nint main(void) { x = 10; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"x\" None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_assign_to_const_var (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_assign_to_const_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"x\" None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"x\" None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_assign_to_const_var (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_assign_to_const_var\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Assignment(Var(\"x\" None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Var(name='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_compound_assign_to_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_compound_assign_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const int x = 5;\\nint main(void) { x += 1; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(CompoundAssignment(Add Var(\"x\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_compound_assign_to_const (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_compound_assign_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(CompoundAssignment(Add Var(\"x\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(CompoundAssignment(Add Var(\"x\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_compound_assign_to_const (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_compound_assign_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(CompoundAssignment(Add Var(\"x\" None) Constant(ConstInt(1) None) None None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot modify const-qualified lvalue with compound assignment: Var(name='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_modify_array_of_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_array_of_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    const int arr[3] = {1, 2, 3};\\n    arr[0] = 99;\\n    return arr[0];\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"arr\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Const(Int) 3) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"arr\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Subscript(Var(\"arr\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_modify_array_of_const (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_array_of_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"arr\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Const(Int) 3) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"arr\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Subscript(Var(\"arr\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Const(Int) 3) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"@0.arr\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Subscript(Var(\"@0.arr\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_modify_array_of_const (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_array_of_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Const(Int) 3) None None None None false)))\n"
      "    S(Expression(Assignment(Subscript(Var(\"@0.arr\" None) Constant(ConstInt(0) None) None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Subscript(Var(\"@0.arr\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Subscript(array=AddressOf(exp=Var(name='@0.arr', data_type=Array(element_type=Const(referenced_type=Int()), size=3)), data_type=Pointer(referenced_type=Const(referenced_type=Int()))), index=Constant(const=ConstInt(value=0), data_type=Int()), data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_modify_const_pointer_itself (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_const_pointer_itself\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int x = 5;\\n    int y = 10;\\n    int * const p = &x;\\n    p = &y;\\n    return *p;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"y\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) AddressOf(Var(\"y\" None) None) None)))\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_modify_const_pointer_itself (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_const_pointer_itself\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"y\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"p\" None) AddressOf(Var(\"y\" None) None) None)))\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@0.x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@2.p\" None) AddressOf(Var(\"@1.y\" None) None) None)))\n"
      "    S(Return(Dereference(Var(\"@2.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_modify_const_pointer_itself (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_const_pointer_itself\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Constant(ConstInt(10) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@0.x\" None) None) Const(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@2.p\" None) AddressOf(Var(\"@1.y\" None) None) None)))\n"
      "    S(Return(Dereference(Var(\"@2.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Var(name='@2.p', data_type=Const(referenced_type=Pointer(referenced_type=Int()))) has type Const(referenced_type=Pointer(referenced_type=Int()))\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_modify_const_struct_member_directly (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_const_struct_member_directly\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { const int x; int y; };\\nint main(void) {\\n    struct S s; s.x = 5; return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Const(Int)) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" None Structure(\"S\") None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"s\" None) \"x\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_modify_const_struct_member_directly (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_const_struct_member_directly\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Const(Int)) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" None Structure(\"S\") None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"s\" None) \"x\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Const(Int)) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" None Structure(\"S\") None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"@0.s\" None) \"x\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_modify_const_struct_member_directly (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_const_struct_member_directly\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Const(Int)) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" None Structure(\"S\") None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"@0.s\" None) \"x\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Dot(operand=Var(name='@0.s', data_type=Structure(tag='S')), member='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_modify_member_of_const_struct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_member_of_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { int x; int y; };\\nint main(void) {\\n    const struct S s = {1, 2};\\n    s.y = 99;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"S\")) None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"s\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_modify_member_of_const_struct (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_member_of_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"S\")) None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"s\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"S\")) None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"@0.s\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_modify_member_of_const_struct (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_member_of_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"S\")) None None None None false)))\n"
      "    S(Expression(Assignment(Dot(Var(\"@0.s\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Dot(operand=Var(name='@0.s', data_type=Const(referenced_type=Structure(tag='S'))), member='y', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {\\n    int x = 5;\\n    const int *p = &x;\\n    *p = 10;\\n    return *p;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"@1.p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Dereference(Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Const(Int)) None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"@1.p\" None) None) Constant(ConstInt(10) None) None)))\n"
      "    S(Return(Dereference(Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Dereference(exp=Var(name='@1.p', data_type=Pointer(referenced_type=Const(referenced_type=Int()))), data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const_struct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct S { int x; int y; };\\nint main(void) {\\n    struct S s = {1, 2};\\n    const struct S *p = &s;\\n    p->y = 99;\\n    return 0;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Structure(\"S\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"s\" None) None) Pointer(Const(Structure(\"S\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"p\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const_struct (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Structure(\"S\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"s\" None) None) Pointer(Const(Structure(\"S\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"p\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Structure(\"S\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.s\" None) None) Pointer(Const(Structure(\"S\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"@1.p\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const_struct (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_modify_through_pointer_to_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"S\" false [member_decl(\"x\" Int) member_decl(\"y\" Int)]))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Structure(\"S\") None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.s\" None) None) Pointer(Const(Structure(\"S\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"@1.p\" None) \"y\" None) Constant(ConstInt(99) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Arrow(operand=Var(name='@1.p', data_type=Pointer(referenced_type=Const(referenced_type=Structure(tag='S')))), member='y', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_postfix_increment_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_postfix_increment_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const int x = 5;\\nint main(void) { x++; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_postfix_increment_const (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_postfix_increment_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_postfix_increment_const (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_postfix_increment_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot use ++/-- on const-qualified lvalue: Var(name='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestModificationErrors::test_prefix_decrement_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_prefix_decrement_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const int x = 5;\\nint main(void) { --x; return x; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Prefix(Decrement Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_const_qualifier.py::TestModificationErrors::test_prefix_decrement_const (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_prefix_decrement_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Prefix(Decrement Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Prefix(Decrement Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_const_qualifier.py::TestModificationErrors::test_prefix_decrement_const (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_const_qualifier.py::TestModificationErrors::test_prefix_decrement_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Const(Int) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Expression(Prefix(Decrement Var(\"x\" None) None)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot use ++/-- on const-qualified lvalue: Var(name='x', data_type=Const(referenced_type=Int())) has type Const(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestVolatileQualifierAndRestrictDrop::test_restrict_pointer_parses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestVolatileQualifierAndRestrictDrop::test_restrict_pointer_parses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *restrict p;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_const_qualifier.py::TestVolatileQualifierAndRestrictDrop::test_volatile_int_parses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_const_qualifier.py::TestVolatileQualifierAndRestrictDrop::test_volatile_int_parses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"volatile int x = 5;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Volatile(Int) None None None None false))\n"
      "])\n");
}
