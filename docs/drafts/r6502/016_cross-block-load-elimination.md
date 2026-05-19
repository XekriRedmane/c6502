# Cross-block load elimination: diamond merges and a self-Mov gotcha

c6502 has a peephole pass (`redundant_load.py`) that drops `LDA M` /
`LDX M` / `LDY M` when the target register already holds a copy of
`M`. The original was a single-pass linear walker that reset its
register-equivalence tracker at every basic-block boundary, with
one carve-out: a `Label` whose only predecessor was a single Branch
/ Jump got the saved snapshot restored, so `JMP target; target:
LDA M` patterns still drop the second LDA.

That carve-out misses the diamond merge. Concretely, in one of the
example functions I had this:

```
.if_end@1:
   LDX   __zpabi_entity_proximity__slot
   ...
.if_end@2:
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$40
   BCC   .if_end@5
   CMP   #$47
   BCS   .if_end@5
   ; body — doesn't touch X or the slot
.if_end@5:
   LDX   __zpabi_entity_proximity__slot   ; redundant
   LDA   companion_dir,X
   ...
```

Both predecessors of `.if_end@5` (the two BCC/BCS branches and the
fall-through) leave X holding the slot. Neither path touches X or
the slot in between. The second LDX is dead weight. But with two
predecessors, the linear pass resets state at the label and can't
see it.

The fix is the textbook one: replace the linear walker with a
forward must-availability dataflow over the asm CFG, joining at
multi-predecessor labels by intersection on `_operands_equal`. An
equivalence "A === M" survives into a block iff every incoming
path agreed on it. Iterate to a fixed point so loop back-edges
fall out naturally.

That's the easy part. Two soundness gotchas surfaced when I ran
the full test suite.

**Gotcha 1: cross-block Z propagation breaks under DSE.**

`redundant_load.py` also tracks a `z_reflects` list — operands
whose zeroness the Z flag currently matches. The point is to skip
`LDA M; Branch(EQ)` when Z already reflects `M`, so the LDA's flag
side-effect is unobservable. Propagating that list across the CFG
seemed harmless — Z carries through Branches and Jumps unchanged.

It isn't harmless. An "A === M" entry has the load itself as its
producer; dropping the consumer load based on cross-block
agreement leaves the producer in place. A `z_reflects = [M]`
entry, in contrast, records that some *upstream* instruction set
Z. At a multi-pred join, every path can agree "Z reflects M" via
a *different* upstream producer. When I dropped a consumer LDA
based on that, the next dead-store pass dropped one path's
producer (it doesn't track Z), and Z was stale at the Branch.

The fix: intersect a/x/y across multi-pred joins, but DON'T carry
`z_reflects` across them. Single-pred is fine — that's a
Jump/Branch carrying Z through, and the producer on the single
incoming path is intact.

**Gotcha 2: self-Mov is not LDA + STA.**

The asm IR has a mem-to-mem `Mov(M1, M2)` atom; emit lowers it to
`LDA M1; STA M2`. The tracker accounts for the implicit LDA by
setting `state.a = [M2]` after the Mov — A now mirrors M2.

But there's a special case: `Mov(M, M)`. Self-copies show up when
SSA destruction emits a Phi copy between two SSA names that
coalescing landed at the same byte. The emit-time peephole
(`asm_emit.py:513`) drops the whole atom: `src == dst` → emit
nothing. No LDA, no STA. A is NOT loaded.

The tracker was claiming "A === M" after a self-Mov it had no
business making. The diamond join then collapsed across loop
back-edges that ended in `Mov(__local_main__i, __local_main__i);
JMP loop_header`, deciding "A holds `__local_main__i` on every
back-edge entry to the header" — and dropping the `LDA
__local_main__i` at the header that was actually needed because
A held the residue of a leftover `LDA #$00; ADC #$00` (high byte
of a 2-byte add that asm_dead_store hadn't yet trimmed).

Symptom: loops didn't terminate. `CMP #$0C` was reading garbage
instead of the loop counter.

Fix in one line:

```python
# In _update_for_mov, mem-to-mem branch:
if _operands_equal(mov.src, mov.dst):
    return  # self-Mov emits nothing; A not loaded
```

**Net result on the example corpus:** the targeted `LDX
__zpabi_..._slot` drop landed; a handful of unrelated diamond
merges also folded (one example file lost a local entirely as
the round-trip through it became visible end-to-end and got
collapsed). Per-function size deltas are 1–3 bytes; the
correctness win is the more interesting half.

The lesson I'm taking away: when a peephole tracker has a model
of emit-time behavior, that model has to match emit's edge cases
too. Self-Mov is "the LDA happens" *almost* always — except for
the exact case the emit peephole catches. And cross-block flag
propagation is sound for a single-pass tracker that resets at
every label, but the moment you start joining states across the
CFG, the producers can be on *paths*, and the rest of the
pipeline doesn't know that.
