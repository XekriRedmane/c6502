// The calls that the tests of tests/test_branch_to_next_drop.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(NE \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CC \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CS \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(MI \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(PL \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(VC \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_all_conditions_drop\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(VS \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_different_label_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_different_label_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".other\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".other\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_end_label_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_end_label_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_next_label_dropped (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_next_label_dropped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_non_adjacent_label_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_branch_to_non_adjacent_label_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_idempotent (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_idempotent\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CC \".L\")\n"
      "    Label(\".L\")\n"
      "    Branch(NE \".M\")\n"
      "    Label(\".M\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_idempotent (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_idempotent\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_label_preserved_for_other_refs (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_label_preserved_for_other_refs\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(MI \".L\")\n"
      "    Label(\".L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Jump(\".L\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Label(\".L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Jump(\".L\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_label_with_jump_target_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_label_with_jump_target_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Label(\".L\")\n"
      "    Jump(\".L\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Label(\".L\")\n"
      "    Jump(\".L\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_residual_sx_diamond_collapses (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_residual_sx_diamond_collapses\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Branch(PL \".sx_done@1\")\n"
      "    Label(\".sx_done@1\")\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "    Mov(Reg(X) Reg(A) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_stacked_branches_to_same_label (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_stacked_branches_to_same_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Branch(NE \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestBranchToNextDrop::test_stacked_branches_to_same_label (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestBranchToNextDrop::test_stacked_branches_to_same_label\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Label(\".L\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestJumpToNextDrop::test_jump_over_code_kept (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestJumpToNextDrop::test_jump_over_code_kept\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Jump(\".b\" [])\n"
      "    Return(false false false)\n"
      "    Label(\".b\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Jump(\".b\" [])\n"
      "    Return(false false false)\n"
      "    Label(\".b\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestJumpToNextDrop::test_jump_past_run_of_labels_dropped (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestJumpToNextDrop::test_jump_past_run_of_labels_dropped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Jump(\".b\" [])\n"
      "    Label(\".a\")\n"
      "    Label(\".b\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Label(\".a\")\n"
      "    Label(\".b\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_to_next_drop: test_branch_to_next_drop.py::TestJumpToNextDrop::test_jump_to_next_label_dropped (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_to_next_drop\n"
      "source tests/test_branch_to_next_drop.py::TestJumpToNextDrop::test_jump_to_next_label_dropped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Label(\".top\")\n"
      "    Branch(CC \".top\")\n"
      "    Jump(\".done\" [])\n"
      "    Label(\".done\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Label(\".top\")\n"
      "    Branch(CC \".top\")\n"
      "    Label(\".done\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}
