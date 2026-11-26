# Shifting a bit in through the carry

`Translator::rotate_in` in
[cpp/src/tac_to_asm_arith.cpp](../../cpp/src/tac_to_asm_arith.cpp)
lowers `(x << 1) | b`, where `b` is a byte that's 0 or 1, as `LDA b;
LSR` followed by a `ROL` of each byte of `x`. It's part of the
translation to asm under `-O`. `Translator::plan_rotations`
finds these ORs once per function.

The `LSR` puts `b` in C, and the rotates shift it into bit 0 and the
other bits up, so the result is the same. C comes out as the bit
shifted out of the top, which is what an `ASL` / `ROL` chain leaves.
When the result goes back where `x` came from, the asm peepholes make
each rotate one instruction in memory ([shift in A](shift_in_a.md)),
`ROL x`. In zero page that's 7 bytes for a 2-byte `x`, against 10 for
the shift and the `ORA` into the low byte.

A shift-and-subtract divide shifts a result bit into its quotient and
the quotient's top bit into its remainder at each step. The second
rotate-in then takes no `LDA b; LSR` at all. The bit it shifts in is
`(uint8_t)(q >> 15)` of the value the first rotate-in shifted, and
that bit is what the first one's last `ROL` left in C. The pass drops
the load and the `LSR` when nothing between the two rotate-ins can
change C, and the asm DCE then drops the computation of the bit.
That's how Zork's `divu16` (examples/zork/numbers.c)
comes out with the original's `ROL` chain.

## Example

```c
uint8_t out = (uint8_t)(quotient >> 15);
quotient = (quotient << 1) | fits;
partial = (partial << 1) | out;
```

### Before

```
.if_end@1:
   LDA   __local_divu16__dividend_1
   ASL
   LDA   #$00
   ROL
   STA   __local_divu16__0
   ASL   __local_divu16__dividend_0
   ROL   __local_divu16__dividend_1
   LDA   __local_divu16__dividend_0
   ORA   __local_divu16__fits
   STA   __local_divu16__dividend_0
   ASL   __local_divu16__partial_0
   ROL   __local_divu16__partial_1
   LDA   __local_divu16__partial_0
   ORA   __local_divu16__0
   STA   __local_divu16__partial_0
```

### After

```
   LDA   #$01                 ; LDA #$00 on the path where it didn't fit
.if_end@1:
   LSR
   ROL   __local_divu16__quotient
   TYA
   ROL
   TAY
   ROL   __local_divu16__partial_0
   ROL   __local_divu16__partial_1
```

The `fits` byte now lives in A, set on each side of the `if`, since
nothing else reads it. The quotient's high byte is in Y, and the
loop's counter is in X.

## When it applies

- The OR's operands are a temporary that is `x << 1` and the zero
  extension of a byte `b`, each defined once and read only by the OR,
  so neither is lowered on its own.
- `x` has the size of the result.
- Every definition of `b` leaves it 0 or 1: the constant 0 or 1, a
  comparison, a logical not, `& 1`, the top bit of an unsigned value
  (`(uint8_t)(y >> 15)`), or a copy or extend of such a byte.
- For the carried case: `b` is the top bit of the `x` of the
  rotate-in just before this one, read before it with nothing in
  between writing that `x`, and the only instructions between the
  two rotate-ins are the shift and the extend this one reads.

## What it leaves alone

- `(x << k) | b` for a shift of more than one bit.
- An OR with a byte that might be more than 1, such as a byte of
  data.
- The unoptimized pipeline, whose TAC isn't in this shape.

## Related

[Compare and subtract of the same operands](compare_subtract.md)
makes the subtract in the same loop set the bit's value, and
[count-down counter in X or Y](counter_to_index_reg.md) puts the
loop's counter in X.
