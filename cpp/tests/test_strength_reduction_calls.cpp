// The calls of `tac_reduce_strength`
// (passes/optimization/strength_reduction.py) that the Python unit
// tests make, each with the function the test gives the pass, the
// symbols the call read and the SSA names, and what the Python pass
// returned and changed, which the C++ pass must return and change too.
// They were recorded by wrapping the Python passes while
// tests/test_constant_folding.py, test_strength_reduction.py,
// test_static_const_fold.py, test_reassoc_offset_licm.py,
// test_split_tables.py, test_pointer_member_access.py, and
// test_narrow_byte_range.py ran, leaving out each call whose record
// takes more than 15 KB. Every call that
// tests/test_strength_reduction.py makes is here, and of the other
// tests the calls that changed their function. A test that calls the
// pass several times gives a case for each distinct call.
#include "doctest.h"
#include "stage_case.h"

using c6502::testing::check_stage_case;

TEST_CASE("tac_reduce_strength: test_pointer_member_access.TestRuns.test_choice_of_member (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%37.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%35.1\", \"%36.1\", \"%37.1\", \"%37.1.dc0\", \"%38.1\", \"@14.p\", \"@15.n\"]}}}",
      "Function(\"option\" true [\"@14.p\" \"@15.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@15.n\") Var(\"%35.1\"))\n"
      "  Binary(Subtract Var(\"%35.1\") Constant(ConstInt(1)) Var(\"%36.1\"))\n"
      "  Binary(Multiply Var(\"%35.1\") Constant(ConstInt(9)) Var(\"%37.1.dc0\"))\n"
      "  Binary(Subtract Var(\"%37.1.dc0\") Constant(ConstInt(9)) Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"@14.p\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "Function(\"option\" true [\"@14.p\" \"@15.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@15.n\") Var(\"%35.1\"))\n"
      "  Binary(Subtract Var(\"%35.1\") Constant(ConstInt(1)) Var(\"%36.1\"))\n"
      "  Binary(LeftShift Var(\"%35.1\") Constant(ConstInt(3)) Var(\"%37.1.dc0.sr0\"))\n"
      "  Binary(Add Var(\"%37.1.dc0.sr0\") Var(\"%35.1\") Var(\"%37.1.dc0\"))\n"
      "  Binary(Subtract Var(\"%37.1.dc0\") Constant(ConstInt(9)) Var(\"%37.1\"))\n"
      "  Binary(Add Var(\"@14.p\") Var(\"%37.1\") Var(\"%38.1\"))\n"
      "  Ret(Var(\"%38.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%37.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%37.1.dc0.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%35.1\", \"%36.1\", \"%37.1\", \"%37.1.dc0\", \"%37.1.dc0.sr0\", \"%38.1\", \"@14.p\", \"@15.n\"]}}}");
}

TEST_CASE("tac_reduce_strength: test_pointer_member_access.TestRuns.test_choice_of_member (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%48.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%50.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(255)) Var(\"%52.1\"))\n"
      "  Copy(Constant(ConstUChar(33)) Var(\"%53.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%52.1\") Var(\"%53.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%54.1\"))\n"
      "  Binary(Add Var(\"%54.1\") Constant(ConstInt(7)) Var(\"%55.1\"))\n"
      "  Load(Var(\"%55.1\") Var(\"%56.1\") false)\n"
      "  ZeroExtend(Var(\"%56.1\") Var(\"%57.1\"))\n"
      "  Binary(Multiply Var(\"%57.1\") Constant(ConstInt(256)) Var(\"%58.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%59.1\"))\n"
      "  Binary(Add Var(\"%59.1\") Constant(ConstInt(9)) Var(\"%60.1\"))\n"
      "  Load(Var(\"%60.1\") Var(\"%61.1\") false)\n"
      "  ZeroExtend(Var(\"%61.1\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%58.1\") Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%64.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Constant(ConstInt(8)) Var(\"%65.1\"))\n"
      "  Load(Var(\"%65.1\") Var(\"%66.1\") false)\n"
      "  ZeroExtend(Var(\"%66.1\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%63.1\") Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Ret(Var(\"%68.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%48.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%50.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(255)) Var(\"%52.1\"))\n"
      "  Copy(Constant(ConstUChar(33)) Var(\"%53.1\"))\n"
      "  FunctionCall(\"choose\" [Var(\"%52.1\") Var(\"%53.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%54.1\"))\n"
      "  Binary(Add Var(\"%54.1\") Constant(ConstInt(7)) Var(\"%55.1\"))\n"
      "  Load(Var(\"%55.1\") Var(\"%56.1\") false)\n"
      "  ZeroExtend(Var(\"%56.1\") Var(\"%57.1\"))\n"
      "  Binary(LeftShift Var(\"%57.1\") Constant(ConstInt(8)) Var(\"%58.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%59.1\"))\n"
      "  Binary(Add Var(\"%59.1\") Constant(ConstInt(9)) Var(\"%60.1\"))\n"
      "  Load(Var(\"%60.1\") Var(\"%61.1\") false)\n"
      "  ZeroExtend(Var(\"%61.1\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%58.1\") Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%64.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Constant(ConstInt(8)) Var(\"%65.1\"))\n"
      "  Load(Var(\"%65.1\") Var(\"%66.1\") false)\n"
      "  ZeroExtend(Var(\"%66.1\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%63.1\") Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Ret(Var(\"%68.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_pointer_member_access.TestRuns.test_choice_of_member_after_a_test (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\", \"%69.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%51.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%52.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%51.1\") Var(\"%52.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%53.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%54.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%53.1\") Var(\"%54.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%55.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%56.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Constant(ConstInt(10)) Var(\"%57.1\"))\n"
      "  Store(Var(\"%55.1\") Var(\"%57.1\") false)\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%58.1\"))\n"
      "  Copy(Constant(ConstUChar(33)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%58.1\") Var(\"%59.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%60.1\") Constant(ConstInt(7)) Var(\"%61.1\"))\n"
      "  Load(Var(\"%61.1\") Var(\"%62.1\") false)\n"
      "  ZeroExtend(Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  Binary(Multiply Var(\"%63.1\") Constant(ConstInt(256)) Var(\"%64.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Constant(ConstInt(9)) Var(\"%66.1\"))\n"
      "  Load(Var(\"%66.1\") Var(\"%67.1\") false)\n"
      "  ZeroExtend(Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Var(\"%68.1\") Var(\"%69.1\"))\n"
      "  Ret(Var(\"%69.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(3)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%51.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%52.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%51.1\") Var(\"%52.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%53.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%54.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%53.1\") Var(\"%54.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(4)) Var(\"%55.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%56.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Constant(ConstInt(10)) Var(\"%57.1\"))\n"
      "  Store(Var(\"%55.1\") Var(\"%57.1\") false)\n"
      "  Copy(Constant(ConstUChar(7)) Var(\"%58.1\"))\n"
      "  Copy(Constant(ConstUChar(33)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"choose_if\" [Var(\"%58.1\") Var(\"%59.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%60.1\") Constant(ConstInt(7)) Var(\"%61.1\"))\n"
      "  Load(Var(\"%61.1\") Var(\"%62.1\") false)\n"
      "  ZeroExtend(Var(\"%62.1\") Var(\"%63.1\"))\n"
      "  Binary(LeftShift Var(\"%63.1\") Constant(ConstInt(8)) Var(\"%64.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Constant(ConstInt(9)) Var(\"%66.1\"))\n"
      "  Load(Var(\"%66.1\") Var(\"%67.1\") false)\n"
      "  ZeroExtend(Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Binary(Add Var(\"%64.1\") Var(\"%68.1\") Var(\"%69.1\"))\n"
      "  Ret(Var(\"%69.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_pointer_member_access.TestRuns.test_choice_of_member_after_the_pointer_moves (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%48.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%50.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%52.1\"))\n"
      "  Binary(Add Var(\"%52.1\") Constant(ConstInt(7)) Var(\"%53.1\"))\n"
      "  Load(Var(\"%53.1\") Var(\"%54.1\") false)\n"
      "  ZeroExtend(Var(\"%54.1\") Var(\"%55.1\"))\n"
      "  Binary(Multiply Var(\"%55.1\") Constant(ConstInt(256)) Var(\"%56.1\"))\n"
      "  GetAddress(Var(\"m2\") Var(\"%57.1\"))\n"
      "  Binary(Add Var(\"%57.1\") Constant(ConstInt(9)) Var(\"%58.1\"))\n"
      "  Load(Var(\"%58.1\") Var(\"%59.1\") false)\n"
      "  ZeroExtend(Var(\"%59.1\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%62.1\") Constant(ConstInt(9)) Var(\"%63.1\"))\n"
      "  Load(Var(\"%63.1\") Var(\"%64.1\") false)\n"
      "  ZeroExtend(Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%61.1\") Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Ret(Var(\"%66.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(5)) Var(\"%48.1\"))\n"
      "  Copy(Constant(ConstUChar(11)) Var(\"%49.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%48.1\") Var(\"%49.1\")] None false false)\n"
      "  Copy(Constant(ConstUChar(6)) Var(\"%50.1\"))\n"
      "  Copy(Constant(ConstUChar(22)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"choose_moved\" [Var(\"%50.1\") Var(\"%51.1\")] None false false)\n"
      "  GetAddress(Var(\"m\") Var(\"%52.1\"))\n"
      "  Binary(Add Var(\"%52.1\") Constant(ConstInt(7)) Var(\"%53.1\"))\n"
      "  Load(Var(\"%53.1\") Var(\"%54.1\") false)\n"
      "  ZeroExtend(Var(\"%54.1\") Var(\"%55.1\"))\n"
      "  Binary(LeftShift Var(\"%55.1\") Constant(ConstInt(8)) Var(\"%56.1\"))\n"
      "  GetAddress(Var(\"m2\") Var(\"%57.1\"))\n"
      "  Binary(Add Var(\"%57.1\") Constant(ConstInt(9)) Var(\"%58.1\"))\n"
      "  Load(Var(\"%58.1\") Var(\"%59.1\") false)\n"
      "  ZeroExtend(Var(\"%59.1\") Var(\"%60.1\"))\n"
      "  Binary(Add Var(\"%56.1\") Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%62.1\"))\n"
      "  Binary(Add Var(\"%62.1\") Constant(ConstInt(9)) Var(\"%63.1\"))\n"
      "  Load(Var(\"%63.1\") Var(\"%64.1\") false)\n"
      "  ZeroExtend(Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  Binary(Add Var(\"%61.1\") Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Ret(Var(\"%66.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_pointer_member_access.TestRuns.test_compound_assignment_with_a_call (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%48.1\", \"%49.1\", \"%50.1\", \"%51.1\", \"%52.1\", \"%53.1\", \"%54.1\", \"%55.1\", \"%56.1\", \"%57.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\", \"%69.1\", \"%70.1\", \"%71.1\", \"%72.1\", \"%73.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(64)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%51.1\")] Var(\"%52.1\") false false)\n"
      "  ZeroExtend(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%54.1\"))\n"
      "  Load(Var(\"%54.1\") Var(\"%55.1\") false)\n"
      "  ZeroExtend(Var(\"%55.1\") Var(\"%56.1\"))\n"
      "  Binary(BitwiseOr Var(\"%56.1\") Var(\"%53.1\") Var(\"%57.1\"))\n"
      "  Truncate(Var(\"%57.1\") Var(\"%58.1\"))\n"
      "  Store(Var(\"%58.1\") Var(\"%54.1\") false)\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%59.1\")] Var(\"%60.1\") false false)\n"
      "  ZeroExtend(Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%62.1\"))\n"
      "  Load(Var(\"%62.1\") Var(\"%63.1\") false)\n"
      "  ZeroExtend(Var(\"%63.1\") Var(\"%64.1\"))\n"
      "  Binary(Subtract Var(\"%64.1\") Var(\"%61.1\") Var(\"%65.1\"))\n"
      "  Truncate(Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Store(Var(\"%66.1\") Var(\"%62.1\") false)\n"
      "  GetAddress(Var(\"m\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%67.1\") Constant(ConstInt(10)) Var(\"%68.1\"))\n"
      "  Load(Var(\"%68.1\") Var(\"%69.1\") false)\n"
      "  ZeroExtend(Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Binary(Multiply Var(\"%70.1\") Constant(ConstInt(16)) Var(\"%71.1\"))\n"
      "  ZeroExtend(Var(\"calls\") Var(\"%72.1\"))\n"
      "  Binary(Add Var(\"%71.1\") Var(\"%72.1\") Var(\"%73.1\"))\n"
      "  Ret(Var(\"%73.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  Copy(Constant(ConstUChar(64)) Var(\"%48.1\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(10)) Var(\"%50.1\"))\n"
      "  Store(Var(\"%48.1\") Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUChar(2)) Var(\"%51.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%51.1\")] Var(\"%52.1\") false false)\n"
      "  ZeroExtend(Var(\"%52.1\") Var(\"%53.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%54.1\"))\n"
      "  Load(Var(\"%54.1\") Var(\"%55.1\") false)\n"
      "  ZeroExtend(Var(\"%55.1\") Var(\"%56.1\"))\n"
      "  Binary(BitwiseOr Var(\"%56.1\") Var(\"%53.1\") Var(\"%57.1\"))\n"
      "  Truncate(Var(\"%57.1\") Var(\"%58.1\"))\n"
      "  Store(Var(\"%58.1\") Var(\"%54.1\") false)\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%59.1\"))\n"
      "  FunctionCall(\"h\" [Var(\"%59.1\")] Var(\"%60.1\") false false)\n"
      "  ZeroExtend(Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"E\") Constant(ConstInt(10)) Var(\"%62.1\"))\n"
      "  Load(Var(\"%62.1\") Var(\"%63.1\") false)\n"
      "  ZeroExtend(Var(\"%63.1\") Var(\"%64.1\"))\n"
      "  Binary(Subtract Var(\"%64.1\") Var(\"%61.1\") Var(\"%65.1\"))\n"
      "  Truncate(Var(\"%65.1\") Var(\"%66.1\"))\n"
      "  Store(Var(\"%66.1\") Var(\"%62.1\") false)\n"
      "  GetAddress(Var(\"m\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%67.1\") Constant(ConstInt(10)) Var(\"%68.1\"))\n"
      "  Load(Var(\"%68.1\") Var(\"%69.1\") false)\n"
      "  ZeroExtend(Var(\"%69.1\") Var(\"%70.1\"))\n"
      "  Binary(LeftShift Var(\"%70.1\") Constant(ConstInt(4)) Var(\"%71.1\"))\n"
      "  ZeroExtend(Var(\"calls\") Var(\"%72.1\"))\n"
      "  Binary(Add Var(\"%71.1\") Var(\"%72.1\") Var(\"%73.1\"))\n"
      "  Ret(Var(\"%73.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_pointer_member_access.TestRuns.test_member_compare (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%47.1\", \"%49.1\", \"%50.1\", \"%52.1\", \"%54.1\", \"%56.1\", \"%58.1\", \"%59.1\", \"%60.1\", \"%61.1\", \"%62.1\", \"%63.1\", \"%64.1\", \"%65.1\", \"%66.1\", \"%67.1\", \"%68.1\", \"@18.a.1\", \"@19.b.1\", \"@20.c.1\", \"@21.d.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(11)) Var(\"%50.1\"))\n"
      "  Store(Constant(ConstUInt(4608)) Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUInt(4607)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%52.1\") false false)\n"
      "  Copy(Var(\"%52.1\") Var(\"@18.a.1\"))\n"
      "  Copy(Constant(ConstUInt(4608)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%54.1\") false false)\n"
      "  Copy(Var(\"%54.1\") Var(\"@19.b.1\"))\n"
      "  Copy(Constant(ConstUInt(4609)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%56.1\") false false)\n"
      "  Copy(Var(\"%56.1\") Var(\"@20.c.1\"))\n"
      "  Copy(Constant(ConstUInt(4864)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%58.1\") false false)\n"
      "  Copy(Var(\"%58.1\") Var(\"@21.d.1\"))\n"
      "  ZeroExtend(Var(\"@18.a.1\") Var(\"%59.1\"))\n"
      "  ZeroExtend(Var(\"@19.b.1\") Var(\"%60.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(2)) Var(\"%60.1\") Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"%59.1\") Var(\"%61.1\") Var(\"%62.1\"))\n"
      "  ZeroExtend(Var(\"@20.c.1\") Var(\"%63.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(4)) Var(\"%63.1\") Var(\"%64.1\"))\n"
      "  Binary(Add Var(\"%62.1\") Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  ZeroExtend(Var(\"@21.d.1\") Var(\"%66.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(8)) Var(\"%66.1\") Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Ret(Var(\"%68.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  GetAddress(Var(\"m\") Var(\"%47.1\"))\n"
      "  Copy(Var(\"%47.1\") Var(\"E\"))\n"
      "  GetAddress(Var(\"m\") Var(\"%49.1\"))\n"
      "  Binary(Add Var(\"%49.1\") Constant(ConstInt(11)) Var(\"%50.1\"))\n"
      "  Store(Constant(ConstUInt(4608)) Var(\"%50.1\") false)\n"
      "  Copy(Constant(ConstUInt(4607)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%52.1\") false false)\n"
      "  Copy(Var(\"%52.1\") Var(\"@18.a.1\"))\n"
      "  Copy(Constant(ConstUInt(4608)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%54.1\") false false)\n"
      "  Copy(Var(\"%54.1\") Var(\"@19.b.1\"))\n"
      "  Copy(Constant(ConstUInt(4609)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%56.1\") false false)\n"
      "  Copy(Var(\"%56.1\") Var(\"@20.c.1\"))\n"
      "  Copy(Constant(ConstUInt(4864)) Var(\"W\"))\n"
      "  FunctionCall(\"le\" [] Var(\"%58.1\") false false)\n"
      "  Copy(Var(\"%58.1\") Var(\"@21.d.1\"))\n"
      "  ZeroExtend(Var(\"@18.a.1\") Var(\"%59.1\"))\n"
      "  ZeroExtend(Var(\"@19.b.1\") Var(\"%60.1\"))\n"
      "  Binary(LeftShift Var(\"%60.1\") Constant(ConstInt(1)) Var(\"%61.1\"))\n"
      "  Binary(Add Var(\"%59.1\") Var(\"%61.1\") Var(\"%62.1\"))\n"
      "  ZeroExtend(Var(\"@20.c.1\") Var(\"%63.1\"))\n"
      "  Binary(LeftShift Var(\"%63.1\") Constant(ConstInt(2)) Var(\"%64.1\"))\n"
      "  Binary(Add Var(\"%62.1\") Var(\"%64.1\") Var(\"%65.1\"))\n"
      "  ZeroExtend(Var(\"@21.d.1\") Var(\"%66.1\"))\n"
      "  Binary(LeftShift Var(\"%66.1\") Constant(ConstInt(3)) Var(\"%67.1\"))\n"
      "  Binary(Add Var(\"%65.1\") Var(\"%67.1\") Var(\"%68.1\"))\n"
      "  Ret(Var(\"%68.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_pointer_member_access.TestShapes.test_index_minus_one_multiplies_the_byte (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.dc0\", \"%3.1\", \"@0.p\", \"@1.n\"]}}}",
      "Function(\"option\" true [\"@0.p\" \"@1.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%0.1\"))\n"
      "  Binary(Subtract Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Binary(Multiply Var(\"%0.1\") Constant(ConstInt(9)) Var(\"%2.1.dc0\"))\n"
      "  Binary(Subtract Var(\"%2.1.dc0\") Constant(ConstInt(9)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"option\" true [\"@0.p\" \"@1.n\"] [\n"
      "  Label(\".option@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.n\") Var(\"%0.1\"))\n"
      "  Binary(Subtract Var(\"%0.1\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Binary(LeftShift Var(\"%0.1\") Constant(ConstInt(3)) Var(\"%2.1.dc0.sr0\"))\n"
      "  Binary(Add Var(\"%2.1.dc0.sr0\") Var(\"%0.1\") Var(\"%2.1.dc0\"))\n"
      "  Binary(Subtract Var(\"%2.1.dc0\") Constant(ConstInt(9)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"@0.p\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1.dc0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.dc0.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.dc0\", \"%2.1.dc0.sr0\", \"%3.1\", \"@0.p\", \"@1.n\"]}}}");
}

TEST_CASE("tac_reduce_strength: test_reassoc_offset_licm.TestReassocLicmSim.test_header_fields_after_variable_skip (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.d\", \"@1.n.1\"]}}}",
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
      "  Binary(Multiply Var(\"%12.1\") Constant(ConstInt(10)) Var(\"%13.1\"))\n"
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
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%7.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%13.1.sr1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%13.1\", \"%13.1.sr0\", \"%13.1.sr1\", \"%14.1\", \"%15.1\", \"%16.1\", \"%17.1\", \"%18.1\", \"%19.1\", \"%2.1\", \"%20.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.d\", \"@1.n.1\"]}}}");
}

TEST_CASE("tac_reduce_strength: test_split_tables.TestSplitTablesSim.test_long_table (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"@0.i\"]}}}",
      "Function(\"get\" true [\"@0.i\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  GetAddress(Var(\"T\") Var(\"%0.1\"))\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%1.1\"))\n"
      "  Binary(Multiply Var(\"%1.1\") Constant(ConstInt(2)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  Ret(Var(\"%4.1\"))\n"
      "])\n",
      "Function(\"get\" true [\"@0.i\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  GetAddress(Var(\"T\") Var(\"%0.1\"))\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%1.1\"))\n"
      "  Binary(LeftShift Var(\"%1.1\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  Binary(Add Var(\"%0.1\") Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Load(Var(\"%3.1\") Var(\"%4.1\") false)\n"
      "  Ret(Var(\"%4.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_split_tables.TestSplitTablesSim.test_pointer_table (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"@0.i\", \"@1.j\"]}}}",
      "Function(\"get\" true [\"@0.i\" \"@1.j\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(Multiply Var(\"%1.1\") Constant(ConstUChar(2)) Var(\"%2.1\"))\n"
      "  IndexedLoad(\"P\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"@1.j\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%3.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Load(Var(\"%5.1\") Var(\"%6.1\") false)\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "Function(\"get\" true [\"@0.i\" \"@1.j\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(LeftShift Var(\"%1.1\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  IndexedLoad(\"P\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  ZeroExtend(Var(\"@1.j\") Var(\"%4.1\"))\n"
      "  Binary(Add Var(\"%3.1\") Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Load(Var(\"%5.1\") Var(\"%6.1\") false)\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_split_tables.TestSplitTablesSim.test_uint16_table (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.i\"]}}}",
      "Function(\"get\" true [\"@0.i\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(Multiply Var(\"%1.1\") Constant(ConstUChar(2)) Var(\"%2.1\"))\n"
      "  IndexedLoad(\"T\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"get\" true [\"@0.i\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(LeftShift Var(\"%1.1\") Constant(ConstInt(1)) Var(\"%2.1\"))\n"
      "  IndexedLoad(\"T\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_split_tables.TestSplitTablesSim.test_uint16_table (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"@1.i\"]}}}",
      "Function(\"next\" true [\"@1.i\"] [\n"
      "  Label(\".next@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(Multiply Var(\"%5.1\") Constant(ConstUChar(2)) Var(\"%6.1\"))\n"
      "  IndexedLoad(\"T\" Var(\"%6.1\") Var(\"%7.1\") false 2 0)\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "Function(\"next\" true [\"@1.i\"] [\n"
      "  Label(\".next@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@1.i\") Var(\"%4.1\"))\n"
      "  Truncate(Var(\"%4.1\") Var(\"%5.1\"))\n"
      "  Binary(LeftShift Var(\"%5.1\") Constant(ConstInt(1)) Var(\"%6.1\"))\n"
      "  IndexedLoad(\"T\" Var(\"%6.1\") Var(\"%7.1\") false 2 0)\n"
      "  Ret(Var(\"%7.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_split_tables.TestSplitTablesSim.test_uint32_table (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"@0.i\"]}}}",
      "Function(\"get\" true [\"@0.i\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(Multiply Var(\"%1.1\") Constant(ConstUChar(4)) Var(\"%2.1\"))\n"
      "  IndexedLoad(\"T\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"get\" true [\"@0.i\"] [\n"
      "  Label(\".get@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@0.i\") Var(\"%0.1\"))\n"
      "  Truncate(Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(LeftShift Var(\"%1.1\") Constant(ConstInt(2)) Var(\"%2.1\"))\n"
      "  IndexedLoad(\"T\" Var(\"%2.1\") Var(\"%3.1\") false 0 0)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_divide_by_4_keeps_truncation (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(Divide Var(\"@0.x\") Constant(ConstInt(4)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(Divide Var(\"@0.x\") Constant(ConstInt(4)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_divide_by_4_keeps_truncation (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Divide Var(\"@0.x\") Constant(ConstInt(4)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Divide Var(\"@0.x\") Constant(ConstInt(4)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_divide_by_4_keeps_truncation (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(-7))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(-7))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_divide_by_4_keeps_truncation (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-7)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-7)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_multiply_by_4 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(2)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(2)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_multiply_by_4 (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Multiply Var(\"@0.x\") Constant(ConstInt(4)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(2)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_multiply_by_4 (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(7))] Var(\"%1.1\") false false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(7))] Var(\"%1.1\") false false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_signed_multiply_by_4 (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(7))] Var(\"%1.1\") false false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(7))] Var(\"%1.1\") false false)\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_two_multiplies_into_one_global (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"z\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 128}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%11.1.lo1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.w.1\", \"@0.w.2\", \"@0.w.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(Multiply Var(\"z\") Constant(ConstULong(3)) Var(\"z\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%7.1\"))])\n"
      "  JumpIfMasked(Var(\"@0.w.2\") 1 false \".if_end@0\")\n"
      "  Binary(Multiply Var(\"z\") Constant(ConstULong(129)) Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%7.1\") Constant(ConstUChar(4)) \".loop@0_start\")\n"
      "  Truncate(Var(\"z\") Var(\"%11.1.lo1\"))\n"
      "  Binary(BitwiseAnd Var(\"%11.1.lo1\") Constant(ConstInt(32767)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"z\") Constant(ConstInt(1)) Var(\"z.sr0\"))\n"
      "  Binary(Add Var(\"z.sr0\") Var(\"z\") Var(\"z\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%7.1\"))])\n"
      "  JumpIfMasked(Var(\"@0.w.2\") 1 false \".if_end@0\")\n"
      "  Binary(LeftShift Var(\"z\") Constant(ConstInt(7)) Var(\"z.sr1\"))\n"
      "  Binary(Add Var(\"z.sr1\") Var(\"z\") Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%7.1\") Constant(ConstUChar(4)) \".loop@0_start\")\n"
      "  Truncate(Var(\"z\") Var(\"%11.1.lo1\"))\n"
      "  Binary(BitwiseAnd Var(\"%11.1.lo1\") Constant(ConstInt(32767)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"z\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.StaticAttr\", \"fields\": {\"initial_value\": {\"object\": \"passes.type_checking.Initial\", \"fields\": {\"value\": 128}}, \"is_global\": true, \"zeropage\": true, \"zp_address\": null, \"address\": null, \"section\": null, \"planes\": null, \"mergeable\": false, \"was_bool\": false}}}}], [\"z.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"z.sr1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%11.1.lo1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.w.1\", \"@0.w.2\", \"@0.w.3\", \"z.sr0\", \"z.sr1\"]}}}");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_two_multiplies_into_one_global (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%11.1.lo1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.w.1\", \"@0.w.2\", \"@0.w.3\", \"z.sr0\", \"z.sr1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"z\") Constant(ConstInt(1)) Var(\"z.sr0\"))\n"
      "  Binary(Add Var(\"z.sr0\") Var(\"z\") Var(\"z\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%7.1\"))])\n"
      "  JumpIfMasked(Var(\"@0.w.2\") 1 false \".if_end@0\")\n"
      "  Binary(LeftShift Var(\"z\") Constant(ConstInt(7)) Var(\"z.sr1\"))\n"
      "  Binary(Add Var(\"z.sr1\") Var(\"z\") Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%7.1\") Constant(ConstUChar(4)) \".loop@0_start\")\n"
      "  Truncate(Var(\"z\") Var(\"%11.1.lo1\"))\n"
      "  Binary(BitwiseAnd Var(\"%11.1.lo1\") Constant(ConstInt(32767)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"z\") Constant(ConstInt(1)) Var(\"z.sr0\"))\n"
      "  Binary(Add Var(\"z.sr0\") Var(\"z\") Var(\"z\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Constant(ConstUChar(0))) PhiArg(\".loop@0_continue\" Var(\"%7.1\"))])\n"
      "  JumpIfMasked(Var(\"@0.w.2\") 1 false \".if_end@0\")\n"
      "  Binary(LeftShift Var(\"z\") Constant(ConstInt(7)) Var(\"z.sr1\"))\n"
      "  Binary(Add Var(\"z.sr1\") Var(\"z\") Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  JumpIfCmp(LessThan Var(\"%7.1\") Constant(ConstUChar(4)) \".loop@0_start\")\n"
      "  Truncate(Var(\"z\") Var(\"%11.1.lo1\"))\n"
      "  Binary(BitwiseAnd Var(\"%11.1.lo1\") Constant(ConstInt(32767)) Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_two_multiplies_into_one_global (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%10.1\", \"%11.1\", \"%12.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\", \"%7.1\", \"%8.1\", \"%9.1\", \"@0.w.1\", \"@0.w.2\", \"@0.w.3\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstULong(3)) Var(\"%0.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"z\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"@0.w.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.w.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.w.3\"))])\n"
      "  ZeroExtend(Var(\"@0.w.2\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  JumpIfFalse(Var(\"%4.1\") \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Copy(Constant(ConstULong(129)) Var(\"%5.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@0.w.3\"))\n"
      "  ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Binary(LessThan Var(\"%8.1\") Constant(ConstInt(4)) Var(\"%9.1\"))\n"
      "  JumpIfTrue(Var(\"%9.1\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Constant(ConstULong(32767)) Var(\"%10.1\"))\n"
      "  Binary(BitwiseAnd Var(\"z\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstULong(3)) Var(\"%0.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Copy(Var(\"%1.1\") Var(\"z\"))\n"
      "  Copy(Constant(ConstUChar(0)) Var(\"%2.1\"))\n"
      "  Copy(Var(\"%2.1\") Var(\"@0.w.1\"))\n"
      "  Label(\".loop@0_start\")\n"
      "  Phi(Var(\"@0.w.2\") [PhiArg(\".main@ssa_block@0\" Var(\"@0.w.1\")) PhiArg(\".loop@0_continue\" Var(\"@0.w.3\"))])\n"
      "  ZeroExtend(Var(\"@0.w.2\") Var(\"%3.1\"))\n"
      "  Binary(BitwiseAnd Var(\"%3.1\") Constant(ConstInt(1)) Var(\"%4.1\"))\n"
      "  JumpIfFalse(Var(\"%4.1\") \".if_end@0\")\n"
      "  Label(\".main@ssa_block@1\")\n"
      "  Copy(Constant(ConstULong(129)) Var(\"%5.1\"))\n"
      "  Binary(Multiply Var(\"z\") Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Copy(Var(\"%6.1\") Var(\"z\"))\n"
      "  Label(\".if_end@0\")\n"
      "  Label(\".loop@0_continue\")\n"
      "  Binary(Add Var(\"@0.w.2\") Constant(ConstUChar(1)) Var(\"%7.1\"))\n"
      "  Copy(Var(\"%7.1\") Var(\"@0.w.3\"))\n"
      "  ZeroExtend(Var(\"%7.1\") Var(\"%8.1\"))\n"
      "  Binary(LessThan Var(\"%8.1\") Constant(ConstInt(4)) Var(\"%9.1\"))\n"
      "  JumpIfTrue(Var(\"%9.1\") \".loop@0_start\")\n"
      "  Label(\".loop@0_break\")\n"
      "  Copy(Constant(ConstULong(32767)) Var(\"%10.1\"))\n"
      "  Binary(BitwiseAnd Var(\"z\") Var(\"%10.1\") Var(\"%11.1\"))\n"
      "  Truncate(Var(\"%11.1\") Var(\"%12.1\"))\n"
      "  Ret(Var(\"%12.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_divide_by_2 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Divide Var(\"@0.x\") Constant(ConstUInt(2)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(RightShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_divide_by_2 (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(RightShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(RightShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_divide_by_2 (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(15))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(15))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_divide_by_2 (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(15))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(15))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_modulo_by_4 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Modulo Var(\"@0.x\") Constant(ConstUInt(4)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(BitwiseAnd Var(\"@0.x\") Constant(ConstUInt(3)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_modulo_by_4 (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(BitwiseAnd Var(\"@0.x\") Constant(ConstUInt(3)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(BitwiseAnd Var(\"@0.x\") Constant(ConstUInt(3)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_modulo_by_4 (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_modulo_by_4 (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_2 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_2 (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Multiply Var(\"@0.x\") Constant(ConstUInt(2)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_2 (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13579))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13579))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_2 (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13579))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(13579))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_8 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(3)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(3)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_8 (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(Multiply Var(\"@0.x\") Constant(ConstUInt(8)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(3)) Var(\"%1.1\"))\n"
      "  Ret(Var(\"%1.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_8 (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(100))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(100))] Var(\"%3.1\") false false)\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestEndToEndShifts.test_unsigned_multiply_by_8 (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%2.1\", \"%3.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(100))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(100))] Var(\"%2.1\") false false)\n"
      "  Copy(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Ret(Var(\"%3.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_by_1_signed (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(RightShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(RightShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_by_1_signed (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(RightShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(RightShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_by_1_signed (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(-2))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(-2))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_by_1_signed (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-2)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-2)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_negative_odd (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(-3))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstInt(-3))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_int16_shift_right_negative_odd (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-3)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  Copy(Constant(ConstInt(-3)) Var(\"%1.1\"))\n"
      "  FunctionCall(\"f\" [Var(\"%1.1\")] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_uint16_shift_left_by_1 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_uint16_shift_left_by_1 (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"@0.x\"]}}}",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "Function(\"f\" true [\"@0.x\"] [\n"
      "  Label(\".f@ssa_block@0\")\n"
      "  Binary(LeftShift Var(\"@0.x\") Constant(ConstInt(1)) Var(\"%0.1\"))\n"
      "  Ret(Var(\"%0.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_uint16_shift_left_by_1 (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(16384))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(16384))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_uint16_shift_left_by_1 (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(16384))] Var(\"%1.1\") false false)\n"
      "  Copy(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(16384))] Var(\"%1.1\") false false)\n"
      "  Copy(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_uint16_shift_right_by_1 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(65534))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(65534))] Var(\"%2.1\") false false)\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestInlineShiftByOne.test_uint16_shift_right_by_1 (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%1.1\", \"%2.1\"]}}}",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(65534))] Var(\"%1.1\") false false)\n"
      "  Copy(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "Function(\"main\" true [] [\n"
      "  Label(\".main@ssa_block@0\")\n"
      "  FunctionCall(\"f\" [Constant(ConstUInt(65534))] Var(\"%1.1\") false false)\n"
      "  Copy(Var(\"%1.1\") Var(\"%2.1\"))\n"
      "  Ret(Var(\"%2.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceDivide.test_signed_divide_unchanged (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Divide Var(\"%x\") Constant(ConstInt(2)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Divide Var(\"%x\") Constant(ConstInt(2)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceDivide.test_unsigned_var_divided_by_1_to_copy (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Divide Var(\"%x\") Constant(ConstUInt(1)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"%x\") Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceDivide.test_unsigned_var_divided_by_4_to_shift_2 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Divide Var(\"%x\") Constant(ConstUInt(4)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(RightShift Var(\"%x\") Constant(ConstInt(2)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceModulo.test_signed_modulo_unchanged (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Modulo Var(\"%x\") Constant(ConstInt(8)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Modulo Var(\"%x\") Constant(ConstInt(8)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceModulo.test_unsigned_var_mod_1_to_zero (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Modulo Var(\"%x\") Constant(ConstUInt(1)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Constant(ConstUInt(0)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceModulo.test_unsigned_var_mod_8_to_and_7 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Modulo Var(\"%x\") Constant(ConstUInt(8)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(BitwiseAnd Var(\"%x\") Constant(ConstUInt(7)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_both_constants_unchanged (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Constant(ConstUInt(3)) Constant(ConstUInt(4)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Constant(ConstUInt(3)) Constant(ConstUInt(4)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_const_times_var_commutes (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Constant(ConstUInt(4)) Var(\"%x\") Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"%x\") Constant(ConstInt(2)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_non_power_of_two_unchanged (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(11)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(11)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_power_minus_one_is_shift_and_subtract (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(7)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"%x\") Constant(ConstInt(3)) Var(\"%dst.sr0\"))\n"
      "  Binary(Subtract Var(\"%dst.sr0\") Var(\"%x\") Var(\"%dst\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_power_plus_one_is_shift_and_add (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(9)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"%x\") Constant(ConstInt(3)) Var(\"%dst.sr0\"))\n"
      "  Binary(Add Var(\"%dst.sr0\") Var(\"%x\") Var(\"%dst\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_signed_var_times_2_to_shift_1 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstInt(2)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"%x\") Constant(ConstInt(1)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_two_shifted_terms_share_the_low_shift (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(10)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"%x\") Constant(ConstInt(1)) Var(\"%dst.sr0\"))\n"
      "  Binary(LeftShift Var(\"%dst.sr0\") Constant(ConstInt(2)) Var(\"%dst.sr1\"))\n"
      "  Binary(Add Var(\"%dst.sr1\") Var(\"%dst.sr0\") Var(\"%dst\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst.sr1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}}}");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_two_term_rejected_when_shift_would_call_helper (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(65)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(65)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_unsigned_var_times_8_to_shift_3 (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(8)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Binary(LeftShift Var(\"%x\") Constant(ConstInt(3)) Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestStrengthReduceMultiply.test_var_times_1_to_copy (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": null}}",
      "Function(\"f\" true [] [\n"
      "  Binary(Multiply Var(\"%x\") Constant(ConstUInt(1)) Var(\"%dst\"))\n"
      "])\n",
      "Function(\"f\" true [] [\n"
      "  Copy(Var(\"%x\") Var(\"%dst\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestThreeTermByteMultiply.test_every_byte_constant (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%4866.1\", \"%4867.1\", \"%4868.1\", \"%4869.1\", \"%4870.1\", \"%4871.1\", \"%4872.1\", \"%4873.1\", \"@259.c\", \"@260.k\"]}}}",
      "Function(\"check\" true [\"@259.c\" \"@260.k\"] [\n"
      "  IndexedLoad(\"v\" Var(\"@260.k\") Var(\"%4869.1\") true 0 0)\n"
      "  Binary(Multiply Var(\"@259.c\") Var(\"%4869.1\") Var(\"%4872.1\"))\n"
      "  ZeroExtend(Var(\"%4872.1\") Var(\"%4873.1\"))\n"
      "  Ret(Var(\"%4873.1\"))\n"
      "])\n",
      "Function(\"check\" true [\"@259.c\" \"@260.k\"] [\n"
      "  IndexedLoad(\"v\" Var(\"@260.k\") Var(\"%4869.1\") true 0 0)\n"
      "  Binary(Multiply Var(\"@259.c\") Var(\"%4869.1\") Var(\"%4872.1\"))\n"
      "  ZeroExtend(Var(\"%4872.1\") Var(\"%4873.1\"))\n"
      "  Ret(Var(\"%4873.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestThreeTermByteMultiply.test_every_byte_constant (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%4866.1\", \"%4867.1\", \"%4868.1\", \"%4869.1\", \"%4870.1\", \"%4871.1\", \"%4872.1\", \"%4873.1\", \"@259.c\", \"@260.k\"]}}}",
      "Function(\"check\" true [\"@259.c\" \"@260.k\"] [\n"
      "  Label(\".check@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@259.c\") Var(\"%4866.1\"))\n"
      "  ZeroExtend(Var(\"@260.k\") Var(\"%4867.1\"))\n"
      "  Truncate(Var(\"%4867.1\") Var(\"%4868.1\"))\n"
      "  IndexedLoad(\"v\" Var(\"%4868.1\") Var(\"%4869.1\") true 0 0)\n"
      "  ZeroExtend(Var(\"%4869.1\") Var(\"%4870.1\"))\n"
      "  Binary(Multiply Var(\"%4866.1\") Var(\"%4870.1\") Var(\"%4871.1\"))\n"
      "  Truncate(Var(\"%4871.1\") Var(\"%4872.1\"))\n"
      "  ZeroExtend(Var(\"%4872.1\") Var(\"%4873.1\"))\n"
      "  Ret(Var(\"%4873.1\"))\n"
      "])\n",
      "Function(\"check\" true [\"@259.c\" \"@260.k\"] [\n"
      "  Label(\".check@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"@259.c\") Var(\"%4866.1\"))\n"
      "  ZeroExtend(Var(\"@260.k\") Var(\"%4867.1\"))\n"
      "  Truncate(Var(\"%4867.1\") Var(\"%4868.1\"))\n"
      "  IndexedLoad(\"v\" Var(\"%4868.1\") Var(\"%4869.1\") true 0 0)\n"
      "  ZeroExtend(Var(\"%4869.1\") Var(\"%4870.1\"))\n"
      "  Binary(Multiply Var(\"%4866.1\") Var(\"%4870.1\") Var(\"%4871.1\"))\n"
      "  Truncate(Var(\"%4871.1\") Var(\"%4872.1\"))\n"
      "  ZeroExtend(Var(\"%4872.1\") Var(\"%4873.1\"))\n"
      "  Ret(Var(\"%4873.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestThreeTermByteMultiply.test_every_byte_constant (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%4866.1\", \"%4867.1\", \"%4868.1\", \"%4869.1\", \"%4870.1\", \"%4871.1\", \"%4872.1\", \"%4873.1\", \"@259.c\", \"@260.k\"]}}}",
      "Function(\"check\" true [\"@259.c\" \"@260.k\"] [\n"
      "  ZeroExtend(Var(\"@260.k\") Var(\"%4867.1\"))\n"
      "  Copy(Var(\"@260.k\") Var(\"%4868.1\"))\n"
      "  IndexedLoad(\"v\" Var(\"%4868.1\") Var(\"%4869.1\") true 0 0)\n"
      "  Binary(Multiply Var(\"@259.c\") Var(\"%4869.1\") Var(\"%4872.1\"))\n"
      "  ZeroExtend(Var(\"%4872.1\") Var(\"%4873.1\"))\n"
      "  Ret(Var(\"%4873.1\"))\n"
      "])\n",
      "Function(\"check\" true [\"@259.c\" \"@260.k\"] [\n"
      "  ZeroExtend(Var(\"@260.k\") Var(\"%4867.1\"))\n"
      "  Copy(Var(\"@260.k\") Var(\"%4868.1\"))\n"
      "  IndexedLoad(\"v\" Var(\"%4868.1\") Var(\"%4869.1\") true 0 0)\n"
      "  Binary(Multiply Var(\"@259.c\") Var(\"%4869.1\") Var(\"%4872.1\"))\n"
      "  ZeroExtend(Var(\"%4872.1\") Var(\"%4873.1\"))\n"
      "  Ret(Var(\"%4873.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestThreeTermByteMultiply.test_prng_step (1)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.lo1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\"]}}}",
      "Function(\"prng\" true [] [\n"
      "  ZeroExtend(Var(\"st\") Var(\"%0.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(69)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1.lo1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1\") Constant(ConstUChar(83)) Var(\"st\"))\n"
      "  Binary(RightShift Var(\"st\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "Function(\"prng\" true [] [\n"
      "  ZeroExtend(Var(\"st\") Var(\"%0.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(69)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Truncate(Var(\"%1.1\") Var(\"%2.1.lo1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1\") Constant(ConstUChar(83)) Var(\"st\"))\n"
      "  Binary(RightShift Var(\"st\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestThreeTermByteMultiply.test_prng_step (2)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%1.1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\"]}}}",
      "Function(\"prng\" true [] [\n"
      "  Label(\".prng@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"st\") Var(\"%0.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(69)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(83)) Var(\"%2.1\"))\n"
      "  Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"st\"))\n"
      "  ZeroExtend(Var(\"st\") Var(\"%4.1\"))\n"
      "  Binary(RightShift Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "Function(\"prng\" true [] [\n"
      "  Label(\".prng@ssa_block@0\")\n"
      "  ZeroExtend(Var(\"st\") Var(\"%0.1\"))\n"
      "  Binary(Multiply Constant(ConstInt(69)) Var(\"%0.1\") Var(\"%1.1\"))\n"
      "  Binary(Add Var(\"%1.1\") Constant(ConstInt(83)) Var(\"%2.1\"))\n"
      "  Truncate(Var(\"%2.1\") Var(\"%3.1\"))\n"
      "  Copy(Var(\"%3.1\") Var(\"st\"))\n"
      "  ZeroExtend(Var(\"st\") Var(\"%4.1\"))\n"
      "  Binary(RightShift Var(\"%4.1\") Constant(ConstInt(1)) Var(\"%5.1\"))\n"
      "  Truncate(Var(\"%5.1\") Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestThreeTermByteMultiply.test_prng_step (3)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.lo1\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\"]}}}",
      "Function(\"prng\" true [] [\n"
      "  Binary(Multiply Constant(ConstUChar(69)) Var(\"st\") Var(\"%2.1.lo1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1\") Constant(ConstUChar(83)) Var(\"st\"))\n"
      "  Binary(RightShift Var(\"st\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "Function(\"prng\" true [] [\n"
      "  Binary(LeftShift Var(\"st\") Constant(ConstInt(2)) Var(\"%2.1.lo1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%2.1.lo1.sr0\") Constant(ConstInt(4)) Var(\"%2.1.lo1.sr1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1.sr1\") Var(\"%2.1.lo1.sr0\") Var(\"%2.1.lo1.sr2\"))\n"
      "  Binary(Add Var(\"%2.1.lo1.sr2\") Var(\"st\") Var(\"%2.1.lo1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1\") Constant(ConstUChar(83)) Var(\"st\"))\n"
      "  Binary(RightShift Var(\"st\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "{\"args\": {}, \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%2.1.lo1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.lo1.sr0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.lo1.sr1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%2.1.lo1.sr2\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.lo1\", \"%2.1.lo1.sr0\", \"%2.1.lo1.sr1\", \"%2.1.lo1.sr2\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\"]}}}");
}

TEST_CASE("tac_reduce_strength: test_strength_reduction.TestThreeTermByteMultiply.test_prng_step (4)") {
  check_stage_case(
      "tac_reduce_strength",
      "{\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"%0.1\", \"%1.1\", \"%2.1\", \"%2.1.lo1\", \"%2.1.lo1.sr0\", \"%2.1.lo1.sr1\", \"%2.1.lo1.sr2\", \"%3.1\", \"%4.1\", \"%5.1\", \"%6.1\"]}}}",
      "Function(\"prng\" true [] [\n"
      "  Binary(LeftShift Var(\"st\") Constant(ConstInt(2)) Var(\"%2.1.lo1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%2.1.lo1.sr0\") Constant(ConstInt(4)) Var(\"%2.1.lo1.sr1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1.sr1\") Var(\"%2.1.lo1.sr0\") Var(\"%2.1.lo1.sr2\"))\n"
      "  Binary(Add Var(\"%2.1.lo1.sr2\") Var(\"st\") Var(\"%2.1.lo1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1\") Constant(ConstUChar(83)) Var(\"st\"))\n"
      "  Binary(RightShift Var(\"st\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "Function(\"prng\" true [] [\n"
      "  Binary(LeftShift Var(\"st\") Constant(ConstInt(2)) Var(\"%2.1.lo1.sr0\"))\n"
      "  Binary(LeftShift Var(\"%2.1.lo1.sr0\") Constant(ConstInt(4)) Var(\"%2.1.lo1.sr1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1.sr1\") Var(\"%2.1.lo1.sr0\") Var(\"%2.1.lo1.sr2\"))\n"
      "  Binary(Add Var(\"%2.1.lo1.sr2\") Var(\"st\") Var(\"%2.1.lo1\"))\n"
      "  Binary(Add Var(\"%2.1.lo1\") Constant(ConstUChar(83)) Var(\"st\"))\n"
      "  Binary(RightShift Var(\"st\") Constant(ConstUChar(1)) Var(\"%6.1\"))\n"
      "  Ret(Var(\"%6.1\"))\n"
      "])\n",
      "");
}
