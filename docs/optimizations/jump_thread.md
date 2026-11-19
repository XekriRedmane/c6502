# Jump threading on a constant

`ThreadConstantJumps` in
[passes/optimization/jump_thread.py](../../passes/optimization/jump_thread.py)
sends an edge into a join straight to the place the join's test sends
the constant that the edge brings. It runs in the TAC fixed-point
loop, right before [unreachable code
elimination](unreachable_code_elimination.md), and only under `-O`.

A function that returns a constant on some of its paths, once it's
[inlined](inline_functions.md) into a caller that tests the result,
puts each path's constant in one temporary and tests it after the
paths meet. In SSA form the meeting point is a `Phi` of the
constants, followed by a jump on the `Phi`'s result. Each path into
it already decides that jump, so the test is work done at run time
for an answer the compiler has. `&&` and `||` whose value is tested
leave the same shape, with a 0 or a 1 from each path.

## Example

```c
#include <stdint.h>
#include <stdbool.h>
uint8_t a[8], n, hits;
static bool find(uint8_t v) {
    uint8_t i = n;
    do { if (a[i] == v) { hits++; return true; } } while (--i != 0);
    return false;
}
void f(uint8_t v) { if (find(v)) return; hits += 2; }
```

The listings compare two compiles, with the pass turned off and on.
The TAC is the optimizer's output, after SSA destruction. Before it,
the `return true` path copied 1 into `%11.3` and the `return false`
path copied 0, and `.i0@end` tested the copy.

### Before

```
.i0_loop@0_start:
   IndexedLoad(a, @4.i.2, %17.1)
   JumpIfCmp(!=, %17.1, @2.v, .i0_if_end@0)
.f@ssa_block@1:
   Binary(+, hits, 1uc, hits)
   Copy(1uc, %11.3)
   Jump(.i0@end)
.i0_if_end@0:
.i0_loop@0_continue:
   Binary(-, @4.i.2, 1uc, %23.1)
   Copy(%23.1, @4.i.2)
   JumpIfTrue(%23.1, .i0_loop@0_start)
.i0_loop@0_break:
   Copy(0uc, %11.3)
.i0@end:
   JumpIfFalse(%11.3, .if_end@1)
   Ret(None)
.if_end@1:
   Binary(+, hits, 2uc, hits)
   Ret(None)
```

```
   LDX   n
.i0_loop@0_start:
   CMP   a,X
   BNE   .i0_if_end@0
   INC   hits
   RTS
.i0_if_end@0:
   DEX
   BNE   .i0_loop@0_start
   LDA   #$00
   BEQ   .if_end@1
   RTS
.if_end@1:
   LDA   hits
   CLC
   ADC   #$02
   STA   hits
   RTS
```

### After

```
.i0_loop@0_start:
   IndexedLoad(a, @4.i.2, %17.1)
   JumpIfCmp(!=, %17.1, @2.v, .i0_if_end@0)
   Binary(+, hits, 1uc, hits)
   Jump(.f@jthread@0)
.i0_if_end@0:
.i0_loop@0_continue:
   Binary(-, @4.i.2, 1uc, %23.1)
   Copy(%23.1, @4.i.2)
   JumpIfTrue(%23.1, .i0_loop@0_start)
   Jump(.if_end@1)
.f@jthread@0:
   Ret(None)
.if_end@1:
   Binary(+, hits, 2uc, hits)
   Ret(None)
```

```
   LDX   n
.i0_loop@0_start:
   CMP   a,X
   BNE   .i0_if_end@0
   INC   hits
   RTS
.i0_if_end@0:
   DEX
   BNE   .i0_loop@0_start
   LDA   hits
   CLC
   ADC   #$02
   STA   hits
   RTS
```

The edge that brought 1 goes to the `Ret` the test would have fallen
through to, under a new label, and the edge that brought 0 goes to
`.if_end@1`. Nothing reaches `.i0@end` after that, and unreachable
code elimination removes it with its test. The asm peepholes then
make the first `Jump` an `RTS` and the second a fall-through, and the
function is 5 bytes shorter. In Lode Runner's `handle_timers`, where
`fill_hole` and `kill_guard_in_hole` are inlined, the pass removes the
`LDA #$00; BNE .loop@3_continue` that the `return false` path had
ended with.

## When it applies

- A block holds its label, one `Phi`, and a `JumpIfTrue` or
  `JumpIfFalse` on the `Phi`'s result, which is an SSA name that
  nothing else reads.
- An argument of the `Phi` is an integer constant, from a predecessor
  with one edge into the block. The constant decides the jump, and the
  edge goes to the successor it picks, which mustn't be the block
  itself, or the end of the function.
- The predecessor has no edge to that successor already, and the new
  edge doesn't enter a loop's body anywhere but at its header.

The predecessor's jump into the block goes to the successor instead,
or a `Jump` to the successor goes at the predecessor's end when it
fell through. When the predecessor is empty, or holds only a `Jump`
to the block, and every way into it is a jump, as for the
`.and_false@N` and `.or_true@N` labels of `&&` and `||`, the jumps into
it go to the successor, and it's left with no way in. Each `Phi` at
the successor that took a value from the block takes the same value
from the block whose edge moved.

The test reads only the `Phi`'s result, which is the constant on the
moved edge, so control leaves the block for the same successor
whatever else holds. The block computes nothing else, so skipping it
skips no effect. A value that the successor's `Phi` took from the
block is defined in a block that dominates it, and that block
dominates the moved edge's source too. The module docstring has the
argument, and the one that the graph stays reducible.

## Why not at the asm level

[Branch threading on known values](branch_value_thread.md) follows a
branch or `JMP` to its target and past the loads and branches there
that its edge decides. The `LDA #$00; BNE` that `handle_timers` had
was reached by falling through from `DEC; BNE`, which isn't an edge
that pass moves, so it stayed. Removing the test in the TAC also
leaves a simpler graph for every pass after it. Most tested `&&` and
`||` values are gone before SSA destruction now, so [the short-circuit
fold](short_circuit_jump_fold.md), which runs after it, finds fewer of
them.
