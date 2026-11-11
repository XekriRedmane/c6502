# Branch inversion

`apply_branch_invert` in
[passes/branch_invert.py](../../passes/branch_invert.py) replaces a
conditional branch around a `JMP` with one branch on the opposite
condition to the `JMP`'s target. It runs in the peephole fixed point,
in both the optimized and the unoptimized pipelines.

`tac_to_asm` lowers a conditional jump in TAC whose sense is the wrong
way round for a fall-through as a branch over an unconditional jump,
as in `BNE L; JMP T; L:`. The 6502 has a branch for each condition and
its opposite (`BEQ`/`BNE`, `BCC`/`BCS`, `BMI`/`BPL`, `BVC`/`BVS`), so
`BEQ T; L:` does the same thing. The branch and the jump take 5
bytes, and the inverted branch takes 2. The path that used to fall
into the `JMP` takes 3 cycles instead of 5, and the path that used to
take the branch takes 2 cycles instead of 3.

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

### After

```
   LDX   #$27
.loop@0_start:
   LDA   __zpabi_fill__v
   STA   buf,X
   TXA
   BEQ   .loop@0_break
.if_end@0:
   DEX
   JMP   .loop@0_start
.loop@0_break:
   RTS
```

The pass keeps `.if_end@0`, since another instruction could name it.
Here nothing does, and [dead label drop](dead_label_drop.md) deletes
it on its next call.

## When it applies

- A `Branch` to label `L`, then a `Jump` to `T`, then `Label(L)`, with
  nothing in between. An unreferenced label between the branch and
  the jump blocks the match until dead label drop deletes it, which
  happens earlier in the same round.
- `T` is a label in the same function.

The inverted branch can be further from its target than the original
was from `L`. If it ends up out of the 6502's branch range,
`expand_long_branches` rewrites it after the peepholes as an inverted
branch over a `JMP`, the shape this pass started from.

## What it leaves alone

- A `Jump` to a name that isn't a label in the function, which is a
  tail call to another routine. A branch can't be measured against
  such a target, because its address isn't known until link time, and
  `expand_long_branches` only measures branches within one function.
- A branch over anything other than a single `Jump`.
- A branch whose target isn't the label right after the `Jump`.

## Related

[Branch to next label](branch_to_next_drop.md) deletes a branch or
jump to the label right after it, a shape this pass can leave behind
when the code between the branch and the label has been deleted.
[Branch through
jump](branch_through_jump.md) retargets a branch whose target label
holds only a `JMP`.
