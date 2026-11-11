# Dead label drop

`apply_dead_label_drop` in
[passes/dead_label_drop.py](../../passes/dead_label_drop.py) deletes
every label in a function that no jump, branch, or phi argument
names. It runs in the peephole fixed point, in both the optimized and
the unoptimized pipelines, just before
[unreachable code drop](unreachable_drop.md) and
[branch inversion](branch_invert.md).

A label emits no code, so deleting one changes neither the size nor
the speed of the program. It matters because many peepholes match
instructions that are adjacent in the IR, and a label between two
instructions breaks the match. The asm SSA round trip leaves a label
at the head of every block it built (`.f@asm_ssa_block@N`,
`.f@asm_ssa_preheader@0`), and loop labeling leaves
`.loop@N_continue` on a loop that has no `continue`. Without
`--optimize` the pass still finds those `continue` labels, as in
`for (i = 0; i < n; i++) buf[i] = 0;`.

## Example

```c
#include <stdint.h>
uint8_t buf[40];
void fill(uint8_t v)
{
    uint8_t i = 39;
    for (;;) {
        buf[i] = v;
        if (i == 0)
            break;
        i--;
    }
}
```

### Before

```
.fill@asm_ssa_preheader@0:
.fill@ssa_block@0:
   LDX   #$27
.loop@0_start:
   LDA   __zpabi_fill__v
   STA   buf,X
   TXA
   BNE   .if_end@0
.fill@asm_ssa_block@0:
   JMP   .loop@0_break
.if_end@0:
   DEX
.loop@0_continue:
   JMP   .loop@0_start
.loop@0_break:
   RTS
```

### After

```
   LDX   #$27
.loop@0_start:
   LDA   __zpabi_fill__v
   STA   buf,X
   TXA
   BNE   .if_end@0
   JMP   .loop@0_break
.if_end@0:
   DEX
   JMP   .loop@0_start
.loop@0_break:
   RTS
```

With `.fill@asm_ssa_block@0` gone, the `BNE` and `JMP` are adjacent,
and branch inversion turns them into `BEQ .loop@0_break` later in the
same round. That leaves `.if_end@0` with no reference, and this pass
deletes it on its next call.

## When it applies

The pass collects the target of every `Jump` and `Branch` and the
predecessor label of every `Phi` argument in the function, and deletes
each `Label` whose name isn't among them. The function's entry point
is the function's name and isn't a `Label` in the IR, so it's never a
candidate.

## What it leaves alone

- A label that some jump, branch, or phi argument in the function
  names, even when the instruction that names it can't be reached.
  [Unreachable code drop](unreachable_drop.md) removes such
  instructions, and the label goes on a later call.
- The instructions after a deleted label. Only the label goes.

## Related

[Branch inversion](branch_invert.md),
[branch to next label](branch_to_next_drop.md), and
[branch through jump](branch_through_jump.md) all leave labels behind
that this pass removes. Branch inversion matches only when no label
sits between the branch and the jump.
