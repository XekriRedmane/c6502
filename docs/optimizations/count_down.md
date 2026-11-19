# A loop counted down to 0

`CountLoopsDown` in
[passes/optimization/count_down.py](../../passes/optimization/count_down.py)
makes a loop whose byte counter is read only by its own test and its
own step count the passes left, down to 0. It runs once after the TAC
fixed point, in SSA form. It doesn't run in the unoptimized pipeline.

A count up to `n` ends each pass with `INC i; LDA i; CMP #n; BCC`.
A count down ends it with `DEC i; BNE`, since `DEC` sets Z, which is 3
bytes and 2 cycles a pass fewer. In Ali Baba, `print_panel_name` went
from 53 bytes to 50, `delay_with_animation` from 16 to 12, and
`hrcg_fill_line` from 47 to 40.

## Example

This listing compares two complete compiles.

```c
#include <stdint.h>
void out(uint8_t c);
void f(void) { for (uint8_t i = 0; i < 20; i++) out(0xA0); }
```

### Before

```
   LDA   #$00
   STA   __local_f__i
.f@rot@0:
   LDA   #$A0
   JSR   out
   INC   __local_f__i
   LDA   __local_f__i
   CMP   #$14
   BCC   .f@rot@0
   RTS
```

### After

```
   LDA   #$14
   STA   __local_f__i
.f@rot@0:
   LDA   #$A0
   JSR   out
   DEC   __local_f__i
   BNE   .f@rot@0
   RTS
```

The pass rewrites the loop's `Phi`, its test, and its step, and keeps
their names:

```
Phi(@1.i.2, [(.f@ssa_block@0, 0uc), (.loop@0_continue, %5.1)])
JumpIfCmp(>=, @1.i.2, 20uc, .loop@0_break)
FunctionCall(out, [160uc], None)
Label(.loop@0_continue)
Binary(+, @1.i.2, 1uc, %5.1)
```

becomes

```
Phi(@1.i.2, [(.f@ssa_block@0, 20uc), (.loop@0_continue, %5.1)])
JumpIfCmp(==, @1.i.2, 0uc, .loop@0_break)
FunctionCall(out, [160uc], None)
Label(.loop@0_continue)
Binary(-, @1.i.2, 1uc, %5.1)
```

The [loop test at the bottom](rotate_loop_test.md) then moves the test
next to the step, where the peepholes drop the load between the `DEC`
and the `BNE`.

## When it applies

- The counter is a `Phi` of an unsigned byte with two arguments, a
  constant `c0` and the result of `Binary(+, counter, 1)`, which only
  the `Phi` reads.
- The counter is read by that add and by one `JumpIfCmp`, the first
  instruction after the `Phi`s of its block, and by nothing else.
- The jump compares the counter with a bound, by `>=` or `==` to leave
  the loop or by `<` or `!=` to stay in it, with the counter on either
  side.
- The bound is a constant `c1` with `c0 <= c1`, or `c0` is 0 and the
  bound is a variable that SSA construction renamed, or a parameter,
  defined in a block that strictly dominates the test's, so the loop
  doesn't change it.
- The block the loop stays in after the test has the test's block as
  its only predecessor, and it dominates the add's block.

Every pass through the add then comes from the test with `i < n`, so
the counter goes from `c0` up to at most `n` and never wraps, and the
tests of `i >= n` and of `n - i == 0` take the same branch. The
counter holds `n - i` after the rewrite, which starts at `c1 - c0` (or
the bound) and drops by 1 at each step.

## What it leaves alone

- A counter read in the loop's body or after it, such as an index.
- A variable bound with a start other than 0. With a bound below the
  start, `n - i` would wrap, where the original test leaves at once.
- A counter wider than a byte, whose count down isn't a single `DEC`.
