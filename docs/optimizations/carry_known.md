# Carry already known

`apply_carry_known` in
[passes/carry_known.py](../../passes/carry_known.py) drops a `SEC` or
`CLC` where the carry already has that value, and moves one in front
of a loop when every way back into the loop already has it. It runs
once under `--optimize`, after the peephole fixed point's third run.
Several peepholes match a `SEC` or `CLC` ([transfer plus or minus
one](transfer_pm1_store.md) turns `TXA; CLC; ADC #1; STA M` into
`INX; STX M`, and [CMP and SBC fusion](cmp_sbc_fusion.md) moves a
`SEC; SBC`), so it runs after they've had their turn.

A forward walk over the function's control flow tracks C before each
instruction as set, clear, or unknown. `SEC` and `CLC` set it. A
`BCS` or `BCC` gives each of its two ways out a known value: after
`BCC L` C is clear at `L` and set on the fall-through. Instructions
that don't touch C pass its value on: loads and stores, `INC` and
`DEC`, `AND`, `ORA`, `EOR`, `BIT`, `PHA` and `PLA`, labels, and
jumps. Any other instruction, such as `ADC`, `SBC`, a compare, a
shift or rotate, or a call, leaves it unknown. Where two paths meet
with different values, it's unknown too.

Then:

- A `SEC` where C is known set, or a `CLC` where it's known clear,
  goes. `do_rwts_on_sector` ends its loop with `BCC .loop@0_break`,
  so the `CLC` in front of the exit's `ADC SECTORS_PER_TRACK` goes.
- A `SEC` or `CLC` that starts a block, after its labels, moves in
  front of those labels when every way into the block other than
  falling into it already has C that way. The way in from above then
  sets C, and the loop's back edge, which already had it, skips the
  instruction.

## Example

From `do_rwts_on_sector` in `examples/zork/instr_save/disk.c`:

### Before

```
   LDA   SCRATCH1
   LDX   SCRATCH1+1
.loop@0_continue:
   SEC
   SBC   SECTORS_PER_TRACK
   BCS   .thread@0
   CPX   #$01
   DEX
   BCC   .loop@0_break
.thread@0:
   INC   iob+4
   JMP   .loop@0_continue
.loop@0_break:
   CLC
   ADC   SECTORS_PER_TRACK
```

### After

```
   LDA   SCRATCH1
   LDX   SCRATCH1+1
   SEC
.loop@0_continue:
   SBC   SECTORS_PER_TRACK
   BCS   .thread@0
   CPX   #$01
   DEX
   BCC   .loop@0_break
.thread@0:
   INC   iob+4
   JMP   .loop@0_continue
.loop@0_break:
   ADC   SECTORS_PER_TRACK
```

Both ways to `.thread@0` have C set, the `BCS` taken and the `BCC` not
taken, and `INC` leaves it, so the `JMP` back to the loop has C set
and the `SEC` moves in front. A pass through the loop is then `SBC;
BCS; INC; JMP`, 15 cycles, which is Zork's own loop.

## When it applies

- For dropping: C is known at the `SEC` or `CLC` along every path that
  reaches it.
- For moving: the `SEC` or `CLC` follows one or more labels, the
  instruction before the labels falls through into them (it isn't a
  `JMP` or a return), and every other edge into the labels has C
  known to be the value the instruction sets.

## What it leaves alone

- A carry setter after a call, or after anything else not known to
  leave C alone, unless a later `SEC` or `CLC` sets it again.
- A setter at the start of a function, where C comes from the caller.
- A setter whose block can be reached with C the other way or unknown
  along a path other than falling into it.

## Related

[Branch threading on a known flag](branch_thread.md) uses known
flags to skip a branch, and [carry INC](carry_inc.md) leaves the
carry as the loop's `BCC` needs it.
