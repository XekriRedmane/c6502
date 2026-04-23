import unittest

import c99_ast
import tac_ast
from parser import parse
from tac_translator import Translator, translate_program


class TestTranslateExp(unittest.TestCase):
    def test_constant_returns_tac_constant_emits_nothing(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(c99_ast.Constant(value=42), instrs)
        self.assertEqual(result, tac_ast.Constant(value=42))
        self.assertEqual(instrs, [])

    def test_unary_emits_instruction_and_returns_dst_var(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Unary(
                unary_operator=c99_ast.Negate(),
                exp=c99_ast.Constant(value=5),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(
            instrs,
            [tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Constant(value=5),
                dst=tac_ast.Var(name="%0"),
            )],
        )

    def test_nested_unary_chains_temps(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Unary(
                unary_operator=c99_ast.Negate(),
                exp=c99_ast.Unary(
                    unary_operator=c99_ast.Complement(),
                    exp=c99_ast.Constant(value=5),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%1"))
        self.assertEqual(instrs, [
            tac_ast.Unary(
                op=tac_ast.Complement(),
                src=tac_ast.Constant(value=5),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Var(name="%0"),
                dst=tac_ast.Var(name="%1"),
            ),
        ])


class TestTranslateProgram(unittest.TestCase):
    def test_return_constant(self):
        prog = c99_ast.Program(function_definition=c99_ast.Function(
            name="main",
            body=c99_ast.Return(exp=c99_ast.Constant(value=42)),
        ))
        self.assertEqual(
            translate_program(prog),
            tac_ast.Program(function_definition=tac_ast.Function(
                name="main",
                instructions=[tac_ast.Ret(val=tac_ast.Constant(value=42))],
            )),
        )

    def test_return_unary(self):
        tac = translate_program(c99_ast.Program(
            function_definition=c99_ast.Function(
                name="main",
                body=c99_ast.Return(exp=c99_ast.Unary(
                    unary_operator=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=5),
                )),
            ),
        ))
        self.assertEqual(
            tac.function_definition.instructions,
            [
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Constant(value=5),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%0")),
            ],
        )

    def test_end_to_end_nested_from_source(self):
        tac = translate_program(parse("int main(void) { return -(~5); }"))
        self.assertEqual(
            tac.function_definition.instructions,
            [
                tac_ast.Unary(
                    op=tac_ast.Complement(),
                    src=tac_ast.Constant(value=5),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="%1"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%1")),
            ],
        )

    def test_counter_resets_per_translate_program_call(self):
        # translate_program uses a fresh Translator each call, so both
        # programs get %0 as the first temporary.
        src = "int main(void) { return -5; }"
        a = translate_program(parse(src))
        b = translate_program(parse(src))
        self.assertEqual(a.function_definition.instructions[0].dst,
                         tac_ast.Var(name="%0"))
        self.assertEqual(b.function_definition.instructions[0].dst,
                         tac_ast.Var(name="%0"))


class TestMakeTemporaryVariableName(unittest.TestCase):
    def test_sequential_counter(self):
        t = Translator()
        self.assertEqual(t.make_temporary_variable_name(), "%0")
        self.assertEqual(t.make_temporary_variable_name(), "%1")
        self.assertEqual(t.make_temporary_variable_name(), "%2")


if __name__ == "__main__":
    unittest.main()
