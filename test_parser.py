import unittest

import c99_ast
from parser import parse


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


if __name__ == "__main__":
    unittest.main()
