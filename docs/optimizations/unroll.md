# Loop unrolling

`unroll_program` in
[passes/optimization_ast/unroll.py](../../passes/optimization_ast/unroll.py)
replaces each `for` loop marked with `#pragma c6502 loop
unroll(enable)` by one copy of its body per iteration. It runs once,
on the C AST, after parsing and before identifier resolution. It runs
only under `--optimize`, and without that flag the pragma has no
effect and the loop compiles as written.

The preprocessor turns the pragma into a token that the parser
attaches to the next `for` statement. The pass works out the values
the induction variable takes by stepping it from its initial value
until the condition fails, then emits the body once per value, each
copy in its own block, with every read of the induction variable
replaced by that iteration's value as an integer constant. The copies
are made before identifier resolution, so each copy's locals get
their own unique names from the usual renaming, and the type checker
and TAC passes treat the substituted values like constants written in
the source.

Each copy loses the loop's compare, branch, increment, and jump, and
a constant index lets an array access use the absolute address of the
element instead of an indexed one. The cost is code size, which grows
with the iteration count.

## Example

```c
uint8_t row[4];
void fill(uint8_t v) {
#pragma c6502 loop unroll(enable)
    for (uint8_t i = 0; i < 4; i++)
        row[i] = v + i;
}
```

The pass changes the AST, which has no printed form in the pass
tracing script, so the listings below are the output of `compile.py
--tac --optimize` on the same source without the pragma and with it,
printed one instruction per line.

### Before

```
Label(.fill@ssa_block@0)
Copy(0uc, @1.i.2)
Label(.loop@0_start)
JumpIfCmp(>=, @1.i.2, 4uc, .loop@0_break)
Binary(+, @0.v, @1.i.2, %6.1)
IndexedSymbolStore(row, @1.i.2, %6.1)
Label(.loop@0_continue)
Binary(+, @1.i.2, 1uc, @1.i.2)
Jump(.loop@0_start)
Label(.loop@0_break)
Ret(None)
```

### After

```
Binary(+, @0.v, 0uc, %3.1)
IndexedSymbolStore(row, 0uc, %3.1)
Binary(+, @0.v, 1uc, %9.1)
IndexedSymbolStore(row, 1uc, %9.1)
Binary(+, @0.v, 2uc, %15.1)
IndexedSymbolStore(row, 2uc, %15.1)
Binary(+, @0.v, 3uc, %21.1)
IndexedSymbolStore(row, 3uc, %21.1)
Ret(None)
```

In the final assembly each copy is `LDA v; CLC; ADC #k; STA row+k`.
Not counting the `RTS`, the unrolled body is 32 bytes and takes 44
cycles, where the loop is 25 bytes and takes 137 cycles. The first copy keeps its `ADC
#$00`, because no pass in either pipeline removes an add of zero.

## When it applies

The pass unrolls a marked loop that has this shape, and reports an
`UnrollError` for a marked loop that doesn't.

- The init clause declares exactly one induction variable, of a
  character or integer type, with no storage class other than
  `register`, initialized to an integer constant.
- The condition is `i < B`, `i <= B`, `i > B`, or `i >= B`, with the
  induction variable on the left.
- The post clause is `i++`, `i--`, `++i`, `--i`, `i += K`, or `i -=
  K`, where `K` is a positive integer constant.
- The body contains no `break`, `continue`, `goto`, or labeled
  statement, never assigns, increments, decrements, or takes the
  address of the induction variable, and declares nothing with the
  same name.
- The loop ends, and runs at most 256 iterations.

The initial value, `B`, and `K` may each be an integer literal, an
integer cast of one, or an element of a file-scope `static` array
whose elements are `const` integers and whose initializer is all
integer literals, at indices that are themselves such constants. A
cast converts its operand's value, so `(unsigned char)260` is 4.

The pass steps the induction variable the way C does. The initial
value is converted to the variable's type, and each step wraps at the
variable's width and signedness. The condition compares the variable
with `B` after the integer promotions and the usual arithmetic
conversions, so an `int` compared with an `unsigned int` bound
compares as unsigned. For `for (unsigned char i = 250; i > 3; i +=
4)`, the values are 250 and 254, and 258 wraps to 2, which ends the
loop after two copies.
That lets an inner loop's bound come from a table indexed by an outer
unrolled loop's variable, as in `for (uint8_t i = 0; i < COUNTS[b];
i++)` inside a loop over `b`. Marked loops nested inside a marked
loop are unrolled in each copy, and marked loops anywhere inside an
unmarked statement are found and unrolled too.

## What it leaves alone

- Every loop without the pragma, and every loop when `--optimize` is
  off.
- A loop marked `#pragma c6502 loop unroll(disable)`, which the
  preprocessor drops as a no-op.
- `while` and `do` loops, which the pragma doesn't attach to.

A marked loop that doesn't match the shape above is a compile
error, because the pragma states that the loop is to be unrolled. A bound held in a variable, as in `i < n`,
fails with "condition's right operand must be an integer constant".

A marked loop that never ends is a compile error too. The loop's
state is the induction variable alone, so when the variable takes a
value a second time the pass reports "loop never terminates". In
`for (uint8_t i = 0; i <= 255; i++)`, `i` wraps from 255 to 0, and
`compile.py` prints one line ending in "unroll: loop never
terminates (`i` wraps at the width of `unsigned char` and the
condition stays true)". A loop that ends after more than 256
iterations fails with "iteration count exceeds cap of 256".

## Related

[Constant folding](constant_folding.md) evaluates the arithmetic on
the substituted constants, and [redundant load](redundant_load.md)
removes the repeated loads that back-to-back copies leave.
