# Shifts by a loop

`tac_to_asm` lowers a 1- or 2-byte shift by a variable count as a loop
of one-bit shifts with the count in X (`shift_loop` in
[cpp/src/tac_to_asm_arith.cpp](../../cpp/src/tac_to_asm_arith.cpp)),
where it called `asl8`, `asl16`, `lsr8`, `lsr16`, `asr8`, or `asr16`. A shift of those widths
by a constant count that isn't lowered as a fixed run of shifts
(`inline_shift_ok` in cpp/src/passes/shift_cost.h) is the same loop with the count
loaded as a constant. It applies in both pipelines. Shifts of 4 and 8
bytes keep their helpers.

The value is copied into the destination, and the loop shifts the
destination in place.

```
   LDX   n
   CPX   #0        ; a variable count only
   BEQ   done
loop:
   ASL   dst       ; << : ASL dst; ROL dst+1
   DEX             ; >> unsigned: LSR dst+1; ROR dst
   BNE   loop      ; >> signed: LDA dst+1; ASL A; ROR dst+1; ROR dst
done:
```

The `CPX #0` goes when the instruction before it sets the flags from
the count, as an `LDX` does (`redundant_compare`). The count is in X
because a byte on the soft stack is reached through Y. A count of the
width in bits or more shifts every bit out, so a left or logical
shift gives 0 and an arithmetic one gives copies of the sign bit, the
same results as the helpers, which loop the same way. C leaves such a
count undefined, except for a promoted byte, where `(uint8_t)(x << 9)`
is 0.

## Example

```c
#include <stdint.h>
uint8_t shr(uint8_t sample, uint8_t shift) { return sample >> shift; }
unsigned int shl16(unsigned int x, uint8_t n) { return x << n; }
```

[Narrowing arithmetic on values from 0 to 255](narrow_byte_range.md)
makes `sample >> shift` a byte shift, `Binary(>>, @5.sample, @6.shift,
%22.1.nb0)`, which had been a 16-bit signed shift of the promoted
byte.

### Before

```
shr:
   JSR   row_of@run0@run2
   STX   HARGS+2
   JSR   asr16
   LDA   HARGS+3
   RTS
shl16:
   STA   HARGS
   STX   HARGS+1
   LDA   __zpabi_shl16__n
   STA   HARGS+2
   JSR   asl16
   LDA   HARGS+3
   LDX   HARGS+4
   RTS
```

`row_of@run0@run2` is a run that `outline_runs` shared between
functions, `STA HARGS; LDA #$00; STA HARGS+1; RTS`.

### After

```
shr:
   STA   __local_shr__sample
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   LSR   __local_shr__sample
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   __local_shr__sample
   RTS
shl16:
   STA   __local_shl16__x_0
   STX   __local_shl16__x_1
   LDX   __zpabi_shl16__n
   BEQ   .shift_done@5
.shift_loop@4:
   ASL   __local_shl16__x_0
   ROL   __local_shl16__x_1
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDX   __local_shl16__x_1
   LDA   __local_shl16__x_0
   RTS
```

In `shr` the count arrives in X, so nothing before the `CPX` sets the
flags from it. A pass of the 2-byte loop takes 15 cycles on zero-page
bytes, and `asl16` loops the same way after its call and the staging
of its operands and result in HARGS.

## When it applies

- A `<<` or `>>` of a 1- or 2-byte value by a variable, or by a
  constant from 1 to 255 that `inline_shift_ok` rejects. Before that,
  `tac_to_asm` takes the inline forms it already had: a left shift of
  a value with one significant byte, the low byte of a right shift
  that only a `Truncate` to a byte reads, and the constant counts
  `inline_shift_ok` accepts.
- A constant count of 0 is a copy.
- A `>>` is arithmetic when the value is signed, unless its top byte
  is known to be zero (cpp/src/passes/significant_bytes.h), when it's
  logical.
- When the count is the destination, it's loaded into X before the
  copy into the destination overwrites it.

## What it leaves alone

- A shift of a 4- or 8-byte value, which keeps its helper, or a 3-,
  5-, 6-, or 7-byte one, which goes to the helper of the next width.
