# Two ways to be wrong about what's already in A

c6502 is a C99 compiler I'm writing for the 6502. The asm-level
peephole layer has a pass called `redundant_load.py` that tracks
which memory cell each of A / X / Y currently holds a copy of,
and drops `LDA M` / `LDX M` / `LDY M` whose target register
already mirrors `M`. The point is to clean up the `LDA M; ...; LDA
M` shapes that arise after loop unrolling, SSA destruction, and
the per-byte fan-outs in multi-byte lowerings.

It was a single-pass linear walker. State at every basic-block
boundary — `Label`, `Jump`, `Branch`, `Call`, `Ret`, `Return` —
got reset to empty. There was one carve-out: at a `Label` whose
only predecessor was a single forward `Branch` or `Jump`, the
tracker restored the snapshot it had taken just before that
predecessor's terminator. That handled the post-`apply_bobble`
shape (`STA b0; BPL .else; ...; .else: LDA b0` → drop the LDA),
and not much else.

While reading the output of one of the example files I had
checked in, I noticed this in `entity_proximity`:

```
.if_end@1:
   LDX   __zpabi_entity_proximity__slot
   LDA   companion_row,X
   STA   __local_entity_proximity__0
   LDA   __local_entity_proximity__entity_row
   CMP   __local_entity_proximity__0
   BEQ   .if_end@2
   RTS
.if_end@2:
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$40
   BCC   .if_end@5
   CMP   #$47
   BCS   .if_end@5
   LDA   #$FF
   STA   companion_state,X
   LDA   companion_row,X
   CLC
   ADC   #$04
   STA   companion_row,X
   RTS
.if_end@5:
   LDX   __zpabi_entity_proximity__slot
   LDA   companion_dir,X
```

`__zpabi_entity_proximity__slot` is a function parameter — the
slot index for the companion entity. It's read several times, never
written. The first `LDX __zpabi_entity_proximity__slot` happens at
`.if_end@1:`. The second one is at `.if_end@5:`, after two `BCC` /
`BCS` branches that converge on that label. Both incoming paths
leave X holding the slot — the body in between doesn't touch X or
overwrite the slot — so the second LDX is dead weight.

The per-block tracker can't see this. With two predecessors,
`.if_end@5` gets reset.

## The textbook fix

What this needs is forward must-availability dataflow over the
asm CFG. At each basic block's entry, the state is the
intersection of every predecessor's exit state. An equivalence
"A === M" survives into a successor iff every path from `ENTRY`
to that successor's entry leaves the equivalence intact. Iterate
to a fixed point so loop back-edges fall out naturally.

I asked the user upfront how aggressive to be — single-pass
forward (only handles forward joins, misses loop back-edges) vs.
full fixed-point. They picked full fixed-point. Reusing the
existing `passes/optimization_asm/cfg.py` builder, the
implementation is maybe 100 lines.

The state lattice:

- Bottom: empty lists (nothing known).
- Per-block transfer adds equivalences (loads, stores tracked as
  new mirrors via the existing `_update_state` logic) and removes
  them (clobbering writes via the aliasing lattice).
- Join at multi-pred labels: list intersection on
  `_operands_equal`.

The dataflow direction is standard. Initialize `IN[entry] = ∅`
and `IN[non-entry] = None` (uninitialized). Visit blocks via
worklist. At each visit, `IN[B] = ∩ OUT[P]` over initialized
predecessors only — uninitialized preds get skipped, so the
first sweep through a loop doesn't immediately collapse to empty
when it can't see the back-edge yet. After visiting, if `IN[B]`
changed, re-transfer and re-add successors. Lattice height is
bounded by the number of distinct operands in the function, so
the iteration terminates.

The per-pass unit tests passed. Then I ran the full suite.

15 tests failed.

## Failure #1: cross-block Z propagation under downstream DSE

The chapter_14 `pointers_as_conditions.c` simulator-differential
test failed: optimized run returned 3 where unoptimized returned 0.
Return value 3 in that test means

```c
if (a != 0) {
    return 3;
}
```

triggered, which would mean the prior `||` short-circuit broke
and assigned `a = 10`. The pointer was non-null; `||` should
short-circuit; `a` should still be 0. Something in the optimized
output was getting the branch wrong.

Diffing the asm: my pass had dropped a `LDA __local_main__i_0` at
a label `.or_true@3:`, and the dead-store pass had then dropped
two upstream `STA __local_main__i_0` instructions (one per
predecessor) along with their feeding `LDA #$0A` and `LDA #$00`.

The chain was:

```
.lb_skip@0:                       ; pred 1 — fall through to or_true
   LDA #$0A                       ; Z = ($0A == 0) = 0
   STA __local_main__i_0
.or_true@3:                       ; multi-pred merge
   LDA __local_main__i_0          ; what my pass dropped
   BEQ .if_end@6
   ...
.main@asm_ssa_split@0:            ; pred 2 — JMP to or_true
   LDA #$00                       ; Z = ($00 == 0) = 1
   STA __local_main__i_0
   JMP .or_true@3
```

`redundant_load.py` doesn't only track register equivalences. It
also tracks a `z_reflects` list — operands whose zeroness the Z
flag currently matches. After `LDA #$0A; STA __local_main__i_0`,
both A and Z reflect `Imm($0A)` AND `__local_main__i_0` (the STA
extends the equivalence class for both). After `LDA #$00; STA
__local_main__i_0` the same, with `Imm($00)` instead.

My cross-block join intersected both lists. `__local_main__i_0`
was in both predecessors' `z_reflects` → the intersection
preserved it. The consumer LDA at `.or_true@3:` was redundant on
both fronts (A already mirrors `__local_main__i_0`; Z already
reflects its zeroness) → my pass dropped it.

After my pass dropped that LDA, `__local_main__i_0` had no
in-block consumer between either upstream STA and the next
downstream write. `asm_dead_store` ran (no awareness of Z) and
dropped both STAs. `dead_a_arith` then dropped the two `LDA
#$0A` / `LDA #$00` (A is dead after their flags get clobbered).

Net: the producers for Z were gone, but the consumer (the BEQ)
was still reading Z. Whatever set Z before — whatever it was on
each path — now drove the branch. Optimization built on the
assumption "every path leaves Z = (i_0 == 0)" turned into
"whatever Z happens to be on each path." Sometimes BEQ took,
sometimes it didn't, sometimes the program returned 3.

The asymmetry between `state.a / x / y` and `z_reflects` is the
underlying issue. A `LDA M` is its own producer — the load
itself establishes A === M and is also the instruction in the
IR. If I drop the *consumer* load based on cross-block
agreement, the producer is still there.

But `z_reflects = [M]` records that *some* upstream instruction
set Z to reflect M's zeroness. Different paths to a merge can
establish this via different upstream instructions. Dropping
the consumer based on cross-block agreement relies on at least
one producer per path surviving — and the rest of the
pipeline doesn't see the cross-block dependency.

The fix: only carry `z_reflects` across single-predecessor
edges (where there's just one producer, and dropping the
consumer leaves it intact). At multi-pred joins, intersect
a/x/y but reset `z_reflects` to empty.

```python
def _join_states(states):
    states = list(states)
    result = _clone_state(states[0])
    for s in states[1:]:
        result.a = _intersect_operands(result.a, s.a)
        result.x = _intersect_operands(result.x, s.x)
        result.y = _intersect_operands(result.y, s.y)
    if len(states) > 1:
        result.z_reflects = []   # see project memory for full rationale
    return result
```

This recovered the chapter_14 test and ~5 related FP-comparison
files that had similar shapes.

## Failure #2: self-Mov is not LDA + STA

`test_refresh_hit_entities_sim` still failed with "didn't
terminate." That test runs a `do { ... } while ((x & 0x80) == 0)`
loop calling a stub `draw_sprite_opaque` 12 times. The optimized
build was timing out at 500,000 cycles — an infinite loop.

The asm diff looked harmless. A `STA __local_lo` had moved from
inside two if-else branches to a single point at the merge label,
which is what you'd get if some pass noticed both branches ended
with the same `LDA src; STA __local_lo` shape and sunk the STA.
That's mathematically equivalent. Cycle counts shouldn't change
much.

But running just `tests.test_refresh_hit_entities_sim`, the
failure persisted. So the asm I was looking at wasn't broken —
something *else* was.

I added a debug print to my pass: log every load it drops, plus
the state at the drop. The first drop was at the loop header
`.loop@1_start`:

```
DROP at i=4: Mov(src=Data('__local_main__i'), dst=Reg(A))
  state.a = [Data('__local_main__i')]
  state.x = []
  state.y = []
```

Before my CFG dataflow, `LDA __local_main__i` at the loop header
was always preserved (multi-pred → reset). With the dataflow,
state.a held `__local_main__i` on entry to the header → drop the
load.

For the drop to be sound, A would need to hold `__local_main__i`
on every back-edge to the header. The back-edge block ended with
`INC __local_main__i; LDA #$00; ADC #$00; JMP .loop@1_start` — a
leftover from a 2-byte add chain (int promotion of `i + 1`) whose
high byte got truncated. The `LDA #$00; ADC #$00` is a no-op-ish
residue: A ends up holding 0 or 1 depending on carry. Definitely
not `__local_main__i`.

So why was state.a saying it did?

I dumped the IR just before my pass ran. The back-edge body, in
the SSA-destruction output, ended with this:

```
Mov(src=Data('__local_main__i'), dst=Data('__local_main__i'))
Jump(target='.loop@1_start')
```

A self-copy. Move `__local_main__i` to itself.

The asm IR has a mem-to-mem `Mov(M1, M2)` atom — single IR node,
emit lowers it to `LDA M1; STA M2`. `redundant_load.py`'s
tracker handles this: after a mem-to-mem Mov, `state.a = [dst]`
(A now mirrors dst because of the emit-time LDA-STA).

But there's a peephole at `asm_emit.py:513`:

```python
if src == dst:
    return []
```

If src and dst are byte-identical (same register, same ZP byte,
same Data symbol+offset, etc.), emit nothing. No LDA, no STA.
A is NOT loaded.

SSA destruction emits these self-Movs when a Phi src and dst
coalesce to the same byte. The Mov is a no-op at emit time —
but the tracker was treating it as if `LDA M; STA M` happened,
adding `M` to `state.a`. That's the bug.

The fix is one line in the mem-to-mem branch of
`_update_for_mov`:

```python
if _operands_equal(mov.src, mov.dst):
    return   # self-Mov emits nothing — A not loaded
```

With that, state.a at the back-edge JMP was correctly empty,
the intersection at the loop header was empty, and the LDA at
the header was preserved.

12-call loop, terminating.

The interesting thing about this bug is that it was *latent in
the old pass too*. `_update_for_mov`'s mem-to-mem branch has
always treated `Mov(M, M)` as a load. The old per-block tracker
just reset state at every label, so the bogus equivalence never
propagated anywhere useful before getting wiped. My CFG dataflow
exposed it.

## What I'd say to past me

Two things worth holding onto:

**An IR-level tracker's model of emit-time behavior has to match
emit's edge cases.** The mem-to-mem `Mov(M1, M2)` → `LDA; STA`
lowering is almost-always right, except for the one case where
emit folds it to nothing. A tracker that doesn't know about that
fold will produce ghost equivalences in exactly the shape SSA
destruction loves to emit.

**Cross-block flag-effect dataflow is unsound under downstream
DSE.** When you're propagating an equivalence whose producer is
a *separate instruction* (not the load itself), the producers
live on paths. Downstream passes that prune dead code based on
local information will happily drop a producer on one path,
unaware that some other pass concluded the producer was safe to
rely on. The asymmetry is fundamental: an equivalence "A holds
M" can be dropped without consequence (the load is its own
producer). An equivalence "Z reflects M" cannot.

Net change for the originally-reported case: one redundant LDX
gone, plus a handful of incidental drops in other diamond
merges. Net change for me: two soundness gotchas saved as
memory, so the next dataflow pass I write starts from a slightly
less wrong place.
