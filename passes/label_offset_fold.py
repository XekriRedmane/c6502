"""Link-time address arithmetic: a label's address plus a constant as
one immediate.

# Pattern

`&r[2].a` on a static array, or `prompts + 15`, reaches the asm as a
2-byte add of a label's halves and a constant:

    LDA   #$2F                Mov(Imm(lo), A)
    CLC                       ClearCarry          (before or after the LDA)
    ADC   #<r                 Add(ImmLabelLow(r, o), A)
    STA   d0                  Mov(A, D0)
    LDA   #$00                Mov(Imm(hi), A)
    ADC   #>r                 Add(ImmLabelHigh(r, o), A)
    STA   d1                  Mov(A, D1)

(either operand of each `ADC` may be the label). The sum is a
link-time constant, so the pass writes

    LDA   #<(r+47)
    STA   d0
    LDA   #>(r+47)
    STA   d1

dasm computes `r+47` as a full address, so the high half carries out
of the low as the `ADC` did. That's 4 instructions instead of 7.

# Conditions

C and V must be dead after the last store, since the `ADC`s set them
and the loads don't. A and N/Z end the same: A holds the high byte of
the sum either way, and both `ADC` and `LDA` set N/Z from it.

# Where to run

In the asm peephole fixed point, which both pipelines run. Memory
value propagation then forwards the immediates to where `d0` and `d1`
are read.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import c_dead_at, v_dead_at

_A = asm_ast.Reg(reg=asm_ast.A())


def apply_label_offset_fold(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _byte_add(instrs, i, label_cls, first: bool):
    """`(label, constant, dst, next index)` when an add of a byte
    starts at `instrs[i]`: `LDA x; ADC y; STA dst` with one of x / y
    a `label_cls` and the other an immediate, and for the low byte
    (`first`) a `CLC` before or after the `LDA`."""
    j = i
    clc = False
    if first and j < len(instrs) and isinstance(instrs[j], asm_ast.ClearCarry):
        clc, j = True, j + 1
    if j >= len(instrs):
        return None
    load = instrs[j]
    if not (isinstance(load, asm_ast.Mov) and load.dst == _A
            and not load.is_volatile):
        return None
    j += 1
    if first and not clc:
        if j >= len(instrs) or not isinstance(instrs[j], asm_ast.ClearCarry):
            return None
        j += 1
    if j + 1 >= len(instrs):
        return None
    add, store = instrs[j], instrs[j + 1]
    if not (isinstance(add, asm_ast.Add) and add.dst == _A):
        return None
    if not (isinstance(store, asm_ast.Mov) and store.src == _A
            and not store.is_volatile
            and isinstance(store.dst, (asm_ast.Data, asm_ast.ZP))):
        return None
    for lab, imm in ((load.src, add.src), (add.src, load.src)):
        if isinstance(lab, label_cls) and isinstance(imm, asm_ast.Imm):
            return lab, imm.value, store.dst, j + 2
    return None


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    changed = False
    i = 0
    while i < len(instrs):
        low = _byte_add(instrs, i, asm_ast.ImmLabelLow, first=True)
        high = (
            _byte_add(instrs, low[3], asm_ast.ImmLabelHigh, first=False)
            if low is not None else None
        )
        if (low is not None and high is not None
                and low[0].name == high[0].name
                and low[0].offset == high[0].offset
                and c_dead_at(instrs, high[3])
                and v_dead_at(instrs, high[3])):
            name, base = low[0].name, low[0].offset
            off = base + ((low[1] + 256 * high[1]) & 0xFFFF)
            out += [
                asm_ast.Mov(src=asm_ast.ImmLabelLow(name=name, offset=off),
                            dst=_A),
                asm_ast.Mov(src=_A, dst=low[2]),
                asm_ast.Mov(src=asm_ast.ImmLabelHigh(name=name, offset=off),
                            dst=_A),
                asm_ast.Mov(src=_A, dst=high[2]),
            ]
            i = high[3]
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
