"""Tests for `passes.string_lifting`.

The lifting pass hoists every String literal whose context is NOT
a direct char-array initializer into a fresh file-scope `static
char[N+1]` declaration, replacing the original String with a Var
referencing the new declaration. After lifting, the rest of the
pipeline treats string literals just like ordinary file-scope
char-array statics — array decay, AddressOf, subscript, etc., all
fall out for free.
"""

from __future__ import annotations

import unittest

import c99_ast
from parser import parse
from passes.identifier_resolution import (
    resolve_program as resolve_identifiers,
)
from passes.string_lifting import lift_program


def _lift(src: str) -> c99_ast.Type_program:
    return lift_program(resolve_identifiers(parse(src)))


class TestStringLiftingDirect(unittest.TestCase):
    def test_string_in_pointer_init_is_hoisted(self):
        # `static char *p = "abc";` — String becomes a file-scope
        # static array; `p` initialises with a Var referencing it.
        prog = _lift('static char *p = "abc";')
        # Two top-level decls: the lifted .str@0, then the original p.
        names = [d.var_decl.name for d in prog.declaration]
        self.assertEqual(names, [".str@0", "p"])
        # The lifted decl is a static char[4] (3 chars + null).
        s_decl = prog.declaration[0].var_decl
        self.assertIsInstance(s_decl.storage_class, c99_ast.Static)
        self.assertEqual(s_decl.data_type, c99_ast.Array(
            element_type=c99_ast.Char(), size=4,
        ))
        self.assertEqual(s_decl.init, c99_ast.String(str="abc"))
        # `p`'s init is now a Var pointing at the lifted decl.
        p_decl = prog.declaration[1].var_decl
        self.assertEqual(p_decl.init, c99_ast.Var(name=".str@0"))

    def test_string_in_char_array_init_stays_inline(self):
        # `char arr[4] = "abc";` — String stays inline; no lift.
        prog = _lift("char arr[4] = \"abc\";")
        self.assertEqual(len(prog.declaration), 1)
        self.assertEqual(prog.declaration[0].var_decl.name, "arr")
        self.assertIsInstance(
            prog.declaration[0].var_decl.init, c99_ast.String,
        )

    def test_string_in_signed_char_array_stays_inline(self):
        prog = _lift("signed char arr[4] = \"abc\";")
        self.assertEqual(len(prog.declaration), 1)
        self.assertIsInstance(
            prog.declaration[0].var_decl.init, c99_ast.String,
        )

    def test_string_in_unsigned_char_array_stays_inline(self):
        prog = _lift("unsigned char arr[4] = \"abc\";")
        self.assertEqual(len(prog.declaration), 1)
        self.assertIsInstance(
            prog.declaration[0].var_decl.init, c99_ast.String,
        )

    def test_string_in_int_array_init_is_hoisted(self):
        # An `int[]` with a String init is a malformed C program,
        # but the lifter only knows about array-element-type, not
        # the standard's rules. The type checker is the right
        # place to reject this. The lifter still hoists the String
        # so the type checker fails on the lifted-Var-not-an-array
        # mismatch rather than on the raw String.
        prog = _lift("int arr[4] = \"abc\";")
        # 2 decls — the lifted .str@0 and the original `arr`.
        self.assertEqual(len(prog.declaration), 2)


class TestStringLiftingInExpressions(unittest.TestCase):
    def test_address_of_string_lifts(self):
        # `&"abc"` — the String is inside AddressOf, not a direct
        # array init, so it gets hoisted.
        prog = _lift("char (*p)[4] = &\"abc\";")
        names = [d.var_decl.name for d in prog.declaration]
        self.assertEqual(names, [".str@0", "p"])

    def test_string_in_return_lifts(self):
        prog = _lift(
            "char *get(void) { return \"hi\"; }"
        )
        # 2 decls: lifted .str@0, then `get` (function).
        self.assertEqual(len(prog.declaration), 2)
        self.assertEqual(prog.declaration[0].var_decl.name, ".str@0")

    def test_string_in_subscript_lifts(self):
        prog = _lift(
            "int main(void) { return \"abc\"[0]; }"
        )
        # 2 decls: lifted .str@0, then `main`.
        self.assertEqual(len(prog.declaration), 2)
        names = [
            d.var_decl.name if isinstance(d, c99_ast.VarDecl)
            else d.function_decl.name
            for d in prog.declaration
        ]
        self.assertEqual(names, [".str@0", "main"])


class TestStringLiftingMultiple(unittest.TestCase):
    def test_multiple_strings_get_unique_names(self):
        prog = _lift(
            "char *a = \"first\"; char *b = \"second\";"
        )
        # Three top-level decls: two lifted statics, plus `a`, `b`.
        # Wait — that's four. The lifted statics ride at the top.
        var_decls = [
            d.var_decl for d in prog.declaration
            if isinstance(d, c99_ast.VarDecl)
        ]
        names = [d.name for d in var_decls]
        self.assertEqual(names, [".str@0", ".str@1", "a", "b"])

    def test_string_inside_function_body(self):
        # The lifted decls go to file scope, not stay in the body.
        prog = _lift(
            "int main(void) { "
            "  char *p = \"hello\"; "
            "  return p[0]; "
            "}"
        )
        # First decl is the lifted `.str@0`; second is `main`.
        self.assertIsInstance(prog.declaration[0], c99_ast.VarDecl)
        self.assertEqual(prog.declaration[0].var_decl.name, ".str@0")
        self.assertIsInstance(prog.declaration[1], c99_ast.FunctionDecl)


if __name__ == "__main__":
    unittest.main()
