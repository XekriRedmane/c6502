// The calls that the tests of tests/test_c23_bool.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "preprocessor_case.h"

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int true;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 5: `true` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int false = 0;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 5: `false` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int bool = 1;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 10: `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int bool[2];\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 9: `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int bool, x;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 9: `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int bool;\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"'bool' cannot combine with another type specifier; `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be a name either\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int x, int true) { return x; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 18: `true` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int false; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 16: `false` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct true { int x; };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 8: `true` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { goto bool; }\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 23: `bool` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_names_are_errors (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_names_are_errors\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { false };\"\n"
      "output-error\n"
      "{\"type\": \"ParserError\", \"message\": \"line 1, column 8: `false` is a keyword in C23 (\\u00a76.4.1), so it can't be the name of a variable, function, parameter, type, tag, member, or label\"}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_true_and_false_are_bool_constants (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_true_and_false_are_bool_constants\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a = true; int b = false;\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" Constant(ConstBool(1) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(0) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("parse: test_c23_bool.py::TestKeywords::test_type_of_true_is_bool (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestKeywords::test_type_of_true_is_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) { return true; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstBool(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestKeywords::test_type_of_true_is_bool (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestKeywords::test_type_of_true_is_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstBool(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstBool(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestKeywords::test_type_of_true_is_bool (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestKeywords::test_type_of_true_is_bool\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Int Constant(ConstBool(1) Bool) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    S(Return(Constant(ConstBool(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Cast(Int Constant(ConstBool(1) Bool) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_constant_expressions (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_constant_expressions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"enum { E = true + 4, F = false };int a[true + 2];unsigned long n = sizeof a;int e = E, f = F;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Var(\"a\" None) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"e\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstInt(0) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_constant_expressions (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_constant_expressions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Var(\"a\" None) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"e\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstInt(0) None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Var(\"a\" None) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"e\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstInt(0) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_constant_expressions (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_constant_expressions\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 3) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Var(\\\"a\\\" Array(Int 3)) UInt) ULong) ULong None None None None false))\\n  VarDecl(var_decl(\\\"e\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"f\\\" Constant(ConstInt(0) Int) Int None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Var(\"a\" None) None) ULong None None None None false))\n"
      "  VarDecl(var_decl(\"e\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"f\" Constant(ConstInt(0) None) Int None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 3) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Var(\\\"a\\\" Array(Int 3)) UInt) ULong) ULong None None None None false))\\n  VarDecl(var_decl(\\\"e\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  VarDecl(var_decl(\\\"f\\\" Constant(ConstInt(0) Int) Int None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 6}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_sizes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned long n = sizeof(true);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(1) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_sizes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(1) None) None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(1) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_sizes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Constant(ConstBool(1) Bool) UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(1) None) None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Constant(ConstBool(1) Bool) UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_sizes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned long n = sizeof false;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(0) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_sizes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(0) None) None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(0) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_sizes (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Constant(ConstBool(0) Bool) UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Constant(ConstBool(0) None) None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Constant(ConstBool(0) Bool) UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_sizes (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned long n = sizeof(bool);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Bool None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_sizes (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Bool None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Bool None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_sizes (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfType(Bool UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfType(Bool None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfType(Bool UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_sizes (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned long n = sizeof(true + true);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_sizes (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_sizes (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_sizes (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned long n = sizeof(-true);\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Unary(Negate Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_sizes (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Unary(Negate Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Unary(Negate Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_sizes (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_sizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Unary(Negate Cast(Int Constant(ConstBool(1) Bool) Int) Int) UInt) ULong) ULong None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"n\" SizeOfExp(Unary(Negate Constant(ConstBool(1) None) None) None) ULong None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(ULong SizeOfExp(Unary(Negate Cast(Int Constant(ConstBool(1) Bool) Int) Int) UInt) ULong) ULong None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool b = true;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstBool(1) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstBool(1) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool b = false;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(0) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(0) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(0) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstBool(0) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(0) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Constant(ConstBool(0) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int b = true + true;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) Int None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(Add Constant(ConstBool(1) None) Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Binary(Add Cast(Int Constant(ConstBool(1) Bool) Int) Cast(Int Constant(ConstBool(1) Bool) Int) Int) Int None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int b = -true;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Unary(Negate Cast(Int Constant(ConstBool(1) Bool) Int) Int) Int None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(Negate Constant(ConstBool(1) None) None) Int None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Unary(Negate Cast(Int Constant(ConstBool(1) Bool) Int) Int) Int None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": -1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int b = true << 14;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(LeftShift Constant(ConstBool(1) None) Constant(ConstInt(14) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 14)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(LeftShift Constant(ConstBool(1) None) Constant(ConstInt(14) None) None) Int None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(LeftShift Constant(ConstBool(1) None) Constant(ConstInt(14) None) None) Int None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 15)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Binary(LeftShift Cast(Int Constant(ConstBool(1) Bool) Int) Constant(ConstInt(14) Int) Int) Int None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Binary(LeftShift Constant(ConstBool(1) None) Constant(ConstInt(14) None) None) Int None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Binary(LeftShift Cast(Int Constant(ConstBool(1) Bool) Int) Constant(ConstInt(14) Int) Int) Int None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 16384}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 16)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"bool b = !false;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(LogicalNot Constant(ConstBool(0) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 17)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(LogicalNot Constant(ConstBool(0) None) None) Bool None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(LogicalNot Constant(ConstBool(0) None) None) Bool None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 18)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Unary(LogicalNot Constant(ConstBool(0) Bool) Int) Bool) Bool None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Unary(LogicalNot Constant(ConstBool(0) None) None) Bool None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Bool Unary(LogicalNot Constant(ConstBool(0) Bool) Int) Bool) Bool None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Bool\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 1}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 19)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char b = true \077 7 : 9;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Conditional(Constant(ConstBool(1) None) Constant(ConstInt(7) None) Constant(ConstInt(9) None) None) UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 20)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Conditional(Constant(ConstBool(1) None) Constant(ConstInt(7) None) Constant(ConstInt(9) None) None) UChar None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Conditional(Constant(ConstBool(1) None) Constant(ConstInt(7) None) Constant(ConstInt(9) None) None) UChar None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 21)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(UChar Conditional(Constant(ConstBool(1) Bool) Constant(ConstInt(7) Int) Constant(ConstInt(9) Int) Int) UChar) UChar None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Conditional(Constant(ConstBool(1) None) Constant(ConstInt(7) None) Constant(ConstInt(9) None) None) UChar None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(UChar Conditional(Constant(ConstBool(1) Bool) Constant(ConstInt(7) Int) Constant(ConstInt(9) Int) Int) UChar) UChar None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 7}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_c23_bool.py::TestValues::test_static_initializers (call 22)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"double b = true;\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Double None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_c23_bool.py::TestValues::test_static_initializers (call 23)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Double None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Double None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_c23_bool.py::TestValues::test_static_initializers (call 24)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_c23_bool.py::TestValues::test_static_initializers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Double Constant(ConstBool(1) Bool) Double) Double None None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"b\" Constant(ConstBool(1) None) Double None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"b\\\" Cast(Double Constant(ConstBool(1) Bool) Double) Double None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4607182418800017408}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("preprocess: test_c23_bool.py::TestAltBoolWarning::test_no_warnings_outside_a_collecting_block (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"_Bool a;\\n\", \"source\": \"_Bool a;\\n\", \"stderr\": \"\", \"warnings\": [[\"line 1\", \"`_Bool` is deprecated: C23 makes it an obsolescent spelling of the keyword `bool` (C23 \\u00a76.4.1). Use `bool` instead.\"]]}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestAltBoolWarning::test_one_warning_per_use (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": null, \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\n_Bool a; _Bool b;\\n_Bool c;\\nbool d;\\nchar *s = \\\"_Bool\\\";\\n\", \"source\": \"#define B _Bool\\n_Bool a; _Bool b;\\nB c;\\nbool d;\\nchar *s = \\\"_Bool\\\";\\n\", \"stderr\": \"\", \"warnings\": [[\"line 2\", \"`_Bool` is deprecated: C23 makes it an obsolescent spelling of the keyword `bool` (C23 \\u00a76.4.1). Use `bool` instead.\"], [\"line 2\", \"`_Bool` is deprecated: C23 makes it an obsolescent spelling of the keyword `bool` (C23 \\u00a76.4.1). Use `bool` instead.\"], [\"line 3\", \"`_Bool` is deprecated: C23 makes it an obsolescent spelling of the keyword `bool` (C23 \\u00a76.4.1). Use `bool` instead.\"]]}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_a_macro_takes_precedence (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [\"-Dtrue=0\"], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\", \"source\": \"#if true\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint kept;\\n\", \"source\": \"#if true\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 2)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\", \"source\": \"#if false\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 3)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint kept;\\n\", \"source\": \"#if true == 1\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 4)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint kept;\\n\", \"source\": \"#if true + true == 2\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 5)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint kept;\\n\", \"source\": \"#if -true < 0\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 6)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint kept;\\n\", \"source\": \"#if !false\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 7)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint kept;\\n\", \"source\": \"#if false == 0\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 8)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\", \"source\": \"#if defined(true)\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_true_is_one_and_false_is_zero (call 9)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\", \"source\": \"#if bool\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_with_passthru_unknown_exprs (call 1)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [\"--passthru-unknown-exprs\"], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\\nint kept;\\n\", \"source\": \"#if true\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}

TEST_CASE("preprocess: test_c23_bool.py::TestIf::test_with_passthru_unknown_exprs (call 2)") {
  c6502::testing::check_preprocess_case(
      "{\"argv\": [\"--passthru-unknown-exprs\"], \"cwd\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\", \"error\": null, \"filename\": null, \"files\": {}, \"include_dir\": \"C:\\\\Users\\\\rober\\\\c6502\\\\.claude\\\\worktrees\\\\agent-a491cc35feb6adb51\\\\include\", \"nt\": true, \"output\": \"\", \"source\": \"#if false\\nint kept;\\n#endif\\n\", \"stderr\": \"\", \"warnings\": []}", {});
}
