// The calls that the unit tests of tests/test_asm_to_asm2.py make of
// passes/asm_to_asm2.py's `translate_program`, each with what the test gives the function
// and what the Python function returned or raised, which the C++
// port must return or throw too. They were recorded by wrapping the
// Python functions while their unit tests ran.
#include "doctest.h"
#include "call_case.h"

TEST_CASE("lower_to_asm2: TestAllocateStackExpansion.test_high_byte_set_when_amount_exceeds_byte (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    AllocateStack(256)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Sub(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Sub(Imm(1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestAllocateStackExpansion.test_one_byte_sub (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    AllocateStack(1)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    SetCarry\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Sub(Imm(1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Sub(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestAllocateStackExpansion.test_zero_emits_nothing (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    AllocateStack(0)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestFunctionPrologueExpansion.test_amt_one_emits_full_prologue (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(0 1 [])\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Comment(\"prologue: 0 arg bytes, 1 local bytes\")\n"
      "    SetCarry\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Sub(Imm(3) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Sub(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Stack(2))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Stack(3))\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Blank\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestFunctionPrologueExpansion.test_a_frame_access_past_the_ldy_reach_is_a_frame_size_error (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(4 250 [])\n"
      "    Mov(Frame(256) Reg(A) false)\n"
      "  ])\n"
      "])\n",
      "exception: FrameSizeError: function 'f': it reads or writes the byte at frame offset 256, but the `LDY #offset` of each frame access reaches at most 255 (frame offset 256 out of range 1..255); a large array fits better as a `static` or a global\n");
}

TEST_CASE("lower_to_asm2: TestFunctionPrologueExpansion.test_a_frame_access_past_the_ldy_reach_is_a_frame_size_error (call 2)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(4 250 [])\n"
      "    Mov(Frame(255) Reg(A) false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Comment(\"prologue: 4 arg bytes, 250 local bytes\")\n"
      "    SetCarry\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Sub(Imm(252) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Sub(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Stack(251))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Stack(252))\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Blank\n"
      "    Mov(Frame(255) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestFunctionPrologueExpansion.test_arguments_past_the_ldy_reach_that_arent_read_are_allowed (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(4 250 [])\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Comment(\"prologue: 4 arg bytes, 250 local bytes\")\n"
      "    SetCarry\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Sub(Imm(252) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Sub(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Stack(251))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Stack(252))\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Blank\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestFunctionPrologueExpansion.test_callee_saves_emit_extra_pairs (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(0 2 [192 193])\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Comment(\"prologue: 0 arg bytes, 2 local bytes, 2 callee-saved bytes\")\n"
      "    SetCarry\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Sub(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Sub(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Stack(3))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Stack(4))\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Mov(ZP(192 0) Reg(A))\n"
      "    Mov(Reg(A) Frame(1))\n"
      "    Mov(ZP(193 0) Reg(A))\n"
      "    Mov(Reg(A) Frame(2))\n"
      "    Blank\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestFunctionPrologueExpansion.test_callee_saves_emit_extra_pairs (call 2)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(0 2 [])\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Comment(\"prologue: 0 arg bytes, 2 local bytes\")\n"
      "    SetCarry\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Sub(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Sub(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Stack(3))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Stack(4))\n"
      "    Mov(Data(\"SSP\" 0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Mov(Data(\"SSP\" 1) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Blank\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestFunctionPrologueExpansion.test_zero_emits_nothing (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    FunctionPrologue(0 0 [])\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestPassthroughInstructions.test_branch_re_tags_condition (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".end@0\")\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Branch(EQ \".end@0\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestPassthroughInstructions.test_label_passes_through (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".start@0\")\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Label(\".start@0\")\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestPassthroughInstructions.test_load_address_stays_compound (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    LoadAddress(Frame(1) Frame(3))\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    LoadAddress(Frame(1) Frame(3))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestPassthroughInstructions.test_mov_imm_to_reg (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(66) Reg(A) false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Mov(Imm(66) Reg(A))\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestRetExpansion.test_callee_restores_emit_pairs (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(0 2 true false [192 193] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Blank\n"
      "    Comment(\"epilogue\")\n"
      "    Push(Reg(A))\n"
      "    Mov(Frame(1) Reg(A))\n"
      "    Mov(Reg(A) ZP(192 0))\n"
      "    Mov(Frame(2) Reg(A))\n"
      "    Mov(Reg(A) ZP(193 0))\n"
      "    ClearCarry\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Add(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Frame(3) Reg(A))\n"
      "    Mov(Reg(A) Reg(X))\n"
      "    Mov(Frame(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Mov(Reg(X) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Pop(Reg(A))\n"
      "    Return\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestRetExpansion.test_callee_restores_emit_pairs (call 2)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(0 2 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Blank\n"
      "    Comment(\"epilogue\")\n"
      "    Push(Reg(A))\n"
      "    ClearCarry\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Add(Imm(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Frame(3) Reg(A))\n"
      "    Mov(Reg(A) Reg(X))\n"
      "    Mov(Frame(4) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Mov(Reg(X) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Pop(Reg(A))\n"
      "    Return\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestRetExpansion.test_save_a_false_skips_push_pop (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(0 3 false false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Blank\n"
      "    Comment(\"epilogue\")\n"
      "    ClearCarry\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Add(Imm(5) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Frame(4) Reg(A))\n"
      "    Mov(Reg(A) Reg(X))\n"
      "    Mov(Frame(5) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Mov(Reg(X) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Return\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestRetExpansion.test_with_frame_save_a_true (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(0 3 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Blank\n"
      "    Comment(\"epilogue\")\n"
      "    Push(Reg(A))\n"
      "    ClearCarry\n"
      "    Mov(Data(\"FP\" 0) Reg(A))\n"
      "    Add(Imm(5) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 0))\n"
      "    Mov(Data(\"FP\" 1) Reg(A))\n"
      "    Add(Imm(0) Reg(A))\n"
      "    Mov(Reg(A) Data(\"SSP\" 1))\n"
      "    Mov(Frame(4) Reg(A))\n"
      "    Mov(Reg(A) Reg(X))\n"
      "    Mov(Frame(5) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 1))\n"
      "    Mov(Reg(X) Reg(A))\n"
      "    Mov(Reg(A) Data(\"FP\" 0))\n"
      "    Pop(Reg(A))\n"
      "    Return\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestRetExpansion.test_zero_dimensions_just_return (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Ret(0 0 true false [] false)\n"
      "  ])\n"
      "])\n",
      "Program([\n"
      "  Function(\"f\" true [] [\n"
      "    Return\n"
      "  ])\n"
      "])\n");
}

TEST_CASE("lower_to_asm2: TestStaticVariablesPassThrough.test_int_init_re_tags (call 1)") {
  c6502::testing::check_call(
      "lower_to_asm2",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"g\" true [IntInit(42)])\n"
      "])\n",
      "Program([\n"
      "  StaticVariable(\"g\" true [IntInit(42)])\n"
      "])\n");
}
