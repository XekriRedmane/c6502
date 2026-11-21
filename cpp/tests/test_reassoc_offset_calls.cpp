// The calls of `tac_reassoc_offset_constant`
// (passes/optimization/reassoc_offset.py) that the Python unit tests
// make, each with the function the test gives the pass, the symbols the
// call read and the SSA names, and what the Python pass returned and
// changed, which the C++ pass must return and change too. They were
// recorded by wrapping the Python passes while
// tests/test_constant_folding.py, test_strength_reduction.py,
// test_static_const_fold.py, test_reassoc_offset_licm.py,
// test_split_tables.py, test_pointer_member_access.py, and
// test_narrow_byte_range.py ran, leaving out each call whose record
// takes more than 15 KB. Every call that TestReassocOffset in
// tests/test_reassoc_offset_licm.py makes is here, and of the other
// tests the calls that changed their function. A test that calls the
// pass several times gives a case for each distinct call.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_case;

TEST_CASE("tac_reassoc_offset_constant: test_reassoc_offset_licm.TestReassocLicmSim.test_header_fields_after_variable_skip (1)") {
  check_stage_case(
      "tac_reassoc_offset_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%13.1.sr0\", \"%13.1.sr1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.d\", \"@1.n.1\"]}}}",
      "Function(\"f\" true [\"@0.d\"] [\n"
      "  Binary(Add Var(\"@0.d\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"%2.1\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Multiply Var(\"%6.1\") Constant(ConstInt(100)) Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%2.1\") Constant(ConstInt(2)) Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "  Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%2.1\") Constant(ConstInt(3)) Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Load(Var(\"%17.1\") Var(\"%18.1\") false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.d\"] [\n"
      "  Binary(Add Var(\"@0.d\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%2.1\") Var(\"%4.1.ra\"))\n"
      "  Binary(Add Var(\"%4.1.ra\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Multiply Var(\"%6.1\") Constant(ConstInt(100)) Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%2.1\") Var(\"%10.1.ra\"))\n"
      "  Binary(Add Var(\"%10.1.ra\") Constant(ConstInt(2)) Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "  Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%2.1\") Var(\"%17.1.ra\"))\n"
      "  Binary(Add Var(\"%17.1.ra\") Constant(ConstInt(3)) Var(\"%17.1\"))\n"
      "  Load(Var(\"%17.1\") Var(\"%18.1\") false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1.ra\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1.ra\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1.ra\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%10.1.ra\", \"%11.1\", \"%12.1\", \"%13.1\", \"%13.1.sr0\", \"%13.1.sr1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%17.1.ra\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%3.1\", \"%4.1\", \"%4.1.ra\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.d\", \"@1.n.1\"]}}}");
}

TEST_CASE("tac_reassoc_offset_constant: test_reassoc_offset_licm.TestReassocOffset.test_constant_moves_out (1)") {
  check_stage_case(
      "tac_reassoc_offset_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"off\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"addr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"addr\", \"b\", \"off\", \"p\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(3)) Var(\"off\"))\n"
      "  Binary(Add Var(\"p\") Var(\"off\") Var(\"addr\"))\n"
      "  Load(Var(\"addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Var(\"x\") Var(\"addr.ra\"))\n"
      "  Binary(Add Var(\"addr.ra\") Constant(ConstInt(3)) Var(\"addr\"))\n"
      "  Load(Var(\"addr\") Var(\"b\") false)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"off\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"addr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"addr.ra\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"addr\", \"addr.ra\", \"b\", \"off\", \"p\", \"x\"]}}}");
}

TEST_CASE("tac_reassoc_offset_constant: test_reassoc_offset_licm.TestReassocOffset.test_different_widths_left_alone (1)") {
  check_stage_case(
      "tac_reassoc_offset_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"off\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"addr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"addr\", \"b\", \"off\", \"p\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x\") Constant(ConstLong(3)) Var(\"off\"))\n"
      "  Binary(Add Var(\"p\") Var(\"off\") Var(\"addr\"))\n"
      "  Load(Var(\"addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x\") Constant(ConstLong(3)) Var(\"off\"))\n"
      "  Binary(Add Var(\"p\") Var(\"off\") Var(\"addr\"))\n"
      "  Load(Var(\"addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_reassoc_offset_constant: test_reassoc_offset_licm.TestReassocOffset.test_multi_use_offset_left_alone (1)") {
  check_stage_case(
      "tac_reassoc_offset_constant",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"addr\", \"b\", \"c\", \"off\", \"p\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(3)) Var(\"off\"))\n"
      "  Binary(Add Var(\"p\") Var(\"off\") Var(\"addr\"))\n"
      "  Load(Var(\"addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"off\") Var(\"c\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(3)) Var(\"off\"))\n"
      "  Binary(Add Var(\"p\") Var(\"off\") Var(\"addr\"))\n"
      "  Load(Var(\"addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"off\") Var(\"c\"))\n"
      "])\n",
      "");
}
