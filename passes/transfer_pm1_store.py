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
  3. C and V are dead — they were set by the SBC in the original
     sequence, and DEX leaves them stale. N and Z come out the same:
     DEX sets them from `X-1`, the value the SBC left in A. So a
     branch on N or Z may follow, as in `get_next_code_byte2`'s
     `if (++Z_PC2_L == 0)` when examples/zork/pages.c
     kept Z_PC2 in three bytes, which became `INY; STY Z_PC2_L; BNE`.

For the +1 form (`CLC; ADC #1`) the C/V mutation by the original
ADC is also lost; the gate is the same.

# Destination operand restriction

STX / STY support only `zp`, `zp,Y` (STX) / `zp,X` (STY), and
`abs` addressing modes. Plain `Data(name)` and `ZP(addr)` resolve
to either zp or abs at link time; both are valid. The
`IndexedData`, `Indirect`, `IndirectY`, `IndirectZp`,
`IndirectZpY`, `Frame`, and `Stack` operand shapes use addressing
modes STX / STY don't support — skip those.

The Mov atom must not be volatile (preserves the existing peephole
convention of leaving volatile mem-to-A traffic alone).

# An index register that already holds the byte

The converse shape steps a byte into an index register that already
holds it:

    LDY M; ...; LDA M; CLC; ADC #1; TAY     ->     LDY M; ...; INY

`_rewrite_index_steps` turns `LDA M; CLC; ADC #k; TAY` into `INY`
once or twice for `k` of 1 or 2, `SEC; SBC #k` into `DEY`, and the
same with `TAX`, `INX`, and `DEX`, when the register still holds M:
its last write in the block is `LDY M` (or `LDX M`), and nothing
since writes M or might. A store through a pointer might, except to
a byte no pointer can point to, a `static` or one of the function's
own zero-page slots whose address the unit never takes
(`asm_aliasing.unreachable_by_pointer`), such as Zork's `token_idx`
in examples/zork/input.c. A, C, and V must be dead
after. N and Z come out the same, from M plus or minus `k`.

# A step on either side of a transfer

`_rewrite_steps_across_transfer` moves a step of 1 or 2 from A to
the index register on the other side of a transfer:

    CLC; ADC #1; TAY      ->     TAY; INY        (A, C, V dead after)
    TYA; CLC; ADC #1      ->     INY; TYA        (Y, C, V dead after)

and the same for `SEC; SBC #k` with `DEY`, and for X. When the
second is followed by a `TAY` back, all four are `INY` (A, C, V dead
after). In the first,
Y ends up with A's sum and A keeps its old value. In the second, A
ends up with the sum and Y with it too. N and Z come out the same,
from the sum. Zork's `match_dictionary_word`
(examples/zork/input.c) steps its index past the count
of separators with `y++` and adds the last step to the dictionary's
address, the first and second forms. Each saves 2 bytes and 2 cycles
for a step of 1, and 1 byte for a step of 2.

# A step stored and loaded into an index register

`_rewrite_step_then_reload` steps a byte in an index register when the
stepped byte is stored back and then loaded into that register:

    LDA M; SEC; SBC #2; STA M; LDX M     ->     LDX M; DEX; DEX; STX M

for a step `k` of 1 or 2, `CLC; ADC #k` with `INX`, and the Y forms,
when A, C, and V are dead after. X and M end up with the same value
as before, and N and Z come out the same, from the last step. Zork's
`return_value` (examples/zork/decode.c) steps the index
of the local it restores this way, `x -= 2`. That saves 3 bytes for a
step of 2 in zero page, and 4 for a step of 1.

# Where to run

Inside the asm-peephole fixedpoint, after `replace_pseudoregisters`
(so Pseudos are resolved to concrete addressing modes) and before
`expand_long_branches` (shrinks the program — no displacement
growth). The fold is monotone-shrinking (4 atoms → 2), so it's
safe in the fixed point.
"""
from __future__ import annotations

import asm_ast
from passes.asm_aliasing import unreachable_by_pointer
from passes.asm_liveness import (
    a_dead_at, c_dead_at, v_dead_at, x_dead_at, y_dead_at,
)


def apply_transfer_pm1_store(prog: asm_ast.Program) -> asm_ast.Program:
    unaliased = unreachable_by_pointer(prog)
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(
                _rewrite_step_then_reload(
                    _rewrite_steps_across_transfer(_rewrite_index_steps(
                        _rewrite_function(tl), unaliased,
                    ))
                )
            )
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


_INDIRECT = (
    asm_ast.Indirect, asm_ast.IndirectY, asm_ast.IndirectZp,
    asm_ast.IndirectZpY, asm_ast.Frame, asm_ast.Stack,
    asm_ast.IndexedData,
)


def _rewrite_index_steps(
    fn: asm_ast.Function, unaliased: frozenset[str],
) -> asm_ast.Function:
    """`LDA M; CLC; ADC #k; TAY` as `INY` k times (and the `SBC`, X
    forms) where Y still holds M. See "An index register that already
    holds the byte" above."""
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i + 3 < len(instrs):
        load, carry, arith, move = instrs[i:i + 4]
        if not (isinstance(load, asm_ast.Mov) and not load.is_volatile
                and isinstance(load.dst, asm_ast.Reg)
                and isinstance(load.dst.reg, asm_ast.A)
                and isinstance(load.src, (asm_ast.Data, asm_ast.ZP))
                and isinstance(move, asm_ast.Mov)
                and isinstance(move.src, asm_ast.Reg)
                and isinstance(move.src.reg, asm_ast.A)
                and isinstance(move.dst, asm_ast.Reg)
                and isinstance(move.dst.reg, (asm_ast.X, asm_ast.Y))):
            i += 1
            continue
        if isinstance(arith, asm_ast.Add) and isinstance(
            carry, asm_ast.ClearCarry,
        ):
            step = asm_ast.Inc
        elif isinstance(arith, asm_ast.Sub) and isinstance(
            carry, asm_ast.SetCarry,
        ):
            step = asm_ast.Dec
        else:
            i += 1
            continue
        if not (isinstance(arith.src, asm_ast.Imm)
                and arith.src.value in (1, 2)
                and isinstance(arith.dst, asm_ast.Reg)
                and isinstance(arith.dst.reg, asm_ast.A)):
            i += 1
            continue
        reg = move.dst
        after = i + 4
        if not (_holds(instrs, i, reg, load.src, unaliased)
                and a_dead_at(instrs, after) and c_dead_at(instrs, after)
                and v_dead_at(instrs, after)):
            i += 1
            continue
        instrs[i:i + 4] = [step(dst=reg)] * arith.src.value
        changed = True
        i += 1
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _small_step(carry, arith):
    """`(step, k)` when `carry; arith` is `CLC; ADC #k` (`Inc`) or `SEC;
    SBC #k` (`Dec`) on A with k of 1 or 2, else None."""
    if isinstance(arith, asm_ast.Add) and isinstance(carry, asm_ast.ClearCarry):
        step = asm_ast.Inc
    elif isinstance(arith, asm_ast.Sub) and isinstance(carry, asm_ast.SetCarry):
        step = asm_ast.Dec
    else:
        return None
    if not (isinstance(arith.src, asm_ast.Imm) and arith.src.value in (1, 2)
            and isinstance(arith.dst, asm_ast.Reg)
            and isinstance(arith.dst.reg, asm_ast.A)):
        return None
    return step, arith.src.value


def _transfer(ins, src_cls, dst_cls) -> bool:
    return (isinstance(ins, asm_ast.Mov) and not ins.is_volatile
            and isinstance(ins.src, asm_ast.Reg)
            and isinstance(ins.src.reg, src_cls)
            and isinstance(ins.dst, asm_ast.Reg)
            and isinstance(ins.dst.reg, dst_cls))


def _rewrite_steps_across_transfer(fn: asm_ast.Function) -> asm_ast.Function:
    """`CLC; ADC #k; TAY` as `TAY; INY`, and `TYA; CLC; ADC #k` as
    `INY; TYA` (k of 1 or 2, `SBC` as `DEY`, and the X forms). See
    "A step on either side of a transfer" above."""
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i + 2 < len(instrs):
        first, second, third = instrs[i:i + 3]
        after = i + 3
        rewritten = None
        found = _small_step(first, second)
        if (found is not None
                and _transfer(third, asm_ast.A, (asm_ast.X, asm_ast.Y))
                and a_dead_at(instrs, after) and c_dead_at(instrs, after)
                and v_dead_at(instrs, after)):
            step, k = found
            rewritten = [third] + [step(dst=third.dst)] * k
        found = _small_step(second, third)
        if (rewritten is None and found is not None
                and _transfer(first, (asm_ast.X, asm_ast.Y), asm_ast.A)):
            reg = first.src
            reg_dead = (x_dead_at if isinstance(reg.reg, asm_ast.X)
                        else y_dead_at)
            back = instrs[after] if after < len(instrs) else None
            step, k = found
            if (_transfer(back, asm_ast.A, type(reg.reg))
                    and a_dead_at(instrs, after + 1)
                    and c_dead_at(instrs, after + 1)
                    and v_dead_at(instrs, after + 1)):
                # `TXA; CLC; ADC #1; TAX` is `INX`.
                rewritten = [step(dst=reg)] * k
                after += 1
            elif (reg_dead(instrs, after) and c_dead_at(instrs, after)
                    and v_dead_at(instrs, after)):
                rewritten = [step(dst=reg)] * k + [first]
        if rewritten is None:
            i += 1
            continue
        instrs[i:after] = rewritten
        changed = True
        i += len(rewritten)
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _rewrite_step_then_reload(fn: asm_ast.Function) -> asm_ast.Function:
    """`LDA M; SEC; SBC #k; STA M; LDX M` as `LDX M; DEX; [DEX;] STX
    M` (k of 1 or 2, `ADC` as `INX`, and the Y forms). See "A step
    stored and loaded into an index register" above."""
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i + 4 < len(instrs):
        load, carry, arith, store, reload = instrs[i:i + 5]
        found = _small_step(carry, arith)
        if not (found is not None
                and isinstance(load, asm_ast.Mov) and not load.is_volatile
                and isinstance(load.src, (asm_ast.Data, asm_ast.ZP))
                and isinstance(load.dst, asm_ast.Reg)
                and isinstance(load.dst.reg, asm_ast.A)
                and store == asm_ast.Mov(src=load.dst, dst=load.src)
                and isinstance(reload, asm_ast.Mov)
                and not reload.is_volatile and reload.src == load.src
                and isinstance(reload.dst, asm_ast.Reg)
                and isinstance(reload.dst.reg, (asm_ast.X, asm_ast.Y))):
            i += 1
            continue
        after = i + 5
        if not (a_dead_at(instrs, after) and c_dead_at(instrs, after)
                and v_dead_at(instrs, after)):
            i += 1
            continue
        step, k = found
        reg = reload.dst
        instrs[i:after] = [reload] + [step(dst=reg)] * k + [
            asm_ast.Mov(src=reg, dst=load.src),
        ]
        changed = True
        i += k + 2
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _holds(instrs, i: int, reg, mem, unaliased) -> bool:
    """True when `reg` (X or Y) holds the byte `mem` at `instrs[i]`:
    walking back through the block, the last write to `reg` is a load
    of `mem`, and nothing after it writes `mem` or might."""
    from passes.asm_aliasing import may_alias
    from passes.asm_liveness import kills_x, kills_y, sets_up_y
    is_y = isinstance(reg.reg, asm_ast.Y)
    kills = kills_y if is_y else kills_x
    private = isinstance(mem, asm_ast.Data) and mem.name in unaliased
    for k in range(i - 1, -1, -1):
        ins = instrs[k]
        # A LoadAddress writes two bytes, which the check of `dst`
        # below would compare only the first of.
        if isinstance(ins, (asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
                            asm_ast.Return, asm_ast.Ret, asm_ast.Call,
                            asm_ast.FunctionPrologue,
                            asm_ast.AllocateStack, asm_ast.LoadAddress)):
            return False
        if (isinstance(ins, asm_ast.Mov) and ins.dst == reg
                and ins.src == mem and not ins.is_volatile):
            return True
        # Any write to the register, including `INX` / `DEX`, which
        # `kills_x` doesn't count since they read X too.
        if (kills(ins) or (is_y and sets_up_y(ins))
                or getattr(ins, "dst", None) == reg):
            return False
        dst = getattr(ins, "dst", None)
        if dst is None or isinstance(dst, asm_ast.Reg):
            continue
        if isinstance(dst, _INDIRECT):
            # Only an indexed store into `mem`'s own object reaches a
            # byte no pointer can.
            if not private or getattr(dst, "name", None) == mem.name:
                return False
            continue
        if dst == mem or may_alias(dst, mem):
            return False
    return False



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
    if not (c_dead_at(instrs, after) and v_dead_at(instrs, after)):
        return None
    return reg_cls, is_decrement, d.dst
