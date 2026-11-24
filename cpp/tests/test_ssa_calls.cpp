// The calls that the tests of tests/test_ssa.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_to_ssa: test_ssa.py::TestSSAConstruction::test_address_taken_var_is_not_promoted (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_ssa\n"
      "source tests/test_ssa.py::TestSSAConstruction::test_address_taken_var_is_not_promoted\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%p.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@a\"))\n"
      "  GetAddress(Var(\"@a\") Var(\"%p\"))\n"
      "  Store(Constant(ConstInt(2)) Var(\"%p\") false)\n"
      "  Ret(Var(\"@a\"))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"main\\\" true [] [\\n  Label(\\\".main@ssa_block@0\\\")\\n  Copy(Constant(ConstInt(1)) Var(\\\"@a\\\"))\\n  GetAddress(Var(\\\"@a\\\") Var(\\\"%p.1\\\"))\\n  Store(Constant(ConstInt(2)) Var(\\\"%p.1\\\") false)\\n  Ret(Var(\\\"@a\\\"))\\n])\\n\"}, {\"set\": [\"%p.1\"]}]}\n");
}

TEST_CASE("tac_from_ssa: test_ssa.py::TestSSADestruction::test_2_cycle_broken_with_temp (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_from_ssa\n"
      "source tests/test_ssa.py::TestSSADestruction::test_2_cycle_broken_with_temp\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\".main@cycle_tmp@1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"a\"))\n"
      "  Copy(Constant(ConstInt(11)) Var(\"b\"))\n"
      "  Jump(\".loop_continue\")\n"
      "  Label(\".loop_continue\")\n"
      "  Label(\".join\")\n"
      "  Phi(Var(\"a\") [PhiArg(\".pre\" Constant(ConstInt(0))) PhiArg(\".loop_continue\" Var(\"b\"))])\n"
      "  Phi(Var(\"b\") [PhiArg(\".pre\" Constant(ConstInt(0))) PhiArg(\".loop_continue\" Var(\"a\"))])\n"
      "  Ret(Var(\"a\"))\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  Copy(Constant(ConstInt(7)) Var(\"a\"))\n"
      "  Copy(Constant(ConstInt(11)) Var(\"b\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"a\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"b\"))\n"
      "  Jump(\".loop_continue\")\n"
      "  Label(\".loop_continue\")\n"
      "  Copy(Var(\"a\") Var(\".main@cycle_tmp@1\"))\n"
      "  Copy(Var(\"b\") Var(\"a\"))\n"
      "  Copy(Var(\".main@cycle_tmp@1\") Var(\"b\"))\n"
      "  Label(\".join\")\n"
      "  Ret(Var(\"a\"))\n"
      "])\n");
}

TEST_CASE("tac_to_ssa: test_ssa.py::TestSSARoundTrip::test_roundtrip_preserves_name_and_params (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_ssa\n"
      "source tests/test_ssa.py::TestSSARoundTrip::test_roundtrip_preserves_name_and_params\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "mutated {\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}\n"
      "input\n"
      "Function(\"foo\" true [\"p\"] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@x\"))\n"
      "  Ret(Var(\"@x\"))\n"
      "])\n"
      "output-json\n"
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"foo\\\" true [\\\"p\\\"] [\\n  Label(\\\".foo@ssa_block@0\\\")\\n  Copy(Constant(ConstInt(1)) Var(\\\"@x.1\\\"))\\n  Ret(Var(\\\"@x.1\\\"))\\n])\\n\"}, {\"set\": [\"@x.1\"]}]}\n");
}
