# Strength reduction

The three rules in
[passes/optimization/strength_reduction.py](../../passes/optimization/strength_reduction.py)
rewrite a multiply, divide, or modulo by a constant as shifts, masks,
adds, or copies. The pipeline runs them as one `RuleSet` named
`reduce_strength` in the TAC fixed-point loop, right after constant
folding. They run only under `--optimize`.

The 6502 has no multiply or divide instruction, so `tac_to_asm` lowers
`*`, `/`, and `%` to a call to a runtime helper (`mul16`,
`udivmod16`, and so on). Each call stages its operands through HARGS
and loops once per bit inside the helper, and every value live across
the call has to be kept somewhere the helper doesn't touch. A shift by
a small constant lowers inline as one `ASL` / `ROL` (or `LSR` / `ROR`)
per byte per bit, and a mask is one `AND` per byte, so the rewrite
removes the call and its staging.

## Example

```c
unsigned int scale(unsigned int x) { return x * 8; }
```

### Before

```
Label(.scale@ssa_block@0)
Binary(*, @0.x, 8u, %1.1)
Ret(%1.1)
```

### After

```
Label(.scale@ssa_block@0)
Binary(<<, @0.x, 3, %1.1)
Ret(%1.1)
```

The final code for `scale` is three `ASL` / `ROL` pairs on a copy of
`x` and no `JSR`.

The other rewrites, each checked by a run, are these.

- An unsigned divide by a power of two becomes a right shift, so
  `x / 4` becomes `Binary(>>, @0.x, 2, %1.1)`.
- An unsigned modulo by a power of two becomes a mask, so `x % 8`
  becomes `Binary(&, @0.x, 7u, %1.1)`.
- A multiply or divide by 1 becomes a `Copy` of the other operand,
  and a modulo by 1 becomes a `Copy` of a zero of the operand's type,
  as in `Copy(0u, %4.1)`.
- A multiply by `2^a + 1` or `2^a - 1` becomes one shift and an add
  or subtract of `x`. For `int x`, `x * 9` becomes
  `Binary(<<, @0.x, 3, %0.1.sr0); Binary(+, %0.1.sr0, @0.x, %0.1)`,
  and `x * 7` becomes the same shift followed by
  `Binary(-, %0.1.sr0, @0.x, %0.1)`.
- A multiply by `2^a ± 2^b` with `b > 0` shifts by `b` first and
  shifts that result by `a - b`, so both terms come from one chain.
  For `unsigned int y`, `y * 10` becomes the following.

  ```
  Binary(<<, @0.y, 1, %1.1.sr0)
  Binary(<<, %1.1.sr0, 2, %1.1.sr1)
  Binary(+, %1.1.sr1, %1.1.sr0, %1.1)
  ```

The two-term rewrite names its temporaries `<dst>.sr0` and
`<dst>.sr1`, registers them in the symbol table with the type of
`dst`, and adds them to the set of SSA names, so the later passes
treat them like any other SSA temporary.

## When it applies

- The operation is a `Binary` `Multiply`, `Divide`, or `Modulo` with
  exactly one integer `Constant` operand. For a multiply the constant
  may be on either side. For a divide or modulo it must be the
  divisor.
- The constant is positive. A power of two, including 1, takes the
  shift, mask, or copy form.
- A divide or modulo by a power of two above 1 has an unsigned
  dividend. The pass counts `unsigned char`, plain `char`, `unsigned
  int`, `unsigned long`, `unsigned long long`, and pointer variables
  as unsigned, along with the unsigned constant variants.
- A multiply by a constant that is not a power of two takes the
  two-term form when the constant equals `2^a + 2^b` or `2^a - 2^b`
  with `a > b >= 0`, the other operand is a variable, the result has
  an integer type, and `inline_shift_ok` in
  [passes/shift_cost.py](../../passes/shift_cost.py) accepts every
  shift in the rewrite at the result's width. A value that fits both
  forms (only 3) takes the sum.

The `inline_shift_ok` gate keeps the two-term rewrite from trading one
`mul16` call for an `asl16` call. It accepts a shift when it lowers to
at most 8 one-bit shift instructions after any whole-byte moves, or
when the count is 1, a multiple of 8, or of the form `8q + 7`. For a
16-bit value that means a shift by up to 4 bits.

## What it leaves alone

- A signed divide or modulo by a power of two. C99 rounds a quotient
  toward zero and an arithmetic right shift rounds toward negative
  infinity, so `-3 / 2` is -1 where `-3 >> 1` is -2. The sign of a
  remainder follows the dividend, and a mask can't produce a negative
  result. A run on `int x` with `x / 2` and `x % 4` reports 0
  changes.
- A two-term multiply whose shifts would call a helper. For `unsigned
  int x`, `x * 96` is `64 + 32`, and the first shift by 5 bits of a
  16-bit value takes 10 shift instructions, so the run reports 0
  changes and the final code keeps `JSR mul16`.
- A multiply by a constant with more than two set bits in either
  form, such as `x * 11`, which keeps its `mul16` call.
- A multiply by zero or a negative constant. Constant folding handles
  the case where both operands are constants, but no TAC pass folds
  `x * 0` with a variable `x`, so `return x * 0;` still compiles to a
  `JSR mul16`.
- An operation whose operands are both constants, which constant
  folding evaluates.

The single power-of-two rewrite has no shift-cost gate. For `unsigned
int x`, `x * 64` becomes `x << 6`, which `tac_to_asm` lowers to a
`JSR asl16` because a 6-bit shift of 16 bits takes 12 shift
instructions inline. That still replaces a multiply loop with a shift
loop, which is the cheaper helper.

## Related

[Constant folding](constant_folding.md) evaluates the cases with two
constant operands, and [narrowing of widened arithmetic](narrow_widened_arith.md)
can shrink a promoted multiply to one byte before this pass sees it.
