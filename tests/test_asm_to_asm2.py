"""Tests for `passes.asm_to_asm2.translate_program`.

The pass takes an asm_ast.Program and returns an asm2_ast.Program
with the three asm_ast compound nodes (`AllocateStack`,
`FunctionPrologue`, `Ret`) expanded into their atom sequences.
Operands / static_inits / regs / conditions get re-tagged at the
asm2_ast type but otherwise round-trip unchanged.
"""

from __future__ import annotations

import unittest

import asm_ast
import asm2_ast
from passes.asm_to_asm2 import translate_program


def _fn(*instructions: asm_ast.Type_instruction) -> asm_ast.Function:
    return asm_ast.Function(
        name="f", is_global=True, params=[],
        instructions=list(instructions),
    )


def _prog(*instructions: asm_ast.Type_instruction) -> asm_ast.Program:
    return asm_ast.Program(top_level=[_fn(*instructions)])


def _lower(*instructions: asm_ast.Type_instruction):
    """Lower a one-function program; return the function's
    instruction list."""
    out = translate_program(_prog(*instructions))
    self_fn = out.top_level[0]
    assert isinstance(self_fn, asm2_ast.Function)
    return self_fn.instructions


class TestPassthroughInstructions(unittest.TestCase):
    """Single-instruction asm_ast nodes pass through to a single
    same-shape asm2_ast node. Operand / reg / condition payloads
    re-tag at the asm2 type."""

    def test_mov_imm_to_reg(self):
        out = _lower(asm_ast.Mov(
            src=asm_ast.Imm(value=0x42),
            dst=asm_ast.Reg(reg=asm_ast.A()),
        ))
        self.assertEqual(len(out), 1)
        self.assertIsInstance(out[0], asm2_ast.Mov)
        self.assertEqual(out[0].src, asm2_ast.Imm(value=0x42))
        self.assertEqual(out[0].dst, asm2_ast.Reg(reg=asm2_ast.A()))

    def test_branch_re_tags_condition(self):
        out = _lower(asm_ast.Branch(cond=asm_ast.EQ(), target=".end@0"))
        self.assertEqual(len(out), 1)
        self.assertIsInstance(out[0], asm2_ast.Branch)
        self.assertIsInstance(out[0].cond, asm2_ast.EQ)
        self.assertEqual(out[0].target, ".end@0")

    def test_label_passes_through(self):
        out = _lower(asm_ast.Label(name=".start@0"))
        self.assertEqual(out, [asm2_ast.Label(name=".start@0")])

    def test_load_address_stays_compound(self):
        # LoadAddress is one of the asm2 atoms — stays a single
        # node (its expansion is short enough to keep as "one
        # logical compute-an-address-into-two-bytes step").
        out = _lower(asm_ast.LoadAddress(
            src=asm_ast.Frame(offset=1),
            dst=asm_ast.Frame(offset=3),
        ))
        self.assertEqual(len(out), 1)
        self.assertIsInstance(out[0], asm2_ast.LoadAddress)


class TestAllocateStackExpansion(unittest.TestCase):
    """`AllocateStack(n)` lowers to a 7-atom 16-bit `SSP -= n`
    sequence (or [] when n == 0)."""

    def test_zero_emits_nothing(self):
        self.assertEqual(_lower(asm_ast.AllocateStack(bytes=0)), [])

    def test_one_byte_sub(self):
        out = _lower(asm_ast.AllocateStack(bytes=1))
        # SetCarry, LDA SSP, Sub #1, STA SSP, LDA SSP+1, Sub #0, STA SSP+1
        self.assertEqual(len(out), 7)
        self.assertIsInstance(out[0], asm2_ast.SetCarry)
        # Mov LDA SSP
        self.assertIsInstance(out[1], asm2_ast.Mov)
        self.assertEqual(out[1].src, asm2_ast.Data(name="SSP", offset=0))
        # Sub #1
        self.assertIsInstance(out[2], asm2_ast.Sub)
        self.assertEqual(out[2].src, asm2_ast.Imm(value=1))
        # STA SSP
        self.assertIsInstance(out[3], asm2_ast.Mov)
        self.assertEqual(out[3].dst, asm2_ast.Data(name="SSP", offset=0))

    def test_high_byte_set_when_amount_exceeds_byte(self):
        out = _lower(asm_ast.AllocateStack(bytes=0x0100))
        # The two Sub atoms should carry the lo and hi immediates.
        subs = [i for i in out if isinstance(i, asm2_ast.Sub)]
        self.assertEqual(len(subs), 2)
        self.assertEqual(subs[0].src, asm2_ast.Imm(value=0x00))
        self.assertEqual(subs[1].src, asm2_ast.Imm(value=0x01))


class TestFunctionPrologueExpansion(unittest.TestCase):
    """`FunctionPrologue` expands to:
        Comment "prologue: ..."
        SSP-sub of (M+2)
        save FP into Stack(M+1) / Stack(M+2)  (4 atoms — naive)
        FP = SSP                              (4 atoms)
        per callee-save: 2 atoms (LDA $XX; STA Frame(i+1))
        Blank
    """

    def test_zero_emits_nothing(self):
        self.assertEqual(
            _lower(asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=0)),
            [],
        )

    def test_amt_one_emits_full_prologue(self):
        out = _lower(asm_ast.FunctionPrologue(
            arg_bytes=0, local_bytes=1,
        ))
        # Expected lengths: 1 comment + 7 ssp_sub + 4 save_fp +
        # 4 set_fp_to_ssp + 1 blank = 17 atoms.
        self.assertEqual(len(out), 17)
        self.assertIsInstance(out[0], asm2_ast.Comment)
        self.assertIn("prologue", out[0].text)
        self.assertIsInstance(out[-1], asm2_ast.Blank)

    def test_callee_saves_emit_extra_pairs(self):
        out = _lower(asm_ast.FunctionPrologue(
            arg_bytes=0, local_bytes=2,
            callee_saved_addrs=[0xC0, 0xC1],
        ))
        # Each callee save is 2 atoms (LDA ZP; STA Frame). Two
        # saves = 4 extra atoms vs. the no-save case for the
        # same M.
        no_saves = _lower(asm_ast.FunctionPrologue(
            arg_bytes=0, local_bytes=2,
        ))
        self.assertEqual(len(out) - len(no_saves), 4)


class TestRetExpansion(unittest.TestCase):
    """`Ret` expands to:
        no-frame case: just `Return` (RTS)
        with-frame case:
            Blank
            Comment "epilogue"
            per callee-restore: 2 atoms (LDA Frame(i+1); STA $XX)
            optional Push(A)        (when save_a=True)
            SSP = FP + (N+M+2)      (7 atoms or 4 when amt=0)
            restore FP from Frame(M+1) / Frame(M+2) via X (6 atoms — naive)
            optional Pop(A)         (when save_a=True)
            Return
    """

    def test_zero_dimensions_just_return(self):
        self.assertEqual(
            _lower(asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True)),
            [asm2_ast.Return()],
        )

    def test_with_frame_save_a_true(self):
        out = _lower(asm_ast.Ret(
            arg_bytes=0, local_bytes=3, save_a=True,
        ))
        # Last instruction is the Return atom.
        self.assertIsInstance(out[-1], asm2_ast.Return)
        # First two are Blank + Comment.
        self.assertIsInstance(out[0], asm2_ast.Blank)
        self.assertIsInstance(out[1], asm2_ast.Comment)
        self.assertEqual(out[1].text, "epilogue")
        # Push(A) and Pop(A) bracket the SSP/FP teardown when save_a.
        pushes = [i for i in out if isinstance(i, asm2_ast.Push)]
        pops = [i for i in out if isinstance(i, asm2_ast.Pop)]
        self.assertEqual(len(pushes), 1)
        self.assertEqual(len(pops), 1)

    def test_save_a_false_skips_push_pop(self):
        out = _lower(asm_ast.Ret(
            arg_bytes=0, local_bytes=3, save_a=False,
        ))
        for i in out:
            self.assertNotIsInstance(i, asm2_ast.Push)
            self.assertNotIsInstance(i, asm2_ast.Pop)

    def test_callee_restores_emit_pairs(self):
        out = _lower(asm_ast.Ret(
            arg_bytes=0, local_bytes=2, save_a=True,
            callee_saved_addrs=[0xC0, 0xC1],
        ))
        no_saves = _lower(asm_ast.Ret(
            arg_bytes=0, local_bytes=2, save_a=True,
        ))
        # Each restore is 2 atoms.
        self.assertEqual(len(out) - len(no_saves), 4)


class TestStaticVariablesPassThrough(unittest.TestCase):
    def test_int_init_re_tags(self):
        prog = asm_ast.Program(top_level=[
            asm_ast.StaticVariable(
                name="g", is_global=True,
                init=[asm_ast.IntInit(value=42)],
            ),
        ])
        out = translate_program(prog)
        self.assertEqual(len(out.top_level), 1)
        sv = out.top_level[0]
        self.assertIsInstance(sv, asm2_ast.StaticVariable)
        self.assertEqual(sv.name, "g")
        self.assertEqual(len(sv.init), 1)
        self.assertIsInstance(sv.init[0], asm2_ast.IntInit)
        self.assertEqual(sv.init[0].value, 42)


if __name__ == "__main__":
    unittest.main()
