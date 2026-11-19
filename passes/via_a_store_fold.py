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

# A copy while A is busy

A byte copied from one place to another while A holds a value it
needs again goes through X (or Y) instead of parking A in a slot:

    STA   t                     LDX   M
    LDA   M            ->       STX   D
    STA   D
    LDA   t

when `t` is one of the function's own `__local_*` slots that nothing
reads after the `LDA t`, `M` is a byte or a constant `LDX` can load
and `D` one `STX` can store, X is dead from the `STA t` on (else Y,
the same way), and N and Z are dead after, since they come from `M`
where the `LDA t` set them from A. A ends with the value it had.
Zork's `do_var` (examples/zork/decode.c) copies
`OPERANDS[0]` into a slot on entry while A holds the opcode it
dispatches on. It saves 4 bytes with the bytes in zero page.

The copies between the `STA t` and the `LDA t` may be several, and
each may be a load and any number of stores, or a memory-to-memory
`Mov(M, D)`, which `asm_emit` writes as `LDA M; STA D`. A constant
already in the register isn't loaded again. Ali Baba's
`print_bottom_centered` (examples/ali_baba/text.c) gets its pointer in
A and X and stores two constants before passing it on, so X is read
by the tail call and Y carries them:

    STA   t                     LDY   #$17
    LDA   #$17                  STY   TEXT_ROW
    STA   TEXT_ROW      ->      LDY   #$00
    LDA   #$00                  STY   TEXT_COL
    STA   TEXT_COL              JMP   print_centered_plus_col
    LDA   t
    JMP   print_centered_plus_col

The conditions are the ones above, for every copy: nothing in the run
reads or writes `t`, every load is a byte or a constant the register
can load and every store a `Data` or `ZP` byte, X (or Y) is dead from
the `STA t` on, which also means no copy reads it, and N and Z are
dead after the `LDA t`, since the last load sets them. C and V are
unchanged both ways. A store before the run's first load stores A,
which holds the parked value both ways. A volatile load or store
stays one access of the same byte in the same order, through the
other register.

`apply_copy_through_index` runs once, after the last peephole run,
`carry_known`, and the jump tables. Before the jump table, `do_var`'s
dispatch starts with a `BEQ` that reads the Z from `LDA t`. Inside
the fixed point it took the `LDA` of a copy
before the folds that remove such copies outright had run, and
`branch_to_offset` came out 6 bytes longer.

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
from passes.asm_liveness import (
    a_dead_at, flags_dead_at, x_dead_at, y_dead_at,
)


def apply_via_a_store_fold(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def apply_copy_through_index(
    prog: asm_ast.Program, *, zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Copies each byte that A is parked for through X or Y instead,
    in every function. See "A copy while A is busy" above."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _copy_through_index(tl, zp_slot_symbols)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _copy_through_index(
    fn: asm_ast.Function, zp_slot_symbols,
) -> asm_ast.Function:
    """`STA t; LDA M; STA D; ...; LDA t` as `LDX M; STX D; ...` (or
    `LDY` / `STY`). See "A copy while A is busy" above."""
    from passes.asm_dead_store import mem_dead_after
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i + 3 < len(instrs):
        park = instrs[i]
        t = getattr(park, "dst", None)
        if not (isinstance(park, asm_ast.Mov) and not park.is_volatile
                and _is_reg_a(park.src)
                and isinstance(t, asm_ast.Data)
                and t.name.startswith(f"__local_{fn.name}__")):
            i += 1
            continue
        back = _copies_end(instrs, i + 1, t)
        if back is None:
            i += 1
            continue
        after = back + 1
        if not (flags_dead_at(instrs, after)
                and mem_dead_after(instrs, back, t, fn_name=fn.name,
                                   zp_slot_symbols=zp_slot_symbols)):
            i += 1
            continue
        if x_dead_at(instrs, i):
            reg = asm_ast.Reg(reg=asm_ast.X())
        elif y_dead_at(instrs, i):
            reg = asm_ast.Reg(reg=asm_ast.Y())
        else:
            i += 1
            continue
        instrs[i:after] = _through_index(instrs[i + 1:back], reg)
        changed = True
        i += back - i - 1
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


_INDEX_LOADABLE = (asm_ast.Data, asm_ast.ZP, asm_ast.Imm,
                   asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)
_INDEX_STORABLE = (asm_ast.Data, asm_ast.ZP)


def _copies_end(instrs, start: int, t) -> int | None:
    """Returns the index of the `LDA t` that ends a run of copies
    starting at `instrs[start]`, or None when the instructions from
    there aren't such a run. The run holds loads into A of a byte or a
    constant (`_INDEX_LOADABLE`), stores of A into `Data` / `ZP`, and
    copies from one to the other (`Mov(M, D)`, which `asm_emit` writes
    as `LDA M; STA D`), none of them reading or writing `t`, with at
    least one load or copy."""
    loads = 0
    k = start
    while k < len(instrs):
        ins = instrs[k]
        if not isinstance(ins, asm_ast.Mov) or t in (ins.src, ins.dst):
            if (isinstance(ins, asm_ast.Mov) and ins.src == t
                    and _is_reg_a(ins.dst) and not ins.is_volatile
                    and loads):
                return k
            return None
        if _is_reg_a(ins.dst):
            if not isinstance(ins.src, _INDEX_LOADABLE):
                return None
            loads += 1
        elif _is_reg_a(ins.src):
            if not isinstance(ins.dst, _INDEX_STORABLE):
                return None
        elif (isinstance(ins.src, _INDEX_LOADABLE)
                and isinstance(ins.dst, _INDEX_STORABLE)
                and not ins.is_volatile):
            loads += 1
        else:
            return None
        k += 1
    return None


_CONSTANTS = (asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh)


def _through_index(body, reg):
    """The copies of `body` through `reg`. A store before the first
    load stores A, which still holds the parked value. A constant
    already in `reg` isn't loaded again."""
    out = []
    held = None     # what `reg` holds: None before the first load
    for ins in body:
        if _is_reg_a(ins.dst) or not _is_reg_a(ins.src):
            if not (isinstance(ins.src, _CONSTANTS) and ins.src == held):
                out.append(asm_ast.Mov(src=ins.src, dst=reg,
                                       is_volatile=ins.is_volatile))
            held = ins.src
        if not _is_reg_a(ins.dst):
            out.append(asm_ast.Mov(
                src=asm_ast.Reg(reg=asm_ast.A()) if held is None else reg,
                dst=ins.dst, is_volatile=ins.is_volatile))
    return out


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
