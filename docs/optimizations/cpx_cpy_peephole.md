# CPX and CPY

`apply_cpx_cpy_peephole` in
[passes/cpx_cpy_peephole.py](../../passes/cpx_cpy_peephole.py)
rewrites `TXA; CMP R` as `CPX R`, and `TYA; CMP R` as `CPY R`, when
nothing reads A afterwards. It runs in the peephole fixed point, in
both pipelines, but in practice only the optimized pipeline produces
the shape it matches.

`tac_to_asm` loads the left side of every comparison into A, because
the right side might be on the soft stack or behind a pointer, and
only `CMP` has the `(zp),Y` mode. When the register allocator later
puts the left side in X or Y, as it does for a loop counter, the
comparison becomes a transfer to A and a `CMP`. `CPX` and `CPY`
compare X or Y directly with an immediate, an absolute address, or a
zero-page byte, and set N, Z, and C the same way the `CMP` does after
the transfer. That saves the 1-byte, 2-cycle transfer, once per
iteration in a loop test.

## Example

```c
#include <stdint.h>
static uint8_t buf[16];
void clear(void) { for (uint8_t i = 0; i < 10; i++) buf[i] = 0; }
```

### Before

```
   LDX   #$00
.loop@0_start:
   TXA
   CMP   #$0A
   BCS   .loop@0_break
.clear@asm_ssa_block@0:
   LDA   #$00
   STA   buf,X
...
```

### After

```
   LDX   #$00
.loop@0_start:
   CPX   #$0A
   BCS   .loop@0_break
.clear@asm_ssa_block@0:
   LDA   #$00
   STA   buf,X
...
```

The right side doesn't have to be a constant. With the bound
`lim[k]` the allocator puts `i` in Y and stages the bound in a
zero-page local, and the pass turns `TYA; CMP __local_clear__0` into
`CPY __local_clear__0`.

## When it applies

The two atoms must be consecutive, a `Mov(X, A)` or `Mov(Y, A)`
followed by a `Compare(A, R)` where R is an `Imm`, `Data`, or `ZP`
operand. The transfer would have left the counter in A, and the
rewrite leaves A as it was, so A must be dead after the compare along
every path, as `asm_liveness.a_dead_at` checks. A return that hands
back A counts as a read. No check of the flags is required, since the compare
overwrites N, Z, and C in both forms and neither form changes V.

## What it leaves alone

- A right side on the soft stack or behind a pointer (`Frame`,
  `Stack`, `Indirect`, `IndirectY`), since `CPX` and `CPY` have no
  `(zp),Y` mode. An indexed right side such as `lim,X` is also left
  alone, since `CPX` and `CPY` have no indexed modes.
- A compare where A is read afterwards, on the fall-through or at
  the branch target.
- The unoptimized pipeline's comparisons. Nothing there is kept in X
  or Y, so the pass made no change on any `examples/` program or any
  program under `tests/chapter_*/valid/` under `--unopt`.

The unit tests in `tests/test_cpx_cpy_peephole.py` cover the soft
stack and pointer operands and each case where A is read afterwards.

## Related

[Loop counter in X](loop_counter_to_x.md) also rewrites a counter's
compare to `CPX` when it moves the counter into X after the peepholes,
and [subtract-and-test fusion](sub1_test_zero_peephole.md) drops the
compare from a count-down loop test.
