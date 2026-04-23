import unittest

import asm_ast
import c99_ast
from asm_translator import (
    translate_exp,
    translate_function,
    translate_program,
    translate_statement,
)
from parser import parse


class TestAsmTranslator(unittest.TestCase):
    def test_translate_exp_constant_becomes_imm(self):
        self.assertEqual(
            translate_exp(c99_ast.Constant(value=42)),
            asm_ast.Imm(value=42),
        )

    def test_translate_statement_return_emits_mov_ret(self):
        stmt = c99_ast.Return(exp=c99_ast.Constant(value=7))
        self.assertEqual(
            translate_statement(stmt),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=7), dst=asm_ast.Register()),
                asm_ast.Ret(),
            ],
        )

    def test_translate_function_wraps_statement_as_instruction_list(self):
        fn = c99_ast.Function(
            name="main",
            body=c99_ast.Return(exp=c99_ast.Constant(value=0)),
        )
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(
                name="main",
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=0), dst=asm_ast.Register()),
                    asm_ast.Ret(),
                ],
            ),
        )

    def test_translate_program_full_tree(self):
        prog = c99_ast.Program(
            function_definition=c99_ast.Function(
                name="main",
                body=c99_ast.Return(exp=c99_ast.Constant(value=42)),
            ),
        )
        expected = asm_ast.Program(
            function_definition=asm_ast.Function(
                name="main",
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=42), dst=asm_ast.Register()),
                    asm_ast.Ret(),
                ],
            ),
        )
        self.assertEqual(translate_program(prog), expected)

    def test_end_to_end_from_source(self):
        # Parse a real source string, then translate.
        asm = translate_program(parse("int main(void) { return 99; }"))
        self.assertEqual(asm.function_definition.name, "main")
        self.assertEqual(
            asm.function_definition.instructions,
            [
                asm_ast.Mov(src=asm_ast.Imm(value=99), dst=asm_ast.Register()),
                asm_ast.Ret(),
            ],
        )

    def test_unknown_node_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_exp,), {})
        with self.assertRaises(TypeError):
            translate_exp(stub())


if __name__ == "__main__":
    unittest.main()
