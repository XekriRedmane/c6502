# CMP and SBC fusion

`apply_cmp_sbc_fusion` in
[passes/cmp_sbc_fusion.py](../../passes/cmp_sbc_fusion.py) rewrites
`LDA M; CMP N; Bcc L1; SEC; SBC N` as `LDA M; SEC; SBC N; Bcc L1`, so
the subtraction sets the flags the branch reads and the `CMP` goes
away. It runs in the peephole fixed point, which both pipelines run.

`CMP N` sets C, Z, and N from `A - N`, and `SEC; SBC N` sets the same
three flags from the same subtraction. It also sets V, which `CMP`
leaves alone, and it writes the difference to A. So when the code
after the branch starts with `SEC; SBC N`, the subtraction can move
ahead of the branch and stand in for the compare, for any branch
that reads only C, Z, or N. That saves the `CMP`, 2 or 3 bytes. The
fall-through path is 3 or 4 cycles faster, and the taken path 2
cycles slower, because it now runs `SEC; SBC N` where it ran `CMP N`.

This is the 1-byte form of [compare and subtract of the same
operands](compare_subtract.md). The wider form is done on the TAC,
where the compare is already a chain of `SBC`s that overwrites A. A
1-byte compare is a `CMP`, which leaves A alone, so whether the
subtraction can take its place depends on whether A is read on the
taken path, which only the asm shows.

## Example

From `examples/refresh_hit_entities.c`:

```c
uint8_t hy = entity_hit_y[x];
if (hy >= player_y) {
    uint8_t delta = (uint8_t)(hy - player_y);
    if (delta < 0x2F) {
```

### Before

```
.loop@0_start:
   LDA   entity_hit_y,X
   CMP   __zpabi_refresh_hit_entities__player_y
   BCC   .if_end@0
   SEC
   SBC   __zpabi_refresh_hit_entities__player_y
   STA   __local_refresh_hit_entities__1
   CMP   #$2F
```

### After

```
.loop@0_start:
   LDA   entity_hit_y,X
   SEC
   SBC   __zpabi_refresh_hit_entities__player_y
   BCC   .if_end@0
   STA   __local_refresh_hit_entities__1
   CMP   #$2F
```

## When it applies

- The instructions are adjacent. The first is any `Mov` into A, or
  an `ADC`, `SBC`, `AND`, `ORA`, or `EOR` into A.
- The second is a compare of A, and its operand equals the `SBC`
  operand, both being the same `Data`, `ZP`, or immediate operand.
- The third is a `BCC`, `BCS`, `BEQ`, `BNE`, `BMI`, or `BPL`.
- Then, after an optional label that no branch or jump in the
  function targets, come `SEC` and `SBC` of the compare's operand.
- The branch target is a label in the function, and A and V are dead
  there (`asm_liveness.a_dead_at` and `v_dead_at`). After the rewrite
  the taken path has run the `SBC`, so A holds `M - N` where it held
  `M`, and V holds the subtraction's overflow.

The fall-through path runs the `SBC` either way, so nothing is
checked there.

## What it leaves alone

- A `BVC` or `BVS`, since `SBC` sets V and `CMP` doesn't.
- A branch whose target reads A or V, such as `A_mod_3` in
  `examples/zork_print_string_literal.c`, whose loop exit returns the
  value it compared, still in A.
- A label between the branch and the `SEC` that some other branch or
  jump targets, since a path arriving there would skip the moved
  subtraction.
- A compare and subtract of different operands, or of an operand
  that isn't `Data`, `ZP`, or an immediate.

## Related

[Compare and subtract of the same operands](compare_subtract.md)
does the same for operands of 2 bytes or more.
