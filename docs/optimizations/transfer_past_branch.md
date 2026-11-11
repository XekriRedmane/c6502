# TXA past a branch

`apply_transfer_past_branch` in
[passes/transfer_past_branch.py](../../passes/transfer_past_branch.py)
moves a `TXA` or `TYA` from just before a conditional branch to just
after it, when only the fall-through path reads A. It runs in the
peephole fixed point, in both pipelines. It hasn't fired on the
chapter test corpus or `examples/` in either pipeline, and the
example below was written for this page. The unoptimized pipeline
doesn't keep variables in X or Y between statements, so it doesn't
produce the shape.

A branch after `LDY m; TYA` reads the flags that `TYA` set, which
reflect Y. The `LDY` had already set the same flags from the same
value, so the branch goes the same way without the `TYA` in front of
it. Moving the `TYA` past the branch means the taken path no longer
runs it, which saves 2 cycles on that path. The size doesn't change.

## Example

```c
#include <stdint.h>
uint8_t tab[8], g;
void f(uint8_t i)
{
    uint8_t j = tab[i];
    if (j & 0x80)
        g = j & 0x0F;
    else
        g = 1;
    tab[j] = 0;
}
```

### Before

```
   LDX   __zpabi_f__i
   LDY   tab,X
   TYA
   BPL   .if_else@1
   AND   #$0F
   STA   g
   JMP   .if_end@0
.if_else@1:
   LDA   #$01
   STA   g
.if_end@0:
   LDA   #$00
   STA   tab,Y
   RTS
```

### After

```
   LDX   __zpabi_f__i
   LDY   tab,X
   BPL   .if_else@1
   TYA
   AND   #$0F
   STA   g
   JMP   .if_end@0
.if_else@1:
   LDA   #$01
   STA   g
.if_end@0:
   LDA   #$00
   STA   tab,Y
   RTS
```

## When it applies

- Three consecutive instructions. The first writes X or Y and sets N
  and Z from the new value, such as `LDX`, `LDY`, `TAX`, `TAY`, `INX`,
  or `DEY`. The second is a non-volatile `TXA` or `TYA` from that
  register. The third is a conditional branch.
- The branch target is a label in the function, and A is dead there
  (`asm_liveness.a_dead_at`).
- A is live on the fall-through path.

## What it leaves alone

- A branch whose taken path reads A. In `LDX m; TXA; BPL T` with `T:
  STA h`, the store reads the A that the `TXA` wrote before the
  branch.
- A transfer with A dead on both paths.
  [Dead A arithmetic](dead_a_arith.md), which runs earlier in each
  round, deletes such a transfer outright.
- A first instruction that sets the flags from something other than
  the register, as in `LDA m; TXA; BPL T`, where the branch reads
  flags that only the `TXA` set from X.
- A branch to a label outside the function, where A might be read.

## Related

[Dead A arithmetic](dead_a_arith.md) deletes the transfer when
neither path reads A.
