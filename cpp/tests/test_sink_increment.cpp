// The recorded calls of `tac_sink_increments`, the C++ port of
// passes/optimization/sink_increment.py, with what the Python pass
// returned. They were recorded with the capture harness's wrappers
// while tests/test_sink_increment.py ran, through the TAC optimizer's
// driver and through the Python entry points that the tests call
// directly. They are the calls that changed the function or the
// symbols, and the calls of small synthetic functions through those
// entry points.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkIncrementSim.test_src_y_postincrement_chain (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.y\", \"@0.y.1\", \"@0.y.2\"]}}}",
      "Function(\"copy3\" true [\"@0.y\"] [\n"
      "  Binary(Add Var(\"@0.y\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"@0.y\") Var(\"%4.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"%4.1\") false 0)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%1.1\") Var(\"%9.1\") false 0 0)\n"
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

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@5.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.n.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%24.1\", \"%25.1\", \"%27.1\", \"@4.n\", \"@4.n.1\", \"@4.n.2\", \"@5.k.1\", \"@5.k.2\", \"@5.k.3\"]}}}",
      "Function(\"wdown\" true [\"@4.n\"] [\n"
      "  Label(\".wdown@ssa_block@0\")\n"
      "  Label(\".loop@2_continue\")\n"
      "  Phi(Var(\"@5.k.2\") [PhiArg(\".wdown@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".wdown@ssa_block@1\" Var(\"%27.1\"))])\n"
      "  Phi(Var(\"@4.n.1\") [PhiArg(\".wdown@ssa_block@0\" Var(\"@4.n\")) PhiArg(\".wdown@ssa_block@1\" Var(\"%25.1\"))])\n"
      "  Binary(Subtract Var(\"@4.n.1\") Constant(ConstUInt(1)) Var(\"%25.1\"))\n"
      "  JumpIfFalse(Var(\"@4.n.1\") \".loop@2_break\")\n"
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

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkIncrementSim.test_post_decrement_becomes_dex (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.x.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.c\", \"@1.x.1\", \"@1.x.2\", \"@1.x.3\"]}}}",
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
      "  JumpIfTrue(Var(\"@1.x.2\") \".loop@0_start\")\n"
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

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@3.k.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.n.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"@2.n\", \"@2.n.1\", \"@2.n.2\", \"@3.k.1\", \"@3.k.2\", \"@3.k.3\"]}}}",
      "Function(\"sdown\" true [\"@2.n\"] [\n"
      "  Label(\".sdown@ssa_block@0\")\n"
      "  Label(\".loop@1_continue\")\n"
      "  Phi(Var(\"@3.k.2\") [PhiArg(\".sdown@ssa_block@0\" Constant(ConstChar(0))) PhiArg(\".sdown@ssa_block@1\" Var(\"%22.1\"))])\n"
      "  Phi(Var(\"@2.n.1\") [PhiArg(\".sdown@ssa_block@0\" Var(\"@2.n\")) PhiArg(\".sdown@ssa_block@1\" Var(\"%17.1\"))])\n"
      "  Binary(Subtract Var(\"@2.n.1\") Constant(ConstChar(1)) Var(\"%17.1\"))\n"
      "  SignExtend(Var(\"@2.n.1\") Var(\"%18.1\"))\n"
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

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkGates.test_write_to_source_ends_scan_without_ssa_info (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"s\") Constant(ConstUChar(1)) Var(\"t\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"s\"))\n"
      "  Copy(Var(\"s\") Var(\"g\"))\n"
      "  Copy(Var(\"t\") Var(\"h\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"s\") Constant(ConstUChar(1)) Var(\"t\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"s\"))\n"
      "  Copy(Var(\"s\") Var(\"g\"))\n"
      "  Copy(Var(\"t\") Var(\"h\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkIncrementSim.test_src_y_postincrement_chain (call 2)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.y\", \"@0.y.1\", \"@0.y.2\"]}}}",
      "Function(\"copy3\" true [\"@0.y\"] [\n"
      "  Binary(Add Var(\"@0.y\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "  ZeroExtend(Var(\"@0.y\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"@0.y\") Var(\"%3.1\"))\n"
      "  IndexedLoad(\"tab\" Var(\"%3.1\") Var(\"%4.1\") false 0 0)\n"
      "  IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"%4.1\") false 0)\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  ZeroExtend(Var(\"%1.1\") Var(\"%7.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"%8.1\"))\n"
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

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkGates.test_sinks_ssa_add_past_read (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"x.1\", \"y.1\", \"z.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"x.1\") Constant(ConstUChar(1)) Var(\"y.1\"))\n"
      "  Copy(Var(\"x.1\") Var(\"z.1\"))\n"
      "  Ret(Var(\"y.1\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"x.1\") Var(\"z.1\"))\n"
      "  Binary(Add Var(\"x.1\") Constant(ConstUChar(1)) Var(\"y.1\"))\n"
      "  Ret(Var(\"y.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkIncrementSim.test_post_decrement_tests (call 3)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.x.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.c\", \"@1.x.1\", \"@1.x.2\", \"@1.x.3\"]}}}",
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
      "  JumpIfTrue(Var(\"@1.x.2\") \".loop@0_start\")\n"
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

TEST_CASE("tac_sink_increments: test_sink_increment.TestSinkGates.test_non_ssa_source_stays (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_sink_increments",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": {\"set\": [\"t.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"s\") Constant(ConstUChar(1)) Var(\"t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"s\"))\n"
      "  Copy(Var(\"s\") Var(\"g\"))\n"
      "  Copy(Var(\"t.1\") Var(\"h\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"s\") Constant(ConstUChar(1)) Var(\"t.1\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"s\"))\n"
      "  Copy(Var(\"s\") Var(\"g\"))\n"
      "  Copy(Var(\"t.1\") Var(\"h\"))\n"
      "])\n",
      "");
}
