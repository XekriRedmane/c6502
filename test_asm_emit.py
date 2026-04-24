import io
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import asm_ast
from asm_emit import (
    emit_function,
    emit_instruction,
    emit_program,
    main,
)


def _reg(r):
    return asm_ast.Reg(reg=r)


_A = asm_ast.A()
_X = asm_ast.X()
_Y = asm_ast.Y()


def _prog(*instrs, name="main") -> asm_ast.Type_program:
    return asm_ast.Program(function_definition=asm_ast.Function(
        name=name, instructions=list(instrs),
    ))


class TestEmitMov(unittest.TestCase):
    def test_imm_to_a_emits_lda(self):
        for v, expected in [(0, "#$00"), (1, "#$01"), (0x2A, "#$2A"),
                            (0xFF, "#$FF"), (10, "#$0A")]:
            with self.subTest(v=v):
                self.assertEqual(
                    emit_instruction(
                        asm_ast.Mov(src=asm_ast.Imm(value=v), dst=_reg(_A))
                    ),
                    [f"   LDA   {expected}"],
                )

    def test_imm_to_x_emits_ldx(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=_reg(_X))
            ),
            ["   LDX   #$2A"],
        )

    def test_imm_to_y_emits_ldy(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=_reg(_Y))
            ),
            ["   LDY   #$2A"],
        )

    def test_imm_out_of_range_raises(self):
        for v in [-1, 256, 1000, -100]:
            with self.subTest(v=v):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Mov(src=asm_ast.Imm(value=v), dst=_reg(_A))
                    )

    def test_x_to_a_emits_txa(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_X), dst=_reg(_A))),
            ["   TXA"],
        )

    def test_y_to_a_emits_tya(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_Y), dst=_reg(_A))),
            ["   TYA"],
        )

    def test_a_to_x_emits_tax(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_A), dst=_reg(_X))),
            ["   TAX"],
        )

    def test_a_to_y_emits_tay(self):
        self.assertEqual(
            emit_instruction(asm_ast.Mov(src=_reg(_A), dst=_reg(_Y))),
            ["   TAY"],
        )

    def test_unsupported_mov_combinations_raise(self):
        unsupported = [
            # No 6502 instruction for register-to-register among same reg or
            # the X<->Y pair.
            asm_ast.Mov(src=_reg(_A), dst=_reg(_A)),
            asm_ast.Mov(src=_reg(_X), dst=_reg(_X)),
            asm_ast.Mov(src=_reg(_Y), dst=_reg(_Y)),
            asm_ast.Mov(src=_reg(_X), dst=_reg(_Y)),
            asm_ast.Mov(src=_reg(_Y), dst=_reg(_X)),
            # X/Y <-> Stack not handled (would clobber A); codegen must go
            # via A explicitly.
            asm_ast.Mov(src=_reg(_X), dst=asm_ast.Stack(offset=2)),
            asm_ast.Mov(src=asm_ast.Stack(offset=2), dst=_reg(_X)),
            # Imm cannot be a destination.
            asm_ast.Mov(src=_reg(_A), dst=asm_ast.Imm(value=0)),
        ]
        for instr in unsupported:
            with self.subTest(instr=instr):
                with self.assertRaises(ValueError):
                    emit_instruction(instr)


class TestEmitMovStack(unittest.TestCase):
    def test_imm_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A),
                            dst=asm_ast.Stack(offset=3))
            ),
            ["   LDA   #$2A", "   LDY   #$03", "   STA   (SSP),Y"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Stack(offset=5), dst=_reg(_A))
            ),
            ["   LDY   #$05", "   LDA   (SSP),Y"],
        )

    def test_a_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=_reg(_A), dst=asm_ast.Stack(offset=7))
            ),
            ["   LDY   #$07", "   STA   (SSP),Y"],
        )

    def test_stack_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Stack(offset=1),
                            dst=asm_ast.Stack(offset=4))
            ),
            [
                "   LDY   #$01",
                "   LDA   (SSP),Y",
                "   LDY   #$04",
                "   STA   (SSP),Y",
            ],
        )

    def test_stack_offset_out_of_range_raises(self):
        for off in [-1, 256, 1000]:
            with self.subTest(off=off):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Mov(src=_reg(_A), dst=asm_ast.Stack(offset=off))
                    )


class TestEmitMovFrame(unittest.TestCase):
    def test_imm_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Imm(value=0x2A),
                            dst=asm_ast.Frame(offset=3))
            ),
            ["   LDA   #$2A", "   LDY   #$03", "   STA   (FP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Frame(offset=5), dst=_reg(_A))
            ),
            ["   LDY   #$05", "   LDA   (FP),Y"],
        )

    def test_a_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=_reg(_A), dst=asm_ast.Frame(offset=7))
            ),
            ["   LDY   #$07", "   STA   (FP),Y"],
        )

    def test_frame_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Frame(offset=1),
                            dst=asm_ast.Frame(offset=4))
            ),
            [
                "   LDY   #$01",
                "   LDA   (FP),Y",
                "   LDY   #$04",
                "   STA   (FP),Y",
            ],
        )

    def test_frame_offset_out_of_range_raises(self):
        for off in [-1, 256, 1000]:
            with self.subTest(off=off):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Mov(src=_reg(_A), dst=asm_ast.Frame(offset=off))
                    )


class TestEmitMovMixed(unittest.TestCase):
    """Stack and Frame can appear together in a single Mov; each side
    resolves through its own pointer."""

    def test_stack_to_frame(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Stack(offset=2),
                            dst=asm_ast.Frame(offset=3))
            ),
            [
                "   LDY   #$02",
                "   LDA   (SSP),Y",
                "   LDY   #$03",
                "   STA   (FP),Y",
            ],
        )

    def test_frame_to_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Mov(src=asm_ast.Frame(offset=2),
                            dst=asm_ast.Stack(offset=3))
            ),
            [
                "   LDY   #$02",
                "   LDA   (FP),Y",
                "   LDY   #$03",
                "   STA   (SSP),Y",
            ],
        )


class TestEmitRejectsCompoundOps(unittest.TestCase):
    """Mul, Div, Mod are not valid at the final emit stage — an
    earlier pass is required to lower them into the atomic
    instruction set (a Call instruction, once it exists)."""

    def test_mul_div_mod_raise(self):
        for ctor in [asm_ast.Mul, asm_ast.Div, asm_ast.Mod]:
            with self.subTest(op=ctor.__name__):
                with self.assertRaises(ValueError) as cm:
                    emit_instruction(
                        ctor(src=asm_ast.Imm(value=1), dst=_reg(_A))
                    )
                self.assertIn("Mul/Div/Mod", str(cm.exception))


class TestEmitRejectsPseudo(unittest.TestCase):
    """Pseudo operands must be eliminated before emit; reaching the
    emitter with one indicates the pseudo->stack pass didn't run."""

    def _assert_pseudo_error(self, instr):
        with self.assertRaises(ValueError) as cm:
            emit_instruction(instr)
        self.assertIn("Pseudo", str(cm.exception))

    def test_mov_with_pseudo_src(self):
        self._assert_pseudo_error(
            asm_ast.Mov(src=asm_ast.Pseudo(name="t"), dst=_reg(_A))
        )

    def test_mov_with_pseudo_dst(self):
        self._assert_pseudo_error(
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=asm_ast.Pseudo(name="t"))
        )

    def test_mov_with_pseudo_on_both_sides(self):
        self._assert_pseudo_error(
            asm_ast.Mov(src=asm_ast.Pseudo(name="a"),
                        dst=asm_ast.Pseudo(name="b"))
        )


class TestEmitInstruction(unittest.TestCase):
    def test_unknown_instruction_raises(self):
        stub = type("Stub", (asm_ast.Type_instruction,), {})
        with self.assertRaises(TypeError):
            emit_instruction(stub())


class TestEmitFunctionPrologue(unittest.TestCase):
    def test_zero_emits_nothing(self):
        # No locals (and no args yet) means no FP setup is needed.
        self.assertEqual(emit_instruction(asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=0)), [])

    def test_amt_one_emits_full_prologue(self):
        # SSP -= (M+2) = 3, then write FP into the slot at SSP+2/+3,
        # then FP = SSP.
        self.assertEqual(
            emit_instruction(asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=1)),
            [
                # SSP -= 3
                "   SEC",
                "   LDA   SSP",
                "   SBC   #$03",
                "   STA   SSP",
                "   LDA   SSP+1",
                "   SBC   #$00",
                "   STA   SSP+1",
                # save caller FP into slot at SSP+2 (low) / SSP+3 (high)
                "   LDY   #$02",
                "   LDA   FP",
                "   STA   (SSP),Y",
                "   INY",
                "   LDA   FP+1",
                "   STA   (SSP),Y",
                # FP = SSP
                "   LDA   SSP",
                "   STA   FP",
                "   LDA   SSP+1",
                "   STA   FP+1",
            ],
        )

    def test_max_amt_253_uses_max_ldy(self):
        # M=253 -> save-FP at SSP+254 (low) and SSP+255 (high) — the
        # largest offsets LDY #imm + INY can address.
        out = emit_instruction(asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=253))
        # First line of the SSP-= sub is the only one whose immediate
        # depends on M+2 = 255.
        self.assertIn("   SBC   #$FF", out)
        # The save-FP block uses LDY #$FE then INY (-> $FF).
        self.assertIn("   LDY   #$FE", out)

    def test_amt_too_large_for_ldy_raises(self):
        # M = 254 would need LDY #$FF then INY -> $00, which would
        # overwrite the wrong byte. Pass should reject.
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=254))

    def test_local_bytes_out_of_range_raises(self):
        for lb in [-1, 0x10000, 100000]:
            with self.subTest(local_bytes=lb):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=lb)
                    )


class TestEmitRet(unittest.TestCase):
    def test_zero_dimensions_just_rts(self):
        # No args and no locals — nothing to dealloc, no FP to restore.
        self.assertEqual(
            emit_instruction(asm_ast.Ret(arg_bytes=0, local_bytes=0)),
            ["   RTS"],
        )

    def test_locals_only_full_epilogue(self):
        # M=3, N=0: SSP = FP + (M+N+2) = FP + 5; saved FP at FP+M+1=4
        # (low) / FP+M+2=5 (high), read via (FP),Y with X as scratch.
        self.assertEqual(
            emit_instruction(asm_ast.Ret(arg_bytes=0, local_bytes=3)),
            [
                "   PHA",
                # SSP = FP + 5
                "   CLC",
                "   LDA   FP",
                "   ADC   #$05",
                "   STA   SSP",
                "   LDA   FP+1",
                "   ADC   #$00",
                "   STA   SSP+1",
                # restore caller FP from FP+4 (low) / FP+5 (high)
                "   LDY   #$04",
                "   LDA   (FP),Y",
                "   TAX",
                "   INY",
                "   LDA   (FP),Y",
                "   STA   FP+1",
                "   STX   FP",
                "   PLA",
                "   RTS",
            ],
        )

    def test_args_shift_ssp_rewind_not_fp_slot(self):
        # M=2, N=4: SSP rewind is FP + (4+2+2) = FP + 8, but the
        # saved-FP slot is still at FP+M+1=3 / FP+M+2=4 — args don't
        # shift the slot location.
        out = emit_instruction(asm_ast.Ret(arg_bytes=4, local_bytes=2))
        # SSP rewind low byte = M+N+2 = 8
        self.assertIn("   ADC   #$08", out)
        # FP-slot read uses LDY #(M+1) = #$03
        self.assertIn("   LDY   #$03", out)

    def test_two_byte_rewind_propagates_to_high(self):
        # The SSP-rewind ADC pair carries between low and high. With
        # a 9-bit-ish total (e.g. N=0x100, M=0), low byte = $02
        # (= 0x100+0+2 low byte), high byte = $01.
        out = emit_instruction(asm_ast.Ret(arg_bytes=0x100, local_bytes=0))
        self.assertIn("   ADC   #$02", out)
        self.assertIn("   ADC   #$01", out)

    def test_local_bytes_out_of_range_raises(self):
        for lb in [-1, 254, 1000]:
            with self.subTest(local_bytes=lb):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Ret(arg_bytes=0, local_bytes=lb))

    def test_total_out_of_range_raises(self):
        # M+N+2 > 0xFFFF can't fit in a 16-bit SSP arithmetic.
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Ret(arg_bytes=0xFFFE, local_bytes=0))


class TestEmitFunction(unittest.TestCase):
    def test_label_subroutine_blank_then_instructions(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_reg(_A)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        ])
        self.assertEqual(
            emit_function(fn),
            [
                "main:",
                "   SUBROUTINE",
                "",
                "   LDA   #$00",
                "   RTS",
            ],
        )

    def test_empty_instructions_label_and_subroutine_only(self):
        fn = asm_ast.Function(name="main", instructions=[])
        self.assertEqual(emit_function(fn), ["main:", "   SUBROUTINE"])


class TestEmitProgram(unittest.TestCase):
    def test_full(self):
        prog = _prog(
            asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_reg(_A)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        )
        self.assertEqual(
            emit_program(prog),
            "main:\n   SUBROUTINE\n\n   LDA   #$2A\n   RTS\n",
        )


class TestColumnAlignment(unittest.TestCase):
    """Column 1 labels, column 4 opcodes / directives, column 10 operands."""

    def test_columns(self):
        prog = _prog(
            asm_ast.Mov(src=asm_ast.Imm(value=0x2A), dst=_reg(_A)),
            asm_ast.Ret(arg_bytes=0, local_bytes=0),
        )
        lines = emit_program(prog).splitlines()
        # Label at column 1 (index 0).
        self.assertTrue(lines[0].startswith("main:"))
        # SUBROUTINE directive at column 4.
        self.assertEqual(lines[1][:3], "   ")
        self.assertEqual(lines[1][3:], "SUBROUTINE")
        # Blank line separating directive from instructions.
        self.assertEqual(lines[2], "")
        # Opcode at column 4 (index 3), operand at column 10 (index 9).
        self.assertEqual(lines[3][:3], "   ")
        self.assertEqual(lines[3][3:6], "LDA")
        self.assertEqual(lines[3][6:9], "   ")
        self.assertEqual(lines[3][9:], "#$2A")
        # RTS has no operand.
        self.assertEqual(lines[4], "   RTS")


class TestEmitAdd(unittest.TestCase):
    """Add at emit is a single ADC (with addressing-mode setup for
    indirect-Y sources). Carry must be set up by an earlier ClearCarry;
    dst must be Reg(A); src can be Imm/Stack/Frame."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Add(src=asm_ast.Imm(value=0x2A), dst=_reg(_A))
            ),
            ["   ADC   #$2A"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Add(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   ADC   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Add(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   ADC   (FP),Y"],
        )

    def test_imm_out_of_range_raises(self):
        for v in [-1, 256, 1000]:
            with self.subTest(v=v):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Add(src=asm_ast.Imm(value=v), dst=_reg(_A))
                    )

    def test_dst_must_be_a(self):
        for dst in [_reg(_X), _reg(_Y), asm_ast.Stack(offset=1),
                    asm_ast.Frame(offset=1), asm_ast.Imm(value=0)]:
            with self.subTest(dst=dst):
                with self.assertRaises(ValueError):
                    emit_instruction(
                        asm_ast.Add(src=asm_ast.Imm(value=1), dst=dst)
                    )

    def test_register_src_raises(self):
        # Reg(X)/Reg(Y)/Reg(A) — ADC has no register-direct source.
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Add(src=src, dst=_reg(_A)))

    def test_pseudo_src_rejected(self):
        with self.assertRaises(ValueError) as cm:
            emit_instruction(
                asm_ast.Add(src=asm_ast.Pseudo(name="t"), dst=_reg(_A))
            )
        self.assertIn("Pseudo", str(cm.exception))


class TestEmitSub(unittest.TestCase):
    """Sub at emit is a single SBC (with addressing-mode setup for
    indirect-Y sources); same operand constraints as Add. Carry must
    be set by a preceding SetCarry."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Imm(value=0x2A), dst=_reg(_A))
            ),
            ["   SBC   #$2A"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   SBC   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   SBC   (FP),Y"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(
                asm_ast.Sub(src=asm_ast.Imm(value=1), dst=_reg(_X))
            )

    def test_register_src_raises(self):
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Sub(src=src, dst=_reg(_A)))


class TestEmitClearSetCarry(unittest.TestCase):
    def test_clear_carry(self):
        self.assertEqual(emit_instruction(asm_ast.ClearCarry()), ["   CLC"])

    def test_set_carry(self):
        self.assertEqual(emit_instruction(asm_ast.SetCarry()), ["   SEC"])


class TestEmitInc(unittest.TestCase):
    def test_inc_x(self):
        self.assertEqual(
            emit_instruction(asm_ast.Inc(dst=_reg(_X))),
            ["   INX"],
        )

    def test_inc_y(self):
        self.assertEqual(
            emit_instruction(asm_ast.Inc(dst=_reg(_Y))),
            ["   INY"],
        )

    def test_inc_a_raises(self):
        # Plain 6502 has no INA.
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Inc(dst=_reg(_A)))

    def test_inc_other_raises(self):
        for dst in [asm_ast.Imm(value=1), asm_ast.Stack(offset=1),
                    asm_ast.Frame(offset=1)]:
            with self.subTest(dst=dst):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Inc(dst=dst))


class TestEmitDec(unittest.TestCase):
    def test_dec_x(self):
        self.assertEqual(
            emit_instruction(asm_ast.Dec(dst=_reg(_X))),
            ["   DEX"],
        )

    def test_dec_y(self):
        self.assertEqual(
            emit_instruction(asm_ast.Dec(dst=_reg(_Y))),
            ["   DEY"],
        )

    def test_dec_a_raises(self):
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Dec(dst=_reg(_A)))


class TestEmitPushPop(unittest.TestCase):
    def test_push_a(self):
        self.assertEqual(
            emit_instruction(asm_ast.Push(src=_reg(_A))),
            ["   PHA"],
        )

    def test_pop_a(self):
        self.assertEqual(
            emit_instruction(asm_ast.Pop(dst=_reg(_A))),
            ["   PLA"],
        )

    def test_push_non_a_raises(self):
        for src in [_reg(_X), _reg(_Y),
                    asm_ast.Imm(value=1), asm_ast.Stack(offset=1)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Push(src=src))

    def test_pop_non_a_raises(self):
        for dst in [_reg(_X), _reg(_Y), asm_ast.Stack(offset=1)]:
            with self.subTest(dst=dst):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Pop(dst=dst))


class TestEmitXor(unittest.TestCase):
    def test_a_imm(self):
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Imm(value=0xFF),
                dst=_reg(_A),
            )),
            ["   EOR   #$FF"],
        )

    def test_imm_a_other_order(self):
        # XOR is commutative; emit accepts either ordering of the
        # (Reg(A), Imm) pair.
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=asm_ast.Imm(value=0x0A),
                src2=_reg(_A),
                dst=_reg(_A),
            )),
            ["   EOR   #$0A"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Imm(value=1),
                dst=_reg(_X),
            ))

    def test_srcs_must_be_a_and_imm(self):
        bad = [
            (asm_ast.Imm(value=1), asm_ast.Imm(value=2)),     # both Imm
            (_reg(_A), _reg(_A)),                              # both A
            (_reg(_X), asm_ast.Imm(value=1)),                  # X not A
            (asm_ast.Stack(offset=1), asm_ast.Imm(value=1)),   # Stack not A
            (_reg(_A), asm_ast.Stack(offset=1)),               # Stack not Imm
        ]
        for s1, s2 in bad:
            with self.subTest(src1=s1, src2=s2):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Xor(
                        src1=s1, src2=s2, dst=_reg(_A),
                    ))

    def test_imm_out_of_range_raises(self):
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Imm(value=256),
                dst=_reg(_A),
            ))


class TestMainCLI(unittest.TestCase):
    def test_stdout_output(self):
        src = "int main(void) { return 42; }"
        with patch("sys.stdin", io.StringIO(src)), \
             patch("sys.stdout", new_callable=io.StringIO) as out:
            rc = main(["asm_emit.py", "-"])
        self.assertEqual(rc, 0)
        self.assertEqual(
            out.getvalue(),
            "main:\n   SUBROUTINE\n\n   LDA   #$2A\n   RTS\n",
        )

    def test_output_file_must_end_in_asm(self):
        with patch("sys.stdin", io.StringIO("int main(void) { return 0; }")), \
             patch("sys.stderr", new_callable=io.StringIO) as err:
            rc = main(["asm_emit.py", "-", "-o", "out.txt"])
        self.assertNotEqual(rc, 0)
        self.assertIn(".asm suffix", err.getvalue())

    def test_file_output_writes_asm(self):
        with tempfile.TemporaryDirectory() as tmp:
            out_path = Path(tmp) / "hello.asm"
            with patch("sys.stdin", io.StringIO("int main(void) { return 7; }")):
                rc = main(["asm_emit.py", "-", "-o", str(out_path)])
            self.assertEqual(rc, 0)
            self.assertEqual(
                out_path.read_text(),
                "main:\n   SUBROUTINE\n\n   LDA   #$07\n   RTS\n",
            )


if __name__ == "__main__":
    unittest.main()
