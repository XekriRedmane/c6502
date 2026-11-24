// The calls that the tests of tests/test_type_checking.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 to G8). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_type_checking.py::TestSymbolTableContents::test_function_decl_params_not_added_to_table (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_decl_params_not_added_to_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSymbolTableContents::test_function_decl_params_not_added_to_table (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_decl_params_not_added_to_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSymbolTableContents::test_function_decl_params_not_added_to_table (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_decl_params_not_added_to_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSymbolTableContents::test_function_decl_recorded_with_arity (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_decl_recorded_with_arity\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a, int b); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSymbolTableContents::test_function_decl_recorded_with_arity (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_decl_recorded_with_arity\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSymbolTableContents::test_function_decl_recorded_with_arity (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_decl_recorded_with_arity\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSymbolTableContents::test_function_definition_is_marked_defined (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_definition_is_marked_defined\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSymbolTableContents::test_function_definition_is_marked_defined (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_function_definition_is_marked_defined\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSymbolTableContents::test_local_variable_is_local_attr (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_local_variable_is_local_attr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 3; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSymbolTableContents::test_local_variable_is_local_attr (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_local_variable_is_local_attr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSymbolTableContents::test_local_variable_is_local_attr (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_local_variable_is_local_attr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(3) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(3) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSymbolTableContents::test_parameters_recorded_as_local_attr (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSymbolTableContents::test_parameters_recorded_as_local_attr\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [\\\"@0.x\\\" \\\"@1.y\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Var(\\\"@1.y\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"@0.x\" \"@1.y\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.x\" None) Var(\"@1.y\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [\\\"@0.x\\\" \\\"@1.y\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Var(\\\"@1.y\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_inherits_file_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_inherits_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false)))\\n    S(Return(Var(\\\"x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false)))\\n    S(Return(Var(\\\"x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_with_initializer_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_with_initializer_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { extern int x = 5; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_with_initializer_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_with_initializer_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_with_initializer_raises (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_extern_with_initializer_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Extern None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"block-scope `extern` declaration of 'x' may not have an initializer\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_no_init_zero_initializes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_no_init_zero_initializes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int x; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_no_init_zero_initializes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_no_init_zero_initializes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" None Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_no_init_zero_initializes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_no_init_zero_initializes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" None Int Static None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" None Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" None Int Static None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_with_init (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_with_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int x = 7; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(7) None) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_with_init (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_with_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(7) None) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(7) None) Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_with_init (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_block_scope_static_with_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(7) Int) Int Static None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(7) None) Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(7) Int) Int Static None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 7}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_extern_after_static_inherits_internal_linkage (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_after_static_inherits_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static int x; extern int x; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_extern_after_static_inherits_internal_linkage (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_after_static_inherits_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_extern_after_static_inherits_internal_linkage (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_after_static_inherits_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Static None None None false))\\n  VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Static None None None false))\\n  VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_extern_at_file_scope_no_init_is_no_initializer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_at_file_scope_no_init_is_no_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern int x; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_extern_at_file_scope_no_init_is_no_initializer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_at_file_scope_no_init_is_no_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_extern_at_file_scope_no_init_is_no_initializer (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_at_file_scope_no_init_is_no_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_extern_then_initializer_promotes_to_initial (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_then_initializer_promotes_to_initial\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern int x; int x = 5; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_extern_then_initializer_promotes_to_initial (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_then_initializer_promotes_to_initial\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_extern_then_initializer_promotes_to_initial (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_extern_then_initializer_promotes_to_initial\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false))\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Extern None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Extern None None None false))\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_file_scope_default_object_is_tentative (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_file_scope_default_object_is_tentative\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_file_scope_default_object_is_tentative (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_file_scope_default_object_is_tentative\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_file_scope_object_with_initializer_is_initial (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_file_scope_object_with_initializer_is_initial\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 5; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_file_scope_object_with_initializer_is_initial (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_file_scope_object_with_initializer_is_initial\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_file_scope_object_with_initializer_is_initial (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_file_scope_object_with_initializer_is_initial\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_is_internal_linkage (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_is_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static int x = 5; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_is_internal_linkage (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_is_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_is_internal_linkage (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_is_internal_linkage\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Constant(ConstInt(5) Int) Int Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_no_init_is_tentative_internal (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_no_init_is_tentative_internal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"static int x; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_no_init_is_tentative_internal (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_no_init_is_tentative_internal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_no_init_is_tentative_internal (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_at_file_scope_no_init_is_tentative_internal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Int Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" None Int Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_static_storage_initializer_must_be_constant (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_storage_initializer_must_be_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); static int x = foo(); return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" FunctionCall(\"foo\" [] None false) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_static_storage_initializer_must_be_constant (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_storage_initializer_must_be_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" FunctionCall(\"foo\" [] None false) Int Static None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@0.x\" FunctionCall(\"foo\" [] None false) Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_static_storage_initializer_must_be_constant (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_static_storage_initializer_must_be_constant\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@0.x\" FunctionCall(\"foo\" [] None false) Int Static None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object '@0.x' is not a constant expression\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStaticStorageAttrs::test_two_initializers_at_file_scope_raise (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_two_initializers_at_file_scope_raise\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int x = 1; int x = 2; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(2) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStaticStorageAttrs::test_two_initializers_at_file_scope_raise (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_two_initializers_at_file_scope_raise\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(2) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(2) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStaticStorageAttrs::test_two_initializers_at_file_scope_raise (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStaticStorageAttrs::test_two_initializers_at_file_scope_raise\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false))\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(2) None) Int None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"redefinition of object 'x': prior initializer 1, new initializer 2\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestVariableUsedAsFunction::test_local_variable_called_as_function (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestVariableUsedAsFunction::test_local_variable_called_as_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x; x(); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(Expression(FunctionCall(\"x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestVariableUsedAsFunction::test_local_variable_called_as_function (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestVariableUsedAsFunction::test_local_variable_called_as_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" None Int None None None None false)))\n"
      "    S(Expression(FunctionCall(\"x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" None Int None None None None false)))\n"
      "    S(Expression(FunctionCall(\"@0.x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestVariableUsedAsFunction::test_local_variable_called_as_function (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestVariableUsedAsFunction::test_local_variable_called_as_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" None Int None None None None false)))\n"
      "    S(Expression(FunctionCall(\"@0.x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"variable '@0.x' called as a function\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestVariableUsedAsFunction::test_param_called_as_function (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestVariableUsedAsFunction::test_param_called_as_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(int x) { x(); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"x\"] Block([\n"
      "    S(Expression(FunctionCall(\"x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestVariableUsedAsFunction::test_param_called_as_function (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestVariableUsedAsFunction::test_param_called_as_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"x\"] Block([\n"
      "    S(Expression(FunctionCall(\"x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"@0.x\"] Block([\n"
      "    S(Expression(FunctionCall(\"@0.x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestVariableUsedAsFunction::test_param_called_as_function (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestVariableUsedAsFunction::test_param_called_as_function\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [\"@0.x\"] Block([\n"
      "    S(Expression(FunctionCall(\"@0.x\" [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"variable '@0.x' called as a function\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestFunctionUsedAsVariable::test_assign_to_function_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_assign_to_function_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); foo = 0; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Expression(Assignment(Var(\"foo\" None) Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestFunctionUsedAsVariable::test_assign_to_function_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_assign_to_function_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Expression(Assignment(Var(\"foo\" None) Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Expression(Assignment(Var(\"foo\" None) Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestFunctionUsedAsVariable::test_assign_to_function_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_assign_to_function_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Expression(Assignment(Var(\"foo\" None) Constant(ConstInt(0) None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"assignment target is a function, not a modifiable lvalue\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestFunctionUsedAsVariable::test_function_decays_to_pointer_argument (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_decays_to_pointer_argument\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int add(int a, int b) { return a + b; } int apply(int (*f)(int, int)) { return f(1, 2); } int main(void) { int (*g)(int, int) = add; return apply(add) + g(3, 4) + (g == add); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"add\" [\"a\" \"b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"apply\" [\"f\"] Block([\n"
      "    S(Return(FunctionCall(\"f\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Int Int] Int))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"g\" Var(\"add\" None) Pointer(FunType([Int Int] Int)) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add FunctionCall(\"apply\" [Var(\"add\" None)] None false) FunctionCall(\"g\" [Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None false) None) Binary(Equal Var(\"g\" None) Var(\"add\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestFunctionUsedAsVariable::test_function_decays_to_pointer_argument (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_decays_to_pointer_argument\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"add\" [\"a\" \"b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"apply\" [\"f\"] Block([\n"
      "    S(Return(FunctionCall(\"f\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Int Int] Int))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"g\" Var(\"add\" None) Pointer(FunType([Int Int] Int)) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add FunctionCall(\"apply\" [Var(\"add\" None)] None false) FunctionCall(\"g\" [Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None false) None) Binary(Equal Var(\"g\" None) Var(\"add\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"add\" [\"@0.a\" \"@1.b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"apply\" [\"@2.f\"] Block([\n"
      "    S(Return(FunctionCall(\"@2.f\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Int Int] Int))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@3.g\" Var(\"add\" None) Pointer(FunType([Int Int] Int)) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add FunctionCall(\"apply\" [Var(\"add\" None)] None false) FunctionCall(\"@3.g\" [Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None false) None) Binary(Equal Var(\"@3.g\" None) Var(\"add\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestFunctionUsedAsVariable::test_function_decays_to_pointer_argument (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_decays_to_pointer_argument\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"add\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"apply\\\" [\\\"@2.f\\\"] Block([\\n    S(Return(FunctionCall(\\\"@2.f\\\" [Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Int false)))\\n  ]) FunType([Pointer(FunType([Int Int] Int))] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@3.g\\\" AddressOf(Var(\\\"add\\\" FunType([Int Int] Int)) Pointer(FunType([Int Int] Int))) Pointer(FunType([Int Int] Int)) None None None None false)))\\n    S(Return(Binary(Add Binary(Add FunctionCall(\\\"apply\\\" [AddressOf(Var(\\\"add\\\" FunType([Int Int] Int)) Pointer(FunType([Int Int] Int)))] Int false) FunctionCall(\\\"@3.g\\\" [Constant(ConstInt(3) Int) Constant(ConstInt(4) Int)] Int false) Int) Binary(Equal Var(\\\"@3.g\\\" Pointer(FunType([Int Int] Int))) AddressOf(Var(\\\"add\\\" FunType([Int Int] Int)) Pointer(FunType([Int Int] Int))) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"add\" [\"@0.a\" \"@1.b\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"apply\" [\"@2.f\"] Block([\n"
      "    S(Return(FunctionCall(\"@2.f\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([Pointer(FunType([Int Int] Int))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@3.g\" Var(\"add\" None) Pointer(FunType([Int Int] Int)) None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add FunctionCall(\"apply\" [Var(\"add\" None)] None false) FunctionCall(\"@3.g\" [Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None false) None) Binary(Equal Var(\"@3.g\" None) Var(\"add\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"add\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"apply\\\" [\\\"@2.f\\\"] Block([\\n    S(Return(FunctionCall(\\\"@2.f\\\" [Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Int false)))\\n  ]) FunType([Pointer(FunType([Int Int] Int))] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@3.g\\\" AddressOf(Var(\\\"add\\\" FunType([Int Int] Int)) Pointer(FunType([Int Int] Int))) Pointer(FunType([Int Int] Int)) None None None None false)))\\n    S(Return(Binary(Add Binary(Add FunctionCall(\\\"apply\\\" [AddressOf(Var(\\\"add\\\" FunType([Int Int] Int)) Pointer(FunType([Int Int] Int)))] Int false) FunctionCall(\\\"@3.g\\\" [Constant(ConstInt(3) Int) Constant(ConstInt(4) Int)] Int false) Int) Binary(Equal Var(\\\"@3.g\\\" Pointer(FunType([Int Int] Int))) AddressOf(Var(\\\"add\\\" FunType([Int Int] Int)) Pointer(FunType([Int Int] Int))) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"add\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"apply\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(FunType([Int Int] Int))] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@2.f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([Int Int] Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@3.g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(FunType([Int Int] Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_arithmetic (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_arithmetic\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); return foo + 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Binary(Add Var(\"foo\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_arithmetic (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_arithmetic\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Binary(Add Var(\"foo\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Binary(Add Var(\"foo\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_arithmetic (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_arithmetic\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Binary(Add Var(\"foo\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '+' is not defined on pointer-to-function operands (Pointer(referenced_type=FunType(params=[], ret=Int())) + Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_initializer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); int x = foo; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" Var(\"foo\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_initializer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" Var(\"foo\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@0.x\" Var(\"foo\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_initializer (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@0.x\" Var(\"foo\" None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot implicitly convert pointer type Pointer(referenced_type=FunType(params=[], ret=Int())) to arithmetic type Int(); use an explicit cast\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_return (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_return\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); return foo; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Var(\"foo\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_return (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_return\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Var(\"foo\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Var(\"foo\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_return (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestFunctionUsedAsVariable::test_function_in_return\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Var(\"foo\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot implicitly convert pointer type Pointer(referenced_type=FunType(params=[], ret=Int())) to arithmetic type Int(); use an explicit cast\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCallArity::test_correct_arity_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCallArity::test_correct_arity_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a, int b); return foo(1, 2); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCallArity::test_correct_arity_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCallArity::test_correct_arity_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCallArity::test_correct_arity_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCallArity::test_correct_arity_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([Int Int] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCallArity::test_no_args_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCallArity::test_no_args_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); return foo(); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCallArity::test_no_args_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCallArity::test_no_args_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCallArity::test_no_args_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCallArity::test_no_args_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCallArity::test_too_few_args_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCallArity::test_too_few_args_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a, int b); return foo(1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCallArity::test_too_few_args_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCallArity::test_too_few_args_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCallArity::test_too_few_args_raises (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCallArity::test_too_few_args_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"function 'foo' called with 1 argument, expected 2\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCallArity::test_too_many_args_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCallArity::test_too_many_args_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a); return foo(1, 2); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCallArity::test_too_many_args_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCallArity::test_too_many_args_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCallArity::test_too_many_args_raises (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCallArity::test_too_many_args_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"function 'foo' called with 2 arguments, expected 1\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestRedeclaration::test_decl_then_matching_def_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_decl_then_matching_def_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); return 0; } int foo(void) { return 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestRedeclaration::test_decl_then_matching_def_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_decl_then_matching_def_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRedeclaration::test_decl_then_matching_def_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_decl_then_matching_def_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestRedeclaration::test_def_then_matching_decl_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_def_then_matching_decl_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(void) { return 1; } int main(void) { int foo(void); return foo(); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestRedeclaration::test_def_then_matching_decl_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_def_then_matching_decl_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRedeclaration::test_def_then_matching_decl_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_def_then_matching_decl_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestRedeclaration::test_different_arity_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_different_arity_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a); int foo(int a, int b); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestRedeclaration::test_different_arity_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_different_arity_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@1.a\" \"@2.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRedeclaration::test_different_arity_raises (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_different_arity_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@1.a\" \"@2.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"incompatible redeclaration of 'foo': previous FunType(params=[Int()], ret=Int()), new FunType(params=[Int(), Int()], ret=Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestRedeclaration::test_matching_redeclaration_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_matching_redeclaration_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(int a); int foo(int a); return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestRedeclaration::test_matching_redeclaration_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_matching_redeclaration_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@1.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRedeclaration::test_matching_redeclaration_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRedeclaration::test_matching_redeclaration_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@1.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"foo\" [\"@1.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [\\\"@1.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRedefinition::test_single_definition_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRedefinition::test_single_definition_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] Block([\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestRedefinition::test_two_definitions_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestRedefinition::test_two_definitions_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(void) { return 1; } int foo(void) { return 2; } int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestRedefinition::test_two_definitions_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestRedefinition::test_two_definitions_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRedefinition::test_two_definitions_raises (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRedefinition::test_two_definitions_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [] Block([\n"
      "    S(Return(Constant(ConstInt(2) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"redefinition of function 'foo'\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestRecursion::test_mutual_recursion_via_decl_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestRecursion::test_mutual_recursion_via_decl_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int even(int n); int odd(int n); return even(0); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"even\" [\"n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"odd\" [\"n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"even\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestRecursion::test_mutual_recursion_via_decl_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestRecursion::test_mutual_recursion_via_decl_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"even\" [\"n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"odd\" [\"n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"even\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"even\" [\"@0.n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"odd\" [\"@1.n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"even\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRecursion::test_mutual_recursion_via_decl_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRecursion::test_mutual_recursion_via_decl_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"even\\\" [\\\"@0.n\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    D(FunctionDecl(function_decl(\\\"odd\\\" [\\\"@1.n\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(FunctionCall(\\\"even\\\" [Constant(ConstInt(0) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"even\" [\"@0.n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"odd\" [\"@1.n\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"even\" [Constant(ConstInt(0) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"even\\\" [\\\"@0.n\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    D(FunctionDecl(function_decl(\\\"odd\\\" [\\\"@1.n\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(FunctionCall(\\\"even\\\" [Constant(ConstInt(0) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"even\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"odd\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestRecursion::test_self_recursive_call_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestRecursion::test_self_recursive_call_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"main\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"main\" [] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"main\\\" [] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestVarInitializer::test_call_as_init_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_call_as_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int foo(void); int x = foo(); return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" FunctionCall(\"foo\" [] None false) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestVarInitializer::test_call_as_init_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_call_as_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"x\" FunctionCall(\"foo\" [] None false) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@0.x\" FunctionCall(\"foo\" [] None false) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestVarInitializer::test_call_as_init_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_call_as_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    D(VarDecl(var_decl(\\\"@0.x\\\" FunctionCall(\\\"foo\\\" [] Int false) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"foo\" [] None FunType([] Int) None None [] false None [] None false)))\n"
      "    D(VarDecl(var_decl(\"@0.x\" FunctionCall(\"foo\" [] None false) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Int) None None [] false None [] None false)))\\n    D(VarDecl(var_decl(\\\"@0.x\\\" FunctionCall(\\\"foo\\\" [] Int false) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestVarInitializer::test_compound_init_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_compound_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 1 + 2 * 3; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Binary(Add Constant(ConstInt(1) None) Binary(Multiply Constant(ConstInt(2) None) Constant(ConstInt(3) None) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestVarInitializer::test_compound_init_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_compound_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Binary(Add Constant(ConstInt(1) None) Binary(Multiply Constant(ConstInt(2) None) Constant(ConstInt(3) None) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Binary(Add Constant(ConstInt(1) None) Binary(Multiply Constant(ConstInt(2) None) Constant(ConstInt(3) None) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestVarInitializer::test_compound_init_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_compound_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Binary(Add Constant(ConstInt(1) Int) Binary(Multiply Constant(ConstInt(2) Int) Constant(ConstInt(3) Int) Int) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Binary(Add Constant(ConstInt(1) None) Binary(Multiply Constant(ConstInt(2) None) Constant(ConstInt(3) None) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Binary(Add Constant(ConstInt(1) Int) Binary(Multiply Constant(ConstInt(2) Int) Constant(ConstInt(3) Int) Int) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestVarInitializer::test_constant_init_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_constant_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 5; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestVarInitializer::test_constant_init_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_constant_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestVarInitializer::test_constant_init_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestVarInitializer::test_constant_init_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStatementsAndExpressions::test_call_arg_is_type_checked (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_call_arg_is_type_checked\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int f(int a); int g(int a); return f(g(1)); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"g\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"f\" [FunctionCall(\"g\" [Constant(ConstInt(1) None)] None false)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStatementsAndExpressions::test_call_arg_is_type_checked (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_call_arg_is_type_checked\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"g\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"f\" [FunctionCall(\"g\" [Constant(ConstInt(1) None)] None false)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"g\" [\"@1.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"f\" [FunctionCall(\"g\" [Constant(ConstInt(1) None)] None false)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStatementsAndExpressions::test_call_arg_is_type_checked (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_call_arg_is_type_checked\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    D(FunctionDecl(function_decl(\\\"g\\\" [\\\"@1.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(FunctionCall(\\\"f\\\" [FunctionCall(\\\"g\\\" [Constant(ConstInt(1) Int)] Int false)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"@0.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    D(FunctionDecl(function_decl(\"g\" [\"@1.a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"f\" [FunctionCall(\"g\" [Constant(ConstInt(1) None)] None false)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    D(FunctionDecl(function_decl(\\\"g\\\" [\\\"@1.a\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false)))\\n    S(Return(FunctionCall(\\\"f\\\" [FunctionCall(\\\"g\\\" [Constant(ConstInt(1) Int)] Int false)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStatementsAndExpressions::test_call_arg_with_undeclared_inside_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_call_arg_with_undeclared_inside_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int f(int a); return f(bar()); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"f\" [FunctionCall(\"bar\" [] None false)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStatementsAndExpressions::test_call_arg_with_undeclared_inside_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_call_arg_with_undeclared_inside_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(FunctionDecl(function_decl(\"f\" [\"a\"] None FunType([Int] Int) None None [\"\"] false None [] None false)))\n"
      "    S(Return(FunctionCall(\"f\" [FunctionCall(\"bar\" [] None false)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"IdentifierResolutionError\", \"message\": \"undeclared identifier 'bar'\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStatementsAndExpressions::test_compound_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_compound_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 1; { int y = x + 1; } return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"y\" Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStatementsAndExpressions::test_compound_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_compound_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"y\" Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStatementsAndExpressions::test_compound_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_compound_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    S(Compound(Block([\\n      D(VarDecl(var_decl(\\\"@1.y\\\" Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\\n    ])))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Compound(Block([\n"
      "      D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    ])))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    S(Compound(Block([\\n      D(VarDecl(var_decl(\\\"@1.y\\\" Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\\n    ])))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStatementsAndExpressions::test_for_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_for_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { for (int i = 0; i < 10; i++) ; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(10) None) None) Postfix(Increment Var(\"i\" None) None) Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStatementsAndExpressions::test_for_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_for_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"i\" None) Constant(ConstInt(10) None) None) Postfix(Increment Var(\"i\" None) None) Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(10) None) None) Postfix(Increment Var(\"@0.i\" None) None) Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStatementsAndExpressions::test_for_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_for_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@0.i\\\" Constant(ConstInt(0) Int) Int None None None None false)]) Binary(LessThan Var(\\\"@0.i\\\" Int) Constant(ConstInt(10) Int) Int) Postfix(Increment Var(\\\"@0.i\\\" Int) Int) Null \\\"\\\" None))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(ForStmt(InitDecl([var_decl(\"@0.i\" Constant(ConstInt(0) None) Int None None None None false)]) Binary(LessThan Var(\"@0.i\" None) Constant(ConstInt(10) None) None) Postfix(Increment Var(\"@0.i\" None) None) Null \"\" None))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@0.i\\\" Constant(ConstInt(0) Int) Int None None None None false)]) Binary(LessThan Var(\\\"@0.i\\\" Int) Constant(ConstInt(10) Int) Int) Postfix(Increment Var(\\\"@0.i\\\" Int) Int) Null \\\"\\\" None))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStatementsAndExpressions::test_if_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_if_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 0; if (x) return 1; else return 2; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(IfStmt(Var(\"x\" None) Return(Constant(ConstInt(1) None)) Return(Constant(ConstInt(2) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStatementsAndExpressions::test_if_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_if_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(IfStmt(Var(\"x\" None) Return(Constant(ConstInt(1) None)) Return(Constant(ConstInt(2) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(IfStmt(Var(\"@0.x\" None) Return(Constant(ConstInt(1) None)) Return(Constant(ConstInt(2) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStatementsAndExpressions::test_if_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_if_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(IfStmt(Var(\\\"@0.x\\\" Int) Return(Constant(ConstInt(1) Int)) Return(Constant(ConstInt(2) Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(IfStmt(Var(\"@0.x\" None) Return(Constant(ConstInt(1) None)) Return(Constant(ConstInt(2) None))))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(IfStmt(Var(\\\"@0.x\\\" Int) Return(Constant(ConstInt(1) Int)) Return(Constant(ConstInt(2) Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestStatementsAndExpressions::test_while_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_while_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 0; while (x) x = x - 1; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(WhileStmt(Var(\"x\" None) Expression(Assignment(Var(\"x\" None) Binary(Subtract Var(\"x\" None) Constant(ConstInt(1) None) None) None)) \"\"))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestStatementsAndExpressions::test_while_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_while_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(WhileStmt(Var(\"x\" None) Expression(Assignment(Var(\"x\" None) Binary(Subtract Var(\"x\" None) Constant(ConstInt(1) None) None) None)) \"\"))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(WhileStmt(Var(\"@0.x\" None) Expression(Assignment(Var(\"@0.x\" None) Binary(Subtract Var(\"@0.x\" None) Constant(ConstInt(1) None) None) None)) \"\"))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestStatementsAndExpressions::test_while_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestStatementsAndExpressions::test_while_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(WhileStmt(Var(\\\"@0.x\\\" Int) Expression(Assignment(Var(\\\"@0.x\\\" Int) Binary(Subtract Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int) Int)) \\\"\\\"))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    S(WhileStmt(Var(\"@0.x\" None) Expression(Assignment(Var(\"@0.x\" None) Binary(Subtract Var(\"@0.x\" None) Constant(ConstInt(1) None) None) None)) \"\"))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(WhileStmt(Var(\\\"@0.x\\\" Int) Expression(Assignment(Var(\\\"@0.x\\\" Int) Binary(Subtract Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int) Int)) \\\"\\\"))\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_basic_switch_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_basic_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_basic_switch_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_basic_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_basic_switch_passes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_basic_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(2) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(2) None)) \".default@3\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstInt(1) None) \".case@2\")] \".default@3\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_basic_switch_passes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_basic_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Constant(ConstInt(1) Int) Return(Constant(ConstInt(1) Int)) \\\".case@2\\\"))\\n      S(DefaultStmt(Return(Constant(ConstInt(2) Int)) \\\".default@3\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(0) Int) \\\".case@1\\\") switch_case(Constant(ConstInt(1) Int) \\\".case@2\\\")] \\\".default@3\\\" Int))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstInt(1) None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "      S(DefaultStmt(Return(Constant(ConstInt(2) None)) \".default@3\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstInt(1) None) \".case@2\")] \".default@3\" None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Constant(ConstInt(1) Int) Return(Constant(ConstInt(1) Int)) \\\".case@2\\\"))\\n      S(DefaultStmt(Return(Constant(ConstInt(2) Int)) \\\".default@3\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(0) Int) \\\".case@1\\\") switch_case(Constant(ConstInt(1) Int) \\\".case@2\\\")] \\\".default@3\\\" Int))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (3) { case 5L: return 0; } return 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstLong(5) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_case_value_canonicalized_to_promoted_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstLong(5) Long) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(5) Int) \\\".case@1\\\")] None Int))\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstLong(5) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(SwitchStmt(Constant(ConstInt(3) Int) Compound(Block([\\n      S(CaseStmt(Constant(ConstLong(5) Long) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstInt(5) Int) \\\".case@1\\\")] None Int))\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_double_case_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_double_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 3; switch (a) { case 1.0: return 0; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_double_case_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_double_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_double_case_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_double_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_double_case_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_double_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstDouble(4607182418800017408) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_double_case_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_double_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstDouble(4607182418800017408) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstDouble(4607182418800017408) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"case label is not an integer constant expression: floating constant ConstDouble(bits=4607182418800017408) is not an integer constant\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (3) { case 0: return 0; case 65536: return 1; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstLong(65536) None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_duplicate_case_after_promotion_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstInt(3) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstLong(65536) None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Constant(ConstLong(65536) None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"duplicate case value 0 in switch statement (C99 \\u00a76.8.4.2.3)\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_long_switch_passes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_long_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long x = 5; switch (x) { case 0L: return 0; case 5L: return 1; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_long_switch_passes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_long_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_long_switch_passes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_long_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_long_switch_passes (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_long_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstLong(0) None) \".case@1\") switch_case(Constant(ConstLong(5) None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_long_switch_passes (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_long_switch_passes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(SwitchStmt(Var(\\\"@0.x\\\" Long) Compound(Block([\\n      S(CaseStmt(Constant(ConstLong(0) Long) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Constant(ConstLong(5) Long) Return(Constant(ConstInt(1) Int)) \\\".case@2\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstLong(0) Long) \\\".case@1\\\") switch_case(Constant(ConstLong(5) Long) \\\".case@2\\\")] None Long))\\n    S(Return(Constant(ConstInt(9) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstLong(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Constant(ConstLong(5) None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstLong(0) None) \".case@1\") switch_case(Constant(ConstLong(5) None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(SwitchStmt(Var(\\\"@0.x\\\" Long) Compound(Block([\\n      S(CaseStmt(Constant(ConstLong(0) Long) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n      S(CaseStmt(Constant(ConstLong(5) Long) Return(Constant(ConstInt(1) Int)) \\\".case@2\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstLong(0) Long) \\\".case@1\\\") switch_case(Constant(ConstLong(5) Long) \\\".case@2\\\")] None Long))\\n    S(Return(Constant(ConstInt(9) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_non_constant_case_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_non_constant_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 3; switch (a) { case 0: return 0; case a: return 1; } return 9; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"a\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_non_constant_case_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_non_constant_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"a\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"@0.a\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_non_constant_case_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_non_constant_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"@0.a\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"@0.a\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_non_constant_case_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_non_constant_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "      S(CaseStmt(Var(\"@0.a\" None) Return(Constant(ConstInt(1) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Var(\"@0.a\" None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Var(\"@0.a\" None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_non_constant_case_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_non_constant_case_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(3) None) Int None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.a\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "      S(CaseStmt(Var(\"@0.a\" None) Return(Constant(ConstInt(1) None)) \".case@2\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\") switch_case(Var(\"@0.a\" None) \".case@2\")] None None))\n"
      "    S(Return(Constant(ConstInt(9) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"case label is not an integer constant expression: expression is not an integer constant expression: Var(name='@0.a', data_type=Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_promoted_type_stamped (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_promoted_type_stamped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long x = 5; switch (x) { case 0: return 0; } return 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_promoted_type_stamped (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_promoted_type_stamped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_promoted_type_stamped (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_promoted_type_stamped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_promoted_type_stamped (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_promoted_type_stamped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_promoted_type_stamped (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_promoted_type_stamped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(SwitchStmt(Var(\\\"@0.x\\\" Long) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstLong(0) Long) \\\".case@1\\\")] None Long))\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.x\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(SwitchStmt(Var(\\\"@0.x\\\" Long) Compound(Block([\\n      S(CaseStmt(Constant(ConstInt(0) Int) Return(Constant(ConstInt(0) Int)) \\\".case@1\\\"))\\n    ])) \\\".switch@0\\\" [switch_case(Constant(ConstLong(0) Long) \\\".case@1\\\")] None Long))\\n    S(Return(Constant(ConstInt(1) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_switch_on_double_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { switch (3.0) { case 0: return 0; } }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_switch_on_double_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_switch_on_double_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_switch_on_double_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_switch_on_double_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(SwitchStmt(Constant(ConstDouble(4613937818241073152) None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"switch controlling expression must have integer type (got Double()); C99 \\u00a76.8.4.2.1\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int *p = 0; switch (p) { case 0: return 0; } return 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \"\"))\n"
      "    ])) \"\" [] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestSwitch::test_switch_on_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Constant(ConstInt(0) None) Pointer(Int) None None None None false)))\n"
      "    S(SwitchStmt(Var(\"@0.p\" None) Compound(Block([\n"
      "      S(CaseStmt(Constant(ConstInt(0) None) Return(Constant(ConstInt(0) None)) \".case@1\"))\n"
      "    ])) \".switch@0\" [switch_case(Constant(ConstInt(0) None) \".case@1\")] None None))\n"
      "    S(Return(Constant(ConstInt(1) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"switch controlling expression must have integer type (got Pointer(referenced_type=Int())); C99 \\u00a76.8.4.2.1\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_assignment_narrows_rval_with_implicit_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_narrows_rval_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; long b = (long)200; a = b; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(200) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Var(\"b\" None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_assignment_narrows_rval_with_implicit_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_narrows_rval_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(200) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Var(\"b\" None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(200) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_assignment_narrows_rval_with_implicit_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_narrows_rval_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(200) Int) Long) Long None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@0.a\\\" Int) Cast(Int Var(\\\"@1.b\\\" Long) Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(200) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(200) Int) Long) Long None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@0.a\\\" Int) Cast(Int Var(\\\"@1.b\\\" Long) Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_assignment_no_cast_when_types_match (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int b = 1; a = b; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Var(\"b\" None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_assignment_no_cast_when_types_match (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Var(\"b\" None) None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_assignment_no_cast_when_types_match (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_assignment_widens_rval_with_implicit_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_widens_rval_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; long b = (long)0; b = a; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"b\" None) Var(\"a\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_assignment_widens_rval_with_implicit_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_widens_rval_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"b\" None) Var(\"a\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@1.b\" None) Var(\"@0.a\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_assignment_widens_rval_with_implicit_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_assignment_widens_rval_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(0) Int) Long) Long None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@1.b\\\" Long) Cast(Long Var(\\\"@0.a\\\" Int) Long) Long)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@1.b\" None) Var(\"@0.a\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(0) Int) Long) Long None None None None false)))\\n    S(Expression(Assignment(Var(\\\"@1.b\\\" Long) Cast(Long Var(\\\"@0.a\\\" Int) Long) Long)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_binary_promotion_inserts_implicit_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_binary_promotion_inserts_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 1; long b = (long)2; return (long)0 + (a + b); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Cast(Long Constant(ConstInt(0) None) None) Binary(Add Var(\"a\" None) Var(\"b\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_binary_promotion_inserts_implicit_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_binary_promotion_inserts_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Cast(Long Constant(ConstInt(0) None) None) Binary(Add Var(\"a\" None) Var(\"b\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Cast(Long Constant(ConstInt(0) None) None) Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_binary_promotion_inserts_implicit_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_binary_promotion_inserts_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Binary(Add Cast(Long Constant(ConstInt(0) Int) Long) Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Cast(Long Constant(ConstInt(0) None) None) Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Binary(Add Cast(Long Constant(ConstInt(0) Int) Long) Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_call_arg_narrows_with_implicit_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_narrows_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int x); int main(void) { long y = (long)1; return foo(y); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"y\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"y\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_call_arg_narrows_with_implicit_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_narrows_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"y\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"y\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"@1.y\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_call_arg_narrows_with_implicit_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_narrows_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.x\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [Cast(Int Var(\\\"@1.y\\\" Long) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@1.y\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"@1.y\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.x\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [Cast(Int Var(\\\"@1.y\\\" Long) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_call_arg_no_cast_when_types_match (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int x); int main(void) { return foo(5); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(5) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_call_arg_no_cast_when_types_match (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(5) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(5) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_call_arg_no_cast_when_types_match (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.x\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [Constant(ConstInt(5) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(5) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.x\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [Constant(ConstInt(5) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_call_arg_widens_with_implicit_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_widens_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(long x); int main(void) { return foo(1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Long] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_call_arg_widens_with_implicit_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_widens_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"x\"] None FunType([Long] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] None FunType([Long] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_call_arg_widens_with_implicit_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arg_widens_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.x\\\"] None FunType([Long] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [Cast(Long Constant(ConstInt(1) Int) Long)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.x\"] None FunType([Long] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.x\\\"] None FunType([Long] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"foo\\\" [Cast(Long Constant(ConstInt(1) Int) Long)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Long] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_call_arity_mismatch_still_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arity_mismatch_still_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a, int b); int main(void) { return foo(1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_call_arity_mismatch_still_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arity_mismatch_still_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_call_arity_mismatch_still_raises (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_call_arity_mismatch_still_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Int] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(FunctionCall(\"foo\" [Constant(ConstInt(1) None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"function 'foo' called with 1 argument, expected 2\"}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_cast_target_must_be_object_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_cast_target_must_be_object_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(FunType([] Int) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(FunType([] Int) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_cast_target_must_be_object_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_cast_target_must_be_object_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(FunType([] Int) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cast target type must be an object type (Int / Long / LongLong / UInt / ULong / ULongLong / Float / Double / Pointer) or void, got FunType(params=[], ret=Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_comparison_on_longs_returns_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_comparison_on_longs_returns_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long a = (long)1; long b = (long)2; return a == b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_comparison_on_longs_returns_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_comparison_on_longs_returns_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_comparison_on_longs_returns_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_comparison_on_longs_returns_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Binary(Equal Var(\\\"@0.a\\\" Long) Var(\\\"@1.b\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Binary(Equal Var(\\\"@0.a\\\" Long) Var(\\\"@1.b\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_comparison_with_promotion_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_comparison_with_promotion_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; long b = (long)2; return a == b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_comparison_with_promotion_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_comparison_with_promotion_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_comparison_with_promotion_yields_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_comparison_with_promotion_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Binary(Equal Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Equal Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Binary(Equal Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_compound_assignment_to_int_promotes_intermediate_to_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_compound_assignment_to_int_promotes_intermediate_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; long b = (long)2; a += b; return a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" None) Var(\"b\" None) None None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_compound_assignment_to_int_promotes_intermediate_to_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_compound_assignment_to_int_promotes_intermediate_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"a\" None) Var(\"b\" None) None None)))\n"
      "    S(Return(Var(\"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_compound_assignment_to_int_promotes_intermediate_to_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_compound_assignment_to_int_promotes_intermediate_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Expression(CompoundAssignment(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Long) Long Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None None)))\n"
      "    S(Return(Var(\"@0.a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Expression(CompoundAssignment(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Long) Long Int)))\\n    S(Return(Var(\\\"@0.a\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_conditional_branches_promote_to_common_type (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_conditional_branches_promote_to_common_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 1; long b = (long)2; return 1 \077 a : b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_conditional_branches_promote_to_common_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_conditional_branches_promote_to_common_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_conditional_branches_promote_to_common_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_conditional_branches_promote_to_common_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Conditional(Constant(ConstInt(1) Int) Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Conditional(Constant(ConstInt(1) None) Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Conditional(Constant(ConstInt(1) Int) Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_data_type_set_on_constant_var_unary_postfix (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_data_type_set_on_constant_var_unary_postfix\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 5; a++; return -a; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"a\" None) None)))\n"
      "    S(Return(Unary(Negate Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_data_type_set_on_constant_var_unary_postfix (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_data_type_set_on_constant_var_unary_postfix\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"a\" None) None)))\n"
      "    S(Return(Unary(Negate Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.a\" None) None)))\n"
      "    S(Return(Unary(Negate Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_data_type_set_on_constant_var_unary_postfix (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_data_type_set_on_constant_var_unary_postfix\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.a\\\" Int) Int)))\\n    S(Return(Unary(Negate Var(\\\"@0.a\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.a\" None) None)))\n"
      "    S(Return(Unary(Negate Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.a\\\" Int) Int)))\\n    S(Return(Unary(Negate Var(\\\"@0.a\\\" Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_file_scope_long_init_with_int_literal (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_file_scope_long_init_with_int_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long x = 5; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_file_scope_long_init_with_int_literal (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_file_scope_long_init_with_int_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_file_scope_long_init_with_int_literal (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_file_scope_long_init_with_int_literal\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_function_param_types (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_function_param_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a, long b); int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Long] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_function_param_types (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_function_param_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\" \"b\"] None FunType([Int Long] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Long] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_function_param_types (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_function_param_types\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([Int Long] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\" \"@1.b\"] None FunType([Int Long] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([Int Long] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Long] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_function_return_type_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_function_return_type_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long foo(void); int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] None FunType([] Long) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_function_return_type_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_function_return_type_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] None FunType([] Long) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] None FunType([] Long) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_function_return_type_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_function_return_type_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Long) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [] None FunType([] Long) None None [] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [] None FunType([] Long) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_init_no_cast_when_types_match (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_init_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long x = (long)5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_init_no_cast_when_types_match (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_init_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_init_no_cast_when_types_match (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_init_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_int_init_with_long_literal_inserts_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_init_with_long_literal_inserts_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = 100000; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstLong(100000) None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_int_init_with_long_literal_inserts_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_init_with_long_literal_inserts_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstLong(100000) None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstLong(100000) None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_int_init_with_long_literal_inserts_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_init_with_long_literal_inserts_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Int Constant(ConstLong(100000) Long) Int) Int None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstLong(100000) None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Int Constant(ConstLong(100000) Long) Int) Int None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_int_long_addition_promotes_to_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_long_addition_promotes_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; long b = (long)2; return a + b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_int_long_addition_promotes_to_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_long_addition_promotes_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_int_long_addition_promotes_to_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_long_addition_promotes_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_int_long_addition_with_outer_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_long_addition_with_outer_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; long b = (long)2; return (int)(a + b); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"a\" None) Var(\"b\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_int_long_addition_with_outer_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_long_addition_with_outer_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"a\" None) Var(\"b\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_int_long_addition_with_outer_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_int_long_addition_with_outer_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(2) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(1) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Cast(Long Constant(ConstInt(2) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(Add Cast(Long Var(\\\"@0.a\\\" Int) Long) Var(\\\"@1.b\\\" Long) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_logical_not_long_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_logical_not_long_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { return !((long)1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(LogicalNot Cast(Long Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_logical_not_long_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_logical_not_long_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(LogicalNot Cast(Long Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(LogicalNot Cast(Long Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_logical_not_long_yields_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_logical_not_long_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(LogicalNot Cast(Long Constant(ConstInt(1) Int) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Unary(LogicalNot Cast(Long Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Unary(LogicalNot Cast(Long Constant(ConstInt(1) Int) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_logical_not_returns_int_for_long_operand (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_logical_not_returns_int_for_long_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long x = (long)1; return !x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Unary(LogicalNot Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_logical_not_returns_int_for_long_operand (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_logical_not_returns_int_for_long_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Unary(LogicalNot Var(\"x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Unary(LogicalNot Var(\"@0.x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_logical_not_returns_int_for_long_operand (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_logical_not_returns_int_for_long_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(Unary(LogicalNot Var(\\\"@0.x\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Unary(LogicalNot Var(\"@0.x\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(Unary(LogicalNot Var(\\\"@0.x\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_long_init_with_int_literal_inserts_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_init_with_int_literal_inserts_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long x = 5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_long_init_with_int_literal_inserts_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_init_with_int_literal_inserts_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_long_init_with_int_literal_inserts_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_init_with_int_literal_inserts_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_long_local_is_recorded_as_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_local_is_recorded_as_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long x = (long)5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_long_local_is_recorded_as_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_local_is_recorded_as_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_long_local_is_recorded_as_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_local_is_recorded_as_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_long_long_addition_promotes_to_long_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_long_addition_promotes_to_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long long main(void) { long a = (long)1; long long b = 2LL; return a + b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_long_long_addition_promotes_to_long_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_long_addition_promotes_to_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_long_long_addition_promotes_to_long_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_long_addition_promotes_to_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstLongLong(2) LongLong) LongLong None None None None false)))\\n    S(Return(Binary(Add Cast(LongLong Var(\\\"@0.a\\\" Long) LongLong) Var(\\\"@1.b\\\" LongLong) LongLong)))\\n  ]) FunType([] LongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstLongLong(2) LongLong) LongLong None None None None false)))\\n    S(Return(Binary(Add Cast(LongLong Var(\\\"@0.a\\\" Long) LongLong) Var(\\\"@1.b\\\" LongLong) LongLong)))\\n  ]) FunType([] LongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] LongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_long_long_plus_uint_promotes_to_long_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_long_plus_uint_promotes_to_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long long main(void) { unsigned int a = 1U; long long b = 2LL; return a + b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstUInt(1) None) UInt None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_long_long_plus_uint_promotes_to_long_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_long_plus_uint_promotes_to_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstUInt(1) None) UInt None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstUInt(1) None) UInt None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_long_long_plus_uint_promotes_to_long_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_long_plus_uint_promotes_to_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstUInt(1) UInt) UInt None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstLongLong(2) LongLong) LongLong None None None None false)))\\n    S(Return(Binary(Add Cast(LongLong Var(\\\"@0.a\\\" UInt) LongLong) Var(\\\"@1.b\\\" LongLong) LongLong)))\\n  ]) FunType([] LongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstUInt(1) None) UInt None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstLongLong(2) None) LongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] LongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstUInt(1) UInt) UInt None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstLongLong(2) LongLong) LongLong None None None None false)))\\n    S(Return(Binary(Add Cast(LongLong Var(\\\"@0.a\\\" UInt) LongLong) Var(\\\"@1.b\\\" LongLong) LongLong)))\\n  ]) FunType([] LongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] LongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_long_plus_unsigned_long_long_promotes_to_unsigned_long_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_plus_unsigned_long_long_promotes_to_unsigned_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned long long main(void) { long a = (long)1; unsigned long long b = 2ULL; return a + b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstULongLong(2) None) ULongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_long_plus_unsigned_long_long_promotes_to_unsigned_long_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_plus_unsigned_long_long_promotes_to_unsigned_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstULongLong(2) None) ULongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstULongLong(2) None) ULongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_long_plus_unsigned_long_long_promotes_to_unsigned_long_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_plus_unsigned_long_long_promotes_to_unsigned_long_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstULongLong(2) ULongLong) ULongLong None None None None false)))\\n    S(Return(Binary(Add Cast(ULongLong Var(\\\"@0.a\\\" Long) ULongLong) Var(\\\"@1.b\\\" ULongLong) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstULongLong(2) None) ULongLong None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] ULongLong) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" Constant(ConstULongLong(2) ULongLong) ULongLong None None None None false)))\\n    S(Return(Binary(Add Cast(ULongLong Var(\\\"@0.a\\\" Long) ULongLong) Var(\\\"@1.b\\\" ULongLong) ULongLong)))\\n  ]) FunType([] ULongLong) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] ULongLong)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_long_variable_is_recorded_as_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_variable_is_recorded_as_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long x; int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_long_variable_is_recorded_as_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_long_variable_is_recorded_as_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"x\" None Long None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_no_implicit_cast_when_operands_already_match (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_no_implicit_cast_when_operands_already_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; int b = 2; return a + b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_no_implicit_cast_when_operands_already_match (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_no_implicit_cast_when_operands_already_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Constant(ConstInt(2) None) Int None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_return_narrows_value_with_implicit_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_return_narrows_value_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long x = (long)5; return x; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_return_narrows_value_with_implicit_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_return_narrows_value_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Var(\"x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_return_narrows_value_with_implicit_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_return_narrows_value_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@0.x\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long None None None None false)))\n"
      "    S(Return(Var(\"@0.x\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Var(\\\"@0.x\\\" Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_return_no_cast_when_types_match (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_return_no_cast_when_types_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_return_widens_value_with_implicit_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_return_widens_value_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { return 5; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_return_widens_value_with_implicit_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_return_widens_value_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_return_widens_value_with_implicit_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_return_widens_value_with_implicit_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Long Constant(ConstInt(5) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(5) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Long Constant(ConstInt(5) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_static_init_with_non_constant_raises (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_init_with_non_constant_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 1; static int x = a; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"x\" Var(\"a\" None) Int Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_static_init_with_non_constant_raises (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_init_with_non_constant_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"x\" Var(\"a\" None) Int Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.x\" Var(\"@0.a\" None) Int Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_static_init_with_non_constant_raises (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_init_with_non_constant_raises\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(1) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.x\" Var(\"@0.a\" None) Int Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object '@1.x' is not a constant expression\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_static_long_init_with_int_literal_converts (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_long_init_with_int_literal_converts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static long x = 5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_static_long_init_with_int_literal_converts (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_long_init_with_int_literal_converts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_static_long_init_with_int_literal_converts (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_long_init_with_int_literal_converts\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestLongAndCasts::test_static_long_with_explicit_cast_initializer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_long_with_explicit_cast_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static long x = (long)5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestLongAndCasts::test_static_long_with_explicit_cast_initializer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_long_with_explicit_cast_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Cast(Long Constant(ConstInt(5) None) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestLongAndCasts::test_static_long_with_explicit_cast_initializer (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestLongAndCasts::test_static_long_with_explicit_cast_initializer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Cast(Long Constant(ConstInt(5) None) None) Long Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Cast(Long Constant(ConstInt(5) Int) Long) Long Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_int_minus_pointer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_int_minus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(1 - p); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(1) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_int_minus_pointer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_int_minus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(1) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(1) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_int_minus_pointer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_int_minus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(1) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '-' between integer and pointer is not defined (Int() - Pointer(referenced_type=Int()))\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_int_operand_widened_to_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_int_operand_widened_to_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(p + 1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_int_operand_widened_to_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_int_operand_widened_to_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_int_plus_pointer_yields_pointer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_int_plus_pointer_yields_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(1 + p); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(1) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_int_plus_pointer_yields_pointer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_int_plus_pointer_yields_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(1) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(1) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_long_operand_truncated_to_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_long_operand_truncated_to_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; long n = (long)1; return (long)(p + n); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"n\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Var(\"n\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_long_operand_truncated_to_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_long_operand_truncated_to_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"n\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Var(\"n\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.n\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Var(\"@2.n\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_long_operand_truncated_to_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_long_operand_truncated_to_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.n\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Cast(Int Var(\\\"@2.n\\\" Long) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.n\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Var(\"@2.n\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.n\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Cast(Int Var(\\\"@2.n\\\" Long) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_distinct_pointer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_distinct_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; long b = (long)0; int *p = &a; long *q = &b; return p - q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_distinct_pointer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_distinct_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.q\" AddressOf(Var(\"@1.b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"@2.p\" None) Var(\"@3.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_distinct_pointer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_distinct_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.q\" AddressOf(Var(\"@1.b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"@2.p\" None) Var(\"@3.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"subtraction of distinct pointer types: Pointer(referenced_type=Int()) - Pointer(referenced_type=Long())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_double_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(p - 1.0); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_double_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_double_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_double_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '-' is not defined on a pointer and a floating-point operand (Pointer(referenced_type=Int()) - Double())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_int_yields_pointer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_int_yields_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(p - 1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_int_yields_pointer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_int_yields_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_pointer_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; int *q = &a; return p - q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_pointer_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_zero_accepted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_zero_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(p - 0); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_zero_accepted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_zero_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_pointer_minus_zero_accepted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_minus_zero_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(0) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Var(\"@1.p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(0) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_float_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_float_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(p + 1.0); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_float_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_float_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_float_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_float_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstDouble(4607182418800017408) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '+' is not defined on a pointer and a floating-point operand (Pointer(referenced_type=Int()) + Double())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_int_yields_pointer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_int_yields_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(p + 1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_int_yields_pointer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_int_yields_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_pointer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int *p = &a; int *q = &a; return (int)(p + q); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"p\" None) Var(\"q\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_pointer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"p\" None) Var(\"q\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"@1.p\" None) Var(\"@2.q\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_pointer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(Add Var(\"@1.p\" None) Var(\"@2.q\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '+' is not defined on two pointer operands (Pointer(referenced_type=Int()) + Pointer(referenced_type=Int()))\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_zero_accepted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_zero_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(p + 0); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_zero_accepted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_zero_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_pointer_plus_zero_accepted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_plus_zero_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(0) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(0) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(0) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_pointer_to_long_arithmetic_yields_pointer_to_long (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_to_long_arithmetic_yields_pointer_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { long a = (long)0; long *p = &a; return (long)(p + 1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_pointer_to_long_arithmetic_yields_pointer_to_long (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_to_long_arithmetic_yields_pointer_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_pointer_to_long_arithmetic_yields_pointer_to_long (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_pointer_to_long_arithmetic_yields_pointer_to_long\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(0) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Long) Pointer(Long)) Pointer(Long) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Long)) Constant(ConstInt(1) Int) Pointer(Long)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(0) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Long) Pointer(Long)) Pointer(Long) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Var(\\\"@1.p\\\" Pointer(Long)) Constant(ConstInt(1) Int) Pointer(Long)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_zero_minus_pointer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_zero_minus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(0 - p); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(0) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_zero_minus_pointer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_zero_minus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(0) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(0) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_zero_minus_pointer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_zero_minus_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Subtract Constant(ConstInt(0) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '-' between integer and pointer is not defined (Int() - Pointer(referenced_type=Int()))\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerArithmetic::test_zero_plus_pointer_accepted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_zero_plus_pointer_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a = 0; int *p = &a; return (long)(0 + p); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(0) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerArithmetic::test_zero_plus_pointer_accepted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_zero_plus_pointer_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(0) None) Var(\"p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(0) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerArithmetic::test_zero_plus_pointer_accepted (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerArithmetic::test_zero_plus_pointer_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Constant(ConstInt(0) Int) Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Constant(ConstInt(0) None) Var(\"@1.p\" None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Binary(Add Constant(ConstInt(0) Int) Var(\\\"@1.p\\\" Pointer(Int)) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_array_decay_in_ordering (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_array_decay_in_ordering\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[10]; int b[10]; return a < b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_array_decay_in_ordering (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_array_decay_in_ordering\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_array_decay_in_ordering (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_array_decay_in_ordering\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Binary(LessThan AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) AddressOf(Var(\\\"@1.b\\\" Array(Int 10)) Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Binary(LessThan AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) AddressOf(Var(\\\"@1.b\\\" Array(Int 10)) Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_distinct_pointer_types_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_distinct_pointer_types_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; long b = (long)0; int *p = &a; long *q = &b; return p < q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_distinct_pointer_types_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_distinct_pointer_types_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.q\" AddressOf(Var(\"@1.b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@2.p\" None) Var(\"@3.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_distinct_pointer_types_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_distinct_pointer_types_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@3.q\" AddressOf(Var(\"@1.b\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@2.p\" None) Var(\"@3.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"comparison of distinct pointer types: Pointer(referenced_type=Int()) < Pointer(referenced_type=Long())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_int_vs_pointer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_int_vs_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int *p = &a; return 0 < p; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Constant(ConstInt(0) None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_int_vs_pointer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_int_vs_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Constant(ConstInt(0) None) Var(\"p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Constant(ConstInt(0) None) Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_int_vs_pointer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_int_vs_pointer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Constant(ConstInt(0) None) Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '<' between pointer and non-pointer operand: Int() < Pointer(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_pointer_ge_pointer_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_ge_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int *p = &a; int *q = &a; return p >= q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_pointer_ge_pointer_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_ge_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_pointer_ge_pointer_yields_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_ge_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(GreaterOrEqual Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterOrEqual Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(GreaterOrEqual Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_pointer_gt_pointer_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_gt_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int *p = &a; int *q = &a; return p > q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_pointer_gt_pointer_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_gt_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterThan Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_pointer_gt_pointer_yields_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_gt_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(GreaterThan Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(GreaterThan Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(GreaterThan Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_pointer_le_pointer_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_le_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int *p = &a; int *q = &a; return p <= q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessOrEqual Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_pointer_le_pointer_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_le_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessOrEqual Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessOrEqual Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_pointer_le_pointer_yields_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_le_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(LessOrEqual Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessOrEqual Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(LessOrEqual Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_pointer_lt_pointer_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_lt_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int *p = &a; int *q = &a; return p < q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_pointer_lt_pointer_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_lt_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_pointer_lt_pointer_yields_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_lt_pointer_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(LessThan Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Binary(LessThan Var(\\\"@1.p\\\" Pointer(Int)) Var(\\\"@2.q\\\" Pointer(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_pointer_to_long_ordering (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_to_long_ordering\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long a = (long)0; long *p = &a; long *q = &a; return p < q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Long) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_pointer_to_long_ordering (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_to_long_ordering\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Long) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Var(\"a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Long) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_pointer_to_long_ordering (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_to_long_ordering\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(0) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Long) Pointer(Long)) Pointer(Long) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Long) Pointer(Long)) Pointer(Long) None None None None false)))\\n    S(Return(Binary(LessThan Var(\\\"@1.p\\\" Pointer(Long)) Var(\\\"@2.q\\\" Pointer(Long)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Cast(Long Constant(ConstInt(0) None) None) Long None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Long) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Var(\"@0.a\" None) None) Pointer(Long) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@1.p\" None) Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" Cast(Long Constant(ConstInt(0) Int) Long) Long None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Long) Pointer(Long)) Pointer(Long) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Var(\\\"@0.a\\\" Long) Pointer(Long)) Pointer(Long) None None None None false)))\\n    S(Return(Binary(LessThan Var(\\\"@1.p\\\" Pointer(Long)) Var(\\\"@2.q\\\" Pointer(Long)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestPointerOrdering::test_pointer_vs_int_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_vs_int_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a = 0; int *p = &a; return p < 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestPointerOrdering::test_pointer_vs_int_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_vs_int_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@1.p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestPointerOrdering::test_pointer_vs_int_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestPointerOrdering::test_pointer_vs_int_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Binary(LessThan Var(\"@1.p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '<' between pointer and non-pointer operand: Pointer(referenced_type=Int()) < Int()\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_address_of_array_subscript (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_array_subscript\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int arr[10]; int *q = &arr[3]; return (long)q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"arr\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"arr\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_address_of_array_subscript (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_array_subscript\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"arr\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"arr\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.q\" AddressOf(Subscript(Var(\"@0.arr\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_address_of_array_subscript (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_array_subscript\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.q\\\" AddressOf(Subscript(AddressOf(Var(\\\"@0.arr\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Var(\\\"@1.q\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.arr\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.q\" AddressOf(Subscript(Var(\"@0.arr\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.arr\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.q\\\" AddressOf(Subscript(AddressOf(Var(\\\"@0.arr\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Cast(Long Var(\\\"@1.q\\\" Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_address_of_array_typed_as_pointer_to_array (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_array_typed_as_pointer_to_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[10]; int (*p)[10] = &a; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Array(Int 10)) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_address_of_array_typed_as_pointer_to_array (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_array_typed_as_pointer_to_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Array(Int 10)) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Array(Int 10)) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_address_of_array_typed_as_pointer_to_array (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_array_typed_as_pointer_to_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Array(Int 10))) Pointer(Array(Int 10)) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Array(Int 10)) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Array(Int 10))) Pointer(Array(Int 10)) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(Int 10))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_address_of_subscript_yields_pointer_to_element (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_subscript_yields_pointer_to_element\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int x = 0; int *p = &x; int *q = &p[3]; return (long)q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"p\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_address_of_subscript_yields_pointer_to_element (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_address_of_subscript_yields_pointer_to_element\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"p\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(0) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.x\" None) None) Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.q\" AddressOf(Subscript(Var(\"@1.p\" None) Constant(ConstInt(3) None) None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Long Var(\"@2.q\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_array_decl_records_array_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_array_decl_records_array_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_array_decl_records_array_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_array_decl_records_array_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_array_init_list_stamps_data_type_and_converts_items (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_array_init_list_stamps_data_type_and_converts_items\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_array_init_list_widens_items_with_cast (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_array_init_list_widens_items_with_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long a[2] = {1, 2}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_array_init_list_widens_items_with_cast (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_array_init_list_widens_items_with_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_array_init_list_widens_items_with_cast (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_array_init_list_widens_items_with_cast\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long)] Array(Long 2) None) Array(Long 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Long 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long)] Array(Long 2) None) Array(Long 2) None None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_array_minus_array_yields_int (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_array_minus_array_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a[10]; int b[10]; return a - b; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_array_minus_array_yields_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_array_minus_array_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"a\" None) Var(\"b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_array_minus_array_yields_int (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_array_minus_array_yields_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) AddressOf(Var(\\\"@1.b\\\" Array(Int 10)) Pointer(Int)) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Binary(Subtract Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.b\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Cast(Long Binary(Subtract AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) AddressOf(Var(\\\"@1.b\\\" Array(Int 10)) Pointer(Int)) Int) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_array_param_adjusts_to_pointer_in_symbol_table (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_array_param_adjusts_to_pointer_in_symbol_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a[3]) { return a[0]; } int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_array_param_adjusts_to_pointer_in_symbol_table (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_array_param_adjusts_to_pointer_in_symbol_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_array_param_adjusts_to_pointer_in_symbol_table (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_array_param_adjusts_to_pointer_in_symbol_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@0.a\\\" Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@0.a\\\" Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Int)] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_array_param_decl_compatible_with_pointer_param_def (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_array_param_decl_compatible_with_pointer_param_def\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a[3]); int foo(int *a) { return a[0]; } int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_array_param_decl_compatible_with_pointer_param_def (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_array_param_decl_compatible_with_pointer_param_def\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] None FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@1.a\"] Block([\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_array_param_decl_compatible_with_pointer_param_def (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_array_param_decl_compatible_with_pointer_param_def\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] None FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@1.a\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@1.a\\\" Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] None FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@1.a\"] Block([\n"
      "    S(Return(Subscript(Var(\"@1.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] None FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@1.a\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@1.a\\\" Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Int)] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_assigning_to_array_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_assigning_to_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[10]; int b[10]; a = b; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Var(\"b\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_assigning_to_array_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_assigning_to_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"b\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"a\" None) Var(\"b\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_assigning_to_array_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_assigning_to_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.b\" None Array(Int 10) None None None None false)))\n"
      "    S(Expression(Assignment(Var(\"@0.a\" None) Var(\"@1.b\" None) None)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to an array (use subscript): Var(name='@0.a', data_type=Array(element_type=Int(), size=10))\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_extern_array_redeclares_file_scope_array (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_redeclares_file_scope_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {1, 2, 3}; int main(void) { extern int a[3]; return a[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_extern_array_redeclares_file_scope_array (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_redeclares_file_scope_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_extern_array_redeclares_file_scope_array (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_redeclares_file_scope_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Array(Int 3) Extern None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Array(Int 3) Extern None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 3]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_extern_array_size_mismatch_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_size_mismatch_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3]; int main(void) { extern int a[5]; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 5) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_extern_array_size_mismatch_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_size_mismatch_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 5) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 5) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_extern_array_size_mismatch_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_size_mismatch_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 5) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"incompatible redeclaration of 'a': previous Array(element_type=Int(), size=3), new Array(element_type=Int(), size=5)\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_extern_array_with_initializer_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_with_initializer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { extern int a[3] = {1,2,3}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_extern_array_with_initializer_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_with_initializer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_extern_array_with_initializer_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_with_initializer_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"block-scope `extern` declaration of 'a' may not have an initializer\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_extern_array_with_no_prior_creates_external_symbol (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_with_no_prior_creates_external_symbol\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { extern int a[10]; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_extern_array_with_no_prior_creates_external_symbol (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_with_no_prior_creates_external_symbol\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_extern_array_with_no_prior_creates_external_symbol (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_array_with_no_prior_creates_external_symbol\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Array(Int 10) Extern None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) Extern None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Array(Int 10) Extern None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_extern_unsized_array_at_block_scope (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_unsized_array_at_block_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { extern int a[]; return a[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_extern_unsized_array_at_block_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_unsized_array_at_block_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_extern_unsized_array_at_block_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_unsized_array_at_block_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Array(Int 0) Extern None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 0)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"a\\\" None Array(Int 0) Extern None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 0)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 0)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_extern_unsized_array_at_file_scope (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_unsized_array_at_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_extern_unsized_array_at_file_scope (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_extern_unsized_array_at_file_scope\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None Array(Int 0) Extern None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 0)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_file_scope_array_with_init_list (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_file_scope_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[3] = {10, 20, 30}; int main(void) { return a[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(10) None) Constant(ConstInt(20) None) Constant(ConstInt(30) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_file_scope_array_with_init_list (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_file_scope_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(10) None) Constant(ConstInt(20) None) Constant(ConstInt(30) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(10) None) Constant(ConstInt(20) None) Constant(ConstInt(30) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_file_scope_array_with_init_list (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_file_scope_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(10) Int) Constant(ConstInt(20) Int) Constant(ConstInt(30) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(10) None) Constant(ConstInt(20) None) Constant(ConstInt(30) None)] None None) Array(Int 3) None None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" InitList([Constant(ConstInt(10) Int) Constant(ConstInt(20) Int) Constant(ConstInt(30) Int)] Array(Int 3) None) Array(Int 3) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Subscript(AddressOf(Var(\\\"a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [10, 20, 30]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[2][3] = {1,2,3,4,5,6}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None) Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for array '@0.a': 6 given, array has 2 elements\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_short_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_short_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[3][3] = {1, 2, 3}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Array(Int 3) 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_short_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_short_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Array(Int 3) 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Array(Int 3) 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_short_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_flat_init_for_multi_dim_short_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Array(Int 3) 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"expected nested initializer (`{...}`) at index 0 of '@0.a'; element type is Array(element_type=Int(), size=3)\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_init_list_for_scalar_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_init_list_for_scalar_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int x = {1, 2}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_init_list_for_scalar_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_init_list_for_scalar_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_init_list_for_scalar_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_init_list_for_scalar_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Int None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for scalar variable 'x': 2 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_init_list_in_expression_position_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_init_list_in_expression_position_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_init_list_in_expression_position_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_init_list_in_expression_position_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"brace-enclosed initializer (`{...}`) is only valid as a variable initializer for an array\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_nested_init_extra_braces_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_nested_init_extra_braces_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[3] = {{1,2,3}}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_nested_init_extra_braces_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_nested_init_extra_braces_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_nested_init_extra_braces_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_nested_init_extra_braces_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None)] None None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for element 0 of 'a': 3 given, and a scalar's braces hold one expression (C23 \\u00a76.7.11p12)\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_nested_init_list_accepted_for_multi_dim (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_nested_init_list_accepted_for_multi_dim\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[2][3] = {{1,2,3},{4,5,6}}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_nested_init_list_accepted_for_multi_dim (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_nested_init_list_accepted_for_multi_dim\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) InitList([Constant(ConstInt(4) None) Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 3) 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_block_scope_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_block_scope_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[]; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_block_scope_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_block_scope_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_block_scope_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_block_scope_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 0) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"object '@0.a': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_file_scope_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_file_scope_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[];\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_file_scope_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_file_scope_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_file_scope_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_non_extern_unsized_array_at_file_scope_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"file-scope object 'a': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_passing_array_to_array_param_decays (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_passing_array_to_array_param_decays\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int foo(int a[3]) { return a[0]; } int main(void) { int arr[3]; return foo(arr); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"arr\" None Array(Int 3) None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"arr\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_passing_array_to_array_param_decays (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_passing_array_to_array_param_decays\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"a\"] Block([\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"arr\" None Array(Int 3) None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"arr\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@1.arr\" None Array(Int 3) None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"@1.arr\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_passing_array_to_array_param_decays (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_passing_array_to_array_param_decays\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@0.a\\\" Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.arr\\\" None Array(Int 3) None None None None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [AddressOf(Var(\\\"@1.arr\\\" Array(Int 3)) Pointer(Int))] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"foo\" [\"@0.a\"] Block([\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@1.arr\" None Array(Int 3) None None None None false)))\n"
      "    S(Return(FunctionCall(\"foo\" [Var(\"@1.arr\" None)] None false)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"foo\\\" [\\\"@0.a\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@0.a\\\" Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.arr\\\" None Array(Int 3) None None None None false)))\\n    S(Return(FunctionCall(\\\"foo\\\" [AddressOf(Var(\\\"@1.arr\\\" Array(Int 3)) Pointer(Int))] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"foo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Int)] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_pointer_arithmetic_on_array (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_arithmetic_on_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"long main(void) { int a[10]; return (long)(a + 1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_pointer_arithmetic_on_array (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_arithmetic_on_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"a\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@0.a\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_pointer_arithmetic_on_array (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_arithmetic_on_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Cast(Long Binary(Add AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Cast(Long Binary(Add Var(\"@0.a\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Long) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Cast(Long Binary(Add AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(1) Int) Pointer(Int)) Long)))\\n  ]) FunType([] Long) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Long)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_pointer_init_from_array_decays (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_init_from_array_decays\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[10]; int *p = a; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_pointer_init_from_array_decays (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_init_from_array_decays\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_pointer_subscript_works_too (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_subscript_works_too\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[10]; int *p = a; return p[3]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"p\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_pointer_subscript_works_too (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_subscript_works_too\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"p\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.p\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_pointer_subscript_works_too (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_pointer_subscript_works_too\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Subscript(Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.a\" None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@1.p\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Pointer(Int) None None None None false)))\\n    S(Return(Subscript(Var(\\\"@1.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_reverse_subscript_int_on_array_side_canonicalizes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_reverse_subscript_int_on_array_side_canonicalizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[10]; return 3[a]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_reverse_subscript_int_on_array_side_canonicalizes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_reverse_subscript_int_on_array_side_canonicalizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_reverse_subscript_int_on_array_side_canonicalizes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_reverse_subscript_int_on_array_side_canonicalizes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"@0.a\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 10) None None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 10)) Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 10)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_reverse_subscript_with_pointer_param (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_reverse_subscript_with_pointer_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int *p) { return 3[p]; } int main(void) { return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_reverse_subscript_with_pointer_param (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_reverse_subscript_with_pointer_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"@0.p\" None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_reverse_subscript_with_pointer_param (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_reverse_subscript_with_pointer_param\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@0.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(Constant(ConstInt(3) None) Var(\"@0.p\" None) None)))\n"
      "  ]) FunType([Pointer(Int)] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@0.p\\\" Pointer(Int)) Constant(ConstInt(3) Int) Int)))\\n  ]) FunType([Pointer(Int)] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Int)] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_scalar_init_for_array_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_scalar_init_for_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[3] = 5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_scalar_init_for_array_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_scalar_init_for_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_scalar_init_for_array_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_scalar_init_for_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"array '@0.a' requires a brace-enclosed initializer (`{...}`)\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_scalar_initializer_for_array_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_scalar_initializer_for_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[3] = 5; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_scalar_initializer_for_array_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_scalar_initializer_for_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_scalar_initializer_for_array_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_scalar_initializer_for_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" Constant(ConstInt(5) None) Array(Int 3) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"array '@0.a' requires a brace-enclosed initializer (`{...}`)\"}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_short_init_list_accepted (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_short_init_list_accepted\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Int 5) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Int 5) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_sizeof_incomplete_array_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_sizeof_incomplete_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern int a[]; int main(void) { return (int)sizeof(a); }\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Int SizeOfExp(Var(\"a\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_sizeof_incomplete_array_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_sizeof_incomplete_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Int SizeOfExp(Var(\"a\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Int SizeOfExp(Var(\"a\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_sizeof_incomplete_array_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_sizeof_incomplete_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 0) Extern None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Int SizeOfExp(Var(\"a\" None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"sizeof of an incomplete array type is illegal (C99 \\u00a76.5.3.4.1)\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_static_array_no_init_zeroes (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_no_init_zeroes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int a[3]; return a[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_static_array_no_init_zeroes (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_no_init_zeroes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_static_array_no_init_zeroes (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_no_init_zeroes\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 3) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Int 3) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_static_array_partial_init_zero_pads (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_partial_init_zero_pads\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int a[5] = {1, 2}; return a[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_static_array_partial_init_zero_pads (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_partial_init_zero_pads\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_static_array_partial_init_zero_pads (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_partial_init_zero_pads\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 5)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Array(Int 5) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 5) None) Array(Int 5) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 5)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 0, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_static_array_with_init_list (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int a[3] = {1, 2, 3}; return a[0]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_static_array_with_init_list (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_static_array_with_init_list (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_static_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 3) Static None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(3) Int)] Array(Int 3) None) Array(Int 3) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Int 3)) Pointer(Int)) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 3]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_static_long_array_with_init_list (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_static_long_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static long a[3] = {1, 2, 3}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Long 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_static_long_array_with_init_list (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_static_long_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Long 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Long 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_static_long_array_with_init_list (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_static_long_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long) Cast(Long Constant(ConstInt(3) Int) Long)] Array(Long 3) None) Array(Long 3) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Long 3) Static None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" InitList([Cast(Long Constant(ConstInt(1) Int) Long) Cast(Long Constant(ConstInt(2) Int) Long) Cast(Long Constant(ConstInt(3) Int) Long)] Array(Long 3) None) Array(Long 3) Static None None None false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Long 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 3]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_static_multi_dim_array_with_init_list (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_static_multi_dim_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { static int nested[3][2] = {{1,2},{3,4},{5,6}}; return nested[1][1]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"nested\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 3) Static None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"nested\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_static_multi_dim_array_with_init_list (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_static_multi_dim_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"nested\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 3) Static None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"nested\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.nested\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 3) Static None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"@0.nested\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_static_multi_dim_array_with_init_list (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_static_multi_dim_array_with_init_list\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.nested\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) InitList([Constant(ConstInt(3) Int) Constant(ConstInt(4) Int)] Array(Int 2) None) InitList([Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 2) None)] Array(Array(Int 2) 3) None) Array(Array(Int 2) 3) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.nested\\\" Array(Array(Int 2) 3)) Pointer(Array(Int 2))) Constant(ConstInt(1) Int) Array(Int 2)) Pointer(Int)) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.nested\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Int 2) 3) Static None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"@0.nested\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.nested\\\" InitList([InitList([Constant(ConstInt(1) Int) Constant(ConstInt(2) Int)] Array(Int 2) None) InitList([Constant(ConstInt(3) Int) Constant(ConstInt(4) Int)] Array(Int 2) None) InitList([Constant(ConstInt(5) Int) Constant(ConstInt(6) Int)] Array(Int 2) None)] Array(Array(Int 2) 3) None) Array(Array(Int 2) 3) Static None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.nested\\\" Array(Array(Int 2) 3)) Pointer(Array(Int 2))) Constant(ConstInt(1) Int) Array(Int 2)) Pointer(Int)) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.nested\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}, {\"tuple\": [5, 6]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_struct_member_incomplete_array_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_struct_member_incomplete_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { int a[]; }; \"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(Int 0))]))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_struct_member_incomplete_array_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_struct_member_incomplete_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(Int 0))]))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(Int 0))]))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_struct_member_incomplete_array_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_struct_member_incomplete_array_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(Int 0))]))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"member 'a' of struct/union 's': incomplete array type (`[]`) is allowed only at the outermost type of an `extern` declaration\"}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_subscript_array_operand_decays_to_pointer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_array_operand_decays_to_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_subscript_index_already_int (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_index_already_int\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_subscript_index_must_be_integer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_index_must_be_integer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[10]; double i = 1.0; return a[i]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"i\" Constant(ConstDouble(4607182418800017408) None) Double None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Var(\"i\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_subscript_index_must_be_integer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_index_must_be_integer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"i\" Constant(ConstDouble(4607182418800017408) None) Double None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Var(\"i\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstDouble(4607182418800017408) None) Double None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Var(\"@1.i\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_subscript_index_must_be_integer (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_index_must_be_integer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.i\" Constant(ConstDouble(4607182418800017408) None) Double None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Var(\"@1.i\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"subscript needs a pointer/array operand and an integer operand; got Pointer(referenced_type=Int()) and Double()\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_subscript_with_two_pointers_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_with_two_pointers_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int *p; int *q; return p[q]; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" None Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_subscript_with_two_pointers_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_with_two_pointers_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"q\" None Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" None Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.q\" None Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_subscript_with_two_pointers_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_with_two_pointers_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" None Pointer(Int) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.q\" None Pointer(Int) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"subscript needs a pointer/array operand and an integer operand; got Pointer(referenced_type=Int()) and Pointer(referenced_type=Int())\"}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_subscript_yields_element_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_subscript_yields_element_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Int 10) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@0.a\" None) Constant(ConstInt(3) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("parse: test_type_checking.py::TestArrays::test_too_many_initializers_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestArrays::test_too_many_initializers_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a[2] = {1, 2, 3}; return 0; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_too_many_initializers_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_too_many_initializers_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_too_many_initializers_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_too_many_initializers_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None) Constant(ConstInt(3) None)] None None) Array(Int 2) None None None None false)))\n"
      "    S(Return(Constant(ConstInt(0) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"too many initializers for array '@0.a': 3 given, array has 2 elements\"}\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestArrays::test_two_dim_subscript_yields_element_type (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestArrays::test_two_dim_subscript_yields_element_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Array(Array(Int 4) 3) None None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Array(Int 4) 3) None None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"@0.a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestArrays::test_two_dim_subscript_yields_element_type (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestArrays::test_two_dim_subscript_yields_element_type\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Array(Int 4) 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Array(Int 4) 3)) Pointer(Array(Int 4))) Constant(ConstInt(1) Int) Array(Int 4)) Pointer(Int)) Constant(ConstInt(2) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Array(Array(Int 4) 3) None None None None false)))\n"
      "    S(Return(Subscript(Subscript(Var(\"@0.a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(2) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.a\\\" None Array(Array(Int 4) 3) None None None None false)))\\n    S(Return(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"@0.a\\\" Array(Array(Int 4) 3)) Pointer(Array(Int 4))) Constant(ConstInt(1) Int) Array(Int 4)) Pointer(Int)) Constant(ConstInt(2) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Int 4) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_char_array_no_room_for_terminator (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_array_no_room_for_terminator\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char arr[3] = \\\"abc\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_char_array_no_room_for_terminator (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_array_no_room_for_terminator\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_char_array_no_room_for_terminator (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_array_no_room_for_terminator\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 3) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" String(\\\"abc\\\") Array(Char 3) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [97, 98, 99]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_char_array_string_too_long_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_array_string_too_long_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"char arr[3] = \\\"abcd\\\";\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abcd\") Array(Char 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_char_array_string_too_long_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_array_string_too_long_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abcd\") Array(Char 3) None None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abcd\") Array(Char 3) None None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_char_array_string_too_long_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_array_string_too_long_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abcd\") Array(Char 3) None None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"string literal initializer for array 'arr' has 4 bytes but the declared array size is 3 (C99 \\u00a76.7.8.14 requires the literal length to be \\u2264 the array size, with the null terminator omitted when the array has no room for it)\"}\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_char_array_with_string_init (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_array_with_string_init\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" String(\"abc\") Array(Char 4) None None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" String(\\\"abc\\\") Array(Char 4) None None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [97, 98, 99, 0]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_char_local_records_in_symbol_table (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_local_records_in_symbol_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { char c = 'a'; signed char s = 'b'; unsigned char u = 'c'; return c + s + u; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(98) None) SChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"u\" Constant(ConstInt(99) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Var(\"c\" None) Var(\"s\" None) None) Var(\"u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_char_local_records_in_symbol_table (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_local_records_in_symbol_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"s\" Constant(ConstInt(98) None) SChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"u\" Constant(ConstInt(99) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Var(\"c\" None) Var(\"s\" None) None) Var(\"u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.s\" Constant(ConstInt(98) None) SChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.u\" Constant(ConstInt(99) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Var(\"@0.c\" None) Var(\"@1.s\" None) None) Var(\"@2.u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_char_local_records_in_symbol_table (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_local_records_in_symbol_table\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.c\\\" Cast(Char Constant(ConstInt(97) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.s\\\" Cast(SChar Constant(ConstInt(98) Int) SChar) SChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.u\\\" Cast(UChar Constant(ConstInt(99) Int) UChar) UChar None None None None false)))\\n    S(Return(Binary(Add Binary(Add Cast(Int Var(\\\"@0.c\\\" Char) Int) Cast(Int Var(\\\"@1.s\\\" SChar) Int) Int) Cast(Int Var(\\\"@2.u\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.s\" Constant(ConstInt(98) None) SChar None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@2.u\" Constant(ConstInt(99) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Binary(Add Var(\"@0.c\" None) Var(\"@1.s\" None) None) Var(\"@2.u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.c\\\" Cast(Char Constant(ConstInt(97) Int) Char) Char None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.s\\\" Cast(SChar Constant(ConstInt(98) Int) SChar) SChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.u\\\" Cast(UChar Constant(ConstInt(99) Int) UChar) UChar None None None None false)))\\n    S(Return(Binary(Add Binary(Add Cast(Int Var(\\\"@0.c\\\" Char) Int) Cast(Int Var(\\\"@1.s\\\" SChar) Int) Int) Cast(Int Var(\\\"@2.u\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_char_pointer_init_with_string (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_pointer_init_with_string\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"str@0\\\" String(\\\"abc\\\") Array(Char 4) Static None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"str@0\\\" Array(Char 4)) Pointer(Char)) Pointer(Char) Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"str@0\" String(\"abc\") Array(Char 4) Static None None None false))\n"
      "  VarDecl(var_decl(\"p\" Var(\"str@0\" None) Pointer(Char) Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"str@0\\\" String(\\\"abc\\\") Array(Char 4) Static None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"str@0\\\" Array(Char 4)) Pointer(Char)) Pointer(Char) Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"str@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Char 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [97, 98, 99, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Char)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"str@0\", \"offset\": 0}}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_char_promotes_to_int_in_arithmetic (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_promotes_to_int_in_arithmetic\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { char c = 'a'; return c + c; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"c\" None) Var(\"c\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_char_promotes_to_int_in_arithmetic (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_promotes_to_int_in_arithmetic\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"c\" None) Var(\"c\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.c\" None) Var(\"@0.c\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_char_promotes_to_int_in_arithmetic (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_char_promotes_to_int_in_arithmetic\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.c\\\" Cast(Char Constant(ConstInt(97) Int) Char) Char None None None None false)))\\n    S(Return(Binary(Add Cast(Int Var(\\\"@0.c\\\" Char) Int) Cast(Int Var(\\\"@0.c\\\" Char) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.c\" Constant(ConstInt(97) None) Char None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.c\" None) Var(\"@0.c\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.c\\\" Cast(Char Constant(ConstInt(97) Int) Char) Char None None None None false)))\\n    S(Return(Binary(Add Cast(Int Var(\\\"@0.c\\\" Char) Int) Cast(Int Var(\\\"@0.c\\\" Char) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_pointer_shift_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_pointer_shift_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; int *p = &a; return (int)(p << 1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_pointer_shift_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_pointer_shift_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_pointer_shift_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_pointer_shift_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '<<' requires integer operands (Pointer(referenced_type=Int()) << Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_pointer_shift_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_pointer_shift_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { int a; int *p = &a; return (int)(p >> 1); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(RightShift Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_pointer_shift_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_pointer_shift_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"p\" AddressOf(Var(\"a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(RightShift Var(\"p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(RightShift Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_pointer_shift_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_pointer_shift_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.a\" None Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.p\" AddressOf(Var(\"@0.a\" None) None) Pointer(Int) None None None None false)))\n"
      "    S(Return(Cast(Int Binary(RightShift Var(\"@1.p\" None) Constant(ConstInt(1) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '>>' requires integer operands (Pointer(referenced_type=Int()) >> Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_shift_result_type_is_promoted_left_only (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_shift_result_type_is_promoted_left_only\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { unsigned long u = -1ul; return (int)(u << 2ll); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"u\" Unary(Negate Constant(ConstULong(1) None) None) ULong None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"u\" None) Constant(ConstLongLong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_shift_result_type_is_promoted_left_only (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_shift_result_type_is_promoted_left_only\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"u\" Unary(Negate Constant(ConstULong(1) None) None) ULong None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"u\" None) Constant(ConstLongLong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" Unary(Negate Constant(ConstULong(1) None) None) ULong None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"@0.u\" None) Constant(ConstLongLong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_shift_result_type_is_promoted_left_only (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_shift_result_type_is_promoted_left_only\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.u\\\" Unary(Negate Constant(ConstULong(1) ULong) ULong) ULong None None None None false)))\\n    S(Return(Cast(Int Binary(LeftShift Var(\\\"@0.u\\\" ULong) Constant(ConstLongLong(2) LongLong) ULong) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" Unary(Negate Constant(ConstULong(1) None) None) ULong None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"@0.u\" None) Constant(ConstLongLong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.u\\\" Unary(Negate Constant(ConstULong(1) ULong) ULong) ULong None None None None false)))\\n    S(Return(Cast(Int Binary(LeftShift Var(\\\"@0.u\\\" ULong) Constant(ConstLongLong(2) LongLong) ULong) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_shift_signed_left_long_unsigned_right_keeps_signedness (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_shift_signed_left_long_unsigned_right_keeps_signedness\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { long s = (long)1; return (int)(s << 2ul); }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"s\" None) Constant(ConstULong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_shift_signed_left_long_unsigned_right_keeps_signedness (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_shift_signed_left_long_unsigned_right_keeps_signedness\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"s\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"s\" None) Constant(ConstULong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"@0.s\" None) Constant(ConstULong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_shift_signed_left_long_unsigned_right_keeps_signedness (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_shift_signed_left_long_unsigned_right_keeps_signedness\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.s\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(LeftShift Var(\\\"@0.s\\\" Long) Constant(ConstULong(2) ULong) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.s\" Cast(Long Constant(ConstInt(1) None) None) Long None None None None false)))\n"
      "    S(Return(Cast(Int Binary(LeftShift Var(\"@0.s\" None) Constant(ConstULong(2) None) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.s\\\" Cast(Long Constant(ConstInt(1) Int) Long) Long None None None None false)))\\n    S(Return(Cast(Int Binary(LeftShift Var(\\\"@0.s\\\" Long) Constant(ConstULong(2) ULong) Long) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestCharAndString::test_uchar_promotes_to_int_now_that_int_covers_full_range (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestCharAndString::test_uchar_promotes_to_int_now_that_int_covers_full_range\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) { unsigned char u = 'a'; return u + u; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"u\" Constant(ConstInt(97) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"u\" None) Var(\"u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestCharAndString::test_uchar_promotes_to_int_now_that_int_covers_full_range (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestCharAndString::test_uchar_promotes_to_int_now_that_int_covers_full_range\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"u\" Constant(ConstInt(97) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"u\" None) Var(\"u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" Constant(ConstInt(97) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.u\" None) Var(\"@0.u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestCharAndString::test_uchar_promotes_to_int_now_that_int_covers_full_range (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestCharAndString::test_uchar_promotes_to_int_now_that_int_covers_full_range\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.u\\\" Cast(UChar Constant(ConstInt(97) Int) UChar) UChar None None None None false)))\\n    S(Return(Binary(Add Cast(Int Var(\\\"@0.u\\\" UChar) Int) Cast(Int Var(\\\"@0.u\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.u\" Constant(ConstInt(97) None) UChar None None None None false)))\n"
      "    S(Return(Binary(Add Var(\"@0.u\" None) Var(\"@0.u\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.u\\\" Cast(UChar Constant(ConstInt(97) Int) UChar) UChar None None None None false)))\\n    S(Return(Binary(Add Cast(Int Var(\\\"@0.u\\\" UChar) Int) Cast(Int Var(\\\"@0.u\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"const void *f(const void *p) { return p + 1; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Pointer(Const(Void))) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Binary(Add Var(\"p\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Pointer(Const(Void))) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.p\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Pointer(Const(Void))) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Binary(Add Var(\"@0.p\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Pointer(Const(Void))) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"binary '+' is not defined on void pointer operands (Pointer(referenced_type=Const(referenced_type=Void())) + Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"void f(const void *p) { p++; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"@0.p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_arithmetic_on_pointer_to_const_void_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Expression(Postfix(Increment Var(\"@0.p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"increment/decrement on void pointer is not defined (sizeof(void) is undefined)\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_converts_to_pointer_to_const (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_converts_to_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct t { unsigned char a[3]; };\\nunsigned char f(const struct t *p, int i) {\\n    const unsigned char *q = p->a;\\n    return q[i];\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"i\"] Block([\n"
      "    D(VarDecl(var_decl(\"q\" Arrow(Var(\"p\" None) \"a\" None) Pointer(Const(UChar)) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"q\" None) Var(\"i\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"t\"))) Int] UChar) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_converts_to_pointer_to_const (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_converts_to_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"i\"] Block([\n"
      "    D(VarDecl(var_decl(\"q\" Arrow(Var(\"p\" None) \"a\" None) Pointer(Const(UChar)) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"q\" None) Var(\"i\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"t\"))) Int] UChar) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.i\"] Block([\n"
      "    D(VarDecl(var_decl(\"@2.q\" Arrow(Var(\"@0.p\" None) \"a\" None) Pointer(Const(UChar)) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@2.q\" None) Var(\"@1.i\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"t\"))) Int] UChar) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_converts_to_pointer_to_const (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_converts_to_pointer_to_const\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"t\\\" false [member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\" \\\"@1.i\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"t\\\")))) \\\"a\\\" Array(Const(UChar) 3)) Pointer(Const(UChar))) Pointer(Const(UChar)) None None None None false)))\\n    S(Return(Subscript(Var(\\\"@2.q\\\" Pointer(Const(UChar))) Var(\\\"@1.i\\\" Int) Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"t\\\"))) Int] UChar) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.i\"] Block([\n"
      "    D(VarDecl(var_decl(\"@2.q\" Arrow(Var(\"@0.p\" None) \"a\" None) Pointer(Const(UChar)) None None None None false)))\n"
      "    S(Return(Subscript(Var(\"@2.q\" None) Var(\"@1.i\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"t\"))) Int] UChar) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"t\\\" false [member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\" \\\"@1.i\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.q\\\" AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"t\\\")))) \\\"a\\\" Array(Const(UChar) 3)) Pointer(Const(UChar))) Pointer(Const(UChar)) None None None None false)))\\n    S(Return(Subscript(Var(\\\"@2.q\\\" Pointer(Const(UChar))) Var(\\\"@1.i\\\" Int) Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"t\\\"))) Int] UChar) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"t\\\"))) Int] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"t\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"t\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"byte_offset\": 0}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_has_const_elements (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_has_const_elements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { unsigned char a[3]; };\\nunsigned char f(const struct s *p) { return p->a[1]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_has_const_elements (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_has_const_elements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"@0.p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_has_const_elements (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_const_struct_has_const_elements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"s\\\")))) \\\"a\\\" Array(Const(UChar) 3)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"@0.p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Const(Structure(\\\"s\\\")))) \\\"a\\\" Array(Const(UChar) 3)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"byte_offset\": 0}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_array_member_of_volatile_struct_has_volatile_elements (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_volatile_struct_has_volatile_elements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { unsigned char a[3]; };\\nunsigned char f(volatile struct s *p) { return p->a[1]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Volatile(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_array_member_of_volatile_struct_has_volatile_elements (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_volatile_struct_has_volatile_elements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Volatile(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"@0.p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Volatile(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_array_member_of_volatile_struct_has_volatile_elements (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_array_member_of_volatile_struct_has_volatile_elements\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Volatile(Structure(\\\"s\\\")))) \\\"a\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(1) Int) Volatile(UChar))))\\n  ]) FunType([Pointer(Volatile(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Subscript(Arrow(Var(\"@0.p\" None) \"a\" None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([Pointer(Volatile(Structure(\"s\")))] UChar) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(AddressOf(Arrow(Var(\\\"@0.p\\\" Pointer(Volatile(Structure(\\\"s\\\")))) \\\"a\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(1) Int) Volatile(UChar))))\\n  ]) FunType([Pointer(Volatile(Structure(\\\"s\\\")))] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Volatile(Structure(\\\"s\\\")))] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Volatile(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"byte_offset\": 0}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_call_through_const_function_pointer (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_call_through_const_function_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int h(int);\\nint (*const fp)(int) = h;\\nint f(int x) { return fp(x); }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"h\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"h\" None) Const(Pointer(FunType([Int] Int))) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(FunctionCall(\"fp\" [Var(\"x\" None)] None false)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_call_through_const_function_pointer (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_call_through_const_function_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"h\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"h\" None) Const(Pointer(FunType([Int] Int))) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"x\"] Block([\n"
      "    S(Return(FunctionCall(\"fp\" [Var(\"x\" None)] None false)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"h\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"h\" None) Const(Pointer(FunType([Int] Int))) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(FunctionCall(\"fp\" [Var(\"@0.x\" None)] None false)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_call_through_const_function_pointer (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_call_through_const_function_pointer\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"h\\\" [\\\"\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"fp\\\" AddressOf(Var(\\\"h\\\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Const(Pointer(FunType([Int] Int))) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(FunctionCall(\\\"fp\\\" [Var(\\\"@0.x\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"h\" [\"\"] None FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"fp\" Var(\"h\" None) Const(Pointer(FunType([Int] Int))) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.x\"] Block([\n"
      "    S(Return(FunctionCall(\"fp\" [Var(\"@0.x\" None)] None false)))\n"
      "  ]) FunType([Int] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"h\\\" [\\\"\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"fp\\\" AddressOf(Var(\\\"h\\\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Const(Pointer(FunType([Int] Int))) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(FunctionCall(\\\"fp\\\" [Var(\\\"@0.x\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"h\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"fp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(FunType([Int] Int)))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"h\", \"offset\": 0}}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_comparisons_through_const_pointers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_comparisons_through_const_pointers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(int *const p, int *q) { return p - q + (p < q) + (p == q); }\\nint *g(int c, int *const p) { return c \077 p : 0; }\\nint *const cp = 0;\\nvoid *h(void) { void *v = cp; return v; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\"p\" None) Var(\"q\" None) None) Binary(LessThan Var(\"p\" None) Var(\"q\" None) None) None) Binary(Equal Var(\"p\" None) Var(\"q\" None) None) None)))\n"
      "  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"c\" \"p\"] Block([\n"
      "    S(Return(Conditional(Var(\"c\" None) Var(\"p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\"\" \"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"cp\" Constant(ConstInt(0) None) Const(Pointer(Int)) None None None None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" Var(\"cp\" None) Pointer(Void) None None None None false)))\n"
      "    S(Return(Var(\"v\" None)))\n"
      "  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_comparisons_through_const_pointers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_comparisons_through_const_pointers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\"p\" None) Var(\"q\" None) None) Binary(LessThan Var(\"p\" None) Var(\"q\" None) None) None) Binary(Equal Var(\"p\" None) Var(\"q\" None) None) None)))\n"
      "  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"c\" \"p\"] Block([\n"
      "    S(Return(Conditional(Var(\"c\" None) Var(\"p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\"\" \"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"cp\" Constant(ConstInt(0) None) Const(Pointer(Int)) None None None None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    D(VarDecl(var_decl(\"v\" Var(\"cp\" None) Pointer(Void) None None None None false)))\n"
      "    S(Return(Var(\"v\" None)))\n"
      "  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\"@0.p\" None) Var(\"@1.q\" None) None) Binary(LessThan Var(\"@0.p\" None) Var(\"@1.q\" None) None) None) Binary(Equal Var(\"@0.p\" None) Var(\"@1.q\" None) None) None)))\n"
      "  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@2.c\" \"@3.p\"] Block([\n"
      "    S(Return(Conditional(Var(\"@2.c\" None) Var(\"@3.p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\"\" \"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"cp\" Constant(ConstInt(0) None) Const(Pointer(Int)) None None None None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@4.v\" Var(\"cp\" None) Pointer(Void) None None None None false)))\n"
      "    S(Return(Var(\"@4.v\" None)))\n"
      "  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_comparisons_through_const_pointers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_comparisons_through_const_pointers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\" \\\"@1.q\\\"] Block([\\n    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\\\"@0.p\\\" Const(Pointer(Int))) Var(\\\"@1.q\\\" Pointer(Int)) Int) Binary(LessThan Var(\\\"@0.p\\\" Const(Pointer(Int))) Var(\\\"@1.q\\\" Pointer(Int)) Int) Int) Binary(Equal Var(\\\"@0.p\\\" Const(Pointer(Int))) Var(\\\"@1.q\\\" Pointer(Int)) Int) Int)))\\n  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@2.c\\\" \\\"@3.p\\\"] Block([\\n    S(Return(Conditional(Var(\\\"@2.c\\\" Int) Var(\\\"@3.p\\\" Const(Pointer(Int))) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int))))\\n  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"cp\\\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) None None None None false))\\n  FunctionDecl(function_decl(\\\"h\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@4.v\\\" Cast(Pointer(Void) Var(\\\"cp\\\" Const(Pointer(Int))) Pointer(Void)) Pointer(Void) None None None None false)))\\n    S(Return(Var(\\\"@4.v\\\" Pointer(Void))))\\n  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\"@0.p\" None) Var(\"@1.q\" None) None) Binary(LessThan Var(\"@0.p\" None) Var(\"@1.q\" None) None) None) Binary(Equal Var(\"@0.p\" None) Var(\"@1.q\" None) None) None)))\n"
      "  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@2.c\" \"@3.p\"] Block([\n"
      "    S(Return(Conditional(Var(\"@2.c\" None) Var(\"@3.p\" None) Constant(ConstInt(0) None) None)))\n"
      "  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\"\" \"\"] false None [] None false))\n"
      "  VarDecl(var_decl(\"cp\" Constant(ConstInt(0) None) Const(Pointer(Int)) None None None None false))\n"
      "  FunctionDecl(function_decl(\"h\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@4.v\" Var(\"cp\" None) Pointer(Void) None None None None false)))\n"
      "    S(Return(Var(\"@4.v\" None)))\n"
      "  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\" \\\"@1.q\\\"] Block([\\n    S(Return(Binary(Add Binary(Add Binary(Subtract Var(\\\"@0.p\\\" Const(Pointer(Int))) Var(\\\"@1.q\\\" Pointer(Int)) Int) Binary(LessThan Var(\\\"@0.p\\\" Const(Pointer(Int))) Var(\\\"@1.q\\\" Pointer(Int)) Int) Int) Binary(Equal Var(\\\"@0.p\\\" Const(Pointer(Int))) Var(\\\"@1.q\\\" Pointer(Int)) Int) Int)))\\n  ]) FunType([Const(Pointer(Int)) Pointer(Int)] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@2.c\\\" \\\"@3.p\\\"] Block([\\n    S(Return(Conditional(Var(\\\"@2.c\\\" Int) Var(\\\"@3.p\\\" Const(Pointer(Int))) Cast(Pointer(Int) Constant(ConstInt(0) Int) Pointer(Int)) Pointer(Int))))\\n  ]) FunType([Int Const(Pointer(Int))] Pointer(Int)) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"cp\\\" Cast(Const(Pointer(Int)) Constant(ConstInt(0) Int) Const(Pointer(Int))) Const(Pointer(Int)) None None None None false))\\n  FunctionDecl(function_decl(\\\"h\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@4.v\\\" Cast(Pointer(Void) Var(\\\"cp\\\" Const(Pointer(Int))) Pointer(Void)) Pointer(Void) None None None None false)))\\n    S(Return(Var(\\\"@4.v\\\" Pointer(Void))))\\n  ]) FunType([] Pointer(Void)) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Const(Pointer(Int)) Pointer(Int)] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Const(Pointer(Int))] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"cp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"h\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"as"
      "dl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Void))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@4.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_conditional_pointer_takes_both_pointee_qualifiers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_conditional_pointer_takes_both_pointee_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char buf[4];\\nconst unsigned char *f(int c, const unsigned char *p) {\\n    return c \077 buf : p;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\" \"p\"] Block([\n"
      "    S(Return(Conditional(Var(\"c\" None) Var(\"buf\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([Int Pointer(Const(UChar))] Pointer(Const(UChar))) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_conditional_pointer_takes_both_pointee_qualifiers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_conditional_pointer_takes_both_pointee_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"c\" \"p\"] Block([\n"
      "    S(Return(Conditional(Var(\"c\" None) Var(\"buf\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([Int Pointer(Const(UChar))] Pointer(Const(UChar))) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\" \"@1.p\"] Block([\n"
      "    S(Return(Conditional(Var(\"@0.c\" None) Var(\"buf\" None) Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([Int Pointer(Const(UChar))] Pointer(Const(UChar))) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_conditional_pointer_takes_both_pointee_qualifiers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_conditional_pointer_takes_both_pointee_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\" \\\"@1.p\\\"] Block([\\n    S(Return(Conditional(Var(\\\"@0.c\\\" Int) AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Var(\\\"@1.p\\\" Pointer(Const(UChar))) Pointer(Const(UChar)))))\\n  ]) FunType([Int Pointer(Const(UChar))] Pointer(Const(UChar))) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.c\" \"@1.p\"] Block([\n"
      "    S(Return(Conditional(Var(\"@0.c\" None) Var(\"buf\" None) Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([Int Pointer(Const(UChar))] Pointer(Const(UChar))) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\" \\\"@1.p\\\"] Block([\\n    S(Return(Conditional(Var(\\\"@0.c\\\" Int) AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Var(\\\"@1.p\\\" Pointer(Const(UChar))) Pointer(Const(UChar)))))\\n  ]) FunType([Int Pointer(Const(UChar))] Pointer(Const(UChar))) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"buf\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int Pointer(Const(UChar))] Pointer(Const(UChar)))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_const_struct_objects_take_initializer_lists (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_const_struct_objects_take_initializer_lists\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { unsigned char a; int b; };\\nconst struct s g = {1, 2};\\nconst struct s ga[2] = {{3, 4}, {5, 6}};\\nint f(void) {\\n    const struct s l = {7, 8};\\n    static const struct s sl = {9, 10};\\n    return l.b + sl.b;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  VarDecl(var_decl(\"g\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"ga\" InitList([InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Const(Structure(\"s\")) 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"l\" InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None) Const(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"sl\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(10) None)] None None) Const(Structure(\"s\")) Static None None None false)))\n"
      "    S(Return(Binary(Add Dot(Var(\"l\" None) \"b\" None) Dot(Var(\"sl\" None) \"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_const_struct_objects_take_initializer_lists (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_const_struct_objects_take_initializer_lists\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  VarDecl(var_decl(\"g\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"ga\" InitList([InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Const(Structure(\"s\")) 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"l\" InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None) Const(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"sl\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(10) None)] None None) Const(Structure(\"s\")) Static None None None false)))\n"
      "    S(Return(Binary(Add Dot(Var(\"l\" None) \"b\" None) Dot(Var(\"sl\" None) \"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  VarDecl(var_decl(\"g\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"ga\" InitList([InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Const(Structure(\"s\")) 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.l\" InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None) Const(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.sl\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(10) None)] None None) Const(Structure(\"s\")) Static None None None false)))\n"
      "    S(Return(Binary(Add Dot(Var(\"@0.l\" None) \"b\" None) Dot(Var(\"@1.sl\" None) \"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_const_struct_objects_take_initializer_lists (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_const_struct_objects_take_initializer_lists\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  VarDecl(var_decl(\\\"g\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Constant(ConstInt(2) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) None None None None false))\\n  VarDecl(var_decl(\\\"ga\\\" InitList([InitList([Cast(UChar Constant(ConstInt(3) Int) UChar) Constant(ConstInt(4) Int)] Structure(\\\"s\\\") None) InitList([Cast(UChar Constant(ConstInt(5) Int) UChar) Constant(ConstInt(6) Int)] Structure(\\\"s\\\") None)] Array(Const(Structure(\\\"s\\\")) 2) None) Array(Const(Structure(\\\"s\\\")) 2) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.l\\\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Constant(ConstInt(8) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.sl\\\" InitList([Cast(UChar Constant(ConstInt(9) Int) UChar) Constant(ConstInt(10) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None false)))\\n    S(Return(Binary(Add Dot(Var(\\\"@0.l\\\" Const(Structure(\\\"s\\\"))) \\\"b\\\" Const(Int)) Dot(Var(\\\"@1.sl\\\" Const(Structure(\\\"s\\\"))) \\\"b\\\" Const(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  VarDecl(var_decl(\"g\" InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) Const(Structure(\"s\")) None None None None false))\n"
      "  VarDecl(var_decl(\"ga\" InitList([InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Const(Structure(\"s\")) 2) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.l\" InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None) Const(Structure(\"s\")) None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.sl\" InitList([Constant(ConstInt(9) None) Constant(ConstInt(10) None)] None None) Const(Structure(\"s\")) Static None None None false)))\n"
      "    S(Return(Binary(Add Dot(Var(\"@0.l\" None) \"b\" None) Dot(Var(\"@1.sl\" None) \"b\" None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  VarDecl(var_decl(\\\"g\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Constant(ConstInt(2) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) None None None None false))\\n  VarDecl(var_decl(\\\"ga\\\" InitList([InitList([Cast(UChar Constant(ConstInt(3) Int) UChar) Constant(ConstInt(4) Int)] Structure(\\\"s\\\") None) InitList([Cast(UChar Constant(ConstInt(5) Int) UChar) Constant(ConstInt(6) Int)] Structure(\\\"s\\\") None)] Array(Const(Structure(\\\"s\\\")) 2) None) Array(Const(Structure(\\\"s\\\")) 2) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.l\\\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Constant(ConstInt(8) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.sl\\\" InitList([Cast(UChar Constant(ConstInt(9) Int) UChar) Constant(ConstInt(10) Int)] Structure(\\\"s\\\") None) Const(Structure(\\\"s\\\")) Static None None None false)))\\n    S(Return(Binary(Add Dot(Var(\\\"@0.l\\\" Const(Structure(\\\"s\\\"))) \\\"b\\\" Const(Int)) Dot(Var(\\\"@1.sl\\\" Const(Structure(\\\"s\\\"))) \\\"b\\\" Const(Int)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"ga\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Structure(\\\"s\\\")) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [3, 4]}, {\"tuple\": [5, 6]}]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.l\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.sl\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [9, 10]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": "
      "[{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_const_struct_with_block_scope_tag (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_const_struct_with_block_scope_tag\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(void) {\\n    struct t { int a; };\\n    const struct t x = {3};\\n    return x.a;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Int)])))\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(3) None)] None None) Const(Structure(\"t\")) None None None None false)))\n"
      "    S(Return(Dot(Var(\"x\" None) \"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_const_struct_with_block_scope_tag (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_const_struct_with_block_scope_tag\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(StructDecl(struct_decl(\"t\" false [member_decl(\"a\" Int)])))\n"
      "    D(VarDecl(var_decl(\"x\" InitList([Constant(ConstInt(3) None)] None None) Const(Structure(\"t\")) None None None None false)))\n"
      "    S(Return(Dot(Var(\"x\" None) \"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(StructDecl(struct_decl(\"@0.t\" false [member_decl(\"a\" Int)])))\n"
      "    D(VarDecl(var_decl(\"@1.x\" InitList([Constant(ConstInt(3) None)] None None) Const(Structure(\"@0.t\")) None None None None false)))\n"
      "    S(Return(Dot(Var(\"@1.x\" None) \"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_const_struct_with_block_scope_tag (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_const_struct_with_block_scope_tag\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(StructDecl(struct_decl(\\\"@0.t\\\" false [member_decl(\\\"a\\\" Int)])))\\n    D(VarDecl(var_decl(\\\"@1.x\\\" InitList([Constant(ConstInt(3) Int)] Structure(\\\"@0.t\\\") None) Const(Structure(\\\"@0.t\\\")) None None None None false)))\\n    S(Return(Dot(Var(\\\"@1.x\\\" Const(Structure(\\\"@0.t\\\"))) \\\"a\\\" Const(Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(StructDecl(struct_decl(\"@0.t\" false [member_decl(\"a\" Int)])))\n"
      "    D(VarDecl(var_decl(\"@1.x\" InitList([Constant(ConstInt(3) None)] None None) Const(Structure(\"@0.t\")) None None None None false)))\n"
      "    S(Return(Dot(Var(\"@1.x\" None) \"a\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(StructDecl(struct_decl(\\\"@0.t\\\" false [member_decl(\\\"a\\\" Int)])))\\n    D(VarDecl(var_decl(\\\"@1.x\\\" InitList([Constant(ConstInt(3) Int)] Structure(\\\"@0.t\\\") None) Const(Structure(\\\"@0.t\\\")) None None None None false)))\\n    S(Return(Dot(Var(\\\"@1.x\\\" Const(Structure(\\\"@0.t\\\"))) \\\"a\\\" Const(Int))))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Structure(\\\"@0.t\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.t\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"@0.t\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 0}}], \"size\": 2, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_equality_of_pointers_differing_in_pointee_qualifiers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_equality_of_pointers_differing_in_pointee_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char buf[4];\\nint f(const unsigned char *p) { return p != buf + 4; }\\nint g(volatile unsigned char *p) { return buf == p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Binary(NotEqual Var(\"p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"p\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"buf\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_equality_of_pointers_differing_in_pointee_qualifiers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_equality_of_pointers_differing_in_pointee_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Binary(NotEqual Var(\"p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"p\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"buf\" None) Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Binary(NotEqual Var(\"@0.p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@1.p\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"buf\" None) Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_equality_of_pointers_differing_in_pointee_qualifiers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_equality_of_pointers_differing_in_pointee_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Binary(NotEqual Var(\\\"@0.p\\\" Pointer(Const(UChar))) Binary(Add AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Constant(ConstInt(4) Int) Pointer(UChar)) Int)))\\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@1.p\\\"] Block([\\n    S(Return(Binary(Equal AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Var(\\\"@1.p\\\" Pointer(Volatile(UChar))) Int)))\\n  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Binary(NotEqual Var(\"@0.p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@1.p\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"buf\" None) Var(\"@1.p\" None) None)))\n"
      "  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Binary(NotEqual Var(\\\"@0.p\\\" Pointer(Const(UChar))) Binary(Add AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Constant(ConstInt(4) Int) Pointer(UChar)) Int)))\\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@1.p\\\"] Block([\\n    S(Return(Binary(Equal AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Var(\\\"@1.p\\\" Pointer(Volatile(UChar))) Int)))\\n  ]) FunType([Pointer(Volatile(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"buf\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(UChar))] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Volatile(UChar))] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Volatile(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_member_of_const_struct_isnt_writable (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_member_of_const_struct_isnt_writable\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { unsigned char a[3]; };\\nvoid f(const struct s *p) { p->a[1] = 5; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_member_of_const_struct_isnt_writable (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_member_of_const_struct_isnt_writable\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"@0.p\" None) \"a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_member_of_const_struct_isnt_writable (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_member_of_const_struct_isnt_writable\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"@0.p\" None) \"a\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(5) None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"cannot assign to const-qualified lvalue: Subscript(array=AddressOf(exp=Arrow(operand=Var(name='@0.p', data_type=Pointer(referenced_type=Const(referenced_type=Structure(tag='s')))), member='a', data_type=Array(element_type=Const(referenced_type=UChar()), size=3)), data_type=Pointer(referenced_type=Const(referenced_type=UChar()))), index=Constant(const=ConstInt(value=1), data_type=Int()), data_type=Const(referenced_type=UChar())) has type Const(referenced_type=UChar())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_member_through_const_pointer_is_writable (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_member_through_const_pointer_is_writable\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { unsigned char a; unsigned char b[3]; };\\nvoid f(struct s *q) {\\n    struct s *const p = q;\\n    p->a = 5; p->b[1] = 6; (*p).a = 7;\\n}\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"q\"] Block([\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"q\" None) Const(Pointer(Structure(\"s\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"p\" None) \"b\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(6) None) None)))\n"
      "    S(Expression(Assignment(Dot(Dereference(Var(\"p\" None) None) \"a\" None) Constant(ConstInt(7) None) None)))\n"
      "  ]) FunType([Pointer(Structure(\"s\"))] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_member_through_const_pointer_is_writable (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_member_through_const_pointer_is_writable\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"q\"] Block([\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"q\" None) Const(Pointer(Structure(\"s\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"p\" None) \"a\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"p\" None) \"b\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(6) None) None)))\n"
      "    S(Expression(Assignment(Dot(Dereference(Var(\"p\" None) None) \"a\" None) Constant(ConstInt(7) None) None)))\n"
      "  ]) FunType([Pointer(Structure(\"s\"))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.q\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.q\" None) Const(Pointer(Structure(\"s\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"@1.p\" None) \"a\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"@1.p\" None) \"b\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(6) None) None)))\n"
      "    S(Expression(Assignment(Dot(Dereference(Var(\"@1.p\" None) None) \"a\" None) Constant(ConstInt(7) None) None)))\n"
      "  ]) FunType([Pointer(Structure(\"s\"))] Void) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_member_through_const_pointer_is_writable (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_member_through_const_pointer_is_writable\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.q\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.p\\\" Var(\\\"@0.q\\\" Pointer(Structure(\\\"s\\\"))) Const(Pointer(Structure(\\\"s\\\"))) None None None None false)))\\n    S(Expression(Assignment(Arrow(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) \\\"a\\\" UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\\n    S(Expression(Assignment(Subscript(AddressOf(Arrow(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) \\\"b\\\" Array(UChar 3)) Pointer(UChar)) Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) UChar)))\\n    S(Expression(Assignment(Dot(Dereference(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) Structure(\\\"s\\\")) \\\"a\\\" UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) UChar)))\\n  ]) FunType([Pointer(Structure(\\\"s\\\"))] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Array(UChar 3))]))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.q\"] Block([\n"
      "    D(VarDecl(var_decl(\"@1.p\" Var(\"@0.q\" None) Const(Pointer(Structure(\"s\"))) None None None None false)))\n"
      "    S(Expression(Assignment(Arrow(Var(\"@1.p\" None) \"a\" None) Constant(ConstInt(5) None) None)))\n"
      "    S(Expression(Assignment(Subscript(Arrow(Var(\"@1.p\" None) \"b\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(6) None) None)))\n"
      "    S(Expression(Assignment(Dot(Dereference(Var(\"@1.p\" None) None) \"a\" None) Constant(ConstInt(7) None) None)))\n"
      "  ]) FunType([Pointer(Structure(\"s\"))] Void) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Array(UChar 3))]))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.q\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.p\\\" Var(\\\"@0.q\\\" Pointer(Structure(\\\"s\\\"))) Const(Pointer(Structure(\\\"s\\\"))) None None None None false)))\\n    S(Expression(Assignment(Arrow(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) \\\"a\\\" UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\\n    S(Expression(Assignment(Subscript(AddressOf(Arrow(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) \\\"b\\\" Array(UChar 3)) Pointer(UChar)) Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) UChar)))\\n    S(Expression(Assignment(Dot(Dereference(Var(\\\"@1.p\\\" Const(Pointer(Structure(\\\"s\\\")))) Structure(\\\"s\\\")) \\\"a\\\" UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) UChar)))\\n  ]) FunType([Pointer(Structure(\\\"s\\\"))] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Structure(\\\"s\\\"))] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"byte_offset\": 1}}], \"size\": 4, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_ordering_a_void_pointer_and_an_object_pointer_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_ordering_a_void_pointer_and_an_object_pointer_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(const void *p, int *q) { return p < q; }\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_ordering_a_void_pointer_and_an_object_pointer_is_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_ordering_a_void_pointer_and_an_object_pointer_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_ordering_a_void_pointer_and_an_object_pointer_is_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_ordering_a_void_pointer_and_an_object_pointer_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Void)) Pointer(Int)] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"comparison of distinct pointer types: Pointer(referenced_type=Const(referenced_type=Void())) < Pointer(referenced_type=Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(const unsigned char **p, unsigned char **q) { return p == q; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([Pointer(Pointer(Const(UChar))) Pointer(Pointer(UChar))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([Pointer(Pointer(Const(UChar))) Pointer(Pointer(UChar))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([Pointer(Pointer(Const(UChar))) Pointer(Pointer(UChar))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([Pointer(Pointer(Const(UChar))) Pointer(Pointer(UChar))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"comparison of distinct pointer types: Pointer(referenced_type=Pointer(referenced_type=Const(referenced_type=UChar()))) vs Pointer(referenced_type=Pointer(referenced_type=UChar()))\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int f(const unsigned char *const *p, const unsigned char **q) { return p == q; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Pointer(Const(UChar)))) Pointer(Pointer(Const(UChar)))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\" \"q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"p\" None) Var(\"q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Pointer(Const(UChar)))) Pointer(Pointer(Const(UChar)))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Pointer(Const(UChar)))) Pointer(Pointer(Const(UChar)))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_qualifiers_deeper_than_the_pointee_still_have_to_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\" \\\"@1.q\\\"] Block([\\n    S(Return(Binary(Equal Var(\\\"@0.p\\\" Pointer(Const(Pointer(Const(UChar))))) Var(\\\"@1.q\\\" Pointer(Pointer(Const(UChar)))) Int)))\\n  ]) FunType([Pointer(Const(Pointer(Const(UChar)))) Pointer(Pointer(Const(UChar)))] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\" \"@1.q\"] Block([\n"
      "    S(Return(Binary(Equal Var(\"@0.p\" None) Var(\"@1.q\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Pointer(Const(UChar)))) Pointer(Pointer(Const(UChar)))] Int) None None [\"\" \"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\" \\\"@1.q\\\"] Block([\\n    S(Return(Binary(Equal Var(\\\"@0.p\\\" Pointer(Const(Pointer(Const(UChar))))) Var(\\\"@1.q\\\" Pointer(Pointer(Const(UChar)))) Int)))\\n  ]) FunType([Pointer(Const(Pointer(Const(UChar)))) Pointer(Pointer(Const(UChar)))] Int) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Pointer(Const(UChar)))) Pointer(Pointer(Const(UChar)))] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Pointer(Const(UChar))))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Pointer(Const(UChar)))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_relational_and_subtraction_of_pointers_differing_in_qualifiers (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_relational_and_subtraction_of_pointers_differing_in_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"unsigned char buf[4];\\nint f(const unsigned char *p) { return p < buf + 4; }\\nint g(const unsigned char *p) { return p - buf; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"p\"] Block([\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Var(\"buf\" None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_relational_and_subtraction_of_pointers_differing_in_qualifiers (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_relational_and_subtraction_of_pointers_differing_in_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"p\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"p\"] Block([\n"
      "    S(Return(Binary(Subtract Var(\"p\" None) Var(\"buf\" None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"@0.p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@1.p\"] Block([\n"
      "    S(Return(Binary(Subtract Var(\"@1.p\" None) Var(\"buf\" None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_relational_and_subtraction_of_pointers_differing_in_qualifiers (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_relational_and_subtraction_of_pointers_differing_in_qualifiers\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Binary(LessThan Var(\\\"@0.p\\\" Pointer(Const(UChar))) Binary(Add AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Constant(ConstInt(4) Int) Pointer(UChar)) Int)))\\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@1.p\\\"] Block([\\n    S(Return(Binary(Subtract Var(\\\"@1.p\\\" Pointer(Const(UChar))) AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Int)))\\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"buf\" None Array(UChar 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@0.p\"] Block([\n"
      "    S(Return(Binary(LessThan Var(\"@0.p\" None) Binary(Add Var(\"buf\" None) Constant(ConstInt(4) None) None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@1.p\"] Block([\n"
      "    S(Return(Binary(Subtract Var(\"@1.p\" None) Var(\"buf\" None) None)))\n"
      "  ]) FunType([Pointer(Const(UChar))] Int) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Binary(LessThan Var(\\\"@0.p\\\" Pointer(Const(UChar))) Binary(Add AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Constant(ConstInt(4) Int) Pointer(UChar)) Int)))\\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@1.p\\\"] Block([\\n    S(Return(Binary(Subtract Var(\\\"@1.p\\\" Pointer(Const(UChar))) AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Int)))\\n  ]) FunType([Pointer(Const(UChar))] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"buf\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(UChar))] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(UChar))] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_struct_copy_from_const_struct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_struct_copy_from_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct s { unsigned char a; int b; };\\nvoid g(struct s v);\\nvoid f(struct s *d, const struct s *p) {\\n    struct s v = *p;\\n    *d = *p;\\n    g(*p);\\n}\\nstruct s h(const struct s *p) { return *p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  FunctionDecl(function_decl(\"g\" [\"v\"] None FunType([Structure(\"s\")] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"d\" \"p\"] Block([\n"
      "    D(VarDecl(var_decl(\"v\" Dereference(Var(\"p\" None) None) Structure(\"s\") None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"d\" None) None) Dereference(Var(\"p\" None) None) None)))\n"
      "    S(Expression(FunctionCall(\"g\" [Dereference(Var(\"p\" None) None)] None false)))\n"
      "  ]) FunType([Pointer(Structure(\"s\")) Pointer(Const(Structure(\"s\")))] Void) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [\"p\"] Block([\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Structure(\"s\")) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_struct_copy_from_const_struct (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_struct_copy_from_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  FunctionDecl(function_decl(\"g\" [\"v\"] None FunType([Structure(\"s\")] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"d\" \"p\"] Block([\n"
      "    D(VarDecl(var_decl(\"v\" Dereference(Var(\"p\" None) None) Structure(\"s\") None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"d\" None) None) Dereference(Var(\"p\" None) None) None)))\n"
      "    S(Expression(FunctionCall(\"g\" [Dereference(Var(\"p\" None) None)] None false)))\n"
      "  ]) FunType([Pointer(Structure(\"s\")) Pointer(Const(Structure(\"s\")))] Void) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [\"p\"] Block([\n"
      "    S(Return(Dereference(Var(\"p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Structure(\"s\")) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@0.v\"] None FunType([Structure(\"s\")] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@1.d\" \"@2.p\"] Block([\n"
      "    D(VarDecl(var_decl(\"@3.v\" Dereference(Var(\"@2.p\" None) None) Structure(\"s\") None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"@1.d\" None) None) Dereference(Var(\"@2.p\" None) None) None)))\n"
      "    S(Expression(FunctionCall(\"g\" [Dereference(Var(\"@2.p\" None) None)] None false)))\n"
      "  ]) FunType([Pointer(Structure(\"s\")) Pointer(Const(Structure(\"s\")))] Void) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [\"@4.p\"] Block([\n"
      "    S(Return(Dereference(Var(\"@4.p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Structure(\"s\")) None None [\"\"] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_struct_copy_from_const_struct (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_struct_copy_from_const_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@0.v\\\"] None FunType([Structure(\\\"s\\\")] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.d\\\" \\\"@2.p\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@3.v\\\" Dereference(Var(\\\"@2.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\"))) Structure(\\\"s\\\") None None None None false)))\\n    S(Expression(Assignment(Dereference(Var(\\\"@1.d\\\" Pointer(Structure(\\\"s\\\"))) Structure(\\\"s\\\")) Dereference(Var(\\\"@2.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\"))) Structure(\\\"s\\\"))))\\n    S(Expression(FunctionCall(\\\"g\\\" [Dereference(Var(\\\"@2.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\")))] Void false)))\\n  ]) FunType([Pointer(Structure(\\\"s\\\")) Pointer(Const(Structure(\\\"s\\\")))] Void) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"h\\\" [\\\"@4.p\\\"] Block([\\n    S(Return(Dereference(Var(\\\"@4.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\")))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] Structure(\\\"s\\\")) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"s\" false [member_decl(\"a\" UChar) member_decl(\"b\" Int)]))\n"
      "  FunctionDecl(function_decl(\"g\" [\"@0.v\"] None FunType([Structure(\"s\")] Void) None None [\"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"f\" [\"@1.d\" \"@2.p\"] Block([\n"
      "    D(VarDecl(var_decl(\"@3.v\" Dereference(Var(\"@2.p\" None) None) Structure(\"s\") None None None None false)))\n"
      "    S(Expression(Assignment(Dereference(Var(\"@1.d\" None) None) Dereference(Var(\"@2.p\" None) None) None)))\n"
      "    S(Expression(FunctionCall(\"g\" [Dereference(Var(\"@2.p\" None) None)] None false)))\n"
      "  ]) FunType([Pointer(Structure(\"s\")) Pointer(Const(Structure(\"s\")))] Void) None None [\"\" \"\"] false None [] None false))\n"
      "  FunctionDecl(function_decl(\"h\" [\"@4.p\"] Block([\n"
      "    S(Return(Dereference(Var(\"@4.p\" None) None)))\n"
      "  ]) FunType([Pointer(Const(Structure(\"s\")))] Structure(\"s\")) None None [\"\"] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" Int)]))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@0.v\\\"] None FunType([Structure(\\\"s\\\")] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.d\\\" \\\"@2.p\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@3.v\\\" Dereference(Var(\\\"@2.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\"))) Structure(\\\"s\\\") None None None None false)))\\n    S(Expression(Assignment(Dereference(Var(\\\"@1.d\\\" Pointer(Structure(\\\"s\\\"))) Structure(\\\"s\\\")) Dereference(Var(\\\"@2.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\"))) Structure(\\\"s\\\"))))\\n    S(Expression(FunctionCall(\\\"g\\\" [Dereference(Var(\\\"@2.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\")))] Void false)))\\n  ]) FunType([Pointer(Structure(\\\"s\\\")) Pointer(Const(Structure(\\\"s\\\")))] Void) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"h\\\" [\\\"@4.p\\\"] Block([\\n    S(Return(Dereference(Var(\\\"@4.p\\\" Pointer(Const(Structure(\\\"s\\\")))) Const(Structure(\\\"s\\\")))))\\n  ]) FunType([Pointer(Const(Structure(\\\"s\\\")))] Structure(\\\"s\\\")) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Structure(\\\"s\\\")] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Structure(\\\"s\\\")) Pointer(Const(Structure(\\\"s\\\")))] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"s\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"h\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Pointer(Const(Structure(\\\"s\\\")))] Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@4.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(Structure(\\\"s\\\")))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, "
      "\"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestQualifiedTypes::test_volatile_pointer_steps (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_volatile_pointer_steps\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int arr[4];\\nint *f(void) { int *volatile p = arr; p++; p += 2; return p; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"arr\" None) Volatile(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"p\" None) None)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"p\" None) Constant(ConstInt(2) None) None None)))\n"
      "    S(Return(Var(\"p\" None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestQualifiedTypes::test_volatile_pointer_steps (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_volatile_pointer_steps\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"p\" Var(\"arr\" None) Volatile(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"p\" None) None)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"p\" None) Constant(ConstInt(2) None) None None)))\n"
      "    S(Return(Var(\"p\" None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Var(\"arr\" None) Volatile(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.p\" None) None)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.p\" None) Constant(ConstInt(2) None) None None)))\n"
      "    S(Return(Var(\"@0.p\" None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestQualifiedTypes::test_volatile_pointer_steps (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestQualifiedTypes::test_volatile_pointer_steps\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Int 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.p\\\" AddressOf(Var(\\\"arr\\\" Array(Int 4)) Pointer(Int)) Volatile(Pointer(Int)) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.p\\\" Volatile(Pointer(Int))) Volatile(Pointer(Int)))))\\n    S(Expression(CompoundAssignment(Add Var(\\\"@0.p\\\" Volatile(Pointer(Int))) Constant(ConstInt(2) Int) Volatile(Pointer(Int)) Volatile(Pointer(Int)))))\\n    S(Return(Var(\\\"@0.p\\\" Volatile(Pointer(Int)))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"arr\" None Array(Int 4) None None None None false))\n"
      "  FunctionDecl(function_decl(\"f\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.p\" Var(\"arr\" None) Volatile(Pointer(Int)) None None None None false)))\n"
      "    S(Expression(Postfix(Increment Var(\"@0.p\" None) None)))\n"
      "    S(Expression(CompoundAssignment(Add Var(\"@0.p\" None) Constant(ConstInt(2) None) None None)))\n"
      "    S(Return(Var(\"@0.p\" None)))\n"
      "  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"arr\\\" None Array(Int 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.p\\\" AddressOf(Var(\\\"arr\\\" Array(Int 4)) Pointer(Int)) Volatile(Pointer(Int)) None None None None false)))\\n    S(Expression(Postfix(Increment Var(\\\"@0.p\\\" Volatile(Pointer(Int))) Volatile(Pointer(Int)))))\\n    S(Expression(CompoundAssignment(Add Var(\\\"@0.p\\\" Volatile(Pointer(Int))) Constant(ConstInt(2) Int) Volatile(Pointer(Int)) Volatile(Pointer(Int)))))\\n    S(Return(Var(\\\"@0.p\\\" Volatile(Pointer(Int)))))\\n  ]) FunType([] Pointer(Int)) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Pointer(Int))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestAddressConstants::test_element_of_two_dimensional_array (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_of_two_dimensional_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"extern const unsigned char A[11][14];\\nstatic const unsigned char *const t[] = {\\n    A[5], &A[2][3], A[5] + 2, &A[1][0] + 1, A[5] - 1,\\n};\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\n"
      "  VarDecl(var_decl(\"t\" InitList([Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None) None) Binary(Add Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(2) None) None) Binary(Add AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) None) Constant(ConstInt(1) None) None) Binary(Subtract Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(1) None) None)] None None) Array(Const(Pointer(Const(UChar))) 0) Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestAddressConstants::test_element_of_two_dimensional_array (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_of_two_dimensional_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\n"
      "  VarDecl(var_decl(\"t\" InitList([Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None) None) Binary(Add Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(2) None) None) Binary(Add AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) None) Constant(ConstInt(1) None) None) Binary(Subtract Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(1) None) None)] None None) Array(Const(Pointer(Const(UChar))) 0) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\n"
      "  VarDecl(var_decl(\"t\" InitList([Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None) None) Binary(Add Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(2) None) None) Binary(Add AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) None) Constant(ConstInt(1) None) None) Binary(Subtract Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(1) None) None)] None None) Array(Const(Pointer(Const(UChar))) 0) Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestAddressConstants::test_element_of_two_dimensional_array (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_of_two_dimensional_array\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"A\\\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\\n  VarDecl(var_decl(\\\"t\\\" InitList([AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(2) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(3) Int) Const(UChar)) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(2) Int) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(1) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(0) Int) Const(UChar)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar))) Binary(Subtract AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar)))] Array(Const(Pointer(Const(UChar))) 5) None) Array(Const(Pointer(Const(UChar))) 5) Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\n"
      "  VarDecl(var_decl(\"t\" InitList([Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(2) None) None) Constant(ConstInt(3) None) None) None) Binary(Add Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(2) None) None) Binary(Add AddressOf(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) None) Constant(ConstInt(1) None) None) Binary(Subtract Subscript(Var(\"A\" None) Constant(ConstInt(5) None) None) Constant(ConstInt(1) None) None)] None None) Array(Const(Pointer(Const(UChar))) 0) Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"A\\\" None Array(Array(Const(UChar) 14) 11) Extern None None None false))\\n  VarDecl(var_decl(\\\"t\\\" InitList([AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(2) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(3) Int) Const(UChar)) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(2) Int) Pointer(Const(UChar))) Binary(Add AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(1) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(0) Int) Const(UChar)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar))) Binary(Subtract AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Const(UChar) 14) 11)) Pointer(Array(Const(UChar) 14))) Constant(ConstInt(5) Int) Array(Const(UChar) 14)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Pointer(Const(UChar)))] Array(Const(Pointer(Const(UChar))) 5) None) Array(Const(Pointer(Const(UChar))) 5) Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"A\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Const(UChar) 14) 11)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Pointer(Const(UChar))) 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"A\", \"offset\": 70}}, {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"A\", \"offset\": 31}}, {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"A\", \"offset\": 72}}, {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"A\", \"offset\": 15}}, {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"A\", \"offset\": 69}}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("parse: test_type_checking.py::TestAddressConstants::test_element_through_a_pointer_variable_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_through_a_pointer_variable_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int *p;\\nstatic int *q = &p[1];\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"p\" None) Constant(ConstInt(1) None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestAddressConstants::test_element_through_a_pointer_variable_is_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_through_a_pointer_variable_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"p\" None) Constant(ConstInt(1) None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"p\" None) Constant(ConstInt(1) None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestAddressConstants::test_element_through_a_pointer_variable_is_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_through_a_pointer_variable_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"p\" None Pointer(Int) None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"p\" None) Constant(ConstInt(1) None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object 'q' is not an address constant: Var(name='p', data_type=Pointer(referenced_type=Int()))\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestAddressConstants::test_element_with_a_variable_index_is_rejected (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_with_a_variable_index_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int a[4];\\nint i;\\nstatic int *q = &a[i];\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"i\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"a\" None) Var(\"i\" None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestAddressConstants::test_element_with_a_variable_index_is_rejected (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_with_a_variable_index_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"i\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"a\" None) Var(\"i\" None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"i\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"a\" None) Var(\"i\" None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestAddressConstants::test_element_with_a_variable_index_is_rejected (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_element_with_a_variable_index_is_rejected\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"a\" None Array(Int 4) None None None None false))\n"
      "  VarDecl(var_decl(\"i\" None Int None None None None false))\n"
      "  VarDecl(var_decl(\"q\" AddressOf(Subscript(Var(\"a\" None) Var(\"i\" None) None) None) Pointer(Int) Static None None None false))\n"
      "])\n"
      "output-error\n"
      "{\"type\": \"TypeCheckError\", \"message\": \"initializer for static-storage object 'q' is not a constant expression: expression is not an integer constant expression: Var(name='i', data_type=Int())\"}\n");
}

TEST_CASE("parse: test_type_checking.py::TestAddressConstants::test_member_of_struct (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_member_of_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"struct m { unsigned char x; int m[4]; };\\nstruct m S;\\nstatic int *const u[] = {\\n    &S.m[1], S.m + 3, &(&S)->m[2], &(*&S).m[0],\\n};\\n\"\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"m\" false [member_decl(\"x\" UChar) member_decl(\"m\" Array(Int 4))]))\n"
      "  VarDecl(var_decl(\"S\" None Structure(\"m\") None None None None false))\n"
      "  VarDecl(var_decl(\"u\" InitList([AddressOf(Subscript(Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(1) None) None) None) Binary(Add Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(3) None) None) AddressOf(Subscript(Arrow(AddressOf(Var(\"S\" None) None) \"m\" None) Constant(ConstInt(2) None) None) None) AddressOf(Subscript(Dot(Dereference(AddressOf(Var(\"S\" None) None) None) \"m\" None) Constant(ConstInt(0) None) None) None)] None None) Array(Const(Pointer(Int)) 0) Static None None None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_type_checking.py::TestAddressConstants::test_member_of_struct (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_member_of_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"m\" false [member_decl(\"x\" UChar) member_decl(\"m\" Array(Int 4))]))\n"
      "  VarDecl(var_decl(\"S\" None Structure(\"m\") None None None None false))\n"
      "  VarDecl(var_decl(\"u\" InitList([AddressOf(Subscript(Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(1) None) None) None) Binary(Add Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(3) None) None) AddressOf(Subscript(Arrow(AddressOf(Var(\"S\" None) None) \"m\" None) Constant(ConstInt(2) None) None) None) AddressOf(Subscript(Dot(Dereference(AddressOf(Var(\"S\" None) None) None) \"m\" None) Constant(ConstInt(0) None) None) None)] None None) Array(Const(Pointer(Int)) 0) Static None None None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"m\" false [member_decl(\"x\" UChar) member_decl(\"m\" Array(Int 4))]))\n"
      "  VarDecl(var_decl(\"S\" None Structure(\"m\") None None None None false))\n"
      "  VarDecl(var_decl(\"u\" InitList([AddressOf(Subscript(Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(1) None) None) None) Binary(Add Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(3) None) None) AddressOf(Subscript(Arrow(AddressOf(Var(\"S\" None) None) \"m\" None) Constant(ConstInt(2) None) None) None) AddressOf(Subscript(Dot(Dereference(AddressOf(Var(\"S\" None) None) None) \"m\" None) Constant(ConstInt(0) None) None) None)] None None) Array(Const(Pointer(Int)) 0) Static None None None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_type_checking.py::TestAddressConstants::test_member_of_struct (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_type_checking.py::TestAddressConstants::test_member_of_struct\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"m\\\" false [member_decl(\\\"x\\\" UChar) member_decl(\\\"m\\\" Array(Int 4))]))\\n  VarDecl(var_decl(\\\"S\\\" None Structure(\\\"m\\\") None None None None false))\\n  VarDecl(var_decl(\\\"u\\\" InitList([AddressOf(Subscript(AddressOf(Dot(Var(\\\"S\\\" Structure(\\\"m\\\")) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Pointer(Int)) Binary(Add AddressOf(Dot(Var(\\\"S\\\" Structure(\\\"m\\\")) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(3) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Arrow(AddressOf(Var(\\\"S\\\" Structure(\\\"m\\\")) Pointer(Structure(\\\"m\\\"))) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Dot(Dereference(AddressOf(Var(\\\"S\\\" Structure(\\\"m\\\")) Pointer(Structure(\\\"m\\\"))) Structure(\\\"m\\\")) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Pointer(Int))] Array(Const(Pointer(Int)) 4) None) Array(Const(Pointer(Int)) 4) Static None None None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  StructDecl(struct_decl(\"m\" false [member_decl(\"x\" UChar) member_decl(\"m\" Array(Int 4))]))\n"
      "  VarDecl(var_decl(\"S\" None Structure(\"m\") None None None None false))\n"
      "  VarDecl(var_decl(\"u\" InitList([AddressOf(Subscript(Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(1) None) None) None) Binary(Add Dot(Var(\"S\" None) \"m\" None) Constant(ConstInt(3) None) None) AddressOf(Subscript(Arrow(AddressOf(Var(\"S\" None) None) \"m\" None) Constant(ConstInt(2) None) None) None) AddressOf(Subscript(Dot(Dereference(AddressOf(Var(\"S\" None) None) None) \"m\" None) Constant(ConstInt(0) None) None) None)] None None) Array(Const(Pointer(Int)) 0) Static None None None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"m\\\" false [member_decl(\\\"x\\\" UChar) member_decl(\\\"m\\\" Array(Int 4))]))\\n  VarDecl(var_decl(\\\"S\\\" None Structure(\\\"m\\\") None None None None false))\\n  VarDecl(var_decl(\\\"u\\\" InitList([AddressOf(Subscript(AddressOf(Dot(Var(\\\"S\\\" Structure(\\\"m\\\")) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(1) Int) Int) Pointer(Int)) Binary(Add AddressOf(Dot(Var(\\\"S\\\" Structure(\\\"m\\\")) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(3) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Arrow(AddressOf(Var(\\\"S\\\" Structure(\\\"m\\\")) Pointer(Structure(\\\"m\\\"))) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(2) Int) Int) Pointer(Int)) AddressOf(Subscript(AddressOf(Dot(Dereference(AddressOf(Var(\\\"S\\\" Structure(\\\"m\\\")) Pointer(Structure(\\\"m\\\"))) Structure(\\\"m\\\")) \\\"m\\\" Array(Int 4)) Pointer(Int)) Constant(ConstInt(0) Int) Int) Pointer(Int))] Array(Const(Pointer(Int)) 4) None) Array(Const(Pointer(Int)) 4) Static None None None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"S\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Structure(\\\"m\\\")\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Pointer(Int)) 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"S\", \"offset\": 3}}, {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"S\", \"offset\": 7}}, {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"S\", \"offset\": 5}}, {\"object\": \"passes.type_checking.AddressInit\", \"fields\": {\"name\": \"S\", \"offset\": 1}}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"m\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"m\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"x\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"m\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Int 4)\\n\"}, \"byte_offset\": 1}}], \"size\": 9, \"complete\": true}}]]}}}]}\n");
}
