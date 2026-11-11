# Signed countdown loop rotation

`rotate_signed_countdown_loops` in
[passes/optimization/loop_rotate.py](../../passes/optimization/loop_rotate.py)
moves the test of a `for (T i = N; i >= 0; i--)` loop from the top
of the loop to the bottom. It runs once per function on the TAC,
before SSA construction, and only under `--optimize`. It runs before
SSA because the counter still has one name across the init, the
body, and the decrement there, so the rotation reorders instructions
without renaming anything.

`c99_to_tac` puts the test of a `for` loop at the top, so each
iteration runs the test, a conditional jump out of the loop, the
body, the decrement, and a `Jump` back to the test. When the counter
starts at a constant of zero or more, the first test always passes.
The pass drops that first test, moves the test after the decrement,
and replaces the jump out and the jump back with a single
`JumpIfTrue` to the top. A compile of the example below with the
pass disabled gives the loop `TXA; BMI .loop@0_break; LDA #$00; STA
buf,X; DEX; JMP .loop@0_start`, which is 12 bytes and takes 16 cycles
per iteration. The rotated loop is 8 bytes and takes 12 cycles per
iteration.

Inside the rotated body the counter only takes values from 0 to N.
When the body doesn't write the counter, the pass also rewrites each
`SignExtend` of the counter in the body to a `ZeroExtend`, since the
two give the same bytes for a non-negative value. `tac_to_asm`
lowers a `ZeroExtend` by storing a literal 0 in the high bytes, where
the lowering of a `SignExtend` uses a `BMI` and two loads to pick
`$00` or `$FF`.

## Example

```c
#include <stdint.h>
static uint8_t buf[16];
void clear(void) {
    for (int8_t i = 15; i >= 0; i--)
        buf[i] = 0;
}
```

### Before

```
Truncate(15, %0)
Copy(%0, @0.i)
Label(.loop@0_start)
SignExtend(@0.i, %1)
Binary(>=, %1, 0, %2)
JumpIfFalse(%2, .loop@0_break)
Truncate(0, %3)
SignExtend(@0.i, %4)
Truncate(%4, %5)
IndexedSymbolStore(buf, %5, %3)
Label(.loop@0_continue)
Copy(@0.i, %6)
Binary(-, @0.i, 1c, %7)
Copy(%7, @0.i)
Jump(.loop@0_start)
Label(.loop@0_break)
Ret(None)
```

### After

```
Truncate(15, %0)
Copy(%0, @0.i)
Label(.loop@0_start)
Truncate(0, %3)
ZeroExtend(@0.i, %4)
Truncate(%4, %5)
IndexedSymbolStore(buf, %5, %3)
Label(.loop@0_continue)
Copy(@0.i, %6)
Binary(-, @0.i, 1c, %7)
Copy(%7, @0.i)
SignExtend(@0.i, %1)
Binary(>=, %1, 0, %2)
JumpIfTrue(%2, .loop@0_start)
Label(.loop@0_break)
Ret(None)
```

The `SignExtend` in the moved test is left alone, because there the
counter can be -1. The final assembly is `LDX #$0F`, then the loop
`LDA #$00; STA buf,X; DEX; BPL .loop@0_start`.

## When it applies

The pass matches the layout `c99_to_tac` emits for a `for`
statement, with labels ending in `_start`, `_continue`, and `_break`.
It rotates the loop when all of these are true.

- The instructions between `_start` and the `JumpIfFalse` to `_break`
  are straight-line, and they compute the condition as
  `Binary(>=, x, 0)`, where `x` may reach the compare through
  `SignExtend`, `ZeroExtend`, or `Copy` temporaries and the 0 may
  reach it through extends, truncations, or copies.
- `x` is a `signed char`, `int`, `long`, or `long long`.
- The instructions between `_continue` and the `Jump` back to
  `_start` are straight-line and end with `Binary(-, x, 1, %t);
  Copy(%t, x)`, which is what `i--`, `--i`, and `i -= 1` lower to.
  The `_break` label follows the `Jump` directly.
- The last instruction before `_start` that writes `x` is a `Copy` or
  an extend or truncation that resolves, through at most four more
  such steps, to an integer constant between 0 and the largest
  positive value of `x`'s type.

The body may contain anything, including calls, nested loops,
`break`, and `continue`. The `SignExtend` to `ZeroExtend` rewrite is
made only when no instruction in the body writes `x`.

## What it leaves alone

- A loop whose counter starts at a value that isn't a compile-time
  constant, such as a parameter. The first test could fail, so it
  can't be dropped.
- A test other than `>= 0`, such as `i > 0` or `i != 0`.
- An unsigned or plain `char` counter, for which `i >= 0` is always
  true.
- A `while` loop, even one with the same test and a decrement at the
  end of its body. Its lowering puts the test at the `_continue`
  label and has no `_start` label, so the matcher never starts. A
  `do`/`while` loop already has its test at the bottom.
- A post expression other than a decrement by 1, such as `i -= 2`.

## Known problem

The init check looks at the last write to `x` that comes before the
loop in the instruction list. When that write sits on a conditional
path, a different value can reach the loop. In this function the
last write before the loop is `i = 5` inside the `if`, so the loop
is rotated.

```c
uint8_t count(int8_t i, uint8_t c) {
    uint8_t k = 0;
    if (c) i = 5;
    for (; i >= 0; i--)
        k++;
    return k;
}
```

`count(-1, 0)` returns 0 in the unoptimized build and 1 in the
optimized one, because the rotated loop runs its body once before
the first test.
