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
    safely at any flag-overwriting instruction or block exit). For
    the register-transfer subset (`TXA` / `TYA`) we additionally
    accept the case where the current N/Z flags already reflect
    the src register's value via `_flags_reflect_src_reg` — see
    "Redundant-flag drop for TXA / TYA" below.

Handled instruction kinds:

  * `Mov(Imm | Data | ZP | Reg(X|Y), Reg(A))` — LDA imm/abs/zp,
    TXA, TYA. Writes A + N/Z.
  * `Add` / `Sub` / `And` / `Or` with `src ∈ {Imm, Data, ZP}` and
    `dst = Reg(A)` — ADC/SBC/AND/ORA. Reads A, writes A + flags.
  * `Xor` with `src1, src2 ∈ {Imm, Data, ZP, Reg(A)}` and
    `dst = Reg(A)` — EOR. Reads operands, writes A + N/Z.

# Redundant-flag drop for TXA / TYA

A TXA whose only flag consumer is a downstream Branch (and whose
A-side is dead) is usually NOT droppable by the flags-dead check
— `flags_dead_at` bails at the Branch. But when the most-recent
flag-setter before the TXA was itself a write to the same source
register (e.g. `LDX M; TXA; BMI L` — LDX set N/Z based on the
loaded value, which IS X's current value), the TXA's own flag
effect (N/Z based on A_new = X) duplicates the current flag state.
Dropping the TXA leaves the Branch reading the LDX-set flags,
which match what the TXA would have produced. Sound iff:

  * A is dead at i+1 (the TXA's A-side has no observers), AND
  * `_flags_reflect_src_reg` returns True — the most recent
    flag-setter on the backward straight-line path wrote the same
    register the TXA reads, so the current flags already reflect
    that register's value.

This is what was missing on the beam_target_tick `LDX
beam_snd_ctr; TXA; BMI .if_end@0` shape: A is killed on both
arms (LDA on fall-through, TXA on the branch target), flags ARE
read by BMI, but LDX already set them to beam_snd_ctr's value.

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
    a_dead_at, all_flags_dead_at, flags_dead_at, is_reg_a,
    sets_flags,
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
    """Iterate the single-walk drop step to a local fixed point.
    A single walk catches the trailing atom of a chain (e.g. the
    ADC #0 in `LDA #0; ADC #0`); the next round catches the LDA
    that was previously kept alive by the ADC reading A. Without
    iterating, a downstream pass like the volatile-void-read CMP
    rewrite can lock in a still-droppable atom by extending A's
    liveness across it before dead_a_arith next runs."""
    instrs = list(fn.instructions)
    while True:
        prev_len = len(instrs)
        drop: list[bool] = [False] * len(instrs)
        for i, instr in enumerate(instrs):
            if not _writes_only_a_and_flags(instr):
                continue
            after = i + 1
            if not a_dead_at(instrs, after):
                continue
            # Add/Sub also write C/V. Dropping such an instruction
            # must not strand a downstream C-reader (e.g. a
            # subsequent SBC in the multi-byte chain that threads
            # carry without an intervening CLC/SEC). For non-C-
            # writing A-arithmetic (Mov/And/Or/Xor — LDA/TXA/TYA/
            # AND/ORA/EOR), the C/V flags carry through unchanged,
            # so only N/Z liveness matters.
            if isinstance(instr, (asm_ast.Add, asm_ast.Sub)):
                if not all_flags_dead_at(instrs, after):
                    continue
            else:
                if not flags_dead_at(instrs, after):
                    # Redundant-flag fallback for TXA / TYA: the
                    # current N/Z already reflect src reg's value
                    # (set by a prior LDX / LDY / INX / ...), so
                    # the transfer's flag effect is redundant.
                    # Other Mov shapes (LDA imm / LDA mem) and the
                    # ALU ops don't have an analog — their flag
                    # effect depends on the loaded / computed value,
                    # not on a register's current value.
                    if not _flags_reflect_src_reg(instrs, i):
                        continue
            drop[i] = True
        instrs = [
            instr for i, instr in enumerate(instrs) if not drop[i]
        ]
        if len(instrs) == prev_len:
            break
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _writes_only_a_and_flags(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr`'s only observable effects are writes to
    `Reg(A)` and to the N/Z/C/V flags — no memory write, no
    secondary register clobber from operand-shape emission, no
    control-flow effect."""
    if isinstance(instr, asm_ast.Mov):
        # A volatile-flagged Mov derives from a volatile lvalue
        # access — its memory read IS the observable effect even
        # when A and flags are dead afterward. Never droppable.
        if instr.is_volatile:
            return False
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
    an LDY (or other register clobber) in emission. Imm /
    ImmLabelLow / ImmLabelHigh / Data / ZP / IndexedData qualify
    — they emit as single LDA / ADC / ... opcodes without a
    setup LDY. Frame / Stack / Indirect / IndirectY don't (their
    emit prepends an `LDY #off` that's a real side effect)."""
    return isinstance(op, (
        asm_ast.Imm,
        asm_ast.ImmLabelLow,
        asm_ast.ImmLabelHigh,
        asm_ast.Data,
        asm_ast.ZP,
        asm_ast.IndexedData,
    ))


def _is_xy_reg(op: asm_ast.Type_operand) -> bool:
    """True iff `op` is `Reg(X)` or `Reg(Y)` — the register-to-A
    transfer sources (TXA / TYA)."""
    return (
        isinstance(op, asm_ast.Reg)
        and isinstance(op.reg, (asm_ast.X, asm_ast.Y))
    )


def _flags_reflect_src_reg(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> bool:
    """True iff `instrs[i]` is `Mov(Reg(X|Y), Reg(A))` and the CPU
    N/Z flags at position `i` already reflect the src register's
    current value. When True, the Mov's own flag effect (N/Z based
    on A_new = src_reg) is a no-op against the current flag state
    and the Mov can be dropped (provided A is also dead).

    Backward scan within the basic block from `i-1`. The first
    flag-setter we encounter is the one that produced the current
    N/Z. If it wrote the same register the Mov reads, flags
    reflect that register's new value — and since any later
    register write would have been a flag-setter we'd have stopped
    at first, the src register's value hasn't changed since.

    Bails on labels, terminators, calls, prologues, and
    `LoadAddress` — any cross-block reasoning isn't worth the
    complexity for this pattern and these atoms either expand into
    multi-instruction sequences (`asm_to_asm2`) or are CFG joins
    where the prior-flag-setter on different predecessors could
    differ.
    """
    instr = instrs[i]
    if not isinstance(instr, asm_ast.Mov):
        return False
    if not is_reg_a(instr.dst):
        return False
    if not _is_xy_reg(instr.src):
        return False
    src_reg_type = type(instr.src.reg)
    j = i - 1
    while j >= 0:
        prev = instrs[j]
        if isinstance(prev, (
            asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
            asm_ast.Ret, asm_ast.Return, asm_ast.Call,
            asm_ast.FunctionPrologue, asm_ast.AllocateStack,
            asm_ast.LoadAddress,
        )):
            return False
        if not sets_flags(prev):
            j -= 1
            continue
        return _writes_reg_setting_n_z(prev, src_reg_type)
    return False


def _writes_reg_setting_n_z(
    instr: asm_ast.Type_instruction, reg_type: type,
) -> bool:
    """True iff `instr` writes `Reg(reg_type)` and sets N/Z based
    on the new register value — the standard 6502 behavior of
    every register-writing opcode the IR models (LDA/LDX/LDY,
    TAX/TAY/TXA/TYA, INX/INY/DEX/DEY, ADC/SBC/AND/ORA/EOR on A,
    ASL/LSR/ROL/ROR on A, PLA).

    Store / mem-to-mem Movs return False because they either don't
    touch flags (STA) or set them based on the loaded mem value
    rather than the register's value (mem-to-mem `LDA src; STA
    dst`).
    """
    if isinstance(instr, asm_ast.Mov):
        if isinstance(instr.dst, asm_ast.Reg):
            return isinstance(instr.dst.reg, reg_type)
        return False
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        if isinstance(instr.dst, asm_ast.Reg):
            return isinstance(instr.dst.reg, reg_type)
        return False
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
        asm_ast.Xor,
    )):
        # ADC / SBC / AND / ORA / EOR always write Reg(A).
        return reg_type is asm_ast.A
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        if isinstance(instr.dst, asm_ast.Reg):
            return isinstance(instr.dst.reg, reg_type)
        return False
    if isinstance(instr, asm_ast.Pop):
        if isinstance(instr.dst, asm_ast.Reg):
            return isinstance(instr.dst.reg, reg_type)
        return False
    return False
