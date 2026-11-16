"""Tests for `passes.branch_value_thread.apply_branch_value_thread`: a
branch goes past the loads, compares, and branches at its target that
its path has already decided."""

import shutil
import unittest

import asm_ast
from passes.branch_value_thread import apply_branch_value_thread
from sim.harness import build_sim


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])
    return apply_branch_value_thread(prog).top_level[0].instructions


def _br(cond, target):
    return asm_ast.Branch(cond=cond, target=target)


_A = asm_ast.Reg(reg=asm_ast.A())
_LO = asm_ast.Data(name="b", offset=0)
_HI = asm_ast.Data(name="b", offset=1)
_N = asm_ast.Data(name="n", offset=0)


def _lda(op):
    return asm_ast.Mov(src=op, dst=_A)


def _cmp(k):
    return asm_ast.Compare(left=_A, right=asm_ast.Imm(value=k))


def _two_tests():
    """`if (b == 2) n = 1; else if (b == 4) n = 2;` on a 2-byte b,
    as the compiler leaves it."""
    return [
        _lda(_HI), _br(asm_ast.NE(), ".else"),
        _lda(_LO), _cmp(2), _br(asm_ast.NE(), ".else"),
        asm_ast.Inc(dst=_N),
        asm_ast.Jump(target=".end"),
        asm_ast.Label(name=".else"),
        _lda(_HI), _br(asm_ast.NE(), ".end"),
        _lda(_LO), _cmp(4), _br(asm_ast.NE(), ".end"),
        asm_ast.Dec(dst=_N),
        asm_ast.Label(name=".end"),
        asm_ast.Return(save_a=False),
    ]


class TestThreading(unittest.TestCase):

    def test_high_byte_tested_once(self):
        out = _run(_two_tests())
        # The high byte isn't 0: the second test's high-byte branch is
        # taken too.
        self.assertEqual(out[1], _br(asm_ast.NE(), ".end"))
        # The low byte isn't 2, and the high byte is 0: past the
        # second test's high-byte branch and its reload of the low
        # byte, to the CMP #4.
        self.assertEqual(out[4], _br(asm_ast.NE(), ".vthread@0"))
        k = out.index(asm_ast.Label(name=".vthread@0"))
        self.assertEqual(out[k + 1], _cmp(4))

    def test_store_forgets(self):
        # A store to the high byte between the tests: nothing is known
        # of it at the second.
        instrs = _two_tests()
        instrs.insert(2, asm_ast.Mov(src=_A, dst=_HI))
        out = _run(instrs)
        self.assertEqual(out[5], _br(asm_ast.NE(), ".else"))

    def test_volatile_not_followed(self):
        instrs = _two_tests()
        instrs[0] = asm_ast.Mov(src=_HI, dst=_A, is_volatile=True)
        out = _run(instrs)
        self.assertEqual(out[1], _br(asm_ast.NE(), ".else"))

    def test_a_live_with_another_value(self):
        # At the target, A is loaded with the low byte, and the code
        # after reads A: an edge where A holds the high byte can't
        # skip that load.
        out = _run([
            _lda(_HI), _br(asm_ast.EQ(), ".t"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".t"),
            _lda(_LO), _br(asm_ast.NE(), ".x"),
            asm_ast.Mov(src=_A, dst=_N),
            asm_ast.Label(name=".x"),
            asm_ast.Return(save_a=False),
        ])
        self.assertEqual(out[1], _br(asm_ast.EQ(), ".t"))


_PROGRAM = """
#include <stdint.h>
uint16_t t[8] = {0, 2, 4, 6, 0x102, 0x104, 0x200, 0xFFFF};
int r[8];
static int scale(uint16_t a, uint16_t b) {
    if (b == 2)
        return a << 1;
    else if (b == 4)
        return a << 2;
    return a + b;
}
int main(void) {
    for (uint8_t i = 0; i != 8; i++)
        r[i] = scale(3, t[i]);
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestRuns(unittest.TestCase):

    def test_values(self):
        want = [3, 6, 12, 9, 0x105, 0x107, 0x203, 2]
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(_PROGRAM, optimize=optimize)
                sim.run()
                base = sim.symbols["r"]
                mem = sim.mpu.memory
                got = [mem[base + 2 * k] | mem[base + 2 * k + 1] << 8
                       for k in range(8)]
                self.assertEqual(got, want)


if __name__ == "__main__":
    unittest.main()
