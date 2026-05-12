"""Asm-level dead-Reg(A)-arithmetic elimination.

Drops instructions whose only observable effects are writes to
`Reg(A)` and the N/Z (and C/V for arithmetic) flags, when both
`Reg(A)` and the flags are dead afterward.

# Motivating case

The unoptimized `y += 35` skip path in `interlace_blit_p1` lowers
the 16-bit ADC chain (because the C type checker promotes `y` to
`int` for the add) as:

    TYA              ; A = y
    CLC
    ADC #$23         ; A = y + 35 (low byte)
    TAY              ; y = A (truncated back to uchar)
    LDA #$00         ; A = 0      ← high-byte add of the
    ADC #$00         ; A = 0+0+C  ←   promoted int — never stored
    JMP .if_end@0

The `LDA #$00; ADC #$00` computes the high byte of the promoted-
int sum, but the result is never stored (uchar truncates back to
1 byte) and A is killed downstream before being read. The result
sitting in A is dead; both instructions can be dropped.

# Eligibility

The instruction is droppable when:

  * Its only memory effect is on `Reg(A)` (no memory write, no
    register write other than to A, no PC effect other than
    fall-through, no helper call).
  * Its emission doesn't have secondary side effects — the
    operand-shape constraint rules out Frame / Stack / Indirect /
    IndirectY operands (which trigger an LDY-setup in emission,
    clobbering the Y register). Only `Imm`, `Data`, `ZP`, and the
    direct register transfers (TXA / TYA) qualify.
  * `Reg(A)` is dead after the instruction (CFG-wide forward
    walk via `asm_liveness.a_dead_at`).
  * The flags are dead after the instruction (within-block walk
    via `asm_liveness.flags_dead_at` — bails at any Branch, ends
    safely at any flag-overwriting instruction or block exit).

Handled instruction kinds:

  * `Mov(Imm | Data | ZP | Reg(X|Y), Reg(A))` — LDA imm/abs/zp,
    TXA, TYA. Writes A + N/Z.
  * `Add` / `Sub` / `And` / `Or` with `src ∈ {Imm, Data, ZP}` and
    `dst = Reg(A)` — ADC/SBC/AND/ORA. Reads A, writes A + flags.
  * `Xor` with `src1, src2 ∈ {Imm, Data, ZP, Reg(A)}` and
    `dst = Reg(A)` — EOR. Reads operands, writes A + N/Z.

# Iteration

The pass runs inside `_peephole_fixedpoint`. A single forward
sweep handles the headline case in two iterations: iteration 1
drops the ADC #$00 (its A is dead after); iteration 2 drops the
LDA #$00 (now its A is also dead, since the ADC that read it is
gone). The fixed-point loop iterates until a sweep makes no
change.
"""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import (
    a_dead_at, flags_dead_at, is_reg_a,
)


def apply_dead_a_arith_elimination(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    drop: list[bool] = [False] * len(instrs)
    for i, instr in enumerate(instrs):
        if not _writes_only_a_and_flags(instr):
            continue
        after = i + 1
        if not a_dead_at(instrs, after):
            continue
        if not flags_dead_at(instrs, after):
            continue
        drop[i] = True
    out = [
        instr for i, instr in enumerate(instrs) if not drop[i]
    ]
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _writes_only_a_and_flags(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr`'s only observable effects are writes to
    `Reg(A)` and to the N/Z/C/V flags — no memory write, no
    secondary register clobber from operand-shape emission, no
    control-flow effect."""
    if isinstance(instr, asm_ast.Mov):
        if not is_reg_a(instr.dst):
            return False
        return _is_pure_source(instr.src) or _is_xy_reg(instr.src)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        if not is_reg_a(instr.dst):
            return False
        return _is_pure_source(instr.src)
    if isinstance(instr, asm_ast.Xor):
        if not is_reg_a(instr.dst):
            return False
        for s in (instr.src1, instr.src2):
            if is_reg_a(s):
                continue
            if not _is_pure_source(s):
                return False
        return True
    return False


def _is_pure_source(op: asm_ast.Type_operand) -> bool:
    """True iff `op`, used as a load/arith source, doesn't trigger
    an LDY (or other register clobber) in emission. Imm, Data, ZP
    qualify; Frame/Stack/Indirect/IndirectY don't."""
    return isinstance(op, (asm_ast.Imm, asm_ast.Data, asm_ast.ZP))


def _is_xy_reg(op: asm_ast.Type_operand) -> bool:
    """True iff `op` is `Reg(X)` or `Reg(Y)` — the register-to-A
    transfer sources (TXA / TYA)."""
    return (
        isinstance(op, asm_ast.Reg)
        and isinstance(op.reg, (asm_ast.X, asm_ast.Y))
    )
