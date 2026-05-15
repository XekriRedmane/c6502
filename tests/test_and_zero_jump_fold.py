"""Tests for `passes.optimization.and_zero_jump_fold`.

The pass rewrites `Binary(BitwiseAnd, %ext, ConstInt(C), %res);
JumpIfTrue/False(%res, t)` — where `%ext` traces through a
`ZeroExtend` from a 1-byte unsigned var, `C` fits in 0..255, and
`%res` is single-use — as a single `JumpIfMasked` on the
narrow source. The motivating C idiom is `if (uchar & 0x80)`,
which previously emitted a 2-byte AND + zero-extend + reload
chain (~6 asm instructions) and now collapses through the
existing `and_sign_bit_branch` peephole to `LDA u; BPL t` (2
instructions).

Coverage:
  * Sign-bit folds for both sense flips.
  * Mask values other than 0x80.
  * Operand-order symmetry (constant on either side of the AND).
  * Guards: multi-use %res, non-uchar source, mask above 0xFF.
  * End-to-end sim: the optimized pipeline produces the expected
    return value (correctness anchor — opt vs. unopt agreement is
    enforced by `test_sim_differential`, but a focused sim test
    here catches regressions before the full chapter sweep runs).
"""
from __future__ import annotations

import shutil
import unittest

import tac_ast
import c99_ast
from passes.optimization.and_zero_jump_fold import fold_narrow_and_jump
from passes.type_checking import LocalAttr, Symbol, SymbolTable
from sim.harness import build_sim


def _var(name: str) -> tac_ast.Var:
    return tac_ast.Var(name=name)


def _cint(v: int) -> tac_ast.Constant:
    return tac_ast.Constant(const=tac_ast.ConstInt(value=v))


def _fn(*instrs) -> tac_ast.Function:
    return tac_ast.Function(
        name="f", is_global=True, params=[],
        instructions=list(instrs),
    )


def _symbols_with(name: str, c_type) -> SymbolTable:
    """Single-entry symbol table: `name → LocalAttr` with the given
    c99 type."""
    tbl = SymbolTable()
    tbl[name] = Symbol(type=c_type, attrs=LocalAttr())
    return tbl


class TestFold(unittest.TestCase):
    """TAC-level rewrite verification."""

    def test_sign_bit_jumpiffalse_folds_to_masked(self) -> None:
        # ZeroExtend(@u, %ext); Binary(And, %ext, 0x80, %res);
        # JumpIfFalse(%res, t)
        # ⇒ JumpIfMasked(@u, mask=0x80, jump_when_nonzero=False, t)
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@u"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_var("%ext"), src2=_cint(0x80), dst=_var("%res"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%res"), target=".t"),
        )
        syms = _symbols_with("@u", c99_ast.UChar())
        out = fold_narrow_and_jump(fn, symbols=syms)
        # Expect: ZeroExtend kept (its dst is now unused but DSE
        # runs separately), Binary dropped, JumpIfMasked replaces
        # the JumpIfFalse.
        self.assertEqual(len(out.instructions), 2)
        masked = out.instructions[1]
        self.assertIsInstance(masked, tac_ast.JumpIfMasked)
        self.assertEqual(masked.val, _var("@u"))
        self.assertEqual(masked.mask, 0x80)
        self.assertFalse(masked.jump_when_nonzero)
        self.assertEqual(masked.target, ".t")

    def test_sign_bit_jumpiftrue_folds_with_nonzero_sense(self) -> None:
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@u"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_var("%ext"), src2=_cint(0x80), dst=_var("%res"),
            ),
            tac_ast.JumpIfTrue(condition=_var("%res"), target=".t"),
        )
        syms = _symbols_with("@u", c99_ast.UChar())
        out = fold_narrow_and_jump(fn, symbols=syms)
        masked = out.instructions[1]
        self.assertIsInstance(masked, tac_ast.JumpIfMasked)
        self.assertTrue(masked.jump_when_nonzero)

    def test_arbitrary_byte_mask_folds(self) -> None:
        # Any mask in 0..255 folds; bit 7 isn't special.
        for mask in (0x01, 0x0F, 0x40, 0xFE):
            with self.subTest(mask=mask):
                fn = _fn(
                    tac_ast.ZeroExtend(src=_var("@u"), dst=_var("%ext")),
                    tac_ast.Binary(
                        op=tac_ast.BitwiseAnd(),
                        src1=_var("%ext"), src2=_cint(mask),
                        dst=_var("%res"),
                    ),
                    tac_ast.JumpIfFalse(
                        condition=_var("%res"), target=".t",
                    ),
                )
                syms = _symbols_with("@u", c99_ast.UChar())
                out = fold_narrow_and_jump(fn, symbols=syms)
                self.assertIsInstance(
                    out.instructions[-1], tac_ast.JumpIfMasked,
                )
                self.assertEqual(out.instructions[-1].mask, mask)

    def test_constant_on_left_side_folds(self) -> None:
        # AND is commutative — the pass tries either operand order.
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@u"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_cint(0x80), src2=_var("%ext"), dst=_var("%res"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%res"), target=".t"),
        )
        syms = _symbols_with("@u", c99_ast.UChar())
        out = fold_narrow_and_jump(fn, symbols=syms)
        self.assertIsInstance(out.instructions[-1], tac_ast.JumpIfMasked)

    def test_uchar_source_via_char_type(self) -> None:
        # `Char` is unsigned in c6502 — same fold applies.
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@c"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_var("%ext"), src2=_cint(0x80), dst=_var("%res"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%res"), target=".t"),
        )
        syms = _symbols_with("@c", c99_ast.Char())
        out = fold_narrow_and_jump(fn, symbols=syms)
        self.assertIsInstance(out.instructions[-1], tac_ast.JumpIfMasked)


class TestGuards(unittest.TestCase):
    """Cases that must NOT fold — preserve correctness when the
    pattern doesn't fit."""

    def test_multi_use_res_blocks_fold(self) -> None:
        # %res is used twice (once by the JumpIf, once elsewhere).
        # The AND can't be dropped — its result has a live consumer
        # after the test.
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@u"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_var("%ext"), src2=_cint(0x80), dst=_var("%res"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%res"), target=".t"),
            tac_ast.Copy(src=_var("%res"), dst=_var("%y")),
        )
        syms = _symbols_with("@u", c99_ast.UChar())
        out = fold_narrow_and_jump(fn, symbols=syms)
        # No fold — JumpIfFalse stays.
        self.assertIsInstance(out.instructions[2], tac_ast.JumpIfFalse)

    def test_non_uchar_source_blocks_fold(self) -> None:
        # Source is `Int`, not Char/UChar — narrowing isn't sound
        # (an Int's full 16 bits matter; we can't drop the high
        # byte's contribution to `& 0x80`).
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@i"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_var("%ext"), src2=_cint(0x80), dst=_var("%res"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%res"), target=".t"),
        )
        # NOTE: a real Int feeding a ZeroExtend is unusual (you
        # normally narrow, not widen, an Int) but the source-type
        # check has to be defensive.
        syms = _symbols_with("@i", c99_ast.Int())
        out = fold_narrow_and_jump(fn, symbols=syms)
        self.assertIsInstance(out.instructions[2], tac_ast.JumpIfFalse)

    def test_mask_above_byte_range_blocks_fold(self) -> None:
        # `& 0x100` on a 1-byte source is statically zero — folding
        # to a 1-byte AND would always test against zero. Refuse and
        # let constant folding handle it instead.
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@u"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_var("%ext"), src2=_cint(0x100), dst=_var("%res"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%res"), target=".t"),
        )
        syms = _symbols_with("@u", c99_ast.UChar())
        out = fold_narrow_and_jump(fn, symbols=syms)
        self.assertIsInstance(out.instructions[2], tac_ast.JumpIfFalse)

    def test_no_symbols_is_noop(self) -> None:
        # Without a symbol table the source-type check can't run, so
        # the pass returns the function unchanged.
        fn = _fn(
            tac_ast.ZeroExtend(src=_var("@u"), dst=_var("%ext")),
            tac_ast.Binary(
                op=tac_ast.BitwiseAnd(),
                src1=_var("%ext"), src2=_cint(0x80), dst=_var("%res"),
            ),
            tac_ast.JumpIfFalse(condition=_var("%res"), target=".t"),
        )
        out = fold_narrow_and_jump(fn, symbols=None)
        self.assertEqual(out.instructions, fn.instructions)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestSimIntegration(unittest.TestCase):
    """End-to-end: a small C program exercising the (uchar & 0x80)
    idiom returns the right value under --optimize."""

    def _run(self, src: str) -> int:
        sim = build_sim(src, optimize=True)
        r = sim.run(max_cycles=200_000)
        self.assertFalse(r.timed_out)
        return r.return_int()

    def test_high_bit_set(self) -> None:
        src = """
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x80) return 1;
            return 0;
        }
        int main(void) { return test(0xFF); }
        """
        self.assertEqual(self._run(src), 1)

    def test_high_bit_clear(self) -> None:
        src = """
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x80) return 1;
            return 0;
        }
        int main(void) { return test(0x7F); }
        """
        self.assertEqual(self._run(src), 0)

    def test_arbitrary_mask(self) -> None:
        # Mask 0x10 — exercises a non-sign-bit fold path. Existing
        # peepholes don't collapse this further than `AND #$10;
        # BEQ`, but the high-byte zero stage is gone.
        src = """
        #include <stdint.h>
        int test(uint8_t flags) {
            if (flags & 0x10) return 7;
            return 3;
        }
        int main(void) { return test(0x10); }
        """
        self.assertEqual(self._run(src), 7)


if __name__ == "__main__":
    unittest.main()
