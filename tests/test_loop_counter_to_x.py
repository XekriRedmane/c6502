"""Tests for the loop-counter-to-X promotion pass."""

import unittest

import asm_ast
from passes.loop_counter_to_x import apply_loop_counter_to_x


_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


def _M(name="b"):
    return asm_ast.Data(name=name, offset=0)


class TestLoopCounterToX(unittest.TestCase):

    def test_canonical_pattern_fires(self):
        # LDA p; STA b; loop_start: LDX b; ...; DEC b; BPL loop_start.
        prog = _wrap([
            asm_ast.Mov(src=_M("p"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("b")),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Mov(src=asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.X()), dst=_A),
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        # TAX inserted after the init STA.
        self.assertEqual(out[1], asm_ast.Mov(src=_A, dst=_M("b")))
        self.assertEqual(out[2], asm_ast.Mov(src=_A, dst=_X))  # TAX
        # The LDX at loop top is dropped. No Calls in this test, so
        # no STX-before-Call insertion. The tail's DEC becomes DEX
        # with no trailing STX — X carries to next iter via the
        # back-edge.
        # New shape: [LDA p, STA b, TAX, Label, LDA arr,X, DEX, BPL, Return]
        self.assertEqual(len(out), 8)
        # DEC b replaced with DEX.
        self.assertTrue(any(isinstance(i, asm_ast.Dec) and isinstance(i.dst, asm_ast.Reg)
                            and isinstance(i.dst.reg, asm_ast.X)
                            for i in out))

    def test_other_use_of_M_disqualifies(self):
        # Adding a `Compare(_, M)` use disqualifies — M now has a
        # read that isn't an LDX or DEC.
        prog = _wrap([
            asm_ast.Mov(src=_M("p"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("b")),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Compare(left=_A, right=_M("b")),       # other use!
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        # Unchanged.
        self.assertEqual(len(out), 8)

    def test_passive_label_between_dec_and_branch_allowed(self):
        prog = _wrap([
            asm_ast.Mov(src=_M("p"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("b")),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Label(name=".loop_continue"),         # passive
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        # Fires: TAX inserted, loop-top LDX dropped, Dec(M) → Dec(X).
        self.assertTrue(any(isinstance(i, asm_ast.Mov)
                            and i.src == _A and i.dst == _X for i in out))
        self.assertTrue(any(isinstance(i, asm_ast.Dec)
                            and isinstance(i.dst, asm_ast.Reg)
                            and isinstance(i.dst.reg, asm_ast.X) for i in out))

    def test_active_label_between_dec_and_branch_blocks(self):
        # A label between DEC and Branch that something else jumps
        # to means there's a control path bypassing the DEC — our
        # transform would be unsound on that path.
        prog = _wrap([
            asm_ast.Jump(target=".active"),                # makes .active a target
            asm_ast.Mov(src=_M("p"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("b")),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Label(name=".active"),                 # bypass entry
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        # Unchanged.
        self.assertEqual(len(out), 9)


if __name__ == "__main__":
    unittest.main()
