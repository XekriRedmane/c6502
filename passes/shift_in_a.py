"""Shift a byte in A instead of in the memory it's stored to.

# Pattern

    Mov(Reg(A), M)            # STA M
    <op>(M)                   # ASL / LSR / ROL / ROR M, one or more
    ...
    [Mov(M, Reg(A))]          # LDA M, optional

becomes

    <op>(Reg(A))              # ASL A ... (each op on A)
    ...
    Mov(Reg(A), M)            # STA M

A held M's value at the `STA`, so the shifts give the same result in
A, and the final `STA` leaves M as it was. Each shift on A is one
byte and 2 cycles, against 2 bytes and 5 cycles on a zero-page byte.

The flags come out the same. Each `ASL A` sets C, N, and Z as `ASL M`
did, and `STA` changes none of them. A `ROL` / `ROR` reads the same C
either way. The trailing `LDA M` set N and Z from M's final value,
which the last shift already set them to.

# When it applies

- `M` is a `ZP` or `Data` byte, and neither the `STA` nor the `LDA`
  is volatile.
- The shifts are on `M`, one right after another, right after the
  `STA`.
- After them comes `LDA M`, which the rewrite deletes, since A now
  holds M. Without it, A has to be dead after the shifts, since the
  rewrite leaves the shifted value in A instead of the unshifted
  one.

When M isn't read again, the `STA` the rewrite leaves is dead, and
`asm_dead_store` deletes it, so `LDA w; STA t; ASL t; LDA t` ends as
`LDA w; ASL A`.

# Where it comes from

A 1-byte constant shift is already lowered in A by `tac_to_asm`. A
multi-byte one shifts its bytes in place, and when the high bytes
turn out to be dead, what's left is a single byte shifted in memory.
The first step of a byte widened and then shifted (`(int)c << 3`) is
the same shape.

# Where to run

Inside the asm-peephole fixed-point loop."""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at

_SHIFTS = (
    asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
    asm_ast.RotateLeft, asm_ast.RotateRight,
)

_REG_A = asm_ast.Reg(reg=asm_ast.A())


def apply_shift_in_a(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = list(fn.instructions)
    i = 0
    changed = False
    while i < len(instrs):
        store = instrs[i]
        if not (
            isinstance(store, asm_ast.Mov)
            and not store.is_volatile
            and _is_reg_a(store.src)
            and isinstance(store.dst, (asm_ast.ZP, asm_ast.Data))
        ):
            i += 1
            continue
        mem = store.dst
        j = i + 1
        while (
            j < len(instrs)
            and isinstance(instrs[j], _SHIFTS)
            and instrs[j].dst == mem
        ):
            j += 1
        if j == i + 1:
            i += 1
            continue
        load = instrs[j] if j < len(instrs) else None
        reloads = (
            isinstance(load, asm_ast.Mov)
            and not load.is_volatile
            and load.src == mem
            and _is_reg_a(load.dst)
        )
        if not reloads and not a_dead_at(instrs, j):
            i += 1
            continue
        shifts = [type(instr)(dst=_REG_A) for instr in instrs[i + 1:j]]
        end = j + 1 if reloads else j
        instrs[i:end] = [*shifts, store]
        changed = True
        i += len(shifts) + 1
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )
