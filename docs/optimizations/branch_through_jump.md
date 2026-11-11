# Branch through jump

`apply_branch_through_jump` in
[passes/branch_through_jump.py](../../passes/branch_through_jump.py)
points a conditional branch whose target label holds only a `JMP`
straight at the `JMP`'s target, and deletes the label and the `JMP`
when nothing else reaches them. It runs in the peephole fixed point,
in both the optimized and the unoptimized pipelines.

The asm SSA round trip splits the back edge of a loop whose exit test
sits at the bottom, so the branch back goes through a block that holds
only `JMP .loop@0_start`. Each time the branch is taken, the 6502 then
spends 3 more cycles on the `JMP`. Branching straight to the loop head
saves those 3 cycles per iteration, and deleting the trampoline saves
3 bytes. Without `--optimize` the pass finds the same shape in nested
`if` statements and around `goto`.

## Example

```c
#include <stdint.h>
uint8_t src[16], dst[16];
void copy(void)
{
    for (int8_t x = 15; x >= 0; x--)
        dst[x] = src[x];
}
```

### Before

```
   LDX   #$0F
.loop@0_start:
   LDA   src,X
   STA   dst,X
   DEX
   BPL   .copy@asm_ssa_split@0
   RTS
.copy@asm_ssa_split@0:
   JMP   .loop@0_start
```

### After

```
   LDX   #$0F
.loop@0_start:
   LDA   src,X
   STA   dst,X
   DEX
   BPL   .loop@0_start
   RTS
```

When the code before the label falls into it, the pass retargets the
branch and keeps the trampoline. In

```c
if (a) { if (b) g = 1; } else { g = 2; }
```

the inner `if` ends at `.if_end@2: JMP .if_end@0`, which `STA g` falls
into. The pass turns `BEQ .if_end@2` into `BEQ .if_end@0` and leaves
the `JMP` in place for the fall-through path.

## When it applies

- The branch targets a label `L` in the function. Past `L`, skipping
  any further labels and any non-volatile `Mov` whose source and
  destination are the same, the first instruction is a `Jump` to a
  label `T` in the same function.
- The pass follows chains of such labels, up to 16 deep, so a branch
  to a trampoline that jumps to another trampoline goes to the final
  target.
- The label and the `JMP` are deleted when no other `Jump`, `Branch`,
  or `Phi` argument names `L` after the retargeting, and the
  instruction before `L` is a `Jump`, `Return`, or `Ret`, so nothing
  falls into it. The no-op moves and unreferenced labels between `L`
  and the `JMP` go with it.

## What it leaves alone

- A `JMP` to another function, as a tail call leaves it. A branch
  can't leave its function, so `BEQ L` with `L: JMP tick` stays.
- A `JMP` whose target label is a trampoline. Only conditional
  branches are retargeted.
- A trampoline that something else still names or falls into, as in
  the nested `if` above.

The retargeted branch can be further from its target than the
trampoline was. If it ends up out of range, `expand_long_branches`
rewrites it after the peepholes.

## Related

[Branch inversion](branch_invert.md) handles the other trampoline
shape, a branch over a `JMP`. [Dead label drop](dead_label_drop.md)
deletes the labels this pass leaves without references.
