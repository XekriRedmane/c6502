// The calls of `tac_narrow_byte_bitwise` that the Python unit tests
// make, each with the function the test gives the pass, the symbols the
// call read and the SSA names, and what the Python pass returned and
// changed, which the C++ pass must return and change too. They were
// recorded by wrapping the Python passes while
// tests/test_cmp_zero_jump_fold.py, test_and_zero_jump_fold.py,
// test_lnot_jump_fold.py, test_narrow_byte_bitwise.py,
// test_narrow_byte_range.py, test_narrow_widened_arith.py,
// test_flag_compares.py, test_truncate_extend_fold.py, and test_bool.py
// ran, leaving out each call whose record takes more than 15 KB. They
// are the calls through the driver that changed their function or the
// symbols, and every call the tests make of the pass on its own. A test
// that calls the pass several times gives a case for each distinct
// call. A static that a synthetic test gives no initial value, which
// the codec can't read, has a NoInitializer in its place.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_case;

TEST_CASE("tac_narrow_byte_bitwise: test_bool.TestPrograms.test_asm_sim (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.b.1\", \"@0.b.2\", \"@0.b.3\", \"@0.b.4\", \"@0.b.5\", \"@1.r.1\", \"@1.r.2\", \"@1.r.3\", \"@1.r.4\"]}}}",
      "Function(\"main\" true [] [\n"
      "  ZeroExtend(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(2)) Var(\"%1.1\"))\n"
      "  Binary(NotEqual Var(\"%1.1\") Constant(ConstInt(0)) Var(\"%2.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(Subtract Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Binary(NotEqual Var(\"%5.1\") Constant(ConstInt(0)) Var(\"%6.1\"))\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(LeftShift Var(\"%7.1\") Constant(ConstInt(1)) Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%3.1\") Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Constant(ConstInt(4)) Var(\"%11.1\"))\n"
      "  Binary(NotEqual Var(\"%11.1\") Constant(ConstInt(0)) Var(\"%12.1\"))\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Binary(LeftShift Var(\"%13.1\") Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%9.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Binary(Multiply Var(\"%13.1\") Constant(ConstInt(0)) Var(\"%17.1\"))\n"
      "  Binary(NotEqual Var(\"%17.1\") Constant(ConstInt(0)) Var(\"%18.1\"))\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(LeftShift Var(\"%19.1\") Constant(ConstInt(3)) Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Ret(Var(\"%21.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  ZeroExtend(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(2)) Var(\"%1.1\"))\n"
      "  Binary(NotEqual Var(\"%1.1\") Constant(ConstInt(0)) Var(\"%2.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(Subtract Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Binary(NotEqual Var(\"%5.1\") Constant(ConstInt(0)) Var(\"%6.1\"))\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(LeftShift Var(\"%7.1\") Constant(ConstInt(1)) Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%3.1\") Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Binary(BitwiseOr Var(\"%6.1\") Constant(ConstUChar(4)) Var(\"%11.1.n0\"))\n"
      "  ZeroExtend(Var(\"%11.1.n0\") Var(\"%11.1\"))\n"
      "  Binary(NotEqual Var(\"%11.1\") Constant(ConstInt(0)) Var(\"%12.1\"))\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Binary(LeftShift Var(\"%13.1\") Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%9.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Binary(Multiply Var(\"%13.1\") Constant(ConstInt(0)) Var(\"%17.1\"))\n"
      "  Binary(NotEqual Var(\"%17.1\") Constant(ConstInt(0)) Var(\"%18.1\"))\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(LeftShift Var(\"%19.1\") Constant(ConstInt(3)) Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Ret(Var(\"%21.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%11.1.n0\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.b.1\", \"@0.b.2\", \"@0.b.3\", \"@0.b.4\", \"@0.b.5\", \"@1.r.1\", \"@1.r.2\", \"@1.r.3\", \"@1.r.4\"]}}}");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestEndToEnd.test_byte_compares (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%3.1.u0\", \"%4.1\", \"%5.1\", \"@0.q\", \"@1.lim\"]}}}",
      "Function(\"below\" true [\"@0.q\" \"@1.lim\"] [\n"
      "  Binary(Add Var(\"@0.q\") Constant(ConstInt(6)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%2.1\") Constant(ConstInt(63)) Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"@1.lim\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"%3.1.u0\"))\n"
      "  Binary(LessThan Var(\"%3.1.u0\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Ret(Var(\"%5.1\"))\n"
      "])\n",
      "Function(\"below\" true [\"@0.q\" \"@1.lim\"] [\n"
      "  Binary(Add Var(\"@0.q\") Constant(ConstInt(6)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%1.1\") Constant(ConstUChar(63)) Var(\"%3.1.n0\"))\n"
      "  ZeroExtend(Var(\"%3.1.n0\") Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"@1.lim\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"%3.1.u0\"))\n"
      "  Binary(LessThan Var(\"%3.1.u0\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Ret(Var(\"%5.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%3.1.n0\", \"%3.1.u0\", \"%4.1\", \"%5.1\", \"@0.q\", \"@1.lim\"]}}}");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_and_of_sign_extend_and_small_constant (standalone) (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"se\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"se\") Constant(ConstInt(127)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"s\") Constant(ConstUChar(127)) Var(\"r.n0\"))\n"
      "  ZeroExtend(Var(\"r.n0\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"se\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"r.n0\", \"s\", \"se\", \"w\", \"ze\"]}}}");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_and_of_zero_extend_and_constant (standalone) (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"ze\") Constant(ConstInt(319)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"b\") Constant(ConstUChar(63)) Var(\"r.n0\"))\n"
      "  ZeroExtend(Var(\"r.n0\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"r.n0\", \"s\", \"se\", \"w\", \"ze\"]}}}");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_and_of_zero_extend_and_wide_var_truncates_it (standalone) (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"w\") Var(\"ze\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Truncate(Var(\"w\") Var(\"r.lo0\"))\n"
      "  Binary(BitwiseAnd Var(\"r.lo0\") Var(\"b\") Var(\"r.n0\"))\n"
      "  ZeroExtend(Var(\"r.n0\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.lo0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"r.lo0\", \"r.n0\", \"s\", \"se\", \"w\", \"ze\"]}}}");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_and_with_neither_high_byte_zero_stays (standalone) (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"se\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"se\") Constant(ConstInt(-2)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"se\") Constant(ConstInt(-2)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"ze\") Var(\"w\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"ze\") Var(\"w\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (2)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"ze\") Var(\"w\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"ze\") Var(\"w\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (3)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"ze\") Constant(ConstInt(-1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"ze\") Constant(ConstInt(-1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (4)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"ze\") Constant(ConstInt(128)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"b\") Constant(ConstUChar(128)) Var(\"r.n0\"))\n"
      "  ZeroExtend(Var(\"r.n0\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"r.n0\", \"s\", \"se\", \"w\", \"ze\"]}}}");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (5)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"ze\") Constant(ConstInt(256)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"ze\") Constant(ConstInt(256)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (6)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"ze\") Constant(ConstInt(-1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"ze\") Constant(ConstInt(-1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (7)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"ze\") Constant(ConstInt(128)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"b\") Constant(ConstUChar(128)) Var(\"r.n0\"))\n"
      "  ZeroExtend(Var(\"r.n0\") Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"r.n0\", \"s\", \"se\", \"w\", \"ze\"]}}}");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (8)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ze\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"ze\") Constant(ConstInt(256)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"ze\") Constant(ConstInt(256)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (9)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"se\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"se\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseOr Var(\"se\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_or_needs_both_high_bytes_zero (standalone) (10)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"se\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"se\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseXor Var(\"se\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_bitwise.TestRule.test_without_an_extend_stays (standalone) (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"b\", \"r\", \"s\", \"se\", \"w\", \"ze\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"w\") Constant(ConstInt(63)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"ze\"))\n"
      "  SignExtend(Var(\"s\") Var(\"se\"))\n"
      "  Binary(BitwiseAnd Var(\"w\") Constant(ConstInt(63)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_byte_bitwise: test_narrow_byte_range.TestLowerings.test_shift_of_promoted_byte_is_logical (1)") {
  check_stage_case(
      "tac_narrow_byte_bitwise",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.nb0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.nb0\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%6.1.lo0\", \"%6.1.nw0\", \"@0.p\", \"@1.s\"]}}}",
      "Function(\"f\" true [\"@0.p\" \"@1.s\"] [\n"
      "  ZeroExtend(Var(\"@0.p\") Var(\"%0.1\"))\n"
      "  ZeroExtend(Var(\"@1.s\") Var(\"%1.1\"))\n"
      "  Binary(RightShift Var(\"@1.s\") Constant(ConstInt(5)) Var(\"%2.1.nb0\"))\n"
      "  ZeroExtend(Var(\"%2.1.nb0\") Var(\"%2.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%2.1\") Constant(ConstInt(3)) Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%6.1.lo0\"))\n"
      "  ZeroExtend(Var(\"%6.1.lo0\") Var(\"%6.1.nw0\"))\n"
      "  Binary(Multiply Var(\"%0.1\") Var(\"%6.1.nw0\") Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.p\" \"@1.s\"] [\n"
      "  ZeroExtend(Var(\"@0.p\") Var(\"%0.1\"))\n"
      "  ZeroExtend(Var(\"@1.s\") Var(\"%1.1\"))\n"
      "  Binary(RightShift Var(\"@1.s\") Constant(ConstInt(5)) Var(\"%2.1.nb0\"))\n"
      "  ZeroExtend(Var(\"%2.1.nb0\") Var(\"%2.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%2.1.nb0\") Constant(ConstUChar(3)) Var(\"%3.1.n0\"))\n"
      "  ZeroExtend(Var(\"%3.1.n0\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%6.1.lo0\"))\n"
      "  ZeroExtend(Var(\"%6.1.lo0\") Var(\"%6.1.nw0\"))\n"
      "  Binary(Multiply Var(\"%0.1\") Var(\"%6.1.nw0\") Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.nb0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.nb0\", \"%3.1\", \"%3.1.n0\", \"%4.1\", \"%5.1\", \"%6.1\", \"%6.1.lo0\", \"%6.1.nw0\", \"@0.p\", \"@1.s\"]}}}");
}
