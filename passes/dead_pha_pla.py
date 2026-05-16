"""Drop dead `PHA` / `PLA` pairs.

`tac_to_asm._translate_indirect_indexed_store` emits the conservative
shape

    Mov(src, A)         ; LDA src
    Push(Reg(A))        ; PHA          (save src across idx load)
    Mov(idx, A)         ; LDA idx
    Mov(A, Reg(Y))      ; TAY
    Pop(Reg(A))         ; PLA          (restore src)
    Mov(A, IndirectY)   ; STA (DPTR),Y

The PHA/PLA is needed when `idx` lives in `Frame` (the LDA idx step
clobbers A) — that's why the lowering is conservative. But after
`direct_index_load` fuses `LDA idx; TAY` into a bare `LDY idx` (when
`idx` resolves to `Data` / `ZP` / `Imm`), the body between PHA and
PLA no longer touches A. The save/restore pair has nothing to do.

This peephole detects the post-fusion shape

    Push(Reg(A))                ; PHA
    <body that preserves A>     ; e.g. LDY Data
    Pop(Reg(A))                 ; PLA

and drops the PHA/PLA pair. The body's effect on Y / X / memory /
flags is unchanged; A keeps its pre-PHA value past the (now-deleted)
PLA, which is exactly what the original PHA-restore-PLA sequence
delivered.

# Eligibility

Match `Push(Reg(A))` at position `i`, scan forward to find
`Pop(Reg(A))` at position `j`. The body `instrs[i+1 .. j-1]` must:

* Not read `Reg(A)` (would observe a value preserved by the original
  PHA; after removal, A holds the same pre-PHA value, but the read
  is still the same value — so reads-of-A in the body are actually
  fine; the stricter "neither reads nor writes A" rule keeps the
  scan small and avoids a subtle case below).
* Not write `Reg(A)`. A write would mean PLA restores the original
  A while the body's write would be lost — dropping the pair would
  leak the body's write past position j. (This is the only hard
  constraint; everything else is bail-for-simplicity.)
* Not contain another `Push` / `Pop` — nested pairs complicate the
  match (the inner pair could mis-pair with the outer Pop). The
  fixed-point loop will re-fire on the surviving pairs after an
  inner pair collapses, so this restriction doesn't lose any folds.
* Not contain a `Call` (clobbers A; also stack-balance), `Ret` /
  `Return` (mid-stream control transfer — shouldn't happen anyway),
  or a `Label` / `Jump` / `Branch` (control-flow ambiguity: another
  predecessor could enter mid-body with A holding a different
  value).

After the body match, the N/Z flags from the original `PLA` (set
based on the saved A value) must be dead — no `Branch` may read N
or Z between position j and the next flag-setter. `flags_dead_at(
instrs, j + 1)` does the within-block walk. C and V flags aren't
affected by PHA / PLA, so no extra check needed there.

# Soundness

Before: PHA / body / PLA  → A=original, Y/X/memory=body-modified,
                           N/Z=from-original-A (PLA).
After:  body              → A=original (body preserved it),
                           Y/X/memory=body-modified,
                           N/Z=from-last-body-flag-setter (or unchanged
                           if body has no flag setter).

The A / Y / X / memory state is identical. The N/Z state may differ,
but the flag-dead check guarantees no observer reads the difference.

# Where to run

In `_peephole_fixedpoint`, after `direct_index_load` (which is what
turns the body into a flag-preserving `LDY Data` rather than the
A-clobbering `LDA Data; TAY`). The peephole runs in both the
optimized and unoptimized pipelines — the save/restore is wasted in
both, and the soundness check is local.
"""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import (
    flags_dead_at as _flags_dead_at,
    is_reg_a as _is_reg_a,
    kills_a as _kills_a,
    reads_a as _reads_a,
)


def apply_dead_pha_pla(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function top-level and drop dead PHA/PLA pairs."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    # Collect indices to drop. Two-pass shape mirrors direct_index_
    # load: pass 1 identifies droppable pairs, pass 2 rebuilds.
    drop: set[int] = set()
    i = 0
    while i < len(instrs):
        first = instrs[i]
        if isinstance(first, asm_ast.Push) and _is_reg_a(first.src):
            j = _find_matching_pop(instrs, i + 1)
            if j is not None and _flags_dead_at(instrs, j + 1):
                drop.add(i)
                drop.add(j)
                # Skip past the matched Pop so we don't re-examine
                # the body's instructions as candidate Push heads
                # this iteration (they aren't, but staying tidy).
                i = j + 1
                continue
        i += 1
    if not drop:
        return fn
    out = [ins for k, ins in enumerate(instrs) if k not in drop]
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _find_matching_pop(
    instrs: list[asm_ast.Type_instruction], start: int,
) -> int | None:
    """Scan forward from `start` looking for `Pop(Reg(A))`. Return
    its index if the intervening body preserves A and contains no
    control-flow / stack-balance bail conditions. Otherwise None."""
    for j in range(start, len(instrs)):
        instr = instrs[j]
        if isinstance(instr, asm_ast.Pop):
            if _is_reg_a(instr.dst):
                return j
            # Pop into another register — stack mismatch; bail.
            return None
        if isinstance(instr, asm_ast.Push):
            # Nested push — bail; the fixed-point loop will retry
            # after the inner pair collapses.
            return None
        if isinstance(instr, (
            asm_ast.Call, asm_ast.Ret, asm_ast.Return,
            asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
        )):
            return None
        if _reads_a(instr) or _kills_a(instr):
            return None
    return None
