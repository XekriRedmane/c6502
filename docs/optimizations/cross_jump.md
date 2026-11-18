# Cross jumping

`apply_cross_jump` in [passes/cross_jump.py](../../passes/cross_jump.py)
shares the instructions two blocks end with when both then jump to
the same place: the later block jumps into the earlier one's copy. It
runs in the peephole fixed point, which both pipelines run.

The block that jumps runs the same instructions as before and the
same final jump, so what it computes doesn't change. It takes one
more `JMP`, 3 cycles, and the bytes of the instructions it no longer
holds go.

## Example

From `instr_mul` in `examples/zork/numbers.c`, where `a <<=
1` for `b == 2` and `a <<= 2` for `b == 4` end in the same shift and
the same jump.

### Before

```
   BNE   .vthread@0
   ASL   __zpabi_store_with_sign__n_0
   ROL   __zpabi_store_with_sign__n_1
   JMP   .if_end@4
.vthread@0:
   CMP   #$04
   BNE   .if_else@7
   ASL   __zpabi_store_with_sign__n_0
   ROL   __zpabi_store_with_sign__n_1
   ASL   __zpabi_store_with_sign__n_0
   ROL   __zpabi_store_with_sign__n_1
   JMP   .if_end@6
```

### After

```
   BNE   .vthread@0
.xjump@0:
   ASL   __zpabi_store_with_sign__n_0
   ROL   __zpabi_store_with_sign__n_1
   JMP   .if_end@4
.vthread@0:
   CMP   #$04
   BNE   .if_else@7
   ASL   __zpabi_store_with_sign__n_0
   ROL   __zpabi_store_with_sign__n_1
   JMP   .xjump@0
```

`.if_end@6` and `.if_end@4` label the same instruction, so the two
jumps go to the same place. The `* 4` path shifts once and joins the
`* 2` path, as Zork's code does, 4 bytes shorter. In the whole
interpreter it takes 56 bytes off, and the first Zork session runs
35,000 cycles longer, 0.09%.

## When it applies

- Two `JMP`s go to the same place: labels in the function that name
  the same instruction, or the same function as a tail call, with
  the same register arguments.
- The instructions before each, back to the first that differs, are
  equal one for one, and none of them is a label, branch, jump, or
  return, so no path enters either run partway.
- The run is at least two instructions long.
- The later block's run and `JMP` become one `JMP` to the start of the
  earlier run. When no label is there, the pass adds one,
  `.xjump@N`, numbered across the program.
- The pair with the longest run goes first, and the pass looks again
  until no pair is left in the function.

## What it leaves alone

- Blocks that end in a return, which [tail merge](tail_merge.md)
  shares by inverting a branch around one copy.
- A run that differs only in its labels or branches.
- A run of one instruction. Many of the comparisons in Zork's
  `do_2op` end in the same `LDA #1; JMP`, and sharing the `LDA #1`
  saves 2 bytes for a 3-cycle `JMP`. On the interpreter's hottest
  paths, that came to 59 more bytes saved for 52,000 more cycles in
  the first session.

## Related

[Tail merge](tail_merge.md) does the same for repeated return tails.
[Tail call](tail_call.md) makes the `JSR f; RTS` at the end of a
function the `JMP f` that this pass compares.
