// The calls that the unit tests of tests/test_asm_emit.py make of
// asm_emit.py's `emit_program`, `emit_instruction`, `emit_function`, and `emit_static_variable`, each with what the test gives the function
// and what the Python function returned or raised, which the C++
// port must return or throw too. They were recorded by wrapping the
// Python functions while their unit tests ran.
#include "doctest.h"
#include "call_case.h"

TEST_CASE("emit_program: TestColumnAlignment.test_columns (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(42) Reg(A))\n"
      "    Return\n"
      "  ])\n"
      "])\n",
      "\"main:\\n   SUBROUTINE\\n\\n   LDA   #$2A\\n   RTS\\n\"\n");
}

TEST_CASE("emit_instruction: TestEmitAdd.test_dst_must_be_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(1) Reg(X))\n",
      {
        "exception: ValueError: Add dst must be Reg(A), got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_dst_must_be_a (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(1) Reg(Y))\n",
      {
        "exception: ValueError: Add dst must be Reg(A), got Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_dst_must_be_a (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(1) Stack(1))\n",
      {
        "exception: ValueError: Add dst must be Reg(A), got Stack(offset=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_dst_must_be_a (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(1) Frame(1))\n",
      {
        "exception: ValueError: Add dst must be Reg(A), got Frame(offset=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_dst_must_be_a (call 5)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(1) Imm(0))\n",
      {
        "exception: ValueError: Add dst must be Reg(A), got Imm(value=0)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_frame_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Frame(2) Reg(A))\n",
      {
        "   LDY   #$02",
        "   ADC   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_imm_out_of_range_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(-1) Reg(A))\n",
      {
        "exception: ValueError: immediate -1 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_imm_out_of_range_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(256) Reg(A))\n",
      {
        "exception: ValueError: immediate 256 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_imm_out_of_range_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(1000) Reg(A))\n",
      {
        "exception: ValueError: immediate 1000 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_imm_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Imm(42) Reg(A))\n",
      {
        "   ADC   #$2A",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_register_src_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Reg(X) Reg(A))\n",
      {
        "exception: ValueError: unsupported ADC source: Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_register_src_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Reg(Y) Reg(A))\n",
      {
        "exception: ValueError: unsupported ADC source: Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_register_src_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Reg(A) Reg(A))\n",
      {
        "exception: ValueError: unsupported ADC source: Reg(reg=A())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAdd.test_stack_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Stack(3) Reg(A))\n",
      {
        "   LDY   #$03",
        "   ADC   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitAnd.test_dst_must_be_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Imm(1) Reg(X))\n",
      {
        "exception: ValueError: And dst must be Reg(A), got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAnd.test_frame_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Frame(2) Reg(A))\n",
      {
        "   LDY   #$02",
        "   AND   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitAnd.test_imm_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Imm(15) Reg(A))\n",
      {
        "   AND   #$0F",
      });
}

TEST_CASE("emit_instruction: TestEmitAnd.test_register_src_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Reg(X) Reg(A))\n",
      {
        "exception: ValueError: unsupported AND source: Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAnd.test_register_src_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Reg(Y) Reg(A))\n",
      {
        "exception: ValueError: unsupported AND source: Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAnd.test_register_src_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Reg(A) Reg(A))\n",
      {
        "exception: ValueError: unsupported AND source: Reg(reg=A())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitAnd.test_stack_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Stack(3) Reg(A))\n",
      {
        "   LDY   #$03",
        "   AND   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(CC \"L_then\")\n",
      {
        "   BCC   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(CS \"L_then\")\n",
      {
        "   BCS   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(EQ \"L_then\")\n",
      {
        "   BEQ   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(MI \"L_then\")\n",
      {
        "   BMI   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 5)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(NE \"L_then\")\n",
      {
        "   BNE   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 6)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(PL \"L_then\")\n",
      {
        "   BPL   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 7)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(VC \"L_then\")\n",
      {
        "   BVC   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitBranch.test_each_condition (call 8)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Branch(VS \"L_then\")\n",
      {
        "   BVS   L_then",
      });
}

TEST_CASE("emit_instruction: TestEmitCall.test_call_arbitrary_name (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Call(\"my_fn\")\n",
      {
        "   JSR   my_fn",
      });
}

TEST_CASE("emit_instruction: TestEmitCall.test_call_divmod8 (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Call(\"divmod8\")\n",
      {
        "   JSR   divmod8",
      });
}

TEST_CASE("emit_instruction: TestEmitCall.test_call_mul8 (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Call(\"mul8\")\n",
      {
        "   JSR   mul8",
      });
}

TEST_CASE("emit_instruction: TestEmitClearSetCarry.test_clear_carry (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ClearCarry\n",
      {
        "   CLC",
      });
}

TEST_CASE("emit_instruction: TestEmitClearSetCarry.test_set_carry (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "SetCarry\n",
      {
        "   SEC",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_add_data_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(Data(\"g\" 0) Reg(A))\n",
      {
        "   ADC   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_and_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "And(Data(\"g\" 0) Reg(A))\n",
      {
        "   AND   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_bit_test_with_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "BitTest(Data(\"flags\" 0))\n",
      {
        "   BIT   flags",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_bit_test_with_data_offset (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "BitTest(Data(\"x\" 2))\n",
      {
        "   BIT   x+2",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_bit_test_with_frame_rejected (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "BitTest(Frame(3))\n",
      {
        "exception: ValueError: BitTest src must be Data or ZP (NMOS 6502 has no `BIT #imm` or indirect-Y BIT); got Frame(offset=3)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_bit_test_with_imm_rejected (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "BitTest(Imm(128))\n",
      {
        "exception: ValueError: BitTest src must be Data or ZP (NMOS 6502 has no `BIT #imm` or indirect-Y BIT); got Imm(value=128)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_bit_test_with_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "BitTest(ZP(128 0))\n",
      {
        "   BIT   $80",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_compare_a_with_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Compare(Reg(A) Data(\"g\" 0))\n",
      {
        "   CMP   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_compare_x_with_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Compare(Reg(X) Data(\"g\" 0))\n",
      {
        "   CPX   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_mov_a_to_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Data(\"g\" 0))\n",
      {
        "   STA   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_mov_data_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Data(\"g\" 0) Reg(A))\n",
      {
        "   LDA   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_mov_data_to_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Data(\"src\" 0) Data(\"dst\" 0))\n",
      {
        "   LDA   src",
        "   STA   dst",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_mov_data_to_frame (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Data(\"g\" 0) Frame(1))\n",
      {
        "   LDA   g",
        "   LDY   #$01",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_mov_frame_to_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Frame(1) Data(\"g\" 0))\n",
      {
        "   LDY   #$01",
        "   LDA   (FP),Y",
        "   STA   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_mov_imm_to_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(42) Data(\"g\" 0))\n",
      {
        "   LDA   #$2A",
        "   STA   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_or_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Data(\"g\" 0) Reg(A))\n",
      {
        "   ORA   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_sub_data_from_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Data(\"g\" 0) Reg(A))\n",
      {
        "   SBC   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDataOperand.test_xor_data (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(A) Data(\"g\" 0) Reg(A))\n",
      {
        "   EOR   g",
      });
}

TEST_CASE("emit_instruction: TestEmitDec.test_dec_a_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Dec(Reg(A))\n",
      {
        "exception: ValueError: Dec dst must be Reg(X), Reg(Y), Data, ZP, or IndexedData indexed by X, got Reg(reg=A())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitDec.test_dec_x (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Dec(Reg(X))\n",
      {
        "   DEX",
      });
}

TEST_CASE("emit_instruction: TestEmitDec.test_dec_y (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Dec(Reg(Y))\n",
      {
        "   DEY",
      });
}

TEST_CASE("emit_function: TestEmitFunction.test_comment_blank_section_markers (call 1)") {
  c6502::testing::check_lines(
      "emit_function",
      "Function(\"main\" true [] [\n"
      "  Comment(\"prologue: 0 arg bytes, 1 local bytes\")\n"
      "  Mov(Imm(7) Frame(1))\n"
      "  Blank\n"
      "  Comment(\"epilogue\")\n"
      "  Return\n"
      "])\n",
      {
        "main:",
        "   SUBROUTINE",
        "",
        "   ; prologue: 0 arg bytes, 1 local bytes",
        "   LDA   #$07",
        "   LDY   #$01",
        "   STA   (FP),Y",
        "",
        "   ; epilogue",
        "   RTS",
      });
}

TEST_CASE("emit_function: TestEmitFunction.test_consecutive_blanks_collapse (call 1)") {
  c6502::testing::check_lines(
      "emit_function",
      "Function(\"main\" true [] [\n"
      "  Blank\n"
      "  Blank\n"
      "  Return\n"
      "])\n",
      {
        "main:",
        "   SUBROUTINE",
        "",
        "   RTS",
      });
}

TEST_CASE("emit_function: TestEmitFunction.test_empty_instructions_label_and_subroutine_only (call 1)") {
  c6502::testing::check_lines(
      "emit_function",
      "Function(\"main\" true [] [])\n",
      {
        "main:",
        "   SUBROUTINE",
      });
}

TEST_CASE("emit_function: TestEmitFunction.test_label_subroutine_blank_then_instructions (call 1)") {
  c6502::testing::check_lines(
      "emit_function",
      "Function(\"main\" true [] [\n"
      "  Mov(Imm(0) Reg(A))\n"
      "  Return\n"
      "])\n",
      {
        "main:",
        "   SUBROUTINE",
        "",
        "   LDA   #$00",
        "   RTS",
      });
}

TEST_CASE("emit_function: TestEmitFunctionWithLabels.test_branch_then_label_layout (call 1)") {
  c6502::testing::check_lines(
      "emit_function",
      "Function(\"main\" true [] [\n"
      "  Branch(NE \"L_skip\")\n"
      "  Mov(Imm(1) Reg(A))\n"
      "  Label(\"L_skip\")\n"
      "  Return\n"
      "])\n",
      {
        "main:",
        "   SUBROUTINE",
        "",
        "   BNE   L_skip",
        "   LDA   #$01",
        "L_skip:",
        "   RTS",
      });
}

TEST_CASE("emit_function: TestEmitFunctionWithLabels.test_jump_back_to_top (call 1)") {
  c6502::testing::check_lines(
      "emit_function",
      "Function(\"main\" true [] [\n"
      "  Label(\"L_top\")\n"
      "  Jump(\"L_top\")\n"
      "])\n",
      {
        "main:",
        "   SUBROUTINE",
        "",
        "L_top:",
        "   JMP   L_top",
      });
}

TEST_CASE("emit_instruction: TestEmitInc.test_inc_a_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Inc(Reg(A))\n",
      {
        "exception: ValueError: Inc dst must be Reg(X), Reg(Y), Data, ZP, or IndexedData indexed by X, got Reg(reg=A())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitInc.test_inc_other_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Inc(Imm(1))\n",
      {
        "exception: ValueError: Inc dst must be Reg(X), Reg(Y), Data, ZP, or IndexedData indexed by X, got Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitInc.test_inc_other_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Inc(Stack(1))\n",
      {
        "exception: ValueError: Inc dst must be Reg(X), Reg(Y), Data, ZP, or IndexedData indexed by X, got Stack(offset=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitInc.test_inc_other_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Inc(Frame(1))\n",
      {
        "exception: ValueError: Inc dst must be Reg(X), Reg(Y), Data, ZP, or IndexedData indexed by X, got Frame(offset=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitInc.test_inc_x (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Inc(Reg(X))\n",
      {
        "   INX",
      });
}

TEST_CASE("emit_instruction: TestEmitInc.test_inc_y (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Inc(Reg(Y))\n",
      {
        "   INY",
      });
}

TEST_CASE("emit_instruction: TestEmitJump.test_jump_emits_jmp_with_target (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Jump(\"exit\")\n",
      {
        "   JMP   exit",
      });
}

TEST_CASE("emit_instruction: TestEmitLabel.test_label_arbitrary_name (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Label(\"L_then_42\")\n",
      {
        "L_then_42:",
      });
}

TEST_CASE("emit_instruction: TestEmitLabel.test_label_at_column_1 (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Label(\"loop\")\n",
      {
        "loop:",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_a_to_x_emits_tax (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Reg(X))\n",
      {
        "   TAX",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_a_to_y_emits_tay (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Reg(Y))\n",
      {
        "   TAY",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_out_of_range_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(-1) Reg(A))\n",
      {
        "exception: ValueError: immediate -1 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_out_of_range_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(256) Reg(A))\n",
      {
        "exception: ValueError: immediate 256 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_out_of_range_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(1000) Reg(A))\n",
      {
        "exception: ValueError: immediate 1000 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_out_of_range_raises (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(-100) Reg(A))\n",
      {
        "exception: ValueError: immediate -100 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_to_a_emits_lda (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(0) Reg(A))\n",
      {
        "   LDA   #$00",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_to_a_emits_lda (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(1) Reg(A))\n",
      {
        "   LDA   #$01",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_to_a_emits_lda (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(42) Reg(A))\n",
      {
        "   LDA   #$2A",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_to_a_emits_lda (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(255) Reg(A))\n",
      {
        "   LDA   #$FF",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_to_a_emits_lda (call 5)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(10) Reg(A))\n",
      {
        "   LDA   #$0A",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_to_x_emits_ldx (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(42) Reg(X))\n",
      {
        "   LDX   #$2A",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_imm_to_y_emits_ldy (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(42) Reg(Y))\n",
      {
        "   LDY   #$2A",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_self_mov_drops (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Reg(A))\n",
      {});
}

TEST_CASE("emit_instruction: TestEmitMov.test_self_mov_drops (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(X) Reg(X))\n",
      {});
}

TEST_CASE("emit_instruction: TestEmitMov.test_self_mov_drops (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(Y) Reg(Y))\n",
      {});
}

TEST_CASE("emit_instruction: TestEmitMov.test_self_mov_drops (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(130 0) ZP(130 0))\n",
      {});
}

TEST_CASE("emit_instruction: TestEmitMov.test_self_mov_drops (call 5)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Frame(1) Frame(1))\n",
      {});
}

TEST_CASE("emit_instruction: TestEmitMov.test_self_mov_drops (call 6)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Data(\"g\" 0) Data(\"g\" 0))\n",
      {});
}

TEST_CASE("emit_instruction: TestEmitMov.test_unsupported_mov_combinations_raise (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Stack(2) Reg(X))\n",
      {
        "exception: ValueError: cannot emit Mov(src=Stack(offset=2), dst=Reg(reg=X()))\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_unsupported_mov_combinations_raise (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Imm(0))\n",
      {
        "exception: ValueError: cannot emit Mov(src=Reg(reg=A()), dst=Imm(value=0))\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_x_to_a_emits_txa (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(X) Reg(A))\n",
      {
        "   TXA",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_x_y_cross_transfers_via_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(X) Reg(Y))\n",
      {
        "   TXA",
        "   TAY",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_x_y_cross_transfers_via_a (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(Y) Reg(X))\n",
      {
        "   TYA",
        "   TAX",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_x_y_to_stack_via_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(X) Stack(2))\n",
      {
        "   TXA",
        "   LDY   #$02",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMov.test_y_to_a_emits_tya (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(Y) Reg(A))\n",
      {
        "   TYA",
      });
}

TEST_CASE("emit_instruction: TestEmitMovFrame.test_a_to_frame (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Frame(7))\n",
      {
        "   LDY   #$07",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovFrame.test_frame_offset_out_of_range_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Frame(-1))\n",
      {
        "exception: ValueError: offset -1 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMovFrame.test_frame_offset_out_of_range_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Frame(256))\n",
      {
        "exception: ValueError: offset 256 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMovFrame.test_frame_offset_out_of_range_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Frame(1000))\n",
      {
        "exception: ValueError: offset 1000 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMovFrame.test_frame_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Frame(5) Reg(A))\n",
      {
        "   LDY   #$05",
        "   LDA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovFrame.test_frame_to_frame (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Frame(1) Frame(4))\n",
      {
        "   LDY   #$01",
        "   LDA   (FP),Y",
        "   LDY   #$04",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovFrame.test_imm_to_frame (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(42) Frame(3))\n",
      {
        "   LDA   #$2A",
        "   LDY   #$03",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovMixed.test_frame_to_stack (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Frame(2) Stack(3))\n",
      {
        "   LDY   #$02",
        "   LDA   (FP),Y",
        "   LDY   #$03",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovMixed.test_stack_to_frame (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Stack(2) Frame(3))\n",
      {
        "   LDY   #$02",
        "   LDA   (SSP),Y",
        "   LDY   #$03",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovStack.test_a_to_stack (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Stack(7))\n",
      {
        "   LDY   #$07",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovStack.test_imm_to_stack (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(42) Stack(3))\n",
      {
        "   LDA   #$2A",
        "   LDY   #$03",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovStack.test_stack_offset_out_of_range_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Stack(-1))\n",
      {
        "exception: ValueError: offset -1 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMovStack.test_stack_offset_out_of_range_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Stack(256))\n",
      {
        "exception: ValueError: offset 256 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMovStack.test_stack_offset_out_of_range_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) Stack(1000))\n",
      {
        "exception: ValueError: offset 1000 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitMovStack.test_stack_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Stack(5) Reg(A))\n",
      {
        "   LDY   #$05",
        "   LDA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitMovStack.test_stack_to_stack (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Stack(1) Stack(4))\n",
      {
        "   LDY   #$01",
        "   LDA   (SSP),Y",
        "   LDY   #$04",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitOr.test_dst_must_be_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Imm(1) Reg(X))\n",
      {
        "exception: ValueError: Or dst must be Reg(A), got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitOr.test_frame_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Frame(2) Reg(A))\n",
      {
        "   LDY   #$02",
        "   ORA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitOr.test_imm_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Imm(240) Reg(A))\n",
      {
        "   ORA   #$F0",
      });
}

TEST_CASE("emit_instruction: TestEmitOr.test_register_src_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Reg(X) Reg(A))\n",
      {
        "exception: ValueError: unsupported ORA source: Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitOr.test_register_src_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Reg(Y) Reg(A))\n",
      {
        "exception: ValueError: unsupported ORA source: Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitOr.test_register_src_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Reg(A) Reg(A))\n",
      {
        "exception: ValueError: unsupported ORA source: Reg(reg=A())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitOr.test_stack_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Or(Stack(3) Reg(A))\n",
      {
        "   LDY   #$03",
        "   ORA   (SSP),Y",
      });
}

TEST_CASE("emit_program: TestEmitProgram.test_empty_program_emits_just_a_newline (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([])\n",
      "\"\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_full (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(42) Reg(A))\n"
      "    Return\n"
      "  ])\n"
      "])\n",
      "\"main:\\n   SUBROUTINE\\n\\n   LDA   #$2A\\n   RTS\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_function_then_static_variable (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"main\" true [] [\n"
      "    Mov(Imm(42) Reg(A))\n"
      "    Return\n"
      "  ])\n"
      "  StaticVariable(\"g\" false [CharInit(7)])\n"
      "])\n",
      "\"main:\\n   SUBROUTINE\\n\\n   LDA   #$2A\\n   RTS\\n\\ng:\\n   DC.B  $07\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_multi_function (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"foo\" true [] [\n"
      "    Mov(Imm(1) Reg(A))\n"
      "    Return\n"
      "  ])\n"
      "  Function(\"main\" true [] [\n"
      "    Call(\"foo\")\n"
      "    Return\n"
      "  ])\n"
      "])\n",
      "\"foo:\\n   SUBROUTINE\\n\\n   LDA   #$01\\n   RTS\\n\\nmain:\\n   SUBROUTINE\\n\\n   JSR   foo\\n   RTS\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_array_renders_per_element (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"a\" false [CharInit(1) CharInit(2) CharInit(3)])\n"
      "])\n",
      "\"a:\\n   DC.B  $01\\n   DC.B  $02\\n   DC.B  $03\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_double_pi_byte_pattern (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"pi\" false [DoubleInit(4614253070214989087)])\n"
      "])\n",
      "\"pi:\\n   DC.L  $51EB851F\\n   DC.L  $40091EB8\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_double_renders_two_dc_ls (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"half_d\" false [DoubleInit(4602678819172646912)])\n"
      "])\n",
      "\"half_d:\\n   DC.L  $00000000\\n   DC.L  $3FE00000\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_float_renders_dc_l (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"half\" false [FloatInit(1056964608)])\n"
      "])\n",
      "\"half:\\n   DC.L  $3F000000\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_int_array_renders_per_element_dc_w (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"a\" false [IntInit(1) IntInit(2) IntInit(3)])\n"
      "])\n",
      "\"a:\\n   DC.W  $0001\\n   DC.W  $0002\\n   DC.W  $0003\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_long_init_range_check (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"bad\" false [LongInit(4294967296)])\n"
      "])\n",
      "exception: ValueError: init for 'bad' 4294967296 out of range for 32-bit (-2147483648..4294967295)\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_long_renders_dc_l (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"g\" true [LongInit(1234567890)])\n"
      "])\n",
      "\"g:\\n   DC.L  $499602D2\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_long_renders_negative_as_twos_complement (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"g\" false [LongInit(-1)])\n"
      "])\n",
      "\"g:\\n   DC.L  $FFFFFFFF\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_string_init_bytes_lt_len_raises (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"s\" false [StringInit(\"abcdef\" 3)])\n"
      "])\n",
      "exception: ValueError: StringInit for 's': bytes=3 less than string length 6\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_string_init_extra_padding (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"s\" false [StringInit(\"hi\" 5)])\n"
      "])\n",
      "\"s:\\n   DC.B  $68, $69, $00, $00, $00\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_string_init_no_terminator_when_bytes_equals_len (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"s\" false [StringInit(\"abc\" 3)])\n"
      "])\n",
      "\"s:\\n   DC.B  $61, $62, $63\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_string_init_renders_dc_b_bytes (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"s\" false [StringInit(\"abc\" 4)])\n"
      "])\n",
      "\"s:\\n   DC.B  $61, $62, $63, $00\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_variable_init_byte_range_check (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"bad\" false [CharInit(256)])\n"
      "])\n",
      "exception: ValueError: init for 'bad' 256 out of range for 1-byte (-128..255)\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_variable_renders_label_and_dc_b (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"g\" false [CharInit(5)])\n"
      "])\n",
      "\"g:\\n   DC.B  $05\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_zero_init_byte_count_must_be_positive (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"a\" false [ZeroInit(0)])\n"
      "])\n",
      "exception: ValueError: ZeroInit byte count must be positive, got 0\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_static_zero_init_renders_ds_b (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"a\" false [CharInit(1) ZeroInit(4)])\n"
      "])\n",
      "\"a:\\n   DC.B  $01\\n   DS.B  4\\n\"\n");
}

TEST_CASE("emit_program: TestEmitProgram.test_zero_initialized_static_variable (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  StaticVariable(\"x\" true [CharInit(0)])\n"
      "])\n",
      "\"x:\\n   DC.B  $00\\n\"\n");
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_pop_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Pop(Reg(A))\n",
      {
        "   PLA",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_pop_non_a_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Pop(Reg(X))\n",
      {
        "exception: ValueError: Pop dst must be Reg(A), got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_pop_non_a_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Pop(Reg(Y))\n",
      {
        "exception: ValueError: Pop dst must be Reg(A), got Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_pop_non_a_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Pop(Stack(1))\n",
      {
        "exception: ValueError: Pop dst must be Reg(A), got Stack(offset=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_push_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Push(Reg(A))\n",
      {
        "   PHA",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_push_non_a_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Push(Reg(X))\n",
      {
        "exception: ValueError: Push src must be Reg(A), got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_push_non_a_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Push(Reg(Y))\n",
      {
        "exception: ValueError: Push src must be Reg(A), got Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_push_non_a_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Push(Imm(1))\n",
      {
        "exception: ValueError: Push src must be Reg(A), got Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitPushPop.test_push_non_a_raises (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Push(Stack(1))\n",
      {
        "exception: ValueError: Push src must be Reg(A), got Stack(offset=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_acc_emit (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ArithmeticShiftLeft(Reg(A))\n",
      {
        "   ASL",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_acc_emit (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "LogicalShiftRight(Reg(A))\n",
      {
        "   LSR",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_acc_emit (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateLeft(Reg(A))\n",
      {
        "   ROL",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_acc_emit (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateRight(Reg(A))\n",
      {
        "   ROR",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_frame_synthesis (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ArithmeticShiftLeft(Frame(2))\n",
      {
        "   LDY   #$02",
        "   LDA   (FP),Y",
        "   ASL",
        "   LDY   #$02",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_frame_synthesis (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "LogicalShiftRight(Frame(2))\n",
      {
        "   LDY   #$02",
        "   LDA   (FP),Y",
        "   LSR",
        "   LDY   #$02",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_frame_synthesis (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateLeft(Frame(2))\n",
      {
        "   LDY   #$02",
        "   LDA   (FP),Y",
        "   ROL",
        "   LDY   #$02",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_frame_synthesis (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateRight(Frame(2))\n",
      {
        "   LDY   #$02",
        "   LDA   (FP),Y",
        "   ROR",
        "   LDY   #$02",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_stack_synthesis (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ArithmeticShiftLeft(Stack(3))\n",
      {
        "   LDY   #$03",
        "   LDA   (SSP),Y",
        "   ASL",
        "   LDY   #$03",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_stack_synthesis (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "LogicalShiftRight(Stack(3))\n",
      {
        "   LDY   #$03",
        "   LDA   (SSP),Y",
        "   LSR",
        "   LDY   #$03",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_stack_synthesis (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateLeft(Stack(3))\n",
      {
        "   LDY   #$03",
        "   LDA   (SSP),Y",
        "   ROL",
        "   LDY   #$03",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_stack_synthesis (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateRight(Stack(3))\n",
      {
        "   LDY   #$03",
        "   LDA   (SSP),Y",
        "   ROR",
        "   LDY   #$03",
        "   STA   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ArithmeticShiftLeft(Reg(X))\n",
      {
        "exception: ValueError: ArithmeticShiftLeft dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ArithmeticShiftLeft(Reg(Y))\n",
      {
        "exception: ValueError: ArithmeticShiftLeft dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ArithmeticShiftLeft(Imm(1))\n",
      {
        "exception: ValueError: ArithmeticShiftLeft dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "LogicalShiftRight(Reg(X))\n",
      {
        "exception: ValueError: LogicalShiftRight dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 5)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "LogicalShiftRight(Reg(Y))\n",
      {
        "exception: ValueError: LogicalShiftRight dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 6)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "LogicalShiftRight(Imm(1))\n",
      {
        "exception: ValueError: LogicalShiftRight dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 7)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateLeft(Reg(X))\n",
      {
        "exception: ValueError: RotateLeft dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 8)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateLeft(Reg(Y))\n",
      {
        "exception: ValueError: RotateLeft dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 9)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateLeft(Imm(1))\n",
      {
        "exception: ValueError: RotateLeft dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 10)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateRight(Reg(X))\n",
      {
        "exception: ValueError: RotateRight dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 11)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateRight(Reg(Y))\n",
      {
        "exception: ValueError: RotateRight dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitShiftRotateAcc.test_unsupported_dst_raises (call 12)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "RotateRight(Imm(1))\n",
      {
        "exception: ValueError: RotateRight dst must be Reg(A), Data, ZP, IndexedData indexed by X, Stack, or Frame, got Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitSub.test_dst_must_be_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Imm(1) Reg(X))\n",
      {
        "exception: ValueError: Sub dst must be Reg(A), got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitSub.test_frame_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Frame(2) Reg(A))\n",
      {
        "   LDY   #$02",
        "   SBC   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitSub.test_imm_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Imm(42) Reg(A))\n",
      {
        "   SBC   #$2A",
      });
}

TEST_CASE("emit_instruction: TestEmitSub.test_register_src_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Reg(X) Reg(A))\n",
      {
        "exception: ValueError: unsupported SBC source: Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitSub.test_register_src_raises (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Reg(Y) Reg(A))\n",
      {
        "exception: ValueError: unsupported SBC source: Reg(reg=Y())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitSub.test_register_src_raises (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Reg(A) Reg(A))\n",
      {
        "exception: ValueError: unsupported SBC source: Reg(reg=A())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitSub.test_stack_to_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(Stack(3) Reg(A))\n",
      {
        "   LDY   #$03",
        "   SBC   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_a_frame_either_order (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(A) Frame(2) Reg(A))\n",
      {
        "   LDY   #$02",
        "   EOR   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_a_frame_either_order (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Frame(2) Reg(A) Reg(A))\n",
      {
        "   LDY   #$02",
        "   EOR   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_a_imm (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(A) Imm(255) Reg(A))\n",
      {
        "   EOR   #$FF",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_a_stack (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(A) Stack(3) Reg(A))\n",
      {
        "   LDY   #$03",
        "   EOR   (SSP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_dst_must_be_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(A) Imm(1) Reg(X))\n",
      {
        "exception: ValueError: Xor dst must be Reg(A), got Reg(reg=X())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_imm_a_other_order (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Imm(10) Reg(A) Reg(A))\n",
      {
        "   EOR   #$0A",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_imm_out_of_range_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(A) Imm(256) Reg(A))\n",
      {
        "exception: ValueError: immediate 256 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_one_src_must_be_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Imm(1) Imm(2) Reg(A))\n",
      {
        "exception: ValueError: Xor srcs must include Reg(A); got src1=Imm(value=1), src2=Imm(value=2)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_one_src_must_be_a (call 2)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(A) Reg(A) Reg(A))\n",
      {
        "exception: ValueError: unsupported EOR source: Reg(reg=A())\n",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_one_src_must_be_a (call 3)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Reg(X) Imm(1) Reg(A))\n",
      {
        "exception: ValueError: Xor srcs must include Reg(A); got src1=Reg(reg=X()), src2=Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitXor.test_one_src_must_be_a (call 4)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Xor(Stack(1) Imm(1) Reg(A))\n",
      {
        "exception: ValueError: Xor srcs must include Reg(A); got src1=Stack(offset=1), src2=Imm(value=1)\n",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_add_zp_source (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Add(ZP(130 0) Reg(A))\n",
      {
        "   ADC   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_asl_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "ArithmeticShiftLeft(ZP(130 0))\n",
      {
        "   ASL   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_compare_a_with_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Compare(Reg(A) ZP(130 0))\n",
      {
        "   CMP   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_compare_x_with_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Compare(Reg(X) ZP(130 0))\n",
      {
        "   CPX   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_dec_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Dec(ZP(130 0))\n",
      {
        "   DEC   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_inc_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Inc(ZP(130 0))\n",
      {
        "   INC   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_a_to_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Reg(A) ZP(130 0))\n",
      {
        "   STA   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_imm_to_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(Imm(7) ZP(130 0))\n",
      {
        "   LDA   #$07",
        "   STA   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_zp_high_byte_offset_folds (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(130 1) Reg(A))\n",
      {
        "   LDA   $83",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_zp_to_frame (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(130 0) Frame(1))\n",
      {
        "   LDA   $82",
        "   LDY   #$01",
        "   STA   (FP),Y",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_zp_to_reg_a (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(130 0) Reg(A))\n",
      {
        "   LDA   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_zp_to_reg_x (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(130 0) Reg(X))\n",
      {
        "   LDX   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_zp_to_reg_y (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(130 0) Reg(Y))\n",
      {
        "   LDY   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_mov_zp_to_zp (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(130 0) ZP(144 0))\n",
      {
        "   LDA   $82",
        "   STA   $90",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_sub_zp_source (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Sub(ZP(130 0) Reg(A))\n",
      {
        "   SBC   $82",
      });
}

TEST_CASE("emit_instruction: TestEmitZPOperand.test_zp_address_out_of_range_raises (call 1)") {
  c6502::testing::check_lines(
      "emit_instruction",
      "Mov(ZP(256 0) Reg(A))\n",
      {
        "exception: ValueError: ZP address 256 out of range for 6502 (expected 0..255)\n",
      });
}

TEST_CASE("emit_program: TestIndirectZpSymbolSelection.test_name_that_isnt_a_slot (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {\"zp_slot_symbols\": {\"dict\": [[\"__local_other_fn__0\", 144], [\"zp_ptr\", 144]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"some_fn\" true [] [\n"
      "    Mov(IndirectZpY(144) Reg(A))\n"
      "  ])\n"
      "])\n",
      "\"__local_other_fn__0\\tEQU\\t$90\\nzp_ptr\\tEQU\\t$90\\n\\nsome_fn:\\n   SUBROUTINE\\n\\n   LDA   (zp_ptr),Y\\n\"\n");
}

TEST_CASE("emit_program: TestIndirectZpSymbolSelection.test_other_functions_slot_is_never_named (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {\"zp_slot_symbols\": {\"dict\": [[\"__local_other_fn__0\", 144]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"some_fn\" true [] [\n"
      "    Mov(Reg(A) IndirectZpY(144))\n"
      "  ])\n"
      "])\n",
      "\"__local_other_fn__0\\tEQU\\t$90\\n\\nsome_fn:\\n   SUBROUTINE\\n\\n   STA   ($90),Y\\n\"\n");
}

TEST_CASE("emit_program: TestIndirectZpSymbolSelection.test_prefers_current_functions_local_slot (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {\"zp_slot_symbols\": {\"dict\": [[\"__local_other_fn__0\", 136], [\"__local_player_catch__1\", 136]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"player_catch\" true [] [\n"
      "    Mov(Reg(A) IndirectZpY(136))\n"
      "  ])\n"
      "])\n",
      "\"__local_other_fn__0\\tEQU\\t$88\\n__local_player_catch__1\\tEQU\\t$88\\n\\nplayer_catch:\\n   SUBROUTINE\\n\\n   STA   (__local_player_catch__1),Y\\n\"\n");
}

TEST_CASE("emit_program: TestIndirectZpSymbolSelection.test_prefers_current_functions_zpabi_slot (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {\"zp_slot_symbols\": {\"dict\": [[\"__zpabi_find_active_entity__out_row_0\", 139], [\"__local_player_catch__1\", 139]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"find_active_entity\" true [] [\n"
      "    Mov(Reg(A) IndirectZpY(139))\n"
      "  ])\n"
      "])\n",
      "\"__local_player_catch__1\\tEQU\\t$8B\\n__zpabi_find_active_entity__out_row_0\\tEQU\\t$8B\\n\\nfind_active_entity:\\n   SUBROUTINE\\n\\n   STA   (__zpabi_find_active_entity__out_row_0),Y\\n\"\n");
}

TEST_CASE("emit_program: TestIndirectZpSymbolSelection.test_slot_the_function_names_itself (call 1)") {
  c6502::testing::check_call(
      "emit_program",
      "{\"args\": [], \"kwargs\": {\"zp_slot_symbols\": {\"dict\": [[\"__local_other_fn__0\", 144], [\"__zpabi_callee__p_0\", 144]]}}}",
      "{\"layout\":[0,2,4,32,36],\"user_zp_globals\":[],\"user_zp_bytes\":[]}",
      false,
      "Program([\n"
      "  Function(\"caller\" true [] [\n"
      "    Mov(Reg(A) Data(\"__zpabi_callee__p_0\" 0))\n"
      "    Mov(IndirectZpY(144) Reg(A))\n"
      "  ])\n"
      "])\n",
      "\"__local_other_fn__0\\tEQU\\t$90\\n__zpabi_callee__p_0\\tEQU\\t$90\\n\\ncaller:\\n   SUBROUTINE\\n\\n   STA   __zpabi_callee__p_0\\n   LDA   (__zpabi_callee__p_0),Y\\n\"\n");
}
