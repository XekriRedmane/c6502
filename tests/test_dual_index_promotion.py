"""Tests for the X→Y dual-index promotion pass."""

import shutil
import unittest

import asm_ast
from passes.dual_index_promotion import apply_dual_index_promotion
from sim.harness import build_sim


_A = asm_ast.Reg(reg=asm_ast.A())
_X = asm_ast.Reg(reg=asm_ast.X())
_Y = asm_ast.Reg(reg=asm_ast.Y())


def _wrap(instrs, name="f"):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name=name, is_global=False, params=[], instructions=instrs,
    )])


def _instrs(prog):
    return prog.top_level[0].instructions


def _data(name, off=0):
    return asm_ast.Data(name=name, offset=off)


def _ixX(name, off=0):
    return asm_ast.IndexedData(name=name, offset=off, index=asm_ast.X())


def _ixY(name, off=0):
    return asm_ast.IndexedData(name=name, offset=off, index=asm_ast.Y())


def _ldx(src):
    return asm_ast.Mov(src=src, dst=_X, is_volatile=False)


def _ldy(src):
    return asm_ast.Mov(src=src, dst=_Y, is_volatile=False)


def _lda(src):
    return asm_ast.Mov(src=src, dst=_A, is_volatile=False)


def _sta(dst):
    return asm_ast.Mov(src=_A, dst=dst, is_volatile=False)


class TestDualIndexPromotionApplyBobble(unittest.TestCase):
    """Reproduces the apply_bobble shape: two LDX of __zpabi_p0
    reloads (one per branch), interleaved with an LDX of a
    different param at function entry."""

    def test_apply_bobble_pattern_promotes(self):
        # Synthetic apply_bobble-like body. The two LDX of "slot"
        # at branches collapse into one LDY at entry.
        instrs = [
            asm_ast.Label(name=".entry"),
            _ldx(_data("__zpabi_f_p1")),     # X = bobble_idx
            _lda(_ixX("rescue_bobble")),     # LDA rescue_bobble,X
            asm_ast.Branch(cond=asm_ast.PL(), target=".else"),
            # Fall-through (add path).
            asm_ast.And(src=asm_ast.Imm(value=0x7F), dst=_A),
            _ldx(_data("__zpabi_f_p0")),     # X = slot (reload #1)
            asm_ast.ClearCarry(),
            asm_ast.Add(src=_ixX("entity"), dst=_A),
            _sta(_ixX("entity")),
            asm_ast.Jump(target=".end"),
            asm_ast.Label(name=".else"),
            asm_ast.And(src=asm_ast.Imm(value=0x7F), dst=_A),
            _ldx(_data("__zpabi_f_p0")),     # X = slot (reload #2)
            _lda(_ixX("entity")),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=_data("temp"), dst=_A),
            _sta(_ixX("entity")),
            asm_ast.Label(name=".end"),
            asm_ast.Return(save_a=False),
        ]
        prog = _wrap(instrs)
        out = _instrs(apply_dual_index_promotion(prog))
        # First post-Label instr should now be LDY __zpabi_f_p0.
        self.assertIsInstance(out[0], asm_ast.Label)
        self.assertEqual(out[1], _ldy(_data("__zpabi_f_p0")))
        # No more LDX of __zpabi_f_p0 in the body.
        for instr in out:
            if isinstance(instr, asm_ast.Mov) and isinstance(
                instr.dst, asm_ast.Reg,
            ) and isinstance(instr.dst.reg, asm_ast.X):
                src = instr.src
                self.assertFalse(
                    isinstance(src, asm_ast.Data)
                    and src.name == "__zpabi_f_p0",
                    "an LDX __zpabi_f_p0 survived",
                )
        # The entity_floor_pos accesses (modeled as `entity` here)
        # are now indexed by Y, not X.
        entity_accesses = [
            i for i in out
            if (
                isinstance(i, asm_ast.Mov)
                and (
                    (isinstance(i.src, asm_ast.IndexedData)
                     and i.src.name == "entity")
                    or (isinstance(i.dst, asm_ast.IndexedData)
                        and i.dst.name == "entity")
                )
            ) or (
                isinstance(i, (asm_ast.Add, asm_ast.Sub))
                and isinstance(i.src, asm_ast.IndexedData)
                and i.src.name == "entity"
            )
        ]
        self.assertGreater(len(entity_accesses), 0)
        for instr in entity_accesses:
            for op in (
                getattr(instr, "src", None),
                getattr(instr, "dst", None),
            ):
                if (
                    isinstance(op, asm_ast.IndexedData)
                    and op.name == "entity"
                ):
                    self.assertIsInstance(
                        op.index, asm_ast.Y,
                        f"entity access still using X: {instr}",
                    )
        # The rescue_bobble lookup (single LDX use, indexes by X)
        # is untouched.
        rescue = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.src, asm_ast.IndexedData)
            and i.src.name == "rescue_bobble"
        ]
        self.assertEqual(len(rescue), 1)
        self.assertIsInstance(rescue[0].src.index, asm_ast.X)


class TestDualIndexPromotionEligibility(unittest.TestCase):
    """Cases that must NOT fire — eligibility-gate verification."""

    def test_single_ldx_use_no_promote(self):
        # Only one LDX of __zpabi_f_p0; promotion would just shift
        # the load to LDY, not save anything.
        instrs = [
            asm_ast.Label(name=".entry"),
            _ldx(_data("__zpabi_f_p1")),
            _ldx(_data("__zpabi_f_p0")),     # only one
            _lda(_ixX("entity")),
            _sta(_ixX("entity")),
            asm_ast.Return(save_a=False),
        ]
        prog = _wrap(instrs)
        out = _instrs(apply_dual_index_promotion(prog))
        # No change.
        self.assertEqual(out, instrs)

    def test_y_already_used_no_promote(self):
        # Y is used as an indirect-Y base — adding our own LDY
        # would clobber that.
        instrs = [
            asm_ast.Label(name=".entry"),
            _ldy(asm_ast.Imm(value=0)),       # Y used here
            _ldx(_data("__zpabi_f_p0")),
            _lda(_ixX("entity")),
            _sta(_ixX("entity")),
            _ldx(_data("__zpabi_f_p0")),
            _lda(_ixX("entity")),
            _sta(_ixX("entity")),
            asm_ast.Return(save_a=False),
        ]
        prog = _wrap(instrs)
        out = _instrs(apply_dual_index_promotion(prog))
        self.assertEqual(out, instrs)

    def test_no_other_x_user_no_promote(self):
        # Only __zpabi_f_p0 is ever loaded into X. Y-promotion
        # has no win — X could just hold p0 throughout.
        instrs = [
            asm_ast.Label(name=".entry"),
            _ldx(_data("__zpabi_f_p0")),
            _lda(_ixX("entity")),
            _ldx(_data("__zpabi_f_p0")),
            _sta(_ixX("entity")),
            asm_ast.Return(save_a=False),
        ]
        prog = _wrap(instrs)
        out = _instrs(apply_dual_index_promotion(prog))
        self.assertEqual(out, instrs)

    def test_inc_with_indexed_x_blocks(self):
        # The 6502 has no `INC abs,Y`. If we'd rewrite a
        # `Inc(IndexedData(_, _, X))` to ,Y, the result wouldn't
        # assemble. Gate rejects.
        instrs = [
            asm_ast.Label(name=".entry"),
            _ldx(_data("__zpabi_f_p1")),
            _lda(_ixX("rescue")),            # X has another user
            _ldx(_data("__zpabi_f_p0")),     # promotion candidate #1
            asm_ast.Inc(dst=_ixX("entity")),  # blocks rewrite
            _ldx(_data("__zpabi_f_p0")),     # promotion candidate #2
            _lda(_ixX("entity")),
            asm_ast.Return(save_a=False),
        ]
        prog = _wrap(instrs)
        out = _instrs(apply_dual_index_promotion(prog))
        self.assertEqual(out, instrs)

    def _promotable_around(self, middle):
        """The promotable shape with `middle` between the two uses."""
        return [
            asm_ast.Label(name=".entry"),
            _ldx(_data("__zpabi_f_p1")),
            _lda(_ixX("rescue")),
            _ldx(_data("__local_f__1")),
            _lda(_ixX("entity")),
        ] + middle + [
            _ldx(_data("__local_f__1")),
            _sta(_ixX("entity")),
            asm_ast.Return(save_a=False),
        ]

    def test_promotable_shape_promotes(self):
        # The control for the two cases below.
        instrs = self._promotable_around([])
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertNotEqual(out, instrs)

    def test_written_symbol_blocks(self):
        # A loop temporary stored and reloaded into X: Y, loaded once
        # at entry, would hold the value from before the store.
        instrs = self._promotable_around([_sta(_data("__local_f__1"))])
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_call_blocks(self):
        # A callee may change Y before the second use.
        instrs = self._promotable_around(
            [asm_ast.Call(name="g", reg_args=[], preserves=[])],
        )
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertEqual(out, instrs)


class TestDualIndexPromotionRewriteRange(unittest.TestCase):
    """Verify the rewrite respects X-clobber and block boundaries."""

    def test_rewrite_stops_at_x_reload(self):
        # After LDX(promoted), the IndexedData,X access reads
        # promoted's value (rewrite OK). After a LDX(other),
        # subsequent IndexedData,X reads OTHER's value, NOT
        # promoted — rewrite must NOT touch those.
        instrs = [
            asm_ast.Label(name=".entry"),
            _ldx(_data("OTHER1")),           # other X user
            _lda(_ixX("any")),               # uses OTHER1
            _ldx(_data("__zpabi_f_p0")),     # candidate, #1
            _lda(_ixX("entity")),            # uses p0 → ,Y
            _ldx(_data("OTHER2")),           # X reload — promote ends
            _lda(_ixX("other_arr")),         # uses OTHER2, NOT touched
            _ldx(_data("__zpabi_f_p0")),     # candidate, #2
            _sta(_ixX("entity")),            # uses p0 → ,Y
            asm_ast.Return(save_a=False),
        ]
        prog = _wrap(instrs)
        out = _instrs(apply_dual_index_promotion(prog))
        # `other_arr` access stays IndexedData,X.
        other_arr_uses = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.src, asm_ast.IndexedData)
            and i.src.name == "other_arr"
        ]
        self.assertEqual(len(other_arr_uses), 1)
        self.assertIsInstance(other_arr_uses[0].src.index, asm_ast.X)
        # `entity` accesses both rewritten to ,Y.
        entity_accesses = []
        for i in out:
            if isinstance(i, asm_ast.Mov):
                for op in (i.src, i.dst):
                    if (
                        isinstance(op, asm_ast.IndexedData)
                        and op.name == "entity"
                    ):
                        entity_accesses.append(op)
        self.assertEqual(len(entity_accesses), 2)
        for op in entity_accesses:
            self.assertIsInstance(op.index, asm_ast.Y)

    def _branch_shape(self, after_label):
        """`o = t[k]; a = u[d]; if (a) v[d] = a; o = t[k];` followed
        by `after_label` at the join, with d the promoted symbol."""
        return [
            _ldx(_data("k")),
            _lda(_ixX("t")),
            _sta(_data("o")),
            _ldx(_data("d")),
            _lda(_ixX("u")),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".end"),
            _sta(_ixX("v")),
            asm_ast.Label(name=".end"),
            *after_label,
            asm_ast.Return(save_a=False),
        ]

    def test_read_after_branch_follows_x(self):
        # `STA v,X` comes after the `BEQ`, and X still holds d there.
        # The rewrite used to stop at the branch and leave `v,X`
        # reading X, which no longer held d once the `LDX d` was
        # dropped.
        instrs = self._branch_shape([
            _ldx(_data("k")),
            _lda(_ixX("t")),
            _sta(_data("o")),
            _ldx(_data("d")),
            _lda(_ixX("w")),
            _sta(_data("o2")),
        ])
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertIn(_sta(_ixY("v")), out)
        self.assertIn(_lda(_ixY("u")), out)
        self.assertIn(_lda(_ixY("w")), out)
        self.assertEqual(
            [i for i in out if isinstance(i, asm_ast.Mov)
             and isinstance(i.src, asm_ast.IndexedData)
             and i.src.name == "t"],
            [_lda(_ixX("t")), _lda(_ixX("t"))],
        )

    def test_read_where_x_may_or_may_not_hold_d_blocks(self):
        # At `.end`, X holds d on the taken path and k on the
        # fall-through path, so `LDA w,X` has no single register to
        # read.
        instrs = [
            _ldx(_data("d")),
            _lda(_ixX("u")),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".end"),
            _ldx(_data("k")),
            _lda(_ixX("t")),
            asm_ast.Label(name=".end"),
            _lda(_ixX("w")),
            _ldx(_data("d")),
            _sta(_ixX("v")),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_x_value_read_blocks(self):
        # `TXA` reads d's value from X, and X won't hold it.
        instrs = self._branch_shape([
            _ldx(_data("d")),
            asm_ast.Mov(src=_X, dst=_A, is_volatile=False),
            _sta(_data("o2")),
        ])
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_live_flags_after_ldx_block(self):
        # The `BEQ` reads the Z flag of `LDX d`, which is dropped.
        instrs = [
            _ldx(_data("k")),
            _lda(_ixX("t")),
            _ldx(_data("d")),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".end"),
            _lda(_ixX("u")),
            asm_ast.Label(name=".end"),
            _ldx(_data("d")),
            _sta(_ixX("v")),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertEqual(out, instrs)


class TestDualIndexPromotionYUses(unittest.TestCase):
    """Operands whose lowering writes or reads Y."""

    def _with(self, middle):
        return [
            _ldx(_data("k")),
            _lda(_ixX("t")),
            _ldx(_data("d")),
            _lda(_ixX("u")),
            _ldx(_data("k")),
            _lda(_ixX("t")),
            *middle,
            _ldx(_data("d")),
            _sta(_ixX("w")),
            asm_ast.Return(save_a=False),
        ]

    def test_control_promotes(self):
        instrs = self._with([])
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertNotEqual(out, instrs)

    def test_indirect_zp_blocks(self):
        # `LDA (p),Y` on a zero-page pointer is emitted as `LDY #$00;
        # LDA (p),Y`, which overwrites d in Y. It wasn't counted as a
        # use of Y.
        instrs = self._with([_lda(asm_ast.IndirectZp(address=0x80, offset=0))])
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_indirect_zp_y_blocks(self):
        instrs = self._with([_lda(asm_ast.IndirectZpY(address=0x80))])
        out = _instrs(apply_dual_index_promotion(_wrap(instrs)))
        self.assertEqual(out, instrs)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not installed")
class TestDualIndexPromotionSim(unittest.TestCase):

    def _check(self, src, expected):
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                r = build_sim(src, optimize=optimize).run()
                self.assertFalse(r.timed_out)
                self.assertEqual(r.return_int(), expected)

    def test_store_after_branch(self):
        # `v[d] = a` sits after `if (a != 0)`. The optimized build
        # used to store to `v[k]`.
        self._check(r'''
        #include <stdint.h>
        uint8_t t[8], u[8] = {0,0,0,0,0,7,0,0}, v[8], w[8];
        uint8_t o, o2;
        void f(uint8_t k, uint8_t d)
        {
            o = t[k];
            uint8_t a = u[d];
            if (a != 0)
                v[d] = a;
            o = t[k];
            o2 = w[d];
        }
        int main(void) { f(2, 5); return v[5] * 10 + v[2]; }
        ''', 70)

    def test_pointer_read_between_uses(self):
        # `*p` through a zero-page pointer loads Y with 0, so the
        # optimized build used to read `w[0]` where it should read
        # `w[d]`.
        self._check(r'''
        #include <stdint.h>
        uint8_t t[8], u[8], w[8] = {0,0,0,0,0,9,0,0};
        uint8_t o, o2, o3, cell;
        void f(uint8_t k, uint8_t d, uint8_t *p)
        {
            o = t[k];
            o2 = u[d];
            o = t[k];
            o3 = *p;
            o2 = w[d];
        }
        int main(void) { f(2, 5, &cell); return o2; }
        ''', 9)


if __name__ == "__main__":
    unittest.main()
