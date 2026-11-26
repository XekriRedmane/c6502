# Narrowing arithmetic on values from 0 to 255

`NarrowByteRange` in
[cpp/src/passes/optimization/narrow_byte_range.h](../../cpp/src/passes/optimization/narrow_byte_range.h)
does a promoted byte's `/`, `%`, and `>>` on one byte, zero-extended,
and makes a 16-bit `*` of two such values read each as a
zero-extended byte. It runs in the TAC fixed-point loop, after
[narrowing widened arithmetic](narrow_widened_arith.md) and before
[narrowing a bitwise operation on a byte](narrow_byte_bitwise.md), and
it doesn't run in the unoptimized pipeline.

C promotes a byte to `int` before `/`, so `pos / 20` on a `uint8_t
pos` divides two `int`s, and `tac_to_asm` called the signed 16-bit
helper `sdivmod16`. Narrowing widened arithmetic leaves a divide
alone, because the high bytes of a wide dividend reach the low byte
of the quotient. Here they're zero. A dividend from 0 to 255 and a
divisor from 1 to 255 give the quotient and remainder of an unsigned
byte divide, whether the `int` divide is signed or not.

The bound comes from `max_value` in
[cpp/src/passes/optimization/value_range.h](../../cpp/src/passes/optimization/value_range.h),
which walks back through the definitions of SSA names. A
`ZeroExtend` of a byte is at most 255, an `&` is at most its smaller
bounded operand, a sum or product is bounded when it doesn't wrap, a
right shift divides the bound, and an unsigned type bounds any value
of it. A bound also means the value isn't negative, which
[strength reduction](strength_reduction.md) uses to make a signed
divide by a power of two a shift.

## Example

```c
#include <stdint.h>
uint8_t row_of(uint8_t pos) { return pos / 20; }
```

### Before

```
ZeroExtend(@0.pos, %0.1)
Binary(/, %0.1, 20, %1.1)
Truncate(%1.1, %2.1)
Ret(%2.1)
```

### After

```
Binary(/, @0.pos, 20uc, %1.1.nb0)
Ret(%1.1.nb0)
```

The pass writes `Binary(/, @0.pos, 20uc, %1.1.nb0)` and
`ZeroExtend(%1.1.nb0, %1.1)`, and [the fold of a truncate of an
extend](truncate_extend_fold.md) and dead-store elimination leave the
byte divide. The assembly had been

```
row_of:
   JSR   row_of@run0
   LDA   HARGS+4
   RTS
row_of@run0:
   JSR   row_of@run0@run2
   LDY   #$14
   STY   HARGS+2
   STA   HARGS+3
   JMP   sdivmod16
row_of@run0@run2:
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   RTS
```

with the two outlined runs shared with other functions of the unit,
and is now [a byte divided by a constant](byte_divide.md).

```
row_of:
   LDX   #$FF
   SEC
.div_loop@0:
   INX
   SBC   #$14
   BCS   .div_loop@0
   TXA
   RTS
```

A right shift narrows the same way. `PRICE = p * (((style >> 5) & 3)
+ 1);` with two `uint8_t` operands shifted a promoted `int` right by
5, which is a signed 16-bit shift and called `asr16`, and it
multiplied two `int`s with `mul16`. The shift becomes `style >> 5`
on the byte, the `&` narrows, and the multiply reads `(… & 3) + 1`,
whose bound is 4, as a `ZeroExtend` of a `Truncate` of it.

```
ZeroExtend(@9.p, %26.1)
Binary(>>, @10.style, 5, %28.1.nb0)
Binary(&, %28.1.nb0, 3uc, %29.1.n0)
Binary(+, %29.1.n0, 1uc, %32.1.lo0)
ZeroExtend(%32.1.lo0, %32.1.nw0)
Binary(*, %26.1, %32.1.nw0, PRICE)
```

Narrowing widened arithmetic made the `+` a byte add, and `tac_to_asm`
lowers the multiply as [a product of two bytes](byte_product.md).

## When it applies

- The `Binary` is `/`, `%`, `>>`, `<<`, or `*`, and its result is an
  integer wider than a byte. A `*` qualifies only for a 2-byte result.
- The first operand is a non-volatile variable as wide as the result,
  or a constant, and `max_value` bounds it by 255.
- For `/` and `%`, the divisor is a constant from 1 to 255, or a
  variable that `max_value` bounds by 255. A zero divisor is
  undefined at either width.
- For `*`, the second operand is bounded by 255 too, and at least one
  operand isn't already a byte constant or a `ZeroExtend` of a byte.
- For `<<`, the count is a constant `k` from 0 to 7, and the bound of
  the value shifted left by `k` is at most 255. The bits the byte
  shift drops are then zeros, and the high bytes of the result are 0.
  Strength reduction makes `0x28 * (row / 8)` on a byte `row` into
  `((row / 8) << 3) + ((row / 8) << 5)`, and the first shift, of a
  value at most 31, had computed its high byte with five `LSR`s. Ali
  Baba's `hrcg_row_address` went from 90 bytes to 73.
- For `>>`, a constant count of 8 or more gives 0 at either width, and
  the shift becomes a copy of 0. A variable count is read as the byte
  a `ZeroExtend` widened, since the lowering reads only the count's
  low byte. A count of 8 to 15 gives 0 at both widths, because the
  byte shift's loop shifts every bit out.

Each operand becomes a byte. A constant becomes a `ConstUChar`, and a
`ZeroExtend` of an unsigned byte becomes the byte, when the byte holds
the same value at the operation
(`cmp_zero_jump_fold.extend_source_unchanged`). Any other operand gets
a new `Truncate` just before the operation.

## What it leaves alone

- A `SignExtend` of a `signed char`, or any value that may be negative,
  since `max_value` returns no bound for it.
- A dividend that may exceed 255, as `(space + 1) / 2` on a `uint8_t
  space` may. Strength reduction makes that one a shift, because it's
  nonnegative and the divisor is a power of two.
- A `/` or `%` of a byte by a 16-bit divisor that may exceed 255.
- A multiply with a result of 4 or 8 bytes.

## Related

[Narrowing widened arithmetic](narrow_widened_arith.md) narrows the
operations whose low bytes depend only on the operands' low bytes,
when only the low byte of the result is read. This pass narrows
operations whose result depends on every byte of the operands, when
the operands' high bytes are zero.
