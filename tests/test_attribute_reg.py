"""Behavioral tests for `__attribute__((reg("..")))` parsing.

Coverage:
  - Function-level prefix `reg("A"|"X"|"Y")` sets `return_register`
    on the resulting `Type_function_decl`.
  - Per-parameter postfix attribute populates `param_registers` in
    parallel with `params`.
  - Local-variable postfix attribute on `init_declarator` populates
    `register_class` on the resulting `Type_var_decl`.
  - Combined `__attribute__((zp_abi, reg("A")))` parses as a single
    clause carrying both specs.
  - Invalid register names, missing/extra args, and unknown
    attribute names are rejected at parse time.
  - `reg(...)` on an abstract-declarator parameter is rejected
    (no name to bind).
  - Postfix `reg(...)` on a function-typed init-declarator is
    rejected (return-register slot lives on the prefix).
  - `zp_abi` in a postfix position (param / local) is rejected.
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


class TestAttributeRegParsing(unittest.TestCase):
    def test_function_level_return_register_X(self) -> None:
        decls = _function_decls(
            '__attribute__((reg("X"))) char f(char x);'
        )
        self.assertEqual(decls[0].return_register, "X")
        self.assertEqual(decls[0].abi_annotation, None)
        self.assertEqual(decls[0].param_registers, [""])

    def test_function_level_zp_abi_and_return_register_combined(self) -> None:
        decls = _function_decls(
            '__attribute__((zp_abi, reg("A"))) char f(char x);'
        )
        self.assertEqual(decls[0].abi_annotation, "zp_abi")
        self.assertEqual(decls[0].return_register, "A")

    def test_per_parameter_register(self) -> None:
        decls = _function_decls(
            'char f(char x __attribute__((reg("X"))), '
            'char y __attribute__((reg("Y"))), char z);'
        )
        self.assertEqual(decls[0].params, ["x", "y", "z"])
        self.assertEqual(decls[0].param_registers, ["X", "Y", ""])

    def test_local_register_class(self) -> None:
        decls = _var_decls(
            'char i __attribute__((reg("X")));'
        )
        self.assertEqual(decls[0].name, "i")
        self.assertEqual(decls[0].register_class, "X")

    def test_multi_init_declarator_per_var_register_class(self) -> None:
        # Postfix attribute attaches to the specific init-declarator,
        # not the whole declaration. `b` is reg-attributed; `a`/`c`
        # are not.
        decls = _var_decls(
            'char a, b __attribute__((reg("X"))), c;'
        )
        self.assertEqual([d.name for d in decls], ["a", "b", "c"])
        self.assertEqual(
            [d.register_class for d in decls], [None, "X", None],
        )

    def test_definition_carries_attributes(self) -> None:
        # Function definitions accept the same prefix / postfix
        # attribute set as forward declarations.
        decls = _function_decls(
            '__attribute__((zp_abi, reg("A"))) '
            'char f(char x __attribute__((reg("X")))) { return x; }'
        )
        self.assertEqual(decls[0].abi_annotation, "zp_abi")
        self.assertEqual(decls[0].return_register, "A")
        self.assertEqual(decls[0].param_registers, ["X"])
        self.assertIsNotNone(decls[0].body)

    def test_invalid_register_name_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse('__attribute__((reg("Z"))) char f(char x);')
        self.assertIn("register name", str(cm.exception))

    def test_reg_missing_arg_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse('__attribute__((reg)) char f(char x);')
        self.assertIn("requires a string-literal", str(cm.exception))

    def test_zp_abi_with_arg_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse('__attribute__((zp_abi("X"))) char f(char x);')
        self.assertIn("zp_abi", str(cm.exception))

    def test_zp_abi_on_parameter_rejected(self) -> None:
        # zp_abi is a function-level attribute only; rejecting it
        # postfix prevents confusing combinations.
        with self.assertRaises(ParserError) as cm:
            parse('char f(char x __attribute__((zp_abi)));')
        self.assertIn("zp_abi", str(cm.exception))

    def test_zp_abi_on_local_rejected(self) -> None:
        # Same check on init-declarator postfix. We catch it via the
        # function-body parse — locals only appear inside a block.
        with self.assertRaises(ParserError) as cm:
            parse(
                'int main(void) {\n'
                '  char x __attribute__((zp_abi));\n'
                '  return 0;\n'
                '}'
            )
        self.assertIn("zp_abi", str(cm.exception))

    def test_reg_on_abstract_parameter_rejected(self) -> None:
        # Abstract-declarator parameters have no name to bind the
        # register to. The grammar rejects `char __attribute__((..))`
        # in parameter position at the LALR level (the attribute slot
        # is only on the named-declarator alternative), so the
        # failure surfaces as a generic parse error rather than a
        # tailored ParserError. The important behavior is the same:
        # `reg(...)` can't be silently dropped on an abstract param.
        with self.assertRaises(Exception):
            parse('char f(char __attribute__((reg("X"))));')

    def test_postfix_reg_on_function_typed_init_declarator_rejected(self) -> None:
        # `int foo(int) __attribute__((reg("X")));` would shadow the
        # function-level return-register slot. Force the user to
        # write the prefix form instead.
        with self.assertRaises(ParserError) as cm:
            parse('char foo(char) __attribute__((reg("X")));')
        self.assertIn("must appear as a prefix", str(cm.exception))

    def test_unknown_attribute_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse('__attribute__((funky)) char f(void);')
        self.assertIn("unknown attribute name", str(cm.exception))

    def test_duplicate_spec_in_one_clause_rejected(self) -> None:
        with self.assertRaises(ParserError) as cm:
            parse('__attribute__((reg("X"), reg("Y"))) char f(char);')
        self.assertIn("duplicate", str(cm.exception))


if __name__ == "__main__":
    unittest.main()
