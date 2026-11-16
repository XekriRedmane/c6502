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
        # The STA m stays, and the step moves to X before the TXA, so A
        # holds X - 1 for both stores.
        self.assertEqual(out[:3], [
            asm_ast.Dec(dst=X),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
        ])
        self.assertEqual(len(out), 5)

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
        # The step moves to X before the TXA, and the STA stays.
        m = asm_ast.Indirect(offset=0)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out[:3], [
            asm_ast.Dec(dst=X),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
        ])

    def test_skips_when_dst_is_frame(self):
        # STX has no Frame (indirect-FP-relative) addressing mode.
        # The step moves to X before the TXA, and the STA stays.
        m = asm_ast.Frame(offset=1)
        out = _run([
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out[:3], [
            asm_ast.Dec(dst=X),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
        ])

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
        # The store stays a volatile STA from A. The step moves to X
        # before the TXA, which doesn't touch the store.
        self.assertEqual(out, [
            asm_ast.Dec(dst=X),
            asm_ast.Mov(src=X, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=True),
            asm_ast.Return(save_a=False),
        ])


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


class TestBranchOnResult(unittest.TestCase):
    """N/Z may be read after the store: INY sets them as the ADC did.
    `get_next_code_byte2`'s `if (++Z_PC2_L == 0)`, from when Zork's
    print_zstring example kept Z_PC2 in three bytes."""

    def test_iny_sty_bne(self):
        m = asm_ast.Data(name="Z_PC2_L", offset=0)
        out = _run([
            asm_ast.Mov(src=Y, dst=A, is_volatile=False),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.NE(), target=".l"),
            asm_ast.Label(name=".l"),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out[:3], [
            asm_ast.Inc(dst=Y),
            asm_ast.Mov(src=Y, dst=m, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.NE(), target=".l"),
        ])

    def test_kept_when_carry_is_read(self):
        m = asm_ast.Data(name="lo", offset=0)
        instrs = [
            asm_ast.Mov(src=Y, dst=A, is_volatile=False),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=1), dst=A),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.CS(), target=".l"),
            asm_ast.Label(name=".l"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(list(instrs)), instrs)


class TestIndexStep(unittest.TestCase):
    """`LDA M; CLC; ADC #k; TAY` as `INY` k times where Y still holds
    M, as in Zork's `parse[(uint8_t)(token_idx + 1)]` after
    `LDY token_idx`."""

    _M = asm_ast.Data(name="idx", offset=0)

    def _run(self, instrs, static=True):
        # `idx` is a static whose address the unit never takes, unless
        # `static` is false, when it's a global another unit may point
        # at.
        fn = asm_ast.Function(
            name="f", is_global=True, params=[], instructions=instrs,
        )
        var = asm_ast.StaticVariable(
            name="idx", is_global=not static,
            init=[asm_ast.ZeroInit(bytes=1)],
        )
        out = apply_transfer_pm1_store(asm_ast.Program(top_level=[fn, var]))
        return out.top_level[0].instructions

    def _step(self, k=1, between=(), reg=Y):
        p = asm_ast.IndirectZpY(address=0x90)
        return [
            asm_ast.Mov(src=self._M, dst=reg, is_volatile=False),
            asm_ast.Mov(src=A, dst=p, is_volatile=False),
            *between,
            asm_ast.Mov(src=self._M, dst=A, is_volatile=False),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=asm_ast.Imm(value=k), dst=A),
            asm_ast.Mov(src=A, dst=reg, is_volatile=False),
            asm_ast.Mov(src=X, dst=p, is_volatile=False),
            asm_ast.Return(save_a=False),
        ]

    @staticmethod
    def _stepped_after_transfer(instrs):
        """`instrs` with `CLC; ADC #k; TAY` as `TAY; INY` k times, the
        rewrite for a register that doesn't hold M."""
        out = list(instrs)
        for n in range(len(out) - 2):
            if isinstance(out[n], asm_ast.ClearCarry):
                k, move = out[n + 1].src.value, out[n + 2]
                return (out[:n] + [move] + [asm_ast.Inc(dst=move.dst)] * k
                        + out[n + 3:])
        raise AssertionError("no step")

    def test_iny(self):
        out = self._run(self._step())
        self.assertEqual(out[2], asm_ast.Inc(dst=Y))
        self.assertEqual(len(out), 5)

    def test_two_inys(self):
        out = self._run(self._step(k=2))
        self.assertEqual(out[2:4], [asm_ast.Inc(dst=Y)] * 2)

    def test_store_through_pointer_may_change_a_global(self):
        # The store through ($90),Y could write a global's byte.
        instrs = self._step()
        self.assertEqual(self._run(list(instrs), static=False),
                         self._stepped_after_transfer(instrs))

    def test_register_changed_since_load(self):
        # INX changes X after `LDX idx`, which `kills_x` doesn't count
        # since INX also reads X. beam_target_tick read the wrong input
        # when this rewrote `LDA m; CLC; ADC #2; TAX` to `INX; INX`.
        instrs = self._step(k=2, reg=X, between=[asm_ast.Inc(dst=X)])
        self.assertEqual(self._run(list(instrs)),
                         self._stepped_after_transfer(instrs))

    def test_byte_written_since_load(self):
        instrs = self._step(between=[
            asm_ast.Mov(src=A, dst=self._M, is_volatile=False),
        ])
        self.assertEqual(self._run(list(instrs)),
                         self._stepped_after_transfer(instrs))


class TestStepAcrossTransfer(unittest.TestCase):
    """`CLC; ADC #k; TAY` as `TAY; INY`, and `TYA; CLC; ADC #k` as
    `INY; TYA`, as in Zork's `match_dictionary_word` stepping its index
    through the dictionary's header."""

    def _clc_adc(self, k=1):
        return [asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Imm(value=k), dst=A)]

    def test_step_after_tay(self):
        out = _run(self._clc_adc() + [
            asm_ast.Mov(src=A, dst=Y, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out, [
            asm_ast.Mov(src=A, dst=Y, is_volatile=False),
            asm_ast.Inc(dst=Y),
            asm_ast.Return(save_a=False),
        ])

    def test_step_after_tay_needs_a_dead(self):
        instrs = self._clc_adc() + [
            asm_ast.Mov(src=A, dst=Y, is_volatile=False),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_step_before_tya(self):
        m = asm_ast.Data(name="m", offset=0)
        out = _run([asm_ast.Mov(src=Y, dst=A, is_volatile=False)]
                   + self._clc_adc(2)
                   + [asm_ast.Mov(src=A, dst=m, is_volatile=False),
                      asm_ast.Return(save_a=False)])
        self.assertEqual(out, [
            asm_ast.Inc(dst=Y), asm_ast.Inc(dst=Y),
            asm_ast.Mov(src=Y, dst=A, is_volatile=False),
            asm_ast.Mov(src=A, dst=m, is_volatile=False),
            asm_ast.Return(save_a=False),
        ])

    def test_step_before_tya_needs_y_dead(self):
        m = asm_ast.Data(name="m", offset=0)
        instrs = ([asm_ast.Mov(src=Y, dst=A, is_volatile=False)]
                  + self._clc_adc()
                  + [asm_ast.Mov(src=A, dst=m, is_volatile=False),
                     asm_ast.Mov(src=Y, dst=m, is_volatile=False),
                     asm_ast.Return(save_a=False)])
        self.assertEqual(_run(list(instrs)), instrs)

    def test_transfer_back_is_one_step(self):
        # `TXA; CLC; ADC #1; TAX` is `INX`.
        tbl = asm_ast.IndexedData(name="tbl", offset=0, index=asm_ast.X())
        out = _run([asm_ast.Mov(src=X, dst=A, is_volatile=False)]
                   + self._clc_adc()
                   + [asm_ast.Mov(src=A, dst=X, is_volatile=False),
                      asm_ast.Mov(src=tbl, dst=A, is_volatile=False),
                      asm_ast.Return(save_a=True)])
        self.assertEqual(out, [
            asm_ast.Inc(dst=X),
            asm_ast.Mov(src=tbl, dst=A, is_volatile=False),
            asm_ast.Return(save_a=True),
        ])


class TestStepThenReload(unittest.TestCase):
    """`LDA M; SEC; SBC #2; STA M; LDX M` as `LDX M; DEX; DEX; STX M`,
    as in Zork's `return_value` stepping the index of the local it
    restores."""

    M = asm_ast.Data(name="__local_f__x", offset=0)

    def _step(self, after):
        return [
            asm_ast.Mov(src=self.M, dst=A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=asm_ast.Imm(value=2), dst=A),
            asm_ast.Mov(src=A, dst=self.M),
            asm_ast.Mov(src=self.M, dst=X),
            *after,
            asm_ast.Return(save_a=False),
        ]

    def test_step_in_x(self):
        out = _run(self._step([
            asm_ast.Mov(src=asm_ast.Data(name="v", offset=0), dst=A),
        ]))
        self.assertEqual(out[:4], [
            asm_ast.Mov(src=self.M, dst=X),
            asm_ast.Dec(dst=X), asm_ast.Dec(dst=X),
            asm_ast.Mov(src=X, dst=self.M),
        ])

    def test_kept_when_a_is_read_after(self):
        instrs = self._step([
            asm_ast.Mov(src=A, dst=asm_ast.Data(name="v", offset=0)),
        ])
        self.assertEqual(_run(list(instrs)), instrs)


if __name__ == "__main__":
    unittest.main()
