# Title

From 30 instructions to (almost) the hand-written 7 — closing the gap on a speaker-tone routine

# Body

I'm writing a C99 compiler that targets the 6502 (project's `c6502`,
Python). Last session I ported an Apple II speaker-click delay
routine and got the compiler producing roughly the right *shape* of
output, but it was about 4× longer than the hand-written reference.
This session I closed most of the gap. Here's the walkthrough.

The C source:

```c
extern const volatile uint8_t *sfx_click_ptr;

__attribute__((zp_abi))
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        volatile uint8_t y = pitch;
        while (--y != 0) { }       /* delay loop */
        (void)*sfx_click_ptr;      /* speaker click */
    } while (--duration != 0);
}
```

Hand-written reference (7 instructions, ~9 bytes, the kind of code a
real 6502 programmer writes):

```
SFX_TONE:
        TAY                     ; Y = pitch (A holds pitch)
.delay: DEY
        BNE .delay              ; inner delay loop
        CMP (ZP_SFX_CLICK),Y    ; Y=0, read speaker (no clobber of A)
        DEX
        BNE SFX_TONE            ; A is still pitch, TAY at the top reloads Y
        RTS
```

The clever choices:
- A holds `pitch` across the entire function (never clobbered).
- Y is the inner counter — `DEY`/`BNE` is the tightest inner-loop
  shape on a 6502.
- The speaker read uses `CMP (zp),Y` instead of `LDA (zp),Y`. CMP
  also reads through the pointer but **doesn't clobber A** — so A
  can stay holding `pitch` for the next outer iteration.
- X is the outer counter via `DEX`/`BNE`.
- The label `ZP_SFX_CLICK` is — as the name announces — at zero
  page, so `(zp),Y` works directly without DPTR staging.

## Where my compiler started

```
sfx_tone:
   SUBROUTINE
.preheader:
   LDA __zpabi_sfx_tone_p1
   STA __local_sfx_tone_b2
.loop@0_start:
   LDA __zpabi_sfx_tone_p0
   STA __local_sfx_tone_b1
.loop@1_continue:
   LDA __local_sfx_tone_b1
   SEC
   SBC #$01
   STA __local_sfx_tone_b0
   LDA __local_sfx_tone_b0
   STA __local_sfx_tone_b1
   LDA __local_sfx_tone_b0
   BEQ .loop@1_break
   JMP .loop@1_continue
.loop@1_break:
   LDA sfx_click_ptr
   STA DPTR
   LDA sfx_click_ptr+1
   STA DPTR+1
   LDY #$00
   LDA (DPTR),Y
.loop@0_continue:
   DEC __local_sfx_tone_b2
   BNE .split
   RTS
.split:
   JMP .loop@0_start
```

~30 instructions. Three optimization opportunities I tackled:

## 1. LICM the DPTR staging out of the loop

`sfx_click_ptr` is a global pointer that doesn't change inside the
function. The four-instruction `LDA ptr; STA DPTR; LDA ptr+1; STA
DPTR+1` chain is loop-invariant, but my LICM-lite pass only handled
constant stores (`LDA #c; STA M`), not memory-to-memory copies.

Extending `_match_candidate` to also accept `Mov(Data|ZP, A); Mov(A,
Data|ZP)` and additionally check that the *source* cell isn't
written inside the body got the staging out of the loop. Hoist
condition: source cell never written in body, dest cell only
written by this pair, no Call in body, single-entry. ~30 lines of
new code in `asm_licm.py` plus tests.

After: 4 instructions × `duration` iterations saved.

## 2. The Z-flag tracker

The inner loop had 9 instructions; my redundant-load pass was
holding back. Look at this slice:

```
STA b0
LDA b0         ; <-- A already has b0's value (from STA)
STA b1   ; volatile
LDA b0         ; <-- A still has b0's value, AND Z still reflects it
BEQ break
```

My existing tracker recognized that A mirrors `b0` after the STA,
so the next LDA's *value* effect is redundant. But it conservatively
kept the LDA because dropping it would change the Z flag at the
BEQ. Conservative because Z was already in the right state from the
preceding SBC + STA chain — but the analyzer didn't track *what Z
reflects*, only whether anyone reads it.

Added a parallel `z_reflects: list[Type_operand]` field to the
tracker. Each entry means "Z's current value matches (this operand
== 0)". Update rules per instruction:

- `LDA M`: `z_reflects = [M]`
- `STA M` from A: append M (A's value === M now)
- `SBC` / `ADC` / `AND` / `OR` / `EOR`: clear (Z reflects A's new
  value, but A's identity is no longer tracked)
- `INC M` / `DEC M`: `z_reflects = [M]`
- `Compare` / `BitTest`: clear (Z meaning doesn't fit "operand
  zeroness")

When a candidate LDA M arrives, drop iff `state.a` contains M
(existing check) AND z_reflects contains M (new check) OR
`_flags_dead_at(next instruction)` (existing fallback).

One redundant LDA dropped per iteration of every loop with this
shape. `floor_enemy_advance` and `floor_enemy_draw` also shrunk.

## 3. Volatile keeps the inner loop honest

Earlier session detail: my compiler was happily eliminating the
entire `while (--y != 0) {}` loop because the body was empty and
my dead-pure-loop pass didn't know `y` was volatile. I plumbed an
`is_volatile` bit through the c99 type system, into the TAC IR
(on every Load / Store / IndexedLoad / IndexedStore /
IndirectIndexedLoad / IndirectIndexedStore), and onto every asm
`Mov` atom. All the optimization passes that could elide or
coalesce a volatile access now skip volatile-flagged atoms.

Result after this session: the inner loop is now 8 instructions
(down from 9), and the DPTR staging is once-per-function instead of
once-per-outer-iteration.

## What's left

The remaining gap to the hand-written 7 instructions:

1. **`volatile uint8_t y` lives in memory, not Y register.** Strict
   C99 says every access to a volatile object is a side effect, so
   the compiler can't put `y` in a register. The hand-written
   version uses `DEY`/`BNE` because the author manually chose to
   represent "cycle-observable but not value-observable" via
   register usage. Closing this would mean a non-strict mode that
   recognizes "this volatile is purely a delay counter, no observer
   can see individual writes" — a real design call.

2. **The "use A directly" trick for the speaker read.** The
   hand-written uses `CMP (zp),Y` so A is preserved. My compiler
   uses `LDA (zp),Y` which clobbers A. A peephole that converts
   `LDA (zp),Y` to `CMP (zp),Y` when the loaded value is dead but
   A is live would help — but only if a downstream optimization
   was caching pitch in A across iterations. Today there isn't.

3. **Hidden LDA inside mem-to-mem Mov atoms.** My asm IR has
   `Mov(M1, M2)` as a single atom that lowers to `LDA M1; STA M2`
   at emit time. The implicit LDA is invisible to peephole passes,
   which is why `round_trip_load_drop` can't catch one of the
   redundant LDAs in the inner loop. Fix would be to either expand
   mem-to-mem before the peephole, or recognize the pattern
   "Mov(M1, M2) where A already === M1" and rewrite to `Mov(A, M2)`.

4. **The `DEC __local_*` for the outer counter is fine, but the
   hand-written `DEX` is one cycle faster.** My `loop_counter_to_x`
   pass promotes loop counters to X, but it's not firing here.
   Need to investigate why — possibly the volatile-y machinations
   inside the inner loop are disqualifying it.

The gap is now ~16 instructions, down from ~23 last session. The
remaining gap is mostly the "C semantics force memory" issue plus
two specific peepholes. Most of it could be closed with another
half-day of work.

Has anyone else done the CMP-as-read trick in a compiler? I haven't
seen it documented as a peephole — it's so 6502-specific that I'd
expect it to come up in 6502-targeted compilers but my searches
turned up empty. Curious whether it's standard prior art or actually
novel.
