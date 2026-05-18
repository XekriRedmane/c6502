"""Unit tests for `passes.memory_value_propagation.apply_memory_value_propagation`.

Milestone 1 coverage: the CFG-aware DPTR-stage rewrite. Verifies
that an indirect-via-DPTR access whose DPTR-stage is in a
predecessor block (across one or more `Label` boundaries, plus
loop back-edges) gets rewritten to use the source ZP pair directly.
"""

import unittest

import asm_ast
from passes.memory_value_propagation import (
    apply_memory_value_propagation,
)


_DPTR = 0x24
_P0 = 0x80
_P1 = 0x81


def _A():
    return asm_ast.Reg(reg=asm_ast.A())


def _X():
    return asm_ast.Reg(reg=asm_ast.X())


def _Y():
    return asm_ast.Reg(reg=asm_ast.Y())


def _zp(addr, off=0):
    return asm_ast.ZP(address=addr, offset=off)


def _dptr_byte(off):
    return asm_ast.Data(name="DPTR", offset=off)


def _stage_dptr_from_zp_pair(p_lo, p_hi):
    """Build the 4-instruction DPTR-staging sequence:
    LDA p_lo; STA DPTR; LDA p_hi; STA DPTR+1."""
    return [
        asm_ast.Mov(src=_zp(p_lo), dst=_A(), is_volatile=False),
        asm_ast.Mov(src=_A(), dst=_dptr_byte(0), is_volatile=False),
        asm_ast.Mov(src=_zp(p_hi), dst=_A(), is_volatile=False),
        asm_ast.Mov(src=_A(), dst=_dptr_byte(1), is_volatile=False),
    ]


def _wrap(instrs):
    fn = asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )
    prog = asm_ast.Program(top_level=[fn])
    return apply_memory_value_propagation(prog)


def _instrs(prog):
    return prog.top_level[0].instructions


class TestSameBlockRewrite(unittest.TestCase):
    """Sanity: when the stage and the use sit in the same block,
    the rewrite happens (mirrors apply_indirect_base_prop's
    coverage)."""

    def test_indirecty_in_same_block_rewrites(self):
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=_zp(0x10), dst=_A(), is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # The final Mov's dst should be IndirectZpY(_P0).
        self.assertIsInstance(out[-1].dst, asm_ast.IndirectZpY)
        self.assertEqual(out[-1].dst.address, _P0)


class TestCrossBlockRewrite(unittest.TestCase):
    """The headline case: stage in preheader, use after a label
    boundary (loop entry)."""

    def test_use_after_label_rewrites(self):
        # Stage DPTR in preheader, use across a label boundary.
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(
                src=_zp(0x10), dst=_A(), is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # Find the IndirectY-shaped Mov post-label.
        last = next(
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, (asm_ast.IndirectY, asm_ast.IndirectZpY))
        )
        self.assertIsInstance(last.dst, asm_ast.IndirectZpY)
        self.assertEqual(last.dst.address, _P0)

    def test_use_on_loop_exit_path_rewrites(self):
        # find_active_entity's shape: stage DPTR in preheader, loop
        # scans for a match, the indirect-Y write happens on the
        # exit path (not the back-edge). The fact survives the loop
        # because the back-edge doesn't include the indirect write.
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=_zp(0x20), dst=_X(), is_volatile=False,
            ),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Compare(left=_X(), right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.MI(), target=".loop_end"),
            # "Match found" path branches out to the indirect-write.
            asm_ast.Mov(
                src=asm_ast.IndexedData(
                    name="arr", offset=0, index=asm_ast.X(),
                ),
                dst=_A(), is_volatile=False,
            ),
            asm_ast.Compare(left=_A(), right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.NE(), target=".write_and_exit"),
            # No-match path: DEX, JMP back.
            asm_ast.Dec(dst=_X()),
            asm_ast.Jump(target=".loop_start"),
            asm_ast.Label(name=".write_and_exit"),
            asm_ast.Mov(
                src=_zp(0x21), dst=_A(), is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
            asm_ast.Label(name=".loop_end"),
            asm_ast.Return(save_a=False),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # The indirect-Y on the exit path should be rewritten.
        indirect_uses = [
            i for i in out
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, (asm_ast.IndirectY, asm_ast.IndirectZpY))
        ]
        self.assertEqual(len(indirect_uses), 1)
        self.assertIsInstance(indirect_uses[0].dst, asm_ast.IndirectZpY)
        self.assertEqual(indirect_uses[0].dst.address, _P0)


class TestInvalidation(unittest.TestCase):
    """Writes that invalidate the equivalence must prevent the
    rewrite."""

    def test_write_to_dptr_kills_equivalence(self):
        # Stage, then overwrite DPTR low byte, then use — should NOT
        # rewrite (DPTR no longer matches the staged pair).
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=asm_ast.Imm(value=0x99), dst=_A(),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=_dptr_byte(0),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        last = out[-1]
        # DPTR low byte was overwritten — should still be IndirectY,
        # NOT IndirectZpY.
        self.assertIsInstance(last.dst, asm_ast.IndirectY)

    def test_write_to_source_kills_equivalence(self):
        # Stage, then overwrite the SOURCE pair's byte, then use —
        # should NOT rewrite (DPTR still has the OLD source value,
        # not the source's current value).
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Mov(
                src=asm_ast.Imm(value=0x77), dst=_A(),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=_zp(_P0),
                is_volatile=False,
            ),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        last = out[-1]
        self.assertIsInstance(last.dst, asm_ast.IndirectY)

    def test_call_invalidates(self):
        # Stage, then Call, then use — Call clobbers everything,
        # so should NOT rewrite.
        instrs = _stage_dptr_from_zp_pair(_P0, _P1) + [
            asm_ast.Call(name="callee"),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        last = out[-1]
        self.assertIsInstance(last.dst, asm_ast.IndirectY)


class TestMeetAcrossBranches(unittest.TestCase):
    """At a join, only facts agreed by every predecessor survive."""

    def test_one_predecessor_doesnt_stage_kills_fact(self):
        # Block A stages DPTR from (P0, P1). Block B (other
        # predecessor of the join) does NOT stage. At the join,
        # DPTR is not known to equal anything. The use after the
        # join should NOT rewrite.
        instrs = [
            # Entry: branch on something.
            asm_ast.Mov(
                src=_zp(0x10), dst=_A(), is_volatile=False,
            ),
            asm_ast.Compare(left=_A(), right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".other"),
            # Fall-through: stage DPTR.
            *_stage_dptr_from_zp_pair(_P0, _P1),
            asm_ast.Jump(target=".join"),
            asm_ast.Label(name=".other"),
            # Branch arm: don't stage; just clobber A.
            asm_ast.Mov(
                src=asm_ast.Imm(value=0x55), dst=_A(),
                is_volatile=False,
            ),
            asm_ast.Label(name=".join"),
            asm_ast.Mov(
                src=_A(), dst=asm_ast.IndirectY(),
                is_volatile=False,
            ),
            asm_ast.Return(save_a=False),
        ]
        result = _wrap(instrs)
        out = _instrs(result)
        # Find the IndirectY use after the .join label.
        join_idx = next(
            i for i, ins in enumerate(out)
            if isinstance(ins, asm_ast.Label) and ins.name == ".join"
        )
        post_join = out[join_idx:]
        indirect_uses = [
            i for i in post_join
            if isinstance(i, asm_ast.Mov)
            and isinstance(i.dst, (asm_ast.IndirectY, asm_ast.IndirectZpY))
        ]
        self.assertEqual(len(indirect_uses), 1)
        self.assertIsInstance(indirect_uses[0].dst, asm_ast.IndirectY,
                              "meet should drop the fact when one "
                              "predecessor doesn't have it")


if __name__ == "__main__":
    unittest.main()
