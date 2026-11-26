# An operation of a choice of constants

`FoldIntoConstantPhi` in
[cpp/src/passes/optimization/phi_constant_fold.h](../../cpp/src/passes/optimization/phi_constant_fold.h)
computes an operation on a `Phi` whose arguments are all constants in
the `Phi`'s arguments, so that the join picks the result instead of
the operand. It runs in the TAC fixed-point loop, right after
[constant folding](constant_folding.md), and only under `-O`.

`c ? 3 : 2` copies 3 or 2 into one temporary on two paths, and in SSA
form the join is a `Phi` of the two constants. Whatever the code does
with the temporary after the join, it does at run time on whichever
constant arrived. When that is a single operation with constant
operands, its result on each path is a constant too, and the compiler
can choose between the results.

## Example

```c
#include <stdint.h>
static bool flag;
extern uint8_t B [[c6502::address(0x5A01)]];
void f(void) { B = (flag ? 3 : 2) + 1; }
```

The listings compare the optimized TAC of two compiles, before and
after the pass was added.

### Before

```
JumpIfFalse(flag, .cond_else@0)
Copy(3, %2.3)
Jump(.cond_end@1)
Label(.cond_else@0)
Copy(2, %2.3)
Label(.cond_end@1)
Truncate(%2.3, %3.1.lo1)
Binary(+, %3.1.lo1, 1uc, B)
Ret(None)
```

### After

```
JumpIfFalse(flag, .cond_else@0)
Copy(4uc, B.p0)
Jump(.cond_end@1)
Label(.cond_else@0)
Copy(3uc, B.p0)
Label(.cond_end@1)
Copy(B.p0, B)
Ret(None)
```

The pass moves the `Truncate` into the `Phi` on one sweep and the add
on the next. The function's code goes from `LDA #$03 ... LDA #$02;
CLC; ADC #$01; STA B` to `LDA #$04 ... LDA #$03; STA B`, 3 bytes
less. Ali Baba's `configure_input` computes a menu setting this way.

## When it applies

- A `Phi` whose arguments are all integer constants defines an SSA
  name that exactly one instruction reads.
- That instruction is a `Unary`, a `Binary` whose other operand is a
  constant, or a `Truncate`.
- Constant folding folds the instruction for every one of the `Phi`'s
  constants. A divide by a constant 0 doesn't fold, and leaves the
  `Phi` alone.

The new `Phi` defines the instruction's destination when that is an
SSA name, and the instruction goes. Otherwise, as for the global `B`
above, the `Phi` defines a new temporary and the instruction becomes a
copy of it into the destination, so the store stays where it was.

The instruction reads the `Phi`'s result, so the `Phi`'s block
dominates it, and every read of the instruction's result comes after
it. The `Phi` computes the value on every path into its block, which
may be more paths than the instruction ran on, but the value is a
constant on each, and computing it has no effect.

## What it leaves alone

- A `ZeroExtend` or `SignExtend` of the `Phi`. It would only widen
  the join, and a join of a byte's constants is what other lowerings
  look for: Zork's `divu16` shifts a quotient bit of 0 or 1 in through
  the carry ([shifting a bit in through the carry](rotate_in.md)),
  which a 2-byte join of 1 and 0 lost.
- A `Phi` with a variable among its arguments, or whose result is
  read twice.

## Related

[Constant folding](constant_folding.md) folds a `Phi` whose arguments
are all the same constant into a copy.
