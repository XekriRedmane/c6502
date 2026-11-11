# Logical-not jump fold

`LNOT_RULE` in
[passes/optimization/lnot_jump_fold.py](../../passes/optimization/lnot_jump_fold.py)
removes a `!` whose only use is a conditional jump, and flips the
sense of the jump. It runs in the TAC fixed-point loop as part of the
`fold_producer_jumps` `RuleSet`, together with the
[comparison jump fold](cmp_zero_jump_fold.md) and the
[AND jump fold](and_zero_jump_fold.md). It runs only under
`--optimize`.

`tac_to_asm` lowers `Unary(!, x, %t)` by testing `x`, loading `#0` or
`#1` into `%t` on two paths, and joining them. A jump on `%t` then
loads it again and branches with `ORA #$00; BEQ`. Jumping on `x` with
the opposite sense skips both the 0-or-1 value and the second test.
The rewrite doesn't depend on the width of `x`, because the jump's
own lowering ORs every byte of a multi-byte condition together.

## Example

```c
#include <stdint.h>
uint8_t ready(void);
uint8_t waits;
void poll(void) {
    if (!ready()) waits = 1;
}
```

### Before

```
Label(.poll@ssa_block@0)
FunctionCall(ready, [], %0.1)
Unary(!, %0.1, %1.1)
JumpIfFalse(%1.1, .if_end@0)
...
```

### After

```
Label(.poll@ssa_block@0)
FunctionCall(ready, [], %0.1)
JumpIfTrue(%0.1, .if_end@0)
...
```

The final code is `JSR ready; STA __local_poll__0; LDA
__local_poll__0; BNE .if_end@0`. The store and the reload of the
returned byte are left over from the asm passes, so the branch
doesn't yet follow the `JSR` directly.

## When it applies

- A `Unary(!, src, %t)` is followed immediately by a `JumpIfTrue` or
  `JumpIfFalse` on `%t`.
- `%t` is used exactly once in the function, by that jump.

`src` can be any value of any width. A `JumpIfTrue` becomes a
`JumpIfFalse` on `src`, and a `JumpIfFalse` becomes a `JumpIfTrue`.
When `src` is itself a comparison or a masked byte, the
[comparison](cmp_zero_jump_fold.md) or [AND](and_zero_jump_fold.md)
fold can then fold the new jump on a later round. That is how
`if (!(flags & 0x80))` ends as `LDA __zpabi_check__flags; BMI
.if_end@0`.

## What it leaves alone

- A `!` whose result has another use. In
  `int t = !a; if (t) seen = 1; return t;` the run reports 0 changes,
  and the final code keeps the `.lnot_true@0` / `.lnot_end@1`
  diamond and the `ORA #$00; BEQ` test after it.
- A `!` that isn't adjacent to its jump, until copy propagation and
  dead-store elimination close the gap on a later round.

## Related

[Short-circuit jump fold](short_circuit_jump_fold.md) removes the
0-or-1 value of `&&` and `||` in front of a jump in the same way.
