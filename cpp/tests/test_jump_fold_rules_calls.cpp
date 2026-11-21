// The calls of rule sets of some of the jump-fold rules
// (fold_producer_jumps.h), named by their rules, that the Python unit
// tests make, each with the function the test gives the pass, the
// symbols the call read and the SSA names, and what the Python pass
// returned and changed, which the C++ pass must return and change too.
// They were recorded by wrapping the Python passes while
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
#include "tac_rules_case.h"

using c6502::passes::JumpFoldRule;
using c6502::testing::check_rules_case;

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_arbitrary_byte_mask_folds (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(1)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@u\") 1 false \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_arbitrary_byte_mask_folds (standalone) (2)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(15)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@u\") 15 false \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_arbitrary_byte_mask_folds (standalone) (3)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(254)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@u\") 254 false \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_arbitrary_byte_mask_folds (standalone) (4)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(64)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@u\") 64 false \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_constant_on_left_side_folds (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Constant(ConstInt(128)) Var(\"%ext\") Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@u\") 128 false \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_sign_bit_jumpiffalse_folds_to_masked (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(128)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@u\") 128 false \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_sign_bit_jumpiftrue_folds_with_nonzero_sense (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(128)) Var(\"%res\"))\n"
      "  JumpIfTrue(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@u\") 128 true \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestFold.test_uchar_source_via_char_type (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@c\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(128)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@c\") Var(\"%ext\"))\n"
      "  JumpIfMasked(Var(\"@c\") 128 false \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestGuards.test_mask_above_byte_range_blocks_fold (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@u\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(256)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(256)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestGuards.test_multi_use_res_blocks_fold (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(128)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "  Copy(Var(\"%res\") Var(\"%y\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@u\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(128)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "  Copy(Var(\"%res\") Var(\"%y\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_narrow_and_jump: test_and_zero_jump_fold.TestGuards.test_non_uchar_source_blocks_fold (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kAndZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@i\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(128)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@i\") Var(\"%ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%ext\") Constant(ConstInt(128)) Var(\"%res\"))\n"
      "  JumpIfFalse(Var(\"%res\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpNarrowing.test_narrowing_fails_for_out_of_range_const (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  Binary(LessThan Var(\"%wide\") Constant(ConstInt(300)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%wide\") Constant(ConstInt(300)) \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpNarrowing.test_narrows_const_lt_uchar (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  Binary(LessThan Constant(ConstInt(5)) Var(\"%wide\") Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  JumpIfCmp(LessThan Constant(ConstUChar(5)) Var(\"@a\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpNarrowing.test_narrows_uchar_lt_const (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  Binary(LessThan Var(\"%wide\") Constant(ConstInt(105)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@a\") Constant(ConstUChar(105)) \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpNarrowing.test_narrows_uchar_lt_uchar (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wa\"))\n"
      "  ZeroExtend(Var(\"@b\") Var(\"%wb\"))\n"
      "  Binary(LessThan Var(\"%wa\") Var(\"%wb\") Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wa\"))\n"
      "  ZeroExtend(Var(\"@b\") Var(\"%wb\"))\n"
      "  JumpIfCmp(LessThan Var(\"@a\") Var(\"@b\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpRewrite.test_eq_against_nonzero_const_inverts_to_ne (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstInt(5)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(NotEqual Var(\"%x\") Constant(ConstInt(5)) \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpRewrite.test_gt_jumpiffalse_inverts_to_le (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(GreaterThan Var(\"%x\") Var(\"%y\") Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(LessOrEqual Var(\"%x\") Var(\"%y\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpRewrite.test_lt_jumpiffalse_inverts_to_ge (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LessThan Var(\"%x\") Var(\"%y\") Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%x\") Var(\"%y\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpRewrite.test_lt_jumpiftrue_keeps_op (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LessThan Var(\"%x\") Var(\"%y\") Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(LessThan Var(\"%x\") Var(\"%y\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestJumpIfCmpRewrite.test_multi_use_blocks_jumpifcmp_too (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LessThan Var(\"%x\") Var(\"%y\") Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "  Copy(Var(\"%c\") Var(\"%retain\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LessThan Var(\"%x\") Var(\"%y\") Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "  Copy(Var(\"%c\") Var(\"%retain\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestMultiUseSsaExtend.test_narrows_each_compare (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"n.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%c.3\", \"%c.4\", \"%keep.5\", \"%w.2\", \"n.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"n.1\") Var(\"%w.2\"))\n"
      "  Binary(Equal Var(\"%w.2\") Constant(ConstInt(3)) Var(\"%c.3\"))\n"
      "  JumpIfTrue(Var(\"%c.3\") \".a\")\n"
      "  Binary(Equal Var(\"%w.2\") Constant(ConstInt(7)) Var(\"%c.4\"))\n"
      "  JumpIfTrue(Var(\"%c.4\") \".b\")\n"
      "  Copy(Var(\"%w.2\") Var(\"%keep.5\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"n.1\") Var(\"%w.2\"))\n"
      "  JumpIfCmp(Equal Var(\"n.1\") Constant(ConstUChar(3)) \".a\")\n"
      "  JumpIfCmp(Equal Var(\"n.1\") Constant(ConstUChar(7)) \".b\")\n"
      "  Copy(Var(\"%w.2\") Var(\"%keep.5\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteGuards.test_address_taken_cond_not_folded (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  GetAddress(Var(\"%c\") Var(\"%p\"))\n"
      "  Binary(LessThan Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  GetAddress(Var(\"%c\") Var(\"%p\"))\n"
      "  Binary(LessThan Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteGuards.test_both_var_folds_to_jumpifcmp (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Var(\"%y\") Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(NotEqual Var(\"%x\") Var(\"%y\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteGuards.test_multi_use_cond_doesnt_fire (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "  Copy(Var(\"%c\") Var(\"%result\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "  Copy(Var(\"%c\") Var(\"%result\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteGuards.test_non_adjacent_doesnt_fire (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  Copy(Var(\"%y\") Var(\"%z\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  Copy(Var(\"%y\") Var(\"%z\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteGuards.test_non_eq_op_folds_to_jumpifcmp (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LessThan Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%x\") Constant(ConstInt(0)) \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteGuards.test_static_cond_not_folded (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LessThan Var(\"%x\") Constant(ConstInt(0)) Var(\"g\"))\n"
      "  JumpIfFalse(Var(\"g\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LessThan Var(\"%x\") Constant(ConstInt(0)) Var(\"g\"))\n"
      "  JumpIfFalse(Var(\"g\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteSenseTable.test_eq_then_jumpiffalse_becomes_jumpiftrue (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%x\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteSenseTable.test_eq_then_jumpiftrue_becomes_jumpiffalse (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%x\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteSenseTable.test_ne_then_jumpiffalse_becomes_jumpiffalse (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%x\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteSenseTable.test_ne_then_jumpiftrue_becomes_jumpiftrue (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"%x\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%x\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteSenseTable.test_unsigned_zero_constant (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Var(\"%x\") Constant(ConstUInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%x\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestRewriteSenseTable.test_zero_on_left_side (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Equal Constant(ConstInt(0)) Var(\"%x\") Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%x\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_does_not_narrow_against_a_wider_constant (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(LessThan Var(\"%wide\") Constant(ConstInt(-129)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".pos\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(LessThan Var(\"%wide\") Constant(ConstInt(-129)) \".pos\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_does_not_narrow_against_a_wider_constant (standalone) (2)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(LessThan Var(\"%wide\") Constant(ConstInt(128)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".pos\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(LessThan Var(\"%wide\") Constant(ConstInt(128)) \".pos\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_does_not_narrow_uchar_through_signextend (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(GreaterOrEqual Var(\"%wide\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".top\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%wide\") Constant(ConstInt(0)) \".top\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_narrows_gt_against_any_signed_byte (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(GreaterThan Var(\"%wide\") Constant(ConstInt(-12)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".pos\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterThan Var(\"@x\") Constant(ConstChar(-12)) \".pos\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_narrows_gt_against_any_signed_byte (standalone) (2)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(GreaterThan Var(\"%wide\") Constant(ConstInt(-128)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".pos\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterThan Var(\"@x\") Constant(ConstChar(-128)) \".pos\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_narrows_gt_against_any_signed_byte (standalone) (3)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(GreaterThan Var(\"%wide\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".pos\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterThan Var(\"@x\") Constant(ConstChar(0)) \".pos\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_narrows_gt_against_any_signed_byte (standalone) (4)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(GreaterThan Var(\"%wide\") Constant(ConstInt(127)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".pos\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterThan Var(\"@x\") Constant(ConstChar(127)) \".pos\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_narrows_schar_ge_zero (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(GreaterOrEqual Var(\"%wide\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".top\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@x\") Constant(ConstChar(0)) \".top\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_narrows_schar_jumpiffalse_inverts_to_lt (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(GreaterOrEqual Var(\"%wide\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(LessThan Var(\"@x\") Constant(ConstChar(0)) \".break\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestSignExtendNarrowingAgainstZero.test_narrows_schar_lt_zero (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  Binary(LessThan Var(\"%wide\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfTrue(Var(\"%c\") \".neg\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"@x\") Var(\"%wide\"))\n"
      "  JumpIfCmp(LessThan Var(\"@x\") Constant(ConstChar(0)) \".neg\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_narrow_value_compare: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_constant_out_of_byte_range_left_alone (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kNarrowValueCompare},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"r\", \"t\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"t\"))\n"
      "  Binary(LessThan Var(\"t\") Constant(ConstInt(-1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"t\"))\n"
      "  Binary(LessThan Var(\"t\") Constant(ConstInt(-1)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_narrow_value_compare: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_constant_out_of_byte_range_left_alone (standalone) (2)") {
  check_rules_case(
      {JumpFoldRule::kNarrowValueCompare},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"r\", \"t\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"t\"))\n"
      "  Binary(LessThan Var(\"t\") Constant(ConstInt(256)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"t\"))\n"
      "  Binary(LessThan Var(\"t\") Constant(ConstInt(256)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_narrow_value_compare: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_rule_keeps_the_dst (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kNarrowValueCompare},
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"r\", \"t\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"t\"))\n"
      "  Binary(LessThan Var(\"t\") Constant(ConstInt(10)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"t\"))\n"
      "  Binary(LessThan Var(\"x\") Constant(ConstUChar(10)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestZeroExtendNarrowing.test_doesnt_trace_through_multi_use_widen (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  Binary(Equal Var(\"%wide\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "  Copy(Var(\"%wide\") Var(\"%retain\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  JumpIfTrue(Var(\"%wide\") \".t\")\n"
      "  Copy(Var(\"%wide\") Var(\"%retain\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_cmp_zero_jump: test_cmp_zero_jump_fold.TestZeroExtendNarrowing.test_traces_through_zero_extend (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kCmpZero},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  Binary(Equal Var(\"%wide\") Constant(ConstInt(0)) Var(\"%c\"))\n"
      "  JumpIfFalse(Var(\"%c\") \".t\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"@a\") Var(\"%wide\"))\n"
      "  JumpIfTrue(Var(\"@a\") \".t\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteGuards.test_jumpif_on_different_var_doesnt_fire (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%other\") \".L\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%other\") \".L\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteGuards.test_multi_use_t_doesnt_fire (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%retain\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%retain\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteGuards.test_no_jumpif_follower_doesnt_fire (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Var(\"%out\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Var(\"%out\"))\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteGuards.test_non_adjacent_doesnt_fire (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  Copy(Var(\"%y\") Var(\"%z\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  Copy(Var(\"%y\") Var(\"%z\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteGuards.test_other_unary_ops_dont_fire (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(Complement Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Unary(Complement Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteGuards.test_other_unary_ops_dont_fire (standalone) (2)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(Negate Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Unary(Negate Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteSenseTable.test_lnot_then_jumpiffalse_becomes_jumpiftrue (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfFalse(Var(\"%t\") \".L\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%x\") \".L\")\n"
      "])\n",
      "");
}

TEST_CASE("jumprules_fold_lnot_jump: test_lnot_jump_fold.TestRewriteSenseTable.test_lnot_then_jumpiftrue_becomes_jumpiffalse (standalone) (1)") {
  check_rules_case(
      {JumpFoldRule::kLnot},
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(LogicalNot Var(\"%x\") Var(\"%t\"))\n"
      "  JumpIfTrue(Var(\"%t\") \".L\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%x\") \".L\")\n"
      "])\n",
      "");
}
