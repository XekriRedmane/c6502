// The calls of `tac_constant_fold`
// (passes/optimization/constant_folding.py) that the Python unit tests
// make, each with the function the test gives the pass, the symbols the
// call read and the SSA names, and what the Python pass returned and
// changed, which the C++ pass must return and change too. They were
// recorded by wrapping the Python passes while
// tests/test_constant_folding.py, test_strength_reduction.py,
// test_static_const_fold.py, test_reassoc_offset_licm.py,
// test_split_tables.py, test_pointer_member_access.py, and
// test_narrow_byte_range.py ran, leaving out each call whose record
// takes more than 15 KB. Every call that tests/test_constant_folding.py
// makes is here, and of the other tests the calls that changed their
// function. A test that calls the pass several times gives a case for
// each distinct call.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_case;

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_add_basic (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstInt(2)) Constant(ConstInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_add_overflow_wraps (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstInt(32767)) Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-32768)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_binary_mismatched_widths_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstInt(1)) Constant(ConstLong(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstInt(1)) Constant(ConstLong(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_binary_with_one_var_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstInt(1)) Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstInt(1)) Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_bitwise_and_or_xor (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(BitwiseAnd Constant(ConstInt(12)) Constant(ConstInt(10)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(8)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_bitwise_and_or_xor (2)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(BitwiseOr Constant(ConstInt(12)) Constant(ConstInt(10)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(14)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_bitwise_and_or_xor (3)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(BitwiseXor Constant(ConstInt(12)) Constant(ConstInt(10)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(6)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_bitwise_on_float_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(BitwiseAnd Constant(ConstFloat(1065353216)) Constant(ConstFloat(1073741824)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(BitwiseAnd Constant(ConstFloat(1065353216)) Constant(ConstFloat(1073741824)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_divide_by_zero_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Divide Constant(ConstInt(7)) Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Divide Constant(ConstInt(7)) Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_divide_truncates_toward_zero (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Divide Constant(ConstInt(-7)) Constant(ConstInt(2)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_long_long_arithmetic (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstLongLong(100000)) Constant(ConstLongLong(200000)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLongLong(300000)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_modulo_by_zero_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Modulo Constant(ConstInt(7)) Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Modulo Constant(ConstInt(7)) Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_modulo_dividend_signed (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Modulo Constant(ConstInt(-7)) Constant(ConstInt(2)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_modulo_on_float_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Modulo Constant(ConstFloat(1065353216)) Constant(ConstFloat(1073741824)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Modulo Constant(ConstFloat(1065353216)) Constant(ConstFloat(1073741824)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_mul_overflow_wraps (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Multiply Constant(ConstInt(256)) Constant(ConstInt(256)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldBinaryArith.test_sub_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Subtract Constant(ConstLong(1000)) Constant(ConstLong(7)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLong(993)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldComparison.test_equal_false (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Equal Constant(ConstLong(7)) Constant(ConstLong(8)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldComparison.test_equal_true (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Equal Constant(ConstLong(7)) Constant(ConstLong(7)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldComparison.test_greater_or_equal_boundary (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(GreaterOrEqual Constant(ConstInt(3)) Constant(ConstInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldComparison.test_greater_than (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(GreaterThan Constant(ConstInt(5)) Constant(ConstInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldComparison.test_less_or_equal_boundary (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessOrEqual Constant(ConstInt(3)) Constant(ConstInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldComparison.test_less_than_signed (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessThan Constant(ConstInt(-1)) Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldComparison.test_not_equal (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(NotEqual Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_matching_variant_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_signed_long_long_to_unsigned_long_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLongLong(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstULongLong(18446744073709551615)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_signed_long_to_unsigned_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLong(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstULong(4294967295)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_signed_to_unsigned_negative_canonicalizes (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(65535)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_signed_to_unsigned_same_width (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_skip_fp_dst (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1065353216)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1065353216)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_skip_unknown_var (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_skip_width_mismatch (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_skip_with_var_src (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_skip_without_symbols (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_uchar_dst_with_char_const_rewraps (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstChar(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUChar(255)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_uchar_dst_with_uchar_const_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUChar(42)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUChar(42)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldCopy.test_unsigned_to_signed_high_bit_set (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(40000)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-25536)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_double_div (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Divide Constant(ConstDouble(4607182418800017408)) Constant(ConstDouble(4611686018427387904)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstDouble(4602678819172646912)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_double_sub (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Subtract Constant(ConstDouble(4617315517961601024)) Constant(ConstDouble(4611686018427387904)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstDouble(4613937818241073152)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_mismatched_fp_precision_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstFloat(1065353216)) Constant(ConstDouble(4611686018427387904)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstFloat(1065353216)) Constant(ConstDouble(4611686018427387904)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_single_add (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstFloat(1065353216)) Constant(ConstFloat(1073741824)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1077936128)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_single_add_rounds_at_single_precision (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstFloat(1036831949)) Constant(ConstFloat(1045220557)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1050253722)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_single_div_by_zero_yields_inf (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Divide Constant(ConstFloat(1065353216)) Constant(ConstFloat(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(2139095040)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_single_inf_minus_inf_is_nan (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Subtract Constant(ConstFloat(2139095040)) Constant(ConstFloat(2139095040)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(4290772992)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_single_mul (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Multiply Constant(ConstFloat(1069547520)) Constant(ConstFloat(1073741824)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1077936128)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_single_overflow_to_inf (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Multiply Constant(ConstFloat(1621981420)) Constant(ConstFloat(1621981420)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(2139095040)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPArithmetic.test_single_zero_div_zero_yields_nan (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Divide Constant(ConstFloat(0)) Constant(ConstFloat(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(4290772992)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_equal_nan_yields_zero (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Equal Constant(ConstFloat(2143289344)) Constant(ConstFloat(2143289344)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_equal_signed_zero (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Equal Constant(ConstFloat(0)) Constant(ConstFloat(2147483648)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_equal_true (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Equal Constant(ConstFloat(1069547520)) Constant(ConstFloat(1069547520)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_less_or_equal_boundary (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessOrEqual Constant(ConstFloat(1077936128)) Constant(ConstFloat(1077936128)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_less_than_double (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessThan Constant(ConstDouble(4607182418800017408)) Constant(ConstDouble(4611686018427387904)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_mismatched_precision_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Equal Constant(ConstFloat(1065353216)) Constant(ConstDouble(4607182418800017408)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Equal Constant(ConstFloat(1065353216)) Constant(ConstDouble(4607182418800017408)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_not_equal_nan_yields_one (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(NotEqual Constant(ConstFloat(2143289344)) Constant(ConstFloat(2143289344)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_relational_against_nan_returns_zero (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(GreaterOrEqual Constant(ConstFloat(2143289344)) Constant(ConstFloat(1065353216)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_relational_against_nan_returns_zero (2)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(GreaterThan Constant(ConstFloat(2143289344)) Constant(ConstFloat(1065353216)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_relational_against_nan_returns_zero (3)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessOrEqual Constant(ConstFloat(2143289344)) Constant(ConstFloat(1065353216)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPComparison.test_relational_against_nan_returns_zero (4)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessThan Constant(ConstFloat(2143289344)) Constant(ConstFloat(1065353216)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPCrossPrecision.test_double_to_float_overflow_to_inf (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  DoubleToFloat(Constant(ConstDouble(9094988921128908188)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(2139095040)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPCrossPrecision.test_double_to_float_round_trip (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  DoubleToFloat(Constant(ConstDouble(4609434218613702656)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1069547520)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPCrossPrecision.test_float_to_double (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToDouble(Constant(ConstFloat(1069547520)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstDouble(4609434218613702656)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPCrossPrecision.test_skip_with_var_src (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToDouble(Var(\"a\") Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FloatToDouble(Var(\"a\") Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPCrossPrecision.test_skip_with_wrong_const_variant (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToDouble(Constant(ConstDouble(4609434218613702656)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FloatToDouble(Constant(ConstDouble(4609434218613702656)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_double_inf_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  DoubleToInt(Constant(ConstDouble(18442240474082181120)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  DoubleToInt(Constant(ConstDouble(18442240474082181120)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_double_to_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  DoubleToInt(Constant(ConstDouble(4653144203864309760)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLong(1234)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_double_to_long_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  DoubleToInt(Constant(ConstDouble(4683220299150161609)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLongLong(123456)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_float_to_int (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(1080872141)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_float_to_int_negative (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(3228355789)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_float_to_uint (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(1080872141)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_inf_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(2139095040)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(2139095040)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_nan_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(2143289344)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(2143289344)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPToInt.test_skip_without_symbols (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(1080872141)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FloatToInt(Constant(ConstFloat(1080872141)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_logical_not_double_returns_int (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstDouble(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_logical_not_nan_is_truthy (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstFloat(2143289344)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_logical_not_negative_zero_single (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstFloat(2147483648)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_logical_not_nonzero_single (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstFloat(1069547520)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_logical_not_zero_single (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstFloat(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_negate_double (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Constant(ConstDouble(4614253070214989087)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstDouble(13837625107069764895)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_negate_positive_zero_yields_negative_zero (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Constant(ConstFloat(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(2147483648)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldFPUnary.test_negate_single (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Constant(ConstFloat(1065353216)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(3212836864)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_int_to_double_positive (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToDouble(Constant(ConstLong(1234)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstDouble(4653142004841054208)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_int_to_float_positive (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToFloat(Constant(ConstInt(5)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1084227584)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_int_to_float_zero (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToFloat(Constant(ConstInt(0)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(0)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_long_long_to_double_positive (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToDouble(Constant(ConstLongLong(2000000)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstDouble(4701340746312056832)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_max_unsigned_int_folds_to_positive_fp (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToFloat(Constant(ConstUInt(255)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(1132396544)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_negative_signed_int_folds_to_negative_fp (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToFloat(Constant(ConstInt(-1)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstFloat(3212836864)) Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_negative_signed_long_folds_to_negative_double (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToDouble(Constant(ConstLong(-1234)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstDouble(13876514041695830016)) Var(\"d\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldIntToFP.test_skip_with_var_src (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  IntToFloat(Var(\"a\") Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  IntToFloat(Var(\"a\") Var(\"f\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIf.test_jump_if_false_nonzero_dropped (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Constant(ConstInt(42)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIf.test_jump_if_false_zero_replaced (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Constant(ConstInt(0)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIf.test_jump_if_true_nonzero_replaced (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Constant(ConstInt(1)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIf.test_jump_if_true_zero_dropped (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Constant(ConstInt(0)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIf.test_jump_if_with_long_constant (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Constant(ConstLong(256)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIf.test_jump_if_with_var_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Var(\"c\") \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Var(\"c\") \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfCmp.test_false_compare_dropped (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfCmp(GreaterThan Constant(ConstInt(0)) Constant(ConstInt(10)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfCmp.test_true_compare_becomes_jump (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfCmp(LessThan Constant(ConstInt(0)) Constant(ConstInt(105)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfCmp.test_variable_operand_left_alone (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstInt(0)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstInt(0)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfFP.test_jump_if_false_nan_dropped (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Constant(ConstDouble(9221120237041090560)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfFP.test_jump_if_false_zero_replaced (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Constant(ConstFloat(0)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfFP.test_jump_if_true_nan_replaced (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Constant(ConstFloat(2143289344)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfFP.test_jump_if_true_negative_zero_dropped (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Constant(ConstFloat(2147483648)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfFP.test_jump_if_true_nonzero_replaced (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Constant(ConstFloat(1069547520)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldJumpIfFP.test_jump_if_true_zero_dropped (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Constant(ConstFloat(0)) \"L\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldShift.test_arithmetic_right_shift_negative_value (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(RightShift Constant(ConstInt(-8)) Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-4)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldShift.test_left_shift_basic (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LeftShift Constant(ConstInt(1)) Constant(ConstInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(8)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldShift.test_left_shift_overflows_into_sign (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LeftShift Constant(ConstInt(1)) Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-32768)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldShift.test_shift_count_at_width_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LeftShift Constant(ConstInt(1)) Constant(ConstInt(16)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(LeftShift Constant(ConstInt(1)) Constant(ConstInt(16)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldShift.test_shift_count_negative_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LeftShift Constant(ConstInt(1)) Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(LeftShift Constant(ConstInt(1)) Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldShift.test_shift_long_with_int_count (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LeftShift Constant(ConstLong(1)) Constant(ConstInt(8)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLong(256)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignExtend.test_int_to_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLong(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignExtend.test_int_to_long_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLongLong(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignExtend.test_int_to_unsigned_long_widens_via_signed (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstULong(4294967295)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignExtend.test_long_to_long_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"LongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstLong(-1234)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLongLong(-1234)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignExtend.test_skip_unknown_var (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignExtend.test_skip_with_var_src (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignExtend.test_skip_without_symbols (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  SignExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_mismatched_signedness_folds_to_unsigned (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstInt(1)) Constant(ConstUInt(2)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_signed_less_than_above_signed_threshold (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessThan Constant(ConstInt(-56)) Constant(ConstInt(100)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_unsigned_arithmetic_canonicalizes_to_non_negative (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(50000)) Constant(ConstUInt(20000)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(4464)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_unsigned_complement (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Complement Constant(ConstUInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(65535)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_unsigned_division_uses_unsigned_arithmetic (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Divide Constant(ConstUInt(200)) Constant(ConstUInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(66)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_unsigned_less_than_above_signed_threshold (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(LessThan Constant(ConstUInt(200)) Constant(ConstUInt(100)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_unsigned_negate_wraps (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Constant(ConstUInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(65535)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldSignednessAware.test_unsigned_right_shift_zero_fills (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Binary(RightShift Constant(ConstUInt(200)) Constant(ConstUInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(100)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldTruncate.test_long_long_to_int (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstLongLong(12302652056957900408)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(22136)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldTruncate.test_long_long_to_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstLongLong(12302652056957900408)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLong(305419896)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldTruncate.test_long_to_int_canonicalizes_sign (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstLong(2882404351)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldTruncate.test_long_to_int_keeps_low_bytes (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstLong(2882343476)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(4660)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldTruncate.test_skip_without_symbols (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstLong(4660)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstLong(4660)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_complement_int (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Complement Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_complement_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Complement Constant(ConstLong(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstLong(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_logical_not_long_returns_int (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstLong(1234)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_logical_not_nonzero_yields_zero (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstInt(42)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_logical_not_zero_yields_one (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(LogicalNot Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_negate_basic (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-5)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_negate_int_min_wraps (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Constant(ConstInt(-32768)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-32768)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_unary_complement_on_float_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Complement Constant(ConstFloat(1069547520)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Unary(Complement Constant(ConstFloat(1069547520)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldUnary.test_unary_with_var_operand_unchanged (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Unary(Negate Var(\"y\") Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldZeroExtend.test_skip_without_symbols (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  ZeroExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  ZeroExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldZeroExtend.test_uint_to_long_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  ZeroExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstULongLong(65535)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldZeroExtend.test_uint_to_ulong_negative_value_masks (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  ZeroExtend(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstULong(65535)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestFoldZeroExtend.test_ulong_to_ulong_long (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULongLong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  ZeroExtend(Constant(ConstULong(65535)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstULongLong(65535)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestProgramShape.test_function_metadata_preserved (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"foo\" false [\"a\" \"b\"] [\n"
      "  Binary(Add Constant(ConstInt(1)) Constant(ConstInt(2)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"foo\" false [\"a\" \"b\"] [\n"
      "  Copy(Constant(ConstInt(3)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_constant_folding.TestProgramShape.test_non_foldable_instructions_pass_through (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"a\"))\n"
      "  Binary(Add Var(\"a\") Constant(ConstInt(1)) Var(\"b\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"a\"))\n"
      "  Binary(Add Var(\"a\") Constant(ConstInt(1)) Var(\"b\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_choice_of_member (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%40.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"@16.n\"]}}}",
      "Function(\"scaled\" true [\"@16.n\"] [\n"
      "  Label(\".scaled@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@16.n\") Var(\"%39.1\"))\n"
      "  SignExtend(Constant(ConstInt(5)) Var(\"%40.1\"))\n"
      "  Binary(Subtract Var(\"%39.1\") Var(\"%40.1\") Var(\"%41.1\"))\n"
      "  Binary(Multiply Var(\"%41.1\") Constant(ConstLong(100000)) Var(\"%42.1\"))\n"
      "  Ret(Var(\"%42.1\"))\n"
      "])\n",
      "Function(\"scaled\" true [\"@16.n\"] [\n"
      "  Label(\".scaled@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@16.n\") Var(\"%39.1\"))\n"
      "  Copy(Constant(ConstLong(5)) Var(\"%40.1\"))\n"
      "  Binary(Subtract Var(\"%39.1\") Var(\"%40.1\") Var(\"%41.1\"))\n"
      "  Binary(Multiply Var(\"%41.1\") Constant(ConstLong(100000)) Var(\"%42.1\"))\n"
      "  Ret(Var(\"%42.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_choice_of_member (2)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%50.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%53.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Truncate(Constant(ConstInt(2)) Var(\"%48.1\"))\n"
      "  Truncate(Constant(ConstInt(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Truncate(Constant(ConstInt(3)) Var(\"%50.1\"))\n"
      "  Truncate(Constant(ConstInt(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  Truncate(Constant(ConstInt(255)) Var(\"%52.1\"))\n"
      "  Truncate(Constant(ConstInt(33)) Var(\"%53.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%52.1\") Var(\"%53.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%54.1\"))\n"
      "  Binary(Add Var(\"%54.1\") Constant(ConstInt(7)) Var(\"%55.1\"))\n"
      "  Load(Var(\"%55.1\") Var(\"%56.1\") false)\n"
      "  ZeroExtend(Var(\"%56.1\") Var(\"%57.1\"))\n"
      "  Binary(Multiply Var(\"%57.1\") Constant(ConstInt(256)) Var(\"%58.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%59.1\"))\n"
      "  Binary(Add Var(\"%59.1\") Constant(ConstInt(9)) Var(\"%60.1\"))\n"
      "  Load(Var(\"%60.1\") Var(\"%61.1\") false)\n"
      "  ZeroExtend(Var(\"%61.1\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%58.1\") Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%64.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Constant(ConstInt(8)) Var(\"%65.1\"))\n"
      "  Load(Var(\"%65.1\") Var(\"%66.1\") false)\n"
      "  ZeroExtend(Var(\"%66.1\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%63.1\") Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Ret(Var(\"%68.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%48.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%50.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(255)) Var(\"%52.1\"))\n"
      "  Copy(Constant(ConstUChar(33)) Var(\"%53.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%52.1\") Var(\"%53.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%54.1\"))\n"
      "  Binary(Add Var(\"%54.1\") Constant(ConstInt(7)) Var(\"%55.1\"))\n"
      "  Load(Var(\"%55.1\") Var(\"%56.1\") false)\n"
      "  ZeroExtend(Var(\"%56.1\") Var(\"%57.1\"))\n"
      "  Binary(Multiply Var(\"%57.1\") Constant(ConstInt(256)) Var(\"%58.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%59.1\"))\n"
      "  Binary(Add Var(\"%59.1\") Constant(ConstInt(9)) Var(\"%60.1\"))\n"
      "  Load(Var(\"%60.1\") Var(\"%61.1\") false)\n"
      "  ZeroExtend(Var(\"%61.1\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%58.1\") Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%64.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Constant(ConstInt(8)) Var(\"%65.1\"))\n"
      "  Load(Var(\"%65.1\") Var(\"%66.1\") false)\n"
      "  ZeroExtend(Var(\"%66.1\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%63.1\") Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Ret(Var(\"%68.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_choice_of_member_after_a_test (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%53.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%54.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%55.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%58.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%59.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\", \"%69.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Truncate(Constant(ConstInt(3)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Truncate(Constant(ConstInt(5)) Var(\"%51.1\"))\n"
      "  Truncate(Constant(ConstInt(11)) Var(\"%52.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%51.1\") Var(\"%52.1\")] None false false)\n"
      "  Truncate(Constant(ConstInt(6)) Var(\"%53.1\"))\n"
      "  Truncate(Constant(ConstInt(22)) Var(\"%54.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%53.1\") Var(\"%54.1\")] None false false)\n"
      "  Truncate(Constant(ConstInt(4)) Var(\"%55.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%56.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Constant(ConstInt(10)) Var(\"%57.1\"))\n"
      "  Store(Var(\"%55.1\") Var(\"%57.1\") false)\n"
      "  Truncate(Constant(ConstInt(7)) Var(\"%58.1\"))\n"
      "  Truncate(Constant(ConstInt(33)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%58.1\") Var(\"%59.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%60.1\") Constant(ConstInt(7)) Var(\"%61.1\"))\n"
      "  Load(Var(\"%61.1\") Var(\"%62.1\") false)\n"
      "  ZeroExtend(Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  Binary(Multiply Var(\"%63.1\") Constant(ConstInt(256)) Var(\"%64.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Constant(ConstInt(9)) Var(\"%66.1\"))\n"
      "  Load(Var(\"%66.1\") Var(\"%67.1\") false)\n"
      "  ZeroExtend(Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Var(\"%68.1\") Var(\"%69.1\"))\n"
      "  Ret(Var(\"%69.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%51.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%52.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%51.1\") Var(\"%52.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%53.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%54.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%53.1\") Var(\"%54.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%55.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%56.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Constant(ConstInt(10)) Var(\"%57.1\"))\n"
      "  Store(Var(\"%55.1\") Var(\"%57.1\") false)\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%58.1\"))\n"
      "  Copy(Constant(ConstUChar(33)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%58.1\") Var(\"%59.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%60.1\") Constant(ConstInt(7)) Var(\"%61.1\"))\n"
      "  Load(Var(\"%61.1\") Var(\"%62.1\") false)\n"
      "  ZeroExtend(Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  Binary(Multiply Var(\"%63.1\") Constant(ConstInt(256)) Var(\"%64.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Constant(ConstInt(9)) Var(\"%66.1\"))\n"
      "  Load(Var(\"%66.1\") Var(\"%67.1\") false)\n"
      "  ZeroExtend(Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Var(\"%68.1\") Var(\"%69.1\"))\n"
      "  Ret(Var(\"%69.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_choice_of_member_after_the_pointer_moves (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%50.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Truncate(Constant(ConstInt(5)) Var(\"%48.1\"))\n"
      "  Truncate(Constant(ConstInt(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Truncate(Constant(ConstInt(6)) Var(\"%50.1\"))\n"
      "  Truncate(Constant(ConstInt(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%52.1\"))\n"
      "  Binary(Add Var(\"%52.1\") Constant(ConstInt(7)) Var(\"%53.1\"))\n"
      "  Load(Var(\"%53.1\") Var(\"%54.1\") false)\n"
      "  ZeroExtend(Var(\"%54.1\") Var(\"%55.1\"))\n"
      "  Binary(Multiply Var(\"%55.1\") Constant(ConstInt(256)) Var(\"%56.1\"))\n"
      "  GetAddress(Var(\"m2\") Var(\"%57.1\"))\n"
      "  Binary(Add Var(\"%57.1\") Constant(ConstInt(9)) Var(\"%58.1\"))\n"
      "  Load(Var(\"%58.1\") Var(\"%59.1\") false)\n"
      "  ZeroExtend(Var(\"%59.1\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%62.1\") Constant(ConstInt(9)) Var(\"%63.1\"))\n"
      "  Load(Var(\"%63.1\") Var(\"%64.1\") false)\n"
      "  ZeroExtend(Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%61.1\") Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Ret(Var(\"%66.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%48.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%50.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%52.1\"))\n"
      "  Binary(Add Var(\"%52.1\") Constant(ConstInt(7)) Var(\"%53.1\"))\n"
      "  Load(Var(\"%53.1\") Var(\"%54.1\") false)\n"
      "  ZeroExtend(Var(\"%54.1\") Var(\"%55.1\"))\n"
      "  Binary(Multiply Var(\"%55.1\") Constant(ConstInt(256)) Var(\"%56.1\"))\n"
      "  GetAddress(Var(\"m2\") Var(\"%57.1\"))\n"
      "  Binary(Add Var(\"%57.1\") Constant(ConstInt(9)) Var(\"%58.1\"))\n"
      "  Load(Var(\"%58.1\") Var(\"%59.1\") false)\n"
      "  ZeroExtend(Var(\"%59.1\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%62.1\") Constant(ConstInt(9)) Var(\"%63.1\"))\n"
      "  Load(Var(\"%63.1\") Var(\"%64.1\") false)\n"
      "  ZeroExtend(Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%61.1\") Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Ret(Var(\"%66.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_compound_assignment_with_a_call (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%59.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\", \"%69.1\", \"%70.1\", \"%71.1\", \"%72.1\", \"%73.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Truncate(Constant(ConstInt(64)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Truncate(Constant(ConstInt(2)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%51.1\")] Var(\"%52.1\") false false)\n"
      "  ZeroExtend(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%54.1\"))\n"
      "  Load(Var(\"%54.1\") Var(\"%55.1\") false)\n"
      "  ZeroExtend(Var(\"%55.1\") Var(\"%56.1\"))\n"
      "  Binary(BitwiseOr Var(\"%56.1\") Var(\"%53.1\") Var(\"%57.1\"))\n"
      "  Truncate(Var(\"%57.1\") Var(\"%58.1\"))\n"
      "  Store(Var(\"%58.1\") Var(\"%54.1\") false)\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%59.1\")] Var(\"%60.1\") false false)\n"
      "  ZeroExtend(Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%62.1\"))\n"
      "  Load(Var(\"%62.1\") Var(\"%63.1\") false)\n"
      "  ZeroExtend(Var(\"%63.1\") Var(\"%64.1\"))\n"
      "  Binary(Subtract Var(\"%64.1\") Var(\"%61.1\") Var(\"%65.1\"))\n"
      "  Truncate(Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Store(Var(\"%66.1\") Var(\"%62.1\") false)\n"
      "  GetAddress(Var(\"m\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%67.1\") Constant(ConstInt(10)) Var(\"%68.1\"))\n"
      "  Load(Var(\"%68.1\") Var(\"%69.1\") false)\n"
      "  ZeroExtend(Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Binary(Multiply Var(\"%70.1\") Constant(ConstInt(16)) Var(\"%71.1\"))\n"
      "  ZeroExtend(Var(\"calls\") Var(\"%72.1\"))\n"
      "  Binary(Add Var(\"%71.1\") Var(\"%72.1\") Var(\"%73.1\"))\n"
      "  Ret(Var(\"%73.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(64)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%51.1\")] Var(\"%52.1\") false false)\n"
      "  ZeroExtend(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%54.1\"))\n"
      "  Load(Var(\"%54.1\") Var(\"%55.1\") false)\n"
      "  ZeroExtend(Var(\"%55.1\") Var(\"%56.1\"))\n"
      "  Binary(BitwiseOr Var(\"%56.1\") Var(\"%53.1\") Var(\"%57.1\"))\n"
      "  Truncate(Var(\"%57.1\") Var(\"%58.1\"))\n"
      "  Store(Var(\"%58.1\") Var(\"%54.1\") false)\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%59.1\")] Var(\"%60.1\") false false)\n"
      "  ZeroExtend(Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%62.1\"))\n"
      "  Load(Var(\"%62.1\") Var(\"%63.1\") false)\n"
      "  ZeroExtend(Var(\"%63.1\") Var(\"%64.1\"))\n"
      "  Binary(Subtract Var(\"%64.1\") Var(\"%61.1\") Var(\"%65.1\"))\n"
      "  Truncate(Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Store(Var(\"%66.1\") Var(\"%62.1\") false)\n"
      "  GetAddress(Var(\"m\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%67.1\") Constant(ConstInt(10)) Var(\"%68.1\"))\n"
      "  Load(Var(\"%68.1\") Var(\"%69.1\") false)\n"
      "  ZeroExtend(Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Binary(Multiply Var(\"%70.1\") Constant(ConstInt(16)) Var(\"%71.1\"))\n"
      "  ZeroExtend(Var(\"calls\") Var(\"%72.1\"))\n"
      "  Binary(Add Var(\"%71.1\") Var(\"%72.1\") Var(\"%73.1\"))\n"
      "  Ret(Var(\"%73.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_index_minus_one (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@18.base.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"o\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%48.1\", \"%49.1\", \"%52.1\", \"%53.1\", \"@18.base.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_index_minus_one (2)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@18.base.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"o\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%48.1\", \"%49.1\", \"%52.1\", \"%53.1\", \"@18.base.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Truncate(Constant(ConstInt(1)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_index_minus_one (3)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@18.base.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"o\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%48.1\", \"%49.1\", \"%52.1\", \"%53.1\", \"@18.base.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Truncate(Constant(ConstInt(255)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Copy(Constant(ConstUChar(255)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_index_minus_one (4)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@18.base.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"o\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%48.1\", \"%49.1\", \"%52.1\", \"%53.1\", \"@18.base.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Truncate(Constant(ConstInt(30)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"@18.base.1\"))\n"
      "  Copy(Constant(ConstUChar(30)) Var(\"%48.1\"))\n"
      "  FunctionCall(\"option\" [Var(\"@18.base.1\") Var(\"%48.1\")] Var(\"%49.1\") false false)\n"
      "  Binary(Subtract Var(\"%49.1\") Constant(ConstUInt(16384)) Var(\"%52.1\"))\n"
      "  Copy(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Ret(Var(\"%53.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_other_products (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%50.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%54.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%57.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%61.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%47.2\", \"%47.3\", \"%48.1\", \"%48.2\", \"%48.3\", \"%49.1\", \"%49.2\", \"%49.3\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%50.1\"))\n"
      "  FunctionCall(\"scaled\" [Var(\"%50.1\")] Var(\"%51.1\") false false)\n"
      "  Unary(Negate Constant(ConstLong(500000)) Var(\"%52.1\"))\n"
      "  Binary(Equal Var(\"%51.1\") Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  JumpIfFalse(Var(\"%53.1\") \".and_false@10\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Truncate(Constant(ConstInt(255)) Var(\"%54.1\"))\n"
      "  FunctionCall(\"scaled\" [Var(\"%54.1\")] Var(\"%55.1\") false false)\n"
      "  Binary(Equal Var(\"%55.1\") Constant(ConstLong(25000000)) Var(\"%56.1\"))\n"
      "  JumpIfFalse(Var(\"%56.1\") \".and_false@10\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%49.1\"))\n"
      "  Jump(\".and_end@11\")\n"
      "  Label(\".and_false@10\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%49.2\"))\n"
      "  Label(\".and_end@11\")\n"
      "  Phi(Var(\"%49.3\") [PhiArg(\".main@ssa_block@2\" Var(\"%49.1\")) PhiArg(\".and_false@10\" Var(\"%49.2\"))])\n"
      "  JumpIfFalse(Var(\"%49.3\") \".and_false@8\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%57.1\"))\n"
      "  FunctionCall(\"times37\" [Var(\"%57.1\")] Var(\"%58.1\") false false)\n"
      "  Binary(Equal Var(\"%58.1\") Constant(ConstUInt(111)) Var(\"%60.1\"))\n"
      "  JumpIfFalse(Var(\"%60.1\") \".and_false@8\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%48.1\"))\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%48.2\"))\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%48.3\") [PhiArg(\".main@ssa_block@4\" Var(\"%48.1\")) PhiArg(\".and_false@8\" Var(\"%48.2\"))])\n"
      "  JumpIfFalse(Var(\"%48.3\") \".and_false@6\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Truncate(Constant(ConstInt(255)) Var(\"%61.1\"))\n"
      "  FunctionCall(\"times37\" [Var(\"%61.1\")] Var(\"%62.1\") false false)\n"
      "  Binary(Multiply Constant(ConstInt(258)) Constant(ConstInt(37)) Var(\"%63.1\"))\n"
      "  Copy(Var(\"%63.1\") Var(\"%64.1\"))\n"
      "  Binary(Equal Var(\"%62.1\") Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  JumpIfFalse(Var(\"%65.1\") \".and_false@6\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%47.1\"))\n"
      "  Jump(\".and_end@7\")\n"
      "  Label(\".and_false@6\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%47.2\"))\n"
      "  Label(\".and_end@7\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@6\" Var(\"%47.1\")) PhiArg(\".and_false@6\" Var(\"%47.2\"))])\n"
      "  Ret(Var(\"%47.3\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%50.1\"))\n"
      "  FunctionCall(\"scaled\" [Var(\"%50.1\")] Var(\"%51.1\") false false)\n"
      "  Copy(Constant(ConstLong(-500000)) Var(\"%52.1\"))\n"
      "  Binary(Equal Var(\"%51.1\") Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  JumpIfFalse(Var(\"%53.1\") \".and_false@10\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(255)) Var(\"%54.1\"))\n"
      "  FunctionCall(\"scaled\" [Var(\"%54.1\")] Var(\"%55.1\") false false)\n"
      "  Binary(Equal Var(\"%55.1\") Constant(ConstLong(25000000)) Var(\"%56.1\"))\n"
      "  JumpIfFalse(Var(\"%56.1\") \".and_false@10\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%49.1\"))\n"
      "  Jump(\".and_end@11\")\n"
      "  Label(\".and_false@10\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%49.2\"))\n"
      "  Label(\".and_end@11\")\n"
      "  Phi(Var(\"%49.3\") [PhiArg(\".main@ssa_block@2\" Var(\"%49.1\")) PhiArg(\".and_false@10\" Var(\"%49.2\"))])\n"
      "  JumpIfFalse(Var(\"%49.3\") \".and_false@8\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%57.1\"))\n"
      "  FunctionCall(\"times37\" [Var(\"%57.1\")] Var(\"%58.1\") false false)\n"
      "  Binary(Equal Var(\"%58.1\") Constant(ConstUInt(111)) Var(\"%60.1\"))\n"
      "  JumpIfFalse(Var(\"%60.1\") \".and_false@8\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%48.1\"))\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%48.2\"))\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%48.3\") [PhiArg(\".main@ssa_block@4\" Var(\"%48.1\")) PhiArg(\".and_false@8\" Var(\"%48.2\"))])\n"
      "  JumpIfFalse(Var(\"%48.3\") \".and_false@6\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Copy(Constant(ConstUChar(255)) Var(\"%61.1\"))\n"
      "  FunctionCall(\"times37\" [Var(\"%61.1\")] Var(\"%62.1\") false false)\n"
      "  Copy(Constant(ConstInt(9546)) Var(\"%63.1\"))\n"
      "  Copy(Var(\"%63.1\") Var(\"%64.1\"))\n"
      "  Binary(Equal Var(\"%62.1\") Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  JumpIfFalse(Var(\"%65.1\") \".and_false@6\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%47.1\"))\n"
      "  Jump(\".and_end@7\")\n"
      "  Label(\".and_false@6\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%47.2\"))\n"
      "  Label(\".and_end@7\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@6\" Var(\"%47.1\")) PhiArg(\".and_false@6\" Var(\"%47.2\"))])\n"
      "  Ret(Var(\"%47.3\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_pointer_member_access.TestRuns.test_other_products (2)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%64.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%47.2\", \"%47.3\", \"%48.1\", \"%48.2\", \"%48.3\", \"%49.1\", \"%49.2\", \"%49.3\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"scaled\" [Constant(ConstUChar(0))] Var(\"%51.1\") false false)\n"
      "  JumpIfCmp(NotEqual Var(\"%51.1\") Constant(ConstLong(-500000)) \".and_false@10\")\n"
      "  FunctionCall(\"scaled\" [Constant(ConstUChar(255))] Var(\"%55.1\") false false)\n"
      "  JumpIfCmp(NotEqual Var(\"%55.1\") Constant(ConstLong(25000000)) \".and_false@10\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Jump(\".and_end@11\")\n"
      "  Label(\".and_false@10\")\n"
      "  Label(\".and_end@11\")\n"
      "  Phi(Var(\"%49.3\") [PhiArg(\".main@ssa_block@2\" Constant(ConstInt(1))) PhiArg(\".and_false@10\" Constant(ConstInt(0)))])\n"
      "  JumpIfFalse(Var(\"%49.3\") \".and_false@8\")\n"
      "  FunctionCall(\"times37\" [Constant(ConstUChar(0))] Var(\"%58.1\") false false)\n"
      "  JumpIfCmp(NotEqual Var(\"%58.1\") Constant(ConstUInt(111)) \".and_false@8\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%48.3\") [PhiArg(\".main@ssa_block@4\" Constant(ConstInt(1))) PhiArg(\".and_false@8\" Constant(ConstInt(0)))])\n"
      "  JumpIfFalse(Var(\"%48.3\") \".and_false@6\")\n"
      "  FunctionCall(\"times37\" [Constant(ConstUChar(255))] Var(\"%62.1\") false false)\n"
      "  Copy(Constant(ConstInt(9546)) Var(\"%64.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%62.1\") Var(\"%64.1\") \".and_false@6\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".and_end@7\")\n"
      "  Label(\".and_false@6\")\n"
      "  Label(\".and_end@7\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@6\" Constant(ConstInt(1))) PhiArg(\".and_false@6\" Constant(ConstInt(0)))])\n"
      "  Ret(Var(\"%47.3\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"scaled\" [Constant(ConstUChar(0))] Var(\"%51.1\") false false)\n"
      "  JumpIfCmp(NotEqual Var(\"%51.1\") Constant(ConstLong(-500000)) \".and_false@10\")\n"
      "  FunctionCall(\"scaled\" [Constant(ConstUChar(255))] Var(\"%55.1\") false false)\n"
      "  JumpIfCmp(NotEqual Var(\"%55.1\") Constant(ConstLong(25000000)) \".and_false@10\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Jump(\".and_end@11\")\n"
      "  Label(\".and_false@10\")\n"
      "  Label(\".and_end@11\")\n"
      "  Phi(Var(\"%49.3\") [PhiArg(\".main@ssa_block@2\" Constant(ConstInt(1))) PhiArg(\".and_false@10\" Constant(ConstInt(0)))])\n"
      "  JumpIfFalse(Var(\"%49.3\") \".and_false@8\")\n"
      "  FunctionCall(\"times37\" [Constant(ConstUChar(0))] Var(\"%58.1\") false false)\n"
      "  JumpIfCmp(NotEqual Var(\"%58.1\") Constant(ConstUInt(111)) \".and_false@8\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%48.3\") [PhiArg(\".main@ssa_block@4\" Constant(ConstInt(1))) PhiArg(\".and_false@8\" Constant(ConstInt(0)))])\n"
      "  JumpIfFalse(Var(\"%48.3\") \".and_false@6\")\n"
      "  FunctionCall(\"times37\" [Constant(ConstUChar(255))] Var(\"%62.1\") false false)\n"
      "  Copy(Constant(ConstUInt(9546)) Var(\"%64.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%62.1\") Var(\"%64.1\") \".and_false@6\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".and_end@7\")\n"
      "  Label(\".and_false@6\")\n"
      "  Label(\".and_end@7\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@6\" Constant(ConstInt(1))) PhiArg(\".and_false@6\" Constant(ConstInt(0)))])\n"
      "  Ret(Var(\"%47.3\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_reassoc_offset_licm.TestReassocLicmSim.test_pointer_advanced_by_byte_in_loop (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@4.i.1\", \"@4.i.2\", \"@4.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@4.i.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@4.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.i.1\")) PhiArg(\".loop@1_continue\" Var(\"@4.i.3\"))])\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%8.1\"))\n"
      "  Binary(LessThan Var(\"%8.1\") Constant(ConstInt(40)) Var(\"%9.1\"))\n"
      "  JumpIfFalse(Var(\"%9.1\") \".loop@1_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  IndexedSymbolStore(\"t\" Var(\"%11.1\") Var(\"@4.i.2\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@4.i.2\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@4.i.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"t\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstInt(1)) Var(\"%15.1\"))\n"
      "  Truncate(Constant(ConstInt(3)) Var(\"%16.1\"))\n"
      "  Truncate(Constant(ConstInt(12)) Var(\"%17.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%15.1\") Var(\"%16.1\") Var(\"%17.1\")] Var(\"%18.1\") false false)\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@4.i.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@4.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.i.1\")) PhiArg(\".loop@1_continue\" Var(\"@4.i.3\"))])\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%8.1\"))\n"
      "  Binary(LessThan Var(\"%8.1\") Constant(ConstInt(40)) Var(\"%9.1\"))\n"
      "  JumpIfFalse(Var(\"%9.1\") \".loop@1_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  IndexedSymbolStore(\"t\" Var(\"%11.1\") Var(\"@4.i.2\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@4.i.2\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@4.i.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"t\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstInt(1)) Var(\"%15.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%16.1\"))\n"
      "  Copy(Constant(ConstUChar(12)) Var(\"%17.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%15.1\") Var(\"%16.1\") Var(\"%17.1\")] Var(\"%18.1\") false false)\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_split_tables.TestSplitTablesSim.test_long_table (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%5.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%5.1\")] Var(\"%6.1\") false false)\n"
      "  Truncate(Constant(ConstInt(1)) Var(\"%7.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%7.1\")] Var(\"%8.1\") false false)\n"
      "  Binary(Add Var(\"%6.1\") Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Truncate(Constant(ConstInt(2)) Var(\"%10.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%10.1\")] Var(\"%11.1\") false false)\n"
      "  Binary(Add Var(\"%9.1\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Truncate(Constant(ConstInt(199)) Var(\"%13.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%13.1\")] Var(\"%14.1\") false false)\n"
      "  Binary(Add Var(\"%12.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Ret(Var(\"%16.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%5.1\")] Var(\"%6.1\") false false)\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%7.1\")] Var(\"%8.1\") false false)\n"
      "  Binary(Add Var(\"%6.1\") Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%10.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%10.1\")] Var(\"%11.1\") false false)\n"
      "  Binary(Add Var(\"%9.1\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Copy(Constant(ConstUChar(199)) Var(\"%13.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%13.1\")] Var(\"%14.1\") false false)\n"
      "  Binary(Add Var(\"%12.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Ret(Var(\"%16.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_split_tables.TestSplitTablesSim.test_pointer_table (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@2.s.1\", \"@2.s.2\", \"@2.s.3\", \"@3.i.1\", \"@3.i.2\", \"@3.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@2.s.1\"))\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%8.1\"))\n"
      "  Copy(Var(\"%8.1\") Var(\"@3.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.s.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.s.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.s.3\"))])\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@3.i.3\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%9.1\"))\n"
      "  Binary(LessThan Var(\"%9.1\") Constant(ConstInt(9)) Var(\"%10.1\"))\n"
      "  JumpIfFalse(Var(\"%10.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%11.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"@3.i.2\") Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstInt(1)) Var(\"%15.1\"))\n"
      "  Binary(Multiply Var(\"%13.1\") Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  ZeroExtend(Var(\"@2.s.2\") Var(\"%17.1\"))\n"
      "  Binary(Add Var(\"%17.1\") Var(\"%16.1\") Var(\"%18.1\"))\n"
      "  Truncate(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Copy(Var(\"%19.1\") Var(\"@2.s.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@3.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  ZeroExtend(Var(\"@2.s.2\") Var(\"%22.1\"))\n"
      "  Truncate(Constant(ConstInt(2)) Var(\"%23.1\"))\n"
      "  Truncate(Constant(ConstInt(1)) Var(\"%24.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%23.1\") Var(\"%24.1\")] Var(\"%25.1\") false false)\n"
      "  ZeroExtend(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"%22.1\") Var(\"%26.1\") Var(\"%27.1\"))\n"
      "  Ret(Var(\"%27.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@2.s.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%8.1\"))\n"
      "  Copy(Var(\"%8.1\") Var(\"@3.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.s.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.s.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.s.3\"))])\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@3.i.3\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%9.1\"))\n"
      "  Binary(LessThan Var(\"%9.1\") Constant(ConstInt(9)) Var(\"%10.1\"))\n"
      "  JumpIfFalse(Var(\"%10.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%11.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"@3.i.2\") Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstInt(1)) Var(\"%15.1\"))\n"
      "  Binary(Multiply Var(\"%13.1\") Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  ZeroExtend(Var(\"@2.s.2\") Var(\"%17.1\"))\n"
      "  Binary(Add Var(\"%17.1\") Var(\"%16.1\") Var(\"%18.1\"))\n"
      "  Truncate(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Copy(Var(\"%19.1\") Var(\"@2.s.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@3.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  ZeroExtend(Var(\"@2.s.2\") Var(\"%22.1\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%23.1\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%24.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%23.1\") Var(\"%24.1\")] Var(\"%25.1\") false false)\n"
      "  ZeroExtend(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"%22.1\") Var(\"%26.1\") Var(\"%27.1\"))\n"
      "  Ret(Var(\"%27.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_split_tables.TestSplitTablesSim.test_uint16_table (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"T\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [258, 772, 4096, 8192, 4660, 65280, 7, 32769]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%8.1\", \"%9.1\", \"@2.s.1\", \"@2.s.2\", \"@2.s.3\", \"@3.i.1\", \"@3.i.2\", \"@3.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  IndexedLoad(\"T\" Constant(ConstUChar(8)) Var(\"%8.1\") false 0 0)\n"
      "  Copy(Var(\"%8.1\") Var(\"@2.s.1\"))\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"@3.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.s.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.s.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.s.3\"))])\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@3.i.3\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%10.1\"))\n"
      "  Binary(LessThan Var(\"%10.1\") Constant(ConstInt(8)) Var(\"%11.1\"))\n"
      "  JumpIfFalse(Var(\"%11.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  FunctionCall(\"get\" [Var(\"@3.i.2\")] Var(\"%12.1\") false false)\n"
      "  Binary(Add Var(\"@2.s.2\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@2.s.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@3.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Truncate(Constant(ConstInt(6)) Var(\"%16.1\"))\n"
      "  FunctionCall(\"next\" [Var(\"%16.1\")] Var(\"%17.1\") false false)\n"
      "  Binary(Add Var(\"@2.s.2\") Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%19.1\"))\n"
      "  FunctionCall(\"next\" [Var(\"%19.1\")] Var(\"%20.1\") false false)\n"
      "  Binary(Add Var(\"%18.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Ret(Var(\"%22.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(4660)) Var(\"%8.1\"))\n"
      "  Copy(Var(\"%8.1\") Var(\"@2.s.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"@3.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.s.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.s.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.s.3\"))])\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@3.i.3\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%10.1\"))\n"
      "  Binary(LessThan Var(\"%10.1\") Constant(ConstInt(8)) Var(\"%11.1\"))\n"
      "  JumpIfFalse(Var(\"%11.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  FunctionCall(\"get\" [Var(\"@3.i.2\")] Var(\"%12.1\") false false)\n"
      "  Binary(Add Var(\"@2.s.2\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@2.s.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@3.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%16.1\"))\n"
      "  FunctionCall(\"next\" [Var(\"%16.1\")] Var(\"%17.1\") false false)\n"
      "  Binary(Add Var(\"@2.s.2\") Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%19.1\"))\n"
      "  FunctionCall(\"next\" [Var(\"%19.1\")] Var(\"%20.1\") false false)\n"
      "  Binary(Add Var(\"%18.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Ret(Var(\"%22.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_split_tables.TestSplitTablesSim.test_uint32_table (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Truncate(Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%4.1\")] Var(\"%5.1\") false false)\n"
      "  Binary(RightShift Var(\"%5.1\") Constant(ConstInt(16)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%8.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%8.1\")] Var(\"%9.1\") false false)\n"
      "  ZeroExtend(Constant(ConstUInt(65535)) Var(\"%10.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%9.1\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Truncate(Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%14.1\")] Var(\"%15.1\") false false)\n"
      "  Binary(RightShift Var(\"%15.1\") Constant(ConstInt(24)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%4.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%4.1\")] Var(\"%5.1\") false false)\n"
      "  Binary(RightShift Var(\"%5.1\") Constant(ConstInt(16)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%8.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%8.1\")] Var(\"%9.1\") false false)\n"
      "  Copy(Constant(ConstULong(65535)) Var(\"%10.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%9.1\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%14.1\"))\n"
      "  FunctionCall(\"get\" [Var(\"%14.1\")] Var(\"%15.1\") false false)\n"
      "  Binary(RightShift Var(\"%15.1\") Constant(ConstInt(24)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestConstArraySubscriptFoldUnit.test_indexed_load_aligned_constant_folds (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [256, 512, 768]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%t\") false 0 0)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(768)) Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestMultiDimConstArrayFoldUnit.test_2d_aligned_constant_folds (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Const(UChar) 3) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2, 3]}, {\"tuple\": [4, 5, 6]}, {\"tuple\": [7, 8, 9]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(5)) Var(\"%t\") false 0 0)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestMultiDimConstArrayFoldUnit.test_2d_padded_zero_folds (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(Const(UChar) 3) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [{\"tuple\": [1, 2, 3]}, {\"tuple\": [7, 0, 0]}, {\"tuple\": [0, 0, 0]}]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(5)) Var(\"%t\") false 0 0)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestMultiDimConstArrayFoldUnit.test_pointer_element_array_folds (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(Pointer(UChar)) 4)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [8192, 9216, 10240, 11264]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%t\") false 0 0)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(10240)) Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestStaticConstFoldAsmShape.test_add_reassoc_combines_constants (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.col\"]}}}",
      "Function(\"main\" true [\"@0.col\"] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Add Constant(ConstInt(100)) Constant(ConstInt(200)) Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"@0.col\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"main\" true [\"@0.col\"] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(300)) Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"@0.col\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestStaticConstFoldAsmShape.test_const_array_subscript_with_constant_index_folds (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(39612)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(-25924)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestStaticConstFoldAsmShape.test_const_array_subscript_with_constant_index_folds (2)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"arr\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [4660, 22136, 39612]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  IndexedLoad(\"arr\" Constant(ConstUChar(4)) Var(\"%0.1\") false 0 0)\n"
      "  Copy(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(39612)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestStaticConstFoldAsmShape.test_scalar_const_static_collapses_to_immediate (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Add Constant(ConstInt(4660)) Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(4661)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestStaticConstFoldCorrectness.test_pointer_const_static_indexed_write (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"offsets\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UInt) 3)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [256, 512, 768]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%3.1\", \"%4.1\", \"%5.1\"]}}}",
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
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(66)) Var(\"%0.1\"))\n"
      "  Copy(Constant(ConstUInt(512)) Var(\"%1.1\"))\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUInt(5)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Constant(ConstUInt(16384)) Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Store(Var(\"%0.1\") Var(\"%5.1\") false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_static_const_fold.TestStaticConstFoldCorrectness.test_pointer_const_static_indexed_write (2)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%3.1\", \"%4.1\", \"%5.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Constant(ConstUInt(16896)) Constant(ConstUInt(5)) Var(\"%5.1\"))\n"
      "  Store(Constant(ConstUChar(66)) Var(\"%5.1\") false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(16901)) Var(\"%5.1\"))\n"
      "  Store(Constant(ConstUChar(66)) Var(\"%5.1\") false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_strength_reduction.TestEndToEndShifts.test_signed_divide_by_4_keeps_truncation (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Unary(Negate Constant(ConstInt(7)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-7)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_strength_reduction.TestEndToEndShifts.test_two_multiplies_into_one_global (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.w.1\", \"@0.w.2\", \"@0.w.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  SignExtend(Constant(ConstInt(3)) Var(\"%0.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"z\"))\n"
      "  Truncate(Constant(ConstInt(0)) Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"@0.w.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.w.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.w.3\"))])\n"
      "  ZeroExtend(Var(\"@0.w.2\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  JumpIfFalse(Var(\"%4.1\") \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  SignExtend(Constant(ConstInt(129)) Var(\"%5.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@0.w.3\"))\n"
      "  ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Binary(LessThan Var(\"%8.1\") Constant(ConstInt(4)) Var(\"%9.1\"))\n"
      "  JumpIfTrue(Var(\"%9.1\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  SignExtend(Constant(ConstInt(32767)) Var(\"%10.1\"))\n"
      "  Binary(BitwiseAnd Var(\"z\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstULong(3)) Var(\"%0.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"z\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"@0.w.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.w.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.w.3\"))])\n"
      "  ZeroExtend(Var(\"@0.w.2\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  JumpIfFalse(Var(\"%4.1\") \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Copy(Constant(ConstULong(129)) Var(\"%5.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@0.w.3\"))\n"
      "  ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Binary(LessThan Var(\"%8.1\") Constant(ConstInt(4)) Var(\"%9.1\"))\n"
      "  JumpIfTrue(Var(\"%9.1\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Constant(ConstULong(32767)) Var(\"%10.1\"))\n"
      "  Binary(BitwiseAnd Var(\"z\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_by_1_signed (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Unary(Negate Constant(ConstInt(2)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-2)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_constant_fold: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_negative_odd (1)") {
  check_stage_case(
      "tac_constant_fold",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Unary(Negate Constant(ConstInt(3)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-3)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}
