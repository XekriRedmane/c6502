// The calls that the tests of tests/test_branch_through_jump.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("branch_through_jump: test_branch_through_jump.py::TestBranch::test_branch_retargeted_and_trampoline_dropped (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_through_jump\n"
      "source tests/test_branch_through_jump.py::TestBranch::test_branch_retargeted_and_trampoline_dropped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\"top\")\n"
      "    Branch(EQ \"tramp\")\n"
      "    Ret(0 0 false false [] false)\n"
      "    Label(\"tramp\")\n"
      "    Jump(\"top\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\"top\")\n"
      "    Branch(EQ \"top\")\n"
      "    Ret(0 0 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_through_jump: test_branch_through_jump.py::TestJump::test_chain_followed_to_its_end (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_through_jump\n"
      "source tests/test_branch_through_jump.py::TestJump::test_chain_followed_to_its_end\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"a\" [])\n"
      "    Label(\"end\")\n"
      "    Ret(0 0 false false [] false)\n"
      "    Label(\"a\")\n"
      "    Jump(\"b\" [])\n"
      "    Label(\"b\")\n"
      "    Jump(\"end\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"end\" [])\n"
      "    Label(\"end\")\n"
      "    Ret(0 0 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_through_jump: test_branch_through_jump.py::TestJump::test_cycle_left_alone (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_through_jump\n"
      "source tests/test_branch_through_jump.py::TestJump::test_cycle_left_alone\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"a\" [])\n"
      "    Label(\"a\")\n"
      "    Jump(\"b\" [])\n"
      "    Label(\"b\")\n"
      "    Jump(\"a\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"a\" [])\n"
      "    Label(\"a\")\n"
      "    Jump(\"b\" [])\n"
      "    Label(\"b\")\n"
      "    Jump(\"a\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_through_jump: test_branch_through_jump.py::TestJump::test_jump_retargeted_and_trampoline_dropped (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_through_jump\n"
      "source tests/test_branch_through_jump.py::TestJump::test_jump_retargeted_and_trampoline_dropped\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"if_end\" [])\n"
      "    Label(\"label\")\n"
      "    Ret(0 0 false false [] false)\n"
      "    Label(\"if_end\")\n"
      "    Jump(\"label\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"label\" [])\n"
      "    Label(\"label\")\n"
      "    Ret(0 0 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_through_jump: test_branch_through_jump.py::TestJump::test_tail_call_not_followed (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_through_jump\n"
      "source tests/test_branch_through_jump.py::TestJump::test_tail_call_not_followed\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"t\" [])\n"
      "    Label(\"u\")\n"
      "    Ret(0 0 false false [] false)\n"
      "    Label(\"t\")\n"
      "    Jump(\"tick\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"t\" [])\n"
      "    Label(\"u\")\n"
      "    Ret(0 0 false false [] false)\n"
      "    Label(\"t\")\n"
      "    Jump(\"tick\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("branch_through_jump: test_branch_through_jump.py::TestJump::test_trampoline_kept_when_fallen_into (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass branch_through_jump\n"
      "source tests/test_branch_through_jump.py::TestJump::test_trampoline_kept_when_fallen_into\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"mid\" [])\n"
      "    Label(\"end\")\n"
      "    Ret(0 0 false false [] false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Label(\"mid\")\n"
      "    Jump(\"end\" [])\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"end\" [])\n"
      "    Label(\"end\")\n"
      "    Ret(0 0 false false [] false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Label(\"mid\")\n"
      "    Jump(\"end\" [])\n"
      "  ])\n"
      "])\n");
}
