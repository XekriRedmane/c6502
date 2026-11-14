# Branching directly on `&&` and `||`

`FoldShortCircuitJump` in
[passes/optimization/short_circuit_jump_fold.py](../../passes/optimization/short_circuit_jump_fold.py)
removes the 0-or-1 value that `&&` and `||` compute when the only
reader of that value is the conditional jump right after it. The
short-circuit jumps then go straight to where that conditional jump
would have gone. The pass runs after SSA destruction and the
post-destruction [copy folding](copy_folding.md), repeated until it
makes no change. It doesn't run in the unoptimized pipeline.

`c99_to_tac` lowers `a && b` as a chain of jumps to a "false" label,
followed by a tail that stores 1 or 0 into a temporary `%t` and
joins at an "end" label. In `if (a && b)` the next instruction tests
`%t` again. The two tests make two layers of branches for one
decision, and the asm also stores `%t` to a zero-page byte and loads
it back. In the example below the function body drops from 28 bytes
to 13.

## Example

```c
#include <stdint.h>
uint8_t g;
void both(uint8_t a, uint8_t b) { if (a && b) g = 1; }
```

### Before

```
JumpIfFalse(@0.a, .and_false@0)
JumpIfFalse(@1.b, .and_false@0)
Label(.both@ssa_block@2)
Copy(1, %0.3)
Jump(.and_end@1)
Label(.and_false@0)
Copy(0, %0.3)
Label(.and_end@1)
JumpIfFalse(%0.3, .if_end@2)
Copy(1uc, g)
Label(.if_end@2)
Ret(None)
```

### After

```
JumpIfFalse(@0.a, .if_end@2)
JumpIfFalse(@1.b, .if_end@2)
Label(.both@ssa_block@2)
Copy(1uc, g)
Label(.if_end@2)
Ret(None)
```

The final assembly is `LDA __zpabi_both__a; BEQ .if_end@2;
LDA __zpabi_both__b; BEQ .if_end@2; LDA #$01; STA g`.

In `if (a || b)` a chain jump means the value is 1, and on 1 the
consumer `JumpIfFalse` falls through into the body. The pass then points the chain at a new label
`.either@scfold@0` in front of the body, and puts `Jump(.if_end@5)`
before that label for the case where no chain jump fired. The final
assembly is `LDA __zpabi_either__a; BNE .either@scfold@0;
LDA __zpabi_either__b; BEQ .if_end@5`.

## When it applies

The pass looks for six adjacent instructions.

```
Copy(C_ft, %t)
Jump(end)
Label(branch)
Copy(C_sc, %t)
Label(end)
JumpIfTrue(%t, T)   or   JumpIfFalse(%t, T)
```

These must all be true.

- Both copies write the same variable `%t`, and their sources are
  the `int` constants 0 and 1, in either order.
- The `Jump` targets the second label.
- `%t` is read only by the final conditional jump.
- Only that one `Jump` targets `end`.

The value `C_sc` is what `%t` holds when a chain jump to `branch`
fired. When the consumer jumps to `T` on `C_sc`, every jump to
`branch` is retargeted to `T` and the six instructions are deleted.
Otherwise the jumps to `branch` go to a new label
`.<function>@scfold@N`, and the six instructions become
`Jump(T)` followed by that label.

`Copy`s may sit between `end` and the consumer when they neither
read nor write `%t`. SSA destruction puts a phi's copies there when
the consumer skips an assignment: `if (c >= 0x60 && c < 0x80) c -=
0x20;` in `buffer_char` (examples/zork/print_zstring/output.c) has
`Copy(@0.c, @0.c.2)` between `.and_end` and the test. The copies run
on every path through the consumer, so they stay on both: in place
for the path that falls through, and for the chain, in a stub
`Label; <copies>; Jump(T)` after the function's last instruction,
which has to be a `Ret` or a `Jump`. In the flipped case they go
before the `Jump(T)` and again after the new label. The asm optimizer
gives a phi's names one location, so the copy emits nothing and the
stub is a bare `JMP`, which branch threading removes. `buffer_char`'s
test became `CMP #$60; BCC; CMP #$80; BCS; SEC; SBC #$20`, with `c`
kept in A, as in Zork's code.

Any number of jumps may target `branch`, since those are the chain.
A nested expression such as `(a && b) || c` produces two tails. One
retargets its chain to the other's `branch` label, which is itself
retargeted, so the pass follows the map until it reaches a label
that stays.

## What it leaves alone

- A value that something else reads. In
  `uint8_t c = a && b; h = c; if (c) g = 1;` the value is stored to
  `h`, so the 0 or 1 has to exist.
- A value that isn't tested by a jump right after the tail, such as
  `return a && b;`.
- A tail with anything between its instructions, or anything but
  copies that don't touch `%t` between it and the jump.
- Constants of another type than `int`, since the `c99_to_tac`
  lowering always uses `int` 0 and 1.

The pass runs after SSA destruction because before it the two
copies write different SSA names and a `Phi` joins them, which this
match doesn't recognize.

## Related

The [comparison jump fold](cmp_zero_jump_fold.md) and the
[logical-not jump fold](lnot_jump_fold.md) remove a computed 0 or 1
in front of a jump in other shapes.
