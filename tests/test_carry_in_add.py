"""Tests for `passes/carry_in_add.py`: `p + b + 1` as one add with the
carry set."""

from __future__ import annotations

import shutil
import unittest

import asm_ast
from passes.carry_in_add import apply_carry_in_add
from sim.harness import build_sim
from tests.no_inlining import no_inlining

_A = asm_ast.Reg(reg=asm_ast.A())
_M = asm_ast.Data(name="m", offset=0)
_H = asm_ast.Data(name="__local_f__h", offset=0)
_T0 = asm_ast.Data(name="__local_f__t0", offset=0)
_T1 = asm_ast.Data(name="__local_f__t1", offset=0)
_R0 = asm_ast.Data(name="r", offset=0)
_R1 = asm_ast.Data(name="r", offset=1)
_ZERO = asm_ast.Imm(value=0)
_ONE = asm_ast.Imm(value=1)


def _mov(src, dst):
    return asm_ast.Mov(src=src, dst=dst, is_volatile=False)


def _two_adds(h=_H, t1=_T1, r0=_R0):
    return [
        asm_ast.ClearCarry(), asm_ast.Add(src=_M, dst=_A), _mov(_A, _T0),
        _mov(h, _A), asm_ast.Add(src=_ZERO, dst=_A), _mov(_A, t1),
        _mov(_T0, _A), asm_ast.ClearCarry(), asm_ast.Add(src=_ONE, dst=_A),
        _mov(_A, r0), _mov(t1, _A), asm_ast.Add(src=_ZERO, dst=_A),
        _mov(_A, _R1),
    ]


def _run(instrs):
    fn = asm_ast.Function(name="f", is_global=True, params=[],
                          instructions=instrs)
    out = apply_carry_in_add(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


class TestCarryInAdd(unittest.TestCase):

    def test_folds(self):
        out = _run(_two_adds() + [asm_ast.Return(save_a=False)])
        self.assertEqual(out, [
            asm_ast.SetCarry(), asm_ast.Add(src=_M, dst=_A), _mov(_A, _R0),
            _mov(_H, _A), asm_ast.Add(src=_ZERO, dst=_A), _mov(_A, _R1),
            asm_ast.Return(save_a=False),
        ])

    def test_high_byte_added_in_place(self):
        out = _run(_two_adds(t1=_H) + [asm_ast.Return(save_a=False)])
        self.assertEqual(len(out), 7)

    def test_kept_when_c_is_read(self):
        instrs = _two_adds() + [
            asm_ast.Branch(cond=asm_ast.CS(), target=".l"),
            asm_ast.Label(name=".l"), asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_when_the_high_byte_is_a_global(self):
        # `h` is read after the store to `r0`, which might be it.
        instrs = _two_adds(h=asm_ast.Data(name="g", offset=0)) + [
            asm_ast.Return(save_a=False)]
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_when_a_temporary_is_read_later(self):
        instrs = _two_adds() + [_mov(_T1, asm_ast.Data(name="g", offset=0)),
                                asm_ast.Return(save_a=False)]
        self.assertEqual(_run(list(instrs)), instrs)


_SRC = """
#include <stdint.h>
static const uint8_t table[] = {3, 'a', 'b', 'c', 0, 255};
static const uint8_t *after(const uint8_t *s) { return s + s[0] + 1; }
static uint16_t sum(uint16_t p, uint8_t b) { return p + b + 1; }
uint16_t main(void) {
    static const uint16_t ps[] = {0, 1, 0xFF, 0x1FF, 0xFFFE, 0xFFFF};
    static const uint8_t bs[] = {0, 1, 0x7F, 0xFE, 0xFF};
    uint16_t n = (uint16_t)(after(table) - table);
    n = n * 3 + (uint16_t)(after(table + 4) - table);
    for (uint8_t i = 0; i < 6; i++)
        for (uint8_t j = 0; j < 5; j++)
            n = n * 7 + sum(ps[i], bs[j]);
    return n;
}
"""


def _expected() -> int:
    n = 4
    n = (n * 3 + 5) & 0xFFFF
    for p in [0, 1, 0xFF, 0x1FF, 0xFFFE, 0xFFFF]:
        for b in [0, 1, 0x7F, 0xFE, 0xFF]:
            n = (n * 7 + ((p + b + 1) & 0xFFFF)) & 0xFFFF
    return n


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_values(self):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(_SRC, optimize=optimize).run()
                self.assertEqual(r.return_int(), _expected())

    @no_inlining()
    def test_one_add(self):
        from compile import _run_stage
        from preprocessor import preprocess
        asm = _run_stage("codegen", preprocess(_SRC), optimize=True)
        body = asm[asm.index("\nafter:"):asm.index("\nsum:")]
        self.assertIn("SEC", body)
        self.assertNotIn("#$01", body)


if __name__ == "__main__":
    unittest.main()
