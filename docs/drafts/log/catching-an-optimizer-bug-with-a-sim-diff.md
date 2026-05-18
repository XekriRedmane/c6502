# Catching an optimizer bug with a sim differential test

I have a habit when I add a new example to my C-to-6502 compiler:
write a sim differential test. The test inlines the example
source, wraps it in a `main()` that exercises the function under a
battery of scenarios, and asserts that the unoptimized and
optimized pipelines produce identical observable state byte for
byte. The unoptimized side is the ground truth — I hand-compute
the expected post-call state from the C source. The optimized side
just has to agree.

These tests are slow to write (you're writing a tiny game-engine
simulator's expected state in your head), but they pay back
roughly every time. Last week one of them caught a real optimizer
bug — the kind that ships incorrect runtime behavior in an
otherwise compiling .asm file. This post is the walk through.

## The example

The function is `companion_update` — a per-frame tick for two
"companion" sprites that bobble around the screen in an Apple II
game I'm porting. Two slots, indexed 1 then 0, each running a
three-state machine (idle / active / drift) plus a draw step that
runs perspective transform, off-screen clip, entity-proximity
check, and a player-catch hit-box.

The whole routine is `__attribute__((zp_abi))` — its seven params
live in zero page rather than on c6502's soft data stack, and it
calls a half-dozen zp_abi helpers (`active_pos_step`,
`active_neg_step`, `drift_step`, `compute_screen_x`,
`entity_proximity`, `smc_body_draw`, `player_catch`). The
call-graph-disjoint ZP allocator gives the whole call clique a
shared private slot pool: `__zpabi_active_pos_step__slot`,
`__zpabi_entity_proximity__slot`, etc., all colocated at `$87`
because they're never live simultaneously.

I want to know: does the optimized output behave identically to
the unoptimized output?

## Writing the differential

The harness pattern is straightforward. I inline the example
source verbatim (minus the extern declarations of mutable
globals), provide stubs for the `prng()` and `draw_sprite()`
externs that the function calls, set up the perspective-transform
tables as all-zero so screen_x equals the companion's world-x,
and pick `floor_thresh[]` values that make the rearm row land on
the drift anchor rows (`$63`, `$8B`, `$B3`).

Then I write a `main()` with 8 scenarios:

```c
int main(void) {
    log_idx = 0;
    prng_value = 0xFF;   /* never trigger 5/256 direction flip */

    /* A. Gate disabled -> early return. */
    /* ... set up state ... */
    companion_update(0xFF, 0, 0, 0, 0, 0, 0);
    record();

    /* B. Both slots inactive -> self-activate + draw. */
    /* ... */
    companion_update(0x00, 0, 0, 0x80, 0, 0, 0);
    record();

    /* C. Slot 1 active +dir midstream + slot 0 off-screen. */
    /* ... */

    /* D. Slot 1 +dir rearm boundary. */
    /* E. Slot 1 -dir rearm boundary. */
    /* F. Drift: slot 1 on anchor, slot 0 non-anchor. */
    /* G. Entity proximity drift transition. */
    /* H. Player-catch hit. */

    return (int)log_idx;
}
```

`record()` snapshots `companion_state[0..1]`, `companion_dir[0..1]`,
`companion_pos_lo[0..1]`, `companion_pos_hi[0..1]`,
`companion_row[0..1]`, `hit_flag`, and the stubbed `draw_calls` /
`prng_calls` counters into a flat 16-byte slot of a `result_log`
buffer.

The expected state for each scenario I hand-compute from the C
source. For scenario C ("slot 1 active +dir midstream + slot 0
off-screen"):

```python
# C. Slot 1 active +dir midstream, slot 0 off-screen.
# Slot 1: pos $00:$60 + 3 -> $00:$63, no rearm, draws.
# Slot 0: pos $02:$00 + 3 -> $02:$03, off-screen clip, skips.
# prng called twice (one per active step).
[
    0x01, 0x01,         # state[0], state[1]
    0x01, 0x01,         # dir[0], dir[1]
    0x03, 0x63,         # pos_lo[0], pos_lo[1]
    0x02, 0x00,         # pos_hi[0], pos_hi[1]
    0x60, 0x70,         # row[0], row[1]
    0x00, 0x01, 0x02,   # hit_flag, draw_calls, prng_calls
    0x00, 0x00, 0x00,   # padding to 16 bytes
],
```

Three tests: unopt matches expected, opt matches expected, opt
matches unopt byte for byte.

## The failure

The unopt test passed first try. The opt tests both failed. The
diff:

```
scn C ! got=01 01 01 01 00 66 02 00 60 70 00 01 02 00 00 00
        exp=01 01 01 01 03 63 02 00 60 70 00 01 02 00 00 00
scn D ! got=01 01 01 ff 10 56 05 03 60 8b 00 00 02 00 00 00
        exp=01 01 01 ff 13 53 05 03 60 8b 00 00 02 00 00 00
scn G ! got=01 ff 01 01 10 46 05 00 60 6c 00 01 02 00 00 00
        exp=01 ff 01 01 13 43 05 00 60 6c 00 01 02 00 00 00
```

Across scenarios the pattern was consistent: slot 0's `pos_lo`
never got updated (still at its preset value), slot 1's `pos_lo`
got incremented by 6 instead of 3. Slot 1's other state (dir, row,
state) updated correctly. So slot 1's `active_pos_step` ran TWICE
and slot 0's didn't run at all.

That's not a "slightly wrong arithmetic" bug. That's "the wrong
slot index reached the callee."

## Reducing the repro

The companion_update example is 391 lines of C. The bug shape
clearly involved a loop down-iterating two slot indices and
passing each to a zp_abi callee. So I tried to reproduce it with
the smallest C I could:

```c
#include <stdint.h>

uint8_t data[2];
uint8_t state[2];

__attribute__((zp_abi))
void op_a(uint8_t slot) {
    data[slot] = (uint8_t)(data[slot] + 0x10);
}

__attribute__((zp_abi))
void op_b(uint8_t slot, uint8_t v) {
    state[slot] = v;
}

__attribute__((zp_abi))
void caller(void) {
    for (int8_t slot = 1; slot >= 0; slot--) {
        if (state[slot] & 0x80) {
            op_b((uint8_t)slot, 0xAA);
        } else {
            op_a((uint8_t)slot);
            op_b((uint8_t)slot, 0x55);
        }
    }
}

int main(void) {
    data[0] = 0; data[1] = 0;
    state[0] = 0; state[1] = 0;
    caller();
    return ((int)data[0] << 8) | (int)data[1];
}
```

Expected: `0x1010` (both slots incremented).
Optimized actual: `0x0020` (slot 0 not incremented; slot 1
incremented twice).

The earlier attempts at reduction missed: the loop body needed
both a branch and multiple calls to a zp_abi helper, AND the
caller had to be zp_abi too. Without all three, the regalloc made
different choices and the bug didn't trigger.

## Reading the buggy asm

```
caller:
    LDY   __local_caller__slot   ; weird LDY from uninitialized M
    LDA   #$01
    STA   __local_caller__slot   ; M = 1
    TAX                           ; X = 1
.loop:
    LDA   state,X                ; X-indexed; fine
    BPL   .if_else
.if_then:
    LDA   __local_caller__slot   ; STALE!
    STA   __zpabi_op_b__slot
    LDA   #$AA
    STA   __zpabi_op_b__v
    STX   __local_caller__slot   ; sync M (too late)
    JSR   op_b
    JMP   .if_end
.if_else:
    LDA   __local_caller__slot
    STA   __zpabi_op_a__slot
    STX   __local_caller__slot
    JSR   op_a
    LDA   __local_caller__slot
    STA   __zpabi_op_b__slot
    LDA   #$55
    STA   __zpabi_op_b__v
    STX   __local_caller__slot
    JSR   op_b
.if_end:
.loop_continue:
    DEX                          ; X--; M not synced!
    BPL   .loop
    RTS
```

The asm-SSA regalloc had colored `slot` to BOTH `Reg(X)` (for
`state,X` indexed access and the loop-tail `DEX`) AND a memory
slot `__local_caller__slot` (the X-save home around each `JSR`).
The init `STA M; TAX` set them both to 1. The body's
`STX M; JSR; LDX M` wrap saved and restored X around each call.

But `DEX` only decremented X. M stayed at the previous iteration's
value. On the next iteration, the first `LDA M` for arg-passing
read the stale M, passing the previous iteration's slot index to
the callee.

For two iterations starting at slot=1:

| Step | X | M | Action |
|---|---|---|---|
| init | 1 | 1 | STA M; TAX |
| loop body slot=1 | 1 | 1 | LDA M (=1), call op_a(1), STX M, ... |
| loop tail | 0 | 1 | DEX → X=0, M still 1 |
| loop body slot=0 | 0 | 1 | LDA M (=1!), call op_a(1) — wrong slot |
| loop tail | -1 | 0 | DEX → X=-1, exit |

That's why slot 0 never got incremented and slot 1 got
incremented twice: both iterations of the loop called `op_a(1)`.

(The mysterious `LDY __local_caller__slot` at the top is a
vestige of asm-SSA parallel-copy resolution for some phi —
harmless, but it reads uninitialized memory. I left it alone for
this fix.)

## What pass produced this?

The X-promotion (DEX instead of DEC M) wasn't from
`loop_counter_to_x` — that pass's eligibility requires `Dec(M)`,
but the input already had `Dec(Reg(X))` by the time it ran. So
the X-promotion happened upstream in the asm-SSA byte-granular
regalloc, in a way that:

- Colored some SSA names of `slot` to X (so `Dec(slot)` and
  `IndexedData(_, X)` accesses came out as `DEX` and `state,X`).
- Coalesced some other SSA name of `slot` to M (so reads for
  arg-passing came out as `LDA M`).

The two SSA names should have stayed in lockstep but didn't: the
`DEX` updated X without updating M, and the regalloc didn't
notice that the value-read sites of M were stale.

Fixing this in the regalloc proper would be a deep change to the
SSA destruction / coalescing logic. But there's a simpler fix:
post-process the asm after regalloc and rewrite the stale reads.

## The fix

A new pass, `passes/x_save_slot_load.py`. For each function:

1. **Identify X-save slots.** Memory operands `M` that appear as
   the destination of any `Mov(Reg(X), M)` — i.e., STX M is
   somewhere. These are slots the regalloc uses as X's spill
   home around calls.

2. **Disqualify unsafe candidates.** Reject any M with an in-place
   RMW (`Inc(M)`, `Dec(M)`, shift), a non-X non-A write that isn't
   followed by `TAX`, or a `Mov(Reg(Y), M)` (STY M doesn't touch
   A, so a following TAX wouldn't sync M with X). The accepted
   init shape is `Mov(<v>, M); Mov(Reg(A), Reg(X))` — the
   `LDA c; STA M; TAX` pattern that establishes M = X = c.

3. **Rewrite reads.** For every surviving M:
   - `Mov(M, Reg(A))` (LDA M) → `Mov(Reg(X), Reg(A))` (TXA).
   - `Mov(M, Data|ZP)` (mem-to-mem) → `Mov(Reg(X), Data|ZP)`
     (single STX). The 6502 has STX zp and STX abs, so this is
     direct.

Mem-to-mem dsts that STX doesn't support (Frame, Stack, Indirect,
IndexedData) are left alone — STX has no `(zp),Y` or `abs,X`
form. Those shapes don't appear in the buggy pattern in practice.

I almost shipped this with only the first rewrite (LDA M → TXA).
The unit test passed in isolation, but end-to-end the bug
persisted. Why? Because the buggy code's stale read wasn't a
two-atom `Mov(M, Reg(A)); Mov(Reg(A), Data(callee))` — it was a
single mem-to-mem `Mov(M, Data(callee))` atom that hides the
`LDA` in the emitter. The pass had no `Mov(M, Reg(A))` to match,
because there wasn't one at the IR level.

This is documented in my project memory as
"mem-to-mem Mov hides emit-time LDA" — a gotcha I've hit before.
I'd just forgotten to consult it when writing the pass.

The full pass is about 150 lines including docstring; the
operative logic fits in a 30-line function. Wire-up in
`compile.py` and `sim/harness.py` after `apply_loop_counter_to_x`
and before the post-promotion peephole fixedpoint.

## The numbers

After the fix:

- Minimal repro: returns `0x1010` (correct).
- `companion_update` sim test: 3/3 pass (was 1/3).
- `examples/companion_update.asm`: 1154 → 1142 lines. The
  `LDA M; STA __zpabi_*` pairs collapsed to single `STX __zpabi_*`
  instructions; downstream DSE dropped the now-dead `STX M` save
  wraps too (because all reads of M became reads of X, M became
  dead).
- Full test suite: 2585 passing (was 2577 — eight new unit tests
  for the new pass).

## What I would tell past-me

Three things:

1. **Sim differential tests catch what gold-output diffs can't.**
   The gold-output test (`tests/test_example_outputs.py`) compares
   the new `.asm` against the checked-in one byte for byte — it
   catches codegen *drift* but not codegen *bugs*. The checked-in
   `.asm` was buggy from the day it landed; the gold-output test
   pinned the buggy code as the expected output. Only a behavioral
   test could catch it.

2. **Reduce before diagnosing.** I spent maybe an hour reading the
   companion_update asm before I bothered to write a minimal C
   repro. The minimal repro took five minutes and made the bug
   pattern obvious — once I had it, the rest of the diagnosis was
   linear.

3. **Consult memory before writing code.** The "mem-to-mem hides
   LDA" gotcha is documented. I'd written it down precisely so
   future me wouldn't fall into the trap. Future me fell into the
   trap anyway. Add a habit: before writing a new peephole, grep
   memory for `mem-to-mem`, `compound atom`, `hidden LDA`, etc.

The shipped fix is small, the regression test pins the bug shape,
and the example's `.asm` is now shorter than before. Most days
this is what a good optimizer bug looks like.
