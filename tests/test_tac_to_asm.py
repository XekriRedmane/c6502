import unittest

import asm_ast
import tac_ast
from tac_to_asm import (
    translate_binary,
    translate_function,
    translate_instruction,
    translate_program,
    translate_unop_atoms,
    translate_val,
)


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())


class TestTranslateVal(unittest.TestCase):
    def test_constant_becomes_imm(self):
        self.assertEqual(
            translate_val(tac_ast.Constant(const=tac_ast.ConstInt(int=42))),
            asm_ast.Imm(value=42),
        )

    def test_var_becomes_pseudo(self):
        self.assertEqual(
            translate_val(tac_ast.Var(name="%0")),
            asm_ast.Pseudo(name="%0", offset=0),
        )


class TestTranslateUnopAtoms(unittest.TestCase):
    def test_complement_emits_xor_with_ff(self):
        self.assertEqual(
            translate_unop_atoms(tac_ast.Complement()),
            [asm_ast.Xor(
                src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
            )],
        )

    def test_negate_emits_xor_clearcarry_add_one(self):
        self.assertEqual(
            translate_unop_atoms(tac_ast.Negate()),
            [
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                ),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
            ],
        )

    def test_logical_not_lowers_inline_with_beq_and_0_1_select(self):
        # !A := 1 if A == 0 else 0. The framing Mov(src, A) around
        # this atom sequence already sets Z, so we branch on EQ
        # directly (no extra Compare). Module-level wrapper builds a
        # fresh Translator, so labels start at _0 / _1.
        self.assertEqual(
            translate_unop_atoms(tac_ast.LogicalNot()),
            [
                asm_ast.Branch(cond=asm_ast.EQ(), target=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                asm_ast.Jump(target=".lnot_end@1"),
                asm_ast.Label(name=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Label(name=".lnot_end@1"),
            ],
        )

    def test_logical_not_labels_are_unique_across_uses(self):
        # Reusing a Translator (as happens within a program) keeps
        # the counter advancing so two ! uses don't collide.
        from tac_to_asm import Translator
        t = Translator()
        first = t.translate_unop_atoms(tac_ast.LogicalNot())
        second = t.translate_unop_atoms(tac_ast.LogicalNot())
        first_labels = {
            i.name for i in first if isinstance(i, asm_ast.Label)
        }
        second_labels = {
            i.name for i in second if isinstance(i, asm_ast.Label)
        }
        self.assertTrue(first_labels.isdisjoint(second_labels))


class TestTranslateInstruction(unittest.TestCase):
    def test_ret_emits_mov_to_a_then_ret(self):
        self.assertEqual(
            translate_instruction(tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=7)))),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ],
        )

    def test_ret_with_var_value(self):
        self.assertEqual(
            translate_instruction(tac_ast.Ret(val=tac_ast.Var(name="%3"))),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%3", offset=0), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ],
        )

    def test_ret_long_constant_loads_high_into_x_low_into_a(self):
        # 2-byte returns: high byte routed through A → X first (so A
        # is free for the low byte at the call point), then low byte
        # into A. save_a=True so the epilogue PHA/PLA preserves A.
        self.assertEqual(
            translate_instruction(
                tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstLong(int=0x1234)))
            ),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=0x12), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Imm(value=0x34), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
            ],
        )

    def test_ret_float_writes_hargs_8_through_11_no_save_a(self):
        # Float return: write 4 bytes of the IEEE 754 single bit
        # pattern into HARGS+8..11 (the same slot fadd/fsub/fmul/fdiv
        # write to). save_a=False because HARGS isn't clobbered by
        # SSP/FP arithmetic, so the epilogue skips PHA/PLA.
        # 1.5f → bit pattern 0x3FC00000, little-endian bytes
        # 00 00 C0 3F.
        from tac_to_asm import Translator
        from passes.type_checking import (
            LocalAttr, Symbol, SymbolTable,
        )
        import c99_ast
        symbols = SymbolTable()
        symbols["%f"] = Symbol(type=c99_ast.Float(), attrs=LocalAttr())
        t = Translator(symbols=symbols)
        self.assertEqual(
            t.translate_instruction(tac_ast.Ret(val=tac_ast.Var(name="%f"))),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%f", offset=0), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=8)),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%f", offset=1), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=9)),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%f", offset=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=10)),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%f", offset=3), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=11)),
                asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=False),
            ],
        )

    def test_ret_double_writes_hargs_16_through_23_no_save_a(self):
        # Double return: 8 bytes into HARGS+16..23 (the dadd/dsub/
        # dmul/ddiv output slot). save_a=False as for Float.
        from tac_to_asm import Translator
        from passes.type_checking import (
            LocalAttr, Symbol, SymbolTable,
        )
        import c99_ast
        symbols = SymbolTable()
        symbols["%d"] = Symbol(type=c99_ast.Double(), attrs=LocalAttr())
        t = Translator(symbols=symbols)
        out = t.translate_instruction(tac_ast.Ret(val=tac_ast.Var(name="%d")))
        # Expect 8 byte-pairs (Pseudo→A, A→HARGS+16..23) followed by
        # Ret(save_a=False).
        expected: list = []
        for k in range(8):
            expected.append(asm_ast.Mov(
                src=asm_ast.Pseudo(name="%d", offset=k), dst=_REG_A,
            ))
            expected.append(asm_ast.Mov(
                src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=16 + k),
            ))
        expected.append(asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=False))
        self.assertEqual(out, expected)

    def test_function_call_captures_float_return_from_hargs_8_through_11(self):
        # Caller-side: after JSR, read the 4-byte Float return from
        # HARGS+8..11 into the dst pseudo, byte-by-byte through A.
        from tac_to_asm import Translator
        from passes.type_checking import (
            LocalAttr, Symbol, SymbolTable,
        )
        import c99_ast
        symbols = SymbolTable()
        symbols["%dst"] = Symbol(type=c99_ast.Float(), attrs=LocalAttr())
        t = Translator(symbols=symbols)
        out = t.translate_instruction(tac_ast.FunctionCall(
            name="ret_f", args=[], dst=tac_ast.Var(name="%dst"),
        ))
        # No args → no AllocateStack, no arg writes; just Call then
        # 4 read-pairs.
        expected: list = [asm_ast.Call(name="ret_f")]
        for k in range(4):
            expected.append(asm_ast.Mov(
                src=asm_ast.Data(name="HARGS", offset=8 + k), dst=_REG_A,
            ))
            expected.append(asm_ast.Mov(
                src=_REG_A, dst=asm_ast.Pseudo(name="%dst", offset=k),
            ))
        self.assertEqual(out, expected)

    def test_function_call_captures_double_return_from_hargs_16_through_23(self):
        from tac_to_asm import Translator
        from passes.type_checking import (
            LocalAttr, Symbol, SymbolTable,
        )
        import c99_ast
        symbols = SymbolTable()
        symbols["%dst"] = Symbol(type=c99_ast.Double(), attrs=LocalAttr())
        t = Translator(symbols=symbols)
        out = t.translate_instruction(tac_ast.FunctionCall(
            name="ret_d", args=[], dst=tac_ast.Var(name="%dst"),
        ))
        expected: list = [asm_ast.Call(name="ret_d")]
        for k in range(8):
            expected.append(asm_ast.Mov(
                src=asm_ast.Data(name="HARGS", offset=16 + k), dst=_REG_A,
            ))
            expected.append(asm_ast.Mov(
                src=_REG_A, dst=asm_ast.Pseudo(name="%dst", offset=k),
            ))
        self.assertEqual(out, expected)

    def test_unary_negate_lowered_to_atoms_around_a(self):
        # Mov(src, A) -> Xor(A, $FF, A) -> ClearCarry -> Add(1, A)
        # -> Mov(A, dst).
        instr = tac_ast.Unary(
            op=tac_ast.Negate(),
            src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                ),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0", offset=0)),
            ],
        )

    def test_unary_complement_lowered_to_xor(self):
        # Mov(src, A) -> Xor(A, $FF, A) -> Mov(A, dst).
        instr = tac_ast.Unary(
            op=tac_ast.Complement(),
            src=tac_ast.Var(name="%1"),
            dst=tac_ast.Var(name="%2"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%1", offset=0), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%2", offset=0)),
            ],
        )

    def test_unary_logical_not_lowered_inline(self):
        # Mov(src, A) -> Branch(EQ, true) -> Mov(0, A) -> Jump(end)
        # -> Label(true) -> Mov(1, A) -> Label(end) -> Mov(A, dst).
        # No Compare — LDA already set Z.
        instr = tac_ast.Unary(
            op=tac_ast.LogicalNot(),
            src=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.EQ(), target=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                asm_ast.Jump(target=".lnot_end@1"),
                asm_ast.Label(name=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Label(name=".lnot_end@1"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_add_lowered(self):
        # Mov(src1, A) -> ClearCarry -> Add(src2, A) -> Mov(A, dst).
        instr = tac_ast.Binary(
            op=tac_ast.Add(),
            src1=tac_ast.Constant(const=tac_ast.ConstInt(int=3)),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_REG_A),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_subtract_lowered(self):
        # Mov(src1, A) -> SetCarry -> Sub(src2, A) -> Mov(A, dst).
        instr = tac_ast.Binary(
            op=tac_ast.Subtract(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.SetCarry(),
                asm_ast.Sub(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_multiply_lowered_to_mul8_call(self):
        # 8-bit operands → mul8: src1 → HARGS+0, src2 → HARGS+1, Call,
        # then result low byte from HARGS+2 → dst (the high byte at
        # HARGS+3 is discarded, since int*int truncates to int).
        instr = tac_ast.Binary(
            op=tac_ast.Multiply(),
            src1=tac_ast.Constant(const=tac_ast.ConstInt(int=3)),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=0)),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=1)),
                asm_ast.Call(name="mul8"),
                asm_ast.Mov(src=asm_ast.Data(name="HARGS", offset=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_divide_lowered_to_divmod8_call(self):
        # divmod8: dividend → HARGS+0, divisor → HARGS+1, Call,
        # quotient (HARGS+2) → dst. Remainder at HARGS+3 is unused
        # for `/` (Modulo reads it; see below).
        instr = tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=0)),
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=1)),
                asm_ast.Call(name="divmod8"),
                asm_ast.Mov(src=asm_ast.Data(name="HARGS", offset=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_bitwise_and_lowered(self):
        # Mov(src1, A) -> And(src2, A) -> Mov(A, dst). No carry setup
        # because AND doesn't touch carry.
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseAnd(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(const=tac_ast.ConstInt(int=15)),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_bitwise_or_lowered(self):
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseOr(),
            src1=tac_ast.Constant(const=tac_ast.ConstInt(int=-16)),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=0xF0), dst=_REG_A),
                asm_ast.Or(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_bitwise_xor_lowered(self):
        # Reuses the existing ternary Xor shape. The src1 of the asm
        # Xor is Reg(A); the src2 carries the addressing mode.
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseXor(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Var(name="%1"),
            dst=tac_ast.Var(name="%2"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A,
                    src2=asm_ast.Pseudo(name="%1", offset=0),
                    dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%2", offset=0)),
            ],
        )

    def test_binary_left_shift_lowered_to_asl8_call(self):
        # asl8: value → HARGS+0, count → HARGS+1, Call, result
        # (HARGS+2) → dst.
        instr = tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=0)),
                asm_ast.Mov(src=asm_ast.Imm(value=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=1)),
                asm_ast.Call(name="asl8"),
                asm_ast.Mov(src=asm_ast.Data(name="HARGS", offset=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1", offset=0)),
            ],
        )

    def test_binary_right_shift_lowered_to_asr8_call(self):
        # `>>` is arithmetic — c6502 currently treats every integer
        # as signed for shift purposes, so it goes through asr8
        # (sign-preserving) rather than a logical-right-shift helper.
        instr = tac_ast.Binary(
            op=tac_ast.RightShift(),
            src1=tac_ast.Constant(const=tac_ast.ConstInt(int=64)),
            src2=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=64), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=0)),
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=1)),
                asm_ast.Call(name="asr8"),
                asm_ast.Mov(src=asm_ast.Data(name="HARGS", offset=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0", offset=0)),
            ],
        )

    def test_binary_modulo_lowered_to_divmod8_remainder(self):
        # Same input layout as Divide; the remainder lives at the
        # slot pair after the quotient (HARGS+3 for divmod8).
        instr = tac_ast.Binary(
            op=tac_ast.Modulo(),
            src1=tac_ast.Constant(const=tac_ast.ConstInt(int=17)),
            src2=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=17), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=0)),
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Data(name="HARGS", offset=1)),
                asm_ast.Call(name="divmod8"),
                asm_ast.Mov(src=asm_ast.Data(name="HARGS", offset=3), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0", offset=0)),
            ],
        )


class TestTranslateShortCircuitAtoms(unittest.TestCase):
    """Copy/Jump/Label/JumpIfTrue/JumpIfFalse are the TAC atoms that
    c99_to_tac emits for `&&` and `||`. Copy becomes a single Mov (the
    emitter already handles every legal operand shape). Jump and Label
    are atom-for-atom. Conditional jumps stage the value through A so
    the LDA's Z flag drives a BEQ/BNE to the target."""

    def test_copy_constant_to_var_becomes_single_mov(self):
        self.assertEqual(
            translate_instruction(tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=0)),
                dst=tac_ast.Var(name="%0"),
            )),
            [asm_ast.Mov(
                src=asm_ast.Imm(value=0), dst=asm_ast.Pseudo(name="%0", offset=0),
            )],
        )

    def test_copy_var_to_var_becomes_single_mov(self):
        # Emit handles Frame->Frame via an internal load-then-store
        # pair, so tac_to_asm doesn't need to split it here.
        self.assertEqual(
            translate_instruction(tac_ast.Copy(
                src=tac_ast.Var(name="%a"),
                dst=tac_ast.Var(name="%b"),
            )),
            [asm_ast.Mov(
                src=asm_ast.Pseudo(name="%a", offset=0),
                dst=asm_ast.Pseudo(name="%b", offset=0),
            )],
        )

    def test_jump_is_atom_for_atom(self):
        self.assertEqual(
            translate_instruction(tac_ast.Jump(target=".and_end@0")),
            [asm_ast.Jump(target=".and_end@0")],
        )

    def test_label_is_atom_for_atom(self):
        self.assertEqual(
            translate_instruction(tac_ast.Label(name=".or_true@3")),
            [asm_ast.Label(name=".or_true@3")],
        )

    def test_jump_if_true_constant_stages_through_a_then_bne(self):
        self.assertEqual(
            translate_instruction(tac_ast.JumpIfTrue(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".or_true@0",
            )),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.NE(), target=".or_true@0"),
            ],
        )

    def test_jump_if_true_var_stages_through_a_then_bne(self):
        self.assertEqual(
            translate_instruction(tac_ast.JumpIfTrue(
                condition=tac_ast.Var(name="%0"),
                target=".or_true@0",
            )),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.NE(), target=".or_true@0"),
            ],
        )

    def test_jump_if_false_constant_stages_through_a_then_beq(self):
        self.assertEqual(
            translate_instruction(tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=0)),
                target=".and_false@0",
            )),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.EQ(), target=".and_false@0"),
            ],
        )

    def test_jump_if_false_var_stages_through_a_then_beq(self):
        self.assertEqual(
            translate_instruction(tac_ast.JumpIfFalse(
                condition=tac_ast.Var(name="%2"),
                target=".and_false@0",
            )),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%2", offset=0), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.EQ(), target=".and_false@0"),
            ],
        )

    def test_full_logical_and_lowering(self):
        # What c99_to_tac emits for `1 && 2`, lowered instruction by
        # instruction through translate_function. Verifies that the
        # five short-circuit atoms compose with the existing Ret
        # lowering into a coherent asm sequence.
        fn = tac_ast.Function(
            name="main",
            is_global=True,
            instructions=[
                tac_ast.JumpIfFalse(
                    condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    target=".and_false@0",
                ),
                tac_ast.JumpIfFalse(
                    condition=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                    target=".and_false@0",
                ),
                tac_ast.Copy(
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Jump(target=".and_end@1"),
                tac_ast.Label(name=".and_false@0"),
                tac_ast.Copy(
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=0)),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Label(name=".and_end@1"),
                tac_ast.Ret(val=tac_ast.Var(name="%0")),
            ],
        )
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(
                name="main",
                is_global=True, instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Branch(
                        cond=asm_ast.EQ(), target=".and_false@0",
                    ),
                    asm_ast.Mov(src=asm_ast.Imm(value=2), dst=_REG_A),
                    asm_ast.Branch(
                        cond=asm_ast.EQ(), target=".and_false@0",
                    ),
                    asm_ast.Mov(
                        src=asm_ast.Imm(value=1),
                        dst=asm_ast.Pseudo(name="%0", offset=0),
                    ),
                    asm_ast.Jump(target=".and_end@1"),
                    asm_ast.Label(name=".and_false@0"),
                    asm_ast.Mov(
                        src=asm_ast.Imm(value=0),
                        dst=asm_ast.Pseudo(name="%0", offset=0),
                    ),
                    asm_ast.Label(name=".and_end@1"),
                    asm_ast.Mov(
                        src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A,
                    ),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
                ],
            ),
        )


class TestTranslateComparisons(unittest.TestCase):
    """== / != lower to Compare + Branch(EQ|NE) + 0/1 select. The four
    signed ordering operators lower to SBC with a V-flag correction
    (BVC skip; EOR #$80; skip:) and then Branch(MI|PL) + 0/1 select.
    `>` and `<=` swap operands rather than branching on a combined
    NE & PL (the EOR correction makes the Z flag unreliable)."""

    @staticmethod
    def _src1():
        return tac_ast.Var(name="%0")

    @staticmethod
    def _src2():
        return tac_ast.Constant(const=tac_ast.ConstInt(int=5))

    @staticmethod
    def _dst():
        return tac_ast.Var(name="%1")

    @staticmethod
    def _src1_op():
        return asm_ast.Pseudo(name="%0", offset=0)

    @staticmethod
    def _src2_op():
        return asm_ast.Imm(value=5)

    @staticmethod
    def _dst_op():
        return asm_ast.Pseudo(name="%1", offset=0)

    def _instr(self, op):
        return tac_ast.Binary(
            op=op, src1=self._src1(), src2=self._src2(), dst=self._dst(),
        )

    def _equality_expected(self, cond):
        return [
            asm_ast.Mov(src=self._src1_op(), dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=self._src2_op()),
            asm_ast.Branch(cond=cond, target=".cmp_true@0"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=".cmp_end@1"),
            asm_ast.Label(name=".cmp_true@0"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=".cmp_end@1"),
            asm_ast.Mov(src=_REG_A, dst=self._dst_op()),
        ]

    def _signed_ordering_expected(self, left_op, right_op, cond):
        return [
            asm_ast.Mov(src=left_op, dst=_REG_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=right_op, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.VC(), target=".cmp_novf@0"),
            asm_ast.Xor(
                src1=_REG_A, src2=asm_ast.Imm(value=0x80), dst=_REG_A,
            ),
            asm_ast.Label(name=".cmp_novf@0"),
            asm_ast.Branch(cond=cond, target=".cmp_true@1"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=".cmp_end@2"),
            asm_ast.Label(name=".cmp_true@1"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=".cmp_end@2"),
            asm_ast.Mov(src=_REG_A, dst=self._dst_op()),
        ]

    def test_equal_uses_compare_and_beq(self):
        self.assertEqual(
            translate_instruction(self._instr(tac_ast.Equal())),
            self._equality_expected(asm_ast.EQ()),
        )

    def test_not_equal_uses_compare_and_bne(self):
        self.assertEqual(
            translate_instruction(self._instr(tac_ast.NotEqual())),
            self._equality_expected(asm_ast.NE()),
        )

    def test_less_than_uses_sbc_and_bmi_no_swap(self):
        # src1 < src2 signed: compute src1 - src2, branch on MI.
        self.assertEqual(
            translate_instruction(self._instr(tac_ast.LessThan())),
            self._signed_ordering_expected(
                self._src1_op(), self._src2_op(), asm_ast.MI(),
            ),
        )

    def test_greater_or_equal_uses_sbc_and_bpl_no_swap(self):
        # src1 >= src2 signed: compute src1 - src2, branch on PL.
        self.assertEqual(
            translate_instruction(self._instr(tac_ast.GreaterOrEqual())),
            self._signed_ordering_expected(
                self._src1_op(), self._src2_op(), asm_ast.PL(),
            ),
        )

    def test_greater_than_swaps_and_uses_bmi(self):
        # src1 > src2 signed <=> src2 < src1 signed. Swap so left=src2,
        # right=src1, then branch on MI.
        self.assertEqual(
            translate_instruction(self._instr(tac_ast.GreaterThan())),
            self._signed_ordering_expected(
                self._src2_op(), self._src1_op(), asm_ast.MI(),
            ),
        )

    def test_less_or_equal_swaps_and_uses_bpl(self):
        # src1 <= src2 signed <=> src2 >= src1 signed. Swap so left=src2,
        # right=src1, then branch on PL.
        self.assertEqual(
            translate_instruction(self._instr(tac_ast.LessOrEqual())),
            self._signed_ordering_expected(
                self._src2_op(), self._src1_op(), asm_ast.PL(),
            ),
        )

    def test_labels_are_unique_across_compares_in_one_translator(self):
        # When the Translator is reused (as it is within a program), the
        # label counter keeps advancing so two compares get disjoint
        # labels instead of colliding.
        from tac_to_asm import Translator
        t = Translator()
        first = t.translate_binary(
            tac_ast.Equal(), self._src1(), self._src2(), self._dst(),
        )
        second = t.translate_binary(
            tac_ast.Equal(), self._src1(), self._src2(), self._dst(),
        )
        first_labels = {
            i.name for i in first if isinstance(i, asm_ast.Label)
        }
        second_labels = {
            i.name for i in second if isinstance(i, asm_ast.Label)
        }
        self.assertTrue(first_labels.isdisjoint(second_labels))


class TestTranslatePointerOrdering(unittest.TestCase):
    """Ordering ops on Pointer-typed operands dispatch to the
    unsigned-ordering lowering: per-byte SBC with carry threading,
    then BCC/BCS (no V-correction). Same operand-swap trick as the
    signed form for `>` / `<=`."""

    @staticmethod
    def _setup():
        from tac_to_asm import Translator
        from passes.type_checking import (
            LocalAttr, Symbol, SymbolTable,
        )
        import c99_ast
        symbols = SymbolTable()
        ptr_int = c99_ast.Pointer(referenced_type=c99_ast.Int())
        symbols["%p"] = Symbol(type=ptr_int, attrs=LocalAttr())
        symbols["%q"] = Symbol(type=ptr_int, attrs=LocalAttr())
        symbols["%r"] = Symbol(type=c99_ast.Int(), attrs=LocalAttr())
        return Translator(symbols=symbols)

    @staticmethod
    def _src1():
        return tac_ast.Var(name="%p")

    @staticmethod
    def _src2():
        return tac_ast.Var(name="%q")

    @staticmethod
    def _dst():
        return tac_ast.Var(name="%r")

    @staticmethod
    def _byte(name, k):
        return asm_ast.Pseudo(name=name, offset=k)

    def _expected(self, left_name, right_name, cond):
        # Two-byte SBC pair (carry threads), then BCC/BCS + 0/1
        # select. No V-correction (no .cmp_novf label).
        return [
            asm_ast.Mov(src=self._byte(left_name, 0), dst=_REG_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=self._byte(right_name, 0), dst=_REG_A),
            asm_ast.Mov(src=self._byte(left_name, 1), dst=_REG_A),
            asm_ast.Sub(src=self._byte(right_name, 1), dst=_REG_A),
            asm_ast.Branch(cond=cond, target=".cmp_true@0"),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=".cmp_end@1"),
            asm_ast.Label(name=".cmp_true@0"),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=".cmp_end@1"),
            asm_ast.Mov(src=_REG_A, dst=self._byte("%r", 0)),
        ]

    def test_pointer_less_than_uses_bcc_no_swap(self):
        # p < q unsigned: compute p - q, branch on CC (no borrow
        # = false; borrow = true → BCC takes branch when borrow).
        t = self._setup()
        self.assertEqual(
            t.translate_binary(
                tac_ast.LessThan(), self._src1(), self._src2(), self._dst(),
            ),
            self._expected("%p", "%q", asm_ast.CC()),
        )

    def test_pointer_greater_or_equal_uses_bcs_no_swap(self):
        # p >= q unsigned: compute p - q, branch on CS (no borrow).
        t = self._setup()
        self.assertEqual(
            t.translate_binary(
                tac_ast.GreaterOrEqual(),
                self._src1(), self._src2(), self._dst(),
            ),
            self._expected("%p", "%q", asm_ast.CS()),
        )

    def test_pointer_greater_than_swaps_and_uses_bcc(self):
        # p > q <=> q < p. Swap so left=%q, right=%p; BCC.
        t = self._setup()
        self.assertEqual(
            t.translate_binary(
                tac_ast.GreaterThan(),
                self._src1(), self._src2(), self._dst(),
            ),
            self._expected("%q", "%p", asm_ast.CC()),
        )

    def test_pointer_less_or_equal_swaps_and_uses_bcs(self):
        # p <= q <=> q >= p. Swap so left=%q, right=%p; BCS.
        t = self._setup()
        self.assertEqual(
            t.translate_binary(
                tac_ast.LessOrEqual(),
                self._src1(), self._src2(), self._dst(),
            ),
            self._expected("%q", "%p", asm_ast.CS()),
        )

    def test_long_ordering_still_signed(self):
        # Sanity check: Long (non-pointer) operands stick with the
        # signed-ordering lowering — V-correction is present.
        from tac_to_asm import Translator
        from passes.type_checking import (
            LocalAttr, Symbol, SymbolTable,
        )
        import c99_ast
        symbols = SymbolTable()
        symbols["%a"] = Symbol(type=c99_ast.Long(), attrs=LocalAttr())
        symbols["%b"] = Symbol(type=c99_ast.Long(), attrs=LocalAttr())
        symbols["%r"] = Symbol(type=c99_ast.Int(), attrs=LocalAttr())
        t = Translator(symbols=symbols)
        out = t.translate_binary(
            tac_ast.LessThan(),
            tac_ast.Var(name="%a"),
            tac_ast.Var(name="%b"),
            tac_ast.Var(name="%r"),
        )
        # The V-correction labels distinguish signed from unsigned.
        labels = {i.name for i in out if isinstance(i, asm_ast.Label)}
        self.assertTrue(any(name.startswith(".cmp_novf@") for name in labels))


class TestTranslateFunction(unittest.TestCase):
    def test_flattens_instructions(self):
        fn = tac_ast.Function(
            name="main",
            is_global=True,
            instructions=[
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%0")),
            ],
        )
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(
                name="main",
                is_global=True, instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Xor(
                        src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0", offset=0)),
                    asm_ast.Mov(src=asm_ast.Pseudo(name="%0", offset=0), dst=_REG_A),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
                ],
            ),
        )

    def test_empty_function(self):
        fn = tac_ast.Function(name="main", is_global=True, instructions=[])
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(name="main", is_global=True, instructions=[]),
        )


class TestTranslateFunctionCall(unittest.TestCase):
    """TAC `FunctionCall(name, args, dst)` lowers to a 3-step
    sequence per the soft-stack convention: AllocateStack(N) to make
    room for args, one Mov per arg into Stack(1)..Stack(N), JSR to
    the callee, and Mov(Reg(A), dst) to capture the return value
    into the call's destination temp."""

    def test_no_args(self):
        # `f()` lowers to: just the JSR plus the return-value
        # capture. No AllocateStack (N=0), no arg writes.
        instrs = translate_instruction(tac_ast.FunctionCall(
            name="f", args=[],
            dst=tac_ast.Var(name="@0.t"),
        ))
        self.assertEqual(instrs, [
            asm_ast.Call(name="f"),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="@0.t", offset=0)),
        ])

    def test_constant_arg(self):
        # `f(42)` — the constant arg gets written directly to
        # Stack(1) via Mov(Imm, Stack), one asm instruction (the
        # emitter handles Imm→Stack as LDA imm + LDY off + STA).
        instrs = translate_instruction(tac_ast.FunctionCall(
            name="f",
            args=[tac_ast.Constant(const=tac_ast.ConstInt(int=42))],
            dst=tac_ast.Var(name="@0.t"),
        ))
        self.assertEqual(instrs, [
            asm_ast.AllocateStack(bytes=1),
            asm_ast.Mov(
                src=asm_ast.Imm(value=42),
                dst=asm_ast.Stack(offset=1),
            ),
            asm_ast.Call(name="f"),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="@0.t", offset=0)),
        ])

    def test_var_arg_uses_pseudo(self):
        # `f(x)` where x is a TAC Var — the arg val translates to
        # Pseudo(x), which after the frame-layout pass becomes a
        # Frame operand. The emitter then handles Frame→Stack as
        # an indirect-Y load + indirect-Y store.
        instrs = translate_instruction(tac_ast.FunctionCall(
            name="f",
            args=[tac_ast.Var(name="@0.x")],
            dst=tac_ast.Var(name="@1.t"),
        ))
        self.assertEqual(instrs, [
            asm_ast.AllocateStack(bytes=1),
            asm_ast.Mov(
                src=asm_ast.Pseudo(name="@0.x", offset=0),
                dst=asm_ast.Stack(offset=1),
            ),
            asm_ast.Call(name="f"),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="@1.t", offset=0)),
        ])

    def test_multiple_args_get_stack_offsets_1_to_n(self):
        # `f(a, b, c)` — args land at Stack(1), Stack(2), Stack(3)
        # in source order. After the callee sets up its frame, those
        # same bytes become Frame(M+3), Frame(M+4), Frame(M+5) on
        # the callee side.
        instrs = translate_instruction(tac_ast.FunctionCall(
            name="f",
            args=[
                tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                tac_ast.Constant(const=tac_ast.ConstInt(int=3)),
            ],
            dst=tac_ast.Var(name="@0.t"),
        ))
        self.assertEqual(instrs, [
            asm_ast.AllocateStack(bytes=3),
            asm_ast.Mov(
                src=asm_ast.Imm(value=1),
                dst=asm_ast.Stack(offset=1),
            ),
            asm_ast.Mov(
                src=asm_ast.Imm(value=2),
                dst=asm_ast.Stack(offset=2),
            ),
            asm_ast.Mov(
                src=asm_ast.Imm(value=3),
                dst=asm_ast.Stack(offset=3),
            ),
            asm_ast.Call(name="f"),
            asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="@0.t", offset=0)),
        ])


class TestTranslateProgram(unittest.TestCase):
    def test_full_tree(self):
        # Both sides plural: a one-function TAC program lowers to a
        # one-function asm program. Param lists ride through.
        prog = tac_ast.Program(
            top_level=[tac_ast.Function(
                name="main",
                is_global=True,
                params=[],
                instructions=[tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=42)))],
            )],
        )
        expected = asm_ast.Program(
            top_level=[asm_ast.Function(
                name="main",
                is_global=True, params=[],
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_REG_A),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0, save_a=True),
                ],
            )],
        )
        self.assertEqual(translate_program(prog), expected)


class TestErrors(unittest.TestCase):
    def test_unknown_val_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_val,), {})
        with self.assertRaises(TypeError):
            translate_val(stub())

    def test_unknown_instruction_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_instruction,), {})
        with self.assertRaises(TypeError):
            translate_instruction(stub())

    def test_unknown_unop_raises_type_error(self):
        stub = type("Stub", (tac_ast.Type_unary_operator,), {})
        with self.assertRaises(TypeError):
            translate_unop_atoms(stub())


class TestSignExtendAndTruncate(unittest.TestCase):
    """SignExtend lowers to an inline byte sequence: load the source
    byte (which sets N based on its sign), store it as the low byte
    of dst (STA preserves flags), then branch on the original N flag
    to write 0x00 / 0xFF to the high byte. Truncate lowers to a
    single byte Mov from the source's low byte (the high byte is
    discarded — memory is little-endian, so the source's offset-0
    byte is the low byte)."""

    def test_sign_extend_lowers_to_inline_byte_sequence(self):
        out = translate_instruction(tac_ast.SignExtend(
            src=tac_ast.Var(name="@0.x"),
            dst=tac_ast.Var(name="%0"),
        ))
        self.assertEqual(out, [
            asm_ast.Mov(
                src=asm_ast.Pseudo(name="@0.x", offset=0),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.Pseudo(name="%0", offset=0),
            ),
            asm_ast.Branch(cond=asm_ast.MI(), target=".sx_neg@0"),
            asm_ast.Mov(
                src=asm_ast.Imm(value=0x00),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Jump(target=".sx_done@1"),
            asm_ast.Label(name=".sx_neg@0"),
            asm_ast.Mov(
                src=asm_ast.Imm(value=0xFF),
                dst=asm_ast.Reg(reg=asm_ast.A()),
            ),
            asm_ast.Label(name=".sx_done@1"),
            asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()),
                dst=asm_ast.Pseudo(name="%0", offset=1),
            ),
        ])

    def test_truncate_lowers_to_byte_mov(self):
        # Memory layout is little-endian, so the source's address
        # already points at the low byte — a single byte Mov
        # transfers exactly that and discards the high byte.
        out = translate_instruction(tac_ast.Truncate(
            src=tac_ast.Var(name="@0.x"),
            dst=tac_ast.Var(name="%0"),
        ))
        self.assertEqual(out, [asm_ast.Mov(
            src=asm_ast.Pseudo(name="@0.x", offset=0),
            dst=asm_ast.Pseudo(name="%0", offset=0),
        )])


class TestStaticVariableTranslation(unittest.TestCase):
    """TAC `StaticVariable` and asm `StaticVariable` both carry a
    typed `IntInit` / `LongInit` wrapper; the translation is a pure
    rewrap, with the variant preserved end-to-end so asm_emit can
    pick the right cell width (`DC.B` vs `DC.W`) at the final
    emit."""

    def test_int_init_passes_through(self):
        prog = tac_ast.Program(top_level=[
            tac_ast.StaticVariable(
                name="g", is_global=True,
                data_type=tac_ast.Int(),
                init=tac_ast.IntInit(int=42),
            ),
        ])
        out = translate_program(prog)
        self.assertEqual(out.top_level, [
            asm_ast.StaticVariable(
                name="g", is_global=True,
                init=asm_ast.IntInit(int=42),
            ),
        ])

    def test_long_init_passes_through_as_long_init(self):
        prog = tac_ast.Program(top_level=[
            tac_ast.StaticVariable(
                name="g", is_global=False,
                data_type=tac_ast.Long(),
                init=tac_ast.LongInit(int=200),
            ),
        ])
        out = translate_program(prog)
        self.assertEqual(out.top_level, [
            asm_ast.StaticVariable(
                name="g", is_global=False,
                init=asm_ast.LongInit(int=200),
            ),
        ])


if __name__ == "__main__":
    unittest.main()
