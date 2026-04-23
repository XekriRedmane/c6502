"""Translate a c99_ast tree into an asm_ast tree.

One `translate_*` function per source-AST node kind. Uses `match` for
dispatch so each case mirrors the constructor signature; adding a new
C99 AST variant is a matter of adding a new `case` clause.

Current mapping:
  Program(fn)           -> Program(translate_function(fn))
  Function(name, body)  -> Function(name, translate_statement(body))
  Return(exp)           -> [Mov(translate_exp(exp), Register()), Ret()]
  Constant(value)       -> Imm(value)
"""

from __future__ import annotations

import sys

import asm_ast
import c99_ast
from parser import parse
from pretty import pretty


def translate_program(prog: c99_ast.Type_program) -> asm_ast.Type_program:
    match prog:
        case c99_ast.Program(function_definition=fn):
            return asm_ast.Program(function_definition=translate_function(fn))
    raise TypeError(f"unexpected program node: {prog!r}")


def translate_function(fn: c99_ast.Type_function_definition) -> asm_ast.Type_function_definition:
    match fn:
        case c99_ast.Function(name=name, body=body):
            return asm_ast.Function(name=name, instructions=translate_statement(body))
    raise TypeError(f"unexpected function node: {fn!r}")


def translate_statement(stmt: c99_ast.Type_statement) -> list[asm_ast.Type_instruction]:
    match stmt:
        case c99_ast.Return(exp=exp):
            return [
                asm_ast.Mov(src=translate_exp(exp), dst=asm_ast.Register()),
                asm_ast.Ret(),
            ]
    raise TypeError(f"unexpected statement node: {stmt!r}")


def translate_exp(exp: c99_ast.Type_exp) -> asm_ast.Type_operand:
    match exp:
        case c99_ast.Constant(value=v):
            return asm_ast.Imm(value=v)
    raise TypeError(f"unexpected exp node: {exp!r}")


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("usage: asm_translator.py <file>|-", file=sys.stderr)
        return 2
    if argv[1] == "-":
        source = sys.stdin.read()
    else:
        with open(argv[1], "r", encoding="utf-8") as f:
            source = f.read()
    print(pretty(translate_program(parse(source))))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
