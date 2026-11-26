# Logical-not jump fold

`LNOT_RULE` in
[cpp/src/passes/optimization/lnot_jump_fold.h](../../cpp/src/passes/optimization/lnot_jump_fold.h)
removes a `!` whose only use is a conditional jump, and flips the
sense of the jump. It runs in the TAC fixed-point loop as part of the
`fold_producer_jumps` `RuleSet`, together with the
[comparison jump fold](cmp_zero_jump_fold.md) and the
[AND jump fold](and_zero_jump_fold.md). It runs only under
`-O`.

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

The final code is `JSR ready; ORA #$00; BNE .if_end@0`. The `ORA`
sets Z from the returned byte, because the compiler never branches on
the flags a call leaves behind.

## When it applies

- A `Unary(!, src, %t)` is followed immediately by a `JumpIfTrue` or
  `JumpIfFalse` on `%t`.
- `%t` is used exactly once in the function, by that jump.

`src` can be any value of any width. A `JumpIfTrue` becomes a
`JumpIfFalse` on `src`, and a `JumpIfFalse` becomes a `JumpIfTrue`.
When `src` is itself a comparison or a masked byte, the
[comparison](cmp_zero_jump_fold.md) or [AND](and_zero_jump_fold.md)
fold can then fold the new jump on a later round. That is how
`if (!(flags & 0x80))`, with `flags` arriving in A, ends as `AND
#$80; BNE .if_end@0`.

## What it leaves alone

- A `!` whose result has another use. In
  `int t = !a; if (t) seen = 1; return t;` the run reports 0 changes,
  and the final code keeps the `.lnot_true@0` / `.lnot_end@1`
  diamond, then stores its 0 or 1 in `t` and branches on it with
  `BEQ`.
- A `!` that isn't adjacent to its jump, until copy propagation and
  dead-store elimination close the gap on a later round.

## A `!` of a value that is 0 or 1

`LNOT_OF_ZERO_OR_ONE_RULE`, in the same module and right after
`LNOT_RULE` in the rule set, rewrites a `!` that no jump tests when
its operand can only be 0 or 1. Then `!x` is `x ^ 1`, which
`tac_to_asm` lowers as `EOR #$01` where `!` branches to load 0 or 1.

```c
#include <stdint.h>
static bool flag;
extern uint8_t A [[c6502::address(0x5A00)]];
void f(void) { A = !flag; }
```

`Unary(!, flag, %0.1); Truncate(%0.1, A)` becomes `Binary(^, flag,
1uc, A)` once the truncate of the extend folds, and the code goes from
`LDA flag; BEQ .lnot_true@0; LDA #$00; BEQ .lnot_end@1; .lnot_true@0:
LDA #$01; .lnot_end@1: STA A` to `LDA flag; EOR #$01; STA A`, 6 bytes
less. That's the listing of two compiles, before and after the rule
was added.

An operand is 0 or 1 when it's an object whose type was `bool` (the
symbol's `was_bool`, which `erase_bool` in cpp/src/c99_to_tac.cpp sets
when it makes
the type `unsigned char`), or an SSA name defined by a comparison, a
`!`, or an extend or copy of such a value. The `^` is done at the
operand's width, a byte for a `bool`, and a `ZeroExtend` (or
`Truncate`) makes the `int` result. A `!` whose result is the
condition of a `JumpIfTrue` or `JumpIfFalse` is left to `LNOT_RULE`,
which drops it and flips the jump, and that's shorter than an `EOR`.

## Related

[Short-circuit jump fold](short_circuit_jump_fold.md) removes the
0-or-1 value of `&&` and `||` in front of a jump in the same way.
