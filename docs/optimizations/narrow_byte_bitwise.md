# Narrowing a bitwise operation on a byte

`NarrowByteBitwise` in
[passes/optimization/narrow_byte_bitwise.py](../../passes/optimization/narrow_byte_bitwise.py)
rewrites an `&`, `|`, or `^` whose operands were promoted from bytes
as the operation on one byte, zero-extended, when the result's high
bytes are zero. It runs last in the TAC fixed-point loop, after
[narrowing widened arithmetic](narrow_widened_arith.md), and it
doesn't run in the unoptimized pipeline.

C promotes a byte to `int` before `&`, so `(mode & 0x3F) < 3`
compares two `int`s. The [comparison and jump
fold](cmp_zero_jump_fold.md) narrows a comparison to one byte when
each operand is a `ZeroExtend` of a byte or a constant from 0 to 255,
and the result of the `&` is neither. The comparison was a signed
16-bit subtraction with an overflow test. Once the `&` is a byte
operation and a `ZeroExtend`, the comparison narrows on the next
round of the fixed point and becomes `CMP #3; BCS`.

## Example

```c
#include <stdint.h>
uint8_t mode;
void f(void);
void c(void) { if ((mode & 0x3F) < 3) f(); }
```

### Before

```
ZeroExtend(mode, %0.1)
Binary(&, %0.1, 63, %1.1)
JumpIfCmp(>=, %1.1, 3, .if_end@0)
FunctionCall(f)
Label(.if_end@0)
Ret(None)
```

### After

```
ZeroExtend(mode, %0.1)
Binary(&, mode, 63uc, %1.1.n0)
ZeroExtend(%1.1.n0, %1.1)
JumpIfCmp(>=, %1.1, 3, .if_end@0)
FunctionCall(f)
Label(.if_end@0)
Ret(None)
```

The first `ZeroExtend` is left for dead-store elimination. The
assembly had been

```
   LDA   mode
   AND   #$3F
   LDY   #$00
   SEC
   SBC   #$03
   TYA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .if_end@0
   JSR   f
.if_end@0:
   RTS
```

and is now

```
   LDA   mode
   AND   #$3F
   CMP   #$03
   BCS   .if_end@0
   JSR   f
.if_end@0:
   RTS
```

## When it applies

- The `Binary` is `&`, `|`, or `^`, and its result is wider than a
  byte.
- Each operand is a `ZeroExtend` or `SignExtend` of a byte, an
  integer constant, or a variable as wide as the result, and at least
  one is an extend. Without an extend, as in `w & 0x3F` on an `int`,
  the high byte is already written as a constant 0.
- The result's high bytes are zero. Those of `a & b` are zero when
  either operand's are, and those of `a | b` and `a ^ b` when both
  operands' are. A `ZeroExtend` and a constant from 0 to 255 have zero
  high bytes. A `SignExtend`, another constant, and a wide variable
  don't, and they contribute their low byte, which for a wide variable
  is a new `Truncate`.
- An extend's source holds the same value at the operation as at the
  extend, which is true of an SSA-renamed name, and of a static when
  the extend is earlier in the same straight run with nothing between
  that might write it (`cmp_zero_jump_fold.extend_source_unchanged`).
  Neither the source nor a wide operand may be volatile.

The comparison fold narrows a signed comparison the same way, when
each operand is a `SignExtend` of a `signed char` or a constant from
-128 to 127, so `(int8_t)(a - b) > 0` compares one byte.
