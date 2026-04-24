"""Replace Pseudo operands in an ASM AST with Frame operands.

Each function gets its own offset map. Walking its instructions in
order, the first time a Pseudo(name) is seen it's assigned the current
`sp` value as its frame offset and `sp` is incremented; subsequent
uses of the same name reuse the same offset.

The result is a Frame(offset), not a Stack(offset), because
SSP-relative offsets shift whenever the function pushes anything
during its body (e.g. arguments for a nested call). FP is captured
once in the prelude and stays put for the function's lifetime, so
Frame-relative offsets remain valid across intra-function pushes.

`sp` starts at `args_bytes + 1`. The +1 is the soft-stack convention
mirrored to FP: FP points at the next-free byte (just below the first
local at the moment the prelude finishes), so the smallest valid
offset is 1. c6502 doesn't yet support function arguments, so
`args_bytes` is 0 and pseudos become `Frame(1)`, `Frame(2)`, ... in
the order they first appear.

Operands other than `Pseudo` pass through unchanged; instructions
with no operand fields (`Ret`, `FunctionPrologue`) pass through too.
"""

from __future__ import annotations

import asm_ast


class Replacer:
    """Per-function state: a name->offset map and the running sp."""

    def __init__(self, args_bytes: int = 0) -> None:
        self.offsets: dict[str, int] = {}
        # +1 because FP points at the next-free byte; FP+1 is the
        # smallest valid offset for a stored value.
        self.sp: int = args_bytes + 1

    def replace_operand(self, op: asm_ast.Type_operand) -> asm_ast.Type_operand:
        match op:
            case asm_ast.Pseudo(name=name):
                if name not in self.offsets:
                    self.offsets[name] = self.sp
                    self.sp += 1
                return asm_ast.Frame(offset=self.offsets[name])
            case _:
                return op

    def replace_instruction(
        self, instr: asm_ast.Type_instruction,
    ) -> asm_ast.Type_instruction:
        match instr:
            case asm_ast.Mov(src=src, dst=dst):
                return asm_ast.Mov(
                    src=self.replace_operand(src),
                    dst=self.replace_operand(dst),
                )
            case asm_ast.Add(src=src, dst=dst):
                return asm_ast.Add(
                    src=self.replace_operand(src),
                    dst=self.replace_operand(dst),
                )
            case asm_ast.Sub(src=src, dst=dst):
                return asm_ast.Sub(
                    src=self.replace_operand(src),
                    dst=self.replace_operand(dst),
                )
            case asm_ast.And(src=src, dst=dst):
                return asm_ast.And(
                    src=self.replace_operand(src),
                    dst=self.replace_operand(dst),
                )
            case asm_ast.Or(src=src, dst=dst):
                return asm_ast.Or(
                    src=self.replace_operand(src),
                    dst=self.replace_operand(dst),
                )
            case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
                return asm_ast.Xor(
                    src1=self.replace_operand(s1),
                    src2=self.replace_operand(s2),
                    dst=self.replace_operand(dst),
                )
            case asm_ast.Inc(dst=dst):
                return asm_ast.Inc(dst=self.replace_operand(dst))
            case asm_ast.Dec(dst=dst):
                return asm_ast.Dec(dst=self.replace_operand(dst))
            case asm_ast.ArithmeticShiftLeft(dst=dst):
                return asm_ast.ArithmeticShiftLeft(
                    dst=self.replace_operand(dst),
                )
            case asm_ast.LogicalShiftRight(dst=dst):
                return asm_ast.LogicalShiftRight(
                    dst=self.replace_operand(dst),
                )
            case asm_ast.RotateLeft(dst=dst):
                return asm_ast.RotateLeft(dst=self.replace_operand(dst))
            case asm_ast.RotateRight(dst=dst):
                return asm_ast.RotateRight(dst=self.replace_operand(dst))
            case asm_ast.Push(src=src):
                return asm_ast.Push(src=self.replace_operand(src))
            case asm_ast.Pop(dst=dst):
                return asm_ast.Pop(dst=self.replace_operand(dst))
            case asm_ast.Compare(left=left, right=right):
                return asm_ast.Compare(
                    left=self.replace_operand(left),
                    right=self.replace_operand(right),
                )
            case _:
                return instr


def replace_function(
    fn: asm_ast.Type_function_definition,
    args_bytes: int = 0,
) -> asm_ast.Type_function_definition:
    match fn:
        case asm_ast.Function(name=name, instructions=instrs):
            r = Replacer(args_bytes=args_bytes)
            return asm_ast.Function(
                name=name,
                instructions=[r.replace_instruction(i) for i in instrs],
            )
        case _:
            raise TypeError(f"unexpected function node: {fn!r}")


def replace_program(prog: asm_ast.Type_program) -> asm_ast.Type_program:
    match prog:
        case asm_ast.Program(function_definition=fn):
            return asm_ast.Program(function_definition=replace_function(fn))
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")
