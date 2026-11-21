// The calls of `tac_fold_static_const_reads`
// (passes/optimization/static_const_fold.py) that the Python unit tests
// make, each with the function the test gives the pass, the symbols the
// call read and the SSA names, and what the Python pass returned and
// changed, which the C++ pass must return and change too. They were
// recorded by wrapping the Python passes while
// tests/test_constant_folding.py, test_strength_reduction.py,
// test_static_const_fold.py, test_reassoc_offset_licm.py,
// test_split_tables.py, test_pointer_member_access.py, and
// test_narrow_byte_range.py ran, leaving out each call whose record
// takes more than 15 KB. Every call that
// tests/test_static_const_fold.py makes is here, and of the other tests
// the calls that changed their function. A test that calls the pass
// several times gives a case for each distinct call.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_case;

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstFoldAsmShape.test_add_reassoc_combines_constants (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 100}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 200}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.col\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.col\"]}}}",
      "Function(\"main\" true [\"@0.col\"] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"@0.col\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"main\" true [\"@0.col\"] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Add Constant(ConstInt(100)) Constant(ConstInt(200)) Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"@0.col\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstFoldAsmShape.test_const_array_subscript_with_constant_index_folds (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [4660, 22136, 39612]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%0.1\") false 0 0)\n"
      "  Copy(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%0.1\") false 0 0)\n"
      "  Copy(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstFoldAsmShape.test_scalar_const_static_collapses_to_immediate (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"magic\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4660}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Add Var(\"magic\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Add Constant(ConstInt(4660)) Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstFoldCorrectness.test_pointer_const_static_indexed_write (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"buf\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 16384}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"offsets\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [256, 512, 768]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%3.1\", \"%4.1\", \"%5.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(66)) Var(\"%0.1\"))\n"
      "  IndexedLoad(\"offsets\" Constant(ConstUChar(2)) Var(\"%1.1\") false 0 0)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUInt(5)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"buf\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Store(Var(\"%0.1\") Var(\"%5.1\") false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(66)) Var(\"%0.1\"))\n"
      "  IndexedLoad(\"offsets\" Constant(ConstUChar(2)) Var(\"%1.1\") false 0 0)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUInt(5)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Constant(ConstUInt(16384)) Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Store(Var(\"%0.1\") Var(\"%5.1\") false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_aggregate_initial_value_does_not_fold (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Int) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 3]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"arr\") Var(\"%t\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"arr\") Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_const_int_static_var_folds_to_constant (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"magic\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 4660}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"magic\") Var(\"y\") Var(\"%t\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstInt(4660)) Var(\"y\") Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_const_pointer_static_folds_via_uint (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Pointer(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 8192}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"p\") Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(8192)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_def_position_var_not_substituted (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 5}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"y\") Var(\"x\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"y\") Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_indexed_load_name_field_preserved (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [1, 2, 3]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"idx\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 2}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Var(\"idx\") Var(\"%t\") false 0 0)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(2)) Var(\"%t\") false 0 0)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_local_attr_does_not_fold (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"x\") Var(\"%t\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"x\") Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_no_initializer_does_not_fold (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Const(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"x\") Var(\"%t\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"x\") Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_static_const_reads: test_static_const_fold.TestStaticConstReadsFoldUnit.test_non_const_static_does_not_fold (1)") {
  check_stage_case(
      "tac_fold_static_const_reads",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 42}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"x\") Var(\"%t\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"x\") Var(\"%t\"))\n"
      "])\n",
      "");
}
