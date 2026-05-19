"""Fold `TXA; STA M` → `STX M` (and the Y variant) when safe.

# Pattern

Two consecutive instructions:

    Mov(Reg(X), Reg(A))     # TXA — A := X, sets N/Z to N/Z(X).
    Mov(Reg(A), <Data|ZP>)  # STA M.

→ `Mov(Reg(X), <Data|ZP>)` (STX M). Same shape for Y (`TYA; STA M
→ STY M`).

`STX` and `STY` support `zp` / `zp,Y` / `abs` (STX) and `zp` /
`zp,X` / `abs` (STY) addressing modes; plain `Data(name)` and
`ZP(addr)` operands resolve to either zp or abs at link time, so
both are valid targets. `Frame` / `Stack` / `Indirect*` use
indirect-Y, which STX / STY don't support — leave those alone.

# Soundness

`TXA` reads X and writes A, setting N/Z to N/Z(X). `STA M` writes
A's value to M; flags untouched. Exit: A = X, M = X, flags =
N/Z(X).

`STX M` writes X to M, flags untouched. Exit: A = unchanged, M =
X, flags = unchanged from before TXA.

The rewrite differs from the original in two ways:

  1. A's value at exit: original has A = X, rewrite has A =
     whatever it was before the TXA. Sound iff A is dead at the
     next instruction.
  2. Flag state at exit: original has N/Z = N/Z(X), rewrite has
     N/Z = unchanged. Sound iff the flags are dead at the next
     instruction (no downstream Branch reads N/Z before another
     flag-affecting op overwrites them).

Both checks use the existing liveness helpers
(`a_dead_at` / `flags_dead_at`).

# Where this hits

The pattern arises after `passes.split_mem_to_mem` splits a
mem-to-mem `Mov(Data(__local_<counter>), Data(__zpabi_*))` and
`passes.x_save_slot_load` rewrites the LDA half (`Mov(M, Reg(A))`)
to `Mov(Reg(X), Reg(A))` (TXA). Before split, the mem-to-mem
shape went through `x_save_slot_load`'s Pass 3 mem-to-mem case
(`Mov(M, D)` with D = Data/ZP → `Mov(Reg(X), D)`) and emitted
STX directly. After split, the LDA + STA pair survives as two
atoms; this peephole re-collapses it.

The same shape can arise from `TAX; STA M` and `TAY; STA M`
chains via other paths — register transfers followed by a store
of the same value through A. Generalizing to all (X, Y) sources
catches those for free.

# Where to run

Inside the asm-peephole fixed-point loop, after
`apply_x_save_slot_load` has had a chance to convert
`Mov(M, Reg(A))` to `Mov(Reg(X), Reg(A))` and before
`expand_long_branches`. The fold produces one fewer byte per
occurrence (STX abs = 3 vs. TXA + STA abs = 1 + 3 = 4), so no
long-branch displacements grow.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at, flags_dead_at


def apply_via_a_store_fold(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg(op, regtype) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, regtype)


def _is_reg_a(op) -> bool:
    return _is_reg(op, asm_ast.A)


def _is_xy_to_a_transfer(instr) -> tuple[asm_ast.Type_reg, bool] | None:
    """Return (source-reg, is_volatile) when `instr` is `Mov(Reg(X),
    Reg(A))` or `Mov(Reg(Y), Reg(A))` — TXA or TYA. None otherwise."""
    if not isinstance(instr, asm_ast.Mov):
        return None
    if instr.is_volatile:
        return None
    if not _is_reg_a(instr.dst):
        return None
    if not isinstance(instr.src, asm_ast.Reg):
        return None
    if isinstance(instr.src.reg, (asm_ast.X, asm_ast.Y)):
        return instr.src.reg, instr.is_volatile
    return None


def _is_a_store_to_addressable(instr) -> bool:
    """True iff `instr` is `Mov(Reg(A), <Data|ZP>)` — a STA whose
    destination's addressing mode is also valid for STX / STY."""
    return (isinstance(instr, asm_ast.Mov)
            and not instr.is_volatile
            and _is_reg_a(instr.src)
            and isinstance(instr.dst, (asm_ast.Data, asm_ast.ZP)))


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    changed = False
    while i < len(instrs):
        if i + 1 < len(instrs):
            xfer = _is_xy_to_a_transfer(instrs[i])
            if (xfer is not None
                    and _is_a_store_to_addressable(instrs[i + 1])
                    and a_dead_at(instrs, i + 2)
                    and flags_dead_at(instrs, i + 2)):
                src_reg, _ = xfer
                out.append(asm_ast.Mov(
                    src=asm_ast.Reg(reg=src_reg),
                    dst=instrs[i + 1].dst,
                    is_volatile=False,
                ))
                i += 2
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
