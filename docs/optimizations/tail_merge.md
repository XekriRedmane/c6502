# Tail merge

`apply_tail_merge` in [passes/tail_merge.py](../../passes/tail_merge.py)
replaces a branch over a return sequence with a branch to another
copy of the same sequence. It runs in the peephole fixed point, which
both pipelines run, but it can't fire in the unoptimized pipeline,
where every function exits through the compound `Ret` atom and the
pass matches only sequences that end in the bare `Return`.

A function with several early returns of the same value ends each
one with the same few instructions, such as `LDX #$00; TXA; RTS` for
`return 0` from a function that returns an `int` in A and X. Where a
copy is skipped over by a conditional branch, as in `BCS L; <tail>;
L:`, the pass removes that copy and branches on the opposite
condition to the last copy in the function. Hand-written 6502 code
shares a not-found exit the same way. Each removed copy of a
3-instruction tail like the one above saves 4 bytes. The path that
returns now takes the branch, which costs 1 cycle more (2 more if
the branch crosses a page), and the path that continues falls
through, which costs 1 cycle less.

## Example

```c
int lookup(unsigned char key, unsigned char lo, unsigned char hi) {
    if (key < lo)
        return 0;
    if (key > hi)
        return 0;
    return key - lo;
}
```

### Before

```
lookup:
   LDA   __zpabi_lookup__key
   CMP   __zpabi_lookup__lo
   BCS   .if_end@0
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDA   __zpabi_lookup__hi
   CMP   __zpabi_lookup__key
   BCS   .if_end@1
   LDX   #$00
   TXA
   RTS
.if_end@1:
   ...
```

### After

```
lookup:
   LDA   __zpabi_lookup__key
   CMP   __zpabi_lookup__lo
   BCC   .tail@0
.if_end@0:
   LDA   __zpabi_lookup__hi
   CMP   __zpabi_lookup__key
   BCS   .if_end@1
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@1:
   ...
```

The second copy is the one the pass keeps, so its own branch stays.
The pass put the fresh label `.tail@0` in front of it, because no
label preceded it. When a label already sits there, the branch
targets that label instead.

## When it applies

- A conditional branch to label L is followed by a tail and then by
  L itself.
- The tail is 2 to 4 instructions ending in a `Return`, with no
  label, branch, jump, or other exit before the end. Since nothing in
  it can be entered or left partway, it computes the same result
  wherever it runs, given the same registers and flags, and a branch
  carries those over unchanged.
- An identical tail, compared instruction by instruction, appears
  elsewhere in the function outside the tails being removed. The pass
  keeps the last such copy.
- The estimated distance from the branch to the kept copy, using the
  instruction sizes from the simulator's assembler, is at most 120
  bytes. The margin under the 127-byte reach of a branch allows for
  later passes that add code between them. A farther target would
  make `expand_long_branches` emit an inverted branch over a `JMP`,
  which saves nothing.

## What it leaves alone

- A lone `RTS`. In a `void` function with two early `return;`
  statements, each `BCS .if_end@N; RTS` stays, since a branch to a
  shared `RTS` would save at most one byte.
- Tails that differ in any instruction, such as `return 0` and
  `return 1`.
- A tail whose only other copy is out of branch reach.
- Every function in the unoptimized pipeline.

## Related

[Tail call](tail_call.md) shortens a different kind of exit, and
[Branch invert](branch_invert.md) cleans up branches over jumps.
