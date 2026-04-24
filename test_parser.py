import shutil
import subprocess
import unittest
from pathlib import Path

from lark.exceptions import UnexpectedInput

import c99_ast
from lexer import LexError
from parser import parse


_TESTS_DIR = Path(__file__).parent / "tests"


def _preprocess(src: str) -> str:
    result = subprocess.run(
        ["pcpp", "-", "--line-directive"],
        input=src, capture_output=True, text=True, check=True,
    )
    return result.stdout


class TestParser(unittest.TestCase):
    def test_minimal_function(self):
        ast = parse("int main(void) { return 42; }")
        expected = c99_ast.Program(
            function_definition=c99_ast.Function(
                name="main",
                body=c99_ast.Return(exp=c99_ast.Constant(value=42)),
            ),
        )
        self.assertEqual(ast, expected)

    def test_whitespace_insensitive(self):
        for src in [
            "int main(void){return 42;}",
            "int  main  ( void )  {  return  42  ;  }",
            "int\nmain(void)\n{\n    return 42;\n}",
        ]:
            with self.subTest(src=src):
                ast = parse(src)
                self.assertEqual(ast.function_definition.body.exp.value, 42)

    def test_various_return_values(self):
        for val in [0, 1, 42, 255, 1000, 0xDEADBEEF]:
            with self.subTest(val=val):
                ast = parse(f"int main(void) {{ return {val}; }}")
                self.assertEqual(ast.function_definition.body.exp.value, val)

    def test_function_name_captured(self):
        for name in ["main", "foo", "_start", "a1b2"]:
            with self.subTest(name=name):
                ast = parse(f"int {name}(void) {{ return 0; }}")
                self.assertEqual(ast.function_definition.name, name)

    def test_unary_negate(self):
        ast = parse("int main(void) { return -42; }")
        self.assertEqual(
            ast.function_definition.body,
            c99_ast.Return(exp=c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Constant(value=42),
            )),
        )

    def test_unary_complement(self):
        ast = parse("int main(void) { return ~10; }")
        self.assertEqual(
            ast.function_definition.body,
            c99_ast.Return(exp=c99_ast.Unary(
                op=c99_ast.Complement(),
                exp=c99_ast.Constant(value=10),
            )),
        )

    def test_parens_do_not_appear_in_ast(self):
        ast = parse("int main(void) { return (42); }")
        self.assertEqual(
            ast.function_definition.body,
            c99_ast.Return(exp=c99_ast.Constant(value=42)),
        )

    def test_nested_unary(self):
        ast = parse("int main(void) { return -(-42); }")
        self.assertEqual(
            ast.function_definition.body.exp,
            c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=42),
                ),
            ),
        )

    def test_mixed_unary_with_parens(self):
        ast = parse("int main(void) { return ~(-5); }")
        self.assertEqual(
            ast.function_definition.body.exp,
            c99_ast.Unary(
                op=c99_ast.Complement(),
                exp=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=5),
                ),
            ),
        )

    def test_returned_ast_types(self):
        ast = parse("int main(void) { return 0; }")
        self.assertIsInstance(ast, c99_ast.Type_program)
        self.assertIsInstance(ast, c99_ast.Program)
        self.assertIsInstance(ast.function_definition, c99_ast.Type_function_definition)
        self.assertIsInstance(ast.function_definition, c99_ast.Function)
        self.assertIsInstance(ast.function_definition.body, c99_ast.Type_statement)
        self.assertIsInstance(ast.function_definition.body, c99_ast.Return)
        self.assertIsInstance(ast.function_definition.body.exp, c99_ast.Type_exp)
        self.assertIsInstance(ast.function_definition.body.exp, c99_ast.Constant)


def _exp_of(src: str) -> c99_ast.Type_exp:
    return parse(f"int main(void) {{ return {src}; }}").function_definition.body.exp


class TestBinaryPrecedence(unittest.TestCase):
    def test_add_then_multiply_groups_multiply(self):
        # 1 + 2 * 3  ->  +(1, *(2, 3))
        # The * has two int children; the + has int left + Binary right.
        self.assertEqual(
            _exp_of("1 + 2 * 3"),
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_multiply_then_add_groups_multiply(self):
        # 1 * 2 + 3  ->  +(*(1, 2), 3)
        # The * has two int children; the + has Binary left + int right.
        self.assertEqual(
            _exp_of("1 * 2 + 3"),
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_multiply_add_multiply(self):
        # 1 * 2 + 3 * 4  ->  +(*(1, 2), *(3, 4))
        # Both * nodes have two int children; the + has Binary on
        # both sides.
        self.assertEqual(
            _exp_of("1 * 2 + 3 * 4"),
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=3),
                    right=c99_ast.Constant(value=4),
                ),
            ),
        )

    def test_left_associative_subtract(self):
        # 1 - 2 - 3  ->  -(-(1, 2), 3)
        self.assertEqual(
            _exp_of("1 - 2 - 3"),
            c99_ast.Binary(
                op=c99_ast.Subtract(),
                left=c99_ast.Binary(
                    op=c99_ast.Subtract(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_left_associative_divide(self):
        # 1 / 2 / 3  ->  /(/(1, 2), 3)
        self.assertEqual(
            _exp_of("1 / 2 / 3"),
            c99_ast.Binary(
                op=c99_ast.Divide(),
                left=c99_ast.Binary(
                    op=c99_ast.Divide(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_parens_override_precedence(self):
        # (1 + 2) * 3  ->  *(+(1, 2), 3)
        self.assertEqual(
            _exp_of("(1 + 2) * 3"),
            c99_ast.Binary(
                op=c99_ast.Multiply(),
                left=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_unary_binds_tighter_than_multiply(self):
        # -1 * 2  ->  *(-1, 2)
        self.assertEqual(
            _exp_of("-1 * 2"),
            c99_ast.Binary(
                op=c99_ast.Multiply(),
                left=c99_ast.Unary(
                    op=c99_ast.Negate(), exp=c99_ast.Constant(value=1),
                ),
                right=c99_ast.Constant(value=2),
            ),
        )

    def test_modulo(self):
        # 10 % 3  ->  %(10, 3)
        self.assertEqual(
            _exp_of("10 % 3"),
            c99_ast.Binary(
                op=c99_ast.Modulo(),
                left=c99_ast.Constant(value=10),
                right=c99_ast.Constant(value=3),
            ),
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestValidFiles(unittest.TestCase):
    """Each file in tests/valid/ must parse into an AST for `int main(void)`
    with a Return of an integer Constant. Most files have comments, so we
    pipe through pcpp first."""

    def test_each_valid_file_parses(self):
        paths = sorted((_TESTS_DIR / "valid").glob("*.c"))
        self.assertGreater(len(paths), 0, "no valid/*.c files")
        for path in paths:
            with self.subTest(file=path.name):
                ast = parse(_preprocess(path.read_text()))
                self.assertIsInstance(ast, c99_ast.Program)
                self.assertEqual(ast.function_definition.name, "main")
                self.assertIsInstance(ast.function_definition.body, c99_ast.Return)
                self.assertIsInstance(
                    ast.function_definition.body.exp, c99_ast.Constant,
                )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestInvalidParseFiles(unittest.TestCase):
    """Each file in tests/invalid_parse/ must fail parsing (either at lex
    time or parse time)."""

    def test_each_invalid_parse_file_fails(self):
        paths = sorted((_TESTS_DIR / "invalid_parse").glob("*.c"))
        self.assertGreater(len(paths), 0, "no invalid_parse/*.c files")
        for path in paths:
            with self.subTest(file=path.name):
                src = _preprocess(path.read_text())
                with self.assertRaises((LexError, UnexpectedInput)):
                    parse(src)


if __name__ == "__main__":
    unittest.main()
