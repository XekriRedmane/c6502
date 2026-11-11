# Common subexpression elimination

`eliminate_common_subexpressions` (the
`EliminateCommonSubexpressions` pass) in
[passes/optimization/cse.py](../../passes/optimization/cse.py)
replaces an instruction that computes the same value as an earlier one
with a `Copy` from the earlier one's result. It runs in the TAC
fixed-point loop under `--optimize`, on SSA form, just before
[loop-invariant code motion](licm.md). The unoptimized pipeline
doesn't run it.

C that repeats a subscript or a test makes the TAC compute the same
thing twice, and on the 6502 a repeated `key[1]` through a pointer
costs an `LDY` and an `LDA (zp),Y` each time. The pass walks the
dominator tree with a table of the computations it has seen. When an
instruction matches an entry, it becomes `Copy(earlier, dst)`, and
[copy propagation](copy_propagation.md) and
[dead-store elimination](dead_store_elimination.md) then forward and
delete the copy. Two kinds of instruction are matched. A pure
computation (`Binary`, `Unary`, `ZeroExtend`, `SignExtend`, or
`Truncate`) depends only on its operands, so in SSA form an identical
one in a dominating block has the same result. A load also depends on
memory, so an earlier load is reused only when nothing could have
written memory in between.

## Example

```c
#include <stdint.h>
uint8_t order(const uint8_t *key, const uint8_t *entry) {
    if (key[1] < entry[0]) return 0;
    if (key[1] == entry[0]) return 1;
    return 2;
}
```

The pass fires twice on this function. Its first call replaces the
second test's address computations. `Binary(+, @0.key, 1, %8.1)`
becomes `Copy(%0.1, %8.1)` and `Binary(+, @1.entry, 0, %11.1)` becomes
`Copy(%3.1, %11.1)`. Copy propagation then gives the second test's
loads the same operands as the first test's, and the second call
reuses the loads.

### Before

```
Binary(+, @0.key, 1, %0.1)
Load(%0.1, %1.1)
Binary(+, @1.entry, 0, %3.1)
Load(%3.1, %4.1)
JumpIfCmp(>=, %1.1, %4.1, .if_end@0)
Ret(0uc)
Label(.if_end@0)
Load(%0.1, %9.1)
Load(%3.1, %12.1)
JumpIfCmp(!=, %9.1, %12.1, .if_end@1)
Ret(1uc)
Label(.if_end@1)
Ret(2uc)
```

### After

```
Binary(+, @0.key, 1, %0.1)
Load(%0.1, %1.1)
Binary(+, @1.entry, 0, %3.1)
Load(%3.1, %4.1)
JumpIfCmp(>=, %1.1, %4.1, .if_end@0)
Ret(0uc)
Label(.if_end@0)
Copy(%1.1, %9.1)
Copy(%4.1, %12.1)
JumpIfCmp(!=, %9.1, %12.1, .if_end@1)
Ret(1uc)
Label(.if_end@1)
Ret(2uc)
```

With the pass disabled, the second test compiles to `LDY #$01; LDA
(key),Y; DEY; CMP (entry),Y; BNE .if_end@1`. With it, the second test
compares the same two values as the first, and the final assembly is a
lone `BNE .if_end@1` on the flags the first `CMP` left. That saves 7
bytes and 14 cycles on the path that reaches the second test.

The other common source is a run of subscripts off one byte index,
such as `table[n + 1] + table[n + 2] + table[n + 3]`. Each subscript
zero-extends `n`, and the pass turns the second and third
`ZeroExtend(@0.n, ...)` into `Copy(%0.1, ...)` from the first.

## When it applies

An instruction is a candidate when its dst is an SSA value, so that
replacing its one definition changes nothing else. Every operand must
be a Constant or an SSA value. The match key contains the instruction
kind, the operator, the operands, and the dst's C type, so
`ZeroExtend(c)` to `int` and to `long` aren't merged. The operands of
`+`, `*`, `&`, `|`, `^`, `==`, and `!=` are sorted first, so `a + b`
matches `b + a`.

- A pure computation is reused from any earlier instruction in the
  same block or in a block that dominates this one.
- A non-volatile `Load`, `IndexedLoad`, `IndexedConstLoad`, or
  `IndirectIndexedLoad` is reused only along a chain of blocks where
  each block's one predecessor is the block before it, so every path
  to the reuse passes through the earlier load. The table of loads is
  emptied by any store, any call, any volatile access, and any write to
  a name that isn't an SSA value (a static or an address-taken local).

## What it leaves alone

- A repeated load after a store, as in `a = *p; p[1] = 5; return a +
  *p;`, since the store could have changed `*p`.
- A repeated load in a block with more than one predecessor, such as
  the code after an `if`, even when both paths passed through the
  first load.
- A computation on a static or an address-taken local, such as `g + 1`
  twice with a global `g`. Those names aren't SSA values, so another
  function or a pointer could change them between the two reads.
- A volatile load, which C99 §6.7.3 requires to happen each time.
- `Copy`, `GetAddress`, calls, and Phis, which the pass doesn't match.

## Related

[Copy propagation](copy_propagation.md) and
[dead-store elimination](dead_store_elimination.md) finish the job.
[Offset reassociation](reassoc_offset.md) rewrites `p + (n + k)` as
`(p + n) + k` so that subscripts like `dict[n + 1]` and `dict[n + 2]`
share `dict + n` here.
