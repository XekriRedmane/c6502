# A 58% codegen size reduction from one peephole and a stale comment

Catch-up post on my C99-to-6502 compiler. While compiling a small
game-engine helper, I noticed the codegen was doing this around every
indirect-indexed store:

```asm
LDA   #$01
PHA
LDY   __zpabi_spawn_pos_dir_p0
PLA
STA   (__local_spawn_pos_dir_b0),Y
```

Save A on the hardware stack, set up Y, restore A, write. The PHA/PLA
guards `Reg(A)` across an `LDY` that doesn't touch A. It's pure waste.

## Why it's there

The lowering for `arr[i] = val` through a runtime pointer (the
`(zp),Y` shape) is in `tac_to_asm._translate_indirect_indexed_store`:

```python
return [
    *self._stage_dptr(ptr_op),
    asm_ast.Mov(src=src_op, dst=_REG_A),       # LDA val
    asm_ast.Push(src=_REG_A),                  # PHA  — save val
    asm_ast.Mov(src=idx_op, dst=_REG_A),       # LDA idx
    asm_ast.Mov(src=_REG_A, dst=Reg(Y)),       # TAY
    asm_ast.Pop(dst=_REG_A),                   # PLA  — restore val
    asm_ast.Mov(                                # STA (DPTR),Y
        src=_REG_A, dst=asm_ast.IndirectY(),
        is_volatile=is_volatile,
    ),
]
```

The lowering is conservative because `idx` is still a *pseudo* at
this point — register allocation hasn't run yet. If `idx` ends up in
a stack frame, the `LDA idx` step clobbers A (frame access uses
indirect-Y addressing and the LDA-#offset setup sets Y too). The
PHA/PLA pair is the safe fallback.

The lowering even called this out:

> "An asm-level peephole could collapse the save/restore when both
> operands prove to be ZP-resident post-regalloc — deferred."

## What changed after regalloc

I already had a peephole called `direct_index_load` that runs after
`replace_pseudoregisters`. It fuses

```asm
LDA M
TAX        ; or TAY
```

into a direct `LDX M` / `LDY M` when M resolves to `Imm`, `Data`, or
`ZP` (the addressing modes LDX/LDY support). `LDA; TAX/TAY` sets N/Z
twice from M's value, ending up identical to `LDX/LDY M`'s flag
state — so flag soundness is free.

After `direct_index_load` fires on the indirect-indexed-store output,
the sequence becomes

```asm
LDA   #$01                  ; Mov(Imm($01), A)
PHA                         ; Push(A)
LDY   __zpabi_..._p0        ; Mov(Data, Y)  — direct LDY!
PLA                         ; Pop(A)
STA   (...)Y                ; Mov(A, IndirectY)
```

The body between PHA and PLA is now a single `Mov(Data, Reg(Y))`. It
doesn't touch A. The save/restore has nothing to do.

## The new peephole

`apply_dead_pha_pla` matches a `Push(Reg(A)); body; Pop(Reg(A))`
triple and drops both when:

* The body doesn't read `Reg(A)`.
* The body doesn't write `Reg(A)`.
* No nested `Push` / `Pop` / `Call` / `Ret` / `Label` / `Jump` /
  `Branch` in the body — keep the match window straight-line and the
  stack invariants honest.
* The PLA's N/Z flag effect is dead at +1 (forward flag-liveness walk
  via the existing `flags_dead_at` helper).

The flag check matters. PLA sets N/Z based on the *restored* A
value. Before the peephole:

```
LDA val   ; N/Z from val
PHA       ; flags preserved
body      ; flags F'
PLA       ; N/Z from val (PLA reloads + sets flags)
```

After:

```
LDA val   ; N/Z from val
body      ; flags F'
```

If anything downstream reads N/Z before a fresh flag-setter, before
sees val's flags and after sees the body's flags. Different. The
`flags_dead_at` check rejects the fold when any forward `Branch`
could observe the difference; everything else (STA, LDA, JMP, RTS)
either sets fresh flags or doesn't read them, so the within-block
forward walk terminates quickly.

C and V flags are untouched by PHA / PLA — no extra check needed.

## Results

The motivating example dropped from 147 to 62 lines (the function
also dropped its prologue/epilogue because it's `__attribute__((
zp_abi))`, but the per-store savings account for most of the
reduction). Two other examples in the test corpus picked up smaller
hits: 1 pair in one, 9 in another. The end-to-end behavioral
simulator confirmed semantics unchanged on all three before I
regenerated the gold-file snapshots.

The peephole itself is small enough to quote in full
(`passes/dead_pha_pla.py`, body-scan plus the flags check). Runs in
the always-on peephole fixed-point loop, after
`apply_direct_index_load` produces the precondition. Composes with
itself: a nested PHA/PLA inner pair gets dropped first, then the
next iteration sees the outer pair with no body and drops it too.

## Discussion

Curious whether other small-target compilers handle this with a more
principled approach — e.g., delaying the conservative spill until
post-regalloc instead of emitting PHA/PLA speculatively and relying
on a peephole to clean up. I went with the speculative-then-peephole
shape because the lowering is shared between optimized and
unoptimized pipelines, and the peephole is a clean local rewrite.
But "emit it correctly the first time" appeals.

Also curious about the flag-deadness gate. Mine is conservative —
even when the body is provably flag-clobbering (e.g., the body's
last instruction is `Mov(Data, Reg(Y))` which sets N/Z from the
loaded byte), I still check `flags_dead_at` rather than reasoning
that the body itself overwrites the flag state. Was simpler to write
and shouldn't lose folds in practice (anything that reads flags
across this kind of code is rare), but feels like it could be
tightened.

If you're interested in the code, it's at
`passes/dead_pha_pla.py` with tests in
`tests/test_dead_pha_pla.py`. Always-on, ~50 lines.
