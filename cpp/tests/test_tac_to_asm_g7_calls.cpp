// The calls that the tests of tests/test_tac_to_asm.py made of the functions
// that `check-captures` runs, each with what the Python function
// returned, changed, or raised, which the C++ port must return,
// change, or raise too. tools/record_test_calls.py recorded them
// while the Python tests ran, before they were removed (step 8,
// groups G6 and G7). A call that another C++ case already checks
// isn't repeated here.
#include "doctest.h"
#include "record_case.h"
#include "translator_case.h"

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_add_lowered (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(Add Constant(ConstChar(3)) Var(\"%0\") Var(\"%1\"))\n"
      },
      {
        "Mov(Imm(3) Reg(A) false)\n",
        "ClearCarry\n",
        "Add(Pseudo(\"%0\" 0) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_bitwise_and_lowered (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(BitwiseAnd Var(\"%0\") Constant(ConstChar(15)) Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "And(Imm(15) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_bitwise_or_lowered (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(BitwiseOr Constant(ConstChar(-16)) Var(\"%0\") Var(\"%1\"))\n"
      },
      {
        "Mov(Imm(240) Reg(A) false)\n",
        "Or(Pseudo(\"%0\" 0) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_bitwise_xor_lowered (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(BitwiseXor Var(\"%0\") Var(\"%1\") Var(\"%2\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "Xor(Reg(A) Pseudo(\"%1\" 0) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_divide_lowered_to_sdivmod8_call (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(Divide Var(\"%0\") Constant(ConstChar(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
        "Mov(Imm(5) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
        "Call(\"sdivmod8\" [] [])\n",
        "Mov(Data(\"HARGS\" 2) Reg(A) false)\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_left_shift_by_variable_is_a_loop (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(LeftShift Var(\"%0\") Var(\"%c\") Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Pseudo(\"%1\" 0) false)\n",
        "Mov(Pseudo(\"%c\" 0) Reg(A) false)\n",
        "Mov(Reg(A) Reg(X) false)\n",
        "Compare(Reg(X) Imm(0))\n",
        "Branch(EQ \".shift_done@1\")\n",
        "Label(\".shift_loop@0\")\n",
        "ArithmeticShiftLeft(Pseudo(\"%1\" 0))\n",
        "Dec(Reg(X))\n",
        "Branch(NE \".shift_loop@0\")\n",
        "Label(\".shift_done@1\")\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_modulo_lowered_to_sdivmod8_remainder (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(Modulo Constant(ConstChar(17)) Constant(ConstChar(5)) Var(\"%0\"))\n"
      },
      {
        "Mov(Imm(17) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
        "Mov(Imm(5) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
        "Call(\"sdivmod8\" [] [])\n",
        "Mov(Data(\"HARGS\" 3) Reg(A) false)\n",
        "Mov(Reg(A) Pseudo(\"%0\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_multiply_lowered_to_mul8_call (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(Multiply Constant(ConstChar(3)) Var(\"%0\") Var(\"%1\"))\n"
      },
      {
        "Mov(Imm(3) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
        "Call(\"mul8\" [] [])\n",
        "Mov(Data(\"HARGS\" 2) Reg(A) false)\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_right_shift_of_signed_value_is_arithmetic_loop (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(RightShift Constant(ConstChar(64)) Var(\"%c\") Var(\"%0\"))\n"
      },
      {
        "Mov(Imm(64) Pseudo(\"%0\" 0) false)\n",
        "Mov(Pseudo(\"%c\" 0) Reg(A) false)\n",
        "Mov(Reg(A) Reg(X) false)\n",
        "Compare(Reg(X) Imm(0))\n",
        "Branch(EQ \".shift_done@1\")\n",
        "Label(\".shift_loop@0\")\n",
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "ArithmeticShiftLeft(Reg(A))\n",
        "RotateRight(Pseudo(\"%0\" 0))\n",
        "Dec(Reg(X))\n",
        "Branch(NE \".shift_loop@0\")\n",
        "Label(\".shift_done@1\")\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_binary_subtract_lowered (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Binary(Subtract Var(\"%0\") Constant(ConstChar(5)) Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "SetCarry\n",
        "Sub(Imm(5) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_function_call_captures_double_return_from_hargs_16_through_23 (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "FunctionCall(\"ret_d\" [] Var(\"%dst\") false false)\n"},
          {
            "Call(\"ret_d\" [] [])\n",
            "Mov(Data(\"HARGS\" 16) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 17) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 18) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 19) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n",
            "Mov(Data(\"HARGS\" 20) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 4) false)\n",
            "Mov(Data(\"HARGS\" 21) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 5) false)\n",
            "Mov(Data(\"HARGS\" 22) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 6) false)\n",
            "Mov(Data(\"HARGS\" 23) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 7) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_function_call_captures_float_return_from_hargs_8_through_11 (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "FunctionCall(\"ret_f\" [] Var(\"%dst\") false false)\n"},
          {
            "Call(\"ret_f\" [] [])\n",
            "Mov(Data(\"HARGS\" 8) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 9) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 10) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 11) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_left_shift_by_16_on_ulong_emits_two_byte_shuffle (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(LeftShift Var(\"%0\") Constant(ConstInt(16)) Var(\"%1\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 1) Pseudo(\"%1\" 3) false)\n",
            "Mov(Pseudo(\"%0\" 0) Pseudo(\"%1\" 2) false)\n",
            "Mov(Imm(0) Pseudo(\"%1\" 1) false)\n",
            "Mov(Imm(0) Pseudo(\"%1\" 0) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_left_shift_by_byte_aligned_constant_emits_byte_moves (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(LeftShift Var(\"%0\") Constant(ConstInt(8)) Var(\"%1\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Pseudo(\"%1\" 1) false)\n",
            "Mov(Imm(0) Pseudo(\"%1\" 0) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_left_shift_by_size_in_bits_is_a_loop (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(LeftShift Var(\"%0\") Constant(ConstInt(16)) Var(\"%1\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Pseudo(\"%1\" 0) false)\n",
            "Mov(Pseudo(\"%0\" 1) Pseudo(\"%1\" 1) false)\n",
            "Mov(Imm(16) Reg(X) false)\n",
            "Label(\".shift_loop@0\")\n",
            "ArithmeticShiftLeft(Pseudo(\"%1\" 0))\n",
            "RotateLeft(Pseudo(\"%1\" 1))\n",
            "Dec(Reg(X))\n",
            "Branch(NE \".shift_loop@0\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_ret_double_writes_hargs_16_through_23_no_save_a (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%d\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Ret(Var(\"%d\"))\n"},
          {
            "Mov(Pseudo(\"%d\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 16) false)\n",
            "Mov(Pseudo(\"%d\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 17) false)\n",
            "Mov(Pseudo(\"%d\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 18) false)\n",
            "Mov(Pseudo(\"%d\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 19) false)\n",
            "Mov(Pseudo(\"%d\" 4) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 20) false)\n",
            "Mov(Pseudo(\"%d\" 5) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 21) false)\n",
            "Mov(Pseudo(\"%d\" 6) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 22) false)\n",
            "Mov(Pseudo(\"%d\" 7) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 23) false)\n",
            "Ret(0 0 false false [] false)\n"},
        }
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_ret_emits_mov_to_a_then_ret (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Ret(Constant(ConstChar(7)))\n"
      },
      {
        "Mov(Imm(7) Reg(A) false)\n",
        "Ret(0 0 true false [] false)\n"
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_ret_float_writes_hargs_8_through_11_no_save_a (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%f\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Ret(Var(\"%f\"))\n"},
          {
            "Mov(Pseudo(\"%f\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 8) false)\n",
            "Mov(Pseudo(\"%f\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 9) false)\n",
            "Mov(Pseudo(\"%f\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 10) false)\n",
            "Mov(Pseudo(\"%f\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 11) false)\n",
            "Ret(0 0 false false [] false)\n"},
        }
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_ret_long_constant_writes_to_hargs_0_through_1 (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Ret(Constant(ConstInt(4660)))\n"
      },
      {
        "Mov(Imm(52) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
        "Mov(Imm(18) Reg(A) false)\n",
        "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
        "Ret(0 0 false false [] false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_ret_with_var_value (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Ret(Var(\"%3\"))\n"
      },
      {
        "Mov(Pseudo(\"%3\" 0) Reg(A) false)\n",
        "Ret(0 0 true false [] false)\n"
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_signed_right_shift_by_8_is_a_loop (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(RightShift Var(\"%0\") Constant(ConstInt(8)) Var(\"%1\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Pseudo(\"%1\" 0) false)\n",
            "Mov(Pseudo(\"%0\" 1) Pseudo(\"%1\" 1) false)\n",
            "Mov(Imm(8) Reg(X) false)\n",
            "Label(\".shift_loop@0\")\n",
            "Mov(Pseudo(\"%1\" 1) Reg(A) false)\n",
            "ArithmeticShiftLeft(Reg(A))\n",
            "RotateRight(Pseudo(\"%1\" 1))\n",
            "RotateRight(Pseudo(\"%1\" 0))\n",
            "Dec(Reg(X))\n",
            "Branch(NE \".shift_loop@0\")\n"},
        }
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_unary_complement_lowered_to_xor (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Unary(Complement Var(\"%1\") Var(\"%2\"))\n"
      },
      {
        "Mov(Pseudo(\"%1\" 0) Reg(A) false)\n",
        "Xor(Reg(A) Imm(255) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%2\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_unary_logical_not_lowered_inline (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Unary(LogicalNot Var(\"%0\") Var(\"%1\"))\n"
      },
      {
        "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
        "Branch(EQ \".lnot_true@0\")\n",
        "Mov(Imm(0) Reg(A) false)\n",
        "Jump(\".lnot_end@1\" [])\n",
        "Label(\".lnot_true@0\")\n",
        "Mov(Imm(1) Reg(A) false)\n",
        "Label(\".lnot_end@1\")\n",
        "Mov(Reg(A) Pseudo(\"%1\" 0) false)\n"
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestTranslateInstruction::test_unary_negate_lowered_to_atoms_around_a (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Unary(Negate Constant(ConstChar(5)) Var(\"%0\"))\n"
      },
      {
        "Mov(Imm(5) Reg(A) false)\n",
        "Xor(Reg(A) Imm(255) Reg(A))\n",
        "ClearCarry\n",
        "Add(Imm(1) Reg(A))\n",
        "Mov(Reg(A) Pseudo(\"%0\" 0) false)\n"
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateInstruction::test_unsigned_right_shift_by_8_emits_byte_moves (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(RightShift Var(\"%0\") Constant(ConstInt(8)) Var(\"%1\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 1) Pseudo(\"%1\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%1\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateEqualityWideTypes::test_double_equal_short_circuits_via_cmp_differ_label (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(Equal Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 7) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 7))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 6) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 6))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 5) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 5))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 4) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 4))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 3))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 2))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 1))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 0))\n",
            "Label(\".cmp_differ@2\")\n",
            "Branch(EQ \".cmp_true@0\")\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "Jump(\".cmp_end@1\" [])\n",
            "Label(\".cmp_true@0\")\n",
            "Mov(Imm(1) Reg(A) false)\n",
            "Label(\".cmp_end@1\")\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%dst\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateEqualityWideTypes::test_not_equal_on_float_compares_all_four_bytes (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(NotEqual Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 3))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 2))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 1))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 0))\n",
            "Label(\".cmp_differ@2\")\n",
            "Branch(NE \".cmp_true@0\")\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "Jump(\".cmp_end@1\" [])\n",
            "Label(\".cmp_true@0\")\n",
            "Mov(Imm(1) Reg(A) false)\n",
            "Label(\".cmp_end@1\")\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%dst\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_function_call_captures_long_long_return_from_hargs_8_through_11 (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "FunctionCall(\"ret_ll\" [] Var(\"%dst\") false false)\n"},
          {
            "Call(\"ret_ll\" [] [])\n",
            "Mov(Data(\"HARGS\" 8) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 9) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 10) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 11) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_long_long_add_chains_four_byte_adcs (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(Add Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "ClearCarry\n",
            "Add(Pseudo(\"%1\" 0) Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Add(Pseudo(\"%1\" 1) Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Add(Pseudo(\"%1\" 2) Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Add(Pseudo(\"%1\" 3) Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_long_long_equality_walks_four_bytes (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(Equal Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 3))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 2))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 1))\n",
            "Branch(NE \".cmp_differ@2\")\n",
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%1\" 0))\n",
            "Label(\".cmp_differ@2\")\n",
            "Branch(EQ \".cmp_true@0\")\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "Jump(\".cmp_end@1\" [])\n",
            "Label(\".cmp_true@0\")\n",
            "Mov(Imm(1) Reg(A) false)\n",
            "Label(\".cmp_end@1\")\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%dst\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_long_long_left_shift_dispatches_to_asl32 (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(LeftShift Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 2) false)\n",
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 3) false)\n",
            "Mov(Pseudo(\"%1\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 4) false)\n",
            "Call(\"asl32\" [] [])\n",
            "Mov(Data(\"HARGS\" 5) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 6) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 7) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 8) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_long_long_multiply_dispatches_to_mul32 (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(Multiply Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 2) false)\n",
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 3) false)\n",
            "Mov(Pseudo(\"%1\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 4) false)\n",
            "Mov(Pseudo(\"%1\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 5) false)\n",
            "Mov(Pseudo(\"%1\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 6) false)\n",
            "Mov(Pseudo(\"%1\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 7) false)\n",
            "Call(\"mul32\" [] [])\n",
            "Mov(Data(\"HARGS\" 8) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 9) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 10) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 11) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_long_long_return_writes_to_hargs_8_through_11 (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Ret(Var(\"%0\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 8) false)\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 9) false)\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 10) false)\n",
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 11) false)\n",
            "Ret(0 0 false false [] false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_long_long_signed_less_than_chains_four_sbcs (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(LessThan Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "SetCarry\n",
            "Sub(Pseudo(\"%1\" 0) Reg(A))\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Sub(Pseudo(\"%1\" 1) Reg(A))\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Sub(Pseudo(\"%1\" 2) Reg(A))\n",
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Sub(Pseudo(\"%1\" 3) Reg(A))\n",
            "Branch(VC \".cmp_novf@0\")\n",
            "Xor(Reg(A) Imm(128) Reg(A))\n",
            "Label(\".cmp_novf@0\")\n",
            "ArithmeticShiftLeft(Reg(A))\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "RotateLeft(Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%dst\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_long_to_float_uses_l2f_helper (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%src\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Float\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "IntToFloat(Var(\"%src\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%src\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
            "Mov(Pseudo(\"%src\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
            "Mov(Pseudo(\"%src\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 2) false)\n",
            "Mov(Pseudo(\"%src\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 3) false)\n",
            "Call(\"l2f\" [] [])\n",
            "Mov(Data(\"HARGS\" 4) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 5) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 6) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 7) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_ulong_to_double_uses_ul2d_helper (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%src\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Double\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "IntToDouble(Var(\"%src\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%src\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
            "Mov(Pseudo(\"%src\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
            "Mov(Pseudo(\"%src\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 2) false)\n",
            "Mov(Pseudo(\"%src\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 3) false)\n",
            "Call(\"ul2d\" [] [])\n",
            "Mov(Data(\"HARGS\" 4) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 5) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 6) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 7) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n",
            "Mov(Data(\"HARGS\" 8) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 4) false)\n",
            "Mov(Data(\"HARGS\" 9) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 5) false)\n",
            "Mov(Data(\"HARGS\" 10) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 6) false)\n",
            "Mov(Data(\"HARGS\" 11) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 7) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_unsigned_long_long_divide_dispatches_to_udivmod32 (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%1\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%dst\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(Divide Var(\"%0\") Var(\"%1\") Var(\"%dst\"))\n"},
          {
            "Mov(Pseudo(\"%0\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 0) false)\n",
            "Mov(Pseudo(\"%0\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 1) false)\n",
            "Mov(Pseudo(\"%0\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 2) false)\n",
            "Mov(Pseudo(\"%0\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 3) false)\n",
            "Mov(Pseudo(\"%1\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 4) false)\n",
            "Mov(Pseudo(\"%1\" 1) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 5) false)\n",
            "Mov(Pseudo(\"%1\" 2) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 6) false)\n",
            "Mov(Pseudo(\"%1\" 3) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"HARGS\" 7) false)\n",
            "Call(\"udivmod32\" [] [])\n",
            "Mov(Data(\"HARGS\" 8) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 0) false)\n",
            "Mov(Data(\"HARGS\" 9) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 1) false)\n",
            "Mov(Data(\"HARGS\" 10) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 2) false)\n",
            "Mov(Data(\"HARGS\" 11) Reg(A) false)\n",
            "Mov(Reg(A) Pseudo(\"%dst\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateLongLong::test_zero_extend_uint_to_ulong_writes_two_zero_high_bytes (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "ZeroExtend(Var(\"@0.x\") Var(\"%0\"))\n"},
          {
            "Mov(Pseudo(\"@0.x\" 0) Pseudo(\"%0\" 0) false)\n",
            "Mov(Pseudo(\"@0.x\" 1) Pseudo(\"%0\" 1) false)\n",
            "Mov(Imm(0) Pseudo(\"%0\" 2) false)\n",
            "Mov(Imm(0) Pseudo(\"%0\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslatePointerOrdering::test_long_ordering_still_signed (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_binary",
          {
            "LessThan\n",
            "Var(\"%a\")\n",
            "Var(\"%b\")\n",
            "Var(\"%r\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "SetCarry\n",
            "Sub(Pseudo(\"%b\" 0) Reg(A))\n",
            "Mov(Pseudo(\"%a\" 1) Reg(A) false)\n",
            "Sub(Pseudo(\"%b\" 1) Reg(A))\n",
            "Mov(Pseudo(\"%a\" 2) Reg(A) false)\n",
            "Sub(Pseudo(\"%b\" 2) Reg(A))\n",
            "Mov(Pseudo(\"%a\" 3) Reg(A) false)\n",
            "Sub(Pseudo(\"%b\" 3) Reg(A))\n",
            "Branch(VC \".cmp_novf@0\")\n",
            "Xor(Reg(A) Imm(128) Reg(A))\n",
            "Label(\".cmp_novf@0\")\n",
            "ArithmeticShiftLeft(Reg(A))\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "RotateLeft(Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%r\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%r\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslatePointerOrdering::test_pointer_greater_or_equal_uses_bcs_no_swap (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_binary",
          {
            "GreaterOrEqual\n",
            "Var(\"%p\")\n",
            "Var(\"%q\")\n",
            "Var(\"%r\")\n"},
          {
            "Mov(Pseudo(\"%p\" 0) Reg(A) false)\n",
            "SetCarry\n",
            "Sub(Pseudo(\"%q\" 0) Reg(A))\n",
            "Mov(Pseudo(\"%p\" 1) Reg(A) false)\n",
            "Sub(Pseudo(\"%q\" 1) Reg(A))\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "RotateLeft(Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%r\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%r\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslatePointerOrdering::test_pointer_greater_than_swaps_and_uses_bcc (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_binary",
          {
            "GreaterThan\n",
            "Var(\"%p\")\n",
            "Var(\"%q\")\n",
            "Var(\"%r\")\n"},
          {
            "Mov(Pseudo(\"%p\" 0) Reg(A) false)\n",
            "ClearCarry\n",
            "Sub(Pseudo(\"%q\" 0) Reg(A))\n",
            "Mov(Pseudo(\"%p\" 1) Reg(A) false)\n",
            "Sub(Pseudo(\"%q\" 1) Reg(A))\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "RotateLeft(Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%r\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%r\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslatePointerOrdering::test_pointer_less_or_equal_swaps_and_uses_bcs (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_binary",
          {
            "LessOrEqual\n",
            "Var(\"%p\")\n",
            "Var(\"%q\")\n",
            "Var(\"%r\")\n"},
          {
            "Mov(Pseudo(\"%q\" 0) Reg(A) false)\n",
            "SetCarry\n",
            "Sub(Pseudo(\"%p\" 0) Reg(A))\n",
            "Mov(Pseudo(\"%q\" 1) Reg(A) false)\n",
            "Sub(Pseudo(\"%p\" 1) Reg(A))\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "RotateLeft(Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%r\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%r\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslatePointerOrdering::test_pointer_less_than_uses_bcc_no_swap (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%p\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%q\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Pointer(Int)\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%r\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_binary",
          {
            "LessThan\n",
            "Var(\"%p\")\n",
            "Var(\"%q\")\n",
            "Var(\"%r\")\n"},
          {
            "Mov(Pseudo(\"%q\" 0) Reg(A) false)\n",
            "ClearCarry\n",
            "Sub(Pseudo(\"%p\" 0) Reg(A))\n",
            "Mov(Pseudo(\"%q\" 1) Reg(A) false)\n",
            "Sub(Pseudo(\"%p\" 1) Reg(A))\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "RotateLeft(Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%r\" 0) false)\n",
            "Mov(Imm(0) Pseudo(\"%r\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestSignExtendAndTruncate::test_sign_extend_char_to_long (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "SignExtend(Var(\"@0.x\") Var(\"%0\"))\n"},
          {
            "Mov(Pseudo(\"@0.x\" 0) Pseudo(\"%0\" 0) false)\n",
            "Mov(Pseudo(\"@0.x\" 0) Reg(A) false)\n",
            "Branch(MI \".sx_neg@0\")\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "Jump(\".sx_done@1\" [])\n",
            "Label(\".sx_neg@0\")\n",
            "Mov(Imm(255) Reg(A) false)\n",
            "Label(\".sx_done@1\")\n",
            "Mov(Reg(A) Pseudo(\"%0\" 1) false)\n",
            "Mov(Reg(A) Pseudo(\"%0\" 2) false)\n",
            "Mov(Reg(A) Pseudo(\"%0\" 3) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestSignExtendAndTruncate::test_sign_extend_lowers_to_inline_byte_sequence (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"SChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "SignExtend(Var(\"@0.x\") Var(\"%0\"))\n"},
          {
            "Mov(Pseudo(\"@0.x\" 0) Pseudo(\"%0\" 0) false)\n",
            "Mov(Pseudo(\"@0.x\" 0) Reg(A) false)\n",
            "Branch(MI \".sx_neg@0\")\n",
            "Mov(Imm(0) Reg(A) false)\n",
            "Jump(\".sx_done@1\" [])\n",
            "Label(\".sx_neg@0\")\n",
            "Mov(Imm(255) Reg(A) false)\n",
            "Label(\".sx_done@1\")\n",
            "Mov(Reg(A) Pseudo(\"%0\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestSignExtendAndTruncate::test_truncate_long_to_int_copies_two_bytes (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"@0.x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Long\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%0\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Truncate(Var(\"@0.x\") Var(\"%0\"))\n"},
          {
            "Mov(Pseudo(\"@0.x\" 0) Pseudo(\"%0\" 0) false)\n",
            "Mov(Pseudo(\"@0.x\" 1) Pseudo(\"%0\" 1) false)\n"},
        }
      });
}

TEST_CASE("translate_instruction: test_tac_to_asm.py::TestSignExtendAndTruncate::test_truncate_lowers_to_byte_mov (call 1)") {
  c6502::testing::check_translator(
      "translate_instruction",
      {
        "Truncate(Var(\"@0.x\") Var(\"%0\"))\n"
      },
      {
        "Mov(Pseudo(\"@0.x\" 0) Pseudo(\"%0\" 0) false)\n"
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_int_lt_uses_signed_v_corrected_sequence (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"Int\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(LessThan Var(\"%a\") Constant(ConstInt(100)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "SetCarry\n",
            "Sub(Imm(100) Reg(A))\n",
            "Mov(Pseudo(\"%a\" 1) Reg(A) false)\n",
            "Sub(Imm(0) Reg(A))\n",
            "Branch(VC \".jcmp_novf@0\")\n",
            "Xor(Reg(A) Imm(128) Reg(A))\n",
            "Label(\".jcmp_novf@0\")\n",
            "Branch(MI \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_uchar_eq_uses_lda_cmp_beq (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(Equal Var(\"%a\") Constant(ConstUChar(5)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Imm(5))\n",
            "Branch(EQ \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_uchar_ge_const_uses_lda_cmp_bcs (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(GreaterOrEqual Var(\"%a\") Constant(ConstUChar(10)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Imm(10))\n",
            "Branch(CS \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_uchar_gt_swaps_operands (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%b\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(GreaterThan Var(\"%a\") Var(\"%b\") \".end\")\n"},
          {
            "Mov(Pseudo(\"%b\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Pseudo(\"%a\" 0))\n",
            "Branch(CC \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_uchar_lt_const_uses_lda_cmp_bcc (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(LessThan Var(\"%a\") Constant(ConstUChar(105)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Imm(105))\n",
            "Branch(CC \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_uchar_ne_uses_lda_cmp_bne (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(NotEqual Var(\"%a\") Constant(ConstUChar(5)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "Compare(Reg(A) Imm(5))\n",
            "Branch(NE \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_uint_lt_uses_unsigned_sbc_chain (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(LessThan Var(\"%a\") Constant(ConstUInt(1000)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "SetCarry\n",
            "Sub(Imm(232) Reg(A))\n",
            "Mov(Pseudo(\"%a\" 1) Reg(A) false)\n",
            "Sub(Imm(3) Reg(A))\n",
            "Branch(CC \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_wide_equal_zero_ors_the_bytes (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(Equal Var(\"%a\") Constant(ConstULong(0)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "Or(Pseudo(\"%a\" 1) Reg(A))\n",
            "Or(Pseudo(\"%a\" 2) Reg(A))\n",
            "Or(Pseudo(\"%a\" 3) Reg(A))\n",
            "Branch(EQ \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_wide_not_equal_zero_ors_the_bytes (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(NotEqual Var(\"%a\") Constant(ConstULong(0)) \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "Or(Pseudo(\"%a\" 1) Reg(A))\n",
            "Or(Pseudo(\"%a\" 2) Reg(A))\n",
            "Or(Pseudo(\"%a\" 3) Reg(A))\n",
            "Branch(NE \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestTranslateJumpIfCmp::test_wide_zero_on_the_left_ors_the_bytes (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%a\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"ULong\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "JumpIfCmp(Equal Constant(ConstULong(0)) Var(\"%a\") \".end\")\n"},
          {
            "Mov(Pseudo(\"%a\" 0) Reg(A) false)\n",
            "Or(Pseudo(\"%a\" 1) Reg(A))\n",
            "Or(Pseudo(\"%a\" 2) Reg(A))\n",
            "Or(Pseudo(\"%a\" 3) Reg(A))\n",
            "Branch(EQ \".end\")\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestConstantIndexAndByteShift::test_byte_left_shift_in_a (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%x\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}], [\"%y\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "Binary(LeftShift Var(\"%x\") Constant(ConstUChar(3)) Var(\"%y\"))\n"},
          {
            "Mov(Pseudo(\"%x\" 0) Reg(A) false)\n",
            "ArithmeticShiftLeft(Reg(A))\n",
            "ArithmeticShiftLeft(Reg(A))\n",
            "ArithmeticShiftLeft(Reg(A))\n",
            "Mov(Reg(A) Pseudo(\"%y\" 0) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestConstantIndexAndByteShift::test_indexed_load_constant_index (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UInt\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "IndexedLoad(\"arr\" Constant(ConstUChar(3)) Var(\"%v\") false 0 0)\n"},
          {
            "Mov(Data(\"arr\" 3) Pseudo(\"%v\" 0) false)\n",
            "Mov(Data(\"arr\" 4) Pseudo(\"%v\" 1) false)\n"},
        }
      });
}

TEST_CASE("asm_session: test_tac_to_asm.py::TestConstantIndexAndByteShift::test_indexed_symbol_store_constant_index (call 1)") {
  c6502::testing::check_translator_session(
      "{\"object\": \"passes.type_checking.SymbolTable\", \"fields\": {\"_table\": {\"dict\": [[\"%v\", {\"object\": \"passes.type_checking.Symbol\", \"fields\": {\"type\": {\"asdl\": \"c99_ast\", \"dump\": \"UChar\\n\"}, \"attrs\": {\"object\": \"passes.type_checking.LocalAttr\", \"fields\": {\"register\": false, \"was_bool\": false}}}}]]}}}",
      {
        {
          "translate_instruction",
          {
            "IndexedSymbolStore(\"arr\" Constant(ConstUChar(5)) Var(\"%v\") false 0)\n"},
          {
            "Mov(Pseudo(\"%v\" 0) Reg(A) false)\n",
            "Mov(Reg(A) Data(\"arr\" 5) false)\n"},
        }
      });
}
