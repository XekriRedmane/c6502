"""Tests for `passes.counter_to_index_reg`."""

from __future__ import annotations

import unittest

import asm_ast
from passes.counter_to_index_reg import apply_counter_to_index_reg

_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())
_N = asm_ast.Data(name="n", offset=0)
_COUNT = asm_ast.ZP(address=0x84, offset=0)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )])
    return apply_counter_to_index_reg(prog).top_level[0].instructions


def _loop(body):
    return [
        asm_ast.Mov(src=_N, dst=_COUNT, is_volatile=False),
        asm_ast.Label(name=".loop"),
        *body,
        asm_ast.Dec(dst=_COUNT),
        asm_ast.Branch(cond=asm_ast.NE(), target=".loop"),
        asm_ast.Return(save_a=False),
    ]


class TestCounterToIndexReg(unittest.TestCase):

    def test_counter_moves_to_x(self):
        body = [asm_ast.Inc(dst=asm_ast.Data(name="t", offset=0))]
        out = _run(_loop(body))
        self.assertEqual(out[0], asm_ast.Mov(src=_N, dst=_X, is_volatile=False))
        self.assertEqual(out[-3], asm_ast.Dec(dst=_X))

    def test_x_used_in_loop_falls_back_to_y(self):
        body = [asm_ast.Inc(dst=_X)]
        out = _run(_loop(body))
        self.assertEqual(out[0], asm_ast.Mov(src=_N, dst=_Y, is_volatile=False))
        self.assertEqual(out[-3], asm_ast.Dec(dst=_Y))

    def test_both_registers_used_leaves_counter_in_memory(self):
        body = [asm_ast.Inc(dst=_X), asm_ast.Inc(dst=_Y)]
        instrs = _loop(body)
        self.assertEqual(_run(instrs), instrs)

    def test_call_in_loop_leaves_counter_in_memory(self):
        instrs = _loop([asm_ast.Call(name="g", reg_args=[])])
        self.assertEqual(_run(instrs), instrs)

    def test_counter_read_elsewhere_leaves_it_in_memory(self):
        body = [asm_ast.Mov(src=_COUNT, dst=_A, is_volatile=False)]
        instrs = _loop(body)
        self.assertEqual(_run(instrs), instrs)

    def test_register_use_on_exit_path_is_fine(self):
        # The `LDX #0` returns without reaching the DEC again, so the
        # counter is dead there.
        body = [
            asm_ast.Branch(cond=asm_ast.EQ(), target=".found"),
            asm_ast.Jump(target=".next"),
            asm_ast.Label(name=".found"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_X, is_volatile=False),
            asm_ast.Return(save_a=True, save_x=True),
            asm_ast.Label(name=".next"),
        ]
        out = _run(_loop(body))
        self.assertEqual(out[0], asm_ast.Mov(src=_N, dst=_X, is_volatile=False))


class TestInitFromARegister(unittest.TestCase):
    """An init stored from X or Y. The 6502 has no TXY or TYX."""

    def _loop_from(self, reg, after=()):
        return [
            asm_ast.Mov(src=asm_ast.Imm(value=17), dst=reg, is_volatile=False),
            asm_ast.Mov(src=reg, dst=_COUNT, is_volatile=False),
            asm_ast.Label(name=".loop"),
            asm_ast.Inc(dst=asm_ast.Data(name="t", offset=0)),
            asm_ast.Dec(dst=_COUNT),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop"),
            *after,
            asm_ast.Return(save_a=False),
        ]

    def test_counter_stays_in_the_register_it_came_from(self):
        out = _run(self._loop_from(_Y))
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Imm(value=17), dst=_Y, is_volatile=False),
            asm_ast.Label(name=".loop"),
            asm_ast.Inc(dst=asm_ast.Data(name="t", offset=0)),
            asm_ast.Dec(dst=_Y),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop"),
            asm_ast.Return(save_a=False),
        ])

    def test_never_moved_between_x_and_y(self):
        # Y is read after the loop, so the counter can't count it
        # down, and X would take `Mov(Y, X)`, which is `TYA; TAX`.
        instrs = self._loop_from(_Y, after=[
            asm_ast.Mov(src=_Y, dst=asm_ast.Data(name="u", offset=0),
                        is_volatile=False),
        ])
        out = _run(list(instrs))
        self.assertNotIn(
            asm_ast.Mov(src=_Y, dst=_X, is_volatile=False), out,
        )
        self.assertEqual(out, instrs)


class TestOutOfLineBlock(unittest.TestCase):
    """A loop with one of its blocks after the function's return, as
    SSA destruction leaves for one side of an `if`, is still one loop."""

    def _loop(self, jump_in_from_outside=False):
        t = asm_ast.Data(name="t", offset=0)
        out = [
            asm_ast.Mov(src=asm_ast.Imm(value=17), dst=_COUNT, is_volatile=False),
            asm_ast.Label(name=".loop"),
            asm_ast.Mov(src=t, dst=_A, is_volatile=False),
            asm_ast.Branch(cond=asm_ast.CC(), target=".split"),
            asm_ast.Inc(dst=t),
            asm_ast.Label(name=".join"),
            asm_ast.Dec(dst=_COUNT),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".split"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A, is_volatile=False),
            asm_ast.Jump(target=".join", reg_args=[]),
        ]
        if jump_in_from_outside:
            out = [asm_ast.Jump(target=".join", reg_args=[])] + out
        return out

    def test_counter_moves_to_x(self):
        out = _run(self._loop())
        self.assertIn(asm_ast.Dec(dst=_X), out)

    def test_jump_into_the_loop_from_outside(self):
        # The counter would arrive at .join without its init.
        instrs = self._loop(jump_in_from_outside=True)
        self.assertEqual(_run(list(instrs)), instrs)


if __name__ == "__main__":
    unittest.main()
