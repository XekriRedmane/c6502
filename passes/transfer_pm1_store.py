"""Fold `TXA; (SEC|CLC); (SBC|ADC) #1; STA M` to `DEX/INX; STX M`
(and the Y mirror).

# Pattern

Four consecutive instructions:

    Mov(Reg(X), Reg(A))         # TXA — A := X
    SetCarry() / ClearCarry()
    Sub(Imm(1), Reg(A))   # SBC #1 — A := A - 1
        or
    Add(Imm(1), Reg(A))   # ADC #1 — A := A + 1
    Mov(Reg(A), <Data|ZP>)      # STA M

Rewrite to two instructions:

    Dec(Reg(X)) / Inc(Reg(X))   # DEX or INX
    Mov(Reg(X), <Data|ZP>)      # STX M

Same shape for Y (`TYA; ... STA M` → `DEY/INY; STY M`).

# Where this hits

The lowering shape `(c99 expr — 1)` or `(c99 expr + 1)` stored back
to memory always routes through A (the 6502's only ALU register).
When the source value is already in X or Y for some other reason —
e.g. an `LDX m` for indexed-load duty just before — the four-atom
chain above appears. Tac-side lowering can't see "X already holds
m"; only post-coloring asm-level peepholing can.

The direct savings on this single fold are 3 bytes / 4 cycles
versus the through-A sequence. The bigger win is a cascade: if A
was spilled around the arithmetic (because regalloc thought it had
to preserve a value across the A-clobber), the spill becomes dead
once A is no longer touched, and `redundant_load_elimination` /
`asm_dead_store` / `dead_a_arith` collect it on a later fixedpoint
iteration.

# Soundness gates

Original net effect: `A := X-1`, `M := X-1`, X unchanged, all
flags set off `X-1` (TXA sets N/Z, then SBC fully sets N/Z/C/V).

Rewrite net effect: `X := X-1`, `M := X-1`, A unchanged, N/Z set
off `X-1` from DEX (C and V untouched).

The rewrite is sound iff at the boundary after STA M:
  1. A is dead (we no longer leave `X-1` in A).
  2. X is dead (X now holds `X-1` instead of the original X).
  3. All flags (N/Z/C/V) are dead — C and V were set by the SBC
     in the original sequence, and DEX leaves them stale.

For the +1 form (`CLC; ADC #1`) the C/V mutation by the original
ADC is also lost; the gate is the same `all_flags_dead_at`.

# Destination operand restriction

STX / STY support only `zp`, `zp,Y` (STX) / `zp,X` (STY), and
`abs` addressing modes. Plain `Data(name)` and `ZP(addr)` resolve
to either zp or abs at link time; both are valid. The
`IndexedData`, `Indirect`, `IndirectY`, `IndirectZp`,
`IndirectZpY`, `Frame`, and `Stack` operand shapes use addressing
modes STX / STY don't support — skip those.

The Mov atom must not be volatile (preserves the existing peephole
convention of leaving volatile mem-to-A traffic alone).

# Where to run

Inside the asm-peephole fixedpoint, after `replace_pseudoregisters`
(so Pseudos are resolved to concrete addressing modes) and before
`expand_long_branches` (shrinks the program — no displacement
growth). The fold is monotone-shrinking (4 atoms → 2), so it's
safe in the fixed point.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import (
    a_dead_at, all_flags_dead_at, x_dead_at, y_dead_at,
)


def apply_transfer_pm1_store(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    changed = False
    while i < len(instrs):
        match = _match_pattern(instrs, i)
        if match is not None:
            reg_cls, is_decrement, dst = match
            if is_decrement:
                out.append(asm_ast.Dec(dst=asm_ast.Reg(reg=reg_cls())))
            else:
                out.append(asm_ast.Inc(dst=asm_ast.Reg(reg=reg_cls())))
            out.append(asm_ast.Mov(
                src=asm_ast.Reg(reg=reg_cls()),
                dst=dst,
                is_volatile=False,
            ))
            i += 4
            changed = True
            continue
        out.append(instrs[i])
        i += 1
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _match_pattern(
    instrs: list[asm_ast.Type_instruction], i: int,
):
    """Return `(reg_cls, is_decrement, dst_operand)` when the 4-atom
    pattern matches at `i` and all soundness gates pass, else None.

    `reg_cls` is `asm_ast.X` or `asm_ast.Y`; `is_decrement` is True
    for the SBC #1 form, False for the ADC #1 form; `dst_operand`
    is the STA's destination operand (Data or ZP)."""
    if i + 3 >= len(instrs):
        return None
    a, b, c, d = instrs[i], instrs[i + 1], instrs[i + 2], instrs[i + 3]

    # a: Mov(Reg(X)|Reg(Y), Reg(A)), non-volatile.
    if not (isinstance(a, asm_ast.Mov) and not a.is_volatile):
        return None
    if not (
        isinstance(a.src, asm_ast.Reg)
        and isinstance(a.src.reg, (asm_ast.X, asm_ast.Y))
        and isinstance(a.dst, asm_ast.Reg)
        and isinstance(a.dst.reg, asm_ast.A)
    ):
        return None
    reg_cls = type(a.src.reg)

    # c: Sub(Imm(1), Reg(A)) — SBC #1 — or Add(Imm(1), Reg(A)) — ADC #1.
    if isinstance(c, asm_ast.Sub):
        if not isinstance(b, asm_ast.SetCarry):
            return None
        is_decrement = True
    elif isinstance(c, asm_ast.Add):
        if not isinstance(b, asm_ast.ClearCarry):
            return None
        is_decrement = False
    else:
        return None
    if not (
        isinstance(c.src, asm_ast.Imm) and c.src.value == 1
        and isinstance(c.dst, asm_ast.Reg)
        and isinstance(c.dst.reg, asm_ast.A)
    ):
        return None

    # d: Mov(Reg(A), Data|ZP), non-volatile.
    if not (isinstance(d, asm_ast.Mov) and not d.is_volatile):
        return None
    if not (
        isinstance(d.src, asm_ast.Reg)
        and isinstance(d.src.reg, asm_ast.A)
        and isinstance(d.dst, (asm_ast.Data, asm_ast.ZP))
    ):
        return None

    # Soundness gates at the boundary after the STA.
    after = i + 4
    if not a_dead_at(instrs, after):
        return None
    reg_dead = x_dead_at if reg_cls is asm_ast.X else y_dead_at
    if not reg_dead(instrs, after):
        return None
    if not all_flags_dead_at(instrs, after):
        return None
    return reg_cls, is_decrement, d.dst
