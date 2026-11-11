# INY and CPY for an incremented table byte

`apply_index_inc_cmp_y` in
[passes/index_inc_cmp_y.py](../../passes/index_inc_cmp_y.py) turns a
table byte plus or minus one, staged through a temporary and then
compared, into `LDY`, `INY` or `DEY`, and `CPY`. It runs in the
peephole fixed point, in both pipelines, but only fires with
`--optimize`. The unoptimized pipeline keeps its temporaries on the
soft stack, and the pass matches only a zero-page or absolute one.

The register allocator doesn't give Y to short-lived byte values, so
`table[i] + 1` lands in a zero-page temporary, and [multi-byte
INC](inc_peephole.md) turns the add into `INC` on that temporary. When
the sum is used only by a compare and Y is free, doing the arithmetic
in Y removes the store, the `INC`, and the reload. With a zero-page
temporary that saves 5 bytes and 9 cycles.

## Example

```c
#include <stdint.h>
uint8_t ceiling[8];
uint8_t beam_y, hit;
void check(uint8_t i)
{
    uint8_t top = (uint8_t)(ceiling[i] + 1);
    if (top == beam_y)
        hit = 1;
}
```

### Before

```
   LDX   __zpabi_check__i
   LDA   ceiling,X
   STA   __local_check__0
   INC   __local_check__0
   LDA   __local_check__0
   CMP   beam_y
   BNE   .if_end@0
   LDA   #$01
   STA   hit
.if_end@0:
   RTS
```

### After

```
   LDX   __zpabi_check__i
   LDY   ceiling,X
   INY
   CPY   beam_y
   BNE   .if_end@0
   LDA   #$01
   STA   hit
.if_end@0:
   RTS
```

`CPY` sets N, Z, and C from the same subtraction as `CMP`, because Y
holds the value A held in the original.

## When it applies

- Five consecutive instructions. They are a non-volatile load of an
  X-indexed absolute byte into A, a store of A to a temporary in zero
  page or absolute memory, an `INC` or `DEC` of that temporary, a
  load of the temporary into A, and a `CMP` against an immediate, a
  zero-page byte, or an absolute byte.
- A is dead after the `CMP` (`asm_liveness.a_dead_at`).
- Y is dead before the load (`y_dead_at`).
- The temporary is scratch the compiler owns, either a
  `__local_<fn>__*` slot of this function or a zero-page byte of the
  register allocator's pool that no zero-page global occupies.
- No path from the `CMP` reads the temporary before overwriting it.
  The walk follows branches and jumps, and the function's return
  ends it, since the temporary is dead there.

The rewrite leaves the temporary holding its old value, which is why
the last two checks exist. A read that only a loop's back edge reaches
counts, so in

```c
uint8_t prev = 0;
for (uint8_t i = 0; i < 8; i++) {
    out[i] = prev;
    prev = ceiling[i];
    prev++;
    if (prev == beam_y) hit = 1;
}
```

the store to `prev` stays, and `out[i]` gets the previous sum.

## What it leaves alone

- A Y-indexed table load. The 6502 has `LDY abs,X` but no `LDY abs,Y`.
- A compare against an indexed, soft-stack, or pointer operand, which
  `CPY` has no mode for.
- Code that later reads the value Y held before the load.
- A global as the temporary, as in `top = ceiling[i]; top++; if (top
  == beam_y) ...` with a global `top`. Its value can be read after the
  function returns.
- A sum that is used again after the compare, such as `top` in
  `if (top == beam_y) hit = top;`, where the store reads A and the
  temporary is still named later.

## Related

[Multi-byte INC](inc_peephole.md) and [DEC](dec_peephole.md) produce
the `INC` or `DEC` on the temporary that this pass matches.
