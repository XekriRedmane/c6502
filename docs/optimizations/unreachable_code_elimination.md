# Unreachable code elimination

`eliminate_unreachable_code` (the `EliminateUnreachableCode` pass) in
[passes/optimization/unreachable_code_elimination.py](../../passes/optimization/unreachable_code_elimination.py)
removes TAC blocks that no path from the function's entry reaches, and
then tidies the Phis, jumps, and labels that the removal leaves
behind. It runs in the TAC fixed-point loop under `--optimize`, on SSA
form, right after [dead loop elimination](dead_loop_elimination.md).
The unoptimized pipeline doesn't run it.

Constant folding turns a conditional jump on a constant into a `Jump`
or drops it, and dead loop elimination points a loop's header past its
body. Either way, some blocks lose every edge into them, and this pass
is what deletes them. It works on the control-flow graph in five steps,
in this order.

1. A depth-first walk from the entry marks the reachable blocks, and
   every other block is deleted. A Phi argument that named a deleted
   block is dropped too.
2. A Phi argument whose predecessor block remains but no longer has
   an edge into the Phi's block is dropped. Constant folding leaves
   this case when it deletes a `JumpIfFalse` on a true constant.
3. A Phi with one argument left becomes a `Copy` of that argument, and
   a Phi with none is deleted.
4. A block's final jump is deleted when every successor of the block
   is the next block in source order, since falling through reaches
   the same place. This covers `Jump`, `JumpIfTrue`, `JumpIfFalse`,
   `JumpIfCmp`, and `JumpIfMasked`.
5. A label at the start of a block is deleted when no jump targets it
   and no Phi argument names it.

Fewer blocks and labels give the later passes less to look at, and a
`Jump` left in place would become a 3-byte, 3-cycle `JMP`.

## Example

```c
#include <stdint.h>
#define DEBUG 0
uint8_t trace;
uint8_t step(uint8_t x) {
    uint8_t r = x;
    if (DEBUG) {
        trace = x;
        r = 0;
    }
    return r + 1;
}
```

Constant folding has already turned the `if (DEBUG)` test into a
`Jump` over the body.

### Before

```
Label(.step@ssa_block@0)
Copy(@0.x, @1.r.1)
Jump(.if_end@0)
Label(.step@ssa_block@1)
Copy(@0.x, trace)
Copy(0uc, %0.1)
Copy(%0.1, @1.r.2)
Label(.if_end@0)
Phi(@1.r.3, [PhiArg(.step@ssa_block@0, @1.r.1), PhiArg(.step@ssa_block@1, @1.r.2)])
ZeroExtend(@1.r.3, %1.1)
Binary(+, %1.1, 1, %2.1)
Truncate(%2.1, %3.1)
Ret(%3.1)
```

### After

```
Copy(@0.x, @1.r.1)
Copy(@1.r.1, @1.r.3)
ZeroExtend(@1.r.3, %1.1)
Binary(+, %1.1, 1, %2.1)
Truncate(%2.1, %3.1)
Ret(%3.1)
```

All five steps fire here. The `if` body is deleted, the Phi loses the
argument from it and becomes a `Copy`, the `Jump` now targets the next
block and goes, and none of the three labels has a reference left. The
final assembly for `step` is `LDA x; CLC; ADC #$01; RTS`.

The listing under "Before" is the output of the constant-folding call
earlier in the same sweep. The three passes between the two calls made
no change to `step`. The pass prunes Phi arguments in place on
instruction objects that its input shares, so a copy of the input
taken after the call would already show the pruned Phi.

## When it applies

The pass runs on every function in each sweep, and each step fires
wherever its pattern appears.

## What it leaves alone

- A block that any remaining jump reaches, even when the jump can't be
  taken at run time. Deciding that is constant folding's job, and this
  pass only follows the edges in the graph.
- A label that a Phi argument names, even when no jump targets it. SSA
  destruction finds each Phi argument's predecessor block by that
  label.
- A `Ret`, which is never treated as a jump to the next block.
- An empty block. Its label goes when nothing names it, and an empty
  block contributes no instructions to the flattened function, so the
  pass doesn't remove it from the graph.

## Related

[Dead loop elimination](dead_loop_elimination.md) and
[constant folding](constant_folding.md) produce most of the dead
blocks. [Unreachable drop](unreachable_drop.md) does the same job on
the 6502 assembly in the peephole fixed point.
