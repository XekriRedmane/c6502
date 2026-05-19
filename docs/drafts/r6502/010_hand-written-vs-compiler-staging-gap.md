# A hand-written reference closed an optimization gap in my 6502 compiler

I've been comparing the output of c6502 (a C99 → 6502 compiler I'm
writing) against hand-written assembly from an Apple II game I'm
porting. Most of the time the compiler's output is competitive,
sometimes uglier, sometimes about the same byte count. Yesterday I
hit one where the gap was big enough to investigate.

The function: `compute_screen_x` — a 16-bit subtract that perspective-
transforms a sprite's world position into a screen position.

```c
static uint16_t compute_screen_x(uint8_t slot, uint8_t player_y,
                                 uint8_t sprite_xref)
{
    uint16_t xoff =
        ((uint16_t)perspective_xoff_hi[player_y] << 8)
        | perspective_xoff_lo[player_y];
    xoff = (uint16_t)(xoff - sprite_xref);
    uint16_t pos =
        ((uint16_t)companion_pos_hi[slot] << 8)
        | companion_pos_lo[slot];
    return (uint16_t)(pos - xoff);
}
```

The hand-written form (14 instructions, A and the carry threaded
naturally):

```asm
        LDY ZP_PLAYER_Y
        LDA PERSPECTIVE_XOFF_LO,Y
        SEC
        SBC ZP_SPRITE_XREF
        STA ZP_SCREEN_X
        LDA PERSPECTIVE_XOFF_HI,Y
        SBC #$00
        STA ZP_SCREEN_X_HI
        SEC
        LDA ZP_COMPANION_POS_LO,X
        SBC ZP_SCREEN_X
        STA ZP_SCREEN_X         ; reuse for the final result
        LDA ZP_COMPANION_POS_HI,X
        SBC ZP_SCREEN_X_HI
```

What c6502 was emitting (24 instructions):

```asm
   LDX   __zpabi_compute_screen_x__player_y
   LDA   perspective_xoff_lo,X
   STA   __local_compute_screen_x__1
   LDA   perspective_xoff_hi,X
   STA   __local_compute_screen_x__0
   LDA   __local_compute_screen_x__1   ; reload!
   SEC
   SBC   __zpabi_compute_screen_x__sprite_xref
   STA   __local_compute_screen_x__3
   LDA   __local_compute_screen_x__0   ; reload!
   SBC   #$00
   STA   __local_compute_screen_x__2
   ; ... and again for the pos - xoff stage ...
```

Same shape, just with two extra loads per byte: pre-load both
operand bytes into ZP scratch slots, then reload each for its
subtract. 10 extra instructions = ~20 extra cycles per call.

## Where the bloat lives

The TAC-level lowering of multi-byte ADC/SBC chains already does
the byte-interleaved emission — `_translate_add_sub` in
`tac_to_asm.py` walks the bytes low → high, emitting
`LDA src1[k]; ADC/SBC src2[k]; STA dst[k]` per byte. The carry
threads from one byte to the next naturally.

So the IR coming out of `tac_to_asm` was already the right shape.
The bloat appeared LATER, in the asm-SSA pipeline.

The asm-SSA construction renames every byte-versioned value, the
regalloc colors each SSA name to a ZP byte, and SSA destruction
inserts Movs to bridge between SSA names with different colors.
For values that flow through the IR as Pseudos (TAC temps), the
destruction stage emits explicit staging stores — even when the
value could be recomputed cheaply at each use.

A separate pass, `apply_remat`, was supposed to clean this up:
match a `Mov(<recomputable>, Data(__local_<fn>__*))` def, look
forward for the first `Mov(Data(<same>), <any>)` use, and rewrite
the use to read from `<recomputable>` directly.

Eligibility includes `IndexedData(name, off, X|Y)` when the
array's name isn't written in the function and the index register
isn't written between def and use. This was already covering the
single-atom mem-to-mem case (a `Mov` whose src and dst are both
memory operands — one IR atom that emits as `LDA src; STA dst`).

But after SSA destruction, the staging shape was often two atoms:

```python
Mov(IndexedData(arr, X), Reg(A))   # producer: LDA arr,X
Mov(Reg(A), Data(__local__))        # staging def: STA local
```

`apply_remat` looked at the second instruction (the staging def),
saw `src = Reg(A)`, and bailed — `Reg(A)` isn't in its
recomputable-source set, and rightly so (A is a live register, not
a known immutable load).

## The fix

Extend `_classify_stage_def` to look one step back when the
staging def's src is `Reg(A)`. If the prior instruction is a
`Mov(<recomputable>, Reg(A))` producer with no other A-writes in
between, treat the producer's src as the recomputable value and
extend the validation range back to the producer's index. The
existing `_can_remat` then validates the whole range (arr
immutable, X/Y stable, no Call) and the rewrite proceeds as
before.

One ancillary fix: when the staging def's `<src>` is `Reg(A)`,
the dead-stage-dst sweep that previously collapsed
`Mov(<src>, Data(local))` → `Mov(<src>, Reg(A))` would produce a
useless `Mov(Reg(A), Reg(A))` self-Mov that no IR peephole drops.
Special-case: when src is Reg(A), omit the instruction outright —
the producer already left A holding the value.

After the fix, `compute_screen_x` emits exactly 14 instructions:

```asm
   LDX   __zpabi_compute_screen_x__player_y
   LDA   perspective_xoff_lo,X
   SEC
   SBC   __zpabi_compute_screen_x__sprite_xref
   STA   __local_compute_screen_x__3
   LDA   perspective_xoff_hi,X
   SBC   #$00
   STA   __local_compute_screen_x__2
   LDX   __zpabi_compute_screen_x__slot
   LDA   companion_pos_lo,X
   SEC
   SBC   __local_compute_screen_x__3
   STA   HARGS
   LDA   companion_pos_hi,X
   SBC   __local_compute_screen_x__2
   STA   HARGS+1
   RTS
```

Same byte count as hand-written. The only structural difference:
c6502 stores intermediate `xoff` bytes in two separate ZP slots
(`__local_*__3` and `__local_*__2`), while the hand-written code
reuses `ZP_SCREEN_X` / `ZP_SCREEN_X_HI` as scratch. Functionally
equivalent; the ZP-slot choice is the regalloc's call.

Across two examples in my corpus, this saved 14 lines of emitted
asm. Not a huge win in absolute terms, but the
hand-written-equivalent output is what I want from this compiler
for game-engine code.

If your compiler has a similar staging-then-cleanup pass, it's
worth checking whether it handles both the single-atom mem-to-mem
shape AND the two-atom post-SSA-destruction shape. The two-atom
case is what falls out of a register-allocated SSA round-trip; the
single-atom case is what falls out of straight-line code without
the round-trip. You'll see both in any modern compiler.

Repo: <https://github.com/XekriRedmane/c6502>
