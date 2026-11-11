# Comparison and jump fold

`CMP_ZERO_RULE` in
[passes/optimization/cmp_zero_jump_fold.py](../../passes/optimization/cmp_zero_jump_fold.py)
fuses a comparison with the conditional jump that reads its result
into one direct conditional jump. It runs in the TAC fixed-point loop
as part of the `fold_producer_jumps` `RuleSet`, together with the
[AND jump fold](and_zero_jump_fold.md) and the
[logical-not jump fold](lnot_jump_fold.md). It runs only under
`--optimize`.

`c99_to_tac` lowers `if (a == 0)` as a `Binary(==, ...)` that writes
0 or 1 into a temporary, followed by a `JumpIfFalse` on that
temporary. `tac_to_asm` lowers the `Binary` as a compare chain that
loads `#0` or `#1` into the temporary, and the jump then reloads the
temporary and branches on it, about 13 instructions in all according
to the module docstring. After the fold, a test against zero is a load
and a branch on the value itself. Any other comparison becomes a
`JumpIfCmp`, which `tac_to_asm` lowers as a compare chain that ends in
the branch, with no 0-or-1 value in between.

## Example

```c
#include <stdint.h>
uint8_t hits;
void check(uint8_t a) {
    if (a == 0) hits = 1;
}
```

### Before

```
Label(.check@ssa_block@0)
ZeroExtend(@0.a, %0.1)
Binary(==, %0.1, 0, %1.1)
JumpIfFalse(%1.1, .if_end@0)
...
```

### After

```
Label(.check@ssa_block@0)
ZeroExtend(@0.a, %0.1)
JumpIfTrue(@0.a, .if_end@0)
...
```

The fold follows the test back through the `ZeroExtend`, so the jump
reads the 1-byte `a` and the final code is `LDA __zpabi_check__a; BNE
.if_end@0`. Dead-store elimination drops the `ZeroExtend` on a later
round. An `== 0` flips the sense of the jump and a `!= 0` keeps it.

The other comparisons, each checked by a run, become a `JumpIfCmp`.
Its operator is inverted when the original jump was a `JumpIfFalse`,
so the `JumpIfCmp` always jumps when its operator holds.

- For `uint8_t a`, `a < 105` becomes `JumpIfCmp(>=, @0.a, 105uc,
  .if_end@0)`. Both operands narrow to one unsigned byte, and the
  final code is `LDA __zpabi_check__a; CMP #$69; BCS .if_end@0`.
- For `uint8_t a`, `a != 5` becomes `JumpIfCmp(==, @0.a, 5uc,
  .if_end@0)`, which lowers to `LDA __zpabi_check__a; CMP #$05; BEQ .if_end@0`.
- For `signed char a`, `a >= 0` becomes `JumpIfCmp(<, @0.a, 0c,
  .if_end@0)`, which lowers to `LDA __zpabi_check__a; BMI .if_end@0`.
- For `int a, b`, `a < b` becomes `JumpIfCmp(>=, @0.a, @1.b,
  .if_end@0)` at full width, which lowers to a 2-byte `SBC` chain
  with the overflow correction and one `BPL`.

## When it applies

- A `Binary` with one of `==`, `!=`, `<`, `>`, `<=`, `>=` is followed
  immediately by a `JumpIfTrue` or `JumpIfFalse` on its result.
- The result is used exactly once in the function, by that jump.

Within a fold, the operands narrow in three ways.

- For `==` or `!=` against an integer constant zero, the other
  operand is followed back through any chain of `ZeroExtend`s whose
  results each have one use, and the jump tests the narrowest source.
- Both operands narrow to one byte when one of them is the
  single-use result of a `ZeroExtend` from a `char` or `unsigned
  char` variable, and the other is either the same kind of value or
  an integer constant from 0 to 255. The constant becomes a
  `ConstUChar`.
- For `>=` or `<` only, both operands narrow to one byte when one of
  them is the single-use result of a `SignExtend` from a `signed
  char` variable and the other is a constant zero. Both comparisons
  test only the sign bit, which `SignExtend` keeps.

Each narrowing reads the extend's source variable at the compare in
place of the extend's result, so the source must hold the same value
at both points. That is so when the source is an SSA-renamed name,
which has one write. For any other source, such as a static or an
address-taken local, the extend must sit earlier in the same
straight-line run as the compare, with no label, jump, store, or call
between them and no instruction between them that writes the source.
The extend's own result must be SSA-renamed too.

## What it leaves alone

- A comparison whose result has another use. In
  `int t = a < 5; if (t) seen = 1; return t;` the run reports 0
  changes, and the final code materializes `t` with `LDA #$00` /
  `LDA #$01` and then tests it again with `ORA #$00; BEQ`.
- A comparison that isn't adjacent to its jump. Copy propagation and
  dead-store elimination usually close the gap, and the fold fires on
  a later round of the fixed point.
- Narrowing of `>` or `<=` against zero for a `signed char`, which
  would take a second branch for the zero case. These stay at `int`
  width.
- Narrowing against a constant outside 0 to 255, or of an operand
  whose `ZeroExtend` result has other uses. The comparison still
  becomes a `JumpIfCmp`, at the promoted width.
- A floating-point comparison against `0.0`. The zero test accepts
  only integer constants, so the comparison becomes a `JumpIfCmp`
  instead.
- Narrowing through an extend of a static that is written before the
  compare. For a static `uint8_t g`, `int t = g; g = 5; int c = t <
  3; if (c) hits = 1;` keeps the 2-byte compare of `t`, which holds
  the old value of `g`. The `== 0` trace stops at the same extend.

## Related

[AND jump fold](and_zero_jump_fold.md) handles `if (x & mask)` and
[Logical-not jump fold](lnot_jump_fold.md) handles `if (!x)`. The
module docstring gives the bottom test of a
[rotated](loop_rotate.md) signed count-down loop as the case the
`signed char` narrowing was written for, where it leaves `DEC mem;
LDA mem; BPL .top`.
