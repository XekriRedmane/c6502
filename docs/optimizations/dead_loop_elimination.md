# Dead loop elimination

`eliminate_dead_loops` (the `EliminateDeadLoops` pass) in
[passes/optimization/dead_loop_elimination.py](../../passes/optimization/dead_loop_elimination.py)
deletes a loop that has no side effects and whose values nobody reads
after it. It runs in the TAC fixed-point loop under `--optimize`, on
SSA form, right after the jump folds. The unoptimized pipeline doesn't
run it.

A loop like `while (--i);` over a local counter does nothing that the
rest of the program can observe. It spins until the counter reaches
zero and then throws the counter away. The pass finds such a loop by
its back edge, checks that its body is pure and that none of the SSA
values it defines is read outside it, and then replaces the loop's
header with a jump to the loop's exit. The rest of the body becomes
unreachable, and [unreachable code elimination](unreachable_code_elimination.md)
removes it on the same sweep. The whole loop, including its
decrement, compare, and branch on every iteration, disappears from the
output.

## Example

```c
#include <stdint.h>
void delay(void) {
    uint8_t i = 200;
    while (--i)
        ;
}
```

### Before

```
Label(.delay@ssa_block@0)
Copy(200uc, %0.1)
Copy(%0.1, @0.i.1)
Label(.loop@0_continue)
Phi(@0.i.2, [PhiArg(.delay@ssa_block@0, @0.i.1), PhiArg(.delay@ssa_block@1, @0.i.3)])
Binary(-, @0.i.2, 1uc, %1.1)
Copy(%1.1, @0.i.3)
JumpIfFalse(%1.1, .loop@0_break)
Label(.delay@ssa_block@1)
Jump(.loop@0_continue)
Label(.loop@0_break)
Ret(None)
```

### After

```
Label(.delay@ssa_block@0)
Copy(200uc, %0.1)
Copy(%0.1, @0.i.1)
Label(.loop@0_continue)
Jump(.loop@0_break)
Label(.delay@ssa_block@1)
Jump(.loop@0_continue)
Label(.loop@0_break)
Ret(None)
```

The header keeps its label, because code outside the loop could still
jump to it. Unreachable code elimination then drops everything from
`.loop@0_continue` to `.loop@0_break`, and dead-store elimination
drops the initial copies, so `delay` compiles to a lone `RTS`.

A nested pair of empty loops, such as `do { uint8_t y = 0; while
(--y); } while (--d);`, goes in one call, since the outer loop's body
is pure once the inner loop is counted as part of it.

## When it applies

Each natural loop (one header and the blocks that reach its back
edges) is checked against these conditions.

- No instruction in the body is a `FunctionCall`, an `IndirectCall`, a
  `Store`, an `IndexedStore`, an `IndexedSymbolStore`, an
  `IndirectIndexedStore`, or a `Ret`, and no load in the body is
  volatile.
- Every name the body defines is an SSA value. A write to a static,
  an address-taken local, or an aggregate stops the pass, since
  proving that such a write is dead would take a liveness analysis
  over non-SSA names.
- No instruction outside the body reads a value the body defines.
  This covers a Phi at the exit that takes a value from the loop.
- Exactly one edge leaves the loop.
- The exit block and the header both start with a label.

When the exit block has Phis, the argument that named the old exit
edge's source block is retagged to the header, which is now the
exit's predecessor on that side.

## What it leaves alone

- A loop whose result is used afterward, such as a bit count `while
  (n) { n >>= 1; k++; }` followed by `return k;`.
- A loop over a `volatile` counter. A volatile local isn't renamed into
  SSA form, so its writes fail the SSA-def check, and `volatile uint8_t
  i` is the way to keep a busy-wait delay loop in the output.
- A loop with more than one exit, such as one with two `break`
  statements, because merging exit Phi arguments from several edges is
  safe only when their sources agree, and the pass doesn't check that.
- A loop with no exit, such as `for (;;) i++;`. Deleting it would
  change a program that never returns into one that does.
- Any loop outside SSA form. The pass returns the function unchanged
  when it has no set of SSA names.

## Related

[Unreachable code elimination](unreachable_code_elimination.md)
removes the body this pass disconnects.
