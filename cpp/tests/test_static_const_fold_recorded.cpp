// The calls that the tests of tests/test_static_const_fold.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_constant_fold: test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_indexed_load_with_runtime_index_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_constant_fold\n"
      "source tests/test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_indexed_load_with_runtime_index_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Var(\"%i\") Var(\"%t\") false 0 0)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Var(\"%i\") Var(\"%t\") false 0 0)\n"
      "])\n");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_non_const_array_element_type_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_constant_fold\n"
      "source tests/test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_non_const_array_element_type_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UInt 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [256, 512, 768]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%t\") false 0 0)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%t\") false 0 0)\n"
      "])\n");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_out_of_bounds_index_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_constant_fold\n"
      "source tests/test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_out_of_bounds_index_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [256, 512]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%t\") false 0 0)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%t\") false 0 0)\n"
      "])\n");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_unaligned_byte_index_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_constant_fold\n"
      "source tests/test_static_const_fold.py::TestConstArraySubscriptFoldUnit::test_unaligned_byte_index_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [4660, 22136]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(1)) Var(\"%t\") false 0 0)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(1)) Var(\"%t\") false 0 0)\n"
      "])\n");
}

TEST_CASE("tac_reassoc_constants: test_static_const_fold.py::TestReassocConstantsUnit::test_different_widths_do_not_combine (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_reassoc_constants\n"
      "source tests/test_static_const_fold.py::TestReassocConstantsUnit::test_different_widths_do_not_combine\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"%inner\", \"%outer\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUChar(5)) Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(256)) Var(\"%inner\") Var(\"%outer\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUChar(5)) Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(256)) Var(\"%inner\") Var(\"%outer\"))\n"
      "])\n");
}

TEST_CASE("tac_reassoc_constants: test_static_const_fold.py::TestReassocConstantsUnit::test_inner_not_an_add_does_not_fuse (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_reassoc_constants\n"
      "source tests/test_static_const_fold.py::TestReassocConstantsUnit::test_inner_not_an_add_does_not_fuse\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"%inner\", \"%outer\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(256)) Var(\"%inner\") Var(\"%outer\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(256)) Var(\"%inner\") Var(\"%outer\"))\n"
      "])\n");
}

TEST_CASE("tac_reassoc_constants: test_static_const_fold.py::TestReassocConstantsUnit::test_multi_use_inner_does_not_fuse (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_reassoc_constants\n"
      "source tests/test_static_const_fold.py::TestReassocConstantsUnit::test_multi_use_inner_does_not_fuse\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"%inner\", \"%other\", \"%outer\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(256)) Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(512)) Var(\"%inner\") Var(\"%outer\"))\n"
      "  Copy(Var(\"%inner\") Var(\"%other\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(256)) Var(\"v\") Var(\"%inner\"))\n"
      "  Binary(Add Constant(ConstUInt(512)) Var(\"%inner\") Var(\"%outer\"))\n"
      "  Copy(Var(\"%inner\") Var(\"%other\"))\n"
      "])\n");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.py::TestMultiDimConstArrayFoldUnit::test_2d_misaligned_byte_idx_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_constant_fold\n"
      "source tests/test_static_const_fold.py::TestMultiDimConstArrayFoldUnit::test_2d_misaligned_byte_idx_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Const(UInt) 3) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2, 3]}, {\"tuple\": [4, 5, 6]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(3)) Var(\"%t\") false 0 0)\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(3)) Var(\"%t\") false 0 0)\n"
      "])\n");
}

TEST_CASE("parse: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n            static const unsigned char M[3][2] = {\\n                {1, 2}, {3, 4}, {5, 6}\\n            };\\n            int main(void) { return M[1][1]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"M\\\" InitList([InitList([Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(2) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(6) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 3) None) Array(Array(Const(UChar) 2) 3) Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Subscript(AddressOf(Subscript(AddressOf(Var(\\\"M\\\" Array(Array(Const(UChar) 2) 3)) Pointer(Array(Const(UChar) 2))) Constant(ConstInt(1) Int) Array(Const(UChar) 2)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None) InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None)] None None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Var(\"M\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"M\\\" InitList([InitList([Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(2) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(6) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 3) None) Array(Array(Const(UChar) 2) 3) Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Subscript(AddressOf(Subscript(AddressOf(Var(\\\"M\\\" Array(Array(Const(UChar) 2) 3)) Pointer(Array(Const(UChar) 2))) Constant(ConstInt(1) Int) Array(Const(UChar) 2)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"M\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Const(UChar) 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}, {\"tuple\": [5, 6]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_2d_all_constant_emits_indexed_load\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"M\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Const(UChar) 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}, {\"tuple\": [5, 6]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"M\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Const(UChar) 2) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}, {\"tuple\": [5, 6]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"M\" InitList([InitList([Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(2) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(6) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 3) None) Array(Array(Const(UChar) 2) 3) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Int Subscript(AddressOf(Subscript(AddressOf(Var(\"M\" Array(Array(Const(UChar) 2) 3)) Pointer(Array(Const(UChar) 2))) Constant(ConstInt(1) Int) Array(Const(UChar) 2)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar)) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    IndexedLoad(\"M\" Constant(ConstUChar(3)) Var(\"%0\") false 0 0)\n"
      "    ZeroExtend(Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "  StaticVariable(\"M\" false Pointer [StringInit(\"\\x01\\x02\" 2) StringInit(\"\\x03\\x04\" 2) StringInit(\"\\x05\\x06\" 2)])\n"
      "])\n");
}

TEST_CASE("parse: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass parse\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input-json\n"
      "\"\\n            static const unsigned char A[2][2][2] = {\\n                {{1,2},{3,4}}, {{5,6},{7,8}}\\n            };\\n            int main(void) { return A[1][0][1]; }\\n\"\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("identifier_resolution: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass identifier_resolution\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("string_lifting: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass string_lifting\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("label_resolution: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass label_resolution\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("loop_labeling: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass loop_labeling\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n");
}

TEST_CASE("type_checking: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass type_checking\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"input\": {\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"A\\\" InitList([InitList([InitList([Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(2) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 2) None) InitList([InitList([Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(6) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(7) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(8) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 2) None)] Array(Array(Array(Const(UChar) 2) 2) 2) None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Subscript(AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Array(Const(UChar) 2) 2) 2)) Pointer(Array(Array(Const(UChar) 2) 2))) Constant(ConstInt(1) Int) Array(Array(Const(UChar) 2) 2)) Pointer(Array(Const(UChar) 2))) Constant(ConstInt(0) Int) Array(Const(UChar) 2)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, \"args\": {}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Constant(ConstInt(1) None) Constant(ConstInt(2) None)] None None) InitList([Constant(ConstInt(3) None) Constant(ConstInt(4) None)] None None)] None None) InitList([InitList([Constant(ConstInt(5) None) Constant(ConstInt(6) None)] None None) InitList([Constant(ConstInt(7) None) Constant(ConstInt(8) None)] None None)] None None)] None None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Subscript(Subscript(Subscript(Var(\"A\" None) Constant(ConstInt(1) None) None) Constant(ConstInt(0) None) None) Constant(ConstInt(1) None) None)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"A\\\" InitList([InitList([InitList([Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(2) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 2) None) InitList([InitList([Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(6) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(7) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(8) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 2) None)] Array(Array(Array(Const(UChar) 2) 2) 2) None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Subscript(AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"A\\\" Array(Array(Array(Const(UChar) 2) 2) 2)) Pointer(Array(Array(Const(UChar) 2) 2))) Constant(ConstInt(1) Int) Array(Array(Const(UChar) 2) 2)) Pointer(Array(Const(UChar) 2))) Constant(ConstInt(0) Int) Array(Const(UChar) 2)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"A\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Array(Const(UChar) 2) 2) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}]}, {\"tuple\": [{\"tuple\": [5, 6]}, {\"tuple\": [7, 8]}]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}]}\n");
}

TEST_CASE("c99_to_tac: test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass c99_to_tac\n"
      "source tests/test_static_const_fold.py::TestMultiDimRecognizerEmits::test_3d_all_constant_emits_indexed_load\n"
      "args {\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"A\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Array(Const(UChar) 2) 2) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}]}, {\"tuple\": [{\"tuple\": [5, 6]}, {\"tuple\": [7, 8]}]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"A\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Array(Const(UChar) 2) 2) 2)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [{\"tuple\": [1, 2]}, {\"tuple\": [3, 4]}]}, {\"tuple\": [{\"tuple\": [5, 6]}, {\"tuple\": [7, 8]}]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}\n"
      "input\n"
      "Program([\n"
      "  VarDecl(var_decl(\"A\" InitList([InitList([InitList([Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(2) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(3) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(4) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 2) None) InitList([InitList([Cast(Const(UChar) Constant(ConstInt(5) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(6) Int) Const(UChar))] Array(Const(UChar) 2) None) InitList([Cast(Const(UChar) Constant(ConstInt(7) Int) Const(UChar)) Cast(Const(UChar) Constant(ConstInt(8) Int) Const(UChar))] Array(Const(UChar) 2) None)] Array(Array(Const(UChar) 2) 2) None)] Array(Array(Array(Const(UChar) 2) 2) 2) None) Array(Array(Array(Const(UChar) 2) 2) 2) Static None None None false))\n"
      "  FunctionDecl(function_decl(\"main\" [] Block([\n"
      "    S(Return(Cast(Int Subscript(AddressOf(Subscript(AddressOf(Subscript(AddressOf(Var(\"A\" Array(Array(Array(Const(UChar) 2) 2) 2)) Pointer(Array(Array(Const(UChar) 2) 2))) Constant(ConstInt(1) Int) Array(Array(Const(UChar) 2) 2)) Pointer(Array(Const(UChar) 2))) Constant(ConstInt(0) Int) Array(Const(UChar) 2)) Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar)) Int)))\n"
      "  ]) FunType([] Int) None None [] false None [] None false))\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    IndexedLoad(\"A\" Constant(ConstUChar(5)) Var(\"%0\") false 0 0)\n"
      "    ZeroExtend(Var(\"%0\") Var(\"%1\"))\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "  StaticVariable(\"A\" false Pointer [StringInit(\"\\x01\\x02\" 2) StringInit(\"\\x03\\x04\" 2) StringInit(\"\\x05\\x06\" 2) StringInit(\"\\x07\\x08\" 2)])\n"
      "])\n");
}
