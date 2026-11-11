# DEX and DEY for a register counter

`apply_dec_register_peephole` in
[passes/dec_register_peephole.py](../../passes/dec_register_peephole.py)
turns a decrement of a value held in X or Y, which `tac_to_asm`
routes through A and a temporary, into `DEX` or `DEY`. It runs in the
peephole fixed point, in both pipelines, but only fires with
`--optimize`. The unoptimized pipeline keeps a `register` local on
the soft stack, so it never produces the shape.

A `register` local lives in Y. For `--y`, `tac_to_asm` emits a byte
subtract through A, and the result travels back to Y through a
zero-page temporary. `TYA; SEC; SBC #$01; STA tmp; LDY tmp` takes 8
bytes and 12 cycles, and `DEY` takes 1 byte and 2 cycles, which saves
10 cycles on every pass through a delay loop. `DEY` sets N and Z from
the new Y, so the branch on the result reads the same flags.

## Example

```c
#include <stdint.h>
void wait(uint8_t delay)
{
    register volatile uint8_t y = delay;
    while (--y != 0) { }
}
```

### Before

```
   LDY   __zpabi_wait__delay
.loop@0_continue:
   TYA
   SEC
   SBC   #$01
   STA   __local_wait__0
   LDY   __local_wait__0
   BNE   .loop@0_continue
.loop@0_break:
   RTS
```

### After

```
   LDY   __zpabi_wait__delay
.loop@0_continue:
   DEY
   BNE   .loop@0_continue
.loop@0_break:
   RTS
```

`volatile` keeps the empty loop from being deleted. The volatile
counter's storage is Y itself, and `DEY` reads and writes Y once, so
the rewrite keeps the one read and one write that `--y` performs.

## When it applies

- Six consecutive instructions, `TXA` or `TYA`, `SEC`, `SBC #1`, a
  store of A to a temporary, a load of the same temporary into the
  same register, and a `BNE` or `BEQ`. The rewrite is `DEX` or `DEY`
  followed by the branch.
- A is dead at both successors of the branch. The pass's own check
  looks at the first instruction on each path, skipping labels and
  no-op moves and following unconditional jumps. A is dead if that
  instruction writes A, is a call that takes no argument in A, or is
  a return. A read of A, or another branch, counts as live. A branch
  to a label outside the function counts as dead.
- The temporary isn't read by any instruction after the pattern, in
  program order.

The check on the temporary doesn't look at instructions before the
pattern, so a read that a loop's back edge reaches isn't seen. No C
program has been found that puts such a read there.

The pass doesn't check C or V. `SBC` sets both and `DEY` leaves them
alone, so the rewrite relies on c6502's rule that no flag is read
across separate operations and every add and subtract starts with its
own `CLC` or `SEC`.

## What it leaves alone

- A counter in memory. A plain or `volatile` local without `register`
  lives in zero page, where its decrement goes through `DEC` instead
  ([DEC](dec_peephole.md)).
- A subtract of anything other than 1, and a branch other than `BNE`
  or `BEQ`, such as the `BPL` of a signed count-down.
- A decrement whose result A still holds when a successor reads it.

## Related

[Y known zero after a DEY loop](dec_branch_indirect_y_fold.md) uses
the `DEY; BNE` this pass produces. [DEC](dec_peephole.md) and
[counter to index register](counter_to_index_reg.md) handle counters
in memory.
