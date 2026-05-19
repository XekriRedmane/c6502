# Hand-written assembly as a compiler oracle

I have a side project: I'm porting an Apple II game to 6502 source
that compiles from C. The original is in 6502 assembly. The new
version goes C → c6502 (my compiler) → 6502 assembly. The two
outputs ideally have the same functional behavior, with c6502's
output being roughly competitive in size and cycles to a
hand-written equivalent.

"Roughly competitive" is a moving target. The hand-written code
was written by someone with deep target knowledge over many
iterations, with full visibility into the calling convention,
register usage, and code-density tricks. c6502 is a generic
optimizing compiler. The gap will exist; the question is how big
and where.

What I didn't expect: the hand-written code is also an unusually
good *oracle* for finding optimization gaps. When the two outputs
diverge significantly, the hand-written version says "here's what's
possible" in a way that's specific, falsifiable, and code-aligned.
There's no abstract "the compiler should be smarter" — there's
a concrete 14-instruction sequence sitting right next to c6502's
24-instruction sequence, and the diff IS the optimization.

This post is about one such gap: a 16-bit subtract chain in
`compute_screen_x` that c6502 was emitting at ~1.7× the
hand-written cost. The story is partly about the fix itself, and
partly about the methodology of using a hand-written reference to
find optimization gaps.

## The function

`compute_screen_x` does a perspective transform on a sprite's world
position. The C is straightforward:

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

Two 16-bit subtractions. The high bytes of `xoff` and `pos` come
from indexed loads into separate `_hi` arrays; the low bytes come
from `_lo` arrays. After C's integer promotion, `sprite_xref` is
zero-extended to 16 bits (so the high byte of the subtractand is
just 0).

## The hand-written version

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

14 instructions, byte-interleaved. The pattern is: load each byte
of the subtrahend, subtract the corresponding byte of the
subtractor, store, move to the next byte. The 6502's borrow chain
threads through the SBCs naturally — one SEC at the start, then
each subsequent SBC reads the borrow from the previous one's flag
output without an intervening CLC.

Worth lingering on the scratch usage: the hand-written code stores
the intermediate `xoff - sprite_xref` into `ZP_SCREEN_X` /
`ZP_SCREEN_X_HI`, then **reuses** those same slots for the final
`pos - xoff` result. The pattern works because the second
subtract's result happens to land in the same place the first
subtract stored its intermediate; no need to allocate a separate
scratch pair. That's a specific human-written optimization — the
return slot doubles as scratch.

## What c6502 emitted

```asm
   LDX   __zpabi_compute_screen_x__player_y
   LDA   perspective_xoff_lo,X
   STA   __local_compute_screen_x__1
   LDA   perspective_xoff_hi,X
   STA   __local_compute_screen_x__0
   LDA   __local_compute_screen_x__1
   SEC
   SBC   __zpabi_compute_screen_x__sprite_xref
   STA   __local_compute_screen_x__3
   LDA   __local_compute_screen_x__0
   SBC   #$00
   STA   __local_compute_screen_x__2
   LDX   __zpabi_compute_screen_x__slot
   LDA   companion_pos_lo,X
   STA   __local_compute_screen_x__1
   LDA   companion_pos_hi,X
   STA   __local_compute_screen_x__0
   LDA   __local_compute_screen_x__1
   SEC
   SBC   __local_compute_screen_x__3
   STA   HARGS
   LDA   __local_compute_screen_x__0
   SBC   __local_compute_screen_x__2
   STA   HARGS+1
   RTS
```

24 instructions, plus RTS. The shape is the same — load, subtract,
store, byte-by-byte — but each value gets pre-loaded into a local
ZP slot before the subtract reads it back. That's two extra
instructions per byte of operand (4 bytes × 2 instructions = +8
instructions for the same operation set).

The whole-function delta is +10 instructions / ~20 cycles per
call. In a per-frame routine that runs twice per frame, that's
~40 wasted cycles per frame. Not a lot in isolation, but the same
pattern appears in similar shape across other multi-byte operations
in the corpus.

## Where the bloat lives — not where I first looked

My first guess: the multi-byte ADC/SBC lowering in `tac_to_asm` is
emitting the load-all-then-compute shape. If the C is `xoff -
sprite_xref` where `xoff` is composed from two indexed loads, then
the natural TAC-level shape is:

1. Load `xoff_hi` via IndexedLoad.
2. Load `xoff_lo` via IndexedLoad.
3. Compose into a 16-bit value.
4. Zero-extend `sprite_xref` to 16 bits.
5. 16-bit Subtract.

Step 5 emits as a byte-by-byte SBC chain, reading from wherever
steps 1-3 left the bytes of `xoff`. If those bytes ended up in ZP
locals (from steps 1-2's IndexedLoad stagings), the SBC reads them
from there.

Could `_translate_add_sub` instead recognize that `xoff_lo` lives
at `IndexedData(perspective_xoff_lo, X)` and emit the SBC chain
that reads from there directly? In principle yes, but it would
need TAC-level knowledge of which Pseudos are direct aliases for
recoverable expressions — a kind of fusion across TAC operations.
Doable, but invasive.

Reading `_translate_add_sub` confirmed: it already does the right
thing at its level. It walks bytes low → high and emits
`LDA src1[k]; SBC src2[k]; STA dst[k]` per byte. The carry threads
across iterations. The IR coming out of this function is exactly
what the hand-written version would express at the IR level.

So the bloat had to be downstream. Specifically: between
`tac_to_asm` and the final asm, the byte-Pseudos representing
`xoff`'s bytes get **materialized to ZP slots** by SSA construction
+ regalloc, and the byte-by-byte SBC chain then reads from those
ZP slots instead of from the original `IndexedData` expressions.

## Tracing the staging

The asm-SSA pipeline:

1. `to_ssa` — versions every byte-Pseudo's writes and reads with
   fresh SSA names. `xoff.b0` and `xoff.b1` become
   `xoff.b0.v0` / `xoff.b1.v0` etc.
2. `hwreg_eligibility` + `coalesce_moves` — decides which Pseudos
   can live in `Reg(X)` / `Reg(Y)` and merges move-related pairs.
3. `copy_propagate` / `backward_copy_propagate` / `byte_dce` —
   fixed-point cleanup of SSA-form code.
4. `color_graph` — byte-granular regalloc colors each surviving
   Pseudo byte to a ZP byte.
5. `from_ssa` — emits Movs at each Phi's predecessor edges.

After step 5, each byte-Pseudo has a concrete ZP storage location,
and references to it become `Data(__local_<fn>__<...>)`.

Steps 1-3 do propagate values eagerly within the SSA form, but
they only propagate Pseudo → Pseudo copies (because that's all
SSA reasoning gives them for free). They DON'T propagate the
underlying `IndexedData` source through a chain of Pseudos to
final use sites.

That propagation is the job of `apply_remat`, a post-coloring
peephole that does exactly the rewrite I was hoping for.

## apply_remat — eligibility was too narrow

`apply_remat`'s docstring describes the pattern:

```
LDA   <recomputable_src>
STA   __local_<fn>__<stage>          ; def of the stage cell
... (A clobbered by other marshaling) ...
LDA   __local_<fn>__<stage>           ; use of the stage cell
STA   <consumer_dst>
```

When `<recomputable_src>` can be re-computed at the use site
without observable change, the staging round-trip is pure
overhead: rewrite the use's source to `<recomputable_src>`
directly, and the staging store becomes dead.

Eligibility for `<recomputable_src>`:
- `Imm` / `ImmLabelLow` / `ImmLabelHigh`: trivial.
- `Data(name, off)`: safe if `name` is immutable in the function
  and no `Call` between def and use.
- `IndexedData(name, off, reg)`: same, plus the index register
  must not be written between.

The pass looks for staging defs of the shape `Mov(<recomp>,
Data(__local_*))`. For each match, walk forward to find uses, do
the eligibility check, rewrite.

The problem: after SSA destruction, the staging shape was often
**two atoms**, not one:

```python
Mov(IndexedData(arr, X), Reg(A))   # producer: LDA arr,X
Mov(Reg(A), Data(__local__))        # staging def: STA local
```

`apply_remat` looked at the staging def, saw `src = Reg(A)`,
correctly decided that "the value in A" isn't a recoverable
expression (A could have come from anywhere), and bailed.

The single-atom case (`Mov(IndexedData, Data(local))`) was being
handled — that's when the entire `LDA src; STA dst` lowering
stays as one IR atom through the SSA pipeline. The two-atom
case appears when the SSA destruction emits parallel-copy Movs
that split the mem-to-mem atom into its register-routed halves.

In c6502's output the two-atom shape dominates for the patterns
we care about. Hence the staging never got rematerialized.

## The fix

Extend `_classify_stage_def` to look one instruction back when
the def's src is `Reg(A)`. If the previous instruction is a
`Mov(<recomp>, Reg(A))` producer with no intervening A-write,
treat the producer's src as the recomputable value and extend the
validation range back to the producer's index. The existing
`_can_remat` does the validation work unchanged — it already
checks for arr-immutability, X/Y-register stability, and
no-Call-between, all the way back to the validation-range start.

The fix is ~60 lines added to `passes/asm_remat.py`. The
producer-finder (`_find_a_producer`) walks back from the staging
def's index, accepting only `Mov(_, Reg(A))` as the producer and
bailing at any block boundary or A-write that isn't a clean Mov.

One ancillary fix: the dead-stage-dst sweep previously collapsed
`Mov(<src>, Data(__local__))` to `Mov(<src>, Reg(A))` when the
local was no longer read. For the new two-atom case, `<src>` is
`Reg(A)` — so the collapse produced `Mov(Reg(A), Reg(A))`, a
self-Mov that no IR peephole touches (the `self_store_drop`
pass requires both operands to be stable memory; `Reg` doesn't
qualify). Special-case: when src is Reg(A), omit the instruction
entirely. The producer already left A holding the value.

## Result

`compute_screen_x` now emits 14 instructions, same as the
hand-written version. The byte-by-byte structure is identical;
the only difference is the ZP slot choice for intermediate `xoff`
storage (c6502 uses two separate `__local_*` slots while the
hand-written code reuses the return slot). That's a regalloc
allocation difference; it doesn't change cycle count.

Across the example corpus the fix saved a total of 12 emitted-asm
lines in `companion_update.asm` and 2 in `apply_bobble.asm`. Other
examples didn't change — they either didn't have the staging
pattern, or the pattern stayed in single-atom form through the SSA
pipeline (which the existing remat already handled).

## What "hand-written as oracle" means as a methodology

I want to draw out the methodological observation, because it's
the more transferable insight than this specific fix.

When evaluating an optimizing compiler, the common feedback loops
are:

1. **Snapshot tests** — compare today's output to yesterday's
   output, fail on drift. Catches regressions, not gaps.
2. **Behavioral tests** — verify the compiled program produces
   correct outputs at runtime. Catches bugs, not gaps.
3. **Microbenchmark suites** — measure cycle counts on specific
   patterns. Catches gaps, but only at the granularity of the
   benchmark.
4. **Cross-compiler comparison** — diff outputs between two
   compilers. Catches gaps where the other compiler does better.

A hand-written reference adds a 5th loop: **expert-written code as
an oracle**. The expert has visibility into target-specific
patterns (carry-chain SBC, ZP slot reuse for return + scratch) and
has iterated to remove anything the compiler "should know about
but doesn't." When the compiler's output and the hand-written
output diverge significantly, the divergence is a high-quality
signal.

What it surfaces well:
- **Missing target-specific optimization patterns.** Cross-compiler
  diffs surface gaps relative to another generic compiler. A
  hand-written reference surfaces gaps relative to what the target
  CPU actually supports.
- **Implicit allocation choices the compiler could make.** Slot
  reuse (return slot doubling as scratch), register pinning for
  cross-loop values, calling-convention tweaks.
- **The "shape" of an optimal lowering.** Even when you can't
  precisely match the hand-written output (because of regalloc
  decisions, different ABIs, etc.), the shape gives you a target
  to drive your optimizer toward.

What it doesn't surface:
- Bugs (the hand-written code might compute different things and
  you wouldn't know).
- Optimizations beyond the expert's vocabulary (they didn't think
  to do it either).
- Cases where the C semantics force overhead the expert avoided
  (volatile, type promotion, alias analysis).

For a project where the goal is "good 6502 code from C source",
having a corpus of hand-written reference for the same engine is
unusually valuable. I don't have a sim differential or
microbenchmark for every routine, but I do have a hand-written
version of every routine I'm porting. The diff is the to-do list.

This is the second optimizer change in two days that came from a
"why is c6502 doing it that way when the human did it this way?"
diff. Both fixes were small in isolation (~100 LOC each, plus
tests) and the size impact in any one example was modest. But the
methodology — read the hand-written code, find where the compiler
output differs, decide if the gap is structural or fixable, fix
the fixable — has been consistently productive.

I'd run this on any compiler-vs-handwritten pair if you have
access to both. The signal-to-noise is high.
