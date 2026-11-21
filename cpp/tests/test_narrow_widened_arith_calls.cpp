// The calls of `tac_narrow_widened_arith` that the Python unit tests
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

TEST_CASE("tac_narrow_widened_arith: test_and_zero_jump_fold.TestWideMaskInOneByte.test_byte_tested (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%11.1\", \"%12.1\", \"%8.1\", \"%9.1\"]}}}",
      "Function(\"neg\" true [] [\n"
      "  Copy(Var(\"s\") Var(\"%8.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%8.1\") Constant(ConstUInt(32768)) Var(\"%9.1\"))\n"
      "  Binary(NotEqual Var(\"%9.1\") Constant(ConstUInt(0)) Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "Function(\"neg\" true [] [\n"
      "  Copy(Var(\"s\") Var(\"%8.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%8.1\") Constant(ConstUInt(32768)) Var(\"%9.1\"))\n"
      "  Binary(NotEqual Var(\"%9.1\") Constant(ConstUInt(0)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_and_zero_jump_fold.TestWideMaskInOneByte.test_byte_tested (2)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\"]}}}",
      "Function(\"top\" true [] [\n"
      "  Binary(BitwiseAnd Var(\"l\") Constant(ConstULong(8388608)) Var(\"%13.1\"))\n"
      "  Binary(NotEqual Var(\"%13.1\") Constant(ConstULong(0)) Var(\"%15.1\"))\n"
      "  Truncate(Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Ret(Var(\"%16.1\"))\n"
      "])\n",
      "Function(\"top\" true [] [\n"
      "  Binary(BitwiseAnd Var(\"l\") Constant(ConstULong(8388608)) Var(\"%13.1\"))\n"
      "  Binary(NotEqual Var(\"%13.1\") Constant(ConstULong(0)) Var(\"%16.1\"))\n"
      "  Ret(Var(\"%16.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_and_zero_jump_fold.TestWideMaskInOneByte.test_byte_tested (3)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%4.1\", \"%6.1\", \"%7.1\"]}}}",
      "Function(\"mid\" true [] [\n"
      "  Binary(BitwiseAnd Var(\"w\") Constant(ConstUInt(1024)) Var(\"%4.1\"))\n"
      "  Binary(NotEqual Var(\"%4.1\") Constant(ConstUInt(0)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "Function(\"mid\" true [] [\n"
      "  Binary(BitwiseAnd Var(\"w\") Constant(ConstUInt(1024)) Var(\"%4.1\"))\n"
      "  Binary(NotEqual Var(\"%4.1\") Constant(ConstUInt(0)) Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_bool.TestPrograms.test_asm_sim (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.x\"]}}}",
      "Function(\"nz\" true [\"@0.x\"] [\n"
      "  Binary(NotEqual Var(\"@0.x\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"nz\" true [\"@0.x\"] [\n"
      "  Binary(NotEqual Var(\"@0.x\") Constant(ConstInt(0)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_bool.TestPrograms.test_asm_sim (2)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.x\"]}}}",
      "Function(\"odd\" true [\"@0.x\"] [\n"
      "  Binary(BitwiseAnd Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"odd\" true [\"@0.x\"] [\n"
      "  Truncate(Var(\"@0.x\") Var(\"%0.1.lo1\"))\n"
      "  Binary(BitwiseAnd Var(\"%0.1.lo1\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%0.1.lo1\", \"%1.1\", \"@0.x\"]}}}");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestEndToEnd.test_static_cond_kept (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"neg\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": true}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@3.n.1\", \"@3.n.2\", \"@3.n.3\", \"@4.a.1\", \"@4.a.2\", \"@4.a.3\", \"@5.b.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(BitwiseXor Constant(ConstUInt(7)) Constant(ConstUInt(65534)) Var(\"%11.1\"))\n"
      "  Binary(LessThan Var(\"%11.1\") Constant(ConstInt(0)) Var(\"%12.1\"))\n"
      "  Truncate(Var(\"%12.1\") Var(\"neg\"))\n"
      "  JumpIfFalse(Var(\"neg\") \".i1_if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Unary(Negate Constant(ConstUInt(7)) Var(\"%14.1\"))\n"
      "  Label(\".i1_if_end@0\")\n"
      "  Phi(Var(\"@4.a.3\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(7))) PhiArg(\".main@ssa_block@1\" Var(\"%14.1\"))])\n"
      "  JumpIfFalse(Var(\"neg\") \".i0_if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Unary(Negate Var(\"@4.a.3\") Var(\"%9.1\"))\n"
      "  Label(\".i0_if_end@1\")\n"
      "  Phi(Var(\"@3.n.3\") [PhiArg(\".i1_if_end@0\" Var(\"@4.a.3\")) PhiArg(\".main@ssa_block@2\" Var(\"%9.1\"))])\n"
      "  Copy(Var(\"@3.n.3\") Var(\"r\"))\n"
      "  Copy(Var(\"r\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(BitwiseXor Constant(ConstUInt(7)) Constant(ConstUInt(65534)) Var(\"%11.1\"))\n"
      "  Binary(LessThan Var(\"%11.1\") Constant(ConstInt(0)) Var(\"neg\"))\n"
      "  JumpIfFalse(Var(\"neg\") \".i1_if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Unary(Negate Constant(ConstUInt(7)) Var(\"%14.1\"))\n"
      "  Label(\".i1_if_end@0\")\n"
      "  Phi(Var(\"@4.a.3\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(7))) PhiArg(\".main@ssa_block@1\" Var(\"%14.1\"))])\n"
      "  JumpIfFalse(Var(\"neg\") \".i0_if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Unary(Negate Var(\"@4.a.3\") Var(\"%9.1\"))\n"
      "  Label(\".i0_if_end@1\")\n"
      "  Phi(Var(\"@3.n.3\") [PhiArg(\".i1_if_end@0\" Var(\"@4.a.3\")) PhiArg(\".main@ssa_block@2\" Var(\"%9.1\"))])\n"
      "  Copy(Var(\"@3.n.3\") Var(\"r\"))\n"
      "  Copy(Var(\"r\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestNarrowOrderingAsmShape.test_uchar_loop_returns_correct_count (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.i.1\", \"@0.i.2\", \"@0.i.3\", \"@1.sum.1\", \"@1.sum.2\", \"@1.sum.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%3.1\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%6.1\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(105)) \".loop@0_break\")\n"
      "  Binary(Add Var(\"@1.sum.2\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%3.1\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%6.1\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(105)) \".loop@0_break\")\n"
      "  Binary(Add Var(\"@1.sum.2\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestNarrowOrderingAsmShape.test_uchar_lt_const_uses_8bit_cmp_bcs (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@0.i.1\", \"@0.i.2\", \"@0.i.3\", \"@1.sum.1\", \"@1.sum.2\", \"@1.sum.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%4.1\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%7.1\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(105)) \".loop@0_break\")\n"
      "  Binary(Add Var(\"@1.sum.2\") Var(\"%1.1\") Var(\"%4.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%4.1\"))])\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%7.1\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(105)) \".loop@0_break\")\n"
      "  Binary(Add Var(\"@1.sum.2\") Var(\"%1.1\") Var(\"%4.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@1.sum.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@1.t.1\"]}}}",
      "Function(\"eq0\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfTrue(Var(\"%6.1\") \".if_end@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%9.1\"))\n"
      "  Binary(BitwiseOr Var(\"%9.1\") Constant(ConstInt(2)) Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"eq0\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfTrue(Var(\"%6.1\") \".if_end@1\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(2)) Var(\"hits\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (2)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"@2.t.1\", \"@3.c.1\"]}}}",
      "Function(\"lt_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%12.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  Binary(LessThan Var(\"%12.1\") Constant(ConstUInt(3)) Var(\"%14.1\"))\n"
      "  JumpIfFalse(Var(\"%14.1\") \".if_end@2\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%15.1\"))\n"
      "  Binary(BitwiseOr Var(\"%15.1\") Constant(ConstInt(4)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"lt_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%12.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  Binary(LessThan Var(\"%12.1\") Constant(ConstUInt(3)) Var(\"%14.1\"))\n"
      "  JumpIfFalse(Var(\"%14.1\") \".if_end@2\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(4)) Var(\"hits\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (3)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%22.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"@4.t.1\", \"@5.c.1\"]}}}",
      "Function(\"eq0_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%18.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  Binary(Equal Var(\"%18.1\") Constant(ConstInt(0)) Var(\"%20.1\"))\n"
      "  JumpIfFalse(Var(\"%20.1\") \".if_end@3\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%21.1\"))\n"
      "  Binary(BitwiseOr Var(\"%21.1\") Constant(ConstInt(8)) Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"eq0_c\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%18.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  Binary(Equal Var(\"%18.1\") Constant(ConstInt(0)) Var(\"%20.1\"))\n"
      "  JumpIfFalse(Var(\"%20.1\") \".if_end@3\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(8)) Var(\"hits\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (4)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"@6.t.1\"]}}}",
      "Function(\"ge0\" true [] [\n"
      "  SignExtend(Var(\"sg\") Var(\"%24.1\"))\n"
      "  Truncate(Constant(ConstInt(-5)) Var(\"sg\"))\n"
      "  JumpIfCmp(LessThan Var(\"%24.1\") Constant(ConstInt(0)) \".if_end@4\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%28.1\"))\n"
      "  Binary(BitwiseOr Var(\"%28.1\") Constant(ConstInt(16)) Var(\"%29.1\"))\n"
      "  Truncate(Var(\"%29.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"ge0\" true [] [\n"
      "  SignExtend(Var(\"sg\") Var(\"%24.1\"))\n"
      "  Truncate(Constant(ConstInt(-5)) Var(\"sg\"))\n"
      "  JumpIfCmp(LessThan Var(\"%24.1\") Constant(ConstInt(0)) \".if_end@4\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(16)) Var(\"hits\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestStaleExtendSource.test_store_between_extend_and_compare (5)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"@0.t.1\"]}}}",
      "Function(\"lt\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%0.1\") Constant(ConstInt(3)) \".if_end@0\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseOr Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"lt\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"%0.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"g\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%0.1\") Constant(ConstInt(3)) \".if_end@0\")\n"
      "  Binary(BitwiseOr Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_one_byte_compare (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(GreaterOrEqual Var(\"@0.x\") Constant(ConstUChar(200)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(GreaterOrEqual Var(\"@0.x\") Constant(ConstUChar(200)) Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (2)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(GreaterThan Constant(ConstUChar(255)) Var(\"@0.x\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(GreaterThan Constant(ConstUChar(255)) Var(\"@0.x\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (3)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(GreaterThan Var(\"@0.x\") Constant(ConstUChar(127)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(GreaterThan Var(\"@0.x\") Constant(ConstUChar(127)) Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (4)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(LessOrEqual Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(LessOrEqual Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (5)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(LessThan Constant(ConstUChar(10)) Var(\"@0.x\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  Binary(LessThan Constant(ConstUChar(10)) Var(\"@0.x\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (6)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Var(\"%0.1\") Constant(ConstUInt(256)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(LessThan Var(\"%0.1\") Constant(ConstUInt(256)) Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_cmp_zero_jump_fold.TestValueCompareNarrowing.test_runs (7)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.x\"]}}}",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterThan Var(\"%0.1\") Constant(ConstInt(-1)) Var(\"%2.1\"))\n"
      "  Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"test\" false [\"@0.x\"] [\n"
      "  ZeroExtend(Var(\"@0.x\") Var(\"%0.1\"))\n"
      "  Binary(GreaterThan Var(\"%0.1\") Constant(ConstInt(-1)) Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_flag_compares.TestCompiled.test_not_of_bool (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"B\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 23041, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%2.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%0.1.not0\", \"%1.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"%3.1\", \"%4.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(BitwiseXor Var(\"flag\") Constant(ConstUChar(1)) Var(\"%0.1.not0\"))\n"
      "  ZeroExtend(Var(\"%0.1.not0\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1.not0\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3\") [PhiArg(\".f@ssa_block@1\" Constant(ConstInt(3))) PhiArg(\".cond_else@0\" Constant(ConstInt(2)))])\n"
      "  Binary(Add Var(\"%2.3\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Truncate(Var(\"%3.1\") Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(BitwiseXor Var(\"flag\") Constant(ConstUChar(1)) Var(\"%0.1.not0\"))\n"
      "  ZeroExtend(Var(\"%0.1.not0\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1.not0\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3\") [PhiArg(\".f@ssa_block@1\" Constant(ConstInt(3))) PhiArg(\".cond_else@0\" Constant(ConstInt(2)))])\n"
      "  Truncate(Var(\"%2.3\") Var(\"%3.1.lo1\"))\n"
      "  Binary(Add Var(\"%3.1.lo1\") Constant(ConstUChar(1)) Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"B\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 23041, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%2.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%0.1.not0\", \"%1.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"%3.1\", \"%3.1.lo1\", \"%4.1\"]}}}");
}

TEST_CASE("tac_narrow_widened_arith: test_flag_compares.TestSimulated.test_both_pipelines (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%113.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%115.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%125.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%127.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%146.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%147.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%116.1.lo2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%128.1.lo2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%101.1\", \"%102.1\", \"%104.1\", \"%105.1\", \"%106.1\", \"%107.1\", \"%107.1.lo2\", \"%108.1\", \"%110.1\", \"%111.1\", \"%113.1\", \"%114.1\", \"%115.1\", \"%116.1\", \"%116.1.lo2\", \"%118.1\", \"%119.1\", \"%120.1\", \"%121.1\", \"%121.1.lo1\", \"%121.1.lo2\", \"%122.1\", \"%123.1\", \"%124.1\", \"%125.1\", \"%126.1\", \"%127.1\", \"%128.1\", \"%128.1.lo2\", \"%130.1\", \"%131.1\", \"%132.1\", \"%133.1\", \"%133.1.lo1\", \"%133.1.lo2\", \"%134.1\", \"%135.1\", \"%136.1\", \"%137.1\", \"%138.1\", \"%138.1.lo1\", \"%139.1\", \"%140.1\", \"%141.1\", \"%142.1\", \"%143.1\", \"%143.1.lo1\", \"%144.1\", \"%145.1\", \"%146.1\", \"%147.1\", \"%49.1\", \"%51.1\", \"%51.1.sr0\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%58.1\", \"%58.1.sr0\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%65.1\", \"%65.1.sr0\", \"%66.1\", \"%67.1\", \"%68.1\", \"%69.1\", \"%70.1\", \"%71.1\", \"%72.1\", \"%73.1\", \"%74.1\", \"%75.1\", \"%76.1\", \"%77.1\", \"%78.1\", \"%79.1\", \"%80.1\", \"%81.1\", \"%82.1\", \"%83.1\", \"%84.1\", \"%85.1\", \"%86.1\", \"%87.1\", \"%88.1\", \"%89.1\", \"%90.1\", \"%91.1\", \"%92.1\", \"%93.1\", \"%95.1\", \"%97.1\", \"%99.1\", \"@10.v.1\", \"@10.v.2\", \"@10.v.3\", \"@9.sum.1\", \"@9.sum.2\", \"@9.sum.3\", \"@9.sum.4\", \"@9.sum.5\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@9.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%70.1\"))])\n"
      "  Phi(Var(\"@10.v.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%72.1\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@10.v.2\") Constant(ConstUInt(256)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@9.sum.2\") Constant(ConstInt(1)) Var(\"%51.1.sr0\"))\n"
      "  Binary(Add Var(\"%51.1.sr0\") Var(\"@9.sum.2\") Var(\"%51.1\"))\n"
      "  Truncate(Var(\"@10.v.2\") Var(\"%52.1\"))\n"
      "  FunctionCall(\"bit7\" [Var(\"%52.1\")] Var(\"%53.1\") false false)\n"
      "  ZeroExtend(Var(\"%53.1\") Var(\"%55.1\"))\n"
      "  Binary(Add Var(\"%51.1\") Var(\"%55.1\") Var(\"%56.1\"))\n"
      "  Binary(LeftShift Var(\"%56.1\") Constant(ConstInt(1)) Var(\"%58.1.sr0\"))\n"
      "  Binary(Add Var(\"%58.1.sr0\") Var(\"%56.1\") Var(\"%58.1\"))\n"
      "  FunctionCall(\"one\" [Var(\"%52.1\")] Var(\"%60.1\") false false)\n"
      "  ZeroExtend(Var(\"%60.1\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%58.1\") Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  Binary(LeftShift Var(\"%63.1\") Constant(ConstInt(1)) Var(\"%65.1.sr0\"))\n"
      "  Binary(Add Var(\"%65.1.sr0\") Var(\"%63.1\") Var(\"%65.1\"))\n"
      "  Truncate(Var(\"@10.v.2\") Var(\"%66.1\"))\n"
      "  FunctionCall(\"pos\" [Var(\"%66.1\")] Var(\"%67.1\") false false)\n"
      "  ZeroExtend(Var(\"%67.1\") Var(\"%69.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@10.v.2\") Constant(ConstUInt(1)) Var(\"%72.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(0)) Constant(ConstUChar(0))] Var(\"%75.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(0)) Var(\"%75.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(0)) Constant(ConstUChar(1))] Var(\"%78.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(1)) Var(\"%78.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(0)) Constant(ConstUChar(5))] Var(\"%81.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(2)) Var(\"%81.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(3)) Constant(ConstUChar(255))] Var(\"%84.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(3)) Var(\"%84.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(254)) Constant(ConstUChar(255))] Var(\"%87.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(4)) Var(\"%87.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(255)) Constant(ConstUChar(255))] Var(\"%90.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(5)) Var(\"%90.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(250)) Constant(ConstUChar(2))] Var(\"%93.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(6)) Var(\"%93.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(0))] Var(\"%95.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(7)) Var(\"%95.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(1))] Var(\"%97.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(8)) Var(\"%97.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(2))] Var(\"%99.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(9)) Var(\"%99.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(256))] Var(\"%101.1\") false false)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(257))] Var(\"%104.1\") false false)\n"
      "  Binary(LeftShift Var(\"%104.1\") Constant(ConstUChar(4)) Var(\"%107.1.lo2\"))\n"
      "  Binary(Add Var(\"%101.1\") Var(\"%107.1.lo2\") Var(\"%108.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(10)) Var(\"%108.1\") false 0)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(0))] Var(\"%110.1\") false false)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(255))] Var(\"%113.1\") false false)\n"
      "  ZeroExtend(Var(\"%113.1\") Var(\"%114.1\"))\n"
      "  Binary(LeftShift Var(\"%114.1\") Constant(ConstInt(2)) Var(\"%115.1\"))\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(256))] Var(\"%118.1\") false false)\n"
      "  Truncate(Var(\"%115.1\") Var(\"%116.1.lo2\"))\n"
      "  Binary(Add Var(\"%110.1\") Var(\"%116.1.lo2\") Var(\"%121.1.lo1\"))\n"
      "  Binary(LeftShift Var(\"%118.1\") Constant(ConstUChar(4)) Var(\"%121.1.lo2\"))\n"
      "  Binary(Add Var(\"%121.1.lo1\") Var(\"%121.1.lo2\") Var(\"%122.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(11)) Var(\"%122.1\") false 0)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(32768))] Var(\"%123.1\") false false)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(65535))] Var(\"%125.1\") false false)\n"
      "  ZeroExtend(Var(\"%125.1\") Var(\"%126.1\"))\n"
      "  Binary(LeftShift Var(\"%126.1\") Constant(ConstInt(2)) Var(\"%127.1\"))\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(32767))] Var(\"%130.1\") false false)\n"
      "  Truncate(Var(\"%127.1\") Var(\"%128.1.lo2\"))\n"
      "  Binary(Add Var(\"%123.1\") Var(\"%128.1.lo2\") Var(\"%133.1.lo1\"))\n"
      "  Binary(LeftShift Var(\"%130.1\") Constant(ConstUChar(4)) Var(\"%133.1.lo2\"))\n"
      "  Binary(Add Var(\"%133.1.lo1\") Var(\"%133.1.lo2\") Var(\"%134.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(12)) Var(\"%134.1\") false 0)\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"flag\"))\n"
      "  FunctionCall(\"nots\" [] None false false)\n"
      "  Binary(LeftShift Var(\"A\") Constant(ConstUChar(4)) Var(\"%138.1.lo1\"))\n"
      "  Binary(Add Var(\"%138.1.lo1\") Var(\"B\") Var(\"%139.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(13)) Var(\"%139.1\") false 0)\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"flag\"))\n"
      "  FunctionCall(\"nots\" [] None false false)\n"
      "  Binary(LeftShift Var(\"A\") Constant(ConstUChar(4)) Var(\"%143.1.lo1\"))\n"
      "  Binary(Add Var(\"%143.1.lo1\") Var(\"B\") Var(\"%144.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(14)) Var(\"%144.1\") false 0)\n"
      "  Truncate(Var(\"@9.sum.2\") Var(\"%145.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(15)) Var(\"%145.1\") false 0)\n"
      "  Binary(RightShift Var(\"@9.sum.2\") Constant(ConstInt(8)) Var(\"%146.1\"))\n"
      "  Truncate(Var(\"%146.1\") Var(\"%147.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(16)) Var(\"%147.1\") false 0)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@9.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%70.1\"))])\n"
      "  Phi(Var(\"@10.v.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%72.1\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@10.v.2\") Constant(ConstUInt(256)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@9.sum.2\") Constant(ConstInt(1)) Var(\"%51.1.sr0\"))\n"
      "  Binary(Add Var(\"%51.1.sr0\") Var(\"@9.sum.2\") Var(\"%51.1\"))\n"
      "  Truncate(Var(\"@10.v.2\") Var(\"%52.1\"))\n"
      "  FunctionCall(\"bit7\" [Var(\"%52.1\")] Var(\"%53.1\") false false)\n"
      "  ZeroExtend(Var(\"%53.1\") Var(\"%55.1\"))\n"
      "  Binary(Add Var(\"%51.1\") Var(\"%55.1\") Var(\"%56.1\"))\n"
      "  Binary(LeftShift Var(\"%56.1\") Constant(ConstInt(1)) Var(\"%58.1.sr0\"))\n"
      "  Binary(Add Var(\"%58.1.sr0\") Var(\"%56.1\") Var(\"%58.1\"))\n"
      "  FunctionCall(\"one\" [Var(\"%52.1\")] Var(\"%60.1\") false false)\n"
      "  ZeroExtend(Var(\"%60.1\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%58.1\") Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  Binary(LeftShift Var(\"%63.1\") Constant(ConstInt(1)) Var(\"%65.1.sr0\"))\n"
      "  Binary(Add Var(\"%65.1.sr0\") Var(\"%63.1\") Var(\"%65.1\"))\n"
      "  Truncate(Var(\"@10.v.2\") Var(\"%66.1\"))\n"
      "  FunctionCall(\"pos\" [Var(\"%66.1\")] Var(\"%67.1\") false false)\n"
      "  ZeroExtend(Var(\"%67.1\") Var(\"%69.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@10.v.2\") Constant(ConstUInt(1)) Var(\"%72.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(0)) Constant(ConstUChar(0))] Var(\"%75.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(0)) Var(\"%75.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(0)) Constant(ConstUChar(1))] Var(\"%78.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(1)) Var(\"%78.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(0)) Constant(ConstUChar(5))] Var(\"%81.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(2)) Var(\"%81.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(3)) Constant(ConstUChar(255))] Var(\"%84.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(3)) Var(\"%84.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(254)) Constant(ConstUChar(255))] Var(\"%87.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(4)) Var(\"%87.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(255)) Constant(ConstUChar(255))] Var(\"%90.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(5)) Var(\"%90.1\") false 0)\n"
      "  FunctionCall(\"rows\" [Constant(ConstUChar(250)) Constant(ConstUChar(2))] Var(\"%93.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(6)) Var(\"%93.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(0))] Var(\"%95.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(7)) Var(\"%95.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(1))] Var(\"%97.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(8)) Var(\"%97.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(2))] Var(\"%99.1\") false false)\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(9)) Var(\"%99.1\") false 0)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(256))] Var(\"%101.1\") false false)\n"
      "  FunctionCall(\"pair\" [Constant(ConstUInt(257))] Var(\"%104.1\") false false)\n"
      "  Binary(LeftShift Var(\"%104.1\") Constant(ConstUChar(4)) Var(\"%107.1.lo2\"))\n"
      "  Binary(Add Var(\"%101.1\") Var(\"%107.1.lo2\") Var(\"%108.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(10)) Var(\"%108.1\") false 0)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(0))] Var(\"%110.1\") false false)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(255))] Var(\"%113.1\") false false)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(256))] Var(\"%118.1\") false false)\n"
      "  Binary(LeftShift Var(\"%113.1\") Constant(ConstUChar(2)) Var(\"%116.1.lo2\"))\n"
      "  Binary(Add Var(\"%110.1\") Var(\"%116.1.lo2\") Var(\"%121.1.lo1\"))\n"
      "  Binary(LeftShift Var(\"%118.1\") Constant(ConstUChar(4)) Var(\"%121.1.lo2\"))\n"
      "  Binary(Add Var(\"%121.1.lo1\") Var(\"%121.1.lo2\") Var(\"%122.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(11)) Var(\"%122.1\") false 0)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(32768))] Var(\"%123.1\") false false)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(65535))] Var(\"%125.1\") false false)\n"
      "  FunctionCall(\"hi\" [Constant(ConstUInt(32767))] Var(\"%130.1\") false false)\n"
      "  Binary(LeftShift Var(\"%125.1\") Constant(ConstUChar(2)) Var(\"%128.1.lo2\"))\n"
      "  Binary(Add Var(\"%123.1\") Var(\"%128.1.lo2\") Var(\"%133.1.lo1\"))\n"
      "  Binary(LeftShift Var(\"%130.1\") Constant(ConstUChar(4)) Var(\"%133.1.lo2\"))\n"
      "  Binary(Add Var(\"%133.1.lo1\") Var(\"%133.1.lo2\") Var(\"%134.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(12)) Var(\"%134.1\") false 0)\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"flag\"))\n"
      "  FunctionCall(\"nots\" [] None false false)\n"
      "  Binary(LeftShift Var(\"A\") Constant(ConstUChar(4)) Var(\"%138.1.lo1\"))\n"
      "  Binary(Add Var(\"%138.1.lo1\") Var(\"B\") Var(\"%139.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(13)) Var(\"%139.1\") false 0)\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"flag\"))\n"
      "  FunctionCall(\"nots\" [] None false false)\n"
      "  Binary(LeftShift Var(\"A\") Constant(ConstUChar(4)) Var(\"%143.1.lo1\"))\n"
      "  Binary(Add Var(\"%143.1.lo1\") Var(\"B\") Var(\"%144.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(14)) Var(\"%144.1\") false 0)\n"
      "  Truncate(Var(\"@9.sum.2\") Var(\"%145.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(15)) Var(\"%145.1\") false 0)\n"
      "  Binary(RightShift Var(\"@9.sum.2\") Constant(ConstInt(8)) Var(\"%146.1\"))\n"
      "  Truncate(Var(\"%146.1\") Var(\"%147.1\"))\n"
      "  IndexedSymbolStore(\"checks\" Constant(ConstUChar(16)) Var(\"%147.1\") false 0)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_flag_compares.TestSimulated.test_both_pipelines (2)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"B\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%43.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%41.1\", \"%41.1.not0\", \"%42.1\", \"%43.1\", \"%43.2\", \"%43.3\", \"%44.1\", \"%45.1\"]}}}",
      "Function(\"nots\" true [] [\n"
      "  Binary(BitwiseXor Var(\"flag\") Constant(ConstUChar(1)) Var(\"%41.1.not0\"))\n"
      "  ZeroExtend(Var(\"%41.1.not0\") Var(\"%41.1\"))\n"
      "  Copy(Var(\"%41.1.not0\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@11\")\n"
      "  Label(\".nots@ssa_block@1\")\n"
      "  Jump(\".cond_end@12\")\n"
      "  Label(\".cond_else@11\")\n"
      "  Label(\".cond_end@12\")\n"
      "  Phi(Var(\"%43.3\") [PhiArg(\".nots@ssa_block@1\" Constant(ConstInt(3))) PhiArg(\".cond_else@11\" Constant(ConstInt(2)))])\n"
      "  Binary(Add Var(\"%43.3\") Constant(ConstInt(1)) Var(\"%44.1\"))\n"
      "  Truncate(Var(\"%44.1\") Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"nots\" true [] [\n"
      "  Binary(BitwiseXor Var(\"flag\") Constant(ConstUChar(1)) Var(\"%41.1.not0\"))\n"
      "  ZeroExtend(Var(\"%41.1.not0\") Var(\"%41.1\"))\n"
      "  Copy(Var(\"%41.1.not0\") Var(\"A\"))\n"
      "  JumpIfFalse(Var(\"flag\") \".cond_else@11\")\n"
      "  Label(\".nots@ssa_block@1\")\n"
      "  Jump(\".cond_end@12\")\n"
      "  Label(\".cond_else@11\")\n"
      "  Label(\".cond_end@12\")\n"
      "  Phi(Var(\"%43.3\") [PhiArg(\".nots@ssa_block@1\" Constant(ConstInt(3))) PhiArg(\".cond_else@11\" Constant(ConstInt(2)))])\n"
      "  Truncate(Var(\"%43.3\") Var(\"%44.1.lo1\"))\n"
      "  Binary(Add Var(\"%44.1.lo1\") Constant(ConstUChar(1)) Var(\"B\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"B\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%43.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%41.1\", \"%41.1.not0\", \"%42.1\", \"%43.1\", \"%43.2\", \"%43.3\", \"%44.1\", \"%44.1.lo1\", \"%45.1\"]}}}");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_byte_bitwise.TestEndToEnd.test_byte_compares (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 197}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%31.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@2.n.1\", \"@2.n.10\", \"@2.n.11\", \"@2.n.12\", \"@2.n.13\", \"@2.n.14\", \"@2.n.15\", \"@2.n.16\", \"@2.n.17\", \"@2.n.2\", \"@2.n.3\", \"@2.n.4\", \"@2.n.5\", \"@2.n.6\", \"@2.n.7\", \"@2.n.8\", \"@2.n.9\", \"@3.d.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"%6.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%6.1\") Constant(ConstInt(63)) Var(\"%7.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%7.1\") Constant(ConstInt(6)) \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(BitwiseOr Constant(ConstInt(0)) Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.n.3\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".main@ssa_block@1\" Var(\"%9.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "  Binary(BitwiseOr Var(\"%10.1\") Constant(ConstInt(16)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstInt(213)) \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.3\") Constant(ConstInt(2)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.n.5\") [PhiArg(\".if_end@0\" Var(\"@2.n.3\")) PhiArg(\".main@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseXor Var(\"%14.1\") Constant(ConstInt(15)) Var(\"%15.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%15.1\") Constant(ConstInt(201)) \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.5\") Constant(ConstInt(4)) Var(\"%17.1\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.n.7\") [PhiArg(\".if_end@1\" Var(\"@2.n.5\")) PhiArg(\".main@ssa_block@3\" Var(\"%17.1\"))])\n"
      "  SignExtend(Var(\"sg\") Var(\"%18.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%18.1\") Constant(ConstInt(240)) Var(\"%19.1\"))\n"
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
      "  ZeroExtend(Var(\"g\") Var(\"%30.1\"))\n"
      "  Binary(Subtract Var(\"%30.1\") Constant(ConstInt(198)) Var(\"%31.1\"))\n"
      "  Truncate(Var(\"%31.1\") Var(\"%32.1\"))\n"
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
      "  Binary(BitwiseAnd Var(\"%6.1\") Constant(ConstInt(63)) Var(\"%7.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%7.1\") Constant(ConstInt(6)) \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(BitwiseOr Constant(ConstInt(0)) Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@2.n.3\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".main@ssa_block@1\" Var(\"%9.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "  Binary(BitwiseOr Var(\"%10.1\") Constant(ConstInt(16)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstInt(213)) \".if_end@1\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.3\") Constant(ConstInt(2)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@2.n.5\") [PhiArg(\".if_end@0\" Var(\"@2.n.3\")) PhiArg(\".main@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  ZeroExtend(Var(\"g\") Var(\"%14.1\"))\n"
      "  Binary(BitwiseXor Var(\"%14.1\") Constant(ConstInt(15)) Var(\"%15.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%15.1\") Constant(ConstInt(201)) \".if_end@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  Binary(BitwiseOr Var(\"@2.n.5\") Constant(ConstInt(4)) Var(\"%17.1\"))\n"
      "  Label(\".if_end@2\")\n"
      "  Phi(Var(\"@2.n.7\") [PhiArg(\".if_end@1\" Var(\"@2.n.5\")) PhiArg(\".main@ssa_block@3\" Var(\"%17.1\"))])\n"
      "  SignExtend(Var(\"sg\") Var(\"%18.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%18.1\") Constant(ConstInt(240)) Var(\"%19.1\"))\n"
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
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_byte_range.TestLowerings.test_shift_of_promoted_byte_is_logical (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1.lo0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.n0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.nb0\", \"%3.1\", \"%3.1.n0\", \"%4.1\", \"%5.1\", \"%6.1\", \"%6.1.lo0\", \"%6.1.nw0\", \"@0.p\", \"@1.s\"]}}}",
      "Function(\"f\" true [\"@0.p\" \"@1.s\"] [\n"
      "  ZeroExtend(Var(\"@0.p\") Var(\"%0.1\"))\n"
      "  Binary(RightShift Var(\"@1.s\") Constant(ConstInt(5)) Var(\"%2.1.nb0\"))\n"
      "  Binary(BitwiseAnd Var(\"%2.1.nb0\") Constant(ConstUChar(3)) Var(\"%3.1.n0\"))\n"
      "  ZeroExtend(Var(\"%3.1.n0\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%6.1.lo0\"))\n"
      "  ZeroExtend(Var(\"%6.1.lo0\") Var(\"%6.1.nw0\"))\n"
      "  Binary(Multiply Var(\"%0.1\") Var(\"%6.1.nw0\") Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.p\" \"@1.s\"] [\n"
      "  ZeroExtend(Var(\"@0.p\") Var(\"%0.1\"))\n"
      "  Binary(RightShift Var(\"@1.s\") Constant(ConstInt(5)) Var(\"%2.1.nb0\"))\n"
      "  Binary(BitwiseAnd Var(\"%2.1.nb0\") Constant(ConstUChar(3)) Var(\"%3.1.n0\"))\n"
      "  Binary(Add Var(\"%3.1.n0\") Constant(ConstUChar(1)) Var(\"%6.1.lo0\"))\n"
      "  ZeroExtend(Var(\"%6.1.lo0\") Var(\"%6.1.nw0\"))\n"
      "  Binary(Multiply Var(\"%0.1\") Var(\"%6.1.nw0\") Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestGlobalShiftOnTwoPathsSim.test_one_byte_shifts (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"alive\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 129}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(NotEqual Var(\"key\") Constant(ConstUChar(3)) \".if_end@0\")\n"
      "  Binary(Add Var(\"other\") Constant(ConstUChar(1)) Var(\"other\"))\n"
      "  ZeroExtend(Var(\"alive\") Var(\"%4.1\"))\n"
      "  Binary(RightShift Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"alive\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@0\")\n"
      "  JumpIfCmp(NotEqual Var(\"key\") Constant(ConstUChar(4)) \".if_end@1\")\n"
      "  ZeroExtend(Var(\"alive\") Var(\"%9.1\"))\n"
      "  Binary(RightShift Var(\"%9.1\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Truncate(Var(\"%10.1\") Var(\"alive\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(NotEqual Var(\"key\") Constant(ConstUChar(3)) \".if_end@0\")\n"
      "  Binary(Add Var(\"other\") Constant(ConstUChar(1)) Var(\"other\"))\n"
      "  Binary(RightShift Var(\"alive\") Constant(ConstUChar(1)) Var(\"alive\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@0\")\n"
      "  JumpIfCmp(NotEqual Var(\"key\") Constant(ConstUChar(4)) \".if_end@1\")\n"
      "  Binary(RightShift Var(\"alive\") Constant(ConstUChar(1)) Var(\"alive\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestGlobalWrittenOnTwoPaths.test_both_narrow (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"e1\", \"e2\", \"r1\", \"r2\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"e1\"))\n"
      "  Binary(RightShift Var(\"e1\") Constant(ConstInt(1)) Var(\"r1\"))\n"
      "  Truncate(Var(\"r1\") Var(\"g\"))\n"
      "  Ret(None)\n"
      "  Label(\".other\")\n"
      "  ZeroExtend(Var(\"g\") Var(\"e2\"))\n"
      "  Binary(RightShift Var(\"e2\") Constant(ConstInt(1)) Var(\"r2\"))\n"
      "  Truncate(Var(\"r2\") Var(\"g\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(RightShift Var(\"g\") Constant(ConstUChar(1)) Var(\"g\"))\n"
      "  Ret(None)\n"
      "  Label(\".other\")\n"
      "  Binary(RightShift Var(\"g\") Constant(ConstUChar(1)) Var(\"g\"))\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_add_two_zeroextended_uchars (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Add Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_binary_dst_is_global_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r_global\"))\n"
      "  Truncate(Var(\"r_global\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r_global\"))\n"
      "  Truncate(Var(\"r_global\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_bitwise_and_or_xor_narrow (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(BitwiseAnd Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(BitwiseAnd Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_bitwise_and_or_xor_narrow (standalone) (2)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(BitwiseOr Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(BitwiseOr Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_bitwise_and_or_xor_narrow (standalone) (3)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(BitwiseXor Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(BitwiseXor Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_compare_operand_changed_left_alone (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"t\") Constant(ConstInt(0)) Var(\"r\"))\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"t\") Constant(ConstInt(0)) Var(\"r\"))\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_compare_result_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"t\") Constant(ConstInt(0)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"t\") Constant(ConstInt(0)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_compare_with_other_reader_left_alone (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"t\") Constant(ConstInt(0)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "  Copy(Var(\"r\") Var(\"g\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(NotEqual Var(\"t\") Constant(ConstInt(0)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "  Copy(Var(\"r\") Var(\"g\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_divide_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Divide Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Divide Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_dst_width_equals_extend_width_left_alone (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_extend_dst_global_narrowed_when_unchanged (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ea_global\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea_global\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea_global\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea_global\"))\n"
      "  Truncate(Var(\"ea_global\") Var(\"r.lo1\"))\n"
      "  Binary(Subtract Var(\"r.lo1\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ea_global\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"eb\", \"r\", \"r.lo1\"]}}}");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_extend_of_global_changed_before_the_truncate (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"eg\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"eg\"))\n"
      "  Binary(Add Var(\"eg\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"g\") Var(\"eg\"))\n"
      "  Binary(Add Var(\"eg\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_extend_source_width_mismatch_truncates_the_extend (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ea\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"eb\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"a\") Var(\"ea\"))\n"
      "  SignExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Add Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"a\") Var(\"ea\"))\n"
      "  SignExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Truncate(Var(\"ea\") Var(\"r.lo1\"))\n"
      "  Truncate(Var(\"eb\") Var(\"r.lo2\"))\n"
      "  Binary(Add Var(\"r.lo1\") Var(\"r.lo2\") Var(\"d\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"ea\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"eb\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.lo2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\", \"r.lo1\", \"r.lo2\"]}}}");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_large_constant_masks_modulo (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(Subtract Var(\"ea\") Constant(ConstInt(4660)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Subtract Var(\"a\") Constant(ConstUChar(52)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_left_shift_by_constant_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(LeftShift Var(\"ea\") Constant(ConstInt(4)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"a\") Constant(ConstUChar(4)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_left_shift_by_variable_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(LeftShift Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_left_shift_by_width_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(LeftShift Var(\"ea\") Constant(ConstInt(8)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(LeftShift Var(\"ea\") Constant(ConstInt(8)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_modulo_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Modulo Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Modulo Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_multiply_two_zeroextended_uchars (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Multiply Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_negative_constant_signed_target (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(Add Var(\"ea\") Constant(ConstInt(-1)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Constant(ConstChar(-1)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_no_ssa_dsts_is_noop (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_no_symbols_is_noop (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_one_extend_one_small_constant (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(Add Var(\"ea\") Constant(ConstInt(3)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Constant(ConstUChar(3)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_right_shift_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(RightShift Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(RightShift Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_right_shift_of_extended_byte_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(RightShift Var(\"ea\") Constant(ConstInt(5)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(RightShift Var(\"a\") Constant(ConstUChar(5)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_right_shift_of_wider_extended_value_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(RightShift Var(\"ea\") Constant(ConstInt(5)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(RightShift Var(\"ea\") Constant(ConstInt(5)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_signed_right_shift_by_one_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(RightShift Var(\"ea\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(RightShift Var(\"a\") Constant(ConstUChar(1)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_signed_right_shift_by_three_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(RightShift Var(\"ea\") Constant(ConstInt(3)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(RightShift Var(\"a\") Constant(ConstUChar(3)) Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_signext_two_schars (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"a\") Var(\"ea\"))\n"
      "  SignExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Subtract Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_subtract_two_zeroextended_uchars (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"eb\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  ZeroExtend(Var(\"b\") Var(\"eb\"))\n"
      "  Binary(Subtract Var(\"ea\") Var(\"eb\") Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Subtract Var(\"a\") Var(\"b\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_two_truncates_into_one_global (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"r1\", \"r2\", \"w\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"w\") Constant(ConstInt(1)) Var(\"r1\"))\n"
      "  Truncate(Var(\"r1\") Var(\"g\"))\n"
      "  Binary(Add Var(\"w\") Constant(ConstInt(1)) Var(\"r2\"))\n"
      "  Truncate(Var(\"r2\") Var(\"g\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Truncate(Var(\"w\") Var(\"r1.lo1\"))\n"
      "  Binary(Add Var(\"r1.lo1\") Constant(ConstUChar(1)) Var(\"g\"))\n"
      "  Truncate(Var(\"w\") Var(\"r2.lo1\"))\n"
      "  Binary(Add Var(\"r2.lo1\") Constant(ConstUChar(1)) Var(\"g\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r2.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"r1\", \"r1.lo1\", \"r2\", \"r2.lo1\", \"w\"]}}}");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_volatile_wide_global_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_wide_global_changed_before_the_truncate (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Copy(Constant(ConstInt(5)) Var(\"w\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Copy(Constant(ConstInt(5)) Var(\"w\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_wide_global_changed_before_the_truncate (standalone) (2)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_wide_global_changed_before_the_truncate (standalone) (3)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Label(\"L\")\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Label(\"L\")\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_wide_global_changed_before_the_truncate (standalone) (4)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Store(Var(\"x\") Var(\"x\") false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Store(Var(\"x\") Var(\"x\") false)\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_wide_global_operand_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\", \"x\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"w\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"x\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"x\"))\n"
      "  Truncate(Var(\"w\") Var(\"r.lo1\"))\n"
      "  Binary(LeftShift Var(\"r.lo1\") Constant(ConstUChar(1)) Var(\"d\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"r\", \"r.lo1\", \"x\"]}}}");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestNarrowWidenedArith.test_zero_extended_signed_byte_not_narrowed (standalone) (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"d\", \"ea\", \"r\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(RightShift Var(\"ea\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"a\") Var(\"ea\"))\n"
      "  Binary(RightShift Var(\"ea\") Constant(ConstInt(1)) Var(\"r\"))\n"
      "  Truncate(Var(\"r\") Var(\"d\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_narrow_widened_arith.TestVariableLeftShift.test_runs (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.v\", \"@0.v.1\", \"@1.n\"]}}}",
      "Function(\"s\" true [\"@0.v\" \"@1.n\"] [\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%0.1\"))\n"
      "  ZeroExtend(Var(\"@0.v\") Var(\"%1.1\"))\n"
      "  Binary(LeftShift Var(\"%1.1\") Var(\"%0.1\") Var(\"%2.1\"))\n"
      "  Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"s\" true [\"@0.v\" \"@1.n\"] [\n"
      "  Binary(LeftShift Var(\"@0.v\") Var(\"@1.n\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_narrow_widened_arith: test_truncate_extend_fold.TestByteLoopAgainstSizeof.test_one_byte_compare (1)") {
  check_stage_case(
      "tac_narrow_widened_arith",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%3.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.i.1\", \"@0.i.2\", \"@0.i.3\", \"@1.i.1\", \"@1.i.2\", \"@1.i.3\", \"@2.p.1\", \"@3.i.1\", \"@3.i.2\", \"@3.i.3\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".f@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%13.1\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(40)) \".loop@0_break\")\n"
      "  Copy(Var(\"@0.i.2\") Var(\"%5.1\"))\n"
      "  IndexedLoad(\"b\" Var(\"%5.1\") Var(\"%6.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Constant(ConstInt(1)) Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Copy(Var(\"@0.i.2\") Var(\"%11.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%11.1\") Var(\"%9.1\") false 0)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@1.i.2\") [PhiArg(\".loop@0_break\" Constant(ConstUChar(0))) PhiArg(\".loop@1_continue\" Var(\"%24.1\"))])\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%15.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Constant(ConstUChar(40)) \".loop@1_break\")\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%19.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%19.1\") Var(\"%20.1\") false 0 0)\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%22.1\"))\n"
      "  IndexedSymbolStore(\"b\" Var(\"%22.1\") Var(\"%20.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"%24.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"a\") Var(\"%25.1\"))\n"
      "  Label(\".loop@2_start\")\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".loop@1_break\" Constant(ConstUChar(0))) PhiArg(\".loop@2_continue\" Var(\"%37.1\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%27.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.i.2\") Constant(ConstUChar(40)) \".loop@2_break\")\n"
      "  Binary(Add Var(\"%25.1\") Var(\"%27.1\") Var(\"%31.1\"))\n"
      "  Load(Var(\"%31.1\") Var(\"%32.1\") false)\n"
      "  ZeroExtend(Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  Binary(Add Var(\"%33.1\") Constant(ConstInt(2)) Var(\"%34.1\"))\n"
      "  Truncate(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Store(Var(\"%35.1\") Var(\"%31.1\") false)\n"
      "  Label(\".loop@2_continue\")\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%37.1\"))\n"
      "  Jump(\".loop@2_start\")\n"
      "  Label(\".loop@2_break\")\n"
      "  IndexedLoad(\"b\" Constant(ConstUChar(39)) Var(\"%38.1\") false 0 0)\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.i.2\") [PhiArg(\".f@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%13.1\"))])\n"
      "  ZeroExtend(Var(\"@0.i.2\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(40)) \".loop@0_break\")\n"
      "  Copy(Var(\"@0.i.2\") Var(\"%5.1\"))\n"
      "  IndexedLoad(\"b\" Var(\"%5.1\") Var(\"%6.1\") false 0 0)\n"
      "  Binary(Add Var(\"%6.1\") Constant(ConstUChar(1)) Var(\"%9.1\"))\n"
      "  Copy(Var(\"@0.i.2\") Var(\"%11.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%11.1\") Var(\"%9.1\") false 0)\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@1.i.2\") [PhiArg(\".loop@0_break\" Constant(ConstUChar(0))) PhiArg(\".loop@1_continue\" Var(\"%24.1\"))])\n"
      "  ZeroExtend(Var(\"@1.i.2\") Var(\"%15.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Constant(ConstUChar(40)) \".loop@1_break\")\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%19.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%19.1\") Var(\"%20.1\") false 0 0)\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%22.1\"))\n"
      "  IndexedSymbolStore(\"b\" Var(\"%22.1\") Var(\"%20.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"%24.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"a\") Var(\"%25.1\"))\n"
      "  Label(\".loop@2_start\")\n"
      "  Phi(Var(\"@3.i.2\") [PhiArg(\".loop@1_break\" Constant(ConstUChar(0))) PhiArg(\".loop@2_continue\" Var(\"%37.1\"))])\n"
      "  ZeroExtend(Var(\"@3.i.2\") Var(\"%27.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.i.2\") Constant(ConstUChar(40)) \".loop@2_break\")\n"
      "  Binary(Add Var(\"%25.1\") Var(\"%27.1\") Var(\"%31.1\"))\n"
      "  Load(Var(\"%31.1\") Var(\"%32.1\") false)\n"
      "  Binary(Add Var(\"%32.1\") Constant(ConstUChar(2)) Var(\"%35.1\"))\n"
      "  Store(Var(\"%35.1\") Var(\"%31.1\") false)\n"
      "  Label(\".loop@2_continue\")\n"
      "  Binary(Add Var(\"@3.i.2\") Constant(ConstUChar(1)) Var(\"%37.1\"))\n"
      "  Jump(\".loop@2_start\")\n"
      "  Label(\".loop@2_break\")\n"
      "  IndexedLoad(\"b\" Constant(ConstUChar(39)) Var(\"%38.1\") false 0 0)\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}
