"""Translate a tac_ast tree into an asm_ast tree.

The Translator class holds a label counter so it can mint unique
labels for the inline lowerings that need them: the six comparisons
(`== != < > <= >=`) and unary `!`. Module-level `translate_*`
functions construct a fresh Translator per call; use the class
directly when you want the counter to persist across calls.

Mapping:
  Program(fn)              -> Program(translate_function(fn))
  Function(name, instrs)   -> Function(name, flat-mapped instructions)
  Ret(val)                 -> [Mov(translate_val(val), Reg(A)),
                               Ret(arg_bytes=0, local_bytes=0)]
                              (allocate_stack fills in arg/local bytes)
  Unary(op, src, dst)      -> [Mov(translate_val(src), Reg(A)),
                               <atoms for op on A>,
                               Mov(Reg(A), translate_val(dst))]
                              The op is lowered to atomic instructions:
                                Complement -> Xor(A, Imm($FF), A)
                                Negate     -> Xor(A, Imm($FF), A);
                                              ClearCarry;
                                              Add(Imm(1), A)
                                LogicalNot -> Branch(EQ, true);
                                              Mov(0,A); Jump(end);
                                              true: Mov(1,A); end:
                                              (no extra Compare — the
                                              framing Mov(src,A)'s LDA
                                              already set Z.)
                              asm_ast has no Unary node anymore — it's
                              strictly a TAC concept.
  Binary(op, src1, src2, dst) -> see translate_binary below.
  Copy(src, dst)           -> [Mov(src, dst)]. The emitter's Mov
                              handles every legal operand shape so
                              there's nothing to expand here.
  Jump(target)             -> [Jump(target)]   (atom-for-atom)
  Label(name)              -> [Label(name)]    (atom-for-atom)
  JumpIfTrue(cond, target) -> [Mov(cond, A), Branch(NE, target)].
                              LDA sets Z based on the loaded byte, so
                              BNE fires exactly when cond is non-zero
                              (C's truthiness).
  JumpIfFalse(cond, target) -> [Mov(cond, A), Branch(EQ, target)]
                              (mirror of JumpIfTrue).

Binary lowerings:
  Add / Subtract                 [Mov(src1, A), ClearCarry|SetCarry,
                                  Add|Sub(src2, A), Mov(A, dst)]
  BitwiseAnd / BitwiseOr /       [Mov(src1, A), And|Or|Xor(...), Mov(A, dst)]
    BitwiseXor                   No carry setup; AND/ORA/EOR don't touch C.
  Multiply / Divide / Modulo /   [Mov(src2, A), Mov(A, X), Mov(src1, A),
    LeftShift / RightShift       Call(<helper>), <result fetch>, Mov(A, dst)]
                                 Runtime helpers (A, X in; A out):
                                   mul8     A * X -> low in A, high in X
                                   divmod8  A / X -> quot in A, rem in X
                                   shl8     A << X (logical) -> A
                                   asr8     A >> X (arithmetic, signed) -> A
                                 Right shift uses asr8 because c6502
                                 currently treats integers as signed.
                                 Modulo pulls the remainder out of X via
                                 Mov(Reg(X), Reg(A)) before storing.
  Equal / NotEqual               Compare + Branch(EQ|NE) + 0/1 select:
                                   [Mov(src1, A),
                                    Compare(A, src2),
                                    Branch(EQ|NE, true_label),
                                    Mov(Imm(0), A),
                                    Jump(end_label),
                                    Label(true_label),
                                    Mov(Imm(1), A),
                                    Label(end_label),
                                    Mov(A, dst)]
                                 CMP doesn't touch V, but Z is reliable:
                                 Z=1 iff src1==src2 unconditionally.
  LessThan / GreaterOrEqual /    SBC with V-correction + Branch(MI|PL) +
    GreaterThan / LessOrEqual    0/1 select. CMP can't be used for signed
                                 ordering because it leaves V alone, and
                                 the N flag lies when signed subtraction
                                 overflows (e.g., +1 < -128 would be
                                 flagged wrongly). The canonical idiom:
                                   [Mov(L, A),
                                    SetCarry,
                                    Sub(R, A),          ; A = L - R,
                                                        ; sets N, Z, C, V
                                    Branch(VC, novf),
                                    Xor(A, Imm($80), A),; flip N when
                                                        ; overflow made it lie
                                    Label(novf),
                                    Branch(MI|PL, true_label),
                                    ... 0/1 select ...]
                                 For `<` and `>=` we compute src1-src2 and
                                 branch on MI/PL respectively. For `>` and
                                 `<=` we swap operands and compute
                                 src2-src1, then branch on MI/PL — that
                                 sidesteps the fact that Z is unreliable
                                 after the EOR correction (EOR #$80 can
                                 create a spurious Z=1 when the pre-EOR
                                 result was $80).
  Constant(v)              -> Imm(v)
  Var(name)                -> Pseudo(name)
"""

from __future__ import annotations

import asm_ast
import tac_ast


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())

# Runtime helper names for the remaining multi-instruction ops. All
# take operands in A (and X for the binary ones). mul8 / divmod8
# return both halves of the result (A, X). shl8 / asr8 take a value
# in A and a shift count in X and return the shifted value in A.
# (The comparison helpers cmp_*8 and the unary-not helper lnot8 are
# gone — the translator lowers those inline now.)
_MUL8 = "mul8"
_DIVMOD8 = "divmod8"
_SHL8 = "shl8"
_ASR8 = "asr8"


class Translator:
    """Holds the label counter so inline lowerings in the same program
    (comparisons and unary `!`) get unique labels. One Translator per
    program; each `make_label` call bumps the counter."""

    def __init__(self) -> None:
        self._label_counter = 0

    def make_label(self, prefix: str) -> str:
        # Leading `.` makes this a dasm-style local label, scoped to
        # the enclosing SUBROUTINE. See c99_to_tac.Translator.make_label
        # for the `@` convention that keeps these disjoint from any
        # user-written name.
        name = f".{prefix}@{self._label_counter}"
        self._label_counter += 1
        return name

    def translate_program(
        self, prog: tac_ast.Type_program,
    ) -> asm_ast.Type_program:
        match prog:
            case tac_ast.Program(function_definition=fn):
                return asm_ast.Program(
                    function_definition=self.translate_function(fn),
                )
        raise TypeError(f"unexpected program node: {prog!r}")

    def translate_function(
        self, fn: tac_ast.Type_function_definition,
    ) -> asm_ast.Type_function_definition:
        match fn:
            case tac_ast.Function(name=name, instructions=instrs):
                out: list[asm_ast.Type_instruction] = []
                for instr in instrs:
                    out.extend(self.translate_instruction(instr))
                return asm_ast.Function(name=name, instructions=out)
        raise TypeError(f"unexpected function node: {fn!r}")

    def translate_instruction(
        self, instr: tac_ast.Type_instruction,
    ) -> list[asm_ast.Type_instruction]:
        match instr:
            case tac_ast.Ret(val=val):
                # arg_bytes/local_bytes are zeros here; the
                # allocate_stack pass rewrites them to the function's
                # actual N and M.
                return [
                    asm_ast.Mov(src=translate_val(val), dst=_REG_A),
                    asm_ast.Ret(arg_bytes=0, local_bytes=0),
                ]
            case tac_ast.Unary(op=op, src=src, dst=dst):
                return (
                    [asm_ast.Mov(src=translate_val(src), dst=_REG_A)]
                    + self.translate_unop_atoms(op)
                    + [asm_ast.Mov(src=_REG_A, dst=translate_val(dst))]
                )
            case tac_ast.Binary(op=op, src1=src1, src2=src2, dst=dst):
                return self.translate_binary(op, src1, src2, dst)
            case tac_ast.Copy(src=src, dst=dst):
                # A single Mov — the emitter already handles every
                # legal operand shape (Imm→Frame routes through A
                # internally; Frame→Frame emits load-then-store).
                return [asm_ast.Mov(
                    src=translate_val(src), dst=translate_val(dst),
                )]
            case tac_ast.Jump(target=target):
                return [asm_ast.Jump(target=target)]
            case tac_ast.Label(name=name):
                return [asm_ast.Label(name=name)]
            case tac_ast.JumpIfTrue(condition=cond, target=target):
                # LDA (immediate or indirect-Y) sets Z based on the
                # loaded byte, so after the Mov, BNE branches exactly
                # when the condition is non-zero (C's truthiness).
                return [
                    asm_ast.Mov(src=translate_val(cond), dst=_REG_A),
                    asm_ast.Branch(cond=asm_ast.NE(), target=target),
                ]
            case tac_ast.JumpIfFalse(condition=cond, target=target):
                return [
                    asm_ast.Mov(src=translate_val(cond), dst=_REG_A),
                    asm_ast.Branch(cond=asm_ast.EQ(), target=target),
                ]
        raise TypeError(f"unexpected instruction node: {instr!r}")

    def translate_binary(
        self,
        op: tac_ast.Type_binary_operator,
        src1: tac_ast.Type_val,
        src2: tac_ast.Type_val,
        dst: tac_ast.Type_val,
    ) -> list[asm_ast.Type_instruction]:
        """Lower a TAC Binary into an asm sequence. Correctness first:
        each lowering emits a straightforward, unoptimized sequence.
        Optimization is deferred to TAC-level passes."""
        src1_op = translate_val(src1)
        src2_op = translate_val(src2)
        dst_op = translate_val(dst)
        match op:
            case tac_ast.Add():
                return [
                    asm_ast.Mov(src=src1_op, dst=_REG_A),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(src=src2_op, dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=dst_op),
                ]
            case tac_ast.Subtract():
                return [
                    asm_ast.Mov(src=src1_op, dst=_REG_A),
                    asm_ast.SetCarry(),
                    asm_ast.Sub(src=src2_op, dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=dst_op),
                ]
            case tac_ast.Multiply():
                return _translate_ax_call(src1_op, src2_op, dst_op, _MUL8,
                                          result_in_x=False)
            case tac_ast.Divide():
                return _translate_ax_call(src1_op, src2_op, dst_op, _DIVMOD8,
                                          result_in_x=False)
            case tac_ast.Modulo():
                return _translate_ax_call(src1_op, src2_op, dst_op, _DIVMOD8,
                                          result_in_x=True)
            case tac_ast.BitwiseAnd():
                return [
                    asm_ast.Mov(src=src1_op, dst=_REG_A),
                    asm_ast.And(src=src2_op, dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=dst_op),
                ]
            case tac_ast.BitwiseOr():
                return [
                    asm_ast.Mov(src=src1_op, dst=_REG_A),
                    asm_ast.Or(src=src2_op, dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=dst_op),
                ]
            case tac_ast.BitwiseXor():
                return [
                    asm_ast.Mov(src=src1_op, dst=_REG_A),
                    asm_ast.Xor(src1=_REG_A, src2=src2_op, dst=_REG_A),
                    asm_ast.Mov(src=_REG_A, dst=dst_op),
                ]
            case tac_ast.LeftShift():
                return _translate_ax_call(src1_op, src2_op, dst_op, _SHL8,
                                          result_in_x=False)
            case tac_ast.RightShift():
                return _translate_ax_call(src1_op, src2_op, dst_op, _ASR8,
                                          result_in_x=False)
            case tac_ast.Equal():
                return self._translate_equality(
                    src1_op, src2_op, dst_op, asm_ast.EQ(),
                )
            case tac_ast.NotEqual():
                return self._translate_equality(
                    src1_op, src2_op, dst_op, asm_ast.NE(),
                )
            case tac_ast.LessThan():
                # src1 < src2 signed: compute src1 - src2, branch on MI.
                return self._translate_signed_ordering(
                    src1_op, src2_op, dst_op, asm_ast.MI(),
                )
            case tac_ast.GreaterOrEqual():
                # src1 >= src2 signed: compute src1 - src2, branch on PL.
                return self._translate_signed_ordering(
                    src1_op, src2_op, dst_op, asm_ast.PL(),
                )
            case tac_ast.GreaterThan():
                # src1 > src2 signed <=> src2 < src1 signed.
                # Swap and reuse the MI-branch path. (Can't just do
                # src1-src2 and branch on NE & PL because Z is
                # unreliable after the EOR #$80 overflow correction.)
                return self._translate_signed_ordering(
                    src2_op, src1_op, dst_op, asm_ast.MI(),
                )
            case tac_ast.LessOrEqual():
                # src1 <= src2 signed <=> src2 >= src1 signed. Swap
                # and reuse the PL-branch path.
                return self._translate_signed_ordering(
                    src2_op, src1_op, dst_op, asm_ast.PL(),
                )
        raise TypeError(f"unexpected binary operator: {op!r}")

    def _translate_equality(
        self,
        src1_op: asm_ast.Type_operand,
        src2_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        cond: asm_ast.Type_condition,
    ) -> list[asm_ast.Type_instruction]:
        """Lower == / != via Compare + Branch(EQ|NE) + 0/1 select.
        CMP sets Z=1 iff src1==src2 unconditionally (no overflow
        concern), so it's correct for both signed and unsigned."""
        true_label = self.make_label("cmp_true")
        end_label = self.make_label("cmp_end")
        return [
            asm_ast.Mov(src=src1_op, dst=_REG_A),
            asm_ast.Compare(left=_REG_A, right=src2_op),
            asm_ast.Branch(cond=cond, target=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=end_label),
            asm_ast.Mov(src=_REG_A, dst=dst_op),
        ]

    def _translate_signed_ordering(
        self,
        left_op: asm_ast.Type_operand,
        right_op: asm_ast.Type_operand,
        dst_op: asm_ast.Type_operand,
        cond: asm_ast.Type_condition,
    ) -> list[asm_ast.Type_instruction]:
        """Lower a signed ordering compare via SBC with V-correction:
           LDA left; SEC; SBC right       ; sets N, Z, C, V
           BVC novf                       ; if no overflow, N is correct
           EOR #$80                       ; else flip N to correct it
         novf:
           B<cond> true                   ; MI for <, PL for >=
           LDA #0 ; JMP end ; true: LDA #1 ; end: STA dst

        Caller chooses who's on the left and which condition to branch
        on to select among <, >=, >, <= (see translate_binary)."""
        novf_label = self.make_label("cmp_novf")
        true_label = self.make_label("cmp_true")
        end_label = self.make_label("cmp_end")
        return [
            asm_ast.Mov(src=left_op, dst=_REG_A),
            asm_ast.SetCarry(),
            asm_ast.Sub(src=right_op, dst=_REG_A),
            asm_ast.Branch(cond=asm_ast.VC(), target=novf_label),
            asm_ast.Xor(
                src1=_REG_A, src2=asm_ast.Imm(value=0x80), dst=_REG_A,
            ),
            asm_ast.Label(name=novf_label),
            asm_ast.Branch(cond=cond, target=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
            asm_ast.Jump(target=end_label),
            asm_ast.Label(name=true_label),
            asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
            asm_ast.Label(name=end_label),
            asm_ast.Mov(src=_REG_A, dst=dst_op),
        ]

    def translate_unop_atoms(
        self, op: tac_ast.Type_unary_operator,
    ) -> list[asm_ast.Type_instruction]:
        """Atomic asm instructions implementing the unary op on A.
        Result is left in A. LogicalNot lowers inline (no runtime
        helper) and mints two labels per use, so this lives on the
        Translator to share the label counter with the comparison
        lowerings."""
        match op:
            case tac_ast.Complement():
                # ~A = A XOR $FF
                return [asm_ast.Xor(
                    src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                )]
            case tac_ast.Negate():
                # -A = (~A) + 1, two's complement
                return [
                    asm_ast.Xor(
                        src1=_REG_A, src2=asm_ast.Imm(value=0xFF), dst=_REG_A,
                    ),
                    asm_ast.ClearCarry(),
                    asm_ast.Add(src=asm_ast.Imm(value=1), dst=_REG_A),
                ]
            case tac_ast.LogicalNot():
                # !A := 1 if A == 0 else 0. The framing Mov(src, A)
                # around this atom already emits LDA, which sets Z
                # based on the loaded byte — so we can branch on EQ
                # directly without an extra Compare.
                true_label = self.make_label("lnot_true")
                end_label = self.make_label("lnot_end")
                return [
                    asm_ast.Branch(cond=asm_ast.EQ(), target=true_label),
                    asm_ast.Mov(src=asm_ast.Imm(value=0), dst=_REG_A),
                    asm_ast.Jump(target=end_label),
                    asm_ast.Label(name=true_label),
                    asm_ast.Mov(src=asm_ast.Imm(value=1), dst=_REG_A),
                    asm_ast.Label(name=end_label),
                ]
        raise TypeError(f"unexpected unary operator: {op!r}")


def _translate_ax_call(
    src1_op: asm_ast.Type_operand,
    src2_op: asm_ast.Type_operand,
    dst_op: asm_ast.Type_operand,
    helper: str,
    result_in_x: bool,
) -> list[asm_ast.Type_instruction]:
    """Lower a TAC op that delegates to a runtime helper taking A and X.
    src2 is staged through A (the only register the emitter can load
    from a Frame/Stack/Imm uniformly) into X, then src1 is loaded into
    A last so A holds the primary operand at the call. If the
    helper's result comes back in X (Modulo), transfer it to A before
    storing to dst."""
    out: list[asm_ast.Type_instruction] = [
        asm_ast.Mov(src=src2_op, dst=_REG_A),
        asm_ast.Mov(src=_REG_A, dst=_REG_X),
        asm_ast.Mov(src=src1_op, dst=_REG_A),
        asm_ast.Call(name=helper),
    ]
    if result_in_x:
        out.append(asm_ast.Mov(src=_REG_X, dst=_REG_A))
    out.append(asm_ast.Mov(src=_REG_A, dst=dst_op))
    return out


def translate_val(val: tac_ast.Type_val) -> asm_ast.Type_operand:
    match val:
        case tac_ast.Constant(value=v):
            return asm_ast.Imm(value=v)
        case tac_ast.Var(name=n):
            return asm_ast.Pseudo(name=n)
    raise TypeError(f"unexpected val node: {val!r}")


def translate_unop_atoms(
    op: tac_ast.Type_unary_operator,
) -> list[asm_ast.Type_instruction]:
    return Translator().translate_unop_atoms(op)


# Module-level wrappers: each call builds a fresh Translator (so the
# label counter restarts at 0). Use the Translator class directly when
# you need the counter to persist across calls.
def translate_program(prog: tac_ast.Type_program) -> asm_ast.Type_program:
    return Translator().translate_program(prog)


def translate_function(
    fn: tac_ast.Type_function_definition,
) -> asm_ast.Type_function_definition:
    return Translator().translate_function(fn)


def translate_instruction(
    instr: tac_ast.Type_instruction,
) -> list[asm_ast.Type_instruction]:
    return Translator().translate_instruction(instr)


def translate_binary(
    op: tac_ast.Type_binary_operator,
    src1: tac_ast.Type_val,
    src2: tac_ast.Type_val,
    dst: tac_ast.Type_val,
) -> list[asm_ast.Type_instruction]:
    return Translator().translate_binary(op, src1, src2, dst)
