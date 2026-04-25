import textwrap
import unittest
from dataclasses import dataclass, field

import c99_ast
from pretty import pretty


class TestPretty(unittest.TestCase):
    def test_primitives(self):
        self.assertEqual(pretty(42), "42")
        self.assertEqual(pretty(0), "0")
        self.assertEqual(pretty(True), "True")
        self.assertEqual(pretty(None), "None")
        self.assertEqual(pretty("hi"), "'hi'")

    def test_leaf_dataclass(self):
        got = pretty(c99_ast.Constant(value=42))
        self.assertEqual(got, "Constant(\n  value=42,\n)")

    def test_empty_dataclass_inline(self):
        @dataclass
        class Empty:
            pass
        self.assertEqual(pretty(Empty()), "Empty()")

    def test_nested_ast(self):
        # `function_definition` is a list, so pretty-print walks into
        # a list-of-Function and indents one level deeper. Function
        # also carries `params` (a list of strings, empty here for
        # `void`), which appears between `name` and `body`.
        node = c99_ast.Program(
            function_definition=[c99_ast.Function(
                name="main",
                params=[],
                body=c99_ast.Return(exp=c99_ast.Constant(value=0)),
            )],
        )
        expected = textwrap.dedent("""\
            Program(
              function_definition=[
                Function(
                  name='main',
                  params=[],
                  body=Return(
                    exp=Constant(
                      value=0,
                    ),
                  ),
                ),
              ],
            )""")
        self.assertEqual(pretty(node), expected)

    def test_list_field(self):
        @dataclass
        class Box:
            items: list

        self.assertEqual(pretty(Box(items=[])), "Box(\n  items=[],\n)")
        self.assertEqual(
            pretty(Box(items=[1, 2, 3])),
            "Box(\n  items=[\n    1,\n    2,\n    3,\n  ],\n)",
        )

    def test_list_of_dataclasses(self):
        @dataclass
        class Leaf:
            v: int

        @dataclass
        class Container:
            xs: list

        got = pretty(Container(xs=[Leaf(v=1), Leaf(v=2)]))
        expected = textwrap.dedent("""\
            Container(
              xs=[
                Leaf(
                  v=1,
                ),
                Leaf(
                  v=2,
                ),
              ],
            )""")
        self.assertEqual(got, expected)

    def test_output_is_valid_python(self):
        node = c99_ast.Program(
            function_definition=[c99_ast.Function(
                name="main",
                body=c99_ast.Return(exp=c99_ast.Constant(value=42)),
            )],
        )
        ns = {
            "Program": c99_ast.Program,
            "Function": c99_ast.Function,
            "Return": c99_ast.Return,
            "Constant": c99_ast.Constant,
        }
        reconstructed = eval(pretty(node), ns)
        self.assertEqual(reconstructed, node)


if __name__ == "__main__":
    unittest.main()
