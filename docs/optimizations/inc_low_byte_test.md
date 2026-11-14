# Testing the low byte after an INC chain

`apply_inc_low_byte_test` in
[passes/inc_low_byte_test.py](../../passes/inc_low_byte_test.py)
folds a test of the low byte of a value just incremented by an
[INC chain](inc_peephole.md) into the chain's first branch. It runs
in the peephole fixed point, which both pipelines run, but only the
optimized pipeline increments a multi-byte value in memory, so it
fires only there.

The INC peephole turns `x++` on a 2- or more-byte value in memory
into `INC M0; BNE done; INC M1; ...; done:`. A test of the low byte
right after, `if ((uint8_t)x == 0)`, then loads it at `done` and
branches. But the chain already decides the test: its first `BNE`
is taken exactly when the new low byte isn't zero, and every other
path to `done` (the fall-through, and the `BNE` after each higher
byte) has it zero. So the first `BNE` can go where the test would
have gone, and the test after `done` can never branch. The common
case, a low byte that doesn't wrap, skips a load and a branch.

## Example

```c
#include <stdint.h>
struct pc { uint16_t lo; uint8_t hi; } P;
uint8_t valid;
void next(void) {
    P.lo++;
    if ((uint8_t)P.lo == 0)
        valid = 0;
}
```

These listings compare two complete compiles. The `LDA P` that the
pass leaves after the chain is dead, and a later pass deletes it.

### Before

```
next:
   INC   P
   BNE   .inc_done@0
   INC   P+1
.inc_done@0:
   LDA   P
   BNE   .if_end@0
   LDA   #$00
   STA   valid
.if_end@0:
   RTS
```

### After

```
next:
   INC   P
   BNE   .if_end@0
   INC   P+1
   LDA   #$00
   STA   valid
.if_end@0:
   RTS
```

For a `BEQ` after the load, the first `BNE` goes past the `BEQ`, to a
new `.inc_test@N` label, and the `BEQ` becomes a `JMP`.

## When it applies

- The chain starts with `INC M0` on a `Data` or `ZP` byte, followed
  by `BNE done`, and goes on with `INC` and `BNE done` pairs to an
  `INC` and the label `done`. Nothing else branches to `done`.
- Right after `done` come a load of `M0` into A that isn't volatile
  and a `BNE` or `BEQ`.
- A is dead where the moved branch now arrives (by `a_dead_at`),
  since that path skips the load. The flags there are the same:
  `INC M0` set N and Z from the new low byte as the load did, and
  neither touches C or V.

## What it leaves alone

- A `BMI` or `BPL` after the load. The chain doesn't decide the sign
  of the low byte.
- A load of another byte than `M0`.
- A path into `done` from outside the chain.

## Related

[Branch on INC or DEC flags](dec_inc_branch_fold.md) deletes the
load when it comes right after a single `INC` or `DEC`, and
[members computed in place](in_place_member_update.md) produces the
in-place increment of a struct member.
