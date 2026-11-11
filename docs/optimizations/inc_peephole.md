# Multi-byte INC

`apply_inc_peephole` in
[passes/inc_peephole.py](../../passes/inc_peephole.py) rewrites an
in-place add of 1 to a value in memory as a chain of `INC`
instructions. It runs in the peephole fixed point, in both the
optimized and the unoptimized pipelines.

`tac_to_asm` lowers `x += 1` on a multi-byte `x` as one
load-add-store per byte, with the carry from each `ADC` feeding the
next byte. The 6502's `INC` increments a byte in memory without
going through A, and sets Z when the byte wraps to zero. A carry into
the next byte happens exactly when the byte below wraps, so a `BNE`
after each `INC` skips the rest of the chain whenever there's no
carry. For a 2-byte absolute operand the chain takes 8 bytes and 9 to
14 cycles, where the `ADC` form takes 17 bytes and 22 cycles.

## Example

```c
static unsigned int count;
void tick(void) { count += 1; }
```

### Before

```
   LDA   count
   CLC
   ADC   #$01
   STA   count
   LDA   count+1
   ADC   #$00
   STA   count+1
   RTS
```

### After

```
   INC   count
   BNE   .inc_done@0
   INC   count+1
.inc_done@0:
   RTS
```

A 1-byte operand becomes a lone `INC`. When A is read after the add,
the pass reloads it from the top byte, which is still shorter than
the `ADC` form. For `return ++hits;` with a `static unsigned char
hits`, the pass replaces `LDA hits; CLC; ADC #$01; STA hits` with
`INC hits; LDA hits`.

## When it applies

The pass matches a `CLC; ADC #1` on the first byte followed by an
`ADC #0` on each later byte, with no `CLC` between them. Each byte's
load must read the same location its store writes, and that location
must be a `Data` or `ZP` operand. The bytes don't have to be
adjacent in memory, because the byte-granular register allocator can
put the bytes of one value in unrelated zero-page slots, and only the
multi-byte add-1 lowering emits this shape.

## What it leaves alone

- An operand on the soft stack, through a pointer, or indexed by X,
  because `INC` has no `(zp),Y` mode. A soft-stack local in the
  unoptimized pipeline is a `Frame` operand, so `i += 1` on one keeps
  its `ADC` chain.
- An add whose stores go somewhere other than where its loads read,
  as when SSA destruction routes the sum through a temporary.

The `INC` chain leaves C unchanged where the `ADC` chain would set
it. Nothing reads that carry, since c6502 never reads a flag across
separate operations and every add and subtract starts with its own
`CLC` or `SEC`.

## Related

[Carry INC](carry_inc.md) handles the carry bytes of any add in
place, and [DEC](dec_peephole.md) is the 1-byte subtract
counterpart.
