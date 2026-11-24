// The calls that the tests of tests/test_tail_call.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"

TEST_CASE("tail_call: test_tail_call.py::TestTailCall::test_call_followed_by_other_instr_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestTailCall::test_call_followed_by_other_instr_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"helper\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"helper\" [] [])\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestTailCall::test_call_then_ret_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestTailCall::test_call_then_ret_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"helper\" [] [])\n"
      "    Ret(2 4 false false [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"helper\" [] [])\n"
      "    Ret(2 4 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestTailCall::test_call_then_return_becomes_jump (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestTailCall::test_call_then_return_becomes_jump\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Call(\"apply_bobble\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Jump(\"apply_bobble\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestTailCall::test_call_then_return_save_a_true_also_folds (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestTailCall::test_call_then_return_save_a_true_also_folds\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"helper\" [] [])\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"helper\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestTailCall::test_multiple_call_return_pairs_all_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestTailCall::test_multiple_call_return_pairs_all_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".else_path\")\n"
      "    Call(\"path_a\" [] [])\n"
      "    Return(false false false)\n"
      "    Label(\".else_path\")\n"
      "    Call(\"path_b\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".else_path\")\n"
      "    Jump(\"path_a\" [])\n"
      "    Label(\".else_path\")\n"
      "    Jump(\"path_b\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestTailCall::test_return_alone_does_not_fold (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestTailCall::test_return_alone_does_not_fold\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestJumpToReturn::test_case_then_break (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestJumpToReturn::test_case_then_break\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"g\" [] [])\n"
      "    Jump(\".break\" [])\n"
      "    Label(\".other\")\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Label(\".break\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"g\" [])\n"
      "    Label(\".other\")\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Label(\".break\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestJumpToReturn::test_frame_teardown_not_copied (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestJumpToReturn::test_frame_teardown_not_copied\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\".end\" [])\n"
      "    Label(\".end\")\n"
      "    Ret(2 0 false false [] false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\".end\" [])\n"
      "    Label(\".end\")\n"
      "    Ret(2 0 false false [] false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("tail_call: test_tail_call.py::TestTailCall::test_other_top_level_passes_through (call 1)") {
  c6502::testing::check_record(
      "c6502-capture 2\n"
      "pass tail_call\n"
      "source tests/test_tail_call.py::TestTailCall::test_other_top_level_passes_through\n"
      "args {\"args\": [], \"kwargs\": {}}\n"
      "context {\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}\n"
      "input\n"
      "Program([\n"
      "  StaticVariable(\"g\" true [IntInit(0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Call(\"h\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n"
      "output\n"
      "Program([\n"
      "  StaticVariable(\"g\" true [IntInit(0)])\n"
      "  Function(\"f\" true [] [\n"
      "    Jump(\"h\" [])\n"
      "  ])\n"
      "])\n");
}
