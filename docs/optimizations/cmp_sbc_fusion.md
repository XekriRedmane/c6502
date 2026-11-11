# CMP and SBC fusion

`apply_cmp_sbc_fusion` in
[passes/cmp_sbc_fusion.py](../../passes/cmp_sbc_fusion.py) rewrites
`LDA M; CMP N; Bcc L1; L2: SEC; SBC N` as `LDA M; SEC; SBC N; Bcc L1;
L2:`, so the subtraction sets the flags the branch reads and the
`CMP` goes away. It runs in the peephole fixed point, which both
pipelines run. No program in the test corpus makes it fire in either
pipeline, for the reason given under "When it applies".

`CMP N` sets C, Z, and N from `A - N`, and `SEC; SBC N` sets the same
three flags from the same subtraction. It also sets V, which `CMP`
leaves alone, and it writes the difference to A. So when the code
after the branch starts with `SEC; SBC N`, the subtraction can move
ahead of the branch and stand in for the compare, for any branch
that reads only C, Z, or N. That saves the `CMP`, 3 bytes for an
absolute `N`. The fall-through path is 4 cycles faster, and the
taken path is 2 cycles slower, because it now runs `SEC; SBC N`
where it ran `CMP N`.

## Example

No C program reaches this pass in the required shape, so the listing
is the input of the unit test `test_lda_cmp_bcc_label_sec_sbc_fuses`
in `tests/test_cmp_sbc_fusion.py`, lowered to 6502 text, and the
output is what the pass returns for it. The module docstring gives
`if (a >= b) x = a - b;` as the motivating C.

### Before

```
f:
   LDA   M
   CMP   N
   BCC   .skip
.cont:
   SEC
   SBC   N
   RTS
.skip:
   RTS
```

### After

```
f:
   LDA   M
   SEC
   SBC   N
   BCC   .skip
.cont:
   RTS
.skip:
   RTS
```

## When it applies

- The six instructions are adjacent. The first is any `Mov` into A,
  or an `ADC`, `SBC`, `AND`, `ORA`, or `EOR` into A.
- The second is a compare of A, and its operand equals the `SBC`
  operand, both being the same `Data`, `ZP`, or immediate operand.
- The third is a `BCC`, `BCS`, `BEQ`, `BNE`, `BMI`, or `BPL`.
- The fourth is a label that no branch or jump in the function
  targets, so only the fall-through path reaches it.
- The fifth and sixth are `SEC` and `SBC` of the compare's operand.
- The branch target is a label in the function, and A and V are dead
  there (`asm_liveness.a_dead_at` and `v_dead_at`). After the rewrite
  the taken path has run the `SBC`, so A holds `M - N` where it held
  `M`, and V holds the subtraction's overflow.

The fourth condition keeps the pass from firing in practice.
`dead_label_drop` runs earlier in every sweep and deletes each label
that nothing targets, so by the time this pass runs, the label it
requires between the branch and the `SEC` is gone. With `if (a >= b) x
= a - b;` the input to the pass is `LDA a; CMP b; BCC .if_end@0; SEC;
SBC b; STA x`, which doesn't match.

The fall-through path runs the `SBC` either way, so nothing is
checked there.

## What it leaves alone

- A `BVC` or `BVS`, since `SBC` sets V and `CMP` doesn't.
- A branch whose target reads A or V, such as a `.skip:` followed by
  an `RTS` that returns A, which would return `M - N` on the path
  that returns `M`.
- A label that some other branch or jump targets, since a path
  arriving there would skip the moved subtraction.
- A compare and subtract of different operands, or of an operand
  that isn't `Data`, `ZP`, or an immediate.

## Related

[Dead label drop](dead_label_drop.md) removes the label this pattern
depends on.
