// The calls that the unit tests of group A's Python modules make of
// `select_abi`, `fixed_address_layouts`, the carry-return choices, the
// pool eligibility, and the recursion saves, each with what the Python
// function returned or the exception it raised, which the C++ port must
// return or throw too. They were recorded by wrapping the Python
// functions while the unit tests of tests/test_abi_selection.py,
// test_carry_return_choice.py, test_recursion_saves.py,
// test_zp_local_allocation.py, test_leaf_zp_abi.py,
// test_param_registers.py, test_address_attr.py, test_noreturn_call.py,
// test_statement_attributes.py, test_preserves.py,
// test_attribute_zp_abi.py, and test_zp_globals.py ran. Every call that
// raised is kept, and of the others the first call of each function in
// each test that gave a non-empty result. `pool_eligible` is kept only
// from tests/test_abi_selection.py and test_zp_local_allocation.py, and
// `live_out` only from test_recursion_saves.py. The captures of the
// corpus hold the other calls.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("check_marked_calls: test_recursion_saves.TestSyntax.test_call_that_cant_recurse (call 1)") {
  c6502::testing::check_stage_call(
      "check_marked_calls",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [\"@0.x\"] [\n"
      "    Ret(Var(\"@0.x\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.x\"] [\n"
      "    FunctionCall(\"g\" [Var(\"@1.x\")] Var(\"%0\") true false)\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "])\n",
      "exception: RecursionSaveError: `[[c6502::save_hardware_stack]]` is on a call to 'g' in 'f', but 'g' can't call 'f' again, so the call can't recurse\n",
      "");
}

TEST_CASE("check_marked_calls: test_noreturn_call.TestCallGraph.test_save_hardware_stack_on_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "check_marked_calls",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"r\" [] None true true)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"r\" true [] [\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: RecursionSaveError: `[[c6502::save_hardware_stack]]` is on a call to 'r' in 'f', but 'r' is declared `[[noreturn]]` and takes no arguments, so nothing of 'f' is needed after the call and nothing has to be saved\n",
      "");
}

TEST_CASE("choose_carry_returns: test_carry_return_choice.TestChoice.test_branched_on (call 1)") {
  c6502::testing::check_stage_call(
      "choose_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"small\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dum"
      "p\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_check"
      "ing.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" false [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"set\": [\"small\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"small\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", "
      "\"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_c"
      "hecking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_carry_returns: test_carry_return_choice.TestChoice.test_returned_by_a_chosen_function (call 1)") {
  c6502::testing::check_stage_call(
      "choose_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"is_a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"either\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \""
      "section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \""
      "passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\""
      ": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"is_a\" false [\"@0.c\"] [\n"
      "    Binary(Equal Var(\"@0.c\") Constant(ConstUChar(97)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"is_b\" false [\"@1.c\"] [\n"
      "    Binary(Equal Var(\"@1.c\") Constant(ConstUChar(98)) Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"either\" false [\"@2.c\"] [\n"
      "    FunctionCall(\"is_a\" [Var(\"@2.c\")] Var(\"%6.1\") false false)\n"
      "    JumpIfFalse(Var(\"%6.1\") \".if_end@0\")\n"
      "    Ret(Constant(ConstUChar(1)))\n"
      "    Label(\".if_end@0\")\n"
      "    FunctionCall(\"is_b\" [Var(\"@2.c\")] Var(\"%7.1\") false false)\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"either\" [Var(\"g\")] Var(\"%8.1\") false false)\n"
      "    JumpIfFalse(Var(\"%8.1\") \".if_end@1\")\n"
      "    Copy(Constant(ConstUChar(7)) Var(\"g\"))\n"
      "    Label(\".if_end@1\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"set\": [\"either\", \"is_a\", \"is_b\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"is_a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"either\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []},"
      " \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\":"
      " \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asd"
      "l\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_carry_returns: test_carry_return_choice.TestChoice.test_returned_by_a_function_with_external_linkage (call 1)") {
  c6502::testing::check_stage_call(
      "choose_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"in_range\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_digit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"sect"
      "ion\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false,"
      " \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"in_range\" false [\"@0.c\"] [\n"
      "    ZeroExtend(Var(\"@0.c\") Var(\"%0.1\"))\n"
      "    Binary(Subtract Var(\"%0.1\") Constant(ConstInt(48)) Var(\"%1.1\"))\n"
      "    Binary(LessThan Var(\"%1.1\") Constant(ConstInt(10)) Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "  Function(\"is_digit\" true [\"@1.c\"] [\n"
      "    FunctionCall(\"in_range\" [Var(\"@1.c\")] Var(\"%4.1\") false false)\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"is_digit\" [Var(\"g\")] Var(\"%5.1\") false false)\n"
      "    JumpIfFalse(Var(\"%5.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(7)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%7.1\"))\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"set\": [\"in_range\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"in_range\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_digit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \""
      "section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": fa"
      "lse, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_carry_returns: test_noreturn_call.TestCallGraph.test_code (call 1)") {
  c6502::testing::check_stage_call(
      "choose_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"key\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"last\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"command_loop\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": true}}}}], [\"escape\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"get_key\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \""
      "dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"@2.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"wa"
      "s_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_"
      "checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.k.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"last\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"escape\" false [\"@0.k\"] [\n"
      "    Binary(Equal Var(\"@0.k\") Constant(ConstUChar(155)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"get_key\" false [\"@1.n\"] [\n"
      "    IndexedLoad(\"key\" Constant(ConstUChar(0)) Var(\"%3.1\") true 0 0)\n"
      "    Binary(Add Var(\"%3.1\") Var(\"@1.n\") Var(\"%7.1\"))\n"
      "    FunctionCall(\"escape\" [Var(\"%7.1\")] Var(\"%8.1\") false false)\n"
      "    JumpIfFalse(Var(\"%8.1\") \".if_end@0\")\n"
      "    FunctionCall(\"command_loop\" [] None false true)\n"
      "    Ret(None)\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  Function(\"command_loop\" true [] [\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"get_key\" [Constant(ConstUChar(3))] Var(\"last\") false false)\n"
      "    Jump(\".loop@0_start\")\n"
      "  ])\n"
      "  StaticVariable(\"key\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"last\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"set\": [\"escape\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"key\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Volatile(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"last\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"command_loop\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": true}}}}], [\"escape\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"get_key\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast"
      "\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"@2.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false,"
      " \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.t"
      "ype_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.k.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_global_carry_returns: test_carry_return_choice.TestChoice.test_external_linkage (call 1)") {
  c6502::testing::check_stage_call(
      "choose_global_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"small\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump"
      "\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checki"
      "ng.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" true [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"set\": [\"small\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"small\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \""
      "dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_ch"
      "ecking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_global_carry_returns: test_carry_return_choice.TestChoice.test_returned_by_a_function_with_external_linkage (call 1)") {
  c6502::testing::check_stage_call(
      "choose_global_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"in_range\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_digit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"sec"
      "tion\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false"
      ", \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"dict\": [[\"in_range\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_in_range__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"is_digit\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_digit__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_"
      "registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"in_range\" false [\"@0.c\"] [\n"
      "    ZeroExtend(Var(\"@0.c\") Var(\"%0.1\"))\n"
      "    Binary(Subtract Var(\"%0.1\") Constant(ConstInt(48)) Var(\"%1.1\"))\n"
      "    Binary(LessThan Var(\"%1.1\") Constant(ConstInt(10)) Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "  Function(\"is_digit\" true [\"@1.c\"] [\n"
      "    FunctionCall(\"in_range\" [Var(\"@1.c\")] Var(\"%4.1\") false false)\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"is_digit\" [Var(\"g\")] Var(\"%5.1\") false false)\n"
      "    JumpIfFalse(Var(\"%5.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(7)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%7.1\"))\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"set\": [\"is_digit\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"in_range\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": false, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_digit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, "
      "\"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": f"
      "alse, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_global_carry_returns: test_carry_return_choice.TestGlobalChoice.test_defined_and_declared (call 1)") {
  c6502::testing::check_stage_call(
      "choose_global_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"other\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"small\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"pass"
      "es.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was"
      "_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"other\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_other__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], "
      "\"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" true [\"@1.x\"] [\n"
      "    Binary(LessThan Var(\"@1.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"g\") false false)\n"
      "    FunctionCall(\"other\" [Var(\"g\")] Var(\"%5.1\") false false)\n"
      "    JumpIfFalse(Var(\"%5.1\") \".if_end@0\")\n"
      "    Binary(Add Var(\"g\") Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%8.1\"))\n"
      "    Ret(Var(\"%8.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"set\": [\"other\", \"small\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"other\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"small\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"p"
      "asses.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \""
      "was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_global_carry_returns: test_carry_return_choice.TestGlobalChoice.test_values (call 1)") {
  c6502::testing::check_stage_call(
      "choose_global_carry_returns",
      c6502::testing::text({"{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 6)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [97, 120, 98, 121, 98, 97]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"is_a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": ",
      "{\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"either\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@3.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": fal",
      "se}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\"",
      ": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%20\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs",
      "\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%26\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": ",
      "{\"register\": false, \"was_bool\": false}}}}], [\"%30\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@",
      "3.n.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checki",
      "ng.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"as",
      "dl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attr",
      "s\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"dict\": [[\"is_a\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_a__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"is_b\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_b__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"either\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_either__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}"}),
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"is_a\" true [\"@0.c\"] [\n"
      "    Binary(Equal Var(\"@0.c\") Constant(ConstUChar(97)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"is_b\" true [\"@1.c\"] [\n"
      "    Binary(Equal Var(\"@1.c\") Constant(ConstUChar(98)) Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"either\" true [\"@2.c\"] [\n"
      "    FunctionCall(\"is_a\" [Var(\"@2.c\")] Var(\"%6.1\") false false)\n"
      "    JumpIfFalse(Var(\"%6.1\") \".if_end@0\")\n"
      "    Ret(Constant(ConstUChar(1)))\n"
      "    Label(\".if_end@0\")\n"
      "    FunctionCall(\"is_b\" [Var(\"@2.c\")] Var(\"%7.1\") false false)\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@3.n.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@4.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@4.i.2\") Constant(ConstUChar(6)) \".loop@0_break\")\n"
      "    Label(\".main@ssa_block@1\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%13.1\") false 0 0)\n"
      "    FunctionCall(\"either\" [Var(\"%13.1\")] Var(\"%14.1\") false false)\n"
      "    Copy(Var(\"@3.n.2\") Var(\"@3.n.4\"))\n"
      "    JumpIfFalse(Var(\"%14.1\") \".if_end@1\")\n"
      "    Label(\".main@ssa_block@2\")\n"
      "    Binary(Add Var(\"@3.n.2\") Constant(ConstInt(10)) Var(\"@3.n.4\"))\n"
      "    Label(\".if_end@1\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%18.1\") false 0 0)\n"
      "    FunctionCall(\"is_a\" [Var(\"%18.1\")] Var(\"%19.1\") false false)\n"
      "    Copy(Var(\"@3.n.4\") Var(\"@3.n.6\"))\n"
      "    JumpIfTrue(Var(\"%19.1\") \".if_end@2\")\n"
      "    Label(\".main@ssa_block@3\")\n"
      "    Binary(Add Var(\"@3.n.4\") Constant(ConstInt(1)) Var(\"@3.n.6\"))\n"
      "    Label(\".if_end@2\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%24.1\") false 0 0)\n"
      "    FunctionCall(\"is_b\" [Var(\"%24.1\")] Var(\"%25.1\") false false)\n"
      "    ZeroExtend(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "    Binary(Multiply Constant(ConstInt(100)) Var(\"%26.1\") Var(\"%27.1\"))\n"
      "    Binary(Add Var(\"@3.n.6\") Var(\"%27.1\") Var(\"%28.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%30.1\"))\n"
      "    Copy(Var(\"%28.1\") Var(\"@3.n.2\"))\n"
      "    Copy(Var(\"%30.1\") Var(\"@4.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Var(\"@3.n.2\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"s\" true Pointer [StringInit(\"axbyba\" 6)])\n"
      "])\n",
      "{\"set\": [\"either\", \"is_a\", \"is_b\"]}\n",
      c6502::testing::text({"{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 6)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [97, 120, 98, 121, 98, 97]}}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"is_a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"is_b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@1.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields",
      "\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"either\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@2.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@3.n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": f",
      "alse}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"field",
      "s\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%20\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"att",
      "rs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%26\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\"",
      ": {\"register\": false, \"was_bool\": false}}}}], [\"%30\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [",
      "\"@3.n.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_chec",
      "king.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"",
      "asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"at",
      "trs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.n.7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.i.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}"}));
}

TEST_CASE("choose_global_carry_returns: test_carry_return_choice.TestLinked.test_units_agree_and_run (call 1)") {
  c6502::testing::check_stage_call(
      "choose_global_carry_returns",
      "{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"is_digit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%1.1\", {\"object"
      "\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"dict\": [[\"is_digit\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_digit__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"is_digit\" true [\"@0.c\"] [\n"
      "    JumpIfCmp(LessThan Var(\"@0.c\") Constant(ConstUChar(48)) \".and_false@0\")\n"
      "    JumpIfCmp(GreaterThan Var(\"@0.c\") Constant(ConstUChar(57)) \".and_false@0\")\n"
      "    Label(\".is_digit@ssa_block@2\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "    Jump(\".and_end@1\")\n"
      "    Label(\".and_false@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "    Label(\".and_end@1\")\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"is_digit\"]}\n",
      "{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"is_digit\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%1.1\", {\"ob"
      "ject\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}");
}

TEST_CASE("choose_global_carry_returns: test_zp_local_allocation.TestPoolGrowth.test_no_frame (call 1)") {
  c6502::testing::check_stage_call(
      "choose_global_carry_returns",
      c6502::testing::text({"{\"args\": [{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"BG\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(UChar 28) 16)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 24576, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"LIMIT\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 1, \"address\": 1, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"is_floor\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.sprite\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"",
      "asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.col\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.ladder_row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@4.r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fiel",
      "ds\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, ",
      "\"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22\", {\"object\": \"pass",
      "es.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31\", {\"object\": \"passes.type_checking.Sym",
      "bol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%34\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%39\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40\", {\"object\": \"passes.type_checking.Symbol\", \"fi",
      "elds\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%43\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%46\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\",
      "n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%50\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%53\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%54\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%55\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%56\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%57\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%58\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\"",
      ": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%59\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%60\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%61\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%62\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%63\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%64\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%65\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%66\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%67\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"pas",
      "ses.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%68\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%69\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%70\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%71\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%72\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%73\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%74\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%75\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%76\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"registe",
      "r\": false, \"was_bool\": false}}}}], [\"%77\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%78\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%79\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%80\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%81\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%82\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%83\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%84\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%85\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%86\", {\"object\": \"pass",
      "es.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%87\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%88\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%89\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%90\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%91\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%92\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%93\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%94\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%95\", {",
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%96\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%97\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%98\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%99\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%100\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%101\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"BG@lo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 17)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 28, 56, 84, 112, 140, 168, 196, 224, 252, 24, 52, 80, 108, 136, 164, 192]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"plan",
      "es\": null, \"mergeable\": true, \"was_bool\": false}}}}], [\"BG@hi\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 17)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 97, 97]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": true, \"was_bool\": false}}}}], [\"BG@rows\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 34)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 28, 56, 84, 112, 140, 168, 196, 224, 252, 24, 52, 80, 108, 136, 164, 192, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 97, 97]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": {\"tuple\": [\"BG@lo\", \"BG@hi\"]}, \"mergeable\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"r",
      "egister\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.row.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was",
      "_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.ladder_row.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.row.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false",
      "}}}}], [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [",
      "\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%39.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%43.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1\", {",
      "\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%46.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%50.1\", {\"object\": \"passes.type_checking.Symbol\", \"field",
      "s\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%56.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%57.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%58.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%59.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%60.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%61.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%62.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%63.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type",
      "\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%67.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%68.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%69.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%70.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%71.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%72.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%73.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%74.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%75.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asd",
      "l\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%76.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%77.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%78.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"obj",
      "ect\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%79.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%80.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%81.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%82.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.r.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.r.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%83.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%84.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%85.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAt",
      "tr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%89.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%90.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%91.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%92.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.r.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%93.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%94.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%95.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%96.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"ob",
      "ject\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%97.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%98.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%99.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%100.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%101.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, {\"dict\": [[\"is_floor\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_floor__sprite\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__row\", \"__zpabi_f__col\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}"}),
      "{\"layout\":[2,4,0,1,6],\"user_zp_globals\":[[\"LIMIT\",1]],\"user_zp_bytes\":[1]}",
      false,
      "Program([\n"
      "  Function(\"is_floor\" true [\"@0.sprite\"] [\n"
      "    JumpIfCmp(Equal Var(\"@0.sprite\") Constant(ConstUChar(1)) \".or_true@0\")\n"
      "    JumpIfCmp(Equal Var(\"@0.sprite\") Constant(ConstUChar(2)) \".or_true@0\")\n"
      "    Label(\".is_floor@ssa_block@2\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "    Jump(\".or_end@1\")\n"
      "    Label(\".or_true@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "    Label(\".or_end@1\")\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.row\" \"@2.col\"] [\n"
      "    Label(\".f@ssa_preheader@0\")\n"
      "    Copy(Var(\"@1.row\") Var(\"@1.row.1\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@1.row.1\") Var(\"%9.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%9.1\") Var(\"@2.col\") Var(\"%14.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%14.1\") Constant(ConstUChar(3)) \".if_end@2\")\n"
      "    Ret(Var(\"@1.row.1\"))\n"
      "    Label(\".if_end@2\")\n"
      "    Binary(Subtract Var(\"@1.row.1\") Constant(ConstUChar(1)) Var(\"%18.1\"))\n"
      "    JumpIfFalse(Var(\"@2.col\") \".f@cthread@0\")\n"
      "    Binary(Subtract Var(\"@2.col\") Constant(ConstUChar(1)) Var(\"%29.1\"))\n"
      "    ZeroExtend(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "    IndirectIndexedLoad(Var(\"%9.1\") Var(\"%29.1\") Var(\"%32.1\") false)\n"
      "    FunctionCall(\"is_floor\" [Var(\"%32.1\")] Var(\"%33.1\") false false)\n"
      "    JumpIfTrue(Var(\"%33.1\") \".f@jthread@0\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"%18.1\") Var(\"%37.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%37.1\") Var(\"%29.1\") Var(\"%43.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%43.1\") Constant(ConstUChar(4)) \".f@jthread@0\")\n"
      "    Jump(\".if_end@7\")\n"
      "    Label(\".f@jthread@0\")\n"
      "    JumpIfCmp(GreaterThan Var(\"%18.1\") Var(\"LIMIT\") \".if_end@8\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@8\")\n"
      "    Label(\".if_end@7\")\n"
      "    JumpIfCmp(Equal Var(\"@2.col\") Constant(ConstUChar(27)) \".if_end@13\")\n"
      "    Label(\".f@cthread@0\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@1.row.1\") Var(\"%56.1\") false 0 17)\n"
      "    Binary(Add Var(\"@2.col\") Constant(ConstUChar(1)) Var(\"%59.1\"))\n"
      "    ZeroExtend(Var(\"%59.1\") Var(\"%60.1\"))\n"
      "    IndirectIndexedLoad(Var(\"%56.1\") Var(\"%59.1\") Var(\"%62.1\") false)\n"
      "    FunctionCall(\"is_floor\" [Var(\"%62.1\")] Var(\"%63.1\") false false)\n"
      "    JumpIfTrue(Var(\"%63.1\") \".f@jthread@1\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"%18.1\") Var(\"%67.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%67.1\") Var(\"%59.1\") Var(\"%73.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%73.1\") Constant(ConstUChar(4)) \".f@jthread@1\")\n"
      "    Jump(\".if_end@13\")\n"
      "    Label(\".f@jthread@1\")\n"
      "    JumpIfCmp(GreaterThan Var(\"%18.1\") Var(\"LIMIT\") \".if_end@14\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@14\")\n"
      "    Label(\".if_end@13\")\n"
      "    JumpIfTrue(Var(\"%18.1\") \".if_end@15\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@15\")\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"%18.1\") Var(\"@1.row.1\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(9)) Var(\"LIMIT\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@4.r.2\"))\n"
      "    Label(\".loop@1_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@4.r.2\") Constant(ConstUChar(16)) \".loop@1_break\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@4.r.2\") Var(\"%89.1\") false 0 17)\n"
      "    IndirectIndexedStore(Var(\"%89.1\") Constant(ConstUChar(5)) Constant(ConstUChar(3)) false)\n"
      "    Label(\".loop@1_continue\")\n"
      "    Binary(Add Var(\"@4.r.2\") Constant(ConstUChar(1)) Var(\"@4.r.2\"))\n"
      "    Jump(\".loop@1_start\")\n"
      "    Label(\".loop@1_break\")\n"
      "    GetAddress(Var(\"BG\") Var(\"%94.1\"))\n"
      "    Binary(Add Constant(ConstInt(256)) Var(\"%94.1\") Var(\"%97.1\"))\n"
      "    Store(Constant(ConstUChar(4)) Var(\"%97.1\") false)\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(12)) Constant(ConstUChar(5))] Var(\"%100.1\") false false)\n"
      "    ZeroExtend(Var(\"%100.1\") Var(\"%101.1\"))\n"
      "    Ret(Var(\"%101.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"BG@rows\" false Pointer [StringInit(\"\\x00\\x1c8Tp\\x8c\\xa8\\xc4\\xe0\\xfc\\x184Pl\\x88\\xa4\\xc0``````````aaaaaaa\" 34)])\n"
      "])\n",
      "{\"set\": [\"is_floor\"]}\n",
      c6502::testing::text({"{\"args\": {\"0\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"BG\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Array(UChar 28) 16)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": 24576, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"LIMIT\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.NoInitializer\", \"fields\": {}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": 1, \"address\": 1, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"is_floor\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": true, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": true, \"noreturn\": false}}}}], [\"@0.sprite\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar UChar] UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@1.row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\"",
      ": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.col\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.ladder_row\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@4.r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"",
      "fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": fal",
      "se, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22\", {\"object\": \"",
      "passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31\", {\"object\": \"passes.type_checking",
      ".Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%34\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%39\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40\", {\"object\": \"passes.type_checking.Symbol\",",
      " \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%43\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%46\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"",
      "Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%50\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%53\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%54\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%55\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%56\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%57\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%58\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"obj",
      "ect\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%59\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%60\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%61\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%62\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%63\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": true}}}}], [\"%64\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%65\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%66\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%67\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": ",
      "\"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%68\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%69\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%70\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%71\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%72\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%73\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%74\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%75\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%76\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"reg",
      "ister\": false, \"was_bool\": false}}}}], [\"%77\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%78\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%79\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%80\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%81\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%82\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%83\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%84\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%85\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%86\", {\"object\": \"",
      "passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%87\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%88\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%89\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%90\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%91\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%92\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%93\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%94\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%95",
      "\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%96\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%97\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%98\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%99\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%100\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%101\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"BG@lo\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 17)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 28, 56, 84, 112, 140, 168, 196, 224, 252, 24, 52, 80, 108, 136, 164, 192]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"",
      "planes\": null, \"mergeable\": true, \"was_bool\": false}}}}], [\"BG@hi\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 17)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 97, 97]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": true, \"was_bool\": false}}}}], [\"BG@rows\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(Const(UChar) 34)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": {\"tuple\": [0, 28, 56, 84, 112, 140, 168, 196, 224, 252, 24, 52, 80, 108, 136, 164, 192, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 97, 97]}}}, \"is_global\": false, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": {\"tuple\": [\"BG@lo\", \"BG@hi\"]}, \"mergeable\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\":",
      " {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.row.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, ",
      "\"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.ladder_row.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.row.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": f",
      "alse}}}}], [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}",
      "], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%39.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%43.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1",
      "\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%46.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%50.1\", {\"object\": \"passes.type_checking.Symbol\", \"f",
      "ields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%51.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%56.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%57.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%58.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%59.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%60.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%61.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%62.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%63.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"",
      "type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%67.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%68.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%69.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%70.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%71.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%72.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%73.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%74.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%75.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {",
      "\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%52.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%49.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%76.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%77.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%78.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {",
      "\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%79.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%80.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%81.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%82.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.r.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.r.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%83.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%84.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%85.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.Loc",
      "alAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%89.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%90.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%91.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%92.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@4.r.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%93.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%94.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%95.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%96.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Array(UChar 28))\\n\"}, \"attrs\": ",
      "{\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%97.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(UChar)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%98.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%99.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%100.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%101.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}, \"kwargs\": {}}"}));
}

TEST_CASE("cycles: test_abi_selection.TestSelectAbi.test_caller_of_recursive_function (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}], [\"f\", {\"set\": [\"rec\"]}], [\"main\", {\"set\": [\"f\"]}]]}\n",
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_abi_selection.TestSelectAbi.test_unannotated_recursive_silently_falls_back (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"r\", {\"set\": [\"r\"]}], [\"main\", {\"set\": []}]]}\n",
      "{\"dict\": [[\"r\", {\"set\": [\"r\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_abi_selection.TestSelectAbi.test_zp_abi_direct_recursion_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}], [\"main\", {\"set\": []}]]}\n",
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_abi_selection.TestSelectAbi.test_zp_abi_mutual_recursion_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"f\", {\"set\": [\"g\"]}], [\"main\", {\"set\": []}], [\"g\", {\"set\": [\"f\"]}]]}\n",
      "{\"dict\": [[\"g\", {\"set\": [\"f\", \"g\"]}], [\"f\", {\"set\": [\"f\", \"g\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestCodegen.test_caller_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}], [\"main\", {\"set\": [\"f0\"]}], [\"f0\", {\"set\": [\"rec\"]}]]}\n",
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestCodegen.test_pushes_from_and_pulls_into_globals (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"f\", {\"set\": [\"f\", \"load\"]}], [\"load\", {\"set\": []}], [\"main\", {\"set\": [\"f\"]}]]}\n",
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestCodegen.test_saves_around_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"descend\", {\"set\": [\"descend\"]}], [\"main\", {\"set\": [\"descend\"]}]]}\n",
      "{\"dict\": [[\"descend\", {\"set\": [\"descend\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestCodegen.test_unmarked_call_on_the_cycle (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"pong\", {\"set\": [\"ping\"]}], [\"main\", {\"set\": [\"ping\"]}], [\"ping\", {\"set\": [\"pong\"]}]]}\n",
      "{\"dict\": [[\"pong\", {\"set\": [\"ping\", \"pong\"]}], [\"ping\", {\"set\": [\"ping\", \"pong\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestCycles.test_every_call_on_the_cycle_must_be_marked (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"q\", {\"set\": [\"p\"]}], [\"g\", {\"set\": [\"f\"]}], [\"main\", {\"set\": [\"f\", \"p\"]}], [\"f\", {\"set\": [\"g\", \"leaf\"]}], [\"p\", {\"set\": [\"q\"]}], [\"leaf\", {\"set\": []}]]}\n",
      "{\"dict\": [[\"g\", {\"set\": [\"f\", \"g\"]}], [\"f\", {\"set\": [\"f\", \"g\"]}], [\"q\", {\"set\": [\"p\", \"q\"]}], [\"p\", {\"set\": [\"p\", \"q\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestCycles.test_pools (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"f\", {\"set\": [\"g\", \"leaf\"]}], [\"main\", {\"set\": [\"f\"]}], [\"leaf\", {\"set\": []}], [\"g\", {\"set\": [\"f\"]}]]}\n",
      "{\"dict\": [[\"g\", {\"set\": [\"f\", \"g\"]}], [\"f\", {\"set\": [\"f\", \"g\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestCycles.test_unmarked_cycle_gets_no_pool (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}], [\"main\", {\"set\": [\"f\"]}]]}\n",
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_recursion_saves.TestPrograms.test_programs (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"main\", {\"set\": [\"fact\", \"walk\"]}], [\"walk\", {\"set\": [\"walk\"]}], [\"fact\", {\"set\": [\"fact\"]}]]}\n",
      "{\"dict\": [[\"fact\", {\"set\": [\"fact\"]}], [\"walk\", {\"set\": [\"walk\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_zp_local_allocation.TestIneligibility.test_caller_of_recursive_is_ineligible (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}], [\"main\", {\"set\": [\"rec\"]}]]}\n",
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_zp_local_allocation.TestIneligibility.test_mutual_recursion_makes_both_ineligible (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"b\", {\"set\": [\"a\"]}], [\"a\", {\"set\": [\"b\"]}]]}\n",
      "{\"dict\": [[\"b\", {\"set\": [\"a\", \"b\"]}], [\"a\", {\"set\": [\"a\", \"b\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_zp_local_allocation.TestIneligibility.test_self_recursion_makes_ineligible (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}]]}\n",
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_noreturn_call.TestCallGraph.test_call_with_arguments_stays_an_edge (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"r\", {\"set\": [\"f\"]}], [\"f\", {\"set\": [\"r\"]}]]}\n",
      "{\"dict\": [[\"r\", {\"set\": [\"f\", \"r\"]}], [\"f\", {\"set\": [\"f\", \"r\"]}]]}\n",
      "");
}

TEST_CASE("cycles: test_noreturn_call.TestCallGraph.test_recursion_through_ordinary_calls_stays_a_cycle (call 1)") {
  c6502::testing::check_stage_call(
      "cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      true,
      "{\"dict\": [[\"reset\", {\"set\": [\"f\"]}], [\"f\", {\"set\": [\"g\"]}], [\"g\", {\"set\": [\"f\"]}]]}\n",
      "{\"dict\": [[\"g\", {\"set\": [\"f\", \"g\"]}], [\"f\", {\"set\": [\"f\", \"g\"]}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_param_registers.TestAbiErrors.test_errors (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] None FunType([UInt] Void) None \\\"zp_abi\\\" [\\\"Y\\\"] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UInt Constant(ConstInt(1) Int) UInt)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUInt(1))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` parameter `@0.a` is 2 bytes, but `in_y` names 1 register; name one register for each byte, low byte first\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_param_registers.TestShape.test_address_bytes_go_straight_to_registers (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"iob\\\" None Array(UChar 17) None None None None false))\\n  FunctionDecl(function_decl(\\\"RWTS\\\" [\\\"@0.hi\\\" \\\"@1.lo\\\"] None FunType([UChar UChar] UChar) None \\\"zp_abi\\\" [\\\"A\\\" \\\"Y\\\"] true 10496 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"RWTS\\\" [Cast(UChar Binary(RightShift Cast(UInt AddressOf(Var(\\\"iob\\\" Array(UChar 17)) Pointer(UChar)) UInt) Constant(ConstInt(8) Int) UInt) UChar) Cast(UChar Cast(UInt AddressOf(Var(\\\"iob\\\" Array(UChar 17)) Pointer(UChar)) UInt) UChar)] Bool false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"iob\") Var(\"%0.1\"))\n"
      "    Binary(RightShift Var(\"%0.1\") Constant(ConstInt(8)) Var(\"%2.1\"))\n"
      "    Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "    GetAddress(Var(\"iob\") Var(\"%4.1\"))\n"
      "    Truncate(Var(\"%4.1\") Var(\"%6.1\"))\n"
      "    FunctionCall(\"RWTS\" [Var(\"%3.1\") Var(\"%6.1\")] Var(\"%7.1\") false false)\n"
      "    ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    Ret(Var(\"%8.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"iob\" true Pointer [ZeroInit(17)])\n"
      "])\n",
      "{\"dict\": [[\"RWTS\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_RWTS__hi\", \"__zpabi_RWTS__lo\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_param_registers.TestShape.test_pointer_goes_straight_to_registers (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"iob\\\" false [member_decl(\\\"t\\\" Array(UChar 17))]))\\n  VarDecl(var_decl(\\\"iob\\\" None Structure(\\\"iob\\\") None None None None false))\\n  FunctionDecl(function_decl(\\\"RWTS\\\" [\\\"@0.p\\\"] None FunType([Pointer(Structure(\\\"iob\\\"))] UChar) None \\\"zp_abi\\\" [\\\"YA\\\"] true 10496 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"RWTS\\\" [AddressOf(Var(\\\"iob\\\" Structure(\\\"iob\\\")) Pointer(Structure(\\\"iob\\\")))] Bool false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"iob\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"iob\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"t\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 17)\\n\"}, \"byte_offset\": 0}}], \"size\": 17, \"complete\": true}}]]}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"iob\") Var(\"%0.1\"))\n"
      "    FunctionCall(\"RWTS\" [Var(\"%0.1\")] Var(\"%1.1\") false false)\n"
      "    ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"iob\" true Pointer [ZeroInit(17)])\n"
      "])\n",
      "{\"dict\": [[\"RWTS\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_RWTS__p_0\", \"__zpabi_RWTS__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"Y\", \"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_param_registers.TestSim.test_a_argument_from_a_frame_slot (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"rom\\\" [\\\"@0.a\\\" \\\"@1.y\\\"] None FunType([UChar UChar] UChar) None \\\"zp_abi\\\" [\\\"A\\\" \\\"Y\\\"] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@2.a\\\" Cast(UChar Constant(ConstInt(7) Int) UChar) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.b\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@4.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@4.i\\\" UChar) Int) Constant(ConstInt(3) Int) Int) Postfix(Increment Var(\\\"@4.i\\\" UChar) UChar) Compound(Block([\\n      S(Expression(Postfix(Increment Var(\\\"@2.a\\\" UChar) UChar)))\\n      S(Expression(CompoundAssignment(Add Var(\\\"@3.b\\\" UChar) Constant(ConstInt(2) Int) Int UChar)))\\n    ])) \\\".loop@0\\\" None))\\n    S(Return(Cast(Int FunctionCall(\\\"rom\\\" [Var(\\\"@2.a\\\" UChar) Var(\\\"@3.b\\\" UChar)] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(7)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@2.a\"))\n"
      "    Truncate(Constant(ConstInt(5)) Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@3.b\"))\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"@4.i\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    ZeroExtend(Var(\"@4.i\") Var(\"%3\"))\n"
      "    Binary(LessThan Var(\"%3\") Constant(ConstInt(3)) Var(\"%4\"))\n"
      "    JumpIfFalse(Var(\"%4\") \".loop@0_break\")\n"
      "    Copy(Var(\"@2.a\") Var(\"%5\"))\n"
      "    Binary(Add Var(\"@2.a\") Constant(ConstUChar(1)) Var(\"%6\"))\n"
      "    Copy(Var(\"%6\") Var(\"@2.a\"))\n"
      "    ZeroExtend(Var(\"@3.b\") Var(\"%7\"))\n"
      "    Binary(Add Var(\"%7\") Constant(ConstInt(2)) Var(\"%8\"))\n"
      "    Truncate(Var(\"%8\") Var(\"%9\"))\n"
      "    Copy(Var(\"%9\") Var(\"@3.b\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"@4.i\") Var(\"%10\"))\n"
      "    Binary(Add Var(\"@4.i\") Constant(ConstUChar(1)) Var(\"%11\"))\n"
      "    Copy(Var(\"%11\") Var(\"@4.i\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    FunctionCall(\"rom\" [Var(\"@2.a\") Var(\"@3.b\")] Var(\"%12\") false false)\n"
      "    ZeroExtend(Var(\"%12\") Var(\"%13\"))\n"
      "    Ret(Var(\"%13\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"rom\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_rom__a\", \"__zpabi_rom__y\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_param_registers.TestSim.test_fixed_address_routine (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"rom\\\" [\\\"@0.a\\\" \\\"@1.y\\\"] None FunType([UChar UChar] UChar) None \\\"zp_abi\\\" [\\\"A\\\" \\\"Y\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(20) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Add Cast(Int FunctionCall(\\\"rom\\\" [Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(3) Int) UChar)] UChar false) Int) Cast(Int FunctionCall(\\\"rom\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Var(\\\"g\\\" UChar)] UChar false) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(3)) Var(\"%0\"))\n"
      "    FunctionCall(\"rom\" [Var(\"g\") Var(\"%0\")] Var(\"%1\") false false)\n"
      "    ZeroExtend(Var(\"%1\") Var(\"%2\"))\n"
      "    Truncate(Constant(ConstInt(1)) Var(\"%3\"))\n"
      "    FunctionCall(\"rom\" [Var(\"%3\") Var(\"g\")] Var(\"%4\") false false)\n"
      "    ZeroExtend(Var(\"%4\") Var(\"%5\"))\n"
      "    Binary(Add Var(\"%2\") Var(\"%5\") Var(\"%6\"))\n"
      "    Ret(Var(\"%6\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(20)])\n"
      "])\n",
      "{\"dict\": [[\"rom\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_rom__a\", \"__zpabi_rom__y\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_param_registers.TestSim.test_fixed_address_routine_with_a_pointer (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"blk\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UChar) member_decl(\\\"c\\\" UChar)]))\\n  VarDecl(var_decl(\\\"blk\\\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(9) Int) UChar) Cast(UChar Constant(ConstInt(11) Int) UChar)] Structure(\\\"blk\\\") None) Structure(\\\"blk\\\") None None None None false))\\n  FunctionDecl(function_decl(\\\"next\\\" [\\\"@0.p\\\"] None FunType([Pointer(Structure(\\\"blk\\\"))] UChar) None \\\"zp_abi\\\" [\\\"YA\\\"] false 53504 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"next\\\" [AddressOf(Var(\\\"blk\\\" Structure(\\\"blk\\\")) Pointer(Structure(\\\"blk\\\")))] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"blk\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"blk\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 2}}], \"size\": 3, \"complete\": true}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"blk\") Var(\"%0\"))\n"
      "    FunctionCall(\"next\" [Var(\"%0\")] Var(\"%1\") false false)\n"
      "    ZeroExtend(Var(\"%1\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "  StaticVariable(\"blk\" true Pointer [UCharInit(7) UCharInit(9) UCharInit(11)])\n"
      "])\n",
      "{\"dict\": [[\"next\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_next__p_0\", \"__zpabi_next__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"Y\", \"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestCodegen.test_equs_and_absolute_store (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"COUT\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 65005 [] None false))\\n  VarDecl(var_decl(\\\"scratch\\\" None Volatile(UChar) Extern None Absolute(1913) None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"COUT\\\" [Cast(UChar Constant(ConstInt(193) Int) UChar)] Void false)))\\n    S(Expression(Assignment(Var(\\\"scratch\\\" Volatile(UChar)) Cast(Volatile(UChar) Constant(ConstInt(145) Int) Volatile(UChar)) Volatile(UChar))))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Truncate(Constant(ConstInt(193)) Var(\"%0\"))\n"
      "    FunctionCall(\"COUT\" [Var(\"%0\")] None false false)\n"
      "    Truncate(Constant(ConstInt(145)) Var(\"%1\"))\n"
      "    IndexedSymbolStore(\"scratch\" Constant(ConstUChar(0)) Var(\"%1\") true 0)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"COUT\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_COUT__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None None [\\\"auto\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    Truncate(Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    FunctionCall(\"f\" [Var(\"%0\")] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` has a fixed address and a `register` or `zeropage(addr)` parameter but no `[[c6502::zp_abi]]`; without zp_abi its arguments go on the soft stack, so declare it `[[c6502::zp_abi, c6502::address(...)]]`\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 2)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None None [\\\"auto\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` has a fixed address and a `register` or `zeropage(addr)` parameter but no `[[c6502::zp_abi]]`; without zp_abi its arguments go on the soft stack, so declare it `[[c6502::zp_abi, c6502::address(...)]]`\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 3)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([UChar UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\" \\\"\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    Truncate(Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Truncate(Constant(ConstInt(2)) Var(\"%1\"))\n"
      "    FunctionCall(\"f\" [Var(\"%0\") Var(\"%1\")] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: zp_abi function `f` has a fixed address, so every parameter has to be passed in a register or at a zero-page address its code reads; declare each parameter `register` (A, then X), or name its register with `in_a`, `in_x`, or `in_y`, at most 3 bytes in all, or give it `[[c6502::zeropage(addr)]]`\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 4)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([UChar UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\" \\\"\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1)) Constant(ConstUChar(2))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: zp_abi function `f` has a fixed address, so every parameter has to be passed in a register or at a zero-page address its code reads; declare each parameter `register` (A, then X), or name its register with `in_a`, `in_x`, or `in_y`, at most 3 bytes in all, or give it `[[c6502::zeropage(addr)]]`\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 5)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] ULong) None \\\"zp_abi\\\" [] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [] ULong false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [] Var(\"%0\") false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: zp_abi function `f` has a fixed address but returns a value that doesn't fit in A and X; anything wider than 2 bytes comes back in HARGS\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 6)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] ULong) None \\\"zp_abi\\\" [] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [] ULong false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: zp_abi function `f` has a fixed address but returns a value that doesn't fit in A and X; anything wider than 2 bytes comes back in HARGS\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 7)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"f\\\" FunType([UChar] Void)) Pointer(FunType([UChar] Void))) Pointer(FunType([UChar] Void)) None None None None false)))\\n    S(Expression(FunctionCall(\\\"@1.p\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    GetAddress(Var(\"f\") Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.p\"))\n"
      "    Truncate(Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    IndirectCall(Var(\"@1.p\") [Var(\"%1\")] None)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: the address of `f` is taken, but it's a zp_abi function with a fixed address; a call through a pointer passes its arguments on the soft stack, which the code at that address doesn't read\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestConventionErrors.test_rejected_declarations (call 8)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"f\\\" FunType([UChar] Void)) Pointer(FunType([UChar] Void))) Pointer(FunType([UChar] Void)) None None None None false)))\\n    S(Expression(FunctionCall(\\\"@1.p\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    GetAddress(Var(\"f\") Var(\"%0.1\"))\n"
      "    IndirectCall(Var(\"%0.1\") [Constant(ConstUChar(1))] None)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: the address of `f` is taken, but it's a zp_abi function with a fixed address; a call through a pointer passes its arguments on the soft stack, which the code at that address doesn't read\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestLink.test_fixed_names_bound_once (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"COUT\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 65005 [] None false))\\n  VarDecl(var_decl(\\\"KBD\\\" None Volatile(UChar) Extern None Absolute(49152) None false))\\n  FunctionDecl(function_decl(\\\"say\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"say\\\" [Var(\\\"KBD\\\" Volatile(UChar))] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    IndexedLoad(\"KBD\" Constant(ConstUChar(0)) Var(\"%0.1\") true 0 0)\n"
      "    FunctionCall(\"say\" [Var(\"%0.1\")] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"COUT\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_COUT__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestSim.test_call_fixed_routine (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"inc\\\" [\\\"@0.c\\\"] None FunType([UChar] UChar) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.t\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@2.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@2.i\\\" UChar) Int) Constant(ConstInt(3) Int) Int) Postfix(Increment Var(\\\"@2.i\\\" UChar) UChar) Expression(CompoundAssignment(Add Var(\\\"@1.t\\\" UChar) Cast(Int FunctionCall(\\\"inc\\\" [Var(\\\"@2.i\\\" UChar)] UChar false) Int) Int UChar)) \\\".loop@0\\\" None))\\n    S(Return(Cast(Int Var(\\\"@1.t\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.t\"))\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@2.i\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    ZeroExtend(Var(\"@2.i\") Var(\"%2\"))\n"
      "    Binary(LessThan Var(\"%2\") Constant(ConstInt(3)) Var(\"%3\"))\n"
      "    JumpIfFalse(Var(\"%3\") \".loop@0_break\")\n"
      "    FunctionCall(\"inc\" [Var(\"@2.i\")] Var(\"%4\") false false)\n"
      "    ZeroExtend(Var(\"%4\") Var(\"%5\"))\n"
      "    ZeroExtend(Var(\"@1.t\") Var(\"%6\"))\n"
      "    Binary(Add Var(\"%6\") Var(\"%5\") Var(\"%7\"))\n"
      "    Truncate(Var(\"%7\") Var(\"%8\"))\n"
      "    Copy(Var(\"%8\") Var(\"@1.t\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"@2.i\") Var(\"%9\"))\n"
      "    Binary(Add Var(\"@2.i\") Constant(ConstUChar(1)) Var(\"%10\"))\n"
      "    Copy(Var(\"%10\") Var(\"@2.i\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    ZeroExtend(Var(\"@1.t\") Var(\"%11\"))\n"
      "    Ret(Var(\"%11\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"inc\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_inc__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestSim.test_carry_flag_result (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"at_least_5\\\" [\\\"@0.a\\\"] None FunType([UChar] UChar) Extern \\\"zp_abi\\\" [\\\"auto\\\"] true 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.n\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@2.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@2.i\\\" UChar) Int) Constant(ConstInt(9) Int) Int) Postfix(Increment Var(\\\"@2.i\\\" UChar) UChar) IfStmt(FunctionCall(\\\"at_least_5\\\" [Var(\\\"@2.i\\\" UChar)] Bool false) Expression(CompoundAssignment(Add Var(\\\"@1.n\\\" Int) Constant(ConstInt(10) Int) Int Int)) Expression(CompoundAssignment(Add Var(\\\"@1.n\\\" Int) Constant(ConstInt(1) Int) Int Int))) \\\".loop@0\\\" None))\\n    S(Return(Var(\\\"@1.n\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@1.n\"))\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@2.i\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    ZeroExtend(Var(\"@2.i\") Var(\"%1\"))\n"
      "    Binary(LessThan Var(\"%1\") Constant(ConstInt(9)) Var(\"%2\"))\n"
      "    JumpIfFalse(Var(\"%2\") \".loop@0_break\")\n"
      "    FunctionCall(\"at_least_5\" [Var(\"@2.i\")] Var(\"%3\") false false)\n"
      "    JumpIfFalse(Var(\"%3\") \".if_else@1\")\n"
      "    Binary(Add Var(\"@1.n\") Constant(ConstInt(10)) Var(\"%4\"))\n"
      "    Copy(Var(\"%4\") Var(\"@1.n\"))\n"
      "    Jump(\".if_end@0\")\n"
      "    Label(\".if_else@1\")\n"
      "    Binary(Add Var(\"@1.n\") Constant(ConstInt(1)) Var(\"%5\"))\n"
      "    Copy(Var(\"%5\") Var(\"@1.n\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"@2.i\") Var(\"%6\"))\n"
      "    Binary(Add Var(\"@2.i\") Constant(ConstUChar(1)) Var(\"%7\"))\n"
      "    Copy(Var(\"%7\") Var(\"@2.i\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Var(\"@1.n\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"at_least_5\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_at_least_5__a\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestSim.test_pointer_result (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"where\\\" [] None FunType([] Pointer(UChar)) Extern \\\"zp_abi\\\" [] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Cast(UInt FunctionCall(\\\"where\\\" [] Pointer(UChar) false) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"where\" [] Var(\"%0\") false false)\n"
      "    Copy(Var(\"%0\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"where\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_address_attr.TestSim.test_two_byte_argument_and_result (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"swap\\\" [\\\"@0.v\\\"] None FunType([UInt] UInt) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"k\\\" Cast(UInt Constant(ConstInt(4660) Int) UInt) UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Binary(Add FunctionCall(\\\"swap\\\" [Var(\\\"k\\\" UInt)] UInt false) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"swap\" [Var(\"k\")] Var(\"%0\") false false)\n"
      "    Binary(Add Var(\"%0\") Constant(ConstUInt(1)) Var(\"%2\"))\n"
      "    Copy(Var(\"%2\") Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "  StaticVariable(\"k\" true UInt [UIntInit(4660)])\n"
      "])\n",
      "{\"dict\": [[\"swap\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_swap__v_0\", \"__zpabi_swap__v_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestCountDownInY.test_counter_in_memory_without_the_promise (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"tick\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.c\\\" Var(\\\"n\\\" UChar) UChar None None None None false)))\\n    S(DoWhileStmt(Compound(Block([\\n      S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(0) Int) UChar)] Void false)))\\n    ])) Binary(NotEqual Cast(Int Prefix(Decrement Var(\\\"@1.c\\\" UChar) UChar) Int) Constant(ConstInt(0) Int) Int) \\\".loop@0\\\"))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"tick\" true [] [\n"
      "    Label(\".tick@ssa_block@0\")\n"
      "    Copy(Var(\"n\") Var(\"@1.c.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(0))] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Subtract Var(\"@1.c.2\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "    Copy(Var(\"%1.1\") Var(\"@1.c.2\"))\n"
      "    JumpIfTrue(Var(\"%1.1\") \".loop@0_start\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(5)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestCountDownInY.test_counter_in_y (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"Y\\\"] None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"tick\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.c\\\" Var(\\\"n\\\" UChar) UChar None None None None false)))\\n    S(DoWhileStmt(Compound(Block([\\n      S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(0) Int) UChar)] Void false)))\\n    ])) Binary(NotEqual Cast(Int Prefix(Decrement Var(\\\"@1.c\\\" UChar) UChar) Int) Constant(ConstInt(0) Int) Int) \\\".loop@0\\\"))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"tick\" true [] [\n"
      "    Label(\".tick@ssa_block@0\")\n"
      "    Copy(Var(\"n\") Var(\"@1.c.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(0))] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Subtract Var(\"@1.c.2\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "    Copy(Var(\"%1.1\") Var(\"@1.c.2\"))\n"
      "    JumpIfTrue(Var(\"%1.1\") \".loop@0_start\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(5)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestCountUpLoop.test_call_before_the_loop_not_wrapped (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(2) Int) UChar)] Void false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(1))] None false false)\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(2))] None false false)\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%8.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%8.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [ZeroInit(4)])\n"
      "  StaticVariable(\"n\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestCountUpLoop.test_counter_in_x_across_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"X\\\"] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestCountUpLoop.test_runs (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"X\\\"] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.i\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    ZeroExtend(Var(\"@1.i\") Var(\"%1\"))\n"
      "    ZeroExtend(Var(\"n\") Var(\"%2\"))\n"
      "    Binary(NotEqual Var(\"%1\") Var(\"%2\") Var(\"%3\"))\n"
      "    JumpIfFalse(Var(\"%3\") \".loop@0_break\")\n"
      "    ZeroExtend(Var(\"@1.i\") Var(\"%4\"))\n"
      "    Truncate(Var(\"%4\") Var(\"%5\"))\n"
      "    IndexedLoad(\"buf\" Var(\"%5\") Var(\"%6\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"@1.i\") Var(\"%7\"))\n"
      "    Binary(Add Var(\"@1.i\") Constant(ConstUChar(1)) Var(\"%8\"))\n"
      "    Copy(Var(\"%8\") Var(\"@1.i\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestCountUpLoop.test_wrapped_without_the_promise (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestImmediateInX.test_kept_across_a_preserving_call (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestImmediateInX.test_memory_reloaded (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestImmediateInX.test_reloaded_after_a_plain_call (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"lose\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"lose\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"lose\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"lose\" [Constant(ConstUChar(5))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestImmediateInX.test_runs (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(7) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Truncate(Constant(ConstInt(5)) Var(\"%0\"))\n"
      "    FunctionCall(\"keep\" [Var(\"%0\")] None false false)\n"
      "    Truncate(Constant(ConstInt(5)) Var(\"%1\"))\n"
      "    FunctionCall(\"keep\" [Var(\"%1\")] None false false)\n"
      "    Truncate(Constant(ConstInt(7)) Var(\"%2\"))\n"
      "    FunctionCall(\"keep\" [Var(\"%2\")] None false false)\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestRegisterLocalInY.test_in_y_across_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"Y\\\"] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.i\\\" None UChar Register None None None false)))\\n    S(ForStmt(InitExp(Assignment(Var(\\\"@1.i\\\" UChar) Cast(UChar Constant(ConstInt(0) Int) UChar) UChar)) Binary(LessThan Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestRegisterLocalInY.test_rejected_without_the_promise (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.i\\\" None UChar Register None None None false)))\\n    S(ForStmt(InitExp(Assignment(Var(\\\"@1.i\\\" UChar) Cast(UChar Constant(ConstInt(0) Int) UChar) UChar)) Binary(LessThan Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 128}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("fixed_address_layouts: test_preserves.TestRegisterLocalInY.test_runs (call 1)") {
  c6502::testing::check_stage_call(
      "fixed_address_layouts",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"Y\\\"] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.i\\\" None UChar Register None None None false)))\\n    S(ForStmt(InitExp(Assignment(Var(\\\"@1.i\\\" UChar) Cast(UChar Constant(ConstInt(0) Int) UChar) UChar)) Binary(LessThan Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Truncate(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Copy(Var(\"%0\") Var(\"@1.i\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    ZeroExtend(Var(\"@1.i\") Var(\"%1\"))\n"
      "    ZeroExtend(Var(\"n\") Var(\"%2\"))\n"
      "    Binary(LessThan Var(\"%1\") Var(\"%2\") Var(\"%3\"))\n"
      "    JumpIfFalse(Var(\"%3\") \".loop@0_break\")\n"
      "    ZeroExtend(Var(\"@1.i\") Var(\"%4\"))\n"
      "    Truncate(Var(\"%4\") Var(\"%5\"))\n"
      "    IndexedLoad(\"buf\" Var(\"%5\") Var(\"%6\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"@1.i\") Var(\"%7\"))\n"
      "    Binary(Add Var(\"@1.i\") Constant(ConstUChar(1)) Var(\"%8\"))\n"
      "    Copy(Var(\"%8\") Var(\"@1.i\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("hw_stack_cycles: test_abi_selection.TestSelectAbi.test_caller_of_recursive_function (call 1)") {
  c6502::testing::check_stage_call(
      "hw_stack_cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"rec\" true [\"@0.d\"] [\n"
      "    Binary(LessOrEqual Var(\"@0.d\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    JumpIfFalse(Var(\"%0\") \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Subtract Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    FunctionCall(\"rec\" [Var(\"%1\")] Var(\"%2\") true false)\n"
      "    Binary(Add Var(\"%2\") Constant(ConstInt(1)) Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.p\"] [\n"
      "    FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%4\") false false)\n"
      "    Copy(Var(\"%4\") Var(\"@2.l\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"@2.l\") Var(\"%5\"))\n"
      "    Ret(Var(\"%5\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstInt(4))] Var(\"%6\") false false)\n"
      "    Ret(Var(\"%6\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}]]}\n",
      "");
}

TEST_CASE("hw_stack_cycles: test_recursion_saves.TestCodegen.test_caller_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "hw_stack_cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"rec\" true [\"@0.d\"] [\n"
      "    Binary(LeftShift Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%0.1.sr0\"))\n"
      "    Binary(Add Var(\"%0.1.sr0\") Var(\"@0.d\") Var(\"%0.1\"))\n"
      "    JumpIfCmp(GreaterThan Var(\"@0.d\") Constant(ConstInt(0)) \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Subtract Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "    FunctionCall(\"rec\" [Var(\"%2.1\")] Var(\"%3.1\") true false)\n"
      "    Binary(BitwiseAnd Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "    Binary(Add Var(\"%5.1\") Var(\"%3.1\") Var(\"%6.1.ra\"))\n"
      "    Binary(Add Var(\"%6.1.ra\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  Function(\"f0\" true [\"@3.d\" \"@4.p0\"] [\n"
      "    FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%7.1\") false false)\n"
      "    Binary(Add Var(\"@4.p0\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    ZeroExtend(Var(\"@3.d\") Var(\"%9.1\"))\n"
      "    Binary(Add Var(\"%8.1\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f0\" [Constant(ConstUChar(2)) Constant(ConstInt(4))] Var(\"%12.1\") false false)\n"
      "    Ret(Var(\"%12.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}]]}\n",
      "");
}

TEST_CASE("hw_stack_cycles: test_recursion_saves.TestCodegen.test_pushes_from_and_pulls_into_globals (call 1)") {
  c6502::testing::check_stage_call(
      "hw_stack_cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"load\" true [] [\n"
      "    Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"f\" true [] [\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"depth\") Constant(ConstUChar(2)) \".if_end@0\")\n"
      "    Copy(Var(\"a\") Var(\"@0.sa.1\"))\n"
      "    Copy(Var(\"b\") Var(\"@1.sb.1\"))\n"
      "    FunctionCall(\"load\" [] None false false)\n"
      "    FunctionCall(\"f\" [] None true false)\n"
      "    Copy(Var(\"@1.sb.1\") Var(\"b\"))\n"
      "    Copy(Var(\"@0.sa.1\") Var(\"a\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    ZeroExtend(Var(\"depth\") Var(\"%4.1\"))\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"depth\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}]]}\n",
      "");
}

TEST_CASE("hw_stack_cycles: test_recursion_saves.TestCodegen.test_saves_around_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "hw_stack_cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"deepest\",40],[\"level\",38],[\"remaining\",39]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Program([\n"
      "  Function(\"descend\" true [] [\n"
      "    Copy(Var(\"level\") Var(\"@0.here.1\"))\n"
      "    JumpIfCmp(LessOrEqual Var(\"@0.here.1\") Var(\"deepest\") \".if_end@0\")\n"
      "    Copy(Var(\"@0.here.1\") Var(\"deepest\"))\n"
      "    Label(\".if_end@0\")\n"
      "    JumpIfFalse(Var(\"remaining\") \".if_end@1\")\n"
      "    Binary(Subtract Var(\"remaining\") Constant(ConstUChar(1)) Var(\"remaining\"))\n"
      "    Binary(Add Var(\"@0.here.1\") Constant(ConstUChar(1)) Var(\"level\"))\n"
      "    FunctionCall(\"descend\" [] None true false)\n"
      "    Copy(Var(\"@0.here.1\") Var(\"level\"))\n"
      "    Label(\".if_end@1\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstUChar(5)) Var(\"remaining\"))\n"
      "    FunctionCall(\"descend\" [] None false false)\n"
      "    ZeroExtend(Var(\"deepest\") Var(\"%12.1\"))\n"
      "    Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "    Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "    ZeroExtend(Var(\"level\") Var(\"%14.1\"))\n"
      "    Binary(Add Var(\"%13.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"level\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"deepest\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"remaining\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"descend\", {\"set\": [\"descend\"]}]]}\n",
      "");
}

TEST_CASE("hw_stack_cycles: test_recursion_saves.TestCycles.test_every_call_on_the_cycle_must_be_marked (call 1)") {
  c6502::testing::check_stage_call(
      "hw_stack_cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    FunctionCall(\"p\" [] None false false)\n"
      "  ])\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"g\" [] None true false)\n"
      "    FunctionCall(\"leaf\" [] None false false)\n"
      "  ])\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [] None true false)\n"
      "  ])\n"
      "  Function(\"p\" true [] [\n"
      "    FunctionCall(\"q\" [] None true false)\n"
      "  ])\n"
      "  Function(\"q\" true [] [\n"
      "    FunctionCall(\"p\" [] None false false)\n"
      "  ])\n"
      "  Function(\"leaf\" true [] [])\n"
      "])\n",
      "{\"dict\": [[\"g\", {\"set\": [\"f\", \"g\"]}], [\"f\", {\"set\": [\"f\", \"g\"]}]]}\n",
      "");
}

TEST_CASE("hw_stack_cycles: test_recursion_saves.TestCycles.test_pools (call 1)") {
  c6502::testing::check_stage_call(
      "hw_stack_cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "  ])\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"g\" [] None true false)\n"
      "    FunctionCall(\"leaf\" [] None false false)\n"
      "  ])\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [] None true false)\n"
      "  ])\n"
      "  Function(\"leaf\" true [] [])\n"
      "])\n",
      "{\"dict\": [[\"g\", {\"set\": [\"f\", \"g\"]}], [\"f\", {\"set\": [\"f\", \"g\"]}]]}\n",
      "");
}

TEST_CASE("hw_stack_cycles: test_recursion_saves.TestPrograms.test_programs (call 1)") {
  c6502::testing::check_stage_call(
      "hw_stack_cycles",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",39],[\"n\",38]],\"user_zp_bytes\":[38,39]}",
      false,
      "Program([\n"
      "  Function(\"walk\" true [\"@0.k\"] [\n"
      "    Binary(LeftShift Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%1.1.lo1\"))\n"
      "    Binary(Add Var(\"%1.1.lo1\") Var(\"@0.k\") Var(\"%2.1\"))\n"
      "    Copy(Var(\"depth\") Var(\"@2.b.1\"))\n"
      "    Copy(Var(\"n\") Var(\"%6.1\"))\n"
      "    Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "    IndexedSymbolStore(\"out\" Var(\"%6.1\") Var(\"%2.1\") false 0)\n"
      "    JumpIfCmp(LessOrEqual Var(\"@0.k\") Constant(ConstUChar(0)) \".if_end@0\")\n"
      "    Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "    Binary(Subtract Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%17.1\"))\n"
      "    FunctionCall(\"walk\" [Var(\"%17.1\")] None true false)\n"
      "    Copy(Var(\"@2.b.1\") Var(\"depth\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(LeftShift Var(\"@0.k\") Constant(ConstInt(2)) Var(\"%19.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%19.1.sr0\") Constant(ConstInt(3)) Var(\"%19.1.sr1\"))\n"
      "    Binary(LeftShift Var(\"%19.1.sr1\") Constant(ConstInt(2)) Var(\"%19.1.sr2\"))\n"
      "    Binary(Subtract Var(\"%19.1.sr2\") Var(\"%19.1.sr1\") Var(\"%19.1.sr3\"))\n"
      "    Binary(Add Var(\"%19.1.sr3\") Var(\"%19.1.sr0\") Var(\"%19.1\"))\n"
      "    Binary(Add Var(\"%2.1\") Var(\"%19.1\") Var(\"%23.1.lo1\"))\n"
      "    Binary(Add Var(\"%23.1.lo1\") Var(\"@2.b.1\") Var(\"%24.1\"))\n"
      "    Copy(Var(\"n\") Var(\"%25.1\"))\n"
      "    Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "    IndexedSymbolStore(\"out\" Var(\"%25.1\") Var(\"%24.1\") false 0)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"fact\" true [\"@4.x\"] [\n"
      "    JumpIfCmp(GreaterThan Var(\"@4.x\") Constant(ConstInt(1)) \".if_end@1\")\n"
      "    Ret(Constant(ConstInt(1)))\n"
      "    Label(\".if_end@1\")\n"
      "    Binary(Subtract Var(\"@4.x\") Constant(ConstInt(1)) Var(\"%30.1\"))\n"
      "    FunctionCall(\"fact\" [Var(\"%30.1\")] Var(\"%31.1\") true false)\n"
      "    Binary(Multiply Var(\"@4.x\") Var(\"%31.1\") Var(\"%32.1\"))\n"
      "    Ret(Var(\"%32.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    FunctionCall(\"walk\" [Constant(ConstUChar(4))] None false false)\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@6.s.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@7.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@7.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    Binary(LeftShift Var(\"@6.s.2\") Constant(ConstInt(1)) Var(\"%38.1.sr0\"))\n"
      "    Binary(Add Var(\"%38.1.sr0\") Var(\"@6.s.2\") Var(\"%38.1\"))\n"
      "    IndexedLoad(\"out\" Var(\"@7.i.2\") Var(\"%41.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%41.1\") Var(\"%42.1\"))\n"
      "    Binary(Add Var(\"%38.1\") Var(\"%42.1\") Var(\"%43.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@7.i.2\") Constant(ConstUChar(1)) Var(\"%45.1\"))\n"
      "    Copy(Var(\"%43.1\") Var(\"@6.s.2\"))\n"
      "    Copy(Var(\"%45.1\") Var(\"@7.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    FunctionCall(\"fact\" [Constant(ConstInt(6))] Var(\"%46.1\") false false)\n"
      "    Binary(BitwiseXor Var(\"@6.s.2\") Var(\"%46.1\") Var(\"%47.1\"))\n"
      "    Binary(BitwiseAnd Var(\"%47.1\") Constant(ConstInt(32767)) Var(\"%48.1\"))\n"
      "    Ret(Var(\"%48.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"depth\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"out\" true Pointer [ZeroInit(16)])\n"
      "  StaticVariable(\"n\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"fact\", {\"set\": [\"fact\"]}], [\"walk\", {\"set\": [\"walk\"]}]]}\n",
      "");
}

TEST_CASE("live_out: test_recursion_saves.TestCodegen.test_caller_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "live_out",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Function(\"f0\" true [\"@3.d\" \"@4.p0\"] [\n"
      "  FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%7.1\") false false)\n"
      "  Binary(Add Var(\"@4.p0\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  ZeroExtend(Var(\"@3.d\") Var(\"%9.1\"))\n"
      "  Binary(Add Var(\"%8.1\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "  Ret(Var(\"%10.1\"))\n"
      "])\n",
      "[{\"set\": [\"%7.1\", \"@3.d\", \"@4.p0\"]}, {\"set\": [\"%8.1\", \"@3.d\"]}, {\"set\": [\"%8.1\", \"%9.1\"]}, {\"set\": [\"%10.1\"]}, {\"set\": []}]\n",
      "");
}

TEST_CASE("live_out: test_recursion_saves.TestCodegen.test_pushes_from_and_pulls_into_globals (call 1)") {
  c6502::testing::check_stage_call(
      "live_out",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Function(\"load\" true [] [\n"
      "  Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "  Ret(None)\n"
      "])\n",
      "[{\"set\": []}, {\"set\": []}]\n",
      "");
}

TEST_CASE("live_out: test_recursion_saves.TestCodegen.test_saves_around_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "live_out",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"deepest\",40],[\"level\",38],[\"remaining\",39]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"remaining\"))\n"
      "  FunctionCall(\"descend\" [] None false false)\n"
      "  ZeroExtend(Var(\"deepest\") Var(\"%12.1\"))\n"
      "  Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "  Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "  ZeroExtend(Var(\"level\") Var(\"%14.1\"))\n"
      "  Binary(Add Var(\"%13.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "  Ret(Var(\"%15.1\"))\n"
      "])\n",
      "[{\"set\": [\"deepest\", \"level\"]}, {\"set\": [\"deepest\", \"level\"]}, {\"set\": [\"%12.1\", \"level\"]}, {\"set\": [\"%13.1.sr0\", \"level\"]}, {\"set\": [\"%13.1.sr0\", \"%13.1.sr1\", \"level\"]}, {\"set\": [\"%13.1\", \"level\"]}, {\"set\": [\"%13.1\", \"%14.1\"]}, {\"set\": [\"%15.1\"]}, {\"set\": []}]\n",
      "");
}

TEST_CASE("live_out: test_recursion_saves.TestPrograms.test_programs (call 1)") {
  c6502::testing::check_stage_call(
      "live_out",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"deepest\",40],[\"level\",38],[\"remaining\",39]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Function(\"descend\" true [] [\n"
      "  Copy(Var(\"level\") Var(\"@0.here.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"@0.here.1\") Var(\"deepest\") \".if_end@0\")\n"
      "  Copy(Var(\"@0.here.1\") Var(\"deepest\"))\n"
      "  Label(\".if_end@0\")\n"
      "  JumpIfFalse(Var(\"remaining\") \".if_end@1\")\n"
      "  Binary(Subtract Var(\"remaining\") Constant(ConstUChar(1)) Var(\"remaining\"))\n"
      "  Binary(Add Var(\"@0.here.1\") Constant(ConstUChar(1)) Var(\"level\"))\n"
      "  FunctionCall(\"descend\" [] None true false)\n"
      "  Copy(Var(\"@0.here.1\") Var(\"level\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "[{\"set\": [\"@0.here.1\", \"deepest\", \"remaining\"]}, {\"set\": [\"@0.here.1\", \"remaining\"]}, {\"set\": [\"@0.here.1\", \"remaining\"]}, {\"set\": [\"@0.here.1\", \"remaining\"]}, {\"set\": [\"@0.here.1\", \"remaining\"]}, {\"set\": [\"@0.here.1\"]}, {\"set\": [\"@0.here.1\"]}, {\"set\": [\"@0.here.1\"]}, {\"set\": []}, {\"set\": []}, {\"set\": []}]\n",
      "");
}

TEST_CASE("plan_saves: test_recursion_saves.TestCodegen.test_caller_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "plan_saves",
      c6502::testing::text({"{\"args\": [{\"set\": [\"rec\"]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"rec\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"f0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@3.d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": true, \"was_bool\": false}}}}], [\"@4.p0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@5.l\", {\"object\": \"passes.type_checki",
      "ng.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr",
      "\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.k.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2",
      ".1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.r.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1.ra\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@5.l.1\", {\"object\": \"passes.type_checking.Symbol\"",
      ", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}"}),
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Function(\"rec\" true [\"@0.d\"] [\n"
      "  Binary(LeftShift Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%0.1.sr0\"))\n"
      "  Binary(Add Var(\"%0.1.sr0\") Var(\"@0.d\") Var(\"%0.1\"))\n"
      "  JumpIfCmp(GreaterThan Var(\"@0.d\") Constant(ConstInt(0)) \".if_end@0\")\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "  Label(\".if_end@0\")\n"
      "  Binary(Subtract Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  FunctionCall(\"rec\" [Var(\"%2.1\")] Var(\"%3.1\") true false)\n"
      "  Binary(BitwiseAnd Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Binary(Add Var(\"%5.1\") Var(\"%3.1\") Var(\"%6.1.ra\"))\n"
      "  Binary(Add Var(\"%6.1.ra\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "{\"object\": \"passes.recursion_saves.SavePlan\", \"fields\": {\"push_before\": {\"dict\": [[6, [{\"tuple\": [\"%0.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"%0.1\\\")\\n\"}]}]]]}, \"push_after\": {\"dict\": []}, \"pop_after\": {\"dict\": [[6, [{\"tuple\": [\"%0.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"%0.1\\\")\\n\"}]}]]]}, \"skip\": {\"set\": []}}}\n",
      "");
}

TEST_CASE("plan_saves: test_recursion_saves.TestCodegen.test_pushes_from_and_pulls_into_globals (call 1)") {
  c6502::testing::check_stage_call(
      "plan_saves",
      "{\"args\": [{\"set\": [\"f\"]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"depth\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"load\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"pa"
      "ram_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.sa\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.sb\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.t"
      "ype_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.sa.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.sb.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Function(\"f\" true [] [\n"
      "  JumpIfCmp(GreaterOrEqual Var(\"depth\") Constant(ConstUChar(2)) \".if_end@0\")\n"
      "  Copy(Var(\"a\") Var(\"@0.sa.1\"))\n"
      "  Copy(Var(\"b\") Var(\"@1.sb.1\"))\n"
      "  FunctionCall(\"load\" [] None false false)\n"
      "  FunctionCall(\"f\" [] None true false)\n"
      "  Copy(Var(\"@1.sb.1\") Var(\"b\"))\n"
      "  Copy(Var(\"@0.sa.1\") Var(\"a\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"object\": \"passes.recursion_saves.SavePlan\", \"fields\": {\"push_before\": {\"dict\": []}, \"push_after\": {\"dict\": [[1, [{\"tuple\": [\"@0.sa.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"a\\\")\\n\"}]}]], [2, [{\"tuple\": [\"@1.sb.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"b\\\")\\n\"}]}]]]}, \"pop_after\": {\"dict\": [[4, [{\"tuple\": [\"@1.sb.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"b\\\")\\n\"}]}, {\"tuple\": [\"@0.sa.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"a\\\")\\n\"}]}]]]}, \"skip\": {\"set\": [5, 6]}}}\n",
      "");
}

TEST_CASE("plan_saves: test_recursion_saves.TestCodegen.test_saves_around_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "plan_saves",
      c6502::testing::text({"{\"args\": [{\"set\": [\"descend\"]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"level\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"deepest\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"remaining\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"descend\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.here\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\"",
      ": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"regis",
      "ter\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@0.here.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\"",
      ": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"",
      "asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1.sr1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}"}),
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"deepest\",40],[\"level\",38],[\"remaining\",39]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Function(\"descend\" true [] [\n"
      "  Copy(Var(\"level\") Var(\"@0.here.1\"))\n"
      "  JumpIfCmp(LessOrEqual Var(\"@0.here.1\") Var(\"deepest\") \".if_end@0\")\n"
      "  Copy(Var(\"@0.here.1\") Var(\"deepest\"))\n"
      "  Label(\".if_end@0\")\n"
      "  JumpIfFalse(Var(\"remaining\") \".if_end@1\")\n"
      "  Binary(Subtract Var(\"remaining\") Constant(ConstUChar(1)) Var(\"remaining\"))\n"
      "  Binary(Add Var(\"@0.here.1\") Constant(ConstUChar(1)) Var(\"level\"))\n"
      "  FunctionCall(\"descend\" [] None true false)\n"
      "  Copy(Var(\"@0.here.1\") Var(\"level\"))\n"
      "  Label(\".if_end@1\")\n"
      "  Ret(None)\n"
      "])\n",
      "{\"object\": \"passes.recursion_saves.SavePlan\", \"fields\": {\"push_before\": {\"dict\": [[7, [{\"tuple\": [\"@0.here.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"@0.here.1\\\")\\n\"}]}]]]}, \"push_after\": {\"dict\": []}, \"pop_after\": {\"dict\": [[7, [{\"tuple\": [\"@0.here.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"level\\\")\\n\"}]}]]]}, \"skip\": {\"set\": [8]}}}\n",
      "");
}

TEST_CASE("plan_saves: test_recursion_saves.TestPrograms.test_programs (call 1)") {
  c6502::testing::check_stage_call(
      "plan_saves",
      c6502::testing::text({"{\"args\": [{\"set\": [\"walk\"]}, {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"depth\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"out\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 16)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Tentative\", \"fields\": {}}, \"is_global\": true, \"zeropage\": false, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"walk\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([UChar] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@0.k\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"ty",
      "pe\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.w\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"fact\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([Int] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@4.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@5.y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"main\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"@6.s\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}",
      "], [\"@7.i\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {",
      "\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%12\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"obj",
      "ect\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%21\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"regis",
      "ter\": false, \"was_bool\": false}}}}], [\"%27\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36\", {\"object\": \"pa",
      "sses.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%39\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%43\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c9",
      "9_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%46\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@1.a.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@2.b.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%3.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"",
      "passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%4.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%5.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@3.w.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%6.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%8.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%9.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%10.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%11.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {",
      "\"register\": false, \"was_bool\": false}}}}], [\"%12.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%14.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%15.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%16.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%17.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%18.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%20.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]",
      ", [\"%21.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%22.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%24.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%25.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%26.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%27.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%28.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1.1.lo1\", {\"object\": \"passes.ty",
      "pe_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%23.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1.sr1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1.sr2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%19.1.sr3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@5.y.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%29.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%30.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%31.1\", {\"object\": \"passes.type_checking.Sy",
      "mbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%32.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@5.y.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%33.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.s.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%34.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.i.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.s.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.i.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%35.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\":",
      " \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%36.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%39.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%40.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%41.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%42.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%43.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@6.s.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"o",
      "bject\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%44.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%45.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@7.i.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%46.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%47.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%48.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%38.1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}], \"kwargs\": {}}"}),
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",39],[\"n\",38]],\"user_zp_bytes\":[38,39]}",
      false,
      "Function(\"walk\" true [\"@0.k\"] [\n"
      "  Binary(LeftShift Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%1.1.lo1\"))\n"
      "  Binary(Add Var(\"%1.1.lo1\") Var(\"@0.k\") Var(\"%2.1\"))\n"
      "  Copy(Var(\"depth\") Var(\"@2.b.1\"))\n"
      "  Copy(Var(\"n\") Var(\"%6.1\"))\n"
      "  Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "  IndexedSymbolStore(\"out\" Var(\"%6.1\") Var(\"%2.1\") false 0)\n"
      "  JumpIfCmp(LessOrEqual Var(\"@0.k\") Constant(ConstUChar(0)) \".if_end@0\")\n"
      "  Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "  Binary(Subtract Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%17.1\"))\n"
      "  FunctionCall(\"walk\" [Var(\"%17.1\")] None true false)\n"
      "  Copy(Var(\"@2.b.1\") Var(\"depth\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Binary(LeftShift Var(\"@0.k\") Constant(ConstInt(2)) Var(\"%19.1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%19.1.sr0\") Constant(ConstInt(3)) Var(\"%19.1.sr1\"))\n"
      "  Binary(LeftShift Var(\"%19.1.sr1\") Constant(ConstInt(2)) Var(\"%19.1.sr2\"))\n"
      "  Binary(Subtract Var(\"%19.1.sr2\") Var(\"%19.1.sr1\") Var(\"%19.1.sr3\"))\n"
      "  Binary(Add Var(\"%19.1.sr3\") Var(\"%19.1.sr0\") Var(\"%19.1\"))\n"
      "  Binary(Add Var(\"%2.1\") Var(\"%19.1\") Var(\"%23.1.lo1\"))\n"
      "  Binary(Add Var(\"%23.1.lo1\") Var(\"@2.b.1\") Var(\"%24.1\"))\n"
      "  Copy(Var(\"n\") Var(\"%25.1\"))\n"
      "  Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "  IndexedSymbolStore(\"out\" Var(\"%25.1\") Var(\"%24.1\") false 0)\n"
      "  Ret(None)\n"
      "])\n",
      "{\"object\": \"passes.recursion_saves.SavePlan\", \"fields\": {\"push_before\": {\"dict\": [[9, [{\"tuple\": [\"%2.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"%2.1\\\")\\n\"}]}, {\"tuple\": [\"@2.b.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"@2.b.1\\\")\\n\"}]}]]]}, \"push_after\": {\"dict\": []}, \"pop_after\": {\"dict\": [[9, [{\"tuple\": [\"@2.b.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"@2.b.1\\\")\\n\"}]}, {\"tuple\": [\"%2.1\", {\"asdl\": \"tac_ast\", \"dump\": \"Var(\\\"%2.1\\\")\\n\"}]}]]]}, \"skip\": {\"set\": []}}}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_caller_of_pooled_soft_stack_function_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"g\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p_0\", \"__zpabi_f__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.p\"] [\n"
      "    FunctionCall(\"g\" [Constant(ConstInt(3))] Var(\"%1\") false false)\n"
      "    Copy(Var(\"%1\") Var(\"@2.l\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"@2.l\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"g\") Var(\"%3\"))\n"
      "    Copy(Var(\"%3\") Var(\"@3.q\"))\n"
      "    FunctionCall(\"f\" [Constant(ConstInt(1))] Var(\"%4\") false false)\n"
      "    IndirectCall(Var(\"@3.q\") [Constant(ConstInt(2))] Var(\"%5\"))\n"
      "    Binary(Add Var(\"%4\") Var(\"%5\") Var(\"%6\"))\n"
      "    Ret(Var(\"%6\"))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"f\", \"g\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_caller_of_recursive_function (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"rec\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p_0\", \"__zpabi_f__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"rec\" true [\"@0.d\"] [\n"
      "    Binary(LessOrEqual Var(\"@0.d\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    JumpIfFalse(Var(\"%0\") \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Subtract Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    FunctionCall(\"rec\" [Var(\"%1\")] Var(\"%2\") true false)\n"
      "    Binary(Add Var(\"%2\") Constant(ConstInt(1)) Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.p\"] [\n"
      "    FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%4\") false false)\n"
      "    Copy(Var(\"%4\") Var(\"@2.l\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"@2.l\") Var(\"%5\"))\n"
      "    Ret(Var(\"%5\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstInt(4))] Var(\"%6\") false false)\n"
      "    Ret(Var(\"%6\"))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"f\", \"main\", \"rec\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_custom_pool_start (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [144, 145], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Ret(Var(\"@0.x\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"f\", \"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_default_pool_window_used (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Ret(Var(\"@0.x\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"f\", \"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_no_annotation_eligible_defaults_to_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_unannotated_eligible_helper_defaults_to_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"helper\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_helper__x_0\", \"__zpabi_helper__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"helper\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.x\"] [\n"
      "    FunctionCall(\"helper\" [Var(\"@1.x\")] Var(\"%1\") false false)\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"f\", \"helper\", \"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_unannotated_recursive_silently_falls_back (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"r\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"r\" true [\"@0.x\"] [\n"
      "    JumpIfFalse(Var(\"@0.x\") \".cond_else@0\")\n"
      "    Binary(Subtract Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    FunctionCall(\"r\" [Var(\"%1\")] Var(\"%2\") false false)\n"
      "    Copy(Var(\"%2\") Var(\"%0\"))\n"
      "    Jump(\".cond_end@1\")\n"
      "    Label(\".cond_else@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Label(\".cond_end@1\")\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_zp_abi_leaf_with_int_param (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"f\", \"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_abi_selection.TestSelectAbi.test_zp_abi_two_int_params (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__a_0\", \"__zpabi_f__a_1\", \"__zpabi_f__b_0\", \"__zpabi_f__b_1\"], \"addrs\": [128, 129, 130, 131], \"param_registers\": [\"A\", \"X\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Binary(Add Var(\"@0.a\") Var(\"@1.b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"set\": [\"f\", \"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_zp_local_allocation.TestPoolGrowth.test_no_frame (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"is_floor\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_floor__sprite\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__row\", \"__zpabi_f__col\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[2,4,0,1,6],\"user_zp_globals\":[[\"LIMIT\",1]],\"user_zp_bytes\":[1]}",
      false,
      "Program([\n"
      "  Function(\"is_floor\" true [\"@0.sprite\"] [\n"
      "    JumpIfCmp(Equal Var(\"@0.sprite\") Constant(ConstUChar(1)) \".or_true@0\")\n"
      "    JumpIfCmp(Equal Var(\"@0.sprite\") Constant(ConstUChar(2)) \".or_true@0\")\n"
      "    Label(\".is_floor@ssa_block@2\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "    Jump(\".or_end@1\")\n"
      "    Label(\".or_true@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "    Label(\".or_end@1\")\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.row\" \"@2.col\"] [\n"
      "    Label(\".f@ssa_preheader@0\")\n"
      "    Copy(Var(\"@1.row\") Var(\"@1.row.1\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@1.row.1\") Var(\"%9.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%9.1\") Var(\"@2.col\") Var(\"%14.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%14.1\") Constant(ConstUChar(3)) \".if_end@2\")\n"
      "    Ret(Var(\"@1.row.1\"))\n"
      "    Label(\".if_end@2\")\n"
      "    Binary(Subtract Var(\"@1.row.1\") Constant(ConstUChar(1)) Var(\"%18.1\"))\n"
      "    JumpIfFalse(Var(\"@2.col\") \".f@cthread@0\")\n"
      "    Binary(Subtract Var(\"@2.col\") Constant(ConstUChar(1)) Var(\"%29.1\"))\n"
      "    ZeroExtend(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "    IndirectIndexedLoad(Var(\"%9.1\") Var(\"%29.1\") Var(\"%32.1\") false)\n"
      "    FunctionCall(\"is_floor\" [Var(\"%32.1\")] Var(\"%33.1\") false false)\n"
      "    JumpIfTrue(Var(\"%33.1\") \".f@jthread@0\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"%18.1\") Var(\"%37.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%37.1\") Var(\"%29.1\") Var(\"%43.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%43.1\") Constant(ConstUChar(4)) \".f@jthread@0\")\n"
      "    Jump(\".if_end@7\")\n"
      "    Label(\".f@jthread@0\")\n"
      "    JumpIfCmp(GreaterThan Var(\"%18.1\") Var(\"LIMIT\") \".if_end@8\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@8\")\n"
      "    Label(\".if_end@7\")\n"
      "    JumpIfCmp(Equal Var(\"@2.col\") Constant(ConstUChar(27)) \".if_end@13\")\n"
      "    Label(\".f@cthread@0\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@1.row.1\") Var(\"%56.1\") false 0 17)\n"
      "    Binary(Add Var(\"@2.col\") Constant(ConstUChar(1)) Var(\"%59.1\"))\n"
      "    ZeroExtend(Var(\"%59.1\") Var(\"%60.1\"))\n"
      "    IndirectIndexedLoad(Var(\"%56.1\") Var(\"%59.1\") Var(\"%62.1\") false)\n"
      "    FunctionCall(\"is_floor\" [Var(\"%62.1\")] Var(\"%63.1\") false false)\n"
      "    JumpIfTrue(Var(\"%63.1\") \".f@jthread@1\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"%18.1\") Var(\"%67.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%67.1\") Var(\"%59.1\") Var(\"%73.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%73.1\") Constant(ConstUChar(4)) \".f@jthread@1\")\n"
      "    Jump(\".if_end@13\")\n"
      "    Label(\".f@jthread@1\")\n"
      "    JumpIfCmp(GreaterThan Var(\"%18.1\") Var(\"LIMIT\") \".if_end@14\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@14\")\n"
      "    Label(\".if_end@13\")\n"
      "    JumpIfTrue(Var(\"%18.1\") \".if_end@15\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@15\")\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"%18.1\") Var(\"@1.row.1\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(9)) Var(\"LIMIT\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@4.r.2\"))\n"
      "    Label(\".loop@1_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@4.r.2\") Constant(ConstUChar(16)) \".loop@1_break\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@4.r.2\") Var(\"%89.1\") false 0 17)\n"
      "    IndirectIndexedStore(Var(\"%89.1\") Constant(ConstUChar(5)) Constant(ConstUChar(3)) false)\n"
      "    Label(\".loop@1_continue\")\n"
      "    Binary(Add Var(\"@4.r.2\") Constant(ConstUChar(1)) Var(\"@4.r.2\"))\n"
      "    Jump(\".loop@1_start\")\n"
      "    Label(\".loop@1_break\")\n"
      "    GetAddress(Var(\"BG\") Var(\"%94.1\"))\n"
      "    Binary(Add Constant(ConstInt(256)) Var(\"%94.1\") Var(\"%97.1\"))\n"
      "    Store(Constant(ConstUChar(4)) Var(\"%97.1\") false)\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(12)) Constant(ConstUChar(5))] Var(\"%100.1\") false false)\n"
      "    ZeroExtend(Var(\"%100.1\") Var(\"%101.1\"))\n"
      "    Ret(Var(\"%101.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"BG@rows\" false Pointer [StringInit(\"\\x00\\x1c8Tp\\x8c\\xa8\\xc4\\xe0\\xfc\\x184Pl\\x88\\xa4\\xc0``````````aaaaaaa\" 34)])\n"
      "])\n",
      "{\"set\": [\"f\", \"is_floor\", \"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_zp_local_allocation.TestUnitFallback.test_register_parameters_with_a_frame (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__a\", \"__zpabi_f__b\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"pool\",128]],\"user_zp_bytes\":[128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@1.a\" \"@2.b\"] [\n"
      "    Binary(Add Var(\"@2.b\") Var(\"@1.a\") Var(\"%9.1.lo1.ra.ra\"))\n"
      "    Binary(Add Var(\"@1.a\") Var(\"%9.1.lo1.ra.ra\") Var(\"%11.1.lo1.ra.ra\"))\n"
      "    Binary(Add Var(\"@2.b\") Var(\"%11.1.lo1.ra.ra\") Var(\"%12.1.ra.ra\"))\n"
      "    Binary(Add Constant(ConstUChar(2)) Var(\"%12.1.ra.ra\") Var(\"%12.1\"))\n"
      "    Ret(Var(\"%12.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(3)) Constant(ConstUChar(4))] Var(\"%15.1\") false false)\n"
      "    IndexedSymbolStore(\"pool\" Constant(ConstUChar(0)) Var(\"%15.1\") false 0)\n"
      "    IndexedLoad(\"pool\" Constant(ConstUChar(0)) Var(\"%16.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "    Ret(Var(\"%17.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"pool\" true Pointer [ZeroInit(128)])\n"
      "])\n",
      "{\"set\": [\"f\", \"main\"]}\n",
      "");
}

TEST_CASE("pool_eligible: test_zp_local_allocation.TestUnitFallback.test_runs (call 1)") {
  c6502::testing::check_stage_call(
      "pool_eligible",
      "{\"args\": [{\"dict\": [[\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__d\", \"__zpabi_g__x\"], \"addrs\": [128, 129], \"param_registers\": [\"X\", null], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__d\", \"__zpabi_f__a\", \"__zpabi_f__b\"], \"addrs\": [128, 129, 130], \"param_registers\": [\"X\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"pool\",128]],\"user_zp_bytes\":[128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255]}",
      false,
      "Program([\n"
      "  Function(\"g\" false [\"@0.d\" \"@1.x\"] [\n"
      "    Binary(Add Var(\"@1.x\") Var(\"@0.d\") Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@2.d\" \"@3.a\" \"@4.b\"] [\n"
      "    FunctionCall(\"g\" [Var(\"@2.d\") Var(\"@3.a\")] Var(\"%4.1\") false false)\n"
      "    FunctionCall(\"g\" [Var(\"@2.d\") Var(\"@4.b\")] Var(\"%5.1\") false false)\n"
      "    Binary(Add Var(\"%4.1\") Var(\"%5.1\") Var(\"%10.1.lo1\"))\n"
      "    Binary(Add Var(\"%10.1.lo1\") Var(\"@3.a\") Var(\"%12.1.lo1\"))\n"
      "    Binary(Add Var(\"%12.1.lo1\") Var(\"@4.b\") Var(\"%13.1\"))\n"
      "    Ret(Var(\"%13.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1)) Constant(ConstUChar(3)) Constant(ConstUChar(4))] Var(\"%17.1\") false false)\n"
      "    IndexedSymbolStore(\"pool\" Constant(ConstUChar(0)) Var(\"%17.1\") false 0)\n"
      "    IndexedLoad(\"pool\" Constant(ConstUChar(0)) Var(\"%18.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "    Ret(Var(\"%19.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"pool\" true Pointer [ZeroInit(128)])\n"
      "])\n",
      "{\"set\": [\"f\", \"g\", \"main\"]}\n",
      "");
}

TEST_CASE("recursive_pool_functions: test_recursion_saves.TestCodegen.test_caller_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "recursive_pool_functions",
      "{\"args\": [{\"dict\": [[\"rec\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f0\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f0__d\", \"__zpabi_f0__p0_0\", \"__zpabi_f0__p0_1\"], \"addrs\": [128, 129, 130], \"param_registers\": [\"A\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"rec\" true [\"@0.d\"] [\n"
      "    Binary(LeftShift Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%0.1.sr0\"))\n"
      "    Binary(Add Var(\"%0.1.sr0\") Var(\"@0.d\") Var(\"%0.1\"))\n"
      "    JumpIfCmp(GreaterThan Var(\"@0.d\") Constant(ConstInt(0)) \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Subtract Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "    FunctionCall(\"rec\" [Var(\"%2.1\")] Var(\"%3.1\") true false)\n"
      "    Binary(BitwiseAnd Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "    Binary(Add Var(\"%5.1\") Var(\"%3.1\") Var(\"%6.1.ra\"))\n"
      "    Binary(Add Var(\"%6.1.ra\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  Function(\"f0\" true [\"@3.d\" \"@4.p0\"] [\n"
      "    FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%7.1\") false false)\n"
      "    Binary(Add Var(\"@4.p0\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    ZeroExtend(Var(\"@3.d\") Var(\"%9.1\"))\n"
      "    Binary(Add Var(\"%8.1\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f0\" [Constant(ConstUChar(2)) Constant(ConstInt(4))] Var(\"%12.1\") false false)\n"
      "    Ret(Var(\"%12.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"rec\", {\"set\": [\"rec\"]}]]}\n",
      "");
}

TEST_CASE("recursive_pool_functions: test_recursion_saves.TestCodegen.test_pushes_from_and_pulls_into_globals (call 1)") {
  c6502::testing::check_stage_call(
      "recursive_pool_functions",
      "{\"args\": [{\"dict\": [[\"load\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"load\" true [] [\n"
      "    Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"f\" true [] [\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"depth\") Constant(ConstUChar(2)) \".if_end@0\")\n"
      "    Copy(Var(\"a\") Var(\"@0.sa.1\"))\n"
      "    Copy(Var(\"b\") Var(\"@1.sb.1\"))\n"
      "    FunctionCall(\"load\" [] None false false)\n"
      "    FunctionCall(\"f\" [] None true false)\n"
      "    Copy(Var(\"@1.sb.1\") Var(\"b\"))\n"
      "    Copy(Var(\"@0.sa.1\") Var(\"a\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    ZeroExtend(Var(\"depth\") Var(\"%4.1\"))\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"depth\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"set\": [\"f\"]}]]}\n",
      "");
}

TEST_CASE("recursive_pool_functions: test_recursion_saves.TestCodegen.test_saves_around_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "recursive_pool_functions",
      "{\"args\": [{\"dict\": [[\"descend\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"deepest\",40],[\"level\",38],[\"remaining\",39]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Program([\n"
      "  Function(\"descend\" true [] [\n"
      "    Copy(Var(\"level\") Var(\"@0.here.1\"))\n"
      "    JumpIfCmp(LessOrEqual Var(\"@0.here.1\") Var(\"deepest\") \".if_end@0\")\n"
      "    Copy(Var(\"@0.here.1\") Var(\"deepest\"))\n"
      "    Label(\".if_end@0\")\n"
      "    JumpIfFalse(Var(\"remaining\") \".if_end@1\")\n"
      "    Binary(Subtract Var(\"remaining\") Constant(ConstUChar(1)) Var(\"remaining\"))\n"
      "    Binary(Add Var(\"@0.here.1\") Constant(ConstUChar(1)) Var(\"level\"))\n"
      "    FunctionCall(\"descend\" [] None true false)\n"
      "    Copy(Var(\"@0.here.1\") Var(\"level\"))\n"
      "    Label(\".if_end@1\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstUChar(5)) Var(\"remaining\"))\n"
      "    FunctionCall(\"descend\" [] None false false)\n"
      "    ZeroExtend(Var(\"deepest\") Var(\"%12.1\"))\n"
      "    Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "    Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "    ZeroExtend(Var(\"level\") Var(\"%14.1\"))\n"
      "    Binary(Add Var(\"%13.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"level\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"deepest\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"remaining\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"descend\", {\"set\": [\"descend\"]}]]}\n",
      "");
}

TEST_CASE("recursive_pool_functions: test_recursion_saves.TestPrograms.test_programs (call 1)") {
  c6502::testing::check_stage_call(
      "recursive_pool_functions",
      "{\"args\": [{\"dict\": [[\"walk\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"fact\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",39],[\"n\",38]],\"user_zp_bytes\":[38,39]}",
      false,
      "Program([\n"
      "  Function(\"walk\" true [\"@0.k\"] [\n"
      "    Binary(LeftShift Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%1.1.lo1\"))\n"
      "    Binary(Add Var(\"%1.1.lo1\") Var(\"@0.k\") Var(\"%2.1\"))\n"
      "    Copy(Var(\"depth\") Var(\"@2.b.1\"))\n"
      "    Copy(Var(\"n\") Var(\"%6.1\"))\n"
      "    Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "    IndexedSymbolStore(\"out\" Var(\"%6.1\") Var(\"%2.1\") false 0)\n"
      "    JumpIfCmp(LessOrEqual Var(\"@0.k\") Constant(ConstUChar(0)) \".if_end@0\")\n"
      "    Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "    Binary(Subtract Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%17.1\"))\n"
      "    FunctionCall(\"walk\" [Var(\"%17.1\")] None true false)\n"
      "    Copy(Var(\"@2.b.1\") Var(\"depth\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(LeftShift Var(\"@0.k\") Constant(ConstInt(2)) Var(\"%19.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%19.1.sr0\") Constant(ConstInt(3)) Var(\"%19.1.sr1\"))\n"
      "    Binary(LeftShift Var(\"%19.1.sr1\") Constant(ConstInt(2)) Var(\"%19.1.sr2\"))\n"
      "    Binary(Subtract Var(\"%19.1.sr2\") Var(\"%19.1.sr1\") Var(\"%19.1.sr3\"))\n"
      "    Binary(Add Var(\"%19.1.sr3\") Var(\"%19.1.sr0\") Var(\"%19.1\"))\n"
      "    Binary(Add Var(\"%2.1\") Var(\"%19.1\") Var(\"%23.1.lo1\"))\n"
      "    Binary(Add Var(\"%23.1.lo1\") Var(\"@2.b.1\") Var(\"%24.1\"))\n"
      "    Copy(Var(\"n\") Var(\"%25.1\"))\n"
      "    Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "    IndexedSymbolStore(\"out\" Var(\"%25.1\") Var(\"%24.1\") false 0)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"fact\" true [\"@4.x\"] [\n"
      "    JumpIfCmp(GreaterThan Var(\"@4.x\") Constant(ConstInt(1)) \".if_end@1\")\n"
      "    Ret(Constant(ConstInt(1)))\n"
      "    Label(\".if_end@1\")\n"
      "    Binary(Subtract Var(\"@4.x\") Constant(ConstInt(1)) Var(\"%30.1\"))\n"
      "    FunctionCall(\"fact\" [Var(\"%30.1\")] Var(\"%31.1\") true false)\n"
      "    Binary(Multiply Var(\"@4.x\") Var(\"%31.1\") Var(\"%32.1\"))\n"
      "    Ret(Var(\"%32.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    FunctionCall(\"walk\" [Constant(ConstUChar(4))] None false false)\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@6.s.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@7.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@7.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    Binary(LeftShift Var(\"@6.s.2\") Constant(ConstInt(1)) Var(\"%38.1.sr0\"))\n"
      "    Binary(Add Var(\"%38.1.sr0\") Var(\"@6.s.2\") Var(\"%38.1\"))\n"
      "    IndexedLoad(\"out\" Var(\"@7.i.2\") Var(\"%41.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%41.1\") Var(\"%42.1\"))\n"
      "    Binary(Add Var(\"%38.1\") Var(\"%42.1\") Var(\"%43.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@7.i.2\") Constant(ConstUChar(1)) Var(\"%45.1\"))\n"
      "    Copy(Var(\"%43.1\") Var(\"@6.s.2\"))\n"
      "    Copy(Var(\"%45.1\") Var(\"@7.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    FunctionCall(\"fact\" [Constant(ConstInt(6))] Var(\"%46.1\") false false)\n"
      "    Binary(BitwiseXor Var(\"@6.s.2\") Var(\"%46.1\") Var(\"%47.1\"))\n"
      "    Binary(BitwiseAnd Var(\"%47.1\") Constant(ConstInt(32767)) Var(\"%48.1\"))\n"
      "    Ret(Var(\"%48.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"depth\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"out\" true Pointer [ZeroInit(16)])\n"
      "  StaticVariable(\"n\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"fact\", {\"set\": [\"fact\"]}], [\"walk\", {\"set\": [\"walk\"]}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_caller_of_pooled_soft_stack_function_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.p\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.l\\\" FunctionCall(\\\"g\\\" [Constant(ConstInt(3) Int)] Int false) Int None None None None false)))\\n    S(Return(Binary(Add Var(\\\"@1.p\\\" Int) Var(\\\"@2.l\\\" Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@3.q\\\" AddressOf(Var(\\\"g\\\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Pointer(FunType([Int] Int)) None None None None false)))\\n    S(Return(Binary(Add FunctionCall(\\\"f\\\" [Constant(ConstInt(1) Int)] Int false) FunctionCall(\\\"@3.q\\\" [Constant(ConstInt(2) Int)] Int false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.p\"] [\n"
      "    FunctionCall(\"g\" [Constant(ConstInt(3))] Var(\"%1\") false false)\n"
      "    Copy(Var(\"%1\") Var(\"@2.l\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"@2.l\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"g\") Var(\"%3\"))\n"
      "    Copy(Var(\"%3\") Var(\"@3.q\"))\n"
      "    FunctionCall(\"f\" [Constant(ConstInt(1))] Var(\"%4\") false false)\n"
      "    IndirectCall(Var(\"@3.q\") [Constant(ConstInt(2))] Var(\"%5\"))\n"
      "    Binary(Add Var(\"%4\") Var(\"%5\") Var(\"%6\"))\n"
      "    Ret(Var(\"%6\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"g\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__p_0\", \"__zpabi_f__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_caller_of_recursive_function (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"rec\\\" [\\\"@0.d\\\"] Block([\\n    S(IfStmt(Binary(LessOrEqual Var(\\\"@0.d\\\" Int) Constant(ConstInt(0) Int) Int) Return(Constant(ConstInt(0) Int)) None))\\n    S(Return(Binary(Add FunctionCall(\\\"rec\\\" [Binary(Subtract Var(\\\"@0.d\\\" Int) Constant(ConstInt(1) Int) Int)] Int false) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.p\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.l\\\" FunctionCall(\\\"rec\\\" [Constant(ConstInt(3) Int)] Int false) Int None None None None false)))\\n    S(Return(Binary(Add Var(\\\"@1.p\\\" Int) Var(\\\"@2.l\\\" Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"f\\\" [Constant(ConstInt(4) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"rec\" true [\"@0.d\"] [\n"
      "    Binary(LessOrEqual Var(\"@0.d\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    JumpIfFalse(Var(\"%0\") \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Subtract Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    FunctionCall(\"rec\" [Var(\"%1\")] Var(\"%2\") false false)\n"
      "    Binary(Add Var(\"%2\") Constant(ConstInt(1)) Var(\"%3\"))\n"
      "    Ret(Var(\"%3\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.p\"] [\n"
      "    FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%4\") false false)\n"
      "    Copy(Var(\"%4\") Var(\"@2.l\"))\n"
      "    Binary(Add Var(\"@1.p\") Var(\"@2.l\") Var(\"%5\"))\n"
      "    Ret(Var(\"%5\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstInt(4))] Var(\"%6\") false false)\n"
      "    Ret(Var(\"%6\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"rec\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_custom_pool_start (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {\"pool\": {\"object\": \"passes.optimization.pool.Pool\", \"fields\": {\"start\": 144}}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Ret(Var(\"@0.x\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [144, 145], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_default_pool_window_used (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Var(\\\"@0.x\\\" Int)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Ret(Var(\"@0.x\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_no_annotation_eligible_defaults_to_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_unannotated_eligible_helper_defaults_to_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"helper\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.x\\\"] Block([\\n    S(Return(FunctionCall(\\\"helper\\\" [Var(\\\"@1.x\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"helper\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.x\"] [\n"
      "    FunctionCall(\"helper\" [Var(\"@1.x\")] Var(\"%1\") false false)\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"helper\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_helper__x_0\", \"__zpabi_helper__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_unannotated_recursive_silently_falls_back (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"r\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Conditional(Var(\\\"@0.x\\\" Int) FunctionCall(\\\"r\\\" [Binary(Subtract Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)] Int false) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"r\" true [\"@0.x\"] [\n"
      "    JumpIfFalse(Var(\"@0.x\") \".cond_else@0\")\n"
      "    Binary(Subtract Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    FunctionCall(\"r\" [Var(\"%1\")] Var(\"%2\") false false)\n"
      "    Copy(Var(\"%2\") Var(\"%0\"))\n"
      "    Jump(\".cond_end@1\")\n"
      "    Label(\".cond_else@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Label(\".cond_end@1\")\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"r\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_zp_abi_address_taken_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int AddressOf(Var(\\\"f\\\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"f\") Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` declared `[[c6502::zp_abi]]` but its address is taken somewhere in the program; ZP-passing functions can't be reached through a function pointer\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_zp_abi_direct_recursion_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Conditional(Var(\\\"@0.x\\\" Int) FunctionCall(\\\"f\\\" [Binary(Subtract Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)] Int false) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    JumpIfFalse(Var(\"@0.x\") \".cond_else@0\")\n"
      "    Binary(Subtract Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "    FunctionCall(\"f\" [Var(\"%1\")] Var(\"%2\") false false)\n"
      "    Copy(Var(\"%2\") Var(\"%0\"))\n"
      "    Jump(\".cond_end@1\")\n"
      "    Label(\".cond_else@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"%0\"))\n"
      "    Label(\".cond_end@1\")\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` declared `[[c6502::zp_abi]]` but is reachable from itself through the static call graph (direct or mutual recursion); a recursive call would clobber the parameter ZP slots before the outer activation returned\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_zp_abi_indirect_call_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"helper\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.x\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.p\\\" AddressOf(Var(\\\"helper\\\" FunType([Int] Int)) Pointer(FunType([Int] Int))) Pointer(FunType([Int] Int)) None None None None false)))\\n    S(Return(FunctionCall(\\\"@2.p\\\" [Var(\\\"@1.x\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"helper\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.x\"] [\n"
      "    GetAddress(Var(\"helper\") Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"@2.p\"))\n"
      "    IndirectCall(Var(\"@2.p\") [Var(\"@1.x\")] Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` declared `[[c6502::zp_abi]]` but its body contains an indirect call; the callee's ABI can't be known at the call site, so a ZP-passing function can't make indirect calls\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_zp_abi_leaf_with_int_param (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x_0\", \"__zpabi_f__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_zp_abi_mutual_recursion_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@0.x\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.x\\\"] Block([\\n    S(Return(FunctionCall(\\\"g\\\" [Var(\\\"@1.x\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@2.x\\\"] Block([\\n    S(Return(FunctionCall(\\\"f\\\" [Var(\\\"@2.x\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@1.x\"] [\n"
      "    FunctionCall(\"g\" [Var(\"@1.x\")] Var(\"%0\") false false)\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"g\" true [\"@2.x\"] [\n"
      "    FunctionCall(\"f\" [Var(\"@2.x\")] Var(\"%1\") false false)\n"
      "    Ret(Var(\"%1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` declared `[[c6502::zp_abi]]` but is reachable from itself through the static call graph (direct or mutual recursion); a recursive call would clobber the parameter ZP slots before the outer activation returned\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_zp_abi_oversized_params_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.p0\\\" \\\"@1.p1\\\" \\\"@2.p2\\\" \\\"@3.p3\\\" \\\"@4.p4\\\" \\\"@5.p5\\\" \\\"@6.p6\\\" \\\"@7.p7\\\" \\\"@8.p8\\\" \\\"@9.p9\\\" \\\"@10.p10\\\" \\\"@11.p11\\\" \\\"@12.p12\\\" \\\"@13.p13\\\" \\\"@14.p14\\\" \\\"@15.p15\\\" \\\"@16.p16\\\" \\\"@17.p17\\\" \\\"@18.p18\\\" \\\"@19.p19\\\" \\\"@20.p20\\\" \\\"@21.p21\\\" \\\"@22.p22\\\" \\\"@23.p23\\\" \\\"@24.p24\\\" \\\"@25.p25\\\" \\\"@26.p26\\\" \\\"@27.p27\\\" \\\"@28.p28\\\" \\\"@29.p29\\\" \\\"@30.p30\\\" \\\"@31.p31\\\" \\\"@32.p32\\\"] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long Long] Int) None \\\"zp_abi\\\" [\\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.p0\" \"@1.p1\" \"@2.p2\" \"@3.p3\" \"@4.p4\" \"@5.p5\" \"@6.p6\" \"@7.p7\" \"@8.p8\" \"@9.p9\" \"@10.p10\" \"@11.p11\" \"@12.p12\" \"@13.p13\" \"@14.p14\" \"@15.p15\" \"@16.p16\" \"@17.p17\" \"@18.p18\" \"@19.p19\" \"@20.p20\" \"@21.p21\" \"@22.p22\" \"@23.p23\" \"@24.p24\" \"@25.p25\" \"@26.p26\" \"@27.p27\" \"@28.p28\" \"@29.p29\" \"@30.p30\" \"@31.p31\" \"@32.p32\"] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` declared `[[c6502::zp_abi]]` has 132 parameter bytes, exceeding the 64-byte ZP window ($80-$BF)\n",
      "");
}

TEST_CASE("select_abi: test_abi_selection.TestSelectAbi.test_zp_abi_two_int_params (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None \\\"zp_abi\\\" [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Binary(Add Var(\"@0.a\") Var(\"@1.b\") Var(\"%0\"))\n"
      "    Ret(Var(\"%0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__a_0\", \"__zpabi_f__a_1\", \"__zpabi_f__b_0\", \"__zpabi_f__b_1\"], \"addrs\": [128, 129, 130, 131], \"param_registers\": [\"A\", \"X\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestChoice.test_address_taken (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"fp\\\" AddressOf(Var(\\\"small\\\" FunType([UChar] Bool)) Pointer(FunType([UChar] Bool))) Pointer(FunType([UChar] UChar)) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(1) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int FunctionCall(\\\"fp\\\" [Var(\\\"g\\\" UChar)] Bool false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" false [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    IndirectCall(Var(\"fp\") [Var(\"g\")] Var(\"%5.1\"))\n"
      "    ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"fp\" true Pointer [AddressInit(\"small\" 0)])\n"
      "])\n",
      "{\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestChoice.test_branched_on (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(1) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" false [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestChoice.test_external_linkage (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(1) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" true [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestChoice.test_result_kept_as_a_value (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Postfix(Increment Var(\\\"g\\\" UChar) UChar)) None))\\n    S(Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) UChar) UChar)))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" false [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Binary(Add Var(\"g\") Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"g\") false false)\n"
      "    ZeroExtend(Var(\"g\") Var(\"%8.1\"))\n"
      "    Ret(Var(\"%8.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestChoice.test_returned_by_a_chosen_function (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"is_a\\\" [\\\"@0.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@0.c\\\" UChar) Int) Constant(ConstInt(97) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"is_b\\\" [\\\"@1.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@1.c\\\" UChar) Int) Constant(ConstInt(98) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"either\\\" [\\\"@2.c\\\"] Block([\\n    S(IfStmt(FunctionCall(\\\"is_a\\\" [Var(\\\"@2.c\\\" UChar)] Bool false) Return(Constant(ConstBool(1) Bool)) None))\\n    S(Return(FunctionCall(\\\"is_b\\\" [Var(\\\"@2.c\\\" UChar)] Bool false)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"either\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"is_a\" false [\"@0.c\"] [\n"
      "    Binary(Equal Var(\"@0.c\") Constant(ConstUChar(97)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"is_b\" false [\"@1.c\"] [\n"
      "    Binary(Equal Var(\"@1.c\") Constant(ConstUChar(98)) Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"either\" false [\"@2.c\"] [\n"
      "    FunctionCall(\"is_a\" [Var(\"@2.c\")] Var(\"%6.1\") false false)\n"
      "    JumpIfFalse(Var(\"%6.1\") \".if_end@0\")\n"
      "    Ret(Constant(ConstUChar(1)))\n"
      "    Label(\".if_end@0\")\n"
      "    FunctionCall(\"is_b\" [Var(\"@2.c\")] Var(\"%7.1\") false false)\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"either\" [Var(\"g\")] Var(\"%8.1\") false false)\n"
      "    JumpIfFalse(Var(\"%8.1\") \".if_end@1\")\n"
      "    Copy(Constant(ConstUChar(7)) Var(\"g\"))\n"
      "    Label(\".if_end@1\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"is_a\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_a__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"is_b\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_b__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"either\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_either__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestChoice.test_returned_by_a_function_with_external_linkage (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"in_range\\\" [\\\"@0.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Binary(Subtract Cast(Int Var(\\\"@0.c\\\" UChar) Int) Constant(ConstInt(48) Int) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"is_digit\\\" [\\\"@1.c\\\"] Block([\\n    S(Return(FunctionCall(\\\"in_range\\\" [Var(\\\"@1.c\\\" UChar)] Bool false)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"is_digit\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"in_range\" false [\"@0.c\"] [\n"
      "    ZeroExtend(Var(\"@0.c\") Var(\"%0.1\"))\n"
      "    Binary(Subtract Var(\"%0.1\") Constant(ConstInt(48)) Var(\"%1.1\"))\n"
      "    Binary(LessThan Var(\"%1.1\") Constant(ConstInt(10)) Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "  Function(\"is_digit\" true [\"@1.c\"] [\n"
      "    FunctionCall(\"in_range\" [Var(\"@1.c\")] Var(\"%4.1\") false false)\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"is_digit\" [Var(\"g\")] Var(\"%5.1\") false false)\n"
      "    JumpIfFalse(Var(\"%5.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(7)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%7.1\"))\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"in_range\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_in_range__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"is_digit\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_digit__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestChoice.test_values (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"s\\\" InitList([Cast(UChar Constant(ConstInt(97) Int) UChar) Cast(UChar Constant(ConstInt(120) Int) UChar) Cast(UChar Constant(ConstInt(98) Int) UChar) Cast(UChar Constant(ConstInt(121) Int) UChar) Cast(UChar Constant(ConstInt(98) Int) UChar) Cast(UChar Constant(ConstInt(97) Int) UChar)] Array(UChar 6) None) Array(UChar 6) None None None None false))\\n  FunctionDecl(function_decl(\\\"is_a\\\" [\\\"@0.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@0.c\\\" UChar) Int) Constant(ConstInt(97) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"is_b\\\" [\\\"@1.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@1.c\\\" UChar) Int) Constant(ConstInt(98) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"either\\\" [\\\"@2.c\\\"] Block([\\n    S(IfStmt(FunctionCall(\\\"is_a\\\" [Var(\\\"@2.c\\\" UChar)] Bool false) Return(Constant(ConstBool(1) Bool)) None))\\n    S(Return(FunctionCall(\\\"is_b\\\" [Var(\\\"@2.c\\\" UChar)] Bool false)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@3.n\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@4.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@4.i\\\" UChar) Int) Constant(ConstInt(6) Int) Int) Postfix(Increment Var(\\\"@4.i\\\" UChar) UChar) Compound(Block([\\n      S(IfStmt(FunctionCall(\\\"either\\\" [Subscript(AddressOf(Var(\\\"s\\\" Array(UChar 6)) Pointer(UChar)) Cast(Int Var(\\\"@4.i\\\" UChar) Int) UChar)] Bool false) Expression(CompoundAssignment(Add Var(\\\"@3.n\\\" Int) Constant(ConstInt(10) Int) Int Int)) None))\\n      S(IfStmt(Unary(LogicalNot FunctionCall"
      "(\\\"is_a\\\" [Subscript(AddressOf(Var(\\\"s\\\" Array(UChar 6)) Pointer(UChar)) Cast(Int Var(\\\"@4.i\\\" UChar) Int) UChar)] Bool false) Int) Expression(CompoundAssignment(Add Var(\\\"@3.n\\\" Int) Constant(ConstInt(1) Int) Int Int)) None))\\n    ])) \\\".loop@0\\\" None))\\n    S(Return(Var(\\\"@3.n\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"is_a\" false [\"@0.c\"] [\n"
      "    Binary(Equal Var(\"@0.c\") Constant(ConstUChar(97)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"is_b\" false [\"@1.c\"] [\n"
      "    Binary(Equal Var(\"@1.c\") Constant(ConstUChar(98)) Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"either\" false [\"@2.c\"] [\n"
      "    FunctionCall(\"is_a\" [Var(\"@2.c\")] Var(\"%6.1\") false false)\n"
      "    JumpIfFalse(Var(\"%6.1\") \".if_end@0\")\n"
      "    Ret(Constant(ConstUChar(1)))\n"
      "    Label(\".if_end@0\")\n"
      "    FunctionCall(\"is_b\" [Var(\"@2.c\")] Var(\"%7.1\") false false)\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@3.n.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@4.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@4.i.2\") Constant(ConstUChar(6)) \".loop@0_break\")\n"
      "    Label(\".main@ssa_block@1\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%13.1\") false 0 0)\n"
      "    FunctionCall(\"either\" [Var(\"%13.1\")] Var(\"%14.1\") false false)\n"
      "    Copy(Var(\"@3.n.2\") Var(\"@3.n.4\"))\n"
      "    JumpIfFalse(Var(\"%14.1\") \".if_end@1\")\n"
      "    Label(\".main@ssa_block@2\")\n"
      "    Binary(Add Var(\"@3.n.2\") Constant(ConstInt(10)) Var(\"@3.n.4\"))\n"
      "    Label(\".if_end@1\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%18.1\") false 0 0)\n"
      "    FunctionCall(\"is_a\" [Var(\"%18.1\")] Var(\"%19.1\") false false)\n"
      "    Copy(Var(\"@3.n.4\") Var(\"@3.n.6\"))\n"
      "    JumpIfTrue(Var(\"%19.1\") \".if_end@2\")\n"
      "    Label(\".main@ssa_block@3\")\n"
      "    Binary(Add Var(\"@3.n.4\") Constant(ConstInt(1)) Var(\"@3.n.6\"))\n"
      "    Label(\".if_end@2\")\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%23.1\"))\n"
      "    Copy(Var(\"@3.n.6\") Var(\"@3.n.2\"))\n"
      "    Copy(Var(\"%23.1\") Var(\"@4.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Var(\"@3.n.2\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"s\" true Pointer [StringInit(\"axbyba\" 6)])\n"
      "])\n",
      "{\"dict\": [[\"is_a\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_a__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"is_b\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_b__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"either\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_either__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestGlobalChoice.test_address_taken (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"fp\\\" AddressOf(Var(\\\"small\\\" FunType([UChar] Bool)) Pointer(FunType([UChar] Bool))) Pointer(FunType([UChar] UChar)) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(1) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int FunctionCall(\\\"fp\\\" [Var(\\\"g\\\" UChar)] Bool false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" true [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    IndirectCall(Var(\"fp\") [Var(\"g\")] Var(\"%5.1\"))\n"
      "    ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"fp\" true Pointer [AddressInit(\"small\" 0)])\n"
      "])\n",
      "{\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestGlobalChoice.test_defined_and_declared (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"other\\\" [\\\"@0.x\\\"] None FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@1.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Cast(Int Var(\\\"@1.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) UChar) UChar)))\\n    S(IfStmt(FunctionCall(\\\"other\\\" [Var(\\\"g\\\" UChar)] Bool false) Expression(Postfix(Increment Var(\\\"g\\\" UChar) UChar)) None))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" true [\"@1.x\"] [\n"
      "    Binary(LessThan Var(\"@1.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"g\") false false)\n"
      "    FunctionCall(\"other\" [Var(\"g\")] Var(\"%5.1\") false false)\n"
      "    JumpIfFalse(Var(\"%5.1\") \".if_end@0\")\n"
      "    Binary(Add Var(\"g\") Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%8.1\"))\n"
      "    Ret(Var(\"%8.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"other\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_other__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestGlobalChoice.test_fixed_address (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"rom\\\" [] None FunType([] UChar) None None [] false 8192 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"rom\\\" [] Bool false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(1) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"rom\" [] Var(\"%0.1\") false false)\n"
      "    JumpIfFalse(Var(\"%0.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestGlobalChoice.test_not_bool (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(UChar Binary(LessThan Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] UChar false) Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(1) Int) UChar) UChar)) None))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"small\" true [\"@0.x\"] [\n"
      "    Binary(LessThan Var(\"@0.x\") Constant(ConstUChar(10)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"small\" [Var(\"g\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"g\"))\n"
      "    Label(\".if_end@0\")\n"
      "    ZeroExtend(Var(\"g\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestGlobalChoice.test_static_left_to_the_calls (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"small\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(Bool Binary(LessThan Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"g\\\" UChar) Cast(UChar FunctionCall(\\\"small\\\" [Var(\\\"g\\\" UChar)] Bool false) UChar) UChar)))\\n    S(Return(Cast(Int Var(\\\"g\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Var(\"g\") Var(\"@1.x.1\"))\n"
      "    Binary(LessThan Var(\"@1.x.1\") Constant(ConstUChar(10)) Var(\"g\"))\n"
      "    ZeroExtend(Var(\"g\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"small\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_small__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestGlobalChoice.test_values (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"s\\\" InitList([Cast(UChar Constant(ConstInt(97) Int) UChar) Cast(UChar Constant(ConstInt(120) Int) UChar) Cast(UChar Constant(ConstInt(98) Int) UChar) Cast(UChar Constant(ConstInt(121) Int) UChar) Cast(UChar Constant(ConstInt(98) Int) UChar) Cast(UChar Constant(ConstInt(97) Int) UChar)] Array(UChar 6) None) Array(UChar 6) None None None None false))\\n  FunctionDecl(function_decl(\\\"is_a\\\" [\\\"@0.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@0.c\\\" UChar) Int) Constant(ConstInt(97) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"is_b\\\" [\\\"@1.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@1.c\\\" UChar) Int) Constant(ConstInt(98) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"either\\\" [\\\"@2.c\\\"] Block([\\n    S(IfStmt(FunctionCall(\\\"is_a\\\" [Var(\\\"@2.c\\\" UChar)] Bool false) Return(Constant(ConstBool(1) Bool)) None))\\n    S(Return(FunctionCall(\\\"is_b\\\" [Var(\\\"@2.c\\\" UChar)] Bool false)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@3.n\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@4.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@4.i\\\" UChar) Int) Constant(ConstInt(6) Int) Int) Postfix(Increment Var(\\\"@4.i\\\" UChar) UChar) Compound(Block([\\n      S(IfStmt(FunctionCall(\\\"either\\\" [Subscript(AddressOf(Var(\\\"s\\\" Array(UChar 6)) Pointer(UChar)) Cast(Int Var(\\\"@4.i\\\" UChar) Int) UChar)] Bool false) Expression(CompoundAssignment(Add Var(\\\"@3.n\\\" Int) Constant(ConstInt(10) Int) Int Int)) None))\\n      S(IfStmt(Unary(LogicalNot FunctionCall(\\\"is_"
      "a\\\" [Subscript(AddressOf(Var(\\\"s\\\" Array(UChar 6)) Pointer(UChar)) Cast(Int Var(\\\"@4.i\\\" UChar) Int) UChar)] Bool false) Int) Expression(CompoundAssignment(Add Var(\\\"@3.n\\\" Int) Constant(ConstInt(1) Int) Int Int)) None))\\n      S(Expression(CompoundAssignment(Add Var(\\\"@3.n\\\" Int) Binary(Multiply Constant(ConstInt(100) Int) Cast(Int FunctionCall(\\\"is_b\\\" [Subscript(AddressOf(Var(\\\"s\\\" Array(UChar 6)) Pointer(UChar)) Cast(Int Var(\\\"@4.i\\\" UChar) Int) UChar)] Bool false) Int) Int) Int Int)))\\n    ])) \\\".loop@0\\\" None))\\n    S(Return(Var(\\\"@3.n\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"is_a\" true [\"@0.c\"] [\n"
      "    Binary(Equal Var(\"@0.c\") Constant(ConstUChar(97)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"is_b\" true [\"@1.c\"] [\n"
      "    Binary(Equal Var(\"@1.c\") Constant(ConstUChar(98)) Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"either\" true [\"@2.c\"] [\n"
      "    FunctionCall(\"is_a\" [Var(\"@2.c\")] Var(\"%6.1\") false false)\n"
      "    JumpIfFalse(Var(\"%6.1\") \".if_end@0\")\n"
      "    Ret(Constant(ConstUChar(1)))\n"
      "    Label(\".if_end@0\")\n"
      "    FunctionCall(\"is_b\" [Var(\"@2.c\")] Var(\"%7.1\") false false)\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@3.n.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@4.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@4.i.2\") Constant(ConstUChar(6)) \".loop@0_break\")\n"
      "    Label(\".main@ssa_block@1\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%13.1\") false 0 0)\n"
      "    FunctionCall(\"either\" [Var(\"%13.1\")] Var(\"%14.1\") false false)\n"
      "    Copy(Var(\"@3.n.2\") Var(\"@3.n.4\"))\n"
      "    JumpIfFalse(Var(\"%14.1\") \".if_end@1\")\n"
      "    Label(\".main@ssa_block@2\")\n"
      "    Binary(Add Var(\"@3.n.2\") Constant(ConstInt(10)) Var(\"@3.n.4\"))\n"
      "    Label(\".if_end@1\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%18.1\") false 0 0)\n"
      "    FunctionCall(\"is_a\" [Var(\"%18.1\")] Var(\"%19.1\") false false)\n"
      "    Copy(Var(\"@3.n.4\") Var(\"@3.n.6\"))\n"
      "    JumpIfTrue(Var(\"%19.1\") \".if_end@2\")\n"
      "    Label(\".main@ssa_block@3\")\n"
      "    Binary(Add Var(\"@3.n.4\") Constant(ConstInt(1)) Var(\"@3.n.6\"))\n"
      "    Label(\".if_end@2\")\n"
      "    IndexedLoad(\"s\" Var(\"@4.i.2\") Var(\"%24.1\") false 0 0)\n"
      "    FunctionCall(\"is_b\" [Var(\"%24.1\")] Var(\"%25.1\") false false)\n"
      "    ZeroExtend(Var(\"%25.1\") Var(\"%26.1\"))\n"
      "    Binary(Multiply Constant(ConstInt(100)) Var(\"%26.1\") Var(\"%27.1\"))\n"
      "    Binary(Add Var(\"@3.n.6\") Var(\"%27.1\") Var(\"%28.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%30.1\"))\n"
      "    Copy(Var(\"%28.1\") Var(\"@3.n.2\"))\n"
      "    Copy(Var(\"%30.1\") Var(\"@4.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Var(\"@3.n.2\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"s\" true Pointer [StringInit(\"axbyba\" 6)])\n"
      "])\n",
      "{\"dict\": [[\"is_a\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_a__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"is_b\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_b__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"either\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_either__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_carry_return_choice.TestLinked.test_units_agree_and_run (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"is_digit\\\" [\\\"@0.c\\\"] Block([\\n    S(Return(Cast(Bool Binary(LogicalAnd Binary(GreaterOrEqual Cast(Int Var(\\\"@0.c\\\" UChar) Int) Constant(ConstInt(48) Int) Int) Binary(LessOrEqual Cast(Int Var(\\\"@0.c\\\" UChar) Int) Constant(ConstInt(57) Int) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"is_digit\" true [\"@0.c\"] [\n"
      "    JumpIfCmp(LessThan Var(\"@0.c\") Constant(ConstUChar(48)) \".and_false@0\")\n"
      "    JumpIfCmp(GreaterThan Var(\"@0.c\") Constant(ConstUChar(57)) \".and_false@0\")\n"
      "    Label(\".is_digit@ssa_block@2\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "    Jump(\".and_end@1\")\n"
      "    Label(\".and_false@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "    Label(\".and_end@1\")\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"is_digit\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_digit__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_recursion_saves.TestCodegen.test_caller_keeps_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"rec\\\" [\\\"@0.d\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.k\\\" Binary(Multiply Var(\\\"@0.d\\\" Int) Constant(ConstInt(3) Int) Int) Int None None None None false)))\\n    S(IfStmt(Binary(LessOrEqual Var(\\\"@0.d\\\" Int) Constant(ConstInt(0) Int) Int) Return(Constant(ConstInt(0) Int)) None))\\n    D(VarDecl(var_decl(\\\"@2.r\\\" FunctionCall(\\\"rec\\\" [Binary(Subtract Var(\\\"@0.d\\\" Int) Constant(ConstInt(1) Int) Int)] Int true) Int None None None None false)))\\n    S(Return(Binary(Add Binary(Add Var(\\\"@2.r\\\" Int) Constant(ConstInt(1) Int) Int) Binary(BitwiseAnd Var(\\\"@1.k\\\" Int) Constant(ConstInt(1) Int) Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f0\\\" [\\\"@3.d\\\" \\\"@4.p0\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@5.l\\\" FunctionCall(\\\"rec\\\" [Constant(ConstInt(3) Int)] Int false) Int None None None None false)))\\n    S(Return(Binary(Add Binary(Add Var(\\\"@4.p0\\\" Int) Var(\\\"@5.l\\\" Int) Int) Cast(Int Var(\\\"@3.d\\\" UChar) Int) Int)))\\n  ]) FunType([UChar Int] Int) None None [\\\"A\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"f0\\\" [Cast(UChar Constant(ConstInt(2) Int) UChar) Constant(ConstInt(4) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"rec\" true [\"@0.d\"] [\n"
      "    Binary(LeftShift Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%0.1.sr0\"))\n"
      "    Binary(Add Var(\"%0.1.sr0\") Var(\"@0.d\") Var(\"%0.1\"))\n"
      "    JumpIfCmp(GreaterThan Var(\"@0.d\") Constant(ConstInt(0)) \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Subtract Var(\"@0.d\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "    FunctionCall(\"rec\" [Var(\"%2.1\")] Var(\"%3.1\") true false)\n"
      "    Binary(BitwiseAnd Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "    Binary(Add Var(\"%5.1\") Var(\"%3.1\") Var(\"%6.1.ra\"))\n"
      "    Binary(Add Var(\"%6.1.ra\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  Function(\"f0\" true [\"@3.d\" \"@4.p0\"] [\n"
      "    FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%7.1\") false false)\n"
      "    Binary(Add Var(\"@4.p0\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    ZeroExtend(Var(\"@3.d\") Var(\"%9.1\"))\n"
      "    Binary(Add Var(\"%8.1\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f0\" [Constant(ConstUChar(2)) Constant(ConstInt(4))] Var(\"%12.1\") false false)\n"
      "    Ret(Var(\"%12.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"rec\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f0\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f0__d\", \"__zpabi_f0__p0_0\", \"__zpabi_f0__p0_1\"], \"addrs\": [128, 129, 130], \"param_registers\": [\"A\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_recursion_saves.TestCodegen.test_pushes_from_and_pulls_into_globals (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"depth\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"load\\\" [] Block([\\n    S(Expression(Postfix(Increment Var(\\\"depth\\\" UChar) UChar)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(IfStmt(Binary(LessThan Cast(Int Var(\\\"depth\\\" UChar) Int) Constant(ConstInt(2) Int) Int) Compound(Block([\\n      D(VarDecl(var_decl(\\\"@0.sa\\\" Var(\\\"a\\\" UChar) UChar None None None None false)))\\n      D(VarDecl(var_decl(\\\"@1.sb\\\" Var(\\\"b\\\" UChar) UChar None None None None false)))\\n      S(Expression(FunctionCall(\\\"load\\\" [] Void false)))\\n      S(Expression(FunctionCall(\\\"f\\\" [] Void true)))\\n      S(Expression(Assignment(Var(\\\"b\\\" UChar) Var(\\\"@1.sb\\\" UChar) UChar)))\\n      S(Expression(Assignment(Var(\\\"a\\\" UChar) Var(\\\"@0.sa\\\" UChar) UChar)))\\n    ])) None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [] Void false)))\\n    S(Return(Cast(Int Var(\\\"depth\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"load\" true [] [\n"
      "    Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"f\" true [] [\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"depth\") Constant(ConstUChar(2)) \".if_end@0\")\n"
      "    Copy(Var(\"a\") Var(\"@0.sa.1\"))\n"
      "    Copy(Var(\"b\") Var(\"@1.sb.1\"))\n"
      "    FunctionCall(\"load\" [] None false false)\n"
      "    FunctionCall(\"f\" [] None true false)\n"
      "    Copy(Var(\"@1.sb.1\") Var(\"b\"))\n"
      "    Copy(Var(\"@0.sa.1\") Var(\"a\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    ZeroExtend(Var(\"depth\") Var(\"%4.1\"))\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"depth\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"load\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_recursion_saves.TestCodegen.test_saves_around_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"level\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"deepest\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"remaining\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"descend\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.here\\\" Var(\\\"level\\\" UChar) UChar None None None None false)))\\n    S(IfStmt(Binary(GreaterThan Cast(Int Var(\\\"@0.here\\\" UChar) Int) Cast(Int Var(\\\"deepest\\\" UChar) Int) Int) Expression(Assignment(Var(\\\"deepest\\\" UChar) Var(\\\"@0.here\\\" UChar) UChar)) None))\\n    S(IfStmt(Binary(NotEqual Cast(Int Var(\\\"remaining\\\" UChar) Int) Constant(ConstInt(0) Int) Int) Compound(Block([\\n      S(Expression(Assignment(Var(\\\"remaining\\\" UChar) Cast(UChar Binary(Subtract Cast(Int Var(\\\"remaining\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar)))\\n      S(Expression(Assignment(Var(\\\"level\\\" UChar) Cast(UChar Binary(Add Cast(Int Var(\\\"@0.here\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar)))\\n      S(Expression(FunctionCall(\\\"descend\\\" [] Void true)))\\n      S(Expression(Assignment(Var(\\\"level\\\" UChar) Var(\\\"@0.here\\\" UChar) UChar)))\\n    ])) None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"remaining\\\" UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\\n    S(Expression(FunctionCall(\\\"descend\\\" [] Void false)))\\n    S(Return(Binary(Add Binary(Multiply Cast(Int Var(\\\"deepest\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Cast(Int Var(\\\"level\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"deepest\",40],[\"level\",38],[\"remaining\",39]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Program([\n"
      "  Function(\"descend\" true [] [\n"
      "    Copy(Var(\"level\") Var(\"@0.here.1\"))\n"
      "    JumpIfCmp(LessOrEqual Var(\"@0.here.1\") Var(\"deepest\") \".if_end@0\")\n"
      "    Copy(Var(\"@0.here.1\") Var(\"deepest\"))\n"
      "    Label(\".if_end@0\")\n"
      "    JumpIfFalse(Var(\"remaining\") \".if_end@1\")\n"
      "    Binary(Subtract Var(\"remaining\") Constant(ConstUChar(1)) Var(\"remaining\"))\n"
      "    Binary(Add Var(\"@0.here.1\") Constant(ConstUChar(1)) Var(\"level\"))\n"
      "    FunctionCall(\"descend\" [] None true false)\n"
      "    Copy(Var(\"@0.here.1\") Var(\"level\"))\n"
      "    Label(\".if_end@1\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstUChar(5)) Var(\"remaining\"))\n"
      "    FunctionCall(\"descend\" [] None false false)\n"
      "    ZeroExtend(Var(\"deepest\") Var(\"%12.1\"))\n"
      "    Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "    Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "    ZeroExtend(Var(\"level\") Var(\"%14.1\"))\n"
      "    Binary(Add Var(\"%13.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"level\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"deepest\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"remaining\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"descend\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_recursion_saves.TestCodegen.test_unmarked_call_on_the_cycle (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"pong\\\" [\\\"@0.n\\\"] None FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"ping\\\" [\\\"@1.n\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.a\\\" Binary(Multiply Var(\\\"@1.n\\\" Int) Constant(ConstInt(2) Int) Int) Int None None None None false)))\\n    S(IfStmt(Binary(LessOrEqual Var(\\\"@1.n\\\" Int) Constant(ConstInt(0) Int) Int) Return(Constant(ConstInt(0) Int)) None))\\n    D(VarDecl(var_decl(\\\"@3.r\\\" FunctionCall(\\\"pong\\\" [Binary(Subtract Var(\\\"@1.n\\\" Int) Constant(ConstInt(1) Int) Int)] Int true) Int None None None None false)))\\n    S(Return(Binary(Add Var(\\\"@3.r\\\" Int) Var(\\\"@2.a\\\" Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"pong\\\" [\\\"@4.n\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@5.b\\\" Binary(Add Var(\\\"@4.n\\\" Int) Constant(ConstInt(5) Int) Int) Int None None None None false)))\\n    S(IfStmt(Binary(LessOrEqual Var(\\\"@4.n\\\" Int) Constant(ConstInt(0) Int) Int) Return(Constant(ConstInt(1) Int)) None))\\n    D(VarDecl(var_decl(\\\"@6.r\\\" FunctionCall(\\\"ping\\\" [Binary(Subtract Var(\\\"@4.n\\\" Int) Constant(ConstInt(1) Int) Int)] Int false) Int None None None None false)))\\n    S(Return(Binary(Add Binary(Multiply Var(\\\"@6.r\\\" Int) Constant(ConstInt(2) Int) Int) Var(\\\"@5.b\\\" Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"ping\\\" [Constant(ConstInt(5) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"ping\" true [\"@1.n\"] [\n"
      "    Binary(LeftShift Var(\"@1.n\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "    JumpIfCmp(GreaterThan Var(\"@1.n\") Constant(ConstInt(0)) \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Subtract Var(\"@1.n\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "    FunctionCall(\"pong\" [Var(\"%2.1\")] Var(\"%3.1\") true false)\n"
      "    Binary(Add Var(\"%3.1\") Var(\"%0.1\") Var(\"%4.1\"))\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  Function(\"pong\" true [\"@4.n\"] [\n"
      "    JumpIfCmp(GreaterThan Var(\"@4.n\") Constant(ConstInt(0)) \".if_end@1\")\n"
      "    Ret(Constant(ConstInt(1)))\n"
      "    Label(\".if_end@1\")\n"
      "    Binary(Subtract Var(\"@4.n\") Constant(ConstInt(1)) Var(\"%7.1\"))\n"
      "    FunctionCall(\"ping\" [Var(\"%7.1\")] Var(\"%8.1\") false false)\n"
      "    Binary(LeftShift Var(\"%8.1\") Constant(ConstInt(1)) Var(\"%9.1\"))\n"
      "    Binary(Add Var(\"%9.1\") Var(\"@4.n\") Var(\"%10.1.ra\"))\n"
      "    Binary(Add Var(\"%10.1.ra\") Constant(ConstInt(5)) Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"ping\" [Constant(ConstInt(5))] Var(\"%11.1\") false false)\n"
      "    Ret(Var(\"%11.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"ping\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"pong\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_recursion_saves.TestCodegen.test_without_the_attribute (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"level\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"deepest\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"remaining\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"descend\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.here\\\" Var(\\\"level\\\" UChar) UChar None None None None false)))\\n    S(IfStmt(Binary(GreaterThan Cast(Int Var(\\\"@0.here\\\" UChar) Int) Cast(Int Var(\\\"deepest\\\" UChar) Int) Int) Expression(Assignment(Var(\\\"deepest\\\" UChar) Var(\\\"@0.here\\\" UChar) UChar)) None))\\n    S(IfStmt(Binary(NotEqual Cast(Int Var(\\\"remaining\\\" UChar) Int) Constant(ConstInt(0) Int) Int) Compound(Block([\\n      S(Expression(Assignment(Var(\\\"remaining\\\" UChar) Cast(UChar Binary(Subtract Cast(Int Var(\\\"remaining\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar)))\\n      S(Expression(Assignment(Var(\\\"level\\\" UChar) Cast(UChar Binary(Add Cast(Int Var(\\\"@0.here\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar)))\\n      S(Expression(FunctionCall(\\\"descend\\\" [] Void false)))\\n      S(Expression(Assignment(Var(\\\"level\\\" UChar) Var(\\\"@0.here\\\" UChar) UChar)))\\n    ])) None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"remaining\\\" UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\\n    S(Expression(FunctionCall(\\\"descend\\\" [] Void false)))\\n    S(Return(Binary(Add Binary(Multiply Cast(Int Var(\\\"deepest\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Cast(Int Var(\\\"level\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"deepest\",40],[\"level\",38],[\"remaining\",39]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Program([\n"
      "  Function(\"descend\" true [] [\n"
      "    Copy(Var(\"level\") Var(\"@0.here.1\"))\n"
      "    JumpIfCmp(LessOrEqual Var(\"@0.here.1\") Var(\"deepest\") \".if_end@0\")\n"
      "    Copy(Var(\"@0.here.1\") Var(\"deepest\"))\n"
      "    Label(\".if_end@0\")\n"
      "    JumpIfFalse(Var(\"remaining\") \".if_end@1\")\n"
      "    Binary(Subtract Var(\"remaining\") Constant(ConstUChar(1)) Var(\"remaining\"))\n"
      "    Binary(Add Var(\"@0.here.1\") Constant(ConstUChar(1)) Var(\"level\"))\n"
      "    FunctionCall(\"descend\" [] None false false)\n"
      "    Copy(Var(\"@0.here.1\") Var(\"level\"))\n"
      "    Label(\".if_end@1\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstUChar(5)) Var(\"remaining\"))\n"
      "    FunctionCall(\"descend\" [] None false false)\n"
      "    ZeroExtend(Var(\"deepest\") Var(\"%12.1\"))\n"
      "    Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%13.1.sr0\") Constant(ConstInt(2)) Var(\"%13.1.sr1\"))\n"
      "    Binary(Add Var(\"%13.1.sr1\") Var(\"%13.1.sr0\") Var(\"%13.1\"))\n"
      "    ZeroExtend(Var(\"level\") Var(\"%14.1\"))\n"
      "    Binary(Add Var(\"%13.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"level\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"deepest\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"remaining\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"descend\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_recursion_saves.TestPrograms.test_programs (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"depth\\\" None UChar None None None None false))\\n  VarDecl(var_decl(\\\"out\\\" None Array(UChar 16) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"walk\\\" [\\\"@0.k\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.a\\\" Cast(UChar Binary(Multiply Cast(Int Var(\\\"@0.k\\\" UChar) Int) Constant(ConstInt(3) Int) Int) UChar) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.b\\\" Var(\\\"depth\\\" UChar) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.w\\\" Cast(UInt Binary(Multiply Cast(Int Var(\\\"@0.k\\\" UChar) Int) Constant(ConstInt(100) Int) Int) UInt) UInt None None None None false)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 16)) Pointer(UChar)) Cast(Int Postfix(Increment Var(\\\"n\\\" UChar) UChar) Int) UChar) Var(\\\"@1.a\\\" UChar) UChar)))\\n    S(IfStmt(Binary(GreaterThan Cast(Int Var(\\\"@0.k\\\" UChar) Int) Constant(ConstInt(0) Int) Int) Compound(Block([\\n      S(Expression(Assignment(Var(\\\"depth\\\" UChar) Cast(UChar Binary(Add Cast(Int Var(\\\"depth\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar)))\\n      S(Expression(FunctionCall(\\\"walk\\\" [Cast(UChar Binary(Subtract Cast(Int Var(\\\"@0.k\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)] Void true)))\\n      S(Expression(Assignment(Var(\\\"depth\\\" UChar) Var(\\\"@2.b\\\" UChar) UChar)))\\n    ])) None))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 16)) Pointer(UChar)) Cast(Int Postfix(Increment Var(\\\"n\\\" UChar) UChar) Int) UChar) Cast(UChar Binary(Add Binary(Add Cast(Int Var(\\\"@1.a\\\" UChar) Int) Cast(Int Cast(UChar Var(\\\"@3.w\\\" UInt) UChar) Int) Int) Cast(Int Var(\\\"@2.b\\\" UChar) Int) Int) UChar) UChar)))\\n  ]) FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"fact\\\" [\\\"@4.x\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@5.y\\\" Var(\\\"@4.x\\\" Int) Int None None None None false)))\\n"
      "    S(IfStmt(Binary(LessOrEqual Var(\\\"@4.x\\\" Int) Constant(ConstInt(1) Int) Int) Return(Constant(ConstInt(1) Int)) None))\\n    S(Expression(Assignment(Var(\\\"@5.y\\\" Int) Binary(Multiply Var(\\\"@5.y\\\" Int) FunctionCall(\\\"fact\\\" [Binary(Subtract Var(\\\"@4.x\\\" Int) Constant(ConstInt(1) Int) Int)] Int true) Int) Int)))\\n    S(Return(Var(\\\"@5.y\\\" Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"walk\\\" [Cast(UChar Constant(ConstInt(4) Int) UChar)] Void false)))\\n    D(VarDecl(var_decl(\\\"@6.s\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@7.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@7.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@7.i\\\" UChar) UChar) Expression(Assignment(Var(\\\"@6.s\\\" Int) Binary(Add Binary(Multiply Var(\\\"@6.s\\\" Int) Constant(ConstInt(3) Int) Int) Cast(Int Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 16)) Pointer(UChar)) Cast(Int Var(\\\"@7.i\\\" UChar) Int) UChar) Int) Int) Int)) \\\".loop@0\\\" None))\\n    S(Return(Binary(BitwiseAnd Binary(BitwiseXor Var(\\\"@6.s\\\" Int) FunctionCall(\\\"fact\\\" [Constant(ConstInt(6) Int)] Int false) Int) Constant(ConstInt(32767) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"depth\",39],[\"n\",38]],\"user_zp_bytes\":[38,39]}",
      false,
      "Program([\n"
      "  Function(\"walk\" true [\"@0.k\"] [\n"
      "    Binary(LeftShift Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%1.1.lo1\"))\n"
      "    Binary(Add Var(\"%1.1.lo1\") Var(\"@0.k\") Var(\"%2.1\"))\n"
      "    Copy(Var(\"depth\") Var(\"@2.b.1\"))\n"
      "    Copy(Var(\"n\") Var(\"%6.1\"))\n"
      "    Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "    IndexedSymbolStore(\"out\" Var(\"%6.1\") Var(\"%2.1\") false 0)\n"
      "    JumpIfCmp(LessOrEqual Var(\"@0.k\") Constant(ConstUChar(0)) \".if_end@0\")\n"
      "    Binary(Add Var(\"depth\") Constant(ConstUChar(1)) Var(\"depth\"))\n"
      "    Binary(Subtract Var(\"@0.k\") Constant(ConstUChar(1)) Var(\"%17.1\"))\n"
      "    FunctionCall(\"walk\" [Var(\"%17.1\")] None true false)\n"
      "    Copy(Var(\"@2.b.1\") Var(\"depth\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(LeftShift Var(\"@0.k\") Constant(ConstInt(2)) Var(\"%19.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%19.1.sr0\") Constant(ConstInt(3)) Var(\"%19.1.sr1\"))\n"
      "    Binary(LeftShift Var(\"%19.1.sr1\") Constant(ConstInt(2)) Var(\"%19.1.sr2\"))\n"
      "    Binary(Subtract Var(\"%19.1.sr2\") Var(\"%19.1.sr1\") Var(\"%19.1.sr3\"))\n"
      "    Binary(Add Var(\"%19.1.sr3\") Var(\"%19.1.sr0\") Var(\"%19.1\"))\n"
      "    Binary(Add Var(\"%2.1\") Var(\"%19.1\") Var(\"%23.1.lo1\"))\n"
      "    Binary(Add Var(\"%23.1.lo1\") Var(\"@2.b.1\") Var(\"%24.1\"))\n"
      "    Copy(Var(\"n\") Var(\"%25.1\"))\n"
      "    Binary(Add Var(\"n\") Constant(ConstUChar(1)) Var(\"n\"))\n"
      "    IndexedSymbolStore(\"out\" Var(\"%25.1\") Var(\"%24.1\") false 0)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"fact\" true [\"@4.x\"] [\n"
      "    JumpIfCmp(GreaterThan Var(\"@4.x\") Constant(ConstInt(1)) \".if_end@1\")\n"
      "    Ret(Constant(ConstInt(1)))\n"
      "    Label(\".if_end@1\")\n"
      "    Binary(Subtract Var(\"@4.x\") Constant(ConstInt(1)) Var(\"%30.1\"))\n"
      "    FunctionCall(\"fact\" [Var(\"%30.1\")] Var(\"%31.1\") true false)\n"
      "    Binary(Multiply Var(\"@4.x\") Var(\"%31.1\") Var(\"%32.1\"))\n"
      "    Ret(Var(\"%32.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    FunctionCall(\"walk\" [Constant(ConstUChar(4))] None false false)\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@6.s.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@7.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@7.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    Binary(LeftShift Var(\"@6.s.2\") Constant(ConstInt(1)) Var(\"%38.1.sr0\"))\n"
      "    Binary(Add Var(\"%38.1.sr0\") Var(\"@6.s.2\") Var(\"%38.1\"))\n"
      "    IndexedLoad(\"out\" Var(\"@7.i.2\") Var(\"%41.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%41.1\") Var(\"%42.1\"))\n"
      "    Binary(Add Var(\"%38.1\") Var(\"%42.1\") Var(\"%43.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@7.i.2\") Constant(ConstUChar(1)) Var(\"%45.1\"))\n"
      "    Copy(Var(\"%43.1\") Var(\"@6.s.2\"))\n"
      "    Copy(Var(\"%45.1\") Var(\"@7.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    FunctionCall(\"fact\" [Constant(ConstInt(6))] Var(\"%46.1\") false false)\n"
      "    Binary(BitwiseXor Var(\"@6.s.2\") Var(\"%46.1\") Var(\"%47.1\"))\n"
      "    Binary(BitwiseAnd Var(\"%47.1\") Constant(ConstInt(32767)) Var(\"%48.1\"))\n"
      "    Ret(Var(\"%48.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"depth\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"out\" true Pointer [ZeroInit(16)])\n"
      "  StaticVariable(\"n\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"walk\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"fact\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_local_allocation.TestPoolGrowth.test_no_frame (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"BG\\\" None Array(Array(UChar 28) 16) Extern None Absolute(24576) None false))\\n  VarDecl(var_decl(\\\"LIMIT\\\" None UChar Extern None Absolute(1) None false))\\n  FunctionDecl(function_decl(\\\"is_floor\\\" [\\\"@0.sprite\\\"] Block([\\n    S(Return(Cast(Bool Binary(LogicalOr Binary(Equal Cast(Int Var(\\\"@0.sprite\\\" UChar) Int) Constant(ConstInt(1) Int) Int) Binary(Equal Cast(Int Var(\\\"@0.sprite\\\" UChar) Int) Constant(ConstInt(2) Int) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.row\\\" \\\"@2.col\\\"] Block([\\n    S(ForStmt(InitExp(None) None None Compound(Block([\\n      S(IfStmt(Binary(NotEqual Cast(Int Subscript(AddressOf(Subscript(AddressOf(Var(\\\"BG\\\" Array(Array(UChar 28) 16)) Pointer(Array(UChar 28))) Cast(Int Var(\\\"@1.row\\\" UChar) Int) Array(UChar 28)) Pointer(UChar)) Cast(Int Var(\\\"@2.col\\\" UChar) Int) UChar) Int) Constant(ConstInt(3) Int) Int) Return(Var(\\\"@1.row\\\" UChar)) None))\\n      D(VarDecl(var_decl(\\\"@3.ladder_row\\\" Var(\\\"@1.row\\\" UChar) UChar None None None None false)))\\n      S(Expression(Postfix(Decrement Var(\\\"@1.row\\\" UChar) UChar)))\\n      S(IfStmt(Binary(LogicalAnd Binary(NotEqual Cast(Int Var(\\\"@2.col\\\" UChar) Int) Constant(ConstInt(0) Int) Int) Binary(LogicalOr FunctionCall(\\\"is_floor\\\" [Subscript(AddressOf(Subscript(AddressOf(Var(\\\"BG\\\" Array(Array(UChar 28) 16)) Pointer(Array(UChar 28))) Cast(Int Var(\\\"@3.ladder_row\\\" UChar) Int) Array(UChar 28)) Pointer(UChar)) Binary(Subtract Cast(Int Var(\\\"@2.col\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)] Bool false) Binary(Equal Cast(Int Subscript(AddressOf(Subscript(AddressOf(Var(\\\"BG\\\" Array(Array(UChar 28) 16)) Pointer(Array(UChar 28))) Cast(Int Var(\\\"@1.row\\\" UChar) Int) Array(UChar 28)) Pointer(UChar)) Binary(Subtract Cast(Int Var(\\\"@2.col\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) Int) Constant(ConstInt(4) Int) Int) Int) Int) Compound(Block("
      "[\\n        S(IfStmt(Binary(LessOrEqual Cast(Int Var(\\\"@1.row\\\" UChar) Int) Cast(Int Var(\\\"LIMIT\\\" UChar) Int) Int) Return(Var(\\\"@1.row\\\" UChar)) None))\\n      ])) None))\\n      S(IfStmt(Binary(LogicalAnd Binary(NotEqual Cast(Int Var(\\\"@2.col\\\" UChar) Int) Constant(ConstInt(27) Int) Int) Binary(LogicalOr FunctionCall(\\\"is_floor\\\" [Subscript(AddressOf(Subscript(AddressOf(Var(\\\"BG\\\" Array(Array(UChar 28) 16)) Pointer(Array(UChar 28))) Cast(Int Var(\\\"@3.ladder_row\\\" UChar) Int) Array(UChar 28)) Pointer(UChar)) Binary(Add Cast(Int Var(\\\"@2.col\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)] Bool false) Binary(Equal Cast(Int Subscript(AddressOf(Subscript(AddressOf(Var(\\\"BG\\\" Array(Array(UChar 28) 16)) Pointer(Array(UChar 28))) Cast(Int Var(\\\"@1.row\\\" UChar) Int) Array(UChar 28)) Pointer(UChar)) Binary(Add Cast(Int Var(\\\"@2.col\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) Int) Constant(ConstInt(4) Int) Int) Int) Int) Compound(Block([\\n        S(IfStmt(Binary(LessOrEqual Cast(Int Var(\\\"@1.row\\\" UChar) Int) Cast(Int Var(\\\"LIMIT\\\" UChar) Int) Int) Return(Var(\\\"@1.row\\\" UChar)) None))\\n      ])) None))\\n      S(IfStmt(Binary(Equal Cast(Int Var(\\\"@1.row\\\" UChar) Int) Constant(ConstInt(0) Int) Int) Return(Var(\\\"@1.row\\\" UChar)) None))\\n    ])) \\\".loop@0\\\" None))\\n  ]) FunType([UChar UChar] UChar) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"LIMIT\\\" UChar) Cast(UChar Constant(ConstInt(9) Int) UChar) UChar)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@4.r\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@4.r\\\" UChar) Int) Constant(ConstInt(16) Int) Int) Postfix(Increment Var(\\\"@4.r\\\" UChar) UChar) Expression(Assignment(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"BG\\\" Array(Array(UChar 28) 16)) Pointer(Array(UChar 28))) Cast(Int Var(\\\"@4.r\\\" UChar) Int) Array(UChar 28)) Pointer(UChar)) Constant(ConstInt(5) Int) UChar) Cast(U"
      "Char Constant(ConstInt(3) Int) UChar) UChar)) \\\".loop@1\\\" None))\\n    S(Expression(Assignment(Subscript(AddressOf(Subscript(AddressOf(Var(\\\"BG\\\" Array(Array(UChar 28) 16)) Pointer(Array(UChar 28))) Constant(ConstInt(9) Int) Array(UChar 28)) Pointer(UChar)) Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) UChar)))\\n    S(Return(Cast(Int FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(12) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar)] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[2,4,0,1,6],\"user_zp_globals\":[[\"LIMIT\",1]],\"user_zp_bytes\":[1]}",
      false,
      "Program([\n"
      "  Function(\"is_floor\" true [\"@0.sprite\"] [\n"
      "    JumpIfCmp(Equal Var(\"@0.sprite\") Constant(ConstUChar(1)) \".or_true@0\")\n"
      "    JumpIfCmp(Equal Var(\"@0.sprite\") Constant(ConstUChar(2)) \".or_true@0\")\n"
      "    Label(\".is_floor@ssa_block@2\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"%5.1\"))\n"
      "    Jump(\".or_end@1\")\n"
      "    Label(\".or_true@0\")\n"
      "    Copy(Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "    Label(\".or_end@1\")\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@1.row\" \"@2.col\"] [\n"
      "    Label(\".f@ssa_preheader@0\")\n"
      "    Copy(Var(\"@1.row\") Var(\"@1.row.1\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@1.row.1\") Var(\"%9.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%9.1\") Var(\"@2.col\") Var(\"%14.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%14.1\") Constant(ConstUChar(3)) \".if_end@2\")\n"
      "    Ret(Var(\"@1.row.1\"))\n"
      "    Label(\".if_end@2\")\n"
      "    Binary(Subtract Var(\"@1.row.1\") Constant(ConstUChar(1)) Var(\"%18.1\"))\n"
      "    JumpIfFalse(Var(\"@2.col\") \".f@cthread@0\")\n"
      "    Binary(Subtract Var(\"@2.col\") Constant(ConstUChar(1)) Var(\"%29.1\"))\n"
      "    ZeroExtend(Var(\"%29.1\") Var(\"%30.1\"))\n"
      "    IndirectIndexedLoad(Var(\"%9.1\") Var(\"%29.1\") Var(\"%32.1\") false)\n"
      "    FunctionCall(\"is_floor\" [Var(\"%32.1\")] Var(\"%33.1\") false false)\n"
      "    JumpIfTrue(Var(\"%33.1\") \".f@jthread@0\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"%18.1\") Var(\"%37.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%37.1\") Var(\"%29.1\") Var(\"%43.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%43.1\") Constant(ConstUChar(4)) \".f@jthread@0\")\n"
      "    Jump(\".if_end@7\")\n"
      "    Label(\".f@jthread@0\")\n"
      "    JumpIfCmp(GreaterThan Var(\"%18.1\") Var(\"LIMIT\") \".if_end@8\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@8\")\n"
      "    Label(\".if_end@7\")\n"
      "    JumpIfCmp(Equal Var(\"@2.col\") Constant(ConstUChar(27)) \".if_end@13\")\n"
      "    Label(\".f@cthread@0\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@1.row.1\") Var(\"%56.1\") false 0 17)\n"
      "    Binary(Add Var(\"@2.col\") Constant(ConstUChar(1)) Var(\"%59.1\"))\n"
      "    ZeroExtend(Var(\"%59.1\") Var(\"%60.1\"))\n"
      "    IndirectIndexedLoad(Var(\"%56.1\") Var(\"%59.1\") Var(\"%62.1\") false)\n"
      "    FunctionCall(\"is_floor\" [Var(\"%62.1\")] Var(\"%63.1\") false false)\n"
      "    JumpIfTrue(Var(\"%63.1\") \".f@jthread@1\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"%18.1\") Var(\"%67.1\") false 0 17)\n"
      "    IndirectIndexedLoad(Var(\"%67.1\") Var(\"%59.1\") Var(\"%73.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%73.1\") Constant(ConstUChar(4)) \".f@jthread@1\")\n"
      "    Jump(\".if_end@13\")\n"
      "    Label(\".f@jthread@1\")\n"
      "    JumpIfCmp(GreaterThan Var(\"%18.1\") Var(\"LIMIT\") \".if_end@14\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@14\")\n"
      "    Label(\".if_end@13\")\n"
      "    JumpIfTrue(Var(\"%18.1\") \".if_end@15\")\n"
      "    Ret(Var(\"%18.1\"))\n"
      "    Label(\".if_end@15\")\n"
      "    Label(\".loop@0_continue\")\n"
      "    Copy(Var(\"%18.1\") Var(\"@1.row.1\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(9)) Var(\"LIMIT\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@4.r.2\"))\n"
      "    Label(\".loop@1_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@4.r.2\") Constant(ConstUChar(16)) \".loop@1_break\")\n"
      "    IndexedLoad(\"BG@rows\" Var(\"@4.r.2\") Var(\"%89.1\") false 0 17)\n"
      "    IndirectIndexedStore(Var(\"%89.1\") Constant(ConstUChar(5)) Constant(ConstUChar(3)) false)\n"
      "    Label(\".loop@1_continue\")\n"
      "    Binary(Add Var(\"@4.r.2\") Constant(ConstUChar(1)) Var(\"@4.r.2\"))\n"
      "    Jump(\".loop@1_start\")\n"
      "    Label(\".loop@1_break\")\n"
      "    GetAddress(Var(\"BG\") Var(\"%94.1\"))\n"
      "    Binary(Add Constant(ConstInt(256)) Var(\"%94.1\") Var(\"%97.1\"))\n"
      "    Store(Constant(ConstUChar(4)) Var(\"%97.1\") false)\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(12)) Constant(ConstUChar(5))] Var(\"%100.1\") false false)\n"
      "    ZeroExtend(Var(\"%100.1\") Var(\"%101.1\"))\n"
      "    Ret(Var(\"%101.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"BG@rows\" false Pointer [StringInit(\"\\x00\\x1c8Tp\\x8c\\xa8\\xc4\\xe0\\xfc\\x184Pl\\x88\\xa4\\xc0``````````aaaaaaa\" 34)])\n"
      "])\n",
      "{\"dict\": [[\"is_floor\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_is_floor__sprite\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__row\", \"__zpabi_f__col\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_local_allocation.TestUnitFallback.test_register_parameters_with_a_frame (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"pool\\\" None Array(UChar 128) None None ZeroPage(128) None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(UChar Binary(Add Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.a\\\" \\\"@2.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@3.s\\\" FunctionCall(\\\"g\\\" [Var(\\\"@1.a\\\" UChar)] UChar false) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@4.t\\\" FunctionCall(\\\"g\\\" [Var(\\\"@2.b\\\" UChar)] UChar false) UChar None None None None false)))\\n    S(Return(Cast(UChar Binary(Add Binary(Add Binary(Add Cast(Int Var(\\\"@3.s\\\" UChar) Int) Cast(Int Var(\\\"@4.t\\\" UChar) Int) Int) Cast(Int Var(\\\"@1.a\\\" UChar) Int) Int) Cast(Int Var(\\\"@2.b\\\" UChar) Int) Int) UChar)))\\n  ]) FunType([UChar UChar] UChar) None None [\\\"auto\\\" \\\"auto\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"pool\\\" Array(UChar 128)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar)] UChar false) UChar)))\\n    S(Return(Cast(Int Subscript(AddressOf(Var(\\\"pool\\\" Array(UChar 128)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"pool\",128]],\"user_zp_bytes\":[128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@1.a\" \"@2.b\"] [\n"
      "    Binary(Add Var(\"@2.b\") Var(\"@1.a\") Var(\"%9.1.lo1.ra.ra\"))\n"
      "    Binary(Add Var(\"@1.a\") Var(\"%9.1.lo1.ra.ra\") Var(\"%11.1.lo1.ra.ra\"))\n"
      "    Binary(Add Var(\"@2.b\") Var(\"%11.1.lo1.ra.ra\") Var(\"%12.1.ra.ra\"))\n"
      "    Binary(Add Constant(ConstUChar(2)) Var(\"%12.1.ra.ra\") Var(\"%12.1\"))\n"
      "    Ret(Var(\"%12.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(3)) Constant(ConstUChar(4))] Var(\"%15.1\") false false)\n"
      "    IndexedSymbolStore(\"pool\" Constant(ConstUChar(0)) Var(\"%15.1\") false 0)\n"
      "    IndexedLoad(\"pool\" Constant(ConstUChar(0)) Var(\"%16.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "    Ret(Var(\"%17.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"pool\" true Pointer [ZeroInit(128)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__a\", \"__zpabi_f__b\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_local_allocation.TestUnitFallback.test_runs (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"pool\\\" None Array(UChar 128) None None ZeroPage(128) None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Cast(UChar Binary(Add Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.a\\\" \\\"@2.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@3.s\\\" FunctionCall(\\\"g\\\" [Var(\\\"@1.a\\\" UChar)] UChar false) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@4.t\\\" FunctionCall(\\\"g\\\" [Var(\\\"@2.b\\\" UChar)] UChar false) UChar None None None None false)))\\n    S(Return(Cast(UChar Binary(Add Binary(Add Binary(Add Cast(Int Var(\\\"@3.s\\\" UChar) Int) Cast(Int Var(\\\"@4.t\\\" UChar) Int) Int) Cast(Int Var(\\\"@1.a\\\" UChar) Int) Int) Cast(Int Var(\\\"@2.b\\\" UChar) Int) Int) UChar)))\\n  ]) FunType([UChar UChar] UChar) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"pool\\\" Array(UChar 128)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar)] UChar false) UChar)))\\n    S(Return(Cast(Int Subscript(AddressOf(Var(\\\"pool\\\" Array(UChar 128)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"pool\",128]],\"user_zp_bytes\":[128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@1.a\" \"@2.b\"] [\n"
      "    Binary(Add Var(\"@2.b\") Var(\"@1.a\") Var(\"%9.1.lo1.ra.ra\"))\n"
      "    Binary(Add Var(\"@1.a\") Var(\"%9.1.lo1.ra.ra\") Var(\"%11.1.lo1.ra.ra\"))\n"
      "    Binary(Add Var(\"@2.b\") Var(\"%11.1.lo1.ra.ra\") Var(\"%12.1.ra.ra\"))\n"
      "    Binary(Add Constant(ConstUChar(2)) Var(\"%12.1.ra.ra\") Var(\"%12.1\"))\n"
      "    Ret(Var(\"%12.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(3)) Constant(ConstUChar(4))] Var(\"%15.1\") false false)\n"
      "    IndexedSymbolStore(\"pool\" Constant(ConstUChar(0)) Var(\"%15.1\") false 0)\n"
      "    IndexedLoad(\"pool\" Constant(ConstUChar(0)) Var(\"%16.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "    Ret(Var(\"%17.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"pool\" true Pointer [ZeroInit(128)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__a\", \"__zpabi_f__b\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_local_allocation.TestUnitFallback.test_slots_below_the_pool (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"pool\\\" None Array(UChar 128) None None ZeroPage(128) None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@0.d\\\" \\\"@1.x\\\"] Block([\\n    S(Return(Cast(UChar Binary(Add Cast(Int Var(\\\"@1.x\\\" UChar) Int) Cast(Int Var(\\\"@0.d\\\" UChar) Int) Int) UChar)))\\n  ]) FunType([UChar UChar] UChar) Static None [\\\"X\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@2.d\\\" \\\"@3.a\\\" \\\"@4.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@5.s\\\" FunctionCall(\\\"g\\\" [Var(\\\"@2.d\\\" UChar) Var(\\\"@3.a\\\" UChar)] UChar false) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@6.t\\\" FunctionCall(\\\"g\\\" [Var(\\\"@2.d\\\" UChar) Var(\\\"@4.b\\\" UChar)] UChar false) UChar None None None None false)))\\n    S(Return(Cast(UChar Binary(Add Binary(Add Binary(Add Cast(Int Var(\\\"@5.s\\\" UChar) Int) Cast(Int Var(\\\"@6.t\\\" UChar) Int) Int) Cast(Int Var(\\\"@3.a\\\" UChar) Int) Int) Cast(Int Var(\\\"@4.b\\\" UChar) Int) Int) UChar)))\\n  ]) FunType([UChar UChar UChar] UChar) None None [\\\"X\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"pool\\\" Array(UChar 128)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar)] UChar false) UChar)))\\n    S(Return(Cast(Int Subscript(AddressOf(Var(\\\"pool\\\" Array(UChar 128)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"pool\",128]],\"user_zp_bytes\":[128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255]}",
      false,
      "Program([\n"
      "  Function(\"g\" false [\"@0.d\" \"@1.x\"] [\n"
      "    Binary(Add Var(\"@1.x\") Var(\"@0.d\") Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "  Function(\"f\" true [\"@2.d\" \"@3.a\" \"@4.b\"] [\n"
      "    FunctionCall(\"g\" [Var(\"@2.d\") Var(\"@3.a\")] Var(\"%4.1\") false false)\n"
      "    FunctionCall(\"g\" [Var(\"@2.d\") Var(\"@4.b\")] Var(\"%5.1\") false false)\n"
      "    Binary(Add Var(\"%4.1\") Var(\"%5.1\") Var(\"%10.1.lo1\"))\n"
      "    Binary(Add Var(\"%10.1.lo1\") Var(\"@3.a\") Var(\"%12.1.lo1\"))\n"
      "    Binary(Add Var(\"%12.1.lo1\") Var(\"@4.b\") Var(\"%13.1\"))\n"
      "    Ret(Var(\"%13.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1)) Constant(ConstUChar(3)) Constant(ConstUChar(4))] Var(\"%17.1\") false false)\n"
      "    IndexedSymbolStore(\"pool\" Constant(ConstUChar(0)) Var(\"%17.1\") false 0)\n"
      "    IndexedLoad(\"pool\" Constant(ConstUChar(0)) Var(\"%18.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%18.1\") Var(\"%19.1\"))\n"
      "    Ret(Var(\"%19.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"pool\" true Pointer [ZeroInit(128)])\n"
      "])\n",
      "{\"dict\": [[\"g\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_g__d\", \"__zpabi_g__x\"], \"addrs\": [128, 129], \"param_registers\": [\"X\", null], \"pinned\": []}}], [\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__d\", \"__zpabi_f__a\", \"__zpabi_f__b\"], \"addrs\": [128, 129, 130], \"param_registers\": [\"X\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_leaf_zp_abi.TestLeafZpAbi.test_caller_with_locals_calls_zp_abi_correctly (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"add\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None \\\"zp_abi\\\" [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [\\\"@2.n\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@3.x\\\" Binary(Add Var(\\\"@2.n\\\" Int) Constant(ConstInt(10) Int) Int) Int None None None None false)))\\n    D(VarDecl(var_decl(\\\"@4.y\\\" Binary(Add Var(\\\"@2.n\\\" Int) Constant(ConstInt(20) Int) Int) Int None None None None false)))\\n    S(Return(FunctionCall(\\\"add\\\" [Var(\\\"@3.x\\\" Int) Var(\\\"@4.y\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"add\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Binary(Add Var(\"@0.a\") Var(\"@1.b\") Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [\"@2.n\"] [\n"
      "    Binary(Add Var(\"@2.n\") Constant(ConstInt(10)) Var(\"%1.1\"))\n"
      "    Binary(Add Var(\"@2.n\") Constant(ConstInt(20)) Var(\"%2.1\"))\n"
      "    FunctionCall(\"add\" [Var(\"%1.1\") Var(\"%2.1\")] Var(\"%3.1\") false false)\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"add\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_add__a_0\", \"__zpabi_add__a_1\", \"__zpabi_add__b_0\", \"__zpabi_add__b_1\"], \"addrs\": [128, 129, 130, 131], \"param_registers\": [\"A\", \"X\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_main__n_0\", \"__zpabi_main__n_1\"], \"addrs\": [128, 129], \"param_registers\": [null, null], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_leaf_zp_abi.TestLeafZpAbi.test_eligible_non_leaf_emits_no_prologue (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"touch\\\" [\\\"@0.x\\\"] Block([\\n    S(Expression(Cast(Void Var(\\\"@0.x\\\" UChar) Void)))\\n  ]) FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"wrap\\\" [\\\"@1.a\\\" \\\"@2.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@3.x\\\" Var(\\\"@1.a\\\" UChar) UChar None None None None false)))\\n    S(Expression(FunctionCall(\\\"touch\\\" [Var(\\\"@2.b\\\" UChar)] Void false)))\\n    S(Return(Var(\\\"@3.x\\\" UChar)))\\n  ]) FunType([UChar UChar] UChar) None \\\"zp_abi\\\" [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"wrap\\\" [Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar)] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"touch\" true [\"@0.x\"] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"wrap\" true [\"@1.a\" \"@2.b\"] [\n"
      "    FunctionCall(\"touch\" [Var(\"@2.b\")] None false false)\n"
      "    Ret(Var(\"@1.a\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"wrap\" [Constant(ConstUChar(7)) Constant(ConstUChar(3))] Var(\"%2.1\") false false)\n"
      "    ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"touch\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_touch__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"wrap\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_wrap__a\", \"__zpabi_wrap__b\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_leaf_zp_abi.TestLeafZpAbi.test_extern_zp_abi_call_site_uses_zp (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"helper\\\" [\\\"@0.base\\\" \\\"@1.x\\\"] None FunType([Int Int] Int) Extern \\\"zp_abi\\\" [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"helper\\\" [Constant(ConstInt(1) Int) Constant(ConstInt(7) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"helper\" [Constant(ConstInt(1)) Constant(ConstInt(7))] Var(\"%0.1\") false false)\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"helper\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_helper__base_0\", \"__zpabi_helper__base_1\", \"__zpabi_helper__x_0\", \"__zpabi_helper__x_1\"], \"addrs\": [128, 129, 130, 131], \"param_registers\": [\"A\", \"X\", null, null], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_leaf_zp_abi.TestLeafZpAbi.test_zp_abi_caller_callee_get_disjoint_slots (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"noop\\\" [\\\"@0.w\\\" \\\"@1.x\\\" \\\"@2.y\\\"] Block([\\n    S(Expression(Cast(Void Var(\\\"@0.w\\\" Int) Void)))\\n    S(Expression(Cast(Void Var(\\\"@1.x\\\" Int) Void)))\\n    S(Expression(Cast(Void Var(\\\"@2.y\\\" Int) Void)))\\n  ]) FunType([Int Int Int] Void) None \\\"zp_abi\\\" [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"caller\\\" [\\\"@3.v\\\" \\\"@4.a\\\" \\\"@5.b\\\"] Block([\\n    S(Expression(FunctionCall(\\\"noop\\\" [Var(\\\"@3.v\\\" Int) Binary(Add Var(\\\"@4.a\\\" Int) Constant(ConstInt(100) Int) Int) Constant(ConstInt(0) Int)] Void false)))\\n    S(Return(Var(\\\"@5.b\\\" Int)))\\n  ]) FunType([Int Int Int] Int) None \\\"zp_abi\\\" [\\\"\\\" \\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"caller\\\" [Constant(ConstInt(1) Int) Constant(ConstInt(2) Int) Constant(ConstInt(5) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"noop\" true [\"@0.w\" \"@1.x\" \"@2.y\"] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"caller\" true [\"@3.v\" \"@4.a\" \"@5.b\"] [\n"
      "    Binary(Add Var(\"@4.a\") Constant(ConstInt(100)) Var(\"%0.1\"))\n"
      "    FunctionCall(\"noop\" [Var(\"@3.v\") Var(\"%0.1\") Constant(ConstInt(0))] None false false)\n"
      "    Ret(Var(\"@5.b\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"caller\" [Constant(ConstInt(1)) Constant(ConstInt(2)) Constant(ConstInt(5))] Var(\"%1.1\") false false)\n"
      "    Ret(Var(\"%1.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"noop\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_noop__w_0\", \"__zpabi_noop__w_1\", \"__zpabi_noop__x_0\", \"__zpabi_noop__x_1\", \"__zpabi_noop__y_0\", \"__zpabi_noop__y_1\"], \"addrs\": [128, 129, 130, 131, 132, 133], \"param_registers\": [\"A\", \"X\", null, null, null, null], \"pinned\": []}}], [\"caller\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_caller__v_0\", \"__zpabi_caller__v_1\", \"__zpabi_caller__a_0\", \"__zpabi_caller__a_1\", \"__zpabi_caller__b_0\", \"__zpabi_caller__b_1\"], \"addrs\": [128, 129, 130, 131, 132, 133], \"param_registers\": [\"A\", \"X\", null, null, null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_leaf_zp_abi.TestLeafZpAbi.test_zp_abi_caller_no_allocate_stack_pre_call (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"add\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.a\\\" Int) Var(\\\"@1.b\\\" Int) Int)))\\n  ]) FunType([Int Int] Int) None \\\"zp_abi\\\" [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"add\\\" [Constant(ConstInt(3) Int) Constant(ConstInt(4) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"add\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Binary(Add Var(\"@0.a\") Var(\"@1.b\") Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"add\" [Constant(ConstInt(3)) Constant(ConstInt(4))] Var(\"%1.1\") false false)\n"
      "    Ret(Var(\"%1.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"add\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_add__a_0\", \"__zpabi_add__a_1\", \"__zpabi_add__b_0\", \"__zpabi_add__b_1\"], \"addrs\": [128, 129, 130, 131], \"param_registers\": [\"A\", \"X\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_leaf_zp_abi.TestLeafZpAbi.test_zp_abi_nonrecursive_call_accepted (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"helper\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)))\\n  ]) FunType([Int] Int) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"wrap\\\" [\\\"@1.x\\\"] Block([\\n    S(Return(FunctionCall(\\\"helper\\\" [Var(\\\"@1.x\\\" Int)] Int false)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"wrap\\\" [Constant(ConstInt(3) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"helper\" true [\"@0.x\"] [\n"
      "    Binary(Add Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "  Function(\"wrap\" true [\"@1.x\"] [\n"
      "    FunctionCall(\"helper\" [Var(\"@1.x\")] Var(\"%1.1\") false false)\n"
      "    Ret(Var(\"%1.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"wrap\" [Constant(ConstInt(3))] Var(\"%2.1\") false false)\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"helper\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_helper__x_0\", \"__zpabi_helper__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"wrap\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_wrap__x_0\", \"__zpabi_wrap__x_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_leaf_zp_abi.TestLeafZpAbi.test_zp_abi_recursion_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"rec\\\" [\\\"@0.x\\\"] Block([\\n    S(Return(Conditional(Var(\\\"@0.x\\\" Int) FunctionCall(\\\"rec\\\" [Binary(Subtract Var(\\\"@0.x\\\" Int) Constant(ConstInt(1) Int) Int)] Int false) Constant(ConstInt(0) Int) Int)))\\n  ]) FunType([Int] Int) None \\\"zp_abi\\\" [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(FunctionCall(\\\"rec\\\" [Constant(ConstInt(3) Int)] Int false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"rec\" true [\"@0.x\"] [\n"
      "    JumpIfFalse(Var(\"@0.x\") \".cond_else@0\")\n"
      "    Label(\".rec@ssa_block@1\")\n"
      "    Binary(Subtract Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "    FunctionCall(\"rec\" [Var(\"%1.1\")] Var(\"%0.3\") false false)\n"
      "    Jump(\".cond_end@1\")\n"
      "    Label(\".cond_else@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"%0.3\"))\n"
      "    Label(\".cond_end@1\")\n"
      "    Ret(Var(\"%0.3\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"rec\" [Constant(ConstInt(3))] Var(\"%3.1\") false false)\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `rec` declared `[[c6502::zp_abi]]` but is reachable from itself through the static call graph (direct or mutual recursion); a recursive call would clobber the parameter ZP slots before the outer activation returned\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestAbiErrors.test_errors (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] None FunType([UInt] Void) None \\\"zp_abi\\\" [\\\"Y\\\"] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UInt Constant(ConstInt(1) Int) UInt)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUInt(1))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` parameter `@0.a` is 2 bytes, but `in_y` names 1 register; name one register for each byte, low byte first\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestAbiErrors.test_errors (call 2)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([]) FunType([UChar UChar] Void) None None [\\\"Y\\\" \\\"Y\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1)) Constant(ConstUChar(2))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` declares a `register` parameter or local but isn't eligible for the ZP-passing ABI: function `f`: parameters `@0.a` and `@1.b` both arrive in Y\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestAbiErrors.test_errors (call 3)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([]) FunType([UChar UInt] Void) None None [\\\"A\\\" \\\"auto\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UInt Constant(ConstInt(2) Int) UInt)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1)) Constant(ConstUInt(2))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f`: too many `register` argument bytes — a `register` parameter without `in_a`, `in_x`, or `in_y` goes in A, then X, and the free ones here are: X\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestAbiErrors.test_errors (call 4)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] None FunType([UChar] Void) None None [\\\"Y\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@1.a\\\"] Block([]) FunType([UChar] Void) None None [\\\"auto\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@1.a\"] [\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f`: the declarations disagree on which parameters arrive in registers or at a zero-page address, or which ones (`a in_y` against `register a`); every declaration has to give the same calling convention\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestShape.test_address_bytes_go_straight_to_registers (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"iob\\\" None Array(UChar 17) None None None None false))\\n  FunctionDecl(function_decl(\\\"RWTS\\\" [\\\"@0.hi\\\" \\\"@1.lo\\\"] None FunType([UChar UChar] UChar) None \\\"zp_abi\\\" [\\\"A\\\" \\\"Y\\\"] true 10496 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"RWTS\\\" [Cast(UChar Binary(RightShift Cast(UInt AddressOf(Var(\\\"iob\\\" Array(UChar 17)) Pointer(UChar)) UInt) Constant(ConstInt(8) Int) UInt) UChar) Cast(UChar Cast(UInt AddressOf(Var(\\\"iob\\\" Array(UChar 17)) Pointer(UChar)) UInt) UChar)] Bool false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"iob\") Var(\"%0.1\"))\n"
      "    Binary(RightShift Var(\"%0.1\") Constant(ConstInt(8)) Var(\"%2.1\"))\n"
      "    Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "    GetAddress(Var(\"iob\") Var(\"%4.1\"))\n"
      "    Truncate(Var(\"%4.1\") Var(\"%6.1\"))\n"
      "    FunctionCall(\"RWTS\" [Var(\"%3.1\") Var(\"%6.1\")] Var(\"%7.1\") false false)\n"
      "    ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    Ret(Var(\"%8.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"iob\" true Pointer [ZeroInit(17)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"RWTS\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_RWTS__hi\", \"__zpabi_RWTS__lo\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestShape.test_callee_stores_y_and_caller_loads_it (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"out\\\" None Array(UChar 2) None None None None false))\\n  VarDecl(var_decl(\\\"g\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.hi\\\" \\\"@1.lo\\\"] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 2)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Var(\\\"@0.hi\\\" UChar) UChar)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 2)) Pointer(UChar)) Constant(ConstInt(1) Int) UChar) Var(\\\"@1.lo\\\" UChar) UChar)))\\n  ]) FunType([UChar UChar] Void) None None [\\\"A\\\" \\\"Y\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Var(\\\"g\\\" UChar)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"put\" true [\"@0.hi\" \"@1.lo\"] [\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"@0.hi\") false 0)\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Var(\"@1.lo\") false 0)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(1)) Var(\"g\")] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"out\" true Pointer [ZeroInit(2)])\n"
      "  StaticVariable(\"g\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__hi\", \"__zpabi_put__lo\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestShape.test_pointer_goes_straight_to_registers (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"iob\\\" false [member_decl(\\\"t\\\" Array(UChar 17))]))\\n  VarDecl(var_decl(\\\"iob\\\" None Structure(\\\"iob\\\") None None None None false))\\n  FunctionDecl(function_decl(\\\"RWTS\\\" [\\\"@0.p\\\"] None FunType([Pointer(Structure(\\\"iob\\\"))] UChar) None \\\"zp_abi\\\" [\\\"YA\\\"] true 10496 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"RWTS\\\" [AddressOf(Var(\\\"iob\\\" Structure(\\\"iob\\\")) Pointer(Structure(\\\"iob\\\")))] Bool false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"iob\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"iob\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"t\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Array(UChar 17)\\n\"}, \"byte_offset\": 0}}], \"size\": 17, \"complete\": true}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"iob\") Var(\"%0.1\"))\n"
      "    FunctionCall(\"RWTS\" [Var(\"%0.1\")] Var(\"%1.1\") false false)\n"
      "    ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"iob\" true Pointer [ZeroInit(17)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"RWTS\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_RWTS__p_0\", \"__zpabi_RWTS__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"Y\", \"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestSim.test_a_argument_from_a_frame_slot (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"rom\\\" [\\\"@0.a\\\" \\\"@1.y\\\"] None FunType([UChar UChar] UChar) None \\\"zp_abi\\\" [\\\"A\\\" \\\"Y\\\"] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@2.a\\\" Cast(UChar Constant(ConstInt(7) Int) UChar) UChar None None None None false)))\\n    D(VarDecl(var_decl(\\\"@3.b\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@4.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@4.i\\\" UChar) Int) Constant(ConstInt(3) Int) Int) Postfix(Increment Var(\\\"@4.i\\\" UChar) UChar) Compound(Block([\\n      S(Expression(Postfix(Increment Var(\\\"@2.a\\\" UChar) UChar)))\\n      S(Expression(CompoundAssignment(Add Var(\\\"@3.b\\\" UChar) Constant(ConstInt(2) Int) Int UChar)))\\n    ])) \\\".loop@0\\\" None))\\n    S(Return(Cast(Int FunctionCall(\\\"rom\\\" [Var(\\\"@2.a\\\" UChar) Var(\\\"@3.b\\\" UChar)] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(7)) Var(\"@2.a.2\"))\n"
      "    Copy(Constant(ConstUChar(5)) Var(\"@3.b.2\"))\n"
      "    Copy(Constant(ConstUChar(3)) Var(\"@4.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@4.i.2\") Constant(ConstUChar(0)) \".loop@0_break\")\n"
      "    Binary(Add Var(\"@2.a.2\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "    Binary(Add Var(\"@3.b.2\") Constant(ConstUChar(2)) Var(\"%9.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Subtract Var(\"@4.i.2\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "    Copy(Var(\"%6.1\") Var(\"@2.a.2\"))\n"
      "    Copy(Var(\"%9.1\") Var(\"@3.b.2\"))\n"
      "    Copy(Var(\"%11.1\") Var(\"@4.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    FunctionCall(\"rom\" [Var(\"@2.a.2\") Var(\"@3.b.2\")] Var(\"%12.1\") false false)\n"
      "    ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "    Ret(Var(\"%13.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"rom\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_rom__a\", \"__zpabi_rom__y\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestSim.test_compiled_function (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"out\\\" None Array(UChar 2) None None None None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(9) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.hi\\\" \\\"@1.lo\\\"] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 2)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Var(\\\"@0.hi\\\" UChar) UChar)))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 2)) Pointer(UChar)) Constant(ConstInt(1) Int) UChar) Var(\\\"@1.lo\\\" UChar) UChar)))\\n  ]) FunType([UChar UChar] Void) None None [\\\"A\\\" \\\"Y\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@2.a\\\" Cast(UChar Binary(Add Cast(Int Var(\\\"g\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar None None None None false)))\\n    S(Expression(FunctionCall(\\\"put\\\" [Var(\\\"@2.a\\\" UChar) Var(\\\"g\\\" UChar)] Void false)))\\n    S(Return(Binary(Add Binary(Multiply Cast(Int Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 2)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Int) Constant(ConstInt(100) Int) Int) Cast(Int Subscript(AddressOf(Var(\\\"out\\\" Array(UChar 2)) Pointer(UChar)) Constant(ConstInt(1) Int) UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"put\" true [\"@0.hi\" \"@1.lo\"] [\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"@0.hi\") false 0)\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Var(\"@1.lo\") false 0)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Binary(Add Var(\"g\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "    FunctionCall(\"put\" [Var(\"%2.1\") Var(\"g\")] None false false)\n"
      "    IndexedLoad(\"out\" Constant(ConstUChar(0)) Var(\"%3.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "    Binary(Multiply Var(\"%4.1\") Constant(ConstInt(100)) Var(\"%5.1\"))\n"
      "    IndexedLoad(\"out\" Constant(ConstUChar(1)) Var(\"%6.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "    Binary(Add Var(\"%5.1\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    Ret(Var(\"%8.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"out\" true Pointer [ZeroInit(2)])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(9)])\n"
      "])\n",
      "{\"dict\": [[\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__hi\", \"__zpabi_put__lo\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestSim.test_fixed_address_routine (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"rom\\\" [\\\"@0.a\\\" \\\"@1.y\\\"] None FunType([UChar UChar] UChar) None \\\"zp_abi\\\" [\\\"A\\\" \\\"Y\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(20) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Add Cast(Int FunctionCall(\\\"rom\\\" [Var(\\\"g\\\" UChar) Cast(UChar Constant(ConstInt(3) Int) UChar)] UChar false) Int) Cast(Int FunctionCall(\\\"rom\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Var(\\\"g\\\" UChar)] UChar false) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"rom\" [Var(\"g\") Constant(ConstUChar(3))] Var(\"%1.1\") false false)\n"
      "    ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "    FunctionCall(\"rom\" [Constant(ConstUChar(1)) Var(\"g\")] Var(\"%4.1\") false false)\n"
      "    ZeroExtend(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "    Binary(Add Var(\"%2.1\") Var(\"%5.1\") Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(20)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"rom\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_rom__a\", \"__zpabi_rom__y\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"Y\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestSim.test_fixed_address_routine_with_a_pointer (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"blk\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UChar) member_decl(\\\"c\\\" UChar)]))\\n  VarDecl(var_decl(\\\"blk\\\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(9) Int) UChar) Cast(UChar Constant(ConstInt(11) Int) UChar)] Structure(\\\"blk\\\") None) Structure(\\\"blk\\\") None None None None false))\\n  FunctionDecl(function_decl(\\\"next\\\" [\\\"@0.p\\\"] None FunType([Pointer(Structure(\\\"blk\\\"))] UChar) None \\\"zp_abi\\\" [\\\"YA\\\"] false 53504 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"next\\\" [AddressOf(Var(\\\"blk\\\" Structure(\\\"blk\\\")) Pointer(Structure(\\\"blk\\\")))] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"blk\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"blk\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 2}}], \"size\": 3, \"complete\": true}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"blk\") Var(\"%0.1\"))\n"
      "    FunctionCall(\"next\" [Var(\"%0.1\")] Var(\"%1.1\") false false)\n"
      "    ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"blk\" true Pointer [UCharInit(7) UCharInit(9) UCharInit(11)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"next\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_next__p_0\", \"__zpabi_next__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"Y\", \"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestSim.test_pair_and_positional (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"sum\\\" [\\\"@0.w\\\" \\\"@1.k\\\"] Block([\\n    S(Return(Binary(Add Var(\\\"@0.w\\\" UInt) Cast(UInt Cast(Int Var(\\\"@1.k\\\" UChar) Int) UInt) UInt)))\\n  ]) FunType([UInt UChar] UInt) None None [\\\"XY\\\" \\\"auto\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"swap\\\" [\\\"@2.w\\\"] Block([\\n    S(Return(Binary(BitwiseOr Binary(LeftShift Var(\\\"@2.w\\\" UInt) Constant(ConstInt(8) Int) UInt) Binary(RightShift Var(\\\"@2.w\\\" UInt) Constant(ConstInt(8) Int) UInt) UInt)))\\n  ]) FunType([UInt] UInt) None None [\\\"YA\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@3.s\\\" FunctionCall(\\\"sum\\\" [Cast(UInt Constant(ConstInt(4660) Int) UInt) Cast(UChar Constant(ConstInt(3) Int) UChar)] UInt false) UInt None None None None false)))\\n    S(Return(Binary(Add Binary(Equal Var(\\\"@3.s\\\" UInt) Cast(UInt Constant(ConstInt(4663) Int) UInt) Int) Binary(Multiply Binary(Equal FunctionCall(\\\"swap\\\" [Constant(ConstUInt(43981) UInt)] UInt false) Constant(ConstUInt(52651) UInt) Int) Constant(ConstInt(2) Int) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"sum\" true [\"@0.w\" \"@1.k\"] [\n"
      "    ZeroExtend(Var(\"@1.k\") Var(\"%1.1\"))\n"
      "    Binary(Add Var(\"@0.w\") Var(\"%1.1\") Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"swap\" true [\"@2.w\"] [\n"
      "    Binary(LeftShift Var(\"@2.w\") Constant(ConstInt(8)) Var(\"%3.1\"))\n"
      "    Binary(RightShift Var(\"@2.w\") Constant(ConstInt(8)) Var(\"%4.1\"))\n"
      "    Binary(BitwiseOr Var(\"%3.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"sum\" [Constant(ConstUInt(4660)) Constant(ConstUChar(3))] Var(\"%8.1\") false false)\n"
      "    Binary(Equal Var(\"%8.1\") Constant(ConstUInt(4663)) Var(\"%10.1\"))\n"
      "    FunctionCall(\"swap\" [Constant(ConstUInt(43981))] Var(\"%11.1\") false false)\n"
      "    Binary(Equal Var(\"%11.1\") Constant(ConstUInt(52651)) Var(\"%12.1\"))\n"
      "    Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(1)) Var(\"%13.1\"))\n"
      "    Binary(Add Var(\"%10.1\") Var(\"%13.1\") Var(\"%14.1\"))\n"
      "    Ret(Var(\"%14.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"sum\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_sum__w_0\", \"__zpabi_sum__w_1\", \"__zpabi_sum__k\"], \"addrs\": [128, 129, 130], \"param_registers\": [\"X\", \"Y\", \"A\"], \"pinned\": []}}], [\"swap\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_swap__w_0\", \"__zpabi_swap__w_1\"], \"addrs\": [128, 129], \"param_registers\": [\"Y\", \"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_param_registers.TestSim.test_pointer_parameters (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"blk\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UChar) member_decl(\\\"c\\\" UChar)]))\\n  VarDecl(var_decl(\\\"blk\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar)] Structure(\\\"blk\\\") None) Structure(\\\"blk\\\") None None None None false))\\n  VarDecl(var_decl(\\\"tab\\\" InitList([Cast(UChar Constant(ConstInt(10) Int) UChar) Cast(UChar Constant(ConstInt(20) Int) UChar) Cast(UChar Constant(ConstInt(30) Int) UChar) Cast(UChar Constant(ConstInt(40) Int) UChar)] Array(UChar 4) None) Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"second\\\" [\\\"@0.p\\\"] Block([\\n    S(Return(Subscript(Var(\\\"@0.p\\\" Pointer(Const(UChar))) Constant(ConstInt(1) Int) Const(UChar))))\\n  ]) FunType([Pointer(Const(UChar))] UChar) None None [\\\"YA\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"sum\\\" [\\\"@1.b\\\" \\\"@2.k\\\"] Block([\\n    S(Return(Cast(UChar Binary(Add Binary(Add Cast(Int Arrow(Var(\\\"@1.b\\\" Pointer(Structure(\\\"blk\\\"))) \\\"a\\\" UChar) Int) Cast(Int Arrow(Var(\\\"@1.b\\\" Pointer(Structure(\\\"blk\\\"))) \\\"c\\\" UChar) Int) Int) Cast(Int Var(\\\"@2.k\\\" UChar) Int) Int) UChar)))\\n  ]) FunType([Pointer(Structure(\\\"blk\\\")) UChar] UChar) None None [\\\"XY\\\" \\\"A\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Add Cast(Int FunctionCall(\\\"second\\\" [AddressOf(Var(\\\"tab\\\" Array(UChar 4)) Pointer(UChar))] UChar false) Int) Cast(Int FunctionCall(\\\"sum\\\" [AddressOf(Var(\\\"blk\\\" Structure(\\\"blk\\\")) Pointer(Structure(\\\"blk\\\"))) Cast(UChar Constant(ConstInt(100) Int) UChar)] UChar false) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"blk\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"blk\", \"is_union\": false, \"members\": [{\"object\": \"p"
      "asses.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 1}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"c\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 2}}], \"size\": 3, \"complete\": true}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"second\" true [\"@0.p\"] [\n"
      "    IndirectIndexedLoad(Var(\"@0.p\") Constant(ConstUChar(1)) Var(\"%1.1\") false)\n"
      "    Ret(Var(\"%1.1\"))\n"
      "  ])\n"
      "  Function(\"sum\" true [\"@1.b\" \"@2.k\"] [\n"
      "    Load(Var(\"@1.b\") Var(\"%2.1\") false)\n"
      "    IndirectIndexedLoad(Var(\"@1.b\") Constant(ConstUChar(2)) Var(\"%5.1\") false)\n"
      "    Binary(Add Var(\"%2.1\") Var(\"%5.1\") Var(\"%9.1.lo1\"))\n"
      "    Binary(Add Var(\"%9.1.lo1\") Var(\"@2.k\") Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"tab\") Var(\"%11.1\"))\n"
      "    FunctionCall(\"second\" [Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "    ZeroExtend(Var(\"%12.1\") Var(\"%13.1\"))\n"
      "    GetAddress(Var(\"blk\") Var(\"%14.1\"))\n"
      "    FunctionCall(\"sum\" [Var(\"%14.1\") Constant(ConstUChar(100))] Var(\"%16.1\") false false)\n"
      "    ZeroExtend(Var(\"%16.1\") Var(\"%17.1\"))\n"
      "    Binary(Add Var(\"%13.1\") Var(\"%17.1\") Var(\"%18.1\"))\n"
      "    Ret(Var(\"%18.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"blk\" true Pointer [UCharInit(1) UCharInit(2) UCharInit(3)])\n"
      "  StaticVariable(\"tab\" true Pointer [StringInit(\"\\x0a\\x14\\x1e(\" 4)])\n"
      "])\n",
      "{\"dict\": [[\"second\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_second__p_0\", \"__zpabi_second__p_1\"], \"addrs\": [128, 129], \"param_registers\": [\"Y\", \"A\"], \"pinned\": []}}], [\"sum\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_sum__b_0\", \"__zpabi_sum__b_1\", \"__zpabi_sum__k\"], \"addrs\": [128, 129, 130], \"param_registers\": [\"X\", \"Y\", \"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestCodegen.test_equs_and_absolute_store (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"COUT\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 65005 [] None false))\\n  VarDecl(var_decl(\\\"scratch\\\" None Volatile(UChar) Extern None Absolute(1913) None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"COUT\\\" [Cast(UChar Constant(ConstInt(193) Int) UChar)] Void false)))\\n    S(Expression(Assignment(Var(\\\"scratch\\\" Volatile(UChar)) Cast(Volatile(UChar) Constant(ConstInt(145) Int) Volatile(UChar)) Volatile(UChar))))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"COUT\" [Constant(ConstUChar(193))] None false false)\n"
      "    IndexedSymbolStore(\"scratch\" Constant(ConstUChar(0)) Constant(ConstUChar(145)) true 0)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"COUT\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_COUT__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestCodegen.test_without_zp_abi_uses_soft_stack (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None None [\\\"\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [\\\"@1.c\\\"] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Var(\\\"@1.c\\\" UChar)] Void false)))\\n  ]) FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [\"@1.c\"] [\n"
      "    FunctionCall(\"f\" [Var(\"@1.c\")] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"g\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestConventionErrors.test_rejected_declarations (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None None [\\\"auto\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: function `f` has a fixed address and a `register` or `zeropage(addr)` parameter but no `[[c6502::zp_abi]]`; without zp_abi its arguments go on the soft stack, so declare it `[[c6502::zp_abi, c6502::address(...)]]`\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestConventionErrors.test_rejected_declarations (call 2)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] None FunType([UChar UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\" \\\"\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(1)) Constant(ConstUChar(2))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: zp_abi function `f` has a fixed address, so every parameter has to be passed in a register or at a zero-page address its code reads; declare each parameter `register` (A, then X), or name its register with `in_a`, `in_x`, or `in_y`, at most 3 bytes in all, or give it `[[c6502::zeropage(addr)]]`\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestConventionErrors.test_rejected_declarations (call 3)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] ULong) None \\\"zp_abi\\\" [] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [] ULong false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: zp_abi function `f` has a fixed address but returns a value that doesn't fit in A and X; anything wider than 2 bytes comes back in HARGS\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestConventionErrors.test_rejected_declarations (call 4)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 4096 [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.p\\\" AddressOf(Var(\\\"f\\\" FunType([UChar] Void)) Pointer(FunType([UChar] Void))) Pointer(FunType([UChar] Void)) None None None None false)))\\n    S(Expression(FunctionCall(\\\"@1.p\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    GetAddress(Var(\"f\") Var(\"%0.1\"))\n"
      "    IndirectCall(Var(\"%0.1\") [Constant(ConstUChar(1))] None)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "exception: AbiSelectionError: the address of `f` is taken, but it's a zp_abi function with a fixed address; a call through a pointer passes its arguments on the soft stack, which the code at that address doesn't read\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestLink.test_conflicting_addresses_rejected (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"out\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None None [\\\"\\\"] false 65005 [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"out\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"out\" [Constant(ConstUChar(1))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestLink.test_fixed_names_bound_once (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"COUT\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 65005 [] None false))\\n  VarDecl(var_decl(\\\"KBD\\\" None Volatile(UChar) Extern None Absolute(49152) None false))\\n  FunctionDecl(function_decl(\\\"say\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"say\\\" [Var(\\\"KBD\\\" Volatile(UChar))] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    IndexedLoad(\"KBD\" Constant(ConstUChar(0)) Var(\"%0.1\") true 0 0)\n"
      "    FunctionCall(\"say\" [Var(\"%0.1\")] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"COUT\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_COUT__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"say\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_say__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_address_of_fixed_object (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"port\\\" None UChar Extern None Absolute(51200) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int AddressOf(Var(\\\"port\\\" UChar) Pointer(UChar)) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"port\") Var(\"%1.1\"))\n"
      "    Ret(Var(\"%1.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_call_fixed_routine (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"inc\\\" [\\\"@0.c\\\"] None FunType([UChar] UChar) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.t\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@2.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@2.i\\\" UChar) Int) Constant(ConstInt(3) Int) Int) Postfix(Increment Var(\\\"@2.i\\\" UChar) UChar) Expression(CompoundAssignment(Add Var(\\\"@1.t\\\" UChar) Cast(Int FunctionCall(\\\"inc\\\" [Var(\\\"@2.i\\\" UChar)] UChar false) Int) Int UChar)) \\\".loop@0\\\" None))\\n    S(Return(Cast(Int Var(\\\"@1.t\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.t.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@2.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@2.i.2\") Constant(ConstUChar(3)) \".loop@0_break\")\n"
      "    FunctionCall(\"inc\" [Var(\"@2.i.2\")] Var(\"%4.1\") false false)\n"
      "    Binary(Add Var(\"@1.t.2\") Var(\"%4.1\") Var(\"%8.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%10.1\"))\n"
      "    Copy(Var(\"%8.1\") Var(\"@1.t.2\"))\n"
      "    Copy(Var(\"%10.1\") Var(\"@2.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    ZeroExtend(Var(\"@1.t.2\") Var(\"%11.1\"))\n"
      "    Ret(Var(\"%11.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"inc\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_inc__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_carry_flag_result (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"at_least_5\\\" [\\\"@0.a\\\"] None FunType([UChar] UChar) Extern \\\"zp_abi\\\" [\\\"auto\\\"] true 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.n\\\" Constant(ConstInt(0) Int) Int None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@2.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@2.i\\\" UChar) Int) Constant(ConstInt(9) Int) Int) Postfix(Increment Var(\\\"@2.i\\\" UChar) UChar) IfStmt(FunctionCall(\\\"at_least_5\\\" [Var(\\\"@2.i\\\" UChar)] Bool false) Expression(CompoundAssignment(Add Var(\\\"@1.n\\\" Int) Constant(ConstInt(10) Int) Int Int)) Expression(CompoundAssignment(Add Var(\\\"@1.n\\\" Int) Constant(ConstInt(1) Int) Int Int))) \\\".loop@0\\\" None))\\n    S(Return(Var(\\\"@1.n\\\" Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstInt(0)) Var(\"@1.n.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@2.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@2.i.2\") Constant(ConstUChar(9)) \".loop@0_break\")\n"
      "    FunctionCall(\"at_least_5\" [Var(\"@2.i.2\")] Var(\"%3.1\") false false)\n"
      "    JumpIfFalse(Var(\"%3.1\") \".if_else@1\")\n"
      "    Label(\".main@ssa_block@2\")\n"
      "    Binary(Add Var(\"@1.n.2\") Constant(ConstInt(10)) Var(\"@1.n.5\"))\n"
      "    Jump(\".if_end@0\")\n"
      "    Label(\".if_else@1\")\n"
      "    Binary(Add Var(\"@1.n.2\") Constant(ConstInt(1)) Var(\"@1.n.5\"))\n"
      "    Label(\".if_end@0\")\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "    Copy(Var(\"@1.n.5\") Var(\"@1.n.2\"))\n"
      "    Copy(Var(\"%7.1\") Var(\"@2.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Var(\"@1.n.2\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"at_least_5\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_at_least_5__a\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_fixed_objects (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"port\\\" None Volatile(UChar) Extern None Absolute(51200) None false))\\n  VarDecl(var_decl(\\\"word\\\" None UInt Extern None Absolute(51216) None false))\\n  VarDecl(var_decl(\\\"tbl\\\" None Array(UChar 4) Extern None Absolute(51232) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"port\\\" Volatile(UChar)) Cast(Volatile(UChar) Constant(ConstInt(5) Int) Volatile(UChar)) Volatile(UChar))))\\n    D(VarDecl(var_decl(\\\"@0.t\\\" Var(\\\"word\\\" UInt) UInt None None None None false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@1.i\\\" UChar) Int) Constant(ConstInt(4) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(CompoundAssignment(Add Var(\\\"@0.t\\\" UInt) Cast(UInt Subscript(AddressOf(Var(\\\"tbl\\\" Array(UChar 4)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar) UInt) UInt UInt)) \\\".loop@0\\\" None))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"tbl\\\" Array(UChar 4)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Cast(UChar Binary(Add Cast(Int Var(\\\"port\\\" Volatile(UChar)) Int) Constant(ConstInt(1) Int) Int) UChar) UChar)))\\n    S(Return(Cast(Int Var(\\\"@0.t\\\" UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    IndexedSymbolStore(\"port\" Constant(ConstUChar(0)) Constant(ConstUChar(5)) true 0)\n"
      "    Copy(Var(\"word\") Var(\"@0.t.2\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Constant(ConstUChar(4)) \".loop@0_break\")\n"
      "    IndexedLoad(\"tbl\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%6.1\") Var(\"%7.1\"))\n"
      "    Binary(Add Var(\"@0.t.2\") Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"%10.1\"))\n"
      "    Copy(Var(\"%8.1\") Var(\"@0.t.2\"))\n"
      "    Copy(Var(\"%10.1\") Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    IndexedLoad(\"port\" Constant(ConstUChar(0)) Var(\"%11.1\") true 0 0)\n"
      "    Binary(Add Var(\"%11.1\") Constant(ConstUChar(1)) Var(\"%14.1\"))\n"
      "    IndexedSymbolStore(\"tbl\" Constant(ConstUChar(0)) Var(\"%14.1\") false 0)\n"
      "    Copy(Var(\"@0.t.2\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_pointer_result (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"where\\\" [] None FunType([] Pointer(UChar)) Extern \\\"zp_abi\\\" [] false 53248 [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Cast(UInt FunctionCall(\\\"where\\\" [] Pointer(UChar) false) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"where\" [] Var(\"%2.1\") false false)\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"where\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_poll_key (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"KBD\\\" None Volatile(UChar) Extern None Absolute(49152) None false))\\n  VarDecl(var_decl(\\\"KBDSTRB\\\" None Volatile(UChar) Extern None Absolute(49168) None false))\\n  FunctionDecl(function_decl(\\\"poll_key\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.k\\\" Var(\\\"KBD\\\" Volatile(UChar)) UChar None None None None false)))\\n    S(IfStmt(Binary(BitwiseAnd Cast(Int Var(\\\"@0.k\\\" UChar) Int) Constant(ConstInt(128) Int) Int) Expression(Assignment(Var(\\\"KBDSTRB\\\" Volatile(UChar)) Cast(Volatile(UChar) Constant(ConstInt(0) Int) Volatile(UChar)) Volatile(UChar))) None))\\n    S(Return(Var(\\\"@0.k\\\" UChar)))\\n  ]) FunType([] UChar) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"poll_key\\\" [] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"poll_key\" true [] [\n"
      "    IndexedLoad(\"KBD\" Constant(ConstUChar(0)) Var(\"%0.1\") true 0 0)\n"
      "    JumpIfMasked(Var(\"%0.1\") 128 false \".if_end@0\")\n"
      "    IndexedSymbolStore(\"KBDSTRB\" Constant(ConstUChar(0)) Constant(ConstUChar(0)) true 0)\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"poll_key\" [] Var(\"%4.1\") false false)\n"
      "    ZeroExtend(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"poll_key\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_two_byte_argument_and_result (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"swap\\\" [\\\"@0.v\\\"] None FunType([UInt] UInt) Extern \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"k\\\" Cast(UInt Constant(ConstInt(4660) Int) UInt) UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Binary(Add FunctionCall(\\\"swap\\\" [Var(\\\"k\\\" UInt)] UInt false) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"swap\" [Var(\"k\")] Var(\"%0.1\") false false)\n"
      "    Binary(Add Var(\"%0.1\") Constant(ConstUInt(1)) Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"k\" true UInt [UIntInit(4660)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"swap\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_swap__v_0\", \"__zpabi_swap__v_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestSim.test_wait_through_pointer (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(WhileStmt(Unary(LogicalNot Binary(BitwiseAnd Cast(Int Dereference(Cast(Pointer(Volatile(UChar)) Constant(ConstUInt(49152) UInt) Pointer(Volatile(UChar))) Volatile(UChar)) Int) Constant(ConstInt(128) Int) Int) Int) Null \\\".loop@0\\\"))\\n    S(Return(Constant(ConstInt(7) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".loop@0_continue\")\n"
      "    Load(Constant(ConstUInt(49152)) Var(\"%1.1\") true)\n"
      "    Binary(BitwiseAnd Var(\"%1.1\") Constant(ConstUChar(128)) Var(\"%3.1.n0\"))\n"
      "    ZeroExtend(Var(\"%3.1.n0\") Var(\"%3.1\"))\n"
      "    JumpIfTrue(Var(\"%3.1\") \".loop@0_break\")\n"
      "    Jump(\".loop@0_continue\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(Constant(ConstInt(7)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_address_attr.TestTypeCheck.test_block_scope_extern (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"x\\\" None UChar Extern None Absolute(49152) None false)))\\n    S(Return(Cast(Int Var(\\\"x\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    ZeroExtend(Var(\"x\") Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_noreturn_call.TestCallGraph.test_call_with_arguments_stays_an_edge (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"out\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] None FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"r\\\" [\\\"@1.a\\\"] Block([\\n    S(ForStmt(InitExp(None) None None Expression(FunctionCall(\\\"f\\\" [Var(\\\"@1.a\\\" UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([UChar] Void) None None [\\\"\\\"] false None [] None true))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@2.x\\\"] Block([\\n    S(IfStmt(Binary(Equal Cast(Int Var(\\\"@2.x\\\" UChar) Int) Constant(ConstInt(3) Int) Int) Expression(FunctionCall(\\\"r\\\" [Var(\\\"@2.x\\\" UChar)] Void false)) None))\\n    S(Expression(Assignment(Var(\\\"out\\\" UChar) Var(\\\"@2.x\\\" UChar) UChar)))\\n  ]) FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"r\" true [\"@1.a\"] [\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"f\" [Var(\"@1.a\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"f\" true [\"@2.x\"] [\n"
      "    ZeroExtend(Var(\"@2.x\") Var(\"%0\"))\n"
      "    Binary(Equal Var(\"%0\") Constant(ConstInt(3)) Var(\"%1\"))\n"
      "    JumpIfFalse(Var(\"%1\") \".if_end@0\")\n"
      "    FunctionCall(\"r\" [Var(\"@2.x\")] None false true)\n"
      "    Label(\".if_end@0\")\n"
      "    Copy(Var(\"@2.x\") Var(\"out\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"out\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"r\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"f\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_noreturn_call.TestCallGraph.test_code (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"key\\\" None Volatile(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"last\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"command_loop\\\" [] None FunType([] Void) None None [] false None [] None true))\\n  FunctionDecl(function_decl(\\\"escape\\\" [\\\"@0.k\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@0.k\\\" UChar) Int) Constant(ConstInt(155) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"get_key\\\" [\\\"@1.n\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.k\\\" Cast(UChar Binary(Add Cast(Int Var(\\\"key\\\" Volatile(UChar)) Int) Cast(Int Var(\\\"@1.n\\\" UChar) Int) Int) UChar) UChar None None None None false)))\\n    S(IfStmt(FunctionCall(\\\"escape\\\" [Var(\\\"@2.k\\\" UChar)] Bool false) Expression(FunctionCall(\\\"command_loop\\\" [] Void false)) None))\\n    S(Return(Var(\\\"@2.k\\\" UChar)))\\n  ]) FunType([UChar] UChar) Static None [\\\"X\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"command_loop\\\" [] Block([\\n    S(ForStmt(InitExp(None) None None Expression(Assignment(Var(\\\"last\\\" UChar) FunctionCall(\\\"get_key\\\" [Cast(UChar Constant(ConstInt(3) Int) UChar)] UChar false) UChar)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"last\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"escape\" false [\"@0.k\"] [\n"
      "    Binary(Equal Var(\"@0.k\") Constant(ConstUChar(155)) Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"get_key\" false [\"@1.n\"] [\n"
      "    IndexedLoad(\"key\" Constant(ConstUChar(0)) Var(\"%3.1\") true 0 0)\n"
      "    Binary(Add Var(\"%3.1\") Var(\"@1.n\") Var(\"%7.1\"))\n"
      "    FunctionCall(\"escape\" [Var(\"%7.1\")] Var(\"%8.1\") false false)\n"
      "    JumpIfFalse(Var(\"%8.1\") \".if_end@0\")\n"
      "    FunctionCall(\"command_loop\" [] None false true)\n"
      "    Ret(None)\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  Function(\"command_loop\" true [] [\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"get_key\" [Constant(ConstUChar(3))] Var(\"last\") false false)\n"
      "    Jump(\".loop@0_start\")\n"
      "  ])\n"
      "  StaticVariable(\"key\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"last\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"escape\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_escape__k\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"get_key\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_get_key__n\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"command_loop\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_noreturn_call.TestCallGraph.test_cycle_through_the_call_is_zp_abi (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"key\\\" None Volatile(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"last\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"command_loop\\\" [] None FunType([] Void) None None [] false None [] None true))\\n  FunctionDecl(function_decl(\\\"escape\\\" [\\\"@0.k\\\"] Block([\\n    S(Return(Cast(Bool Binary(Equal Cast(Int Var(\\\"@0.k\\\" UChar) Int) Constant(ConstInt(155) Int) Int) Bool)))\\n  ]) FunType([UChar] UChar) Static None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"get_key\\\" [\\\"@1.n\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.k\\\" Cast(UChar Binary(Add Cast(Int Var(\\\"key\\\" Volatile(UChar)) Int) Cast(Int Var(\\\"@1.n\\\" UChar) Int) Int) UChar) UChar None None None None false)))\\n    S(IfStmt(FunctionCall(\\\"escape\\\" [Var(\\\"@2.k\\\" UChar)] Bool false) Expression(FunctionCall(\\\"command_loop\\\" [] Void false)) None))\\n    S(Return(Var(\\\"@2.k\\\" UChar)))\\n  ]) FunType([UChar] UChar) Static None [\\\"X\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"command_loop\\\" [] Block([\\n    S(ForStmt(InitExp(None) None None Expression(Assignment(Var(\\\"last\\\" UChar) FunctionCall(\\\"get_key\\\" [Cast(UChar Constant(ConstInt(3) Int) UChar)] UChar false) UChar)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"escape\" false [\"@0.k\"] [\n"
      "    ZeroExtend(Var(\"@0.k\") Var(\"%0\"))\n"
      "    Binary(Equal Var(\"%0\") Constant(ConstInt(155)) Var(\"%1\"))\n"
      "    Truncate(Var(\"%1\") Var(\"%2\"))\n"
      "    Ret(Var(\"%2\"))\n"
      "  ])\n"
      "  Function(\"get_key\" false [\"@1.n\"] [\n"
      "    IndexedLoad(\"key\" Constant(ConstUChar(0)) Var(\"%3\") true 0 0)\n"
      "    ZeroExtend(Var(\"%3\") Var(\"%4\"))\n"
      "    ZeroExtend(Var(\"@1.n\") Var(\"%5\"))\n"
      "    Binary(Add Var(\"%4\") Var(\"%5\") Var(\"%6\"))\n"
      "    Truncate(Var(\"%6\") Var(\"%7\"))\n"
      "    Copy(Var(\"%7\") Var(\"@2.k\"))\n"
      "    FunctionCall(\"escape\" [Var(\"@2.k\")] Var(\"%8\") false false)\n"
      "    JumpIfFalse(Var(\"%8\") \".if_end@0\")\n"
      "    FunctionCall(\"command_loop\" [] None false true)\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(Var(\"@2.k\"))\n"
      "  ])\n"
      "  Function(\"command_loop\" true [] [\n"
      "    Label(\".loop@0_start\")\n"
      "    Truncate(Constant(ConstInt(3)) Var(\"%9\"))\n"
      "    FunctionCall(\"get_key\" [Var(\"%9\")] Var(\"%10\") false false)\n"
      "    Copy(Var(\"%10\") Var(\"last\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"key\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"last\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"escape\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_escape__k\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"get_key\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_get_key__n\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"command_loop\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_noreturn_call.TestCallGraph.test_recursion_through_ordinary_calls_stays_a_cycle (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"reset\\\" [] None FunType([] Void) None None [] false None [] None true))\\n  FunctionDecl(function_decl(\\\"g\\\" [] None FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(IfStmt(Var(\\\"n\\\" UChar) Expression(FunctionCall(\\\"g\\\" [] Void false)) None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(Postfix(Decrement Var(\\\"n\\\" UChar) UChar)))\\n    S(IfStmt(Binary(Equal Cast(Int Var(\\\"n\\\" UChar) Int) Constant(ConstInt(7) Int) Int) Expression(FunctionCall(\\\"reset\\\" [] Void false)) None))\\n    S(Expression(FunctionCall(\\\"f\\\" [] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"reset\\\" [] Block([\\n    S(ForStmt(InitExp(None) None None Expression(FunctionCall(\\\"f\\\" [] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    JumpIfFalse(Var(\"n\") \".if_end@0\")\n"
      "    FunctionCall(\"g\" [] None false false)\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"g\" true [] [\n"
      "    Copy(Var(\"n\") Var(\"%0\"))\n"
      "    Binary(Subtract Var(\"n\") Constant(ConstUChar(1)) Var(\"%1\"))\n"
      "    Copy(Var(\"%1\") Var(\"n\"))\n"
      "    ZeroExtend(Var(\"n\") Var(\"%2\"))\n"
      "    Binary(Equal Var(\"%2\") Constant(ConstInt(7)) Var(\"%3\"))\n"
      "    JumpIfFalse(Var(\"%3\") \".if_end@1\")\n"
      "    FunctionCall(\"reset\" [] None false true)\n"
      "    Label(\".if_end@1\")\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"reset\" true [] [\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"n\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"g\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}], [\"reset\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_noreturn_call.TestCycleRuns.test_one_unit (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"out\\\" None Array(Volatile(UChar) 3) None None None None false))\\n  VarDecl(var_decl(\\\"steps\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"reset_game\\\" [] None FunType([] Void) None None [] false None [] None true))\\n  FunctionDecl(function_decl(\\\"deep\\\" [\\\"@0.n\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.v\\\" Cast(UChar Binary(Add Cast(Int Var(\\\"@0.n\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar None None None None false)))\\n    S(IfStmt(Binary(Equal Cast(Int Prefix(Increment Var(\\\"steps\\\" UChar) UChar) Int) Constant(ConstInt(3) Int) Int) Compound(Block([\\n      S(Expression(Assignment(Var(\\\"steps\\\" UChar) Cast(UChar Constant(ConstInt(0) Int) UChar) UChar)))\\n      S(Expression(FunctionCall(\\\"reset_game\\\" [] Void false)))\\n    ])) None))\\n    S(Return(Var(\\\"@1.v\\\" UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"middle\\\" [\\\"@2.n\\\"] Block([\\n    S(Return(Cast(UChar Binary(Add Cast(Int FunctionCall(\\\"deep\\\" [Var(\\\"@2.n\\\" UChar)] UChar false) Int) Cast(Int FunctionCall(\\\"deep\\\" [Cast(UChar Binary(Add Cast(Int Var(\\\"@2.n\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)] UChar false) Int) Int) UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"resets\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"middle\\\" [\\\"@3.n\\\"] None FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"halt\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(1) Int) Volatile(UChar)) Cast(Volatile(UChar) Constant(ConstInt(170) Int) Volatile(UChar)) Volatile(UChar))))\\n    S(ForStmt(InitExp(None) None None Compound(Block([])) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) Static None [] false None [] None true))\\n  FunctionD"
      "ecl(function_decl(\\\"reset_game\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(0) Int) Volatile(UChar)) Prefix(Increment Var(\\\"resets\\\" UChar) UChar) Volatile(UChar))))\\n    S(IfStmt(Binary(Equal Cast(Int Var(\\\"resets\\\" UChar) Int) Constant(ConstInt(200) Int) Int) Expression(FunctionCall(\\\"halt\\\" [] Void false)) None))\\n    S(ForStmt(InitExp(None) None None Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(2) Int) Volatile(UChar)) FunctionCall(\\\"middle\\\" [Var(\\\"resets\\\" UChar)] UChar false) Volatile(UChar))) \\\".loop@1\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None true))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"reset_game\\\" [] Void false)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"resets\",38],[\"steps\",39]],\"user_zp_bytes\":[38,39]}",
      false,
      "Program([\n"
      "  Function(\"deep\" true [\"@0.n\"] [\n"
      "    Binary(Add Var(\"@0.n\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "    Binary(Add Var(\"steps\") Constant(ConstUChar(1)) Var(\"steps\"))\n"
      "    JumpIfCmp(NotEqual Var(\"steps\") Constant(ConstUChar(3)) \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"steps\"))\n"
      "    FunctionCall(\"reset_game\" [] None false true)\n"
      "    Ret(None)\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"middle\" true [\"@2.n\"] [\n"
      "    FunctionCall(\"deep\" [Var(\"@2.n\")] Var(\"%7.1\") false false)\n"
      "    Binary(Add Var(\"@2.n\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "    FunctionCall(\"deep\" [Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "    Binary(Add Var(\"%7.1\") Var(\"%12.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  Function(\"halt\" false [] [\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Constant(ConstUChar(170)) true 0)\n"
      "    Label(\".loop@0_start\")\n"
      "    Jump(\".loop@0_start\")\n"
      "  ])\n"
      "  Function(\"reset_game\" true [] [\n"
      "    Binary(Add Var(\"resets\") Constant(ConstUChar(1)) Var(\"resets\"))\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"resets\") true 0)\n"
      "    JumpIfCmp(NotEqual Var(\"resets\") Constant(ConstUChar(200)) \".if_end@1\")\n"
      "    FunctionCall(\"halt\" [] None false true)\n"
      "    Ret(None)\n"
      "    Label(\".if_end@1\")\n"
      "    Label(\".loop@1_start\")\n"
      "    FunctionCall(\"middle\" [Var(\"resets\")] Var(\"%20.1\") false false)\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(2)) Var(\"%20.1\") true 0)\n"
      "    Jump(\".loop@1_start\")\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"reset_game\" [] None false true)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"out\" true Pointer [ZeroInit(3)])\n"
      "  StaticVariable(\"steps\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"resets\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"deep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_deep__n\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"middle\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_middle__n\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"halt\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"reset_game\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_noreturn_call.TestCycleRuns.test_two_units (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"out\\\" None Array(Volatile(UChar) 3) Extern None None None false))\\n  VarDecl(var_decl(\\\"steps\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"reset_game\\\" [] None FunType([] Void) None None [] false None [] None true))\\n  FunctionDecl(function_decl(\\\"deep\\\" [\\\"@0.n\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.v\\\" Cast(UChar Binary(Add Cast(Int Var(\\\"@0.n\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar) UChar None None None None false)))\\n    S(IfStmt(Binary(Equal Cast(Int Prefix(Increment Var(\\\"steps\\\" UChar) UChar) Int) Constant(ConstInt(3) Int) Int) Compound(Block([\\n      S(Expression(Assignment(Var(\\\"steps\\\" UChar) Cast(UChar Constant(ConstInt(0) Int) UChar) UChar)))\\n      S(Expression(FunctionCall(\\\"reset_game\\\" [] Void false)))\\n    ])) None))\\n    S(Return(Var(\\\"@1.v\\\" UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"middle\\\" [\\\"@2.n\\\"] Block([\\n    S(Return(Cast(UChar Binary(Add Cast(Int FunctionCall(\\\"deep\\\" [Var(\\\"@2.n\\\" UChar)] UChar false) Int) Cast(Int FunctionCall(\\\"deep\\\" [Cast(UChar Binary(Add Cast(Int Var(\\\"@2.n\\\" UChar) Int) Constant(ConstInt(1) Int) Int) UChar)] UChar false) Int) Int) UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"steps\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"deep\" true [\"@0.n\"] [\n"
      "    Binary(Add Var(\"@0.n\") Constant(ConstUChar(1)) Var(\"%2.1\"))\n"
      "    Binary(Add Var(\"steps\") Constant(ConstUChar(1)) Var(\"steps\"))\n"
      "    JumpIfCmp(NotEqual Var(\"steps\") Constant(ConstUChar(3)) \".if_end@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"steps\"))\n"
      "    FunctionCall(\"reset_game\" [] None false true)\n"
      "    Ret(None)\n"
      "    Label(\".if_end@0\")\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  Function(\"middle\" true [\"@2.n\"] [\n"
      "    FunctionCall(\"deep\" [Var(\"@2.n\")] Var(\"%7.1\") false false)\n"
      "    Binary(Add Var(\"@2.n\") Constant(ConstUChar(1)) Var(\"%11.1\"))\n"
      "    FunctionCall(\"deep\" [Var(\"%11.1\")] Var(\"%12.1\") false false)\n"
      "    Binary(Add Var(\"%7.1\") Var(\"%12.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"steps\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"deep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_deep__n\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"middle\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_middle__n\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"reset_game\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_noreturn_call.TestRuns.test_call_becomes_a_jump (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"out\\\" None Array(Volatile(UChar) 3) None None None None false))\\n  FunctionDecl(function_decl(\\\"stop\\\" [\\\"@0.code\\\"] None FunType([UChar] Void) None None [\\\"\\\"] false None [] None true))\\n  FunctionDecl(function_decl(\\\"stop\\\" [\\\"@1.code\\\"] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(2) Int) Volatile(UChar)) Var(\\\"@1.code\\\" UChar) Volatile(UChar))))\\n    S(ForStmt(InitExp(None) None None Compound(Block([])) \\\".loop@0\\\" None))\\n  ]) FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"check\\\" [\\\"@2.n\\\"] Block([\\n    S(IfStmt(Binary(Equal Cast(Int Var(\\\"@2.n\\\" UChar) Int) Constant(ConstInt(0) Int) Int) Expression(FunctionCall(\\\"stop\\\" [Cast(UChar Constant(ConstInt(7) Int) UChar)] Void false)) None))\\n    S(Return(Cast(UChar Binary(Divide Constant(ConstInt(100) Int) Cast(Int Var(\\\"@2.n\\\" UChar) Int) Int) UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(0) Int) Volatile(UChar)) FunctionCall(\\\"check\\\" [Cast(UChar Constant(ConstInt(4) Int) UChar)] UChar false) Volatile(UChar))))\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"out\\\" Array(Volatile(UChar) 3)) Pointer(Volatile(UChar))) Constant(ConstInt(1) Int) Volatile(UChar)) FunctionCall(\\\"check\\\" [Cast(UChar Constant(ConstInt(0) Int) UChar)] UChar false) Volatile(UChar))))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"stop\" true [\"@1.code\"] [\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(2)) Var(\"@1.code\") true 0)\n"
      "    Label(\".loop@0_start\")\n"
      "    Jump(\".loop@0_start\")\n"
      "  ])\n"
      "  Function(\"check\" true [\"@2.n\"] [\n"
      "    JumpIfTrue(Var(\"@2.n\") \".if_end@0\")\n"
      "    FunctionCall(\"stop\" [Constant(ConstUChar(7))] None false true)\n"
      "    Ret(None)\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Divide Constant(ConstUChar(100)) Var(\"@2.n\") Var(\"%4.1.nb0\"))\n"
      "    Ret(Var(\"%4.1.nb0\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"check\" [Constant(ConstUChar(4))] Var(\"%7.1\") false false)\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(0)) Var(\"%7.1\") true 0)\n"
      "    FunctionCall(\"check\" [Constant(ConstUChar(0))] Var(\"%9.1\") false false)\n"
      "    IndexedSymbolStore(\"out\" Constant(ConstUChar(1)) Var(\"%9.1\") true 0)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"out\" true Pointer [ZeroInit(3)])\n"
      "])\n",
      "{\"dict\": [[\"stop\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_stop__code\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"check\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_check__n\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_statement_attributes.TestLoopAttributes.test_break_and_continue_in_a_kept_loop (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"even\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.x\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.n\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)))\\n    S(ForStmt(InitExp(None) None None Compound(Block([\\n      S(IfStmt(Binary(Equal Cast(Int Prefix(Decrement Var(\\\"@0.x\\\" UChar) UChar) Int) Constant(ConstInt(0) Int) Int) BreakStmt(\\\".loop@0\\\") None))\\n      S(Expression(Postfix(Increment Var(\\\"@1.n\\\" UChar) UChar)))\\n      S(IfStmt(Binary(BitwiseAnd Cast(Int Var(\\\"@0.x\\\" UChar) Int) Constant(ConstInt(1) Int) Int) ContinueStmt(\\\".loop@0\\\") None))\\n      S(Expression(Postfix(Increment Var(\\\"even\\\" UChar) UChar)))\\n    ])) \\\".loop@0\\\" \\\"keep\\\"))\\n    S(Return(Var(\\\"@1.n\\\" UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Binary(Add Binary(Multiply Cast(Int FunctionCall(\\\"f\\\" [Cast(UChar Constant(ConstInt(7) Int) UChar)] UChar false) Int) Constant(ConstInt(16) Int) Int) Cast(Int Var(\\\"even\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"even\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.x\"] [\n"
      "    Label(\".f@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.n.2\"))\n"
      "    Copy(Var(\"@0.x\") Var(\"@0.x.1\"))\n"
      "    Label(\".loop@0_keep_start\")\n"
      "    Binary(Subtract Var(\"@0.x.1\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "    JumpIfTrue(Var(\"%1.1\") \".if_end@0\")\n"
      "    Jump(\".loop@0_keep_break\")\n"
      "    Label(\".if_end@0\")\n"
      "    Binary(Add Var(\"@1.n.2\") Constant(ConstUChar(1)) Var(\"%5.1\"))\n"
      "    JumpIfMasked(Var(\"%1.1\") 1 false \".if_end@1\")\n"
      "    Jump(\".loop@0_keep_continue\")\n"
      "    Label(\".if_end@1\")\n"
      "    Binary(Add Var(\"even\") Constant(ConstUChar(1)) Var(\"even\"))\n"
      "    Label(\".loop@0_keep_continue\")\n"
      "    Copy(Var(\"%5.1\") Var(\"@1.n.2\"))\n"
      "    Copy(Var(\"%1.1\") Var(\"@0.x.1\"))\n"
      "    Jump(\".loop@0_keep_start\")\n"
      "    Label(\".loop@0_keep_break\")\n"
      "    Ret(Var(\"@1.n.2\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"f\" [Constant(ConstUChar(7))] Var(\"%11.1\") false false)\n"
      "    ZeroExtend(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "    Binary(LeftShift Var(\"%12.1\") Constant(ConstInt(4)) Var(\"%13.1\"))\n"
      "    ZeroExtend(Var(\"even\") Var(\"%14.1\"))\n"
      "    Binary(Add Var(\"%13.1\") Var(\"%14.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"even\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__x\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestCountDownInY.test_counter_in_memory_without_the_promise (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"tick\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.c\\\" Var(\\\"n\\\" UChar) UChar None None None None false)))\\n    S(DoWhileStmt(Compound(Block([\\n      S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(0) Int) UChar)] Void false)))\\n    ])) Binary(NotEqual Cast(Int Prefix(Decrement Var(\\\"@1.c\\\" UChar) UChar) Int) Constant(ConstInt(0) Int) Int) \\\".loop@0\\\"))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"tick\" true [] [\n"
      "    Label(\".tick@ssa_block@0\")\n"
      "    Copy(Var(\"n\") Var(\"@1.c.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(0))] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Subtract Var(\"@1.c.2\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "    Copy(Var(\"%1.1\") Var(\"@1.c.2\"))\n"
      "    JumpIfTrue(Var(\"%1.1\") \".loop@0_start\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(5)])\n"
      "])\n",
      "{\"dict\": [[\"tick\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestCountDownInY.test_counter_in_y (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"Y\\\"] None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"tick\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.c\\\" Var(\\\"n\\\" UChar) UChar None None None None false)))\\n    S(DoWhileStmt(Compound(Block([\\n      S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(0) Int) UChar)] Void false)))\\n    ])) Binary(NotEqual Cast(Int Prefix(Decrement Var(\\\"@1.c\\\" UChar) UChar) Int) Constant(ConstInt(0) Int) Int) \\\".loop@0\\\"))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"tick\" true [] [\n"
      "    Label(\".tick@ssa_block@0\")\n"
      "    Copy(Var(\"n\") Var(\"@1.c.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(0))] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Subtract Var(\"@1.c.2\") Constant(ConstUChar(1)) Var(\"%1.1\"))\n"
      "    Copy(Var(\"%1.1\") Var(\"@1.c.2\"))\n"
      "    JumpIfTrue(Var(\"%1.1\") \".loop@0_start\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(5)])\n"
      "])\n",
      "{\"dict\": [[\"tick\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestCountUpLoop.test_call_before_the_loop_not_wrapped (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"buf\\\" None Array(UChar 4) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" None UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(1) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"put\\\" [Cast(UChar Constant(ConstInt(2) Int) UChar)] Void false)))\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(1))] None false false)\n"
      "    FunctionCall(\"put\" [Constant(ConstUChar(2))] None false false)\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%8.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%8.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [ZeroInit(4)])\n"
      "  StaticVariable(\"n\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"send\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestCountUpLoop.test_counter_in_x_across_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"X\\\"] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"send\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestCountUpLoop.test_runs (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(NotEqual Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"send\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestImmediateInX.test_kept_across_a_preserving_call (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestImmediateInX.test_memory_reloaded (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestImmediateInX.test_reloaded_after_a_plain_call (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"lose\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"lose\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionCall(\"lose\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"lose\" [Constant(ConstUChar(5))] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestImmediateInX.test_runs (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"keep\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [\\\"X\\\"] None false))\\n  FunctionDecl(function_decl(\\\"lose\\\" [\\\"@1.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"X\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"g\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(5) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Cast(UChar Constant(ConstInt(7) Int) UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n    S(Expression(FunctionCall(\\\"keep\\\" [Var(\\\"g\\\" UChar)] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(5))] None false false)\n"
      "    FunctionCall(\"keep\" [Constant(ConstUChar(7))] None false false)\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    FunctionCall(\"keep\" [Var(\"g\")] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"keep\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_keep__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}], [\"lose\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_lose__c\"], \"addrs\": [128], \"param_registers\": [\"X\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestRegisterLocalInY.test_in_y_across_the_call (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [\\\"Y\\\"] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.i\\\" None UChar Register None None None false)))\\n    S(ForStmt(InitExp(Assignment(Var(\\\"@1.i\\\" UChar) Cast(UChar Constant(ConstInt(0) Int) UChar) UChar)) Binary(LessThan Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"send\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestRegisterLocalInY.test_rejected_without_the_promise (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"put\\\" [\\\"@0.c\\\"] None FunType([UChar] Void) None \\\"zp_abi\\\" [\\\"auto\\\"] false 53248 [] None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(8) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"send\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@1.i\\\" None UChar Register None None None false)))\\n    S(ForStmt(InitExp(Assignment(Var(\\\"@1.i\\\" UChar) Cast(UChar Constant(ConstInt(0) Int) UChar) UChar)) Binary(LessThan Cast(Int Var(\\\"@1.i\\\" UChar) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"put\\\" [Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@1.i\\\" UChar) Int) UChar)] Void false)) \\\".loop@0\\\" None))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"send\\\" [] Void false)))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"send\" true [] [\n"
      "    Label(\".send@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Var(\"n\") \".loop@0_break\")\n"
      "    IndexedLoad(\"buf\" Var(\"@1.i.2\") Var(\"%6.1\") false 0 0)\n"
      "    FunctionCall(\"put\" [Var(\"%6.1\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"send\" [] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(8)])\n"
      "])\n",
      "{\"dict\": [[\"send\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"put\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_put__c\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_preserves.TestTypeCheck.test_accepted (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"f\\\" [] None FunType([] Int) None None [] false 4096 [\\\"Y\\\"] None false))\\n  FunctionDecl(function_decl(\\\"g\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"f\\\" [] Int false)))\\n  ]) FunType([] Void) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"g\" true [] [\n"
      "    FunctionCall(\"f\" [] None false false)\n"
      "    Ret(None)\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"g\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestCodegen.test_init_routine (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"w\\\" Cast(UInt Constant(ConstInt(4660) Int) UInt) UInt None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"z\\\" None UChar None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Binary(Add Var(\\\"w\\\" UInt) Cast(UInt Cast(Int Var(\\\"z\\\" UChar) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"w\",38],[\"z\",40]],\"user_zp_bytes\":[38,39,40]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    ZeroExtend(Var(\"z\") Var(\"%1.1\"))\n"
      "    Binary(Add Var(\"w\") Var(\"%1.1\") Var(\"%3.1\"))\n"
      "    Ret(Var(\"%3.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"w\" true UInt [UIntInit(4660)])\n"
      "  StaticVariable(\"z\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestCodegen.test_pinned_pool_bytes_not_reused (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None Array(UChar 16) None None ZeroPage(128) None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\" \\\"@1.b\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@2.t\\\" Binary(Add Binary(Multiply Var(\\\"@0.a\\\" UInt) Cast(UInt Constant(ConstInt(3) Int) UInt) UInt) Var(\\\"@1.b\\\" UInt) UInt) UInt None None None None false)))\\n    S(Return(Binary(BitwiseXor Var(\\\"@2.t\\\" UInt) Binary(Add Var(\\\"@0.a\\\" UInt) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt) UInt)))\\n  ]) FunType([UInt UInt] UInt) None None [\\\"\\\" \\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"g\\\" Array(UChar 16)) Pointer(UChar)) Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(9) Int) UChar) UChar)))\\n    S(Return(Cast(Int Binary(Add FunctionCall(\\\"f\\\" [Cast(UInt Subscript(AddressOf(Var(\\\"g\\\" Array(UChar 16)) Pointer(UChar)) Constant(ConstInt(3) Int) UChar) UInt) Cast(UInt Constant(ConstInt(2) Int) UInt)] UInt false) Cast(UInt Cast(Int Subscript(AddressOf(Var(\\\"g\\\" Array(UChar 16)) Pointer(UChar)) Constant(ConstInt(3) Int) UChar) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"g\",128]],\"user_zp_bytes\":[128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\" \"@1.b\"] [\n"
      "    Binary(LeftShift Var(\"@0.a\") Constant(ConstInt(1)) Var(\"%1.1.sr0\"))\n"
      "    Binary(Add Var(\"%1.1.sr0\") Var(\"@0.a\") Var(\"%1.1\"))\n"
      "    Binary(Add Var(\"%1.1\") Var(\"@1.b\") Var(\"%2.1\"))\n"
      "    Binary(Add Var(\"@0.a\") Constant(ConstUInt(1)) Var(\"%4.1\"))\n"
      "    Binary(BitwiseXor Var(\"%2.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    IndexedSymbolStore(\"g\" Constant(ConstUChar(3)) Constant(ConstUChar(9)) false 0)\n"
      "    IndexedLoad(\"g\" Constant(ConstUChar(3)) Var(\"%7.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "    FunctionCall(\"f\" [Var(\"%8.1\") Constant(ConstUInt(2))] Var(\"%10.1\") false false)\n"
      "    IndexedLoad(\"g\" Constant(ConstUChar(3)) Var(\"%11.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%11.1\") Var(\"%13.1\"))\n"
      "    Binary(Add Var(\"%10.1\") Var(\"%13.1\") Var(\"%15.1\"))\n"
      "    Ret(Var(\"%15.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"g\" true Pointer [ZeroInit(16)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__a_0\", \"__zpabi_f__a_1\", \"__zpabi_f__b_0\", \"__zpabi_f__b_1\"], \"addrs\": [128, 129, 130, 131], \"param_registers\": [\"A\", \"X\", null, null], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestCodegen.test_pointer_is_indirect_base (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar)] Array(UChar 4) None) Array(UChar 4) None None None None false))\\n  VarDecl(var_decl(\\\"src\\\" AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Pointer(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(UChar) None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"p\\\" Pointer(UChar)) Var(\\\"src\\\" Pointer(UChar)) Pointer(UChar))))\\n    S(Return(Cast(Int Subscript(Var(\\\"p\\\" Pointer(UChar)) Constant(ConstInt(2) Int) UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"p\",38]],\"user_zp_bytes\":[38,39]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Var(\"src\") Var(\"p\"))\n"
      "    IndirectIndexedLoad(Var(\"p\") Constant(ConstUChar(2)) Var(\"%1.1\") false)\n"
      "    ZeroExtend(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "    Ret(Var(\"%2.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\" 4)])\n"
      "  StaticVariable(\"src\" true Pointer [AddressInit(\"buf\" 0)])\n"
      "  StaticVariable(\"p\" true Pointer [ZeroInit(2)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestLink.test_errors (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"g\\\" None UChar Extern None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [] Block([\\n    S(Return(Var(\\\"g\\\" UChar)))\\n  ]) FunType([] UChar) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(Var(\"g\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestLink.test_linked_program_runs (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"src\\\" None Pointer(UChar) Extern None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"total\\\" None UChar Extern None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"big\\\" None UInt Extern None ZeroPage(144) None false))\\n  FunctionDecl(function_decl(\\\"sum\\\" [\\\"@0.n\\\"] None FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"src\\\" None Pointer(UChar) None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"total\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"big\\\" Cast(UInt Constant(ConstInt(1000) Int) UInt) UInt None None ZeroPage(144) None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar)] Array(UChar 4) None) Array(UChar 4) None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"src\\\" Pointer(UChar)) AddressOf(Var(\\\"buf\\\" Array(UChar 4)) Pointer(UChar)) Pointer(UChar))))\\n    S(Return(Cast(Int Binary(Add Cast(UInt Cast(Int FunctionCall(\\\"sum\\\" [Cast(UChar Constant(ConstInt(4) Int) UChar)] UChar false) Int) UInt) Var(\\\"big\\\" UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"big\",144],[\"src\",38],[\"total\",40]],\"user_zp_bytes\":[38,39,40,144,145]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    GetAddress(Var(\"buf\") Var(\"src\"))\n"
      "    FunctionCall(\"sum\" [Constant(ConstUChar(4))] Var(\"%2.1\") false false)\n"
      "    ZeroExtend(Var(\"%2.1\") Var(\"%4.1\"))\n"
      "    Binary(Add Var(\"%4.1\") Var(\"big\") Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"src\" true Pointer [ZeroInit(2)])\n"
      "  StaticVariable(\"total\" true UInt [UCharInit(5)])\n"
      "  StaticVariable(\"big\" true UInt [UIntInit(1000)])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\" 4)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"sum\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_sum__n\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestLink.test_objects_placed_for_the_whole_program (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"src\\\" None Pointer(UChar) Extern None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"total\\\" None UChar Extern None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"big\\\" None UInt Extern None ZeroPage(144) None false))\\n  FunctionDecl(function_decl(\\\"sum\\\" [\\\"@0.n\\\"] None FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n  VarDecl(var_decl(\\\"count\\\" Cast(UChar Constant(ConstInt(2) Int) UChar) UChar None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"sum\\\" [\\\"@1.n\\\"] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@2.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@2.i\\\" UChar) Int) Cast(Int Var(\\\"@1.n\\\" UChar) Int) Int) Postfix(Increment Var(\\\"@2.i\\\" UChar) UChar) Expression(CompoundAssignment(Add Var(\\\"total\\\" UChar) Binary(Multiply Cast(Int Subscript(Var(\\\"src\\\" Pointer(UChar)) Cast(Int Var(\\\"@2.i\\\" UChar) Int) UChar) Int) Cast(Int Var(\\\"count\\\" UChar) Int) Int) Int UChar)) \\\".loop@0\\\" None))\\n    S(Expression(CompoundAssignment(Add Var(\\\"big\\\" UInt) Cast(UInt Var(\\\"total\\\" UChar) UInt) UInt UInt)))\\n    S(Return(Var(\\\"total\\\" UChar)))\\n  ]) FunType([UChar] UChar) None None [\\\"\\\"] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"big\",144],[\"count\",38]],\"user_zp_bytes\":[38,144,145]}",
      false,
      "Program([\n"
      "  Function(\"sum\" true [\"@1.n\"] [\n"
      "    Label(\".sum@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@2.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@2.i.2\") Var(\"@1.n\") \".loop@0_break\")\n"
      "    IndirectIndexedLoad(Var(\"src\") Var(\"@2.i.2\") Var(\"%6.1\") false)\n"
      "    Binary(Multiply Var(\"%6.1\") Var(\"count\") Var(\"%11.1.lo2\"))\n"
      "    Binary(Add Var(\"total\") Var(\"%11.1.lo2\") Var(\"total\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@2.i.2\") Constant(ConstUChar(1)) Var(\"@2.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    ZeroExtend(Var(\"total\") Var(\"%15.1\"))\n"
      "    Binary(Add Var(\"big\") Var(\"%15.1\") Var(\"big\"))\n"
      "    Ret(Var(\"total\"))\n"
      "  ])\n"
      "  StaticVariable(\"count\" true UInt [UCharInit(2)])\n"
      "])\n",
      "{\"dict\": [[\"sum\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_sum__n\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestLink.test_promoted_object_that_doesnt_fit_goes_back (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"fill\\\" None Array(UChar 86) None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"count\\\" [] None FunType([] UInt) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Subscript(AddressOf(Var(\\\"fill\\\" Array(UChar 86)) Pointer(UChar)) Constant(ConstInt(85) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) UChar)))\\n    S(Return(Cast(Int Binary(Add FunctionCall(\\\"count\\\" [] UInt false) Cast(UInt Cast(Int Subscript(AddressOf(Var(\\\"fill\\\" Array(UChar 86)) Pointer(UChar)) Constant(ConstInt(85) Int) UChar) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"fill\",38]],\"user_zp_bytes\":[38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    IndexedSymbolStore(\"fill\" Constant(ConstUChar(85)) Constant(ConstUChar(3)) false 0)\n"
      "    FunctionCall(\"count\" [] Var(\"%1.1\") false false)\n"
      "    IndexedLoad(\"fill\" Constant(ConstUChar(85)) Var(\"%2.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%2.1\") Var(\"%4.1\"))\n"
      "    Binary(Add Var(\"%1.1\") Var(\"%4.1\") Var(\"%6.1\"))\n"
      "    Ret(Var(\"%6.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"fill\" true Pointer [ZeroInit(86)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"count\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestLink.test_promoted_object_that_doesnt_fit_runs (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" Cast(UInt Constant(ConstInt(1) Int) UInt) UInt None None None None false))\\n  VarDecl(var_decl(\\\"b\\\" Cast(UInt Constant(ConstInt(2) Int) UInt) UInt None None None None false))\\n  VarDecl(var_decl(\\\"c\\\" Cast(UInt Constant(ConstInt(3) Int) UInt) UInt None None None None false))\\n  FunctionDecl(function_decl(\\\"count\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@0.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@0.i\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Postfix(Increment Var(\\\"@0.i\\\" UChar) UChar) Compound(Block([\\n      S(Expression(CompoundAssignment(Add Var(\\\"a\\\" UInt) Cast(UInt Constant(ConstInt(1) Int) UInt) UInt UInt)))\\n      S(Expression(CompoundAssignment(Add Var(\\\"b\\\" UInt) Var(\\\"a\\\" UInt) UInt UInt)))\\n      S(Expression(CompoundAssignment(Add Var(\\\"c\\\" UInt) Var(\\\"b\\\" UInt) UInt UInt)))\\n    ])) \\\".loop@0\\\" None))\\n    S(Return(Binary(Add Binary(Add Var(\\\"a\\\" UInt) Var(\\\"b\\\" UInt) UInt) Var(\\\"c\\\" UInt) UInt)))\\n  ]) FunType([] UInt) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"a\",38],[\"b\",40],[\"c\",42]],\"user_zp_bytes\":[38,39,40,41,42,43]}",
      false,
      "Program([\n"
      "  Function(\"count\" true [] [\n"
      "    Label(\".count@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(10)) Var(\"@0.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(Equal Var(\"@0.i.2\") Constant(ConstUChar(0)) \".loop@0_break\")\n"
      "    Binary(Add Var(\"a\") Constant(ConstUInt(1)) Var(\"a\"))\n"
      "    Binary(Add Var(\"b\") Var(\"a\") Var(\"b\"))\n"
      "    Binary(Add Var(\"c\") Var(\"b\") Var(\"c\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Subtract Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"@0.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    Binary(Add Var(\"a\") Var(\"b\") Var(\"%9.1\"))\n"
      "    Binary(Add Var(\"%9.1\") Var(\"c\") Var(\"%10.1\"))\n"
      "    Ret(Var(\"%10.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [UIntInit(1)])\n"
      "  StaticVariable(\"b\" true UInt [UIntInit(2)])\n"
      "  StaticVariable(\"c\" true UInt [UIntInit(3)])\n"
      "])\n",
      "{\"dict\": [[\"count\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestLink.test_promoted_pointer_has_to_fit (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"fill\\\" None Array(UChar 88) None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"sum\\\" [] None FunType([] UChar) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int FunctionCall(\\\"sum\\\" [] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"fill\",38]],\"user_zp_bytes\":[38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"sum\" [] Var(\"%0.1\") false false)\n"
      "    ZeroExtend(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "    Ret(Var(\"%1.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"fill\" true Pointer [ZeroInit(88)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"sum\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestPlacement.test_automatic_addresses_follow_fixed_ones (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"b\\\" None UInt None None ZeroPage(39) None false))\\n  VarDecl(var_decl(\\\"c\\\" None Array(UChar 3) None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Binary(Add Binary(Add Cast(UInt Cast(Int Var(\\\"a\\\" UChar) Int) UInt) Var(\\\"b\\\" UInt) UInt) Cast(UInt Cast(Int Subscript(AddressOf(Var(\\\"c\\\" Array(UChar 3)) Pointer(UChar)) Constant(ConstInt(1) Int) UChar) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"a\",38],[\"b\",39],[\"c\",41]],\"user_zp_bytes\":[38,39,40,41,42,43]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    ZeroExtend(Var(\"a\") Var(\"%1.1\"))\n"
      "    Binary(Add Var(\"%1.1\") Var(\"b\") Var(\"%2.1\"))\n"
      "    IndexedLoad(\"c\" Constant(ConstUChar(1)) Var(\"%3.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%3.1\") Var(\"%5.1\"))\n"
      "    Binary(Add Var(\"%2.1\") Var(\"%5.1\") Var(\"%7.1\"))\n"
      "    Ret(Var(\"%7.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"b\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"c\" true Pointer [ZeroInit(3)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestPlacement.test_extern_with_address_is_reserved_not_initialized (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"k\\\" None UChar Extern None ZeroPage(144) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Var(\\\"k\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"k\",144]],\"user_zp_bytes\":[144]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    ZeroExtend(Var(\"k\") Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestPlacement.test_extern_without_address_is_left_to_the_linker (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar Extern None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Var(\\\"a\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    ZeroExtend(Var(\"a\") Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestPlacement.test_redeclaration_accumulates (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"a\\\" None UChar Extern None ZeroPage(80) None false))\\n  VarDecl(var_decl(\\\"a\\\" Cast(UChar Constant(ConstInt(4) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Return(Cast(Int Var(\\\"a\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"a\",80]],\"user_zp_bytes\":[80]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    ZeroExtend(Var(\"a\") Var(\"%0.1\"))\n"
      "    Ret(Var(\"%0.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"a\" true UInt [UCharInit(4)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestPromotion.test_busy_scalars_promoted (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Pointer(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"total\\\" None UInt None None None None false))\\n  VarDecl(var_decl(\\\"step\\\" Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Const(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"once\\\" Cast(UChar Constant(ConstInt(9) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@0.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@0.i\\\" UChar) Int) Constant(ConstInt(8) Int) Int) CompoundAssignment(Add Var(\\\"@0.i\\\" UChar) Cast(Int Var(\\\"step\\\" Const(UChar)) Int) Int UChar) Expression(CompoundAssignment(Add Var(\\\"total\\\" UInt) Cast(UInt Binary(Add Cast(Int Subscript(Var(\\\"p\\\" Pointer(UChar)) Cast(Int Var(\\\"@0.i\\\" UChar) Int) UChar) Int) Cast(Int Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@0.i\\\" UChar) Int) UChar) Int) Int) UInt) UInt UInt)) \\\".loop@0\\\" None))\\n    S(Return(Cast(Int Binary(Add Var(\\\"total\\\" UInt) Cast(UInt Cast(Int Var(\\\"once\\\" UChar) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"p\",40],[\"total\",38]],\"user_zp_bytes\":[38,39,40,41]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@0.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(8)) \".loop@0_break\")\n"
      "    IndirectIndexedLoad(Var(\"p\") Var(\"@0.i.2\") Var(\"%5.1\") false)\n"
      "    ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "    IndexedLoad(\"buf\" Var(\"@0.i.2\") Var(\"%9.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%9.1\") Var(\"%10.1\"))\n"
      "    Binary(Add Var(\"%6.1\") Var(\"%10.1\") Var(\"%12.1\"))\n"
      "    Binary(Add Var(\"total\") Var(\"%12.1\") Var(\"total\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"@0.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    ZeroExtend(Var(\"once\") Var(\"%19.1\"))\n"
      "    Binary(Add Var(\"total\") Var(\"%19.1\") Var(\"%21.1\"))\n"
      "    Ret(Var(\"%21.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"p\" true Pointer [AddressInit(\"buf\" 0)])\n"
      "  StaticVariable(\"total\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"step\" true UInt [UCharInit(1)])\n"
      "  StaticVariable(\"once\" true UInt [UCharInit(9)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestPromotion.test_declared_objects_go_first (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"fill\\\" None Array(UChar 89) None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar) Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar)] Array(UChar 8) None) Array(UChar 8) None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Pointer(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"total\\\" None UInt None None None None false))\\n  VarDecl(var_decl(\\\"step\\\" Cast(Const(UChar) Constant(ConstInt(1) Int) Const(UChar)) Const(UChar) None None None None false))\\n  VarDecl(var_decl(\\\"once\\\" Cast(UChar Constant(ConstInt(9) Int) UChar) UChar None None None None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(ForStmt(InitDecl([var_decl(\\\"@0.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@0.i\\\" UChar) Int) Constant(ConstInt(8) Int) Int) CompoundAssignment(Add Var(\\\"@0.i\\\" UChar) Cast(Int Var(\\\"step\\\" Const(UChar)) Int) Int UChar) Expression(CompoundAssignment(Add Var(\\\"total\\\" UInt) Cast(UInt Binary(Add Cast(Int Subscript(Var(\\\"p\\\" Pointer(UChar)) Cast(Int Var(\\\"@0.i\\\" UChar) Int) UChar) Int) Cast(Int Subscript(AddressOf(Var(\\\"buf\\\" Array(UChar 8)) Pointer(UChar)) Cast(Int Var(\\\"@0.i\\\" UChar) Int) UChar) Int) Int) UInt) UInt UInt)) \\\".loop@0\\\" None))\\n    S(Return(Cast(Int Binary(Add Var(\\\"total\\\" UInt) Cast(UInt Cast(Int Var(\\\"once\\\" UChar) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"fill\",38]],\"user_zp_bytes\":[38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@0.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@0.i.2\") Constant(ConstUChar(8)) \".loop@0_break\")\n"
      "    IndirectIndexedLoad(Var(\"p\") Var(\"@0.i.2\") Var(\"%5.1\") false)\n"
      "    ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "    IndexedLoad(\"buf\" Var(\"@0.i.2\") Var(\"%9.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%9.1\") Var(\"%10.1\"))\n"
      "    Binary(Add Var(\"%6.1\") Var(\"%10.1\") Var(\"%12.1\"))\n"
      "    Binary(Add Var(\"total\") Var(\"%12.1\") Var(\"total\"))\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@0.i.2\") Constant(ConstUChar(1)) Var(\"@0.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    ZeroExtend(Var(\"once\") Var(\"%19.1\"))\n"
      "    Binary(Add Var(\"total\") Var(\"%19.1\") Var(\"%21.1\"))\n"
      "    Ret(Var(\"%21.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"fill\" true Pointer [ZeroInit(89)])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\\x07\\x08\" 8)])\n"
      "  StaticVariable(\"p\" true Pointer [AddressInit(\"buf\" 0)])\n"
      "  StaticVariable(\"total\" true UInt [ZeroInit(2)])\n"
      "  StaticVariable(\"step\" true UInt [UCharInit(1)])\n"
      "  StaticVariable(\"once\" true UInt [UCharInit(9)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestSim.test_block_static_counter (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  FunctionDecl(function_decl(\\\"tick\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.c\\\" Cast(UChar Constant(ConstInt(10) Int) UChar) UChar Static None ZeroPage(None) None false)))\\n    S(Return(Prefix(Increment Var(\\\"@0.c\\\" UChar) UChar)))\\n  ]) FunType([] UChar) None None [] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(FunctionCall(\\\"tick\\\" [] UChar false)))\\n    S(Expression(FunctionCall(\\\"tick\\\" [] UChar false)))\\n    S(Return(Cast(Int FunctionCall(\\\"tick\\\" [] UChar false) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"@0.c\",38]],\"user_zp_bytes\":[38]}",
      false,
      "Program([\n"
      "  Function(\"tick\" true [] [\n"
      "    Binary(Add Var(\"@0.c\") Constant(ConstUChar(1)) Var(\"@0.c\"))\n"
      "    Ret(Var(\"@0.c\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"tick\" [] None false false)\n"
      "    FunctionCall(\"tick\" [] None false false)\n"
      "    FunctionCall(\"tick\" [] Var(\"%3.1\") false false)\n"
      "    ZeroExtend(Var(\"%3.1\") Var(\"%4.1\"))\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"@0.c\" false UInt [UCharInit(10)])\n"
      "])\n",
      "{\"dict\": [[\"tick\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestSim.test_global_in_pool_range_survives_calls (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"keep\\\" Cast(UInt Constant(ConstInt(0) Int) UInt) UInt None None ZeroPage(128) None false))\\n  FunctionDecl(function_decl(\\\"f\\\" [\\\"@0.a\\\"] Block([\\n    D(VarDecl(var_decl(\\\"@1.x\\\" Binary(Multiply Var(\\\"@0.a\\\" UInt) Cast(UInt Constant(ConstInt(5) Int) UInt) UInt) UInt None None None None false)))\\n    D(VarDecl(var_decl(\\\"@2.y\\\" Binary(Add Var(\\\"@0.a\\\" UInt) Cast(UInt Constant(ConstInt(7) Int) UInt) UInt) UInt None None None None false)))\\n    S(Return(Binary(BitwiseXor Var(\\\"@1.x\\\" UInt) Var(\\\"@2.y\\\" UInt) UInt)))\\n  ]) FunType([UInt] UInt) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"keep\\\" UInt) Cast(UInt Constant(ConstInt(1234) Int) UInt) UInt)))\\n    D(VarDecl(var_decl(\\\"@3.r\\\" Binary(Add FunctionCall(\\\"f\\\" [Cast(UInt Constant(ConstInt(3) Int) UInt)] UInt false) FunctionCall(\\\"f\\\" [Cast(UInt Constant(ConstInt(4) Int) UInt)] UInt false) UInt) UInt None None None None false)))\\n    S(Return(Cast(Int Binary(Add Binary(Subtract Var(\\\"keep\\\" UInt) Cast(UInt Constant(ConstInt(1234) Int) UInt) UInt) Cast(UInt Binary(Equal Var(\\\"@3.r\\\" UInt) Binary(Add Binary(BitwiseXor Constant(ConstUInt(15) UInt) Constant(ConstUInt(10) UInt) UInt) Binary(BitwiseXor Constant(ConstUInt(20) UInt) Constant(ConstUInt(11) UInt) UInt) UInt) Int) UInt) UInt) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"keep\",128]],\"user_zp_bytes\":[128,129]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [\"@0.a\"] [\n"
      "    Binary(LeftShift Var(\"@0.a\") Constant(ConstInt(2)) Var(\"%1.1.sr0\"))\n"
      "    Binary(Add Var(\"%1.1.sr0\") Var(\"@0.a\") Var(\"%1.1\"))\n"
      "    Binary(Add Var(\"@0.a\") Constant(ConstUInt(7)) Var(\"%3.1\"))\n"
      "    Binary(BitwiseXor Var(\"%1.1\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "    Ret(Var(\"%4.1\"))\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Constant(ConstUInt(1234)) Var(\"keep\"))\n"
      "    FunctionCall(\"f\" [Constant(ConstUInt(3))] Var(\"%7.1\") false false)\n"
      "    FunctionCall(\"f\" [Constant(ConstUInt(4))] Var(\"%9.1\") false false)\n"
      "    Binary(Add Var(\"%7.1\") Var(\"%9.1\") Var(\"%10.1\"))\n"
      "    Binary(Subtract Var(\"keep\") Constant(ConstUInt(1234)) Var(\"%12.1\"))\n"
      "    Binary(Equal Var(\"%10.1\") Constant(ConstUInt(36)) Var(\"%17.1\"))\n"
      "    Binary(Add Var(\"%12.1\") Var(\"%17.1\") Var(\"%19.1\"))\n"
      "    Ret(Var(\"%19.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"keep\" true UInt [ZeroInit(2)])\n"
      "])\n",
      "{\"dict\": [[\"f\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_f__a_0\", \"__zpabi_f__a_1\"], \"addrs\": [128, 129], \"param_registers\": [\"A\", \"X\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestSim.test_initial_values (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  StructDecl(struct_decl(\\\"s\\\" false [member_decl(\\\"a\\\" UChar) member_decl(\\\"b\\\" UInt)]))\\n  VarDecl(var_decl(\\\"tab\\\" InitList([Cast(UChar Constant(ConstInt(7) Int) UChar) Cast(UChar Constant(ConstInt(8) Int) UChar) Cast(UChar Constant(ConstInt(9) Int) UChar)] Array(UChar 3) None) Array(UChar 3) None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"st\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UInt Constant(ConstInt(300) Int) UInt)] Structure(\\\"s\\\") None) Structure(\\\"s\\\") None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"l\\\" Cast(ULong Constant(ConstLong(70000) Long) ULong) ULong None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"z\\\" None UChar None None ZeroPage(None) None false))\\n  VarDecl(var_decl(\\\"other\\\" Cast(UChar Constant(ConstInt(5) Int) UChar) UChar None None None None false))\\n  VarDecl(var_decl(\\\"ptr\\\" AddressOf(Var(\\\"other\\\" UChar) Pointer(UChar)) Pointer(UChar) None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(IfStmt(Binary(NotEqual Binary(Add Binary(Add Cast(Int Subscript(AddressOf(Var(\\\"tab\\\" Array(UChar 3)) Pointer(UChar)) Constant(ConstInt(0) Int) UChar) Int) Cast(Int Subscript(AddressOf(Var(\\\"tab\\\" Array(UChar 3)) Pointer(UChar)) Constant(ConstInt(1) Int) UChar) Int) Int) Cast(Int Subscript(AddressOf(Var(\\\"tab\\\" Array(UChar 3)) Pointer(UChar)) Constant(ConstInt(2) Int) UChar) Int) Int) Constant(ConstInt(24) Int) Int) Return(Constant(ConstInt(1) Int)) None))\\n    S(IfStmt(Binary(LogicalOr Binary(NotEqual Cast(Int Dot(Var(\\\"st\\\" Structure(\\\"s\\\")) \\\"a\\\" UChar) Int) Constant(ConstInt(1) Int) Int) Binary(NotEqual Dot(Var(\\\"st\\\" Structure(\\\"s\\\")) \\\"b\\\" UInt) Cast(UInt Constant(ConstInt(300) Int) UInt) Int) Int) Return(Constant(ConstInt(2) Int)) None))\\n    S(IfStmt(Binary(NotEqual Var(\\\"l\\\" ULong) Cast(ULong Constant(ConstLong(70000) Long) ULong) Int) Return(Constant(ConstInt(3) Int)) None))\\n    S(IfStmt(Binary(NotE"
      "qual Cast(Int Var(\\\"z\\\" UChar) Int) Constant(ConstInt(0) Int) Int) Return(Constant(ConstInt(4) Int)) None))\\n    S(IfStmt(Binary(NotEqual Cast(Int Dereference(Var(\\\"ptr\\\" Pointer(UChar)) UChar) Int) Constant(ConstInt(5) Int) Int) Return(Constant(ConstInt(5) Int)) None))\\n    S(Return(Constant(ConstInt(0) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": [[\"s\", {\"object\": \"passes.type_checking.StructLayout\", \"fields\": {\"tag\": \"s\", \"is_union\": false, \"members\": [{\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"a\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"byte_offset\": 0}}, {\"object\": \"passes.type_checking.MemberInfo\", \"fields\": {\"name\": \"b\", \"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"byte_offset\": 1}}], \"size\": 3, \"complete\": true}}]]}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"l\",44],[\"ptr\",49],[\"st\",41],[\"tab\",38],[\"z\",48]],\"user_zp_bytes\":[38,39,40,41,42,43,44,45,46,47,48,49,50]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    IndexedLoad(\"tab\" Constant(ConstUChar(0)) Var(\"%0.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "    IndexedLoad(\"tab\" Constant(ConstUChar(1)) Var(\"%2.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "    Binary(Add Var(\"%1.1\") Var(\"%3.1\") Var(\"%4.1\"))\n"
      "    IndexedLoad(\"tab\" Constant(ConstUChar(2)) Var(\"%5.1\") false 0 0)\n"
      "    ZeroExtend(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "    Binary(Add Var(\"%4.1\") Var(\"%6.1\") Var(\"%7.1\"))\n"
      "    JumpIfCmp(Equal Var(\"%7.1\") Constant(ConstInt(24)) \".if_end@0\")\n"
      "    Ret(Constant(ConstInt(1)))\n"
      "    Label(\".if_end@0\")\n"
      "    IndexedLoad(\"st\" Constant(ConstUChar(0)) Var(\"%11.1\") false 0 0)\n"
      "    JumpIfCmp(NotEqual Var(\"%11.1\") Constant(ConstUChar(1)) \".main@jthread@0\")\n"
      "    IndexedLoad(\"st\" Constant(ConstUChar(1)) Var(\"%16.1\") false 0 0)\n"
      "    JumpIfCmp(NotEqual Var(\"%16.1\") Constant(ConstUInt(300)) \".main@jthread@0\")\n"
      "    Jump(\".if_end@3\")\n"
      "    Label(\".main@jthread@0\")\n"
      "    Ret(Constant(ConstInt(2)))\n"
      "    Label(\".if_end@3\")\n"
      "    JumpIfCmp(Equal Var(\"l\") Constant(ConstULong(70000)) \".if_end@4\")\n"
      "    Ret(Constant(ConstInt(3)))\n"
      "    Label(\".if_end@4\")\n"
      "    JumpIfFalse(Var(\"z\") \".if_end@5\")\n"
      "    Ret(Constant(ConstInt(4)))\n"
      "    Label(\".if_end@5\")\n"
      "    Load(Var(\"ptr\") Var(\"%23.1\") false)\n"
      "    JumpIfCmp(Equal Var(\"%23.1\") Constant(ConstUChar(5)) \".if_end@6\")\n"
      "    Ret(Constant(ConstInt(5)))\n"
      "    Label(\".if_end@6\")\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "  StaticVariable(\"tab\" true Pointer [StringInit(\"\\x07\\x08\\x09\" 3)])\n"
      "  StaticVariable(\"st\" true Pointer [UCharInit(1) UIntInit(300)])\n"
      "  StaticVariable(\"l\" true ULong [ULongInit(70000)])\n"
      "  StaticVariable(\"z\" true UInt [ZeroInit(1)])\n"
      "  StaticVariable(\"other\" true UInt [UCharInit(5)])\n"
      "  StaticVariable(\"ptr\" true Pointer [AddressInit(\"other\" 0)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestSim.test_pointer_global_walks_buffer (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"buf\\\" InitList([Cast(UChar Constant(ConstInt(1) Int) UChar) Cast(UChar Constant(ConstInt(2) Int) UChar) Cast(UChar Constant(ConstInt(3) Int) UChar) Cast(UChar Constant(ConstInt(4) Int) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) Cast(UChar Constant(ConstInt(6) Int) UChar)] Array(UChar 6) None) Array(UChar 6) None None None None false))\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(UChar) None None ZeroPage(186) None false))\\n  VarDecl(var_decl(\\\"sum\\\" None UChar None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"add\\\" [\\\"@0.k\\\"] Block([\\n    S(Expression(CompoundAssignment(Add Var(\\\"sum\\\" UChar) Cast(Int Subscript(Var(\\\"p\\\" Pointer(UChar)) Cast(Int Var(\\\"@0.k\\\" UChar) Int) UChar) Int) Int UChar)))\\n  ]) FunType([UChar] Void) None None [\\\"\\\"] false None [] None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    S(Expression(Assignment(Var(\\\"p\\\" Pointer(UChar)) AddressOf(Var(\\\"buf\\\" Array(UChar 6)) Pointer(UChar)) Pointer(UChar))))\\n    S(ForStmt(InitDecl([var_decl(\\\"@1.i\\\" Cast(UChar Constant(ConstInt(0) Int) UChar) UChar None None None None false)]) Binary(LessThan Cast(Int Var(\\\"@1.i\\\" UChar) Int) Constant(ConstInt(6) Int) Int) Postfix(Increment Var(\\\"@1.i\\\" UChar) UChar) Expression(FunctionCall(\\\"add\\\" [Var(\\\"@1.i\\\" UChar)] Void false)) \\\".loop@0\\\" None))\\n    S(Expression(Assignment(Var(\\\"p\\\" Pointer(UChar)) Binary(Add AddressOf(Var(\\\"buf\\\" Array(UChar 6)) Pointer(UChar)) Constant(ConstInt(3) Int) Pointer(UChar)) Pointer(UChar))))\\n    S(Expression(FunctionCall(\\\"add\\\" [Cast(UChar Constant(ConstInt(0) Int) UChar)] Void false)))\\n    S(Return(Cast(Int Var(\\\"sum\\\" UChar) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"p\",186],[\"sum\",38]],\"user_zp_bytes\":[38,186,187]}",
      false,
      "Program([\n"
      "  Function(\"add\" true [\"@0.k\"] [\n"
      "    IndirectIndexedLoad(Var(\"p\") Var(\"@0.k\") Var(\"%2.1\") false)\n"
      "    Binary(Add Var(\"sum\") Var(\"%2.1\") Var(\"sum\"))\n"
      "    Ret(None)\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Label(\".main@ssa_block@0\")\n"
      "    GetAddress(Var(\"buf\") Var(\"p\"))\n"
      "    Copy(Constant(ConstUChar(0)) Var(\"@1.i.2\"))\n"
      "    Label(\".loop@0_start\")\n"
      "    JumpIfCmp(GreaterOrEqual Var(\"@1.i.2\") Constant(ConstUChar(6)) \".loop@0_break\")\n"
      "    FunctionCall(\"add\" [Var(\"@1.i.2\")] None false false)\n"
      "    Label(\".loop@0_continue\")\n"
      "    Binary(Add Var(\"@1.i.2\") Constant(ConstUChar(1)) Var(\"@1.i.2\"))\n"
      "    Jump(\".loop@0_start\")\n"
      "    Label(\".loop@0_break\")\n"
      "    GetAddress(Var(\"buf\") Var(\"%13.1\"))\n"
      "    Binary(Add Var(\"%13.1\") Constant(ConstInt(3)) Var(\"p\"))\n"
      "    FunctionCall(\"add\" [Constant(ConstUChar(0))] None false false)\n"
      "    ZeroExtend(Var(\"sum\") Var(\"%16.1\"))\n"
      "    Ret(Var(\"%16.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"buf\" true Pointer [StringInit(\"\\x01\\x02\\x03\\x04\\x05\\x06\" 6)])\n"
      "  StaticVariable(\"p\" true Pointer [ZeroInit(2)])\n"
      "  StaticVariable(\"sum\" true UInt [ZeroInit(1)])\n"
      "])\n",
      "{\"dict\": [[\"add\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_add__k\"], \"addrs\": [128], \"param_registers\": [\"A\"], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}

TEST_CASE("select_abi: test_zp_globals.TestSim.test_write_through_pointer_to_zp_global (call 1)") {
  c6502::testing::check_stage_call(
      "select_abi",
      "{\"args\": [{\"asdl\": \"c99_ast\", \"dump\": \"Program([\\n  VarDecl(var_decl(\\\"n\\\" Cast(UChar Constant(ConstInt(1) Int) UChar) UChar None None ZeroPage(160) None false))\\n  VarDecl(var_decl(\\\"p\\\" None Pointer(UChar) None None ZeroPage(None) None false))\\n  FunctionDecl(function_decl(\\\"main\\\" [] Block([\\n    D(VarDecl(var_decl(\\\"@0.before\\\" Var(\\\"n\\\" UChar) UChar None None None None false)))\\n    S(Expression(Assignment(Var(\\\"p\\\" Pointer(UChar)) AddressOf(Var(\\\"n\\\" UChar) Pointer(UChar)) Pointer(UChar))))\\n    S(Expression(Assignment(Dereference(Var(\\\"p\\\" Pointer(UChar)) UChar) Cast(UChar Constant(ConstInt(5) Int) UChar) UChar)))\\n    S(Return(Binary(Add Binary(Multiply Cast(Int Var(\\\"@0.before\\\" UChar) Int) Constant(ConstInt(10) Int) Int) Cast(Int Var(\\\"n\\\" UChar) Int) Int)))\\n  ]) FunType([] Int) None None [] false None [] None false))\\n])\\n\"}, {\"object\": \"passes.type_checking.TypeTable\", \"fields\": {\"_table\": {\"dict\": []}}}], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[[\"n\",160],[\"p\",38]],\"user_zp_bytes\":[38,39,160]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Copy(Var(\"n\") Var(\"@0.before.1\"))\n"
      "    GetAddress(Var(\"n\") Var(\"p\"))\n"
      "    Store(Constant(ConstUChar(5)) Var(\"p\") false)\n"
      "    ZeroExtend(Var(\"@0.before.1\") Var(\"%2.1\"))\n"
      "    Binary(LeftShift Var(\"%2.1\") Constant(ConstInt(1)) Var(\"%3.1.sr0\"))\n"
      "    Binary(LeftShift Var(\"%3.1.sr0\") Constant(ConstInt(2)) Var(\"%3.1.sr1\"))\n"
      "    Binary(Add Var(\"%3.1.sr1\") Var(\"%3.1.sr0\") Var(\"%3.1\"))\n"
      "    ZeroExtend(Var(\"n\") Var(\"%4.1\"))\n"
      "    Binary(Add Var(\"%3.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "    Ret(Var(\"%5.1\"))\n"
      "  ])\n"
      "  StaticVariable(\"n\" true UInt [UCharInit(1)])\n"
      "  StaticVariable(\"p\" true Pointer [ZeroInit(2)])\n"
      "])\n",
      "{\"dict\": [[\"main\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [], \"addrs\": [], \"param_registers\": [], \"pinned\": []}}]]}\n",
      "");
}
