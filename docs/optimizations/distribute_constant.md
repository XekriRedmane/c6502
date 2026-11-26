# A widened byte multiplied before the constant

`DistributeConstant` in
[cpp/src/passes/optimization/distribute_constant.h](../../cpp/src/passes/optimization/distribute_constant.h)
rewrites `(x + c) * k` and `(x - c) * k` as `x * k + c * k` and `x * k
- c * k`, where `x` is a byte zero-extended to the multiply's width
and `c` and `k` are constants. It runs in the TAC fixed point right
after [constant folding](constant_folding.md) and before [strength
reduction](strength_reduction.md), which would otherwise make the
multiply shifts of `x ± c` first. It doesn't run in the unoptimized
pipeline.

`x ± c` can need every byte of its width, as `n - 1` does for `n` of 0,
so its product is shifts and adds of two bytes. The product of `x`
itself has one significant byte, and `tac_to_asm` shifts it as two
bytes in A, or multiplies it by a byte constant as an 8-by-8 loop
where the other was a call of `mul16`. The add or subtract of `c * k`
is one instruction, as the add or subtract of `c` was. A product that
comes out negative is subtracted, so that its top byte is an `SBC
#0`, which [carry INC](carry_inc.md) makes `BCS skip; DEC`.

## Example

```c
#include <stdint.h>
struct menu_option { uint8_t enabled, position, next[4]; uint8_t *text; uint8_t exit; };
struct menu { uint8_t count, selection, init; struct menu_option *options; };
extern struct menu *MENU_DESC_PTR [[c6502::zeropage(0xC0)]];
struct menu_option *option(uint8_t n) { return &MENU_DESC_PTR->options[n - 1]; }
```

The 9-byte options make the index `(n - 1) * 9`. The pass makes it `n
* 9 - 9`, and strength reduction makes `n * 9` `(n << 3) + n`.

### Before

```
option:
   STA   __local_option__n
   LDY   #$03
   LDA   (MENU_DESC_PTR),Y
   STA   __local_option__4
   INY
   LDA   (MENU_DESC_PTR),Y
   STA   __local_option__3
   LDA   __local_option__n
   SEC
   SBC   #$01
   STA   __local_option__n
   LDA   #$00
   SBC   #$00
   STA   __local_option__2
   LDA   __local_option__n
   ASL
   STA   __local_option__0
   LDA   __local_option__2
   ROL
   ASL   __local_option__0
   ROL
   ASL   __local_option__0
   ROL
   STA   __local_option__1
   LDA   __local_option__0
   CLC
   ADC   __local_option__n
   STA   __local_option__n
   LDA   __local_option__1
   ADC   __local_option__2
   STA   __local_option__0
   LDA   __local_option__4
   CLC
   ADC   __local_option__n
   STA   __local_option__n
   LDA   __local_option__3
   ADC   __local_option__0
   TAX
   LDA   __local_option__n
   RTS
```

### After

```
option:
   STA   __local_option__n
   LDY   #$03
   LDA   (MENU_DESC_PTR),Y
   STA   __local_option__3
   INY
   LDA   (MENU_DESC_PTR),Y
   STA   __local_option__2
   LDA   __local_option__n
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_option__1
   LDA   __local_option__n
   ASL
   ASL
   ASL
   CLC
   ADC   __local_option__n
   BCC   .carry_inc@0
   INC   __local_option__1
.carry_inc@0:
   SEC
   SBC   #$09
   STA   __local_option__0
   BCS   .carry_inc@1
   DEC   __local_option__1
.carry_inc@1:
   LDA   __local_option__3
   CLC
   ADC   __local_option__0
   STA   __local_option__0
   LDA   __local_option__2
   ADC   __local_option__1
   TAX
   LDA   __local_option__0
   RTS
```

The five `LSR`s give the high byte of `n << 3`, and the three `ASL`s
its low byte. The function is 58 bytes, where it was 70. It still
computes a 16-bit offset, since `n - 1` is -1 for `n` of 0 and up to
254, and `(n - 1) * 9` takes 12 bits.

## When it applies

- The multiply's operands are a constant `k` and an SSA temporary that
  nothing else reads, defined by an add of a constant `c` and `x` (in
  either order) or a subtract of `c` from `x`.
- `x` is an SSA name defined by a `ZeroExtend` of a one-byte value.
- `c` and `k` are constants of the same type. The new constant is
  `c * k` at that type's width, which wraps as the arithmetic does.

## What it leaves alone

- `(x - c) * k` with `x` wider than a byte, or not a `ZeroExtend`,
  where the product of `x` costs what the product of `x - c` does.
- `c - x`, whose product would need a negation.
- A sum or difference that something else reads too.

## Related

[Strength reduction](strength_reduction.md) turns the multiply of the
byte into shifts and adds, and [a product of two
bytes](byte_product.md) multiplies it by a constant that shifts can't
make.
