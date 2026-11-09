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


if __name__ == "__main__":
    unittest.main()
