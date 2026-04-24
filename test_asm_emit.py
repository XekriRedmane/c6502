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


class TestEmitUnary(unittest.TestCase):
    def test_not_on_a_emits_eor_ff(self):
        self.assertEqual(
            emit_instruction(asm_ast.Unary(op=asm_ast.Not(), src_dst=_reg(_A))),
            ["   EOR   #$FF"],
        )

    def test_not_on_other_operands_raise(self):
        unsupported = [
            _reg(_X),
            _reg(_Y),
            asm_ast.Imm(value=0),
        ]
        for sd in unsupported:
            with self.subTest(src_dst=sd):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Unary(op=asm_ast.Not(), src_dst=sd))

    def test_neg_on_a_emits_twos_complement_sequence(self):
        self.assertEqual(
            emit_instruction(asm_ast.Unary(op=asm_ast.Neg(), src_dst=_reg(_A))),
            ["   EOR   #$FF", "   CLC", "   ADC   #$01"],
        )

    def test_neg_on_other_operands_raise(self):
        unsupported = [
            _reg(_X),
            _reg(_Y),
            asm_ast.Imm(value=0),
        ]
        for sd in unsupported:
            with self.subTest(src_dst=sd):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Unary(op=asm_ast.Neg(), src_dst=sd))

    def test_not_on_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Unary(op=asm_ast.Not(), src_dst=asm_ast.Stack(offset=3))
            ),
            [
                "   LDY   #$03",
                "   LDA   (SSP),Y",
                "   EOR   #$FF",
                "   STA   (SSP),Y",
            ],
        )

    def test_neg_on_stack(self):
        self.assertEqual(
            emit_instruction(
                asm_ast.Unary(op=asm_ast.Neg(), src_dst=asm_ast.Stack(offset=2))
            ),
            [
                "   LDY   #$02",
                "   LDA   (SSP),Y",
                "   EOR   #$FF",
                "   CLC",
                "   ADC   #$01",
                "   STA   (SSP),Y",
            ],
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

    def test_unary_with_pseudo(self):
        self._assert_pseudo_error(
            asm_ast.Unary(op=asm_ast.Not(), src_dst=asm_ast.Pseudo(name="t"))
        )
        self._assert_pseudo_error(
            asm_ast.Unary(op=asm_ast.Neg(), src_dst=asm_ast.Pseudo(name="t"))
        )


class TestEmitInstruction(unittest.TestCase):
    def test_unknown_instruction_raises(self):
        stub = type("Stub", (asm_ast.Type_instruction,), {})
        with self.assertRaises(TypeError):
            emit_instruction(stub())


class TestEmitAllocateStack(unittest.TestCase):
    def test_zero_emits_nothing(self):
        self.assertEqual(emit_instruction(asm_ast.AllocateStack(amt=0)), [])

    def test_small_amt_subtracts_from_ssp(self):
        # 16-bit subtract on SSP, low then high; high byte is #$00.
        self.assertEqual(
            emit_instruction(asm_ast.AllocateStack(amt=4)),
            [
                "   SEC",
                "   LDA   SSP",
                "   SBC   #$04",
                "   STA   SSP",
                "   LDA   SSP+1",
                "   SBC   #$00",
                "   STA   SSP+1",
            ],
        )

    def test_two_byte_amt_propagates_to_high(self):
        # amt = 0x0123: low = $23, high = $01.
        self.assertEqual(
            emit_instruction(asm_ast.AllocateStack(amt=0x0123)),
            [
                "   SEC",
                "   LDA   SSP",
                "   SBC   #$23",
                "   STA   SSP",
                "   LDA   SSP+1",
                "   SBC   #$01",
                "   STA   SSP+1",
            ],
        )

    def test_amt_out_of_range_raises(self):
        for amt in [-1, 0x10000, 100000]:
            with self.subTest(amt=amt):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.AllocateStack(amt=amt))


class TestEmitRet(unittest.TestCase):
    def test_zero_amt_just_rts(self):
        self.assertEqual(emit_instruction(asm_ast.Ret(amt=0)), ["   RTS"])

    def test_nonzero_amt_pha_add_pla_rts(self):
        self.assertEqual(
            emit_instruction(asm_ast.Ret(amt=3)),
            [
                "   PHA",
                "   CLC",
                "   LDA   SSP",
                "   ADC   #$03",
                "   STA   SSP",
                "   LDA   SSP+1",
                "   ADC   #$00",
                "   STA   SSP+1",
                "   PLA",
                "   RTS",
            ],
        )

    def test_two_byte_amt_propagates_to_high(self):
        self.assertEqual(
            emit_instruction(asm_ast.Ret(amt=0x0102)),
            [
                "   PHA",
                "   CLC",
                "   LDA   SSP",
                "   ADC   #$02",
                "   STA   SSP",
                "   LDA   SSP+1",
                "   ADC   #$01",
                "   STA   SSP+1",
                "   PLA",
                "   RTS",
            ],
        )

    def test_amt_out_of_range_raises(self):
        for amt in [-1, 0x10000]:
            with self.subTest(amt=amt):
                with self.assertRaises(ValueError):
                    emit_instruction(asm_ast.Ret(amt=amt))


class TestEmitFunction(unittest.TestCase):
    def test_label_subroutine_blank_then_instructions(self):
        fn = asm_ast.Function(name="main", instructions=[
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_reg(_A)),
            asm_ast.Ret(amt=0),
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
            asm_ast.Ret(amt=0),
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
            asm_ast.Ret(amt=0),
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
