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

    def test_non_zero_constant_folds_to_jumpifcmp(self):
        # `Binary(Equal, x, 5, %c); JumpIfFalse(%c, t)` doesn't have
        # the optimal `LDA x; BNE/BEQ` shape (need a CMP), but folds
        # to a single `JumpIfCmp(NotEqual, x, 5, t)` which still
        # avoids the 0/1 materialize.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_int(5), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        jic = out.instructions[0]
        self.assertIsInstance(jic, tac_ast.JumpIfCmp)
        # JumpIfFalse + Equal → invert to NotEqual.
        self.assertIsInstance(jic.op, tac_ast.NotEqual)
        self.assertEqual(jic.target, ".t")

    def test_both_var_folds_to_jumpifcmp(self):
        # `Binary(Equal, x, y, %c)` — neither side is zero, but both
        # operands flow into a JumpIfCmp.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_var("%y"), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        self.assertIsInstance(out.instructions[0], tac_ast.JumpIfCmp)

    def test_non_eq_op_folds_to_jumpifcmp(self):
        # `Binary(LessThan, x, 0, ...); JumpIfFalse(%c, t)` rewrites
        # to `JumpIfCmp(GreaterOrEqual, x, 0, t)` — the JumpIfFalse
        # sense flip inverts LessThan to GreaterOrEqual.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        jic = out.instructions[0]
        self.assertIsInstance(jic, tac_ast.JumpIfCmp)
        self.assertIsInstance(jic.op, tac_ast.GreaterOrEqual)

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


# ---------------------------------------------------------------------------
# JumpIfCmp rewrites: the generalized fold for ordering ops + non-zero
# constants. The pass produces `JumpIfCmp(op', src1, src2, target)`
# where op' is the original op (for JumpIfTrue) or the inverted op
# (for JumpIfFalse).
# ---------------------------------------------------------------------------


def _const_uchar(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstUChar(value=v))


def _symbols(*entries) -> SymbolTable:
    """Build a SymbolTable from (name, type) pairs."""
    table: SymbolTable = SymbolTable()
    for name, t in entries:
        table[name] = Symbol(type=t, attrs=LocalAttr())
    return table


class TestJumpIfCmpRewrite(unittest.TestCase):
    """Folds for ordering ops and non-zero equality constants."""

    def test_lt_jumpiftrue_keeps_op(self):
        # `Binary(LessThan, x, y, %c); JumpIfTrue(%c, t)` ⇒
        # `JumpIfCmp(LessThan, x, y, t)`.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_var("%y"), dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(len(out.instructions), 1)
        jic = out.instructions[0]
        self.assertIsInstance(jic, tac_ast.JumpIfCmp)
        self.assertIsInstance(jic.op, tac_ast.LessThan)

    def test_lt_jumpiffalse_inverts_to_ge(self):
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_var("%y"), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        jic = out.instructions[0]
        self.assertIsInstance(jic.op, tac_ast.GreaterOrEqual)

    def test_gt_jumpiffalse_inverts_to_le(self):
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.GreaterThan(),
                src1=_var("%x"), src2=_var("%y"), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertIsInstance(out.instructions[0].op, tac_ast.LessOrEqual)

    def test_eq_against_nonzero_const_inverts_to_ne(self):
        # The == 0 special case doesn't fire for == 5, so we route
        # through JumpIfCmp.
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%x"), src2=_const_int(5), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        jic = out.instructions[0]
        self.assertIsInstance(jic, tac_ast.JumpIfCmp)
        self.assertIsInstance(jic.op, tac_ast.NotEqual)

    def test_multi_use_blocks_jumpifcmp_too(self):
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_var("%y"), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
            tac_ast.Copy(src=_var("%c"), dst=_var("%retain")),
        )
        out = fold_cmp_zero_jump(fn)
        # Original Binary preserved (not single-use cond).
        self.assertEqual(len(out.instructions), 3)
        self.assertIsInstance(out.instructions[0], tac_ast.Binary)


class TestJumpIfCmpNarrowing(unittest.TestCase):
    """Narrowing both operands through ZeroExtend tracing for ordering
    folds. Pattern: `(int)(uint8) OP int_const` should narrow to a
    1-byte unsigned compare."""

    def test_narrows_uchar_lt_const(self):
        # ZeroExtend(@a /uchar/, %wide); Binary(<, %wide, 105, %c);
        # JumpIfFalse(%c, .t)  ⇒
        # ZeroExtend(...) /dead/; JumpIfCmp(>=, @a, ConstUChar(105), .t)
        symbols = _symbols(
            ("@a", c99_ast.UChar()),
        )
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@a"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%wide"), src2=_const_int(105), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        # ZeroExtend stays; Binary + JumpIfFalse collapsed.
        self.assertEqual(len(out.instructions), 2)
        jic = out.instructions[1]
        self.assertIsInstance(jic, tac_ast.JumpIfCmp)
        self.assertIsInstance(jic.op, tac_ast.GreaterOrEqual)
        self.assertEqual(jic.src1, _var("@a"))
        self.assertEqual(jic.src2, _const_uchar(105))

    def test_narrows_const_lt_uchar(self):
        # `5 < uchar_a` — constant on the left.
        symbols = _symbols(("@a", c99_ast.UChar()))
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@a"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_const_int(5), src2=_var("%wide"), dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        jic = out.instructions[1]
        self.assertIsInstance(jic, tac_ast.JumpIfCmp)
        self.assertEqual(jic.src1, _const_uchar(5))
        self.assertEqual(jic.src2, _var("@a"))

    def test_narrows_uchar_lt_uchar(self):
        # Both sides traced through ZeroExtend.
        symbols = _symbols(
            ("@a", c99_ast.UChar()),
            ("@b", c99_ast.UChar()),
        )
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@a"), dst=_var("%wa")),
            tac_ast.ZeroExtend(src=_var("@b"), dst=_var("%wb")),
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%wa"), src2=_var("%wb"), dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        jic = next(
            i for i in out.instructions if isinstance(i, tac_ast.JumpIfCmp)
        )
        self.assertEqual(jic.src1, _var("@a"))
        self.assertEqual(jic.src2, _var("@b"))

    def test_narrowing_fails_for_out_of_range_const(self):
        # 300 doesn't fit in 0..255, so don't narrow. JumpIfCmp still
        # produced, but operands stay wide.
        symbols = _symbols(("@a", c99_ast.UChar()))
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@a"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%wide"), src2=_const_int(300), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        jic = out.instructions[1]
        self.assertIsInstance(jic, tac_ast.JumpIfCmp)
        self.assertEqual(jic.src1, _var("%wide"))
        self.assertEqual(jic.src2, _const_int(300))


# ---------------------------------------------------------------------------
# Asm-level shape: narrow ordering compare-and-branch produces the
# 3-instruction LDA/CMP/BCS sequence.
# ---------------------------------------------------------------------------


class TestNarrowOrderingAsmShape(unittest.TestCase):
    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_uchar_lt_const_uses_8bit_cmp_bcs(self):
        # The loop test in `for (uint8_t i = 0; i < 105; i++)` should
        # compile to a 1-byte unsigned compare-and-branch:
        #     LDA <i>; CMP #105; BCS <break>
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    uint8_t i;\n"
            "    int sum = 0;\n"
            "    for (i = 0; i < 105; i = i + 1) sum = sum + 1;\n"
            "    return sum;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The 16-bit signed-compare V-correction sequence must be
        # absent for this loop test.
        self.assertNotIn("BVC   .jcmp_novf", asm)
        # And we should see CMP #$69 (105 in hex) followed by a BCS
        # to the loop break label.
        self.assertIn("CMP   #$69", asm)
        self.assertIn("BCS   .loop", asm)

    def test_uchar_loop_returns_correct_count(self):
        # End-to-end: the loop must terminate with sum == 105.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    uint8_t i;\n"
            "    int sum = 0;\n"
            "    for (i = 0; i < 105; i = i + 1) sum = sum + 1;\n"
            "    return sum;\n"
            "}\n"
        )
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=5_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int(), 105)


# ---------------------------------------------------------------------------
# SignExtend narrowing for `>= 0` / `< 0` against zero.
# ---------------------------------------------------------------------------


def _const_char(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstChar(value=v))


def _symbols(*pairs):
    """Build a SymbolTable from `(name, c99_type)` pairs."""
    out: SymbolTable = {}
    for name, t in pairs:
        out[name] = Symbol(type=t, attrs=LocalAttr())
    return out


class TestSignExtendNarrowingAgainstZero(unittest.TestCase):
    """Narrowing `(int)schar >= 0` / `< 0` to a 1-byte signed test
    against ConstChar(0). The rewrite is sound because SignExtend
    preserves the sign bit, and the zero-relational asm path then
    emits `LDA b; B<PL|MI> t` — the rotated countdown loop tail."""

    def test_narrows_schar_ge_zero(self):
        symbols = _symbols(("@x", c99_ast.SChar()))
        fn = _fn(
            tac_ast.SignExtend(src=_var("@x"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(),
                src1=_var("%wide"), src2=_const_int(0),
                dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".top"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        # Three instructions in: SignExtend; Binary; JumpIfTrue.
        # After fold: SignExtend (now dead, DSE will pick it up
        # later); JumpIfCmp(GE, @x, ConstChar(0), .top).
        jic = next(
            i for i in out.instructions
            if isinstance(i, tac_ast.JumpIfCmp)
        )
        self.assertIsInstance(jic.op, tac_ast.GreaterOrEqual)
        self.assertEqual(jic.src1, _var("@x"))
        self.assertEqual(jic.src2, _const_char(0))
        self.assertEqual(jic.target, ".top")

    def test_narrows_schar_lt_zero(self):
        symbols = _symbols(("@x", c99_ast.SChar()))
        fn = _fn(
            tac_ast.SignExtend(src=_var("@x"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%wide"), src2=_const_int(0),
                dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".neg"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        jic = next(
            i for i in out.instructions
            if isinstance(i, tac_ast.JumpIfCmp)
        )
        self.assertIsInstance(jic.op, tac_ast.LessThan)
        self.assertEqual(jic.src1, _var("@x"))
        self.assertEqual(jic.src2, _const_char(0))

    def test_narrows_schar_jumpiffalse_inverts_to_lt(self):
        # JumpIfFalse on `>= 0` ⇒ "jump if NOT (>= 0)" ⇒
        # JumpIfCmp(LT, ...) (op inverted because JumpIfCmp
        # always means "jump if op is true").
        symbols = _symbols(("@x", c99_ast.SChar()))
        fn = _fn(
            tac_ast.SignExtend(src=_var("@x"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(),
                src1=_var("%wide"), src2=_const_int(0),
                dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".break"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        jic = next(
            i for i in out.instructions
            if isinstance(i, tac_ast.JumpIfCmp)
        )
        self.assertIsInstance(jic.op, tac_ast.LessThan)
        self.assertEqual(jic.src1, _var("@x"))
        self.assertEqual(jic.src2, _const_char(0))

    def test_does_not_narrow_gt_against_zero(self):
        # `> 0` would need a separate non-zero check; we only
        # narrow `>= 0` / `< 0`. The fold still produces a
        # JumpIfCmp but operands stay wide.
        symbols = _symbols(("@x", c99_ast.SChar()))
        fn = _fn(
            tac_ast.SignExtend(src=_var("@x"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.GreaterThan(),
                src1=_var("%wide"), src2=_const_int(0),
                dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".pos"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        jic = next(
            i for i in out.instructions
            if isinstance(i, tac_ast.JumpIfCmp)
        )
        # Operand stays wide (not the SChar).
        self.assertEqual(jic.src1, _var("%wide"))
        self.assertEqual(jic.src2, _const_int(0))

    def test_does_not_narrow_uchar_through_signextend(self):
        # SignExtend of an UChar is unusual (c99_to_tac would emit
        # ZeroExtend); but a synthetic case shouldn't narrow because
        # UChar isn't in `_NARROW_SIGNED_TYPES`.
        symbols = _symbols(("@x", c99_ast.UChar()))
        fn = _fn(
            tac_ast.SignExtend(src=_var("@x"), dst=_var("%wide")),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(),
                src1=_var("%wide"), src2=_const_int(0),
                dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".top"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        jic = next(
            i for i in out.instructions
            if isinstance(i, tac_ast.JumpIfCmp)
        )
        self.assertEqual(jic.src1, _var("%wide"))


# ---------------------------------------------------------------------------
# Asm-level: signed `>= 0` lowers to `LDA; B<PL|MI>` (no SBC chain).
# ---------------------------------------------------------------------------


class TestSignedZeroOrderingAsmShape(unittest.TestCase):
    def _compile(self, src: str) -> str:
        from compile import _run_stage
        from preprocessor import preprocess
        return _run_stage("codegen", preprocess(src), optimize=True)

    def test_signed_byte_ge_zero_uses_bare_lda_bpl(self):
        # `int8_t x; for (...; x >= 0; x--)` — the loop tail
        # should NOT contain the V-correction (BVC + EOR #$80)
        # sequence. The lowering of `JumpIfCmp(GE, schar, 0, .top)`
        # is `LDA schar; BPL .top` — 2 instructions.
        src = (
            "#include <stdint.h>\n"
            "int sum;\n"
            "int main(void) {\n"
            "    for (int8_t x = 15; x >= 0; x--) sum += 1;\n"
            "    return sum;\n"
            "}\n"
        )
        asm = self._compile(src)
        # No V-correction in this function.
        self.assertNotIn("BVC   .jcmp_novf", asm)
        # And `BPL` to a loop-back target appears (the
        # `redundant_load_after_rmw` pass also drops the LDA, so
        # asm shows DEC + BPL directly).
        self.assertIn("BPL", asm)


if __name__ == "__main__":
    unittest.main()
