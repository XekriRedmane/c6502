import unittest

import asm_ast
from asm_emit import (
    emit_function,
    emit_instruction,
    emit_program,
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


class TestEmitCall(unittest.TestCase):
    """Call(name) maps to a single JSR <name>. The runtime helpers
    mul8 / divmod8 are the two call targets tac_to_asm emits today."""

    def test_call_mul8(self):
        self.assertEqual(
            emit_instruction(asm_ast.Call(name="mul8")),
            ["   JSR   mul8"],
        )

    def test_call_divmod8(self):
        self.assertEqual(
            emit_instruction(asm_ast.Call(name="divmod8")),
            ["   JSR   divmod8"],
        )

    def test_call_arbitrary_name(self):
        self.assertEqual(
            emit_instruction(asm_ast.Call(name="my_fn")),
            ["   JSR   my_fn"],
        )


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
        # then FP = SSP. A leading `; prologue: ...` comment and
        # trailing blank line mark the boilerplate region.
        self.assertEqual(
            emit_instruction(asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=1)),
            [
                "   ; prologue: 0 arg bytes, 1 local bytes",
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
                "",
            ],
        )

    def test_prologue_header_reports_arg_and_local_bytes(self):
        # The header text embeds both field values so a reader can
        # see the frame shape without inspecting the asm below.
        out = emit_instruction(
            asm_ast.FunctionPrologue(arg_bytes=4, local_bytes=2)
        )
        self.assertEqual(
            out[0], "   ; prologue: 4 arg bytes, 2 local bytes",
        )
        # And the trailing blank separator is always the last element.
        self.assertEqual(out[-1], "")

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
        # A leading blank line + `; epilogue` comment mark where the
        # boilerplate starts.
        self.assertEqual(
            emit_instruction(asm_ast.Ret(arg_bytes=0, local_bytes=3)),
            [
                "",
                "   ; epilogue",
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

    def test_prologue_body_epilogue_section_markers(self):
        # With a non-trivial frame, the output should have `; prologue: ...`
        # before the prologue asm, a blank line + body asm, a blank line,
        # then `; epilogue` before the epilogue asm. This is the visual
        # separator between boilerplate and actual content.
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=1),
            asm_ast.Mov(src=asm_ast.Imm(value=7),
                        dst=asm_ast.Frame(offset=1)),
            asm_ast.Mov(src=asm_ast.Frame(offset=1),
                        dst=asm_ast.Reg(reg=asm_ast.A())),
            asm_ast.Ret(arg_bytes=0, local_bytes=1),
        ])
        out = emit_function(fn)
        self.assertIn("   ; prologue: 0 arg bytes, 1 local bytes", out)
        self.assertIn("   ; epilogue", out)
        # Prologue comment precedes any of the body's Frame accesses.
        prologue_idx = out.index(
            "   ; prologue: 0 arg bytes, 1 local bytes"
        )
        body_idx = out.index("   LDA   #$07")
        epilogue_idx = out.index("   ; epilogue")
        self.assertLess(prologue_idx, body_idx)
        self.assertLess(body_idx, epilogue_idx)
        # Blank line immediately before the epilogue comment, and
        # immediately after the last prologue line (via the trailing
        # blank emitted by the prologue).
        self.assertEqual(out[epilogue_idx - 1], "")
        self.assertEqual(out[body_idx - 1], "")

    def test_empty_body_collapses_consecutive_blanks(self):
        # Prologue trails with a blank; Ret leads with a blank. In a
        # function with no body between them, emit_function collapses
        # the two blanks into one so we don't get a double-blank gap.
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.FunctionPrologue(arg_bytes=0, local_bytes=1),
            asm_ast.Ret(arg_bytes=0, local_bytes=1),
        ])
        out = emit_function(fn)
        # No two adjacent blank lines anywhere.
        for i in range(len(out) - 1):
            with self.subTest(i=i):
                self.assertFalse(
                    out[i] == "" and out[i + 1] == "",
                    f"double blank at lines {i}..{i+1}",
                )


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

    def test_one_src_must_be_a(self):
        # One src must be Reg(A); the other carries the addressing mode.
        # Neither side being A is rejected. Two Reg(A) is also rejected
        # because the non-A side then fails the Imm/Stack/Frame check.
        bad = [
            (asm_ast.Imm(value=1), asm_ast.Imm(value=2)),     # both Imm
            (_reg(_A), _reg(_A)),                              # both A
            (_reg(_X), asm_ast.Imm(value=1)),                  # X not A
            (asm_ast.Stack(offset=1), asm_ast.Imm(value=1)),   # Stack not A
        ]
        for s1, s2 in bad:
            with self.subTest(src1=s1, src2=s2):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Xor(
                        src1=s1, src2=s2, dst=_reg(_A),
                    ))

    def test_a_stack(self):
        # A XOR <byte at SSP+off> — reads via indirect-Y, then EOR.
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Stack(offset=3),
                dst=_reg(_A),
            )),
            ["   LDY   #$03", "   EOR   (SSP),Y"],
        )

    def test_a_frame_either_order(self):
        # XOR is commutative; the non-A operand picks the addressing
        # mode regardless of which slot it sits in.
        expected = ["   LDY   #$02", "   EOR   (FP),Y"]
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Frame(offset=2),
                dst=_reg(_A),
            )),
            expected,
        )
        self.assertEqual(
            emit_instruction(asm_ast.Xor(
                src1=asm_ast.Frame(offset=2),
                src2=_reg(_A),
                dst=_reg(_A),
            )),
            expected,
        )

    def test_imm_out_of_range_raises(self):
        with self.assertRaises(ValueError):
            emit_instruction(asm_ast.Xor(
                src1=_reg(_A),
                src2=asm_ast.Imm(value=256),
                dst=_reg(_A),
            ))


class TestEmitShiftRotateAcc(unittest.TestCase):
    """ASL/LSR/ROL/ROR all currently target the accumulator only —
    soft-stack values can't be addressed by these opcodes (no
    indirect-Y mode). Reg(A) emits `<OP> A`; anything else raises."""

    _CASES = [
        (asm_ast.ArithmeticShiftLeft, "ASL"),
        (asm_ast.LogicalShiftRight,   "LSR"),
        (asm_ast.RotateLeft,          "ROL"),
        (asm_ast.RotateRight,         "ROR"),
    ]

    def test_acc_emit(self):
        for cls, opcode in self._CASES:
            with self.subTest(op=opcode):
                self.assertEqual(
                    emit_instruction(cls(dst=_reg(_A))),
                    [f"   {opcode}   A"],
                )

    def test_non_acc_dst_raises(self):
        for cls, _ in self._CASES:
            for dst in [_reg(_X), _reg(_Y),
                        asm_ast.Imm(value=1),
                        asm_ast.Stack(offset=1),
                        asm_ast.Frame(offset=1)]:
                with self.subTest(op=cls.__name__, dst=dst):
                    with self.assertRaises(ValueError):
                        emit_instruction(cls(dst=dst))

    def test_pseudo_dst_rejected(self):
        for cls, _ in self._CASES:
            with self.subTest(op=cls.__name__):
                with self.assertRaises(ValueError) as cm:
                    emit_instruction(cls(dst=asm_ast.Pseudo(name="t")))
                self.assertIn("Pseudo", str(cm.exception))


class TestEmitAnd(unittest.TestCase):
    """And at emit is a single AND instruction (with addressing-mode
    setup for indirect-Y sources). dst must be Reg(A); src can be
    Imm/Stack/Frame. AND/ORA do not affect carry, so no carry setup
    is required."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=_reg(_A))
            ),
            ["   AND   #$0F"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.And(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   AND   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.And(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   AND   (FP),Y"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(
                asm_ast.And(src=asm_ast.Imm(value=1), dst=_reg(_X))
            )

    def test_register_src_raises(self):
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.And(src=src, dst=_reg(_A)))

    def test_pseudo_src_rejected(self):
        with self.assertRaises(ValueError) as cm:
            emit_instruction(
                asm_ast.And(src=asm_ast.Pseudo(name="t"), dst=_reg(_A))
            )
        self.assertIn("Pseudo", str(cm.exception))


class TestEmitOr(unittest.TestCase):
    """Or at emit is a single ORA instruction; same operand shape as And."""

    def test_imm_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Or(src=asm_ast.Imm(value=0xF0), dst=_reg(_A))
            ),
            ["   ORA   #$F0"],
        )

    def test_stack_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Or(src=asm_ast.Stack(offset=3), dst=_reg(_A))
            ),
            ["   LDY   #$03", "   ORA   (SSP),Y"],
        )

    def test_frame_to_a(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Or(src=asm_ast.Frame(offset=2), dst=_reg(_A))
            ),
            ["   LDY   #$02", "   ORA   (FP),Y"],
        )

    def test_dst_must_be_a(self):
        with self.assertRaises(ValueError):
            emit_instruction(
                asm_ast.Or(src=asm_ast.Imm(value=1), dst=_reg(_X))
            )

    def test_register_src_raises(self):
        for src in [_reg(_X), _reg(_Y), _reg(_A)]:
            with self.subTest(src=src):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Or(src=src, dst=_reg(_A)))


if __name__ == "__main__":
    unittest.main()
