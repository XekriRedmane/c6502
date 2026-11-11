# Indexed store to a fixed address

`RecognizeIndexedStore` in
[passes/optimization/recognize_indexed_store.py](../../passes/optimization/recognize_indexed_store.py)
turns a byte store through `C + i`, where `C` is a constant address
and `i` a one-byte index, into an `IndexedStore`, which `tac_to_asm`
lowers to `STA C,X`. It runs in the TAC fixed-point loop, after
[direct member access](direct_member_access.md). It doesn't run in
the unoptimized pipeline. The walk back from the store to the index
is shared with the [indexed load](recognize_indexed_load.md) and
[indirect-indexed](recognize_indirect_indexed.md) recognizers, and
lives in
[passes/optimization/extend_add_chain.py](../../passes/optimization/extend_add_chain.py).

A pointer with a fixed value, such as `static uint8_t * const screen
= (uint8_t * const)0x0400`, becomes a constant through the
[static const fold](static_const_fold.md). The subscript
`screen[col]` is then a zero-extend of `col`, a 16-bit add of 0x0400,
and a store through the sum. Without this pass, `tac_to_asm` builds
the sum in a zero-page pair and stores with `STA (ptr),Y`, which
takes 19 bytes in the example below. The 6502's absolute,X mode adds
X to a 16-bit base inside the store instruction, so the same store is
`LDX col; LDA ch; STA $0400,X` with `col` and `ch` in zero page,
which takes 7 bytes. Because X is only 8 bits, the zero high byte of
the extended index costs nothing.

## Example

```c
#include <stdint.h>
static uint8_t * const screen = (uint8_t * const)0x0400;
void put(uint8_t col, uint8_t ch) { screen[col] = ch; }
```

### Before

```
ZeroExtend(@0.col, %0.1)
Binary(+, 1024u, %0.1, %1.1)
Store(@1.ch, %1.1)
Ret(None)
```

### After

```
IndexedStore(0x400, @0.col, @1.ch)
Ret(None)
```

The final assembly is `LDX __zpabi_put__col; LDA __zpabi_put__ch;
STA $0400,X; RTS`.

## When it applies

The pass starts at a `Store` whose address is a variable `%addr`, and
walks back through its definitions. These must all be true.

- `%addr` is an SSA-renamed name that only the store reads, and it's
  defined by an `Add` of a constant `C` and a variable `%ext`, in
  either order.
- `%ext` is an SSA-renamed name defined by a `ZeroExtend` of a
  variable `i` whose C type is one byte (`char`, `signed char`, or
  `unsigned char`). The walk looks through up to
  four `Copy` instructions between `%ext` and the extend. Copy
  propagation keeps a `Copy` that changes signedness at the same
  width, which is how an `int` to `unsigned` cast is lowered.
- `C` is between 0 and 0xFF00, so `C + 255` doesn't wrap past $FFFF.
  The 6502's absolute,X mode wraps at 0x10000, so a higher base
  could reach page zero.
- The stored value is a one-byte variable or a `char` or
  `unsigned char` constant, so that one `STA` writes it.

The instructions don't have to be adjacent. The pass drops the
`Add` along with the store, and drops the extend too unless
something else reads it. That happens when
[CSE](cse.md) shares one extend among several subscripts with the
same index. The volatile bit of the store carries over.

## What it leaves alone

- A `signed char` index, which C99 promotes with a `SignExtend`.
  X is unsigned, so the indexed mode reaches `C + 255` for an index
  of -1 where C means `C - 1`, and a negative offset from a pointer
  into the middle of an object is defined. The access keeps its
  16-bit add. The [loop rotation](loop_rotate.md) pass rewrites the
  `SignExtend` of a count-down counter it proves non-negative into a
  `ZeroExtend`, so `buf[i]` in `for (int8_t i = 15; i >= 0; i--)`
  still folds.
- A value wider than a byte. For a `uint16_t *` constant, the store
  of `v` stays a pointer store, since it would take two `STA C+k,X`
  writes. The index there is also shifted left before the add, which
  this shape doesn't match.
- An index wider than a byte. `screen[i]` with a `uint16_t i` keeps
  its 16-bit add, since X can't hold `i`.
- A base above 0xFF00, such as 0xFF80.
- A base that is a pointer variable. That case goes to the
  [indirect-indexed](recognize_indirect_indexed.md) recognizer after
  the fixed point.
- A store into a static array. Its address comes from a
  `GetAddress`, which isn't a constant. For a small array,
  `c99_to_tac` already emits an `IndexedSymbolStore` on the array's
  label, which also lowers to `STA buf,X`.

## Related

[Combining added constants](reassoc_const.md) folds a constant row
offset into the base first, and
[indexed load](recognize_indexed_load.md) is the matching pass for
loads.
