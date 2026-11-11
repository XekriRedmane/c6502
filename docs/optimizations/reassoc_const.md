# Combining added constants

`ReassocConstants` in
[passes/optimization/reassoc_const.py](../../passes/optimization/reassoc_const.py)
rewrites `(v + C2) + C1` as `v + (C1 + C2)`, with the two constants
added at compile time. It runs in the TAC fixed-point loop, after
copy folding and before [offset reassociation](reassoc_offset.md).
It doesn't run in the unoptimized pipeline.

Constant folding can't combine the two constants on its own, because
they sit in different instructions with a variable between them.
Each multi-byte add on the 6502 is a load, an `ADC`, and a store per
byte, so dropping one of the two adds removes that whole chain. The
case that motivated the pass is an address. Once the
[static const fold](static_const_fold.md) and the const-array
subscript fold in [constant folding](constant_folding.md) have
turned a `static uint8_t * const` base and a `static const` row
table entry into constants, the address is a constant plus a
constant plus a zero-extended byte. Combining the constants leaves
one constant plus the byte, which is the shape the
[indexed store](recognize_indexed_store.md) recognizer matches.

## Example

```c
#include <stdint.h>
static uint8_t * const page = (uint8_t * const)0x2000;
static const uint16_t rows[4] = {0x000, 0x080, 0x100, 0x180};
void plot(uint8_t col, uint8_t v) { page[rows[2] + col] = v; }
```

### Before

```
ZeroExtend(@0.col, %4.1)
Binary(+, 256u, %4.1, %6.1)
Binary(+, 8192u, %6.1, %7.1)
Store(@1.v, %7.1)
Ret(None)
```

### After

```
ZeroExtend(@0.col, %4.1)
Binary(+, 8448u, %4.1, %7.1)
Store(@1.v, %7.1)
Ret(None)
```

The listing prints constants in decimal, so 8192 is 0x2000 and 8448
is 0x2100. The indexed-store recognizer then turns the last three
instructions into one `IndexedStore`, and the function compiles to
`LDX __zpabi_plot__col; LDA __zpabi_plot__v; STA $2100,X; RTS`.

## When it applies

- The outer instruction is an `Add` of a constant and a variable
  `%inner`, in either order.
- `%inner` is an SSA-renamed name, so it has exactly one definition,
  and that definition is an `Add` of a constant and any other
  operand, in either order. The two adds don't have to be adjacent.
- `%inner` is read only by the outer add.
- The two constants are integers of the same bit width. They may
  differ in signedness, since an add produces the same bits either
  way. The combined constant takes the outer constant's type, and
  the sum wraps at that width, as the 6502's byte-by-byte `ADC`
  chain does.

The inner add is dropped in the same rewrite. A chain of three or
more adds collapses one pair per sweep of the fixed point.

## What it leaves alone

- An inner sum with a second reader. In
  `unsigned t = x + 3; g = t; return t + 5;` the value `x + 3` is
  stored to `g` too, so rewriting the return as `x + 8` would add a
  second add instead of removing one.
- A subtract on either side. `(x - 3) + 5` keeps both instructions.
  Folding mixed adds and subtracts takes tracking the signs through
  the chain, which isn't implemented.
- Constants of different widths, which only meet across an extend
  that sits between the two adds.
- Floating-point constants, which aren't in the pass's table of
  integer widths.
- A name that isn't SSA-renamed, such as a static, because it may
  have more than one definition. The pass also skips the in-place
  form `x = x + C`, which would otherwise match itself and lose the
  add.

## Related

[Offset reassociation](reassoc_offset.md) handles a variable pointer
plus an index plus a constant, where only one constant is present.
