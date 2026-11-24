// The calls that the tests of tests/test_tac_to_asm_zp_abi.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tac_to_asm: test_tac_to_asm_zp_abi.py::TestZpAbiCallSite::test_soft_stack_default_uses_allocate_stack (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm_zp_abi.py::TestZpAbiCallSite::test_soft_stack_default_uses_allocate_stack\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"callee\" [Constant(ConstInt(42))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    AllocateStack(2)\n"
      "    Mov(Imm(42) Stack(1) false)\n"
      "    Mov(Imm(0) Stack(2) false)\n"
      "    Call(\"callee\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 1) false)\n"
      "    Ret(0 0 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm_zp_abi.py::TestZpAbiCallSite::test_zp_layout_caller_emits_no_allocate_stack (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm_zp_abi.py::TestZpAbiCallSite::test_zp_layout_caller_emits_no_allocate_stack\n"
      "args {\"args\": [], \"kwargs\": {\"abi\": {\"dict\": [[\"callee\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_callee_p0\", \"__zpabi_callee_p1\"], \"addrs\": [128, 129], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"callee\" [Constant(ConstInt(42))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(42) Data(\"__zpabi_callee_p0\" 0) false)\n"
      "    Mov(Imm(0) Data(\"__zpabi_callee_p1\" 0) false)\n"
      "    Call(\"callee\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 1) false)\n"
      "    Ret(0 0 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tac_to_asm: test_tac_to_asm_zp_abi.py::TestZpAbiCallSite::test_zp_layout_two_args_use_consecutive_addrs (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tac_to_asm\n"
      "source tests/test_tac_to_asm_zp_abi.py::TestZpAbiCallSite::test_zp_layout_two_args_use_consecutive_addrs\n"
      "args {\"args\": [], \"kwargs\": {\"abi\": {\"dict\": [[\"callee\", {\"object\": \"passes.abi_selection.ZpLayout\", \"fields\": {\"slot_symbols\": [\"__zpabi_callee_p0\", \"__zpabi_callee_p1\", \"__zpabi_callee_p2\", \"__zpabi_callee_p3\"], \"addrs\": [128, 129, 130, 131], \"param_registers\": [], \"pinned\": []}}], [\"main\", {\"object\": \"passes.abi_selection.SoftStackLayout\", \"fields\": {}}]]}}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    FunctionCall(\"callee\" [Constant(ConstInt(1)) Constant(ConstInt(2))] None false false)\n"
      "    Ret(Constant(ConstInt(0)))\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(1) Data(\"__zpabi_callee_p0\" 0) false)\n"
      "    Mov(Imm(0) Data(\"__zpabi_callee_p1\" 0) false)\n"
      "    Mov(Imm(2) Data(\"__zpabi_callee_p2\" 0) false)\n"
      "    Mov(Imm(0) Data(\"__zpabi_callee_p3\" 0) false)\n"
      "    Call(\"callee\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"HARGS\" 1) false)\n"
      "    Ret(0 0 false false [] false)\n"
      "  ])\n"
      "])\n");
}
