# Five coupled passes for one 30-line function

Another c6502 (C99-to-6502 compiler) update. This time the target
function was small enough that the hand-written original and the
compiler's output could be diffed line by line, which made it a
good lens on what was missing in the optimization pipeline.

## The function

`apply_bobble` updates an enemy's screen Y from a signed-magnitude
delta table:

```c
extern uint8_t entity_floor_pos[20];
extern const uint8_t rescue_bobble[];   /* bit 7 = descend, low 7 = magnitude */

__attribute__((zp_abi))
static void apply_bobble(uint8_t slot, uint8_t bobble_idx) {
    uint8_t bobble    = rescue_bobble[bobble_idx];
    uint8_t magnitude = bobble & 0x7F;
    if (bobble & 0x80) {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] + magnitude);
    } else {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] - magnitude);
    }
}
```

Both params are uchar; with c6502's `zp_abi` attribute they ride in
private ZP slots, and the function emits as a bare body + RTS — no
soft-stack prologue.

## What the compiler emitted (before)

```asm
.apply_bobble@asm_ssa_block@0:
   LDX   __zpabi_apply_bobble_p1
   LDA   rescue_bobble,X
   STA   __local_apply_bobble_b0       ; spill bobble
   AND   #$7F
   STA   __local_apply_bobble_b1       ; spill magnitude
   LDA   __local_apply_bobble_b0       ; reload bobble for branch test
   BPL   .if_else@1
.add_path:
   LDX   __zpabi_apply_bobble_p0       ; X = slot
   LDA   entity_floor_pos,X
   CLC
   ADC   __local_apply_bobble_b1
   STA   entity_floor_pos,X
   JMP   .if_end@0
.if_else@1:
   LDX   __zpabi_apply_bobble_p0       ; X = slot (reload)
   LDA   entity_floor_pos,X
   SEC
   SBC   __local_apply_bobble_b1
   STA   entity_floor_pos,X
.if_end@0:
   RTS
```

19 instructions. Two `__local_*` slots (one for `bobble`, one for
`magnitude`), spilled and reloaded around the branch test. Slot is
LDX'd separately in each arm.

The hand-written equivalent has zero locals: bobble lives in A
straight through the BPL, magnitude is computed in each branch
where it's needed, and slot is pinned to Y so the per-branch
reload disappears.

## Five passes to close the gap

**1. TAC sinker for `BitwiseAnd` past `JumpIfMasked`.** The C
sequence above lowers to TAC as `bobble = rescue_bobble[i]; magnitude
= bobble & 0x7F; if (bobble & 0x80) ...`. The `magnitude` def is
computed before the branch so it's available in both arms, which
keeps `bobble`'s live range crossing the AND — forcing the spill.
Sinking the AND into each arm shrinks `bobble`'s live range to the
LDA right before the BPL.

**2. ADC commutativity peephole.** Once the sinker fires, each arm
has the pattern `STA temp; [intervening]; LDA mem; CLC; ADC temp;
STA mem`. ADC is commutative — if A still holds the value we'd be
spilling, we can drop the STA temp and the LDA mem, rewriting to
`CLC; ADC mem; STA mem` directly. Also works for AND / ORA.

Discovery while wiring this up: c6502's emit + sim assembler
didn't have dispatch for `ADC abs,X|Y`, even though the opcodes
($7D, $79) were in the lookup tables. The lowering had never
produced those operand shapes before, so the dispatch fell
through to "unsupported." Added in a few lines.

**3. Cross-block A-tracking in `redundant_load`.** Even with (1)
and (2), `bobble` was still being spilled (because the BPL block
exits with A holding `bobble`, and the else branch's first
instruction was `LDA __local_b0` to reload it — the per-block A
tracker reset state at the branch-target label).

Extended the pass: at every Branch / Jump, snapshot the
register-mirror state keyed by the target label. At any target
label with a unique predecessor (the saved-from Branch, no
fall-through, no other branches), restore the snapshot. A
Branch / Jump preserves A across both edges, so the unique-pred
target inherits the snapshot soundly. The reload disappears,
the spill becomes dead, DSE drops it.

**4. Unused-locals pruning.** With (3) firing, the `__local_b0`
slot ends up with no readers — but its `EQU $82` directive was
still in the output. Added a late pass that scans the IR for
referenced Data names and filters the slot-symbol table to just
those, dropping the dead EQU.

**5. X→Y dual-index promotion.** Last gap: `slot` is LDX'd in
both arms (X is already taken by `bobble_idx` at function entry).
Pin `slot` to Y at function entry, rewrite the per-arm
`entity_floor_pos,X` to `,Y`, drop the per-arm LDX. Gates on Y
being unused elsewhere and on encodability (the 6502 has `ADC
abs,Y` but no `INC abs,Y`, so the rewrite has to refuse
unsafe slots).

## Final output

```asm
.apply_bobble@asm_ssa_block@0:
   LDY   __zpabi_apply_bobble_p0       ; slot, once at entry
   LDX   __zpabi_apply_bobble_p1
   LDA   rescue_bobble,X
   BPL   .if_else@1
.add_path:
   AND   #$7F
   CLC
   ADC   entity_floor_pos,Y
   STA   entity_floor_pos,Y
   JMP   .if_end@0
.if_else@1:
   AND   #$7F                          ; redundant in this branch
   STA   __local_apply_bobble_b1
   LDA   entity_floor_pos,Y
   SEC
   SBC   __local_apply_bobble_b1
   STA   entity_floor_pos,Y
.if_end@0:
   RTS
```

17 instructions in the body. `__local_b0` is gone; only `b1`
remains for the SBC spill (SBC isn't commutative, so the
peephole can't fold it). Add path: ~12 cycles faster than the
before-state. Else path: ~6 cycles faster.

## The one I didn't ship

The leftover `AND #$7F` in the else branch is genuinely
redundant — at that point bit 7 of A is already 0 (that's why
we took the BPL), so the AND doesn't change A's value, and the
flag effects are dead. Tempting to hoist the AND in front of
the BPL to dedupe both copies, but it breaks the branch:

```asm
LDA rescue_bobble,X    ; N = bit 7 of bobble
AND #$7F               ; N = 0 always (bit 7 forcibly cleared)
BPL .if_else@1         ; always taken — bug!
```

The BIT-trick fix preserves bit 7 in memory and tests it that
way, but that reintroduces the spill of bobble — net cost more
than the dedup saves. Better to leave it as path-sensitive
dead-code elim, which I haven't written yet.

Anyone got a clever way to dedupe the AND without a spill?
