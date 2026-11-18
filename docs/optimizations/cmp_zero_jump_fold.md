# Comparison and jump fold

`CMP_ZERO_RULE` in
[passes/optimization/cmp_zero_jump_fold.py](../../passes/optimization/cmp_zero_jump_fold.py)
fuses a comparison with the conditional jump that reads its result
into one direct conditional jump. It runs in the TAC fixed-point loop
as part of the `fold_producer_jumps` `RuleSet`, together with the
[AND jump fold](and_zero_jump_fold.md) and the
[logical-not jump fold](lnot_jump_fold.md). It runs only under
`-O`.

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

## A comparison kept as a value

`NARROW_VALUE_COMPARE_RULE`, in the same module and the same rule
set, narrows the operands of a comparison whose result isn't a jump's
condition, by the same rule as the ordering compares above. The
result keeps its width, since it is 0 or 1 at any width. It comes
after `CMP_ZERO_RULE` in the rule set, so a comparison followed by a
jump on its result becomes a `JumpIfCmp` first.

```c
#include <stdint.h>
uint8_t g, h;
static uint8_t small(uint8_t x) { return x < 10; }
int main(void) { h = small(g); return h; }
```

Without the rule, `small` compares two bytes with the overflow
correction:

```
small:
   LDA   __zpabi_small__x
   SEC
   SBC   #$0A
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   BMI   .cmp_true@1
   LDA   #$00
   JMP   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   RTS
```

With it, the compare is one unsigned byte:

```
small:
   LDA   __zpabi_small__x
   SEC
   SBC   #$0A
   BCC   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   RTS
```

## A jump folded before its extend was found

`NARROW_JUMP_COMPARE_RULE` narrows the operands of a `JumpIfCmp` the
same way. A comparison can become a `JumpIfCmp` while one of its
operands is still a `Copy` of the extend's result, and copy
propagation removes the `Copy` only later in the fixed point. `i <
40U` on a `uint8_t i` converts the promoted `int` to `unsigned int`
with such a `Copy`, and the loop had compared two bytes, `TXA; SEC;
SBC #$28; LDA #$00; SBC #$00; BCC`, where it now compares one, `CPX
#$28; BCC`. A `JumpIfCmp` has no result to find its position by, so
only an SSA-renamed extend source narrows here.

## When it applies

- A `Binary` with one of `==`, `!=`, `<`, `>`, `<=`, `>=` is followed
  immediately by a `JumpIfTrue` or `JumpIfFalse` on its result.
- The result is used exactly once in the function, by that jump.

Within a fold, the operands narrow in three ways.

- For `==` or `!=` against an integer constant zero, the other
  operand is followed back through any chain of `ZeroExtend`s, and
  the jump tests the narrowest source.
- Both operands narrow to one byte when one of them is the result
  of a `ZeroExtend` from a `char` or `unsigned char` variable, and
  the other is either the same kind of value or an integer constant
  from 0 to 255. The constant becomes a `ConstUChar`.
- For `>=` or `<` only, both operands narrow to one byte when one of
  them is the result of a `SignExtend` from a `signed char` variable
  and the other is a constant zero. Both comparisons test only the
  sign bit, which `SignExtend` keeps.

An extend's result that is SSA-renamed may have other uses. The
narrowed compare reads the source, and the extend stays for the other
uses until dead-store elimination finds none left. A `switch` on an
`unsigned char` compares its one extended value with every case, so
each case becomes `CMP #k; BEQ`. Without SSA names, as when the pass
runs on its own in a unit test, the extend's result must have only
the compare's use, because the pass finds the extend as the last
write of that name.

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
  `int t = a < 5; if (t) seen = 1; return t;` the final code
  materializes `t` with `LDA #$00` / `LDA #$01` and then tests it
  again with `ORA #$00; BEQ`. The value-compare rule still narrows
  the compare to `SEC; SBC #$05; BCC`.
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

## Lowering a 1-byte ordering against a constant

`tac_to_asm` lowers `x > y` and `x <= y` with their operands swapped,
as `y < x` and `y >= x`, because the 6502's compare sets C for `>=`.
Against a constant `k` that would load the constant into A, `LDA #k;
CMP x; BCC`, and leave `x` to be loaded again by whatever reads it
next. For a 1-byte `JumpIfCmp` the lowering first moves a constant on
the left to the right, mirroring the operator, and then rewrites `x >
k` as `x >= k + 1` and `x <= k` as `x < k + 1`
(`tac_to_asm._constant_on_right`). `if (c >= 'A' && c <= 'Z') c |=
0x20;` becomes `LDA c; CMP #$41; BCC end; CMP #$5B; BCS end; ORA
#$20`, with `c` in A throughout, where the second test had been `LDA
#$5A; CMP c; BCC end` and the `ORA` had needed `LDA c` before it. The
rewrite is skipped when `k` is the type's largest value, 255 or 127,
where `k + 1` wraps. Wider comparisons keep the swap, since their
chain of `SBC`s is as long either way.

## Lowering an unsigned ordering kept as a value

A comparison whose result is kept, rather than jumped on, is lowered
by `tac_to_asm._translate_unsigned_ordering` when it's unsigned: a
chain of `SBC`s, and `LDA #0; ROL A` to turn C into 0 or 1. The `SBC`
sets C when its left operand is at least its right, which answers `>=`
directly and `<` only inverted. For `a < b`, which `a > b` becomes by
swapping its operands, the subtract is `b - a - 1`, `LDA b; CLC; SBC
a`, whose carry is `b > a`, the answer itself. Before, it was `LDA a;
SEC; SBC b` and then `EOR #1` after the `ROL`. In a function that
returns its `bool` in C, [carry return](carry_return.md) then drops
the whole `LDA #0; ROL A; CMP #1`, so Lode Runner's high-score test

```c
static bool beats(struct hi_score *entry) {
    if (LEVELNUM != entry->level)
        return LEVELNUM > entry->level;
    ...
```

returns with `CLC; SBC __local_beats__0; RTS` where it had ended `SEC;
SBC LEVELNUM; LDA #$00; ROL; EOR #$01; CMP #$01; RTS`, 26 bytes
shorter in all.

## Related

[AND jump fold](and_zero_jump_fold.md) handles `if (x & mask)` and
[Logical-not jump fold](lnot_jump_fold.md) handles `if (!x)`. The
module docstring gives the bottom test of a
[rotated](loop_rotate.md) signed count-down loop as the case the
`signed char` narrowing was written for, where it leaves `DEC mem;
LDA mem; BPL .top`.
