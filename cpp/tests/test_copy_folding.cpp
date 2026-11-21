// The recorded calls of `tac_fold_copies_in_fixedpoint` and
// `tac_fold_copies_post_destruction`, the C++ port of
// passes/optimization/copy_folding.py, with what the Python pass
// returned. They were recorded with the capture harness's wrappers
// while tests/test_copy_folding.py ran, through the TAC optimizer's
// driver and through the Python entry points that the tests call
// directly. They are every call of an entry point on the tests'
// synthetic TAC and every call through the driver that changed the
// function.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_non_adjacent_does_not_fuse (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "  Label(\"L\")\n"
      "  Copy(Var(\"%t\") Var(\"x\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "  Label(\"L\")\n"
      "  Copy(Var(\"%t\") Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_jump_thread.TestPrograms.test_and_or_results (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"z\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.xs\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 0, 5, 1]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@2.ys\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"pas"
      "ses.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 1, 0, 0, 6]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@3.zs\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 5)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 0, 1, 0, 0]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@0.r.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], ["
      "\"%24.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1\", {\"object\": \"passes.type_checki"
      "ng.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%29.2\", \"%29.3\", \"%29.4\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"%40.2\", \"%40.3\", \"%41.1\", \"%41.2\", \"%41.3\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%47.2\", \"%47.3\", \"%48.1\", \"%49.1\", \"@0.r.1\", \"@0.r.2\", \"@0.r.3\", \"@4.k.1\", \"@4.k.2\", \"@4.k.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.r.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%31.1\"))])\n"
      "  Phi(Var(\"@4.k.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%33.1\"))])\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.k.2\") Constant(ConstUChar(5)) \".loop@0_break\")\n"
      "  Truncate(Var(\"%17.1\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"%21.1\") false 0 0)\n"
      "  Copy(Var(\"%21.1\") Var(\"x\"))\n"
      "  Truncate(Var(\"%17.1\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"%24.1\") false 0 0)\n"
      "  Copy(Var(\"%24.1\") Var(\"y\"))\n"
      "  Truncate(Var(\"%17.1\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"%27.1\") false 0 0)\n"
      "  Copy(Var(\"%27.1\") Var(\"z\"))\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_and_false@0\")\n"
      "  JumpIfTrue(Var(\"y\") \".i0_or_true@2\")\n"
      "  JumpIfTrue(Var(\"z\") \".i0_or_true@2\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Jump(\".i0_or_end@3\")\n"
      "  Label(\".i0_or_true@2\")\n"
      "  Label(\".i0_or_end@3\")\n"
      "  Phi(Var(\"%40.3\") [PhiArg(\".main@ssa_block@4\" Constant(ConstInt(0))) PhiArg(\".i0_or_true@2\" Constant(ConstInt(1)))])\n"
      "  JumpIfFalse(Var(\"%40.3\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Jump(\".i0_and_end@1\")\n"
      "  Label(\".i0_and_false@0\")\n"
      "  Label(\".i0_and_end@1\")\n"
      "  Phi(Var(\"%41.3\") [PhiArg(\".main@ssa_block@5\" Constant(ConstInt(1))) PhiArg(\".i0_and_false@0\" Constant(ConstInt(0)))])\n"
      "  JumpIfFalse(Var(\"%41.3\") \".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".i0_or_true@5\")\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".i0_or_true@5\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Jump(\".i0_or_end@6\")\n"
      "  Label(\".i0_or_true@5\")\n"
      "  Label(\".i0_or_end@6\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@8\" Constant(ConstInt(0))) PhiArg(\".i0_or_true@5\" Constant(ConstInt(1)))])\n"
      "  JumpIfFalse(Var(\"%47.3\") \".i0_if_end@7\")\n"
      "  Label(\".main@ssa_block@9\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@7\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%29.4\") [PhiArg(\".main@ssa_block@6\" Constant(ConstUChar(3))) PhiArg(\".main@ssa_block@9\" Constant(ConstUChar(4))) PhiArg(\".i0_if_end@7\" Constant(ConstUChar(5)))])\n"
      "  ZeroExtend(Var(\"%29.4\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"%28.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@4.k.2\") Constant(ConstUChar(1)) Var(\"%33.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.r.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.r.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%31.1\"))])\n"
      "  Phi(Var(\"@4.k.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%33.1\"))])\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.k.2\") Constant(ConstUChar(5)) \".loop@0_break\")\n"
      "  Truncate(Var(\"%17.1\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"x\") false 0 0)\n"
      "  Truncate(Var(\"%17.1\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"y\") false 0 0)\n"
      "  Truncate(Var(\"%17.1\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"z\") false 0 0)\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_and_false@0\")\n"
      "  JumpIfTrue(Var(\"y\") \".i0_or_true@2\")\n"
      "  JumpIfTrue(Var(\"z\") \".i0_or_true@2\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Jump(\".i0_or_end@3\")\n"
      "  Label(\".i0_or_true@2\")\n"
      "  Label(\".i0_or_end@3\")\n"
      "  Phi(Var(\"%40.3\") [PhiArg(\".main@ssa_block@4\" Constant(ConstInt(0))) PhiArg(\".i0_or_true@2\" Constant(ConstInt(1)))])\n"
      "  JumpIfFalse(Var(\"%40.3\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Jump(\".i0_and_end@1\")\n"
      "  Label(\".i0_and_false@0\")\n"
      "  Label(\".i0_and_end@1\")\n"
      "  Phi(Var(\"%41.3\") [PhiArg(\".main@ssa_block@5\" Constant(ConstInt(1))) PhiArg(\".i0_and_false@0\" Constant(ConstInt(0)))])\n"
      "  JumpIfFalse(Var(\"%41.3\") \".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".i0_or_true@5\")\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".i0_or_true@5\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Jump(\".i0_or_end@6\")\n"
      "  Label(\".i0_or_true@5\")\n"
      "  Label(\".i0_or_end@6\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@8\" Constant(ConstInt(0))) PhiArg(\".i0_or_true@5\" Constant(ConstInt(1)))])\n"
      "  JumpIfFalse(Var(\"%47.3\") \".i0_if_end@7\")\n"
      "  Label(\".main@ssa_block@9\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@7\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%29.4\") [PhiArg(\".main@ssa_block@6\" Constant(ConstUChar(3))) PhiArg(\".main@ssa_block@9\" Constant(ConstUChar(4))) PhiArg(\".i0_if_end@7\" Constant(ConstUChar(5)))])\n"
      "  ZeroExtend(Var(\"%29.4\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"%28.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@4.k.2\") Constant(ConstUChar(1)) Var(\"%33.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.r.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_signextend_plus_copy_fuses (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"x\") Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Var(\"y\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"x\") Var(\"y\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_unary_plus_copy_fuses (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Unary(Negate Var(\"x\") Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Var(\"y\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Unary(Negate Var(\"x\") Var(\"y\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 236, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\"]}}}",
      "Function(\"next\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"P\"))\n"
      "  Load(Var(\"P\") Var(\"%1.1\") false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"next\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"P\"))\n"
      "  Load(Var(\"P\") Var(\"%1.1\") false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_binary_plus_copy_fuses (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Var(\"x\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(1)) Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_jump_thread.TestPrograms.test_found_and_not_found (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@2.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.i.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%39.1\", {"
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%46.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%11.1\", \"%11.2\", \"%1"
      "1.3\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%48.1\", \"%49.1\", \"@2.v\", \"@5.v.1\", \"@6.i.1\", \"@6.i.2\", \"@6.i.3\"]}}}",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"%47.1\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Truncate(Var(\"%39.1\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"%46.1\"))\n"
      "  Copy(Var(\"%46.1\") Var(\"hits\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Label(\".i0_loop@0_break\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%11.3\") [PhiArg(\".f@ssa_block@1\" Constant(ConstUChar(1))) PhiArg(\".i0_loop@0_break\" Constant(ConstUChar(0)))])\n"
      "  JumpIfFalse(Var(\"%11.3\") \".if_end@1\")\n"
      "  Ret(Constant(ConstUChar(10)))\n"
      "  Label(\".if_end@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  Truncate(Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"hits\"))\n"
      "  Ret(Constant(ConstUChar(20)))\n"
      "])\n",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"%47.1\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Truncate(Var(\"%39.1\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Label(\".i0_loop@0_break\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%11.3\") [PhiArg(\".f@ssa_block@1\" Constant(ConstUChar(1))) PhiArg(\".i0_loop@0_break\" Constant(ConstUChar(0)))])\n"
      "  JumpIfFalse(Var(\"%11.3\") \".if_end@1\")\n"
      "  Ret(Constant(ConstUChar(10)))\n"
      "  Label(\".if_end@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  Truncate(Var(\"%14.1\") Var(\"hits\"))\n"
      "  Ret(Constant(ConstUChar(20)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_first_instr_with_no_consumer_passes_through (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_copy_dst_constant_not_a_target (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_phi_dst_does_not_redirect (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Phi(Var(\"%t\") [PhiArg(\"L1\" Var(\"a\")) PhiArg(\"L2\" Var(\"b\"))])\n"
      "  Copy(Var(\"%t\") Var(\"x\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Phi(Var(\"%t\") [PhiArg(\"L1\" Var(\"a\")) PhiArg(\"L2\" Var(\"b\"))])\n"
      "  Copy(Var(\"%t\") Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_consumer_not_a_copy_does_not_fuse (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "  Binary(Add Var(\"%t\") Constant(ConstInt(1)) Var(\"%u\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "  Binary(Add Var(\"%t\") Constant(ConstInt(1)) Var(\"%u\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_function_call_plus_copy_fuses (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  FunctionCall(\"g\" [Var(\"a\")] Var(\"%t\") false false)\n"
      "  Copy(Var(\"%t\") Var(\"y\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  FunctionCall(\"g\" [Var(\"a\")] Var(\"y\") false false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_multi_use_temp_does_not_fuse (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "  Binary(Multiply Var(\"%t\") Constant(ConstInt(2)) Var(\"%u\"))\n"
      "  Copy(Var(\"%t\") Var(\"x\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"a\") Var(\"b\") Var(\"%t\"))\n"
      "  Binary(Multiply Var(\"%t\") Constant(ConstInt(2)) Var(\"%u\"))\n"
      "  Copy(Var(\"%t\") Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingUnit.test_chained_copy_fuses (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"A\") Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Var(\"B\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"A\") Var(\"B\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingObservableNames.test_address_taken_intermediate_not_fused (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"%4\") Var(\"a\"))\n"
      "  Copy(Var(\"a\") Var(\"b\"))\n"
      "  GetAddress(Var(\"a\") Var(\"%p\"))\n"
      "  Load(Var(\"%p\") Var(\"%v\") false)\n"
      "  Ret(Var(\"%v\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"%4\") Var(\"a\"))\n"
      "  Copy(Var(\"a\") Var(\"b\"))\n"
      "  GetAddress(Var(\"a\") Var(\"%p\"))\n"
      "  Load(Var(\"%p\") Var(\"%v\") false)\n"
      "  Ret(Var(\"%v\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 2)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 236, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"Q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.sum.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.L"
      "ocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": fal"
      "se, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%16.1\", \"%16.1.sr0\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%9.1\", \"@0.sum.1\", \"@0.sum.2\", \"@0.sum.3\", \"@1.k.1\", \"@1.k.2\", \"@1.k.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"T\") Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"P\"))\n"
      "  GetAddress(Var(\"T\") Var(\"%10.1\"))\n"
      "  Copy(Var(\"%10.1\") Var(\"Q\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@0_continue\" Var(\"%24.1\"))])\n"
      "  Phi(Var(\"@1.k.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%26.1\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.k.2\") Constant(ConstUChar(7)) \".loop@0_break\")\n"
      "  Binary(LeftShift Var(\"@0.sum.2\") Constant(ConstInt(1)) Var(\"%16.1.sr0\"))\n"
      "  Binary(Add Var(\"%16.1.sr0\") Var(\"@0.sum.2\") Var(\"%16.1\"))\n"
      "  FunctionCall(\"next\" [] Var(\"%17.1\") false false)\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  FunctionCall(\"nextq\" [] Var(\"%21.1\") false false)\n"
      "  ZeroExtend(Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Copy(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  Binary(Add Var(\"%20.1\") Var(\"%23.1\") Var(\"%24.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@1.k.2\") Constant(ConstUChar(1)) Var(\"%26.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  GetAddress(Var(\"T\") Var(\"%27.1\"))\n"
      "  Binary(Subtract Var(\"P\") Var(\"%27.1\") Var(\"%28.1\"))\n"
      "  Binary(Multiply Var(\"%28.1\") Constant(ConstInt(1000)) Var(\"%29.1\"))\n"
      "  Copy(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"@0.sum.2\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Copy(Var(\"%31.1\") Var(\"%32.1\"))\n"
      "  Ret(Var(\"%32.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"T\") Var(\"P\"))\n"
      "  GetAddress(Var(\"T\") Var(\"Q\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.sum.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@0_continue\" Var(\"%24.1\"))])\n"
      "  Phi(Var(\"@1.k.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%26.1\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.k.2\") Constant(ConstUChar(7)) \".loop@0_break\")\n"
      "  Binary(LeftShift Var(\"@0.sum.2\") Constant(ConstInt(1)) Var(\"%16.1.sr0\"))\n"
      "  Binary(Add Var(\"%16.1.sr0\") Var(\"@0.sum.2\") Var(\"%16.1\"))\n"
      "  FunctionCall(\"next\" [] Var(\"%17.1\") false false)\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  FunctionCall(\"nextq\" [] Var(\"%21.1\") false false)\n"
      "  ZeroExtend(Var(\"%21.1\") Var(\"%23.1\"))\n"
      "  Binary(Add Var(\"%20.1\") Var(\"%23.1\") Var(\"%24.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@1.k.2\") Constant(ConstUChar(1)) Var(\"%26.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  GetAddress(Var(\"T\") Var(\"%27.1\"))\n"
      "  Binary(Subtract Var(\"P\") Var(\"%27.1\") Var(\"%28.1\"))\n"
      "  Binary(Multiply Var(\"%28.1\") Constant(ConstInt(1000)) Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"@0.sum.2\") Var(\"%30.1\") Var(\"%32.1\"))\n"
      "  Ret(Var(\"%32.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 3)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"Q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalA"
      "ttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\"]}}}",
      "Function(\"nextq\" true [] [\n"
      "  Binary(Add Var(\"Q\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"Q\"))\n"
      "  Load(Var(\"Q\") Var(\"%3.1\") false)\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"Q\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Truncate(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "Function(\"nextq\" true [] [\n"
      "  Binary(Add Var(\"Q\") Constant(ConstInt(1)) Var(\"Q\"))\n"
      "  Load(Var(\"Q\") Var(\"%3.1\") false)\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"Q\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Truncate(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_in_fixedpoint: test_copy_folding.TestCopyFoldingEndToEnd.test_static_counter_collapses_to_inc_chain (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_in_fixedpoint",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"counter\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 0}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Var(\"counter\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"counter\"))\n"
      "  Ret(Var(\"counter\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Var(\"counter\") Constant(ConstInt(1)) Var(\"counter\"))\n"
      "  Ret(Var(\"counter\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_post_destruction: test_jump_thread.TestPrograms.test_found_and_not_found (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_post_destruction",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@3.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {"
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.lo1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%33.1.lo1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(4))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(4))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_post_destruction: test_jump_thread.TestPrograms.test_found_and_not_found (call 2)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_post_destruction",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@2.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.i.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41.1\", {"
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%11.1\", \"%11.2\", \"%11.3\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%48.1\", \"%49.1\", \"@2.v\", \"@5.v.1\", \"@6.i.1\", \"@6.i.2\", \"@6.i.3\"]}}}",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Copy(Var(\"@6.i.1\") Var(\"@6.i.2\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  IndexedLoad(\"a\" Var(\"@6.i.2\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Jump(\".f@jthread@0\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"@6.i.2\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Jump(\".if_end@1\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(10)))\n"
      "  Label(\".if_end@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(2)) Var(\"hits\"))\n"
      "  Ret(Constant(ConstUChar(20)))\n"
      "])\n",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.2\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  IndexedLoad(\"a\" Var(\"@6.i.2\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Jump(\".f@jthread@0\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"@6.i.2\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Jump(\".if_end@1\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(10)))\n"
      "  Label(\".if_end@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(2)) Var(\"hits\"))\n"
      "  Ret(Constant(ConstUChar(20)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_post_destruction: test_jump_thread.TestPrograms.test_found_and_not_found (call 3)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_post_destruction",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@3.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {"
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.lo1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%33.1.lo1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(21))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(21))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_post_destruction: test_reassoc_offset_licm.TestReassocLicmSim.test_pointer_advanced_by_byte_in_loop (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_post_destruction",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 40)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@4.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@4.i.1\", \"@4.i.2\", \"@4.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@4.i.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.i.2\") Constant(ConstUChar(40)) \".loop@1_break\")\n"
      "  IndexedSymbolStore(\"t\" Var(\"@4.i.2\") Var(\"@4.i.2\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Copy(Var(\"%13.1\") Var(\"@4.i.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"t\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstInt(1)) Var(\"%15.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%15.1\") Constant(ConstUChar(3)) Constant(ConstUChar(12))] Var(\"%18.1\") false false)\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@4.i.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.i.2\") Constant(ConstUChar(40)) \".loop@1_break\")\n"
      "  IndexedSymbolStore(\"t\" Var(\"@4.i.2\") Var(\"@4.i.2\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"@4.i.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  GetAddress(Var(\"t\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstInt(1)) Var(\"%15.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%15.1\") Constant(ConstUChar(3)) Constant(ConstUChar(12))] Var(\"%18.1\") false false)\n"
      "  Ret(Var(\"%18.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_post_destruction: test_jump_thread.TestPrograms.test_found_and_not_found (call 4)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_post_destruction",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@3.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {"
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.lo1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%33.1.lo1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(9))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(9))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_copies_post_destruction: test_jump_thread.TestPrograms.test_found_and_not_found (call 5)") {
  c6502::testing::check_stage_case(
      "tac_fold_copies_post_destruction",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 8)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"hits\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@3.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {"
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.lo1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%33.1.lo1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(0))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@3.k.2\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Binary(LeftShift Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%22.1.lo1\"))\n"
      "  Binary(Add Var(\"%22.1.lo1\") Var(\"@3.k.2\") Var(\"%23.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"@3.k.2\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"@3.k.2\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUChar(0))] Var(\"%30.1\") false false)\n"
      "  Binary(Add Constant(ConstUChar(0)) Var(\"%30.1\") Var(\"%34.1\"))\n"
      "  ZeroExtend(Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}
