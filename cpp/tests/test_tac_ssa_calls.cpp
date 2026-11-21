// The calls that the unit tests of tests/test_ssa.py,
// test_loop_rotate.py, and test_noreturn_call.py make of `to_ssa`,
// `from_ssa`, `rotate_signed_countdown_loops`, and `noreturn_calls`,
// each with what the Python function returned and the symbols it added,
// which the C++ port must return and add too. They were recorded by
// wrapping the Python functions while those tests ran, with the symbol
// tables of `test_loop_rotate.py`, which are dicts of symbols whose
// attributes are a stand-in class, given as `SymbolTable`s of
// `LocalAttr`s, since the pass reads only the types. A call that raised isn't kept, nor a second call of
// the same function on the same input. The captures of the corpus hold
// the other calls. The cycle that `from_ssa` breaks with a temporary
// (test_2_cycle_broken_with_temp) comes up in none of the corpus's
// calls.
#include "doctest.h"
#include "stage_case.h"

TEST_CASE("tac_to_ssa: test_ssa.TestSSAConstruction.test_address_taken_var_is_not_promoted (call 1)") {
  c6502::testing::check_stage_case(
      "tac_to_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@a\"))\n"
      "  GetAddress(Var(\"@a\") Var(\"%p\"))\n"
      "  Store(Constant(ConstInt(2)) Var(\"%p\") false)\n"
      "  Ret(Var(\"@a\"))\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"main\\\" true [] [\\n  Label(\\\".main@ssa_block@0\\\")\\n  Copy(Constant(ConstInt(1)) Var(\\\"@a\\\"))\\n  GetAddress(Var(\\\"@a\\\") Var(\\\"%p.1\\\"))\\n  Store(Constant(ConstInt(2)) Var(\\\"%p.1\\\") false)\\n  Ret(Var(\\\"@a\\\"))\\n])\\n\"}, {\"set\": [\"%p.1\"]}]}",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%p.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_to_ssa: test_ssa.TestSSAConstruction.test_if_else_join_gets_phi (call 1)") {
  c6502::testing::check_stage_case(
      "tac_to_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"@c\") \".else\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@x\"))\n"
      "  Jump(\".join\")\n"
      "  Label(\".else\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"@x\"))\n"
      "  Label(\".join\")\n"
      "  Ret(Var(\"@x\"))\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"main\\\" true [] [\\n  Label(\\\".main@ssa_block@0\\\")\\n  JumpIfFalse(Var(\\\"@c\\\") \\\".else\\\")\\n  Label(\\\".main@ssa_block@1\\\")\\n  Copy(Constant(ConstInt(1)) Var(\\\"@x.1\\\"))\\n  Jump(\\\".join\\\")\\n  Label(\\\".else\\\")\\n  Copy(Constant(ConstInt(2)) Var(\\\"@x.2\\\"))\\n  Label(\\\".join\\\")\\n  Phi(Var(\\\"@x.3\\\") [PhiArg(\\\".main@ssa_block@1\\\" Var(\\\"@x.1\\\")) PhiArg(\\\".else\\\" Var(\\\"@x.2\\\"))])\\n  Ret(Var(\\\"@x.3\\\"))\\n])\\n\"}, {\"set\": [\"@x.1\", \"@x.2\", \"@x.3\"]}]}",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_to_ssa: test_ssa.TestSSAConstruction.test_no_phi_for_dead_temp_at_join (call 1)") {
  c6502::testing::check_stage_case(
      "tac_to_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  JumpIfFalse(Var(\"@c\") \".else\")\n"
      "  Binary(Add Var(\"@x\") Constant(ConstInt(1)) Var(\"%t\"))\n"
      "  Copy(Var(\"%t\") Var(\"@y\"))\n"
      "  Jump(\".join\")\n"
      "  Label(\".else\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@y\"))\n"
      "  Label(\".join\")\n"
      "  Ret(Var(\"@y\"))\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"main\\\" true [] [\\n  Label(\\\".main@ssa_block@0\\\")\\n  JumpIfFalse(Var(\\\"@c\\\") \\\".else\\\")\\n  Label(\\\".main@ssa_block@1\\\")\\n  Binary(Add Var(\\\"@x\\\") Constant(ConstInt(1)) Var(\\\"%t.1\\\"))\\n  Copy(Var(\\\"%t.1\\\") Var(\\\"@y.1\\\"))\\n  Jump(\\\".join\\\")\\n  Label(\\\".else\\\")\\n  Copy(Constant(ConstInt(0)) Var(\\\"@y.2\\\"))\\n  Label(\\\".join\\\")\\n  Phi(Var(\\\"@y.3\\\") [PhiArg(\\\".main@ssa_block@1\\\" Var(\\\"@y.1\\\")) PhiArg(\\\".else\\\" Var(\\\"@y.2\\\"))])\\n  Ret(Var(\\\"@y.3\\\"))\\n])\\n\"}, {\"set\": [\"%t.1\", \"@y.1\", \"@y.2\", \"@y.3\"]}]}",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@c\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y.2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y.3\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_to_ssa: test_ssa.TestSSAConstruction.test_param_retains_original_spelling_as_initial_value (call 1)") {
  c6502::testing::check_stage_case(
      "tac_to_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [\"@p\"] [\n"
      "  Copy(Var(\"@p\") Var(\"%t\"))\n"
      "  Copy(Constant(ConstInt(5)) Var(\"@p\"))\n"
      "  Ret(Var(\"@p\"))\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"main\\\" true [\\\"@p\\\"] [\\n  Label(\\\".main@ssa_block@0\\\")\\n  Copy(Var(\\\"@p\\\") Var(\\\"%t.1\\\"))\\n  Copy(Constant(ConstInt(5)) Var(\\\"@p.1\\\"))\\n  Ret(Var(\\\"@p.1\\\"))\\n])\\n\"}, {\"set\": [\"%t.1\", \"@p\", \"@p.1\"]}]}",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%t.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@p.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_to_ssa: test_ssa.TestSSAConstruction.test_straight_line_no_phis (call 1)") {
  c6502::testing::check_stage_case(
      "tac_to_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@x\"))\n"
      "  Binary(Add Var(\"@x\") Constant(ConstInt(2)) Var(\"@y\"))\n"
      "  Ret(Var(\"@y\"))\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"main\\\" true [] [\\n  Label(\\\".main@ssa_block@0\\\")\\n  Copy(Constant(ConstInt(1)) Var(\\\"@x.1\\\"))\\n  Binary(Add Var(\\\"@x.1\\\") Constant(ConstInt(2)) Var(\\\"@y.1\\\"))\\n  Ret(Var(\\\"@y.1\\\"))\\n])\\n\"}, {\"set\": [\"@x.1\", \"@y.1\"]}]}",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@y.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_from_ssa: test_ssa.TestSSADestruction.test_2_cycle_broken_with_temp (call 1)") {
  c6502::testing::check_stage_case(
      "tac_from_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
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
      "])\n",
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
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\".main@cycle_tmp@1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_from_ssa: test_ssa.TestSSADestruction.test_chain_topologically_sorted (call 1)") {
  c6502::testing::check_stage_case(
      "tac_from_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  Jump(\".loop_continue\")\n"
      "  Label(\".loop_continue\")\n"
      "  Label(\".join\")\n"
      "  Phi(Var(\"@i\") [PhiArg(\".pre\" Constant(ConstInt(0))) PhiArg(\".loop_continue\" Var(\"%4\"))])\n"
      "  Phi(Var(\"@counter\") [PhiArg(\".pre\" Constant(ConstInt(0))) PhiArg(\".loop_continue\" Var(\"@i\"))])\n"
      "  Ret(Var(\"@counter\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@i\"))\n"
      "  Copy(Constant(ConstInt(0)) Var(\"@counter\"))\n"
      "  Jump(\".loop_continue\")\n"
      "  Label(\".loop_continue\")\n"
      "  Copy(Var(\"@i\") Var(\"@counter\"))\n"
      "  Copy(Var(\"%4\") Var(\"@i\"))\n"
      "  Label(\".join\")\n"
      "  Ret(Var(\"@counter\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_from_ssa: test_ssa.TestSSADestruction.test_no_phis_passes_through (call 1)") {
  c6502::testing::check_stage_case(
      "tac_from_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Var(\"x\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Ret(Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_from_ssa: test_ssa.TestSSADestruction.test_phi_lowered_to_copies_in_predecessors (call 1)") {
  c6502::testing::check_stage_case(
      "tac_from_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  JumpIfFalse(Var(\"@c\") \".else\")\n"
      "  Label(\".then\")\n"
      "  Jump(\".join\")\n"
      "  Label(\".else\")\n"
      "  Label(\".join\")\n"
      "  Phi(Var(\"@x.3\") [PhiArg(\".then\" Constant(ConstInt(1))) PhiArg(\".else\" Constant(ConstInt(2)))])\n"
      "  Ret(Var(\"@x.3\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".pre\")\n"
      "  JumpIfFalse(Var(\"@c\") \".else\")\n"
      "  Label(\".then\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@x.3\"))\n"
      "  Jump(\".join\")\n"
      "  Label(\".else\")\n"
      "  Copy(Constant(ConstInt(2)) Var(\"@x.3\"))\n"
      "  Label(\".join\")\n"
      "  Ret(Var(\"@x.3\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_to_ssa: test_ssa.TestSSARoundTrip.test_roundtrip_preserves_name_and_params (call 1)") {
  c6502::testing::check_stage_case(
      "tac_to_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"foo\" true [\"p\"] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@x\"))\n"
      "  Ret(Var(\"@x\"))\n"
      "])\n",
      "{\"tuple\": [{\"asdl\": \"tac_ast\", \"dump\": \"Function(\\\"foo\\\" true [\\\"p\\\"] [\\n  Label(\\\".foo@ssa_block@0\\\")\\n  Copy(Constant(ConstInt(1)) Var(\\\"@x.1\\\"))\\n  Ret(Var(\\\"@x.1\\\"))\\n])\\n\"}, {\"set\": [\"@x.1\"]}]}",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"@x.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_from_ssa: test_ssa.TestSSARoundTrip.test_roundtrip_preserves_name_and_params (call 1)") {
  c6502::testing::check_stage_case(
      "tac_from_ssa",
      "{\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}",
      "Function(\"foo\" true [\"p\"] [\n"
      "  Label(\".foo@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@x.1\"))\n"
      "  Ret(Var(\"@x.1\"))\n"
      "])\n",
      "Function(\"foo\" true [\"p\"] [\n"
      "  Label(\".foo@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(1)) Var(\"@x.1\"))\n"
      "  Ret(Var(\"@x.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_char_counter_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Char\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_compare_against_nonzero_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(1)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_init_zero_is_eligible (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(0)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfTrue(Var(\"%0\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_int_countdown_rotates (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfTrue(Var(\"%0\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_negative_init_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(-1)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_non_ge_condition_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterThan Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterThan Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_post_dec_by_two_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(2)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_post_increment_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_test_that_writes_a_variable_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"n\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(Add Var(\"n\") Constant(ConstInt(1)) Var(\"n\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(Add Var(\"n\") Constant(ConstInt(1)) Var(\"n\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMatch.test_unsigned_counter_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"sum\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Binary(Add Var(\"sum\") Var(\"x\") Var(\"sum\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMisc.test_idempotent (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfFalse(Var(\"%0\") \"L_break\")\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfTrue(Var(\"%0\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMisc.test_idempotent (call 2)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfTrue(Var(\"%0\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%0\"))\n"
      "  JumpIfTrue(Var(\"%0\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMisc.test_no_loop_passthrough (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Ret(Var(\"x\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(5)) Var(\"x\"))\n"
      "  Ret(Var(\"x\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMisc.test_signextend_of_iv_in_body_rewrites_to_zeroextend (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"scratch\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfFalse(Var(\"%cond\") \"L_break\")\n"
      "  SignExtend(Var(\"x\") Var(\"%ext\"))\n"
      "  Copy(Var(\"%ext\") Var(\"scratch\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  ZeroExtend(Var(\"x\") Var(\"%ext\"))\n"
      "  Copy(Var(\"%ext\") Var(\"scratch\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfTrue(Var(\"%cond\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMisc.test_signextend_of_other_var_in_body_unchanged (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfFalse(Var(\"%cond\") \"L_break\")\n"
      "  SignExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  SignExtend(Var(\"y\") Var(\"%ext\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfTrue(Var(\"%cond\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateMisc.test_skips_se_to_ze_when_body_modifies_iv (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfFalse(Var(\"%cond\") \"L_break\")\n"
      "  Copy(Constant(ConstInt(-5)) Var(\"x\"))\n"
      "  SignExtend(Var(\"x\") Var(\"%ext\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstInt(15)) Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Copy(Constant(ConstInt(-5)) Var(\"x\"))\n"
      "  SignExtend(Var(\"x\") Var(\"%ext\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstInt(1)) Var(\"%new\"))\n"
      "  Copy(Var(\"%new\") Var(\"x\"))\n"
      "  Binary(GreaterOrEqual Var(\"x\") Constant(ConstInt(0)) Var(\"%cond\"))\n"
      "  JumpIfTrue(Var(\"%cond\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateSChar.test_schar_init_overflow_skips (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Truncate(Constant(ConstInt(200)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  SignExtend(Var(\"x\") Var(\"%2\"))\n"
      "  Binary(GreaterOrEqual Var(\"%2\") Constant(ConstInt(0)) Var(\"%3\"))\n"
      "  JumpIfFalse(Var(\"%3\") \"L_break\")\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstChar(1)) Var(\"%4\"))\n"
      "  Copy(Var(\"%4\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Truncate(Constant(ConstInt(200)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  SignExtend(Var(\"x\") Var(\"%2\"))\n"
      "  Binary(GreaterOrEqual Var(\"%2\") Constant(ConstInt(0)) Var(\"%3\"))\n"
      "  JumpIfFalse(Var(\"%3\") \"L_break\")\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstChar(1)) Var(\"%4\"))\n"
      "  Copy(Var(\"%4\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_rotate_signed_countdown_loops: test_loop_rotate.TestLoopRotateSChar.test_schar_with_signextend_in_cond_rotates (call 1)") {
  c6502::testing::check_stage_case(
      "tac_rotate_signed_countdown_loops",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"scratch\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Truncate(Constant(ConstInt(15)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  SignExtend(Var(\"x\") Var(\"%2\"))\n"
      "  Binary(GreaterOrEqual Var(\"%2\") Constant(ConstInt(0)) Var(\"%3\"))\n"
      "  JumpIfFalse(Var(\"%3\") \"L_break\")\n"
      "  Copy(Var(\"x\") Var(\"scratch\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstChar(1)) Var(\"%4\"))\n"
      "  Copy(Var(\"%4\") Var(\"x\"))\n"
      "  Jump(\"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Truncate(Constant(ConstInt(15)) Var(\"%1\"))\n"
      "  Copy(Var(\"%1\") Var(\"x\"))\n"
      "  Label(\"L_start\")\n"
      "  Copy(Var(\"x\") Var(\"scratch\"))\n"
      "  Label(\"L_continue\")\n"
      "  Binary(Subtract Var(\"x\") Constant(ConstChar(1)) Var(\"%4\"))\n"
      "  Copy(Var(\"%4\") Var(\"x\"))\n"
      "  SignExtend(Var(\"x\") Var(\"%2\"))\n"
      "  Binary(GreaterOrEqual Var(\"%2\") Constant(ConstInt(0)) Var(\"%3\"))\n"
      "  JumpIfTrue(Var(\"%3\") \"L_start\")\n"
      "  Label(\"L_break\")\n"
      "])\n",
      "");
}

TEST_CASE("tac_noreturn_calls: test_noreturn_call.TestPass.test_code_after_the_call_goes (call 1)") {
  c6502::testing::check_stage_case(
      "tac_noreturn_calls",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"die\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": true}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"x\") \".ok\")\n"
      "  FunctionCall(\"die\" [] None false false)\n"
      "  FunctionCall(\"g\" [] None false false)\n"
      "  Copy(Constant(ConstInt(1)) Var(\"y\"))\n"
      "  Label(\".ok\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  JumpIfTrue(Var(\"x\") \".ok\")\n"
      "  FunctionCall(\"die\" [] None false false)\n"
      "  Ret(None)\n"
      "  Label(\".ok\")\n"
      "  Ret(Constant(ConstInt(1)))\n"
      "])\n",
      "");
}

TEST_CASE("tac_noreturn_calls: test_noreturn_call.TestPass.test_other_calls_left_alone (call 1)") {
  c6502::testing::check_stage_case(
      "tac_noreturn_calls",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"die\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": true}}}}], [\"g\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Void)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": false, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}], [\"f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"FunType([] Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.FunAttr\", \"fields\": {\"defined\": true, \"is_global\": true, \"carry_return\": false, \"address\": null, \"preserves\": {\"set\": []}, \"section\": null, \"param_zeropage\": {\"tuple\": []}, \"returns_bool\": false, \"noreturn\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  FunctionCall(\"g\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  FunctionCall(\"g\" [] None false false)\n"
      "  Ret(None)\n"
      "])\n",
      "");
}
