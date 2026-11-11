# Unreachable code drop

`apply_unreachable_drop` in
[passes/unreachable_drop.py](../../passes/unreachable_drop.py)
deletes the instructions after a `JMP` or a return that no path can
reach. It runs in the peephole fixed point, in both the optimized and
the unoptimized pipelines, right after
[dead label drop](dead_label_drop.md).

An instruction that follows an unconditional jump or a return, with no
referenced label in between, has no way in. Nothing falls into it, and
no branch names it. The unoptimized pipeline produces such code when
a `return` ends one arm of an `if`, because `c99_to_tac` still emits
the jump to the end of the `if` and the implicit `return 0` at the end
of the function. With `--optimize` the TAC passes have removed that
code before the asm stage, and on the chapter test corpus the pass
fired only in the unoptimized pipeline. In the optimized pipeline it
cleans up after the [carry-flag return peepholes](carry_return.md),
which copy a shared return tail into each path that reaches it.

## Example

```c
#include <stdbool.h>
#include <stdint.h>
__attribute__((return_bool_in_carry)) bool is_three(uint8_t x)
{
    return x == 3;
}
```

### Before

```
   LDA   #$00
   CMP   #$00
   BNE   .cmp_differ@2
   LDA   __zpabi_is_three__x
   CMP   #$03
.cmp_differ@2:
   BEQ   .cmp_true@0
   CLC
   RTS
.cmp_true@0:
   SEC
   RTS
   CMP   #$01
   RTS
```

### After

```
   LDA   #$00
   CMP   #$00
   BNE   .cmp_differ@2
   LDA   __zpabi_is_three__x
   CMP   #$03
.cmp_differ@2:
   BEQ   .cmp_true@0
   CLC
   RTS
.cmp_true@0:
   SEC
   RTS
```

The `CMP #$01; RTS` was the shared return tail at `.cmp_end@1`. Once
each path had its own copy, `dead_label_drop` deleted the label, and
the tail was left after an `RTS` with nothing leading to it.

Without `--optimize`, `if (x) return 1; else return 2;` gets the same
treatment. The pass deletes the `JMP .if_end@0` after the first
return's epilogue, which leaves `.if_end@0` with no reference, and
then deletes the label along with the implicit `return 0` and its
whole epilogue.

## When it applies

After each `Jump`, `Return`, or `Ret`, the pass deletes every
instruction up to the next label that some `Jump`, `Branch`, or `Phi`
argument in the function names. Labels with no reference in that
stretch go too. The pass then recomputes the referenced labels and
sweeps again, until a sweep deletes nothing. A chain in which each
dead block holds the only jump to the next label therefore goes in one
call. Without the inner loop the fixed point would spend a round per
label, and a chain longer than its 16-round cap failed the compile
(the `fuzz.stmts` seeds 4411, 4593, and 4793, without `--optimize`).

## What it leaves alone

- Code after a `JSR`, even to a routine that never returns, since a
  call isn't a terminator in the IR.
- Code after a conditional branch, which the fall-through path
  reaches.
- Dead blocks that name each other. If two blocks after a return end
  in jumps to each other's labels, both labels stay referenced and
  both blocks stay.

## Related

[Dead label drop](dead_label_drop.md) removes the labels whose loss
exposes dead code to this pass. [Unreachable code
elimination](unreachable_code_elimination.md) does the same job on the
TAC control-flow graph in the optimized pipeline.
