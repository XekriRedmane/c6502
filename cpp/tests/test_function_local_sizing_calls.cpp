// The calls that the tests of tests/test_function_local_sizing.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("parse: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"int main(void) {   int x = 5;   int y = x + 1;   return y; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"y\" Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"y\" Binary(Add Var(\"x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@1.y\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) None) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" None) Constant(ConstInt(1) None) None) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@1.y\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    D(VarDecl(var_decl(\"@0.x\" Constant(ConstInt(5) Int) Int None None None None false)))\n"
      "    D(VarDecl(var_decl(\"@1.y\" Binary(Add Var(\"@0.x\" Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\n"
      "    S(Return(Var(\"@1.y\" Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(5)) Var(\"@0.x\"))\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.y\"))\n"
      "    Ret(Var(\"@1.y\"))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("optimize_tac: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass optimize_tac\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(5)) Var(\"@0.x\"))\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.y\"))\n"
      "    Ret(Var(\"@1.y\"))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(6)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("select_abi: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 9)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass select_abi\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.x\\\" Constant(ConstInt(5) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@1.y\\\" Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int) Int None None None None false)))\\n    S(Return(Var(\\\"@1.y\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(6)))\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n");
}

TEST_CASE("allocate_zp_slots: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 10)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass allocate_zp_slots\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(6)))\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}, {\"dict\": []}]}\n");
}

TEST_CASE("tac_to_asm: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 11)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"bare_exit\": true, \"abi\": {\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(6)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(6) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("asm_optimizer: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 12)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass asm_optimizer\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {\"extra_statics\": {\"set\": []}, \"param_layouts\": {\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}, \"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.y.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(6) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"asm_ast\", \"dump\": \"Program([\\n  Function(\\\"main\\\" true [] [\\n    Label(\\\".main@asm_ssa_block@0\\\")\\n    Mov(Imm(0) Reg(A) false)\\n    Mov(Reg(A) Reg(X) false)\\n    Mov(Imm(6) Reg(A) false)\\n    Return(true true false)\\n  ])\\n])\\n\"}, {\"dict\": [[\"main\", {\"object\": \"passes.optimization.register_allocation.Coloring\", \"fields\": {\"assignments\": {\"dict\": []}, \"spilled\": {\"set\": []}, \"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}, \"hwreg_assignments\": {\"dict\": []}}}]]}]}\n");
}

TEST_CASE("compute_local_bytes: test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring (call 13)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass compute_local_bytes\n"
      "source tests/test_function_local_sizing.py::TestComputeLocalBytesEndToEnd::test_simple_function_byte_count_matches_coloring\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@asm_ssa_block@0\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(6) Reg(A) false)\n"
      "    Return(true true false)\n"
      "  ])\n"
      "])\n"
      "output-json\n"
      "{\"dict\": [[\"main\", 0]]}\n");
}
