# Branch to next label

`apply_branch_to_next_drop` in
[passes/branch_to_next_drop.py](../../passes/branch_to_next_drop.py)
deletes a conditional branch to the label right after it, and a `JMP`
to one of the labels right after it. It runs in the peephole fixed
point, in both the optimized and the unoptimized pipelines, right
after [branch inversion](branch_invert.md).

Both arms of such a branch arrive at the same place, so the branch
changes nothing but the cycle count. A jump to the next label has no
effect either. A branch costs 2 bytes and 2 or 3 cycles, and a `JMP`
costs 3 bytes and 3 cycles. The shape appears when the passes before it have
emptied the arms of a diamond. The usual source is a sign extension
whose high byte turns out to be unused. `tac_to_asm` lowers it as
`BMI neg; LDA #$00; JMP done; neg: LDA #$FF; done:`, the dead-store
and dead-A passes delete both loads, and branch inversion turns
`BMI neg; JMP done; neg:` into `BPL done`.

## Example

```c
#include <stdint.h>
int8_t s;
void neg(void) { s = -s; }
```

### Before

```
   LDA   s
   BPL   .sx_done@1
.sx_done@1:
   EOR   #$FF
   CLC
   ADC   #$01
   STA   s
   RTS
```

### After

```
   LDA   s
   EOR   #$FF
   CLC
   ADC   #$01
   STA   s
   RTS
```

The jump form comes from an `if` whose `else` arm is empty. For
`if (x) tick(); else ;` the pass turns `JSR tick; JMP .if_end@0;
.if_else@1: .if_end@0: RTS` into `JSR tick; .if_else@1: .if_end@0:
RTS`, and dead label drop then deletes `.if_end@0`.

## When it applies

- A `Branch` to `L` immediately followed by `Label(L)`. The branch
  goes. The label goes in the same call when that branch was its only
  reference, counting every `Jump`, `Branch`, and `Phi` argument in
  the function. Otherwise the label stays.
- A `Jump` to `L` followed by a run of labels that includes `L`. The
  jump goes, and the labels stay for [dead label
  drop](dead_label_drop.md) to judge.

## What it leaves alone

- A branch with a label between it and its target label, as in
  `BPL L2; L1: L2:`. The branch case looks only at the instruction
  right after the branch, so it fires once dead label drop has
  removed `L1`, and stays put when something names `L1`.
- A branch or jump with any instruction between it and its target.
- Two stacked branches to the same following label. One call deletes
  the inner one, and the outer one goes on the next call.

## Related

[Branch inversion](branch_invert.md) produces most of the branches
this pass deletes. [Dead A arithmetic](dead_a_arith.md) and [asm dead
store](asm_dead_store.md) empty the arms of the sign-extension
diamond.
