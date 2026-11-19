# A byte divided by a constant

`tac_to_asm` lowers an unsigned 1-byte `/` or `%` by a constant from
1 to 255 as a loop of subtractions (`_inline_byte_divide` in
[tac_to_asm.py](../../tac_to_asm.py)), where it called `udivmod8`. A
divide and a remainder of the same operands share one loop, and two of
any other width share one call of the `divmod` helper
(`Translator._plan_divmod_pairs`). Both apply in the unoptimized
pipeline too, where a 1-byte divide comes only from an `unsigned
_BitInt(8)`, since C promotes a `uint8_t` to `int`. Under `-O`,
[narrowing arithmetic on values from 0 to 255](narrow_byte_range.md)
makes a promoted byte's divide a 1-byte one.

The loop counts in X how many times the divisor can be subtracted.

```
   LDA   x
   LDX   #$FF
   SEC
loop:
   INX
   SBC   #c
   BCS   loop
   ADC   #c        ; the remainder
```

The last `SBC` borrows, so A holds the remainder less `c` and C is
clear, and `ADC #c` puts the remainder back. The quotient is in X.
That's 11 bytes before the stores, against 15 for the call with its
HARGS setup and results. The loop takes about 7 cycles for each unit
of the quotient, so `x / 20` takes at most about 100 cycles, and `x /
3` of 255 about 600, where `udivmod8`'s eight steps take about 250.

## Example

```c
#include <stdint.h>
uint16_t pos_to_colrow(uint8_t pos) {
    uint8_t col = pos % 20;
    uint8_t row = pos / 20;
    return col | (uint16_t)row << 8;
}
```

After narrowing, the TAC has the two byte operations next to each
other.

```
Binary(%, @1.pos, 20uc, %4.1.nb0)
Binary(/, @1.pos, 20uc, %7.1.nb0)
ZeroExtend(%4.1.nb0, %10.1)
ZeroExtend(%7.1.nb0, %11.1)
Binary(<<, %11.1, 8, %12.1)
Binary(|, %10.1, %12.1, %13.1)
Ret(%13.1)
```

### Before

`pos_to_colrow` called `sdivmod16` twice, through runs that
`outline_runs` shared with another function of the unit.

```
pos_to_colrow:
   STA   __local_pos_to_colrow__pos
   JSR   row_of@run0
   LDA   HARGS+6
   STA   __local_pos_to_colrow__0
   LDA   __local_pos_to_colrow__pos
   JSR   row_of@run0
   LDX   HARGS+4
   LDA   __local_pos_to_colrow__0
   RTS
```

### After

```
pos_to_colrow:
   LDX   #$FF
   SEC
.div_loop@1:
   INX
   SBC   #$14
   BCS   .div_loop@1
   ADC   #$14
   RTS
```

`pos` arrives in A, the remainder is the low byte of the result, which
returns in A, and the quotient is the high byte, which returns in X,
so the register allocator gives each value the register the loop
leaves it in.

## Sharing one helper call

A divide and a remainder that call a helper share one call the same
way. Ali Baba's `num_to_decimal` is `digit = value / place; value %=
place;` on 16-bit values, which called `udivmod16` twice. For

```c
#include <stdint.h>
uint16_t q;
uint16_t digit(uint16_t v, uint16_t place) { q = v / place; return v % place; }
```

the code had been

```
digit:
   STA   __local_digit__v_0
   STX   __local_digit__v_1
   STA   HARGS
   STX   HARGS+1
   JSR   digit@run1
   LDA   HARGS+4
   STA   q
   LDA   HARGS+5
   STA   q+1
   LDA   __local_digit__v_0
   STA   HARGS
   LDA   __local_digit__v_1
   STA   HARGS+1
   JSR   digit@run1
   LDA   HARGS+6
   LDX   HARGS+7
   RTS
digit@run1:
   LDA   __zpabi_digit__place_0
   STA   HARGS+2
   LDA   __zpabi_digit__place_1
   STA   HARGS+3
   JMP   udivmod16
```

and is now

```
digit:
   STA   HARGS
   STX   HARGS+1
   LDA   __zpabi_digit__place_0
   STA   HARGS+2
   LDA   __zpabi_digit__place_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   STA   q
   LDA   HARGS+5
   STA   q+1
   LDA   HARGS+6
   LDX   HARGS+7
   RTS
```

## When it applies

The loop is used for a `Binary` `/` or `%` whose first operand is an
unsigned 1-byte value and whose second is a constant from 1 to 255.
Strength reduction has already made a power of two a shift or a mask
under `-O`, and 1 a copy.

Two instructions are paired when they're a `/` and a `%` of the same
two operands, and the second is at most 8 instructions after the
first. The instructions between them meet these conditions.

- no label, jump, return, call, or store, so the two are in one
  straight run and no memory changes,
- no write of either operand,
- no read or write of the second's destination, which is written at
  the first.

The first's destination isn't an operand, which the second reads, and
no operand is volatile, since the pair reads each once. The
instructions lowered another way (a member computed in place, a
rotate-in, or a value saved around a recursive call) aren't paired.

## What it leaves alone

- A 1-byte divide by a variable, which keeps its `udivmod8` call,
  since a divisor of 0 would make the loop run forever.
- A signed 1-byte divide, which keeps `sdivmod8`.
- A divide of a 3-, 5-, 6-, or 7-byte value, whose helper call is
  made by `_odd_width_helper_op`, and a floating-point divide.
