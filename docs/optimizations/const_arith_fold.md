# Constant AND and ORA fold

`apply_const_arith_fold` in
[passes/const_arith_fold.py](../../passes/const_arith_fold.py) folds
an `AND` or `ORA` whose operands are both known into a single load,
and deletes an `AND #$FF` or `ORA #$00` that leaves A unchanged. It
runs in the peephole fixed point, in both the optimized and the
unoptimized pipelines, right after [memory constant
propagation](mem_const_prop.md).

Integer promotion makes a bitwise operation on byte values an `int`
operation, so `tac_to_asm` computes a high byte that is a constant,
such as `LDA #$00; AND #$00` for the high byte of `flags & 0x80`.
`LDA #k` sets N and Z from `k`, which is what the pair left in the
flags, and neither instruction touches C or V. No liveness check is
required, and each fold saves 2 bytes and 2 cycles.

## Example

```c
#include <stdint.h>
uint8_t flags;
int g;
void f(void) { g = flags & 0x80; }
```

### Before

```
   LDA   flags
   AND   #$80
   STA   g
   LDA   #$00
   AND   #$00
   STA   g+1
   RTS
```

### After

```
   LDA   flags
   AND   #$80
   STA   g
   LDA   #$00
   STA   g+1
   RTS
```

The identity rewrite appears in a byte join. For `w = ((uint16_t)hi
<< 8) | lo;` the high byte is `LDA hi; ORA #$00; STA w+1`, and the
pass deletes the `ORA #$00`. Without `--optimize` the same deletion
happens after a soft-stack load, as in `LDA (FP),Y; ORA #$00`.

A third rewrite turns `LDA #$00; ORA m` and `LDA #$FF; AND m` into
`LDA m`. None of the programs on this page produce that pair, and the
rewrite was checked by running the pass on hand-built IR.

## When it applies

All three rewrites look at two adjacent instructions that write A.

- `LDA #c1` followed by `AND #c2` or `ORA #c2` becomes `LDA` of
  `c1 & c2` or `c1 | c2`.
- `ORA #$00` or `AND #$FF` right after any `LDA`, `TXA`, `TYA`,
  `ADC`, `SBC`, `AND`, `ORA`, or `EOR` that writes A is deleted. The
  instruction before already set N and Z from the same value.
- `LDA #$00; ORA m` or `LDA #$FF; AND m` becomes `LDA m`, for any
  operand `m`.

After a fold the pass moves on past both instructions, so a chain
such as `LDA #1; AND #3; ORA #4` takes a second call to fold
completely.

## What it leaves alone

- `ADC` and `SBC` on constants, as in the high byte `LDA #$00; ADC
  #$00` of `g = a + 1;` with an `int g`. The result depends on C,
  which can't be known from the two instructions alone.
- `EOR`, whose IR form has two sources.
- An identity operation after an instruction outside that list, such
  as a label, a store, or `PLA`. After a label or a store, N and Z
  might not reflect A. `PLA` does set them from A, but the pass
  doesn't include it.
- Instructions that aren't adjacent.

## Related

[Memory constant propagation](mem_const_prop.md) turns memory
operands into the immediates this pass folds. [Dead A
arithmetic](dead_a_arith.md) deletes the high-byte arithmetic
altogether once its result goes unread.
