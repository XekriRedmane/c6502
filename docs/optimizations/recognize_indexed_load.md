# Indexed load from a fixed address

`RecognizeIndexedLoad` in
[passes/optimization/recognize_indexed_load.py](../../passes/optimization/recognize_indexed_load.py)
turns a byte load through `C + i`, where `C` is a constant address
and `i` a one-byte index, into an `IndexedConstLoad`, which
`tac_to_asm` lowers to `LDA C,X`. It runs in the TAC fixed-point
loop, right after the [indexed store](recognize_indexed_store.md)
recognizer, and shares that pass's walk back from the access in
[passes/optimization/extend_add_chain.py](../../passes/optimization/extend_add_chain.py).
It doesn't run in the unoptimized pipeline.

A table at a fixed address, read through a pointer such as
`static const uint8_t * const font = (const uint8_t * const)0xD000`,
gives the TAC a zero-extend of the index, a 16-bit add of the
constant base, and a `Load` through the sum. `tac_to_asm` would build
the sum in a zero-page pair and read with `LDA (ptr),Y`, which is 17
bytes in the example below. With absolute,X the read is
`LDX i; LDA $D000,X`, which is 5 bytes.

## Example

```c
#include <stdint.h>
static const uint8_t * const font = (const uint8_t * const)0xD000;
uint8_t glyph_row(uint8_t i) { return font[i]; }
```

### Before

```
ZeroExtend(@0.i, %0.1)
Binary(+, 53248u, %0.1, %1.1)
Load(%1.1, %2.1)
Ret(%2.1)
```

### After

```
IndexedConstLoad(0xD000, @0.i, %2.1)
Ret(%2.1)
```

The listing prints constants in decimal, so 53248 is 0xD000. The
final assembly is `LDX __zpabi_glyph_row__i; LDA $D000,X; RTS`.

## When it applies

The conditions are the indexed-store recognizer's, applied to a
`Load`.

- The loaded value is a one-byte variable.
- The load's address is an SSA-renamed `%addr` that only the load
  reads, defined by an `Add` of a constant `C` and an extended
  index, in either order.
- The index is a `ZeroExtend` of a variable whose C type is one
  byte, possibly reached through up to four `Copy` instructions.
- `C` is between 0 and 0xFF00.

The add is dropped with the load, and so is the extend unless
something else reads it. The volatile bit of the load carries over.

## What it leaves alone

- A `signed char` index, which C99 promotes with a `SignExtend`.
  X is unsigned, so the indexed mode reaches `C + 255` for an index
  of -1 where C means `C - 1`, and a negative offset from a pointer
  into the middle of an object is defined. The access keeps its
  16-bit add. The [loop rotation](loop_rotate.md) pass rewrites the
  `SignExtend` of a count-down counter it proves non-negative into a
  `ZeroExtend`, so `buf[i]` in `for (int8_t i = 15; i >= 0; i--)`
  still folds.
- A value wider than a byte. Reading `words[i]` through a
  `uint16_t *` constant keeps the pointer load, and its index is
  shifted left before the add, which the pattern doesn't match.
- An index wider than a byte, such as a `uint16_t i`.
- A base above 0xFF00.
- A pointer variable as the base, as in `p[i]` on a parameter
  `p`. The [indirect-indexed](recognize_indirect_indexed.md)
  recognizer turns that into `LDA (p),Y` after the fixed point.
- A subscript of a static array, which `c99_to_tac` lowers as an
  `IndexedLoad` on the array's own label.

## Related

[Indexed store](recognize_indexed_store.md) is the matching pass for
stores, and [constant folding](constant_folding.md) folds an
`IndexedLoad` of a `static const` array at a constant index to the
element's value.
