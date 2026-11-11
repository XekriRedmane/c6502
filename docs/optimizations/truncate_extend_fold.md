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

## What it leaves alone

- A truncate to a width between the source's and the extend's. In
  `uint8_t a; long l = a; return (int)l;`, the `int` result reads
  one byte that came from the zero extension, so the `Truncate`
  from `long` stays.
- An extend of a static or of an address-taken local. In
  `int t = s; s += 1; g = t; return (uint8_t)t;` with a static `s`,
  reading `s` at the truncate would return the incremented value.
- An extend of a constant, which constant folding handles.

## Related

[Narrowing widened arithmetic](narrow_widened_arith.md) removes the
extends around a whole operation when only its low byte is used.
