// The calls of `tac_fold_producer_jumps` that the Python unit tests
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

TEST_CASE("tac_fold_producer_jumps: test_and_zero_jump_fold.TestSimIntegration.test_arbitrary_mask (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.flags\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.flags\"]}}}",
      "Function(\"test\" true [\"@0.flags\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.flags\") Var(\"%0.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%0.1\") Constant(ConstInt(16)) Var(\"%1.1\"))\n"
      "  JumpIfFalse(Var(\"%1.1\") \".if_end@0\")\n"
      "  Label(\".test@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(7)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(3)))\n"
      "])\n",
      "Function(\"test\" true [\"@0.flags\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.flags\") Var(\"%0.1\"))\n"
      "  JumpIfMasked(Var(\"@0.flags\") 16 false \".if_end@0\")\n"
      "  Label(\".test@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(7)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(3)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_and_zero_jump_fold.TestSimIntegration.test_high_bit_clear (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.flags\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.flags\"]}}}",
      "Function(\"test\" true [\"@0.flags\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.flags\") Var(\"%0.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%0.1\") Constant(ConstInt(128)) Var(\"%1.1\"))\n"
      "  JumpIfFalse(Var(\"%1.1\") \".if_end@0\")\n"
      "  Label(\".test@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"test\" true [\"@0.flags\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.flags\") Var(\"%0.1\"))\n"
      "  JumpIfMasked(Var(\"@0.flags\") 128 false \".if_end@0\")\n"
      "  Label(\".test@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_and_zero_jump_fold.TestSimIntegration.test_store_between_extend_and_and (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.t.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.t.1\"]}}}",
      "Function(\"check\" true [] [\n"
      "  Label(\".check@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.t.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"g\"))\n"
      "  Binary(BitwiseAnd Var(\"@0.t.1\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  JumpIfFalse(Var(\"%2.1\") \".if_end@0\")\n"
      "  Label(\".check@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"check\" true [] [\n"
      "  Label(\".check@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.t.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"g\"))\n"
      "  Truncate(Var(\"@0.t.1\") Var(\"%2.1.b\"))\n"
      "  JumpIfMasked(Var(\"%2.1.b\") 1 false \".if_end@0\")\n"
      "  Label(\".check@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.t.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.b\", \"%3.1\", \"@0.t.1\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_and_zero_jump_fold.TestWideMaskInOneByte.test_byte_tested (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.2\", \"%1.3\", \"%2.1\"]}}}",
      "Function(\"hi\" true [] [\n"
      "  Label(\".hi@ssa_block@0\")\n"
      "  Binary(BitwiseAnd Var(\"w\") Constant(ConstUInt(32768)) Var(\"%0.1\"))\n"
      "  JumpIfFalse(Var(\"%0.1\") \".cond_else@0\")\n"
      "  Label(\".hi@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(255)) Var(\"%1.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%1.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%1.3\") [PhiArg(\".hi@ssa_block@1\" Var(\"%1.1\")) PhiArg(\".cond_else@0\" Var(\"%1.2\"))])\n"
      "  Truncate(Var(\"%1.3\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"hi\" true [] [\n"
      "  Label(\".hi@ssa_block@0\")\n"
      "  Binary(RightShift Var(\"w\") Constant(ConstInt(8)) Var(\"%0.1.byte\"))\n"
      "  Truncate(Var(\"%0.1.byte\") Var(\"%0.1.b\"))\n"
      "  JumpIfMasked(Var(\"%0.1.b\") 128 false \".cond_else@0\")\n"
      "  Label(\".hi@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(255)) Var(\"%1.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%1.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%1.3\") [PhiArg(\".hi@ssa_block@1\" Var(\"%1.1\")) PhiArg(\".cond_else@0\" Var(\"%1.2\"))])\n"
      "  Truncate(Var(\"%1.3\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1.byte\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%0.1.b\", \"%0.1.byte\", \"%1.1\", \"%1.2\", \"%1.3\", \"%2.1\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_bool.TestPrograms.test_asm_sim (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%11.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%11.1.n0\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.b.1\", \"@0.b.2\", \"@0.b.3\", \"@0.b.4\", \"@0.b.5\", \"@1.r.1\", \"@1.r.2\", \"@1.r.3\", \"@1.r.4\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%0.1\"))\n"
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
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%0.1\"))\n"
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
      "  Binary(NotEqual Var(\"%11.1.n0\") Constant(ConstUChar(0)) Var(\"%12.1\"))\n"
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
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestAsmShape.test_uint8_eq_zero_uses_bne (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.a\"]}}}",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.a\") Var(\"%0.1\"))\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(0)) Var(\"%1.1\"))\n"
      "  JumpIfFalse(Var(\"%1.1\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "])\n",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.a\") Var(\"%0.1\"))\n"
      "  JumpIfTrue(Var(\"@0.a\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestConstexprBound.test_byte_compare (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"T\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 16953, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"T\") Var(\"%0.1\"))\n"
      "  Copy(Constant(ConstInt(99)) Var(\"%1.1\"))\n"
      "  Binary(LessThan Var(\"%0.1\") Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  JumpIfFalse(Var(\"%2.1\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Var(\"T\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"T\") Constant(ConstUChar(1)) Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"T\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"T\") Var(\"%0.1\"))\n"
      "  Copy(Constant(ConstInt(99)) Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%0.1\") Var(\"%1.1\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Var(\"T\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"T\") Constant(ConstUChar(1)) Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"T\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestConstexprBound.test_byte_compare (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"T\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 16953, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"T\") Var(\"%0.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%0.1\") Constant(ConstInt(99)) \".if_end@0\")\n"
      "  Binary(Add Var(\"T\") Constant(ConstUChar(1)) Var(\"T\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"T\") Var(\"%0.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"T\") Constant(ConstUChar(99)) \".if_end@0\")\n"
      "  Binary(Add Var(\"T\") Constant(ConstUChar(1)) Var(\"T\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestEndToEnd.test_uint16_eq_zero_ternary (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"@0.a\"]}}}",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Equal Var(\"@0.a\") Constant(ConstUInt(0)) Var(\"%1.1\"))\n"
      "  JumpIfFalse(Var(\"%1.1\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(10)) Var(\"%2.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(20)) Var(\"%2.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%2.1\")) PhiArg(\".cond_else@0\" Var(\"%2.2\"))])\n"
      "  Ret(Var(\"%2.3\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  JumpIfTrue(Var(\"@0.a\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(10)) Var(\"%2.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(20)) Var(\"%2.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%2.1\")) PhiArg(\".cond_else@0\" Var(\"%2.2\"))])\n"
      "  Ret(Var(\"%2.3\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestEndToEnd.test_uint8_ne_zero (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.a\"]}}}",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.a\") Var(\"%0.1\"))\n"
      "  Binary(NotEqual Var(\"%0.1\") Constant(ConstInt(0)) Var(\"%1.1\"))\n"
      "  JumpIfFalse(Var(\"%1.1\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "])\n",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.a\") Var(\"%0.1\"))\n"
      "  JumpIfFalse(Var(\"@0.a\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestMultiUseSsaExtend.test_switch_on_byte_compares_bytes (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.n\"]}}}",
      "Function(\"pick\" true [\"@0.n\"] [\n"
      "  Label(\".pick@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.n\") Var(\"%0.1\"))\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(0)) Var(\"%1.1\"))\n"
      "  JumpIfTrue(Var(\"%1.1\") \".case@1\")\n"
      "  Label(\".pick@ssa_block@1\")\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  JumpIfTrue(Var(\"%2.1\") \".case@2\")\n"
      "  Label(\".pick@ssa_block@2\")\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(2)) Var(\"%3.1\"))\n"
      "  JumpIfTrue(Var(\"%3.1\") \".case@3\")\n"
      "  Label(\".pick@ssa_block@3\")\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(7)) Var(\"%4.1\"))\n"
      "  JumpIfTrue(Var(\"%4.1\") \".case@4\")\n"
      "  Label(\".pick@ssa_block@4\")\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(200)) Var(\"%5.1\"))\n"
      "  JumpIfTrue(Var(\"%5.1\") \".case@5\")\n"
      "  Label(\".pick@ssa_block@5\")\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(300)) Var(\"%6.1\"))\n"
      "  JumpIfTrue(Var(\"%6.1\") \".case@6\")\n"
      "  Label(\".pick@ssa_block@6\")\n"
      "  Jump(\".default@7\")\n"
      "  Label(\".case@1\")\n"
      "  Copy(Constant(ConstUChar(10)) Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "  Label(\".case@2\")\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "  Label(\".case@3\")\n"
      "  Copy(Constant(ConstUChar(12)) Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".case@4\")\n"
      "  Copy(Constant(ConstUChar(17)) Var(\"%10.1\"))\n"
      "  Ret(Var(\"%10.1\"))\n"
      "  Label(\".case@5\")\n"
      "  Copy(Constant(ConstUChar(99)) Var(\"%11.1\"))\n"
      "  Ret(Var(\"%11.1\"))\n"
      "  Label(\".case@6\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "  Label(\".default@7\")\n"
      "  Copy(Constant(ConstUChar(50)) Var(\"%13.1\"))\n"
      "  Ret(Var(\"%13.1\"))\n"
      "  Label(\".switch@0_break\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "Function(\"pick\" true [\"@0.n\"] [\n"
      "  Label(\".pick@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.n\") Var(\"%0.1\"))\n"
      "  JumpIfFalse(Var(\"@0.n\") \".case@1\")\n"
      "  Label(\".pick@ssa_block@1\")\n"
      "  JumpIfCmp(Equal Var(\"@0.n\") Constant(ConstUChar(1)) \".case@2\")\n"
      "  Label(\".pick@ssa_block@2\")\n"
      "  JumpIfCmp(Equal Var(\"@0.n\") Constant(ConstUChar(2)) \".case@3\")\n"
      "  Label(\".pick@ssa_block@3\")\n"
      "  JumpIfCmp(Equal Var(\"@0.n\") Constant(ConstUChar(7)) \".case@4\")\n"
      "  Label(\".pick@ssa_block@4\")\n"
      "  JumpIfCmp(Equal Var(\"@0.n\") Constant(ConstUChar(200)) \".case@5\")\n"
      "  Label(\".pick@ssa_block@5\")\n"
      "  JumpIfCmp(Equal Var(\"%0.1\") Constant(ConstInt(300)) \".case@6\")\n"
      "  Label(\".pick@ssa_block@6\")\n"
      "  Jump(\".default@7\")\n"
      "  Label(\".case@1\")\n"
      "  Copy(Constant(ConstUChar(10)) Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "  Label(\".case@2\")\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "  Label(\".case@3\")\n"
      "  Copy(Constant(ConstUChar(12)) Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".case@4\")\n"
      "  Copy(Constant(ConstUChar(17)) Var(\"%10.1\"))\n"
      "  Ret(Var(\"%10.1\"))\n"
      "  Label(\".case@5\")\n"
      "  Copy(Constant(ConstUChar(99)) Var(\"%11.1\"))\n"
      "  Ret(Var(\"%11.1\"))\n"
      "  Label(\".case@6\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "  Label(\".default@7\")\n"
      "  Copy(Constant(ConstUChar(50)) Var(\"%13.1\"))\n"
      "  Ret(Var(\"%13.1\"))\n"
      "  Label(\".switch@0_break\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestNarrowOrderingAsmShape.test_uchar_loop_returns_correct_count (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.i.1\", \"@0.i.2\", \"@0.i.3\", \"@1.sum.1\", \"@1.sum.2\", \"@1.sum.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@1.sum.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.sum.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.sum.3\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.i.3\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  Binary(LessThan Var(\"%1.1\") Constant(ConstInt(105)) Var(\"%2.1\"))\n"
      "  JumpIfFalse(Var(\"%2.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(Add Var(\"@1.sum.2\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"@1.sum.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"@0.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@1.sum.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.sum.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.sum.3\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.i.3\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(105)) \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(Add Var(\"@1.sum.2\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"@1.sum.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"@0.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestNarrowOrderingAsmShape.test_uchar_lt_const_uses_8bit_cmp_bcs (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@0.i.1\", \"@0.i.2\", \"@0.i.3\", \"@1.sum.1\", \"@1.sum.2\", \"@1.sum.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@1.sum.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.sum.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.sum.3\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.i.3\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  Binary(LessThan Var(\"%1.1\") Constant(ConstInt(105)) Var(\"%2.1\"))\n"
      "  JumpIfFalse(Var(\"%2.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@1.sum.2\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"@1.sum.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@0.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@1.sum.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.sum.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.sum.3\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.i.3\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(105)) \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@1.sum.2\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"@1.sum.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@0.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestSignedZeroOrderingAsmShape.test_signed_byte_ge_zero_uses_bare_lda_bpl (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@0.x.1\", \"@0.x.2\", \"@0.x.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstChar(15)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.x.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.x.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.x.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.x.3\"))])\n"
      "  Binary(Add Var(\"sum\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"sum\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@0.x.2\") Var(\"%4.1\"))\n"
      "  Binary(Subtract Var(\"@0.x.2\") Constant(ConstChar(1)) Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"@0.x.3\"))\n"
      "  SignExtend(Var(\"@0.x.3\") Var(\"%1.1\"))\n"
      "  Binary(GreaterOrEqual Var(\"%1.1\") Constant(ConstInt(0)) Var(\"%2.1\"))\n"
      "  JumpIfTrue(Var(\"%2.1\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"sum\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstChar(15)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.x.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.x.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.x.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.x.3\"))])\n"
      "  Binary(Add Var(\"sum\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"sum\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@0.x.2\") Var(\"%4.1\"))\n"
      "  Binary(Subtract Var(\"@0.x.2\") Constant(ConstChar(1)) Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"@0.x.3\"))\n"
      "  SignExtend(Var(\"@0.x.3\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.x.3\") Constant(ConstChar(0)) \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"sum\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@0.t.1\"]}}}",
      "Function(\"lt\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%0.1\") Constant(ConstInt(3)) \".if_end@0\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"lt\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%0.1\") Constant(ConstUInt(3)) \".if_end@0\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.t.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"@2.t.1\", \"@3.c.1\"]}}}",
      "Function(\"lt_c\" true [] [\n"
      "  Label(\".lt_c@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%12.1\"))\n"
      "  Copy(Var(\"%12.1\") Var(\"@2.t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"g\"))\n"
      "  Binary(LessThan Var(\"@2.t.1\") Constant(ConstInt(3)) Var(\"%14.1\"))\n"
      "  Copy(Var(\"%14.1\") Var(\"@3.c.1\"))\n"
      "  JumpIfFalse(Var(\"@3.c.1\") \".if_end@2\")\n"
      "  Label(\".lt_c@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%15.1\"))\n"
      "  Binary(BitwiseOr Var(\"%15.1\") Constant(ConstInt(4)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"lt_c\" true [] [\n"
      "  Label(\".lt_c@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%12.1\"))\n"
      "  Copy(Var(\"%12.1\") Var(\"@2.t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"g\"))\n"
      "  Binary(LessThan Var(\"@2.t.1\") Constant(ConstUInt(3)) Var(\"%14.1\"))\n"
      "  Copy(Var(\"%14.1\") Var(\"@3.c.1\"))\n"
      "  JumpIfFalse(Var(\"@3.c.1\") \".if_end@2\")\n"
      "  Label(\".lt_c@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%15.1\"))\n"
      "  Binary(BitwiseOr Var(\"%15.1\") Constant(ConstInt(4)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (3)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@0.t.1\"]}}}",
      "Function(\"lt\" true [] [\n"
      "  Label(\".lt@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"g\"))\n"
      "  Binary(LessThan Var(\"@0.t.1\") Constant(ConstInt(3)) Var(\"%2.1\"))\n"
      "  JumpIfFalse(Var(\"%2.1\") \".if_end@0\")\n"
      "  Label(\".lt@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseOr Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"lt\" true [] [\n"
      "  Label(\".lt@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"g\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.t.1\") Constant(ConstInt(3)) \".if_end@0\")\n"
      "  Label(\".lt@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseOr Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (4)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@1.t.1\"]}}}",
      "Function(\"eq0\" true [] [\n"
      "  Label(\".eq0@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"@1.t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"g\"))\n"
      "  Binary(Equal Var(\"@1.t.1\") Constant(ConstInt(0)) Var(\"%8.1\"))\n"
      "  JumpIfFalse(Var(\"%8.1\") \".if_end@1\")\n"
      "  Label(\".eq0@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%9.1\"))\n"
      "  Binary(BitwiseOr Var(\"%9.1\") Constant(ConstInt(2)) Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Copy(Var(\"%11.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"eq0\" true [] [\n"
      "  Label(\".eq0@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"@1.t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"g\"))\n"
      "  JumpIfTrue(Var(\"@1.t.1\") \".if_end@1\")\n"
      "  Label(\".eq0@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%9.1\"))\n"
      "  Binary(BitwiseOr Var(\"%9.1\") Constant(ConstInt(2)) Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Copy(Var(\"%11.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (5)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"@2.t.1\", \"@3.c.1\"]}}}",
      "Function(\"lt_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%12.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  Binary(LessThan Var(\"%12.1\") Constant(ConstUInt(3)) Var(\"%14.1\"))\n"
      "  JumpIfFalse(Var(\"%14.1\") \".if_end@2\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(4)) Var(\"hits\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"lt_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%12.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%12.1\") Constant(ConstUInt(3)) \".if_end@2\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(4)) Var(\"hits\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (6)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"@4.t.1\", \"@5.c.1\"]}}}",
      "Function(\"eq0_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%18.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  Binary(Equal Var(\"%18.1\") Constant(ConstInt(0)) Var(\"%20.1\"))\n"
      "  JumpIfFalse(Var(\"%20.1\") \".if_end@3\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(8)) Var(\"hits\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"eq0_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%18.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfTrue(Var(\"%18.1\") \".if_end@3\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(8)) Var(\"hits\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (7)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"@6.t.1\"]}}}",
      "Function(\"ge0\" true [] [\n"
      "  Label(\".ge0@ssa_block@0\")\n"
      "  SignExtend(Var(\"sg\") Var(\"%24.1\"))\n"
      "  Copy(Var(\"%24.1\") Var(\"@6.t.1\"))\n"
      "  Copy(Constant(ConstInt(-5)) Var(\"%25.1\"))\n"
      "  Truncate(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Copy(Var(\"%26.1\") Var(\"sg\"))\n"
      "  Binary(GreaterOrEqual Var(\"@6.t.1\") Constant(ConstInt(0)) Var(\"%27.1\"))\n"
      "  JumpIfFalse(Var(\"%27.1\") \".if_end@4\")\n"
      "  Label(\".ge0@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%28.1\"))\n"
      "  Binary(BitwiseOr Var(\"%28.1\") Constant(ConstInt(16)) Var(\"%29.1\"))\n"
      "  Truncate(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "  Copy(Var(\"%30.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"ge0\" true [] [\n"
      "  Label(\".ge0@ssa_block@0\")\n"
      "  SignExtend(Var(\"sg\") Var(\"%24.1\"))\n"
      "  Copy(Var(\"%24.1\") Var(\"@6.t.1\"))\n"
      "  Copy(Constant(ConstInt(-5)) Var(\"%25.1\"))\n"
      "  Truncate(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Copy(Var(\"%26.1\") Var(\"sg\"))\n"
      "  JumpIfCmp(LessThan Var(\"@6.t.1\") Constant(ConstInt(0)) \".if_end@4\")\n"
      "  Label(\".ge0@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%28.1\"))\n"
      "  Binary(BitwiseOr Var(\"%28.1\") Constant(ConstInt(16)) Var(\"%29.1\"))\n"
      "  Truncate(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "  Copy(Var(\"%30.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_one_byte_compare (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Var(\"%0.1\") Constant(ConstInt(10)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_one_byte_compare (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@2.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@1.n.1\", \"@1.n.2\", \"@1.n.3\", \"@2.i.1\", \"@2.i.2\", \"@2.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"@1.n.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"@2.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.n.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.n.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.n.3\"))])\n"
      "  Phi(Var(\"@2.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.i.3\"))])\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%5.1\"))\n"
      "  Binary(LessThan Var(\"%5.1\") Constant(ConstInt(8)) Var(\"%6.1\"))\n"
      "  JumpIfFalse(Var(\"%6.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@1.n.2\") Constant(ConstInt(1)) Var(\"%7.1\"))\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  IndexedLoad(\"g\" Var(\"%9.1\") Var(\"%10.1\") false 0 0)\n"
      "  FunctionCall(\"test\" [Var(\"%10.1\")] Var(\"%11.1\") false false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Copy(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Copy(Var(\"%14.1\") Var(\"@1.n.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@2.i.2\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%16.1\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@2.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Var(\"@1.n.2\") Var(\"%17.1\"))\n"
      "  Ret(Var(\"%17.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"@1.n.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"@2.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.n.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.n.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.n.3\"))])\n"
      "  Phi(Var(\"@2.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.i.3\"))])\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%5.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@2.i.2\") Constant(ConstUChar(8)) \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@1.n.2\") Constant(ConstInt(1)) Var(\"%7.1\"))\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  IndexedLoad(\"g\" Var(\"%9.1\") Var(\"%10.1\") false 0 0)\n"
      "  FunctionCall(\"test\" [Var(\"%10.1\")] Var(\"%11.1\") false false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Copy(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Copy(Var(\"%14.1\") Var(\"@1.n.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@2.i.2\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%16.1\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@2.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Var(\"@1.n.2\") Var(\"%17.1\"))\n"
      "  Ret(Var(\"%17.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Var(\"%0.1\") Constant(ConstInt(256)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Var(\"%0.1\") Constant(ConstUInt(256)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterOrEqual Var(\"%0.1\") Constant(ConstInt(200)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterOrEqual Var(\"@0.x\") Constant(ConstUChar(200)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (3)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterThan Constant(ConstInt(255)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterThan Constant(ConstUChar(255)) Var(\"@0.x\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (4)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterThan Var(\"%0.1\") Constant(ConstInt(127)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterThan Var(\"@0.x\") Constant(ConstUChar(127)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (5)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessOrEqual Var(\"%0.1\") Constant(ConstInt(10)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessOrEqual Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (6)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Constant(ConstInt(10)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Label(\".test@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Constant(ConstUChar(10)) Var(\"@0.x\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (7)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@2.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@1.n.1\", \"@1.n.2\", \"@1.n.3\", \"@2.i.1\", \"@2.i.2\", \"@2.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"@1.n.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"@2.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.n.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.n.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.n.3\"))])\n"
      "  Phi(Var(\"@2.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.i.3\"))])\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%6.1\"))\n"
      "  Binary(LessThan Var(\"%6.1\") Constant(ConstInt(8)) Var(\"%7.1\"))\n"
      "  JumpIfFalse(Var(\"%7.1\") \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@1.n.2\") Constant(ConstInt(1)) Var(\"%8.1\"))\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%9.1\"))\n"
      "  Truncate(Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  IndexedLoad(\"g\" Var(\"%10.1\") Var(\"%11.1\") false 0 0)\n"
      "  FunctionCall(\"test\" [Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseOr Var(\"%8.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@1.n.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@2.i.2\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"@2.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Var(\"@1.n.2\") Var(\"%18.1\"))\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"@1.n.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"@2.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.n.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.n.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.n.3\"))])\n"
      "  Phi(Var(\"@2.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.i.3\"))])\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%6.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@2.i.2\") Constant(ConstUChar(8)) \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@1.n.2\") Constant(ConstInt(1)) Var(\"%8.1\"))\n"
      "  ZeroExtend(Var(\"@2.i.2\") Var(\"%9.1\"))\n"
      "  Truncate(Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  IndexedLoad(\"g\" Var(\"%10.1\") Var(\"%11.1\") false 0 0)\n"
      "  FunctionCall(\"test\" [Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseOr Var(\"%8.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@1.n.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@2.i.2\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"@2.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Var(\"@1.n.2\") Var(\"%18.1\"))\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestCompiled.test_bit_7_compare (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.c.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.c.1\"]}}}",
      "Function(\"k\" true [] [\n"
      "  Label(\".k@ssa_block@0\")\n"
      "  IndexedLoad(\"KBD\" Constant(ConstUChar(0)) Var(\"%0.1\") true 0 0)\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.c.1\"))\n"
      "  ZeroExtend(Var(\"@0.c.1\") Var(\"%1.1\"))\n"
      "  Binary(LessThan Var(\"%1.1\") Constant(ConstInt(128)) Var(\"%2.1\"))\n"
      "  JumpIfFalse(Var(\"%2.1\") \".if_end@0\")\n"
      "  Label(\".k@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Var(\"@0.c.1\"))\n"
      "])\n",
      "Function(\"k\" true [] [\n"
      "  Label(\".k@ssa_block@0\")\n"
      "  IndexedLoad(\"KBD\" Constant(ConstUChar(0)) Var(\"%0.1\") true 0 0)\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.c.1\"))\n"
      "  ZeroExtend(Var(\"@0.c.1\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.c.1\") Constant(ConstUChar(128)) \".if_end@0\")\n"
      "  Label(\".k@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Var(\"@0.c.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestCompiled.test_byte_plus_one_unsigned (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"W\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 35, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.top\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.r.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@1.top\", \"@2.r.1\", \"@2.r.2\", \"@2.r.3\"]}}}",
      "Function(\"f\" true [\"@1.top\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.r.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@1.top\")) PhiArg(\".loop@0_continue\" Var(\"%5.1\"))])\n"
      "  ZeroExtend(Var(\"@2.r.2\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  ZeroExtend(Var(\"W\") Var(\"%2.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%1.1\") Var(\"%2.1\") \".loop@0_break\")\n"
      "  FunctionCall(\"copy\" [Var(\"@2.r.2\")] None false false)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@2.r.2\") Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@1.top\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.r.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@1.top\")) PhiArg(\".loop@0_continue\" Var(\"%5.1\"))])\n"
      "  ZeroExtend(Var(\"@2.r.2\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  ZeroExtend(Var(\"W\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"%1.1.u0\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%1.1.u0\") Var(\"%2.1\") \".loop@0_break\")\n"
      "  FunctionCall(\"copy\" [Var(\"@2.r.2\")] None false false)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@2.r.2\") Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"W\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 35, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.top\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.r.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1.u0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.1.u0\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@1.top\", \"@2.r.1\", \"@2.r.2\", \"@2.r.3\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestCompiled.test_byte_plus_one_unsigned (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"W\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 35, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@1.top\", \"@2.r.1\", \"@2.r.2\", \"@2.r.3\"]}}}",
      "Function(\"f\" true [\"@1.top\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"@1.top\") Var(\"@2.r.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.r.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@2.r.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.r.3\"))])\n"
      "  ZeroExtend(Var(\"@2.r.2\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  ZeroExtend(Var(\"W\") Var(\"%2.1\"))\n"
      "  Binary(LessThan Var(\"%1.1\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  JumpIfFalse(Var(\"%3.1\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  FunctionCall(\"copy\" [Var(\"@2.r.2\")] None false false)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@2.r.2\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"@2.r.2\") Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"@2.r.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@1.top\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"@1.top\") Var(\"@2.r.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@2.r.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@2.r.1\")) PhiArg(\".loop@0_continue\" Var(\"@2.r.3\"))])\n"
      "  ZeroExtend(Var(\"@2.r.2\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  ZeroExtend(Var(\"W\") Var(\"%2.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%1.1\") Var(\"%2.1\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  FunctionCall(\"copy\" [Var(\"@2.r.2\")] None false false)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@2.r.2\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"@2.r.2\") Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "  Copy(Var(\"%5.1\") Var(\"@2.r.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestCompiled.test_high_byte_test (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"@0.e.1\"]}}}",
      "Function(\"g\" true [] [\n"
      "  Label(\".g@ssa_block@0\")\n"
      "  FunctionCall(\"find\" [] Var(\"%0.1\") false false)\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.e.1\"))\n"
      "  Binary(GreaterThan Var(\"@0.e.1\") Constant(ConstUInt(255)) Var(\"%2.1\"))\n"
      "  JumpIfFalse(Var(\"%2.1\") \".if_end@0\")\n"
      "  Label(\".g@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%4.1\"))\n"
      "  Ret(Var(\"%4.1\"))\n"
      "])\n",
      "Function(\"g\" true [] [\n"
      "  Label(\".g@ssa_block@0\")\n"
      "  FunctionCall(\"find\" [] Var(\"%0.1\") false false)\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.e.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"@0.e.1\") Constant(ConstUInt(255)) \".if_end@0\")\n"
      "  Label(\".g@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%4.1\"))\n"
      "  Ret(Var(\"%4.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestCompiled.test_not_of_bool (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"flag\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": true}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"%3.1\", \"%4.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Unary(LogicalNot Var(\"flag\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(3)) Var(\"%2.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"%2.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%2.1\")) PhiArg(\".cond_else@0\" Var(\"%2.2\"))])\n"
      "  Binary(Add Var(\"%2.3\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Truncate(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(BitwiseXor Var(\"flag\") Constant(ConstUChar(1)) Var(\"%0.1.not0\"))\n"
      "  ZeroExtend(Var(\"%0.1.not0\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(3)) Var(\"%2.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"%2.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%2.1\")) PhiArg(\".cond_else@0\" Var(\"%2.2\"))])\n"
      "  Binary(Add Var(\"%2.3\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Truncate(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%4.1\") Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"flag\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": true}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1.not0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%0.1.not0\", \"%1.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"%3.1\", \"%4.1\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestCompiled.test_two_values_one_test (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.2\", \"%1.3\", \"%3.1\", \"%5.1\", \"@0.e.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  FunctionCall(\"find\" [] Var(\"%0.1\") false false)\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.e.1\"))\n"
      "  Binary(Equal Var(\"@0.e.1\") Constant(ConstUInt(0)) Var(\"%3.1\"))\n"
      "  JumpIfTrue(Var(\"%3.1\") \".or_true@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Binary(Equal Var(\"@0.e.1\") Constant(ConstUInt(1)) Var(\"%5.1\"))\n"
      "  JumpIfTrue(Var(\"%5.1\") \".or_true@0\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%1.1\"))\n"
      "  Jump(\".or_end@1\")\n"
      "  Label(\".or_true@0\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%1.2\"))\n"
      "  Label(\".or_end@1\")\n"
      "  Phi(Var(\"%1.3\") [PhiArg(\".f@ssa_block@2\" Var(\"%1.1\")) PhiArg(\".or_true@0\" Var(\"%1.2\"))])\n"
      "  JumpIfFalse(Var(\"%1.3\") \".if_end@2\")\n"
      "  Label(\".f@ssa_block@3\")\n"
      "  Ret(None)\n"
      "  Label(\".if_end@2\")\n"
      "  FunctionCall(\"draw\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  FunctionCall(\"find\" [] Var(\"%0.1\") false false)\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.e.1\"))\n"
      "  JumpIfFalse(Var(\"@0.e.1\") \".or_true@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  JumpIfCmp(Equal Var(\"@0.e.1\") Constant(ConstUInt(1)) \".or_true@0\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%1.1\"))\n"
      "  Jump(\".or_end@1\")\n"
      "  Label(\".or_true@0\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%1.2\"))\n"
      "  Label(\".or_end@1\")\n"
      "  Phi(Var(\"%1.3\") [PhiArg(\".f@ssa_block@2\" Var(\"%1.1\")) PhiArg(\".or_true@0\" Var(\"%1.2\"))])\n"
      "  JumpIfFalse(Var(\"%1.3\") \".if_end@2\")\n"
      "  Label(\".f@ssa_block@3\")\n"
      "  Ret(None)\n"
      "  Label(\".if_end@2\")\n"
      "  FunctionCall(\"draw\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.v\"]}}}",
      "Function(\"bit7\" true [\"@0.v\"] [\n"
      "  Label(\".bit7@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.v\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Var(\"%0.1\") Constant(ConstInt(128)) Var(\"%1.1\"))\n"
      "  JumpIfFalse(Var(\"%1.1\") \".if_end@0\")\n"
      "  Label(\".bit7@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"bit7\" true [\"@0.v\"] [\n"
      "  Label(\".bit7@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.v\") Var(\"%0.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.v\") Constant(ConstUChar(128)) \".if_end@0\")\n"
      "  Label(\".bit7@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@1.v\"]}}}",
      "Function(\"one\" true [\"@1.v\"] [\n"
      "  Label(\".one@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.v\") Var(\"%4.1\"))\n"
      "  Binary(GreaterOrEqual Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  JumpIfFalse(Var(\"%5.1\") \".if_end@1\")\n"
      "  Label(\".one@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "Function(\"one\" true [\"@1.v\"] [\n"
      "  Label(\".one@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.v\") Var(\"%4.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@1.v\") Constant(ConstUChar(1)) \".if_end@1\")\n"
      "  Label(\".one@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (3)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%8.1\", \"%9.1\", \"@2.c\"]}}}",
      "Function(\"pos\" true [\"@2.c\"] [\n"
      "  Label(\".pos@ssa_block@0\")\n"
      "  SignExtend(Var(\"@2.c\") Var(\"%8.1\"))\n"
      "  Binary(LessThan Var(\"%8.1\") Constant(ConstInt(0)) Var(\"%9.1\"))\n"
      "  JumpIfFalse(Var(\"%9.1\") \".if_end@2\")\n"
      "  Label(\".pos@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%10.1\"))\n"
      "  Ret(Var(\"%10.1\"))\n"
      "  Label(\".if_end@2\")\n"
      "  SignExtend(Var(\"@2.c\") Var(\"%11.1\"))\n"
      "  Binary(GreaterThan Var(\"%11.1\") Constant(ConstInt(28)) Var(\"%12.1\"))\n"
      "  JumpIfFalse(Var(\"%12.1\") \".if_end@3\")\n"
      "  Label(\".pos@ssa_block@2\")\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%13.1\"))\n"
      "  Ret(Var(\"%13.1\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%14.1\"))\n"
      "  Ret(Var(\"%14.1\"))\n"
      "])\n",
      "Function(\"pos\" true [\"@2.c\"] [\n"
      "  Label(\".pos@ssa_block@0\")\n"
      "  SignExtend(Var(\"@2.c\") Var(\"%8.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@2.c\") Constant(ConstChar(0)) \".if_end@2\")\n"
      "  Label(\".pos@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%10.1\"))\n"
      "  Ret(Var(\"%10.1\"))\n"
      "  Label(\".if_end@2\")\n"
      "  SignExtend(Var(\"@2.c\") Var(\"%11.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"@2.c\") Constant(ConstChar(28)) \".if_end@3\")\n"
      "  Label(\".pos@ssa_block@2\")\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%13.1\"))\n"
      "  Ret(Var(\"%13.1\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%14.1\"))\n"
      "  Ret(Var(\"%14.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (4)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@3.top\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.btm\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.r.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"@3.top\", \"@4.btm\", \"@5.n.1\", \"@5.n.2\", \"@5.n.3\", \"@6.r.1\", \"@6.r.2\", \"@6.r.3\"]}}}",
      "Function(\"rows\" true [\"@3.top\" \"@4.btm\"] [\n"
      "  Label(\".rows@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@4.btm\") Var(\"%18.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@5.n.2\") [PhiArg(\".rows@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%21.1\"))])\n"
      "  Phi(Var(\"@6.r.2\") [PhiArg(\".rows@ssa_block@0\" Var(\"@3.top\")) PhiArg(\".loop@0_continue\" Var(\"%23.1\"))])\n"
      "  ZeroExtend(Var(\"@6.r.2\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Constant(ConstInt(1)) Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%17.1\") Var(\"%18.1\") \".loop@0_break\")\n"
      "  Binary(Add Var(\"@5.n.2\") Constant(ConstUChar(1)) Var(\"%21.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@6.r.2\") Constant(ConstUChar(1)) Var(\"%23.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@5.n.2\"))\n"
      "])\n",
      "Function(\"rows\" true [\"@3.top\" \"@4.btm\"] [\n"
      "  Label(\".rows@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@4.btm\") Var(\"%18.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@5.n.2\") [PhiArg(\".rows@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%21.1\"))])\n"
      "  Phi(Var(\"@6.r.2\") [PhiArg(\".rows@ssa_block@0\" Var(\"@3.top\")) PhiArg(\".loop@0_continue\" Var(\"%23.1\"))])\n"
      "  ZeroExtend(Var(\"@6.r.2\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Constant(ConstInt(1)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"%17.1.u0\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%17.1.u0\") Var(\"%18.1\") \".loop@0_break\")\n"
      "  Binary(Add Var(\"@5.n.2\") Constant(ConstUChar(1)) Var(\"%21.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@6.r.2\") Constant(ConstUChar(1)) Var(\"%23.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@5.n.2\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@3.top\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.btm\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.r.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1.u0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%15.1\", \"%16.1\", \"%17.1\", \"%17.1.u0\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"@3.top\", \"@4.btm\", \"@5.n.1\", \"@5.n.2\", \"@5.n.3\", \"@6.r.1\", \"@6.r.2\", \"@6.r.3\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (5)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@4.btm\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"@3.top\", \"@4.btm\", \"@5.n.1\", \"@5.n.2\", \"@5.n.3\", \"@6.r.1\", \"@6.r.2\", \"@6.r.3\"]}}}",
      "Function(\"rows\" true [\"@3.top\" \"@4.btm\"] [\n"
      "  Label(\".rows@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@5.n.1\"))\n"
      "  Copy(Var(\"@3.top\") Var(\"@6.r.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@5.n.2\") [PhiArg(\".rows@ssa_block@0\" Var(\"@5.n.1\")) PhiArg(\".loop@0_continue\" Var(\"@5.n.3\"))])\n"
      "  Phi(Var(\"@6.r.2\") [PhiArg(\".rows@ssa_block@0\" Var(\"@6.r.1\")) PhiArg(\".loop@0_continue\" Var(\"@6.r.3\"))])\n"
      "  ZeroExtend(Var(\"@6.r.2\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Constant(ConstInt(1)) Var(\"%17.1\"))\n"
      "  ZeroExtend(Var(\"@4.btm\") Var(\"%18.1\"))\n"
      "  Binary(LessThan Var(\"%17.1\") Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  JumpIfFalse(Var(\"%19.1\") \".loop@0_break\")\n"
      "  Label(\".rows@ssa_block@1\")\n"
      "  Copy(Var(\"@5.n.2\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"@5.n.2\") Constant(ConstUChar(1)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@5.n.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@6.r.2\") Var(\"%22.1\"))\n"
      "  Binary(Add Var(\"@6.r.2\") Constant(ConstUChar(1)) Var(\"%23.1\"))\n"
      "  Copy(Var(\"%23.1\") Var(\"@6.r.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@5.n.2\"))\n"
      "])\n",
      "Function(\"rows\" true [\"@3.top\" \"@4.btm\"] [\n"
      "  Label(\".rows@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@5.n.1\"))\n"
      "  Copy(Var(\"@3.top\") Var(\"@6.r.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@5.n.2\") [PhiArg(\".rows@ssa_block@0\" Var(\"@5.n.1\")) PhiArg(\".loop@0_continue\" Var(\"@5.n.3\"))])\n"
      "  Phi(Var(\"@6.r.2\") [PhiArg(\".rows@ssa_block@0\" Var(\"@6.r.1\")) PhiArg(\".loop@0_continue\" Var(\"@6.r.3\"))])\n"
      "  ZeroExtend(Var(\"@6.r.2\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Constant(ConstInt(1)) Var(\"%17.1\"))\n"
      "  ZeroExtend(Var(\"@4.btm\") Var(\"%18.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%17.1\") Var(\"%18.1\") \".loop@0_break\")\n"
      "  Label(\".rows@ssa_block@1\")\n"
      "  Copy(Var(\"@5.n.2\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"@5.n.2\") Constant(ConstUChar(1)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@5.n.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@6.r.2\") Var(\"%22.1\"))\n"
      "  Binary(Add Var(\"@6.r.2\") Constant(ConstUChar(1)) Var(\"%23.1\"))\n"
      "  Copy(Var(\"%23.1\") Var(\"@6.r.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@5.n.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (6)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@8.e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%31.1\", \"%31.1.b\", \"%31.1.byte\", \"%32.1\", \"%33.1\", \"%33.2\", \"%33.3\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"@8.e\"]}}}",
      "Function(\"hi\" true [\"@8.e\"] [\n"
      "  Binary(RightShift Var(\"@8.e\") Constant(ConstInt(8)) Var(\"%31.1.byte\"))\n"
      "  Truncate(Var(\"%31.1.byte\") Var(\"%31.1.b\"))\n"
      "  JumpIfMasked(Var(\"%31.1.b\") 128 false \".if_end@7\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@7\")\n"
      "  JumpIfCmp(LessOrEqual Var(\"@8.e\") Constant(ConstUInt(255)) \".and_false@8\")\n"
      "  Binary(BitwiseAnd Var(\"@8.e\") Constant(ConstUInt(32768)) Var(\"%36.1\"))\n"
      "  JumpIfTrue(Var(\"%36.1\") \".and_false@8\")\n"
      "  Label(\".hi@ssa_block@3\")\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%33.3\") [PhiArg(\".hi@ssa_block@3\" Constant(ConstInt(1))) PhiArg(\".and_false@8\" Constant(ConstInt(0)))])\n"
      "  JumpIfTrue(Var(\"%33.3\") \".if_end@10\")\n"
      "  Ret(Constant(ConstUChar(2)))\n"
      "  Label(\".if_end@10\")\n"
      "  Ret(Constant(ConstUChar(3)))\n"
      "])\n",
      "Function(\"hi\" true [\"@8.e\"] [\n"
      "  Binary(RightShift Var(\"@8.e\") Constant(ConstInt(8)) Var(\"%31.1.byte\"))\n"
      "  Truncate(Var(\"%31.1.byte\") Var(\"%31.1.b\"))\n"
      "  JumpIfMasked(Var(\"%31.1.b\") 128 false \".if_end@7\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@7\")\n"
      "  JumpIfCmp(LessOrEqual Var(\"@8.e\") Constant(ConstUInt(255)) \".and_false@8\")\n"
      "  Binary(RightShift Var(\"@8.e\") Constant(ConstInt(8)) Var(\"%36.1.byte\"))\n"
      "  Truncate(Var(\"%36.1.byte\") Var(\"%36.1.b\"))\n"
      "  JumpIfMasked(Var(\"%36.1.b\") 128 true \".and_false@8\")\n"
      "  Label(\".hi@ssa_block@3\")\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%33.3\") [PhiArg(\".hi@ssa_block@3\" Constant(ConstInt(1))) PhiArg(\".and_false@8\" Constant(ConstInt(0)))])\n"
      "  JumpIfTrue(Var(\"%33.3\") \".if_end@10\")\n"
      "  Ret(Constant(ConstUChar(2)))\n"
      "  Label(\".if_end@10\")\n"
      "  Ret(Constant(ConstUChar(3)))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@8.e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36.1.byte\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36.1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%31.1\", \"%31.1.b\", \"%31.1.byte\", \"%32.1\", \"%33.1\", \"%33.2\", \"%33.3\", \"%35.1\", \"%36.1\", \"%36.1.b\", \"%36.1.byte\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"@8.e\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (7)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@8.e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%31.1\", \"%32.1\", \"%33.1\", \"%33.2\", \"%33.3\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"@8.e\"]}}}",
      "Function(\"hi\" true [\"@8.e\"] [\n"
      "  Label(\".hi@ssa_block@0\")\n"
      "  Binary(BitwiseAnd Var(\"@8.e\") Constant(ConstUInt(32768)) Var(\"%31.1\"))\n"
      "  JumpIfFalse(Var(\"%31.1\") \".if_end@7\")\n"
      "  Label(\".hi@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%32.1\"))\n"
      "  Ret(Var(\"%32.1\"))\n"
      "  Label(\".if_end@7\")\n"
      "  Binary(GreaterThan Var(\"@8.e\") Constant(ConstUInt(255)) Var(\"%35.1\"))\n"
      "  JumpIfFalse(Var(\"%35.1\") \".and_false@8\")\n"
      "  Label(\".hi@ssa_block@2\")\n"
      "  Binary(BitwiseAnd Var(\"@8.e\") Constant(ConstUInt(32768)) Var(\"%36.1\"))\n"
      "  Unary(LogicalNot Var(\"%36.1\") Var(\"%37.1\"))\n"
      "  JumpIfFalse(Var(\"%37.1\") \".and_false@8\")\n"
      "  Label(\".hi@ssa_block@3\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%33.1\"))\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%33.2\"))\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%33.3\") [PhiArg(\".hi@ssa_block@3\" Var(\"%33.1\")) PhiArg(\".and_false@8\" Var(\"%33.2\"))])\n"
      "  Unary(LogicalNot Var(\"%33.3\") Var(\"%38.1\"))\n"
      "  JumpIfFalse(Var(\"%38.1\") \".if_end@10\")\n"
      "  Label(\".hi@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%39.1\"))\n"
      "  Ret(Var(\"%39.1\"))\n"
      "  Label(\".if_end@10\")\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%40.1\"))\n"
      "  Ret(Var(\"%40.1\"))\n"
      "])\n",
      "Function(\"hi\" true [\"@8.e\"] [\n"
      "  Label(\".hi@ssa_block@0\")\n"
      "  Binary(RightShift Var(\"@8.e\") Constant(ConstInt(8)) Var(\"%31.1.byte\"))\n"
      "  Truncate(Var(\"%31.1.byte\") Var(\"%31.1.b\"))\n"
      "  JumpIfMasked(Var(\"%31.1.b\") 128 false \".if_end@7\")\n"
      "  Label(\".hi@ssa_block@1\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%32.1\"))\n"
      "  Ret(Var(\"%32.1\"))\n"
      "  Label(\".if_end@7\")\n"
      "  JumpIfCmp(LessOrEqual Var(\"@8.e\") Constant(ConstUInt(255)) \".and_false@8\")\n"
      "  Label(\".hi@ssa_block@2\")\n"
      "  Binary(BitwiseAnd Var(\"@8.e\") Constant(ConstUInt(32768)) Var(\"%36.1\"))\n"
      "  JumpIfTrue(Var(\"%36.1\") \".and_false@8\")\n"
      "  Label(\".hi@ssa_block@3\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%33.1\"))\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%33.2\"))\n"
      "  Label(\".and_end@9\")\n"
      "  Phi(Var(\"%33.3\") [PhiArg(\".hi@ssa_block@3\" Var(\"%33.1\")) PhiArg(\".and_false@8\" Var(\"%33.2\"))])\n"
      "  JumpIfTrue(Var(\"%33.3\") \".if_end@10\")\n"
      "  Label(\".hi@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%39.1\"))\n"
      "  Ret(Var(\"%39.1\"))\n"
      "  Label(\".if_end@10\")\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%40.1\"))\n"
      "  Ret(Var(\"%40.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@8.e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1.byte\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%31.1\", \"%31.1.b\", \"%31.1.byte\", \"%32.1\", \"%33.1\", \"%33.2\", \"%33.3\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"@8.e\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (8)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"flag\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": true}}}}], [\"%41.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%41.1\", \"%42.1\", \"%43.1\", \"%43.2\", \"%43.3\", \"%44.1\", \"%45.1\"]}}}",
      "Function(\"nots\" true [] [\n"
      "  Label(\".nots@ssa_block@0\")\n"
      "  Unary(LogicalNot Var(\"flag\") Var(\"%41.1\"))\n"
      "  Truncate(Var(\"%41.1\") Var(\"%42.1\"))\n"
      "  Copy(Var(\"%42.1\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@11\")\n"
      "  Label(\".nots@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(3)) Var(\"%43.1\"))\n"
      "  Jump(\".cond_end@12\")\n"
      "  Label(\".cond_else@11\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"%43.2\"))\n"
      "  Label(\".cond_end@12\")\n"
      "  Phi(Var(\"%43.3\") [PhiArg(\".nots@ssa_block@1\" Var(\"%43.1\")) PhiArg(\".cond_else@11\" Var(\"%43.2\"))])\n"
      "  Binary(Add Var(\"%43.3\") Constant(ConstInt(1)) Var(\"%44.1\"))\n"
      "  Truncate(Var(\"%44.1\") Var(\"%45.1\"))\n"
      "  Copy(Var(\"%45.1\") Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"nots\" true [] [\n"
      "  Label(\".nots@ssa_block@0\")\n"
      "  Binary(BitwiseXor Var(\"flag\") Constant(ConstUChar(1)) Var(\"%41.1.not0\"))\n"
      "  ZeroExtend(Var(\"%41.1.not0\") Var(\"%41.1\"))\n"
      "  Truncate(Var(\"%41.1\") Var(\"%42.1\"))\n"
      "  Copy(Var(\"%42.1\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@11\")\n"
      "  Label(\".nots@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(3)) Var(\"%43.1\"))\n"
      "  Jump(\".cond_end@12\")\n"
      "  Label(\".cond_else@11\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"%43.2\"))\n"
      "  Label(\".cond_end@12\")\n"
      "  Phi(Var(\"%43.3\") [PhiArg(\".nots@ssa_block@1\" Var(\"%43.1\")) PhiArg(\".cond_else@11\" Var(\"%43.2\"))])\n"
      "  Binary(Add Var(\"%43.3\") Constant(ConstInt(1)) Var(\"%44.1\"))\n"
      "  Truncate(Var(\"%44.1\") Var(\"%45.1\"))\n"
      "  Copy(Var(\"%45.1\") Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"flag\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": true}}}}], [\"%41.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41.1.not0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%41.1\", \"%41.1.not0\", \"%42.1\", \"%43.1\", \"%43.2\", \"%43.3\", \"%44.1\", \"%45.1\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_flag_compares.TestSimulated.test_both_pipelines (9)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%24.1\", \"%24.2\", \"%24.3\", \"%26.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"@7.e\"]}}}",
      "Function(\"pair\" true [\"@7.e\"] [\n"
      "  Label(\".pair@ssa_block@0\")\n"
      "  Binary(Equal Var(\"@7.e\") Constant(ConstUInt(0)) Var(\"%26.1\"))\n"
      "  JumpIfTrue(Var(\"%26.1\") \".or_true@4\")\n"
      "  Label(\".pair@ssa_block@1\")\n"
      "  Binary(Equal Var(\"@7.e\") Constant(ConstUInt(1)) Var(\"%28.1\"))\n"
      "  JumpIfTrue(Var(\"%28.1\") \".or_true@4\")\n"
      "  Label(\".pair@ssa_block@2\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%24.1\"))\n"
      "  Jump(\".or_end@5\")\n"
      "  Label(\".or_true@4\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%24.2\"))\n"
      "  Label(\".or_end@5\")\n"
      "  Phi(Var(\"%24.3\") [PhiArg(\".pair@ssa_block@2\" Var(\"%24.1\")) PhiArg(\".or_true@4\" Var(\"%24.2\"))])\n"
      "  JumpIfFalse(Var(\"%24.3\") \".if_end@6\")\n"
      "  Label(\".pair@ssa_block@3\")\n"
      "  Copy(Constant(ConstUChar(8)) Var(\"%29.1\"))\n"
      "  Ret(Var(\"%29.1\"))\n"
      "  Label(\".if_end@6\")\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%30.1\"))\n"
      "  Ret(Var(\"%30.1\"))\n"
      "])\n",
      "Function(\"pair\" true [\"@7.e\"] [\n"
      "  Label(\".pair@ssa_block@0\")\n"
      "  JumpIfFalse(Var(\"@7.e\") \".or_true@4\")\n"
      "  Label(\".pair@ssa_block@1\")\n"
      "  JumpIfCmp(Equal Var(\"@7.e\") Constant(ConstUInt(1)) \".or_true@4\")\n"
      "  Label(\".pair@ssa_block@2\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%24.1\"))\n"
      "  Jump(\".or_end@5\")\n"
      "  Label(\".or_true@4\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%24.2\"))\n"
      "  Label(\".or_end@5\")\n"
      "  Phi(Var(\"%24.3\") [PhiArg(\".pair@ssa_block@2\" Var(\"%24.1\")) PhiArg(\".or_true@4\" Var(\"%24.2\"))])\n"
      "  JumpIfFalse(Var(\"%24.3\") \".if_end@6\")\n"
      "  Label(\".pair@ssa_block@3\")\n"
      "  Copy(Constant(ConstUChar(8)) Var(\"%29.1\"))\n"
      "  Ret(Var(\"%29.1\"))\n"
      "  Label(\".if_end@6\")\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%30.1\"))\n"
      "  Ret(Var(\"%30.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_lnot_jump_fold.TestAsmShape.test_lnot_uchar_collapses_to_single_branch (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.a\"]}}}",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Unary(LogicalNot Var(\"@0.a\") Var(\"%0.1\"))\n"
      "  JumpIfFalse(Var(\"%0.1\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "])\n",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  JumpIfTrue(Var(\"@0.a\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_lnot_jump_fold.TestEndToEnd.test_negated_in_ternary (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.2\", \"%1.3\", \"@0.a\"]}}}",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Unary(LogicalNot Var(\"@0.a\") Var(\"%0.1\"))\n"
      "  JumpIfFalse(Var(\"%0.1\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(10)) Var(\"%1.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(20)) Var(\"%1.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%1.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%1.1\")) PhiArg(\".cond_else@0\" Var(\"%1.2\"))])\n"
      "  Ret(Var(\"%1.3\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.a\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  JumpIfTrue(Var(\"@0.a\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(10)) Var(\"%1.1\"))\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Copy(Constant(ConstInt(20)) Var(\"%1.2\"))\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%1.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%1.1\")) PhiArg(\".cond_else@0\" Var(\"%1.2\"))])\n"
      "  Ret(Var(\"%1.3\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_narrow_byte_bitwise.TestEndToEnd.test_byte_compares (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%11.1.n0\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.n0\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%19.1.n0\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%6.1\", \"%7.1\", \"%7.1.n0\", \"%8.1\", \"%9.1\", \"@2.n.1\", \"@2.n.10\", \"@2.n.11\", \"@2.n.12\", \"@2.n.13\", \"@2.n.14\", \"@2.n.15\", \"@2.n.16\", \"@2.n.17\", \"@2.n.2\", \"@2.n.3\", \"@2.n.4\", \"@2.n.5\", \"@2.n.6\", \"@2.n.7\", \"@2.n.8\", \"@2.n.9\", \"@3.d.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Binary(BitwiseAnd Var(\"g\") Constant(ConstUChar(63)) Var(\"%7.1.n0\"))\n"
      "  ZeroExtend(Var(\"%7.1.n0\") Var(\"%7.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%7.1\") Constant(ConstInt(6)) \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.n.3\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".main@ssa_block@1\" Var(\"%9.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "  Binary(BitwiseOr Var(\"g\") Constant(ConstUChar(16)) Var(\"%11.1.n0\"))\n"
      "  ZeroExtend(Var(\"%11.1.n0\") Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstInt(213)) \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.3\") Constant(ConstInt(2)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.n.5\") [PhiArg(\".if_end@0\" Var(\"@2.n.3\")) PhiArg(\".main@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseXor Var(\"g\") Constant(ConstUChar(15)) Var(\"%15.1.n0\"))\n"
      "  ZeroExtend(Var(\"%15.1.n0\") Var(\"%15.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%15.1\") Constant(ConstInt(201)) \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.5\") Constant(ConstInt(4)) Var(\"%17.1\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.n.7\") [PhiArg(\".if_end@1\" Var(\"@2.n.5\")) PhiArg(\".main@ssa_block@3\" Var(\"%17.1\"))])\n"
      "  SignExtend(Var(\"sg\") Var(\"%18.1\"))\n"
      "  Binary(BitwiseAnd Var(\"sg\") Constant(ConstUChar(240)) Var(\"%19.1.n0\"))\n"
      "  ZeroExtend(Var(\"%19.1.n0\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%19.1\") Constant(ConstInt(240)) \".if_end@3\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.7\") Constant(ConstInt(8)) Var(\"%21.1\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Phi(Var(\"@2.n.9\") [PhiArg(\".if_end@2\" Var(\"@2.n.7\")) PhiArg(\".main@ssa_block@4\" Var(\"%21.1\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%22.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%22.1\") Constant(ConstUChar(43))] Var(\"%24.1\") false false)\n"
      "  JumpIfFalse(Var(\"%24.1\") \".if_end@4\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.9\") Constant(ConstInt(16)) Var(\"%25.1\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@2.n.11\") [PhiArg(\".if_end@3\" Var(\"@2.n.9\")) PhiArg(\".main@ssa_block@5\" Var(\"%25.1\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%26.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%26.1\") Constant(ConstUChar(42))] Var(\"%28.1\") false false)\n"
      "  JumpIfFalse(Var(\"%28.1\") \".if_end@5\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.11\") Constant(ConstInt(32)) Var(\"%29.1\"))\n"
      "  Label(\".if_end@5\")\n"
      "  Phi(Var(\"@2.n.13\") [PhiArg(\".if_end@4\" Var(\"@2.n.11\")) PhiArg(\".main@ssa_block@6\" Var(\"%29.1\"))])\n"
      "  Binary(Subtract Var(\"g\") Constant(ConstChar(-58)) Var(\"%32.1\"))\n"
      "  SignExtend(Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%32.1\") Constant(ConstChar(0)) \".if_end@6\")\n"
      "  Label(\".main@ssa_block@7\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.13\") Constant(ConstInt(64)) Var(\"%35.1\"))\n"
      "  Label(\".if_end@6\")\n"
      "  Phi(Var(\"@2.n.15\") [PhiArg(\".if_end@5\" Var(\"@2.n.13\")) PhiArg(\".main@ssa_block@7\" Var(\"%35.1\"))])\n"
      "  JumpIfCmp(LessOrEqual Var(\"%33.1\") Constant(ConstInt(-2)) \".if_end@7\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.15\") Constant(ConstInt(128)) Var(\"%39.1\"))\n"
      "  Label(\".if_end@7\")\n"
      "  Phi(Var(\"@2.n.17\") [PhiArg(\".if_end@6\" Var(\"@2.n.15\")) PhiArg(\".main@ssa_block@8\" Var(\"%39.1\"))])\n"
      "  Ret(Var(\"@2.n.17\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Binary(BitwiseAnd Var(\"g\") Constant(ConstUChar(63)) Var(\"%7.1.n0\"))\n"
      "  ZeroExtend(Var(\"%7.1.n0\") Var(\"%7.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%7.1.n0\") Constant(ConstUChar(6)) \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.n.3\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".main@ssa_block@1\" Var(\"%9.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "  Binary(BitwiseOr Var(\"g\") Constant(ConstUChar(16)) Var(\"%11.1.n0\"))\n"
      "  ZeroExtend(Var(\"%11.1.n0\") Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1.n0\") Constant(ConstUChar(213)) \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.3\") Constant(ConstInt(2)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.n.5\") [PhiArg(\".if_end@0\" Var(\"@2.n.3\")) PhiArg(\".main@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseXor Var(\"g\") Constant(ConstUChar(15)) Var(\"%15.1.n0\"))\n"
      "  ZeroExtend(Var(\"%15.1.n0\") Var(\"%15.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%15.1.n0\") Constant(ConstUChar(201)) \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.5\") Constant(ConstInt(4)) Var(\"%17.1\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.n.7\") [PhiArg(\".if_end@1\" Var(\"@2.n.5\")) PhiArg(\".main@ssa_block@3\" Var(\"%17.1\"))])\n"
      "  SignExtend(Var(\"sg\") Var(\"%18.1\"))\n"
      "  Binary(BitwiseAnd Var(\"sg\") Constant(ConstUChar(240)) Var(\"%19.1.n0\"))\n"
      "  ZeroExtend(Var(\"%19.1.n0\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%19.1.n0\") Constant(ConstUChar(240)) \".if_end@3\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.7\") Constant(ConstInt(8)) Var(\"%21.1\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Phi(Var(\"@2.n.9\") [PhiArg(\".if_end@2\" Var(\"@2.n.7\")) PhiArg(\".main@ssa_block@4\" Var(\"%21.1\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%22.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%22.1\") Constant(ConstUChar(43))] Var(\"%24.1\") false false)\n"
      "  JumpIfFalse(Var(\"%24.1\") \".if_end@4\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.9\") Constant(ConstInt(16)) Var(\"%25.1\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@2.n.11\") [PhiArg(\".if_end@3\" Var(\"@2.n.9\")) PhiArg(\".main@ssa_block@5\" Var(\"%25.1\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%26.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%26.1\") Constant(ConstUChar(42))] Var(\"%28.1\") false false)\n"
      "  JumpIfFalse(Var(\"%28.1\") \".if_end@5\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.11\") Constant(ConstInt(32)) Var(\"%29.1\"))\n"
      "  Label(\".if_end@5\")\n"
      "  Phi(Var(\"@2.n.13\") [PhiArg(\".if_end@4\" Var(\"@2.n.11\")) PhiArg(\".main@ssa_block@6\" Var(\"%29.1\"))])\n"
      "  Binary(Subtract Var(\"g\") Constant(ConstChar(-58)) Var(\"%32.1\"))\n"
      "  SignExtend(Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%32.1\") Constant(ConstChar(0)) \".if_end@6\")\n"
      "  Label(\".main@ssa_block@7\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.13\") Constant(ConstInt(64)) Var(\"%35.1\"))\n"
      "  Label(\".if_end@6\")\n"
      "  Phi(Var(\"@2.n.15\") [PhiArg(\".if_end@5\" Var(\"@2.n.13\")) PhiArg(\".main@ssa_block@7\" Var(\"%35.1\"))])\n"
      "  JumpIfCmp(LessOrEqual Var(\"%33.1\") Constant(ConstInt(-2)) \".if_end@7\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.15\") Constant(ConstInt(128)) Var(\"%39.1\"))\n"
      "  Label(\".if_end@7\")\n"
      "  Phi(Var(\"@2.n.17\") [PhiArg(\".if_end@6\" Var(\"@2.n.15\")) PhiArg(\".main@ssa_block@8\" Var(\"%39.1\"))])\n"
      "  Ret(Var(\"@2.n.17\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_narrow_byte_bitwise.TestEndToEnd.test_byte_compares (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.lim\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@0.q\", \"@1.lim\"]}}}",
      "Function(\"below\" true [\"@0.q\" \"@1.lim\"] [\n"
      "  Label(\".below@ssa_block@0\")\n"
      "  Binary(Add Var(\"@0.q\") Constant(ConstInt(6)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%2.1\") Constant(ConstInt(63)) Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"@1.lim\") Var(\"%4.1\"))\n"
      "  Binary(LessThan Var(\"%3.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Ret(Var(\"%5.1\"))\n"
      "])\n",
      "Function(\"below\" true [\"@0.q\" \"@1.lim\"] [\n"
      "  Label(\".below@ssa_block@0\")\n"
      "  Binary(Add Var(\"@0.q\") Constant(ConstInt(6)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%2.1\") Constant(ConstInt(63)) Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"@1.lim\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"%3.1.u0\"))\n"
      "  Binary(LessThan Var(\"%3.1.u0\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Ret(Var(\"%5.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.lim\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.u0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%3.1.u0\", \"%4.1\", \"%5.1\", \"@0.q\", \"@1.lim\"]}}}");
}

TEST_CASE("tac_fold_producer_jumps: test_narrow_byte_bitwise.TestEndToEnd.test_byte_compares (3)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.lim\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%3.1.n0\", \"%3.1.u0\", \"%4.1\", \"%5.1\", \"@0.q\", \"@1.lim\"]}}}",
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
      "Function(\"below\" true [\"@0.q\" \"@1.lim\"] [\n"
      "  Binary(Add Var(\"@0.q\") Constant(ConstInt(6)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%1.1\") Constant(ConstUChar(63)) Var(\"%3.1.n0\"))\n"
      "  ZeroExtend(Var(\"%3.1.n0\") Var(\"%3.1\"))\n"
      "  ZeroExtend(Var(\"@1.lim\") Var(\"%4.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"%3.1.u0\"))\n"
      "  Binary(LessThan Var(\"%3.1.n0\") Var(\"@1.lim\") Var(\"%5.1\"))\n"
      "  Ret(Var(\"%5.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_narrow_byte_bitwise.TestEndToEnd.test_byte_compares (4)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@3.d.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@2.n.1\", \"@2.n.10\", \"@2.n.11\", \"@2.n.12\", \"@2.n.13\", \"@2.n.14\", \"@2.n.15\", \"@2.n.16\", \"@2.n.17\", \"@2.n.2\", \"@2.n.3\", \"@2.n.4\", \"@2.n.5\", \"@2.n.6\", \"@2.n.7\", \"@2.n.8\", \"@2.n.9\", \"@3.d.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@2.n.1\"))\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%6.1\") Constant(ConstInt(63)) Var(\"%7.1\"))\n"
      "  Binary(LessThan Var(\"%7.1\") Constant(ConstInt(6)) Var(\"%8.1\"))\n"
      "  JumpIfFalse(Var(\"%8.1\") \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.1\") Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"@2.n.2\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.n.3\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.n.1\")) PhiArg(\".main@ssa_block@1\" Var(\"@2.n.2\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "  Binary(BitwiseOr Var(\"%10.1\") Constant(ConstInt(16)) Var(\"%11.1\"))\n"
      "  Binary(Equal Var(\"%11.1\") Constant(ConstInt(213)) Var(\"%12.1\"))\n"
      "  JumpIfFalse(Var(\"%12.1\") \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.3\") Constant(ConstInt(2)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@2.n.4\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.n.5\") [PhiArg(\".if_end@0\" Var(\"@2.n.3\")) PhiArg(\".main@ssa_block@2\" Var(\"@2.n.4\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseXor Var(\"%14.1\") Constant(ConstInt(15)) Var(\"%15.1\"))\n"
      "  Binary(GreaterThan Var(\"%15.1\") Constant(ConstInt(201)) Var(\"%16.1\"))\n"
      "  JumpIfFalse(Var(\"%16.1\") \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.5\") Constant(ConstInt(4)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"@2.n.6\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.n.7\") [PhiArg(\".if_end@1\" Var(\"@2.n.5\")) PhiArg(\".main@ssa_block@3\" Var(\"@2.n.6\"))])\n"
      "  SignExtend(Var(\"sg\") Var(\"%18.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%18.1\") Constant(ConstInt(240)) Var(\"%19.1\"))\n"
      "  Binary(Equal Var(\"%19.1\") Constant(ConstInt(240)) Var(\"%20.1\"))\n"
      "  JumpIfFalse(Var(\"%20.1\") \".if_end@3\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.7\") Constant(ConstInt(8)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@2.n.8\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Phi(Var(\"@2.n.9\") [PhiArg(\".if_end@2\" Var(\"@2.n.7\")) PhiArg(\".main@ssa_block@4\" Var(\"@2.n.8\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%22.1\"))\n"
      "  Copy(Constant(ConstUChar(43)) Var(\"%23.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%22.1\") Var(\"%23.1\")] Var(\"%24.1\") false false)\n"
      "  JumpIfFalse(Var(\"%24.1\") \".if_end@4\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.9\") Constant(ConstInt(16)) Var(\"%25.1\"))\n"
      "  Copy(Var(\"%25.1\") Var(\"@2.n.10\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@2.n.11\") [PhiArg(\".if_end@3\" Var(\"@2.n.9\")) PhiArg(\".main@ssa_block@5\" Var(\"@2.n.10\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%26.1\"))\n"
      "  Copy(Constant(ConstUChar(42)) Var(\"%27.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%26.1\") Var(\"%27.1\")] Var(\"%28.1\") false false)\n"
      "  JumpIfFalse(Var(\"%28.1\") \".if_end@5\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.11\") Constant(ConstInt(32)) Var(\"%29.1\"))\n"
      "  Copy(Var(\"%29.1\") Var(\"@2.n.12\"))\n"
      "  Label(\".if_end@5\")\n"
      "  Phi(Var(\"@2.n.13\") [PhiArg(\".if_end@4\" Var(\"@2.n.11\")) PhiArg(\".main@ssa_block@6\" Var(\"@2.n.12\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%30.1\"))\n"
      "  Binary(Subtract Var(\"%30.1\") Constant(ConstInt(198)) Var(\"%31.1\"))\n"
      "  Truncate(Var(\"%31.1\") Var(\"%32.1\"))\n"
      "  Copy(Var(\"%32.1\") Var(\"@3.d.1\"))\n"
      "  SignExtend(Var(\"@3.d.1\") Var(\"%33.1\"))\n"
      "  Binary(GreaterThan Var(\"%33.1\") Constant(ConstInt(0)) Var(\"%34.1\"))\n"
      "  JumpIfFalse(Var(\"%34.1\") \".if_end@6\")\n"
      "  Label(\".main@ssa_block@7\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.13\") Constant(ConstInt(64)) Var(\"%35.1\"))\n"
      "  Copy(Var(\"%35.1\") Var(\"@2.n.14\"))\n"
      "  Label(\".if_end@6\")\n"
      "  Phi(Var(\"@2.n.15\") [PhiArg(\".if_end@5\" Var(\"@2.n.13\")) PhiArg(\".main@ssa_block@7\" Var(\"@2.n.14\"))])\n"
      "  SignExtend(Var(\"@3.d.1\") Var(\"%36.1\"))\n"
      "  Copy(Constant(ConstInt(-2)) Var(\"%37.1\"))\n"
      "  Binary(GreaterThan Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  JumpIfFalse(Var(\"%38.1\") \".if_end@7\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.15\") Constant(ConstInt(128)) Var(\"%39.1\"))\n"
      "  Copy(Var(\"%39.1\") Var(\"@2.n.16\"))\n"
      "  Label(\".if_end@7\")\n"
      "  Phi(Var(\"@2.n.17\") [PhiArg(\".if_end@6\" Var(\"@2.n.15\")) PhiArg(\".main@ssa_block@8\" Var(\"@2.n.16\"))])\n"
      "  Ret(Var(\"@2.n.17\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@2.n.1\"))\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%6.1\") Constant(ConstInt(63)) Var(\"%7.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%7.1\") Constant(ConstInt(6)) \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.1\") Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"@2.n.2\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.n.3\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.n.1\")) PhiArg(\".main@ssa_block@1\" Var(\"@2.n.2\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "  Binary(BitwiseOr Var(\"%10.1\") Constant(ConstInt(16)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstInt(213)) \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.3\") Constant(ConstInt(2)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@2.n.4\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.n.5\") [PhiArg(\".if_end@0\" Var(\"@2.n.3\")) PhiArg(\".main@ssa_block@2\" Var(\"@2.n.4\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseXor Var(\"%14.1\") Constant(ConstInt(15)) Var(\"%15.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%15.1\") Constant(ConstInt(201)) \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.5\") Constant(ConstInt(4)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"@2.n.6\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.n.7\") [PhiArg(\".if_end@1\" Var(\"@2.n.5\")) PhiArg(\".main@ssa_block@3\" Var(\"@2.n.6\"))])\n"
      "  SignExtend(Var(\"sg\") Var(\"%18.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%18.1\") Constant(ConstInt(240)) Var(\"%19.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%19.1\") Constant(ConstInt(240)) \".if_end@3\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.7\") Constant(ConstInt(8)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@2.n.8\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Phi(Var(\"@2.n.9\") [PhiArg(\".if_end@2\" Var(\"@2.n.7\")) PhiArg(\".main@ssa_block@4\" Var(\"@2.n.8\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%22.1\"))\n"
      "  Copy(Constant(ConstUChar(43)) Var(\"%23.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%22.1\") Var(\"%23.1\")] Var(\"%24.1\") false false)\n"
      "  JumpIfFalse(Var(\"%24.1\") \".if_end@4\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.9\") Constant(ConstInt(16)) Var(\"%25.1\"))\n"
      "  Copy(Var(\"%25.1\") Var(\"@2.n.10\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@2.n.11\") [PhiArg(\".if_end@3\" Var(\"@2.n.9\")) PhiArg(\".main@ssa_block@5\" Var(\"@2.n.10\"))])\n"
      "  GetAddress(Var(\"p\") Var(\"%26.1\"))\n"
      "  Copy(Constant(ConstUChar(42)) Var(\"%27.1\"))\n"
      "  FunctionCall(\"below\" [Var(\"%26.1\") Var(\"%27.1\")] Var(\"%28.1\") false false)\n"
      "  JumpIfFalse(Var(\"%28.1\") \".if_end@5\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.11\") Constant(ConstInt(32)) Var(\"%29.1\"))\n"
      "  Copy(Var(\"%29.1\") Var(\"@2.n.12\"))\n"
      "  Label(\".if_end@5\")\n"
      "  Phi(Var(\"@2.n.13\") [PhiArg(\".if_end@4\" Var(\"@2.n.11\")) PhiArg(\".main@ssa_block@6\" Var(\"@2.n.12\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%30.1\"))\n"
      "  Binary(Subtract Var(\"%30.1\") Constant(ConstInt(198)) Var(\"%31.1\"))\n"
      "  Truncate(Var(\"%31.1\") Var(\"%32.1\"))\n"
      "  Copy(Var(\"%32.1\") Var(\"@3.d.1\"))\n"
      "  SignExtend(Var(\"@3.d.1\") Var(\"%33.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"@3.d.1\") Constant(ConstChar(0)) \".if_end@6\")\n"
      "  Label(\".main@ssa_block@7\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.13\") Constant(ConstInt(64)) Var(\"%35.1\"))\n"
      "  Copy(Var(\"%35.1\") Var(\"@2.n.14\"))\n"
      "  Label(\".if_end@6\")\n"
      "  Phi(Var(\"@2.n.15\") [PhiArg(\".if_end@5\" Var(\"@2.n.13\")) PhiArg(\".main@ssa_block@7\" Var(\"@2.n.14\"))])\n"
      "  SignExtend(Var(\"@3.d.1\") Var(\"%36.1\"))\n"
      "  Copy(Constant(ConstInt(-2)) Var(\"%37.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%36.1\") Var(\"%37.1\") \".if_end@7\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.15\") Constant(ConstInt(128)) Var(\"%39.1\"))\n"
      "  Copy(Var(\"%39.1\") Var(\"@2.n.16\"))\n"
      "  Label(\".if_end@7\")\n"
      "  Phi(Var(\"@2.n.17\") [PhiArg(\".if_end@6\" Var(\"@2.n.15\")) PhiArg(\".main@ssa_block@8\" Var(\"@2.n.16\"))])\n"
      "  Ret(Var(\"@2.n.17\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_narrow_widened_arith.TestGlobalShiftOnTwoPathsSim.test_one_byte_shifts (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"key\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"key\") Var(\"%0.1\"))\n"
      "  Binary(Equal Var(\"%0.1\") Constant(ConstInt(3)) Var(\"%1.1\"))\n"
      "  JumpIfFalse(Var(\"%1.1\") \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Var(\"other\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"other\") Constant(ConstUChar(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"other\"))\n"
      "  ZeroExtend(Var(\"alive\") Var(\"%4.1\"))\n"
      "  Binary(RightShift Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"alive\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@0\")\n"
      "  ZeroExtend(Var(\"key\") Var(\"%7.1\"))\n"
      "  Binary(Equal Var(\"%7.1\") Constant(ConstInt(4)) Var(\"%8.1\"))\n"
      "  JumpIfFalse(Var(\"%8.1\") \".if_end@1\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  ZeroExtend(Var(\"alive\") Var(\"%9.1\"))\n"
      "  Binary(RightShift Var(\"%9.1\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Copy(Var(\"%11.1\") Var(\"alive\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"key\") Var(\"%0.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"key\") Constant(ConstUChar(3)) \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Var(\"other\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"other\") Constant(ConstUChar(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"other\"))\n"
      "  ZeroExtend(Var(\"alive\") Var(\"%4.1\"))\n"
      "  Binary(RightShift Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"alive\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@0\")\n"
      "  ZeroExtend(Var(\"key\") Var(\"%7.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"key\") Constant(ConstUChar(4)) \".if_end@1\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  ZeroExtend(Var(\"alive\") Var(\"%9.1\"))\n"
      "  Binary(RightShift Var(\"%9.1\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Copy(Var(\"%11.1\") Var(\"alive\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_narrow_widened_arith.TestVariableLeftShift.test_runs (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@2.bad.1\", \"@2.bad.10\", \"@2.bad.11\", \"@2.bad.2\", \"@2.bad.3\", \"@2.bad.4\", \"@2.bad.5\", \"@2.bad.6\", \"@2.bad.7\", \"@2.bad.8\", \"@2.bad.9\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@2.bad.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%4.1\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%5.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%4.1\") Var(\"%5.1\")] Var(\"%6.1\") false false)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(NotEqual Var(\"%7.1\") Constant(ConstInt(12)) Var(\"%8.1\"))\n"
      "  JumpIfFalse(Var(\"%8.1\") \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.1\") Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"@2.bad.2\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.bad.3\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.bad.1\")) PhiArg(\".main@ssa_block@1\" Var(\"@2.bad.2\"))])\n"
      "  Copy(Constant(ConstUChar(129)) Var(\"%10.1\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%10.1\") Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Binary(NotEqual Var(\"%13.1\") Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  JumpIfFalse(Var(\"%14.1\") \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.3\") Constant(ConstInt(2)) Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@2.bad.4\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.bad.5\") [PhiArg(\".if_end@0\" Var(\"@2.bad.3\")) PhiArg(\".main@ssa_block@2\" Var(\"@2.bad.4\"))])\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%16.1\"))\n"
      "  Copy(Constant(ConstUChar(8)) Var(\"%17.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%16.1\") Var(\"%17.1\")] Var(\"%18.1\") false false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(NotEqual Var(\"%19.1\") Constant(ConstInt(0)) Var(\"%20.1\"))\n"
      "  JumpIfFalse(Var(\"%20.1\") \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.5\") Constant(ConstInt(4)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@2.bad.6\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.bad.7\") [PhiArg(\".if_end@1\" Var(\"@2.bad.5\")) PhiArg(\".main@ssa_block@3\" Var(\"@2.bad.6\"))])\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%22.1\"))\n"
      "  Copy(Constant(ConstUChar(12)) Var(\"%23.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%22.1\") Var(\"%23.1\")] Var(\"%24.1\") false false)\n"
      "  ZeroExtend(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Binary(NotEqual Var(\"%25.1\") Constant(ConstInt(0)) Var(\"%26.1\"))\n"
      "  JumpIfFalse(Var(\"%26.1\") \".if_end@3\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.7\") Constant(ConstInt(8)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@2.bad.8\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Phi(Var(\"@2.bad.9\") [PhiArg(\".if_end@2\" Var(\"@2.bad.7\")) PhiArg(\".main@ssa_block@4\" Var(\"@2.bad.8\"))])\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%28.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%28.1\") Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Binary(NotEqual Var(\"%31.1\") Constant(ConstInt(5)) Var(\"%32.1\"))\n"
      "  JumpIfFalse(Var(\"%32.1\") \".if_end@4\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.9\") Constant(ConstInt(16)) Var(\"%33.1\"))\n"
      "  Copy(Var(\"%33.1\") Var(\"@2.bad.10\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@2.bad.11\") [PhiArg(\".if_end@3\" Var(\"@2.bad.9\")) PhiArg(\".main@ssa_block@5\" Var(\"@2.bad.10\"))])\n"
      "  Ret(Var(\"@2.bad.11\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@2.bad.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%4.1\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%5.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%4.1\") Var(\"%5.1\")] Var(\"%6.1\") false false)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%6.1\") Constant(ConstUChar(12)) \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.1\") Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"@2.bad.2\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.bad.3\") [PhiArg(\".main@ssa_block@0\" Var(\"@2.bad.1\")) PhiArg(\".main@ssa_block@1\" Var(\"@2.bad.2\"))])\n"
      "  Copy(Constant(ConstUChar(129)) Var(\"%10.1\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%10.1\") Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%12.1\") Constant(ConstUChar(2)) \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.3\") Constant(ConstInt(2)) Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"@2.bad.4\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.bad.5\") [PhiArg(\".if_end@0\" Var(\"@2.bad.3\")) PhiArg(\".main@ssa_block@2\" Var(\"@2.bad.4\"))])\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%16.1\"))\n"
      "  Copy(Constant(ConstUChar(8)) Var(\"%17.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%16.1\") Var(\"%17.1\")] Var(\"%18.1\") false false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  JumpIfFalse(Var(\"%18.1\") \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.5\") Constant(ConstInt(4)) Var(\"%21.1\"))\n"
      "  Copy(Var(\"%21.1\") Var(\"@2.bad.6\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.bad.7\") [PhiArg(\".if_end@1\" Var(\"@2.bad.5\")) PhiArg(\".main@ssa_block@3\" Var(\"@2.bad.6\"))])\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%22.1\"))\n"
      "  Copy(Constant(ConstUChar(12)) Var(\"%23.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%22.1\") Var(\"%23.1\")] Var(\"%24.1\") false false)\n"
      "  ZeroExtend(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  JumpIfFalse(Var(\"%24.1\") \".if_end@3\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.7\") Constant(ConstInt(8)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@2.bad.8\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Phi(Var(\"@2.bad.9\") [PhiArg(\".if_end@2\" Var(\"@2.bad.7\")) PhiArg(\".main@ssa_block@4\" Var(\"@2.bad.8\"))])\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%28.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"s\" [Var(\"%28.1\") Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%30.1\") Constant(ConstUChar(5)) \".if_end@4\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Binary(BitwiseOr Var(\"@2.bad.9\") Constant(ConstInt(16)) Var(\"%33.1\"))\n"
      "  Copy(Var(\"%33.1\") Var(\"@2.bad.10\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@2.bad.11\") [PhiArg(\".if_end@3\" Var(\"@2.bad.9\")) PhiArg(\".main@ssa_block@5\" Var(\"@2.bad.10\"))])\n"
      "  Ret(Var(\"@2.bad.11\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_truncate_extend_fold.TestByteLoopAgainstSizeof.test_one_byte_compare (1)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%3.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.i.1\", \"@0.i.2\", \"@0.i.3\", \"@1.i.1\", \"@1.i.2\", \"@1.i.3\", \"@2.p.1\", \"@3.i.1\", \"@3.i.2\", \"@3.i.3\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@0.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.i.3\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Binary(LessThan Var(\"%2.1\") Constant(ConstUInt(40)) Var(\"%3.1\"))\n"
      "  JumpIfFalse(Var(\"%3.1\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  IndexedLoad(\"b\" Var(\"%5.1\") Var(\"%6.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Constant(ConstInt(1)) Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%11.1\") Var(\"%9.1\") false 0)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@0.i.2\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@0.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%14.1\"))\n"
      "  Copy(Var(\"%14.1\") Var(\"@1.i.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@1.i.2\") [PhiArg(\".loop@0_break\" Var(\"@1.i.1\")) PhiArg(\".loop@1_continue\" Var(\"@1.i.3\"))])\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Binary(LessThan Var(\"%16.1\") Constant(ConstUInt(40)) Var(\"%17.1\"))\n"
      "  JumpIfFalse(Var(\"%17.1\") \".loop@1_break\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%18.1\"))\n"
      "  Truncate(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%19.1\") Var(\"%20.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%21.1\"))\n"
      "  Truncate(Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  IndexedSymbolStore(\"b\" Var(\"%22.1\") Var(\"%20.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%23.1\"))\n"
      "  Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"%24.1\"))\n"
      "  Copy(Var(\"%24.1\") Var(\"@1.i.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"a\") Var(\"%25.1\"))\n"
      "  Copy(Var(\"%25.1\") Var(\"@2.p.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%26.1\"))\n"
      "  Copy(Var(\"%26.1\") Var(\"@3.i.1\"))\n"
      "  Label(\".loop@2_start\")\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".loop@1_break\" Var(\"@3.i.1\")) PhiArg(\".loop@2_continue\" Var(\"@3.i.3\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"%28.1\"))\n"
      "  Binary(LessThan Var(\"%28.1\") Constant(ConstUInt(40)) Var(\"%29.1\"))\n"
      "  JumpIfFalse(Var(\"%29.1\") \".loop@2_break\")\n"
      "  Label(\".f@ssa_block@3\")\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"@2.p.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Load(Var(\"%31.1\") Var(\"%32.1\") false)\n"
      "  ZeroExtend(Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  Binary(Add Var(\"%33.1\") Constant(ConstInt(2)) Var(\"%34.1\"))\n"
      "  Truncate(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Store(Var(\"%35.1\") Var(\"%31.1\") false)\n"
      "  Label(\".loop@2_continue\")\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%36.1\"))\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%37.1\"))\n"
      "  Copy(Var(\"%37.1\") Var(\"@3.i.3\"))\n"
      "  Jump(\".loop@2_start\")\n"
      "  Label(\".loop@2_break\")\n"
      "  IndexedLoad(\"b\" Constant(ConstUChar(39)) Var(\"%38.1\") false 0 0)\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"@0.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@0.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.i.3\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(40)) \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  IndexedLoad(\"b\" Var(\"%5.1\") Var(\"%6.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Constant(ConstInt(1)) Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%11.1\") Var(\"%9.1\") false 0)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@0.i.2\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@0.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%14.1\"))\n"
      "  Copy(Var(\"%14.1\") Var(\"@1.i.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@1.i.2\") [PhiArg(\".loop@0_break\" Var(\"@1.i.1\")) PhiArg(\".loop@1_continue\" Var(\"@1.i.3\"))])\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Constant(ConstUChar(40)) \".loop@1_break\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%18.1\"))\n"
      "  Truncate(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%19.1\") Var(\"%20.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%21.1\"))\n"
      "  Truncate(Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  IndexedSymbolStore(\"b\" Var(\"%22.1\") Var(\"%20.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%23.1\"))\n"
      "  Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"%24.1\"))\n"
      "  Copy(Var(\"%24.1\") Var(\"@1.i.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"a\") Var(\"%25.1\"))\n"
      "  Copy(Var(\"%25.1\") Var(\"@2.p.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%26.1\"))\n"
      "  Copy(Var(\"%26.1\") Var(\"@3.i.1\"))\n"
      "  Label(\".loop@2_start\")\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".loop@1_break\" Var(\"@3.i.1\")) PhiArg(\".loop@2_continue\" Var(\"@3.i.3\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"%28.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.i.2\") Constant(ConstUChar(40)) \".loop@2_break\")\n"
      "  Label(\".f@ssa_block@3\")\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"@2.p.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Load(Var(\"%31.1\") Var(\"%32.1\") false)\n"
      "  ZeroExtend(Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  Binary(Add Var(\"%33.1\") Constant(ConstInt(2)) Var(\"%34.1\"))\n"
      "  Truncate(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Store(Var(\"%35.1\") Var(\"%31.1\") false)\n"
      "  Label(\".loop@2_continue\")\n"
      "  Copy(Var(\"@3.i.2\") Var(\"%36.1\"))\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%37.1\"))\n"
      "  Copy(Var(\"%37.1\") Var(\"@3.i.3\"))\n"
      "  Jump(\".loop@2_start\")\n"
      "  Label(\".loop@2_break\")\n"
      "  IndexedLoad(\"b\" Constant(ConstUChar(39)) Var(\"%38.1\") false 0 0)\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_producer_jumps: test_truncate_extend_fold.TestByteLoopAgainstSizeof.test_one_byte_compare (2)") {
  check_stage_case(
      "tac_fold_producer_jumps",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@4.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"@4.i.1\", \"@4.i.2\", \"@4.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%39.1\"))\n"
      "  Copy(Var(\"%39.1\") Var(\"@4.i.1\"))\n"
      "  Label(\".loop@3_start\")\n"
      "  Phi(Var(\"@4.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.i.1\")) PhiArg(\".loop@3_continue\" Var(\"@4.i.3\"))])\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%40.1\"))\n"
      "  Binary(LessThan Var(\"%40.1\") Constant(ConstInt(40)) Var(\"%41.1\"))\n"
      "  JumpIfFalse(Var(\"%41.1\") \".loop@3_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%42.1\"))\n"
      "  Truncate(Var(\"%42.1\") Var(\"%43.1\"))\n"
      "  IndexedSymbolStore(\"b\" Var(\"%43.1\") Var(\"@4.i.2\") false 0)\n"
      "  Label(\".loop@3_continue\")\n"
      "  Copy(Var(\"@4.i.2\") Var(\"%44.1\"))\n"
      "  Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%45.1\"))\n"
      "  Copy(Var(\"%45.1\") Var(\"@4.i.3\"))\n"
      "  Jump(\".loop@3_start\")\n"
      "  Label(\".loop@3_break\")\n"
      "  FunctionCall(\"f\" [] Var(\"%46.1\") false false)\n"
      "  ZeroExtend(Var(\"%46.1\") Var(\"%47.1\"))\n"
      "  Ret(Var(\"%47.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%39.1\"))\n"
      "  Copy(Var(\"%39.1\") Var(\"@4.i.1\"))\n"
      "  Label(\".loop@3_start\")\n"
      "  Phi(Var(\"@4.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.i.1\")) PhiArg(\".loop@3_continue\" Var(\"@4.i.3\"))])\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%40.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.i.2\") Constant(ConstUChar(40)) \".loop@3_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%42.1\"))\n"
      "  Truncate(Var(\"%42.1\") Var(\"%43.1\"))\n"
      "  IndexedSymbolStore(\"b\" Var(\"%43.1\") Var(\"@4.i.2\") false 0)\n"
      "  Label(\".loop@3_continue\")\n"
      "  Copy(Var(\"@4.i.2\") Var(\"%44.1\"))\n"
      "  Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%45.1\"))\n"
      "  Copy(Var(\"%45.1\") Var(\"@4.i.3\"))\n"
      "  Jump(\".loop@3_start\")\n"
      "  Label(\".loop@3_break\")\n"
      "  FunctionCall(\"f\" [] Var(\"%46.1\") false false)\n"
      "  ZeroExtend(Var(\"%46.1\") Var(\"%47.1\"))\n"
      "  Ret(Var(\"%47.1\"))\n"
      "])\n",
      "");
}
