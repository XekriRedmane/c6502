"""Tests for `passes.carry_recompute`: a shift that only sets C to what
C already holds goes, and for the lowering of adds and subtracts of a
constant whose low bytes are zero (`tac_to_asm._translate_add_sub`)."""

import shutil
import unittest

import asm_ast
from compile import _run_stage
from passes.carry_recompute import apply_carry_recompute_drop
from preprocessor import preprocess
from sim.harness import build_sim

_A = asm_ast.Reg(reg=asm_ast.A())
_S0 = asm_ast.Data(name="S", offset=0)
_S1 = asm_ast.Data(name="S", offset=1)
_L = asm_ast.Data(name="L", offset=0)
_H = asm_ast.Data(name="H", offset=0)


def _run(instrs):
    prog = asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )])
    return apply_carry_recompute_drop(prog).top_level[0].instructions


def _mov(src, dst):
    return asm_ast.Mov(src=src, dst=dst)


def _chain(*between):
    """`LDA S; ASL; STA L; <between>; LDA S; ASL; LDA S+1; ROL; STA H`."""
    return [
        _mov(_S0, _A), asm_ast.ArithmeticShiftLeft(dst=_A), _mov(_A, _L),
        *between,
        _mov(_S0, _A), asm_ast.ArithmeticShiftLeft(dst=_A),
        _mov(_S1, _A), asm_ast.RotateLeft(dst=_A), _mov(_A, _H),
        asm_ast.Return(save_a=False),
    ]


class TestCarryRecompute(unittest.TestCase):

    def test_drops_the_recompute(self):
        instrs = _chain()
        out = _run(instrs)
        self.assertEqual(out, instrs[:3] + instrs[5:])

    def test_kept_when_the_byte_changes(self):
        instrs = _chain(asm_ast.Inc(dst=_S0))
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_after_a_store_through_a_pointer(self):
        instrs = _chain(_mov(_A, asm_ast.IndirectZpY(address=0x80)))
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_when_c_changes(self):
        instrs = _chain(asm_ast.ClearCarry())
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_across_a_label(self):
        instrs = _chain(asm_ast.Label(name=".l"))
        self.assertEqual(_run(list(instrs)), instrs)

    def test_kept_when_a_is_read(self):
        # The shifted value is stored, so the shift has to stay.
        instrs = [
            _mov(_S0, _A), asm_ast.ArithmeticShiftLeft(dst=_A), _mov(_A, _L),
            _mov(_S0, _A), asm_ast.ArithmeticShiftLeft(dst=_A), _mov(_A, _H),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(list(instrs)), instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestZeroLowBytes(unittest.TestCase):

    def test_page_steps(self):
        src = """
        unsigned int p;
        unsigned long q;
        unsigned int r;
        void up(void) { p += 0x100; }
        void down(void) { p -= 0x100; }
        void far(void) { q += 0x20000; }
        void copy(void) { r = p + 0x300; }
        int main(void) {
            p = 0x12FF; up(); up(); down();
            q = 0x1FFFF; far();
            copy();
            p = 0x0050; down();
            return (int)(p ^ (unsigned)(q >> 16) ^ r);
        }
        """
        p = (0x12FF + 0x100) & 0xFFFF
        q = (0x1FFFF + 0x20000) & 0xFFFFFFFF
        r = (p + 0x300) & 0xFFFF
        p = (0x0050 - 0x100) & 0xFFFF
        want = (p ^ (q >> 16) ^ r) & 0xFFFF
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, want)
        asm = _run_stage("codegen", preprocess(src), optimize=True)
        self.assertIn("INC   p+1", asm.split("\nup:\n")[1].split("RTS")[0])
        self.assertIn("DEC   p+1", asm.split("\ndown:\n")[1].split("RTS")[0])


if __name__ == "__main__":
    unittest.main()
