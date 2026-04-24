import unittest

import asm_ast
import tac_ast
from tac_to_asm import (
    translate_binary,
    translate_function,
    translate_instruction,
    translate_program,
    translate_unop_atoms,
    translate_val,
)


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())


class TestTranslateVal(unittest.TestCase):
    def test_constant_becomes_imm(self):
        self.assertEqual(
            translate_val(tac_ast.Constant(value=42)),
            asm_ast.Imm(value=42),
        )

    def test_var_becomes_pseudo(self):
        self.assertEqual(
            translate_val(tac_ast.Var(name="%0")),
            asm_ast.Pseudo(name="%0"),
        )


class TestTranslateUnopAtoms(unittest.TestCase):
    def test_complement_emits_xor_with_ff(self):
        self.assertEqual(
            translate_unop_atoms(tac_ast.Complement()),
            [asm_ast.Xor(
                src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
            )],
        )

    def test_negate_emits_xor_clearcarry_add_one(self):
        self.assertEqual(
            translate_unop_atoms(tac_ast.Negate()),
            [
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                ),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
            ],
        )

    def test_logical_not_emits_lnot8_call(self):
        # !A -> JSR lnot8 (helper takes A, returns 1 if A==0 else 0).
        self.assertEqual(
            translate_unop_atoms(tac_ast.LogicalNot()),
            [asm_ast.Call(name="lnot8")],
        )


class TestTranslateInstruction(unittest.TestCase):
    def test_ret_emits_mov_to_a_then_ret(self):
        self.assertEqual(
            translate_instruction(tac_ast.Ret(val=tac_ast.Constant(value=7))),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ],
        )

    def test_ret_with_var_value(self):
        self.assertEqual(
            translate_instruction(tac_ast.Ret(val=tac_ast.Var(name="%3"))),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%3"), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ],
        )

    def test_unary_negate_lowered_to_atoms_around_a(self):
        # Mov(src, A) -> Xor(A, $FF, A) -> ClearCarry -> Add(1, A)
        # -> Mov(A, dst).
        instr = tac_ast.Unary(
            op=tac_ast.Negate(),
            src=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                ),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
            ],
        )

    def test_unary_complement_lowered_to_xor(self):
        # Mov(src, A) -> Xor(A, $FF, A) -> Mov(A, dst).
        instr = tac_ast.Unary(
            op=tac_ast.Complement(),
            src=tac_ast.Var(name="%1"),
            dst=tac_ast.Var(name="%2"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%1"), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%2")),
            ],
        )

    def test_binary_add_lowered(self):
        # Mov(src1, A) -> ClearCarry -> Add(src2, A) -> Mov(A, dst).
        instr = tac_ast.Binary(
            op=tac_ast.Add(),
            src1=tac_ast.Constant(value=3),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_REG_A),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_subtract_lowered(self):
        # Mov(src1, A) -> SetCarry -> Sub(src2, A) -> Mov(A, dst).
        instr = tac_ast.Binary(
            op=tac_ast.Subtract(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.SetCarry(),
                asm_ast.Sub(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_multiply_lowered_to_mul8_call(self):
        # mul8 takes A * X; staged as: src2 -> A -> X, src1 -> A,
        # Call mul8, Mov A -> dst. Result low byte is in A.
        instr = tac_ast.Binary(
            op=tac_ast.Multiply(),
            src1=tac_ast.Constant(value=3),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_REG_A),
                asm_ast.Call(name="mul8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_divide_lowered_to_divmod8_call(self):
        # divmod8 takes A (dividend) / X (divisor) -> A=quot, X=rem.
        # Divide wants the quotient, which is already in A.
        instr = tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Call(name="divmod8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_bitwise_and_lowered(self):
        # Mov(src1, A) -> And(src2, A) -> Mov(A, dst). No carry setup
        # because AND doesn't touch carry.
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseAnd(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=0x0F),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_bitwise_or_lowered(self):
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseOr(),
            src1=tac_ast.Constant(value=0xF0),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=0xF0), dst=_REG_A),
                asm_ast.Or(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_bitwise_xor_lowered(self):
        # Reuses the existing ternary Xor shape. The src1 of the asm
        # Xor is Reg(A); the src2 carries the addressing mode.
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseXor(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Var(name="%1"),
            dst=tac_ast.Var(name="%2"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A,
                    src2=asm_ast.Pseudo(name="%1"),
                    dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%2")),
            ],
        )

    def test_binary_left_shift_lowered_to_shl8_call(self):
        # shl8 takes A << X (logical), result in A.
        instr = tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=2),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Call(name="shl8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_right_shift_lowered_to_asr8_call(self):
        # Right shift is arithmetic — c6502 currently treats integers
        # as signed, so >> goes through asr8 (sign-preserving) rather
        # than a logical shift helper.
        instr = tac_ast.Binary(
            op=tac_ast.RightShift(),
            src1=tac_ast.Constant(value=0x80),
            src2=tac_ast.Constant(value=1),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Imm(value=0x80), dst=_REG_A),
                asm_ast.Call(name="asr8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
            ],
        )

    def test_binary_modulo_lowered_to_divmod8_call_with_x_to_a(self):
        # Modulo wants the remainder from divmod8, which comes back in
        # X — we shuffle X to A before storing.
        instr = tac_ast.Binary(
            op=tac_ast.Modulo(),
            src1=tac_ast.Constant(value=17),
            src2=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Imm(value=17), dst=_REG_A),
                asm_ast.Call(name="divmod8"),
                asm_ast.Mov(src=_REG_X, dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
            ],
        )


class TestTranslateComparisons(unittest.TestCase):
    """Each of ==, !=, <, >, <=, >= lowers to the same AX-call shape
    as Multiply/Divide: stage src2 through A into X, then src1 into A,
    then JSR <helper>. The helper returns 0/1 in A; result_in_x is
    False (we want A directly, not the X-byte fetch that Modulo uses)."""

    _CASES = [
        (tac_ast.Equal(),          "cmp_eq8"),
        (tac_ast.NotEqual(),       "cmp_ne8"),
        (tac_ast.LessThan(),       "cmp_lt8"),
        (tac_ast.GreaterThan(),    "cmp_gt8"),
        (tac_ast.LessOrEqual(),    "cmp_le8"),
        (tac_ast.GreaterOrEqual(), "cmp_ge8"),
    ]

    def test_each_helper(self):
        for tac_op, helper in self._CASES:
            with self.subTest(op=type(tac_op).__name__):
                instr = tac_ast.Binary(
                    op=tac_op,
                    src1=tac_ast.Var(name="%0"),
                    src2=tac_ast.Constant(value=5),
                    dst=tac_ast.Var(name="%1"),
                )
                self.assertEqual(
                    translate_instruction(instr),
                    [
                        asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                        asm_ast.Mov(src=_REG_A, dst=_REG_X),
                        asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                        asm_ast.Call(name=helper),
                        asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
                    ],
                )


class TestTranslateFunction(unittest.TestCase):
    def test_flattens_instructions(self):
        fn = tac_ast.Function(
            name="main",
            instructions=[
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Constant(value=1),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%0")),
            ],
        )
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(
                name="main",
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Xor(
                        src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
                    asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ],
            ),
        )

    def test_empty_function(self):
        fn = tac_ast.Function(name="main", instructions=[])
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(name="main", instructions=[]),
        )


class TestTranslateProgram(unittest.TestCase):
    def test_full_tree(self):
        prog = tac_ast.Program(
            function_definition=tac_ast.Function(
                name="main",
                instructions=[tac_ast.Ret(val=tac_ast.Constant(value=42))],
            ),
        )
        expected = asm_ast.Program(
            function_definition=asm_ast.Function(
                name="main",
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_REG_A),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ],
            ),
        )
        self.assertEqual(translate_program(prog), expected)


class TestErrors(unittest.TestCase):
    def test_unknown_val_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_val,), {})
        with self.assertRaises(TypeError):
            translate_val(stub())

    def test_unknown_instruction_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_instruction,), {})
        with self.assertRaises(TypeError):
            translate_instruction(stub())

    def test_unknown_unop_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_unary_operator,), {})
        with self.assertRaises(TypeError):
            translate_unop_atoms(stub())


if __name__ == "__main__":
    unittest.main()
