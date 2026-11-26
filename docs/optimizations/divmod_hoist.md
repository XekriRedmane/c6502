# A divide and a remainder computed together

`HoistDivmodPairs` in
[cpp/src/passes/optimization/divmod_hoist.h](../../cpp/src/passes/optimization/divmod_hoist.h)
puts a `/` and a `%` of the same operands next to each other, so that
`tac_to_asm` gives both from one loop of subtractions or one call of
a `divmod` helper ([a byte divided by a constant](byte_divide.md)).
It runs once after the TAC fixed point, in SSA form. It doesn't run
in the unoptimized pipeline.

`tac_to_asm` pairs the two only when they're close together in one
straight run with no store or call between. Ali Baba's
`load_scene_from_disk` stores `sector % 13` through a pointer before
it computes `sector / 13`, and `render_font_char_direct` divides
inside an `if` and takes the remainder after it. Each made two loops.

## Example

This listing compares two complete compiles.

```c
#include <stdint.h>
extern uint8_t V [[c6502::address(0x5A00)]], Q [[c6502::address(0x5A01)]],
               R [[c6502::address(0x5A02)]];
void f(void) { if (V >= 24) Q = V / 24; R = V % 24; }
```

### Before

```
   LDA   V
   CMP   #$18
   BCC   .vthread@0
   LDX   #$FF
.div_loop@0:
   INX
   SBC   #$18
   BCS   .div_loop@0
   STX   Q
   LDA   V
.vthread@0:
   LDX   #$FF
   SEC
.div_loop@1:
   INX
   SBC   #$18
   BCS   .div_loop@1
   ADC   #$18
   STA   R
   RTS
```

### After

```
   LDA   V
   LDX   #$FF
   SEC
.div_loop@0:
   INX
   SBC   #$18
   BCS   .div_loop@0
   ADC   #$18
   STA   __local_f__1
   LDA   V
   CMP   #$18
   BCC   .if_end@0
   STX   Q
.if_end@0:
   LDA   __local_f__1
   STA   R
   RTS
```

The pass made the TAC

```
Binary(/, V, 24uc, Q.dm0)
Binary(%, V, 24uc, R.dm0)
JumpIfCmp(<, V, 24uc, .if_end@0)
Copy(Q.dm0, Q)
Label(.if_end@0)
Copy(R.dm0, R)
```

The function went from 34 bytes to 31, and Ali Baba's
`load_scene_from_disk` from 83 to 68 with the two peepholes in
[carry and register fixes after the division](#after-the-loop).

## Where the pair goes

Let `D` be the nearest common dominator of the two instructions'
blocks. When `D` holds one of them, the pair goes at the first of
them in `D`. Otherwise it goes at the end of `D`, in front of its
jump, and every path from there to the function's exit must pass
through one of the two blocks, so the pair never runs on a path that
computed neither. A result whose destination SSA construction renamed
is computed straight into it. Any other destination gets a new
temporary, and a `Copy` of it stays where the instruction was, so the
destination is written at the same point as before.

## When it applies

- The two are a `Binary` divide and remainder with the same `src1`, a
  variable of 1, 2, 4, or 8 bytes that isn't floating, and the same
  `src2`. A constant `src2` must not be 0, and a variable one is
  taken only when both are in one block, so the pair never divides by
  zero on a path that didn't.
- Neither destination is an operand, and no operand is volatile, since
  the pair reads each once.
- From the pair's new place to each instruction, nothing writes an
  operand. When an operand isn't an SSA-renamed name (a static, an
  object at a fixed address, an address-taken local), nothing on the
  way may store through a pointer, store at a fixed address
  (`IndexedStore`), store into the operand as an object, or call a
  function either.
- The two aren't already in one block with at most 7 instructions and
  no store or call between, which `tac_to_asm` pairs as they are.

## What it leaves alone

- A global dividend with a call or a store through a pointer on the
  way, which could change it.
- A pair whose common dominator has a path to the exit that computes
  neither, such as `if (a) q = x / 7; else if (b) r = x % 7;`.

<a id="after-the-loop"></a>
## After the loop

Two peepholes clean up the code around the shared loop.
[Constant AND and ORA fold](const_arith_fold.md) makes the loop's
`ADC #c` and a following `CLC; ADC #k` one `ADC #(c + k)`, so
`a % 21 + 1` is `ADC #$16`. [Commuted ADC, AND, and
ORA](adc_commute.md) reads the quotient from X, so `0x11 + s / 13` is
`TXA; CLC; ADC #$11` where it was `STX t; LDA #$11; CLC; ADC t`.
