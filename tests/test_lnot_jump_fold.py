"""Tests for `passes.optimization.lnot_jump_fold`.

The pass rewrites `Unary(LogicalNot, src, %t); JumpIf{True,False}(%t, t)`
— where `%t` is single-use and the JumpIf is immediately adjacent —
as a sense-flipped direct `JumpIf{False,True}(src, t)`. The Unary
itself is left in place; the now-dead Unary is reaped by standard
DSE in the same fixed-point loop.

Coverage:
  * Both (JumpIfTrue / JumpIfFalse) sense-flip cases.
  * Multi-use `%t` blocks the rewrite (correctness).
  * Non-adjacent rejection.
  * No JumpIf follower → no fold.
  * Non-LogicalNot Unary (Negate / Complement) → no fold.
  * End-to-end sim: `if (!x)` correctness across uint8_t / uint16_t
    sources.
  * Asm-level smoke: the `JSR f; LDA/BEQ/LDA #0/JMP/LDA #1/ORA/BEQ`
    materialize-then-test sequence collapses to a single inverted
    branch.
"""
from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.lnot_jump_fold import fold_lnot_jump
from sim.harness import build_sim, run_c_program


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _fn(*instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )


class TestRewriteSenseTable(unittest.TestCase):
    """The 2 (JumpIfTrue/JumpIfFalse) cases."""

    def test_lnot_then_jumpiffalse_becomes_jumpiftrue(self):
        # `Unary(LogicalNot, x, %t); JumpIfFalse(%t, .t)` ⇒
        # `JumpIfTrue(x, .t)`. (Jump when `!x` is false, i.e. when
        # x is non-zero.)
        fn = _fn(
            tac_ast.Unary(
                op=tac_ast.LogicalNot(),
                src=_var("%x"), dst=_var("%t"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".L"),
        )
        out = fold_lnot_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfTrue)
        self.assertEqual(out.instructions[0].condition, _var("%x"))
        self.assertEqual(out.instructions[0].target, ".L")

    def test_lnot_then_jumpiftrue_becomes_jumpiffalse(self):
        fn = _fn(
            tac_ast.Unary(
                op=tac_ast.LogicalNot(),
                src=_var("%x"), dst=_var("%t"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%t"), target=".L"),
        )
        out = fold_lnot_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfFalse)
        self.assertEqual(out.instructions[0].condition, _var("%x"))
        self.assertEqual(out.instructions[0].target, ".L")


class TestRewriteGuards(unittest.TestCase):
    """Cases that should NOT be rewritten."""

    def test_multi_use_t_doesnt_fire(self):
        # `%t` is read by the JumpIf AND a later Copy — dropping
        # the Unary would lose the second reader's value.
        fn = _fn(
            tac_ast.Unary(
                op=tac_ast.LogicalNot(),
                src=_var("%x"), dst=_var("%t"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".L"),
            tac_ast.Copy(src=_var("%t"), dst=_var("%retain")),
        )
        out = fold_lnot_jump(fn)
        self.assertEqual(len(out.instructions), 3)
        self.assertIsInstance(out.instructions[0], tac_ast.Unary)
        self.assertIsInstance(out.instructions[1], tac_ast.JumpIfFalse)

    def test_non_adjacent_doesnt_fire(self):
        # An intervening instruction between the Unary and the
        # JumpIf — strict adjacency is required.
        fn = _fn(
            tac_ast.Unary(
                op=tac_ast.LogicalNot(),
                src=_var("%x"), dst=_var("%t"),
            ),
            tac_ast.Copy(src=_var("%y"), dst=_var("%z")),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".L"),
        )
        out = fold_lnot_jump(fn)
        self.assertEqual(len(out.instructions), 3)

    def test_no_jumpif_follower_doesnt_fire(self):
        # The Unary is followed by something other than a JumpIf.
        fn = _fn(
            tac_ast.Unary(
                op=tac_ast.LogicalNot(),
                src=_var("%x"), dst=_var("%t"),
            ),
            tac_ast.Copy(src=_var("%t"), dst=_var("%out")),
        )
        out = fold_lnot_jump(fn)
        self.assertEqual(len(out.instructions), 2)
        self.assertIsInstance(out.instructions[0], tac_ast.Unary)

    def test_other_unary_ops_dont_fire(self):
        # Only LogicalNot folds; Negate / Complement don't have
        # the sense-inversion semantics that make the rewrite sound.
        for op in (tac_ast.Negate(), tac_ast.Complement()):
            with self.subTest(op=type(op).__name__):
                fn = _fn(
                    tac_ast.Unary(op=op, src=_var("%x"), dst=_var("%t")),
                    tac_ast.JumpIfFalse(condition=_var("%t"), target=".L"),
                )
                out = fold_lnot_jump(fn)
                self.assertEqual(len(out.instructions), 2)
                self.assertIsInstance(out.instructions[0], tac_ast.Unary)
                self.assertIsInstance(out.instructions[1], tac_ast.JumpIfFalse)

    def test_jumpif_on_different_var_doesnt_fire(self):
        # The JumpIf's condition isn't the LogicalNot's dst.
        fn = _fn(
            tac_ast.Unary(
                op=tac_ast.LogicalNot(),
                src=_var("%x"), dst=_var("%t"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%other"), target=".L"),
        )
        out = fold_lnot_jump(fn)
        self.assertEqual(len(out.instructions), 2)


class TestEndToEnd(unittest.TestCase):
    """Differential opt-vs-unopt for `if (!x)` shapes."""

    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_uchar_negated(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (!a) return 1; return 2; }\n"
            "int main(void) { return f(0) + 10 * f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (21, 21))

    def test_uint16_negated(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint16_t a) { if (!a) return 1; return 2; }\n"
            "int main(void) { return f(0) + 10 * f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (21, 21))

    def test_uint32_negated(self):
        # Multi-byte src: the JumpIf*'s lowering walks every byte
        # (ORA chain), so the fold must still yield the right answer.
        src = (
            "#include <stdint.h>\n"
            "int f(uint32_t a) { if (!a) return 1; return 2; }\n"
            "int main(void) {\n"
            "    return f(0) + 10 * f(0x00010000UL);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (21, 21))

    def test_negated_in_ternary(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { return !a ? 10 : 20; }\n"
            "int main(void) { return f(0) + f(5); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (30, 30))


class TestAsmShape(unittest.TestCase):
    """For `if (!uchar) return;` returning a single byte from a JSR,
    the optimized body should drop the 0/1-materialize + re-test
    sequence and use a single inverted branch."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_lnot_uchar_collapses_to_single_branch(self):
        # `if (!a) return 1;` for uint8_t `a` — the body should NOT
        # contain the LogicalNot's 0/1-materialize labels
        # (.lnot_true / .lnot_end) after the fold.
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (!a) return 1; return 2; }\n"
            "int main(void) { return f(0); }\n"
        )
        asm = self._compile(src)
        body = self._extract_function(asm, "f")
        self.assertNotIn(".lnot_true", body)
        self.assertNotIn(".lnot_end", body)

    @staticmethod
    def _extract_function(asm: str, name: str) -> str:
        out: list[str] = []
        in_fn = False
        for line in asm.splitlines():
            if line.startswith(f"{name}:"):
                in_fn = True
            if in_fn:
                if (
                    line and not line.startswith((" ", "\t", "."))
                    and line.endswith(":") and not line.startswith(f"{name}:")
                ):
                    break
                out.append(line)
        return "\n".join(out)


if __name__ == "__main__":
    unittest.main()
