"""Behavioral tests for the `register` storage-class keyword.

`register` replaces the old `[[c6502::reg("..")]]` extension as
the way to request 6502-register placement for parameters, locals, and
(type-driven) return values. Register assignment is POSITIONAL — the
first register arg-byte goes in A, the second in X; a `register` local
uses Y. The parser only records the request; abi_selection / the
optimizer realize it.

Coverage:
  - A `register` parameter sets that parameter's marker in
    `function_decl.param_registers` (parallel to `params`: "auto" for
    `register`, "" otherwise).
  - A `register` local carries `storage_class == Register()` on its
    `Type_var_decl`.
  - `register` is rejected on a function declarator and (later) at file
    scope; the old `[[c6502::reg(...)]]` form is no longer
    accepted.
  - `zp_abi` (the surviving function-level attribute) still parses.
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


def _var_decls(src: str) -> list[c99_ast.Type_var_decl]:
    prog = parse(src)
    return [
        d.var_decl for d in prog.declaration
        if isinstance(d, c99_ast.VarDecl)
    ]


class TestRegisterKeywordParsing(unittest.TestCase):
    def test_register_param_marker(self) -> None:
        decls = _function_decls(
            "char f(register char a, char b, register char c);"
        )
        self.assertEqual(decls[0].params, ["a", "b", "c"])
        self.assertEqual(decls[0].param_registers, ["auto", "", "auto"])

    def test_no_register_params(self) -> None:
        decls = _function_decls("char f(char a, char b);")
        self.assertEqual(decls[0].param_registers, ["", ""])

    def test_register_local_storage_class(self) -> None:
        # `register` on a block-scope local rides on storage_class.
        prog = parse(
            "int main(void) { register char i; i = 0; return i; }"
        )
        # Pull the var_decl out of main's body.
        fn = prog.declaration[0].function_decl
        from c99_ast import D, VarDecl
        vds = [
            bi.declaration.var_decl
            for bi in fn.body.block_item
            if isinstance(bi, D) and isinstance(bi.declaration, VarDecl)
        ]
        self.assertEqual(vds[0].name, "i")
        self.assertIsInstance(vds[0].storage_class, c99_ast.Register)

    def test_register_param_definition(self) -> None:
        decls = _function_decls(
            "char f(register char a, register char b) "
            "{ return (char)(a + b); }"
        )
        self.assertEqual(decls[0].param_registers, ["auto", "auto"])
        self.assertIsNotNone(decls[0].body)

    def test_zp_abi_still_parses(self) -> None:
        decls = _function_decls(
            "[[c6502::zp_abi]] char f(register char a);"
        )
        self.assertEqual(decls[0].abi_annotation, "zp_abi")
        self.assertEqual(decls[0].param_registers, ["auto"])

    def test_register_on_function_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse("register int foo(int x);")
        self.assertIn("register", str(cm.exception))

    def test_old_reg_attribute_rejected(self) -> None:
        # The `[[c6502::reg(...)]]` extension is gone.
        with self.assertRaises(Exception):
            parse('char f(char x [[c6502::reg("X")]]);')

    def test_static_on_parameter_rejected(self) -> None:
        # `register` is the only storage class allowed on a parameter.
        with self.assertRaises(ParserError) as cm:
            parse("char f(static char x);")
        self.assertIn("storage class", str(cm.exception))

    def test_unknown_attribute_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse("[[c6502::funky]] char f(void);")
        self.assertIn("unknown attribute `c6502::funky`", str(cm.exception))


if __name__ == "__main__":
    unittest.main()
