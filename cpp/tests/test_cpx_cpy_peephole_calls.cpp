// The calls that the tests of tests/test_cpx_cpy_peephole.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_txa_cmp_data_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_txa_cmp_data_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Data(\"g\" 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(X) Data(\"g\" 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_txa_cmp_imm_folds_to_cpx (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_txa_cmp_imm_folds_to_cpx\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(40))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(X) Imm(40))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_txa_cmp_zp_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_txa_cmp_zp_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) ZP(131 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(X) ZP(131 0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_tya_cmp_imm_folds_to_cpy (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyBasic::test_tya_cmp_imm_folds_to_cpy\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(Y) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(16))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(Y) Imm(16))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyRightOperand::test_frame_right_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyRightOperand::test_frame_right_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Frame(3))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Frame(3))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyRightOperand::test_indirect_right_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyRightOperand::test_indirect_right_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Indirect(0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Indirect(0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyRightOperand::test_stack_right_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyRightOperand::test_stack_right_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Stack(0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Stack(0))\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyLiveness::test_save_a_return_blocks_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyLiveness::test_save_a_return_blocks_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyLiveness::test_subsequent_a_kill_allows_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyLiveness::test_subsequent_a_kill_allows_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Mov(Imm(42) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(X) Imm(0))\n"
      "    Mov(Imm(42) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyLiveness::test_subsequent_a_read_blocks_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyLiveness::test_subsequent_a_read_blocks_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyCfg::test_branch_target_reads_a_blocks_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyCfg::test_branch_target_reads_a_blocks_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Branch(NE \"L\")\n"
      "    Return(false false false)\n"
      "    Label(\"L\")\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Branch(NE \"L\")\n"
      "    Return(false false false)\n"
      "    Label(\"L\")\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestCpxCpyCfg::test_branch_with_clean_targets_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestCpxCpyCfg::test_branch_with_clean_targets_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Compare(Reg(A) ZP(131 0))\n"
      "    Branch(NE \"L\")\n"
      "    Return(false false false)\n"
      "    Label(\"L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(X) ZP(131 0))\n"
      "    Branch(NE \"L\")\n"
      "    Return(false false false)\n"
      "    Label(\"L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestThroughTemporary::test_folds_even_for_a_volatile_operand (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestThroughTemporary::test_folds_even_for_a_volatile_operand\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"WNDWDTH\" 0) Reg(A) true)\n"
      "    Mov(Reg(A) Data(\"__local_f__0\" 0) false)\n"
      "    Compare(Reg(X) Data(\"__local_f__0\" 0))\n"
      "    Branch(CS \".l\")\n"
      "    Label(\".l\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Compare(Reg(X) Data(\"WNDWDTH\" 0))\n"
      "    Branch(CS \".l\")\n"
      "    Label(\".l\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("cpx_cpy: test_cpx_cpy_peephole.py::TestThroughTemporary::test_kept_when_a_is_read (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass cpx_cpy\n"
      "source tests/test_cpx_cpy_peephole.py::TestThroughTemporary::test_kept_when_a_is_read\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"WNDWDTH\" 0) Reg(A) true)\n"
      "    Mov(Reg(A) Data(\"__local_f__0\" 0) false)\n"
      "    Compare(Reg(X) Data(\"__local_f__0\" 0))\n"
      "    Branch(CS \".l\")\n"
      "    Mov(Reg(A) Data(\"q\" 0) false)\n"
      "    Label(\".l\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"WNDWDTH\" 0) Reg(A) true)\n"
      "    Mov(Reg(A) Data(\"__local_f__0\" 0) false)\n"
      "    Compare(Reg(X) Data(\"__local_f__0\" 0))\n"
      "    Branch(CS \".l\")\n"
      "    Mov(Reg(A) Data(\"q\" 0) false)\n"
      "    Label(\".l\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}
