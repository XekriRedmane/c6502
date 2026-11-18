# DEC

`apply_dec_peephole` in
[passes/dec_peephole.py](../../passes/dec_peephole.py) rewrites an
in-place subtract of 1 from a 1-byte value as a single `DEC`, or as
`DEX` or `DEY` when the value is in X or Y. It runs in the peephole
fixed point, in both pipelines, but in practice only the optimized
pipeline produces the shape it matches.

`tac_to_asm` lowers `x -= 1` on a 1-byte `x` as `LDA x; SEC; SBC #1;
STA x`. The 6502's `DEC` subtracts 1 from a byte in memory without
going through A, and sets N and Z from the result the same way the
`SBC` does. For an absolute operand the `DEC` takes 3 bytes and 6
cycles, where the `SBC` form takes 9 bytes and 12 cycles. In zero
page it's 2 bytes and 5 cycles against 7 bytes and 10 cycles.

## Example

```c
static unsigned char lives;
void die(void) { lives -= 1; }
```

### Before

```
   LDA   lives
   SEC
   SBC   #$01
   STA   lives
   RTS
```

### After

```
   DEC   lives
   RTS
```

The `SBC` form leaves the new value in A and the `DEC` doesn't. When
A is read afterwards, the pass adds a load of the byte after the
`DEC`. For `return --lives;` the four instructions become `DEC lives;
LDA lives`, which is still 3 bytes shorter.

## When it applies

The pass matches the four atoms `Mov(M, A)`, `SetCarry`,
`Sub(Imm(1), A)`, and `Mov(A, M)`, where the store writes the same
byte the load reads and neither move is volatile. M must be a `Data`
or `ZP` operand, or X or Y.

`DEC` leaves C and V alone, where the `SBC` sets them. So the pass
follows every path from the store, through both arms of each `BEQ`,
`BNE`, `BMI`, or `BPL`, and rejects the match when it reaches a
`BCC`, `BCS`, `BVC`, or `BVS`, or an `ADC`, `SBC`, `ROL`, or `ROR`,
before C is overwritten. A `CMP`, `ASL`, `LSR`, `SEC`, or `CLC`
overwrites C without reading it and ends that path, and so do a call
and a return. Loads, stores, transfers, logical operations, `INC`,
`DEC`, pushes and pulls, and `BIT` leave C alone, and the walk passes
over them.

## A 2-byte decrement

The low byte borrows when it was 0, which a test before the `DEC`
tells. So the 2-byte decrement in place that [carry INC](carry_inc.md)
leaves,

```
   LDA   Z_SP
   SEC
   SBC   #$01
   STA   Z_SP
   BCS   .carry_inc@0
   DEC   Z_SP+1
.carry_inc@0:
```

becomes

```
   LDA   Z_SP
   BNE   .carry_inc@0
   DEC   Z_SP+1
.carry_inc@0:
   DEC   Z_SP
```

8 bytes for zero-page bytes where it took 10. A now holds the old low
byte, C is unchanged, and N and Z come from the low byte's `DEC`, so A
and every flag must be dead after the label, and nothing else may
branch to the label, which the old code reached with the low byte
already decremented. Zork's `push` (examples/zork/decode.c)
decrements its stack pointer twice this way.

When the low byte's load follows a store of A into the same byte,
with only stores between, the `SBC` form needs no load at all, while
the `BNE` form needs one to set the flags. The rewrite is made then
only when the instruction before those stores set N and Z from A, so
the load can go. In `dec_var`, `var_get(var) - 1` leaves the low byte
in A after a `JSR`, and the rewrite would have added an `ORA #$00`,
one byte more. In `please_insert_save_diskette` (`prompts.c`) it
follows an `AND`, and `SEC; SBC #1; BCS` becomes `BNE` and a second
`DEC`, a byte less, since the store stays in both forms.

## What it leaves alone

- The low byte of a multi-byte subtract. If the next three atoms are
  a continuation byte, `Mov(M2, A); Sub(Imm(0), A); Mov(A, M2)`, the
  match is rejected, since that byte's `SBC #0` reads the borrow.
  After `DEC`, no single flag tells whether the byte borrowed. Z is
  set when the byte went from 1 to 0, and N is set both when it went
  from 0 to `$FF` and when it went from `$81` to `$80`. So there's
  no multi-byte form of the chain itself. For `ticks -= 1` on a
  `uint16_t`, [carry INC](carry_inc.md) rewrites the top byte to `BCS
  skip; DEC ticks+1`, and the next section covers what this pass then
  does with it.
- A byte on the soft stack or behind a pointer, because `DEC` has no
  `(zp),Y` mode. In the unoptimized pipeline every local and
  temporary is on the soft stack, and `lives -= 1` on a static goes
  through a temporary there, so the pass reports 0 changes on
  `lives--`, `--lives`, and `lives = lives - 1` under `--unopt`.
- A subtract whose store goes somewhere other than where its load
  reads.

## Related

[Multi-byte INC](inc_peephole.md) is the add-1 counterpart, and
[subtract-and-test](sub1_test_zero_peephole.md) handles a subtract of
1 that a branch on zero follows. [DEC in a register](dec_register_peephole.md)
handles a counter that has been moved into X or Y.
