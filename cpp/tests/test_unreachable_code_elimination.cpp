// The recorded calls of `tac_eliminate_unreachable_code`, the C++ port
// of passes/optimization/unreachable_code_elimination.py, with what the
// Python pass returned. They were recorded with the capture harness's
// wrappers while tests/test_unreachable_code_elimination.py ran,
// through the TAC optimizer's driver and through the Python entry
// points that the tests call directly. They are every call of an entry
// point on the tests' synthetic TAC and every call through the driver
// that changed the function.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_eliminate_unreachable_code: test_cse.TestCseSim.test_repeated_subscripts_and_tests (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.k\", \"@1.n\"]}}}",
      "Function(\"cmp\" true [\"@0.k\" \"@1.n\"] [\n"
      "  Label(\".cmp@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.k\") Var(\"%0.1\"))\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%1.1\"))\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  ZeroExtend(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.k\") Var(\"%4.1\") \".if_end@0\")\n"
      "  Label(\".cmp@ssa_block@1\")\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  ZeroExtend(Var(\"@0.k\") Var(\"%8.1\"))\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"%9.1\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Load(Var(\"%11.1\") Var(\"%12.1\") false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"@0.k\") Var(\"%12.1\") \".if_end@1\")\n"
      "  Label(\".cmp@ssa_block@2\")\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Constant(ConstInt(2)) Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Load(Var(\"%17.1\") Var(\"%18.1\") false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"%20.1\") Constant(ConstInt(3)) Var(\"%21.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Load(Var(\"%22.1\") Var(\"%23.1\") false)\n"
      "  ZeroExtend(Var(\"%23.1\") Var(\"%24.1\"))\n"
      "  Binary(Add Var(\"%19.1\") Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Ret(Var(\"%25.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "Function(\"cmp\" true [\"@0.k\" \"@1.n\"] [\n"
      "  ZeroExtend(Var(\"@0.k\") Var(\"%0.1\"))\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%1.1\"))\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  ZeroExtend(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.k\") Var(\"%4.1\") \".if_end@0\")\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  ZeroExtend(Var(\"@0.k\") Var(\"%8.1\"))\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"%9.1\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Load(Var(\"%11.1\") Var(\"%12.1\") false)\n"
      "  ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"@0.k\") Var(\"%12.1\") \".if_end@1\")\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Constant(ConstInt(2)) Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Load(Var(\"%17.1\") Var(\"%18.1\") false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"%20.1\") Constant(ConstInt(3)) Var(\"%21.1\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Load(Var(\"%22.1\") Var(\"%23.1\") false)\n"
      "  ZeroExtend(Var(\"%23.1\") Var(\"%24.1\"))\n"
      "  Binary(Add Var(\"%19.1\") Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Ret(Var(\"%25.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\"]}}}",
      "Function(\"next\" true [] [\n"
      "  Label(\".next@ssa_block@0\")\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"P\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"next\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"P\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessJumps.test_drops_jump_if_true_to_next_block (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Var(\"c\") \"L1\")\n"
      "  Label(\"L1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_copy_folding.TestCopyFoldingEndToEnd.test_static_counter_collapses_to_inc_chain (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Add Var(\"counter\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"counter\"))\n"
      "  Ret(Var(\"counter\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Binary(Add Var(\"counter\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"counter\"))\n"
      "  Ret(Var(\"counter\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessLabels.test_drops_label_with_no_jump_references (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Label(\"unreferenced\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_cse.TestCseSim.test_doubles (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%3.1\", \"%5.1\", \"%7.1\", \"@0.d\"]}}}",
      "Function(\"f\" true [\"@0.d\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Subtract Var(\"@0.d\") Constant(ConstDouble(4607182418800017408)) Var(\"%1.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%1.1\") Constant(ConstDouble(4617315517961601024)) \".if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Binary(Subtract Var(\"@0.d\") Constant(ConstDouble(4611686018427387904)) Var(\"%5.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%5.1\") Constant(ConstDouble(4616189618054758400)) \".if_end@1\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"f\" true [\"@0.d\"] [\n"
      "  Binary(Subtract Var(\"@0.d\") Constant(ConstDouble(4607182418800017408)) Var(\"%1.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%1.1\") Constant(ConstDouble(4617315517961601024)) \".if_end@0\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@0\")\n"
      "  Binary(Subtract Var(\"@0.d\") Constant(ConstDouble(4611686018427387904)) Var(\"%5.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%5.1\") Constant(ConstDouble(4616189618054758400)) \".if_end@1\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessLabels.test_keeps_label_targeted_by_a_jump (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Label(\"head\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  JumpIfTrue(Var(\"c\") \"head\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\"head\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  JumpIfTrue(Var(\"c\") \"head\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestPreservedAttributesAndShape.test_idempotence (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
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

TEST_CASE("tac_eliminate_unreachable_code: test_copy_folding.TestCopyFoldingEndToEnd.test_loop_counter_int_collapses_to_inc_chain (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.s.1\", \"@0.s.2\", \"@0.s.3\", \"@1.i.1\", \"@1.i.2\", \"@1.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@0.s.1\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@1.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.s.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.s.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.s.3\"))])\n"
      "  Phi(Var(\"@1.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.i.3\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Constant(ConstInt(10)) \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Binary(Add Var(\"@0.s.2\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"@0.s.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"@1.i.2\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"@1.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.s.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@0.s.1\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@1.i.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.s.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.s.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.s.3\"))])\n"
      "  Phi(Var(\"@1.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.i.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.i.3\"))])\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Constant(ConstInt(10)) \".loop@0_break\")\n"
      "  Binary(Add Var(\"@0.s.2\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"@0.s.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@1.i.2\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"@1.i.2\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"@1.i.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.s.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUnreachableBlocks.test_drops_code_after_ret (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_reassoc_offset_licm.TestReassocLicmSim.test_pointer_advanced_by_byte_in_loop (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@4.i.1\", \"@4.i.2\", \"@4.i.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@4.i.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@4.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.i.1\")) PhiArg(\".loop@1_continue\" Var(\"@4.i.3\"))])\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%8.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.i.2\") Constant(ConstUChar(40)) \".loop@1_break\")\n"
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
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@4.i.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@4.i.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.i.1\")) PhiArg(\".loop@1_continue\" Var(\"@4.i.3\"))])\n"
      "  ZeroExtend(Var(\"@4.i.2\") Var(\"%8.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.i.2\") Constant(ConstUChar(40)) \".loop@1_break\")\n"
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

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_found_and_not_found (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%11.1\", \"%11.2\", \"%11.3\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%48.1\", \"%49.1\", \"@2.v\", \"@5.v.1\", \"@6.i.1\", \"@6.i.2\", \"@6.i.3\"]}}}",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"%47.1\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Copy(Var(\"@6.i.2\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Jump(\".f@jthread@0\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Label(\".i0_loop@0_break\")\n"
      "  Jump(\".if_end@1\")\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%11.3\") [])\n"
      "  JumpIfFalse(Var(\"%11.3\") \".if_end@1\")\n"
      "  Label(\".f@jthread@0\")\n"
      "  Ret(Constant(ConstUChar(10)))\n"
      "  Label(\".if_end@1\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(2)) Var(\"hits\"))\n"
      "  Ret(Constant(ConstUChar(20)))\n"
      "])\n",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"%47.1\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Copy(Var(\"@6.i.2\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@2.v\") \".i0_if_end@0\")\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"hits\"))\n"
      "  Jump(\".f@jthread@0\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
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

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestPreservedAttributesAndShape.test_idempotence (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L1\")\n"
      "  Copy(Constant(ConstInt(99)) Var(\"y\"))\n"
      "  Label(\"L1\")\n"
      "  JumpIfTrue(Var(\"c\") \"L2\")\n"
      "  Label(\"L2\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessJumps.test_drops_jump_if_false_to_next_block (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"c\") \"L1\")\n"
      "  Label(\"L1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessJumps.test_keeps_jump_to_non_adjacent_target (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"c\") \"L_skip\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L_after\")\n"
      "  Label(\"L_skip\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"z\"))\n"
      "  Label(\"L_after\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"c\") \"L_skip\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L_after\")\n"
      "  Label(\"L_skip\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"z\"))\n"
      "  Label(\"L_after\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_reassoc_offset_licm.TestReassocLicmSim.test_header_fields_after_variable_skip (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%21.1\", \"%22.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"t\") Var(\"%21.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%21.1\")] Var(\"%22.1\") false false)\n"
      "  Ret(Var(\"%22.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"t\") Var(\"%21.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%21.1\")] Var(\"%22.1\") false false)\n"
      "  Ret(Var(\"%22.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_found_and_not_found (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(21)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(21)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\"]}}}",
      "Function(\"nextq\" true [] [\n"
      "  Label(\".nextq@ssa_block@0\")\n"
      "  Binary(Add Var(\"Q\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"Q\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"Q\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Truncate(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "Function(\"nextq\" true [] [\n"
      "  Binary(Add Var(\"Q\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"Q\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"Q\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Truncate(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_and_or_results (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%29.2\", \"%29.3\", \"%29.4\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"%40.2\", \"%40.3\", \"%41.1\", \"%41.2\", \"%41.3\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%47.2\", \"%47.3\", \"%48.1\", \"%49.1\", \"@0.r.1\", \"@0.r.2\", \"@0.r.3\", \"@4.k.1\", \"@4.k.2\", \"@4.k.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@0.r.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%16.1\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@4.k.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.r.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.r.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.r.3\"))])\n"
      "  Phi(Var(\"@4.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.k.1\")) PhiArg(\".loop@0_continue\" Var(\"@4.k.3\"))])\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.k.2\") Constant(ConstUChar(5)) \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%19.1\"))\n"
      "  Truncate(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"%21.1\") false 0 0)\n"
      "  Copy(Var(\"%21.1\") Var(\"x\"))\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"%24.1\") false 0 0)\n"
      "  Copy(Var(\"%24.1\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%25.1\"))\n"
      "  Truncate(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"%27.1\") false 0 0)\n"
      "  Copy(Var(\"%27.1\") Var(\"z\"))\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  ZeroExtend(Var(\"x\") Var(\"%34.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@2\")\n"
      "  ZeroExtend(Var(\"y\") Var(\"%36.1\"))\n"
      "  JumpIfTrue(Var(\"y\") \".i0_or_true@2\")\n"
      "  Label(\".main@ssa_block@3\")\n"
      "  ZeroExtend(Var(\"z\") Var(\"%38.1\"))\n"
      "  JumpIfTrue(Var(\"z\") \".i0_or_true@2\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%40.1\"))\n"
      "  Jump(\".i0_or_end@3\")\n"
      "  Label(\".i0_or_true@2\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%40.2\"))\n"
      "  Label(\".i0_or_end@3\")\n"
      "  Phi(Var(\"%40.3\") [PhiArg(\".main@ssa_block@4\" Var(\"%40.1\")) PhiArg(\".i0_or_true@2\" Var(\"%40.2\"))])\n"
      "  JumpIfFalse(Var(\"%40.3\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%41.1\"))\n"
      "  Jump(\".i0_and_end@1\")\n"
      "  Label(\".i0_and_false@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%41.2\"))\n"
      "  Label(\".i0_and_end@1\")\n"
      "  Phi(Var(\"%41.3\") [PhiArg(\".main@ssa_block@5\" Var(\"%41.1\")) PhiArg(\".i0_and_false@0\" Var(\"%41.2\"))])\n"
      "  JumpIfFalse(Var(\"%41.3\") \".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%42.1\"))\n"
      "  Copy(Var(\"%42.1\") Var(\"%29.1\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  ZeroExtend(Var(\"x\") Var(\"%43.1\"))\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".i0_or_true@5\")\n"
      "  Label(\".main@ssa_block@7\")\n"
      "  ZeroExtend(Var(\"y\") Var(\"%45.1\"))\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".i0_or_true@5\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%47.1\"))\n"
      "  Jump(\".i0_or_end@6\")\n"
      "  Label(\".i0_or_true@5\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%47.2\"))\n"
      "  Label(\".i0_or_end@6\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@8\" Var(\"%47.1\")) PhiArg(\".i0_or_true@5\" Var(\"%47.2\"))])\n"
      "  JumpIfFalse(Var(\"%47.3\") \".i0_if_end@7\")\n"
      "  Label(\".main@ssa_block@9\")\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%48.1\"))\n"
      "  Copy(Var(\"%48.1\") Var(\"%29.2\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@7\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%49.1\"))\n"
      "  Copy(Var(\"%49.1\") Var(\"%29.3\"))\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%29.4\") [PhiArg(\".main@ssa_block@6\" Var(\"%29.1\")) PhiArg(\".main@ssa_block@9\" Var(\"%29.2\")) PhiArg(\".i0_if_end@7\" Var(\"%29.3\"))])\n"
      "  ZeroExtend(Var(\"%29.4\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"%28.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Copy(Var(\"%31.1\") Var(\"@0.r.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"@4.k.2\") Constant(ConstUChar(1)) Var(\"%33.1\"))\n"
      "  Copy(Var(\"%33.1\") Var(\"@4.k.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.r.2\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@0.r.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%16.1\"))\n"
      "  Copy(Var(\"%16.1\") Var(\"@4.k.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.r.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.r.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.r.3\"))])\n"
      "  Phi(Var(\"@4.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@4.k.1\")) PhiArg(\".loop@0_continue\" Var(\"@4.k.3\"))])\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.k.2\") Constant(ConstUChar(5)) \".loop@0_break\")\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%19.1\"))\n"
      "  Truncate(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"%21.1\") false 0 0)\n"
      "  Copy(Var(\"%21.1\") Var(\"x\"))\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"%24.1\") false 0 0)\n"
      "  Copy(Var(\"%24.1\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%25.1\"))\n"
      "  Truncate(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"%27.1\") false 0 0)\n"
      "  Copy(Var(\"%27.1\") Var(\"z\"))\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  ZeroExtend(Var(\"x\") Var(\"%34.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_and_false@0\")\n"
      "  ZeroExtend(Var(\"y\") Var(\"%36.1\"))\n"
      "  JumpIfTrue(Var(\"y\") \".i0_or_true@2\")\n"
      "  ZeroExtend(Var(\"z\") Var(\"%38.1\"))\n"
      "  JumpIfTrue(Var(\"z\") \".i0_or_true@2\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%40.1\"))\n"
      "  Jump(\".i0_or_end@3\")\n"
      "  Label(\".i0_or_true@2\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%40.2\"))\n"
      "  Label(\".i0_or_end@3\")\n"
      "  Phi(Var(\"%40.3\") [PhiArg(\".main@ssa_block@4\" Var(\"%40.1\")) PhiArg(\".i0_or_true@2\" Var(\"%40.2\"))])\n"
      "  JumpIfFalse(Var(\"%40.3\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%41.1\"))\n"
      "  Jump(\".i0_and_end@1\")\n"
      "  Label(\".i0_and_false@0\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%41.2\"))\n"
      "  Label(\".i0_and_end@1\")\n"
      "  Phi(Var(\"%41.3\") [PhiArg(\".main@ssa_block@5\" Var(\"%41.1\")) PhiArg(\".i0_and_false@0\" Var(\"%41.2\"))])\n"
      "  JumpIfFalse(Var(\"%41.3\") \".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%42.1\"))\n"
      "  Copy(Var(\"%42.1\") Var(\"%29.1\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  ZeroExtend(Var(\"x\") Var(\"%43.1\"))\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".i0_or_true@5\")\n"
      "  ZeroExtend(Var(\"y\") Var(\"%45.1\"))\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".i0_or_true@5\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"%47.1\"))\n"
      "  Jump(\".i0_or_end@6\")\n"
      "  Label(\".i0_or_true@5\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"%47.2\"))\n"
      "  Label(\".i0_or_end@6\")\n"
      "  Phi(Var(\"%47.3\") [PhiArg(\".main@ssa_block@8\" Var(\"%47.1\")) PhiArg(\".i0_or_true@5\" Var(\"%47.2\"))])\n"
      "  JumpIfFalse(Var(\"%47.3\") \".i0_if_end@7\")\n"
      "  Label(\".main@ssa_block@9\")\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%48.1\"))\n"
      "  Copy(Var(\"%48.1\") Var(\"%29.2\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@7\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%49.1\"))\n"
      "  Copy(Var(\"%49.1\") Var(\"%29.3\"))\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%29.4\") [PhiArg(\".main@ssa_block@6\" Var(\"%29.1\")) PhiArg(\".main@ssa_block@9\" Var(\"%29.2\")) PhiArg(\".i0_if_end@7\" Var(\"%29.3\"))])\n"
      "  ZeroExtend(Var(\"%29.4\") Var(\"%30.1\"))\n"
      "  Binary(Add Var(\"%28.1\") Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  Copy(Var(\"%31.1\") Var(\"@0.r.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"@4.k.2\") Constant(ConstUChar(1)) Var(\"%33.1\"))\n"
      "  Copy(Var(\"%33.1\") Var(\"@4.k.3\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.r.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_cse.TestCseSim.test_store_between_loads (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"t\") Var(\"%10.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%10.1\")] Var(\"%11.1\") false false)\n"
      "  Ret(Var(\"%11.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"t\") Var(\"%10.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%10.1\")] Var(\"%11.1\") false false)\n"
      "  Ret(Var(\"%11.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_reassoc_offset_licm.TestReassocLicmSim.test_header_fields_after_variable_skip (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%13.1.sr0\", \"%13.1.sr1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.d\", \"@1.n.1\"]}}}",
      "Function(\"f\" true [\"@0.d\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Add Var(\"@0.d\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Copy(Var(\"%1.1\") Var(\"@1.n.1\"))\n"
      "  ZeroExtend(Var(\"@1.n.1\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"%2.1\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Multiply Var(\"%6.1\") Constant(ConstInt(100)) Var(\"%7.1\"))\n"
      "  ZeroExtend(Var(\"@1.n.1\") Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%8.1\") Constant(ConstInt(2)) Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "  Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  ZeroExtend(Var(\"@1.n.1\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Constant(ConstInt(3)) Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Load(Var(\"%17.1\") Var(\"%18.1\") false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.d\"] [\n"
      "  Binary(Add Var(\"@0.d\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Copy(Var(\"%1.1\") Var(\"@1.n.1\"))\n"
      "  ZeroExtend(Var(\"@1.n.1\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"%2.1\") Constant(ConstInt(1)) Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(Multiply Var(\"%6.1\") Constant(ConstInt(100)) Var(\"%7.1\"))\n"
      "  ZeroExtend(Var(\"@1.n.1\") Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%8.1\") Constant(ConstInt(2)) Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "  Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  ZeroExtend(Var(\"@1.n.1\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Constant(ConstInt(3)) Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"@0.d\") Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Load(Var(\"%17.1\") Var(\"%18.1\") false)\n"
      "  ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_cse.TestCseSim.test_repeated_subscripts_and_tests (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%26.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%27.1\"))\n"
      "  FunctionCall(\"cmp\" [Var(\"%26.1\") Var(\"%27.1\")] Var(\"%28.1\") false false)\n"
      "  Binary(Multiply Var(\"%28.1\") Constant(ConstInt(100)) Var(\"%29.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%30.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%31.1\"))\n"
      "  FunctionCall(\"cmp\" [Var(\"%30.1\") Var(\"%31.1\")] Var(\"%32.1\") false false)\n"
      "  Binary(Add Var(\"%29.1\") Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%34.1\"))\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%35.1\"))\n"
      "  FunctionCall(\"cmp\" [Var(\"%34.1\") Var(\"%35.1\")] Var(\"%36.1\") false false)\n"
      "  Binary(Multiply Var(\"%36.1\") Constant(ConstInt(1000)) Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%33.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%26.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%27.1\"))\n"
      "  FunctionCall(\"cmp\" [Var(\"%26.1\") Var(\"%27.1\")] Var(\"%28.1\") false false)\n"
      "  Binary(Multiply Var(\"%28.1\") Constant(ConstInt(100)) Var(\"%29.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%30.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%31.1\"))\n"
      "  FunctionCall(\"cmp\" [Var(\"%30.1\") Var(\"%31.1\")] Var(\"%32.1\") false false)\n"
      "  Binary(Add Var(\"%29.1\") Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%34.1\"))\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%35.1\"))\n"
      "  FunctionCall(\"cmp\" [Var(\"%34.1\") Var(\"%35.1\")] Var(\"%36.1\") false false)\n"
      "  Binary(Multiply Var(\"%36.1\") Constant(ConstInt(1000)) Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%33.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_cse.TestCseSim.test_store_between_loads (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%5.1.sr0\", \"%5.1.sr1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.p\", \"@1.a.1\"]}}}",
      "Function(\"f\" true [\"@0.p\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"@1.a.1\"))\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Store(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  Binary(LeftShift Var(\"@1.a.1\") Constant(ConstInt(1)) Var(\"%5.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%5.1.sr0\") Constant(ConstInt(2)) Var(\"%5.1.sr1\"))\n"
      "  Binary(Add Var(\"%5.1.sr1\") Var(\"%5.1.sr0\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  Load(Var(\"%6.1\") Var(\"%7.1\") false)\n"
      "  ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.p\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"@1.a.1\"))\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  Store(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  Binary(LeftShift Var(\"@1.a.1\") Constant(ConstInt(1)) Var(\"%5.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%5.1.sr0\") Constant(ConstInt(2)) Var(\"%5.1.sr1\"))\n"
      "  Binary(Add Var(\"%5.1.sr1\") Var(\"%5.1.sr0\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  Load(Var(\"%6.1\") Var(\"%7.1\") false)\n"
      "  ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 3)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%16.1\", \"%16.1.sr0\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%9.1\", \"@0.sum.1\", \"@0.sum.2\", \"@0.sum.3\", \"@1.k.1\", \"@1.k.2\", \"@1.k.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"T\") Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"P\"))\n"
      "  GetAddress(Var(\"T\") Var(\"%10.1\"))\n"
      "  Copy(Var(\"%10.1\") Var(\"Q\"))\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"@0.sum.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%12.1\"))\n"
      "  Copy(Var(\"%12.1\") Var(\"@1.k.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.sum.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.sum.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.sum.3\"))])\n"
      "  Phi(Var(\"@1.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.k.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.k.3\"))])\n"
      "  ZeroExtend(Var(\"@1.k.2\") Var(\"%13.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@1.k.2\") Constant(ConstUChar(7)) \".loop@0_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
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
      "  Copy(Var(\"%24.1\") Var(\"@0.sum.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@1.k.2\") Var(\"%25.1\"))\n"
      "  Binary(Add Var(\"@1.k.2\") Constant(ConstUChar(1)) Var(\"%26.1\"))\n"
      "  Copy(Var(\"%26.1\") Var(\"@1.k.3\"))\n"
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
      "  GetAddress(Var(\"T\") Var(\"%9.1\"))\n"
      "  Copy(Var(\"%9.1\") Var(\"P\"))\n"
      "  GetAddress(Var(\"T\") Var(\"%10.1\"))\n"
      "  Copy(Var(\"%10.1\") Var(\"Q\"))\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"@0.sum.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%12.1\"))\n"
      "  Copy(Var(\"%12.1\") Var(\"@1.k.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.sum.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.sum.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.sum.3\"))])\n"
      "  Phi(Var(\"@1.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@1.k.1\")) PhiArg(\".loop@0_continue\" Var(\"@1.k.3\"))])\n"
      "  ZeroExtend(Var(\"@1.k.2\") Var(\"%13.1\"))\n"
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
      "  Copy(Var(\"%24.1\") Var(\"@0.sum.3\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Copy(Var(\"@1.k.2\") Var(\"%25.1\"))\n"
      "  Binary(Add Var(\"@1.k.2\") Constant(ConstUChar(1)) Var(\"%26.1\"))\n"
      "  Copy(Var(\"%26.1\") Var(\"@1.k.3\"))\n"
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
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_found_and_not_found (call 3)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(9)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestPreservedAttributesAndShape.test_straight_line_code_unchanged (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessJumps.test_drops_unconditional_jump_to_next_block (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L1\")\n"
      "  Label(\"L1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_found_and_not_found (call 4)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUnreachableBlocks.test_drops_code_after_jump_with_no_label (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"L1\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Label(\"L1\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUnreachableBlocks.test_drops_chain_reachable_only_from_dead_code (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Label(\"dead_entry\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"dead_tail\")\n"
      "  Label(\"dead_tail\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUnreachableBlocks.test_keeps_block_reached_via_jump (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Jump(\"L1\")\n"
      "  Copy(Constant(ConstInt(99)) Var(\"y\"))\n"
      "  Label(\"L1\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_found_and_not_found (call 5)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%11.1\", \"%11.2\", \"%11.3\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%39.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%48.1\", \"%49.1\", \"@2.v\", \"@5.v.1\", \"@6.i.1\", \"@6.i.2\", \"@6.i.3\"]}}}",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"@2.v\") Var(\"@5.v.1\"))\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"@6.i.3\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Truncate(Var(\"%39.1\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%41.1\") Var(\"%42.1\"))\n"
      "  ZeroExtend(Var(\"@5.v.1\") Var(\"%43.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@5.v.1\") \".i0_if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Var(\"hits\") Var(\"%45.1\"))\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"%46.1\"))\n"
      "  Copy(Var(\"%46.1\") Var(\"hits\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"@6.i.3\"))\n"
      "  ZeroExtend(Var(\"%47.1\") Var(\"%48.1\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Label(\".i0_loop@0_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%11.2\"))\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%11.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%11.1\")) PhiArg(\".i0_loop@0_break\" Var(\"%11.2\"))])\n"
      "  JumpIfFalse(Var(\"%11.3\") \".if_end@1\")\n"
      "  Label(\".f@ssa_block@2\")\n"
      "  Copy(Constant(ConstUChar(10)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  Truncate(Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"hits\"))\n"
      "  Copy(Constant(ConstUChar(20)) Var(\"%16.1\"))\n"
      "  Ret(Var(\"%16.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@2.v\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"@2.v\") Var(\"@5.v.1\"))\n"
      "  Copy(Var(\"n\") Var(\"@6.i.1\"))\n"
      "  Label(\".i0_loop@0_start\")\n"
      "  Phi(Var(\"@6.i.2\") [PhiArg(\".f@ssa_block@0\" Var(\"@6.i.1\")) PhiArg(\".i0_loop@0_continue\" Var(\"@6.i.3\"))])\n"
      "  ZeroExtend(Var(\"@6.i.2\") Var(\"%39.1\"))\n"
      "  Truncate(Var(\"%39.1\") Var(\"%40.1\"))\n"
      "  IndexedLoad(\"a\" Var(\"%40.1\") Var(\"%41.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%41.1\") Var(\"%42.1\"))\n"
      "  ZeroExtend(Var(\"@5.v.1\") Var(\"%43.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%41.1\") Var(\"@5.v.1\") \".i0_if_end@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Copy(Var(\"hits\") Var(\"%45.1\"))\n"
      "  Binary(Add Var(\"hits\") Constant(ConstUChar(1)) Var(\"%46.1\"))\n"
      "  Copy(Var(\"%46.1\") Var(\"hits\"))\n"
      "  Copy(Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@0\")\n"
      "  Label(\".i0_loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@6.i.2\") Constant(ConstUChar(1)) Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"@6.i.3\"))\n"
      "  ZeroExtend(Var(\"%47.1\") Var(\"%48.1\"))\n"
      "  JumpIfTrue(Var(\"%47.1\") \".i0_loop@0_start\")\n"
      "  Label(\".i0_loop@0_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%11.2\"))\n"
      "  Label(\".i0@end\")\n"
      "  Phi(Var(\"%11.3\") [PhiArg(\".f@ssa_block@1\" Var(\"%11.1\")) PhiArg(\".i0_loop@0_break\" Var(\"%11.2\"))])\n"
      "  JumpIfFalse(Var(\"%11.3\") \".if_end@1\")\n"
      "  Copy(Constant(ConstUChar(10)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%13.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Constant(ConstInt(2)) Var(\"%14.1\"))\n"
      "  Truncate(Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Copy(Var(\"%15.1\") Var(\"hits\"))\n"
      "  Copy(Constant(ConstUChar(20)) Var(\"%16.1\"))\n"
      "  Ret(Var(\"%16.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_and_or_results (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%29.2\", \"%29.3\", \"%29.4\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"%40.2\", \"%40.3\", \"%41.1\", \"%41.2\", \"%41.3\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%47.2\", \"%47.3\", \"%48.1\", \"%49.1\", \"@0.r.1\", \"@0.r.2\", \"@0.r.3\", \"@4.k.1\", \"@4.k.2\", \"@4.k.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.r.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstInt(0))) PhiArg(\".loop@0_continue\" Var(\"%31.1\"))])\n"
      "  Phi(Var(\"@4.k.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%33.1\"))])\n"
      "  ZeroExtend(Var(\"@4.k.2\") Var(\"%17.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@4.k.2\") Constant(ConstUChar(5)) \".loop@0_break\")\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"x\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"y\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"z\") false 0 0)\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_if_end@4\")\n"
      "  JumpIfTrue(Var(\"y\") \".main@ssa_block@6\")\n"
      "  JumpIfTrue(Var(\"z\") \".main@ssa_block@6\")\n"
      "  Label(\".main@ssa_block@4\")\n"
      "  Jump(\".i0_if_end@4\")\n"
      "  Label(\".i0_or_true@2\")\n"
      "  Label(\".i0_or_end@3\")\n"
      "  Phi(Var(\"%40.3\") [])\n"
      "  JumpIfFalse(Var(\"%40.3\") \".i0_and_false@0\")\n"
      "  Label(\".main@ssa_block@5\")\n"
      "  Jump(\".i0_and_end@1\")\n"
      "  Label(\".i0_and_false@0\")\n"
      "  Label(\".i0_and_end@1\")\n"
      "  Phi(Var(\"%41.3\") [])\n"
      "  JumpIfFalse(Var(\"%41.3\") \".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".main@ssa_block@9\")\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".main@ssa_block@9\")\n"
      "  Label(\".main@ssa_block@8\")\n"
      "  Jump(\".i0_if_end@7\")\n"
      "  Label(\".i0_or_true@5\")\n"
      "  Label(\".i0_or_end@6\")\n"
      "  Phi(Var(\"%47.3\") [])\n"
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
      "  Copy(Var(\"@4.k.2\") Var(\"%20.1\"))\n"
      "  IndexedLoad(\"@1.xs\" Var(\"%20.1\") Var(\"x\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%23.1\"))\n"
      "  IndexedLoad(\"@2.ys\" Var(\"%23.1\") Var(\"y\") false 0 0)\n"
      "  Copy(Var(\"@4.k.2\") Var(\"%26.1\"))\n"
      "  IndexedLoad(\"@3.zs\" Var(\"%26.1\") Var(\"z\") false 0 0)\n"
      "  Binary(LeftShift Var(\"@0.r.2\") Constant(ConstInt(3)) Var(\"%28.1\"))\n"
      "  JumpIfTrue(Var(\"x\") \".i0_if_end@4\")\n"
      "  JumpIfTrue(Var(\"y\") \".main@ssa_block@6\")\n"
      "  JumpIfTrue(Var(\"z\") \".main@ssa_block@6\")\n"
      "  Jump(\".i0_if_end@4\")\n"
      "  Label(\".main@ssa_block@6\")\n"
      "  Jump(\".i0@end\")\n"
      "  Label(\".i0_if_end@4\")\n"
      "  JumpIfCmp(Equal Var(\"x\") Constant(ConstUChar(5)) \".main@ssa_block@9\")\n"
      "  JumpIfCmp(Equal Var(\"y\") Constant(ConstUChar(6)) \".main@ssa_block@9\")\n"
      "  Jump(\".i0_if_end@7\")\n"
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

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessJumps.test_keeps_ret_terminator (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(7)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Constant(ConstInt(7)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestUselessLabels.test_drops_label_whose_only_reference_was_in_dead_code (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Jump(\"cleanup\")\n"
      "  Label(\"cleanup\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_cse.TestCseSim.test_doubles (call 2)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%8.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstDouble(4618441417868443648))] Var(\"%8.1\") false false)\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstDouble(4618441417868443648))] Var(\"%8.1\") false false)\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestInputUnchanged.test_pruning_leaves_the_input (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Var(\"c\") \"a\")\n"
      "  JumpIfFalse(Var(\"e\") \"p\")\n"
      "  JumpIfTrue(Var(\"d\") \"q\")\n"
      "  Label(\"p\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Label(\"a\")\n"
      "  Jump(\"join\")\n"
      "  Label(\"q\")\n"
      "  Label(\"join\")\n"
      "  Phi(Var(\"x\") [PhiArg(\"a\" Constant(ConstInt(1))) PhiArg(\"q\" Constant(ConstInt(1))) PhiArg(\"p\" Constant(ConstInt(2)))])\n"
      "  Ret(Var(\"x\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  JumpIfTrue(Var(\"c\") \"a\")\n"
      "  JumpIfFalse(Var(\"e\") \"p\")\n"
      "  JumpIfTrue(Var(\"d\") \"q\")\n"
      "  Label(\"p\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Label(\"a\")\n"
      "  Jump(\"join\")\n"
      "  Label(\"q\")\n"
      "  Label(\"join\")\n"
      "  Phi(Var(\"x\") [PhiArg(\"a\" Constant(ConstInt(1))) PhiArg(\"q\" Constant(ConstInt(1)))])\n"
      "  Ret(Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_jump_thread.TestPrograms.test_found_and_not_found (call 6)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.sr0\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\", \"@4.r.1\", \"@4.r.2\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%17.1\"))\n"
      "  Copy(Var(\"%17.1\") Var(\"n\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%18.1\"))\n"
      "  Copy(Var(\"%18.1\") Var(\"@3.k.1\"))\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@3.k.1\")) PhiArg(\".loop@1_continue\" Var(\"@3.k.3\"))])\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%19.1\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@3.k.2\") Constant(ConstUChar(8)) \".loop@1_break\")\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"%21.1\") Constant(ConstInt(1)) Var(\"%22.1.sr0\"))\n"
      "  Binary(Add Var(\"%22.1.sr0\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  ZeroExtend(Var(\"@3.k.2\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"a\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".loop@1_continue\")\n"
      "  Copy(Var(\"@3.k.2\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Copy(Var(\"%27.1\") Var(\"@3.k.3\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%28.1\"))\n"
      "  Copy(Var(\"%28.1\") Var(\"@4.r.1\"))\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%29.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%29.1\")] Var(\"%30.1\") false false)\n"
      "  ZeroExtend(Var(\"%30.1\") Var(\"%31.1\"))\n"
      "  ZeroExtend(Var(\"@4.r.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Var(\"%31.1\") Var(\"%33.1\"))\n"
      "  Truncate(Var(\"%33.1\") Var(\"%34.1\"))\n"
      "  Copy(Var(\"%34.1\") Var(\"@4.r.2\"))\n"
      "  ZeroExtend(Var(\"@4.r.2\") Var(\"%35.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(8)) Var(\"%36.1\"))\n"
      "  ZeroExtend(Var(\"hits\") Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"%36.1\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_eliminate_unreachable_code: test_unreachable_code_elimination.TestPreservedAttributesAndShape.test_preserves_name_linkage_params (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_eliminate_unreachable_code",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"foo\" true [\"a\" \"b\"] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"foo\" true [\"a\" \"b\"] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}
