# Self-store drop

`apply_self_store_drop` in
[passes/self_store_drop.py](../../passes/self_store_drop.py) deletes
the `STA M` of an `LDA M; STA M` pair, which writes a byte back to
the place it was just read from. It runs in the peephole fixed point,
which both pipelines run, but no program in the test corpus makes it
fire in the unoptimized pipeline.

The store writes M's own value back to M and changes no register or
flag, so it can go regardless of whether M is read later. That saves
2 bytes and 3 cycles for a zero-page `M`, or 3 bytes and 4 cycles for
an absolute one. The pass keeps the `LDA`, since A or the flags may
still be needed. When they aren't, `dead_a_arith` removes the load on
a later sweep.

The pair comes from byte-wise lowering in which one byte of the
result is the same byte of an operand, such as the low byte of
`(unsigned char)ui` stored back into `ui`.

## Example

```c
unsigned int ui;
void low_byte_only(void) {
    ui = (unsigned char)ui;
}
```

### Before

```
low_byte_only:
   LDA   ui
   STA   ui
   LDA   #$00
   STA   ui+1
   RTS
```

### After

```
low_byte_only:
   LDA   ui
   LDA   #$00
   STA   ui+1
   RTS
```

The final output is `LDA #$00; STA ui+1; RTS`, because
`dead_a_arith` drops the `LDA ui` on the next sweep, once the next
load overwrites A and nothing reads the flags.

The pass also deletes a non-volatile `Mov(M, M)` whose source and
destination are the same `Data` or `ZP` operand. `split_mem_to_mem`
drops these at the top of every sweep as well, so this case catches
only one that a later pass creates within the sweep.

## When it applies

The two instructions must be adjacent. The first is a non-volatile
`Mov` from a `Data` or `ZP` operand into A, and the second is a
non-volatile `Mov` from A into the same operand, compared by name or
address and offset.

## What it leaves alone

- A volatile load or store. With `volatile unsigned int ui`, the same
  function keeps both accesses to `ui`, since a volatile read and
  write each have to happen.
- A pair with anything between the load and the store, even an
  instruction that leaves A and M alone.
- An operand on the soft stack or behind a pointer, since only
  `Data` and `ZP` operands are compared.

## Related

[Round-trip load drop](round_trip_load.md) handles the reverse order,
`STA M; LDA M`, and [Dead A arithmetic](dead_a_arith.md) removes the
load that this pass leaves behind when nothing reads it.
