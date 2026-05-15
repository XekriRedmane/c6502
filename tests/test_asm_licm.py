"""Tests for the asm-level LICM-lite pass."""

import unittest

import asm_ast
from passes.asm_licm import apply_licm


_A = asm_ast.Reg(reg=asm_ast.A())


def _instrs(prog):
    return prog.top_level[0].instructions


def _wrap(instrs):
    return asm_ast.Program(top_level=[asm_ast.Function(
        name="f", is_global=False, params=[], instructions=instrs,
    )])


class TestAsmLicm(unittest.TestCase):

    def test_imm_store_inside_loop_hoisted_to_preheader(self):
        # LDA #c; STA M inside a loop body — no other writes of M in
        # the loop, no Call — hoist the pair to just before the
        # loop header.
        instrs = [
            asm_ast.Label(name=".prehdr"),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=asm_ast.Imm(value=0x42), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="slot", offset=0)),
            asm_ast.Mov(src=asm_ast.Data(name="counter", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_licm(_wrap(instrs)))
        # The LDA #$42; STA slot pair appears BEFORE .loop_start.
        loop_idx = next(
            i for i, x in enumerate(out)
            if isinstance(x, asm_ast.Label) and x.name == ".loop_start"
        )
        body = out[loop_idx + 1:]
        # No more STA slot in the body.
        self.assertFalse(any(
            isinstance(x, asm_ast.Mov)
            and isinstance(x.dst, asm_ast.Data)
            and x.dst.name == "slot"
            for x in body
        ))
        # The hoisted pair appears in the preheader region.
        preheader = out[:loop_idx]
        self.assertTrue(any(
            isinstance(x, asm_ast.Mov)
            and isinstance(x.src, asm_ast.Reg)
            and isinstance(x.src.reg, asm_ast.A)
            and isinstance(x.dst, asm_ast.Data)
            and x.dst.name == "slot"
            for x in preheader
        ))

    def test_imm_store_with_call_in_loop_not_hoisted(self):
        # Conservative: any Call in the loop body disqualifies the
        # hoist, because the callee might clobber the dst.
        instrs = [
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=asm_ast.Imm(value=0x42), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="slot", offset=0)),
            asm_ast.Call(name="helper"),
            asm_ast.Mov(src=asm_ast.Data(name="counter", offset=0), dst=_A),
            asm_ast.Compare(left=_A, right=asm_ast.Imm(value=0)),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_licm(_wrap(instrs)))
        # Unchanged.
        self.assertEqual(out, instrs)

    def test_imm_store_with_other_write_of_dst_not_hoisted(self):
        # If something else inside the loop body writes the dst,
        # the hoist isn't safe — the in-loop write would need the
        # in-loop constant store to follow it.
        instrs = [
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=asm_ast.Imm(value=0x42), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="slot", offset=0)),
            # A second write of "slot" inside the body:
            asm_ast.Mov(src=asm_ast.Data(name="other", offset=0), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="slot", offset=0)),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_licm(_wrap(instrs)))
        self.assertEqual(out, instrs)

    def test_zp_target_also_hoists(self):
        # Same pass should hoist when the dst is a ZP slot rather
        # than a Data symbol.
        instrs = [
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=asm_ast.Imm(value=0x07),
                        dst=asm_ast.ZP(address=0x90, offset=0)),
            asm_ast.Mov(src=asm_ast.Data(name="counter", offset=0), dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_licm(_wrap(instrs)))
        # The Mov(Imm, ZP) is now in the preheader.
        loop_idx = next(
            i for i, x in enumerate(out)
            if isinstance(x, asm_ast.Label) and x.name == ".loop_start"
        )
        preheader = out[:loop_idx]
        self.assertTrue(any(
            isinstance(x, asm_ast.Mov)
            and isinstance(x.src, asm_ast.Imm)
            and isinstance(x.dst, asm_ast.ZP)
            for x in preheader
        ))

    def test_side_entry_label_disables_hoist(self):
        # If another label inside the loop body is the target of an
        # OUTSIDE branch/jump, the body has a side entry and the
        # hoist isn't safe (the side-entry path bypasses the
        # preheader-hoisted writes).
        instrs = [
            asm_ast.Jump(target=".side_entry"),
            asm_ast.Label(name=".loop_start"),
            asm_ast.Mov(src=asm_ast.Imm(value=0x42), dst=_A),
            asm_ast.Mov(src=_A, dst=asm_ast.Data(name="slot", offset=0)),
            asm_ast.Label(name=".side_entry"),
            asm_ast.Mov(src=asm_ast.Data(name="counter", offset=0), dst=_A),
            asm_ast.Branch(cond=asm_ast.NE(), target=".loop_start"),
            asm_ast.Return(save_a=False),
        ]
        out = _instrs(apply_licm(_wrap(instrs)))
        self.assertEqual(out, instrs)


if __name__ == "__main__":
    unittest.main()
