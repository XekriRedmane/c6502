// The recorded calls of `tac_read_stored_globals`, the C++ port of
// passes/optimization/read_stored_global.py, with what the Python pass
// returned. They were recorded with the capture harness's wrappers
// while tests/test_read_stored_global.py ran, through the TAC
// optimizer's driver and through the Python entry points that the tests
// call directly. They are every call of an entry point on the tests'
// synthetic TAC and every call through the driver that changed the
// function. The cases after them are eight calls of the whole corpus
// (tools/capture_passes.py) that changed the function and are the
// smallest, since few corpus calls do. Some tests give a symbol a
// `StaticAttr` with no initial value, which the harness's codec can't
// carry, so those records have a `NoInitializer` in its place, which
// the pass doesn't read.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_read_stored_globals: test_read_stored_global.TestReadStoredGlobals.test_kept_when_stored_through_itself (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"t\", \"v\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Store(Var(\"t\") Var(\"t\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Store(Var(\"t\") Var(\"t\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: test_read_stored_global.TestReadStoredGlobals.test_kept_for_a_volatile_global (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(Pointer(Const(UChar)))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"t\", \"v\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Load(Var(\"t\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Load(Var(\"t\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: test_read_stored_global.TestReadStoredGlobals.test_load_through_the_stored_pointer (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"t\", \"v\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Load(Var(\"t\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Load(Var(\"P\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: test_read_stored_global.TestReadStoredGlobals.test_kept_across_a_store (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"t\", \"v\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Store(Var(\"v\") Var(\"t\") false)\n"
      "  Load(Var(\"t\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  Store(Var(\"v\") Var(\"t\") false)\n"
      "  Load(Var(\"t\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 236, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\"]}}}",
      "Function(\"next\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"P\"))\n"
      "  Load(Var(\"%0.1\") Var(\"%1.1\") false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"next\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"P\"))\n"
      "  Load(Var(\"P\") Var(\"%1.1\") false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: test_read_stored_global.TestReadStoredGlobalSim.test_runs (call 2)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"Q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\"]}}}",
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
      "");
}

TEST_CASE("tac_read_stored_globals: test_read_stored_global.TestReadStoredGlobals.test_kept_across_a_call (standalone) (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"P\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Const(UChar))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"t\", \"v\"]}}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  FunctionCall(\"g\" [] None false false)\n"
      "  Load(Var(\"t\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Add Var(\"P\") Constant(ConstInt(1)) Var(\"t\"))\n"
      "  Copy(Var(\"t\") Var(\"P\"))\n"
      "  FunctionCall(\"g\" [] None false false)\n"
      "  Load(Var(\"t\") Var(\"v\") false)\n"
      "  Ret(Var(\"v\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/ali_baba/combat.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"CURRENT_COL\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 23139, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%308.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%308.1\", \"%309.1\", \"%310.1\", \"%311.1\", \"%312.1\", \"%313.1\"]}}}",
      "Function(\"attack_right\" true [] [\n"
      "  Binary(Add Var(\"CURRENT_COL\") Constant(ConstUChar(1)) Var(\"%308.1\"))\n"
      "  Copy(Var(\"%308.1\") Var(\"CURRENT_COL\"))\n"
      "  Copy(Var(\"%308.1\") Var(\"%309.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%309.1\") Constant(ConstChar(0)) \".if_end@49\")\n"
      "  FunctionCall(\"find_attack_target\" [] Var(\"%312.1\") false false)\n"
      "  Ret(Var(\"%312.1\"))\n"
      "  Label(\".if_end@49\")\n"
      "  FunctionCall(\"attack_left\" [] Var(\"%313.1\") false false)\n"
      "  Ret(Var(\"%313.1\"))\n"
      "])\n",
      "Function(\"attack_right\" true [] [\n"
      "  Binary(Add Var(\"CURRENT_COL\") Constant(ConstUChar(1)) Var(\"%308.1\"))\n"
      "  Copy(Var(\"%308.1\") Var(\"CURRENT_COL\"))\n"
      "  Copy(Var(\"CURRENT_COL\") Var(\"%309.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%309.1\") Constant(ConstChar(0)) \".if_end@49\")\n"
      "  FunctionCall(\"find_attack_target\" [] Var(\"%312.1\") false false)\n"
      "  Ret(Var(\"%312.1\"))\n"
      "  Label(\".if_end@49\")\n"
      "  FunctionCall(\"attack_left\" [] Var(\"%313.1\") false false)\n"
      "  Ret(Var(\"%313.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/ali_baba/combat.c -S -O (call 2)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"CURRENT_COL\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 23139, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%314.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%314.1\", \"%315.1\", \"%316.1\", \"%317.1\", \"%318.1\", \"%319.1\"]}}}",
      "Function(\"attack_left\" true [] [\n"
      "  Binary(Subtract Var(\"CURRENT_COL\") Constant(ConstUChar(1)) Var(\"%314.1\"))\n"
      "  Copy(Var(\"%314.1\") Var(\"CURRENT_COL\"))\n"
      "  Copy(Var(\"%314.1\") Var(\"%315.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%315.1\") Constant(ConstChar(0)) \".if_end@50\")\n"
      "  FunctionCall(\"find_attack_target\" [] Var(\"%318.1\") false false)\n"
      "  Ret(Var(\"%318.1\"))\n"
      "  Label(\".if_end@50\")\n"
      "  FunctionCall(\"attack_up\" [] Var(\"%319.1\") false false)\n"
      "  Ret(Var(\"%319.1\"))\n"
      "])\n",
      "Function(\"attack_left\" true [] [\n"
      "  Binary(Subtract Var(\"CURRENT_COL\") Constant(ConstUChar(1)) Var(\"%314.1\"))\n"
      "  Copy(Var(\"%314.1\") Var(\"CURRENT_COL\"))\n"
      "  Copy(Var(\"CURRENT_COL\") Var(\"%315.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%315.1\") Constant(ConstChar(0)) \".if_end@50\")\n"
      "  FunctionCall(\"find_attack_target\" [] Var(\"%318.1\") false false)\n"
      "  Ret(Var(\"%318.1\"))\n"
      "  Label(\".if_end@50\")\n"
      "  FunctionCall(\"attack_up\" [] Var(\"%319.1\") false false)\n"
      "  Ret(Var(\"%319.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/ali_baba/combat.c -S -O (call 3)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"CURRENT_ROW\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 23140, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%320.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%320.1\", \"%321.1\", \"%322.1\", \"%323.1\", \"%324.1\", \"%325.1\"]}}}",
      "Function(\"attack_up\" true [] [\n"
      "  Binary(Subtract Var(\"CURRENT_ROW\") Constant(ConstUChar(1)) Var(\"%320.1\"))\n"
      "  Copy(Var(\"%320.1\") Var(\"CURRENT_ROW\"))\n"
      "  Copy(Var(\"%320.1\") Var(\"%321.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%321.1\") Constant(ConstChar(0)) \".if_end@51\")\n"
      "  FunctionCall(\"find_attack_target\" [] Var(\"%324.1\") false false)\n"
      "  Ret(Var(\"%324.1\"))\n"
      "  Label(\".if_end@51\")\n"
      "  FunctionCall(\"attack_down\" [] Var(\"%325.1\") false false)\n"
      "  Ret(Var(\"%325.1\"))\n"
      "])\n",
      "Function(\"attack_up\" true [] [\n"
      "  Binary(Subtract Var(\"CURRENT_ROW\") Constant(ConstUChar(1)) Var(\"%320.1\"))\n"
      "  Copy(Var(\"%320.1\") Var(\"CURRENT_ROW\"))\n"
      "  Copy(Var(\"CURRENT_ROW\") Var(\"%321.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%321.1\") Constant(ConstChar(0)) \".if_end@51\")\n"
      "  FunctionCall(\"find_attack_target\" [] Var(\"%324.1\") false false)\n"
      "  Ret(Var(\"%324.1\"))\n"
      "  Label(\".if_end@51\")\n"
      "  FunctionCall(\"attack_down\" [] Var(\"%325.1\") false false)\n"
      "  Ret(Var(\"%325.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/ali_baba/util.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"ANIM_FRAME_CTR\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 3}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\"]}}}",
      "Function(\"tick_anim_counter\" true [] [\n"
      "  Binary(Subtract Var(\"ANIM_FRAME_CTR\") Constant(ConstUChar(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"ANIM_FRAME_CTR\"))\n"
      "  JumpIfTrue(Var(\"%0.1\") \".if_end@0\")\n"
      "  FunctionCall(\"draw_blink_alt\" [] None false false)\n"
      "  Label(\".if_end@0\")\n"
      "  ZeroExtend(Var(\"ANIM_INDEX\") Var(\"%3.1\"))\n"
      "  Truncate(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  IndexedLoad(\"ANIM_WAIT_TABLE\" Var(\"%4.1\") Var(\"%5.1\") false 0 0)\n"
      "  Copy(Var(\"%5.1\") Var(\"WAIT_LOOP_COUNT\"))\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"tick_anim_counter\" true [] [\n"
      "  Binary(Subtract Var(\"ANIM_FRAME_CTR\") Constant(ConstUChar(1)) Var(\"%0.1\"))\n"
      "  Copy(Var(\"%0.1\") Var(\"ANIM_FRAME_CTR\"))\n"
      "  JumpIfTrue(Var(\"ANIM_FRAME_CTR\") \".if_end@0\")\n"
      "  FunctionCall(\"draw_blink_alt\" [] None false false)\n"
      "  Label(\".if_end@0\")\n"
      "  ZeroExtend(Var(\"ANIM_INDEX\") Var(\"%3.1\"))\n"
      "  Truncate(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "  IndexedLoad(\"ANIM_WAIT_TABLE\" Var(\"%4.1\") Var(\"%5.1\") false 0 0)\n"
      "  Copy(Var(\"%5.1\") Var(\"WAIT_LOOP_COUNT\"))\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/lode_runner/player.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"PLAYER_Y_ADJ\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 3, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%105.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%105.1\", \"%106.1\", \"%107.1\", \"%108.1\", \"%109.1\", \"%110.1\"]}}}",
      "Function(\"step_down\" false [] [\n"
      "  Binary(Add Var(\"PLAYER_Y_ADJ\") Constant(ConstUChar(1)) Var(\"%105.1\"))\n"
      "  Copy(Var(\"%105.1\") Var(\"PLAYER_Y_ADJ\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%105.1\") Constant(ConstUChar(5)) \".if_end@22\")\n"
      "  FunctionCall(\"check_for_gold_picked_up_by_player\" [] None false false)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@22\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"PLAYER_Y_ADJ\"))\n"
      "  FunctionCall(\"leave_cell\" [] None false false)\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%110.1\"))\n"
      "  Copy(Var(\"%110.1\") Var(\"PLAYER_ROW\"))\n"
      "  FunctionCall(\"enter_cell\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"step_down\" false [] [\n"
      "  Binary(Add Var(\"PLAYER_Y_ADJ\") Constant(ConstUChar(1)) Var(\"%105.1\"))\n"
      "  Copy(Var(\"%105.1\") Var(\"PLAYER_Y_ADJ\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"PLAYER_Y_ADJ\") Constant(ConstUChar(5)) \".if_end@22\")\n"
      "  FunctionCall(\"check_for_gold_picked_up_by_player\" [] None false false)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@22\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"PLAYER_Y_ADJ\"))\n"
      "  FunctionCall(\"leave_cell\" [] None false false)\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%110.1\"))\n"
      "  Copy(Var(\"%110.1\") Var(\"PLAYER_ROW\"))\n"
      "  FunctionCall(\"enter_cell\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/lode_runner/player.c -S -O (call 2)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"PLAYER_Y_ADJ\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 3, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%105.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%105.1\", \"%106.1\", \"%107.1\", \"%108.1\", \"%109.1\", \"%110.1\"]}}}",
      "Function(\"step_down\" false [] [\n"
      "  Binary(Add Var(\"PLAYER_Y_ADJ\") Constant(ConstUChar(1)) Var(\"%105.1\"))\n"
      "  Copy(Var(\"%105.1\") Var(\"PLAYER_Y_ADJ\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"%105.1\") Constant(ConstUChar(5)) \".if_end@22\")\n"
      "  FunctionCall(\"check_for_gold_picked_up_by_player\" [] None false false)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@22\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"PLAYER_Y_ADJ\"))\n"
      "  FunctionCall(\"leave_cell\" [] None false false)\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%110.1\"))\n"
      "  Copy(Var(\"%110.1\") Var(\"PLAYER_ROW\"))\n"
      "  FunctionCall(\"enter_cell\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"step_down\" false [] [\n"
      "  Binary(Add Var(\"PLAYER_Y_ADJ\") Constant(ConstUChar(1)) Var(\"%105.1\"))\n"
      "  Copy(Var(\"%105.1\") Var(\"PLAYER_Y_ADJ\"))\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"PLAYER_Y_ADJ\") Constant(ConstUChar(5)) \".if_end@22\")\n"
      "  FunctionCall(\"check_for_gold_picked_up_by_player\" [] None false false)\n"
      "  Ret(None)\n"
      "  Label(\".if_end@22\")\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"PLAYER_Y_ADJ\"))\n"
      "  FunctionCall(\"leave_cell\" [] None false false)\n"
      "  Binary(Add Var(\"PLAYER_ROW\") Constant(ConstUChar(1)) Var(\"%110.1\"))\n"
      "  Copy(Var(\"%110.1\") Var(\"PLAYER_ROW\"))\n"
      "  FunctionCall(\"enter_cell\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/ali_baba/init.c -S -O (call 1)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"border_anim_index\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 6}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%36.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"@60.glyph.1\"]}}}",
      "Function(\"start_border_passes\" false [] [\n"
      "  Copy(Var(\"border_anim_index\") Var(\"@60.glyph.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"border_pass\"))\n"
      "  Truncate(Constant(ConstInt(9)) Var(\"%35.1\"))\n"
      "  IndexedSymbolStore(\"border_glyph\" Var(\"%35.1\") Var(\"@60.glyph.1\") false 0)\n"
      "  FunctionCall(\"draw_border_cell\" [Constant(ConstUChar(9)) Var(\"@60.glyph.1\")] None false false)\n"
      "  Binary(Add Var(\"border_anim_index\") Constant(ConstUChar(1)) Var(\"%36.1\"))\n"
      "  Copy(Var(\"%36.1\") Var(\"border_anim_index\"))\n"
      "  JumpIfCmp(LessThan Var(\"%36.1\") Constant(ConstUChar(22)) \".if_end@3\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"border_anim_index\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"start_border_passes\" false [] [\n"
      "  Copy(Var(\"border_anim_index\") Var(\"@60.glyph.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"border_pass\"))\n"
      "  Truncate(Constant(ConstInt(9)) Var(\"%35.1\"))\n"
      "  IndexedSymbolStore(\"border_glyph\" Var(\"%35.1\") Var(\"@60.glyph.1\") false 0)\n"
      "  FunctionCall(\"draw_border_cell\" [Constant(ConstUChar(9)) Var(\"@60.glyph.1\")] None false false)\n"
      "  Binary(Add Var(\"border_anim_index\") Constant(ConstUChar(1)) Var(\"%36.1\"))\n"
      "  Copy(Var(\"%36.1\") Var(\"border_anim_index\"))\n"
      "  JumpIfCmp(LessThan Var(\"border_anim_index\") Constant(ConstUChar(22)) \".if_end@3\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"border_anim_index\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}

TEST_CASE("tac_read_stored_globals: examples/ali_baba/init.c -S -O (call 2)") {
  c6502::testing::check_stage_case(
      "tac_read_stored_globals",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"border_anim_index\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 6}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"%36.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%33.1\", \"%34.1\", \"%35.1\", \"%36.1\", \"%37.1\", \"%38.1\", \"%39.1\", \"@60.glyph.1\"]}}}",
      "Function(\"start_border_passes\" false [] [\n"
      "  Copy(Var(\"border_anim_index\") Var(\"@60.glyph.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"border_pass\"))\n"
      "  Truncate(Constant(ConstInt(9)) Var(\"%35.1\"))\n"
      "  IndexedSymbolStore(\"border_glyph\" Var(\"%35.1\") Var(\"@60.glyph.1\") false 0)\n"
      "  FunctionCall(\"draw_border_cell\" [Constant(ConstUChar(9)) Var(\"@60.glyph.1\")] None false false)\n"
      "  Binary(Add Var(\"border_anim_index\") Constant(ConstUChar(1)) Var(\"%36.1\"))\n"
      "  Copy(Var(\"%36.1\") Var(\"border_anim_index\"))\n"
      "  JumpIfCmp(LessThan Var(\"%36.1\") Constant(ConstUChar(22)) \".if_end@3\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"border_anim_index\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"start_border_passes\" false [] [\n"
      "  Copy(Var(\"border_anim_index\") Var(\"@60.glyph.1\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"border_pass\"))\n"
      "  Truncate(Constant(ConstInt(9)) Var(\"%35.1\"))\n"
      "  IndexedSymbolStore(\"border_glyph\" Var(\"%35.1\") Var(\"@60.glyph.1\") false 0)\n"
      "  FunctionCall(\"draw_border_cell\" [Constant(ConstUChar(9)) Var(\"@60.glyph.1\")] None false false)\n"
      "  Binary(Add Var(\"border_anim_index\") Constant(ConstUChar(1)) Var(\"%36.1\"))\n"
      "  Copy(Var(\"%36.1\") Var(\"border_anim_index\"))\n"
      "  JumpIfCmp(LessThan Var(\"border_anim_index\") Constant(ConstUChar(22)) \".if_end@3\")\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"border_anim_index\"))\n"
      "  Label(\".if_end@3\")\n"
      "  Ret(None)\n"
      "])\n",
      "");
}
