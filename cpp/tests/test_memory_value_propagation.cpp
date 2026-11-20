// The calls that the unit tests of tests/test_memory_value_propagation.py make of
// `memory_value_propagation`, each with the program the test gives the pass, the
// call's arguments, the compile's context, and the program the
// Python pass returned, which the C++ pass must return too. They were
// recorded by wrapping the Python pass while its unit tests ran.
#include "doctest.h"
#include "pass_case.h"

using c6502::testing::check_pass_case;

TEST_CASE("memory_value_propagation: TestCallSummaries.test_call_that_never_returns_adds_nothing") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"OTHER\", 132]]}, \"noreturn\": {\"set\": [\"h\"]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Call(\"h\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"OPS\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Call(\"h\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCallSummaries.test_callee_calling_outside_the_program_kills_everything") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"OTHER\", 132]]}, \"noreturn\": {\"set\": []}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Call(\"h\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Call(\"h\" [] [])\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCallSummaries.test_callee_storing_through_a_pointer_kills_everything") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"OTHER\", 132]]}, \"noreturn\": {\"set\": []}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectZpY(132) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectZpY(132) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCallSummaries.test_callee_writing_other_bytes_keeps_the_fact") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"OTHER\", 132]]}, \"noreturn\": {\"set\": []}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OTHER\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"OPS\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OTHER\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCallSummaries.test_callee_writing_the_source_kills_it") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"OTHER\", 132]]}, \"noreturn\": {\"set\": []}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OTHER\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "  Function(\"g\" false [] [\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCopiedPointer.test_call_in_between_keeps_the_copy") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"RET\", 132], [\"__local_f__p\", 148], [\"__zpabi_g__q\", 150]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"RET\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"RET\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(IndirectZpY(148) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"RET\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"RET\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Call(\"g\" [] [])\n"
      "    Mov(IndirectZpY(148) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCopiedPointer.test_copy_of_another_functions_slot_left_alone") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"RET\", 132], [\"__local_f__p\", 148], [\"__zpabi_g__q\", 150]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_g__q\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"__zpabi_g__q\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Mov(IndirectZpY(148) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_g__q\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"__zpabi_g__q\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Mov(IndirectZpY(148) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCopiedPointer.test_read_through_copy_of_named_pointer") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"RET\", 132], [\"__local_f__p\", 148], [\"__zpabi_g__q\", 150]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"RET\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"RET\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Mov(IndirectZpY(148) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"RET\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"RET\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Mov(IndirectZpY(132) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCopiedPointer.test_store_through_a_pointer_keeps_the_copy") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"RET\", 132], [\"__local_f__p\", 148], [\"__zpabi_g__q\", 150]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"RET\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"RET\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Mov(Reg(A) IndirectZpY(150) false)\n"
      "    Mov(IndirectZpY(148) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"RET\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 0) false)\n"
      "    Mov(Data(\"RET\" 1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__p\" 1) false)\n"
      "    Mov(Reg(A) IndirectZpY(150) false)\n"
      "    Mov(IndirectZpY(148) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCrossBlockRewrite.test_use_after_label_rewrites") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Label(\".loop_start\")\n"
      "    Mov(ZP(16 0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Label(\".loop_start\")\n"
      "    Mov(ZP(16 0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectZpY(128) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestCrossBlockRewrite.test_use_on_loop_exit_path_rewrites") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(ZP(32 0) Reg(X) false)\n"
      "    Label(\".loop_start\")\n"
      "    Compare(Reg(X) Imm(0))\n"
      "    Branch(MI \".loop_end\")\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Branch(NE \".write_and_exit\")\n"
      "    Dec(Reg(X))\n"
      "    Jump(\".loop_start\" [])\n"
      "    Label(\".write_and_exit\")\n"
      "    Mov(ZP(33 0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "    Return(false false false)\n"
      "    Label(\".loop_end\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(ZP(32 0) Reg(X) false)\n"
      "    Label(\".loop_start\")\n"
      "    Compare(Reg(X) Imm(0))\n"
      "    Branch(MI \".loop_end\")\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Branch(NE \".write_and_exit\")\n"
      "    Dec(Reg(X))\n"
      "    Jump(\".loop_start\" [])\n"
      "    Label(\".write_and_exit\")\n"
      "    Mov(ZP(33 0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectZpY(128) false)\n"
      "    Return(false false false)\n"
      "    Label(\".loop_end\")\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestDearerReads.test_one_read_rewritten") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestDearerReads.test_read_that_stays_keeps_the_others") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Imm(5) Reg(X) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Imm(5) Reg(X) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestDearerReads.test_three_reads_left_alone") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(2) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(2) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestDearerReads.test_two_reads_rewritten") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestInvalidation.test_call_invalidates") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Call(\"callee\" [] [])\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Call(\"callee\" [] [])\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestInvalidation.test_volatile_source_not_read_again") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"KBD\" 0) ZP(128 0) true)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"KBD\" 0) ZP(128 0) true)\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestInvalidation.test_write_to_dptr_kills_equivalence") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(153) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(153) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestInvalidation.test_write_to_source_kills_equivalence") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(119) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(Imm(119) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(128 0) false)\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestMeetAcrossBranches.test_one_predecessor_doesnt_stage_kills_fact") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(16 0) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Branch(EQ \".other\")\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Jump(\".join\" [])\n"
      "    Label(\".other\")\n"
      "    Mov(Imm(85) Reg(A) false)\n"
      "    Label(\".join\")\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(16 0) Reg(A) false)\n"
      "    Compare(Reg(A) Imm(0))\n"
      "    Branch(EQ \".other\")\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Jump(\".join\" [])\n"
      "    Label(\".other\")\n"
      "    Mov(Imm(85) Reg(A) false)\n"
      "    Label(\".join\")\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestRepeatedInstructionObject.test_second_write_ends_the_fact") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Inc(Reg(X))\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Inc(Reg(X))\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Inc(Reg(X))\n"
      "    Mov(IndexedData(\"arr\" 0 X) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Inc(Reg(X))\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"OPS\" 0) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestSameBlockRewrite.test_indirecty_in_same_block_rewrites") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(ZP(16 0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectY false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(ZP(128 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 0) false)\n"
      "    Mov(ZP(129 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"DPTR\" 1) false)\n"
      "    Mov(ZP(16 0) Reg(A) false)\n"
      "    Mov(Reg(A) IndirectZpY(128) false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestSlotReadsFixedSource.test_copy_of_slot_left_alone") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_g__p\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"__zpabi_g__p\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestSlotReadsFixedSource.test_read_after_tail_call_function") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Jump(\"g\" [])\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"OPS\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Jump(\"g\" [])\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestSlotReadsFixedSource.test_read_of_fixed_source") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Mov(Imm(1) Reg(A) false)\n"
      "    ClearCarry\n"
      "    Add(Data(\"OPS\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"OPS\" 1) false)\n"
      "    Return(false false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestSlotReadsFixedSource.test_web_with_other_store_left_alone") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"OPS\", 130], [\"__local_f__t\", 144], [\"__zpabi_g__p\", 146]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".b\")\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Jump(\".join\" [])\n"
      "    Label(\".b\")\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Label(\".join\")\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".b\")\n"
      "    Mov(Data(\"OPS\" 0) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Add(Data(\"__local_f__t\" 0) Reg(A))\n"
      "    Jump(\".join\" [])\n"
      "    Label(\".b\")\n"
      "    Mov(Imm(5) Reg(A) false)\n"
      "    Mov(Reg(A) Data(\"__local_f__t\" 0) false)\n"
      "    Label(\".join\")\n"
      "    Mov(Data(\"__local_f__t\" 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestYSetupInvalidates.test_alu_setup_kills_fact") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"arr\", 64]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Add(IndirectZp(144 3) Reg(A))\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Add(IndirectZp(144 3) Reg(A))\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestYSetupInvalidates.test_compare_setup_kills_fact") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"arr\", 64]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Compare(Reg(A) IndirectZp(144 3))\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Compare(Reg(A) IndirectZp(144 3))\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestYSetupInvalidates.test_fact_used_without_setup") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {\"zp_symbol_addrs\": {\"dict\": [[\"arr\", 64]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    ClearCarry\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    ClearCarry\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestYSetupInvalidates.test_frame_setup_kills_fact") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Mov(Frame(2) Reg(A) false)\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Mov(Frame(2) Reg(A) false)\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("memory_value_propagation: TestYSetupInvalidates.test_indirect_zp_setup_kills_fact") {
  check_pass_case(
      "memory_value_propagation",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Mov(IndirectZp(144 0) Reg(A) false)\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(IndexedData(\"arr\" 0 Y) Reg(A) false)\n"
      "    Mov(Reg(A) ZP(132 0) false)\n"
      "    Mov(IndirectZp(144 0) Reg(A) false)\n"
      "    Mov(ZP(132 0) Reg(A) false)\n"
      "    Return(true false false)\n"
      "  ])\n"
      "])\n");
}
