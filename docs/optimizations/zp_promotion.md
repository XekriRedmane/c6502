# Zero-page promotion

`--optimize` moves the busiest globals into zero page, where nobody
declared them with `[[c6502::zeropage]]`. The choice is made in
`plan_zero_page` and `_promotion_order` in
[passes/zp_globals.py](../../passes/zp_globals.py), on the unoptimized
TAC, before the TAC optimizer runs, so that every pass after it knows
where the globals are. From then on a promoted global is a zero-page
object like a declared one ([attributes.md](../attributes.md)).

A zero-page access is a 2-byte, 3-cycle instruction where an absolute
one is 3 bytes and 4 cycles. A pointer in zero page also serves as an
`(zp),Y` base directly, where a pointer in the data segment is first
copied to `DPTR` on every path that dereferences it. The cost is at
startup: zero page isn't part of the program image, so `__zp_init`
stores the global's initial value, about 3 cycles and 2 bytes for each
byte, where the data segment held it for free.

## Example

```c
#include <stdint.h>
uint8_t buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t *p = buf;
uint16_t total;
uint8_t once = 9;

uint16_t sum(void)
{
    for (uint8_t i = 0; i < 8; i++)
        total += p[i];
    return total + once;
}
```

These listings compare two complete compiles, the first with
promotion turned off.

### Before

```
sum:
   SUBROUTINE

   LDY   #$00
   LDA   p
   STA   DPTR
   LDA   p+1
   STA   DPTR+1
.loop@0_start:
   CPY   #$08
   BCS   .loop@0_break
   LDA   (DPTR),Y
   CLC
   ADC   total
   STA   total
   ...

p:
   DC.W  buf

total:
   DS.B  2
```

### After

```
total	EQU	$26
p	EQU	$28

sum:
   SUBROUTINE

   LDY   #$00
.loop@0_start:
   CPY   #$08
   BCS   .loop@0_break
   LDA   (p),Y
   CLC
   ADC   total
   STA   total
   ...

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   total
   STA   total+1
   LDA   #<buf
   STA   p
   LDA   #>buf
   STA   p+1
   RTS
```

The loop loses the four instructions that copied `p` to `DPTR`, and
each access to `total` is a byte and a cycle shorter. `once` is read
once, so it stays where it was, and so does the array `buf`.

## When it applies

- The global has static storage, is defined in the translation unit,
  isn't `const`, and has no `zeropage` or `address` attribute. A
  `static` variable inside a function counts.
- It's a scalar: an integer, a floating-point number, or a pointer.
- Its weight is at least 3 for each of its bytes. The weight counts
  each reference in the unoptimized TAC other than taking its
  address, and a reference inside a loop counts 8 times for each loop
  around it, up to three loops deep (`access_weights`).
- The densest candidates (weight per byte) go first, each in the
  lowest free bytes below `$80`, after the runtime's block and the
  objects the program declared in zero page.

## What it leaves alone

- Arrays, structs, and unions. `LDA arr,Y` has no zero-page form, so
  most accesses to an array wouldn't get shorter, and taking an address
  costs the same anywhere.
- A global used too little to pay for its store in `__zp_init`, such
  as `once` above.
- Everything without `--optimize`.
- Bytes from `$80` up, which are the compiler's pools.

## Linking

The link metadata marks a promoted global with its weight (`weight=`
on its `zp` line). `compile.py --link` places the promoted globals of
every unit after the declared ones, densest first. One that doesn't
fit goes back in the data segment with its initial bytes, unless some
unit uses it as an `(zp),Y` pointer, which only works in zero page;
then linking fails.

## Related

[Address of a zero-page local](lower_data_load_address.md) and
[memory value propagation](memory_value_propagation.md) are among the
passes that use the zero-page addresses.
