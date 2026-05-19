"""Tests for `passes.optimization.short_circuit_jump_fold`.

The pass rewrites the canonical 5-instruction short-circuit tail
(`Copy(C_ft,%t); Jump(end); Label(branch); Copy(C_sc,%t); Label(end)`)
plus an adjacent `JumpIf{True,False}(%t, T)` consumer into direct
conditional branches that retarget the chain's short-circuit jumps
to wherever the consumer would have routed `%t == C_sc`. The
tail+consumer is deleted; for the "flipped" case where the
consumer's branch direction routes `%t == C_sc` to the
fall-through, a fresh `.<funcname>@scfold@<N>` label and a
trailing `Jump(T)` materialize the fall-through path.

Coverage:
  * All four (C_ft, C_sc) × consumer-kind combinations: AND/OR
    crossed with JumpIfFalse/JumpIfTrue, covering the natural and
    flipped cases for both senses.
  * Retargeting of multiple chain jumps (3-operand `&&`).
  * Nested short-circuits — outer fold's branch_label flows into
    inner fold's retarget map (transitive closure).
  * Multi-use `%t` blocks the rewrite (correctness).
  * Multi-target `end_label` blocks the rewrite (correctness).
  * Non-{0,1} constants don't fire (Conditional shape).
  * End-to-end sim across `&&` / `||` / nested.
"""
from __future__ import annotations

import unittest

import tac_ast
from passes.optimization.short_circuit_jump_fold import (
    fold_short_circuit_jump,
)
from sim.harness import build_sim, run_c_program


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _cint(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _fn(*instrs, name: str = "f") -> tac_ast.Function:
    return tac_ast.Function(
        name=name, is_global=True, params=[], instructions=list(instrs),
    )


def _and_tail(t: str, br_lbl: str, end_lbl: str):
    """The 5-instruction tail for `&&`: Copy(1, %t); Jump(end);
    Label(branch); Copy(0, %t); Label(end)."""
    return [
        tac_ast.Copy(src=_cint(1), dst=_var(t)),
        tac_ast.Jump(target=end_lbl),
        tac_ast.Label(name=br_lbl),
        tac_ast.Copy(src=_cint(0), dst=_var(t)),
        tac_ast.Label(name=end_lbl),
    ]


def _or_tail(t: str, br_lbl: str, end_lbl: str):
    """The 5-instruction tail for `||`: Copy(0, %t); Jump(end);
    Label(branch); Copy(1, %t); Label(end)."""
    return [
        tac_ast.Copy(src=_cint(0), dst=_var(t)),
        tac_ast.Jump(target=end_lbl),
        tac_ast.Label(name=br_lbl),
        tac_ast.Copy(src=_cint(1), dst=_var(t)),
        tac_ast.Label(name=end_lbl),
    ]


class TestNaturalCase(unittest.TestCase):
    """`&& consumed by JumpIfFalse` and `|| consumed by JumpIfTrue`
    are the natural cases — D_sc == T, D_ft == next. The chain
    retargets to T and the 6-instr tail+consumer is deleted."""

    def test_and_jumpiffalse_retargets_chain_and_deletes_tail(self):
        # `&&` consumed by JumpIfFalse: short-circuit fires when
        # any operand is false → t=0 → JumpIfFalse fires → T.
        # Direct: redirect chain to T, delete tail+consumer.
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_var("%a"), target=".AFTER"),
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            tac_ast.JumpIfFalse(condition=_var("%b"), target=".BR"),
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".T"),
            tac_ast.Label(name=".AFTER"),
        )
        out = fold_short_circuit_jump(fn)
        # 1 prefix + 2 chain + 1 trailing label = 4.
        self.assertEqual(len(out.instructions), 4)
        # Chain jumps retargeted to T.
        self.assertEqual(out.instructions[1].target, ".T")
        self.assertEqual(out.instructions[2].target, ".T")

    def test_or_jumpiftrue_retargets_chain_and_deletes_tail(self):
        # `||` consumed by JumpIfTrue: short-circuit fires when
        # any operand is true → t=1 → JumpIfTrue fires → T.
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_var("%a"), target=".BR"),
            tac_ast.JumpIfTrue(condition=_var("%b"), target=".BR"),
            *_or_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfTrue(condition=_var("%t"), target=".T"),
            tac_ast.Label(name=".AFTER"),
        )
        out = fold_short_circuit_jump(fn)
        self.assertEqual(len(out.instructions), 3)
        self.assertEqual(out.instructions[0].target, ".T")
        self.assertEqual(out.instructions[1].target, ".T")


class TestFlippedCase(unittest.TestCase):
    """`&& consumed by JumpIfTrue` and `|| consumed by JumpIfFalse`
    are the flipped cases — D_sc == next, D_ft == T. The chain
    retargets to a fresh `.<funcname>@scfold@<N>` label and
    `Jump(T); Label(.scfold@N)` replaces the tail+consumer."""

    def test_and_jumpiftrue_mints_scfold_and_jumps_to_t(self):
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            tac_ast.JumpIfFalse(condition=_var("%b"), target=".BR"),
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfTrue(condition=_var("%t"), target=".T"),
            tac_ast.Label(name=".AFTER"),
        )
        out = fold_short_circuit_jump(fn)
        # 2 chain + 1 Jump + 1 scfold label + 1 .AFTER label = 5.
        self.assertEqual(len(out.instructions), 5)
        scfold = ".f@scfold@0"
        self.assertEqual(out.instructions[0].target, scfold)
        self.assertEqual(out.instructions[1].target, scfold)
        self.assertIsInstance(out.instructions[2], tac_ast.Jump)
        self.assertEqual(out.instructions[2].target, ".T")
        self.assertIsInstance(out.instructions[3], tac_ast.Label)
        self.assertEqual(out.instructions[3].name, scfold)

    def test_or_jumpiffalse_mints_scfold_and_jumps_to_t(self):
        fn = _fn(
            tac_ast.JumpIfTrue(condition=_var("%a"), target=".BR"),
            tac_ast.JumpIfTrue(condition=_var("%b"), target=".BR"),
            *_or_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".T"),
        )
        out = fold_short_circuit_jump(fn)
        # 2 chain + 1 Jump + 1 scfold label = 4.
        self.assertEqual(len(out.instructions), 4)
        scfold = ".f@scfold@0"
        self.assertEqual(out.instructions[0].target, scfold)
        self.assertEqual(out.instructions[1].target, scfold)
        self.assertIsInstance(out.instructions[2], tac_ast.Jump)
        self.assertEqual(out.instructions[2].target, ".T")
        self.assertIsInstance(out.instructions[3], tac_ast.Label)
        self.assertEqual(out.instructions[3].name, scfold)


class TestRetargetingShapes(unittest.TestCase):
    """The chain may include several variants of jump-target
    instructions — Jump, JumpIfTrue, JumpIfFalse, JumpIfCmp,
    JumpIfMasked — all of which should be retargeted."""

    def test_jumpifcmp_chain_retargets(self):
        # Three-operand `&&` lowered with JumpIfCmp for each
        # comparison. All three should retarget to T.
        fn = _fn(
            tac_ast.JumpIfCmp(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_cint(64),
                target=".BR",
            ),
            tac_ast.JumpIfCmp(
                op=tac_ast.GreaterOrEqual(),
                src1=_var("%x"), src2=_cint(80),
                target=".BR",
            ),
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".T"),
        )
        out = fold_short_circuit_jump(fn)
        self.assertEqual(len(out.instructions), 2)
        self.assertEqual(out.instructions[0].target, ".T")
        self.assertEqual(out.instructions[1].target, ".T")

    def test_jumpifmasked_chain_retargets(self):
        fn = _fn(
            tac_ast.JumpIfMasked(
                val=_var("%x"), mask=0x80,
                jump_when_nonzero=False, target=".BR",
            ),
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".T"),
        )
        out = fold_short_circuit_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertEqual(out.instructions[0].target, ".T")


class TestNestedShortCircuit(unittest.TestCase):
    """`(a && b) || c` produces two foldable patterns. The inner
    fold's branch_label is what the outer's chain points at, so
    the retarget map needs transitive closure: inner's branch_label
    → outer's branch_label → outer's resolved target."""

    def test_nested_or_of_and(self):
        # `(a && b) || c` consumed by JumpIfFalse → both folds.
        # Inner: AND consumed by JumpIfTrue (the outer's first
        # chain jump). Outer: OR consumed by JumpIfFalse. Both
        # are flipped cases — inner mints scfold@0, outer mints
        # scfold@1.
        fn = _fn(
            # Inner `a && b`
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR1"),
            tac_ast.JumpIfFalse(condition=_var("%b"), target=".BR1"),
            *_and_tail("%t1", ".BR1", ".END1"),
            # Outer `%t1 || c`
            tac_ast.JumpIfTrue(condition=_var("%t1"), target=".BR2"),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".BR2"),
            *_or_tail("%t2", ".BR2", ".END2"),
            # Consumer
            tac_ast.JumpIfFalse(condition=_var("%t2"), target=".T"),
        )
        out = fold_short_circuit_jump(fn)
        scfold0 = ".f@scfold@0"
        scfold1 = ".f@scfold@1"
        # Inner's chain (first 2 JumpIfFalse) lands at the
        # inner-minted scfold@0 — that's where control flows when
        # the AND short-circuited, and from scfold@0 the original
        # outer chain (now JumpIfTrue(c, scfold1)) tests c.
        self.assertEqual(out.instructions[0].target, scfold0)
        self.assertEqual(out.instructions[1].target, scfold0)
        # Inner emitted [Jump(.BR2 → scfold1 via transitive
        # closure), Label(scfold0)].
        self.assertIsInstance(out.instructions[2], tac_ast.Jump)
        self.assertEqual(out.instructions[2].target, scfold1)
        self.assertIsInstance(out.instructions[3], tac_ast.Label)
        self.assertEqual(out.instructions[3].name, scfold0)
        # Remaining outer chain jump (JumpIfTrue(c, .BR2)) →
        # scfold1.
        self.assertEqual(out.instructions[4].target, scfold1)
        # Outer emitted [Jump(.T), Label(scfold1)].
        self.assertIsInstance(out.instructions[5], tac_ast.Jump)
        self.assertEqual(out.instructions[5].target, ".T")
        self.assertIsInstance(out.instructions[6], tac_ast.Label)
        self.assertEqual(out.instructions[6].name, scfold1)


class TestRewriteGuards(unittest.TestCase):
    """Cases that should NOT be rewritten."""

    def test_multi_use_t_doesnt_fire(self):
        # `%t` is read by the consumer AND a later instruction —
        # dropping the tail would lose that reader's value.
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".T"),
            tac_ast.Copy(src=_var("%t"), dst=_var("%retain")),
        )
        before = list(fn.instructions)
        out = fold_short_circuit_jump(fn)
        self.assertEqual(out.instructions, before)

    def test_multi_target_end_label_doesnt_fire(self):
        # Something outside the tail jumps to end_label — deleting
        # the Label would dangle that jump.
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            tac_ast.Jump(target=".END"),  # extra ref to end_label
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".T"),
        )
        before = list(fn.instructions)
        out = fold_short_circuit_jump(fn)
        self.assertEqual(out.instructions, before)

    def test_non_zero_one_constants_dont_fire(self):
        # Conditional-style `cond ? 5 : 0` is structurally similar
        # but doesn't match the short-circuit shape — restrict to
        # (0, 1).
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            tac_ast.Copy(src=_cint(5), dst=_var("%t")),
            tac_ast.Jump(target=".END"),
            tac_ast.Label(name=".BR"),
            tac_ast.Copy(src=_cint(0), dst=_var("%t")),
            tac_ast.Label(name=".END"),
            tac_ast.JumpIfFalse(condition=_var("%t"), target=".T"),
        )
        before = list(fn.instructions)
        out = fold_short_circuit_jump(fn)
        self.assertEqual(out.instructions, before)

    def test_different_t_in_two_copies_doesnt_fire(self):
        # The two Copies write to different temps — not the
        # short-circuit shape.
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            tac_ast.Copy(src=_cint(1), dst=_var("%t1")),
            tac_ast.Jump(target=".END"),
            tac_ast.Label(name=".BR"),
            tac_ast.Copy(src=_cint(0), dst=_var("%t2")),
            tac_ast.Label(name=".END"),
            tac_ast.JumpIfFalse(condition=_var("%t1"), target=".T"),
        )
        before = list(fn.instructions)
        out = fold_short_circuit_jump(fn)
        self.assertEqual(out.instructions, before)

    def test_jumpif_on_different_var_doesnt_fire(self):
        # The consumer reads a different Var than the tail wrote.
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.JumpIfFalse(condition=_var("%other"), target=".T"),
        )
        before = list(fn.instructions)
        out = fold_short_circuit_jump(fn)
        self.assertEqual(out.instructions, before)

    def test_no_consumer_doesnt_fire(self):
        # Tail not followed by a JumpIf at all.
        fn = _fn(
            tac_ast.JumpIfFalse(condition=_var("%a"), target=".BR"),
            *_and_tail("%t", ".BR", ".END"),
            tac_ast.Copy(src=_var("%t"), dst=_var("%out")),
        )
        before = list(fn.instructions)
        out = fold_short_circuit_jump(fn)
        self.assertEqual(out.instructions, before)


class TestEndToEnd(unittest.TestCase):
    """Differential opt-vs-unopt for short-circuit shapes."""

    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_and_uchar_natural(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a, uint8_t b) {\n"
            "    if (a >= 0x40 && a < 0x47) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x44, 0) + 10 * f(0x10, 0) + 100 * f(0x50, 0);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 1 + 10 * 2 + 100 * 2)

    def test_or_uchar_flipped(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a < 0x40 || a >= 0x50) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x44) + 10 * f(0x10) + 100 * f(0x60);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 2 + 10 * 1 + 100 * 1)

    def test_nested_or_of_or(self):
        # Three-operand `||` — c99_to_tac nests as `(a||b)||c`,
        # so two foldable short-circuits stack.
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a == 0x63 || a == 0x8B || a == 0xB3) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x63) + 10 * f(0x8B) + 100 * f(0xB3)\n"
            "        + 1000 * f(0x00);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 1 + 10 * 1 + 100 * 1 + 1000 * 2)

    def test_uint16_operands(self):
        # The chain's JumpIf*Cmp lowering ORs each byte; the fold
        # is width-agnostic, so a uint16_t source should also work.
        src = (
            "#include <stdint.h>\n"
            "int f(uint16_t a) {\n"
            "    if (a >= 0x0100 && a < 0x0200) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) {\n"
            "    return f(0x0150) + 10 * f(0x0050) + 100 * f(0x0250);\n"
            "}\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual(a, b)
        self.assertEqual(a, 1 + 10 * 2 + 100 * 2)


class TestAsmShape(unittest.TestCase):
    """After the fold, the `&&` / `||` 0-or-1 materialize labels
    (.and_false / .and_end / .or_true / .or_end) should be gone
    from the asm body of a function whose only short-circuit is
    consumed by an `if`."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_and_collapses_to_chain(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a >= 0x40 && a < 0x47) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) { return f(0x44); }\n"
        )
        asm = self._compile(src)
        body = self._extract_function(asm, "f")
        self.assertNotIn(".and_false", body)
        self.assertNotIn(".and_end", body)

    def test_or_collapses_to_chain(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) {\n"
            "    if (a < 0x40 || a >= 0x50) return 1;\n"
            "    return 2;\n"
            "}\n"
            "int main(void) { return f(0x44); }\n"
        )
        asm = self._compile(src)
        body = self._extract_function(asm, "f")
        self.assertNotIn(".or_true", body)
        self.assertNotIn(".or_end", body)

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
