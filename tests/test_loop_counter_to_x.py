"""Tests for the loop-counter-to-X promotion pass."""

import shutil
import unittest

import asm_ast
from passes.loop_counter_to_x import apply_loop_counter_to_x
from sim.harness import build_sim


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


def _loop(body, after=None):
    """Builds `LDA p; STA b; .top: LDX b; LDA arr,X; <body>; DEC b;
    BPL .top; <after>`, with an `RTS` when `after` is None."""
    if after is None:
        after = [asm_ast.Return(save_a=False)]
    return [
        asm_ast.Mov(src=_M("p"), dst=_A),
        asm_ast.Mov(src=_A, dst=_M("b")),
        asm_ast.Label(name=".top"),
        asm_ast.Mov(src=_M("b"), dst=_X),
        asm_ast.Mov(src=_IX("arr"), dst=_A),
        *body,
        asm_ast.Dec(dst=_M("b")),
        asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
        *after,
    ]


def _IX(name):
    return asm_ast.IndexedData(name=name, offset=0, index=asm_ast.X())


def _IY(name):
    return asm_ast.IndexedData(name=name, offset=0, index=asm_ast.Y())


_Y = asm_ast.Reg(reg=asm_ast.Y())


class TestPivotRangeBounds(unittest.TestCase):
    """Each of these shapes was miscompiled by the Y-pivot or the init
    rewrite before the range checks were tightened."""

    def test_pivot_range_stops_before_counter_step(self):
        # `LDX k; LDA t,X; STA s2` runs straight into `DEC b`. The
        # range used to swallow the DEC, which then stayed a DEC of
        # memory while the loop-top `LDX b` was dropped, so X never
        # changed and the loop never ended.
        instrs = _loop([
            asm_ast.Mov(src=_M("k"), dst=_X),
            asm_ast.Mov(src=_IX("t"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("s2")),
        ])
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertIn(asm_ast.Dec(dst=_X), out)
        self.assertNotIn(asm_ast.Dec(dst=_M("b")), out)
        self.assertIn(asm_ast.Mov(src=_IY("t"), dst=_A), out)

    def test_ldx_after_loop_is_not_pivoted(self):
        # `return s;` for a 2-byte `s` ends in `LDA s; LDX s+1; RTS`
        # with the high byte in X. The counter is dead there, so the
        # LDX stays. It used to become `LDY s+1`.
        instrs = _loop([asm_ast.Mov(src=_A, dst=_M("s"))], after=[
            asm_ast.Mov(src=_M("s"), dst=_A),
            asm_ast.Mov(src=asm_ast.Data(name="s", offset=1), dst=_X),
            asm_ast.Return(save_a=True, save_x=True),
        ])
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertIn(asm_ast.Dec(dst=_X), out)
        self.assertEqual(
            out[-2], asm_ast.Mov(src=asm_ast.Data(name="s", offset=1), dst=_X),
        )

    def test_cmp_abs_x_in_pivot_range_is_rewritten(self):
        # `CMP u,X` reads the pivoted value, so it has to become
        # `CMP u,Y` along with the LDA. It used to keep reading X,
        # which by then held the counter.
        instrs = _loop([
            asm_ast.Mov(src=_M("k"), dst=_X),
            asm_ast.Mov(src=_IX("t"), dst=_A),
            asm_ast.Compare(left=_A, right=_IX("u")),
            asm_ast.Mov(src=_A, dst=_M("s")),
        ])
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertIn(asm_ast.Compare(left=_A, right=_IY("u")), out)
        self.assertNotIn(asm_ast.Compare(left=_A, right=_IX("u")), out)

    def test_y_read_in_pivot_range_blocks(self):
        # `ADC u,Y` reads the Y the code had before the range. The
        # pivot's `LDY k` would replace it.
        instrs = _loop([
            asm_ast.Mov(src=_M("k"), dst=_X),
            asm_ast.Mov(src=_IX("t"), dst=_A),
            asm_ast.Add(src=_IY("u"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("s")),
        ])
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_frame_access_in_pivot_range_blocks(self):
        # A soft-stack access is lowered as `LDY #off; LDA (FP),Y`,
        # which overwrites the pivoted value in Y.
        instrs = _loop([
            asm_ast.Mov(src=_M("k"), dst=_X),
            asm_ast.Mov(src=asm_ast.Frame(offset=3), dst=_A),
            asm_ast.Mov(src=_IX("t"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("s")),
        ])
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_x_read_after_label_blocks(self):
        # The range ends at `.mid`, but `LDA u,X` after it still reads
        # the value of `LDX k`. Pivoting would hand it the counter.
        instrs = _loop([
            asm_ast.Mov(src=_M("k"), dst=_X),
            asm_ast.Mov(src=_IX("t"), dst=_A),
            asm_ast.Label(name=".mid"),
            asm_ast.Mov(src=_IX("u"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("s")),
        ])
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_stx_init_gets_no_tax(self):
        # The init `STX b` leaves the initial value in X already. A
        # TAX after it used to load X with whatever A held.
        instrs = [
            asm_ast.Mov(src=_X, dst=_M("b")),
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Mov(src=_IX("arr"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("s")),
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertIn(asm_ast.Dec(dst=_X), out)
        self.assertNotIn(asm_ast.Mov(src=_A, dst=_X), out)

    def test_sty_init_reloads_x_from_slot(self):
        # After `STY b` the value is in Y and b, not in A.
        instrs = [
            asm_ast.Mov(src=_Y, dst=_M("b")),
            asm_ast.Label(name=".top"),
            asm_ast.Mov(src=_M("b"), dst=_X),
            asm_ast.Mov(src=_IX("arr"), dst=_A),
            asm_ast.Mov(src=_A, dst=_M("s")),
            asm_ast.Dec(dst=_M("b")),
            asm_ast.Branch(cond=asm_ast.PL(), target=".top"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_loop_counter_to_x(_wrap(instrs)))
        self.assertEqual(out[1], asm_ast.Mov(src=_M("b"), dst=_X))
        self.assertNotIn(asm_ast.Mov(src=_A, dst=_X), out)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestLoopCounterToXSim(unittest.TestCase):

    def test_pivot_after_call_keeps_loop_finite(self):
        # After `JSR g`, `out2 = spr_w[k]` loads `LDX k` and runs on
        # into `DEC i`. The optimized build used to loop forever.
        src = r'''
        #include <stdint.h>
        uint8_t ent_y[8];
        uint8_t spr_w[8];
        volatile uint8_t out1, out2;
        uint8_t calls;
        void g(void) { calls++; }
        void f(uint8_t n, uint8_t k)
        {
            uint8_t i = n;
            do {
                out1 = ent_y[i];
                g();
                out2 = spr_w[k];
                i--;
            } while ((i & 0x80) == 0);
        }
        int main(void) { f(3, 1); return calls; }
        '''
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(src, optimize=optimize).run()
                self.assertFalse(r.timed_out)
                self.assertEqual(r.return_int(), 4)


if __name__ == "__main__":
    unittest.main()
