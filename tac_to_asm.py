"""Translate a tac_ast tree into an asm_ast tree.

One `translate_*` function per source-AST node kind. Uses `match` for
dispatch so each case mirrors the constructor signature; adding a new
TAC AST variant is a matter of adding a new `case` clause.

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
                                LogicalNot -> Call(lnot8) (helper
                                              returns A=1 if A==0 else
                                              A=0)
                              asm_ast has no Unary node anymore — it's
                              strictly a TAC concept.
  Binary(op, src1, src2, dst) -> for Add and Subtract:
                                   [Mov(src1, Reg(A)),
                                    ClearCarry | SetCarry,
                                    Add|Sub(src2, Reg(A)),
                                    Mov(Reg(A), dst)]
                                 for BitwiseAnd / BitwiseOr / BitwiseXor:
                                   [Mov(src1, Reg(A)),
                                    And|Or(src2, Reg(A))
                                      | Xor(Reg(A), src2, Reg(A)),
                                    Mov(Reg(A), dst)]
                                 (no carry setup; AND/ORA/EOR don't
                                 touch carry). Xor keeps the older
                                 ternary shape so the unary-Complement
                                 lowering stays unchanged.
                                 for Multiply / Divide / Modulo /
                                     LeftShift / RightShift /
                                     Equal / NotEqual / LessThan /
                                     GreaterThan / LessOrEqual /
                                     GreaterOrEqual:
                                   [Mov(src2, Reg(A)),
                                    Mov(Reg(A), Reg(X)),
                                    Mov(src1, Reg(A)),
                                    Call(<helper>),
                                    <result fetch>,
                                    Mov(Reg(A), dst)]
                                 The runtime helpers take A and X:
                                   mul8     — A *= X, low byte in A,
                                              high byte in X.
                                   divmod8  — A /= X, quotient in A,
                                              remainder in X.
                                   shl8     — A << X (logical), in A.
                                   asr8     — A >> X (arithmetic /
                                              sign-preserving), in A.
                                   cmp_eq8 / cmp_ne8 / cmp_lt8 /
                                   cmp_gt8 / cmp_le8 / cmp_ge8 —
                                              return A=1 if the
                                              relation A <op> X holds,
                                              else A=0. Ordering
                                              helpers are signed.
                                 Right shift uses asr8 because c6502
                                 currently treats integers as signed
                                 (same applies to the < / > / <= / >=
                                 helpers).
                                 src2 is staged through A into X
                                 because the emitter has no direct
                                 Stack/Frame -> X mov. Multiply and
                                 Divide keep A as the result; Modulo
                                 pulls the remainder out of X via
                                 Mov(Reg(X), Reg(A)) before storing.
  Constant(v)              -> Imm(v)
  Var(name)                -> Pseudo(name)
"""

from __future__ import annotations

import asm_ast
import tac_ast


_REG_A = asm_ast.Reg(reg=asm_ast.A())
_REG_X = asm_ast.Reg(reg=asm_ast.X())

# Runtime helper names for the multi-instruction arithmetic, shift,
# comparison, and logical-NOT ops. All take their operands in A (and
# X for the binary ones); the runtime header (not in this repo yet)
# defines these labels. mul8 / divmod8 return both halves of the
# result (A, X); shl8 / asr8 take a value in A and a shift count in
# X and return the shifted value in A; the cmp_*8 helpers take A and
# X and return 1 in A if the comparison holds (signed for the
# ordering ones — c6502 currently treats every integer as signed),
# 0 otherwise; lnot8 takes A and returns 1 if A==0, else 0.
_MUL8 = "mul8"
_DIVMOD8 = "divmod8"
_SHL8 = "shl8"
_ASR8 = "asr8"
_CMP_EQ8 = "cmp_eq8"
_CMP_NE8 = "cmp_ne8"
_CMP_LT8 = "cmp_lt8"
_CMP_GT8 = "cmp_gt8"
_CMP_LE8 = "cmp_le8"
_CMP_GE8 = "cmp_ge8"
_LNOT8 = "lnot8"


def translate_program(prog: tac_ast.Type_program) -> asm_ast.Type_program:
    match prog:
        case tac_ast.Program(function_definition=fn):
            return asm_ast.Program(function_definition=translate_function(fn))
    raise TypeError(f"unexpected program node: {prog!r}")


def translate_function(
    fn: tac_ast.Type_function_definition,
) -> asm_ast.Type_function_definition:
    match fn:
        case tac_ast.Function(name=name, instructions=instrs):
            out: list[asm_ast.Type_instruction] = []
            for instr in instrs:
                out.extend(translate_instruction(instr))
            return asm_ast.Function(name=name, instructions=out)
    raise TypeError(f"unexpected function node: {fn!r}")


def translate_instruction(
    instr: tac_ast.Type_instruction,
) -> list[asm_ast.Type_instruction]:
    match instr:
        case tac_ast.Ret(val=val):
            # arg_bytes/local_bytes are zeros here; the allocate_stack
            # pass rewrites them to the function's actual N and M.
            return [
                asm_ast.Mov(src=translate_val(val), dst=_REG_A),
                asm_ast.Ret(arg_bytes=0, local_bytes=0),
            ]
        case tac_ast.Unary(op=op, src=src, dst=dst):
            return (
                [asm_ast.Mov(src=translate_val(src), dst=_REG_A)]
                + translate_unop_atoms(op)
                + [asm_ast.Mov(src=_REG_A, dst=translate_val(dst))]
            )
        case tac_ast.Binary(op=op, src1=src1, src2=src2, dst=dst):
            return translate_binary(op, src1, src2, dst)
    raise TypeError(f"unexpected instruction node: {instr!r}")


def translate_binary(
    op: tac_ast.Type_binary_operator,
    src1: tac_ast.Type_val,
    src2: tac_ast.Type_val,
    dst: tac_ast.Type_val,
) -> list[asm_ast.Type_instruction]:
    """Lower a TAC Binary into the asm sequence: load src1 into A, set
    up carry, do the op against src2, store A into dst. Optimization
    is deferred to TAC-level passes — this just emits a correct (if
    inefficient) sequence."""
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
            return _translate_ax_call(src1_op, src2_op, dst_op, _CMP_EQ8,
                                      result_in_x=False)
        case tac_ast.NotEqual():
            return _translate_ax_call(src1_op, src2_op, dst_op, _CMP_NE8,
                                      result_in_x=False)
        case tac_ast.LessThan():
            return _translate_ax_call(src1_op, src2_op, dst_op, _CMP_LT8,
                                      result_in_x=False)
        case tac_ast.GreaterThan():
            return _translate_ax_call(src1_op, src2_op, dst_op, _CMP_GT8,
                                      result_in_x=False)
        case tac_ast.LessOrEqual():
            return _translate_ax_call(src1_op, src2_op, dst_op, _CMP_LE8,
                                      result_in_x=False)
        case tac_ast.GreaterOrEqual():
            return _translate_ax_call(src1_op, src2_op, dst_op, _CMP_GE8,
                                      result_in_x=False)
    raise TypeError(f"unexpected binary operator: {op!r}")


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
    """Atomic asm instructions implementing the unary op on A.
    Result is left in A."""
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
            # !A := 1 if A == 0 else 0. Through the runtime helper
            # lnot8 (takes A, returns A) — same shape as the binary
            # comparison helpers.
            return [asm_ast.Call(name=_LNOT8)]
    raise TypeError(f"unexpected unary operator: {op!r}")
