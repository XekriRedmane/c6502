"""Tests for `passes.round_trip_load.apply_round_trip_load_drop`."""

import unittest

import asm_ast
from passes.round_trip_load import apply_round_trip_load_drop


A = asm_ast.Reg(reg=asm_ast.A())


def _fn(instrs):
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs):
    return asm_ast.Program(top_level=[_fn(instrs)])


def _instrs(prog):
    return prog.top_level[0].instructions


class TestExplicitLdaDrop(unittest.TestCase):
    """The original pattern: drop `LDA M` after a flag-effect
    A-writer + `STA M`."""

    def test_drops_lda_after_sub_sta_pair(self):
        m = asm_ast.ZP(address=0x85, offset=0)
        before = [
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=4), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Mov(src=m, dst=A, is_volatile=False),
        ]
        after = _instrs(apply_round_trip_load_drop(_prog(before)))
        self.assertEqual(len(after), 3)
        # SetCarry / SBC / STA survive; LDA is dropped.
        self.assertIsInstance(after[0], asm_ast.SetCarry)
        self.assertIsInstance(after[1], asm_ast.Sub)
        self.assertIsInstance(after[2], asm_ast.Mov)
        self.assertEqual(after[2].dst, m)


class TestMemToMemRewrite(unittest.TestCase):
    """The mem-to-mem variant: rewrite `Mov(M, dst_mem)` after a
    flag-effect A-writer + `STA M` so the hidden `LDA M` inside the
    mem-to-mem Mov is replaced by `STA dst` driven from A directly."""

    def test_rewrites_mem_to_mem_after_sub_sta(self):
        # The exact do_ascend pattern at lines 13-15 of the IR:
        #   SBC #$04            ; A := A - 4 (flag-effect A-writer)
        #   STA  __local_col    ; col := A
        #   Mov(col, player_col); ← mem-to-mem; emits as
        #                         LDA col; STA player_col
        # After the rewrite the third atom becomes
        #   STA player_col      ; driven from A directly
        # which collapses the hidden LDA col into a no-op.
        col = asm_ast.Data(name="__local_col", offset=0)
        player_col = asm_ast.Data(name="player_col", offset=0)
        before = [
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=4), dst=A),
            asm_ast.Mov(src=A, dst=col, is_volatile=False),
            asm_ast.Mov(src=col, dst=player_col, is_volatile=False),
        ]
        after = _instrs(apply_round_trip_load_drop(_prog(before)))
        self.assertEqual(len(after), 4)
        # First three atoms unchanged.
        self.assertIsInstance(after[0], asm_ast.SetCarry)
        self.assertIsInstance(after[1], asm_ast.Sub)
        self.assertEqual(after[2].src, A)
        self.assertEqual(after[2].dst, col)
        # Fourth atom: src rewritten from `col` to `Reg(A)`; dst
        # preserved.
        self.assertIsInstance(after[3], asm_ast.Mov)
        self.assertEqual(after[3].src, A)
        self.assertEqual(after[3].dst, player_col)

    def test_rewrites_mem_to_mem_zp_source(self):
        # Same pattern but the STA target is ZP rather than Data.
        m = asm_ast.ZP(address=0x85, offset=0)
        dst = asm_ast.Data(name="player_col", offset=0)
        before = [
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=A,
                        is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Mov(src=m, dst=dst, is_volatile=False),
        ]
        after = _instrs(apply_round_trip_load_drop(_prog(before)))
        self.assertEqual(len(after), 3)
        self.assertEqual(after[2].src, A)
        self.assertEqual(after[2].dst, dst)

    def test_skips_volatile_mem_to_mem(self):
        # A volatile read MUST happen and can't be elided.
        m = asm_ast.Data(name="m", offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        before = [
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=A,
                        is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Mov(src=m, dst=dst, is_volatile=True),
        ]
        after = _instrs(apply_round_trip_load_drop(_prog(before)))
        # The mem-to-mem stays exactly as is.
        self.assertEqual(after[2].src, m)
        self.assertEqual(after[2].dst, dst)
        self.assertTrue(after[2].is_volatile)

    def test_skips_when_addresses_differ(self):
        m1 = asm_ast.Data(name="m1", offset=0)
        m2 = asm_ast.Data(name="m2", offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        before = [
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=A,
                        is_volatile=False),
            asm_ast.Mov(src=A, dst=m1, is_volatile=False),
            asm_ast.Mov(src=m2, dst=dst, is_volatile=False),
        ]
        after = _instrs(apply_round_trip_load_drop(_prog(before)))
        # The mem-to-mem reads m2, not m1 — no rewrite.
        self.assertEqual(after[2].src, m2)

    def test_skips_when_writer_lacks_flag_effect(self):
        # SetCarry doesn't set N/Z based on A — the flag soundness
        # gate must reject this case.
        m = asm_ast.Data(name="m", offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        before = [
            asm_ast.SetCarry(),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Mov(src=m, dst=dst, is_volatile=False),
        ]
        after = _instrs(apply_round_trip_load_drop(_prog(before)))
        self.assertEqual(after[2].src, m)


if __name__ == "__main__":
    unittest.main()
