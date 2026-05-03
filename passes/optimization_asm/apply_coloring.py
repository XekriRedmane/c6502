"""Apply a `Coloring` to an asm-SSA function: substitute every
`Pseudo(name, offset)` whose name is in `coloring.assignments` with
the corresponding `ZP(address, offset)` operand.

Runs between `byte_dce` and `from_ssa`. By the time `from_ssa`
sees the function, colored values have been lowered to ZP, so the
parallel-copy ordering can spot cross-Mov cycles at the PHYSICAL
slot level (which is the actual hazard) instead of just the SSA
name level. Cycles like:

    Phi(X, [(P, Y)])  ; X := Y
    Phi(Y, [(P, X)])  ; Y := X

where X and Y get DIFFERENT colors $A and $B form a 2-cycle at
the predecessor edge (Mov $B → $A clobbers $A before Mov $A → $B
reads it). With pre-applied coloring, those Movs become
`Mov(ZP($B), ZP($A))` and `Mov(ZP($A), ZP($B))`, and the cycle
detector sees the `ZP($A)` repetition.

Pseudos NOT in the coloring (params, address-taken, statics,
spilled) pass through unchanged — they're handled later by
`replace_pseudoregisters_bare_exit`.
"""
from __future__ import annotations

import asm_ast
from passes.optimization.register_allocation import Coloring


def apply_coloring(
    fn: asm_ast.Function, coloring: Coloring,
) -> asm_ast.Function:
    """Return `fn` with every colored Pseudo lowered to ZP."""
    if not coloring.assignments:
        return fn
    new_instrs = [
        _apply_to_instruction(instr, coloring)
        for instr in fn.instructions
    ]
    return asm_ast.Function(
        name=fn.name,
        is_global=fn.is_global,
        params=list(fn.params),
        instructions=new_instrs,
    )


def _apply_to_op(
    op: asm_ast.Type_operand, coloring: Coloring,
) -> asm_ast.Type_operand:
    if isinstance(op, asm_ast.Pseudo) and op.name in coloring.assignments:
        addr = coloring.assignments[op.name] + op.offset
        return asm_ast.ZP(address=addr, offset=0)
    return op


def _apply_to_instruction(
    instr: asm_ast.Type_instruction, coloring: Coloring,
) -> asm_ast.Type_instruction:
    apply = lambda op: _apply_to_op(op, coloring)
    match instr:
        case asm_ast.Mov(src=src, dst=dst):
            return asm_ast.Mov(src=apply(src), dst=apply(dst))
        case asm_ast.Add(src=src, dst=dst):
            return asm_ast.Add(src=apply(src), dst=apply(dst))
        case asm_ast.Sub(src=src, dst=dst):
            return asm_ast.Sub(src=apply(src), dst=apply(dst))
        case asm_ast.And(src=src, dst=dst):
            return asm_ast.And(src=apply(src), dst=apply(dst))
        case asm_ast.Or(src=src, dst=dst):
            return asm_ast.Or(src=apply(src), dst=apply(dst))
        case asm_ast.Xor(src1=s1, src2=s2, dst=dst):
            return asm_ast.Xor(
                src1=apply(s1), src2=apply(s2), dst=apply(dst),
            )
        case asm_ast.Inc(dst=dst):
            return asm_ast.Inc(dst=apply(dst))
        case asm_ast.Dec(dst=dst):
            return asm_ast.Dec(dst=apply(dst))
        case asm_ast.ArithmeticShiftLeft(dst=dst):
            return asm_ast.ArithmeticShiftLeft(dst=apply(dst))
        case asm_ast.LogicalShiftRight(dst=dst):
            return asm_ast.LogicalShiftRight(dst=apply(dst))
        case asm_ast.RotateLeft(dst=dst):
            return asm_ast.RotateLeft(dst=apply(dst))
        case asm_ast.RotateRight(dst=dst):
            return asm_ast.RotateRight(dst=apply(dst))
        case asm_ast.Push(src=src):
            return asm_ast.Push(src=apply(src))
        case asm_ast.Pop(dst=dst):
            return asm_ast.Pop(dst=apply(dst))
        case asm_ast.Compare(left=left, right=right):
            return asm_ast.Compare(
                left=apply(left), right=apply(right),
            )
        case asm_ast.LoadAddress(src=src, dst=dst):
            # `src` is by construction excluded from coloring
            # (address-taken), so apply() is a no-op for it. Apply
            # to dst for completeness.
            return asm_ast.LoadAddress(
                src=apply(src), dst=apply(dst),
            )
        case asm_ast.Phi(dst=dst, args=args):
            return asm_ast.Phi(
                dst=apply(dst),
                args=[
                    asm_ast.AsmPhiArg(
                        pred_label=a.pred_label,
                        source=apply(a.source),
                    )
                    for a in args
                ],
            )
        case _:
            return instr
