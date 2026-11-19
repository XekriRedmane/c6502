# Branch threading on known values

`apply_branch_value_thread` in
[passes/branch_value_thread.py](../../passes/branch_value_thread.py)
points a branch or a `JMP` past code at its target whose tests the
path into it has already decided. It runs in the peephole fixed
point, which both pipelines run.

`if (b == 2) ... else if (b == 4) ...` on a 2-byte `b` tests `b`'s
high byte for each comparison. The path that leaves the first test
because the high byte isn't 0 arrives at the second with A holding
that byte, and the second test reloads it and branches the same way.
The path that leaves because the low byte isn't 2 knows the high
byte is 0, so the second test's high-byte branch isn't taken, and
the low byte it loads next is already in A. Each branch can go past
the tests it knows the answer to.

## Example

From `instr_mul` in `examples/zork/numbers.c`, with [cross
jumping](cross_jump.md) switched off in both compiles.

### Before

```
   LDA   __zpabi_check_sign__b_1
   CMP   #$00
   BNE   .if_else@5
   LDA   __zpabi_check_sign__b_0
   CMP   #$02
   BNE   .if_else@5
   ASL   __local_instr_mul__product
   ROL   __local_instr_mul__0
   JMP   .if_end@4
.if_else@5:
   LDA   __zpabi_check_sign__b_1
   CMP   #$00
   BNE   .if_else@7
   LDA   __zpabi_check_sign__b_0
   CMP   #$04
   BNE   .if_else@7
   ...
.if_else@7:
   LDA   __zpabi_check_sign__b_0
   STA   __zpabi_mulu16__b_0
```

### After

```
   LDA   __zpabi_check_sign__b_1
   CMP   #$00
   BNE   .if_else@7
   LDA   __zpabi_check_sign__b_0
   CMP   #$02
   BNE   .vthread@0
   ASL   __local_instr_mul__product
   ROL   __local_instr_mul__0
   JMP   .if_end@4
.if_else@5:
   LDA   __zpabi_check_sign__b_1
   CMP   #$00
   BNE   .if_else@7
   LDA   __zpabi_check_sign__b_0
.vthread@0:
   CMP   #$04
   BNE   .vthread@1
   ...
.if_else@7:
   LDA   __zpabi_check_sign__b_0
.vthread@1:
   STA   __zpabi_mulu16__b_0
```

The first `BNE` goes straight to `.if_else@7`, the multiply. The
second goes to the `CMP #$04`, with A still holding the low byte, and
the third goes past the multiply's reload of that byte, which saves 3
cycles on the way to `mulu16`. No branch reaches the high-byte test
at `.if_else@5` any more, and [unreachable code
drop](unreachable_drop.md) removes it with the low-byte load after
it. That's 6 bytes in each of `instr_mul` and `instr_div`, which now
test `b` the way Zork's code does. The `CMP #$00` after each load of
the high byte goes later, in [redundant compare](redundant_compare.md).

## When it applies

- A conditional branch, or a `JMP`, goes to a label in the function.
- The straight-line code before it, back to the previous label, tells
  the pass what A holds (a byte of memory or a constant), what N and
  Z were set from (a load of a byte or a `CMP #k` of one), and what
  the branches it passed decided about bytes of memory: that one
  equals a constant, or doesn't. A store forgets what it knew about
  every byte it may write, and a call, or any instruction the scan
  doesn't model, forgets everything.
- From the target, the pass simulates loads of a byte or a constant
  into A, `CMP #k` of A, and branches, following each branch it can
  decide, for at most 24 instructions.
- The edge goes to the furthest point the simulation reaches where A
  holds what it held on the edge, or is dead, N and Z are the same or
  dead, and C is unchanged or dead. When no label is there, the pass
  adds one, `.vthread@N`, numbered across the program.

## What it leaves alone

- A volatile byte, whose loads it doesn't simulate.
- Code at the target that writes memory, X, or Y, or that does
  arithmetic in A.
- A fall-through edge, which would need a `JMP` added.

## Related

[Branch threading on a known flag](branch_thread.md) handles a target
that starts with a branch on the flag the first branch tested.
[Redundant compare](redundant_compare.md) drops a repeated compare at
a label that only such branches reach.
