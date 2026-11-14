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


if __name__ == "__main__":
    unittest.main()
