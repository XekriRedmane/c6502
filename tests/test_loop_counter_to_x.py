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

    def test_shifted_slot_not_promoted(self):
        # `x * 9` builds its high byte with `STA hi; ROL hi; ...;
        # INC hi; LDX hi`. The ROL is a use the promotion doesn't
        # rewrite, so moving `hi` to X would lose it.
        hi = _M("hi")
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=hi),
            asm_ast.RotateLeft(dst=hi),
            asm_ast.Inc(dst=hi),
            asm_ast.Mov(src=hi, dst=_X),
            asm_ast.Return(save_a=True, save_x=True),
        ]
        self.assertEqual(_instrs(apply_loop_counter_to_x(_wrap(instrs))),
                         instrs)

    def test_count_up_loop(self):
        # LDA #0; STA i; top: LDA i; CMP n; BEQ done; LDX i;
        # LDA arr,X; INC i; JMP top -> LDX #0 ... CPX n ... INX.
        i = _M("i")
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_A),
            asm_ast.Mov(src=_A, dst=i),
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=i, dst=_A),
            asm_ast.Compare(left=_A, right=_M("n")),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".done"),
            asm_ast.Mov(src=i, dst=_X),
            asm_ast.Mov(src=asm_ast.IndexedData(
                name="arr", offset=0, index=asm_ast.X()), dst=_A),
            asm_ast.Inc(dst=i),
            asm_ast.Jump(target=".top", reg_args=[]),
            asm_ast.Label(name=".done"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        self.assertIn(asm_ast.Compare(left=_X, right=_M("n")), out)
        self.assertIn(asm_ast.Inc(dst=_X), out)
        self.assertNotIn(asm_ast.Mov(src=i, dst=_X), out)
        self.assertNotIn(asm_ast.Inc(dst=i), out)

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

    def test_lda_m_rewritten_to_txa(self):
        # `Mov(M, Reg(A))` (LDA M) is value-equivalent to TXA after
        # promotion (since X = M is the invariant). The pass should
        # accept the slot and rewrite the LDA M to TXA.
        prog = _wrap([
            asm_ast.Mov(src=_M("p"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("b")),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=_M("b"), dst=_X),            # loop-top reload
            asm_ast.Mov(src=_M("b"), dst=_A),            # LDA M — was disqualifying
            asm_ast.Mov(src=_A, dst=_M("scratch")),
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        # Fires: the LDA M is rewritten to TXA, the loop-top LDX is
        # dropped, init gets a TAX, Dec(M) → Dec(X).
        # Look for TXA (Mov(Reg(X), Reg(A))) in the body.
        self.assertTrue(any(isinstance(i, asm_ast.Mov)
                            and i.src == _X and i.dst == _A for i in out))
        self.assertTrue(any(isinstance(i, asm_ast.Dec)
                            and isinstance(i.dst, asm_ast.Reg)
                            and isinstance(i.dst.reg, asm_ast.X) for i in out))

    def test_save_restore_around_call(self):
        # A counter loop containing a Call gets STX/LDX wrapping
        # around each Call — the callee clobbers X but the counter
        # is preserved via the save/restore.
        prog = _wrap([
            asm_ast.Mov(src=_M("p"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("b")),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Mov(src=asm_ast.IndexedData(name="arr", offset=0, index=asm_ast.X()),
                        dst=_A),
            asm_ast.Call(name="helper"),
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        # Find the Call and confirm it's preceded by STX M and
        # followed by LDX M.
        call_idx = next(
            k for k, i in enumerate(out) if isinstance(i, asm_ast.Call)
        )
        self.assertEqual(out[call_idx - 1],
                         asm_ast.Mov(src=_X, dst=_M("b")))
        self.assertEqual(out[call_idx + 1],
                         asm_ast.Mov(src=_M("b"), dst=_X))

    def test_pivot_blocked_by_indirect_y_access(self):
        # If a non-counter `LDX <other>` opens what would otherwise
        # be a pivot range, but the range contains an Indirect-Y
        # operand (which reads Y for its addressing mode), the
        # pivot is unsound — rewriting LDX→LDY would clobber the
        # value Y carries for the indirect access. The pass must
        # reject the promotion.
        prog = _wrap([
            asm_ast.Mov(src=_M("p"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("b")),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Mov(src=_M("temp"), dst=_X),     # non-counter LDX — pivot start
            asm_ast.Mov(src=asm_ast.IndirectZpY(address=0x10), dst=_A),  # uses Y!
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Branch(cond=asm_ast.PL(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ])
        out = _instrs(apply_loop_counter_to_x(prog))
        # Unchanged — pivot couldn't form.
        self.assertEqual(len(out), 9)
        self.assertEqual(out, prog.top_level[0].instructions)

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
