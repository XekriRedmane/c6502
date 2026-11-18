# Indexed access to a static array of structs

`static_indexed_access` in
[passes/optimization/static_indexed_access.py](../../passes/optimization/static_indexed_access.py)
turns a read or write of `o[i].m`, where `o` is a static array, into
`IndexedLoad(o, i, offset=m)` or `IndexedSymbolStore(o, i, v,
offset=m)`, which `tac_to_asm` lowers as `LDX i; LDA o+m,X`. It runs
once, after the TAC fixed point and before
[indirect-indexed access through a pointer](recognize_indirect_indexed.md),
under `-O`.

Four other changes make the index a byte for it to work with, and
put a constant part of it in the offset:

- `c99_to_tac` scales an index into an array object at byte width
  when the array spans at most 255 bytes and has at least two
  elements (`_translate_subscript_address`). Every index C defines
  then lands within those bytes (C99 §6.5.6.8: anything else is
  undefined), so the offset fits a byte: the index is truncated to a
  byte, multiplied there, and zero-extended. `prompts[which]` on a
  `struct prompt prompts[3]` of 16-byte records shifts `which` left
  four times in A instead of shifting a 16-bit value. An array of one
  element is left alone, since a struct's last `[1]` member is often
  indexed past its end.
- The TAC `IndexedLoad` and `IndexedSymbolStore` carry a constant
  `offset`, added to the address of every byte they access. Every
  pass that rebuilds one keeps it.
- `c99_to_tac` writes `arr[x + k]` on a static array of at most 256
  bytes, with `x` an unsigned byte and `k` a constant from 0 to the
  last index, as `IndexedLoad(arr, x, offset=k)` (scaled by the
  element size), and the same for a store (`_split_index_addend`).
  The sum can't wrap, since `x` is at most 255 and the add is at
  least 16 bits wide, and an index C defines leaves `x` times the
  element size within a byte. Zork's `instr_call` reads
  `LOCAL_ZVARS[x + 1]` as `LDA LOCAL_ZVARS+1,X`, where it computed
  `x + 1` in A and kept it in a slot. `(uint8_t)(x + 1)` wraps at 255,
  and the cast keeps the add in place. The constant may be a `sizeof`,
  as in `((uint8_t *)HI_SCORES)[i + sizeof(struct hi_score)]`. A
  constant index is the element's byte offset, so `t[2]` reads `t+2`
  in both pipelines.
- A subscript of a cast of an object's address, `((T *)&obj)[i]` or
  `*((T *)&obj + i)`, reads `obj` as an array of `T`
  (`c99_to_tac._object_view`). C99 §6.5.7 allows the access for any
  object when `T` is a character type, and it's how Lode Runner reads
  the packed BCD `uint32_t SCORE` a byte at a time. When `obj` has
  static storage and spans at most 256 bytes, and `T` is an integer
  or pointer type, `c99_to_tac` lowers the subscript as it lowers one
  of a static array at `obj`'s address, to `IndexedLoad(obj, i)` or
  `IndexedSymbolStore(obj, i, v)`. `SCORE_BYTES[3 - i]` in Lode
  Runner's `beats` is then `LDA SCORE,X`, where it built the pointer
  `SCORE + (3 - i)` in two zero-page bytes and read `LDA (p),Y`. That
  took `beats` from 87 bytes to 61, the size it had when `SCORE` was
  declared as a byte array. The access names `obj`, so every pass
  that tracks `obj`'s value treats it as a read or write of `obj`. A
  compound assignment or an increment goes through the address, as
  one of an array element does, and `_byte_offset_scale` scales its
  index at byte width when `obj` spans at most 255 bytes, so this pass
  makes it indexed. An automatic object is left to the pointer, since
  it has no link-time address.

The rewrite is exact for any size of object: `abs,X` adds the index
to the full 16-bit address `o+m`, as the `Load` through `o + i + m`
did.

A pointer to a member that's passed on, rather than read through, is
`(&o + i) + m`. The pass rewrites it as `(&o + m) + i`, so the
constant part is one link-time address, which
[link-time address arithmetic](label_offset_fold.md) turns into
`#<(o+m)` / `#>(o+m)`, and the pointer takes one 16-bit add.

## Example

From `get_prompted_number_from_user` in
`examples/zork/prompts.c`:

```c
cout_string(&prompts[which].answer, 1);
...
if (key == 0x8D)
    key = prompts[which].answer;
```

### Before

```
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_get_prompted_number_from_user__0
   LDA   #>prompts
   ADC   #$00
   STA   __local_get_prompted_number_from_user__key
   LDA   __local_get_prompted_number_from_user__0
   CLC
   ADC   #$0F
   ...
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_get_prompted_number_from_user__key
   LDA   #>prompts
   ADC   #$00
   STA   __local_get_prompted_number_from_user__0
   LDY   #$0F
   LDA   (__local_get_prompted_number_from_user__key),Y
```

### After

```
   CLC
   LDA   #<(prompts+15)
   ADC   __local_get_prompted_number_from_user__1
   STA   __zpabi_cout_string__s_0
   LDA   #>(prompts+15)
   ADC   #$00
   STA   __zpabi_cout_string__s_1
   ...
   LDX   __local_get_prompted_number_from_user__1
   LDA   prompts+15,X
```

The pointer goes straight into `cout_string`'s parameter bytes (see
[backward copy propagation](backward_copy_propagation.md)). Zork's own
routine does the same, with a byte offset it keeps in `prompt_offset`.

## When it applies

- The address of a `Load` or `Store` (or an `IndirectIndexedLoad` /
  `IndirectIndexedStore` with a constant index) is `GetAddress(o) +
  ZeroExtend(b)`, optionally plus a constant, where `o` is a static
  object, `b` is one byte, and each intermediate is an SSA value.
- For the reassociation, an `Add` of such a sum and a nonzero
  constant.

The address computation stays in place for anything else that uses
it, and dead-code elimination removes it otherwise.

## What it leaves alone

- An automatic array, or one reached through a pointer: the base
  isn't a link-time address.
- An index wider than a byte, as in an array of more than 255 bytes,
  or of one element.
- A volatile access is rewritten too, with `is_volatile` kept.

## Related

[Indexed load from a fixed address](recognize_indexed_load.md) does
the same for a constant numeric base, and [direct member
access](direct_member_access.md) for a constant offset with no index.
