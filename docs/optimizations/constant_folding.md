# Constant folding

`ConstantFold` (`constant_fold`) in
[passes/optimization/constant_folding.py](../../passes/optimization/constant_folding.py)
evaluates TAC instructions whose operands are all constants and
replaces each one with a `Copy` of the result. It runs first in each
sweep of the TAC fixed-point loop, after SSA construction, and only
under `--optimize`.

The pass looks at one instruction at a time. A `Unary`, a `Binary`, or
an integer or floating-point conversion whose operands are all
`Constant`s becomes `Copy(Constant(result), dst)`. A `JumpIfTrue`,
`JumpIfFalse`, or `JumpIfCmp` whose outcome is known becomes a `Jump`
or is dropped. A `Phi` whose arguments all name the same value becomes
a `Copy` of it. An `IndexedLoad` from a `const` static array at a
constant index becomes a `Copy` of the element's initial value. Copy
propagation then forwards each new constant into its readers, so the
next sweep folds the instruction after it, and a chain of constant
arithmetic collapses one step per sweep.

The arithmetic matches what the 6502 lowering computes. Each result
wraps at the width of its operands' constant variant, so a `Truncate`
of `960` to `unsigned char` gives `192`. Signed `/` and `%` truncate
toward zero as C99 §6.5.5 requires, a signed `>>` shifts in copies of
the sign bit the way `asr16` does, and comparisons are signed or
unsigned according to the operands' variant. Floating-point operations
round to nearest at the operand's precision through `fp_arith`. The
payoff is largest for `*`, `/`, `%`, and shifts by a variable count,
which the 6502 has no instructions for. Each of those becomes a
`JSR` to a runtime helper with its operands staged through HARGS, and
a folded one costs nothing at run time.

## Example

```c
#define WIDTH 40
#define ROWS 24
uint8_t limit(uint8_t x) {
    if (WIDTH * ROWS > 900)
        return x + (uint8_t)(WIDTH * ROWS);
    return x - 1;
}
```

The first sweep's call folded both copies of `40 * 24` to `Copy(960,
...)`. Copy propagation and the comparison-jump fold then turned the
`if` into a `JumpIfCmp` of two constants, which the second call folds.

### Before

```
JumpIfCmp(<=, 960, 900, .if_end@0)
Truncate(960, %4.1)
Binary(+, @0.x, %4.1, %7.1)
Ret(%7.1)
Label(.if_end@0)
Binary(-, @0.x, 1uc, %10.1)
Ret(%10.1)
```

### After

```
Copy(192uc, %4.1)
Binary(+, @0.x, %4.1, %7.1)
Ret(%7.1)
Label(.if_end@0)
Binary(-, @0.x, 1uc, %10.1)
Ret(%10.1)
```

The jump is never taken, so the pass drops it, and
[unreachable code elimination](unreachable_code_elimination.md) then
removes the `x - 1` path. The function compiles to `LDA x; CLC; ADC
#$C0; RTS`. The unoptimized compile of the same source calls `mul16`
twice.

The const-array-subscript fold (`_fold_indexed_load`) works the same
way. With `static const uint16_t steps[4] = {100, 250, 600, 1500};`,
`return base + steps[2];` reaches the pass as `IndexedLoad(steps,
4uc, %2.1)` once the byte index has folded, and the pass rewrites it
to `Copy(600u, %2.1)`. The add then reads `#$58` and `#$02` as
immediates, and nothing references `steps` any more, so its storage
is dropped from the output.

## When it applies

- A `Unary` or `Binary` with every value operand a `Constant`. For
  integer arithmetic and bitwise operations the two constants must
  have the same variant, or the same width with different
  signedness, in which case both are treated as unsigned. Comparisons
  fold only when both constants have the same variant. A comparison
  and a `!` each yield an `int` 0 or 1.
- A `SignExtend`, `ZeroExtend`, `Truncate`, or floating-point
  conversion of a `Constant`. The integer casts and the
  floating-to-integer conversions take their result type from the
  symbol table's entry for the destination.
- A `Copy` of an integer `Constant` whose variant differs in
  signedness from the destination's type, such as `ConstInt(1)` into
  an `unsigned int`. The constant is rewrapped in the destination's
  variant with the same bit pattern.
- A `JumpIfTrue` or `JumpIfFalse` on a `Constant`, and a `JumpIfCmp`
  of two `Constant`s. A floating-point `-0.0` counts as false and a
  NaN as true, per C99 §6.3.1.2.
- A `Phi` whose arguments all have the same source, which includes a
  `Phi` with only one argument.
- A non-volatile `IndexedLoad(name, Constant(i), dst)` where `name` is
  a static array with a constant initializer list, the array's leaf
  element type is `const` and not `volatile`, `i` is a multiple of the
  element size and inside the array, the element's initial value is a
  number, and the destination's type has the element's width. This
  walks nested arrays too, so `grid[1][2]` of a `const` 2-D array
  folds.

## What it leaves alone

- An integer `/` or `%` by zero. The behavior is undefined, so the
  pass keeps the helper call. A floating-point division by zero does
  fold, to an infinity or a NaN.
- A shift whose count is negative or at least the width of the
  shifted value, since C99 §6.5.7 leaves the result undefined.
  `1 << 16` on an `int` stays a call to `asl16`.
- `/` and `%` whose constants differ in signedness, and comparisons
  whose constants differ in variant, because the result depends on
  which interpretation wins.
- A floating-point to integer conversion of a NaN or an infinity.
- An `IndexedLoad` from an array without `const` elements, from a
  `volatile` array, or from an array whose element is an address
  (`AddressInit`), whose value isn't known until link time. A plain
  `static uint16_t steps[4]` keeps its `ADC steps+4`.
- A `Phi` whose arguments are different constants, since its value
  depends on the path taken.
- Anything whose operand is a variable. The pass doesn't look up
  definitions, so a constant reaches an operand only after
  [copy propagation](copy_propagation.md) puts it there.

## Related

[Static const reads](static_const_fold.md) turns reads of `static
const` scalars into constants before the fixed point starts.
[Strength reduction](strength_reduction.md) handles a multiply or
divide where only one operand is a constant.
