# A signed compare of values that can't be negative

`NONNEGATIVE_COMPARE_RULE` in
[cpp/src/passes/optimization/nonnegative_compare.h](../../cpp/src/passes/optimization/nonnegative_compare.h)
makes a signed ordering compare unsigned when neither operand can be
negative. It runs in the TAC fixed-point loop, last in the
`fold_producer_jumps` `RuleSet` with the
[comparison jump fold](cmp_zero_jump_fold.md), and only under `-O`.

C promotes a byte to `int` before it compares it, and a byte plus 1
stays an `int`, so `row + 1 < WNDBTM` with two `uint8_t`s is a signed
compare of 2-byte values. A signed compare subtracts and then corrects
the sign for overflow, `BVC skip; EOR #$80; skip: BMI`. The values
here are from 1 to 256 and from 0 to 255, and two values from 0 to a
type's largest value are ordered the same way whether they're read as
signed or unsigned numbers. An unsigned compare branches on the carry
of the same subtraction, with no correction.

## Example

```c
#include <stdint.h>
extern uint8_t WNDTOP [[c6502::zeropage(0x22)]];
extern uint8_t WNDBTM [[c6502::zeropage(0x23)]];
void copy(uint8_t row);
void f(void) {
    for (uint8_t row = WNDTOP; row + 1 < WNDBTM; row++)
        copy(row);
}
```

### Before

```
JumpIfCmp(>=, %1.1, %2.1, .loop@0_break)
```

### After

```
Copy(%1.1, %1.1.u0)
JumpIfCmp(>=, %1.1.u0, %2.1, .loop@0_break)
```

`%1.1` is `ZeroExtend(row) + 1` and `%2.1` is `ZeroExtend(WNDBTM)`,
both `int`. `%1.1.u0` is an `unsigned int`, which makes the compare
unsigned. The asm optimizer's copy propagation removes the copy.
The test at the loop's top goes from

```
   LDA   __local_f__2
   SEC
   SBC   WNDBTM
   LDA   __local_f__1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
```

to

```
   LDA   __local_f__2
   SEC
   SBC   WNDBTM
   LDA   __local_f__1
   SBC   #$00
   BCC   .f@rot@0
```

which the loop rotation then moves to the loop's bottom. The listings
compare two compiles, before and after the rule was added. Ali Baba's
`hrcg_scroll_up` became 5 bytes smaller.

## When it applies

- The instruction is a `JumpIfCmp` or a comparison kept as a value
  with `<`, `>`, `<=`, or `>=`.
- Each operand is a signed integer of 2 bytes or more, a variable or
  a constant.
- `value_range.max_value` gives each variable a bound, which it gives
  only to a value that can't be negative, and each constant is
  greater than 0.

A constant operand becomes the unsigned constant of its width, which
makes the compare unsigned by itself. When both operands are
variables, the first is copied into a new unsigned temporary of its
width. Copy propagation keeps a copy that changes the signedness, so
the compare stays unsigned until `tac_to_asm` lowers it.

## What it leaves alone

- A compare with the constant 0, which `tac_to_asm` lowers as a test
  of the sign bit.
- A compare of bytes. One of two extended bytes narrows to an
  unsigned byte compare by the
  [comparison jump fold](cmp_zero_jump_fold.md), which comes first.
- A value that can be negative as far as `max_value` can tell. It
  bounds a value by its definition only, so `c > 0x1C` after `if (c <
  0) return;` on an `int8_t c` keeps its signed compare here. The
  [redundant compare](redundant_compare.md) pass drops that compare's
  overflow test instead, from the branch before it.
