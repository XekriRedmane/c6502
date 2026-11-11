"""Tests for the AND #$80 + BEQ/BNE → BPL/BMI peephole."""

import unittest

import asm_ast
from passes.and_sign_bit_branch import apply_and_sign_bit_branch


_A = asm_ast.Reg(reg=asm_ast.A())


# The length of the tail the last `_wrap` added.
_TAIL = [0]


def _instrs(prog):
    """The function's instructions without the tail `_wrap` adds."""
    out = prog.top_level[0].instructions
    return out[:len(out) - _TAIL[0]]


def _wrap(instrs):
    """`instrs` as a function, followed by a label and a return for
    each branch target that isn't already a label in it. The passes
    check A at a branch's target, and a target that isn't in the
    function counts as live."""
    labels = {i.name for i in instrs if isinstance(i, asm_ast.Label)}
    tail = []
    for i in instrs:
        if isinstance(i, asm_ast.Branch) and i.target not in labels:
            labels.add(i.target)
            tail += [asm_ast.Label(name=i.target), asm_ast.Return(save_a=False)]
    fn = asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs + tail,
    )
    _TAIL[0] = len(tail)
    return asm_ast.Program(top_level=[fn])


def _ret():
    """Function-exit atom — used to terminate the function so the
    A-liveness walk has somewhere to bottom out. `save_a=False`
    means A is dead at exit."""
    return asm_ast.Return(save_a=False)


class TestAndSignBitBranch(unittest.TestCase):

    def test_lda_and_80_beq_folds_to_lda_bpl(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(out, [
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.Branch(cond=asm_ast.PL(), target=".target"),
            _ret(),
        ])

    def test_lda_and_80_bne_folds_to_lda_bmi(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(out[1], asm_ast.Branch(cond=asm_ast.MI(), target=".target"))

    def test_does_not_fire_when_a_live_after_branch(self):
        # If A is read after the Branch (e.g., used by an STA), the
        # AND's value matters — don't drop.
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".target"),
            # A is live here:
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="y", offset=0)),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        # Unchanged.
        self.assertEqual(len(out), 5)
        self.assertEqual(out[1], asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A))

    def test_other_and_mask_doesnt_fire(self):
        # The peephole is specific to mask = 0x80 (sign bit).
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x40), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(len(out), 4)

    def test_non_branch_after_and_doesnt_fire(self):
        prog = _wrap([
            asm_ast.Mov(src=asm_ast.Data(name="x", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="y", offset=0)),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(len(out), 4)


class TestIntermediateStoreVariant(unittest.TestCase):
    """4-instruction form with a STA between LDA and AND. Arises
    after `passes.split_mem_to_mem` lowers a mem-to-mem
    `Mov(M, dst); AND #80; Branch` into `Mov(M, A); Mov(A, dst);
    AND #80; Branch`."""

    def test_lda_sta_and_bne_folds(self):
        x = asm_ast.Data(name="x", offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=x, dst=_A),
            asm_ast.Mov(src=_A, dst=dst),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        # AND dropped; Branch converted to MI.
        self.assertEqual(len(out), 4)
        self.assertEqual(out[0].src, x)
        self.assertEqual(out[1].dst, dst)  # STA preserved
        self.assertEqual(out[2], asm_ast.Branch(
            cond=asm_ast.MI(), target=".target",
        ))

    def test_lda_sta_and_beq_folds(self):
        x = asm_ast.Data(name="x", offset=0)
        dst = asm_ast.ZP(address=0x85, offset=0)
        prog = _wrap([
            asm_ast.Mov(src=x, dst=_A),
            asm_ast.Mov(src=_A, dst=dst),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".target"),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(out[2], asm_ast.Branch(
            cond=asm_ast.PL(), target=".target",
        ))

    def test_skipped_when_a_live_after_branch(self):
        # If A is still live after the Branch, the original AND's
        # value matters — don't drop.
        x = asm_ast.Data(name="x", offset=0)
        dst = asm_ast.Data(name="dst", offset=0)
        prog = _wrap([
            asm_ast.Mov(src=x, dst=_A),
            asm_ast.Mov(src=_A, dst=dst),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".target"),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="y", offset=0)),
            _ret(),
        ])
        out = _instrs(apply_and_sign_bit_branch(prog))
        self.assertEqual(len(out), 6)  # unchanged


class TestBranchTargetAndLoad(unittest.TestCase):
    """The AND is dropped on both paths, so A has to be dead at the
    branch target as well as on the fall-through, and the first
    instruction has to load A."""

    def test_a_read_at_target_blocks(self):
        # `t = flags & 0x80; if (t) {...} return t;` reads t in A at
        # the branch target.
        instrs = [
            asm_ast.Mov(src=asm_ast.Data(name="flags", offset=0), dst=_A),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.EQ(), target=".t"),
            asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_A),
            asm_ast.Return(save_a=True),
            asm_ast.Label(name=".t"),
            asm_ast.Return(save_a=True),
        ]
        self.assertEqual(_instrs(apply_and_sign_bit_branch(_wrap(instrs))),
                         instrs)

    def test_load_into_x_blocks(self):
        # `LDX M` leaves A alone, so the AND tests whatever A held.
        x = asm_ast.Reg(reg=asm_ast.X())
        instrs = [
            asm_ast.Mov(src=asm_ast.Data(name="m", offset=0), dst=x),
            asm_ast.And(src=asm_ast.Imm(value=0x80), dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".t"),
            _ret(),
        ]
        self.assertEqual(_instrs(apply_and_sign_bit_branch(_wrap(instrs))),
                         instrs)


if __name__ == "__main__":
    unittest.main()
