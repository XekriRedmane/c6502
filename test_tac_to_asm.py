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

    def test_binary_mul_div_mod_not_yet_implemented(self):
        for op in [tac_ast.Multiply(), tac_ast.Divide(), tac_ast.Modulo()]:
            with self.subTest(op=type(op).__name__):
                with self.assertRaises(NotImplementedError):
                    translate_binary(
                        op,
                        tac_ast.Constant(value=1),
                        tac_ast.Constant(value=2),
                        tac_ast.Var(name="%0"),
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
