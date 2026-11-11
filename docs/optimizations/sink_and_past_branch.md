# Sinking an AND past a bit test

`SinkAndPastBranch` in
[passes/optimization/sink_and_past_branch.py](../../passes/optimization/sink_and_past_branch.py)
moves the computation of `x & C`, on a byte `x`, from just before a
`JumpIfMasked` that tests `x` into the start of both successor
blocks. It runs in the TAC fixed-point loop, after
[sinking increments](sink_increment.md) and before
[narrowing widened arithmetic](narrow_widened_arith.md). It doesn't
run in the unoptimized pipeline.

The shape comes from code like `uint8_t mag = b & 0x7F; if (b &
0x80) ...`, after the [AND jump fold](and_zero_jump_fold.md) has
turned the test of bit 7 into a `JumpIfMasked`. The test lowers to
`LDA b; BPL`, which reads N from the load. With the AND computed
first, A holds `b & 0x7F` at the branch, so the allocator keeps the
masked value in a zero-page byte and the branch reloads `b` from
memory. With a copy of the AND in each successor, the branch reads
the flags of the one load, and each successor computes `AND #$7F`
from the `b` still in A. In the example below the function body is
3 bytes shorter and the second `LDA bob,X` is gone, and on the
`else` path the masked value never goes to memory.

## Example

```c
#include <stdint.h>
uint8_t bob[8];
uint8_t y;
void step(uint8_t i) {
    uint8_t b = bob[i];
    uint8_t mag = b & 0x7F;
    if (b & 0x80) y -= mag; else y += mag;
}
```

### Before

```
IndexedLoad(bob, %1.1, %2.1)
ZeroExtend(%2.1, %3.1)
Binary(&, %3.1, 127, %4.1)
Truncate(%4.1, %5.1)
JumpIfMasked(%2.1, 128, False, .if_else@1)
ZeroExtend(%5.1, %8.1)
...
Jump(.if_end@0)
Label(.if_else@1)
ZeroExtend(%5.1, %12.1)
...
```

### After

```
IndexedLoad(bob, %1.1, %2.1)
JumpIfMasked(%2.1, 128, False, .if_else@1)
ZeroExtend(%2.1, %3.1.snk_then@0)
Binary(&, %3.1.snk_then@0, 127, %4.1.snk_then@0)
Truncate(%4.1.snk_then@0, %5.1.snk_then@0)
ZeroExtend(%5.1.snk_then@0, %8.1)
...
Jump(.if_end@0)
Label(.if_else@1)
ZeroExtend(%2.1, %3.1.snk_else@0)
Binary(&, %3.1.snk_else@0, 127, %4.1.snk_else@0)
Truncate(%4.1.snk_else@0, %5.1.snk_else@0)
ZeroExtend(%5.1.snk_else@0, %12.1)
...
```

The final assembly starts `LDX __zpabi_step__i; LDA bob,X;
BPL .if_else@1; AND #$7F`, and the `else` block starts with
`AND #$7F; CLC; ADC y`. With the pass switched off, the function computes `AND #$7F` and
stores it before the branch, then reloads `bob,X` for the `BPL`.

## When it applies

The pass looks at the last four instructions of each basic block.

- The block ends in `JumpIfMasked(x, ...)`, and the three
  instructions before it are, in order, `ZeroExtend(x, %e)`,
  `Binary(&, %e, C, %a)` with the operands in either order, and
  `Truncate(%a, %t)`. The jump tests the same `x` that the extend
  reads.
- `x` is a variable whose C type is `char`, `signed char`, or
  `unsigned char`.
- `C` is an integer constant between 0 and 0xFF.
- Every reader of `%t` is in the block the jump falls through to or
  in the block that starts with the jump's target label. A reader
  after the two paths merge would require a `Phi`, which the pass
  doesn't insert.

The pass inserts a renamed copy of the three instructions at the
start of each successor, after its label if it has one, with names
ending in `.snk_then@N` and `.snk_else@N`. It rewrites each reader of
`%t` to the copy in its own block. The copy always uses an `int`
constant for the mask.

The pass doesn't count the readers of `%e` and `%a`, although the
module docstring lists single use of both as a condition. It drops
the original definitions in any case. When the same extend also feeds
another reader, as in

```c
uint8_t b = bob[i];
int w = b;
uint8_t mag = b & 0x7F;
if (b & 0x80) y -= mag; else y += mag;
g16 = w;
```

the store to `g16` after the merge reads `%3.1`, which no longer has
a definition, and the final assembly stores two zero-page bytes that
nothing writes.

## What it leaves alone

- A trio that isn't directly in front of the `JumpIfMasked`. With
  `uint8_t mag = b & 0x7F; unsigned w = b;` the extend for `w` ends
  up between the AND and the jump.
- A masked value used after the two paths join, such as `z = mag;`
  after the `if`. Narrowing then turns the trio into a one-byte
  `Binary(&, %2.1, 127uc, %5.1)` in front of the jump, and the
  masked value goes to memory before the branch.
- A mask above 0xFF, and a test of a variable wider than a byte.

## Related

[Narrowing widened arithmetic](narrow_widened_arith.md) runs right
after this pass. It rewrites the same trio to a one-byte AND, which
this pass doesn't match, so the order inside the fixed point
matters.
