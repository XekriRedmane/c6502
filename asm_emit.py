"""Emit 6502 assembly from an asm_ast program.

Formatting rules:
  - labels start in column 1
  - opcodes (uppercase) start in column 4
  - operands start in column 10

CLI: `asm_emit.py <input.c>|- [-o output.asm]`. The full pipeline goes
C source -> parse -> tac translate -> asm translate -> emit. If -o is
given the filename must have a .asm suffix; otherwise output goes to
stdout.
"""

from __future__ import annotations

import argparse
import sys

import asm_ast
from asm_translator import translate_program as translate_to_asm
from parser import parse
from tac_translator import translate_program as translate_to_tac


# 0-indexed column positions (column 1 = index 0).
_OPCODE_COL = 3    # "column 4"
_OPERAND_COL = 9   # "column 10"


def _instr_line(opcode: str, operand: str = "") -> str:
    line = " " * _OPCODE_COL + opcode.upper()
    if operand:
        pad = max(1, _OPERAND_COL - len(line))
        line += " " * pad + operand
    return line


def _reg_letter(r: asm_ast.Type_reg) -> str:
    match r:
        case asm_ast.A():
            return "A"
        case asm_ast.X():
            return "X"
        case asm_ast.Y():
            return "Y"
    raise TypeError(f"unexpected reg: {r!r}")


def _emit_mov(src: asm_ast.Type_operand, dst: asm_ast.Type_operand) -> str:
    match src, dst:
        case asm_ast.Imm(value=v), asm_ast.Reg(reg=r):
            if not 0 <= v <= 255:
                raise ValueError(
                    f"immediate {v} out of range for 6502 (expected 0..255)"
                )
            return _instr_line(f"LD{_reg_letter(r)}", f"#${v:02X}")
        case asm_ast.Reg(reg=asm_ast.X()), asm_ast.Reg(reg=asm_ast.A()):
            return _instr_line("TXA")
        case asm_ast.Reg(reg=asm_ast.Y()), asm_ast.Reg(reg=asm_ast.A()):
            return _instr_line("TYA")
        case asm_ast.Reg(reg=asm_ast.A()), asm_ast.Reg(reg=asm_ast.X()):
            return _instr_line("TAX")
        case asm_ast.Reg(reg=asm_ast.A()), asm_ast.Reg(reg=asm_ast.Y()):
            return _instr_line("TAY")
    raise ValueError(f"cannot emit Mov(src={src!r}, dst={dst!r})")


def _emit_unary(
    op: asm_ast.Type_unary_operator, src_dst: asm_ast.Type_operand,
) -> list[str]:
    match op, src_dst:
        case asm_ast.Not(), asm_ast.Reg(reg=asm_ast.A()):
            return [_instr_line("EOR", "#$FF")]
        case asm_ast.Neg(), asm_ast.Reg(reg=asm_ast.A()):
            # Two's complement: invert, then +1 with a known-clear carry.
            return [
                _instr_line("EOR", "#$FF"),
                _instr_line("CLC"),
                _instr_line("ADC", "#$01"),
            ]
    raise ValueError(f"cannot emit Unary(op={op!r}, src_dst={src_dst!r})")


def emit_instruction(instr: asm_ast.Type_instruction) -> list[str]:
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            return [_emit_mov(src, dst)]
        case asm_ast.Unary(op=op, src_dst=src_dst):
            return _emit_unary(op, src_dst)
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

    text = emit_program(translate_to_asm(translate_to_tac(parse(source))))

    if args.output is not None:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(text)
    else:
        sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
