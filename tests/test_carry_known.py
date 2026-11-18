"""Tests for `passes.carry_known.apply_carry_known`: `SEC` and `CLC`
where the carry already has that value, or has it on every path but
the one into a loop."""

import unittest

import asm_ast
from passes.carry_known import apply_carry_known

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_S = asm_ast.Data(name="S", offset=0)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_carry_known(prog).top_level[0].instructions


def _ret():
    return asm_ast.Return(save_a=True)


class TestCarryKnown(unittest.TestCase):

    def test_known_set_and_clear(self):
        out = _run([
            asm_ast.SetCarry(),
            asm_ast.Mov(src=_S, dst=_A),
            asm_ast.SetCarry(),                       # still set
            asm_ast.Sub(src=_S, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".lt"),
            asm_ast.SetCarry(),                       # BCC not taken
            asm_ast.Sub(src=_S, dst=_A),
            _ret(),
            asm_ast.Label(name=".lt"),
            asm_ast.ClearCarry(),                     # BCC taken
            asm_ast.Add(src=_S, dst=_A),
            _ret(),
        ])
        self.assertEqual(out, [
            asm_ast.SetCarry(),
            asm_ast.Mov(src=_S, dst=_A),
            asm_ast.Sub(src=_S, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".lt"),
            asm_ast.Sub(src=_S, dst=_A),
            _ret(),
            asm_ast.Label(name=".lt"),
            asm_ast.Add(src=_S, dst=_A),
            _ret(),
        ])

    def test_moves_in_front_of_a_loop(self):
        # do_rwts_on_sector: C is set on the back edge.
        loop = [
            asm_ast.Sub(src=_S, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".done"),
            asm_ast.Inc(dst=asm_ast.Data(name="track", offset=0)),
            asm_ast.Jump(target=".loop"),
            asm_ast.Label(name=".done"),
            _ret(),
        ]
        out = _run([
            asm_ast.Mov(src=_S, dst=_X),
            asm_ast.Label(name=".loop"),
            asm_ast.SetCarry(),
        ] + loop)
        self.assertEqual(out, [
            asm_ast.Mov(src=_S, dst=_X),
            asm_ast.SetCarry(),
            asm_ast.Label(name=".loop"),
        ] + loop)

    def test_stays_when_a_path_back_changes_c(self):
        instrs = [
            asm_ast.Mov(src=_S, dst=_X),
            asm_ast.Label(name=".loop"),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=_S, dst=_A),
            asm_ast.Branch(cond=asm_ast.CC(), target=".done"),
            asm_ast.Compare(left=_A, right=_S),       # C unknown
            asm_ast.Jump(target=".loop"),
            asm_ast.Label(name=".done"),
            _ret(),
        ]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_stays_after_a_call(self):
        # The first SEC is dead, since the call overwrites C; the
        # second is needed, since C is unknown after the call.
        instrs = [
            asm_ast.SetCarry(),
            asm_ast.Call(name="g", reg_args=[], preserves=[]),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=_S, dst=_A),
            _ret(),
        ]
        self.assertEqual(_run(list(instrs)), instrs[1:])

    def test_dead_and_redundant_together(self):
        # A dead CLC followed by the one the ADC needs: only the dead
        # one goes, though it made the second look redundant.
        instrs = [
            asm_ast.Call(name="g", reg_args=[], preserves=[]),
            asm_ast.ClearCarry(),
            asm_ast.Mov(src=_S, dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Add(src=_S, dst=_A),
            _ret(),
        ]
        out = _run(list(instrs))
        self.assertEqual(out.count(asm_ast.ClearCarry()), 1)
        self.assertLess(
            out.index(asm_ast.ClearCarry()), out.index(asm_ast.Add(src=_S, dst=_A)),
        )



class TestAddOfZero(unittest.TestCase):
    """An `ADC #0` with C known clear, or `SBC #0` with C known set,
    leaves A and C as they were."""

    _LO = asm_ast.ImmLabelLow(name="LOCS", offset=0)
    _HI = asm_ast.ImmLabelHigh(name="LOCS", offset=0)
    _P = asm_ast.Data(name="P", offset=0)
    _P1 = asm_ast.Data(name="P", offset=1)
    _ZERO = asm_ast.Imm(value=0)

    def test_high_byte_of_an_address_plus_zero(self):
        # `P = &LOCS[0]` after the low byte's `ADC #0` was folded.
        out = _run([
            asm_ast.Mov(src=self._LO, dst=_A),
            asm_ast.ClearCarry(),
            asm_ast.Mov(src=_A, dst=self._P),
            asm_ast.Mov(src=self._HI, dst=_A),
            asm_ast.Add(src=self._ZERO, dst=_A),
            asm_ast.Mov(src=_A, dst=self._P1),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out, [
            asm_ast.Mov(src=self._LO, dst=_A),
            asm_ast.Mov(src=_A, dst=self._P),
            asm_ast.Mov(src=self._HI, dst=_A),
            asm_ast.Mov(src=_A, dst=self._P1),
            asm_ast.Return(save_a=False),
        ])

    def test_subtract_of_zero_with_carry_set(self):
        out = _run([
            asm_ast.SetCarry(),
            asm_ast.Mov(src=_S, dst=_A),
            asm_ast.Sub(src=self._ZERO, dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".z"),
            asm_ast.Label(name=".z"),
            _ret(),
        ])
        self.assertNotIn(asm_ast.Sub(src=self._ZERO, dst=_A), out)

    def test_stays_when_carry_unknown_or_v_read(self):
        unknown = [
            asm_ast.Mov(src=_S, dst=_A),
            asm_ast.Add(src=self._ZERO, dst=_A),
            _ret(),
        ]
        self.assertEqual(_run(unknown), unknown)
        v_read = [
            asm_ast.ClearCarry(),
            asm_ast.Mov(src=_S, dst=_A),
            asm_ast.Add(src=self._ZERO, dst=_A),
            asm_ast.Branch(cond=asm_ast.VC(), target=".v"),
            asm_ast.Label(name=".v"),
            _ret(),
        ]
        self.assertEqual(_run(v_read), v_read)

    def test_stays_when_flags_come_from_elsewhere(self):
        # N/Z after `ADC #0` come from A, and before it from the `BIT`,
        # which leaves C alone, so a branch on Z keeps it.
        instrs = [
            asm_ast.ClearCarry(),
            asm_ast.Mov(src=_S, dst=_A),
            asm_ast.BitTest(src=self._P),
            asm_ast.Add(src=self._ZERO, dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".z"),
            asm_ast.Label(name=".z"),
            _ret(),
        ]
        self.assertIn(asm_ast.Add(src=self._ZERO, dst=_A), _run(instrs))


if __name__ == "__main__":
    unittest.main()
