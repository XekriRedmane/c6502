"""Halve a 9-bit sum with `ROR` of its low byte, taking bit 8 from C.

# Pattern

`(x + y) / 2` or `(x + y) >> 1` with `x` and `y` bytes (or one of them
a constant below 256) promotes both to `int`, so the sum has a high
byte, which is only the carry out of the low bytes' add. When the
result is truncated to a byte, `tac_to_asm` and the asm optimizer
leave the high byte's add and its shift:

    ADC   s                 ; the low byte of the sum, C = bit 8
    STA   t
    LDA   #$00
    ADC   #$00              ; the high byte, 0 + 0 + C
    LSR                     ; C = bit 8 again, A = 0
    LDA   t
    ROR                     ; A = bit 8 and bits 7-1 of the sum

`print_centered_plus_col` in `examples/ali_baba/text.c` rounds
`(space + 1) / 2` that way. The carry out of the `ADC` is already bit
8, so the pass drops the four instructions between it and the `ROR`:

    ADC   s
    ROR

# Soundness

After the `ADC s`, A is the low byte `L` of the sum and C its bit 8,
`B`. In the original, `LDA #$00; ADC #$00` makes A = `B` and clears C
and V, `LSR` makes C = `B` and A = 0, `LDA t` makes A = `L`, and `ROR`
makes A = `B << 7 | L >> 1` and C = bit 0 of `L`, with N and Z from
the new A. The rewrite makes the same A, C, N, and Z from the same `L`
and `B`. The differences are:

  * V, which the original leaves clear (from `ADC #$00`) and the
    rewrite leaves as the `ADC s` set it. V must be dead after the
    `ROR` (`asm_liveness.v_dead_at`).
  * `t`, which the rewrite doesn't write. `t` must be `Data` or `ZP`
    and dead after the `LDA t` on every path
    (`asm_dead_store.mem_dead_after`), and the `LDA t` mustn't be
    volatile.

The seven instructions are consecutive, so no label or branch is
between them. The `ADC s` stays as it was, with whatever set C before
it, so a `SEC` in front of it (`(x + y + 1) / 2` folded into the
carry) works the same. `s` is read once, as before.

The `LDA #$00` may be a `TXA` or `TYA`, which
`redundant_load_elimination` makes of it when X or Y holds 0. The
pass takes one when the last instruction in the block before it that
can write the register is `LDX #$00` or `LDY #$00`.

# Where to run

In the asm peephole fixed point, which both pipelines run. The high
byte is often stored and loaded again (`STA u; LDA u`) between the
`ADC #$00` and the `LSR` in the first sweep, and
`round_trip_load_drop` removes that pair for a later sweep.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import kills_x, kills_y, sets_up_y, v_dead_at

_A = asm_ast.Reg(reg=asm_ast.A())
_ZERO = asm_ast.Imm(value=0)


def apply_carry_halve(
    prog: asm_ast.Program,
    *,
    zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Drops the high byte's add and shift of each halved 9-bit sum
    in every function. `zp_slot_symbols` is passed to
    `mem_dead_after`."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl, zp_slot_symbols)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _holds_zero(instrs, idx: int, reg) -> bool:
    """True when `reg` (X or Y) holds 0 at `instrs[idx]`, because the
    last instruction before it in its block that can write `reg` is
    `Mov(Imm(0), reg)`. A label ends the search, since another path
    may reach it."""
    name = "X" if isinstance(reg.reg, asm_ast.X) else "Y"
    for k in range(idx - 1, -1, -1):
        ins = instrs[k]
        if isinstance(ins, asm_ast.Mov) and ins.dst == reg:
            return ins.src == _ZERO and not ins.is_volatile
        if isinstance(ins, (asm_ast.Label, asm_ast.Call, asm_ast.Jump,
                            asm_ast.JumpTable, asm_ast.Phi)):
            return False
        if isinstance(ins, (asm_ast.Inc, asm_ast.Dec)) and ins.dst == reg:
            return False
        if name == "X" and kills_x(ins):
            return False
        if name == "Y" and (kills_y(ins) or sets_up_y(ins)):
            return False
    return False


def _is_zero_load(instrs, idx: int) -> bool:
    """True when `instrs[idx]` loads A with 0: `LDA #$00`, or `TXA` /
    `TYA` with that register holding 0 (`_holds_zero`)."""
    ins = instrs[idx]
    if not (isinstance(ins, asm_ast.Mov) and ins.dst == _A):
        return False
    if ins.src == _ZERO:
        return True
    return (isinstance(ins.src, asm_ast.Reg)
            and isinstance(ins.src.reg, (asm_ast.X, asm_ast.Y))
            and _holds_zero(instrs, idx, ins.src))


def _matches(instrs, i: int) -> bool:
    """True when `instrs[i:i + 7]` is `ADC s; STA t; LDA #0; ADC #0;
    LSR A; LDA t; ROR A`, with `t` a `Data` or `ZP` byte, and the
    `LDA #0` possibly a `TXA` or `TYA` of a register holding 0."""
    add, store, _zero, carry, lsr, load, ror = instrs[i:i + 7]
    t = getattr(store, "dst", None)
    return (
        isinstance(add, asm_ast.Add) and add.dst == _A
        and isinstance(store, asm_ast.Mov) and store.src == _A
        and not store.is_volatile
        and isinstance(t, (asm_ast.Data, asm_ast.ZP))
        and _is_zero_load(instrs, i + 2)
        and isinstance(carry, asm_ast.Add) and carry.src == _ZERO
        and carry.dst == _A
        and isinstance(lsr, asm_ast.LogicalShiftRight) and lsr.dst == _A
        and isinstance(load, asm_ast.Mov) and load.src == t
        and load.dst == _A and not load.is_volatile
        and isinstance(ror, asm_ast.RotateRight) and ror.dst == _A
    )


def _rewrite_function(fn: asm_ast.Function, zp_slot_symbols):
    from passes.asm_dead_store import mem_dead_after
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i + 7 <= len(instrs):
        window = instrs[i:i + 7]
        if not (_matches(instrs, i)
                and v_dead_at(instrs, i + 7)
                and mem_dead_after(instrs, i + 5, window[1].dst,
                                   fn_name=fn.name,
                                   zp_slot_symbols=zp_slot_symbols)):
            i += 1
            continue
        instrs[i:i + 7] = [window[0], window[6]]
        changed = True
        i += 2
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )
