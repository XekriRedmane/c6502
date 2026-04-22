import textwrap
import unittest

from asdl import ASDLError, Constructor, Field, Module, Type, generate, parse


def dedent(s: str) -> str:
    return textwrap.dedent(s).strip() + "\n"


class TestParser(unittest.TestCase):
    def test_empty_module(self):
        m = parse("module Foo {}")
        self.assertEqual(m.name, "Foo")
        self.assertEqual(m.types, [])

    def test_product_type(self):
        m = parse("module F { point = (int x, int y) }")
        t = m.types[0]
        self.assertEqual(t.name, "point")
        self.assertTrue(t.is_product)
        self.assertEqual(len(t.fields), 2)
        self.assertEqual((t.fields[0].type, t.fields[0].name), ("int", "x"))
        self.assertEqual((t.fields[1].type, t.fields[1].name), ("int", "y"))

    def test_product_empty_fields(self):
        m = parse("module F { unit = () }")
        self.assertEqual(m.types[0].fields, [])

    def test_enum_like_sum(self):
        m = parse("module F { op = Add | Sub | Mul }")
        t = m.types[0]
        self.assertFalse(t.is_product)
        self.assertEqual([c.name for c in t.constructors], ["Add", "Sub", "Mul"])
        self.assertTrue(all(c.fields == [] for c in t.constructors))

    def test_sum_with_fields(self):
        m = parse(dedent("""
            module F {
                expr = Num(int value)
                     | BinOp(expr left, op op, expr right)
            }
        """))
        t = m.types[0]
        self.assertEqual(len(t.constructors), 2)
        self.assertEqual(t.constructors[0].fields[0].name, "value")
        self.assertEqual(
            [f.name for f in t.constructors[1].fields],
            ["left", "op", "right"],
        )

    def test_field_modifiers(self):
        m = parse("module F { x = Foo(expr? a, expr* b, expr c) }")
        fs = m.types[0].constructors[0].fields
        self.assertEqual((fs[0].optional, fs[0].sequence), (True, False))
        self.assertEqual((fs[1].optional, fs[1].sequence), (False, True))
        self.assertEqual((fs[2].optional, fs[2].sequence), (False, False))

    def test_attributes(self):
        m = parse(dedent("""
            module F {
                stmt = Return | Break
                     attributes (int lineno, int col)
            }
        """))
        t = m.types[0]
        self.assertEqual([f.name for f in t.fields], ["lineno", "col"])
        self.assertEqual([c.name for c in t.constructors], ["Return", "Break"])

    def test_line_comments(self):
        m = parse(dedent("""
            -- head comment
            module F {
                -- inside
                foo = (int x) -- trailing
                -- between
                bar = Baz
            }
            -- tail
        """))
        self.assertEqual([t.name for t in m.types], ["foo", "bar"])

    def test_unnamed_field(self):
        m = parse("module F { x = Foo(int, string) }")
        fs = m.types[0].constructors[0].fields
        self.assertIsNone(fs[0].name)
        self.assertIsNone(fs[1].name)
        self.assertEqual(fs[0].type, "int")

    def test_missing_module_keyword(self):
        with self.assertRaises(ASDLError):
            parse("Foo {}")

    def test_missing_brace(self):
        with self.assertRaises(ASDLError):
            parse("module F")

    def test_uppercase_type_name_rejected(self):
        with self.assertRaises(ASDLError):
            parse("module F { Foo = X }")

    def test_lowercase_constructor_rejected(self):
        with self.assertRaises(ASDLError):
            parse("module F { foo = bar }")

    def test_uppercase_field_type_rejected(self):
        with self.assertRaises(ASDLError):
            parse("module F { foo = X(Int value) }")

    def test_constructor_field_can_be_primitive(self):
        # Primitive types are lowercase, so they pass the type-ref check.
        m = parse("module F { foo = X(int a, string b, identifier c) }")
        self.assertEqual([f.type for f in m.types[0].constructors[0].fields],
                         ["int", "string", "identifier"])

    def test_type_after_attributes_parses(self):
        # Ensures the 'attributes' lookahead rewinds correctly when the next
        # identifier is actually the start of the next type definition.
        m = parse(dedent("""
            module F {
                a = X
                b = Y
            }
        """))
        self.assertEqual([t.name for t in m.types], ["a", "b"])


class TestGenerator(unittest.TestCase):
    def _exec(self, src: str) -> dict:
        code = generate(parse(src))
        ns: dict = {}
        exec(code, ns)
        return ns

    def test_product(self):
        ns = self._exec("module F { point = (int x, int y) }")
        p = ns["Type_point"](1, 2)
        self.assertEqual((p.x, p.y), (1, 2))

    def test_sum_inheritance(self):
        ns = self._exec(dedent("""
            module F {
                expr = Num(int value)
                     | Neg(expr e)
            }
        """))
        num = ns["Num"](42)
        neg = ns["Neg"](num)
        self.assertIsInstance(num, ns["Type_expr"])
        self.assertIsInstance(neg, ns["Type_expr"])
        self.assertEqual(neg.e.value, 42)

    def test_optional_field_defaults_to_none(self):
        ns = self._exec("module F { x = X(int? v) }")
        self.assertIsNone(ns["X"]().v)
        self.assertEqual(ns["X"](7).v, 7)

    def test_sequence_field_defaults_to_empty_list(self):
        ns = self._exec("module F { x = X(int* v) }")
        self.assertEqual(ns["X"]().v, [])
        self.assertEqual(ns["X"]([1, 2, 3]).v, [1, 2, 3])
        # Distinct default instances (no shared mutable default).
        a, b = ns["X"](), ns["X"]()
        a.v.append(9)
        self.assertEqual(b.v, [])

    def test_attributes_inherited_kw_only(self):
        ns = self._exec(dedent("""
            module F {
                stmt = Ret(int v)
                     attributes (int lineno)
            }
        """))
        r = ns["Ret"](3, lineno=10)
        self.assertEqual((r.v, r.lineno), (3, 10))
        # Primitive attribute has a default so lineno can be omitted.
        self.assertEqual(ns["Ret"](3).lineno, 0)

    def test_enum_like_sum_instances(self):
        ns = self._exec("module F { op = Add | Sub }")
        self.assertIsInstance(ns["Add"](), ns["Type_op"])
        self.assertIsInstance(ns["Sub"](), ns["Type_op"])
        self.assertNotIsInstance(ns["Add"](), ns["Sub"])

    def test_identifier_maps_to_str(self):
        ns = self._exec("module F { x = X(identifier name) }")
        self.assertEqual(ns["X"]("foo").name, "foo")

    def test_mutual_recursion(self):
        # Forward references work due to `from __future__ import annotations`.
        ns = self._exec(dedent("""
            module F {
                a = A(b b)
                b = B(a a)
            }
        """))
        b = ns["B"](None)
        a = ns["A"](b)
        self.assertIs(a.b, b)

    def test_unnamed_field_uses_type_name(self):
        ns = self._exec("module F { x = X(int) }")
        self.assertEqual(ns["X"](5).int, 5)

    def test_generated_output_shape(self):
        code = generate(parse("module F { point = (int x, int y) }"))
        self.assertIn("from __future__ import annotations", code)
        self.assertIn("from dataclasses import dataclass, field", code)
        self.assertIn("class Type_point:", code)
        self.assertIn("    x: int", code)

    def test_type_prefix_for_user_type_reference(self):
        ns = self._exec(dedent("""
            module F {
                expr = Num(int value)
                     | Wrap(expr inner)
            }
        """))
        # Field `inner` should be annotated Type_expr (not `expr`).
        self.assertEqual(ns["Wrap"].__annotations__["inner"], "Type_expr")
        # But primitive `int` stays `int`, no prefix.
        self.assertEqual(ns["Num"].__annotations__["value"], "int")


class TestRealistic(unittest.TestCase):
    """A small C-ish AST to sanity-check a realistic mixture of features."""

    SRC = dedent("""
        module C99 {
            -- primitive operators
            binop = Add | Sub | Mul | Div

            -- expressions
            expr = IntConst(int value)
                 | Var(identifier name)
                 | Binary(expr left, binop op, expr right)
                 attributes (int line, int col)

            -- statements
            stmt = ExprStmt(expr e)
                 | Return(expr? value)
                 | Block(stmt* body)

            -- top-level function
            func = (identifier name, stmt* body)
        }
    """)

    def test_builds(self):
        code = generate(parse(self.SRC))
        ns: dict = {}
        exec(code, ns)
        add = ns["Add"]()
        x = ns["Var"]("x", line=1, col=1)
        self.assertIsInstance(x, ns["Type_expr"])
        b = ns["Binary"](x, add, ns["IntConst"](1, line=1, col=3), line=1, col=2)
        self.assertIsInstance(b.op, ns["Type_binop"])
        f = ns["Type_func"]("main", [ns["Return"](b), ns["Return"]()])
        self.assertEqual(f.name, "main")
        self.assertEqual(len(f.body), 2)
        self.assertIsNone(f.body[1].value)


if __name__ == "__main__":
    unittest.main()
