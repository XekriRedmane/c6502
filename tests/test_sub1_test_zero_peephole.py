"""Tests for `passes.sub1_test_zero_peephole.apply_sub1_test_zero_peephole`."""

import unittest

import asm_ast
from passes.sub1_test_zero_peephole import apply_sub1_test_zero_peephole


_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_M = asm_ast.Data(name="n", offset=0)


def _run(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    out = apply_sub1_test_zero_peephole(asm_ast.Program(top_level=[fn]))
    return out.top_level[0].instructions


def _pattern(cond=None):
    """`LDA n; SEC; SBC #1; TAX; LDA #0; CMP n; BCC .l`."""
    return [
        asm_ast.Mov(src=_M, dst=_A, is_volatile=False),
        asm_ast.SetCarry(),
        asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_A),
        asm_ast.Mov(src=_A, dst=_X, is_volatile=False),
        asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A, is_volatile=False),
        asm_ast.Compare(left=_A, right=_M),
        asm_ast.Branch(cond=cond or asm_ast.CC(), target=".l"),
    ]


def _reload():
    return asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_A, is_volatile=False)


class TestSub1TestZero(unittest.TestCase):

    def test_folds_when_outputs_dead(self):
        instrs = _pattern() + [
            _reload(),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="out", offset=0),
                        is_volatile=False),
            asm_ast.Label(name=".l"),
            asm_ast.Return(save_a=False),
        ]
        out = _run(instrs)
        self.assertEqual(out[:4], instrs[:4])
        self.assertEqual(out[4], asm_ast.Branch(cond=asm_ast.CS(), target=".l"))
        self.assertEqual(out[5:], instrs[7:])

    def test_skips_when_a_read_on_fall_through(self):
        # `STA out` reads the 0 that `LDA #0` left in A. After the
        # rewrite A would hold n - 1.
        instrs = _pattern() + [
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="out", offset=0),
                        is_volatile=False),
            asm_ast.Label(name=".l"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_skips_when_a_read_at_target(self):
        instrs = _pattern() + [
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".l"),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_skips_when_z_read_at_target(self):
        # Z comes from the `CMP` in the original and from the `TAX`
        # after the rewrite.
        instrs = _pattern() + [
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".l"),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".z"),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".z"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(instrs), instrs)

    def test_skips_when_c_read_on_fall_through(self):
        # C is set iff n == 0 after the `CMP`, and iff n != 0 after the
        # `SBC`, so a later `ADC` would add a different carry.
        instrs = _pattern() + [
            _reload(),
            asm_ast.Add(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Return(save_a=True),
            asm_ast.Label(name=".l"),
            asm_ast.Return(save_a=False),
        ]
        self.assertEqual(_run(instrs), instrs)


if __name__ == "__main__":
    unittest.main()
