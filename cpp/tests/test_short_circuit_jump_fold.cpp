// The recorded calls of `tac_fold_short_circuit_jump`, the C++ port of
// passes/optimization/short_circuit_jump_fold.py, with what the Python
// pass returned. They were recorded with the capture harness's wrappers
// while tests/test_short_circuit_jump_fold.py,
// tests/test_known_compare_thread.py, and tests/test_flag_compares.py
// ran, through the TAC optimizer's driver and through the Python entry
// points that the tests call directly. They are the calls that changed
// the function or the symbols, and the calls of small synthetic
// functions through those entry points. The cases after them are the
// calls of the corpus (tools/capture_passes.py) that changed the
// function and whose input dump is under 3,000 characters long.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRewriteGuards.test_jumpif_on_different_var_doesnt_fire (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%other\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%other\") \".T\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRewriteGuards.test_non_zero_one_constants_dont_fire (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(5)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(5)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestCopiesBeforeConsumer.test_copies_on_the_short_circuit_arm (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"a\") \".and_false\")\n"
      "  Copy(Var(\"l.2\") Var(\"l.4\"))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Jump(\".and_end\")\n"
      "  Label(\".and_false\")\n"
      "  Copy(Var(\"l.3\") Var(\"l.4\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"t\"))\n"
      "  Label(\".and_end\")\n"
      "  JumpIfFalse(Var(\"t\") \".if_end\")\n"
      "  Copy(Constant(ConstInt(9)) Var(\"l.4\"))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Var(\"l.4\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"a\") \".f@scfold@0\")\n"
      "  Copy(Var(\"l.2\") Var(\"l.4\"))\n"
      "  Copy(Constant(ConstInt(9)) Var(\"l.4\"))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Var(\"l.4\"))\n"
      "  Label(\".f@scfold@0\")\n"
      "  Copy(Var(\"l.3\") Var(\"l.4\"))\n"
      "  Jump(\".if_end\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRetargetingShapes.test_jumpifcmp_chain_retargets (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(LessThan Var(\"%x\") Constant(ConstInt(64)) \".BR\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%x\") Constant(ConstInt(80)) \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(LessThan Var(\"%x\") Constant(ConstInt(64)) \".T\")\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%x\") Constant(ConstInt(80)) \".T\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestCopiesBeforeConsumer.test_copy_of_the_temporary_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"a\") \".and_false\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Jump(\".and_end\")\n"
      "  Label(\".and_false\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"t\"))\n"
      "  Label(\".and_end\")\n"
      "  Copy(Var(\"t\") Var(\"u\"))\n"
      "  JumpIfFalse(Var(\"t\") \".if_end\")\n"
      "  Label(\".if_end\")\n"
      "  Ret(Var(\"u\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"a\") \".and_false\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Jump(\".and_end\")\n"
      "  Label(\".and_false\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"t\"))\n"
      "  Label(\".and_end\")\n"
      "  Copy(Var(\"t\") Var(\"u\"))\n"
      "  JumpIfFalse(Var(\"t\") \".if_end\")\n"
      "  Label(\".if_end\")\n"
      "  Ret(Var(\"u\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestNestedShortCircuit.test_nested_or_of_and (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR1\")\n"
      "  JumpIfFalse(Var(\"%b\") \".BR1\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t1\"))\n"
      "  Jump(\".END1\")\n"
      "  Label(\".BR1\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t1\"))\n"
      "  Label(\".END1\")\n"
      "  JumpIfTrue(Var(\"%t1\") \".BR2\")\n"
      "  JumpIfTrue(Var(\"%c\") \".BR2\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t2\"))\n"
      "  Jump(\".END2\")\n"
      "  Label(\".BR2\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t2\"))\n"
      "  Label(\".END2\")\n"
      "  JumpIfFalse(Var(\"%t2\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".f@scfold@0\")\n"
      "  JumpIfFalse(Var(\"%b\") \".f@scfold@0\")\n"
      "  Jump(\".f@scfold@1\")\n"
      "  Label(\".f@scfold@0\")\n"
      "  JumpIfTrue(Var(\"%c\") \".f@scfold@1\")\n"
      "  Jump(\".T\")\n"
      "  Label(\".f@scfold@1\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRewriteGuards.test_multi_use_t_doesnt_fire (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "  Copy(Var(\"%t\") Var(\"%retain\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "  Copy(Var(\"%t\") Var(\"%retain\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestCopiesBeforeConsumer.test_natural_case_gets_a_stub (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"a\") \".and_false\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Jump(\".and_end\")\n"
      "  Label(\".and_false\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"t\"))\n"
      "  Label(\".and_end\")\n"
      "  Copy(Var(\"c\") Var(\"c.2\"))\n"
      "  JumpIfFalse(Var(\"t\") \".if_end\")\n"
      "  Copy(Constant(ConstInt(9)) Var(\"c.2\"))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Var(\"c.2\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"a\") \".f@scfold@0\")\n"
      "  Copy(Var(\"c\") Var(\"c.2\"))\n"
      "  Copy(Constant(ConstInt(9)) Var(\"c.2\"))\n"
      "  Label(\".if_end\")\n"
      "  Ret(Var(\"c.2\"))\n"
      "  Label(\".f@scfold@0\")\n"
      "  Copy(Var(\"c\") Var(\"c.2\"))\n"
      "  Jump(\".if_end\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestFlippedCase.test_or_jumpiffalse_mints_scfold_and_jumps_to_t (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%a\") \".BR\")\n"
      "  JumpIfTrue(Var(\"%b\") \".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%a\") \".f@scfold@0\")\n"
      "  JumpIfTrue(Var(\"%b\") \".f@scfold@0\")\n"
      "  Jump(\".T\")\n"
      "  Label(\".f@scfold@0\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestFlippedCase.test_and_jumpiftrue_mints_scfold_and_jumps_to_t (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  JumpIfFalse(Var(\"%b\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfTrue(Var(\"%t\") \".T\")\n"
      "  Label(\".AFTER\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".f@scfold@0\")\n"
      "  JumpIfFalse(Var(\"%b\") \".f@scfold@0\")\n"
      "  Jump(\".T\")\n"
      "  Label(\".f@scfold@0\")\n"
      "  Label(\".AFTER\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRewriteGuards.test_no_consumer_doesnt_fire (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  Copy(Var(\"%t\") Var(\"%out\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  Copy(Var(\"%t\") Var(\"%out\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_known_compare_thread.TestRuns.test_values (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.2\", \"%1.3\", \"%10.1\", \"%11.1\", \"%11.2\", \"%11.3\", \"%13.1\", \"%14.1\", \"%14.2\", \"%14.3\", \"%15.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"%4.1\", \"%6.1\", \"%8.1\", \"@0.t\", \"@1.r.1\", \"@1.r.2\", \"@1.r.3\"]}}}",
      "Function(\"f\" false [\"@0.t\"] [\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUInt(1)) \".f@jthread@1\")\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUInt(2)) \".f@jthread@1\")\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUInt(3)) \".f@jthread@1\")\n"
      "  Label(\".f@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@1.r.3\"))\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".f@jthread@1\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUInt(2)) \".cond_else@5\")\n"
      "  Label(\".f@ssa_block@6\")\n"
      "  Copy(Constant(ConstInt(5)) Var(\"%11.3\"))\n"
      "  Jump(\".cond_end@6\")\n"
      "  Label(\".cond_else@5\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUInt(3)) \".cond_else@7\")\n"
      "  Label(\".f@ssa_block@7\")\n"
      "  Copy(Constant(ConstInt(6)) Var(\"%14.3\"))\n"
      "  Jump(\".cond_end@8\")\n"
      "  Label(\".cond_else@7\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"%14.3\"))\n"
      "  Label(\".cond_end@8\")\n"
      "  Copy(Var(\"%14.3\") Var(\"%11.3\"))\n"
      "  Label(\".cond_end@6\")\n"
      "  Truncate(Var(\"%11.3\") Var(\"@1.r.3\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(Var(\"@1.r.3\"))\n"
      "])\n",
      "Function(\"f\" false [\"@0.t\"] [\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUInt(1)) \".f@jthread@1\")\n"
      "  Binary(BitwiseAnd Var(\"@0.t\") Constant(ConstUInt(65534)) Var(\"@0.t.eqm0\"))\n"
      "  JumpIfCmp(Equal Var(\"@0.t.eqm0\") Constant(ConstUInt(2)) \".f@jthread@1\")\n"
      "  Label(\".f@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@1.r.3\"))\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".f@jthread@1\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUInt(2)) \".cond_else@5\")\n"
      "  Label(\".f@ssa_block@6\")\n"
      "  Copy(Constant(ConstInt(5)) Var(\"%11.3\"))\n"
      "  Jump(\".cond_end@6\")\n"
      "  Label(\".cond_else@5\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUInt(3)) \".cond_else@7\")\n"
      "  Label(\".f@ssa_block@7\")\n"
      "  Copy(Constant(ConstInt(6)) Var(\"%14.3\"))\n"
      "  Jump(\".cond_end@8\")\n"
      "  Label(\".cond_else@7\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"%14.3\"))\n"
      "  Label(\".cond_end@8\")\n"
      "  Copy(Var(\"%14.3\") Var(\"%11.3\"))\n"
      "  Label(\".cond_end@6\")\n"
      "  Truncate(Var(\"%11.3\") Var(\"@1.r.3\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(Var(\"@1.r.3\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.t.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestNaturalCase.test_or_jumpiftrue_retargets_chain_and_deletes_tail (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%a\") \".BR\")\n"
      "  JumpIfTrue(Var(\"%b\") \".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfTrue(Var(\"%t\") \".T\")\n"
      "  Label(\".AFTER\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%a\") \".T\")\n"
      "  JumpIfTrue(Var(\"%b\") \".T\")\n"
      "  Label(\".AFTER\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_flag_compares.TestSimulated.test_both_pipelines (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@7.e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%24.1\", \"%24.2\", \"%24.3\", \"%26.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"@7.e\"]}}}",
      "Function(\"pair\" true [\"@7.e\"] [\n"
      "  JumpIfFalse(Var(\"@7.e\") \".pair@jthread@0\")\n"
      "  JumpIfCmp(Equal Var(\"@7.e\") Constant(ConstUInt(1)) \".pair@jthread@0\")\n"
      "  Jump(\".if_end@6\")\n"
      "  Label(\".pair@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(8)))\n"
      "  Label(\".if_end@6\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "Function(\"pair\" true [\"@7.e\"] [\n"
      "  Binary(BitwiseAnd Var(\"@7.e\") Constant(ConstUInt(65534)) Var(\"@7.e.eqm0\"))\n"
      "  JumpIfFalse(Var(\"@7.e.eqm0\") \".pair@jthread@0\")\n"
      "  Jump(\".if_end@6\")\n"
      "  Label(\".pair@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(8)))\n"
      "  Label(\".if_end@6\")\n"
      "  Ret(Constant(ConstUChar(9)))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@7.e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.e.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: test_flag_compares.TestCompiled.test_two_values_one_test (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.2\", \"%1.3\", \"%3.1\", \"%5.1\", \"@0.e.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  FunctionCall(\"find\" [] Var(\"%0.1\") false false)\n"
      "  JumpIfFalse(Var(\"%0.1\") \".f@jthread@0\")\n"
      "  JumpIfCmp(Equal Var(\"%0.1\") Constant(ConstUInt(1)) \".f@jthread@0\")\n"
      "  Jump(\".if_end@2\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(None)\n"
      "  Label(\".if_end@2\")\n"
      "  FunctionCall(\"draw\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  FunctionCall(\"find\" [] Var(\"%0.1\") false false)\n"
      "  Binary(BitwiseAnd Var(\"%0.1\") Constant(ConstUInt(65534)) Var(\"%0.1.eqm0\"))\n"
      "  JumpIfFalse(Var(\"%0.1.eqm0\") \".f@jthread@0\")\n"
      "  Jump(\".if_end@2\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(None)\n"
      "  Label(\".if_end@2\")\n"
      "  FunctionCall(\"draw\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: test_known_compare_thread.TestRuns.test_values (call 2)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.2\", \"%1.3\", \"%10.1\", \"%11.1\", \"%11.2\", \"%11.3\", \"%12.1\", \"%13.1\", \"%14.1\", \"%14.2\", \"%14.3\", \"%15.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.t\", \"@1.r.1\", \"@1.r.2\", \"@1.r.3\"]}}}",
      "Function(\"f\" false [\"@0.t\"] [\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUChar(1)) \".f@jthread@1\")\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUChar(2)) \".f@jthread@1\")\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUChar(3)) \".f@jthread@1\")\n"
      "  Label(\".f@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@1.r.3\"))\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".f@jthread@1\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUChar(2)) \".cond_else@5\")\n"
      "  Label(\".f@ssa_block@6\")\n"
      "  Copy(Constant(ConstInt(5)) Var(\"%11.3\"))\n"
      "  Jump(\".cond_end@6\")\n"
      "  Label(\".cond_else@5\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUChar(3)) \".cond_else@7\")\n"
      "  Label(\".f@ssa_block@7\")\n"
      "  Copy(Constant(ConstInt(6)) Var(\"%14.3\"))\n"
      "  Jump(\".cond_end@8\")\n"
      "  Label(\".cond_else@7\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"%14.3\"))\n"
      "  Label(\".cond_end@8\")\n"
      "  Copy(Var(\"%14.3\") Var(\"%11.3\"))\n"
      "  Label(\".cond_end@6\")\n"
      "  Truncate(Var(\"%11.3\") Var(\"@1.r.3\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(Var(\"@1.r.3\"))\n"
      "])\n",
      "Function(\"f\" false [\"@0.t\"] [\n"
      "  JumpIfCmp(Equal Var(\"@0.t\") Constant(ConstUChar(1)) \".f@jthread@1\")\n"
      "  Binary(BitwiseAnd Var(\"@0.t\") Constant(ConstUChar(254)) Var(\"@0.t.eqm0\"))\n"
      "  JumpIfCmp(Equal Var(\"@0.t.eqm0\") Constant(ConstUChar(2)) \".f@jthread@1\")\n"
      "  Label(\".f@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"@1.r.3\"))\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".f@jthread@1\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUChar(2)) \".cond_else@5\")\n"
      "  Label(\".f@ssa_block@6\")\n"
      "  Copy(Constant(ConstInt(5)) Var(\"%11.3\"))\n"
      "  Jump(\".cond_end@6\")\n"
      "  Label(\".cond_else@5\")\n"
      "  JumpIfCmp(NotEqual Var(\"@0.t\") Constant(ConstUChar(3)) \".cond_else@7\")\n"
      "  Label(\".f@ssa_block@7\")\n"
      "  Copy(Constant(ConstInt(6)) Var(\"%14.3\"))\n"
      "  Jump(\".cond_end@8\")\n"
      "  Label(\".cond_else@7\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"%14.3\"))\n"
      "  Label(\".cond_end@8\")\n"
      "  Copy(Var(\"%14.3\") Var(\"%11.3\"))\n"
      "  Label(\".cond_end@6\")\n"
      "  Truncate(Var(\"%11.3\") Var(\"@1.r.3\"))\n"
      "  Label(\".if_end@4\")\n"
      "  Ret(Var(\"@1.r.3\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.t.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestNaturalCase.test_and_jumpiffalse_retargets_chain_and_deletes_tail (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%a\") \".AFTER\")\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  JumpIfFalse(Var(\"%b\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "  Label(\".AFTER\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"%a\") \".AFTER\")\n"
      "  JumpIfFalse(Var(\"%a\") \".T\")\n"
      "  JumpIfFalse(Var(\"%b\") \".T\")\n"
      "  Label(\".AFTER\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRewriteGuards.test_multi_target_end_label_doesnt_fire (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Jump(\".END\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Jump(\".END\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRetargetingShapes.test_jumpifmasked_chain_retargets (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfMasked(Var(\"%x\") 128 false \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfMasked(Var(\"%x\") 128 false \".T\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: test_short_circuit_jump_fold.TestRewriteGuards.test_different_t_in_two_copies_doesnt_fire (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t1\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t2\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t1\") \".T\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfFalse(Var(\"%a\") \".BR\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%t1\"))\n"
      "  Jump(\".END\")\n"
      "  Label(\".BR\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%t2\"))\n"
      "  Label(\".END\")\n"
      "  JumpIfFalse(Var(\"%t1\") \".T\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: tests/chapter_9/valid/arguments_in_registers/fibonacci.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%0.2\", \"%0.3\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@0.n\"]}}}",
      "Function(\"fib\" true [\"@0.n\"] [\n"
      "  JumpIfFalse(Var(\"@0.n\") \".fib@jthread@0\")\n"
      "  JumpIfCmp(Equal Var(\"@0.n\") Constant(ConstInt(1)) \".fib@jthread@0\")\n"
      "  Jump(\".if_else@3\")\n"
      "  Label(\".fib@jthread@0\")\n"
      "  Ret(Var(\"@0.n\"))\n"
      "  Label(\".if_else@3\")\n"
      "  Binary(Subtract Var(\"@0.n\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  FunctionCall(\"fib\" [Var(\"%3.1\")] Var(\"%4.1\") false false)\n"
      "  Binary(Subtract Var(\"@0.n\") Constant(ConstInt(2)) Var(\"%5.1\"))\n"
      "  FunctionCall(\"fib\" [Var(\"%5.1\")] Var(\"%6.1\") false false)\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "Function(\"fib\" true [\"@0.n\"] [\n"
      "  Binary(BitwiseAnd Var(\"@0.n\") Constant(ConstInt(-2)) Var(\"@0.n.eqm0\"))\n"
      "  JumpIfFalse(Var(\"@0.n.eqm0\") \".fib@jthread@0\")\n"
      "  Jump(\".if_else@3\")\n"
      "  Label(\".fib@jthread@0\")\n"
      "  Ret(Var(\"@0.n\"))\n"
      "  Label(\".if_else@3\")\n"
      "  Binary(Subtract Var(\"@0.n\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  FunctionCall(\"fib\" [Var(\"%3.1\")] Var(\"%4.1\") false false)\n"
      "  Binary(Subtract Var(\"@0.n\") Constant(ConstInt(2)) Var(\"%5.1\"))\n"
      "  FunctionCall(\"fib\" [Var(\"%5.1\")] Var(\"%6.1\") false false)\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.n.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/ali_baba/combat.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%167.1\", \"%168.1\", \"%169.1\", \"%170.1\", \"%173.1\", \"%174.1\", \"%175.1\", \"%176.1\", \"%176.2\", \"%176.3\", \"%177.1\", \"%178.1\", \"%179.1\", \"%180.1\", \"%181.1\", \"%182.1\", \"%183.1\", \"%184.1\", \"@145.n.1\", \"@145.n.2\", \"@145.n.3\", \"@145.n.4\", \"@145.n.5\", \"@146.count.1\"]}}}",
      "Function(\"pick_random_member\" true [] [\n"
      "  Label(\".loop@4_start\")\n"
      "  IndirectIndexedLoad(Var(\"CHAR_PTR\") Constant(ConstUChar(8)) Var(\"%168.1\") false)\n"
      "  JumpIfTrue(Var(\"%168.1\") \".if_end@24\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"MOB_PTR\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@24\")\n"
      "  FunctionCall(\"random_in_range\" [Var(\"%168.1\")] Var(\"%173.1\") false false)\n"
      "  JumpIfFalse(Var(\"%173.1\") \".loop@4_start\")\n"
      "  Label(\".loop@4_break\")\n"
      "  FunctionCall(\"first_group_member\" [] None false false)\n"
      "  Copy(Var(\"%173.1\") Var(\"@145.n.2\"))\n"
      "  Label(\".loop@5_start\")\n"
      "  IndirectIndexedLoad(Var(\"MOB_PTR\") Constant(ConstUChar(4)) Var(\"%178.1\") false)\n"
      "  Copy(Var(\"@145.n.2\") Var(\"@145.n.4\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%178.1\") Constant(ConstUChar(21)) \".and_false@25\")\n"
      "  Label(\".pick_random_member@ssa_block@1\")\n"
      "  Binary(Subtract Var(\"@145.n.2\") Constant(ConstUChar(1)) Var(\"%182.1\"))\n"
      "  Copy(Var(\"%182.1\") Var(\"@145.n.4\"))\n"
      "  JumpIfTrue(Var(\"%182.1\") \".and_false@25\")\n"
      "  Label(\".pick_random_member@ssa_block@2\")\n"
      "  Copy(Var(\"%182.1\") Var(\"@145.n.5\"))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%176.3\"))\n"
      "  Jump(\".and_end@26\")\n"
      "  Label(\".and_false@25\")\n"
      "  Copy(Var(\"@145.n.4\") Var(\"@145.n.5\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%176.3\"))\n"
      "  Label(\".and_end@26\")\n"
      "  JumpIfFalse(Var(\"%176.3\") \".if_end@27\")\n"
      "  Ret(None)\n"
      "  Label(\".if_end@27\")\n"
      "  FunctionCall(\"next_group_member\" [] None false false)\n"
      "  Label(\".loop@5_continue\")\n"
      "  Copy(Var(\"@145.n.5\") Var(\"@145.n.2\"))\n"
      "  Jump(\".loop@5_start\")\n"
      "])\n",
      "Function(\"pick_random_member\" true [] [\n"
      "  Label(\".loop@4_start\")\n"
      "  IndirectIndexedLoad(Var(\"CHAR_PTR\") Constant(ConstUChar(8)) Var(\"%168.1\") false)\n"
      "  JumpIfTrue(Var(\"%168.1\") \".if_end@24\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"MOB_PTR\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@24\")\n"
      "  FunctionCall(\"random_in_range\" [Var(\"%168.1\")] Var(\"%173.1\") false false)\n"
      "  JumpIfFalse(Var(\"%173.1\") \".loop@4_start\")\n"
      "  Label(\".loop@4_break\")\n"
      "  FunctionCall(\"first_group_member\" [] None false false)\n"
      "  Copy(Var(\"%173.1\") Var(\"@145.n.2\"))\n"
      "  Label(\".loop@5_start\")\n"
      "  IndirectIndexedLoad(Var(\"MOB_PTR\") Constant(ConstUChar(4)) Var(\"%178.1\") false)\n"
      "  Copy(Var(\"@145.n.2\") Var(\"@145.n.4\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%178.1\") Constant(ConstUChar(21)) \".pick_random_member@scfold@0\")\n"
      "  Label(\".pick_random_member@ssa_block@1\")\n"
      "  Binary(Subtract Var(\"@145.n.2\") Constant(ConstUChar(1)) Var(\"%182.1\"))\n"
      "  Copy(Var(\"%182.1\") Var(\"@145.n.4\"))\n"
      "  JumpIfTrue(Var(\"%182.1\") \".pick_random_member@scfold@0\")\n"
      "  Label(\".pick_random_member@ssa_block@2\")\n"
      "  Copy(Var(\"%182.1\") Var(\"@145.n.5\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@27\")\n"
      "  FunctionCall(\"next_group_member\" [] None false false)\n"
      "  Label(\".loop@5_continue\")\n"
      "  Copy(Var(\"@145.n.5\") Var(\"@145.n.2\"))\n"
      "  Jump(\".loop@5_start\")\n"
      "  Label(\".pick_random_member@scfold@0\")\n"
      "  Copy(Var(\"@145.n.4\") Var(\"@145.n.5\"))\n"
      "  Jump(\".if_end@27\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/lode_runner/main.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%18.2\", \"%18.3\", \"%19.1\", \"%19.2\", \"%19.3\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%5.1\", \"%5.2\", \"%5.3\", \"%6.1\", \"%7.1\", \"%8.1\", \"%8.2\", \"%8.3\", \"%9.1\", \"@38.mode\", \"@38.mode.1\", \"@38.mode.2\", \"@39.x\", \"@39.x.1\", \"@39.x.2\", \"@40.y\", \"@40.y.1\", \"@40.y.2\", \"@40.y.3\", \"@40.y.4\", \"@41.key.1\"]}}}",
      "Function(\"poll_inputs\" false [\"@38.mode\" \"@39.x\" \"@40.y\"] [\n"
      "  Label(\".poll_inputs@ssa_preheader@0\")\n"
      "  Copy(Var(\"@39.x\") Var(\"@39.x.1\"))\n"
      "  Copy(Var(\"@40.y\") Var(\"@40.y.1\"))\n"
      "  Copy(Var(\"@38.mode\") Var(\"@38.mode.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  JumpIfCmp(Equal Var(\"@38.mode.1\") Constant(ConstUChar(203)) \".if_end@4\")\n"
      "  IndexedLoad(\"BUTN1\" Constant(ConstUChar(0)) Var(\"%9.1\") true 0 0)\n"
      "  JumpIfMasked(Var(\"%9.1\") 128 true \".poll_inputs@jthread@0\")\n"
      "  IndexedLoad(\"BUTN0\" Constant(ConstUChar(0)) Var(\"%12.1\") true 0 0)\n"
      "  JumpIfMasked(Var(\"%12.1\") 128 true \".poll_inputs@jthread@0\")\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".poll_inputs@jthread@0\")\n"
      "  FunctionCall(\"start_at_level_1\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@4\")\n"
      "  IndexedLoad(\"KBD\" Constant(ConstUChar(0)) Var(\"%15.1\") true 0 0)\n"
      "  JumpIfMasked(Var(\"%15.1\") 128 false \".if_end@5\")\n"
      "  FunctionCall(\"key_pressed\" [Var(\"%15.1\")] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@5\")\n"
      "  Binary(Subtract Var(\"@39.x.1\") Constant(ConstUChar(1)) Var(\"%20.1\"))\n"
      "  Copy(Var(\"@40.y.1\") Var(\"@40.y.3\"))\n"
      "  JumpIfTrue(Var(\"%20.1\") \".and_false@8\")\n"
      "  Label(\".poll_inputs@ssa_block@6\")\n"
      "  Binary(Subtract Var(\"@40.y.1\") Constant(ConstUChar(1)) Var(\"%23.1\"))\n"
      "  Copy(Var(\"%23.1\") Var(\"@40.y.3\"))\n"
      "  JumpIfTrue(Var(\"%23.1\") \".and_false@8\")\n"
      "  Label(\".poll_inputs@ssa_block@7\")\n"
      "  Copy(Var(\"%23.1\") Var(\"@40.y.4\"))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%19.3\"))\n"
      "  Jump(\".and_end@9\")\n"
      "  Label(\".and_false@8\")\n"
      "  Copy(Var(\"@40.y.3\") Var(\"@40.y.4\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%19.3\"))\n"
      "  Label(\".and_end@9\")\n"
      "  JumpIfFalse(Var(\"%19.3\") \".if_end@10\")\n"
      "  Binary(Subtract Var(\"GAME_ROWNUM\") Constant(ConstUChar(1)) Var(\"GAME_ROWNUM\"))\n"
      "  JumpIfTrue(Var(\"GAME_ROWNUM\") \".if_end@10\")\n"
      "  Jump(\".loop@0_break\")\n"
      "  Label(\".if_end@10\")\n"
      "  Copy(Var(\"INPUT_MODE\") Var(\"@38.mode.2\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"%20.1\") Var(\"@39.x.1\"))\n"
      "  Copy(Var(\"@40.y.4\") Var(\"@40.y.1\"))\n"
      "  Copy(Var(\"@38.mode.2\") Var(\"@38.mode.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  JumpIfTrue(Var(\"GAME_MODE\") \".if_end@11\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"GAME_MODE\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"LEVELNUM\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"DEMO_RUNNING\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"HI_SCORE_ELIGIBLE\"))\n"
      "  Copy(Var(\"ENABLE_SOUND\") Var(\"saved_enable_sound\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"ENABLE_SOUND\"))\n"
      "  FunctionCall(\"init_game_data\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@11\")\n"
      "  JumpIfCmp(NotEqual Var(\"GAME_MODE\") Constant(ConstUChar(1)) \".if_end@12\")\n"
      "  FunctionCall(\"show_hi_scores\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@12\")\n"
      "  FunctionCall(\"reset_game\" [] None false true)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"poll_inputs\" false [\"@38.mode\" \"@39.x\" \"@40.y\"] [\n"
      "  Label(\".poll_inputs@ssa_preheader@0\")\n"
      "  Copy(Var(\"@39.x\") Var(\"@39.x.1\"))\n"
      "  Copy(Var(\"@40.y\") Var(\"@40.y.1\"))\n"
      "  Copy(Var(\"@38.mode\") Var(\"@38.mode.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  JumpIfCmp(Equal Var(\"@38.mode.1\") Constant(ConstUChar(203)) \".if_end@4\")\n"
      "  IndexedLoad(\"BUTN1\" Constant(ConstUChar(0)) Var(\"%9.1\") true 0 0)\n"
      "  JumpIfMasked(Var(\"%9.1\") 128 true \".poll_inputs@jthread@0\")\n"
      "  IndexedLoad(\"BUTN0\" Constant(ConstUChar(0)) Var(\"%12.1\") true 0 0)\n"
      "  JumpIfMasked(Var(\"%12.1\") 128 true \".poll_inputs@jthread@0\")\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".poll_inputs@jthread@0\")\n"
      "  FunctionCall(\"start_at_level_1\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@4\")\n"
      "  IndexedLoad(\"KBD\" Constant(ConstUChar(0)) Var(\"%15.1\") true 0 0)\n"
      "  JumpIfMasked(Var(\"%15.1\") 128 false \".if_end@5\")\n"
      "  FunctionCall(\"key_pressed\" [Var(\"%15.1\")] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@5\")\n"
      "  Binary(Subtract Var(\"@39.x.1\") Constant(ConstUChar(1)) Var(\"%20.1\"))\n"
      "  Copy(Var(\"@40.y.1\") Var(\"@40.y.3\"))\n"
      "  JumpIfTrue(Var(\"%20.1\") \".poll_inputs@scfold@0\")\n"
      "  Label(\".poll_inputs@ssa_block@6\")\n"
      "  Binary(Subtract Var(\"@40.y.1\") Constant(ConstUChar(1)) Var(\"%23.1\"))\n"
      "  Copy(Var(\"%23.1\") Var(\"@40.y.3\"))\n"
      "  JumpIfTrue(Var(\"%23.1\") \".poll_inputs@scfold@0\")\n"
      "  Label(\".poll_inputs@ssa_block@7\")\n"
      "  Copy(Var(\"%23.1\") Var(\"@40.y.4\"))\n"
      "  Binary(Subtract Var(\"GAME_ROWNUM\") Constant(ConstUChar(1)) Var(\"GAME_ROWNUM\"))\n"
      "  JumpIfTrue(Var(\"GAME_ROWNUM\") \".if_end@10\")\n"
      "  Jump(\".loop@0_break\")\n"
      "  Label(\".if_end@10\")\n"
      "  Copy(Var(\"INPUT_MODE\") Var(\"@38.mode.2\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"%20.1\") Var(\"@39.x.1\"))\n"
      "  Copy(Var(\"@40.y.4\") Var(\"@40.y.1\"))\n"
      "  Copy(Var(\"@38.mode.2\") Var(\"@38.mode.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  JumpIfTrue(Var(\"GAME_MODE\") \".if_end@11\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"GAME_MODE\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"LEVELNUM\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"DEMO_RUNNING\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"HI_SCORE_ELIGIBLE\"))\n"
      "  Copy(Var(\"ENABLE_SOUND\") Var(\"saved_enable_sound\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"ENABLE_SOUND\"))\n"
      "  FunctionCall(\"init_game_data\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@11\")\n"
      "  JumpIfCmp(NotEqual Var(\"GAME_MODE\") Constant(ConstUChar(1)) \".if_end@12\")\n"
      "  FunctionCall(\"show_hi_scores\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@12\")\n"
      "  FunctionCall(\"reset_game\" [] None false true)\n"
      "  Ret(None)\n"
      "  Label(\".poll_inputs@scfold@0\")\n"
      "  Copy(Var(\"@40.y.3\") Var(\"@40.y.4\"))\n"
      "  Jump(\".if_end@10\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/ali_baba/events.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%215.1\", \"%216.1\", \"%217.1\", \"%218.1\", \"%219.1\", \"%220.1\", \"%221.1\", \"%222.1\", \"%223.1\", \"%224.1\", \"%225.1\", \"%226.1\", \"%227.1\", \"%228.1\", \"%229.1\", \"%230.1\", \"%230.2\", \"%230.3\", \"%231.1\", \"%232.1\", \"%233.1\", \"%234.1\", \"%235.1\", \"%236.1\", \"@67.r.1\", \"@68.remaining.1\", \"@68.remaining.2\", \"@68.remaining.3\", \"@69.index.1\", \"@69.index.2\", \"@69.index.3\", \"@69.index.4\", \"@69.index.5\", \"@70.loc.1\"]}}}",
      "Function(\"pick_random_mob\" false [] [\n"
      "  JumpIfTrue(Var(\"TOTAL_MOB_COUNT\") \".if_end@22\")\n"
      "  FunctionCall(\"pick_random_char\" [] Var(\"%217.1\") false false)\n"
      "  Ret(Var(\"%217.1\"))\n"
      "  Label(\".if_end@22\")\n"
      "  Label(\".loop@7_start\")\n"
      "  FunctionCall(\"random_in_range\" [Var(\"TOTAL_MOB_COUNT\")] Var(\"%218.1\") false false)\n"
      "  JumpIfFalse(Var(\"%218.1\") \".loop@7_start\")\n"
      "  Label(\".loop@7_break\")\n"
      "  Copy(Var(\"%218.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"@69.index.2\"))\n"
      "  Label(\".loop@8_start\")\n"
      "  FunctionCall(\"get_char_record\" [Var(\"@69.index.2\")] Var(\"%223.1\") false false)\n"
      "  IndirectIndexedLoad(Var(\"%223.1\") Constant(ConstUChar(8)) Var(\"%225.1\") false)\n"
      "  Binary(Subtract Var(\"@68.remaining.2\") Var(\"%225.1\") Var(\"%229.1\"))\n"
      "  Label(\".loop@8_continue\")\n"
      "  Copy(Var(\"@69.index.2\") Var(\"@69.index.4\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%229.1\") Constant(ConstChar(0)) \".and_false@23\")\n"
      "  Label(\".pick_random_mob@ssa_block@2\")\n"
      "  Binary(Add Var(\"@69.index.2\") Constant(ConstUChar(1)) Var(\"%233.1\"))\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.4\"))\n"
      "  JumpIfFalse(Var(\"%233.1\") \".and_false@23\")\n"
      "  Label(\".pick_random_mob@ssa_block@3\")\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.5\"))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%230.3\"))\n"
      "  Jump(\".and_end@24\")\n"
      "  Label(\".and_false@23\")\n"
      "  Copy(Var(\"@69.index.4\") Var(\"@69.index.5\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%230.3\"))\n"
      "  Label(\".and_end@24\")\n"
      "  Copy(Var(\"%229.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Var(\"@69.index.5\") Var(\"@69.index.2\"))\n"
      "  JumpIfTrue(Var(\"%230.3\") \".loop@8_start\")\n"
      "  Copy(Var(\"%229.1\") Var(\"%236.1\"))\n"
      "  IndexedSymbolStore(\"MOB_PTR\" Constant(ConstUChar(0)) Var(\"%236.1\") false 0)\n"
      "  Copy(Var(\"%223.1\") Var(\"CHAR_PTR\"))\n"
      "  Ret(Var(\"@69.index.5\"))\n"
      "])\n",
      "Function(\"pick_random_mob\" false [] [\n"
      "  JumpIfTrue(Var(\"TOTAL_MOB_COUNT\") \".if_end@22\")\n"
      "  FunctionCall(\"pick_random_char\" [] Var(\"%217.1\") false false)\n"
      "  Ret(Var(\"%217.1\"))\n"
      "  Label(\".if_end@22\")\n"
      "  Label(\".loop@7_start\")\n"
      "  FunctionCall(\"random_in_range\" [Var(\"TOTAL_MOB_COUNT\")] Var(\"%218.1\") false false)\n"
      "  JumpIfFalse(Var(\"%218.1\") \".loop@7_start\")\n"
      "  Label(\".loop@7_break\")\n"
      "  Copy(Var(\"%218.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"@69.index.2\"))\n"
      "  Label(\".loop@8_start\")\n"
      "  FunctionCall(\"get_char_record\" [Var(\"@69.index.2\")] Var(\"%223.1\") false false)\n"
      "  IndirectIndexedLoad(Var(\"%223.1\") Constant(ConstUChar(8)) Var(\"%225.1\") false)\n"
      "  Binary(Subtract Var(\"@68.remaining.2\") Var(\"%225.1\") Var(\"%229.1\"))\n"
      "  Label(\".loop@8_continue\")\n"
      "  Copy(Var(\"@69.index.2\") Var(\"@69.index.4\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%229.1\") Constant(ConstChar(0)) \".pick_random_mob@scfold@0\")\n"
      "  Label(\".pick_random_mob@ssa_block@2\")\n"
      "  Binary(Add Var(\"@69.index.2\") Constant(ConstUChar(1)) Var(\"%233.1\"))\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.4\"))\n"
      "  JumpIfFalse(Var(\"%233.1\") \".pick_random_mob@scfold@0\")\n"
      "  Label(\".pick_random_mob@ssa_block@3\")\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.5\"))\n"
      "  Copy(Var(\"%229.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Var(\"@69.index.5\") Var(\"@69.index.2\"))\n"
      "  Jump(\".loop@8_start\")\n"
      "  Label(\".pick_random_mob@scfold@0\")\n"
      "  Copy(Var(\"@69.index.4\") Var(\"@69.index.5\"))\n"
      "  Copy(Var(\"%229.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Var(\"@69.index.5\") Var(\"@69.index.2\"))\n"
      "  Copy(Var(\"%229.1\") Var(\"%236.1\"))\n"
      "  IndexedSymbolStore(\"MOB_PTR\" Constant(ConstUChar(0)) Var(\"%236.1\") false 0)\n"
      "  Copy(Var(\"%223.1\") Var(\"CHAR_PTR\"))\n"
      "  Ret(Var(\"@69.index.5\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/lode_runner/player.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%389.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%370.1\", \"%371.1\", \"%372.1\", \"%373.1\", \"%374.1\", \"%375.1\", \"%376.1\", \"%377.1\", \"%378.1\", \"%379.1\", \"%380.1\", \"%381.1\", \"%382.1\", \"%383.1\", \"%384.1\", \"%385.1\", \"%386.1\", \"%387.1\", \"%388.1\", \"%389.1\", \"%390.1\", \"%390.2\", \"%390.3\", \"%391.1\", \"%391.2\", \"%391.3\", \"%392.1\", \"%393.1\", \"%394.1\", \"%395.1\", \"%396.1\", \"%397.1\", \"%398.1\", \"@60.below.1\", \"@60.below.2\"]}}}",
      "Function(\"stands_on_something\" false [] [\n"
      "  JumpIfCmp(NotEqual Var(\"PLAYER_ROW\") Constant(ConstUChar(15)) \".if_end@72\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@72\")\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%374.1\"))\n"
      "  FunctionCall(\"map_row\" [Var(\"%374.1\")] None false false)\n"
      "  Copy(Var(\"map_row@ret\") Var(\"%375.1\"))\n"
      "  IndirectIndexedLoad(Var(\"%375.1\") Var(\"PLAYER_COL\") Var(\"%378.1\") false)\n"
      "  JumpIfTrue(Var(\"%378.1\") \".if_end@73\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "  Label(\".if_end@73\")\n"
      "  JumpIfCmp(NotEqual Var(\"%378.1\") Constant(ConstUChar(8)) \".if_end@74\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@74\")\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%385.1\"))\n"
      "  FunctionCall(\"bg_row\" [Var(\"%385.1\")] None false false)\n"
      "  Copy(Var(\"bg_row@ret\") Var(\"%386.1\"))\n"
      "  IndirectIndexedLoad(Var(\"%386.1\") Var(\"PLAYER_COL\") Var(\"%389.1\") false)\n"
      "  JumpIfCmp(Equal Var(\"%389.1\") Constant(ConstUChar(1)) \".or_true@75\")\n"
      "  JumpIfCmp(Equal Var(\"%389.1\") Constant(ConstUChar(2)) \".or_true@75\")\n"
      "  JumpIfCmp(Equal Var(\"%389.1\") Constant(ConstUChar(3)) \".or_true@75\")\n"
      "  Label(\".stands_on_something@ssa_block@7\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%398.1\"))\n"
      "  Jump(\".or_end@76\")\n"
      "  Label(\".or_true@75\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%398.1\"))\n"
      "  Label(\".or_end@76\")\n"
      "  Ret(Var(\"%398.1\"))\n"
      "])\n",
      "Function(\"stands_on_something\" false [] [\n"
      "  JumpIfCmp(NotEqual Var(\"PLAYER_ROW\") Constant(ConstUChar(15)) \".if_end@72\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@72\")\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%374.1\"))\n"
      "  FunctionCall(\"map_row\" [Var(\"%374.1\")] None false false)\n"
      "  Copy(Var(\"map_row@ret\") Var(\"%375.1\"))\n"
      "  IndirectIndexedLoad(Var(\"%375.1\") Var(\"PLAYER_COL\") Var(\"%378.1\") false)\n"
      "  JumpIfTrue(Var(\"%378.1\") \".if_end@73\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "  Label(\".if_end@73\")\n"
      "  JumpIfCmp(NotEqual Var(\"%378.1\") Constant(ConstUChar(8)) \".if_end@74\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@74\")\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%385.1\"))\n"
      "  FunctionCall(\"bg_row\" [Var(\"%385.1\")] None false false)\n"
      "  Copy(Var(\"bg_row@ret\") Var(\"%386.1\"))\n"
      "  IndirectIndexedLoad(Var(\"%386.1\") Var(\"PLAYER_COL\") Var(\"%389.1\") false)\n"
      "  JumpIfCmp(Equal Var(\"%389.1\") Constant(ConstUChar(1)) \".or_true@75\")\n"
      "  Binary(BitwiseAnd Var(\"%389.1\") Constant(ConstUChar(254)) Var(\"%389.1.eqm0\"))\n"
      "  JumpIfCmp(Equal Var(\"%389.1.eqm0\") Constant(ConstUChar(2)) \".or_true@75\")\n"
      "  Label(\".stands_on_something@ssa_block@7\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%398.1\"))\n"
      "  Jump(\".or_end@76\")\n"
      "  Label(\".or_true@75\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%398.1\"))\n"
      "  Label(\".or_end@76\")\n"
      "  Ret(Var(\"%398.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%389.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%389.1.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/lode_runner/guards.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@53.sprite\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%216.1\", \"%216.2\", \"%216.3\", \"%217.1\", \"%217.2\", \"%217.3\", \"%218.1\", \"%219.1\", \"%220.1\", \"%221.1\", \"%222.1\", \"%223.1\", \"%224.1\", \"@53.sprite\"]}}}",
      "Function(\"is_floor\" false [\"@53.sprite\"] [\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(1)) \".or_true@27\")\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(2)) \".or_true@27\")\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(3)) \".or_true@27\")\n"
      "  Label(\".is_floor@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%224.1\"))\n"
      "  Jump(\".or_end@28\")\n"
      "  Label(\".or_true@27\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%224.1\"))\n"
      "  Label(\".or_end@28\")\n"
      "  Ret(Var(\"%224.1\"))\n"
      "])\n",
      "Function(\"is_floor\" false [\"@53.sprite\"] [\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(1)) \".or_true@27\")\n"
      "  Binary(BitwiseAnd Var(\"@53.sprite\") Constant(ConstUChar(254)) Var(\"@53.sprite.eqm0\"))\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite.eqm0\") Constant(ConstUChar(2)) \".or_true@27\")\n"
      "  Label(\".is_floor@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%224.1\"))\n"
      "  Jump(\".or_end@28\")\n"
      "  Label(\".or_true@27\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%224.1\"))\n"
      "  Label(\".or_end@28\")\n"
      "  Ret(Var(\"%224.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@53.sprite\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@53.sprite.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/lode_runner/guards.c -S -O (call 2)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@53.sprite\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%216.1\", \"%216.2\", \"%216.3\", \"%217.1\", \"%217.2\", \"%217.3\", \"%218.1\", \"%219.1\", \"%220.1\", \"%221.1\", \"%222.1\", \"%223.1\", \"%224.1\", \"@53.sprite\"]}}}",
      "Function(\"is_floor\" false [\"@53.sprite\"] [\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(1)) \".or_true@27\")\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(2)) \".or_true@27\")\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(3)) \".or_true@27\")\n"
      "  Label(\".is_floor@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%224.1\"))\n"
      "  Jump(\".or_end@28\")\n"
      "  Label(\".or_true@27\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%224.1\"))\n"
      "  Label(\".or_end@28\")\n"
      "  Ret(Var(\"%224.1\"))\n"
      "])\n",
      "Function(\"is_floor\" false [\"@53.sprite\"] [\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite\") Constant(ConstUChar(1)) \".or_true@27\")\n"
      "  Binary(BitwiseAnd Var(\"@53.sprite\") Constant(ConstUChar(254)) Var(\"@53.sprite.eqm0\"))\n"
      "  JumpIfCmp(Equal Var(\"@53.sprite.eqm0\") Constant(ConstUChar(2)) \".or_true@27\")\n"
      "  Label(\".is_floor@ssa_block@4\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%224.1\"))\n"
      "  Jump(\".or_end@28\")\n"
      "  Label(\".or_true@27\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%224.1\"))\n"
      "  Label(\".or_end@28\")\n"
      "  Ret(Var(\"%224.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@53.sprite\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@53.sprite.eqm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/lode_runner/sound.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%14.1\", \"%15.1\", \"%15.2\", \"%15.3\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"@42.pitch\", \"@43.duration\", \"@43.duration.1\", \"@43.duration.2\", \"@43.duration.3\", \"@43.duration.4\", \"@43.duration.5\", \"@44.y\", \"@44.y.1\", \"@44.y.2\", \"@44.y.3\", \"@45.x.1\", \"@45.x.2\", \"@45.x.3\", \"@45.x.4\", \"@45.x.5\"]}}}",
      "Function(\"play_note\" true [\"@42.pitch\" \"@43.duration\" \"@44.y\"] [\n"
      "  Label(\".play_note@ssa_block@0\")\n"
      "  Copy(Var(\"@43.duration\") Var(\"@45.x.2\"))\n"
      "  Copy(Var(\"@44.y\") Var(\"@44.y.1\"))\n"
      "  Copy(Var(\"@43.duration\") Var(\"@43.duration.1\"))\n"
      "  Label(\".loop@4_start\")\n"
      "  JumpIfFalse(Var(\"ENABLE_SOUND\") \".if_end@1\")\n"
      "  IndexedLoad(\"SPKR\" Constant(ConstUChar(0)) Var(\"%14.1\") true 0 0)\n"
      "  Label(\".if_end@1\")\n"
      "  Copy(Var(\"@45.x.2\") Var(\"@45.x.3\"))\n"
      "  Copy(Var(\"@44.y.1\") Var(\"@44.y.2\"))\n"
      "  Copy(Var(\"@43.duration.1\") Var(\"@43.duration.2\"))\n"
      "  Label(\".loop@5_start\")\n"
      "  Binary(Subtract Var(\"@44.y.2\") Constant(ConstUChar(1)) Var(\"%16.1\"))\n"
      "  Copy(Var(\"@43.duration.2\") Var(\"@43.duration.4\"))\n"
      "  JumpIfTrue(Var(\"%16.1\") \".and_false@2\")\n"
      "  Label(\".play_note@ssa_block@2\")\n"
      "  Binary(Subtract Var(\"@43.duration.2\") Constant(ConstUChar(1)) Var(\"%19.1\"))\n"
      "  Copy(Var(\"%19.1\") Var(\"@43.duration.4\"))\n"
      "  JumpIfTrue(Var(\"%19.1\") \".and_false@2\")\n"
      "  Label(\".play_note@ssa_block@3\")\n"
      "  Copy(Var(\"%19.1\") Var(\"@43.duration.5\"))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%15.3\"))\n"
      "  Jump(\".and_end@3\")\n"
      "  Label(\".and_false@2\")\n"
      "  Copy(Var(\"@43.duration.4\") Var(\"@43.duration.5\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%15.3\"))\n"
      "  Label(\".and_end@3\")\n"
      "  JumpIfFalse(Var(\"%15.3\") \".if_end@4\")\n"
      "  Ret(None)\n"
      "  Label(\".if_end@4\")\n"
      "  Label(\".loop@5_continue\")\n"
      "  Binary(Subtract Var(\"@45.x.3\") Constant(ConstUChar(1)) Var(\"%22.1\"))\n"
      "  Copy(Var(\"%22.1\") Var(\"@45.x.3\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@44.y.2\"))\n"
      "  Copy(Var(\"@43.duration.5\") Var(\"@43.duration.2\"))\n"
      "  JumpIfTrue(Var(\"%22.1\") \".loop@5_start\")\n"
      "  Label(\".loop@4_continue\")\n"
      "  Copy(Var(\"@42.pitch\") Var(\"@45.x.2\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@44.y.1\"))\n"
      "  Copy(Var(\"@43.duration.5\") Var(\"@43.duration.1\"))\n"
      "  Jump(\".loop@4_start\")\n"
      "])\n",
      "Function(\"play_note\" true [\"@42.pitch\" \"@43.duration\" \"@44.y\"] [\n"
      "  Label(\".play_note@ssa_block@0\")\n"
      "  Copy(Var(\"@43.duration\") Var(\"@45.x.2\"))\n"
      "  Copy(Var(\"@44.y\") Var(\"@44.y.1\"))\n"
      "  Copy(Var(\"@43.duration\") Var(\"@43.duration.1\"))\n"
      "  Label(\".loop@4_start\")\n"
      "  JumpIfFalse(Var(\"ENABLE_SOUND\") \".if_end@1\")\n"
      "  IndexedLoad(\"SPKR\" Constant(ConstUChar(0)) Var(\"%14.1\") true 0 0)\n"
      "  Label(\".if_end@1\")\n"
      "  Copy(Var(\"@45.x.2\") Var(\"@45.x.3\"))\n"
      "  Copy(Var(\"@44.y.1\") Var(\"@44.y.2\"))\n"
      "  Copy(Var(\"@43.duration.1\") Var(\"@43.duration.2\"))\n"
      "  Label(\".loop@5_start\")\n"
      "  Binary(Subtract Var(\"@44.y.2\") Constant(ConstUChar(1)) Var(\"%16.1\"))\n"
      "  Copy(Var(\"@43.duration.2\") Var(\"@43.duration.4\"))\n"
      "  JumpIfTrue(Var(\"%16.1\") \".play_note@scfold@0\")\n"
      "  Label(\".play_note@ssa_block@2\")\n"
      "  Binary(Subtract Var(\"@43.duration.2\") Constant(ConstUChar(1)) Var(\"%19.1\"))\n"
      "  Copy(Var(\"%19.1\") Var(\"@43.duration.4\"))\n"
      "  JumpIfTrue(Var(\"%19.1\") \".play_note@scfold@0\")\n"
      "  Label(\".play_note@ssa_block@3\")\n"
      "  Copy(Var(\"%19.1\") Var(\"@43.duration.5\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@4\")\n"
      "  Label(\".loop@5_continue\")\n"
      "  Binary(Subtract Var(\"@45.x.3\") Constant(ConstUChar(1)) Var(\"%22.1\"))\n"
      "  Copy(Var(\"%22.1\") Var(\"@45.x.3\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@44.y.2\"))\n"
      "  Copy(Var(\"@43.duration.5\") Var(\"@43.duration.2\"))\n"
      "  JumpIfTrue(Var(\"%22.1\") \".loop@5_start\")\n"
      "  Label(\".loop@4_continue\")\n"
      "  Copy(Var(\"@42.pitch\") Var(\"@45.x.2\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@44.y.1\"))\n"
      "  Copy(Var(\"@43.duration.5\") Var(\"@43.duration.1\"))\n"
      "  Jump(\".loop@4_start\")\n"
      "  Label(\".play_note@scfold@0\")\n"
      "  Copy(Var(\"@43.duration.4\") Var(\"@43.duration.5\"))\n"
      "  Jump(\".if_end@4\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_fold_short_circuit_jump: examples/ali_baba/events.c -S -O (call 2)") {
  c6502::testing::check_stage_case(
      "tac_fold_short_circuit_jump",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%215.1\", \"%216.1\", \"%217.1\", \"%218.1\", \"%219.1\", \"%220.1\", \"%221.1\", \"%222.1\", \"%223.1\", \"%224.1\", \"%225.1\", \"%226.1\", \"%227.1\", \"%228.1\", \"%229.1\", \"%230.1\", \"%230.2\", \"%230.3\", \"%231.1\", \"%232.1\", \"%233.1\", \"%234.1\", \"%235.1\", \"%236.1\", \"@67.r.1\", \"@68.remaining.1\", \"@68.remaining.2\", \"@68.remaining.3\", \"@69.index.1\", \"@69.index.2\", \"@69.index.3\", \"@69.index.4\", \"@69.index.5\", \"@70.loc.1\"]}}}",
      "Function(\"pick_random_mob\" false [] [\n"
      "  JumpIfTrue(Var(\"TOTAL_MOB_COUNT\") \".if_end@22\")\n"
      "  FunctionCall(\"pick_random_char\" [] Var(\"%217.1\") false false)\n"
      "  Ret(Var(\"%217.1\"))\n"
      "  Label(\".if_end@22\")\n"
      "  Label(\".loop@7_start\")\n"
      "  FunctionCall(\"random_in_range\" [Var(\"TOTAL_MOB_COUNT\")] Var(\"%218.1\") false false)\n"
      "  JumpIfFalse(Var(\"%218.1\") \".loop@7_start\")\n"
      "  Label(\".loop@7_break\")\n"
      "  Copy(Var(\"%218.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"@69.index.2\"))\n"
      "  Label(\".loop@8_start\")\n"
      "  FunctionCall(\"get_char_record\" [Var(\"@69.index.2\")] Var(\"%223.1\") false false)\n"
      "  IndirectIndexedLoad(Var(\"%223.1\") Constant(ConstUChar(8)) Var(\"%225.1\") false)\n"
      "  Binary(Subtract Var(\"@68.remaining.2\") Var(\"%225.1\") Var(\"%229.1\"))\n"
      "  Label(\".loop@8_continue\")\n"
      "  Copy(Var(\"@69.index.2\") Var(\"@69.index.4\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%229.1\") Constant(ConstChar(0)) \".and_false@23\")\n"
      "  Label(\".pick_random_mob@ssa_block@2\")\n"
      "  Binary(Add Var(\"@69.index.2\") Constant(ConstUChar(1)) Var(\"%233.1\"))\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.4\"))\n"
      "  JumpIfFalse(Var(\"%233.1\") \".and_false@23\")\n"
      "  Label(\".pick_random_mob@ssa_block@3\")\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.5\"))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%230.3\"))\n"
      "  Jump(\".and_end@24\")\n"
      "  Label(\".and_false@23\")\n"
      "  Copy(Var(\"@69.index.4\") Var(\"@69.index.5\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%230.3\"))\n"
      "  Label(\".and_end@24\")\n"
      "  Copy(Var(\"%229.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Var(\"@69.index.5\") Var(\"@69.index.2\"))\n"
      "  JumpIfTrue(Var(\"%230.3\") \".loop@8_start\")\n"
      "  Copy(Var(\"%229.1\") Var(\"%236.1\"))\n"
      "  IndexedSymbolStore(\"MOB_PTR\" Constant(ConstUChar(0)) Var(\"%236.1\") false 0)\n"
      "  Copy(Var(\"%223.1\") Var(\"CHAR_PTR\"))\n"
      "  Ret(Var(\"@69.index.5\"))\n"
      "])\n",
      "Function(\"pick_random_mob\" false [] [\n"
      "  JumpIfTrue(Var(\"TOTAL_MOB_COUNT\") \".if_end@22\")\n"
      "  FunctionCall(\"pick_random_char\" [] Var(\"%217.1\") false false)\n"
      "  Ret(Var(\"%217.1\"))\n"
      "  Label(\".if_end@22\")\n"
      "  Label(\".loop@7_start\")\n"
      "  FunctionCall(\"random_in_range\" [Var(\"TOTAL_MOB_COUNT\")] Var(\"%218.1\") false false)\n"
      "  JumpIfFalse(Var(\"%218.1\") \".loop@7_start\")\n"
      "  Label(\".loop@7_break\")\n"
      "  Copy(Var(\"%218.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"@69.index.2\"))\n"
      "  Label(\".loop@8_start\")\n"
      "  FunctionCall(\"get_char_record\" [Var(\"@69.index.2\")] Var(\"%223.1\") false false)\n"
      "  IndirectIndexedLoad(Var(\"%223.1\") Constant(ConstUChar(8)) Var(\"%225.1\") false)\n"
      "  Binary(Subtract Var(\"@68.remaining.2\") Var(\"%225.1\") Var(\"%229.1\"))\n"
      "  Label(\".loop@8_continue\")\n"
      "  Copy(Var(\"@69.index.2\") Var(\"@69.index.4\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"%229.1\") Constant(ConstChar(0)) \".pick_random_mob@scfold@0\")\n"
      "  Label(\".pick_random_mob@ssa_block@2\")\n"
      "  Binary(Add Var(\"@69.index.2\") Constant(ConstUChar(1)) Var(\"%233.1\"))\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.4\"))\n"
      "  JumpIfFalse(Var(\"%233.1\") \".pick_random_mob@scfold@0\")\n"
      "  Label(\".pick_random_mob@ssa_block@3\")\n"
      "  Copy(Var(\"%233.1\") Var(\"@69.index.5\"))\n"
      "  Copy(Var(\"%229.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Var(\"@69.index.5\") Var(\"@69.index.2\"))\n"
      "  Jump(\".loop@8_start\")\n"
      "  Label(\".pick_random_mob@scfold@0\")\n"
      "  Copy(Var(\"@69.index.4\") Var(\"@69.index.5\"))\n"
      "  Copy(Var(\"%229.1\") Var(\"@68.remaining.2\"))\n"
      "  Copy(Var(\"@69.index.5\") Var(\"@69.index.2\"))\n"
      "  Copy(Var(\"%229.1\") Var(\"%236.1\"))\n"
      "  IndexedSymbolStore(\"MOB_PTR\" Constant(ConstUChar(0)) Var(\"%236.1\") false 0)\n"
      "  Copy(Var(\"%223.1\") Var(\"CHAR_PTR\"))\n"
      "  Ret(Var(\"@69.index.5\"))\n"
      "])\n",
      "");
}
