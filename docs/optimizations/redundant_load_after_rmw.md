# Load after read-modify-write

`apply_redundant_load_after_rmw` in
[passes/redundant_load_after_rmw.py](../../passes/redundant_load_after_rmw.py)
deletes a load of a byte that the instruction just before it has
modified in place, when the loaded value isn't used. It runs in the
peephole fixed point, in both the optimized and the unoptimized
pipelines. It hasn't been seen to fire in the unoptimized pipeline,
which routes every update through a soft-stack temporary and so
doesn't produce an in-place `INC` or `DEC` on a variable.

`INC`, `DEC`, `ASL`, `LSR`, `ROL`, and `ROR` on a memory byte set N
and Z from the value they write back. A load of the same byte right
after sets N and Z to the same thing, and C and V are left alone by
both. When A isn't read before it's next written, the load does
nothing that a later instruction can observe. The usual source is a
signed count-down loop, where the loop test lowers to a load and
`BPL`, and `dec_peephole` has already turned the decrement into
`DEC`. Deleting the load saves 2 bytes and 3 cycles per iteration
for a zero-page counter. The pass also deletes `TXA` after `DEX` or
`INX`, and `TYA` after `DEY` or `INY`, which saves 1 byte and 2
cycles.

## Example

```c
#include <stdint.h>
void put(int8_t v);
void count(void) {
    for (int8_t x = 10; x >= 0; x--) put(x);
}
```

### Before

```
   LDA   #$0A
   STA   __local_count__x
.loop@0_start:
   LDA   __local_count__x
   STA   __zpabi_put__v
   JSR   put
.loop@0_continue:
   DEC   __local_count__x
   LDA   __local_count__x
   BPL   .count@asm_ssa_split@0
.count@asm_ssa_block@0:
   RTS
.count@asm_ssa_split@0:
   JMP   .loop@0_start
```

### After

```
   LDA   #$0A
   STA   __local_count__x
.loop@0_start:
   LDA   __local_count__x
   STA   __zpabi_put__v
   JSR   put
.loop@0_continue:
   DEC   __local_count__x
   BPL   .count@asm_ssa_split@0
.count@asm_ssa_block@0:
   RTS
.count@asm_ssa_split@0:
   JMP   .loop@0_start
```

A is dead on both paths out of the `BPL`. The `LDA` at the loop
top overwrites it on one, and the `RTS` on the other returns no
value in A. When the loop body leaves X free, as in
`for (int8_t x = 10; x >= 0; x--) buf[x] = 0;`, the counter lives in
X and the pass turns `DEX; TXA; BPL` into `DEX; BPL`.

## When it applies

- The read-modify-write and the load are adjacent. The
  read-modify-write is an `INC`, `DEC`, or shift or rotate of a
  `Data` or `ZP` byte, or an `INX`, `INY`, `DEX`, or `DEY`.
- The load reads the same operand into A, and it isn't volatile.
- A is dead after the load. The pass follows both arms of every
  branch and the target of every jump until it finds an instruction
  that writes A without reading it, a call, or a return that doesn't
  return a value in A. A path that comes back to a branch it has
  already followed counts as dead.

## What it leaves alone

- A load whose value is used, as in `return --lives;`, where the
  return reads A and `DEC lives; LDA lives; RTS` stays as it is.
- A volatile load, since the read itself is the observable effect
  of a volatile access.
- A load with anything between it and the read-modify-write.
- A soft-stack, pointer, or indexed operand. The 6502 has no
  read-modify-write in the `(zp),Y` mode, so those updates go
  through A and never take this shape.
- A path that jumps to a label outside the function, such as a tail
  call. The pass treats A as live there.

## Related

[DEC](dec_peephole.md) and [Multi-byte INC](inc_peephole.md) produce
the in-place forms this pass matches.
[DEC/INC branch fold](dec_inc_branch_fold.md) matches a narrower
form of the same shape, a `DEC` or `INC` and its load followed by a
branch on N or Z.
