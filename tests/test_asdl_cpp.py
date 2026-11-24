"""Tests for `asdl.py`'s C++ generator (`asdl.py --cpp`)."""
from __future__ import annotations

import os
import textwrap
import unittest

import asdl


_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_MODULES = ("asm", "asm2", "tac", "c99")


def _generate(text: str) -> dict[str, str]:
    return asdl.generate_cpp(asdl.parse(textwrap.dedent(text)), "t.asdl", "t_ast")


class TestCheckedInFiles(unittest.TestCase):
    def test_regenerating_gives_the_checked_in_files(self):
        # After editing an .asdl file, regenerate with
        # `uv run python asdl.py --cpp <name>.asdl cpp/generated`.
        for stem in _MODULES:
            path = os.path.join(_ROOT, f"{stem}.asdl")
            with open(path, encoding="utf-8") as f:
                module = asdl.parse(f.read())
            files = asdl.generate_cpp(
                module, f"{stem}.asdl", asdl.cpp_module_name(path),
            )
            self.assertEqual(
                sorted(files), [f"{stem}_ast.cpp", f"{stem}_ast.h"],
            )
            for name, text in files.items():
                with self.subTest(file=name):
                    with open(os.path.join(_ROOT, "cpp", "generated", name),
                              "rb") as f:
                        self.assertEqual(f.read().decode("utf-8"), text)


class TestGenerator(unittest.TestCase):
    def test_dependencies_come_first(self):
        header = _generate("""
            module T {
                program = Program(stmt* body)
                stmt = Assign(identifier name, expr value) | Pass
                expr = Num(int n) | Neg(expr operand) | Paren(expr? inner)
            }
        """)["t_ast.h"]
        self.assertLess(header.index("using Type_expr ="),
                        header.index("using Type_stmt ="))
        self.assertLess(header.index("using Type_stmt ="),
                        header.index("using Type_program ="))
        # A field in a cycle with its struct is boxed, an optional one
        # inside std::optional, and one of an earlier type is held by
        # value.
        self.assertIn("::c6502::asdl::Box<Type_expr> operand;", header)
        self.assertIn(
            "std::optional<::c6502::asdl::Box<Type_expr>> inner;", header,
        )
        self.assertIn("  Type_expr value;", header)
        self.assertIn("std::vector<Type_stmt> body;", header)
        self.assertIn("enum class Kind_expr : std::uint8_t {", header)

    def test_keywords_get_an_underscore(self):
        header = _generate("""
            module T { c = Constant(const, int default) const = K }
        """)["t_ast.h"]
        self.assertIn("Type_const const_;", header)
        self.assertIn("::c6502::asdl::Int default_ = 0;", header)

    def test_product_types_are_structs(self):
        files = _generate("""
            module T { member = (identifier name, int? size) }
        """)
        self.assertIn("struct Type_member {", files["t_ast.h"])
        self.assertIn("std::optional<::c6502::asdl::Int> size;", files["t_ast.h"])
        self.assertIn('r.expect_name("member");', files["t_ast.cpp"])

    def test_pretty_follows_the_python_classes(self):
        # Each struct gets a `pretty`, and a constructor's fields come in
        # the order of `dataclasses.fields`, which puts the sum type's
        # attributes, on the base class, first.
        files = _generate("""
            module T {
                e = A(int x, e* items) | B attributes (string line)
            }
        """)
        header, source = files["t_ast.h"], files["t_ast.cpp"]
        for name in ("Type_e", "A", "B"):
            self.assertIn(
                f"void pretty(std::string& out, const {name}& v, int depth);",
                header,
            )
        body = source[source.index("void pretty(std::string& out, const A&"):]
        body = body[:body.index("\n}\n")]
        self.assertLess(body.index('"line="'), body.index('"x="'))
        self.assertLess(body.index('"x="'), body.index('"items="'))
        self.assertIn('out += "[]";', body)
        self.assertIn("::c6502::pcpp::py_repr(v.line)", body)

    def test_rejects_types_with_no_cpp_form(self):
        with self.assertRaises(asdl.ASDLError):
            _generate("module T { x = X(float f) }")

    def test_python_output_is_unchanged(self):
        for stem in _MODULES:
            with self.subTest(module=stem):
                with open(os.path.join(_ROOT, f"{stem}.asdl"),
                          encoding="utf-8") as f:
                    code = asdl.generate(asdl.parse(f.read()),
                                         source=f"{stem}.asdl")
                with open(os.path.join(_ROOT, f"{stem}_ast.py"), "rb") as f:
                    self.assertEqual(f.read().decode("utf-8"), code)


if __name__ == "__main__":
    unittest.main()
