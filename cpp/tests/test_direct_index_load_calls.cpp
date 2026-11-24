// The calls that the tests of tests/test_direct_index_load.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_a_killed_before_pair_fold_still_works (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_a_killed_before_pair_fold_still_works\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(99) Reg(A) false)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(99) Reg(A) false)\n"
      "    Mov(ZP(128 0) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_a_live_after_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_a_live_after_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Reg(A) ZP(144 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Reg(A) ZP(144 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_branch_after_pair_with_clean_targets_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_branch_after_pair_with_clean_targets_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Branch(NE \"L\")\n"
      "    Label(\"L\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(X) false)\n"
      "    Branch(NE \"L\")\n"
      "    Label(\"L\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_branch_target_reads_a_blocks_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_branch_target_reads_a_blocks_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Branch(NE \"L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "    Label(\"L\")\n"
      "    Mov(Reg(A) ZP(144 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Branch(NE \"L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "    Label(\"L\")\n"
      "    Mov(Reg(A) ZP(144 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_call_kills_a_so_pair_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_call_kills_a_so_pair_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Call(\"foo\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(X) false)\n"
      "    Call(\"foo\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_data_to_y_with_dead_a_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_data_to_y_with_dead_a_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"x\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(Y) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"x\" 0) Reg(Y) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_first_mov_not_to_a_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_first_mov_not_to_a_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(X) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(X) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_frame_operand_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_frame_operand_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Frame(4) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Frame(4) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_imm_to_x_with_dead_a_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_imm_to_x_with_dead_a_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(5) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_indirect_operand_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_indirect_operand_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Indirect(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Indirect(0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_second_mov_to_a_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_second_mov_to_a_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("direct_index_load: test_direct_index_load.py::TestDirectIndexLoadUnit::test_zp_to_x_with_dead_a_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass direct_index_load\n"
      "source tests/test_direct_index_load.py::TestDirectIndexLoadUnit::test_zp_to_x_with_dead_a_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Reg(X) false)\n"
      "    Mov(Imm(66) Reg(A) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(X) false)\n"
      "    Mov(Imm(66) Reg(A) false)\n"
      "  ])\n"
      "])\n");
}
