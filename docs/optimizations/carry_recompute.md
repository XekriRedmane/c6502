# Carry set again by a shift

`apply_carry_recompute_drop` in
[passes/carry_recompute.py](../../passes/carry_recompute.py) deletes a
load and shift whose only effect is to set the carry to the value it
already holds. It runs in the peephole fixed point, which both
pipelines run.

`tac_to_asm` lowers `x >> 7` on a 16-bit `x` as the high byte of `x <<
1`, and `x >> 15` as the bit shifted out of the top. Three statements
that take three bytes of one shifted value each start by putting the
bit shifted out of the byte below into C again, where Zork's code
runs one `ASL` / `ROL` / `ROL` chain. The pass follows what C holds,
bit 7 of a byte `M` after `LDA M` and `ASL A` or `ROL A`, or bit 0
after `LSR A` or `ROR A`, and deletes a later `LDA M` and shift that
would set C the same way.

## Example

From `load_packed_address` in `examples/zork_print_string_literal.c`:

```c
Z_PC2_L = (uint8_t)(SCRATCH2 << 1);
Z_PC2_H = (uint8_t)(SCRATCH2 >> 7);
Z_PC2_HH = (uint8_t)(SCRATCH2 >> 15);
```

### Before

```
   LDA   SCRATCH2
   ASL
   STA   Z_PC2_L
   LDA   SCRATCH2
   ASL
   LDA   SCRATCH2+1
   ROL
   STA   Z_PC2_H
   LDA   SCRATCH2+1
   ASL
   LDA   #$00
   ROL
   STA   Z_PC2_HH
```

### After

```
   LDA   SCRATCH2
   ASL
   STA   Z_PC2_L
   LDA   SCRATCH2+1
   ROL
   STA   Z_PC2_H
   LDA   #$00
   ROL
   STA   Z_PC2_HH
```

That's Zork's own code.

## When it applies

- A non-volatile `LDA M` of a zero-page or absolute byte is followed
  by `ASL A`, `ROL A`, `LSR A`, or `ROR A`.
- C already holds the bit that shift puts there: bit 7 of `M` for
  `ASL` and `ROL`, bit 0 for `LSR` and `ROR`, from an earlier load of
  `M` and shift in the same basic block.
- Between the two, nothing writes C (`ADC`, `SBC`, a compare, a shift
  of memory, `SEC`, `CLC`, a call) or might write `M`. A store to
  another byte of the same kind (`Data` or `ZP`) doesn't count; a
  store through a pointer does.
- A and N/Z are dead after the shift, since the deleted pair would
  have set them.

## What it leaves alone

- Anything across a label, which might be reached with C some other
  way.
- A load of a volatile byte, whose read is part of what the program
  does.

## Related

[Carry already known](carry_known.md) does the same for a `SEC` or
`CLC` whose value C already has.
