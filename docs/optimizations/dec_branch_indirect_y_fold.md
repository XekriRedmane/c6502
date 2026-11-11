# Y known zero after a DEY loop

`apply_dec_branch_indirect_y_fold` in
[passes/dec_branch_indirect_y_fold.py](../../passes/dec_branch_indirect_y_fold.py)
drops the `LDY #$00` in front of a `(DPTR),Y` access that directly
follows the fall-through of a `DEY; BNE` loop. It runs in the peephole
fixed point, in both pipelines, but only fires with `--optimize`,
because only [DEX and DEY for a register
counter](dec_register_peephole.md) and the later loop promotions
produce `DEY`.

The IR has no separate instruction for the `LDY #$00`. An
`Indirect(0)` operand means "the byte at `(DPTR),Y` with Y = 0", and
`asm_emit` writes the `LDY #$00` as part of it. The `IndirectY`
operand means the same byte with Y already loaded, and emits no
`LDY`. The fall-through of `DEY; BNE` runs only when Y has reached 0,
so the pass swaps one operand for the other and saves 2 bytes and 2
cycles. The `LDY #$00` would have set Z and cleared N, which is what
`DEY` reaching 0 already left in the flags.

## Example

```c
#include <stdint.h>
extern const volatile uint8_t *speaker;
void buzz(uint8_t delay, uint8_t n)
{
    do {
        register volatile uint8_t y = delay;
        while (--y != 0) { }
        (void)*speaker;
    } while (--n != 0);
}
```

### Before

```
   LDA   __zpabi_buzz__n
   STA   __local_buzz__n
   LDA   speaker
   STA   DPTR
   LDA   speaker+1
   STA   DPTR+1
.loop@0_start:
   LDY   __zpabi_buzz__delay
.loop@1_continue:
   DEY
   BNE   .loop@1_continue
   LDY   #$00
   CMP   (DPTR),Y
   DEC   __local_buzz__n
   BNE   .loop@0_start
   RTS
```

### After

```
   LDA   __zpabi_buzz__n
   STA   __local_buzz__n
   LDA   speaker
   STA   DPTR
   LDA   speaker+1
   STA   DPTR+1
.loop@0_start:
   LDY   __zpabi_buzz__delay
.loop@1_continue:
   DEY
   BNE   .loop@1_continue
   CMP   (DPTR),Y
   DEC   __local_buzz__n
   BNE   .loop@0_start
   RTS
```

The `CMP` comes from [CMP for a discarded volatile
read](volatile_void_read_cmp.md). The pass fired on the second call,
after the label `.loop@1_break` between the `BNE` and the `LDY` had
been deleted.

## When it applies

- A `DEY`, then a `BNE`, then a `Mov` or `Compare` with an
  `Indirect(0)` operand, with nothing in between.
- Every `Indirect(0)` operand of that instruction becomes
  `IndirectY`.

## What it leaves alone

- An access with a label between it and the `BNE`. Another branch
  could reach the label with any value in Y.
- An access at a nonzero offset, such as `(DPTR),Y` with Y = 1 for
  the high byte of a pointer target.
- Other instructions with an `Indirect(0)` operand, such as `ADC`,
  `AND`, or `EOR`. Only loads, stores, and compares are matched.
- A `(zp),Y` access through a zero-page pointer (`IndirectZp`), and a
  `DEX; BNE` loop.
- The fall-through of `DEY; BEQ`, where Y is nonzero.

## Related

[DEX and DEY for a register counter](dec_register_peephole.md)
produces the `DEY; BNE` this pass matches.
