# Round-trip load drop

`apply_round_trip_load_drop` in
[passes/round_trip_load.py](../../passes/round_trip_load.py) deletes
the `LDA M` of an `STA M; LDA M` pair when the instruction before the
store already set N and Z from A. It runs in the peephole fixed
point, which both pipelines run, but no program in the test corpus
makes it fire in the unoptimized pipeline.

After `STA M`, A and M hold the same byte, so a following `LDA M`
leaves A as it was. Its only other effect is to set N and Z from
that byte. If the instruction before the store was an `LDA`, `TXA`,
`TYA`, `PLA`, `ADC`, `SBC`, `AND`, `ORA`, `EOR`, or an `ASL`, `LSR`,
`ROL`, or `ROR` of A, the flags already hold exactly those values,
so the load changes nothing and can go whether or not
anything reads A or the flags afterward. Each deletion saves 2 bytes
and 3 cycles for a zero-page `M`, or 3 bytes and 4 cycles for an
absolute one.

The pattern is common after register allocation, which routes a
value through a zero-page temporary and reads it straight back.
Once the load is gone, the store is often dead too, and
`asm_dead_store` removes it on a later sweep.

## Example

```c
unsigned char bump(unsigned char *p) {
    *p += 3;
    return *p;
}
```

### Before

```
bump:
   LDY   #$00
   LDA   ($80),Y
   CLC
   ADC   #$03
   STA   __local_bump__0
   LDA   __local_bump__0
   STA   ($80),Y
   LDA   ($80),Y
   RTS
```

### After

```
bump:
   LDY   #$00
   LDA   ($80),Y
   CLC
   ADC   #$03
   STA   __local_bump__0
   STA   ($80),Y
   LDA   ($80),Y
   RTS
```

`$80` is the zero-page slot that holds `p`, which the final output
names `__zpabi_bump__p_0`. The `STA __local_bump__0` is unread after
the rewrite, and `asm_dead_store` deletes it before the output is
emitted.

The pass has a second form for a memory-to-memory `Mov(M, D)`, which
emits as `LDA M; STA D`. When that atom follows the same writer and
`STA M`, the pass rewrites it to `Mov(Reg(A), D)`, a plain `STA D`.
`split_mem_to_mem` runs at the top of each sweep and splits every
non-volatile memory-to-memory `Mov` into a separate load and store
first, so this form fires only on one that a later pass creates
within the same sweep. No program in the test corpus produces one,
and the unit test `test_rewrites_mem_to_mem_after_sub_sta` is the
only place it's exercised.

## When it applies

The pass looks at three consecutive instructions.

1. The first writes A and sets N and Z from A's new value. That is
   any `Mov` into A, an `Add`, `Sub`, `And`, `Or`, or `Xor` into A, a
   `Pop` into A, or a shift or rotate of A.
2. The second is a non-volatile `STA M`, where M is a `Data` or `ZP`
   operand.
3. The third is a non-volatile `LDA M` of the same operand, which is
   deleted. In the second form it's a non-volatile `Mov(M, D)` with a
   memory destination, which becomes `STA D`.

## What it leaves alone

- A store after an instruction whose flags don't come from A. A
  `JSR` is the common case, as in `x = sample(); if (x == 0) ...`,
  which keeps its `STA x; LDA x; BNE`. A `CMP`, an `INC` or `DEC` of
  memory, and `CLC` or `SEC` don't qualify either.
- A volatile load or store, because the access has to happen.
- An `M` on the soft stack or behind a pointer, since the pass
  compares only `Data` and `ZP` operands for equality.
- A pair with anything between the store and the load.

## Related

[Redundant load](redundant_load.md) removes loads of a value already
in a register more generally, and [Self-store drop](self_store_drop.md)
handles the reverse order, `LDA M; STA M`.
