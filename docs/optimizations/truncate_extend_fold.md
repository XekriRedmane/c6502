# Truncate of an extend

`FoldTruncateExtend` in
[passes/optimization/truncate_extend_fold.py](../../passes/optimization/truncate_extend_fold.py)
rewrites `Truncate(%t, u)`, where `%t` is a `ZeroExtend` or
`SignExtend` of `x`, to read `x` directly. The result is
`Copy(x, u)` when `u` and `x` have the same width, or a narrower
`Truncate(x, u)`. It runs in the TAC fixed-point loop, after the
indexed load recognizer. It doesn't run in the unoptimized pipeline.

C's integer promotions widen every `char` operand to `int`, and a
cast or assignment back to `char` truncates it again. The low bytes
of an extend are the bytes of its source, so a truncate that reads
only those bytes can read the source instead. Once no truncate reads
the extended value, dead-store elimination drops the extend, and
with it the instructions that build the high byte. When another
instruction still reads the extended value, as the store to `g`
does below, the extend stays, and the truncate becomes a `Copy` that
copy propagation can forward.

## Example

```c
#include <stdint.h>
int g;
uint8_t f(uint8_t a) { int t = a; g = t; return (uint8_t)t; }
```

### Before

```
ZeroExtend(@0.a, %0.1)
Copy(%0.1, g)
Truncate(%0.1, %1.1)
Ret(%1.1)
```

### After

```
ZeroExtend(@0.a, %0.1)
Copy(%0.1, g)
Copy(@0.a, %1.1)
Ret(%1.1)
```

Copy propagation then forwards `@0.a` into the `Ret`, and the
function returns with `LDA __zpabi_f__a`.

## When it applies

- The truncate's source is an SSA-renamed variable whose one
  definition is a `ZeroExtend` or a `SignExtend`.
- The extend's source `x` is an SSA-renamed variable too. The
  rewrite moves the read of `x` from the extend to the truncate, and
  only an SSA-renamed name is sure to hold the same value at both
  places.
- The truncate's destination is no wider than `x`. Both widths come
  from the symbol table, and a type without a known width disables
  the rewrite.

The extended value may have other readers. Each truncate of it is
rewritten on its own, and dead-store elimination removes the extend
after the last reader goes.

A truncate to a width between the source's and the extend's keeps
some of the extend's bytes. Those are the bytes the same kind of
extend to the narrower width makes, so in `uint8_t a; long l = a;
return (int)l;` the `Truncate` from `long` becomes `ZeroExtend(a)` to
`int`. A truncate of a truncate is one truncate of the first one's
source.

## An extend of an extend

`FoldExtendExtend`, in the same module, runs right after
`FoldTruncateExtend` and rewrites an extend of a widening extend of
`x` as one extend of `x`. A `ZeroExtend` that widens leaves the top
bit of its result 0, so a `SignExtend` of that result adds zero bytes,
the same bytes a `ZeroExtend` adds.

| Inner | Outer | Result |
|---|---|---|
| `ZeroExtend` | `ZeroExtend` or `SignExtend` | `ZeroExtend(x)` |
| `SignExtend` | `SignExtend` | `SignExtend(x)` |
| `SignExtend` | `ZeroExtend` | unchanged |

The last row stays, because its middle bytes copy the sign of `x` and
its top bytes are 0, which neither extend of `x` makes. The gates are
the ones above. Both names must be SSA-renamed, since `x` is read at
the outer extend in place of the inner one, and the widths must come
from the symbol table and grow strictly at each step.

C converts a byte compared with `sizeof` in two steps. In

```c
#include <stdint.h>
uint8_t a[40], b[40];
void f(void) {
    for (uint8_t i = 0; i < sizeof a; i++)
        a[i] = b[i];
}
```

`i` is promoted to `int` and then converted to `size_t`, which is
`unsigned long`, so the compare read `SignExtend(ZeroExtend(i))`. The
[comparison narrowing](cmp_zero_jump_fold.md) follows one
`ZeroExtend`, and the loop compared four bytes, with the sign
extension of a high byte known to be 0 before it:

```
.loop@0_start:
   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_f__1
   STA   __local_f__0
   TXA
   SEC
   SBC   #$28
   LDA   #$00
   SBC   #$00
   LDA   __local_f__1
   SBC   #$00
   LDA   __local_f__0
   SBC   #$00
   BCS   .loop@0_break
```

With one `ZeroExtend` the compare narrows to a byte, and the loop is
`INX; CPX #$28; BCC`. Lode Runner's `editor.c` had cast each `sizeof`
to `uint8_t` to get that.

## What it leaves alone

- An extend of a static or of an address-taken local. In
  `int t = s; s += 1; g = t; return (uint8_t)t;` with a static `s`,
  reading `s` at the truncate would return the incremented value.
- An extend of a constant, which constant folding handles.

## Related

[Narrowing widened arithmetic](narrow_widened_arith.md) removes the
extends around a whole operation when only its low byte is used.
