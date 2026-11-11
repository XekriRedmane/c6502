# Move coalescing

`coalesce_moves` in
[passes/optimization_asm/coalescing.py](../../passes/optimization_asm/coalescing.py)
merges two values joined by a copy into one node of the interference
graph, so that the register allocator gives both the same byte. It
runs once per function in each of the two runs of the asm-level SSA
optimizer, after the copy propagation and dead-code fixed point and
after `build_interference`, and before the X / Y eligibility scan and
`color_graph`. Only the `--optimize` pipeline runs it.

Asm SSA gives every definition of every byte its own name. A loop
variable therefore has one name for its value at the loop head (the
Phi), one for the value that enters the loop, and one for each value
the body computes. If the allocator puts these in different bytes,
SSA destruction has to copy one into the other on each edge into the
loop head, and each copy is an `LDA` and an `STA`. When the names
share a byte, each of those copies is a copy of the byte onto itself,
which is dropped before emit. An add into the loop variable then
reads and writes the same byte, which is the shape the
[INC](inc_peephole.md) and [carry INC](carry_inc.md) peepholes
rewrite.

The pass merges three kinds of pairs, in this order.

1. Move pairs. These are a `Mov(Pseudo a, Pseudo b)`, a Phi's
   destination with each of its Pseudo arguments, and an adjacent
   `Mov(Pseudo a, A); Mov(A, Pseudo b)`, which is how `tac_to_asm`
   copies one value to another through A.
2. Byte copies, by either of the two copy forms, where one side is a
   byte of a multi-byte name. Asm SSA doesn't split a name that's a
   `(zp),Y` base pointer or the target of an in-place shift, so a
   copy between it and a 1-byte name isn't a move pair. The 1-byte
   name merges into the multi-byte node at the byte that lines the
   two up, and `CoalesceResult.byte_offset` records the position.
3. Carry bytes, `Mov(P_a, A); Add(Imm(0), A); Mov(A, P_b)` and the
   same with `Sub`. That's the high byte of an add whose other
   operand's high byte is zero. When `P_a` and `P_b` share a byte,
   [carry INC](carry_inc.md) turns the three instructions into `BCC
   skip; INC P` (or `BCS skip; DEC P`).

The merge is aggressive in the Chaitin style, with no check on the
merged node's degree. If a merged node finds no free byte, the
allocator spills it to the soft-stack frame. The module docstring
states that this hasn't been seen on any program in the corpus.

## Example

```c
#include <stdint.h>
uint16_t sum(const uint8_t *p, uint8_t n) {
    uint16_t s = 0;
    for (uint8_t i = 0; i < n; i++) s += p[i];
    return s;
}
```

The pass returns a map from names to their representatives and
doesn't change the IR, so these listings are the final assembly of
two complete compiles with `--optimize`. The first has
`coalesce_moves` replaced by a function that merges nothing. In the
second, the pass merges each of the three Phis at the loop head with
its argument from the loop body, as `%9.1.b0.v1` into
`@2.s.2.b0.v2`, `%9.1.b1.v1` into `@2.s.2.b1.v2`, and `%11.1.b0.v1`
into `@3.i.2.b0.v2`.

### Before

```
sum:
   SUBROUTINE

   LDA   #$00
   STA   __local_sum__s
   LDX   #$00
   LDY   #$00
.loop@0_start:
   CPY   __zpabi_sum__n
   BCS   .loop@0_break
   LDA   (__zpabi_sum__p_0),Y
   STA   __local_sum__0
   LDA   __local_sum__s
   CLC
   ADC   __local_sum__0
   STA   __local_sum__1
   TXA
   ADC   #$00
   STA   __local_sum__s
   INY
   LDX   __local_sum__s
   LDA   __local_sum__1
   STA   __local_sum__s
   JMP   .loop@0_start
.loop@0_break:
   RTS
```

### After

```
sum:
   SUBROUTINE

   LDA   #$00
   STA   __local_sum__s
   LDX   #$00
   LDY   #$00
.loop@0_start:
   CPY   __zpabi_sum__n
   BCS   .loop@0_break
   LDA   (__zpabi_sum__p_0),Y
   CLC
   ADC   __local_sum__s
   STA   __local_sum__s
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   INY
   JMP   .loop@0_start
.loop@0_break:
   RTS
```

Without the merges, the new low byte of `s` goes to a temporary
(`__local_sum__1`) and the new high byte goes through a pool byte to
X, and the back edge copies both into place. With them, the add
writes `s` in place and its high byte, which the allocator puts in
X, becomes `BCC; INX`. The loop goes from 30 bytes and 45 cycles per
iteration to 18 bytes and 26 or 27 cycles.

## When it applies

A pair merges when all of these are true.

- Both names are nodes of the interference graph.
- For a move pair, the two nodes have the same width, both Pseudos
  have offset 0, and there's no interference edge between them, so
  the two values are never live at once.
- For a byte copy or a carry byte, the smaller node fits inside the
  larger one at the offset that lines the two bytes up, and there's
  no interference edge between them. An edge through a multi-byte
  node is imprecise, since the node is live while any of its bytes
  is. When the larger node has more than one byte, the pass instead
  renames each byte of every multi-byte name, recomputes liveness,
  and merges when no two names that would share a byte are live at
  once.
- Neither `Mov` is volatile.

Each round takes its pairs in instruction order, with the A-routed
move pairs after the others. A merged node takes the union of both
nodes' edges, and it's live across a call when either input was.

## What it leaves alone

- Two values that are live at once. In the loop step `t = a + b; a =
  b; b = t;`, the Phi copies `a ← b` and `b ← t` each join two values
  that overlap, so only the loop counter's Phi merges, and both
  copies stay.
- Names outside the graph, which are statics, the function's own
  parameters, and the sources of a `LoadAddress`. A copy into a
  callee's zp_abi slot is a store to a `Data` operand, so it isn't a
  pair at all.
- A volatile `Mov`, because merging would put a volatile value and a
  plain temporary in one storage cell, so that ordinary reads and
  writes of the temporary would touch the volatile cell.
- An A-routed copy whose two `Mov`s aren't adjacent, since anything
  between them might change A.
- A move pair of two different widths, unless the byte-copy round
  can line one up inside the other.

## Related

[Register allocation](regalloc.md) colors the merged nodes.
[INC](inc_peephole.md) and [carry INC](carry_inc.md) rewrite the
in-place adds that the merges produce.
