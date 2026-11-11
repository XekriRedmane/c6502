# PHA and PLA removal

`apply_dead_pha_pla` in
[passes/dead_pha_pla.py](../../passes/dead_pha_pla.py) drops a `PHA`
and its matching `PLA` when the instructions between them leave A
alone. It runs in the peephole fixed point, after [direct load into X
or Y](direct_index_load.md), in both pipelines. In practice only the
optimized pipeline produces the shape it matches.

A store through a pointer with an index, `p[i] = c`, is lowered by
`tac_to_asm` as a load of `c` into A, then a load of `i` into Y, then
`STA (DPTR),Y`. When `tac_to_asm` lowers it, `i` is still a
pseudoregister that might end up on the soft stack, and loading it
from there goes through A. So the lowering saves `c` with `PHA`
around the index load and restores it with `PLA`. Once direct load
into X or Y has turned `LDA i; TAY` into `LDY i`, nothing between the
`PHA` and the `PLA` touches A, and the pair can go. That saves 2 bytes
and 7 cycles.

## Example

```c
#include <stdint.h>
static uint8_t *p;
static uint8_t i, c;
void put(void) { p[i] = c; }
```

### Before

```
   LDA   p
   STA   DPTR
   LDA   p+1
   STA   DPTR+1
   LDA   c
   PHA
   LDY   i
   PLA
   STA   (DPTR),Y
   RTS
```

### After

```
   LDA   p
   STA   DPTR
   LDA   p+1
   STA   DPTR+1
   LDA   c
   LDY   i
   STA   (DPTR),Y
   RTS
```

## When it applies

From each `PHA`, the pass scans forward to the first `PLA`. Every
instruction in between must neither read nor write A. A write would
be lost when the `PLA` is removed. A read would get the same value
either way, but the pass rejects it to keep the scan simple. The body
also mustn't contain another push or pull, a call, a return, a label,
a jump, or a branch. A label or branch would let another path enter
or leave the body with a different A, and an inner push or pull would
pair with the wrong instruction. When pairs are nested, the inner pair
is removed first, and the outer one on a later call.

The `PLA` sets N and Z from the restored A, and the body may have set
them from something else, such as the `LDY`. So N and Z must be dead
after the `PLA`, as `asm_liveness.flags_dead_at` checks along every
path. `PHA` and `PLA` don't touch C or V, so those aren't checked.

## What it leaves alone

- A body that loads A, such as `LDA i; TAY` when `i` is on the soft
  stack and can't be loaded straight into Y.
- A body with a call, a branch, or a label in it.
- A pair whose `PLA` sets flags that a branch reads.

The unit tests in `tests/test_dead_pha_pla.py` cover each of these.
The unoptimized pipeline doesn't produce the `PHA` at all, because
there `p[i] = c` is lowered as pointer arithmetic on the soft stack
followed by `LDY #$00; STA (DPTR),Y`. The pass made no change on any
`examples/` program or any program under `tests/chapter_*/valid/`
under `--unopt`.

## Related

[Direct load into X or Y](direct_index_load.md) creates the shape
this pass removes, and [indirect indexed
recognition](recognize_indirect_indexed.md) produces the indexed store
in the first place.
