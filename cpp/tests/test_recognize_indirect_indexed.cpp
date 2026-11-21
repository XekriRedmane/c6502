// The recorded calls of `tac_recognize_indirect_indexed`, the C++ port
// of passes/optimization/recognize_indirect_indexed.py, with what the
// Python pass returned. They were recorded with the capture harness's
// wrappers while tests/test_recognize_indirect_indexed.py ran, through
// the TAC optimizer's driver and through the Python entry points that
// the tests call directly. They are the calls that changed the function
// or the symbols, and the calls of small synthetic functions through
// those entry points. Two tests give a symbol a `StaticAttr` with no
// initial value, which the harness's codec can't carry, so those
// records have the `NoInitializer` that the pass doesn't read in its
// place.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_outside_a_byte_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(256)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(256)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_store_writes_correct_byte (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.p\", \"@1.i\", \"@2.v\"]}}}",
      "Function(\"store\" true [\"@0.p\" \"@1.i\" \"@2.v\"] [\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Store(Var(\"@2.v\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"store\" true [\"@0.p\" \"@1.i\" \"@2.v\"] [\n"
      "  IndirectIndexedStore(Var(\"@0.p\") Var(\"@1.i\") Var(\"@2.v\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_commutative_arrangement_folds (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ext\") Var(\"p\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Var(\"y\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_outside_a_byte_does_not_fold (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(-1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(-1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_global_pointer_changed_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"gp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\", \"value\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  FunctionCall(\"h\" [] None false false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_int_index_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_load_reads_correct_byte (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.p\", \"@1.i\"]}}}",
      "Function(\"load\" true [\"@0.p\" \"@1.i\"] [\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"load\" true [\"@0.p\" \"@1.i\"] [\n"
      "  IndirectIndexedLoad(Var(\"@0.p\") Var(\"@1.i\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestSharedByteIndex.test_other_reader_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%other\", \"b\", \"y\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedDefersToIndexedStore.test_copy_chain_to_constant_defers (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%p1\", \"%p2\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%p1\"))\n"
      "  Copy(Var(\"%p1\") Var(\"%p2\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%p2\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%p1\"))\n"
      "  Copy(Var(\"%p1\") Var(\"%p2\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%p2\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestShapes.test_member_store_through_zero_page_pointer (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.v\"]}}}",
      "Function(\"f\" true [\"@0.v\"] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Store(Var(\"@0.v\") Var(\"%0.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@0.v\"] [\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(13)) Var(\"@0.v\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_access_end_to_end (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_load_folds (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(0)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(0)) Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_static_written_before_last_read (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestShapes.test_member_through_pointer_parameter (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"@1.r\"]}}}",
      "Function(\"get\" true [\"@1.r\"] [\n"
      "  Binary(Add Var(\"@1.r\") Constant(ConstInt(13)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"get\" true [\"@1.r\"] [\n"
      "  IndirectIndexedLoad(Var(\"@1.r\") Constant(ConstUChar(13)) Var(\"%3.1\") false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_negative_signed_char_index (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_static_written_before_last_read (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestShapes.test_pointer_member_add (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 244, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(15)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(256)) Var(\"%2.1\"))\n"
      "  Store(Var(\"%2.1\") Var(\"%0.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(15)) Var(\"%0.1\"))\n"
      "  IndirectIndexedLoad(Var(\"P\") Constant(ConstUChar(15)) Var(\"%1.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(256)) Var(\"%2.1\"))\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(15)) Var(\"%2.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@2.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.ra\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"@2.i\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"table\") Var(\"@2.i\") Var(\"%17.1\") false)\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%18.1\"))\n"
      "  Binary(LeftShift Var(\"%18.1\") Constant(ConstInt(8)) Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%15.1\") Var(\"%22.1.ra\"))\n"
      "  IndirectIndexedLoad(Var(\"%22.1.ra\") Constant(ConstUChar(1)) Var(\"%23.1\") false)\n"
      "  ZeroExtend(Var(\"%23.1\") Var(\"%25.1\"))\n"
      "  Binary(BitwiseOr Var(\"%19.1\") Var(\"%25.1\") Var(\"%26.1\"))\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"table\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%10.1.ra\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.j\", \"@1.i.1\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"table\") Var(\"%2.1\") Var(\"%5.1\") false)\n"
      "  ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Binary(LeftShift Var(\"%6.1\") Constant(ConstInt(8)) Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"table\") Var(\"%3.1\") Var(\"%10.1.ra\"))\n"
      "  Binary(Add Var(\"%2.1\") Constant(ConstUChar(1)) Var(\"%10.1.ra.y1\"))\n"
      "  IndirectIndexedLoad(Var(\"table\") Var(\"%10.1.ra.y1\") Var(\"%11.1\") false)\n"
      "  ZeroExtend(Var(\"%11.1\") Var(\"%13.1\"))\n"
      "  Binary(BitwiseOr Var(\"%7.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "  Ret(Var(\"%14.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"table\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1.ra.y1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%10.1.ra\", \"%10.1.ra.y1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.j\", \"@1.i.1\"]}}}");
}

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_choice_of_member (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.v\"]}}}",
      "Function(\"put\" true [\"@0.v\"] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Store(Var(\"@0.v\") Var(\"%0.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.v\"] [\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(13)) Var(\"@0.v\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_and_past_branch.TestSinkAndPastBranchKeepsSharedDefs.test_shared_zero_extend_sim (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_choice_of_member (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\"]}}}",
      "Function(\"le\" true [] [\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(11)) Var(\"%17.1\"))\n"
      "  Load(Var(\"%17.1\") Var(\"%18.1\") false)\n"
      "  Binary(LessOrEqual Var(\"W\") Var(\"%18.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "Function(\"le\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"E\") Constant(ConstUChar(11)) Var(\"%18.1\") false)\n"
      "  Binary(LessOrEqual Var(\"W\") Var(\"%18.1\") Var(\"%20.1\"))\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_read_and_write_end_to_end (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestShapes.test_choice_of_member_is_an_offset (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.3.o\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.2\", \"%2.3\", \"%2.3.e\", \"%2.3.o\", \"%3.1\", \"%4.1\", \"@0.n\", \"@1.v\", \"@2.w.1\"]}}}",
      "Function(\"f\" true [\"@0.n\" \"@1.v\"] [\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.n\") Constant(ConstUChar(3)) \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3.o\") [PhiArg(\".f@ssa_block@1\" Constant(ConstUChar(7))) PhiArg(\".cond_else@0\" Constant(ConstUChar(9)))])\n"
      "  ZeroExtend(Var(\"%2.3.o\") Var(\"%2.3.e\"))\n"
      "  Binary(Add Var(\"E\") Var(\"%2.3.e\") Var(\"%2.3\"))\n"
      "  Store(Var(\"@1.v\") Var(\"%2.3\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@0.n\" \"@1.v\"] [\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@0.n\") Constant(ConstUChar(3)) \".cond_else@0\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%2.3.o\") [PhiArg(\".f@ssa_block@1\" Constant(ConstUChar(7))) PhiArg(\".cond_else@0\" Constant(ConstUChar(9)))])\n"
      "  IndirectIndexedStore(Var(\"E\") Var(\"%2.3.o\") Var(\"@1.v\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedStoreUnit.test_canonical_store_folds (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedStore(Var(\"p\") Var(\"y\") Var(\"value\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_offset_load_folds (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(254)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(254)) Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_src_y_postincrement_chain (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_src_y_postincrement_chain (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_negative_signed_char_index (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_unoptimized_still_works (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_load_folds (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(10)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(10)) Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_member_end_to_end (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%68.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%72.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%58.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%59.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%64.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%65.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%16.1\", \"%17.1\", \"%18.1\", \"%20.1\", \"%21.1\", \"%21.2\", \"%21.3\", \"%23.1\", \"%24.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%33.1\", \"%34.1\", \"%34.2\", \"%34.3\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"%41.1\", \"%42.1\", \"%43.1\", \"%44.1\", \"%45.1\", \"%46.1\", \"%47.1\", \"%48.1\", \"%49.1\", \"%49.2\", \"%49.3\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%53.2\", \"%53.3\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\", \"%69.1\", \"%70.1\", \"%71.1\", \"%72.1\", \"%73.1\", \"%74.1\""
      ", \"@4.q.1\", \"@5.ptr.1\", \"@5.ptr.2\", \"@6.price.1\", \"@7.price.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"r\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Constant(ConstInt(16)) Var(\"P\"))\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%20.1\"))\n"
      "  Store(Constant(ConstUInt(1000)) Var(\"%20.1\") false)\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%67.1\"))\n"
      "  Load(Var(\"%67.1\") Var(\"%68.1\") false)\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%68.1\") Constant(ConstUInt(1001)) \".main@jthread@0\")\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%71.1\"))\n"
      "  Load(Var(\"%71.1\") Var(\"%72.1\") false)\n"
      "  JumpIfCmp(LessThan Var(\"%72.1\") Constant(ConstUInt(1000)) \".main@jthread@0\")\n"
      "  Jump(\".if_end@3\")\n"
      "  Label(\".main@jthread@0\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@3\")\n"
      "  Copy(Var(\"P\") Var(\"@4.q.1\"))\n"
      "  Binary(Add Var(\"@4.q.1\") Constant(ConstInt(13)) Var(\"%57.1\"))\n"
      "  Load(Var(\"%57.1\") Var(\"%58.1\") false)\n"
      "  Binary(Add Var(\"%58.1\") Constant(ConstUInt(5)) Var(\"%59.1\"))\n"
      "  Store(Var(\"%59.1\") Var(\"%57.1\") false)\n"
      "  IndexedLoad(\"r\" Constant(ConstUChar(29)) Var(\"%31.1\") false 0 0)\n"
      "  JumpIfCmp(Equal Var(\"%31.1\") Constant(ConstUInt(1005)) \".if_end@4\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "  Label(\".if_end@4\")\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%66.1\"))\n"
      "  Store(Constant(ConstUInt(0)) Var(\"%66.1\") false)\n"
      "  IndexedLoad(\"r\" Constant(ConstUChar(29)) Var(\"%39.1\") false 0 0)\n"
      "  JumpIfTrue(Var(\"%39.1\") \".main@jthread@1\")\n"
      "  IndexedLoad(\"r\" Constant(ConstUChar(31)) Var(\"%46.1\") false 0 0)\n"
      "  JumpIfTrue(Var(\"%46.1\") \".main@jthread@1\")\n"
      "  Jump(\".if_end@7\")\n"
      "  Label(\".main@jthread@1\")\n"
      "  Ret(Constant(ConstInt(3)))\n"
      "  Label(\".if_end@7\")\n"
      "  GetAddress(Var(\"@3.arr\") Var(\"%50.1\"))\n"
      "  Binary(Add Var(\"%50.1\") Constant(ConstInt(2)) Var(\"%52.1\"))\n"
      "  Binary(Subtract Var(\"%52.1\") Constant(ConstInt(2)) Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"%61.1\") Constant(ConstInt(6)) Var(\"%63.1\"))\n"
      "  Load(Var(\"%63.1\") Var(\"%64.1\") false)\n"
      "  Binary(Add Var(\"%64.1\") Constant(ConstInt(4)) Var(\"%65.1\"))\n"
      "  Store(Var(\"%65.1\") Var(\"%63.1\") false)\n"
      "  JumpIfFalse(Var(\"%65.1\") \".main@jthread@2\")\n"
      "  Jump(\".main@jthread@3\")\n"
      "  Label(\".main@jthread@2\")\n"
      "  IndexedLoad(\"@3.arr\" Constant(ConstUChar(6)) Var(\"%55.1\") false 0 0)\n"
      "  JumpIfTrue(Var(\"%55.1\") \".main@jthread@3\")\n"
      "  Jump(\".if_end@10\")\n"
      "  Label(\".main@jthread@3\")\n"
      "  Ret(Constant(ConstInt(4)))\n"
      "  Label(\".if_end@10\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"r\") Var(\"%16.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Constant(ConstInt(16)) Var(\"P\"))\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(13)) Constant(ConstUInt(1000)) false)\n"
      "  IndirectIndexedLoad(Var(\"P\") Constant(ConstUChar(13)) Var(\"%68.1\") false)\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%68.1\") Constant(ConstUInt(1001)) \".main@jthread@0\")\n"
      "  IndirectIndexedLoad(Var(\"P\") Constant(ConstUChar(13)) Var(\"%72.1\") false)\n"
      "  JumpIfCmp(LessThan Var(\"%72.1\") Constant(ConstUInt(1000)) \".main@jthread@0\")\n"
      "  Jump(\".if_end@3\")\n"
      "  Label(\".main@jthread@0\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "  Label(\".if_end@3\")\n"
      "  Copy(Var(\"P\") Var(\"@4.q.1\"))\n"
      "  Binary(Add Var(\"@4.q.1\") Constant(ConstInt(13)) Var(\"%57.1\"))\n"
      "  IndirectIndexedLoad(Var(\"@4.q.1\") Constant(ConstUChar(13)) Var(\"%58.1\") false)\n"
      "  Binary(Add Var(\"%58.1\") Constant(ConstUInt(5)) Var(\"%59.1\"))\n"
      "  IndirectIndexedStore(Var(\"@4.q.1\") Constant(ConstUChar(13)) Var(\"%59.1\") false)\n"
      "  IndexedLoad(\"r\" Constant(ConstUChar(29)) Var(\"%31.1\") false 0 0)\n"
      "  JumpIfCmp(Equal Var(\"%31.1\") Constant(ConstUInt(1005)) \".if_end@4\")\n"
      "  Ret(Constant(ConstInt(2)))\n"
      "  Label(\".if_end@4\")\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(13)) Constant(ConstUInt(0)) false)\n"
      "  IndexedLoad(\"r\" Constant(ConstUChar(29)) Var(\"%39.1\") false 0 0)\n"
      "  JumpIfTrue(Var(\"%39.1\") \".main@jthread@1\")\n"
      "  IndexedLoad(\"r\" Constant(ConstUChar(31)) Var(\"%46.1\") false 0 0)\n"
      "  JumpIfTrue(Var(\"%46.1\") \".main@jthread@1\")\n"
      "  Jump(\".if_end@7\")\n"
      "  Label(\".main@jthread@1\")\n"
      "  Ret(Constant(ConstInt(3)))\n"
      "  Label(\".if_end@7\")\n"
      "  GetAddress(Var(\"@3.arr\") Var(\"%50.1\"))\n"
      "  Binary(Add Var(\"%50.1\") Constant(ConstInt(2)) Var(\"%52.1\"))\n"
      "  Binary(Subtract Var(\"%52.1\") Constant(ConstInt(2)) Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"%61.1\") Constant(ConstInt(6)) Var(\"%63.1\"))\n"
      "  IndirectIndexedLoad(Var(\"%61.1\") Constant(ConstUChar(6)) Var(\"%64.1\") false)\n"
      "  Binary(Add Var(\"%64.1\") Constant(ConstInt(4)) Var(\"%65.1\"))\n"
      "  IndirectIndexedStore(Var(\"%61.1\") Constant(ConstUChar(6)) Var(\"%65.1\") false)\n"
      "  JumpIfFalse(Var(\"%65.1\") \".main@jthread@2\")\n"
      "  Jump(\".main@jthread@3\")\n"
      "  Label(\".main@jthread@2\")\n"
      "  IndexedLoad(\"@3.arr\" Constant(ConstUChar(6)) Var(\"%55.1\") false 0 0)\n"
      "  JumpIfTrue(Var(\"%55.1\") \".main@jthread@3\")\n"
      "  Jump(\".if_end@10\")\n"
      "  Label(\".main@jthread@3\")\n"
      "  Ret(Constant(ConstInt(4)))\n"
      "  Label(\".if_end@10\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedStoreUnit.test_uint_src_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedAsmShape.test_indirect_indexed_load_emits_short_sequence (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.idx\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.src\", \"@1.idx\"]}}}",
      "Function(\"copy\" true [\"@0.src\" \"@1.idx\"] [\n"
      "  ZeroExtend(Var(\"@1.idx\") Var(\"%0.1\"))\n"
      "  Binary(Add Var(\"@0.src\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Load(Var(\"%1.1\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"copy\" true [\"@0.src\" \"@1.idx\"] [\n"
      "  IndirectIndexedLoad(Var(\"@0.src\") Var(\"@1.idx\") Var(\"%2.1\") false)\n"
      "  IndexedSymbolStore(\"result\" Constant(ConstUChar(0)) Var(\"%2.1\") true 0)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_unoptimized_still_works (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestShapes.test_compound_assignment_calls_first (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"E\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"mob\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 248, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@1.x\"]}}}",
      "Function(\"f\" true [\"@1.x\"] [\n"
      "  FunctionCall(\"h\" [Var(\"@1.x\")] Var(\"%0.1\") false false)\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(11)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  Binary(BitwiseOr Var(\"%3.1\") Var(\"%0.1\") Var(\"%6.1\"))\n"
      "  Store(Var(\"%6.1\") Var(\"%2.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"@1.x\"] [\n"
      "  FunctionCall(\"h\" [Var(\"@1.x\")] Var(\"%0.1\") false false)\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(11)) Var(\"%2.1\"))\n"
      "  IndirectIndexedLoad(Var(\"E\") Constant(ConstUChar(11)) Var(\"%3.1\") false)\n"
      "  Binary(BitwiseOr Var(\"%3.1\") Var(\"%0.1\") Var(\"%6.1\"))\n"
      "  IndirectIndexedStore(Var(\"E\") Constant(ConstUChar(11)) Var(\"%6.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_direct_member_access.TestDirectMemberAccessSim.test_address_escapes_after_member_writes (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"@0.p\"]}}}",
      "Function(\"sum\" true [\"@0.p\"] [\n"
      "  Load(Var(\"@0.p\") Var(\"%0.1\") false)\n"
      "  ZeroExtend(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(3)) Var(\"%5.1\"))\n"
      "  Load(Var(\"%5.1\") Var(\"%6.1\") false)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "Function(\"sum\" true [\"@0.p\"] [\n"
      "  Load(Var(\"@0.p\") Var(\"%0.1\") false)\n"
      "  ZeroExtend(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  IndirectIndexedLoad(Var(\"@0.p\") Constant(ConstUChar(1)) Var(\"%3.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  IndirectIndexedLoad(Var(\"@0.p\") Constant(ConstUChar(3)) Var(\"%6.1\") false)\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Ret(Var(\"%8.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_read_and_write_end_to_end (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"gp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.p\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"@0.p\") Constant(ConstUChar(1)) Var(\"%1.1\") false)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "  IndirectIndexedStore(Var(\"@0.p\") Constant(ConstUChar(1)) Var(\"%2.1\") false)\n"
      "  Binary(Add Var(\"@0.p\") Constant(ConstInt(2)) Var(\"%3.1\"))\n"
      "  IndirectIndexedLoad(Var(\"@0.p\") Constant(ConstUChar(2)) Var(\"%4.1\") false)\n"
      "  Binary(Add Var(\"%4.1\") Constant(ConstUChar(3)) Var(\"%7.1\"))\n"
      "  IndirectIndexedStore(Var(\"@0.p\") Constant(ConstUChar(2)) Var(\"%7.1\") false)\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(3)) Var(\"%8.1\"))\n"
      "  IndirectIndexedLoad(Var(\"gp\") Constant(ConstUChar(3)) Var(\"%9.1\") false)\n"
      "  Binary(Subtract Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%10.1\"))\n"
      "  IndirectIndexedStore(Var(\"gp\") Constant(ConstUChar(3)) Var(\"%10.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_constant_addend_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Constant(ConstUInt(8192)) Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Constant(ConstUInt(8192)) Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestShapes.test_member_compare_reads_through_the_pointer (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\"]}}}",
      "Function(\"c\" true [] [\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(LessOrEqual Var(\"W\") Var(\"%1.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"c\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"E\") Constant(ConstUChar(13)) Var(\"%1.1\") false)\n"
      "  Binary(LessOrEqual Var(\"W\") Var(\"%1.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_uint_dst_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.line\", \"@1.x\", \"@2.m\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"@0.line\") Var(\"@1.x\") Var(\"%2.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@2.m\") Var(\"%5.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%2.1\") Var(\"%5.1\") Var(\"%9.1\"))\n"
      "  IndirectIndexedStore(Var(\"@0.line\") Var(\"@1.x\") Var(\"%9.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_load_reads_correct_byte (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_choice_of_member (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@6.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.3.o\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%12.1\", \"%13.1\", \"%14.1\", \"%14.2\", \"%14.3\", \"%14.3.e\", \"%14.3.o\", \"%15.1\", \"%16.1\", \"@5.n\", \"@6.v\", \"@7.w.1\"]}}}",
      "Function(\"choose\" true [\"@5.n\" \"@6.v\"] [\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@5.n\") Constant(ConstUChar(3)) \".cond_else@0\")\n"
      "  Label(\".choose@ssa_block@1\")\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%14.3.o\") [PhiArg(\".choose@ssa_block@1\" Constant(ConstUChar(7))) PhiArg(\".cond_else@0\" Constant(ConstUChar(9)))])\n"
      "  ZeroExtend(Var(\"%14.3.o\") Var(\"%14.3.e\"))\n"
      "  Binary(Add Var(\"E\") Var(\"%14.3.e\") Var(\"%14.3\"))\n"
      "  Store(Var(\"@6.v\") Var(\"%14.3\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"choose\" true [\"@5.n\" \"@6.v\"] [\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"@5.n\") Constant(ConstUChar(3)) \".cond_else@0\")\n"
      "  Label(\".choose@ssa_block@1\")\n"
      "  Jump(\".cond_end@1\")\n"
      "  Label(\".cond_else@0\")\n"
      "  Label(\".cond_end@1\")\n"
      "  Phi(Var(\"%14.3.o\") [PhiArg(\".choose@ssa_block@1\" Constant(ConstUChar(7))) PhiArg(\".cond_else@0\" Constant(ConstUChar(9)))])\n"
      "  IndirectIndexedStore(Var(\"E\") Var(\"%14.3.o\") Var(\"@6.v\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_sign_extend_def_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  SignExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestByteIndexSum.test_even_index_reads_with_iny (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_sink_and_past_branch.TestSinkAndPastBranchKeepsSharedDefs.test_shared_zero_extend_sim (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_negative_signed_char_index (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_access_end_to_end (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.p\", \"@1.q\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"@0.p\") Constant(ConstUChar(0)) Var(\"%1.1\") false)\n"
      "  IndexedSymbolStore(\"a\" Constant(ConstUChar(0)) Var(\"%1.1\") true 0)\n"
      "  IndirectIndexedLoad(Var(\"@0.p\") Constant(ConstUChar(255)) Var(\"%3.1\") false)\n"
      "  IndexedSymbolStore(\"b\" Constant(ConstUChar(0)) Var(\"%3.1\") true 0)\n"
      "  IndirectIndexedStore(Var(\"@1.q\") Constant(ConstUChar(7)) Constant(ConstChar(-122)) false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_byte_lands_at_indexed_address (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_multi_use_addr_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%other\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"%other\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_offset_load_folds (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(0)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(0)) Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_offset_load_folds (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(13)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(13)) Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_member_end_to_end (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.price\"]}}}",
      "Function(\"less\" false [\"@0.price\"] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(13)) Var(\"%0.1\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Binary(LessThan Var(\"%1.1\") Var(\"@0.price\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"less\" false [\"@0.price\"] [\n"
      "  IndirectIndexedLoad(Var(\"P\") Constant(ConstUChar(13)) Var(\"%1.1\") false)\n"
      "  Binary(LessThan Var(\"%1.1\") Var(\"@0.price\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_static_indexed_access.TestStaticIndexedAccess.test_member_reads (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@3.j\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"@2.i\", \"@3.j\"]}}}",
      "Function(\"t\" true [\"@2.i\" \"@3.j\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%16.1\"))\n"
      "  Binary(LeftShift Var(\"@2.i\") Constant(ConstInt(3)) Var(\"%19.1\"))\n"
      "  ZeroExtend(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  ZeroExtend(Var(\"@3.j\") Var(\"%24.1\"))\n"
      "  Binary(Add Var(\"%21.1\") Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Load(Var(\"%25.1\") Var(\"%26.1\") false)\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "Function(\"t\" true [\"@2.i\" \"@3.j\"] [\n"
      "  GetAddress(Var(\"r\") Var(\"%16.1\"))\n"
      "  Binary(LeftShift Var(\"@2.i\") Constant(ConstInt(3)) Var(\"%19.1\"))\n"
      "  ZeroExtend(Var(\"%19.1\") Var(\"%20.1\"))\n"
      "  Binary(Add Var(\"%16.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  IndirectIndexedLoad(Var(\"%21.1\") Var(\"@3.j\") Var(\"%26.1\") false)\n"
      "  Ret(Var(\"%26.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_indexed_load.TestIndexedLoadAsmShape.test_local_array_falls_back (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%0.2\", \"%0.3\", \"%1.1\", \"%10.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.i\"]}}}",
      "Function(\"main\" true [\"@0.i\"] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".clear@0\")\n"
      "  Phi(Var(\"%0.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(20))) PhiArg(\".clear@0\" Var(\"%0.3\"))])\n"
      "  Binary(Subtract Var(\"%0.2\") Constant(ConstUChar(1)) Var(\"%0.3\"))\n"
      "  ZeroExtend(Var(\"%0.3\") Var(\"%1.1\"))\n"
      "  GetAddress(Var(\"@1.arr\") Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"%2.1\") Var(\"%1.1\") Var(\"%3.1\"))\n"
      "  Store(Constant(ConstUChar(0)) Var(\"%3.1\") false)\n"
      "  JumpIfTrue(Var(\"%0.3\") \".clear@0\")\n"
      "  GetAddress(Var(\"@1.arr\") Var(\"%4.1\"))\n"
      "  Truncate(Var(\"@0.i\") Var(\"%5.1\"))\n"
      "  Binary(LeftShift Var(\"%5.1\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Load(Var(\"%8.1\") Var(\"%10.1\") false)\n"
      "  Ret(Var(\"%10.1\"))\n"
      "])\n",
      "Function(\"main\" true [\"@0.i\"] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Label(\".clear@0\")\n"
      "  Phi(Var(\"%0.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(20))) PhiArg(\".clear@0\" Var(\"%0.3\"))])\n"
      "  Binary(Subtract Var(\"%0.2\") Constant(ConstUChar(1)) Var(\"%0.3\"))\n"
      "  GetAddress(Var(\"@1.arr\") Var(\"%2.1\"))\n"
      "  IndirectIndexedStore(Var(\"%2.1\") Var(\"%0.3\") Constant(ConstUChar(0)) false)\n"
      "  JumpIfTrue(Var(\"%0.3\") \".clear@0\")\n"
      "  GetAddress(Var(\"@1.arr\") Var(\"%4.1\"))\n"
      "  Truncate(Var(\"@0.i\") Var(\"%5.1\"))\n"
      "  Binary(LeftShift Var(\"%5.1\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Load(Var(\"%8.1\") Var(\"%10.1\") false)\n"
      "  Ret(Var(\"%10.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_canonical_indexed_store_collapses (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@7.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"%30.1\", \"%31.1\", \"%32.1\", \"%33.1\", \"%34.1\", \"@7.x\", \"@8.v.1\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"row\") Var(\"@7.x\") Var(\"%28.1\") false)\n"
      "  GetAddress(Var(\"screen\") Var(\"row\"))\n"
      "  Binary(BitwiseOr Var(\"%28.1\") Constant(ConstUChar(1)) Var(\"%32.1\"))\n"
      "  IndirectIndexedStore(Var(\"row\") Var(\"@7.x\") Var(\"%32.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_offset_indexed_store_lands_correctly (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedDefersToIndexedStore.test_copy_constant_ptr_defers_load (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%ptr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestSharedByteIndex.test_runs (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@3.line\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.ra\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.n0\", \"%23.1\", \"%24.1\", \"%24.1.lo2\", \"%25.1\", \"@3.line\", \"@4.x\", \"@5.m\", \"@6.background.1\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"@3.line\") Var(\"@4.x\") Var(\"%13.1\") false)\n"
      "  Binary(Add Var(\"%12.1\") Constant(ConstInt(8192)) Var(\"%15.1\"))\n"
      "  Binary(Add Var(\"@3.line\") Constant(ConstInt(8192)) Var(\"%15.1.base\"))\n"
      "  IndirectIndexedLoad(Var(\"%15.1.base\") Var(\"@4.x\") Var(\"%16.1\") false)\n"
      "  IndexedLoad(\"mask\" Var(\"@5.m\") Var(\"%20.1\") false 0 0)\n"
      "  Binary(BitwiseAnd Var(\"%16.1\") Var(\"%20.1\") Var(\"%22.1.n0\"))\n"
      "  Binary(BitwiseOr Var(\"%13.1\") Var(\"%22.1.n0\") Var(\"%25.1\"))\n"
      "  IndirectIndexedStore(Var(\"@3.line\") Var(\"@4.x\") Var(\"%25.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@3.line\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1.base\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.base\", \"%15.1.ra\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%22.1.n0\", \"%23.1\", \"%24.1\", \"%24.1.lo2\", \"%25.1\", \"@3.line\", \"@4.x\", \"@5.m\", \"@6.background.1\"]}}}");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_other_reader_of_offset_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\", \"q\", \"value\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"q\"))\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%addr\") Var(\"q\"))\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_negative_signed_char_index (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_choice_of_member (standalone) (call 4)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"@3.r\"]}}}",
      "Function(\"get\" true [\"@3.r\"] [\n"
      "  Binary(Add Var(\"@3.r\") Constant(ConstInt(13)) Var(\"%2.1\"))\n"
      "  Load(Var(\"%2.1\") Var(\"%3.1\") false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"get\" true [\"@3.r\"] [\n"
      "  IndirectIndexedLoad(Var(\"@3.r\") Constant(ConstUChar(13)) Var(\"%3.1\") false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_const_offset_indexed_store (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedAsmShape.test_global_pointer_stored_constant_on_another_path (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.n\"]}}}",
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
      "  IndirectIndexedLoad(Var(\"P\") Constant(ConstUChar(4)) Var(\"%4.1\") false)\n"
      "  JumpIfCmp(LessThan Var(\"%4.1\") Constant(ConstUChar(21)) \".loop@0_break\")\n"
      "  FunctionCall(\"next\" [] None false false)\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_choice_of_member (standalone) (call 5)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"s\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 244, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%4.1\", \"%5.1\", \"%6.1\"]}}}",
      "Function(\"step\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(15)) Var(\"%4.1\"))\n"
      "  Load(Var(\"%4.1\") Var(\"%5.1\") false)\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(256)) Var(\"%6.1\"))\n"
      "  Store(Var(\"%6.1\") Var(\"%4.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"step\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(15)) Var(\"%4.1\"))\n"
      "  IndirectIndexedLoad(Var(\"P\") Constant(ConstUChar(15)) Var(\"%5.1\") false)\n"
      "  Binary(Add Var(\"%5.1\") Constant(ConstInt(256)) Var(\"%6.1\"))\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(15)) Var(\"%6.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_choice_of_member (standalone) (call 6)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@2.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@1.r\", \"@2.v\"]}}}",
      "Function(\"put2\" true [\"@1.r\" \"@2.v\"] [\n"
      "  Binary(Add Var(\"@1.r\") Constant(ConstInt(13)) Var(\"%1.1\"))\n"
      "  Store(Var(\"@2.v\") Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put2\" true [\"@1.r\" \"@2.v\"] [\n"
      "  IndirectIndexedStore(Var(\"@1.r\") Constant(ConstUChar(13)) Var(\"@2.v\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_non_uchar_index_does_not_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_ext_through_sign_changing_copy_folds (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%e0\", \"%ext\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%e0\"))\n"
      "  Copy(Var(\"%e0\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%e0\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Var(\"y\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_int_index_does_not_fold (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestSharedByteIndex.test_small_offset_keeps_the_sum (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%far\", \"b\", \"c\", \"y\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Binary(Add Var(\"%addr\") Constant(ConstInt(1)) Var(\"%far\"))\n"
      "  Load(Var(\"%far\") Var(\"c\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  IndirectIndexedLoad(Var(\"%addr\") Constant(ConstUChar(1)) Var(\"c\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_compound_assignment_with_a_call (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"E\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Structure(\\\"mob\\\"))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 248, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%55.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%58.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%63.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%66.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\", \"%69.1\", \"%70.1\", \"%71.1\", \"%72.1\", \"%73.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"m\") Var(\"E\"))\n"
      "  IndexedSymbolStore(\"m\" Constant(ConstUChar(10)) Constant(ConstUChar(64)) false 0)\n"
      "  FunctionCall(\"h\" [Constant(ConstUChar(2))] Var(\"%52.1\") false false)\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%54.1\"))\n"
      "  Load(Var(\"%54.1\") Var(\"%55.1\") false)\n"
      "  Binary(BitwiseOr Var(\"%55.1\") Var(\"%52.1\") Var(\"%58.1\"))\n"
      "  Store(Var(\"%58.1\") Var(\"%54.1\") false)\n"
      "  FunctionCall(\"h\" [Constant(ConstUChar(0))] Var(\"%60.1\") false false)\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%62.1\"))\n"
      "  Load(Var(\"%62.1\") Var(\"%63.1\") false)\n"
      "  Binary(Subtract Var(\"%63.1\") Var(\"%60.1\") Var(\"%66.1\"))\n"
      "  Store(Var(\"%66.1\") Var(\"%62.1\") false)\n"
      "  IndexedLoad(\"m\" Constant(ConstUChar(10)) Var(\"%69.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Binary(LeftShift Var(\"%70.1\") Constant(ConstInt(4)) Var(\"%71.1\"))\n"
      "  ZeroExtend(Var(\"calls\") Var(\"%72.1\"))\n"
      "  Binary(Add Var(\"%71.1\") Var(\"%72.1\") Var(\"%73.1\"))\n"
      "  Ret(Var(\"%73.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  GetAddress(Var(\"m\") Var(\"E\"))\n"
      "  IndexedSymbolStore(\"m\" Constant(ConstUChar(10)) Constant(ConstUChar(64)) false 0)\n"
      "  FunctionCall(\"h\" [Constant(ConstUChar(2))] Var(\"%52.1\") false false)\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%54.1\"))\n"
      "  IndirectIndexedLoad(Var(\"E\") Constant(ConstUChar(10)) Var(\"%55.1\") false)\n"
      "  Binary(BitwiseOr Var(\"%55.1\") Var(\"%52.1\") Var(\"%58.1\"))\n"
      "  IndirectIndexedStore(Var(\"E\") Constant(ConstUChar(10)) Var(\"%58.1\") false)\n"
      "  FunctionCall(\"h\" [Constant(ConstUChar(0))] Var(\"%60.1\") false false)\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%62.1\"))\n"
      "  IndirectIndexedLoad(Var(\"E\") Constant(ConstUChar(10)) Var(\"%63.1\") false)\n"
      "  Binary(Subtract Var(\"%63.1\") Var(\"%60.1\") Var(\"%66.1\"))\n"
      "  IndirectIndexedStore(Var(\"E\") Constant(ConstUChar(10)) Var(\"%66.1\") false)\n"
      "  IndexedLoad(\"m\" Constant(ConstUChar(10)) Var(\"%69.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Binary(LeftShift Var(\"%70.1\") Constant(ConstInt(4)) Var(\"%71.1\"))\n"
      "  ZeroExtend(Var(\"calls\") Var(\"%72.1\"))\n"
      "  Binary(Add Var(\"%71.1\") Var(\"%72.1\") Var(\"%73.1\"))\n"
      "  Ret(Var(\"%73.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestSharedByteIndex.test_read_and_write_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"b\", \"y\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Var(\"y\") Var(\"b\") false)\n"
      "  IndirectIndexedStore(Var(\"p\") Var(\"y\") Var(\"value\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_store_folds (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Constant(ConstInt(11)) Var(\"p\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(11)) Var(\"value\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestShapes.test_member_through_pointer_parameter (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.r\"]}}}",
      "Function(\"put\" true [\"@0.r\"] [\n"
      "  Binary(Add Var(\"@0.r\") Constant(ConstInt(13)) Var(\"%1.1\"))\n"
      "  Store(Constant(ConstUInt(4660)) Var(\"%1.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"put\" true [\"@0.r\"] [\n"
      "  IndirectIndexedStore(Var(\"@0.r\") Constant(ConstUChar(13)) Constant(ConstUInt(4660)) false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_const_offset_indexed_store (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreAsmShape.test_canonical_indexed_store_collapses (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_unoptimized_still_works (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedDefersToIndexedStore.test_var_with_no_copy_def_still_fires (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Var(\"y\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedAsmShape.test_indirect_indexed_load_emits_short_sequence (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": []}}}",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_post_decrement_becomes_dex (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_negative_signed_char_index (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_pointer_member_access.TestRuns.test_choice_of_member (standalone) (call 7)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@9.v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@10.w.3.o\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%21.1\", \"%22.1\", \"%22.2\", \"%22.3\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%29.1\", \"@10.w.1\", \"@10.w.2\", \"@10.w.3\", \"@10.w.3.e\", \"@10.w.3.o\", \"@8.n\", \"@9.v\"]}}}",
      "Function(\"choose_if\" true [\"@8.n\" \"@9.v\"] [\n"
      "  JumpIfCmp(LessOrEqual Var(\"@8.n\") Constant(ConstUChar(5)) \".and_false@2\")\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%25.1\"))\n"
      "  Load(Var(\"%25.1\") Var(\"%26.1\") false)\n"
      "  JumpIfCmp(NotEqual Var(\"%26.1\") Constant(ConstUChar(3)) \".and_false@2\")\n"
      "  Jump(\".choose_if@ssa_block@3\")\n"
      "  Label(\".and_false@2\")\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".choose_if@ssa_block@3\")\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@10.w.3.o\") [PhiArg(\".choose_if@ssa_block@3\" Constant(ConstUChar(9))) PhiArg(\".and_false@2\" Constant(ConstUChar(7)))])\n"
      "  ZeroExtend(Var(\"@10.w.3.o\") Var(\"@10.w.3.e\"))\n"
      "  Binary(Add Var(\"E\") Var(\"@10.w.3.e\") Var(\"@10.w.3\"))\n"
      "  Store(Var(\"@9.v\") Var(\"@10.w.3\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"choose_if\" true [\"@8.n\" \"@9.v\"] [\n"
      "  JumpIfCmp(LessOrEqual Var(\"@8.n\") Constant(ConstUChar(5)) \".and_false@2\")\n"
      "  IndirectIndexedLoad(Var(\"E\") Constant(ConstUChar(10)) Var(\"%26.1\") false)\n"
      "  JumpIfCmp(NotEqual Var(\"%26.1\") Constant(ConstUChar(3)) \".and_false@2\")\n"
      "  Jump(\".choose_if@ssa_block@3\")\n"
      "  Label(\".and_false@2\")\n"
      "  Jump(\".if_end@4\")\n"
      "  Label(\".choose_if@ssa_block@3\")\n"
      "  Label(\".if_end@4\")\n"
      "  Phi(Var(\"@10.w.3.o\") [PhiArg(\".choose_if@ssa_block@3\" Constant(ConstUChar(9))) PhiArg(\".and_false@2\" Constant(ConstUChar(7)))])\n"
      "  IndirectIndexedStore(Var(\"E\") Var(\"@10.w.3.o\") Var(\"@9.v\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_wide_offset_load_folds (standalone) (call 4)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(255)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(255)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (standalone) (call 4)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_read_and_write_of_one_offset_fold (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\", \"value\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(1)) Var(\"b\") false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(1)) Var(\"value\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedStoreUnit.test_constant_byte_source_folds (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Constant(ConstUChar(66)) Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedStore(Var(\"p\") Var(\"y\") Constant(ConstUChar(66)) false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_byte_lands_at_indexed_address (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_offset_load_folds (standalone) (call 3)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(255)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(255)) Var(\"b\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestSharedByteIndex.test_far_offset_reads_through_its_own_pointer (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%far\", \"b\", \"c\", \"y\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Binary(Add Var(\"%addr\") Constant(ConstInt(8192)) Var(\"%far\"))\n"
      "  Load(Var(\"%far\") Var(\"c\") false)\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"value\") Var(\"y\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Var(\"y\") Var(\"b\") false)\n"
      "  Binary(Add Var(\"%addr\") Constant(ConstInt(8192)) Var(\"%far\"))\n"
      "  Binary(Add Var(\"p\") Constant(ConstInt(8192)) Var(\"%far.base\"))\n"
      "  IndirectIndexedLoad(Var(\"%far.base\") Var(\"y\") Var(\"c\") false)\n"
      "  IndirectIndexedStore(Var(\"p\") Var(\"y\") Var(\"value\") false)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%far.base\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%far\", \"%far.base\", \"b\", \"c\", \"y\"]}}}");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedDefersToIndexedStore.test_copy_constant_ptr_defers_store (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%ptr\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(16384)) Var(\"%ptr\"))\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"%ptr\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedLoadUnit.test_multi_use_ext_folds_and_keeps_ext (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"%ext\", \"%other\", \"b\"]}}}",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Binary(Add Var(\"p\") Var(\"%ext\") Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Copy(Var(\"%ext\") Var(\"%other\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  ZeroExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  IndirectIndexedLoad(Var(\"p\") Var(\"y\") Var(\"b\") false)\n"
      "  Copy(Var(\"%ext\") Var(\"%other\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestRecognizeIndirectIndexedConstOffset.test_read_and_write_of_one_offset_fold (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"value\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"gp\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%addr\", \"b\", \"value\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  Load(Var(\"%addr\") Var(\"b\") false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  Store(Var(\"value\") Var(\"%addr\") false)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"gp\") Constant(ConstInt(1)) Var(\"%addr\"))\n"
      "  IndirectIndexedLoad(Var(\"gp\") Constant(ConstUChar(1)) Var(\"b\") false)\n"
      "  Binary(Add Var(\"b\") Constant(ConstUChar(1)) Var(\"value\"))\n"
      "  IndirectIndexedStore(Var(\"gp\") Constant(ConstUChar(1)) Var(\"value\") false)\n"
      "])\n",
      "");
}

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indexed_store.TestIndexedStoreCorrectness.test_offset_indexed_store_lands_correctly (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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

TEST_CASE("tac_recognize_indirect_indexed: test_recognize_indirect_indexed.TestIndirectIndexedCorrectness.test_store_writes_correct_byte (standalone) (call 2)") {
  c6502::testing::check_stage_case(
      "tac_recognize_indirect_indexed",
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
