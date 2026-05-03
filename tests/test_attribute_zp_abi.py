"""Behavioral tests for `__attribute__((zp_abi))` parsing.

Coverage:
  - Forward declaration with the annotation: `abi_annotation`
    field on the resulting `Type_function_decl` is `"zp_abi"`.
  - Function definition with the annotation: same.
  - Function declarations without the annotation: field is None.
  - Object declarations: annotation rejected at parse time.
  - Tag-only declarations (`struct foo;`): annotation rejected.
  - Unknown attribute name: rejected.
"""
from __future__ import annotations

import unittest

import c99_ast
from parser import ParserError, parse


def _function_decls(src: str) -> list[c99_ast.Type_function_decl]:
    prog = parse(src)
    return [
        d.function_decl for d in prog.declaration
        if isinstance(d, c99_ast.FunctionDecl)
    ]


class TestAttributeZpAbiParsing(unittest.TestCase):
    def test_forward_decl_with_annotation(self) -> None:
        decls = _function_decls("__attribute__((zp_abi)) int f(int x);")
        self.assertEqual(len(decls), 1)
        self.assertEqual(decls[0].name, "f")
        self.assertIsNone(decls[0].body)
        self.assertEqual(decls[0].abi_annotation, "zp_abi")

    def test_function_definition_with_annotation(self) -> None:
        decls = _function_decls(
            "__attribute__((zp_abi)) int g(int x) { return x + 1; }",
        )
        self.assertEqual(len(decls), 1)
        self.assertEqual(decls[0].name, "g")
        self.assertIsNotNone(decls[0].body)
        self.assertEqual(decls[0].abi_annotation, "zp_abi")

    def test_function_decl_without_annotation_has_none(self) -> None:
        decls = _function_decls("int h(int x) { return x; }")
        self.assertEqual(decls[0].abi_annotation, None)

    def test_unknown_attribute_name_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse("__attribute__((wrong)) int f(int x);")
        self.assertIn("unknown attribute name", str(cm.exception))
        self.assertIn("wrong", str(cm.exception))

    def test_attribute_on_object_decl_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse("__attribute__((zp_abi)) int x;")
        self.assertIn("only valid on function declarations", str(cm.exception))

    def test_attribute_on_tag_only_decl_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse("__attribute__((zp_abi)) struct foo;")
        self.assertIn("only valid on function declarations", str(cm.exception))


if __name__ == "__main__":
    unittest.main()
