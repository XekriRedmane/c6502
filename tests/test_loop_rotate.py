"""Tests for the TAC pre-SSA signed-countdown loop-rotation pass.

Coverage:
  * Unit tests on synthetic pre-SSA TAC: matches the canonical
    for-loop shape, rotates correctly for `int` and `int8_t`
    counters, refuses unsigned counters, refuses non-`>= 0`
    conditions, refuses non-decrement-by-1 post-steps, refuses
    non-constant or negative inits.
  * End-to-end through the optimizer + simulator: programs
    compute the same result with rotation as without.
"""
from __future__ import annotations

import shutil
import subprocess
import unittest
from pathlib import Path

import c99_ast
import tac_ast
from passes.optimization.loop_rotate import (
    rotate_signed_countdown_loops,
)
from passes.type_checking import IdAttr, Symbol


def _var(n: str) -> tac_ast.Var:
    return tac_ast.Var(name=n)


def _constI(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _constC(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstChar(value=v))


class _LocalAttr(IdAttr):
    """Stand-in for the real `LocalAttr`. The pass only reads
    `.type`, so any IdAttr subclass works in unit tests."""


def _fn(instrs: list[tac_ast.Type_instruction]) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[], instructions=list(instrs),
    )


def _symbols_with(*pairs: tuple[str, type]) -> dict[str, Symbol]:
    """Build a minimal symbol table mapping `name -> Symbol(type=T())`
    for each `(name, T)` pair. The pass only reads `.type` to gate
    on signed-integer-ness."""
    out: dict[str, Symbol] = {}
    for name, t_cls in pairs:
        out[name] = Symbol(type=t_cls(), attrs=_LocalAttr())
    return out


class TestLoopRotateMatch(unittest.TestCase):
    """Direct calls to the pass on synthetic pre-SSA TAC."""

    def _make_int_loop(
        self, *, init_value: int, op: type = tac_ast.Subtract,
        cond_op: type = tac_ast.GreaterOrEqual, cond_const: int = 0,
        post_dec_const: int = 1,
    ) -> tuple[
        list[tac_ast.Type_instruction], dict[str, Symbol],
    ]:
        """Build the canonical `int` for-loop TAC for testing.
        Init: `Copy(Constant(init_value), x)`.
        Cond: `Binary(cond_op, x, Constant(cond_const), %0); JumpIfFalse(%0, .break)`.
        Post: `Binary(op, x, Constant(post_dec_const), %1); Copy(%1, x)`.
        Body: a single `Binary(Add, sum, x, sum)` for shape."""
        instrs = [
            tac_ast.Copy(src=_constI(init_value), dst=_var("x")),
            tac_ast.Label(name="L_start"),
            tac_ast.Binary(
                op=cond_op(), src1=_var("x"),
                src2=_constI(cond_const), dst=_var("%0"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%0"), target="L_break"),
            tac_ast.Binary(
                op=tac_ast.Add(), src1=_var("sum"), src2=_var("x"),
                dst=_var("sum"),
            ),
            tac_ast.Label(name="L_continue"),
            tac_ast.Binary(
                op=op(), src1=_var("x"),
                src2=_constI(post_dec_const), dst=_var("%1"),
            ),
            tac_ast.Copy(src=_var("%1"), dst=_var("x")),
            tac_ast.Jump(target="L_start"),
            tac_ast.Label(name="L_break"),
        ]
        symbols = _symbols_with(
            ("x", c99_ast.Int), ("sum", c99_ast.Int),
        )
        return instrs, symbols

    def test_int_countdown_rotates(self) -> None:
        instrs, symbols = self._make_int_loop(init_value=15)
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        # Expected rotated shape:
        #   Copy(15, x)
        #   Label(L_start)
        #   Binary(Add, sum, x, sum)        ; body
        #   Label(L_continue)
        #   Binary(Subtract, x, 1, %1)      ; post
        #   Copy(%1, x)                     ; post
        #   Binary(GE, x, 0, %0)            ; moved cond
        #   JumpIfTrue(%0, L_start)         ; replaces Jump+JIF pair
        #   Label(L_break)
        self.assertEqual(len(out.instructions), 9)
        self.assertEqual(out.instructions[0], instrs[0])     # init
        self.assertEqual(out.instructions[1], instrs[1])     # L_start
        self.assertEqual(out.instructions[2], instrs[4])     # body
        self.assertEqual(out.instructions[3], instrs[5])     # L_continue
        self.assertEqual(out.instructions[4], instrs[6])     # post Sub
        self.assertEqual(out.instructions[5], instrs[7])     # post Copy
        self.assertEqual(out.instructions[6], instrs[2])     # moved cond
        self.assertEqual(
            out.instructions[7],
            tac_ast.JumpIfTrue(condition=_var("%0"), target="L_start"),
        )
        self.assertEqual(out.instructions[8], instrs[9])     # L_break

    def test_init_zero_is_eligible(self) -> None:
        # `for (int x = 0; x >= 0; x--)` rotates: 0 >= 0 is true, so
        # the loop body runs at least once.
        instrs, symbols = self._make_int_loop(init_value=0)
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertIsInstance(out.instructions[-2], tac_ast.JumpIfTrue)

    def test_negative_init_skips(self) -> None:
        # `for (int x = -1; x >= 0; x--)` — first iter test fails;
        # rotating would incorrectly run the body once.
        instrs, symbols = self._make_int_loop(init_value=-1)
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)

    def test_unsigned_counter_skips(self) -> None:
        # `unsigned int x` — `x >= 0` is trivially true; we don't
        # touch.
        instrs, symbols = self._make_int_loop(init_value=15)
        symbols["x"] = Symbol(type=c99_ast.UInt(), attrs=_LocalAttr())
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)

    def test_char_counter_skips(self) -> None:
        # `char` is unsigned in c6502; same as UInt — skip.
        instrs, symbols = self._make_int_loop(init_value=15)
        symbols["x"] = Symbol(type=c99_ast.Char(), attrs=_LocalAttr())
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)

    def test_non_ge_condition_skips(self) -> None:
        # `for (int x = 15; x > 0; x--)` — strict >. Loop body runs
        # one fewer time AND the post-decrement-test semantics
        # differ. Out of scope for v1.
        instrs, symbols = self._make_int_loop(
            init_value=15, cond_op=tac_ast.GreaterThan,
        )
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)

    def test_compare_against_nonzero_skips(self) -> None:
        # `for (int x = 15; x >= 1; x--)` — could rotate with a
        # bias adjustment, but out of scope.
        instrs, symbols = self._make_int_loop(
            init_value=15, cond_const=1,
        )
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)

    def test_post_increment_skips(self) -> None:
        # `for (int x = 15; x >= 0; x++)` — post is Add not
        # Subtract; loop is infinite anyway.
        instrs, symbols = self._make_int_loop(
            init_value=15, op=tac_ast.Add,
        )
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)

    def test_post_dec_by_two_skips(self) -> None:
        # `for (int x = 15; x >= 0; x -= 2)` — same `>= 0` test
        # would still work, but only when starting on an even
        # value would the rotation be "safe" via underflow alone;
        # the matcher conservatively requires step == 1.
        instrs, symbols = self._make_int_loop(
            init_value=15, post_dec_const=2,
        )
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)


class TestLoopRotateSChar(unittest.TestCase):
    """The int8_t / SChar case: cond block has a SignExtend in
    front of the GE because the comparison happens at Int width
    after C99 integer promotion."""

    def test_schar_with_signextend_in_cond_rotates(self) -> None:
        # Init: `Truncate(ConstInt(15), %1); Copy(%1, x)` — what
        # c99_to_tac emits for `int8_t x = 15;`. Cond:
        # `SignExtend(x, %2); Binary(GE, %2, ConstInt(0), %3);
        # JumpIfFalse(%3, L_break)`. Post: `Binary(Subtract, x,
        # ConstChar(1), %4); Copy(%4, x)`.
        instrs = [
            tac_ast.Truncate(src=_constI(15), dst=_var("%1")),
            tac_ast.Copy(src=_var("%1"), dst=_var("x")),
            tac_ast.Label(name="L_start"),
            tac_ast.SignExtend(src=_var("x"), dst=_var("%2")),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(),
                src1=_var("%2"), src2=_constI(0), dst=_var("%3"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%3"), target="L_break"),
            # Body: a use of x.
            tac_ast.Copy(src=_var("x"), dst=_var("scratch")),
            tac_ast.Label(name="L_continue"),
            tac_ast.Binary(
                op=tac_ast.Subtract(), src1=_var("x"),
                src2=_constC(1), dst=_var("%4"),
            ),
            tac_ast.Copy(src=_var("%4"), dst=_var("x")),
            tac_ast.Jump(target="L_start"),
            tac_ast.Label(name="L_break"),
        ]
        symbols = _symbols_with(
            ("x", c99_ast.SChar), ("scratch", c99_ast.SChar),
        )
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        # Expected rotated instructions:
        # 0: init Truncate
        # 1: init Copy
        # 2: Label(L_start)
        # 3: body Copy(x, scratch)
        # 4: Label(L_continue)
        # 5: post Subtract
        # 6: post Copy
        # 7: moved SignExtend(x, %2)
        # 8: moved Binary(GE, %2, 0, %3)
        # 9: JumpIfTrue(%3, L_start)
        # 10: Label(L_break)
        self.assertEqual(len(out.instructions), 11)
        self.assertEqual(out.instructions[0], instrs[0])
        self.assertEqual(out.instructions[1], instrs[1])
        self.assertEqual(out.instructions[2], instrs[2])
        self.assertEqual(out.instructions[3], instrs[6])
        self.assertEqual(out.instructions[4], instrs[7])
        self.assertEqual(out.instructions[5], instrs[8])
        self.assertEqual(out.instructions[6], instrs[9])
        self.assertEqual(out.instructions[7], instrs[3])  # moved SignExtend
        self.assertEqual(out.instructions[8], instrs[4])  # moved Binary GE
        self.assertEqual(
            out.instructions[9],
            tac_ast.JumpIfTrue(condition=_var("%3"), target="L_start"),
        )
        self.assertEqual(out.instructions[10], instrs[11])

    def test_schar_init_overflow_skips(self) -> None:
        # `int8_t x = 200` — Truncate(200, %1) → %1 holds 0xC8 =
        # -56 as signed. Not >= 0; reject.
        instrs = [
            tac_ast.Truncate(src=_constI(200), dst=_var("%1")),
            tac_ast.Copy(src=_var("%1"), dst=_var("x")),
            tac_ast.Label(name="L_start"),
            tac_ast.SignExtend(src=_var("x"), dst=_var("%2")),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(),
                src1=_var("%2"), src2=_constI(0), dst=_var("%3"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%3"), target="L_break"),
            tac_ast.Label(name="L_continue"),
            tac_ast.Binary(
                op=tac_ast.Subtract(), src1=_var("x"),
                src2=_constC(1), dst=_var("%4"),
            ),
            tac_ast.Copy(src=_var("%4"), dst=_var("x")),
            tac_ast.Jump(target="L_start"),
            tac_ast.Label(name="L_break"),
        ]
        symbols = _symbols_with(("x", c99_ast.SChar))
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)


class TestLoopRotateMisc(unittest.TestCase):
    def test_no_loop_passthrough(self) -> None:
        # No loop labels at all — pass-through.
        instrs = [
            tac_ast.Copy(src=_constI(5), dst=_var("x")),
            tac_ast.Ret(val=_var("x")),
        ]
        symbols = _symbols_with(("x", c99_ast.Int))
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        self.assertEqual(out.instructions, instrs)

    def test_signextend_of_iv_in_body_rewrites_to_zeroextend(self) -> None:
        # Body has `SignExtend(x, %t)` where x is the iv. Since
        # the rotation's preconditions guarantee x is in [0, init]
        # throughout the body, the SignExtend is equivalent to
        # ZeroExtend at the bit-pattern level. Rewrite it.
        instrs = [
            tac_ast.Copy(src=_constI(15), dst=_var("x")),
            tac_ast.Label(name="L_start"),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(), src1=_var("x"),
                src2=_constI(0), dst=_var("%cond"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%cond"), target="L_break"),
            # Body uses SignExtend(x).
            tac_ast.SignExtend(src=_var("x"), dst=_var("%ext")),
            tac_ast.Copy(src=_var("%ext"), dst=_var("scratch")),
            tac_ast.Label(name="L_continue"),
            tac_ast.Binary(
                op=tac_ast.Subtract(), src1=_var("x"),
                src2=_constI(1), dst=_var("%new"),
            ),
            tac_ast.Copy(src=_var("%new"), dst=_var("x")),
            tac_ast.Jump(target="L_start"),
            tac_ast.Label(name="L_break"),
        ]
        symbols = _symbols_with(
            ("x", c99_ast.Int), ("scratch", c99_ast.Int),
        )
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        # The SignExtend in the body should be a ZeroExtend now.
        body_instrs = [
            i for i in out.instructions
            if isinstance(i, (tac_ast.SignExtend, tac_ast.ZeroExtend))
        ]
        self.assertEqual(len(body_instrs), 1)
        self.assertIsInstance(body_instrs[0], tac_ast.ZeroExtend)
        self.assertEqual(body_instrs[0].src, _var("x"))
        self.assertEqual(body_instrs[0].dst, _var("%ext"))

    def test_signextend_of_other_var_in_body_unchanged(self) -> None:
        # Body has `SignExtend(other_var, %t)` — not the iv. Leave
        # it alone (we don't know other_var is non-negative).
        instrs = [
            tac_ast.Copy(src=_constI(15), dst=_var("x")),
            tac_ast.Label(name="L_start"),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(), src1=_var("x"),
                src2=_constI(0), dst=_var("%cond"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%cond"), target="L_break"),
            tac_ast.SignExtend(src=_var("y"), dst=_var("%ext")),
            tac_ast.Label(name="L_continue"),
            tac_ast.Binary(
                op=tac_ast.Subtract(), src1=_var("x"),
                src2=_constI(1), dst=_var("%new"),
            ),
            tac_ast.Copy(src=_var("%new"), dst=_var("x")),
            tac_ast.Jump(target="L_start"),
            tac_ast.Label(name="L_break"),
        ]
        symbols = _symbols_with(
            ("x", c99_ast.Int), ("y", c99_ast.SChar),
        )
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        ses = [
            i for i in out.instructions
            if isinstance(i, tac_ast.SignExtend)
        ]
        self.assertEqual(len(ses), 1)
        self.assertEqual(ses[0].src, _var("y"))

    def test_skips_se_to_ze_when_body_modifies_iv(self) -> None:
        # Body re-assigns x. Without invariance we can't claim
        # x stays non-negative — keep the SignExtend untouched.
        instrs = [
            tac_ast.Copy(src=_constI(15), dst=_var("x")),
            tac_ast.Label(name="L_start"),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(), src1=_var("x"),
                src2=_constI(0), dst=_var("%cond"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%cond"), target="L_break"),
            # Body modifies x.
            tac_ast.Copy(src=_constI(-5), dst=_var("x")),
            tac_ast.SignExtend(src=_var("x"), dst=_var("%ext")),
            tac_ast.Label(name="L_continue"),
            tac_ast.Binary(
                op=tac_ast.Subtract(), src1=_var("x"),
                src2=_constI(1), dst=_var("%new"),
            ),
            tac_ast.Copy(src=_var("%new"), dst=_var("x")),
            tac_ast.Jump(target="L_start"),
            tac_ast.Label(name="L_break"),
        ]
        symbols = _symbols_with(("x", c99_ast.Int))
        out = rotate_signed_countdown_loops(_fn(instrs), symbols)
        # Rotation should still apply, but SignExtend is untouched.
        ses = [
            i for i in out.instructions
            if isinstance(i, tac_ast.SignExtend)
        ]
        self.assertEqual(len(ses), 1, msg=str(out.instructions))

    def test_idempotent(self) -> None:
        # Run twice; second call sees a rotated loop and should
        # be a no-op (the matcher refuses to re-fire).
        instrs = [
            tac_ast.Copy(src=_constI(5), dst=_var("x")),
            tac_ast.Label(name="L_start"),
            tac_ast.Binary(
                op=tac_ast.GreaterOrEqual(), src1=_var("x"),
                src2=_constI(0), dst=_var("%0"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%0"), target="L_break"),
            tac_ast.Label(name="L_continue"),
            tac_ast.Binary(
                op=tac_ast.Subtract(), src1=_var("x"),
                src2=_constI(1), dst=_var("%1"),
            ),
            tac_ast.Copy(src=_var("%1"), dst=_var("x")),
            tac_ast.Jump(target="L_start"),
            tac_ast.Label(name="L_break"),
        ]
        symbols = _symbols_with(("x", c99_ast.Int))
        once = rotate_signed_countdown_loops(_fn(instrs), symbols)
        twice = rotate_signed_countdown_loops(once, symbols)
        self.assertEqual(once.instructions, twice.instructions)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestLoopRotateE2E(unittest.TestCase):
    """End-to-end through the simulator: rotated programs return
    the same result as unrotated ones."""

    def _sim(self, src: str, expected: int) -> None:
        """Compile via `--codegen --optimize`, assemble, simulate
        until the program halts, assert the return value matches."""
        from sim.harness import build_sim
        sim = build_sim(src, optimize=True)
        result = sim.run(max_cycles=200_000)
        self.assertFalse(
            result.timed_out, f"sim timed out at {result.cycles}",
        )
        self.assertEqual(result.return_int_signed(), expected)

    def test_simple_int_countdown(self) -> None:
        # sum 15..0 = 15*16/2 = 120
        src = (
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        self._sim(src, 120)

    def test_simple_int_countdown_init_zero(self) -> None:
        # Single iteration: body runs once, x == 0.
        src = (
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int x = 0; x >= 0; x--) sum += 1;\n"
            "    return sum;\n"
            "}\n"
        )
        self._sim(src, 1)

    def test_int8_countdown(self) -> None:
        # sum 15..0 from int8_t counter. Verifies the SignExtend-
        # in-cond shape rotates and computes correctly.
        src = (
            "#include <stdint.h>\n"
            "int main(void) {\n"
            "    int sum = 0;\n"
            "    for (int8_t x = 15; x >= 0; x--) sum += x;\n"
            "    return sum;\n"
            "}\n"
        )
        self._sim(src, 120)


if __name__ == "__main__":
    unittest.main()
