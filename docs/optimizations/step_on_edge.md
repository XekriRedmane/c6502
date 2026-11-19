# A step moved onto its edge

`MoveStepsOntoEdges` in
[passes/optimization/step_on_edge.py](../../passes/optimization/step_on_edge.py)
moves an add or subtract of a constant, whose result reaches its
variable only through a copy on one edge out of the block, into the
place of that copy. It runs once after SSA destruction, right after
the post-destruction copy folding. It doesn't run in the unoptimized
pipeline.

A post-decrement in a loop's condition, `while (i-- != first)`,
computes `i - 1` before the test reads the old `i`. SSA destruction
puts the copy of the new value into `i` on the loop's back edge, so
both values are live at the test, and the new one takes a byte of its
own. With the step in place of the copy, the step is `i = i - 1` in
place, which the [DEC peephole](dec_peephole.md) makes `DEC i`. Ali
Baba's `draw_border_strings` went from 44 bytes to 35 and
`move_border_icons` from 70 to 61.

## Example

```c
#include <stdint.h>
void g(uint8_t i);
void f(uint8_t first, uint8_t last) {
    uint8_t i = last;
    do g(i); while (i-- != first);
}
```

### Before

```
Binary(-, @3.i.2, 1uc, %1.1)
JumpIfCmp(!=, @3.i.2, @1.first, .f@ssa_split@1)
Ret(None)
Label(.f@ssa_split@1)
Copy(%1.1, @3.i.2)
Jump(.loop@0_start)
```

### After

```
JumpIfCmp(!=, @3.i.2, @1.first, .f@ssa_split@1)
Ret(None)
Label(.f@ssa_split@1)
Binary(-, @3.i.2, 1uc, @3.i.2)
Jump(.loop@0_start)
```

The loop's step and test were `LDA i; SEC; SBC #1; STA t; LDA i; CMP
first; BNE more; RTS; more: LDA t; STA i; JMP loop`, 21 bytes, and are
now `LDA i; CMP first; BNE more; RTS; more: DEC i; JMP loop`, 12.

## When it applies

- The step is a `Binary` add, subtract, AND, OR, or XOR of a variable
  `X` and a constant into `T`, where SSA construction renamed both
  `X` and `T`. No store through a pointer and no call can write such
  a name.
- `T` is written by the step alone and read by one `Copy(T, Z)` alone.
- No instruction from the step to the end of its block writes `X`,
  and the block ends at a jump.
- The `Copy` is in the block the jump goes to, or in the block it
  falls through to, and nothing before it there writes `X` or is a
  label or a jump. A block reached through a label must have that
  jump as its only way in: no other jump names the label, and the
  instruction before the label is a jump or a return.

`X` then holds on the way to the `Copy` the value the step read, so
the step at the `Copy` writes `Z` the same value. Every other path
loses a computation whose result nothing read.

## What it leaves alone

- A step whose result is read anywhere besides the one `Copy`, such as
  `return i--;`.
- A static `X`, which a call between the step and the copy could
  change.
- A copy in a block that other paths reach too, where the step would
  run on paths that never computed it.

## Related

[Sinking increments](sink_increment.md) moves a step down within its
block, and tests the new value in place of the old one when the
comparison is with a constant. This pass covers a comparison with a
variable, which can't be moved by the step's constant.
