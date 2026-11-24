// The calls that the tests of tests/test_branch_invert.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(NE \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(NE \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 3)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CC \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CS \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 4)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CS \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CC \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 5)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(MI \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(PL \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 6)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(PL \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(MI \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 7)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(VC \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(VS \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_all_conditions_invert (call 8)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_all_conditions_invert\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(VS \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(VC \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_branch_around_jump_collapses_to_inverted_branch (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_branch_around_jump_collapses_to_inverted_branch\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CC \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".other\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CS \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".other\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_idempotent (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_idempotent\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CC \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CS \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_idempotent (call 2)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_idempotent\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CS \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(CS \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_label_mismatch_doesnt_match (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_label_mismatch_doesnt_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".other\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".other\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_label_preserved_for_other_jump_targets (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_label_preserved_for_other_jump_targets\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Jump(\".L\" [])\n"
      "    Branch(EQ \".L\")\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Jump(\".L\" [])\n"
      "    Branch(NE \".T\")\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_non_adjacent_doesnt_match (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_non_adjacent_doesnt_match\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(EQ \".L\")\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Jump(\".T\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_invert: test_branch_invert.py::TestBranchInvert::test_tail_call_stays_a_jump (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_invert\n"
      "source tests/test_branch_invert.py::TestBranchInvert::test_tail_call_stays_a_jump\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(NE \".L\")\n"
      "    Jump(\"other\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" false [] [\n"
      "    Branch(NE \".L\")\n"
      "    Jump(\"other\" [])\n"
      "    Label(\".L\")\n"
      "    Label(\".T\")\n"
      "  ])\n"
      "])\n");
}
