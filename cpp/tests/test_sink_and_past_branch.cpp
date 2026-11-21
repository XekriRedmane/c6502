// The recorded calls of `tac_sink_and_past_branch`, the C++ port of
// passes/optimization/sink_and_past_branch.py, with what the Python
// pass returned. They were recorded with the capture harness's wrappers
// while tests/test_sink_and_past_branch.py ran, through the TAC
// optimizer's driver and through the Python entry points that the tests
// call directly. They are the calls that changed the function or the
// symbols, and the calls of small synthetic functions through those
// entry points. The cases after them are the calls of the whole corpus
// (tools/capture_passes.py) that changed the function, since few corpus
// calls reach the rewrite.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchEligibility.test_use_past_merge_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".else\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".else\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Copy(Var(\"%t\") Var(\"%u_post\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".else\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".else\")\n"
      "  Jump(\".end\")\n"
      "  Label(\".end\")\n"
      "  Copy(Var(\"%t\") Var(\"%u_post\"))\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchKeepsSharedDefs.test_shared_zero_extend_sim (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"step\" [Constant(ConstUChar(0))] None false false)\n"
      "  FunctionCall(\"step\" [Constant(ConstUChar(1))] None false false)\n"
      "  ZeroExtend(Var(\"y\") Var(\"%19.1\"))\n"
      "  Binary(LeftShift Var(\"%19.1\") Constant(ConstInt(8)) Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"g16\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Ret(Var(\"%21.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"step\" [Constant(ConstUChar(0))] None false false)\n"
      "  FunctionCall(\"step\" [Constant(ConstUChar(1))] None false false)\n"
      "  ZeroExtend(Var(\"y\") Var(\"%19.1\"))\n"
      "  Binary(LeftShift Var(\"%19.1\") Constant(ConstInt(8)) Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"g16\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Ret(Var(\"%21.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchEligibility.test_non_uchar_x_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_load_reads_correct_byte (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%3.1\", \"%4.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%3.1\"))\n"
      "  FunctionCall(\"load\" [Var(\"%3.1\") Constant(ConstUChar(5))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%3.1\"))\n"
      "  FunctionCall(\"load\" [Var(\"%3.1\") Constant(ConstUChar(5))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_read_and_write_end_to_end (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.p\"]}}}",
      "Function(\"f\" true [\"@0.p\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  Store(Var(\"%2.1\") Var(\"%0.1\") false)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(2)) Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  ZeroExtend(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(3)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Store(Var(\"%7.1\") Var(\"%3.1\") false)\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(3)) Var(\"%8.1\"))\n"
      "  Load(Var(\"%8.1\") Var(\"%9.1\") false)\n"
      "  Binary(Subtract Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%10.1\"))\n"
      "  Store(Var(\"%10.1\") Var(\"%8.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@0.p\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  Store(Var(\"%2.1\") Var(\"%0.1\") false)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(2)) Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  ZeroExtend(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(3)) Var(\"%6.1\"))\n"
      "  Truncate(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Store(Var(\"%7.1\") Var(\"%3.1\") false)\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(3)) Var(\"%8.1\"))\n"
      "  Load(Var(\"%8.1\") Var(\"%9.1\") false)\n"
      "  Binary(Subtract Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%10.1\"))\n"
      "  Store(Var(\"%10.1\") Var(\"%8.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.ra\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"@3.line\", \"@4.x\", \"@5.m\", \"@6.background.1\"]}}}",
      "Function(\"open\" true [\"@3.line\" \"@4.x\" \"@5.m\"] [\n"
      "  ZeroExtend(Var(\"@4.x\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"@3.line\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Load(Var(\"%12.1\") Var(\"%13.1\") false)\n"
      "  Binary(Add Var(\"%11.1\") Var(\"@3.line\") Var(\"%15.1.ra\"))\n"
      "  Binary(Add Var(\"%15.1.ra\") Constant(ConstInt(8192)) Var(\"%15.1\"))\n"
      "  Load(Var(\"%15.1\") Var(\"%16.1\") false)\n"
      "  ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  ZeroExtend(Var(\"@5.m\") Var(\"%18.1\"))\n"
      "  Copy(Var(\"@5.m\") Var(\"%19.1\"))\n"
      "  IndexedLoad(\"mask\" Var(\"%19.1\") Var(\"%20.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%17.1\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  ZeroExtend(Var(\"%13.1\") Var(\"%23.1\"))\n"
      "  Binary(BitwiseOr Var(\"%23.1\") Var(\"%22.1\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Store(Var(\"%25.1\") Var(\"%12.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"open\" true [\"@3.line\" \"@4.x\" \"@5.m\"] [\n"
      "  ZeroExtend(Var(\"@4.x\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"@3.line\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Load(Var(\"%12.1\") Var(\"%13.1\") false)\n"
      "  Binary(Add Var(\"%11.1\") Var(\"@3.line\") Var(\"%15.1.ra\"))\n"
      "  Binary(Add Var(\"%15.1.ra\") Constant(ConstInt(8192)) Var(\"%15.1\"))\n"
      "  Load(Var(\"%15.1\") Var(\"%16.1\") false)\n"
      "  ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  ZeroExtend(Var(\"@5.m\") Var(\"%18.1\"))\n"
      "  Copy(Var(\"@5.m\") Var(\"%19.1\"))\n"
      "  IndexedLoad(\"mask\" Var(\"%19.1\") Var(\"%20.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%17.1\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  ZeroExtend(Var(\"%13.1\") Var(\"%23.1\"))\n"
      "  Binary(BitwiseOr Var(\"%23.1\") Var(\"%22.1\") Var(\"%24.1\"))\n"
      "  Truncate(Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Store(Var(\"%25.1\") Var(\"%12.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_negative_signed_char_index (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@5.r.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(4)) Var(\"%6.1\"))\n"
      "  FunctionCall(\"ld\" [Var(\"%6.1\") Constant(ConstChar(-1))] Var(\"%9.1\") false false)\n"
      "  GetAddress(Var(\"data\") Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"%10.1\") Constant(ConstInt(4)) Var(\"%11.1\"))\n"
      "  FunctionCall(\"st\" [Var(\"%11.1\") Constant(ConstChar(-2)) Constant(ConstUChar(99))] None false false)\n"
      "  ZeroExtend(Var(\"%9.1\") Var(\"%15.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(2)) Var(\"%16.1\") false 0 0)\n"
      "  Binary(Equal Var(\"%16.1\") Constant(ConstUChar(99)) Var(\"%18.1\"))\n"
      "  Binary(Multiply Var(\"%18.1\") Constant(ConstInt(100)) Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(4)) Var(\"%6.1\"))\n"
      "  FunctionCall(\"ld\" [Var(\"%6.1\") Constant(ConstChar(-1))] Var(\"%9.1\") false false)\n"
      "  GetAddress(Var(\"data\") Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"%10.1\") Constant(ConstInt(4)) Var(\"%11.1\"))\n"
      "  FunctionCall(\"st\" [Var(\"%11.1\") Constant(ConstChar(-2)) Constant(ConstUChar(99))] None false false)\n"
      "  ZeroExtend(Var(\"%9.1\") Var(\"%15.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(2)) Var(\"%16.1\") false 0 0)\n"
      "  Binary(Equal Var(\"%16.1\") Constant(ConstUChar(99)) Var(\"%18.1\"))\n"
      "  Binary(Multiply Var(\"%18.1\") Constant(ConstInt(100)) Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchKeepsSharedDefs.test_shared_zero_extend_sim (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.i\", \"@1.b.1\", \"@2.w.1\", \"@3.mag.1\"]}}}",
      "Function(\"step\" true [\"@0.i\"] [\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"@0.i\") Var(\"%1.1\"))\n"
      "  IndexedLoad(\"bob\" Var(\"%1.1\") Var(\"%2.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1\") Constant(ConstInt(127)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  JumpIfMasked(Var(\"%2.1\") 128 false \".if_else@1\")\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%9.1\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%10.1\"))\n"
      "  Binary(Subtract Var(\"%10.1\") Var(\"%9.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"y\"))\n"
      "  Jump(\".if_end@0\")\n"
      "  Label(\".if_else@1\")\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%13.1\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%13.1\") Var(\"%15.1\"))\n"
      "  Truncate(Var(\"%15.1\") Var(\"y\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Var(\"%3.1\") Var(\"g16\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"step\" true [\"@0.i\"] [\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"@0.i\") Var(\"%1.1\"))\n"
      "  IndexedLoad(\"bob\" Var(\"%1.1\") Var(\"%2.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  JumpIfMasked(Var(\"%2.1\") 128 false \".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_then@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_then@0\") Constant(ConstInt(127)) Var(\"%5.1.snk_then@0\"))\n"
      "  Truncate(Var(\"%5.1.snk_then@0\") Var(\"%6.1.snk_then@0\"))\n"
      "  ZeroExtend(Var(\"%6.1.snk_then@0\") Var(\"%9.1\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%10.1\"))\n"
      "  Binary(Subtract Var(\"%10.1\") Var(\"%9.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"y\"))\n"
      "  Jump(\".if_end@0\")\n"
      "  Label(\".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_else@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_else@0\") Constant(ConstInt(127)) Var(\"%5.1.snk_else@0\"))\n"
      "  Truncate(Var(\"%5.1.snk_else@0\") Var(\"%6.1.snk_else@0\"))\n"
      "  ZeroExtend(Var(\"%6.1.snk_else@0\") Var(\"%13.1\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%13.1\") Var(\"%15.1\"))\n"
      "  Truncate(Var(\"%15.1\") Var(\"y\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Var(\"%3.1\") Var(\"g16\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": "
      "\"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.ra\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.n0\", \"%23.1\", \"%24.1\", \"%24.1.lo2\", \"%25.1\", \"@3.line\", \"@4.x\", \"@5.m\", \"@6.background.1\"]}}}",
      "Function(\"open\" true [\"@3.line\" \"@4.x\" \"@5.m\"] [\n"
      "  ZeroExtend(Var(\"@4.x\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"@3.line\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Load(Var(\"%12.1\") Var(\"%13.1\") false)\n"
      "  Binary(Add Var(\"%12.1\") Constant(ConstInt(8192)) Var(\"%15.1\"))\n"
      "  Load(Var(\"%15.1\") Var(\"%16.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@5.m\") Var(\"%20.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%16.1\") Var(\"%20.1\") Var(\"%22.1.n0\"))\n"
      "  ZeroExtend(Var(\"%22.1.n0\") Var(\"%22.1\"))\n"
      "  Copy(Var(\"%22.1.n0\") Var(\"%24.1.lo2\"))\n"
      "  Binary(BitwiseOr Var(\"%13.1\") Var(\"%24.1.lo2\") Var(\"%25.1\"))\n"
      "  Store(Var(\"%25.1\") Var(\"%12.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"open\" true [\"@3.line\" \"@4.x\" \"@5.m\"] [\n"
      "  ZeroExtend(Var(\"@4.x\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"@3.line\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Load(Var(\"%12.1\") Var(\"%13.1\") false)\n"
      "  Binary(Add Var(\"%12.1\") Constant(ConstInt(8192)) Var(\"%15.1\"))\n"
      "  Load(Var(\"%15.1\") Var(\"%16.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@5.m\") Var(\"%20.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%16.1\") Var(\"%20.1\") Var(\"%22.1.n0\"))\n"
      "  ZeroExtend(Var(\"%22.1.n0\") Var(\"%22.1\"))\n"
      "  Copy(Var(\"%22.1.n0\") Var(\"%24.1.lo2\"))\n"
      "  Binary(BitwiseOr Var(\"%13.1\") Var(\"%24.1.lo2\") Var(\"%25.1\"))\n"
      "  Store(Var(\"%25.1\") Var(\"%12.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_store_writes_correct_byte (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.p\", \"@1.i\", \"@2.v\"]}}}",
      "Function(\"store\" true [\"@0.p\" \"@1.i\" \"@2.v\"] [\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Store(Var(\"@2.v\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"store\" true [\"@0.p\" \"@1.i\" \"@2.v\"] [\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Store(Var(\"@2.v\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%10.1.ra\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.j\", \"@1.i.1\"]}}}",
      "Function(\"word\" true [\"@0.j\"] [\n"
      "  Binary(LeftShift Var(\"@0.j\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(LeftShift Var(\"%6.1\") Constant(ConstInt(8)) Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%10.1.ra\"))\n"
      "  Binary(Add Var(\"%10.1.ra\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%13.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Ret(Var(\"%14.1\"))\n"
      "])\n",
      "Function(\"word\" true [\"@0.j\"] [\n"
      "  Binary(LeftShift Var(\"@0.j\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(LeftShift Var(\"%6.1\") Constant(ConstInt(8)) Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%10.1.ra\"))\n"
      "  Binary(Add Var(\"%10.1.ra\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%13.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Ret(Var(\"%14.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_src_y_postincrement_chain (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"copy3\" [Constant(ConstUChar(1))] None false false)\n"
      "  IndexedLoad(\"out\" Constant(ConstUChar(0)) Var(\"%14.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  IndexedLoad(\"out\" Constant(ConstUChar(1)) Var(\"%16.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(LeftShift Var(\"%17.1\") Constant(ConstInt(4)) Var(\"%18.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Ret(Var(\"%19.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"copy3\" [Constant(ConstUChar(1))] None false false)\n"
      "  IndexedLoad(\"out\" Constant(ConstUChar(0)) Var(\"%14.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  IndexedLoad(\"out\" Constant(ConstUChar(1)) Var(\"%16.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(LeftShift Var(\"%17.1\") Constant(ConstInt(4)) Var(\"%18.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%18.1\") Var(\"%19.1\"))\n"
      "  Ret(Var(\"%19.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_const_offset_indexed_store (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%3.1\", \"%4.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(119)) Constant(ConstUChar(3))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(119)) Constant(ConstUChar(3))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_load_reads_correct_byte (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.p\", \"@1.i\"]}}}",
      "Function(\"load\" true [\"@0.p\" \"@1.i\"] [\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"load\" true [\"@0.p\" \"@1.i\"] [\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchKeepsSharedDefs.test_shared_zero_extend_stays (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"%e\"))\n"
      "  Binary(BitwiseAnd Var(\"%e\") Constant(ConstInt(127)) Var(\"%a\"))\n"
      "  Truncate(Var(\"%a\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"b\") 128 false \".else\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Jump(\".end\")\n"
      "  Label(\".else\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Label(\".end\")\n"
      "  Copy(Var(\"%e\") Var(\"w\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"b\") Var(\"%e\"))\n"
      "  JumpIfMasked(Var(\"b\") 128 false \".else\")\n"
      "  ZeroExtend(Var(\"b\") Var(\"%e.snk_then@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%e.snk_then@0\") Constant(ConstInt(127)) Var(\"%a.snk_then@0\"))\n"
      "  Truncate(Var(\"%a.snk_then@0\") Var(\"%t.snk_then@0\"))\n"
      "  Copy(Var(\"%t.snk_then@0\") Var(\"%u\"))\n"
      "  Jump(\".end\")\n"
      "  Label(\".else\")\n"
      "  ZeroExtend(Var(\"b\") Var(\"%e.snk_else@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%e.snk_else@0\") Constant(ConstInt(127)) Var(\"%a.snk_else@0\"))\n"
      "  Truncate(Var(\"%a.snk_else@0\") Var(\"%t.snk_else@0\"))\n"
      "  Copy(Var(\"%t.snk_else@0\") Var(\"%u\"))\n"
      "  Label(\".end\")\n"
      "  Copy(Var(\"%e\") Var(\"w\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%e\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%e.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%a.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%e.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%a.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"at"
      "trs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_canonical_indexed_store_collapses (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.value\", \"@1.col\"]}}}",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16675 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16675 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%44.1\", \"%45.1\", \"%46.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"table\"))\n"
      "  GetAddress(Var(\"data\") Var(\"%29.1\"))\n"
      "  Binary(Add Var(\"%29.1\") Constant(ConstInt(254)) Var(\"%30.1\"))\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(254)) Constant(ConstUChar(18)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Constant(ConstInt(255)) Var(\"%33.1\"))\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(255)) Constant(ConstUChar(52)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%35.1\"))\n"
      "  Binary(Add Var(\"%35.1\") Constant(ConstInt(256)) Var(\"%36.1\"))\n"
      "  Store(Constant(ConstUChar(86)) Var(\"%36.1\") false)\n"
      "  FunctionCall(\"word\" [Constant(ConstUChar(127))] Var(\"%38.1\") false false)\n"
      "  Binary(Equal Var(\"%38.1\") Constant(ConstUInt(4660)) Var(\"%40.1\"))\n"
      "  FunctionCall(\"odd\" [Constant(ConstUChar(255))] Var(\"%42.1\") false false)\n"
      "  Binary(Equal Var(\"%42.1\") Constant(ConstUInt(13398)) Var(\"%44.1\"))\n"
      "  Binary(LeftShift Var(\"%44.1\") Constant(ConstInt(1)) Var(\"%45.1\"))\n"
      "  Binary(Add Var(\"%40.1\") Var(\"%45.1\") Var(\"%46.1\"))\n"
      "  Ret(Var(\"%46.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"table\"))\n"
      "  GetAddress(Var(\"data\") Var(\"%29.1\"))\n"
      "  Binary(Add Var(\"%29.1\") Constant(ConstInt(254)) Var(\"%30.1\"))\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(254)) Constant(ConstUChar(18)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"%32.1\") Constant(ConstInt(255)) Var(\"%33.1\"))\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(255)) Constant(ConstUChar(52)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%35.1\"))\n"
      "  Binary(Add Var(\"%35.1\") Constant(ConstInt(256)) Var(\"%36.1\"))\n"
      "  Store(Constant(ConstUChar(86)) Var(\"%36.1\") false)\n"
      "  FunctionCall(\"word\" [Constant(ConstUChar(127))] Var(\"%38.1\") false false)\n"
      "  Binary(Equal Var(\"%38.1\") Constant(ConstUInt(4660)) Var(\"%40.1\"))\n"
      "  FunctionCall(\"odd\" [Constant(ConstUChar(255))] Var(\"%42.1\") false false)\n"
      "  Binary(Equal Var(\"%42.1\") Constant(ConstUInt(13398)) Var(\"%44.1\"))\n"
      "  Binary(LeftShift Var(\"%44.1\") Constant(ConstInt(1)) Var(\"%45.1\"))\n"
      "  Binary(Add Var(\"%40.1\") Var(\"%45.1\") Var(\"%46.1\"))\n"
      "  Ret(Var(\"%46.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.c\", \"@1.x.1\", \"@1.x.2\", \"@1.x.3\"]}}}",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  IndexedLoad(\"tab\" Var(\"@1.x.2\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"@1.x.2\") Constant(ConstUChar(10)) Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  IndexedLoad(\"tab\" Var(\"@1.x.2\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"@1.x.2\") Constant(ConstUChar(10)) Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_unoptimized_still_works (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%3.1\", \"%4.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%3.1\"))\n"
      "  FunctionCall(\"load\" [Var(\"%3.1\") Constant(ConstUChar(4))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%3.1\"))\n"
      "  FunctionCall(\"load\" [Var(\"%3.1\") Constant(ConstUChar(4))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_offset_indexed_store_lands_correctly (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.value\", \"@1.col\"]}}}",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16584 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16584 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_negative_signed_char_index (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%3.1\", \"%4.1\", \"@2.p\", \"@3.i\", \"@4.v\"]}}}",
      "Function(\"st\" true [\"@2.p\" \"@3.i\" \"@4.v\"] [\n"
      "  SignExtend(Var(\"@3.i\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@2.p\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Store(Var(\"@4.v\") Var(\"%4.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"st\" true [\"@2.p\" \"@3.i\" \"@4.v\"] [\n"
      "  SignExtend(Var(\"@3.i\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@2.p\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Store(Var(\"@4.v\") Var(\"%4.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedAsmShape.test_indirect_indexed_load_emits_short_sequence (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": []}}}",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_post_decrement_becomes_dex (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.c\", \"@1.x.1\", \"@1.x.2\", \"@1.x.3\"]}}}",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  IndexedLoad(\"tab\" Var(\"@1.x.2\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"@1.x.2\") Constant(ConstUChar(8)) Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  IndexedLoad(\"tab\" Var(\"@1.x.2\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"@1.x.2\") Constant(ConstUChar(8)) Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_access_end_to_end (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.p\", \"@1.q\"]}}}",
      "Function(\"f\" true [\"@0.p\" \"@1.q\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  IndexedSymbolStore(\"a\" Constant(ConstUChar(0)) Var(\"%1.1\") true 0)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(255)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  IndexedSymbolStore(\"b\" Constant(ConstUChar(0)) Var(\"%3.1\") true 0)\n"
      "  Binary(Add Var(\"@1.q\") Constant(ConstInt(7)) Var(\"%6.1\"))\n"
      "  Store(Constant(ConstChar(-122)) Var(\"%6.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@0.p\" \"@1.q\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  IndexedSymbolStore(\"a\" Constant(ConstUChar(0)) Var(\"%1.1\") true 0)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(255)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  IndexedSymbolStore(\"b\" Constant(ConstUChar(0)) Var(\"%3.1\") true 0)\n"
      "  Binary(Add Var(\"@1.q\") Constant(ConstInt(7)) Var(\"%6.1\"))\n"
      "  Store(Constant(ConstChar(-122)) Var(\"%6.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchEligibility.test_out_of_range_mask_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(511)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(511)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_negative_signed_char_index (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.value\", \"@1.col\"]}}}",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  SignExtend(Var(\"@1.col\") Var(\"%0.1\"))\n"
      "  Binary(Add Constant(ConstUInt(16896)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Store(Var(\"@0.value\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  SignExtend(Var(\"@1.col\") Var(\"%0.1\"))\n"
      "  Binary(Add Constant(ConstUInt(16896)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Store(Var(\"@0.value\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchMatching.test_apply_bobble_shape_sinks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"bobble\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_ext\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_and\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"bobble\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"bobble\") 128 false \".else\")\n"
      "  Copy(Var(\"%t\") Var(\"%use_then\"))\n"
      "  Jump(\".end\")\n"
      "  Label(\".else\")\n"
      "  Copy(Var(\"%t\") Var(\"%use_else\"))\n"
      "  Label(\".end\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  JumpIfMasked(Var(\"bobble\") 128 false \".else\")\n"
      "  ZeroExtend(Var(\"bobble\") Var(\"%x_ext.snk_then@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext.snk_then@0\") Constant(ConstInt(127)) Var(\"%x_and.snk_then@0\"))\n"
      "  Truncate(Var(\"%x_and.snk_then@0\") Var(\"%t.snk_then@0\"))\n"
      "  Copy(Var(\"%t.snk_then@0\") Var(\"%use_then\"))\n"
      "  Jump(\".end\")\n"
      "  Label(\".else\")\n"
      "  ZeroExtend(Var(\"bobble\") Var(\"%x_ext.snk_else@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext.snk_else@0\") Constant(ConstInt(127)) Var(\"%x_and.snk_else@0\"))\n"
      "  Truncate(Var(\"%x_and.snk_else@0\") Var(\"%t.snk_else@0\"))\n"
      "  Copy(Var(\"%t.snk_else@0\") Var(\"%use_else\"))\n"
      "  Label(\".end\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"bobble\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_ext\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_and\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_ext.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_and.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_ext.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%x_and.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99"
      "_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%10.1.ra\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.j\", \"@1.i.1\"]}}}",
      "Function(\"word\" true [\"@0.j\"] [\n"
      "  ZeroExtend(Var(\"@0.j\") Var(\"%0.1\"))\n"
      "  Binary(LeftShift Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(LeftShift Var(\"%6.1\") Constant(ConstInt(8)) Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%10.1.ra\"))\n"
      "  Binary(Add Var(\"%10.1.ra\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%13.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Ret(Var(\"%14.1\"))\n"
      "])\n",
      "Function(\"word\" true [\"@0.j\"] [\n"
      "  ZeroExtend(Var(\"@0.j\") Var(\"%0.1\"))\n"
      "  Binary(LeftShift Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(LeftShift Var(\"%6.1\") Constant(ConstInt(8)) Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%10.1.ra\"))\n"
      "  Binary(Add Var(\"%10.1.ra\") Constant(ConstInt(1)) Var(\"%10.1\"))\n"
      "  Load(Var(\"%10.1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%13.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Ret(Var(\"%14.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%40.1\", \"%42.1\", \"%43.1\", \"%44.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"find\" [Constant(ConstUChar(5))] Var(\"%29.1\") false false)\n"
      "  ZeroExtend(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "  FunctionCall(\"find\" [Constant(ConstUChar(1))] Var(\"%32.1\") false false)\n"
      "  ZeroExtend(Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  Binary(Multiply Var(\"%33.1\") Constant(ConstInt(100)) Var(\"%34.1\"))\n"
      "  Binary(Add Var(\"%30.1\") Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  FunctionCall(\"sdown\" [Constant(ConstChar(4))] Var(\"%37.1\") false false)\n"
      "  SignExtend(Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Binary(Multiply Var(\"%38.1\") Constant(ConstInt(1000)) Var(\"%39.1\"))\n"
      "  Binary(Add Var(\"%35.1\") Var(\"%39.1\") Var(\"%40.1\"))\n"
      "  FunctionCall(\"wdown\" [Constant(ConstUInt(300))] Var(\"%43.1\") false false)\n"
      "  Binary(Add Var(\"%40.1\") Var(\"%43.1\") Var(\"%44.1\"))\n"
      "  Ret(Var(\"%44.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"find\" [Constant(ConstUChar(5))] Var(\"%29.1\") false false)\n"
      "  ZeroExtend(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "  FunctionCall(\"find\" [Constant(ConstUChar(1))] Var(\"%32.1\") false false)\n"
      "  ZeroExtend(Var(\"%32.1\") Var(\"%33.1\"))\n"
      "  Binary(Multiply Var(\"%33.1\") Constant(ConstInt(100)) Var(\"%34.1\"))\n"
      "  Binary(Add Var(\"%30.1\") Var(\"%34.1\") Var(\"%35.1\"))\n"
      "  FunctionCall(\"sdown\" [Constant(ConstChar(4))] Var(\"%37.1\") false false)\n"
      "  SignExtend(Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Binary(Multiply Var(\"%38.1\") Constant(ConstInt(1000)) Var(\"%39.1\"))\n"
      "  Binary(Add Var(\"%35.1\") Var(\"%39.1\") Var(\"%40.1\"))\n"
      "  FunctionCall(\"wdown\" [Constant(ConstUInt(300))] Var(\"%43.1\") false false)\n"
      "  Binary(Add Var(\"%40.1\") Var(\"%43.1\") Var(\"%44.1\"))\n"
      "  Ret(Var(\"%44.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_read_and_write_end_to_end (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.p\"]}}}",
      "Function(\"f\" true [\"@0.p\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  Store(Var(\"%2.1\") Var(\"%0.1\") false)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(2)) Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  Binary(Add Var(\"%4.1\") Constant(ConstUChar(3)) Var(\"%7.1\"))\n"
      "  Store(Var(\"%7.1\") Var(\"%3.1\") false)\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(3)) Var(\"%8.1\"))\n"
      "  Load(Var(\"%8.1\") Var(\"%9.1\") false)\n"
      "  Binary(Subtract Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%10.1\"))\n"
      "  Store(Var(\"%10.1\") Var(\"%8.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@0.p\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  Store(Var(\"%2.1\") Var(\"%0.1\") false)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(2)) Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  Binary(Add Var(\"%4.1\") Constant(ConstUChar(3)) Var(\"%7.1\"))\n"
      "  Store(Var(\"%7.1\") Var(\"%3.1\") false)\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(3)) Var(\"%8.1\"))\n"
      "  Load(Var(\"%8.1\") Var(\"%9.1\") false)\n"
      "  Binary(Subtract Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%10.1\"))\n"
      "  Store(Var(\"%10.1\") Var(\"%8.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_byte_lands_at_indexed_address (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.value\", \"@1.col\"]}}}",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16640 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16640 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_unoptimized_still_works (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.value\", \"@1.col\"]}}}",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16896 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16896 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedAsmShape.test_indirect_indexed_load_emits_short_sequence (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.src\", \"@1.idx\"]}}}",
      "Function(\"copy\" true [\"@0.src\" \"@1.idx\"] [\n"
      "  ZeroExtend(Var(\"@1.idx\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.src\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"copy\" true [\"@0.src\" \"@1.idx\"] [\n"
      "  ZeroExtend(Var(\"@1.idx\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.src\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_negative_signed_char_index (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.p\", \"@1.i\"]}}}",
      "Function(\"ld\" true [\"@0.p\" \"@1.i\"] [\n"
      "  SignExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"ld\" true [\"@0.p\" \"@1.i\"] [\n"
      "  SignExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_int_index_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.value\", \"@1.col\"]}}}",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  Binary(Add Constant(ConstUInt(16384)) Var(\"@1.col\") Var(\"%0.1\"))\n"
      "  Store(Var(\"@0.value\") Var(\"%0.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  Binary(Add Constant(ConstUInt(16384)) Var(\"@1.col\") Var(\"%0.1\"))\n"
      "  Store(Var(\"@0.value\") Var(\"%0.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_src_y_postincrement_chain (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.y\", \"@0.y.1\", \"@0.y.2\"]}}}",
      "Function(\"copy3\" true [\"@0.y\"] [\n"
      "  IndexedLoad(\"tab\" Var(\"@0.y\") Var(\"%4.1\") false 0 0)\n"
      "  Binary(Add Var(\"@0.y\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"%4.1\") false 0)\n"
      "  IndexedLoad(\"tab\" Var(\"%1.1\") Var(\"%9.1\") false 0 0)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Var(\"%9.1\") false 0)\n"
      "  IndexedLoad(\"tab\" Var(\"%6.1\") Var(\"%12.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(2)) Var(\"%12.1\") false 0)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"copy3\" true [\"@0.y\"] [\n"
      "  IndexedLoad(\"tab\" Var(\"@0.y\") Var(\"%4.1\") false 0 0)\n"
      "  Binary(Add Var(\"@0.y\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"%4.1\") false 0)\n"
      "  IndexedLoad(\"tab\" Var(\"%1.1\") Var(\"%9.1\") false 0 0)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Var(\"%9.1\") false 0)\n"
      "  IndexedLoad(\"tab\" Var(\"%6.1\") Var(\"%12.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(2)) Var(\"%12.1\") false 0)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.line\", \"@1.x\", \"@2.m\"]}}}",
      "Function(\"close\" true [\"@0.line\" \"@1.x\" \"@2.m\"] [\n"
      "  ZeroExtend(Var(\"@1.x\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.line\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@2.m\") Var(\"%5.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%2.1\") Var(\"%5.1\") Var(\"%9.1\"))\n"
      "  Store(Var(\"%9.1\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"close\" true [\"@0.line\" \"@1.x\" \"@2.m\"] [\n"
      "  ZeroExtend(Var(\"@1.x\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.line\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@2.m\") Var(\"%5.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%2.1\") Var(\"%5.1\") Var(\"%9.1\"))\n"
      "  Store(Var(\"%9.1\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"@2.n\", \"@2.n.1\", \"@2.n.2\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\"]}}}",
      "Function(\"sdown\" true [\"@2.n\"] [\n"
      "  Label(\".sdown@ssa_block@0\")\n"
      "  Label(\".loop@1_continue\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".sdown@ssa_block@0\" Constant(ConstChar(0))) PhiArg(\".sdown@ssa_block@1\" Var(\"%22.1\"))])\n"
      "  Phi(Var(\"@2.n.1\") [PhiArg(\".sdown@ssa_block@0\" Var(\"@2.n\")) PhiArg(\".sdown@ssa_block@1\" Var(\"%17.1\"))])\n"
      "  SignExtend(Var(\"@2.n.1\") Var(\"%18.1\"))\n"
      "  Binary(Subtract Var(\"@2.n.1\") Constant(ConstChar(1)) Var(\"%17.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%18.1\") Constant(ConstInt(-3)) \".loop@1_break\")\n"
      "  Label(\".sdown@ssa_block@1\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstChar(1)) Var(\"%22.1\"))\n"
      "  Jump(\".loop@1_continue\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Ret(Var(\"@3.k.2\"))\n"
      "])\n",
      "Function(\"sdown\" true [\"@2.n\"] [\n"
      "  Label(\".sdown@ssa_block@0\")\n"
      "  Label(\".loop@1_continue\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".sdown@ssa_block@0\" Constant(ConstChar(0))) PhiArg(\".sdown@ssa_block@1\" Var(\"%22.1\"))])\n"
      "  Phi(Var(\"@2.n.1\") [PhiArg(\".sdown@ssa_block@0\" Var(\"@2.n\")) PhiArg(\".sdown@ssa_block@1\" Var(\"%17.1\"))])\n"
      "  SignExtend(Var(\"@2.n.1\") Var(\"%18.1\"))\n"
      "  Binary(Subtract Var(\"@2.n.1\") Constant(ConstChar(1)) Var(\"%17.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%18.1\") Constant(ConstInt(-3)) \".loop@1_break\")\n"
      "  Label(\".sdown@ssa_block@1\")\n"
      "  Binary(Add Var(\"@3.k.2\") Constant(ConstChar(1)) Var(\"%22.1\"))\n"
      "  Jump(\".loop@1_continue\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Ret(Var(\"@3.k.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_read_and_write_end_to_end (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%17.1.sr0\", \"%17.1.sr1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"gp\"))\n"
      "  GetAddress(Var(\"data\") Var(\"%12.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%12.1\")] None false false)\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(0)) Var(\"%13.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(1)) Var(\"%15.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Binary(LeftShift Var(\"%16.1\") Constant(ConstInt(1)) Var(\"%17.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%17.1.sr0\") Constant(ConstInt(2)) Var(\"%17.1.sr1\"))\n"
      "  Binary(Add Var(\"%17.1.sr1\") Var(\"%17.1.sr0\") Var(\"%17.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(2)) Var(\"%19.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(100)) Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(Add Var(\"%18.1\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(3)) Var(\"%23.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%23.1\") Var(\"%24.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(1000)) Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Binary(Add Var(\"%22.1\") Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"gp\"))\n"
      "  GetAddress(Var(\"data\") Var(\"%12.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%12.1\")] None false false)\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(0)) Var(\"%13.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(1)) Var(\"%15.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Binary(LeftShift Var(\"%16.1\") Constant(ConstInt(1)) Var(\"%17.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%17.1.sr0\") Constant(ConstInt(2)) Var(\"%17.1.sr1\"))\n"
      "  Binary(Add Var(\"%17.1.sr1\") Var(\"%17.1.sr0\") Var(\"%17.1\"))\n"
      "  Binary(Add Var(\"%14.1\") Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(2)) Var(\"%19.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(100)) Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(Add Var(\"%18.1\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(3)) Var(\"%23.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%23.1\") Var(\"%24.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(1000)) Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Binary(Add Var(\"%22.1\") Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchNoSymbols.test_no_symbols_passes_through (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%e\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".L\")\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%e\"))\n"
      "  JumpIfMasked(Var(\"x\") 128 false \".L\")\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_src_y_postincrement_chain (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.y\", \"@0.y.1\", \"@0.y.2\"]}}}",
      "Function(\"copy3\" true [\"@0.y\"] [\n"
      "  ZeroExtend(Var(\"@0.y\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"@0.y\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@0.y\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%3.1\") Var(\"%4.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"%4.1\") false 0)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%7.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%8.1\") Var(\"%9.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Var(\"%9.1\") false 0)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%10.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"%11.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%11.1\") Var(\"%12.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(2)) Var(\"%12.1\") false 0)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"copy3\" true [\"@0.y\"] [\n"
      "  ZeroExtend(Var(\"@0.y\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"@0.y\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"@0.y\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%3.1\") Var(\"%4.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"%4.1\") false 0)\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%7.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"%8.1\"))\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%8.1\") Var(\"%9.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Var(\"%9.1\") false 0)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%10.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"%11.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%11.1\") Var(\"%12.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(2)) Var(\"%12.1\") false 0)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchEligibility.test_no_jumpifmasked_terminator_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  Ret(Var(\"%t\"))\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  Ret(Var(\"%t\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_static_written_before_last_read (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.t.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"s\") Constant(ConstUInt(1)) Var(\"%1.1\"))\n"
      "  Copy(Constant(ConstUInt(5)) Var(\"s\"))\n"
      "  Copy(Var(\"s\") Var(\"g\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"h\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"s\") Constant(ConstUInt(1)) Var(\"%1.1\"))\n"
      "  Copy(Constant(ConstUInt(5)) Var(\"s\"))\n"
      "  Copy(Var(\"s\") Var(\"g\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"h\"))\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 4)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.c\", \"@1.x.1\", \"@1.x.2\", \"@1.x.3\"]}}}",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  ZeroExtend(Var(\"@1.x.2\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"@1.x.2\") Var(\"%2.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(10)) Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  ZeroExtend(Var(\"@1.x.2\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"@1.x.2\") Var(\"%2.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(10)) Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 4)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"@7.x\", \"@8.v.1\"]}}}",
      "Function(\"open_global\" true [\"@7.x\"] [\n"
      "  ZeroExtend(Var(\"@7.x\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%27.1\"))\n"
      "  Load(Var(\"%27.1\") Var(\"%28.1\") false)\n"
      "  GetAddress(Var(\"screen\") Var(\"row\"))\n"
      "  ZeroExtend(Var(\"%28.1\") Var(\"%30.1\"))\n"
      "  Binary(BitwiseOr Var(\"%30.1\") Constant(ConstInt(1)) Var(\"%31.1\"))\n"
      "  Truncate(Var(\"%31.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%34.1\"))\n"
      "  Store(Var(\"%32.1\") Var(\"%34.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"open_global\" true [\"@7.x\"] [\n"
      "  ZeroExtend(Var(\"@7.x\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%27.1\"))\n"
      "  Load(Var(\"%27.1\") Var(\"%28.1\") false)\n"
      "  GetAddress(Var(\"screen\") Var(\"row\"))\n"
      "  ZeroExtend(Var(\"%28.1\") Var(\"%30.1\"))\n"
      "  Binary(BitwiseOr Var(\"%30.1\") Constant(ConstInt(1)) Var(\"%31.1\"))\n"
      "  Truncate(Var(\"%31.1\") Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%34.1\"))\n"
      "  Store(Var(\"%32.1\") Var(\"%34.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 5)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.ra\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.n0\", \"%23.1\", \"%24.1\", \"%24.1.lo2\", \"%25.1\", \"@3.line\", \"@4.x\", \"@5.m\", \"@6.background.1\"]}}}",
      "Function(\"open\" true [\"@3.line\" \"@4.x\" \"@5.m\"] [\n"
      "  ZeroExtend(Var(\"@4.x\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"@3.line\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Load(Var(\"%12.1\") Var(\"%13.1\") false)\n"
      "  Binary(Add Var(\"%12.1\") Constant(ConstInt(8192)) Var(\"%15.1\"))\n"
      "  Load(Var(\"%15.1\") Var(\"%16.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@5.m\") Var(\"%20.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%16.1\") Var(\"%20.1\") Var(\"%22.1.n0\"))\n"
      "  Binary(BitwiseOr Var(\"%13.1\") Var(\"%22.1.n0\") Var(\"%25.1\"))\n"
      "  Store(Var(\"%25.1\") Var(\"%12.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"open\" true [\"@3.line\" \"@4.x\" \"@5.m\"] [\n"
      "  ZeroExtend(Var(\"@4.x\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"@3.line\") Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Load(Var(\"%12.1\") Var(\"%13.1\") false)\n"
      "  Binary(Add Var(\"%12.1\") Constant(ConstInt(8192)) Var(\"%15.1\"))\n"
      "  Load(Var(\"%15.1\") Var(\"%16.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@5.m\") Var(\"%20.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%16.1\") Var(\"%20.1\") Var(\"%22.1.n0\"))\n"
      "  Binary(BitwiseOr Var(\"%13.1\") Var(\"%22.1.n0\") Var(\"%25.1\"))\n"
      "  Store(Var(\"%25.1\") Var(\"%12.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_member_end_to_end (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.price\"]}}}",
      "Function(\"less\" false [\"@0.price\"] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(LessThan Var(\"%1.1\") Var(\"@0.price\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"less\" false [\"@0.price\"] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(LessThan Var(\"%1.1\") Var(\"@0.price\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_member_end_to_end (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.price\"]}}}",
      "Function(\"less\" false [\"@0.price\"] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(LessThan Var(\"%1.1\") Var(\"@0.price\") Var(\"%2.1\"))\n"
      "  Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"less\" false [\"@0.price\"] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(LessThan Var(\"%1.1\") Var(\"@0.price\") Var(\"%2.1\"))\n"
      "  Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_unoptimized_still_works (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(51)) Constant(ConstUChar(9))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(51)) Constant(ConstUChar(9))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_byte_lands_at_indexed_address (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(85)) Constant(ConstUChar(17))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(85)) Constant(ConstUChar(17))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 6)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.line\", \"@1.x\", \"@2.m\"]}}}",
      "Function(\"close\" true [\"@0.line\" \"@1.x\" \"@2.m\"] [\n"
      "  ZeroExtend(Var(\"@1.x\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.line\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  ZeroExtend(Var(\"@2.m\") Var(\"%3.1\"))\n"
      "  Copy(Var(\"@2.m\") Var(\"%4.1\"))\n"
      "  IndexedLoad(\"mask\" Var(\"%4.1\") Var(\"%5.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%7.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%7.1\") Var(\"%6.1\") Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Store(Var(\"%9.1\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"close\" true [\"@0.line\" \"@1.x\" \"@2.m\"] [\n"
      "  ZeroExtend(Var(\"@1.x\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.line\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  ZeroExtend(Var(\"@2.m\") Var(\"%3.1\"))\n"
      "  Copy(Var(\"@2.m\") Var(\"%4.1\"))\n"
      "  IndexedLoad(\"mask\" Var(\"%4.1\") Var(\"%5.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%7.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%7.1\") Var(\"%6.1\") Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Store(Var(\"%9.1\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_negative_signed_char_index (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"%4.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(51)) Constant(ConstChar(-1))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(51)) Constant(ConstChar(-1))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_canonical_indexed_store_collapses (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(66)) Constant(ConstUChar(5))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(66)) Constant(ConstUChar(5))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 5)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%24.1\", \"%25.1\", \"%27.1\", \"@4.n\", \"@4.n.1\", \"@4.n.2\", \"@5.k.1\", \"@5.k.2\", \"@5.k.3\"]}}}",
      "Function(\"wdown\" true [\"@4.n\"] [\n"
      "  Label(\".wdown@ssa_block@0\")\n"
      "  Label(\".loop@2_continue\")\n"
      "  Phi(Var(\"@5.k.2\") [PhiArg(\".wdown@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".wdown@ssa_block@1\" Var(\"%27.1\"))])\n"
      "  Phi(Var(\"@4.n.1\") [PhiArg(\".wdown@ssa_block@0\" Var(\"@4.n\")) PhiArg(\".wdown@ssa_block@1\" Var(\"%25.1\"))])\n"
      "  Binary(Subtract Var(\"@4.n.1\") Constant(ConstUInt(1)) Var(\"%25.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%25.1\") Constant(ConstUInt(65535)) \".loop@2_break\")\n"
      "  Label(\".wdown@ssa_block@1\")\n"
      "  Binary(Add Var(\"@5.k.2\") Constant(ConstUInt(2)) Var(\"%27.1\"))\n"
      "  Jump(\".loop@2_continue\")\n"
      "  Label(\".loop@2_break\")\n"
      "  Ret(Var(\"@5.k.2\"))\n"
      "])\n",
      "Function(\"wdown\" true [\"@4.n\"] [\n"
      "  Label(\".wdown@ssa_block@0\")\n"
      "  Label(\".loop@2_continue\")\n"
      "  Phi(Var(\"@5.k.2\") [PhiArg(\".wdown@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".wdown@ssa_block@1\" Var(\"%27.1\"))])\n"
      "  Phi(Var(\"@4.n.1\") [PhiArg(\".wdown@ssa_block@0\" Var(\"@4.n\")) PhiArg(\".wdown@ssa_block@1\" Var(\"%25.1\"))])\n"
      "  Binary(Subtract Var(\"@4.n.1\") Constant(ConstUInt(1)) Var(\"%25.1\"))\n"
      "  JumpIfCmp(Equal Var(\"%25.1\") Constant(ConstUInt(65535)) \".loop@2_break\")\n"
      "  Label(\".wdown@ssa_block@1\")\n"
      "  Binary(Add Var(\"@5.k.2\") Constant(ConstUInt(2)) Var(\"%27.1\"))\n"
      "  Jump(\".loop@2_continue\")\n"
      "  Label(\".loop@2_break\")\n"
      "  Ret(Var(\"@5.k.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 4)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%40.1\", \"%41.1\", \"%42.1\", \"%44.1\", \"%45.1\", \"%46.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"table\"))\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(254)) Constant(ConstUChar(18)) false 0)\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(255)) Constant(ConstUChar(52)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%35.1\"))\n"
      "  Binary(Add Var(\"%35.1\") Constant(ConstInt(256)) Var(\"%36.1\"))\n"
      "  Store(Constant(ConstUChar(86)) Var(\"%36.1\") false)\n"
      "  FunctionCall(\"word\" [Constant(ConstUChar(127))] Var(\"%38.1\") false false)\n"
      "  Binary(Equal Var(\"%38.1\") Constant(ConstUInt(4660)) Var(\"%40.1\"))\n"
      "  FunctionCall(\"odd\" [Constant(ConstUChar(255))] Var(\"%42.1\") false false)\n"
      "  Binary(Equal Var(\"%42.1\") Constant(ConstUInt(13398)) Var(\"%44.1\"))\n"
      "  Binary(LeftShift Var(\"%44.1\") Constant(ConstInt(1)) Var(\"%45.1\"))\n"
      "  Binary(Add Var(\"%40.1\") Var(\"%45.1\") Var(\"%46.1\"))\n"
      "  Ret(Var(\"%46.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"table\"))\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(254)) Constant(ConstUChar(18)) false 0)\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(255)) Constant(ConstUChar(52)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%35.1\"))\n"
      "  Binary(Add Var(\"%35.1\") Constant(ConstInt(256)) Var(\"%36.1\"))\n"
      "  Store(Constant(ConstUChar(86)) Var(\"%36.1\") false)\n"
      "  FunctionCall(\"word\" [Constant(ConstUChar(127))] Var(\"%38.1\") false false)\n"
      "  Binary(Equal Var(\"%38.1\") Constant(ConstUInt(4660)) Var(\"%40.1\"))\n"
      "  FunctionCall(\"odd\" [Constant(ConstUChar(255))] Var(\"%42.1\") false false)\n"
      "  Binary(Equal Var(\"%42.1\") Constant(ConstUInt(13398)) Var(\"%44.1\"))\n"
      "  Binary(LeftShift Var(\"%44.1\") Constant(ConstInt(1)) Var(\"%45.1\"))\n"
      "  Binary(Add Var(\"%40.1\") Var(\"%45.1\") Var(\"%46.1\"))\n"
      "  Ret(Var(\"%46.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 7)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"@7.x\", \"@8.v.1\"]}}}",
      "Function(\"open_global\" true [\"@7.x\"] [\n"
      "  ZeroExtend(Var(\"@7.x\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%27.1\"))\n"
      "  Load(Var(\"%27.1\") Var(\"%28.1\") false)\n"
      "  GetAddress(Var(\"screen\") Var(\"row\"))\n"
      "  Binary(BitwiseOr Var(\"%28.1\") Constant(ConstUChar(1)) Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%34.1\"))\n"
      "  Store(Var(\"%32.1\") Var(\"%34.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"open_global\" true [\"@7.x\"] [\n"
      "  ZeroExtend(Var(\"@7.x\") Var(\"%26.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%27.1\"))\n"
      "  Load(Var(\"%27.1\") Var(\"%28.1\") false)\n"
      "  GetAddress(Var(\"screen\") Var(\"row\"))\n"
      "  Binary(BitwiseOr Var(\"%28.1\") Constant(ConstUChar(1)) Var(\"%32.1\"))\n"
      "  Binary(Add Var(\"row\") Var(\"%26.1\") Var(\"%34.1\"))\n"
      "  Store(Var(\"%32.1\") Var(\"%34.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_static_written_before_last_read (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%5.1\", \"%5.1.sr0\", \"%5.1.sr1\", \"%6.1\", \"%7.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(1)) Var(\"s\"))\n"
      "  FunctionCall(\"f\" [] None false false)\n"
      "  Binary(LeftShift Var(\"h\") Constant(ConstInt(1)) Var(\"%5.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%5.1.sr0\") Constant(ConstInt(2)) Var(\"%5.1.sr1\"))\n"
      "  Binary(Add Var(\"%5.1.sr1\") Var(\"%5.1.sr0\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Var(\"g\") Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUInt(1)) Var(\"s\"))\n"
      "  FunctionCall(\"f\" [] None false false)\n"
      "  Binary(LeftShift Var(\"h\") Constant(ConstInt(1)) Var(\"%5.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%5.1.sr0\") Constant(ConstInt(2)) Var(\"%5.1.sr1\"))\n"
      "  Binary(Add Var(\"%5.1.sr1\") Var(\"%5.1.sr0\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Var(\"g\") Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_int_index_does_not_fold (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(66)) Constant(ConstInt(5))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(66)) Constant(ConstInt(5))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_increment.TestSinkIncrementSim.test_post_decrement_becomes_dex (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.c\", \"@1.x.1\", \"@1.x.2\", \"@1.x.3\"]}}}",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  ZeroExtend(Var(\"@1.x.2\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"@1.x.2\") Var(\"%2.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(8)) Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "Function(\"find\" true [\"@0.c\"] [\n"
      "  Label(\".find@ssa_block@0\")\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@1.x.2\") [PhiArg(\".find@ssa_block@0\" Constant(ConstUChar(7))) PhiArg(\".loop@0_continue\" Var(\"%11.1\"))])\n"
      "  ZeroExtend(Var(\"@1.x.2\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"@1.x.2\") Var(\"%2.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  JumpIfCmp(NotEqual Var(\"%3.1\") Var(\"@0.c\") \".if_end@0\")\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(8)) Var(\"%8.1\"))\n"
      "  Truncate(Var(\"%8.1\") Var(\"%9.1\"))\n"
      "  Ret(Var(\"%9.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Subtract Var(\"@1.x.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "  JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(255)) \".loop@0_start\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_access_end_to_end (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%7.1\", \"%8.1\", \"%9.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(0)) Constant(ConstUChar(17)) false 0)\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(255)) Constant(ConstUChar(153)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%9.1\"))\n"
      "  GetAddress(Var(\"data\") Var(\"%10.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%9.1\") Var(\"%10.1\")] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(0)) Constant(ConstUChar(17)) false 0)\n"
      "  IndexedSymbolStore(\"data\" Constant(ConstUChar(255)) Constant(ConstUChar(153)) false 0)\n"
      "  GetAddress(Var(\"data\") Var(\"%9.1\"))\n"
      "  GetAddress(Var(\"data\") Var(\"%10.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%9.1\") Var(\"%10.1\")] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_access_end_to_end (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.p\", \"@1.q\"]}}}",
      "Function(\"f\" true [\"@0.p\" \"@1.q\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  IndexedSymbolStore(\"a\" Constant(ConstUChar(0)) Var(\"%1.1\") true 0)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(255)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  IndexedSymbolStore(\"b\" Constant(ConstUChar(0)) Var(\"%3.1\") true 0)\n"
      "  Truncate(Constant(ConstInt(-122)) Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"@1.q\") Constant(ConstInt(7)) Var(\"%6.1\"))\n"
      "  Store(Var(\"%5.1\") Var(\"%6.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@0.p\" \"@1.q\"] [\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(0)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  IndexedSymbolStore(\"a\" Constant(ConstUChar(0)) Var(\"%1.1\") true 0)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(255)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  IndexedSymbolStore(\"b\" Constant(ConstUChar(0)) Var(\"%3.1\") true 0)\n"
      "  Truncate(Constant(ConstInt(-122)) Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"@1.q\") Constant(ConstInt(7)) Var(\"%6.1\"))\n"
      "  Store(Var(\"%5.1\") Var(\"%6.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 5)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.ra\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"@2.i\"]}}}",
      "Function(\"odd\" true [\"@2.i\"] [\n"
      "  ZeroExtend(Var(\"@2.i\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Load(Var(\"%16.1\") Var(\"%17.1\") false)\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Binary(LeftShift Var(\"%18.1\") Constant(ConstInt(8)) Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%15.1\") Var(\"%22.1.ra\"))\n"
      "  Binary(Add Var(\"%22.1.ra\") Constant(ConstInt(1)) Var(\"%22.1\"))\n"
      "  Load(Var(\"%22.1\") Var(\"%23.1\") false)\n"
      "  ZeroExtend(Var(\"%23.1\") Var(\"%25.1\"))\n"
      "  Binary(BitwiseOr Var(\"%19.1\") Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "Function(\"odd\" true [\"@2.i\"] [\n"
      "  ZeroExtend(Var(\"@2.i\") Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%15.1\") Var(\"%16.1\"))\n"
      "  Load(Var(\"%16.1\") Var(\"%17.1\") false)\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Binary(LeftShift Var(\"%18.1\") Constant(ConstInt(8)) Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%15.1\") Var(\"%22.1.ra\"))\n"
      "  Binary(Add Var(\"%22.1.ra\") Constant(ConstInt(1)) Var(\"%22.1\"))\n"
      "  Load(Var(\"%22.1\") Var(\"%23.1\") false)\n"
      "  ZeroExtend(Var(\"%23.1\") Var(\"%25.1\"))\n"
      "  Binary(BitwiseOr Var(\"%19.1\") Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_const_offset_indexed_store (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.value\", \"@1.col\"]}}}",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16484 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.value\" \"@1.col\"] [\n"
      "  IndexedStore(16484 Var(\"@1.col\") Var(\"@0.value\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_negative_signed_char_index (standalone) (call 4)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@5.r.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(4)) Var(\"%6.1\"))\n"
      "  Truncate(Constant(ConstInt(-1)) Var(\"%8.1\"))\n"
      "  FunctionCall(\"ld\" [Var(\"%6.1\") Var(\"%8.1\")] Var(\"%9.1\") false false)\n"
      "  GetAddress(Var(\"data\") Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"%10.1\") Constant(ConstInt(4)) Var(\"%11.1\"))\n"
      "  Truncate(Constant(ConstInt(-2)) Var(\"%13.1\"))\n"
      "  FunctionCall(\"st\" [Var(\"%11.1\") Var(\"%13.1\") Constant(ConstUChar(99))] None false false)\n"
      "  ZeroExtend(Var(\"%9.1\") Var(\"%15.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(2)) Var(\"%16.1\") false 0 0)\n"
      "  Binary(Equal Var(\"%16.1\") Constant(ConstUChar(99)) Var(\"%18.1\"))\n"
      "  Binary(Multiply Var(\"%18.1\") Constant(ConstInt(100)) Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"data\") Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(4)) Var(\"%6.1\"))\n"
      "  Truncate(Constant(ConstInt(-1)) Var(\"%8.1\"))\n"
      "  FunctionCall(\"ld\" [Var(\"%6.1\") Var(\"%8.1\")] Var(\"%9.1\") false false)\n"
      "  GetAddress(Var(\"data\") Var(\"%10.1\"))\n"
      "  Binary(Add Var(\"%10.1\") Constant(ConstInt(4)) Var(\"%11.1\"))\n"
      "  Truncate(Constant(ConstInt(-2)) Var(\"%13.1\"))\n"
      "  FunctionCall(\"st\" [Var(\"%11.1\") Var(\"%13.1\") Constant(ConstUChar(99))] None false false)\n"
      "  ZeroExtend(Var(\"%9.1\") Var(\"%15.1\"))\n"
      "  IndexedLoad(\"data\" Constant(ConstUChar(2)) Var(\"%16.1\") false 0 0)\n"
      "  Binary(Equal Var(\"%16.1\") Constant(ConstUChar(99)) Var(\"%18.1\"))\n"
      "  Binary(Multiply Var(\"%18.1\") Constant(ConstInt(100)) Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%15.1\") Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_negative_signed_char_index (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"%4.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstInt(-1)) Var(\"%4.1\"))\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(51)) Var(\"%4.1\")] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Truncate(Constant(ConstInt(-1)) Var(\"%4.1\"))\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(51)) Var(\"%4.1\")] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchKeepsSharedDefs.test_shared_zero_extend_sim (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.i\", \"@1.b.1\", \"@2.w.1\", \"@3.mag.1\"]}}}",
      "Function(\"step\" true [\"@0.i\"] [\n"
      "  IndexedLoad(\"bob\" Var(\"@0.i\") Var(\"%2.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  JumpIfMasked(Var(\"%2.1\") 128 false \".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_then@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_then@0\") Constant(ConstInt(127)) Var(\"%5.1.snk_then@0\"))\n"
      "  Truncate(Var(\"%5.1.snk_then@0\") Var(\"%6.1.snk_then@0\"))\n"
      "  Binary(Subtract Var(\"y\") Var(\"%6.1.snk_then@0\") Var(\"y\"))\n"
      "  Jump(\".if_end@0\")\n"
      "  Label(\".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_else@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_else@0\") Constant(ConstInt(127)) Var(\"%5.1.snk_else@0\"))\n"
      "  Truncate(Var(\"%5.1.snk_else@0\") Var(\"%6.1.snk_else@0\"))\n"
      "  Binary(Add Var(\"y\") Var(\"%6.1.snk_else@0\") Var(\"y\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Var(\"%3.1\") Var(\"g16\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"step\" true [\"@0.i\"] [\n"
      "  IndexedLoad(\"bob\" Var(\"@0.i\") Var(\"%2.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  JumpIfMasked(Var(\"%2.1\") 128 false \".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_then@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_then@0\") Constant(ConstInt(127)) Var(\"%5.1.snk_then@0\"))\n"
      "  Truncate(Var(\"%5.1.snk_then@0\") Var(\"%6.1.snk_then@0\"))\n"
      "  Binary(Subtract Var(\"y\") Var(\"%6.1.snk_then@0\") Var(\"y\"))\n"
      "  Jump(\".if_end@0\")\n"
      "  Label(\".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_else@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_else@0\") Constant(ConstInt(127)) Var(\"%5.1.snk_else@0\"))\n"
      "  Truncate(Var(\"%5.1.snk_else@0\") Var(\"%6.1.snk_else@0\"))\n"
      "  Binary(Add Var(\"y\") Var(\"%6.1.snk_else@0\") Var(\"y\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Var(\"%3.1\") Var(\"g16\"))\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_offset_indexed_store_lands_correctly (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%3.1\", \"%4.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(153)) Constant(ConstUChar(50))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"put\" [Constant(ConstUChar(153)) Constant(ConstUChar(50))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_store_writes_correct_byte (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"%4.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"dest\") Var(\"%2.1\"))\n"
      "  FunctionCall(\"store\" [Var(\"%2.1\") Constant(ConstUChar(3)) Constant(ConstUChar(171))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"dest\") Var(\"%2.1\"))\n"
      "  FunctionCall(\"store\" [Var(\"%2.1\") Constant(ConstUChar(3)) Constant(ConstUChar(171))] None false false)\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_recognize_indirect_indexed.TestIndirectIndexedAsmShape.test_global_pointer_stored_constant_on_another_path (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.n\"]}}}",
      "Function(\"walk\" true [\"@0.n\"] [\n"
      "  JumpIfTrue(Var(\"@0.n\") \".if_end@0\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"P\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(4)) Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  JumpIfCmp(LessThan Var(\"%4.1\") Constant(ConstUChar(21)) \".loop@0_break\")\n"
      "  FunctionCall(\"next\" [] None false false)\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"walk\" true [\"@0.n\"] [\n"
      "  JumpIfTrue(Var(\"@0.n\") \".if_end@0\")\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"P\"))\n"
      "  Ret(None)\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(4)) Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  JumpIfCmp(LessThan Var(\"%4.1\") Constant(ConstUChar(21)) \".loop@0_break\")\n"
      "  FunctionCall(\"next\" [] None false false)\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: test_sink_and_past_branch.TestSinkAndPastBranchEligibility.test_jumpifmasked_on_different_var_blocks (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"y\") 128 false \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" false [] [\n"
      "  ZeroExtend(Var(\"x\") Var(\"%x_ext\"))\n"
      "  Binary(BitwiseAnd Var(\"%x_ext\") Constant(ConstInt(127)) Var(\"%x_and\"))\n"
      "  Truncate(Var(\"%x_and\") Var(\"%t\"))\n"
      "  JumpIfMasked(Var(\"y\") 128 false \".L\")\n"
      "  Copy(Var(\"%t\") Var(\"%u\"))\n"
      "  Label(\".L\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_and_past_branch: examples/apply_bobble.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_and_past_branch",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.slot\", \"@1.bobble_idx\", \"@2.bobble.1\", \"@3.magnitude.1\"]}}}",
      "Function(\"apply_bobble\" false [\"@0.slot\" \"@1.bobble_idx\"] [\n"
      "  ZeroExtend(Var(\"@1.bobble_idx\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"@1.bobble_idx\") Var(\"%1.1\"))\n"
      "  IndexedLoad(\"rescue_bobble\" Var(\"%1.1\") Var(\"%2.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1\") Constant(ConstInt(127)) Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  JumpIfMasked(Var(\"%2.1\") 128 false \".if_else@1\")\n"
      "  ZeroExtend(Var(\"@0.slot\") Var(\"%8.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%9.1\"))\n"
      "  IndexedLoad(\"entity_floor_pos\" Var(\"%9.1\") Var(\"%10.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%11.1\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Truncate(Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%16.1\"))\n"
      "  IndexedSymbolStore(\"entity_floor_pos\" Var(\"%16.1\") Var(\"%14.1\") false 0)\n"
      "  Jump(\".if_end@0\")\n"
      "  Label(\".if_else@1\")\n"
      "  ZeroExtend(Var(\"@0.slot\") Var(\"%17.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%18.1\"))\n"
      "  IndexedLoad(\"entity_floor_pos\" Var(\"%18.1\") Var(\"%19.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%21.1\"))\n"
      "  Binary(Subtract Var(\"%20.1\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"entity_floor_pos\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"apply_bobble\" false [\"@0.slot\" \"@1.bobble_idx\"] [\n"
      "  ZeroExtend(Var(\"@1.bobble_idx\") Var(\"%0.1\"))\n"
      "  Copy(Var(\"@1.bobble_idx\") Var(\"%1.1\"))\n"
      "  IndexedLoad(\"rescue_bobble\" Var(\"%1.1\") Var(\"%2.1\") false 0 0)\n"
      "  JumpIfMasked(Var(\"%2.1\") 128 false \".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_then@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_then@0\") Constant(ConstInt(127)) Var(\"%4.1.snk_then@0\"))\n"
      "  Truncate(Var(\"%4.1.snk_then@0\") Var(\"%5.1.snk_then@0\"))\n"
      "  ZeroExtend(Var(\"@0.slot\") Var(\"%8.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%9.1\"))\n"
      "  IndexedLoad(\"entity_floor_pos\" Var(\"%9.1\") Var(\"%10.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  ZeroExtend(Var(\"%5.1.snk_then@0\") Var(\"%12.1\"))\n"
      "  Binary(Add Var(\"%11.1\") Var(\"%12.1\") Var(\"%13.1\"))\n"
      "  Truncate(Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%16.1\"))\n"
      "  IndexedSymbolStore(\"entity_floor_pos\" Var(\"%16.1\") Var(\"%14.1\") false 0)\n"
      "  Jump(\".if_end@0\")\n"
      "  Label(\".if_else@1\")\n"
      "  ZeroExtend(Var(\"%2.1\") Var(\"%3.1.snk_else@0\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1.snk_else@0\") Constant(ConstInt(127)) Var(\"%4.1.snk_else@0\"))\n"
      "  Truncate(Var(\"%4.1.snk_else@0\") Var(\"%5.1.snk_else@0\"))\n"
      "  ZeroExtend(Var(\"@0.slot\") Var(\"%17.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%18.1\"))\n"
      "  IndexedLoad(\"entity_floor_pos\" Var(\"%18.1\") Var(\"%19.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  ZeroExtend(Var(\"%5.1.snk_else@0\") Var(\"%21.1\"))\n"
      "  Binary(Subtract Var(\"%20.1\") Var(\"%21.1\") Var(\"%22.1\"))\n"
      "  Truncate(Var(\"%22.1\") Var(\"%23.1\"))\n"
      "  Copy(Var(\"@0.slot\") Var(\"%25.1\"))\n"
      "  IndexedSymbolStore(\"entity_floor_pos\" Var(\"%25.1\") Var(\"%23.1\") false 0)\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1.snk_then@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": "
      "\"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1.snk_else@0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}
