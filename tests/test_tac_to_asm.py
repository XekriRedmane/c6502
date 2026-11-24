"""Tests for tac_to_asm.py's `Translator`.

The tests that called `translate_val`, `translate_instruction`,
`translate_function`, `translate_binary`, or the unary atoms on a new
`Translator` with no symbol table are doctest cases of the C++ port
now, each named for the test it came from (docs/cpp_port.md, step 8,
group G6).
"""
import unittest

import asm_ast
import tac_ast
from tac_to_asm import translate_instruction, translate_unop_atoms, translate_val


_REG_A = asm_ast.Reg(reg=asm_ast.A())


class TestTranslateUnopAtoms(unittest.TestCase):
    """`translate_unop_atoms` of LogicalNot, a form that only these
    tests reach, since a compile lowers `!` through
    `translate_instruction`. The C++ port has no counterpart of it, so
    these tests stay until the Python compiler goes. The tests of
    Complement and Negate are doctest cases of the C++ `unop_atoms`."""

    def test_logical_not_lowers_inline_with_beq_and_0_1_select(self):
        # !A := 1 if A == 0 else 0. The framing Mov(src, A) around
        # this atom sequence already sets Z, so we branch on EQ
        # directly (no extra Compare). Module-level wrapper builds a
        # fresh Translator, so labels start at _0 / _1.
        self.assertEqual(
            translate_unop_atoms(tac_ast.LogicalNot()),
            [
                asm_ast.Branch(cond=asm_ast.EQ(), target=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                asm_ast.Jump(target=".lnot_end@1"),
                asm_ast.Label(name=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Label(name=".lnot_end@1"),
            ],
        )

    def test_logical_not_labels_are_unique_across_uses(self):
        # Reusing a Translator (as happens within a program) keeps
        # the counter advancing so two ! uses don't collide.
        from tac_to_asm import Translator
        t = Translator()
        first = t.translate_unop_atoms(tac_ast.LogicalNot())
        second = t.translate_unop_atoms(tac_ast.LogicalNot())
        first_labels = {
            i.name for i in first if isinstance(i, asm_ast.Label)
        }
        second_labels = {
            i.name for i in second if isinstance(i, asm_ast.Label)
        }
        self.assertTrue(first_labels.isdisjoint(second_labels))


class TestErrors(unittest.TestCase):
    def test_unknown_val_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_val,), {})
        with self.assertRaises(TypeError):
            translate_val(stub())

    def test_unknown_instruction_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_instruction,), {})
        with self.assertRaises(TypeError):
            translate_instruction(stub())

    def test_unknown_unop_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_unary_operator,), {})
        with self.assertRaises(TypeError):
            translate_unop_atoms(stub())


if __name__ == "__main__":
    unittest.main()
