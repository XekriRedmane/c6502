# Sinking increments

`SinkIncrements` in
[passes/optimization/sink_increment.py](../../passes/optimization/sink_increment.py)
moves `Y = X + c` or `Y = X - c` down to just after the last
instruction that reads `X`, as long as nothing in between reads `Y`.
It runs in the TAC fixed-point loop, after the truncate-of-extend
fold. It doesn't run in the unoptimized pipeline.

For `src[y++]`, `c99_to_tac` computes `y + 1` first and then reads
the old `y` as the index. Both values are live at the read, so the
register allocator has to put them in different places. After the
add moves past the read, the old `y` is dead by the time the new one
is defined, and the two can share one register. In a chain of
`src[y++]` reads, every value of `y` ends up in X, and each add
becomes an `INX`. In the example below the function body drops from
34 bytes to 22.

## Example

```c
#include <stdint.h>
static const uint8_t * const src = (const uint8_t * const)0x6000;
uint8_t out[3];
void copy3(uint8_t y) {
    out[0] = src[y++];
    out[1] = src[y++];
    out[2] = src[y];
}
```

### Before

```
Binary(+, @0.y, 1uc, %1.1)
IndexedConstLoad(0x6000, @0.y, %4.1)
IndexedSymbolStore(out, 0uc, %4.1)
Binary(+, %1.1, 1uc, %7.1)
IndexedConstLoad(0x6000, %1.1, %10.1)
IndexedSymbolStore(out, 1uc, %10.1)
IndexedConstLoad(0x6000, %7.1, %14.1)
IndexedSymbolStore(out, 2uc, %14.1)
Ret(None)
```

### After

```
IndexedConstLoad(0x6000, @0.y, %4.1)
Binary(+, @0.y, 1uc, %1.1)
IndexedSymbolStore(out, 0uc, %4.1)
IndexedConstLoad(0x6000, %1.1, %10.1)
Binary(+, %1.1, 1uc, %7.1)
IndexedSymbolStore(out, 1uc, %10.1)
IndexedConstLoad(0x6000, %7.1, %14.1)
IndexedSymbolStore(out, 2uc, %14.1)
Ret(None)
```

The final assembly is `LDX __zpabi_copy3__y; LDA $6000,X;
STA out; INX; LDA $6000,X; STA out+1; INX; ...`. With the pass
switched off, the first `y + 1` sits in Y and the second in a
zero-page byte, and each one is computed with `CLC; ADC #$01`.

## When it applies

The instruction must be a `Binary` `Add` or `Subtract` whose first
operand is a variable `X`, whose second operand is a constant, and
whose destination `Y` is a different variable. `X` must be an
SSA-renamed name, such as a local or a parameter, because the moved
instruction reads `X` at its new position. A static or an
address-taken local can be written by name or through a pointer in
between, so an add of one stays where it is.

The pass scans forward from the add and records the last
instruction that reads `X`. The scan stops at the first of these.

- A label, a jump of any kind, a `Ret`, a `FunctionCall`, or an
  `IndirectCall`. The add never moves into another block or past a
  call.
- An instruction that reads or writes `Y`.
- An instruction that writes `X` by name. An SSA-renamed `X` has no
  such write.
- A store of any kind, when the pass runs without SSA information,
  as its standalone `sink_increments` entry point does.
- Another sinkable add or subtract that also reads `X`. Without this
  stop, two such instructions would each move past the other on
  every sweep, and the fixed point would never settle.

If the scan found a read of `X`, the add moves to just after it.
Each sweep moves an instruction once, and the fixed point finishes
longer chains.

## What it leaves alone

- An add of a static. With a static `s`, in `unsigned t = s + 1; s =
  5; g = s; h = t;` the add stays ahead of the store of 5 to `s`, and
  `h` gets the old value plus one.

- An add of two variables. In `uint8_t z = y + d;` the add stays
  where it is, since moving it would also delay the read of `d`.
- An add whose `X` is next read after a call. In
  `uint8_t z = y + 1; use(); out[0] = src[y];` the call ends the
  scan before it reaches the read.
- Multiplies, shifts, and the other operators, which the pass
  doesn't match.

## Related

[Sinking an AND past a branch](sink_and_past_branch.md) moves a
different shape across a conditional jump.
