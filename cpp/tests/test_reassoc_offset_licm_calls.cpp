// The calls that the tests of tests/test_reassoc_offset_licm.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_hoist_loop_invariants: test_reassoc_offset_licm.py::TestHoistLoopInvariants::test_add_not_moved (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_hoist_loop_invariants\n"
      "source tests/test_reassoc_offset_licm.py::TestHoistLoopInvariants::test_add_not_moved\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"c\", \"e\", \"s0\", \"s1\", \"s2\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Label(\"pre\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"s0\"))\n"
      "  Label(\"top\")\n"
      "  Phi(Var(\"s1\") [PhiArg(\"pre\" Var(\"s0\")) PhiArg(\"top\" Var(\"s2\"))])\n"
      "  Binary(Add Var(\"c\") Var(\"c\") Var(\"e\"))\n"
      "  Binary(Add Var(\"s1\") Var(\"e\") Var(\"s2\"))\n"
      "  JumpIfCmp(NotEqual Var(\"s2\") Constant(ConstInt(9)) \"top\")\n"
      "  Ret(Var(\"s2\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Label(\"pre\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"s0\"))\n"
      "  Label(\"top\")\n"
      "  Phi(Var(\"s1\") [PhiArg(\"pre\" Var(\"s0\")) PhiArg(\"top\" Var(\"s2\"))])\n"
      "  Binary(Add Var(\"c\") Var(\"c\") Var(\"e\"))\n"
      "  Binary(Add Var(\"s1\") Var(\"e\") Var(\"s2\"))\n"
      "  JumpIfCmp(NotEqual Var(\"s2\") Constant(ConstInt(9)) \"top\")\n"
      "  Ret(Var(\"s2\"))\n"
      "])\n");
}

TEST_CASE("tac_hoist_loop_invariants: test_reassoc_offset_licm.py::TestHoistLoopInvariants::test_extend_of_invariant_moves_to_preheader (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_hoist_loop_invariants\n"
      "source tests/test_reassoc_offset_licm.py::TestHoistLoopInvariants::test_extend_of_invariant_moves_to_preheader\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"c\", \"e\", \"s0\", \"s1\", \"s2\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Label(\"pre\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"s0\"))\n"
      "  Label(\"top\")\n"
      "  Phi(Var(\"s1\") [PhiArg(\"pre\" Var(\"s0\")) PhiArg(\"top\" Var(\"s2\"))])\n"
      "  ZeroExtend(Var(\"c\") Var(\"e\"))\n"
      "  Binary(Add Var(\"s1\") Var(\"e\") Var(\"s2\"))\n"
      "  JumpIfCmp(NotEqual Var(\"s2\") Constant(ConstInt(9)) \"top\")\n"
      "  Ret(Var(\"s2\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Label(\"pre\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"s0\"))\n"
      "  ZeroExtend(Var(\"c\") Var(\"e\"))\n"
      "  Label(\"top\")\n"
      "  Phi(Var(\"s1\") [PhiArg(\"pre\" Var(\"s0\")) PhiArg(\"top\" Var(\"s2\"))])\n"
      "  Binary(Add Var(\"s1\") Var(\"e\") Var(\"s2\"))\n"
      "  JumpIfCmp(NotEqual Var(\"s2\") Constant(ConstInt(9)) \"top\")\n"
      "  Ret(Var(\"s2\"))\n"
      "])\n");
}

TEST_CASE("tac_hoist_loop_invariants: test_reassoc_offset_licm.py::TestHoistLoopInvariants::test_extend_of_loop_value_stays (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_hoist_loop_invariants\n"
      "source tests/test_reassoc_offset_licm.py::TestHoistLoopInvariants::test_extend_of_loop_value_stays\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": {\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": []}}}, \"ssa_dsts\": {\"set\": [\"c\", \"e\", \"s0\", \"s1\", \"s2\"]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"f\" true [] [\n"
      "  Label(\"pre\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"s0\"))\n"
      "  Label(\"top\")\n"
      "  Phi(Var(\"s1\") [PhiArg(\"pre\" Var(\"s0\")) PhiArg(\"top\" Var(\"s2\"))])\n"
      "  ZeroExtend(Var(\"s1\") Var(\"e\"))\n"
      "  Binary(Add Var(\"s1\") Var(\"e\") Var(\"s2\"))\n"
      "  JumpIfCmp(NotEqual Var(\"s2\") Constant(ConstInt(9)) \"top\")\n"
      "  Ret(Var(\"s2\"))\n"
      "])\n"
      "output\n"
      "Function(\"f\" true [] [\n"
      "  Label(\"pre\")\n"
      "  Copy(Constant(ConstInt(0)) Var(\"s0\"))\n"
      "  Label(\"top\")\n"
      "  Phi(Var(\"s1\") [PhiArg(\"pre\" Var(\"s0\")) PhiArg(\"top\" Var(\"s2\"))])\n"
      "  ZeroExtend(Var(\"s1\") Var(\"e\"))\n"
      "  Binary(Add Var(\"s1\") Var(\"e\") Var(\"s2\"))\n"
      "  JumpIfCmp(NotEqual Var(\"s2\") Constant(ConstInt(9)) \"top\")\n"
      "  Ret(Var(\"s2\"))\n"
      "])\n");
}
