# Indexed access to a static array of structs

`static_indexed_access` in
[passes/optimization/static_indexed_access.py](../../passes/optimization/static_indexed_access.py)
turns a read or write of `o[i].m`, where `o` is a static array, into
`IndexedLoad(o, i, offset=m)` or `IndexedSymbolStore(o, i, v,
offset=m)`, which `tac_to_asm` lowers as `LDX i; LDA o+m,X`. It runs
once, after the TAC fixed point and before
[indirect-indexed access through a pointer](recognize_indirect_indexed.md),
under `--optimize`.

Two other changes make the index a byte for it to work with:

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
`examples/zork/instr_save/prompts.c`:

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
