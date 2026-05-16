"""Tests for the dead-PHA/PLA peephole.

`apply_dead_pha_pla` drops a `Push(Reg(A))` / `Pop(Reg(A))` pair when
the intervening body preserves A and the N/Z flags from the original
PLA are dead. The canonical case is the
`tac_to_asm._translate_indirect_indexed_store` lowering after the
`direct_index_load` peephole has fused its `LDA idx; TAY` into a
bare `LDY idx`.
"""
from __future__ import annotations

import unittest

import asm_ast
from passes.dead_pha_pla import apply_dead_pha_pla


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())
_REG_Y = asm_ast.Reg(reg=asm_ast.Y())


def _fn(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[], instructions=instrs,
    )


def _prog(instrs: list[asm_ast.Type_instruction]) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(instrs)])


def _rewrite(instrs):
    return apply_dead_pha_pla(_prog(instrs)).top_level[0].instructions


class TestDeadPhaPla(unittest.TestCase):
    def test_drops_pair_around_ldy_data(self) -> None:
        # The post-fusion shape from indirect-indexed store.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Mov(
                src=asm_ast.Data(name="zpabi_p0", offset=0), dst=_REG_Y,
            ),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Mov(
                src=_REG_A, dst=asm_ast.IndirectY(),
            ),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(
            out,
            [
                asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
                asm_ast.Mov(
                    src=asm_ast.Data(name="zpabi_p0", offset=0), dst=_REG_Y,
                ),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectY()),
                asm_ast.Return(save_a=False),
            ],
        )

    def test_drops_pair_around_ldx_zp(self) -> None:
        # LDX from ZP — same shape, different register.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x42), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Mov(src=asm_ast.ZP(address=0x80, offset=0), dst=_REG_X),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectY()),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(len(out), len(instrs) - 2)
        self.assertNotIn(asm_ast.Push(src=_REG_A), out)
        self.assertNotIn(asm_ast.Pop(dst=_REG_A), out)

    def test_body_reads_a_does_not_fold(self) -> None:
        # The body's STA reads A — dropping PHA/PLA would still be
        # semantically equivalent (A is unchanged), but the
        # conservative gate refuses. This pins the behavior so a
        # later relaxation is a deliberate change.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Mov(
                src=_REG_A, dst=asm_ast.ZP(address=0x90, offset=0),
            ),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_body_writes_a_does_not_fold(self) -> None:
        # The body kills A; dropping PHA/PLA would leak the body's
        # write past the original PLA position.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Mov(src=asm_ast.Imm(value=0x99), dst=_REG_A),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_body_with_call_does_not_fold(self) -> None:
        # Call clobbers A (and the stack-balance reasoning is fragile).
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Call(name="some_helper"),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_body_with_branch_does_not_fold(self) -> None:
        # Branch makes the body's straight-line assumption invalid.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Branch(
                cond=asm_ast.EQ(), target=".elsewhere",
            ),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_body_with_label_does_not_fold(self) -> None:
        # Another predecessor could jump in mid-body.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Label(name=".midbody"),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_nested_push_pop_inner_drops_outer_remains(self) -> None:
        # An inner push before the matching outer pop — the outer
        # pair bails (the inner Push trips the no-nested-Push rule),
        # but the scanner advances past it and matches the inner
        # pair. The fixed-point loop's next iteration sees the outer
        # pair with no body left and collapses it. This single-pass
        # test pins the per-pass behavior.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),         # outer
            asm_ast.Push(src=_REG_A),         # inner
            asm_ast.Pop(dst=_REG_A),          # inner
            asm_ast.Pop(dst=_REG_A),          # outer
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        # Inner pair dropped; outer pair survives the single pass.
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Return(save_a=False),
        ])
        # Second pass collapses the outer pair too.
        out2 = apply_dead_pha_pla(_prog(out)).top_level[0].instructions
        self.assertEqual(out2, [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Return(save_a=False),
        ])

    def test_flag_live_after_does_not_fold(self) -> None:
        # The PLA's flag effect is observed by a Branch — can't drop
        # without changing branch behavior.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_REG_Y),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".somewhere"),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(out, instrs)

    def test_repeated_pairs_in_one_function(self) -> None:
        # Two independent, both droppable.
        instrs = [
            asm_ast.Mov(src=asm_ast.Imm(value=0x01), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Mov(src=asm_ast.Data(name="a", offset=0), dst=_REG_Y),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectY()),
            asm_ast.Mov(src=asm_ast.Imm(value=0x02), dst=_REG_A),
            asm_ast.Push(src=_REG_A),
            asm_ast.Mov(src=asm_ast.Data(name="b", offset=0), dst=_REG_Y),
            asm_ast.Pop(dst=_REG_A),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.IndirectY()),
            asm_ast.Return(save_a=False),
        ]
        out = _rewrite(instrs)
        self.assertEqual(len(out), len(instrs) - 4)
        # No Push or Pop should survive.
        self.assertFalse(any(
            isinstance(ins, (asm_ast.Push, asm_ast.Pop)) for ins in out
        ))
