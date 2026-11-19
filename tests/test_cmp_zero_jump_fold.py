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

    def test_static_cond_not_folded(self):
        # Another function can read a static `cond`, so its one use
        # here doesn't make the store dead.
        from passes.type_checking import StaticAttr, Initial
        symbols = SymbolTable()
        symbols["g"] = Symbol(
            type=c99_ast.Int(),
            attrs=StaticAttr(initial_value=Initial(0), is_global=True),
        )
        symbols["%x"] = Symbol(type=c99_ast.Int(), attrs=LocalAttr())
        fn = _fn(
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("g"),
            ),
            tac_ast.JumpIfFalse(condition=_var("g"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        self.assertEqual(out.instructions, fn.instructions)

    def test_address_taken_cond_not_folded(self):
        # A Load through the pointer can read `%c`.
        fn = _fn(
            tac_ast.GetAddress(operand=_var("%c"), dst=_var("%p")),
            tac_ast.Binary(
                op=tac_ast.LessThan(),
                src1=_var("%x"), src2=_const_int(0), dst=_var("%c"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%c"), target=".t"),
        )
        out = fold_cmp_zero_jump(fn)
        self.assertEqual(out.instructions, fn.instructions)

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

    def test_static_cond_kept(self):
        # The compare's result goes to a static that another function
        # reads. Its one use in `check` is the jump, but the store
        # must stay. The fold had dropped it, so `apply` never
        # negated and returned -7.
        src = (
            "#include <stdint.h>\n"
            "#include <stdbool.h>\n"
            "static bool neg;\n"
            "uint16_t r;\n"
            "static uint16_t check(uint16_t a, uint16_t b) {\n"
            "    neg = (int16_t)(a ^ b) < 0;\n"
            "    if (neg) a = -a;\n"
            "    return a;\n"
            "}\n"
            "static void apply(uint16_t n) {\n"
            "    if (neg) n = -n;\n"
            "    r = n;\n"
            "}\n"
            "int main(void) { apply(check(7, 0xFFFE)); return (int)r; }\n"
        )
        a, b = self._both_paths(src)
        self.assertEqual((a, b), (7, 7))

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
        #     LDA <i>; CMP #105; BCC <body>
        # at the bottom of the loop (passes/rotate_loop_test.py). The
        # body reads `i`, or the loop would count down to 0
        # (passes/optimization/count_down.py) and have no compare.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    uint8_t i;\n"
            "    int sum = 0;\n"
            "    for (i = 0; i < 105; i = i + 1) sum = sum + i;\n"
            "    return sum;\n"
            "}\n"
        )
        asm = self._compile(src)
        # The 16-bit signed-compare V-correction sequence must be
        # absent for this loop test.
        self.assertNotIn("BVC   .jcmp_novf", asm)
        # And we should see CMP #$69 (105 in hex) followed by a BCC
        # back to the loop's body.
        self.assertIn("CMP   #$69\n   BCC   .main@rot@", asm)

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

    def _signed_compare(self, op, k):
        symbols = _symbols(("@x", c99_ast.SChar()))
        fn = _fn(
            tac_ast.SignExtend(src=_var("@x"), dst=_var("%wide")),
            tac_ast.Binary(
                op=op, src1=_var("%wide"), src2=_const_int(k),
                dst=_var("%c"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c"), target=".pos"),
        )
        out = fold_cmp_zero_jump(fn, symbols=symbols)
        return next(
            i for i in out.instructions
            if isinstance(i, tac_ast.JumpIfCmp)
        )

    def test_narrows_gt_against_any_signed_byte(self):
        # `(int8_t)d > 0` and `> -12` compare the signed byte.
        for k in (0, -12, 127, -128):
            with self.subTest(k=k):
                jic = self._signed_compare(tac_ast.GreaterThan(), k)
                self.assertEqual(jic.src1, _var("@x"))
                self.assertEqual(jic.src2, _const_char(k))

    def test_does_not_narrow_against_a_wider_constant(self):
        # 128 and -129 aren't signed bytes, so the compare stays wide.
        for k in (128, -129):
            with self.subTest(k=k):
                jic = self._signed_compare(tac_ast.LessThan(), k)
                self.assertEqual(jic.src1, _var("%wide"))
                self.assertEqual(jic.src2, _const_int(k))

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


class TestStaleExtendSource(unittest.TestCase):
    """A ZeroExtend or SignExtend of a static that's stored to before
    the compare. The narrowing read the static at the jump, after the
    store, and compiled `int t = g; g = 5; if (t < 3)` to `LDA #$05;
    STA g; CMP #$03; BCS`."""

    _SRC = (
        "#include <stdint.h>\n"
        "uint8_t g;\n"
        "int8_t sg;\n"
        "uint8_t hits;\n"
        "void lt(void) { int t = g; g = 5; if (t < 3) hits |= 1; }\n"
        "void eq0(void) { int t = g; g = 5; if (t == 0) hits |= 2; }\n"
        "void lt_c(void) { int t = g; g = 5; int c = t < 3;"
        " if (c) hits |= 4; }\n"
        "void eq0_c(void) { int t = g; g = 5; int c = t == 0;"
        " if (c) hits |= 8; }\n"
        "void ge0(void) { int t = sg; sg = -5; if (t >= 0) hits |= 16; }\n"
        "int main(void) {\n"
        "    g = 1; lt(); g = 0; eq0(); g = 1; lt_c(); g = 0; eq0_c();\n"
        "    sg = 1; ge0();\n"
        "    return hits;\n"
        "}\n"
    )

    def test_store_between_extend_and_compare(self):
        for optimize in (False, True):
            r = build_sim(self._SRC, optimize=optimize).run()
            self.assertEqual(r.return_int(), 31, f"optimize={optimize}")


class TestMultiUseSsaExtend(unittest.TestCase):
    """A switch on a byte compares the one extended value with every
    case. When that value is SSA-renamed, each compare narrows to the
    byte, and the extend stays for any other use."""

    def test_narrows_each_compare(self):
        from passes.optimization.cmp_zero_jump_fold import CMP_ZERO_RULE
        from passes.optimization.framework import PassContext, RuleSet
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("n.1"), dst=_var("%w.2")),
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%w.2"), src2=_const_int(3), dst=_var("%c.3"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c.3"), target=".a"),
            tac_ast.Binary(
                op=tac_ast.Equal(),
                src1=_var("%w.2"), src2=_const_int(7), dst=_var("%c.4"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%c.4"), target=".b"),
            tac_ast.Copy(src=_var("%w.2"), dst=_var("%keep.5")),
        )
        symbols = _symbols(
            ("n.1", c99_ast.UChar()), ("%w.2", c99_ast.Int()),
            ("%c.3", c99_ast.Int()), ("%c.4", c99_ast.Int()),
            ("%keep.5", c99_ast.Int()),
        )
        ctx = PassContext(
            symbols=symbols,
            ssa_dsts={"n.1", "%w.2", "%c.3", "%c.4", "%keep.5"},
        )
        out = RuleSet(CMP_ZERO_RULE).run(fn, ctx)
        jumps = [i for i in out.instructions
                 if isinstance(i, tac_ast.JumpIfCmp)]
        self.assertEqual(len(jumps), 2)
        for j in jumps:
            self.assertEqual(j.src1, _var("n.1"))
            self.assertIsInstance(j.src2.const, tac_ast.ConstUChar)
        self.assertIsInstance(out.instructions[0], tac_ast.ZeroExtend)

    _SWITCH = (
        "#include <stdint.h>\n"
        "uint8_t pick(uint8_t n) {\n"
        "    switch (n) {\n"
        "    case 0: return 10;\n"
        "    case 1: return 11;\n"
        "    case 2: return 12;\n"
        "    case 7: return 17;\n"
        "    case 200: return 99;\n"
        "    case 300: return 1;\n"
        "    default: return 50;\n"
        "    }\n"
        "}\n"
        "int main(void) {\n"
        "    return pick(0) + pick(1) + pick(2) + pick(7) + pick(200)\n"
        "        + pick(44);\n"
        "}\n"
    )

    def test_switch_on_byte_compares_bytes(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._SWITCH), optimize=True)
        body = asm[asm.index("pick:"):asm.index("main:")]
        # No compare of the promoted value's zero high byte.
        self.assertNotIn("CMP   #$00", body)
        self.assertIn("CMP   #$C8", body)

    def test_switch_on_byte_runs(self):
        for optimize in (False, True):
            r = build_sim(self._SWITCH, optimize=optimize).run()
            self.assertEqual(
                r.return_int(), 10 + 11 + 12 + 17 + 99 + 50,
                f"optimize={optimize}",
            )


class TestValueCompareNarrowing(unittest.TestCase):
    """A comparison kept as a value narrows its operands through their
    ZeroExtends (`NARROW_VALUE_COMPARE_RULE`), so `(int)x < 10` on a
    `uint8_t x` subtracts one byte."""

    _VALUES = (0, 9, 10, 11, 127, 128, 200, 255)

    def _src(self, expr: str) -> str:
        table = ", ".join(str(v) for v in self._VALUES)
        return (
            "#include <stdint.h>\n"
            f"uint8_t g[{len(self._VALUES)}] = {{{table}}};\n"
            "static uint8_t test(uint8_t x) { return " + expr + "; }\n"
            "int main(void) {\n"
            "    uint16_t n = 0;\n"
            f"    for (uint8_t i = 0; i < {len(self._VALUES)}; i++)\n"
            "        n = (uint16_t)(n << 1) | test(g[i]);\n"
            "    return n;\n"
            "}\n"
        )

    def _expected(self, pred) -> int:
        n = 0
        for v in self._VALUES:
            n = ((n << 1) | int(pred(v))) & 0xFFFF
        return n

    _CASES = (
        ("x < 10", lambda x: x < 10),
        ("x <= 10", lambda x: x <= 10),
        ("x > 127", lambda x: x > 127),
        ("x >= 200", lambda x: x >= 200),
        ("10 < x", lambda x: 10 < x),
        ("255 > x", lambda x: 255 > x),
        ("x < 256", lambda x: True),
        ("x > -1", lambda x: True),
    )

    def test_runs(self):
        for expr, pred in self._CASES:
            for optimize in (False, True):
                with self.subTest(expr=expr, optimize=optimize):
                    r = build_sim(self._src(expr), optimize=optimize).run()
                    self.assertEqual(r.return_int(), self._expected(pred))

    def test_one_byte_compare(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(self._src("x < 10")),
                         optimize=True)
        body = asm[asm.index("\ntest:"):asm.index("\nmain:")]
        self.assertNotIn("BVC", body)
        self.assertNotIn("SBC   #$00", body)
        self.assertIn("#$0A", body)

    def test_rule_keeps_the_dst(self):
        # The narrowed compare writes the same dst, and the extend is
        # left for DSE.
        from passes.optimization.cmp_zero_jump_fold import (
            NARROW_VALUE_COMPARE_RULE,
        )
        from passes.optimization.framework import PassContext, RuleSet
        syms = _symbols(("x", c99_ast.UChar()), ("t", c99_ast.Int()),
                        ("r", c99_ast.Int()))
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("x"), dst=_var("t")),
            tac_ast.Binary(op=tac_ast.LessThan(), src1=_var("t"),
                           src2=_const_int(10), dst=_var("r")),
            tac_ast.Ret(val=_var("r")),
        )
        out = RuleSet(NARROW_VALUE_COMPARE_RULE).run(
            fn, PassContext(symbols=syms, ssa_dsts={"t", "r"}),
        )
        self.assertEqual(out.instructions[1], tac_ast.Binary(
            op=tac_ast.LessThan(), src1=_var("x"),
            src2=tac_ast.Constant(const=tac_ast.ConstUChar(value=10)),
            dst=_var("r"),
        ))

    def test_constant_out_of_byte_range_left_alone(self):
        from passes.optimization.cmp_zero_jump_fold import (
            NARROW_VALUE_COMPARE_RULE,
        )
        from passes.optimization.framework import PassContext, RuleSet
        syms = _symbols(("x", c99_ast.UChar()), ("t", c99_ast.Int()),
                        ("r", c99_ast.Int()))
        for k in (256, -1):
            fn = _fn(
                tac_ast.ZeroExtend(src=_var("x"), dst=_var("t")),
                tac_ast.Binary(op=tac_ast.LessThan(), src1=_var("t"),
                               src2=_const_int(k), dst=_var("r")),
                tac_ast.Ret(val=_var("r")),
            )
            out = RuleSet(NARROW_VALUE_COMPARE_RULE).run(
                fn, PassContext(symbols=syms, ssa_dsts={"t", "r"}),
            )
            self.assertEqual(out.instructions, fn.instructions)



class TestConstexprBound(unittest.TestCase):
    """A global byte compared with a `constexpr uint8_t` narrows. The
    constant's read is folded only after the compare reaches its jump,
    where the global's extend is found in the same straight run."""

    def test_byte_compare(self):
        from compile import _run_stage
        from preprocessor import preprocess
        src = (
            "#include <stdint.h>\n"
            "constexpr uint8_t M = 0x63;\n"
            "extern uint8_t T [[c6502::address(0x4239)]];\n"
            "void f(void) { if (T < M) T++; }\n"
        )
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        body = asm[asm.index("\nf:"):]
        self.assertIn("CMP   #$63", body)
        self.assertNotIn("BVC", body)

if __name__ == "__main__":
    unittest.main()
