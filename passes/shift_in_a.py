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

# Shifts of other bytes, and a constant store in the way

A byte widened and then shifted, `(unsigned int)c << 3`, interleaves
its two bytes:

    LDA   c
    ASL
    STA   lo
    LDA   #$00              ; the zero high byte
    STA   hi
    ROL   hi
    ASL   lo
    ROL   hi
    ASL   lo
    ROL   hi

The run may include shifts of other bytes, which leave A alone and
stay where they are; each still reads the carry that the shift of M
before it set, now from A. The `LDA #0; STA hi` in the way, which
would lose A, moves up in front of the code that loaded A (a load
into A and then operations on A alone that don't touch `hi`), so the
whole thing becomes

    LDA   #$00
    STA   hi
    LDA   c
    ASL
    ROL   hi
    ASL
    ROL   hi
    ASL
    ROL   hi
    STA   lo

17 bytes and 32 cycles instead of 19 and 38 in zero page.

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


def _mem(op) -> bool:
    return isinstance(op, (asm_ast.ZP, asm_ast.Data))


def _distinct(a, b) -> bool:
    """True iff the zero-page or absolute bytes `a` and `b` are known
    to be different bytes: the same kind of operand and not equal.
    (A `Data` slot symbol can name a `ZP` address.)"""
    return type(a) is type(b) and a != b


def _constant_stores(instrs, i, mem):
    """The constant stores right after `instrs[i]`: `LDA #k; STA H`
    pairs, or single `Mov(#k, H)`s, which emit the same and use A,
    each `H` a byte known to differ from `mem`. Returns the
    instructions, and how many there are."""
    out = []
    j = i + 1
    while j < len(instrs):
        ins = instrs[j]
        if (isinstance(ins, asm_ast.Mov) and _imm(ins.src)
                and not ins.is_volatile and _mem(ins.dst)
                and _distinct(ins.dst, mem)):
            out.append(ins)
            j += 1
            continue
        if j + 1 >= len(instrs):
            break
        store = instrs[j + 1]
        if not (isinstance(ins, asm_ast.Mov) and _is_reg_a(ins.dst)
                and _imm(ins.src)
                and isinstance(store, asm_ast.Mov) and _is_reg_a(store.src)
                and not store.is_volatile and _mem(store.dst)
                and _distinct(store.dst, mem)):
            break
        out += [ins, store]
        j += 2
    return out


def _imm(op) -> bool:
    return isinstance(op, (
        asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh,
    ))


_A_OPS = (
    asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
    asm_ast.RotateLeft, asm_ast.RotateRight,
    asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
)


def _a_chain_start(instrs, i, avoid) -> int | None:
    """The index of the load into A that starts the code computing A
    for `instrs[i]`: a `Mov(x, A)` followed only by operations on A.
    None when there isn't one, or when it reads or writes a byte in
    `avoid` or might."""
    k = i - 1
    while k >= 0:
        ins = instrs[k]
        if isinstance(ins, asm_ast.Mov) and _is_reg_a(ins.dst):
            src = ins.src
            if ins.is_volatile or not _safe_read(src, avoid):
                return None
            return k
        if isinstance(ins, _A_OPS) and _is_reg_a(ins.dst):
            src = getattr(ins, "src", None)
            if src is not None and not _safe_read(src, avoid):
                return None
            k -= 1
            continue
        return None
    return None


def _safe_read(src, avoid) -> bool:
    if isinstance(src, (asm_ast.Imm, asm_ast.ImmLabelLow,
                        asm_ast.ImmLabelHigh, asm_ast.Reg)):
        return True
    if _mem(src):
        return all(_distinct(src, h) for h in avoid)
    return False


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
        pairs = _constant_stores(instrs, i, mem)
        first = i + 1 + len(pairs)
        j = first
        own = 0
        while j < len(instrs) and isinstance(instrs[j], _SHIFTS):
            if instrs[j].dst == mem:
                own += 1
            elif not (_mem(instrs[j].dst) and _distinct(instrs[j].dst, mem)):
                break
            j += 1
        # Trailing shifts of other bytes stay outside the run.
        while j > first and instrs[j - 1].dst != mem:
            j -= 1
        if own == 0:
            i += 1
            continue
        start = None
        if pairs:
            start = _a_chain_start(
                instrs, i, [p.dst for p in pairs if _mem(p.dst)],
            )
            if start is None:
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
        shifts = [
            type(instr)(dst=_REG_A) if instr.dst == mem else instr
            for instr in instrs[first:j]
        ]
        end = j + 1 if reloads else j
        instrs[i:end] = [*shifts, store]
        if pairs:
            instrs[start:start] = pairs
            i += len(pairs)
        changed = True
        i += len(shifts) + 1
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )
