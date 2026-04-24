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
                              asm_ast has no Unary node anymore — it's
                              strictly a TAC concept.
  Binary(op, src1, src2, dst) -> [Mov(src1, Reg(A)),
                                  <carry setup>,
                                  Add|Sub(src2, Reg(A)),
                                  Mov(Reg(A), dst)]
                              for Add and Subtract:
                                Add      -> ClearCarry + Add(...)
                                Subtract -> SetCarry + Sub(...)
                              Multiply/Divide/Modulo are not yet
                              translated (deferred until a Call
                              instruction lands).
  Constant(v)              -> Imm(v)
  Var(name)                -> Pseudo(name)
"""

from __future__ import annotations

import asm_ast
import tac_ast


_REG_A = asm_ast.Reg(reg=asm_ast.A())


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
        case tac_ast.Multiply() | tac_ast.Divide() | tac_ast.Modulo():
            raise NotImplementedError(
                f"binary op {type(op).__name__} not yet handled by "
                "asm_translator (will be lowered via Call mul8/div8 once "
                "the Call instruction exists)"
            )
    raise TypeError(f"unexpected binary operator: {op!r}")


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
    raise TypeError(f"unexpected unary operator: {op!r}")
