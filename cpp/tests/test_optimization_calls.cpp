// The calls that the tests of tests/test_optimization.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_constant_fold: test_optimization.py::TestPassesAcceptInputs::test_constant_folding_runs (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_constant_fold\n"
      "source tests/test_optimization.py::TestPassesAcceptInputs::test_constant_folding_runs\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n");
}

TEST_CASE("tac_copy_propagate: test_optimization.py::TestPassesAcceptInputs::test_copy_propagation_runs (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_copy_propagate\n"
      "source tests/test_optimization.py::TestPassesAcceptInputs::test_copy_propagation_runs\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n");
}

TEST_CASE("tac_eliminate_dead_stores: test_optimization.py::TestPassesAcceptInputs::test_dead_store_runs (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_eliminate_dead_stores\n"
      "source tests/test_optimization.py::TestPassesAcceptInputs::test_dead_store_runs\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null, \"ssa_dsts\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(1)) Var(\"x\"))\n"
      "  Binary(Add Var(\"x\") Constant(ConstInt(2)) Var(\"y\"))\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n");
}

TEST_CASE("tac_optimize_function: test_optimization.py::TestOptimizeFunction::test_terminates_on_empty_function (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_optimize_function\n"
      "source tests/test_optimization.py::TestOptimizeFunction::test_terminates_on_empty_function\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Ret(Constant(ConstInt(0)))\n"
      "])\n");
}

TEST_CASE("tac_optimize_function: test_optimization.py::TestOptimizeFunction::test_terminates_on_function_with_body (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_optimize_function\n"
      "source tests/test_optimization.py::TestOptimizeFunction::test_terminates_on_function_with_body\n"
      "args {\"args\": [], \"kwargs\": {\"symbols\": null}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"t0\"))\n"
      "  Ret(Var(\"t0\"))\n"
      "])\n"
      "output\n"
      "Function(\"main\" true [] [\n"
      "  Copy(Constant(ConstInt(7)) Var(\"t0\"))\n"
      "  Ret(Var(\"t0\"))\n"
      "])\n");
}

TEST_CASE("optimize_tac: test_optimization.py::TestOptimizeProgram::test_empty_program_passes_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass optimize_tac\n"
      "source tests/test_optimization.py::TestOptimizeProgram::test_empty_program_passes_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([])\n"
      "output\n"
      "Program([])\n");
}

TEST_CASE("optimize_tac: test_optimization.py::TestOptimizeProgram::test_function_and_static_round_trip (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass optimize_tac\n"
      "source tests/test_optimization.py::TestOptimizeProgram::test_function_and_static_round_trip\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" true Int [IntInit(5)])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" true Int [IntInit(5)])\n"
      "  Function(\"main\" true [] [\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("optimize_tac: test_optimization.py::TestOptimizeProgram::test_static_only_program_passes_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass optimize_tac\n"
      "source tests/test_optimization.py::TestOptimizeProgram::test_static_only_program_passes_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" true Long [LongInit(0)])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" true Long [LongInit(0)])\n"
      "])\n");
}
