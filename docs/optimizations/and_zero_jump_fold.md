# AND jump fold

`AND_ZERO_RULE` in
[passes/optimization/and_zero_jump_fold.py](../../passes/optimization/and_zero_jump_fold.py)
turns a bit test of a byte, `if (flags & mask)`, into a
`JumpIfMasked` that tests the byte directly. It runs in the TAC
fixed-point loop as part of the `fold_producer_jumps` `RuleSet`,
together with the [comparison jump fold](cmp_zero_jump_fold.md) and
the [logical-not jump fold](lnot_jump_fold.md). It runs only under
`--optimize`.

C99 promotes an `unsigned char` operand of `&` to `int`, so
`c99_to_tac` emits a `ZeroExtend`, a 2-byte `BitwiseAnd`, and a jump
on the 2-byte result. `tac_to_asm` lowers that as an `AND` of each
byte into a temporary and an `ORA` of the two bytes before the
branch. The high byte of the result is always zero, because the
source is one byte and the mask fits in one. `JumpIfMasked` lowers to
`LDA val; AND #mask; BEQ` or `BNE`, and with a mask of 0x80 the
[AND sign-bit branch](and_sign_bit_branch.md) peephole later shortens
that to `LDA val; BPL` or `BMI`.

## Example

```c
#include <stdint.h>
uint8_t hits;
void check(uint8_t flags) {
    if (flags & 0x40) hits = 1;
}
```

### Before

```
Label(.check@ssa_block@0)
ZeroExtend(@0.flags, %0.1)
Binary(&, %0.1, 64, %1.1)
JumpIfFalse(%1.1, .if_end@0)
...
```

### After

```
Label(.check@ssa_block@0)
ZeroExtend(@0.flags, %0.1)
JumpIfMasked(@0.flags, 64, False, .if_end@0)
...
```

The third field of `JumpIfMasked` is `jump_when_nonzero`. A
`JumpIfTrue` becomes `True` and a `JumpIfFalse` becomes `False`. The
final code is `LDA __zpabi_check__flags; AND #$40; BEQ .if_end@0`,
where the same test on an `int flags` takes 9 instructions, with two
stores to temporaries and an `ORA`.

For `if (!(flags & 0x80))`, the logical-not fold first removes the
`!` and flips the jump to `JumpIfTrue`, and this rule then gives
`JumpIfMasked(@0.flags, 128, True, .if_end@0)`. The final code is
`LDA __zpabi_check__flags; BMI .if_end@0`.

## When it applies

- A `Binary(&, ...)` is followed immediately by a `JumpIfTrue` or
  `JumpIfFalse` on its result, and that result has no other use.
- One operand is an integer constant from 0 to 0xFF. Either operand
  order matches.
- The other operand is the result of a `ZeroExtend` that has no other
  use, and the `ZeroExtend` reads a variable of type `char` or
  `unsigned char`.
- The `ZeroExtend`'s source holds the same value at the AND as at
  the extend, since the `JumpIfMasked` reads the source at the AND's
  position. An SSA-renamed source has one write and always qualifies.
  Any other source, such as a static, qualifies when the extend sits
  earlier in the same straight-line run, with no label, jump, store,
  call, or write to the source in between. The extend's result must
  be SSA-renamed too. The check is `extend_source_unchanged` in
  `cmp_zero_jump_fold.py`, which the comparison fold shares.
- A symbol table is present, since the pass reads the source's type
  from it. The optimizer always passes one.

## What it leaves alone

- An `int` operand with no `ZeroExtend` in front of it. A run on
  `int flags` with `flags & 0x40` reports 0 changes.
- A `signed char` operand, which C99 promotes with a `SignExtend`.
  A run on `signed char flags` reports 0 changes, and the final code
  keeps the sign-extension branch, two `AND`s, and an `ORA`.
- A mask above 0xFF, or a negative one.
- An AND whose result is also stored or returned, or whose
  `ZeroExtend` result has another use.
- An AND that isn't adjacent to its jump, until copy propagation and
  dead-store elimination close the gap on a later round.
- An AND through an extend of a static that is written in between.
  For a static `uint8_t g`, `int t = g; g = 0; if (t & 1) hits = 1;`
  keeps the 2-byte AND of `t`, which holds the old value of `g`.

## Related

[Comparison jump fold](cmp_zero_jump_fold.md) handles `==`, `<`, and
the other comparisons, and [Logical-not jump fold](lnot_jump_fold.md)
removes a `!` in front of the test.
