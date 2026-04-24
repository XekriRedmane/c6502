import unittest

import asm_ast
import tac_ast
from asm_translator import (
    translate_function,
    translate_instruction,
    translate_program,
    translate_unop,
    translate_val,
)


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


class TestTranslateUnop(unittest.TestCase):
    def test_complement_becomes_not(self):
        self.assertEqual(translate_unop(tac_ast.Complement()), asm_ast.Not())

    def test_negate_becomes_neg(self):
        self.assertEqual(translate_unop(tac_ast.Negate()), asm_ast.Neg())


class TestTranslateInstruction(unittest.TestCase):
    def test_ret_emits_mov_to_a_then_ret(self):
        self.assertEqual(
            translate_instruction(tac_ast.Ret(val=tac_ast.Constant(value=7))),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=7), dst=asm_ast.Reg(reg=asm_ast.A())),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ],
        )

    def test_ret_with_var_value(self):
        self.assertEqual(
            translate_instruction(tac_ast.Ret(val=tac_ast.Var(name="%3"))),
            [
                asm_ast.Mov(
                    src=asm_ast.Pseudo(name="%3"),
                    dst=asm_ast.Reg(reg=asm_ast.A()),
                ),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ],
        )

    def test_unary_emits_mov_then_unary_on_dst(self):
        instr = tac_ast.Unary(
            op=tac_ast.Negate(),
            src=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=asm_ast.Pseudo(name="%0")),
                asm_ast.Unary(op=asm_ast.Neg(), src_dst=asm_ast.Pseudo(name="%0")),
            ],
        )

    def test_unary_complement_uses_not(self):
        instr = tac_ast.Unary(
            op=tac_ast.Complement(),
            src=tac_ast.Var(name="%1"),
            dst=tac_ast.Var(name="%2"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(
                    src=asm_ast.Pseudo(name="%1"),
                    dst=asm_ast.Pseudo(name="%2"),
                ),
                asm_ast.Unary(op=asm_ast.Not(), src_dst=asm_ast.Pseudo(name="%2")),
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
                    asm_ast.Mov(
                        src=asm_ast.Imm(value=1),
                        dst=asm_ast.Pseudo(name="%0"),
                    ),
                    asm_ast.Unary(
                        op=asm_ast.Neg(),
                        src_dst=asm_ast.Pseudo(name="%0"),
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Pseudo(name="%0"),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
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
                    asm_ast.Mov(
                        src=asm_ast.Imm(value=42),
                        dst=asm_ast.Reg(reg=asm_ast.A()),
                    ),
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
            translate_unop(stub())


if __name__ == "__main__":
    unittest.main()
