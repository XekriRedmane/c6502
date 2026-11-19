"""Tests for `passes/carry_halve.py`: a halved 9-bit sum takes bit 8
from the carry of its low byte's add."""

from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.carry_halve import apply_carry_halve
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_Y = asm_ast.Reg(reg=asm_ast.Y())
_T = asm_ast.Data(name="__local_f__t", offset=0)
_S = asm_ast.Data(name="s", offset=0)
_ZERO = asm_ast.Imm(value=0)


def _mov(src, dst):
    return asm_ast.Mov(src=src, dst=dst, is_volatile=False)


def _halve(zero_load=None):
    """`CLC; ADC s; STA t; LDA #0; ADC #0; LSR; LDA t; ROR`."""
    return [
        asm_ast.ClearCarry(),
        asm_ast.Add(src=_S, dst=_A),
        _mov(_A, _T),
        zero_load or _mov(_ZERO, _A),
        asm_ast.Add(src=_ZERO, dst=_A),
        asm_ast.LogicalShiftRight(dst=_A),
        _mov(_T, _A),
        asm_ast.RotateRight(dst=_A),
    ]


def _run(instrs):
    fn = asm_ast.Function(name="f", is_global=True, params=[],
                          instructions=instrs)
    out = apply_carry_halve(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


class TestCarryHalve(unittest.TestCase):

    def test_folds(self):
        out = _run(_halve() + [asm_ast.Return(save_a=True)])
        self.assertEqual(out, [
            asm_ast.ClearCarry(),
            asm_ast.Add(src=_S, dst=_A),
            asm_ast.RotateRight(dst=_A),
            asm_ast.Return(save_a=True),
        ])

    def test_tya_of_a_zero_y(self):
        out = _run([_mov(_ZERO, _Y)] + _halve(_mov(_Y, _A))
                   + [asm_ast.Return(save_a=True)])
        self.assertEqual(len(out), 5)

    def test_tya_of_an_unknown_y_kept(self):
        instrs = _halve(_mov(_Y, _A)) + [asm_ast.Return(save_a=True)]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_when_the_slot_is_read_later(self):
        instrs = _halve() + [_mov(_T, asm_ast.Data(name="g", offset=0)),
                             asm_ast.Return(save_a=True)]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_when_v_is_read(self):
        instrs = _halve() + [
            asm_ast.Branch(cond=asm_ast.VS(), target=".l"),
            asm_ast.Label(name=".l"), asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_run(list(instrs)), instrs)


_SRC = """
#include <stdint.h>
uint8_t col;
static uint8_t half(uint8_t a, uint8_t b) { return (a + b) / 2; }
static uint8_t round_up(uint8_t s) { return (s + 1) >> 1; }
static void center(uint8_t s) { col += (s + 1) / 2; }
uint16_t main(void) {
    static const uint8_t v[] = {0, 1, 2, 127, 128, 200, 254, 255};
    uint16_t n = 0;
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++)
            n = n * 7 + half(v[i], v[j]);
        col = 3;
        center(v[i]);
        n = n * 5 + round_up(v[i]) + col;
    }
    return n;
}
"""


def _expected() -> int:
    v = [0, 1, 2, 127, 128, 200, 254, 255]
    n = 0
    for a in v:
        for b in v:
            n = (n * 7 + (a + b) // 2) & 0xFFFF
        col = (3 + (a + 1) // 2) & 0xFF
        n = (n * 5 + ((a + 1) >> 1) + col) & 0xFFFF
    return n


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_values(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())

    def test_no_high_byte(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_SRC), optimize=True)
        body = asm[asm.index("\nhalf:"):asm.index("\nmain:")]
        self.assertNotIn("LSR", body)
        self.assertEqual(body.count("ROR"), 3)


if __name__ == "__main__":
    unittest.main()
