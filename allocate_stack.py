"""Insert a FunctionPrologue at the start of each function and fill
in each Ret's arg/local byte counts.

Runs after replace_pseudoregisters, so every operand that refers to a
local lives in a `Frame(offset)`. The pass walks each function's
instructions, finds the highest Frame offset in use (= `M`, the
function's local byte count), prepends
`FunctionPrologue(arg_bytes=N, local_bytes=M)`, and rewrites every
`Ret(...)` to carry the same `N` and `M` so the epilogue can
compute the SSP rewind and locate the saved-FP slot.

Function arguments are not yet supported, so the pass hardcodes
`arg_bytes=0`. The highest Frame offset directly equals `M`. Once we
add args, the layout will put args at `M+3..M+N+2`; `N` will come
from the function definition (e.g. its parameter list) and `M` will
still be derived from the highest *local* Frame offset.

Functions with no locals and no args get `FunctionPrologue(0, 0)` and
`Ret(0, 0)`, which emit nothing extra (just `RTS`).
"""

from __future__ import annotations

import asm_ast


def _operands(instr: asm_ast.Type_instruction):
    """Yield each operand-typed field of an instruction. Instructions
    with no operands (Ret, FunctionPrologue, ClearCarry, SetCarry,
    Call) yield nothing."""
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Add(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Sub(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.And(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Or(src=src, dst=dst):
            yield src
            yield dst
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            yield s1
            yield s2
            yield dst
        case asm_ast.Inc(dst=dst):
            yield dst
        case asm_ast.Dec(dst=dst):
            yield dst
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            yield dst
        case asm_ast.LogicalShiftRight(dst=dst):
            yield dst
        case asm_ast.RotateLeft(dst=dst):
            yield dst
        case asm_ast.RotateRight(dst=dst):
            yield dst
        case asm_ast.Push(src=src):
            yield src
        case asm_ast.Pop(dst=dst):
            yield dst


def _local_bytes(instrs: list[asm_ast.Type_instruction]) -> int:
    """Return the highest Frame offset referenced by these instructions
    (0 if none use a Frame operand)."""
    m = 0
    for instr in instrs:
        for op in _operands(instr):
            if isinstance(op, asm_ast.Frame) and op.offset > m:
                m = op.offset
    return m


def _set_ret_dims(
    instr: asm_ast.Type_instruction, n: int, m: int,
) -> asm_ast.Type_instruction:
    """Rewrite Ret to carry the function's arg/local byte counts;
    pass other instructions through unchanged."""
    match instr:
        case asm_ast.Ret():
            return asm_ast.Ret(arg_bytes=n, local_bytes=m)
        case _:
            return instr


def allocate_function(
    fn: asm_ast.Type_function_definition,
) -> asm_ast.Type_function_definition:
    match fn:
        case asm_ast.Function(name=name, instructions=instrs):
            m = _local_bytes(instrs)
            n = 0  # function arguments not yet supported
            updated = [_set_ret_dims(i, n, m) for i in instrs]
            prologue = asm_ast.FunctionPrologue(arg_bytes=n, local_bytes=m)
            return asm_ast.Function(
                name=name,
                instructions=[prologue] + updated,
            )
        case _:
            raise TypeError(f"unexpected function node: {fn!r}")


def allocate_program(prog: asm_ast.Type_program) -> asm_ast.Type_program:
    match prog:
        case asm_ast.Program(function_definition=fn):
            return asm_ast.Program(function_definition=allocate_function(fn))
        case _:
            raise TypeError(f"unexpected program node: {prog!r}")
