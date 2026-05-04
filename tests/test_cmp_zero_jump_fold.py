"""Tests for `passes.optimization.cmp_zero_jump_fold`.

The pass rewrites `Binary(==/!=, x, 0, cond); JumpIf(cond, t)` —
where `cond` is single-use — as a direct `JumpIf(x, t)` with the
appropriate sense flip. Optionally narrows by tracing through
ZeroExtend defs.

Coverage:
  * Each of the 4 (Equal/NotEqual) × (JumpIfTrue/JumpIfFalse)
    combinations rewrites correctly.
  * Zero on either side of the comparison.
  * ZeroExtend tracing produces the original narrow value.
  * Multi-use `cond` blocks the rewrite (correctness).
  * Non-zero comparison (e.g., `x == 5`) doesn't fire.
  * End-to-end sim: each pattern produces correct results.
  * Asm-level smoke check: the resulting body uses bare `LDA / BEQ`
    or `LDA / BNE` for the uint8_t case.
"""
from __future__ import annotations

import unittest

import tac_ast
import c99_ast
from passes.optimization.cmp_zero_jump_fold import fold_cmp_zero_jump
from passes.type_checking import LocalAttr, Symbol, SymbolTable
from sim.harness import build_sim, run_c_program


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _const_int(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _const_uint(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUInt(value=v))


def _fn(*instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )


# ---------------------------------------------------------------------------
# TAC-level rewrite tests.
# ---------------------------------------------------------------------------


class TestRewriteSenseTable(unittest.TestCase):
    """The 4 (Equal/NotEqual) × (JumpIfTrue/JumpIfFalse) cases."""

    def test_eq_then_jumpiffalse_becomes_jumpiftrue(self):
        # `Binary(Equal, x, 0, %cond); JumpIfFalse(%cond, t)` ⇒
        # `JumpIfTrue(x, t)`. (When `x == 0` is false, jump — i.e.,
        # when x is non-zero, jump.)
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfTrue)
        self.assertEqual(out.instructions[0].condition, _var("%x"))
        self.assertEqual(out.instructions[0].target, ".t")

    def test_eq_then_jumpiftrue_becomes_jumpiffalse(self):
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfFalse)
        self.assertEqual(out.instructions[0].condition, _var("%x"))

    def test_ne_then_jumpiffalse_becomes_jumpiffalse(self):
        # NotEqual preserves the outer JumpIf's sense.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.NotEqual(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfFalse)
        self.assertEqual(out.instructions[0].condition, _var("%x"))

    def test_ne_then_jumpiftrue_becomes_jumpiftrue(self):
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.NotEqual(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfTrue)
        self.assertEqual(out.instructions[0].condition, _var("%x"))

    def test_zero_on_left_side(self):
        # `Binary(Equal, 0, x, %cond)` should also be recognized
        # — the comparison is commutative.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_const_int(0), src2=_var("%x"), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfTrue)
        self.assertEqual(out.instructions[0].condition, _var("%x"))

    def test_unsigned_zero_constant(self):
        # The integer-constant variant doesn't matter — any
        # variant whose .value is 0 is accepted.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_uint(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfTrue)


class TestRewriteGuards(unittest.TestCase):
    """Cases that should NOT be rewritten."""

    def test_non_zero_constant_doesnt_fire(self):
        # `Binary(Equal, x, 5, %c); JumpIfFalse(%c, t)` is not a
        # zero-comparison; we don't have a single-instruction
        # rewrite for it.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_int(5), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 2)

    def test_both_var_doesnt_fire(self):
        # `Binary(Equal, x, y, %c)` — neither side is zero.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_var("%y"), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 2)

    def test_non_eq_op_doesnt_fire(self):
        # `Binary(LessThan, x, 0, ...)` — different operator, not
        # rewritable by this pass.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 2)

    def test_multi_use_cond_doesnt_fire(self):
        # `cond` is used by both the JumpIf and a later Copy — we
        # can't drop the Binary without breaking the Copy.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
            tac_ast.Copy(src=_var("%c"), dst=_var("%result")),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 3)
        self.assertIsInstance(out.instructions[0], tac_ast.Binary)

    def test_non_adjacent_doesnt_fire(self):
        # An intervening instruction between Binary and JumpIf —
        # this pass requires strict adjacency for simplicity.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.Copy(src=_var("%y"), dst=_var("%z")),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        # Original 3 instructions preserved.
        self.assertEqual(len(out.instructions), 3)


class TestZeroExtendNarrowing(unittest.TestCase):
    """When `x` is the dst of a single-use ZeroExtend, the rewrite
    substitutes the ZeroExtend's source — so the resulting JumpIf
    operates at the narrow width."""

    def test_traces_through_zero_extend(self):
        # `ZeroExtend(@a, %wide); Binary(Equal, %wide, 0, %c);
        #  JumpIfFalse(%c, .t)`
        # ⇒ `ZeroExtend(@a, %wide); JumpIfTrue(@a, .t)`
        # After this pass the ZeroExtend's dst becomes dead (its
        # only use was the Binary, which is dropped); DSE drops
        # the ZeroExtend in a later round.
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@a"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%wide"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        # ZeroExtend stays (DSE will pick it up in the optimizer
        # loop). The Binary is gone, replaced by JumpIfTrue on the
        # narrow source @a.
        self.assertEqual(len(out.instructions), 2)
        self.assertIsInstance(out.instructions[0], tac_ast.ZeroExtend)
        self.assertIsInstance(out.instructions[1], tac_ast.JumpIfTrue)
        self.assertEqual(out.instructions[1].condition, _var("@a"))

    def test_doesnt_trace_through_multi_use_widen(self):
        # If the wide value is also used elsewhere, we can't
        # narrow — substituting the source would lose a needed
        # widening at the other site.
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@a"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%wide"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
            tac_ast.Copy(src=_var("%wide"), dst=_var("%retain")),
        )
        out = fold_cmp_zero_jump(fn)
        # JumpIfTrue still produced, but condition stays at %wide.
        jumpif = next(
            i for i in out.instructions
            if isinstance(i, tac_ast.JumpIfTrue)
        )
        self.assertEqual(jumpif.condition, _var("%wide"))


# ---------------------------------------------------------------------------
# End-to-end sim: each pattern produces correct results.
# ---------------------------------------------------------------------------


class TestEndToEnd(unittest.TestCase):
    def _both_paths(self, src: str):
        no_opt = run_c_program(src).return_int_signed()
        opt = build_sim(src, optimize=True).run().return_int_signed()
        return no_opt, opt

    def test_uint8_eq_zero(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a == 0) return 1; return 2; }\n"
            "int main(void) { return f(0); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (1, 1))

    def test_uint8_eq_zero_false(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a == 0) return 1; return 2; }\n"
            "int main(void) { return f(5); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (2, 2))

    def test_uint8_ne_zero(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a != 0) return 1; return 2; }\n"
            "int main(void) { return f(5); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (1, 1))

    def test_uint16_eq_zero_ternary(self):
        # Ternary lowers to the same comparison-then-jump pattern;
        # verify the rewrite is correct under it too.
        src = (
            "#include <stdint.h>\n"
            "int f(uint16_t a) { return a == 0 ? 10 : 20; }\n"
            "int main(void) { return f(0) + f(7); }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (30, 30))


# ---------------------------------------------------------------------------
# Asm-level smoke check
# ---------------------------------------------------------------------------


class TestAsmShape(unittest.TestCase):
    """For `if (uint8_t a == 0)`, the optimized asm body should
    contain a bare `LDA / BEQ` or `LDA / BNE` for the comparison —
    not a multi-byte CMP / 0/1-select sequence."""

    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_uint8_eq_zero_uses_bne(self):
        src = (
            "#include <stdint.h>\n"
            "int f(uint8_t a) { if (a == 0) return 1; return 2; }\n"
            "int main(void) { return f(0); }\n"
        )
        asm = self._compile(src)
        body = self._function_body_without_frame(asm, "f")
        # The body (excluding prologue/epilogue) should NOT have
        # the V-corrected signed-comparison sequence.
        self.assertNotIn("BVC", body)
        # And should NOT have a `CMP` / multi-byte SBC chain for
        # the comparison — the bare zero-test only needs LDA + BEQ
        # / BNE.
        self.assertNotIn("CMP", body)
        # SHOULD have a BNE for the if-end branch.
        self.assertIn("BNE", body)

    @staticmethod
    def _function_body_without_frame(asm: str, name: str) -> str:
        """Like `_extract_function` but strips the prologue and
        epilogue regions (between the `; prologue:` / `; epilogue`
        comments and the next blank line). Leaves only the
        function's actual user-code body so signed-cmp checks
        aren't fooled by SSP arithmetic in the frame setup."""
        lines: list[str] = []
        in_fn = False
        in_frame_block = False
        for line in asm.splitlines():
            if line.startswith(f"{name}:"):
                in_fn = True
            if not in_fn:
                continue
            if (
                line and not line.startswith((" ", "\t", "."))
                and line.endswith(":") and not line.startswith(f"{name}:")
            ):
                break
            if "; prologue:" in line or "; epilogue" in line:
                in_frame_block = True
                continue
            if in_frame_block:
                if line == "":
                    in_frame_block = False
                continue
            lines.append(line)
        return "\n".join(lines)

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
