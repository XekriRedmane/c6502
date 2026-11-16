"""Tests for `passes.cross_jump.apply_cross_jump`: of two blocks that
end in the same instructions and a jump to the same place, the later
jumps into the earlier one's copy."""

import shutil
import unittest

import asm_ast
from passes.cross_jump import apply_cross_jump
from sim.harness import build_sim


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_cross_jump(prog).top_level[0].instructions


_A = asm_ast.Reg(reg=asm_ast.A())
_N0 = asm_ast.Data(name="n", offset=0)
_N1 = asm_ast.Data(name="n", offset=1)


def _shift():
    return [asm_ast.ArithmeticShiftLeft(dst=_N0),
            asm_ast.RotateLeft(dst=_N1)]


class TestCrossJump(unittest.TestCase):

    def test_shared_shift(self):
        out = _run([
            asm_ast.Branch(cond=asm_ast.NE(), target=".x4"),
            *_shift(),
            asm_ast.Jump(target=".end"),
            asm_ast.Label(name=".x4"),
            *_shift(),
            *_shift(),
            asm_ast.Jump(target=".end2"),
            asm_ast.Label(name=".end"),
            asm_ast.Label(name=".end2"),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out, [
            asm_ast.Branch(cond=asm_ast.NE(), target=".x4"),
            asm_ast.Label(name=".xjump@0"),
            *_shift(),
            asm_ast.Jump(target=".end"),
            asm_ast.Label(name=".x4"),
            *_shift(),
            asm_ast.Jump(target=".xjump@0"),
            asm_ast.Label(name=".end"),
            asm_ast.Label(name=".end2"),
            asm_ast.Return(save_a=False),
        ])

    def test_tail_calls(self):
        # Two tail calls of the same function after the same stores.
        store = [asm_ast.Mov(src=_A, dst=_N0), asm_ast.Inc(dst=_N1)]
        out = _run([
            asm_ast.Branch(cond=asm_ast.EQ(), target=".b"),
            *store, asm_ast.Jump(target="g"),
            asm_ast.Label(name=".b"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A),
            *store, asm_ast.Jump(target="g"),
        ])
        self.assertEqual(out[-1], asm_ast.Jump(target=".xjump@0"))
        self.assertEqual(out.count(store[0]), 1)

    def test_one_instruction_left_alone(self):
        # Sharing one `LDA #1` would save 2 bytes for a 3-cycle JMP.
        one = asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_A)
        instrs = [
            asm_ast.Branch(cond=asm_ast.EQ(), target=".b"),
            one, asm_ast.Jump(target=".end"),
            asm_ast.Label(name=".b"),
            asm_ast.Inc(dst=_N0),
            one, asm_ast.Jump(target=".end"),
            asm_ast.Label(name=".end"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_different_destinations(self):
        instrs = [
            asm_ast.Branch(cond=asm_ast.NE(), target=".b"),
            *_shift(), asm_ast.Jump(target="g"),
            asm_ast.Label(name=".b"),
            *_shift(), asm_ast.Jump(target="h"),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_stops_at_a_label(self):
        # Another path enters the second run at `.mid`, so only the
        # part after the label could go, and that part differs.
        instrs = [
            asm_ast.Branch(cond=asm_ast.NE(), target=".b"),
            asm_ast.Inc(dst=_N0), asm_ast.Jump(target="g"),
            asm_ast.Label(name=".b"),
            asm_ast.Inc(dst=_N0),
            asm_ast.Label(name=".mid"),
            asm_ast.Jump(target="g"),
        ]
        self.assertEqual(_run(instrs), instrs)


_PROGRAM = """
#include <stdint.h>
uint16_t t[5] = {1, 2, 4, 7, 0x300};
uint16_t r[5];
static uint16_t scale(uint16_t a, uint16_t b) {
    uint16_t p;
    if (b == 2)
        p = a << 1;
    else if (b == 4)
        p = a << 2;
    else
        p = a + b;
    return p;
}
int main(void) {
    for (uint8_t i = 0; i != 5; i++)
        r[i] = scale(0x81, t[i]);
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_values(self):
        want = [0x82, 0x102, 0x204, 0x88, 0x381]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                base = sim.symbols["r"]
                mem = sim.mpu.memory
                got = [mem[base + 2 * k] | mem[base + 2 * k + 1] << 8
                       for k in range(5)]
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
