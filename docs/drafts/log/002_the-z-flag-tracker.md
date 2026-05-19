# Title

The Z-flag tracker: teaching my compiler that some loads are flag-redundant too

# Body

The 6502 CPU has a one-bit lightbulb called the Z flag. It turns
ON when the last result was zero, OFF otherwise. The next `BEQ`
("branch if equal") looks at the lightbulb to decide whether to
jump. `BNE` does the opposite.

Almost every data-touching instruction flicks the lightbulb. LDA
turns it on iff the loaded byte was zero. SBC, ADC, AND, OR, EOR
turn it on iff the new A is zero. INC and DEC turn it on iff the
bumped cell is now zero. Even register-to-register transfers
(TAX, TAY, TXA, TYA) update Z.

This is why removing a redundant LDA is more delicate than it
looks. Imagine code like:

```
LDA M     ; A = M, Z = (M == 0)
STA N     ; N = A, A and Z unchanged
LDA M     ; A = M (already true!), Z = (M == 0) (already true!)
BEQ done  ; branch if Z is on
```

The second `LDA M` is doing two things: putting M's value into A
(useless — it's already there) and setting Z to "is M zero" (also
useless — Z already says that, because the first LDA set it and
STA didn't touch it).

The first kind of redundancy — the value duplication — is what
the textbook "register tracker" optimization eliminates. After
each instruction you remember which operand each register
currently mirrors. When the next instruction is `LDA M` and A
already mirrors M, drop it.

My c6502 compiler had that part. What it didn't have was the
second kind of redundancy. The existing analyzer asked "is the Z
flag dead before some downstream instruction overwrites it?" — a
forward CFG walk to the next `Branch` or flag-setter. If a BEQ
shows up before another LDA / arithmetic op, Z is alive and the
conservative answer is "keep the LDA so Z gets re-set". That's
sound but too cautious in cases like the one above, where Z is
already in exactly the state the LDA would set it to.

## The motivating shape

I bumped into this writing a C99 compiler that targets the Apple
II. The test program was a speaker-click delay routine I'd ported
from Apple II disassembly:

```c
void sfx_tone(uint8_t pitch, uint8_t duration)
{
    do {
        volatile uint8_t y = pitch;
        while (--y != 0) { }       /* delay loop */
        (void)*sfx_click_ptr;      /* speaker click */
    } while (--duration != 0);
}
```

My compiler's inner loop for `--y != 0` came out as:

```
LDA y     ; volatile — must run every iteration
SEC
SBC #1    ; A = y - 1, Z = (A == 0)
STA temp
LDA temp  ; <-- redundant: A === temp, Z already reflects this value
STA y     ; volatile write of decremented value
LDA temp  ; <-- redundant again, same reason
BEQ break
JMP continue
```

The two LDA temps both put A's already-correct value back into A
and re-set Z to a value Z already had. The existing
redundant-load pass dropped neither, because both are followed
(eventually) by a flag-reading `BEQ break`, and the conservative
"is Z dead?" check refused to declare it dead.

The hand-written 6502 reference for this routine is two
instructions per iteration:

```
.delay: DEY
        BNE .delay
```

Five-cycle inner loop versus my 22-cycle inner loop. That's a 4×
gap. The "Y register vs memory cell" choice for the counter
explains most of it (more on that below). The redundant LDAs
explain the rest.

## The Z-reflects tracker

I extended the existing per-block state with one more list:
`z_reflects`. Each entry in `z_reflects` is an operand whose
current value's zeroness equals the current state of Z. After
`LDA M`, `z_reflects = [M]`. After `STA N` (which copies A's
value to N without changing A or Z), N now has the same value
as what Z reflects, so `z_reflects = [M, N]`. After `SBC #1` —
which sets Z to A's new value's zeroness but also changes A —
`z_reflects = []` (Z reflects something, but no named operand
matches it now). A subsequent `STA P` then repopulates:
`z_reflects = [P]` (Z still reflects A's current value, A's
current value is now also in P).

The candidate-drop check becomes: drop `LDA M` when A already
mirrors M (existing check) AND either:
- `z_reflects` contains M (Z is already in the state the LDA
  would set it to), OR
- Z is dead before the next flag-reading branch (the existing
  fallback).

Either condition alone is sound. Either one alone misses cases.
The disjunction catches both flavors.

## Walking through the inner loop

```
.loop@1_continue:
   LDA y    ; volatile — clears state.a (volatile LDA can't be
            ; cached for future drops); z_reflects = [y]
   SEC      ; doesn't touch Z; z_reflects unchanged
   SBC #1   ; A's identity cleared; z_reflects = []
   STA temp ; state.a = [temp]; z_reflects = [temp]
            ; (Z still reflects A's value; A's value === temp now)
   LDA temp ; CANDIDATE: state.a contains temp ✓,
            ; z_reflects contains temp ✓. DROP.
   STA y    ; volatile; emit-time write of A to y
   LDA temp ; CANDIDATE: state.a contains temp ✓,
            ; z_reflects contains temp ✓. DROP.
   BEQ break
```

Both LDA temps are now dropped. STA temp is left writing a cell
nobody reads — the `asm_dead_store` pass catches that on a
subsequent fixed-point round. Final inner loop: 6 instructions
instead of 9.

## The update rules

The detail is in keeping `z_reflects` correct after every
instruction. The pattern is "what does this instruction do to Z,
and what operand (if any) does it now reflect?"

- `LDA M` / `LDX M` / `LDY M`: z_reflects = [M]. The load
  unconditionally sets Z to (M == 0).
- `STA M` / `STX M` / `STY M`: Z unchanged. But M now equals the
  source register's value. If z_reflects was previously
  reflecting that register's value (matched any operand in
  `state.<reg>`), z_reflects gains M.
- `ADC` / `SBC` / `AND` / `OR` / `EOR` with dst=A: Z reflects A's
  new value; A's identity is unknown. z_reflects = [].
- `INC M` / `DEC M`: Z = (M's new value == 0). z_reflects = [M].
- `ASL` / `LSR` / `ROL` / `ROR` with dst=A: Z reflects A's new
  value, identity unknown. z_reflects = [].
- Same op with dst=M: z_reflects = [M].
- `Compare A, op`: Z = (A == op). Doesn't fit "operand zeroness"
  — clear z_reflects.
- `BitTest M`: Z = (A & M == 0). Same — clear.
- `Branch`: no register or flag effect (the test only changes
  PC). State unchanged.
- `Jump` / `Ret` / `Return` / `Call`: end-of-block. Reset.
- `SetCarry` / `ClearCarry`: touch only C. Z unchanged.
- `Push`: doesn't touch flags. Unchanged.
- `Pop` (PLA): Z = (popped value == 0). We don't track stack
  values, so z_reflects = [].
- Labels (when they're branch targets): reset.

There's a subtlety in the mem-to-mem `Mov` atom. At my IR level
that's one instruction, but at emit time it's two: `LDA src; STA
dst`. The implicit LDA is invisible to peephole walks, but for
the tracker it's real: Z gets set to (src == 0) by the emit-time
LDA, and dst's value equals src's value after the STA. So
`z_reflects` post-mem-to-mem is `[src, dst]` (parallel to
state.a's post-Mov contents).

## Volatility, briefly

For volatile-flagged Mov atoms, redundant_load's existing rule
("never drop a volatile LDA") was already in place. For
z_reflects, the corresponding rule is "don't trust a volatile
cell's value for future reads, but DO record what Z reflects
right now." A volatile load happens, sets Z to the cell's
current value, and downstream code in the SAME block sees that
Z. A future LDA of the same volatile cell can't be elided
because the cell's value could have changed — but z_reflects
listing the volatile cell for the duration of the current block
is fine; the elide check on the future LDA is rejected by the
volatile gate, not by z_reflects.

## What it bought, what it didn't

The inner loop of `sfx_tone` shrank from 9 to 8 instructions in
the c6502 output. (One of the two redundant LDA temps survives,
because it's hidden inside a mem-to-mem `Mov(temp, y)` atom that
the tracker can see the EFFECTS of but can't rewrite into
something tighter without a separate peephole.)

`floor_enemy_advance` and `floor_enemy_draw` — two other
examples in the test corpus — each shrunk by one redundant LDA
in similar positions.

What it doesn't buy: the gap to the hand-written 7-instruction
reference. That gap is mostly the "y must be in memory, not a
register" cost of strict volatile semantics. The hand-written
version uses `DEY`/`BNE` with `y` in the Y register; my compiler
puts it in memory because every access must be observable per
C99 §6.7.3.6. Closing that gap requires either a non-strict-
volatile mode (recognize that a volatile counter has no observers
that can see individual writes, only the loop's cycle count) or
a different C-source idiom that signals "delay this many cycles"
without claiming each write is a side effect.

Or, you know, just write the inline asm.

## The code

`passes/redundant_load.py`. The `z_reflects` field lives on the
`_RegState` dataclass; `_update_state` and `_update_for_mov`
maintain it per instruction; `_flags_redundant_at` is the new
combined gate (check z_reflects first, fall back to the existing
`_flags_dead_at`).

The diff is around 200 lines of code plus a roughly equal amount
of new docstring — the rules above are exactly the kind of thing
that needs careful comments next to the code, because the
"obvious" implementation is one off-by-one corner case away from
silently miscompiling a branch. A bug in this kind of analysis
doesn't trip tests; it just makes the wrong jump in production.

Lightbulbs are simple. Tracking them isn't.
