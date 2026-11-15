# Indirect-indexed access through a pointer

`RecognizeIndirectIndexed` in
[passes/optimization/recognize_indirect_indexed.py](../../passes/optimization/recognize_indirect_indexed.py)
turns a byte load or store through `p + i`, where `p` is a pointer
variable and `i` a one-byte index or a constant from 0 to 255, into
an `IndirectIndexedLoad` or `IndirectIndexedStore`. `tac_to_asm`
lowers those with the 6502's `(zp),Y` mode. The pass runs once,
after the TAC fixed point has converged and before SSA destruction.
It shares its walk back from the access with the
[indexed store](recognize_indexed_store.md) recognizer, in
[passes/optimization/extend_add_chain.py](../../passes/optimization/extend_add_chain.py).
It doesn't run in the unoptimized pipeline.

For `p[i]`, `c99_to_tac` zero-extends `i`, adds it to `p` with a
16-bit add, and loads through the sum. `tac_to_asm` builds the sum
in a zero-page pair and reads it with `LDY #$00; LDA (sum),Y`. The
`(zp),Y` mode adds Y to the pointer, carrying into the high byte, so
loading `i` into Y does the same add for free. In the example below
the function body drops from 17 bytes to 4. A constant offset such
as `p[10]` or a byte member `o->y` becomes `LDY #k` before the
access.

The pass runs after the fixed point so that a pointer that's going
to fold to a constant has already done so, and gone to the cheaper
absolute,X form of [indexed store](recognize_indexed_store.md) or
[indexed load](recognize_indexed_load.md). Inside the fixed point it
could lock a pointer into `(zp),Y` one iteration before constant
folding would have turned the pointer into a constant.

## Example

```c
#include <stdint.h>
uint8_t at(const uint8_t *p, uint8_t i) { return p[i]; }
```

### Before

```
ZeroExtend(@1.i, %0.1)
Binary(+, @0.p, %0.1, %1.1)
Load(%1.1, %2.1)
Ret(%2.1)
```

### After

```
IndirectIndexedLoad(@0.p, @1.i, %2.1)
Ret(%2.1)
```

The final assembly is `LDY __zpabi_at__i; LDA (__zpabi_at__p_0),Y;
RTS`. For `p[10] = v`, the pass turns `Binary(+, @2.p, 10, %3.1);
Store(@3.v, %3.1)` into `IndirectIndexedStore(@2.p, 10uc, @3.v)`,
which lowers to `LDA __zpabi_put10__v; LDY #$0A;
STA (__zpabi_put10__p_0),Y`.

## When it applies

The pass starts at a `Load` or `Store` whose address is an
SSA-renamed `%addr` defined by an `Add`. It accepts one of two shapes
for the `Add`.

- A variable `p` plus `%ext`, where `%ext` is an SSA-renamed
  `ZeroExtend` of a one-byte variable `i`, possibly
  reached through up to four `Copy` instructions. The index is `i`.
- A non-constant `p` plus an integer constant from 0 to 255. The
  index is that constant as an `unsigned char`.

With the first shape, the access must be the only reader of `%addr`.
With the second, `%addr` may have other readers when each one is a
one-byte load from it or store to it, and `p` holds the same value
at each of them. `p[1]++` loads from `p + 1` and stores to it, and
both become `LDY #1` accesses. An SSA-renamed `p` always holds the
same value. Any other `p`, such as a global, must not be volatile,
and the `Add` must be in the access's block with no call, store, or
assignment to `p` between them. Zork's `parse[1]++` in
examples/zork/interpreter/input.c went from 24 bytes, which built
`parse + 1` in a zero-page pair, to 12.

These must also be true.

- `p` is a variable whose definition isn't a chain of `Copy`
  instructions ending in a constant.
- A load writes a one-byte variable, and a store writes a one-byte
  variable or a `char` or `unsigned char` constant.

The pass doesn't check the C type of `p`. A pointer to a type wider
than a byte scales the index with a shift or multiply first, and
that doesn't match the shape. The pass drops the `Add`, and drops the
extend too unless something else reads it. An `Add` that several
accesses read stays until the asm byte DCE removes it, since each
access is rewritten on its own and the later ones find the
`Add` through the address. The volatile bit of the access carries
over.

## What it leaves alone

- A `signed char` index, which C99 promotes with a `SignExtend`. Y
  is unsigned, so `(p),Y` would read `p + 255` for `p[i]` with an
  `int8_t i` of -1, and for a pointer into the middle of an array C
  defines `p[-1]` as the byte before `p`. The access keeps its
  sign extension and 16-bit add, and reads through the sum with
  `LDY #$00`.
- A value wider than a byte. For a `const uint16_t *p`, `p[i]`
  shifts `i` left before the add, and the load stays a pointer load.
- A constant offset above 255, such as `p[300]`.
- An address with a constant offset that something besides one-byte
  loads and stores reads, such as `q = p + 1;` next to `*q = 0;`.
- An index wider than a byte, such as a `uint16_t i`.
- A constant base, which the fixed point has already handed to the
  absolute,X recognizers.

## Related

[Direct member access](direct_member_access.md) turns an
`IndirectIndexed` access with a constant index on a `GetAddress`
pointer into a direct access to the object.
