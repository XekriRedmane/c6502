# A halved sum's ninth bit from the carry

`apply_carry_halve` in
[cpp/src/passes/carry_halve.h](../../cpp/src/passes/carry_halve.h)
drops the high byte of `(x + y) / 2` when `x` and `y` are bytes (or
one is a constant below 256) and the quotient is used as a byte. It
runs in the peephole fixed point, which both pipelines run.

C promotes both operands to `int`, so the sum has a high byte, which
is only the carry out of the low bytes' add. The quotient's low byte
is that carry and the low byte's bits 7-1, which is what `ROR` makes
of A with the carry in C. The code `tac_to_asm` leaves computes the
high byte with `LDA #$00; ADC #$00`, shifts it right with `LSR` to
put the carry back in C, and reloads the low byte. The pass drops
those four instructions and the store before them, when the
temporary is dead after its load and V is dead after the `ROR`. The
`LDA #$00` may also be a `TXA` or `TYA` of a register the block set
to 0 with `LDX #$00` or `LDY #$00`.

## Example

From `print_centered_plus_col` in `examples/ali_baba/text.c`:

```c
uint8_t space = 40 - s[0];
TEXT_COL += (space + 1) / 2;
```

### Before

```
   SEC
   SBC   (__local_print_centered_plus_col__s),Y
   CLC
   ADC   #$01
   STA   __local_print_centered_plus_col__1
   TYA
   ADC   #$00
   LSR
   LDA   __local_print_centered_plus_col__1
   ROR
   CLC
   ADC   TEXT_COL
   STA   TEXT_COL
```

### After

```
   SEC
   SBC   (__local_print_centered_plus_col__s),Y
   CLC
   ADC   #$01
   ROR
   CLC
   ADC   TEXT_COL
   STA   TEXT_COL
```

The original's `LSR; ADC TEXT_COL` takes in the rounding bit as the
carry instead, which is one byte shorter again. The pass saves 8
bytes here, and nothing in the other examples has the shape.
