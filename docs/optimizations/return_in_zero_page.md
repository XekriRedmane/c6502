# A pointer returned in zero page

`return_pointers_in_zero_page` in
[cpp/src/passes/return_in_zero_page.h](../../cpp/src/passes/return_in_zero_page.h)
makes a `static` function that returns a pointer leave it in a
zero-page pointer of its own, `F@ret`, where its callers read through
it. It runs once on the unoptimized TAC, right after translation to
TAC, and only under `-O`.

A function returns a pointer in A and X, low byte first
([calling_conventions.md](../calling_conventions.md)). The 6502 reads
through a pointer only with `(zp),Y`, so a caller that reads through
the result stores it into a zero-page pair first, `STA p; STX p+1`,
4 bytes at each call. A callee that computes the low byte first, as
an add does, also has to park it while it computes the high one and
then load it again. Zork's own `get_object_addr` leaves the address
in `SCRATCH2`, and its callers read through `SCRATCH2`.

## Example

```c
#include <stdint.h>
uint8_t *table;
static uint8_t *entry(uint8_t n) { return table + n * 4; }
uint8_t second(uint8_t n) { return entry(n)[1]; }
```

### Before

```
entry:
   ...
   LDA   table
   CLC
   ADC   __local_entry__1
   STA   __local_entry__0
   LDA   table+1
   ADC   __local_entry__2
   TAX
   LDA   __local_entry__0
   RTS
second:
   JSR   entry
   STA   __local_second__0
   STX   __local_second__1
   LDY   #$01
   LDA   (__local_second__0),Y
   RTS
```

### After

```
entry:
   ...
   LDA   table
   CLC
   ADC   __local_entry__0
   STA   entry@ret
   LDA   table+1
   ADC   __local_entry__1
   STA   entry@ret+1
   RTS
second:
   JSR   entry
   LDY   #$01
   LDA   (entry@ret),Y
   RTS
```

These are two complete compiles, with the pass turned off and on, and
`...` marks the lines they share. `entry` is 1 byte shorter and the
call 4 bytes shorter.

## How it works

The rewrite is in TAC. In the function, each `Ret(v)` becomes `Copy(v,
F@ret); Ret()`. At each call, `FunctionCall(F, args, d)` becomes
`FunctionCall(F, args); Copy(F@ret, d)`. `F@ret` is a static with
`[[c6502::zeropage]]` and the function's return type, added to the
symbol table and the program, so the
[zero-page plan](zp_promotion.md) that follows gives it 2 bytes and
the linker keeps it in zero page. Its `StaticVariable` has no initial
bytes, so no `__zp_init` stores anything into it. Nothing reads it
before a call has written it.

The copy in the caller goes in the asm.
[Memory value propagation](memory_value_propagation.md) follows that
the pair `d` was copied from `F@ret`, and turns a `(d),Y` access into
`(F@ret),Y` where nothing in between may have written `F@ret`. Then
the copy's stores are dead. A call, or a store through a pointer,
which could reach zero page, ends that, so a pointer used after one
keeps its copy.

In Zork's interpreter (examples/zork) it applies to
`get_object_addr` in `objects.c` and `get_dictionary_addr` in
`input.c`, 22 bytes in the two units.

## When it applies

- The function is defined in the unit and has internal linkage, so
  every call to it is in the unit and goes through the rewrite.
- It returns a pointer.
- Its address isn't taken (`address_taken` in cpp/src/passes/abi_selection.h), since a
  call through a pointer expects the result in A and X.

## What it leaves alone

- A function with external linkage. A unit that calls it can't know
  which convention another unit compiled it with.
- A function returning an integer. A 2-byte integer is more often
  passed on to another function's parameter slots, stored, or
  computed with, where it costs less in A and X.
- A result read after a later call. Its copy stays, and costs `LDA;
  STA` for each byte, 8 bytes where `STA` and `STX` took 4. In
  `objects.c`, `remove_obj` and `instr_insert_obj` keep the object's
  address across calls to `get_object_addr` for the next object, and
  `attr_ptr_and_mask` stores the result into the static `attr_ptr`.
  The two units still come out 22 bytes shorter.

## Related

[Carry-flag return without the annotation](carry_return_choice.md)
also changes how a `static` function returns, for a `bool`.
