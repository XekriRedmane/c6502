# A product of two bytes

`tac_to_asm` lowers a 16-bit `*` whose operands each have one
significant byte as an 8-by-8 shift-and-add loop
(`inline_byte_product` in
[cpp/src/tac_to_asm_arith.cpp](../../cpp/src/tac_to_asm_arith.cpp)),
where it called `mul16`. An operand qualifies when it's a constant from
0 to 255 or a local whose other byte is zero
(cpp/src/passes/significant_bytes.h), such as a `ZeroExtend` of a byte, so
`a * b` on two `uint8_t`s takes the loop in both pipelines. Under
`-O`, [narrowing arithmetic on values from 0 to
255](narrow_byte_range.md) also makes an operand bounded by 255, such
as `tier + 1` for a `tier` from 0 to 3, a `ZeroExtend` of a byte.

The runtime has no helper for a byte product with a 16-bit result.
`mul8` keeps only the low byte, and `mul16` multiplies two 16-bit
values in sixteen steps. The loop is about the size of the `mul16`
call with its four HARGS bytes and two results, and takes about 170
cycles where the call takes about 580, measured in the simulator for
200 times 255 with the call of the function included.

```
   LDA   b
   STA   HARGS     ; the multiplier, then the product's low byte
   LDA   #0        ; the product's high byte
   LDX   #8
   LSR   HARGS
loop:
   BCC   skip
   CLC
   ADC   a
skip:
   ROR   A
   ROR   HARGS
   DEX
   BNE   loop
   STA   dst+1
   LDA   HARGS
   STA   dst
```

Each pass adds `a` to the high byte when the multiplier's low bit is
set, then shifts the high byte and the multiplier right together. The
bits of the product's low byte come out of the high byte into the top
of the multiplier's byte, as the multiplier's own bits leave its
bottom. The multiplier's byte is in HARGS, which is in zero page under
every layout. The loop's `ROR` of it would have to go through A, which
holds the high byte, if it were on the soft stack, where a register
allocator's spill or the unoptimized pipeline puts a local.

## Example

```c
#include <stdint.h>
uint16_t PRICE;
void price(uint8_t p, uint8_t style) { PRICE = p * (((style >> 5) & 3) + 1); }
```

This is the price in Ali Baba's `show_item`.

### Before

```
price:
   STA   __local_price__p
   STX   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   JSR   asr16
   LDA   HARGS+3
   AND   #$03
   CLC
   ADC   #$01
   STA   HARGS+2
   LDA   #$00
   ADC   #$00
   STA   HARGS+3
   LDA   __local_price__p
   JSR   row_of@run0@run2
   JSR   mul16
   LDA   HARGS+4
   STA   PRICE
   LDA   HARGS+5
   STA   PRICE+1
   RTS
```

`row_of@run0@run2` is `STA HARGS; LDA #$00; STA HARGS+1; RTS`, which
`outline_runs` shared between functions. The `asr16` call is the
promoted `style >> 5`, a signed 16-bit shift, which [narrowing
arithmetic on values from 0 to 255](narrow_byte_range.md) makes a
byte shift.

### After

```
price:
   STA   __local_price__p
   TXA
   LSR
   LSR
   LSR
   LSR
   LSR
   AND   #$03
   CLC
   ADC   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@6:
   BCC   .mul_skip@7
   CLC
   ADC   __local_price__p
.mul_skip@7:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@6
   STA   PRICE+1
   LDA   HARGS
   STA   PRICE
   RTS
```

## When it applies

- A `Binary` `*` with a 2-byte result, whose operands each have one
  significant byte: a constant from 0 to 255, or a local that
  `significant_bytes` finds has a zero high byte. A parameter, a
  static, and a local whose address is taken count both bytes.
- When one operand is a constant, it's the one added (`ADC #c`), and
  the other is the multiplier.

## What it leaves alone

- A 1-byte `*`, which keeps `mul8`, since an inline loop that keeps
  only the low byte is longer than its call.
- A product of a byte and a value that may exceed 255, which keeps
  `mul16`.
- A product with a 4- or 8-byte result.
