"""Add 1 to a 16-bit sum through the carry of the add that makes it.

# Pattern

`p + b + 1`, with `p` a 2-byte value and `b` a byte (or any operand
whose high byte is 0), is two 16-bit adds, the second of the constant
1. `tac_to_asm` lowers them one after the other through a temporary:

    CLC
    ADC   m                 ; the low byte of p + b
    STA   t0
    LDA   h
    ADC   #$00              ; the high byte
    STA   t1
    LDA   t0
    CLC
    ADC   #$01              ; + 1
    STA   r0
    LDA   t1
    ADC   #$00
    STA   r1

`next_string` in `examples/ali_baba/text.c` returns the string after
`s` as `s + s[0] + 1` that way. With the carry set going into the
first add, the first add makes the whole sum, as the original's `SEC;
ADC` does:

    SEC
    ADC   m
    STA   r0
    LDA   h
    ADC   #$00
    STA   r1

# Soundness

Let `L` be A at the first `ADC`. The original computes `T = H * 256 +
L + m` modulo 65536, with `H` the byte read from `h`, and then `R = T +
1` modulo 65536. The rewrite computes `L + m + 1` in the low byte, at
most 511, so its carry into the high byte is right, and `R` is the
same. A after the last `ADC` is `R`'s high byte both ways, so N and Z
are the same. C and V can differ (the original's C is the carry out
of `T + 1` alone), so both must be dead after the last store
(`asm_liveness.c_dead_at`, `v_dead_at`).

`t0` and `t1` aren't written by the rewrite, so each must be dead after
its load (`asm_dead_store.mem_dead_after`) and one of the function's
own `__local_*` slots, which no other name in the function reaches.
The rewrite stores `r0` before it reads `h`, so `r0` must be a `Data`
or `ZP` byte other than `h` and `t1`, and `h` a constant or another of
the function's own slots, which no other spelling reaches. `h` may
be `t1`, as when the high byte is added in place, since the rewrite
reads `h` where the original did, before the store to `t1`, and `t1`
is dead after its load. It can't be `t0`, which the original writes
before reading `h`. No instruction in the window may be
volatile. The thirteen instructions are consecutive, so no label is
among them.

# Where to run

In the asm peephole fixed point, before `carry_inc` turns an `ADC
#$00` into `BCC; INC`.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import c_dead_at, v_dead_at

_A = asm_ast.Reg(reg=asm_ast.A())
_ZERO = asm_ast.Imm(value=0)
_ONE = asm_ast.Imm(value=1)
_MEM = (asm_ast.Data, asm_ast.ZP)


def apply_carry_in_add(
    prog: asm_ast.Program,
    *,
    zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Folds each `p + b + 1` pair of 16-bit adds into one add with
    the carry set, in every function. `zp_slot_symbols` is passed to
    `mem_dead_after`."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl, zp_slot_symbols)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _mov(ins, src=None, dst=None) -> bool:
    """True when `ins` is a non-volatile `Mov`, with the given src and
    dst when they're given."""
    return (isinstance(ins, asm_ast.Mov) and not ins.is_volatile
            and (src is None or ins.src == src)
            and (dst is None or ins.dst == dst))


def _add(ins, src=None) -> bool:
    return (isinstance(ins, asm_ast.Add) and ins.dst == _A
            and (src is None or ins.src == src))


def _match(w, prefix: str):
    """`(m, h, t0, t1, r0, r1)` when the thirteen instructions `w` are
    the pattern in the module docstring, else None."""
    if not (isinstance(w[0], asm_ast.ClearCarry) and _add(w[1])
            and _mov(w[2], src=_A) and _mov(w[3], dst=_A)
            and _add(w[4], _ZERO) and _mov(w[5], src=_A)
            and _mov(w[6], dst=_A) and isinstance(w[7], asm_ast.ClearCarry)
            and _add(w[8], _ONE) and _mov(w[9], src=_A)
            and _mov(w[10], dst=_A) and _add(w[11], _ZERO)
            and _mov(w[12], src=_A)):
        return None
    m, h = w[1].src, w[3].src
    t0, t1, r0, r1 = w[2].dst, w[5].dst, w[9].dst, w[12].dst
    if not (w[6].src == t0 and w[10].src == t1):
        return None
    for t in (t0, t1):
        if not (isinstance(t, asm_ast.Data) and t.name.startswith(prefix)):
            return None
    if t0 == t1 or h == t0 or r0 == t1:
        return None
    if not isinstance(r0, _MEM):
        return None
    # `h` is read after the store to `r0`, so it must be a byte no
    # other spelling reaches: a constant, or another of the function's
    # own slots, which only their `Data` names reach.
    if isinstance(h, (asm_ast.Imm, asm_ast.ImmLabelLow,
                      asm_ast.ImmLabelHigh)):
        return m, h, t0, t1, r0, r1
    if (isinstance(h, asm_ast.Data) and h.name.startswith(prefix)
            and h != r0):
        return m, h, t0, t1, r0, r1
    return None


def _rewrite_function(fn: asm_ast.Function, zp_slot_symbols):
    from passes.asm_dead_store import mem_dead_after
    instrs = list(fn.instructions)
    prefix = f"__local_{fn.name}__"
    changed = False
    i = 0
    while i + 13 <= len(instrs):
        found = _match(instrs[i:i + 13], prefix)
        if found is None:
            i += 1
            continue
        m, h, t0, t1, r0, r1 = found
        end = i + 13
        if not (c_dead_at(instrs, end) and v_dead_at(instrs, end)
                and mem_dead_after(instrs, i + 6, t0, fn_name=fn.name,
                                   zp_slot_symbols=zp_slot_symbols)
                and mem_dead_after(instrs, i + 10, t1, fn_name=fn.name,
                                   zp_slot_symbols=zp_slot_symbols)):
            i += 1
            continue
        instrs[i:end] = [
            asm_ast.SetCarry(),
            asm_ast.Add(src=m, dst=_A),
            asm_ast.Mov(src=_A, dst=r0, is_volatile=False),
            asm_ast.Mov(src=h, dst=_A, is_volatile=False),
            asm_ast.Add(src=_ZERO, dst=_A),
            asm_ast.Mov(src=_A, dst=r1, is_volatile=False),
        ]
        changed = True
        i += 6
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )
