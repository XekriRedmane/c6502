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
examples/zork/input.c went from 24 bytes, which built
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

## An index known to fit with the offset

An access at a constant offset `k` through a pointer that is itself
`p + ZeroExtend(b)`, for a byte `b`, is `(p),Y` with Y = `b + k` when
`b`'s largest value plus `k` fits in a byte. There's no carry out of
the byte sum then, so `(p),Y` reaches the address the two 16-bit adds
did. The largest value comes from `b`'s definition: a constant, a
left shift by a constant, which clears the low bits, a logical right
shift, or an AND with a constant, through copies; anything else
counts as 255. The rewrite is made only for `k` of 1 or more, and only
when the access is the sum's only reader, so the sum's 16-bit add
goes. A loop that also needs the sum, as `entry += stride; ...
entry[0]` in Zork's `match_dictionary_word`, would otherwise keep the
old pointer and the new one alive.

Zork's abbreviation lookup reads a word table at an even index, `i =
z << 1`:

```
   ASL                                     ASL
   STA   __local_print_zstring__ch         STA   __local_print_zstring__c_0
   LDY   __local_print_zstring__ch         LDY   __local_print_zstring__c_0
   LDA   (Z_ABBREV_TABLE),Y                LDA   (Z_ABBREV_TABLE),Y
   STA   __local_print_zstring__0    ->    STA   __local_print_zstring__ch
   LDA   Z_ABBREV_TABLE                    INY
   CLC                                     LDA   (Z_ABBREV_TABLE),Y
   ADC   __local_print_zstring__ch
   STA   __local_print_zstring__ch
   LDA   Z_ABBREV_TABLE+1
   ADC   #$00
   STA   __local_print_zstring__c_0
   LDY   #$01
   LDA   (__local_print_zstring__ch),Y
```

which is the original's `INY`, 14 bytes shorter in `print_zstring`
(examples/zork/zstring.c).

## An index shared by several accesses

`p[i] &= m` reads and writes `p + ZeroExtend(i)`. When every reader of
the sum is a 1-byte `Load` from it or `Store` to it, each becomes
`(p),Y` with Y = `i`, and the sum goes once the last one is rewritten
(`_shared_extend_add_chain`). `i` must be SSA-renamed, and `p` must
hold at each access the value the sum read.

A reader may also be `(p + ZeroExtend(i)) + k`, for a constant `k`
outside 0 to 255, read by one 1-byte access. Common subexpression
elimination turns `background[x]`, with `background = line + 0x2000`,
into `(line + x) + 0x2000` so that it shares `line + x` with
`line[x]`. That access becomes `(t),Y` with Y = `x` and `t = line +
0x2000`, a pointer plus a constant, made just before the access
(`_offset_from_extend_chain`). A `k` of 0 to 255 is left as `(q),Y`
with Y = `k`, since several such offsets share `q` where each would
need a pointer of its own.

Lode Runner's wipe draws a block of the closing or opening circle
with

```c
if (wipe_dir == 0)
    line[c->byte] &= WIPE_BLOCK_CLOSE_MASK[c->pixel];
else
    line[c->byte] |= line[c->byte + 0x2000] & WIPE_BLOCK_OPEN_MASK[c->pixel];
```

The closing branch had added `c->byte` to `line` in a zero-page pair
and read and written through it with Y = 0. It now loads Y with
`c->byte` and reads and writes `(line),Y`, and the opening branch adds
0x20 to a copy of `line`'s high byte. Together with the same shapes
in `sprites.c` and `level.c`, the program is 79 bytes shorter.

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
