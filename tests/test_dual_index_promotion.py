"""Tests for the X→Y dual-index promotion pass."""

import unittest

import asm_ast
from passes.dual_index_promotion import apply_dual_index_promotion


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


if __name__ == "__main__":
    unittest.main()
