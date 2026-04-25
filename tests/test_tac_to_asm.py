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
            translate_val(tac_ast.Constant(value=42)),
            asm_ast.Imm(value=42),
        )

    def test_var_becomes_pseudo(self):
        self.assertEqual(
            translate_val(tac_ast.Var(name="%0")),
            asm_ast.Pseudo(name="%0"),
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
            translate_instruction(tac_ast.Ret(val=tac_ast.Constant(value=7))),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=7), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ],
        )

    def test_ret_with_var_value(self):
        self.assertEqual(
            translate_instruction(tac_ast.Ret(val=tac_ast.Var(name="%3"))),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%3"), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ],
        )

    def test_unary_negate_lowered_to_atoms_around_a(self):
        # Mov(src, A) -> Xor(A, $FF, A) -> ClearCarry -> Add(1, A)
        # -> Mov(A, dst).
        instr = tac_ast.Unary(
            op=tac_ast.Negate(),
            src=tac_ast.Constant(value=5),
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
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
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
                asm_ast.Mov(src=asm_ast.Pseudo(name="%1"), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%2")),
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
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.EQ(), target=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                asm_ast.Jump(target=".lnot_end@1"),
                asm_ast.Label(name=".lnot_true@0"),
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Label(name=".lnot_end@1"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_add_lowered(self):
        # Mov(src1, A) -> ClearCarry -> Add(src2, A) -> Mov(A, dst).
        instr = tac_ast.Binary(
            op=tac_ast.Add(),
            src1=tac_ast.Constant(value=3),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_REG_A),
                asm_ast.ClearCarry(),
                asm_ast.Add(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_subtract_lowered(self):
        # Mov(src1, A) -> SetCarry -> Sub(src2, A) -> Mov(A, dst).
        instr = tac_ast.Binary(
            op=tac_ast.Subtract(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.SetCarry(),
                asm_ast.Sub(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_multiply_lowered_to_mul8_call(self):
        # mul8 takes A * X; staged as: src2 -> A -> X, src1 -> A,
        # Call mul8, Mov A -> dst. Result low byte is in A.
        instr = tac_ast.Binary(
            op=tac_ast.Multiply(),
            src1=tac_ast.Constant(value=3),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Imm(value=3), dst=_REG_A),
                asm_ast.Call(name="mul8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_divide_lowered_to_divmod8_call(self):
        # divmod8 takes A (dividend) / X (divisor) -> A=quot, X=rem.
        # Divide wants the quotient, which is already in A.
        instr = tac_ast.Binary(
            op=tac_ast.Divide(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Call(name="divmod8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_bitwise_and_lowered(self):
        # Mov(src1, A) -> And(src2, A) -> Mov(A, dst). No carry setup
        # because AND doesn't touch carry.
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseAnd(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=0x0F),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.And(src=asm_ast.Imm(value=0x0F), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_bitwise_or_lowered(self):
        instr = tac_ast.Binary(
            op=tac_ast.BitwiseOr(),
            src1=tac_ast.Constant(value=0xF0),
            src2=tac_ast.Var(name="%0"),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=0xF0), dst=_REG_A),
                asm_ast.Or(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
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
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Xor(
                    src1=_REG_A,
                    src2=asm_ast.Pseudo(name="%1"),
                    dst=_REG_A,
                ),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%2")),
            ],
        )

    def test_binary_left_shift_lowered_to_shl8_call(self):
        # shl8 takes A << X (logical), result in A.
        instr = tac_ast.Binary(
            op=tac_ast.LeftShift(),
            src1=tac_ast.Var(name="%0"),
            src2=tac_ast.Constant(value=2),
            dst=tac_ast.Var(name="%1"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=2), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Call(name="shl8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%1")),
            ],
        )

    def test_binary_right_shift_lowered_to_asr8_call(self):
        # Right shift is arithmetic — c6502 currently treats integers
        # as signed, so >> goes through asr8 (sign-preserving) rather
        # than a logical shift helper.
        instr = tac_ast.Binary(
            op=tac_ast.RightShift(),
            src1=tac_ast.Constant(value=0x80),
            src2=tac_ast.Constant(value=1),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Imm(value=0x80), dst=_REG_A),
                asm_ast.Call(name="asr8"),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
            ],
        )

    def test_binary_modulo_lowered_to_divmod8_call_with_x_to_a(self):
        # Modulo wants the remainder from divmod8, which comes back in
        # X — we shuffle X to A before storing.
        instr = tac_ast.Binary(
            op=tac_ast.Modulo(),
            src1=tac_ast.Constant(value=17),
            src2=tac_ast.Constant(value=5),
            dst=tac_ast.Var(name="%0"),
        )
        self.assertEqual(
            translate_instruction(instr),
            [
                asm_ast.Mov(src=asm_ast.Imm(value=5), dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=_REG_X),
                asm_ast.Mov(src=asm_ast.Imm(value=17), dst=_REG_A),
                asm_ast.Call(name="divmod8"),
                asm_ast.Mov(src=_REG_X, dst=_REG_A),
                asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
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
                src=tac_ast.Constant(value=0),
                dst=tac_ast.Var(name="%0"),
            )),
            [asm_ast.Mov(
                src=asm_ast.Imm(value=0), dst=asm_ast.Pseudo(name="%0"),
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
                src=asm_ast.Pseudo(name="%a"),
                dst=asm_ast.Pseudo(name="%b"),
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
                condition=tac_ast.Constant(value=1),
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
                asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                asm_ast.Branch(cond=asm_ast.NE(), target=".or_true@0"),
            ],
        )

    def test_jump_if_false_constant_stages_through_a_then_beq(self):
        self.assertEqual(
            translate_instruction(tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=0),
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
                asm_ast.Mov(src=asm_ast.Pseudo(name="%2"), dst=_REG_A),
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
            instructions=[
                tac_ast.JumpIfFalse(
                    condition=tac_ast.Constant(value=1),
                    target=".and_false@0",
                ),
                tac_ast.JumpIfFalse(
                    condition=tac_ast.Constant(value=2),
                    target=".and_false@0",
                ),
                tac_ast.Copy(
                    src=tac_ast.Constant(value=1),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Jump(target=".and_end@1"),
                tac_ast.Label(name=".and_false@0"),
                tac_ast.Copy(
                    src=tac_ast.Constant(value=0),
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
                instructions=[
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
                        dst=asm_ast.Pseudo(name="%0"),
                    ),
                    asm_ast.Jump(target=".and_end@1"),
                    asm_ast.Label(name=".and_false@0"),
                    asm_ast.Mov(
                        src=asm_ast.Imm(value=0),
                        dst=asm_ast.Pseudo(name="%0"),
                    ),
                    asm_ast.Label(name=".and_end@1"),
                    asm_ast.Mov(
                        src=asm_ast.Pseudo(name="%0"), dst=_REG_A,
                    ),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
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
        return tac_ast.Constant(value=5)

    @staticmethod
    def _dst():
        return tac_ast.Var(name="%1")

    @staticmethod
    def _src1_op():
        return asm_ast.Pseudo(name="%0")

    @staticmethod
    def _src2_op():
        return asm_ast.Imm(value=5)

    @staticmethod
    def _dst_op():
        return asm_ast.Pseudo(name="%1")

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


class TestTranslateFunction(unittest.TestCase):
    def test_flattens_instructions(self):
        fn = tac_ast.Function(
            name="main",
            instructions=[
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Constant(value=1),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%0")),
            ],
        )
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(
                name="main",
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Xor(
                        src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=asm_ast.Pseudo(name="%0")),
                    asm_ast.Mov(src=asm_ast.Pseudo(name="%0"), dst=_REG_A),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ],
            ),
        )

    def test_empty_function(self):
        fn = tac_ast.Function(name="main", instructions=[])
        self.assertEqual(
            translate_function(fn),
            asm_ast.Function(name="main", instructions=[]),
        )


class TestTranslateProgram(unittest.TestCase):
    def test_full_tree(self):
        prog = tac_ast.Program(
            function_definition=tac_ast.Function(
                name="main",
                instructions=[tac_ast.Ret(val=tac_ast.Constant(value=42))],
            ),
        )
        expected = asm_ast.Program(
            function_definition=asm_ast.Function(
                name="main",
                instructions=[
                    asm_ast.Mov(src=asm_ast.Imm(value=42), dst=_REG_A),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ],
            ),
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


if __name__ == "__main__":
    unittest.main()
