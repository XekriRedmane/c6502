# Transfer plus or minus one

`apply_transfer_pm1_store` in
[passes/transfer_pm1_store.py](../../passes/transfer_pm1_store.py)
turns `TXA; SEC; SBC #1; STA M` into `DEX; STX M`, and `TXA; CLC;
ADC #1; STA M` into `INX; STX M`, with the same rewrites for Y. It
runs last in the peephole fixed point, which both pipelines run, but
no program in the test corpus makes it fire in the unoptimized
pipeline.

Storing `x - 1` or `x + 1` when the value is already in X routes it
through A, because A is the 6502's only register with an adder. `DEX`
or `INX` computes the same byte in X, and `STX` stores it. The four
instructions take 7 bytes and 10 cycles for an absolute `M`, and the
two that replace them take 4 bytes and 6. The rewrite also leaves A
untouched, so when register allocation had saved A around the
arithmetic, that save and its reload become dead, and later sweeps
remove them.

The pass runs after every other pass in the sweep, for a reason
given in a comment in `compile._peephole_fixedpoint`. Register
allocation can route the result through a temporary, as in `TXA;
SEC; SBC #1; STA __local; LDA __local; STA M`. By the end of the
sweep, `round_trip_load_drop` and `asm_dead_store` have collapsed
that to the plain `STA M` form, so the store this pass rewrites is
the one to `M`. If this pass ran first, it would produce `DEX; STX
__local; LDA __local; STA M`. The comment states that no later pass
collapses that shape, because the `LDA` reads a slot that was
written from X.

## Example

```c
unsigned char table[8];
unsigned char idx, value, prev;
void fetch(void) {
    value = table[idx];
    prev = idx - 1;
}
```

### Before

```
fetch:
   LDX   idx
   LDA   table,X
   STA   value
   TXA
   SEC
   SBC   #$01
   STA   prev
   RTS
```

### After

```
fetch:
   LDX   idx
   LDA   table,X
   STA   value
   DEX
   STX   prev
   RTS
```

With `next = idx + 1;` instead, the pass turns `TXA; CLC; ADC #$01;
STA next` into `INX; STX next`.

## When it applies

- The four instructions are adjacent. The first is a non-volatile
  `TXA` or `TYA`.
- The second and third are `SEC` and `SBC #1`, or `CLC` and `ADC #1`,
  with A as the destination.
- The fourth is a non-volatile `STA` to a `Data` or `ZP` operand.
  `STX` and `STY` have no `(zp),Y` mode, and the pass doesn't use
  their indexed modes.
- After the store, A is dead, the register that was transferred is
  dead (it now holds the new value), and C and V are dead (`c_dead_at`,
  `v_dead_at`). `DEX` and `INX` leave C and V as they were, where the
  `SBC` or `ADC` set them, but set N and Z from the new value just as
  the `SBC` or `ADC` did, so a branch on N or Z may follow. That's
  `get_next_code_byte2`'s `if (++Z_PC2_L == 0)` in
  examples/zork/print_zstring/pages.c, which became `INY; STY
  Z_PC2_L; BNE`, so the byte it read stays in A.

## What it leaves alone

- A read of the original register afterward. In

  ```c
  value = table[idx];
  prev = idx - 1;
  other = table[idx];
  ```

  the third statement indexes with X again, so `TXA; SEC; SBC #$01;
  STA prev` stays.
- A carry that a later instruction reads, as when the add is the low
  byte of a wider sum.
- A store to the soft stack, through a pointer, or to an indexed
  address.
- An add or subtract of any constant other than 1.

## Related

[Store X or Y directly](via_a_store_fold.md) handles a transfer with
no arithmetic before the store, and [DEC on a register](dec_register_peephole.md)
folds a register decrement that feeds a branch.
