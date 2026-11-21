// The recorded calls of `tac_hoist_subtract_to_compare`, the C++ port
// of passes/optimization/compare_subtract.py, with what the Python pass
// returned. They were recorded with the capture harness's wrappers
// while tests/test_compare_subtract.py ran, through the TAC optimizer's
// driver and through the Python entry points that the tests call
// directly. They are the calls that changed the function or the
// symbols, and the calls of small synthetic functions through those
// entry points. The cases after them are the calls of the whole corpus
// (tools/capture_passes.py) that changed the function, since few corpus
// calls reach the rewrite.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_hoist_subtract_to_compare: test_compare_subtract.TestCompareSubtract.test_no_separate_compare (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.s\", \"@0.s.1\", \"@0.s.2\", \"@1.k\"]}}}",
      "Function(\"f\" true [\"@0.s\" \"@1.k\"] [\n"
      "  Label(\".f@ssa_preheader@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@0.s.1\") [PhiArg(\".f@ssa_preheader@0\" Var(\"@0.s\")) PhiArg(\".f@ssa_block@0\" Var(\"%1.1\"))])\n"
      "  JumpIfCmp(LessThan Var(\"@0.s.1\") Var(\"@1.k\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Subtract Var(\"@0.s.1\") Var(\"@1.k\") Var(\"%1.1\"))\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.s.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.s\" \"@1.k\"] [\n"
      "  Label(\".f@ssa_preheader@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@0.s.1\") [PhiArg(\".f@ssa_preheader@0\" Var(\"@0.s\")) PhiArg(\".f@ssa_block@0\" Var(\"%1.1\"))])\n"
      "  Binary(Subtract Var(\"@0.s.1\") Var(\"@1.k\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@0.s.1\") Var(\"@1.k\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Binary(Add Var(\"%1.1\") Var(\"@1.k\") Var(\"@0.s.1.exit\"))\n"
      "  Ret(Var(\"@0.s.1.exit\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.s.1.exit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.s\", \"@0.s.1\", \"@0.s.1.exit\", \"@0.s.2\", \"@1.k\"]}}}");
}

TEST_CASE("tac_hoist_subtract_to_compare: test_compare_subtract.TestLoopInRegisters.test_shape (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@0.n0\", \"@1.s\", \"@1.s.1\", \"@1.s.2\"]}}}",
      "Function(\"f\" true [\"@0.n0\" \"@1.s\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"@0.n0\") Var(\"n\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@1.s.1\") [PhiArg(\".f@ssa_block@0\" Var(\"@1.s\")) PhiArg(\".f@ssa_block@1\" Var(\"%4.1\"))])\n"
      "  ZeroExtend(Var(\"k\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@1.s.1\") Var(\"%1.1\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"k\") Var(\"%3.1\"))\n"
      "  Binary(Subtract Var(\"@1.s.1\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Truncate(Var(\"@1.s.1\") Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.n0\" \"@1.s\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Copy(Var(\"@0.n0\") Var(\"n\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@1.s.1\") [PhiArg(\".f@ssa_block@0\" Var(\"@1.s\")) PhiArg(\".f@ssa_block@1\" Var(\"%4.1\"))])\n"
      "  ZeroExtend(Var(\"k\") Var(\"%1.1\"))\n"
      "  Binary(Subtract Var(\"@1.s.1\") Var(\"%1.1\") Var(\"%4.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@1.s.1\") Var(\"%1.1\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"k\") Var(\"%3.1\"))\n"
      "  Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  ZeroExtend(Var(\"k\") Var(\"%1.1.exit\"))\n"
      "  Binary(Add Var(\"%4.1\") Var(\"%1.1.exit\") Var(\"@1.s.1.exit\"))\n"
      "  Truncate(Var(\"@1.s.1.exit\") Var(\"%7.1\"))\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@1.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.s.1.exit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1.exit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.1.exit\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@0.n0\", \"@1.s\", \"@1.s.1\", \"@1.s.1.exit\", \"@1.s.2\"]}}}");
}

TEST_CASE("tac_hoist_subtract_to_compare: test_compare_subtract.TestCompareSubtract.test_no_separate_compare (call 2)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.s\", \"@0.s.1\", \"@0.s.2\", \"@1.k\"]}}}",
      "Function(\"f\" true [\"@0.s\" \"@1.k\"] [\n"
      "  Label(\".f@ssa_preheader@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@0.s.1\") [PhiArg(\".f@ssa_preheader@0\" Var(\"@0.s\")) PhiArg(\".f@ssa_block@0\" Var(\"%1.1\"))])\n"
      "  JumpIfCmp(LessThan Var(\"@0.s.1\") Var(\"@1.k\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Subtract Var(\"@0.s.1\") Var(\"@1.k\") Var(\"%1.1\"))\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Ret(Var(\"@0.s.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.s\" \"@1.k\"] [\n"
      "  Label(\".f@ssa_preheader@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@0.s.1\") [PhiArg(\".f@ssa_preheader@0\" Var(\"@0.s\")) PhiArg(\".f@ssa_block@0\" Var(\"%1.1\"))])\n"
      "  Binary(Subtract Var(\"@0.s.1\") Var(\"@1.k\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@0.s.1\") Var(\"@1.k\") \".loop@0_break\")\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Binary(Add Var(\"%1.1\") Var(\"@1.k\") Var(\"@0.s.1.exit\"))\n"
      "  Ret(Var(\"@0.s.1.exit\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.s.1.exit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.s\", \"@0.s.1\", \"@0.s.1.exit\", \"@0.s.2\", \"@1.k\"]}}}");
}

TEST_CASE("tac_hoist_subtract_to_compare: test_compare_subtract.TestCompareSubtract.test_no_separate_compare (call 3)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"@0.s\", \"@1.k\"]}}}",
      "Function(\"f\" true [\"@0.s\" \"@1.k\"] [\n"
      "  JumpIfCmp(LessThan Var(\"@0.s\") Var(\"@1.k\") \".if_end@0\")\n"
      "  Binary(Subtract Var(\"@0.s\") Var(\"@1.k\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "Function(\"f\" true [\"@0.s\" \"@1.k\"] [\n"
      "  Binary(Subtract Var(\"@0.s\") Var(\"@1.k\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@0.s\") Var(\"@1.k\") \".if_end@0\")\n"
      "  Ret(Var(\"%1.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_subtract_to_compare: test_compare_subtract.TestCompareSubtract.test_no_separate_compare (call 4)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@1.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"@0.a\", \"@1.b\"]}}}",
      "Function(\"f\" true [\"@0.a\" \"@1.b\"] [\n"
      "  JumpIfCmp(LessOrEqual Var(\"@0.a\") Var(\"@1.b\") \".if_end@0\")\n"
      "  Binary(Subtract Var(\"@1.b\") Var(\"@0.a\") Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstULong(7)))\n"
      "])\n",
      "Function(\"f\" true [\"@0.a\" \"@1.b\"] [\n"
      "  Binary(Subtract Var(\"@1.b\") Var(\"@0.a\") Var(\"%1.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"@0.a\") Var(\"@1.b\") \".if_end@0\")\n"
      "  Ret(Var(\"%1.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(Constant(ConstULong(7)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_subtract_to_compare: examples/lode_runner/wipe.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@39.n.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@41.d.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@37.x\", \"@38.c\", \"@39.n.1\", \"@39.n.2\", \"@39.n.3\", \"@39.n.4\", \"@40.q.1\", \"@40.q.2\", \"@40.q.3\", \"@40.q.4\", \"@40.q.5\", \"@41.d.1\", \"@41.d.2\", \"@41.d.3\"]}}}",
      "Function(\"div_by_7\" false [\"@37.x\" \"@38.c\"] [\n"
      "  Label(\".div_by_7@ssa_block@0\")\n"
      "  Binary(Add Var(\"@37.x\") Constant(ConstInt(35)) Var(\"%1.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@39.n.2\") [PhiArg(\".div_by_7@ssa_block@0\" Var(\"%1.1\")) PhiArg(\".loop@0_continue\" Var(\"@39.n.4\"))])\n"
      "  Phi(Var(\"@40.q.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"@40.q.5\"))])\n"
      "  Phi(Var(\"@41.d.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUInt(448))) PhiArg(\".loop@0_continue\" Var(\"%14.1\"))])\n"
      "  JumpIfCmp(LessThan Var(\"@41.d.2\") Constant(ConstUInt(7)) \".loop@0_break\")\n"
      "  Label(\".div_by_7@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@40.q.2\") Constant(ConstUChar(1)) Var(\"%9.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@39.n.2\") Var(\"@41.d.2\") \".if_end@0\")\n"
      "  Label(\".div_by_7@ssa_block@2\")\n"
      "  Binary(Subtract Var(\"@39.n.2\") Var(\"@41.d.2\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@39.n.4\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"@39.n.2\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%11.1\"))])\n"
      "  Phi(Var(\"@40.q.5\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"%9.1\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(RightShift Var(\"@41.d.2\") Constant(ConstInt(1)) Var(\"%14.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Binary(Subtract Var(\"@40.q.2\") Constant(ConstUChar(5)) Var(\"%17.1\"))\n"
      "  Store(Var(\"%17.1\") Var(\"@38.c\") false)\n"
      "  Truncate(Var(\"@39.n.2\") Var(\"%18.1\"))\n"
      "  IndirectIndexedStore(Var(\"@38.c\") Constant(ConstUChar(1)) Var(\"%18.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"div_by_7\" false [\"@37.x\" \"@38.c\"] [\n"
      "  Label(\".div_by_7@ssa_block@0\")\n"
      "  Binary(Add Var(\"@37.x\") Constant(ConstInt(35)) Var(\"%1.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@39.n.2\") [PhiArg(\".div_by_7@ssa_block@0\" Var(\"%1.1\")) PhiArg(\".loop@0_continue\" Var(\"@39.n.4\"))])\n"
      "  Phi(Var(\"@40.q.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"@40.q.5\"))])\n"
      "  Phi(Var(\"@41.d.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUInt(448))) PhiArg(\".loop@0_continue\" Var(\"%14.1\"))])\n"
      "  JumpIfCmp(LessThan Var(\"@41.d.2\") Constant(ConstUInt(7)) \".loop@0_break\")\n"
      "  Label(\".div_by_7@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@40.q.2\") Constant(ConstUChar(1)) Var(\"%9.1\"))\n"
      "  Binary(Subtract Var(\"@39.n.2\") Var(\"@41.d.2\") Var(\"%11.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@39.n.2\") Var(\"@41.d.2\") \".if_end@0\")\n"
      "  Label(\".div_by_7@ssa_block@2\")\n"
      "  Binary(Add Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@39.n.4\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"@39.n.2\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%11.1\"))])\n"
      "  Phi(Var(\"@40.q.5\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"%9.1\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(RightShift Var(\"@41.d.2\") Constant(ConstInt(1)) Var(\"%14.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Binary(Subtract Var(\"@40.q.2\") Constant(ConstUChar(5)) Var(\"%17.1\"))\n"
      "  Store(Var(\"%17.1\") Var(\"@38.c\") false)\n"
      "  Truncate(Var(\"@39.n.2\") Var(\"%18.1\"))\n"
      "  IndirectIndexedStore(Var(\"@38.c\") Constant(ConstUChar(1)) Var(\"%18.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_subtract_to_compare: examples/zork/numbers.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@24.partial.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%9.1\", \"@21.dividend\", \"@22.divisor\", \"@23.quotient.1\", \"@23.quotient.2\", \"@23.quotient.3\", \"@24.partial.1\", \"@24.partial.2\", \"@24.partial.3\", \"@24.partial.4\", \"@24.partial.5\", \"@25.i.1\", \"@25.i.2\", \"@25.i.3\", \"@26.fits.1\", \"@26.fits.2\", \"@26.fits.3\", \"@27.out.1\"]}}}",
      "Function(\"divu16\" false [\"@21.dividend\" \"@22.divisor\"] [\n"
      "  Label(\".divu16@ssa_block@0\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@23.quotient.2\") [PhiArg(\".divu16@ssa_block@0\" Var(\"@21.dividend\")) PhiArg(\".loop@1_continue\" Var(\"%21.1\"))])\n"
      "  Phi(Var(\"@24.partial.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%25.1\"))])\n"
      "  Phi(Var(\"@25.i.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUChar(17))) PhiArg(\".loop@1_continue\" Var(\"%27.1\"))])\n"
      "  JumpIfFalse(Var(\"@25.i.2\") \".loop@1_break\")\n"
      "  Label(\".divu16@ssa_block@1\")\n"
      "  JumpIfCmp(LessThan Var(\"@24.partial.2\") Var(\"@22.divisor\") \".if_end@1\")\n"
      "  Label(\".divu16@ssa_block@2\")\n"
      "  Binary(Subtract Var(\"@24.partial.2\") Var(\"@22.divisor\") Var(\"%14.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@24.partial.4\") [PhiArg(\".divu16@ssa_block@1\" Var(\"@24.partial.2\")) PhiArg(\".divu16@ssa_block@2\" Var(\"%14.1\"))])\n"
      "  Phi(Var(\"@26.fits.3\") [PhiArg(\".divu16@ssa_block@1\" Constant(ConstUChar(0))) PhiArg(\".divu16@ssa_block@2\" Constant(ConstUChar(1)))])\n"
      "  Binary(RightShift Var(\"@23.quotient.2\") Constant(ConstInt(15)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(LeftShift Var(\"@23.quotient.2\") Constant(ConstInt(1)) Var(\"%18.1\"))\n"
      "  ZeroExtend(Var(\"@26.fits.3\") Var(\"%20.1\"))\n"
      "  Binary(BitwiseOr Var(\"%18.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"@24.partial.4\") Constant(ConstInt(1)) Var(\"%22.1\"))\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%24.1\"))\n"
      "  Binary(BitwiseOr Var(\"%22.1\") Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Subtract Var(\"@25.i.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Binary(RightShift Var(\"@24.partial.2\") Constant(ConstInt(1)) Var(\"remainder\"))\n"
      "  Ret(Var(\"@23.quotient.2\"))\n"
      "])\n",
      "Function(\"divu16\" false [\"@21.dividend\" \"@22.divisor\"] [\n"
      "  Label(\".divu16@ssa_block@0\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@23.quotient.2\") [PhiArg(\".divu16@ssa_block@0\" Var(\"@21.dividend\")) PhiArg(\".loop@1_continue\" Var(\"%21.1\"))])\n"
      "  Phi(Var(\"@24.partial.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%25.1\"))])\n"
      "  Phi(Var(\"@25.i.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUChar(17))) PhiArg(\".loop@1_continue\" Var(\"%27.1\"))])\n"
      "  JumpIfFalse(Var(\"@25.i.2\") \".loop@1_break\")\n"
      "  Label(\".divu16@ssa_block@1\")\n"
      "  Binary(Subtract Var(\"@24.partial.2\") Var(\"@22.divisor\") Var(\"%14.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@24.partial.2\") Var(\"@22.divisor\") \".if_end@1\")\n"
      "  Label(\".divu16@ssa_block@2\")\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@24.partial.4\") [PhiArg(\".divu16@ssa_block@1\" Var(\"@24.partial.2\")) PhiArg(\".divu16@ssa_block@2\" Var(\"%14.1\"))])\n"
      "  Phi(Var(\"@26.fits.3\") [PhiArg(\".divu16@ssa_block@1\" Constant(ConstUChar(0))) PhiArg(\".divu16@ssa_block@2\" Constant(ConstUChar(1)))])\n"
      "  Binary(RightShift Var(\"@23.quotient.2\") Constant(ConstInt(15)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(LeftShift Var(\"@23.quotient.2\") Constant(ConstInt(1)) Var(\"%18.1\"))\n"
      "  ZeroExtend(Var(\"@26.fits.3\") Var(\"%20.1\"))\n"
      "  Binary(BitwiseOr Var(\"%18.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"@24.partial.4\") Constant(ConstInt(1)) Var(\"%22.1\"))\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%24.1\"))\n"
      "  Binary(BitwiseOr Var(\"%22.1\") Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Subtract Var(\"@25.i.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Binary(RightShift Var(\"@24.partial.2\") Constant(ConstInt(1)) Var(\"remainder\"))\n"
      "  Ret(Var(\"@23.quotient.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_subtract_to_compare: examples/zork/numbers.c -S -O (call 2)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@24.partial.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%22.1\", \"%23.1\", \"%24.1\", \"%25.1\", \"%26.1\", \"%27.1\", \"%28.1\", \"%9.1\", \"@21.dividend\", \"@22.divisor\", \"@23.quotient.1\", \"@23.quotient.2\", \"@23.quotient.3\", \"@24.partial.1\", \"@24.partial.2\", \"@24.partial.3\", \"@24.partial.4\", \"@24.partial.5\", \"@25.i.1\", \"@25.i.2\", \"@25.i.3\", \"@26.fits.1\", \"@26.fits.2\", \"@26.fits.3\", \"@27.out.1\"]}}}",
      "Function(\"divu16\" false [\"@21.dividend\" \"@22.divisor\"] [\n"
      "  Label(\".divu16@ssa_block@0\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@23.quotient.2\") [PhiArg(\".divu16@ssa_block@0\" Var(\"@21.dividend\")) PhiArg(\".loop@1_continue\" Var(\"%21.1\"))])\n"
      "  Phi(Var(\"@24.partial.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%25.1\"))])\n"
      "  Phi(Var(\"@25.i.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUChar(17))) PhiArg(\".loop@1_continue\" Var(\"%27.1\"))])\n"
      "  JumpIfFalse(Var(\"@25.i.2\") \".loop@1_break\")\n"
      "  Label(\".divu16@ssa_block@1\")\n"
      "  JumpIfCmp(LessThan Var(\"@24.partial.2\") Var(\"@22.divisor\") \".if_end@1\")\n"
      "  Label(\".divu16@ssa_block@2\")\n"
      "  Binary(Subtract Var(\"@24.partial.2\") Var(\"@22.divisor\") Var(\"%14.1\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@24.partial.4\") [PhiArg(\".divu16@ssa_block@1\" Var(\"@24.partial.2\")) PhiArg(\".divu16@ssa_block@2\" Var(\"%14.1\"))])\n"
      "  Phi(Var(\"@26.fits.3\") [PhiArg(\".divu16@ssa_block@1\" Constant(ConstUChar(0))) PhiArg(\".divu16@ssa_block@2\" Constant(ConstUChar(1)))])\n"
      "  Binary(RightShift Var(\"@23.quotient.2\") Constant(ConstInt(15)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(LeftShift Var(\"@23.quotient.2\") Constant(ConstInt(1)) Var(\"%18.1\"))\n"
      "  ZeroExtend(Var(\"@26.fits.3\") Var(\"%20.1\"))\n"
      "  Binary(BitwiseOr Var(\"%18.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"@24.partial.4\") Constant(ConstInt(1)) Var(\"%22.1\"))\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%24.1\"))\n"
      "  Binary(BitwiseOr Var(\"%22.1\") Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Subtract Var(\"@25.i.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Binary(RightShift Var(\"@24.partial.2\") Constant(ConstInt(1)) Var(\"remainder\"))\n"
      "  Ret(Var(\"@23.quotient.2\"))\n"
      "])\n",
      "Function(\"divu16\" false [\"@21.dividend\" \"@22.divisor\"] [\n"
      "  Label(\".divu16@ssa_block@0\")\n"
      "  Label(\".loop@1_start\")\n"
      "  Phi(Var(\"@23.quotient.2\") [PhiArg(\".divu16@ssa_block@0\" Var(\"@21.dividend\")) PhiArg(\".loop@1_continue\" Var(\"%21.1\"))])\n"
      "  Phi(Var(\"@24.partial.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUInt(0))) PhiArg(\".loop@1_continue\" Var(\"%25.1\"))])\n"
      "  Phi(Var(\"@25.i.2\") [PhiArg(\".divu16@ssa_block@0\" Constant(ConstUChar(17))) PhiArg(\".loop@1_continue\" Var(\"%27.1\"))])\n"
      "  JumpIfFalse(Var(\"@25.i.2\") \".loop@1_break\")\n"
      "  Label(\".divu16@ssa_block@1\")\n"
      "  Binary(Subtract Var(\"@24.partial.2\") Var(\"@22.divisor\") Var(\"%14.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@24.partial.2\") Var(\"@22.divisor\") \".if_end@1\")\n"
      "  Label(\".divu16@ssa_block@2\")\n"
      "  Label(\".if_end@1\")\n"
      "  Phi(Var(\"@24.partial.4\") [PhiArg(\".divu16@ssa_block@1\" Var(\"@24.partial.2\")) PhiArg(\".divu16@ssa_block@2\" Var(\"%14.1\"))])\n"
      "  Phi(Var(\"@26.fits.3\") [PhiArg(\".divu16@ssa_block@1\" Constant(ConstUChar(0))) PhiArg(\".divu16@ssa_block@2\" Constant(ConstUChar(1)))])\n"
      "  Binary(RightShift Var(\"@23.quotient.2\") Constant(ConstInt(15)) Var(\"%16.1\"))\n"
      "  Truncate(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "  Binary(LeftShift Var(\"@23.quotient.2\") Constant(ConstInt(1)) Var(\"%18.1\"))\n"
      "  ZeroExtend(Var(\"@26.fits.3\") Var(\"%20.1\"))\n"
      "  Binary(BitwiseOr Var(\"%18.1\") Var(\"%20.1\") Var(\"%21.1\"))\n"
      "  Binary(LeftShift Var(\"@24.partial.4\") Constant(ConstInt(1)) Var(\"%22.1\"))\n"
      "  ZeroExtend(Var(\"%17.1\") Var(\"%24.1\"))\n"
      "  Binary(BitwiseOr Var(\"%22.1\") Var(\"%24.1\") Var(\"%25.1\"))\n"
      "  Label(\".loop@1_continue\")\n"
      "  Binary(Subtract Var(\"@25.i.2\") Constant(ConstUChar(1)) Var(\"%27.1\"))\n"
      "  Jump(\".loop@1_start\")\n"
      "  Label(\".loop@1_break\")\n"
      "  Binary(RightShift Var(\"@24.partial.2\") Constant(ConstInt(1)) Var(\"remainder\"))\n"
      "  Ret(Var(\"@23.quotient.2\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_subtract_to_compare: examples/zork/disk.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"SECTORS_PER_TRACK\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 127, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@19.s.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@18.command\", \"@19.s.1\", \"@19.s.2\", \"@19.s.3\"]}}}",
      "Function(\"do_rwts_on_sector\" false [\"@18.command\"] [\n"
      "  Label(\".do_rwts_on_sector@ssa_block@0\")\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(12)) Var(\"@18.command\") false 0)\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(8)) Var(\"disk_buffer\") false 0)\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(4)) Constant(ConstUChar(3)) false 0)\n"
      "  Copy(Var(\"disk_sector\") Var(\"@19.s.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@19.s.2\") [PhiArg(\".do_rwts_on_sector@ssa_block@0\" Var(\"@19.s.1\")) PhiArg(\".do_rwts_on_sector@ssa_block@1\" Var(\"%11.1\"))])\n"
      "  ZeroExtend(Var(\"SECTORS_PER_TRACK\") Var(\"%8.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@19.s.2\") Var(\"%8.1\") \".loop@0_break\")\n"
      "  Label(\".do_rwts_on_sector@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"SECTORS_PER_TRACK\") Var(\"%10.1\"))\n"
      "  Binary(Subtract Var(\"@19.s.2\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  IndexedLoad(\"iob\" Constant(ConstUChar(4)) Var(\"%14.1\") false 0 0)\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstUChar(1)) Var(\"%15.1\"))\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(4)) Var(\"%15.1\") false 0)\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Truncate(Var(\"@19.s.2\") Var(\"%16.1\"))\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(5)) Var(\"%16.1\") false 0)\n"
      "  GetAddress(Var(\"iob\") Var(\"%19.1\"))\n"
      "  FunctionCall(\"RWTS\" [Var(\"%19.1\")] Var(\"%20.1\") false false)\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "Function(\"do_rwts_on_sector\" false [\"@18.command\"] [\n"
      "  Label(\".do_rwts_on_sector@ssa_block@0\")\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(12)) Var(\"@18.command\") false 0)\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(8)) Var(\"disk_buffer\") false 0)\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(4)) Constant(ConstUChar(3)) false 0)\n"
      "  Copy(Var(\"disk_sector\") Var(\"@19.s.1\"))\n"
      "  Label(\".loop@0_continue\")\n"
      "  Phi(Var(\"@19.s.2\") [PhiArg(\".do_rwts_on_sector@ssa_block@0\" Var(\"@19.s.1\")) PhiArg(\".do_rwts_on_sector@ssa_block@1\" Var(\"%11.1\"))])\n"
      "  ZeroExtend(Var(\"SECTORS_PER_TRACK\") Var(\"%8.1\"))\n"
      "  Binary(Subtract Var(\"@19.s.2\") Var(\"%8.1\") Var(\"%11.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@19.s.2\") Var(\"%8.1\") \".loop@0_break\")\n"
      "  Label(\".do_rwts_on_sector@ssa_block@1\")\n"
      "  ZeroExtend(Var(\"SECTORS_PER_TRACK\") Var(\"%10.1\"))\n"
      "  IndexedLoad(\"iob\" Constant(ConstUChar(4)) Var(\"%14.1\") false 0 0)\n"
      "  Binary(Add Var(\"%14.1\") Constant(ConstUChar(1)) Var(\"%15.1\"))\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(4)) Var(\"%15.1\") false 0)\n"
      "  Jump(\".loop@0_continue\")\n"
      "  Label(\".loop@0_break\")\n"
      "  ZeroExtend(Var(\"SECTORS_PER_TRACK\") Var(\"%8.1.exit\"))\n"
      "  Binary(Add Var(\"%11.1\") Var(\"%8.1.exit\") Var(\"@19.s.2.exit\"))\n"
      "  Truncate(Var(\"@19.s.2.exit\") Var(\"%16.1\"))\n"
      "  IndexedSymbolStore(\"iob\" Constant(ConstUChar(5)) Var(\"%16.1\") false 0)\n"
      "  GetAddress(Var(\"iob\") Var(\"%19.1\"))\n"
      "  FunctionCall(\"RWTS\" [Var(\"%19.1\")] Var(\"%20.1\") false false)\n"
      "  Ret(Var(\"%20.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"SECTORS_PER_TRACK\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 127, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"@19.s.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@19.s.2.exit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1.exit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\","
      " \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%8.1.exit\", \"%9.1\", \"@18.command\", \"@19.s.1\", \"@19.s.2\", \"@19.s.2.exit\", \"@19.s.3\"]}}}");
}

TEST_CASE("tac_hoist_subtract_to_compare: examples/lode_runner/wipe.c -S -O (call 2)") {
  c6502::testing::check_stage_case(
      "tac_hoist_subtract_to_compare",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@39.n.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@41.d.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@37.x\", \"@38.c\", \"@39.n.1\", \"@39.n.2\", \"@39.n.3\", \"@39.n.4\", \"@40.q.1\", \"@40.q.2\", \"@40.q.3\", \"@40.q.4\", \"@40.q.5\", \"@41.d.1\", \"@41.d.2\", \"@41.d.3\"]}}}",
      "Function(\"div_by_7\" false [\"@37.x\" \"@38.c\"] [\n"
      "  Label(\".div_by_7@ssa_block@0\")\n"
      "  Binary(Add Var(\"@37.x\") Constant(ConstInt(35)) Var(\"%1.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@39.n.2\") [PhiArg(\".div_by_7@ssa_block@0\" Var(\"%1.1\")) PhiArg(\".loop@0_continue\" Var(\"@39.n.4\"))])\n"
      "  Phi(Var(\"@40.q.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"@40.q.5\"))])\n"
      "  Phi(Var(\"@41.d.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUInt(448))) PhiArg(\".loop@0_continue\" Var(\"%14.1\"))])\n"
      "  JumpIfCmp(LessThan Var(\"@41.d.2\") Constant(ConstUInt(7)) \".loop@0_break\")\n"
      "  Label(\".div_by_7@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@40.q.2\") Constant(ConstUChar(1)) Var(\"%9.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@39.n.2\") Var(\"@41.d.2\") \".if_end@0\")\n"
      "  Label(\".div_by_7@ssa_block@2\")\n"
      "  Binary(Subtract Var(\"@39.n.2\") Var(\"@41.d.2\") Var(\"%11.1\"))\n"
      "  Binary(Add Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@39.n.4\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"@39.n.2\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%11.1\"))])\n"
      "  Phi(Var(\"@40.q.5\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"%9.1\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(RightShift Var(\"@41.d.2\") Constant(ConstInt(1)) Var(\"%14.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Binary(Subtract Var(\"@40.q.2\") Constant(ConstUChar(5)) Var(\"%17.1\"))\n"
      "  Store(Var(\"%17.1\") Var(\"@38.c\") false)\n"
      "  Truncate(Var(\"@39.n.2\") Var(\"%18.1\"))\n"
      "  IndirectIndexedStore(Var(\"@38.c\") Constant(ConstUChar(1)) Var(\"%18.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"div_by_7\" false [\"@37.x\" \"@38.c\"] [\n"
      "  Label(\".div_by_7@ssa_block@0\")\n"
      "  Binary(Add Var(\"@37.x\") Constant(ConstInt(35)) Var(\"%1.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@39.n.2\") [PhiArg(\".div_by_7@ssa_block@0\" Var(\"%1.1\")) PhiArg(\".loop@0_continue\" Var(\"@39.n.4\"))])\n"
      "  Phi(Var(\"@40.q.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"@40.q.5\"))])\n"
      "  Phi(Var(\"@41.d.2\") [PhiArg(\".div_by_7@ssa_block@0\" Constant(ConstUInt(448))) PhiArg(\".loop@0_continue\" Var(\"%14.1\"))])\n"
      "  JumpIfCmp(LessThan Var(\"@41.d.2\") Constant(ConstUInt(7)) \".loop@0_break\")\n"
      "  Label(\".div_by_7@ssa_block@1\")\n"
      "  Binary(LeftShift Var(\"@40.q.2\") Constant(ConstUChar(1)) Var(\"%9.1\"))\n"
      "  Binary(Subtract Var(\"@39.n.2\") Var(\"@41.d.2\") Var(\"%11.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"@39.n.2\") Var(\"@41.d.2\") \".if_end@0\")\n"
      "  Label(\".div_by_7@ssa_block@2\")\n"
      "  Binary(Add Var(\"%9.1\") Constant(ConstUChar(1)) Var(\"%13.1\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Phi(Var(\"@39.n.4\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"@39.n.2\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%11.1\"))])\n"
      "  Phi(Var(\"@40.q.5\") [PhiArg(\".div_by_7@ssa_block@1\" Var(\"%9.1\")) PhiArg(\".div_by_7@ssa_block@2\" Var(\"%13.1\"))])\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(RightShift Var(\"@41.d.2\") Constant(ConstInt(1)) Var(\"%14.1\"))\n"
      "  Jump(\".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Binary(Subtract Var(\"@40.q.2\") Constant(ConstUChar(5)) Var(\"%17.1\"))\n"
      "  Store(Var(\"%17.1\") Var(\"@38.c\") false)\n"
      "  Truncate(Var(\"@39.n.2\") Var(\"%18.1\"))\n"
      "  IndirectIndexedStore(Var(\"@38.c\") Constant(ConstUChar(1)) Var(\"%18.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}
