# Tables split into low and high bytes

`apply_split_tables` in
[cpp/src/passes/optimization/split_tables.h](../../cpp/src/passes/optimization/split_tables.h)
stores a `static const` table of 2-byte elements (or wider ones) as
the low bytes of every element followed by the high bytes, when the
program reads it only by an index that fits in a byte. Each read then
uses the element number as the index, with no shift.
`optimized_tac` in cpp/src/passes/translate_for_codegen.cpp calls it
on the whole program right after the
[dispatch on a small pointer table](dispatch_pointer_array.md), so its
input is the TAC after SSA destruction. It doesn't run in the
unoptimized pipeline.

A table of at most 127 two-byte elements is read with `LDA i; ASL;
TAX` and then `LDA T,X` and `LDA T+1,X`. A longer table doesn't fit
byte offsets, so the read shifts a 16-bit index, adds the table's
address, and reads through a pointer. After the split, both are `LDX
i` and then `LDA T,X` and `LDA T+N,X`, where N is the number of
elements. 6502 programmers lay out their tables this way for the same
reason.

The read is an `IndexedLoad` whose `plane` field is N. Byte k of the
element at index `i` is at `T + offset + i + k * N`, which
`tac_to_asm` lowers as `LDA T+offset+k*N,X`. The table keeps its name,
and its symbol becomes an array of `const` bytes in the new order.
An element that's an address becomes two `AddressByteInit` bytes,
written `DC.B <(name+k)` and `DC.B >(name+k)`.

## Example

```c
#include <stdint.h>
static const uint16_t SCORES[200] = {10, 25, 50, 100};
uint16_t total;
void add_score(uint8_t kind) { total += SCORES[kind]; }
```

These listings compare two complete compiles, with and without the
pass. `add_score` shrinks from 56 bytes to 29. `kind` arrives in A,
so the read after the split starts with `TAX`.

### Before

```
   ASL
   STA   __local_add_score__kind
   LDA   #$00
   ROL
   STA   __local_add_score__1
   LDA   #<SCORES
   CLC
   ADC   __local_add_score__kind
   STA   __local_add_score__kind
   LDA   #>SCORES
   ADC   __local_add_score__1
   STA   __local_add_score__0
   STA   DPTR+1
   LDA   __local_add_score__kind
   STA   DPTR
   LDY   #$00
   LDA   (__local_add_score__kind),Y
   STA   __local_add_score__0
   INY
   LDA   (DPTR),Y
   STA   __local_add_score__kind
   LDA   total
   CLC
   ADC   __local_add_score__0
   ...
```

### After

```
   TAX
   LDA   SCORES,X
   STA   __local_add_score__1
   LDA   SCORES+200,X
   STA   __local_add_score__0
   LDA   total
   CLC
   ADC   __local_add_score__1
   ...
```

## When it applies

The table must be a `StaticVariable` with internal linkage, a
one-dimensional array of at most 256 elements whose type is a `const`
scalar of at least 2 bytes, outside zero page, with an initializer of
integers, floating constants, or addresses. Every reference to it in
the unit must be one of these reads, each into a destination as wide
as an element.

- `IndexedLoad(T, s, dst, offset=o)`, where `o` is a multiple of the
  element size `w` and `s` is a constant multiple of `w`, or is
  defined once in the function, earlier in the same block, as `i <<
  log2(w)` or `i * w` for a one-byte `i`. This is the shape
  `c99_to_tac` writes for a table of at most 255 bytes.
- `Load(p, dst)` with `p = GetAddress(T) + (ZeroExtend(i) <<
  log2(w))`, where every intermediate is defined once, in the load's
  block, and `p` has no other reader. This is the shape for a longer
  table.

The function is no longer in SSA form, so the pass checks that `i`
holds the same value at the load as where the shift read it. Nothing
in between may write `i`. When `i` is a static or its address is
taken, nothing in between may call a function or store through a
pointer either. An instruction of the old address computation goes
once nothing reads its result.

## What it leaves alone

- A table with external linkage, since another unit may read it as
  it is.
- A table whose elements aren't `const`, which a store may change.
- A table whose address is taken, or that another static's
  initializer names, or that the unit reads in any other way, such as
  one byte of an element.
- A read through a volatile index, which the rewrite would move to
  the load.
- A multi-dimensional table, and a table of 1-byte elements, which
  needs no shift.

A function that returns an element in A and X, such as `uint16_t
f(uint8_t i) { return T[i]; }`, still moves the high byte through Y
(`LDA T,X; STA t; LDY T+N,X; TYA; TAX; LDA t`), because X holds the
index. `LDY T,X; LDA T+N,X; TAX; TYA` would be 4 bytes shorter. No
pass makes that change yet.

## Related

[Row-address tables](row_address_tables.md) makes split tables of its
own. [Dispatch on a small pointer table](dispatch_pointer_array.md)
runs first and removes the reads of a table of at most 8 pointers
that it turns into compares.
