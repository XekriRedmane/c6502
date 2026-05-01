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
        got = pretty(c99_ast.ConstInt(value=42))
        self.assertEqual(got, "ConstInt(\n  value=42,\n)")

    def test_empty_dataclass_inline(self):
        @dataclass
        class Empty:
            pass
        self.assertEqual(pretty(Empty()), "Empty()")

    def test_nested_ast(self):
        # `Program.declaration` is a list, so pretty-print walks into
        # a list-of-FunctionDecl and indents one level deeper.
        # `Type_function_decl` carries `params`, `body`, `data_type`
        # (here a FunType), and `storage_class`. Constants are wrapped:
        # `Constant(const=ConstInt(value=...))`.
        node = c99_ast.Program(
            declaration=[c99_ast.FunctionDecl(
                function_decl=c99_ast.Type_function_decl(
                    name="main",
                    params=[],
                    body=c99_ast.Return(exp=c99_ast.Constant(
                        const=c99_ast.ConstInt(value=0),
                    )),
                    data_type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
                    storage_class=None,
                ),
            )],
        )
        expected = textwrap.dedent("""\
            Program(
              declaration=[
                FunctionDecl(
                  function_decl=Type_function_decl(
                    name='main',
                    params=[],
                    body=Return(
                      exp=Constant(
                        const=ConstInt(
                          value=0,
                        ),
                        data_type=None,
                      ),
                    ),
                    data_type=FunType(
                      params=[],
                      ret=Int(),
                    ),
                    storage_class=None,
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
            declaration=[c99_ast.FunctionDecl(
                function_decl=c99_ast.Type_function_decl(
                    name="main",
                    body=c99_ast.Return(exp=c99_ast.Constant(
                        const=c99_ast.ConstInt(value=42),
                    )),
                    data_type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
                    storage_class=None,
                ),
            )],
        )
        ns = {
            "Program": c99_ast.Program,
            "FunctionDecl": c99_ast.FunctionDecl,
            "Type_function_decl": c99_ast.Type_function_decl,
            "Return": c99_ast.Return,
            "Constant": c99_ast.Constant,
            "ConstInt": c99_ast.ConstInt,
            "FunType": c99_ast.FunType,
            "Int": c99_ast.Int,
        }
        reconstructed = eval(pretty(node), ns)
        self.assertEqual(reconstructed, node)


if __name__ == "__main__":
    unittest.main()
