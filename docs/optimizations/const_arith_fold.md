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
pass deletes the `ORA #$00`. Without `-O` the same deletion
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

Three more look at one instruction, two, or three.

- `AND #$00` becomes `LDA #$00`, and `ORA #$FF` becomes `LDA #$FF`,
  after any instruction, since the result doesn't depend on A and N
  and Z come out the same. Zork's `branch_to_offset` keeps the high
  byte of `Z_PC.lo` with `(Z_PC.lo & 0xFF00) | new_lo`, whose low byte
  was `LDA Z_PC; AND #$00; ORA new_lo`. That's now `LDA #$00; ORA
  new_lo`, and then `LDA new_lo` by the rule before.
- `LDA #c1; CLC; ADC #c2` becomes `LDA` of `c1 + c2`, and `LDA #c1;
  SEC; SBC #c2` becomes `LDA` of `c1 - c2`, both modulo 256, when C
  and V are dead after (`asm_liveness.c_dead_at`, `v_dead_at`). N and
  Z come out as the add left them. `if (offset & 0x8000) pages +=
  0x100;` on a `pages` whose high byte is 0 adds 1 to 0 this way.

- `CLC; ADC #$00` becomes `CLC`, and `SEC; SBC #$00` becomes `SEC`,
  when V is dead after and N and Z are dead or were set from A by the
  instruction before the `CLC` or `SEC`. The add leaves A and C as
  they were, clears V, and sets N and Z from A. A 2-byte add of a
  value whose low byte is 0 leaves this shape on the low byte, as in
  `Z_HEADER_ADDR + ((uint16_t)sector << 8)` in Zork's `main`, which
  was `LDA Z_HEADER_ADDR; CLC; ADC #$00; STA disk_buffer; LDA
  Z_HEADER_ADDR+1; ADC sector`. Four of these in Zork's interpreter
  took 8 bytes.

After a fold the pass moves on past both instructions, so a chain
such as `LDA #1; AND #3; ORA #4` takes a second call to fold
completely.

## What it leaves alone

- `ADC` and `SBC` on constants without a `CLC` or `SEC` right before
  them, as in the high byte `LDA #$00; ADC #$00` of `g = a + 1;` with
  an `int g`. The result depends on C, which can't be known from the
  instructions alone. With the `CLC` or `SEC`, the fold waits for C
  and V to be dead.
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
