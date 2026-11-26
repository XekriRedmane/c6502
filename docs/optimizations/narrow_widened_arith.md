# Narrowing widened arithmetic

`NarrowWidenedArith` in
[cpp/src/passes/optimization/narrow_widened_arith.h](../../cpp/src/passes/optimization/narrow_widened_arith.h)
rewrites `Truncate(Extend(a) op Extend(b))` as `a op b` at the
truncated width, when the low bytes of the result depend only on the
low bytes of the operands. It's the last pass in the TAC fixed-point
loop, and it has to run after
[sinking an AND past a bit test](sink_and_past_branch.md), for the
reason given under "When it applies". It doesn't run in the
unoptimized pipeline.

C's integer promotions turn `uint8_t d = a * b;` into two
`ZeroExtend`s to `int`, a 16-bit multiply, and a `Truncate` back to
one byte. The high byte of the product feeds only the byte that the
truncate throws away. The asm-level passes can remove the work of an
unread high byte that is computed inline, but a multiply is a call of
a runtime helper, and no asm pass turns a call of `mul16` into one of
`mul8`. Doing the multiply at one byte in TAC makes it a call of
`mul8`, and the function body in the example below drops from 20
bytes to 14. For a subtract, as in `uint8_t d = a - b;`, the asm-level passes
now remove the high byte by themselves, and the final code is the
same with this pass disabled.

## Example

```c
#include <stdint.h>
uint8_t g;
void prod(uint8_t a, uint8_t b) { uint8_t d = a * b; if (d) g = d; }
```

### Before

```
ZeroExtend(@0.a, %0.1)
ZeroExtend(@1.b, %1.1)
Binary(*, %0.1, %1.1, %2.1)
Truncate(%2.1, %3.1)
JumpIfFalse(%3.1, .if_end@0)
Copy(%3.1, g)
Label(.if_end@0)
Ret(None)
```

### After

```
Binary(*, @0.a, @1.b, %3.1)
JumpIfFalse(%3.1, .if_end@0)
Copy(%3.1, g)
Label(.if_end@0)
Ret(None)
```

The final assembly is `STA HARGS; STX HARGS+1; JSR mul8; LDA
HARGS+2; BEQ .if_end@0; STA g`. With the pass disabled, `prod` zeroes
the two high bytes of `mul16`'s operands and calls it, which takes `STA
HARGS; LDA #$00; STA HARGS+1; STA HARGS+3; STX HARGS+2; JSR mul16; LDA
HARGS+4`.

A constant shift narrows the same way. For `return len << 4;` with a
`uint8_t len`, the pass turns `Binary(<<, %0.1, 4, %1.1)` and its
`Truncate` into `Binary(<<, @0.len, 4uc, %2.1)`, which lowers to four
`ASL A`. The final code is the same four `ASL A` with the pass
disabled.

A comparison's result is 0 or 1, so a `Truncate` of one is the same
comparison with a narrow result, and its operands stay as they are.
Zork's `branch` starts `bool on_true = (b & 0x80) != 0;`, which C
computes as an `int` and converts to `bool` with a `Truncate`. With
the comparison's result in one byte, `tac_to_asm` lowers the test
against 0 as `AND #$80; BEQ end; LDA #1; end:`
(`translate_zero_test`), where the `int` result took `BNE true; LDA
#0; JMP end; true: LDA #1; end:`. This needs the truncate to be the
comparison's only reader, and each variable operand to hold at the
truncate the value the comparison read, as below.

## When it applies

- The `Truncate` reads an SSA-renamed variable defined by a
  `Binary` with one of these operators. `+`, `-`, `*`, `&`, `|`, and
  `^` qualify because carries only move from low bytes to high
  bytes. `<<` qualifies with a constant count less than the number
  of bits in the truncated width, or with a variable count, which
  `tac_to_asm` then lowers as a loop of one-bit shifts of the narrow
  value ([shifts by a loop](shift_loop.md)). A count that shifts every
  bit out gives 0 at either width. `>>` qualifies with such a
  count when its first operand is a `ZeroExtend` of an unsigned
  variable or a `SignExtend` of a signed one, as wide as the
  truncate's destination. The bytes the extend adds hold 0 or copies
  of the sign bit, and a narrow logical or arithmetic shift brings in
  the same bits, so `(uint8_t)(props[y] >> 5)` is `LDA (props),Y;
  LSR` five times. When `tac_to_asm` lowers the wide shift inline
  (`shift_cost.inline_shift_ok`) and would lower the narrow one as a
  loop, which is slower, the shift stays wide. A `signed char`
  shifted by 3 narrows, since its byte shift and its 16-bit shift are
  both loops of three passes.
- The `Binary`'s result is wider than the truncate's destination.
- Each operand is one of these:
  - a constant, which is reduced modulo the narrow width and takes a
    type of that width with the destination's signedness;
  - an SSA-renamed variable defined by a `ZeroExtend` or `SignExtend`
    of a variable exactly as wide as the truncate's destination,
    which the new `Binary` reads instead;
  - any other integer variable wider than the destination, such as a
    16-bit global. A new `Truncate` just before the new `Binary`
    takes its low bytes, so `(uint8_t)(SCRATCH2 << 1)` on a 16-bit
    `SCRATCH2` becomes `LDA SCRATCH2; ASL A`. This needs the truncate
    to be the wide `Binary`'s only reader. Otherwise the wide
    `Binary` stays, and a narrow one would do its low byte again, as
    for `(uint8_t)(w + 3)` next to `(w + 3) >> 8`.
- The new `Binary` reads its operands where the truncate was, so each
  variable it reads must still hold the value the wide `Binary` (or
  the extend) read. An SSA-renamed variable always does. Any other
  one, such as a global, must not be volatile, and the extend or
  `Binary` must be in the truncate's block with nothing between them
  that may write it: an assignment to it, a call, or a store through
  a pointer or an index.

The new `Binary` writes the truncate's destination. When the
truncate was the only reader of the wide result, the pass drops the
wide `Binary`, and with it each extend that the `Binary` was the only
reader of. Otherwise the wide `Binary` and its extends stay for their
other readers.

The pass has to run after the AND sink in the fixed point. The sink
matches the wide four-instruction form `ZeroExtend; Binary(&);
Truncate; JumpIfMasked`. Once this pass has turned the first three
into one byte-wide `Binary(&, x, 127uc, t)`, the sink no longer
matches it, so the masked value is computed before the branch and
held in memory across it.

An operand that isn't SSA-renamed, such as a global, is read at the
narrow operation, so the pass checks that nothing between the wide
operation and the matched `Truncate` writes it. It finds the
`Truncate` by identity. Looking it up as the last definition of its
result, as it once did, found a different `Truncate` when a global is
written on two paths. Lode Runner's `check_for_input`
(examples/lode_runner/input.c) halves `ALIVE` in three cases of a
switch, and only the last one had become `LSR ALIVE`. The other two
had stayed `LDA #$00; LSR; ROR ALIVE`, 12 bytes in all.

## What it leaves alone

- `/` and `%`, where high bytes of the operands move into the low
  bytes of the result.
- `>>` of any value other than an extended variable as wide as the
  result. For a 2-byte `w`, `(uint8_t)((uint32_t)w >> 5)` takes bits
  8 to 12 of `w` into the result, so a shift of `w`'s low byte would
  lose them. A `ZeroExtend` of a signed byte isn't narrowed either,
  since its added byte isn't a copy of the sign bit, which an
  arithmetic shift of the byte would bring in.
- A global or other non-SSA operand that may change between the
  wide `Binary` and the truncate, as in `int t = g + 1; h();
  (uint8_t)t` when `h` can write `g`, and a volatile one.
- Floating-point constants.
- A truncate of a plain extend with no operation between, which
  [truncate-of-extend folding](truncate_extend_fold.md) handles.

## Related

A right shift of a wide value isn't narrowed here, since its high
bytes shift down into the low one. `tac_to_asm` handles that case
itself: when a
constant right shift's result is read only through a `Truncate` to
one byte (`significant_bytes.low_byte_only`), it computes just that
byte from the two source bytes it comes from. `(uint8_t)(ZCHARS >>
5)` is `LDA ZCHARS; LSR` five times; `STA t; LDA ZCHARS+1; ASL` three
times; `ORA t`, where it called `lsr16`, and a shift by 1 or 7 is four
instructions (`LDA hi; LSR; LDA lo; ROR`).

`tac_to_asm` also compares only the low bytes of `==` and `!=` that
either operand can have nonzero (`significant_bytes`), since the bytes
above them are 0 in both. In `(b & 0x80) != 0` the `int` operand has
one such byte and 0 has none, so the comparison is one byte. Before
this, the high bytes, which the asm already knew were 0, took `LDA
#$00; CMP #$00; BNE`. Zork's `instr_jin` compares two bytes, which
C promotes to `int`, and now `LDA OPERANDS+2; CMP (p),Y; BEQ` does
it.

[Truncate of an extend](truncate_extend_fold.md) is the case with no
operation in between.
