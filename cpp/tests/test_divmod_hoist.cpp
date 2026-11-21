// The recorded calls of `tac_hoist_divmod_pairs`, the C++ port of
// passes/optimization/divmod_hoist.py, with what the Python pass
// returned. They were recorded with the capture harness's wrappers
// while tests/test_divmod_hoist.py ran, through the TAC optimizer's
// driver and through the Python entry points that the tests call
// directly. They are the calls that changed the function or the
// symbols, and the calls of small synthetic functions through those
// entry points. The cases after them are the calls of the whole corpus
// (tools/capture_passes.py) that changed the function, since few corpus
// calls reach the rewrite. Three tests give a symbol a `StaticAttr`
// with no initial value, which the harness's codec can't carry, so
// those records have the `NoInitializer` that the pass doesn't read in
// its place.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestHoistDivmodPairs.test_divide_by_zero_never_hoisted (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"p\", \"q\", \"r\", \"s\", \"t\"]}}}",
      "Function(\"f\" true [\"s\"] [\n"
      "  Binary(Modulo Var(\"s\") Constant(ConstUChar(0)) Var(\"r\"))\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(5)) Var(\"r\") false)\n"
      "  Binary(Divide Var(\"s\") Constant(ConstUChar(0)) Var(\"q\"))\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "Function(\"f\" true [\"s\"] [\n"
      "  Binary(Modulo Var(\"s\") Constant(ConstUChar(0)) Var(\"r\"))\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(5)) Var(\"r\") false)\n"
      "  Binary(Divide Var(\"s\") Constant(ConstUChar(0)) Var(\"q\"))\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestHoistDivmodPairs.test_branch_between_on_a_global (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"V\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"Q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"R\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"p\", \"q\", \"r\", \"s\", \"t\"]}}}",
      "Function(\"f\" true [\"s\"] [\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".end\")\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"Q\"))\n"
      "  Label(\".end\")\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"R\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"s\"] [\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"Q.dm0\"))\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"R.dm0\"))\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".end\")\n"
      "  Copy(Var(\"Q.dm0\") Var(\"Q\"))\n"
      "  Label(\".end\")\n"
      "  Copy(Var(\"R.dm0\") Var(\"R\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"V\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"Q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"R\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"Q.dm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"R.dm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"Q.dm0\", \"R.dm0\", \"p\", \"q\", \"r\", \"s\", \"t\"]}}}");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestHoistDivmodPairs.test_global_kept_across_a_call (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"V\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"p\", \"q\", \"r\", \"s\", \"t\"]}}}",
      "Function(\"f\" true [\"s\"] [\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".end\")\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"Q\"))\n"
      "  FunctionCall(\"g\" [] None false false)\n"
      "  Label(\".end\")\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"R\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [\"s\"] [\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".end\")\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"Q\"))\n"
      "  FunctionCall(\"g\" [] None false false)\n"
      "  Label(\".end\")\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"R\"))\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestDivmodHoistSim.test_runs (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%1.1.nb0\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%5.1.nb0\", \"%6.1\", \"%6.1.lo2\", \"%7.1\", \"%8.1\", \"@0.s\"]}}}",
      "Function(\"g\" true [\"@0.s\"] [\n"
      "  Binary(Modulo Var(\"@0.s\") Constant(ConstUChar(13)) Var(\"%1.1.nb0\"))\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(5)) Var(\"%1.1.nb0\") false)\n"
      "  Binary(Divide Var(\"@0.s\") Constant(ConstUChar(13)) Var(\"%5.1.nb0\"))\n"
      "  Binary(Add Constant(ConstUChar(17)) Var(\"%5.1.nb0\") Var(\"%7.1\"))\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(4)) Var(\"%7.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"g\" true [\"@0.s\"] [\n"
      "  Binary(Modulo Var(\"@0.s\") Constant(ConstUChar(13)) Var(\"%1.1.nb0\"))\n"
      "  Binary(Divide Var(\"@0.s\") Constant(ConstUChar(13)) Var(\"%5.1.nb0\"))\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(5)) Var(\"%1.1.nb0\") false)\n"
      "  Binary(Add Constant(ConstUChar(17)) Var(\"%5.1.nb0\") Var(\"%7.1\"))\n"
      "  IndirectIndexedStore(Var(\"P\") Constant(ConstUChar(4)) Var(\"%7.1\") false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestHoistDivmodPairs.test_global_kept_across_a_pointer_store (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"V\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"p\", \"q\", \"r\", \"s\", \"t\"]}}}",
      "Function(\"f\" true [\"s\"] [\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(13)) Var(\"r\"))\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(5)) Var(\"r\") false)\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(13)) Var(\"q\"))\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "Function(\"f\" true [\"s\"] [\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(13)) Var(\"r\"))\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(5)) Var(\"r\") false)\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(13)) Var(\"q\"))\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestHoistDivmodPairs.test_store_between_on_a_local (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"p\", \"q\", \"r\", \"s\", \"t\"]}}}",
      "Function(\"f\" true [\"s\"] [\n"
      "  Binary(Modulo Var(\"s\") Constant(ConstUChar(13)) Var(\"r\"))\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(5)) Var(\"r\") false)\n"
      "  Binary(Divide Var(\"s\") Constant(ConstUChar(13)) Var(\"q\"))\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "Function(\"f\" true [\"s\"] [\n"
      "  Binary(Modulo Var(\"s\") Constant(ConstUChar(13)) Var(\"r\"))\n"
      "  Binary(Divide Var(\"s\") Constant(ConstUChar(13)) Var(\"q\"))\n"
      "  IndirectIndexedStore(Var(\"p\") Constant(ConstUChar(5)) Var(\"r\") false)\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestDivmodHoistSim.test_runs (call 2)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"V\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%17.1\", \"%18.1\", \"%19.1\", \"%20.1\", \"%21.1\", \"%21.1.nb0\", \"%22.1\", \"%22.1.lo2\", \"%23.1\", \"%24.1\", \"%25.1\", \"%25.1.nb0\", \"%26.1\", \"%27.1\", \"%27.1.lo1\", \"%28.1\", \"@1.sel\"]}}}",
      "Function(\"h\" true [\"@1.sel\"] [\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".if_end@1\")\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"%21.1.nb0\"))\n"
      "  Binary(Add Var(\"@1.sel\") Var(\"%21.1.nb0\") Var(\"S\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"%25.1.nb0\"))\n"
      "  Binary(LeftShift Var(\"%25.1.nb0\") Constant(ConstUChar(2)) Var(\"%27.1.lo1\"))\n"
      "  Binary(Add Var(\"%27.1.lo1\") Constant(ConstUChar(32)) Var(\"%28.1\"))\n"
      "  Ret(Var(\"%28.1\"))\n"
      "])\n",
      "Function(\"h\" true [\"@1.sel\"] [\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"%21.1.nb0\"))\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"%25.1.nb0\"))\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".if_end@1\")\n"
      "  Binary(Add Var(\"@1.sel\") Var(\"%21.1.nb0\") Var(\"S\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Binary(LeftShift Var(\"%25.1.nb0\") Constant(ConstUChar(2)) Var(\"%27.1.lo1\"))\n"
      "  Binary(Add Var(\"%27.1.lo1\") Constant(ConstUChar(32)) Var(\"%28.1\"))\n"
      "  Ret(Var(\"%28.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestHoistDivmodPairs.test_kept_when_a_path_computes_neither (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"p\", \"q\", \"r\", \"s\", \"t\"]}}}",
      "Function(\"f\" true [\"s\"] [\n"
      "  JumpIfCmp(LessThan Var(\"s\") Constant(ConstUChar(24)) \".a\")\n"
      "  JumpIfTrue(Var(\"t\") \".b\")\n"
      "  Ret(None)\n"
      "  Label(\".b\")\n"
      "  Binary(Modulo Var(\"s\") Constant(ConstUChar(24)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "  Label(\".a\")\n"
      "  Binary(Divide Var(\"s\") Constant(ConstUChar(24)) Var(\"q\"))\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "Function(\"f\" true [\"s\"] [\n"
      "  JumpIfCmp(LessThan Var(\"s\") Constant(ConstUChar(24)) \".a\")\n"
      "  JumpIfTrue(Var(\"t\") \".b\")\n"
      "  Ret(None)\n"
      "  Label(\".b\")\n"
      "  Binary(Modulo Var(\"s\") Constant(ConstUChar(24)) Var(\"r\"))\n"
      "  Ret(Var(\"r\"))\n"
      "  Label(\".a\")\n"
      "  Binary(Divide Var(\"s\") Constant(ConstUChar(24)) Var(\"q\"))\n"
      "  Ret(Var(\"q\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: test_divmod_hoist.TestDivmodHoistSim.test_runs (call 3)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"V\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"Q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"R\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%12.1.nb0\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.nb0\", \"%16.1\", \"%9.1\"]}}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".if_end@0\")\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"Q\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"R\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Divide Var(\"V\") Constant(ConstUChar(24)) Var(\"Q.dm0\"))\n"
      "  Binary(Modulo Var(\"V\") Constant(ConstUChar(24)) Var(\"R.dm0\"))\n"
      "  JumpIfCmp(LessThan Var(\"V\") Constant(ConstUChar(24)) \".if_end@0\")\n"
      "  Copy(Var(\"Q.dm0\") Var(\"Q\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Copy(Var(\"R.dm0\") Var(\"R\"))\n"
      "  Ret(None)\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"V\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"Q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"R\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"Q.dm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"R.dm0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%10.1\", \"%11.1\", \"%12.1\", \"%12.1.nb0\", \"%13.1\", \"%14.1\", \"%15.1\", \"%15.1.nb0\", \"%16.1\", \"%9.1\", \"Q.dm0\", \"R.dm0\"]}}}");
}

TEST_CASE("tac_hoist_divmod_pairs: examples/ali_baba/map.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%251.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%246.1\", \"%247.1\", \"%248.1\", \"%249.1\", \"%250.1\", \"%251.1\", \"%252.1\", \"%253.1\", \"%253.1.nb0\", \"%254.1\", \"%255.1\", \"%256.1\", \"%257.1\", \"%258.1\", \"%258.1.nb0\", \"%259.1\", \"%259.1.lo2\", \"%260.1\", \"%261.1\", \"%262.1\", \"%263.1\", \"%264.1\", \"%265.1\", \"%266.1\", \"@81.scene\", \"@82.iob.1\", \"@83.sector.1\", \"@84.tries.1\", \"@84.tries.2\", \"@84.tries.3\"]}}}",
      "Function(\"load_scene_from_disk\" false [\"@81.scene\"] [\n"
      "  Label(\".load_scene_from_disk@ssa_block@0\")\n"
      "  Copy(Var(\"RWTS_IOB_PTR\") Var(\"@82.iob.1\"))\n"
      "  GetAddress(Var(\"SCENE_TEXT_DATA\") Var(\"%246.1\"))\n"
      "  IndirectIndexedStore(Var(\"@82.iob.1\") Constant(ConstUChar(8)) Var(\"%246.1\") false)\n"
      "  Binary(Add Var(\"GAME_SIG_SECTOR\") Var(\"@81.scene\") Var(\"%251.1\"))\n"
      "  Binary(Modulo Var(\"%251.1\") Constant(ConstUChar(13)) Var(\"%253.1.nb0\"))\n"
      "  IndirectIndexedStore(Var(\"@82.iob.1\") Constant(ConstUChar(5)) Var(\"%253.1.nb0\") false)\n"
      "  Binary(Divide Var(\"%251.1\") Constant(ConstUChar(13)) Var(\"%258.1.nb0\"))\n"
      "  Binary(Add Var(\"GAME_SIG_TRACK\") Var(\"%258.1.nb0\") Var(\"%260.1\"))\n"
      "  IndirectIndexedStore(Var(\"@82.iob.1\") Constant(ConstUChar(4)) Var(\"%260.1\") false)\n"
      "  Label(\".loop@6_continue\")\n"
      "  Phi(Var(\"@84.tries.2\") [PhiArg(\".load_scene_from_disk@ssa_block@0\" Constant(ConstUChar(2))) PhiArg(\".if_end@31\" Var(\"%264.1\"))])\n"
      "  FunctionCall(\"disk_read\" [] Var(\"%263.1\") false false)\n"
      "  JumpIfFalse(Var(\"%263.1\") \".loop@6_break\")\n"
      "  Binary(Subtract Var(\"@84.tries.2\") Constant(ConstUChar(1)) Var(\"%264.1\"))\n"
      "  JumpIfTrue(Var(\"%264.1\") \".if_end@31\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@31\")\n"
      "  Jump(\".loop@6_continue\")\n"
      "  Label(\".loop@6_break\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "Function(\"load_scene_from_disk\" false [\"@81.scene\"] [\n"
      "  Label(\".load_scene_from_disk@ssa_block@0\")\n"
      "  Copy(Var(\"RWTS_IOB_PTR\") Var(\"@82.iob.1\"))\n"
      "  GetAddress(Var(\"SCENE_TEXT_DATA\") Var(\"%246.1\"))\n"
      "  IndirectIndexedStore(Var(\"@82.iob.1\") Constant(ConstUChar(8)) Var(\"%246.1\") false)\n"
      "  Binary(Add Var(\"GAME_SIG_SECTOR\") Var(\"@81.scene\") Var(\"%251.1\"))\n"
      "  Binary(Modulo Var(\"%251.1\") Constant(ConstUChar(13)) Var(\"%253.1.nb0\"))\n"
      "  Binary(Divide Var(\"%251.1\") Constant(ConstUChar(13)) Var(\"%258.1.nb0\"))\n"
      "  IndirectIndexedStore(Var(\"@82.iob.1\") Constant(ConstUChar(5)) Var(\"%253.1.nb0\") false)\n"
      "  Binary(Add Var(\"GAME_SIG_TRACK\") Var(\"%258.1.nb0\") Var(\"%260.1\"))\n"
      "  IndirectIndexedStore(Var(\"@82.iob.1\") Constant(ConstUChar(4)) Var(\"%260.1\") false)\n"
      "  Label(\".loop@6_continue\")\n"
      "  Phi(Var(\"@84.tries.2\") [PhiArg(\".load_scene_from_disk@ssa_block@0\" Constant(ConstUChar(2))) PhiArg(\".if_end@31\" Var(\"%264.1\"))])\n"
      "  FunctionCall(\"disk_read\" [] Var(\"%263.1\") false false)\n"
      "  JumpIfFalse(Var(\"%263.1\") \".loop@6_break\")\n"
      "  Binary(Subtract Var(\"@84.tries.2\") Constant(ConstUChar(1)) Var(\"%264.1\"))\n"
      "  JumpIfTrue(Var(\"%264.1\") \".if_end@31\")\n"
      "  Ret(Constant(ConstUChar(1)))\n"
      "  Label(\".if_end@31\")\n"
      "  Jump(\".loop@6_continue\")\n"
      "  Label(\".loop@6_break\")\n"
      "  Ret(Constant(ConstUChar(0)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_hoist_divmod_pairs: examples/ali_baba/entities.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_hoist_divmod_pairs",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"FONT_CHARNUM\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 23054, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%279.1\", \"%280.1\", \"%281.1\", \"%282.1\", \"%283.1\", \"%284.1\", \"%285.1\", \"%285.1.nb0\", \"%286.1\", \"%286.1.lo2\", \"%287.1\", \"%288.1\", \"%289.1\", \"%290.1\", \"%291.1\", \"%292.1\", \"%292.1.nb0\", \"%293.1\", \"%294.1\", \"%295.1\", \"%295.1.lo1\", \"%296.1\", \"%297.1\", \"%298.1\", \"%299.1\", \"%300.1\", \"%301.1\", \"%302.1\", \"%303.1\", \"%304.1\", \"%305.1\", \"%306.1\", \"%307.1\", \"%308.1\", \"%309.1\", \"%310.1\", \"%311.1\", \"%312.1\", \"%313.1\", \"%314.1\", \"@91.charset_sel\", \"@92.glyph.1\"]}}}",
      "Function(\"render_font_char_direct\" true [\"@91.charset_sel\"] [\n"
      "  JumpIfCmp(LessThan Var(\"FONT_CHARNUM\") Constant(ConstUChar(24)) \".if_end@58\")\n"
      "  Binary(Divide Var(\"FONT_CHARNUM\") Constant(ConstUChar(24)) Var(\"%285.1.nb0\"))\n"
      "  Binary(Add Var(\"@91.charset_sel\") Var(\"%285.1.nb0\") Var(\"%287.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(2)) Var(\"%287.1\") false 0)\n"
      "  Label(\".if_end@58\")\n"
      "  Binary(Modulo Var(\"FONT_CHARNUM\") Constant(ConstUChar(24)) Var(\"%292.1.nb0\"))\n"
      "  Binary(LeftShift Var(\"%292.1.nb0\") Constant(ConstUChar(2)) Var(\"%295.1.lo1\"))\n"
      "  Binary(Add Var(\"%295.1.lo1\") Constant(ConstUChar(32)) Var(\"%296.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(4)) Var(\"%296.1\") false 0)\n"
      "  Binary(Add Var(\"%296.1\") Constant(ConstUChar(1)) Var(\"%301.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(5)) Var(\"%301.1\") false 0)\n"
      "  Binary(Add Var(\"%296.1\") Constant(ConstUChar(2)) Var(\"%306.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(7)) Var(\"%306.1\") false 0)\n"
      "  Binary(Add Var(\"%296.1\") Constant(ConstUChar(3)) Var(\"%311.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(8)) Var(\"%311.1\") false 0)\n"
      "  GetAddress(Var(\"s_PRINT_FONT_CHAR\") Var(\"%314.1\"))\n"
      "  FunctionCall(\"print_string\" [Var(\"%314.1\")] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"render_font_char_direct\" true [\"@91.charset_sel\"] [\n"
      "  Binary(Divide Var(\"FONT_CHARNUM\") Constant(ConstUChar(24)) Var(\"%285.1.nb0\"))\n"
      "  Binary(Modulo Var(\"FONT_CHARNUM\") Constant(ConstUChar(24)) Var(\"%292.1.nb0\"))\n"
      "  JumpIfCmp(LessThan Var(\"FONT_CHARNUM\") Constant(ConstUChar(24)) \".if_end@58\")\n"
      "  Binary(Add Var(\"@91.charset_sel\") Var(\"%285.1.nb0\") Var(\"%287.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(2)) Var(\"%287.1\") false 0)\n"
      "  Label(\".if_end@58\")\n"
      "  Binary(LeftShift Var(\"%292.1.nb0\") Constant(ConstUChar(2)) Var(\"%295.1.lo1\"))\n"
      "  Binary(Add Var(\"%295.1.lo1\") Constant(ConstUChar(32)) Var(\"%296.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(4)) Var(\"%296.1\") false 0)\n"
      "  Binary(Add Var(\"%296.1\") Constant(ConstUChar(1)) Var(\"%301.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(5)) Var(\"%301.1\") false 0)\n"
      "  Binary(Add Var(\"%296.1\") Constant(ConstUChar(2)) Var(\"%306.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(7)) Var(\"%306.1\") false 0)\n"
      "  Binary(Add Var(\"%296.1\") Constant(ConstUChar(3)) Var(\"%311.1\"))\n"
      "  IndexedSymbolStore(\"s_PRINT_FONT_CHAR\" Constant(ConstUChar(8)) Var(\"%311.1\") false 0)\n"
      "  GetAddress(Var(\"s_PRINT_FONT_CHAR\") Var(\"%314.1\"))\n"
      "  FunctionCall(\"print_string\" [Var(\"%314.1\")] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}
