// The calls that the tests of tests/test_tac_to_asm.py made of the asm passes,
// code generation's stage calls, and emission, each with what the
// Python function returned or raised, which the C++ port must
// return or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed. A call
// that another C++ case already checks isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "translator_case.h"

TEST_CASE("translate_val: test_tac_to_asm.py::TestTranslateVal::test_constant_becomes_imm (call 1)") {
  c6502::testing::check_translator(
      "translate_val",
      {
        "Constant(ConstInt(42))\n"
      },
      {
        "Imm(42)\n"
      });
}

TEST_CASE("translate_val: test_tac_to_asm.py::TestTranslateVal::test_var_becomes_pseudo (call 1)") {
  c6502::testing::check_translator(
      "translate_val",
      {
        "Var(\"%0\")\n"
      },
      {
        "Pseudo(\"%0\" 0)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_copy_constant_to_var_becomes_single_mov (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Copy(Constant(ConstChar(0)) Var(\"%0\"))\n"
      },
      {
        "Mov(Imm(0) Pseudo(\"%0\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_copy_var_to_var_becomes_single_mov (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Copy(Var(\"%a\") Var(\"%b\"))\n"
      },
      {
        "Mov(Pseudo(\"%a\" 0) Pseudo(\"%b\" 0) false)\n"
      });
}

TEST_CASE("translate_function: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_full_logical_and_lowering (call 1)") {
  c6502::testing::check_translator(
      "translate_function",
      {
        "Function(\"main\" true [] [\n"
        "  JumpIfFalse(Constant(ConstChar(1)) \".and_false@0\")\n"
        "  JumpIfFalse(Constant(ConstChar(2)) \".and_false@0\")\n"
        "  Copy(Constant(ConstChar(1)) Var(\"%0\"))\n"
        "  Jump(\".and_end@1\")\n"
        "  Label(\".and_false@0\")\n"
        "  Copy(Constant(ConstChar(0)) Var(\"%0\"))\n"
        "  Label(\".and_end@1\")\n"
        "  Ret(Var(\"%0\"))\n"
        "])\n"
      },
      {
        "Function(\"main\" true [] [\n"
        "  Mov(Imm(1) Reg(A) false)\n"
        "  Branch(EQ \".and_false@0\")\n"
        "  Mov(Imm(2) Reg(A) false)\n"
        "  Branch(EQ \".and_false@0\")\n"
        "  Mov(Imm(1) Pseudo(\"%0\" 0) false)\n"
        "  Jump(\".and_end@1\" [])\n"
        "  Label(\".and_false@0\")\n"
        "  Mov(Imm(0) Pseudo(\"%0\" 0) false)\n"
        "  Label(\".and_end@1\")\n"
        "  Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
        "  Ret(0 0 true false [] false)\n"
        "])\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_jump_if_false_constant_stages_through_a_then_beq (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "JumpIfFalse(Constant(ConstChar(0)) \".and_false@0\")\n"
      },
      {
        "Mov(Imm(0) Reg(A) false)\n",
        "Branch(EQ \".and_false@0\")\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_jump_if_false_var_stages_through_a_then_beq (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "JumpIfFalse(Var(\"%2\") \".and_false@0\")\n"
      },
      {
        "Mov(Pseudo(\"%2\" 0) Reg(A) false)\n",
        "Branch(EQ \".and_false@0\")\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_jump_if_true_constant_stages_through_a_then_bne (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "JumpIfTrue(Constant(ConstChar(1)) \".or_true@0\")\n"
      },
      {
        "Mov(Imm(1) Reg(A) false)\n",
        "Branch(NE \".or_true@0\")\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_jump_if_true_var_stages_through_a_then_bne (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "JumpIfTrue(Var(\"%0\") \".or_true@0\")\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "Branch(NE \".or_true@0\")\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_jump_is_atom_for_atom (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Jump(\".and_end@0\")\n"
      },
      {
        "Jump(\".and_end@0\" [])\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateShortCircuitAtoms::test_label_is_atom_for_atom (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Label(\".or_true@3\")\n"
      },
      {
        "Label(\".or_true@3\")\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateComparisons::test_equal_uses_compare_and_beq (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(Equal Var(\"%0\") Constant(ConstInt(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "Compare(Reg(A) Imm(5))\n",
        "Branch(EQ \".cmp_true@0\")\n",
        "Mov(Imm(0) Reg(A) false)\n",
        "Jump(\".cmp_end@1\" [])\n",
        "Label(\".cmp_true@0\")\n",
        "Mov(Imm(1) Reg(A) false)\n",
        "Label(\".cmp_end@1\")\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateComparisons::test_greater_or_equal_uses_sbc_and_bpl_no_swap (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(GreaterOrEqual Var(\"%0\") Constant(ConstInt(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "SetCarry\n",
        "Sub(Imm(5) Reg(A))\n",
        "Branch(VC \".cmp_novf@0\")\n",
        "Xor(Reg(A) Imm(128) Reg(A))\n",
        "Label(\".cmp_novf@0\")\n",
        "ArithmeticShiftLeft(Reg(A))\n",
        "Mov(Imm(0) Reg(A) false)\n",
        "RotateLeft(Reg(A))\n",
        "Xor(Reg(A) Imm(1) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateComparisons::test_greater_than_swaps_and_uses_bmi (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(GreaterThan Var(\"%0\") Constant(ConstInt(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Imm(5) Reg(A) false)\n",
        "SetCarry\n",
        "Sub(Pseudo(\"%0\" 0) Reg(A))\n",
        "Branch(VC \".cmp_novf@0\")\n",
        "Xor(Reg(A) Imm(128) Reg(A))\n",
        "Label(\".cmp_novf@0\")\n",
        "ArithmeticShiftLeft(Reg(A))\n",
        "Mov(Imm(0) Reg(A) false)\n",
        "RotateLeft(Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateComparisons::test_less_or_equal_swaps_and_uses_bpl (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(LessOrEqual Var(\"%0\") Constant(ConstInt(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Imm(5) Reg(A) false)\n",
        "SetCarry\n",
        "Sub(Pseudo(\"%0\" 0) Reg(A))\n",
        "Branch(VC \".cmp_novf@0\")\n",
        "Xor(Reg(A) Imm(128) Reg(A))\n",
        "Label(\".cmp_novf@0\")\n",
        "ArithmeticShiftLeft(Reg(A))\n",
        "Mov(Imm(0) Reg(A) false)\n",
        "RotateLeft(Reg(A))\n",
        "Xor(Reg(A) Imm(1) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateComparisons::test_less_than_uses_sbc_and_bmi_no_swap (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(LessThan Var(\"%0\") Constant(ConstInt(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "SetCarry\n",
        "Sub(Imm(5) Reg(A))\n",
        "Branch(VC \".cmp_novf@0\")\n",
        "Xor(Reg(A) Imm(128) Reg(A))\n",
        "Label(\".cmp_novf@0\")\n",
        "ArithmeticShiftLeft(Reg(A))\n",
        "Mov(Imm(0) Reg(A) false)\n",
        "RotateLeft(Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateComparisons::test_not_equal_uses_compare_and_bne (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(NotEqual Var(\"%0\") Constant(ConstInt(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "Compare(Reg(A) Imm(5))\n",
        "Branch(NE \".cmp_true@0\")\n",
        "Mov(Imm(0) Reg(A) false)\n",
        "Jump(\".cmp_end@1\" [])\n",
        "Label(\".cmp_true@0\")\n",
        "Mov(Imm(1) Reg(A) false)\n",
        "Label(\".cmp_end@1\")\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_function: test_tac_to_asm.py::TestTranslateFunction::test_empty_function (call 1)") {
  c6502::testing::check_translator(
      "translate_function",
      {
        "Function(\"main\" true [] [])\n"
      },
      {
        "Function(\"main\" true [] [])\n"
      });
}

TEST_CASE("translate_function: test_tac_to_asm.py::TestTranslateFunction::test_flattens_instructions (call 1)") {
  c6502::testing::check_translator(
      "translate_function",
      {
        "Function(\"main\" true [] [\n"
        "  Unary(Negate Constant(ConstChar(1)) Var(\"%0\"))\n"
        "  Ret(Var(\"%0\"))\n"
        "])\n"
      },
      {
        "Function(\"main\" true [] [\n"
        "  Mov(Imm(1) Reg(A) false)\n"
        "  Xor(Reg(A) Imm(255) Reg(A))\n"
        "  ClearCarry\n"
        "  Add(Imm(1) Reg(A))\n"
        "  Mov(Reg(A) Pseudo(\"%0\" 0) false)\n"
        "  Mov(Pseudo(\"%0\" 0) Reg(A) false)\n"
        "  Ret(0 0 true false [] false)\n"
        "])\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateFunctionCall::test_constant_arg (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "FunctionCall(\"f\" [Constant(ConstChar(42))] Var(\"@0.t\") false false)\n"
      },
      {
        "AllocateStack(1)\n",
        "Mov(Imm(42) Stack(1) false)\n",
        "Call(\"f\" [] [])\n",
        "Mov(Reg(A) Pseudo(\"@0.t\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateFunctionCall::test_multiple_args_get_stack_offsets_1_to_n (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "FunctionCall(\"f\" [Constant(ConstChar(1)) Constant(ConstChar(2)) Constant(ConstChar(3))] Var(\"@0.t\") false false)\n"
      },
      {
        "AllocateStack(3)\n",
        "Mov(Imm(1) Stack(1) false)\n",
        "Mov(Imm(2) Stack(2) false)\n",
        "Mov(Imm(3) Stack(3) false)\n",
        "Call(\"f\" [] [])\n",
        "Mov(Reg(A) Pseudo(\"@0.t\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateFunctionCall::test_no_args (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "FunctionCall(\"f\" [] Var(\"@0.t\") false false)\n"
      },
      {
        "Call(\"f\" [] [])\n",
        "Mov(Reg(A) Pseudo(\"@0.t\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateFunctionCall::test_var_arg_uses_pseudo (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "FunctionCall(\"f\" [Var(\"@0.x\")] Var(\"@1.t\") false false)\n"
      },
      {
        "AllocateStack(1)\n",
        "Mov(Pseudo(\"@0.x\" 0) Stack(1) false)\n",
        "Call(\"f\" [] [])\n",
        "Mov(Reg(A) Pseudo(\"@1.t\" 0) false)\n"
      });
}
