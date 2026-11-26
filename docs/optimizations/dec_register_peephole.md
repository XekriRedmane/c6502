# DEX and DEY for a register counter

`apply_dec_register_peephole` in
[cpp/src/passes/dec_register_peephole.h](../../cpp/src/passes/dec_register_peephole.h)
turns a decrement of a value held in X or Y, which `tac_to_asm`
routes through A and a temporary, into `DEX` or `DEY`. It runs in the
peephole fixed point, in both pipelines, but only fires with
`-O`. The unoptimized pipeline keeps a `register` local on
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
   STA   __local_wait__delay
   LDY   __local_wait__delay
.loop@0_continue:
   TYA
   SEC
   SBC   #$01
   STA   __local_wait__delay
   LDY   __local_wait__delay
   BNE   .loop@0_continue
.loop@0_break:
   RTS
```

### After

```
   STA   __local_wait__delay
   LDY   __local_wait__delay
.loop@0_continue:
   DEY
   BNE   .loop@0_continue
.loop@0_break:
   RTS
```

`delay` arrives in A and is stored in `__local_wait__delay` on the
way to Y. That byte is dead once Y holds it, so the subtract's
temporary takes the same byte.

`volatile` keeps the empty loop from being deleted. The volatile
counter's storage is Y itself, and `DEY` reads and writes Y once, so
the rewrite keeps the one read and one write that `--y` performs.

Since 19 Nov 2026, [redundant load elimination](redundant_load.md)
makes `STA __local_wait__delay; LDY __local_wait__delay` a `TAY`, and
the same for the temporary inside the loop, so the pass meets the
five instructions `TYA; SEC; SBC #$01; TAY; BNE .loop@0_continue`, and
the whole function is `TAY; .loop@0_continue: DEY; BNE
.loop@0_continue; RTS`. The listing above is from before that change.

## When it applies

- Six consecutive instructions, `TXA` or `TYA`, `SEC`, `SBC #1`, a
  store of A to a temporary, a load of the same temporary into the
  same register, and a `BNE` or `BEQ`. The rewrite is `DEX` or `DEY`
  followed by the branch.
- Or five instructions, the same with `TAX` or `TAY` in place of the
  store and the load.
- `DEX` or `DEY` leaves A, C, and V alone where the `SBC` wrote them,
  so A, C, and V must be dead after the branch on both paths, by
  `asm_liveness`'s `a_dead_at`, `c_dead_at`, and `v_dead_at`, which
  count a return of a value in A as a read of it.
- In the six-instruction form, no path from the branch reads the
  temporary before it writes it, including a path that goes round a
  loop to the top. Until 26 Nov 2026 the pass looked only at the
  instructions after the pattern in program order, and didn't check
  C or V for this form.

The walk of the temporary's readers doesn't look inside calls, since
the pass has no `AsmContext` to ask what a callee reads.

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
