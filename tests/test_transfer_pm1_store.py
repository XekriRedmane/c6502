"""Tests for `passes.transfer_pm1_store.apply_transfer_pm1_store`."""

import unittest

import asm_ast
from passes.transfer_pm1_store import apply_transfer_pm1_store


A = asm_ast.Reg(reg=asm_ast.A())
X = asm_ast.Reg(reg=asm_ast.X())
Y = asm_ast.Reg(reg=asm_ast.Y())


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_transfer_pm1_store(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


class TestDecrement(unittest.TestCase):
    def test_txa_sbc_sta_data_folds_to_dex_stx(self):
        # `Return(save_a=False)` makes A, X, Y all dead and all
        # flags dead at the boundary in one atom (no instructions
        # between STA and Return read anything).
        m = asm_ast.Data(name="ctr", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 3)
        self.assertIsInstance(out[0], asm_ast.Dec)
        self.assertEqual(out[0].dst, X)
        self.assertIsInstance(out[1], asm_ast.Mov)
        self.assertEqual(out[1].src, X)
        self.assertEqual(out[1].dst, m)

    def test_tya_sbc_sta_zp_folds_to_dey_sty(self):
        m = asm_ast.ZP(address=0x85, offset=0)
        out = _run([
            asm_ast.Mov(src=Y, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 3)
        self.assertIsInstance(out[0], asm_ast.Dec)
        self.assertEqual(out[0].dst, Y)
        self.assertEqual(out[1].src, Y)
        self.assertEqual(out[1].dst, m)


class TestIncrement(unittest.TestCase):
    def test_txa_adc_sta_data_folds_to_inx_stx(self):
        m = asm_ast.Data(name="ctr", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 3)
        self.assertIsInstance(out[0], asm_ast.Inc)
        self.assertEqual(out[0].dst, X)
        self.assertEqual(out[1].src, X)
        self.assertEqual(out[1].dst, m)

    def test_tya_adc_sta_zp_folds_to_iny_sty(self):
        m = asm_ast.ZP(address=0x85, offset=0)
        out = _run([
            asm_ast.Mov(src=Y, dst=A, is_volatile=False),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 3)
        self.assertIsInstance(out[0], asm_ast.Inc)
        self.assertEqual(out[0].dst, Y)
        self.assertEqual(out[1].src, Y)
        self.assertEqual(out[1].dst, m)


class TestSkips(unittest.TestCase):
    def test_skips_when_a_live(self):
        # A downstream `STA n` reads A → A is live, can't fold.
        m = asm_ast.Data(name="m", offset=0)
        n = asm_ast.Data(name="n", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Mov(src=A, dst=n, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 6)
        self.assertIsInstance(out[0], asm_ast.Mov)
        self.assertEqual(out[0].src, X)
        self.assertEqual(out[0].dst, A)

    def test_skips_when_x_live(self):
        # `IndexedData(_, index=X)` reads X downstream.
        m = asm_ast.Data(name="m", offset=0)
        n = asm_ast.Data(name="n", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Mov(
                src=asm_ast.IndexedData(name="tbl", index=asm_ast.X()),
                dst=A,
                is_volatile=False,
            ),
            asm_ast.Mov(src=A, dst=n, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 7)
        self.assertIsInstance(out[0], asm_ast.Mov)
        self.assertEqual(out[0].src, X)
        self.assertEqual(out[0].dst, A)

    def test_skips_when_flags_live(self):
        # A subsequent Branch reads N/Z that the SBC set. DEX would
        # set the same N/Z, but C is also set by SBC and DEX leaves
        # C stale — `all_flags_dead_at` rejects.
        m = asm_ast.Data(name="m", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.NE(), target=".skip"),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 6)
        self.assertIsInstance(out[0], asm_ast.Mov)
        self.assertEqual(out[0].src, X)

    def test_skips_when_dst_is_indirect(self):
        # STX has no indirect-Y addressing mode.
        m = asm_ast.Indirect(offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 5)

    def test_skips_when_dst_is_frame(self):
        # STX has no Frame (indirect-FP-relative) addressing mode.
        m = asm_ast.Frame(offset=1)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 5)

    def test_skips_with_mismatched_carry(self):
        # CLC before SBC #1 doesn't decrement. Not the pattern.
        m = asm_ast.Data(name="m", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.ClearCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 5)

    def test_skips_when_constant_not_one(self):
        # SBC #2 → no INX/DEX equivalent.
        m = asm_ast.Data(name="m", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=2), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 5)

    def test_skips_volatile_store(self):
        m = asm_ast.Data(name="m", offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=True),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(len(out), 5)


class TestCascade(unittest.TestCase):
    """The cascade: after the fold, the spill pair around A that
    used to protect a value from being clobbered by the arithmetic
    becomes dead. The peephole alone doesn't remove the spill —
    downstream `dead_a_arith` / `asm_dead_store` /
    `redundant_load` do, on later fixedpoint iterations. This test
    just verifies the post-fold IR contains the two-atom DEX/STX
    pair so the cascade-eligible spill is exposed."""

    def test_dec_form_replaces_with_dex_stx(self):
        m = asm_ast.Data(name="ctr", offset=0)
        local = asm_ast.Data(name="__local_f__2", offset=0)
        out = _run([
            # Spill A around the arithmetic.
            asm_ast.Mov(src=asm_ast.Imm(value=0x10), dst=A,
                        is_volatile=False),
            asm_ast.Mov(src=A, dst=local, is_volatile=False),
            # The 4-atom pattern.
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            # Reload A from the spill, then call.
            asm_ast.Mov(src=local, dst=A, is_volatile=False),
            asm_ast.Mov(src=asm_ast.Imm(value=0x0A), dst=X,
                        is_volatile=False),
            asm_ast.Call(name="snd_delay_up", reg_args=["A", "X"]),
            asm_ast.Return(save_a=False),
        ])
        kinds = [type(i).__name__ for i in out]
        self.assertIn("Dec", kinds)
        dex_idx = kinds.index("Dec")
        self.assertEqual(out[dex_idx].dst, X)
        self.assertIsInstance(out[dex_idx + 1], asm_ast.Mov)
        self.assertEqual(out[dex_idx + 1].src, X)
        self.assertEqual(out[dex_idx + 1].dst, m)


if __name__ == "__main__":
    unittest.main()
