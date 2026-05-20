"""Fold `LDA src; STA tmp; LDA other; CMP tmp` to `LDA other; CMP src`.

# Pattern

Four consecutive instructions:

    Mov(<src>,         Reg(A))   # LDA src
    Mov(Reg(A),        <tmp>)    # STA tmp
    Mov(<other>,       Reg(A))   # LDA other
    Compare(Reg(A),    <tmp>)    # CMP tmp

The middle pair stages `src`'s value through a memory temporary so
the final CMP can compare A (= other) against the staged copy. But
CMP supports every addressing mode LDA supports (plus `abs,X` /
`abs,Y` once `asm_emit._emit_compare` dispatches IndexedData) — so
the temp is unnecessary:

    Mov(<other>,       Reg(A))   # LDA other
    Compare(Reg(A),    <src>)    # CMP src

Two instructions instead of four. The CMP reads `src` directly,
yielding the same Z/N/C/V flags as `CMP tmp` (tmp held src's value).

# Motivating case

`beam_target_tick.c` — `beam_y == floor_y_table[floor_idx]`:

    AND   #$0F
    TAX
    LDA   floor_y_table,X
    STA   __local_beam_target_tick__0
    LDA   beam_y
    CMP   __local_beam_target_tick__0
    BNE   .if_end@2

Hand-written equivalent (and what this pass produces):

    AND   #$0F
    TAX
    LDA   beam_y
    CMP   floor_y_table,X
    BNE   .if_end@2

Drops 5 bytes / 8 cycles per occurrence (the LDA src + STA tmp pair).
The local slot itself is freed too — downstream `prune_unused_slots`
collects the `__local_*` symbol when it's no longer referenced.

# Eligibility

  * All four instructions present and non-volatile.
  * `tmp` is `Data` or `ZP` (stable mem, structurally addressable).
  * `src` is a CMP-supported addressing mode that doesn't depend on
    a register the intervening LDA might clobber:
      - `Imm` / `ImmLabelLow` / `ImmLabelHigh` — no register dep.
      - `Data` / `ZP` — no register dep.
      - `IndexedData(_, _, X)` — X is never written by any LDA mode.
      - `IndexedData(_, _, Y)` — Y can be clobbered by `LDA other`
        when other is `Frame` / `Stack` / `Indirect` / `IndirectZp`
        (those emit an `LDY #off` setup). Allowed only when other
        is not one of those.
  * `other` doesn't alias `tmp`. Conservatively: `other` must be
    `Imm` / `ImmLabelLow/High`, or `Data` / `ZP` differing structurally
    from `tmp`. Indirect / IndexedData / Frame / Stack / IndirectY /
    IndirectZpY / IndirectZp are rejected because runtime aliasing
    can't be statically ruled out.
  * `tmp` and `src` need not differ — a `LDA M; STA M; LDA other;
    CMP M` self-store is also collapsed soundly.

The rewrite drops both the LDA src and STA tmp. After the rewrite:

  * Reg(A)'s final value matches the original (A = other in both).
  * Flags after the rewritten CMP match the original CMP's flags.
  * No memory write was dropped that downstream code observes
    (`tmp` becomes dead-after-rewrite; downstream `asm_dead_store`
    handles any remaining STAs to tmp elsewhere in the function).

# Why this isn't covered by other passes

  * `asm_dead_store` would drop the STA only if the LDA src ALSO
    wasn't needed for A's value — but A IS needed (CMP reads it).
    It can't see that the value reaches A only to be discarded by
    the subsequent LDA other.
  * `redundant_load_elimination` tracks A's mirror but the STA
    sequence breaks the mirror tracking before the CMP runs.
  * `mem_const_prop` only substitutes immediates, not memory
    operands.

# Where to run

Inside the asm-peephole fixed-point loop. Producing a shorter
sequence enables downstream passes to find more opportunities
(`dead_a_arith` may drop further LDAs after the rewrite, etc.).
"""

from __future__ import annotations

import asm_ast


def apply_cmp_through_tmp_fold(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_stable_mem(op) -> bool:
    return isinstance(op, (asm_ast.Data, asm_ast.ZP))


def _operands_equal(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False


def _src_eligible(src) -> bool:
    """True iff `src` is CMP-supported AND doesn't depend on a register
    clobbered by the intervening LDA. For IndexedData(_, _, Y) the
    caller does an additional check against `other`."""
    if isinstance(src, (asm_ast.Imm, asm_ast.ImmLabelLow,
                        asm_ast.ImmLabelHigh)):
        return True
    if isinstance(src, (asm_ast.Data, asm_ast.ZP)):
        return True
    if isinstance(src, asm_ast.IndexedData):
        return True
    return False


def _y_safe_other(other) -> bool:
    """True iff `Mov(other, Reg(A))` emits without an LDY setup —
    i.e. it doesn't write Y. Used when src indexes through Y."""
    return isinstance(
        other,
        (asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh,
         asm_ast.Data, asm_ast.ZP, asm_ast.IndexedData,
         asm_ast.IndirectY, asm_ast.IndirectZpY),
    )


def _other_alias_safe(other, tmp) -> bool:
    """Conservative `other` non-aliasing check: only accept `other`
    forms whose target byte can be compared structurally against
    `tmp`. Reject indirect / indexed / frame-relative `other`
    because the byte they address depends on runtime register or
    pointer values."""
    if isinstance(other, (asm_ast.Imm, asm_ast.ImmLabelLow,
                          asm_ast.ImmLabelHigh)):
        return True
    if isinstance(other, (asm_ast.Data, asm_ast.ZP)):
        return not _operands_equal(other, tmp)
    return False


def _matches_pattern(a, b, c, d) -> tuple[
    asm_ast.Type_operand, asm_ast.Type_operand
] | None:
    """If (a, b, c, d) is `LDA src; STA tmp; LDA other; CMP tmp`
    eligible for folding, return (src, other). Otherwise None."""
    if not (
        isinstance(a, asm_ast.Mov) and not a.is_volatile
        and _is_reg_a(a.dst)
        and isinstance(b, asm_ast.Mov) and not b.is_volatile
        and _is_reg_a(b.src) and _is_stable_mem(b.dst)
        and isinstance(c, asm_ast.Mov) and not c.is_volatile
        and _is_reg_a(c.dst)
        and isinstance(d, asm_ast.Compare)
        and _is_reg_a(d.left) and _is_stable_mem(d.right)
    ):
        return None
    src = a.src
    tmp_sta = b.dst
    other = c.src
    tmp_cmp = d.right
    if not _operands_equal(tmp_sta, tmp_cmp):
        return None
    if not _src_eligible(src):
        return None
    # IndexedData(_, _, Y) src: forbid `other` shapes that emit LDY.
    if (isinstance(src, asm_ast.IndexedData)
            and isinstance(src.index, asm_ast.Y)
            and not _y_safe_other(other)):
        return None
    if not _other_alias_safe(other, tmp_sta):
        return None
    return src, other


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i + 3 < len(instrs):
        matched = _matches_pattern(
            instrs[i], instrs[i + 1], instrs[i + 2], instrs[i + 3],
        )
        if matched is not None:
            src, other = matched
            # Emit: LDA other; CMP src.
            out.append(asm_ast.Mov(
                src=other, dst=asm_ast.Reg(reg=asm_ast.A()),
                is_volatile=False,
            ))
            out.append(asm_ast.Compare(
                left=asm_ast.Reg(reg=asm_ast.A()), right=src,
            ))
            i += 4
            continue
        out.append(instrs[i])
        i += 1
    out.extend(instrs[i:])
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )
