# Jump threading on known compares

`thread_known_compares` in
[cpp/src/passes/optimization/known_compare_thread.h](../../cpp/src/passes/optimization/known_compare_thread.h)
sends a TAC jump past the equality tests at its target that the jump's
own path has decided. It runs under `-O`, after SSA destruction, in
the fixed point with [branching directly on `&&` and
`||`](short_circuit_jump_fold.md).

`if (x == a || x == b)` followed by a test of `x == a` inside the `if`
reaches the `if` by two jumps, one taken when x is a and one when x is
b. Each jump decides the inner test, so neither has to run it. The
first jump goes to the code after the test, and the second to the
test's target. Nothing else reaches the test then, and the pass drops
it.

A scan through the function follows what the conditional jumps it has
passed say about each Var (equal to a constant, or not equal to one),
until the Var is written, a label is reached, or an instruction that
may write other memory runs. A jump's edge adds what the jump itself
decides. From the jump's target, the pass follows labels, the tests
it can decide, and copies into locals that are dead where the walk
ends, and the jump goes to the furthest such point. A conditional
jump that the code before it decided becomes a `Jump`, or goes.

After threading, the pass drops the blocks that no jump names and no
code falls into. It then swaps two blocks when a conditional jump to
the second one is followed by a `Jump` and the first block ends in a
`Jump` to where the second falls through. The asm `branch_invert` then
makes the conditional jump and the `Jump` one branch.

The module docstring gives the conditions. Only `==` and `!=` with an
integer constant give facts or are decided, and a volatile Var is never
followed.

Lode Runner's `handle_timers` (examples/lode_runner/timers.c)
draws one of two brick sprites when a hole's timer is 20 or 10, which
is this shape. The function lost the inner compare and its branch, 4
bytes.

## Why on the TAC

The asm `branch_value_thread` follows what A holds and what the
branches it passed decided about bytes of memory, and it had already
sent the first `BEQ` past the inner test in `handle_timers`, where A
held a zero-page byte. It doesn't treat the fall-through of a branch
as an edge it can send elsewhere, and it loses track of A when A comes
from an indexed load such as `LDA T,X`, which leaves both tests in
place in the example below. In the TAC, both ways into the `if` are
jumps on the Var itself, whatever register or bytes hold it, and a
2-byte Var is one Var. So the pass is on the TAC.

## Example

```c
void draw(unsigned char s);
void other(void);
unsigned char T[8];
void f(void) {
    for (signed char i = 7; i >= 0; i--) {
        unsigned char timer = T[i];
        if (timer == 20 || timer == 10) {
            draw(timer == 20 ? 0x37 : 0x38);
            other();
        }
    }
}
```

### Before

```
   LDA   T,X
   CMP   #$14
   BEQ   .f@scfold@0
   CMP   #$0A
   BNE   .if_end@2
.f@scfold@0:
   CMP   #$14
   BNE   .cond_else@3
   LDA   #$37
   BNE   .cond_end@4
.cond_else@3:
   LDA   #$38
.cond_end@4:
```

### After

```
   LDA   T,X
   CMP   #$14
   BEQ   .f@ssa_block@4
   CMP   #$0A
   BNE   .if_end@2
   LDA   #$38
   BNE   .cond_end@4
.f@ssa_block@4:
   LDA   #$37
.cond_end@4:
```

The second jump into the `if`, taken when the timer is 10, went to
`.cond_else@3`. The swap put that block right after the `if`'s jump to
`.if_end@2`, and `branch_invert` made the two jumps the `BNE`.

## Limits

The walk from a target passes only labels, tests, and dead copies into
locals. A store or a call before the inner test stops it, as in `if (t
== 20 || t == 10) { sink = t; r = t == 20 ? ... }`, where moving the
store onto each edge would be needed.

[Shorter jumps](short_jumps.md) makes a `JMP` right after a
conditional branch the branch on the opposite condition. When the
threading leaves `BEQ .a; JMP .b` and the swap doesn't apply, that
turns the `JMP` into `BNE .b`.
