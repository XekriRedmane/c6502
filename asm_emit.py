"""Emit 6502 assembly from an asm_ast program.

Formatting rules:
  - labels start in column 1
  - opcodes (uppercase) start in column 4
  - operands start in column 10

CLI: `asm_emit.py <input.c>|- [-o output.asm]`. The full pipeline goes
C source → parse → translate → emit. If -o is given the filename must
have a .asm suffix; otherwise output goes to stdout.
"""

from __future__ import annotations

import argparse
import sys

import asm_ast
from asm_translator import translate_program
from parser import parse


# 0-indexed column positions (column 1 = index 0).
_OPCODE_COL = 3    # "column 4"
_OPERAND_COL = 9   # "column 10"


def _instr_line(opcode: str, operand: str = "") -> str:
    line = " " * _OPCODE_COL + opcode.upper()
    if operand:
        pad = max(1, _OPERAND_COL - len(line))
        line += " " * pad + operand
    return line


def emit_operand(op: asm_ast.Type_operand) -> str:
    match op:
        case asm_ast.Imm(value=v):
            if not 0 <= v <= 255:
                raise ValueError(
                    f"immediate {v} out of range for 6502 (expected 0..255)"
                )
            return f"#${v:02X}"
        case asm_ast.Register():
            return "A"
    raise TypeError(f"unexpected operand: {op!r}")


def _mov_opcode(dst: asm_ast.Type_operand) -> str:
    match dst:
        case asm_ast.Register():
            return "LDA"
    raise TypeError(f"unsupported mov destination: {dst!r}")


def emit_instruction(instr: asm_ast.Type_instruction) -> list[str]:
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            return [_instr_line(_mov_opcode(dst), emit_operand(src))]
        case asm_ast.Ret():
            return [_instr_line("RTS")]
    raise TypeError(f"unexpected instruction: {instr!r}")


def emit_function(fn: asm_ast.Type_function_definition) -> list[str]:
    match fn:
        case asm_ast.Function(name=name, instructions=instrs):
            # Label in col 1; SUBROUTINE directive in col 4 (same column as
            # opcodes); blank line before instructions.
            lines = [f"{name}:", _instr_line("SUBROUTINE")]
            if instrs:
                lines.append("")
                for instr in instrs:
                    lines.extend(emit_instruction(instr))
            return lines
    raise TypeError(f"unexpected function: {fn!r}")


def emit_program(prog: asm_ast.Type_program) -> str:
    match prog:
        case asm_ast.Program(function_definition=fn):
            return "\n".join(emit_function(fn)) + "\n"
    raise TypeError(f"unexpected program: {prog!r}")


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(prog="asm_emit.py")
    ap.add_argument("input", help="C source file, or - for stdin")
    ap.add_argument("-o", dest="output",
                    help="output file (must have .asm suffix)")
    args = ap.parse_args(argv[1:])

    if args.output is not None and not args.output.endswith(".asm"):
        print(
            f"asm_emit.py: output file must have .asm suffix: {args.output}",
            file=sys.stderr,
        )
        return 2

    if args.input == "-":
        source = sys.stdin.read()
    else:
        with open(args.input, "r", encoding="utf-8") as f:
            source = f.read()

    text = emit_program(translate_program(parse(source)))

    if args.output is not None:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(text)
    else:
        sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
