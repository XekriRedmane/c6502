"""Translate a tac_ast tree into an asm_ast tree.

One `translate_*` function per source-AST node kind. Uses `match` for
dispatch so each case mirrors the constructor signature; adding a new
TAC AST variant is a matter of adding a new `case` clause.

Mapping:
  Program(fn)              -> Program(translate_function(fn))
  Function(name, instrs)   -> Function(name, flat-mapped instructions)
  Ret(val)                 -> [Mov(translate_val(val), Reg(A)), Ret]
  Unary(op, src, dst)      -> [Mov(translate_val(src), translate_val(dst)),
                               Unary(translate_unop(op), translate_val(dst))]
  Complement               -> Not
  Negate                   -> Neg
  Constant(v)              -> Imm(v)
  Var(name)                -> Pseudo(name)
"""

from __future__ import annotations

import asm_ast
import tac_ast


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
            # amt=0 here; the future pseudo->stack pass rewrites Ret to
            # carry the actual locals+args byte count for this function.
            return [
                asm_ast.Mov(src=translate_val(val), dst=asm_ast.Reg(reg=asm_ast.A())),
                asm_ast.Ret(amt=0),
            ]
        case tac_ast.Unary(op=op, src=src, dst=dst):
            dst_op = translate_val(dst)
            return [
                asm_ast.Mov(src=translate_val(src), dst=dst_op),
                asm_ast.Unary(op=translate_unop(op), src_dst=dst_op),
            ]
    raise TypeError(f"unexpected instruction node: {instr!r}")


def translate_val(val: tac_ast.Type_val) -> asm_ast.Type_operand:
    match val:
        case tac_ast.Constant(value=v):
            return asm_ast.Imm(value=v)
        case tac_ast.Var(name=n):
            return asm_ast.Pseudo(name=n)
    raise TypeError(f"unexpected val node: {val!r}")


def translate_unop(
    op: tac_ast.Type_unary_operator,
) -> asm_ast.Type_unary_operator:
    match op:
        case tac_ast.Complement():
            return asm_ast.Not()
        case tac_ast.Negate():
            return asm_ast.Neg()
    raise TypeError(f"unexpected unary operator: {op!r}")
