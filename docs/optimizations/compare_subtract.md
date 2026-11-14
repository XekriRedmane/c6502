# Compare and subtract of the same operands

`hoist_subtract_to_compare` in
[passes/optimization/compare_subtract.py](../../passes/optimization/compare_subtract.py)
moves a `t = a - b` up to the `a < b` (or `>=`, `>`, `<=`) whose
branch decides whether it runs, and `Translator._fused_subtract` in
[tac_to_asm.py](../../tac_to_asm.py) then lowers the pair as one
subtraction followed by a branch on its flags. It runs once, after the
TAC fixed point, under `--optimize`.

`while (s >= k) s -= k;` compares `s` with `k` and, when the loop
continues, subtracts `k` from `s`. On the 6502 those are the same
subtraction. A 2-byte compare is `LDA s; SEC; SBC k; LDA s+1; SBC
k+1; BCC`, which computes `s - k` and keeps only the borrow, and the
subtract then computes it again to store it. After the rewrite, the
subtraction stores its bytes and the branch reads its carry, as
Zork's `SBC SECTORS_PER_TRACK; BCS` does.

An unsigned compare branches on C: `BCC` for `<` and `>`, `BCS` for
`>=` and `<=` (the last two of each pair swap the operands, so the
subtraction is `b - a`). A signed one reloads the top byte of the
difference, which leaves V as the `SBC` set it, and corrects its sign
for overflow as an ordinary signed compare does (`BVC; EOR #$80;
BMI` or `BPL`).

## Example

From `do_rwts_on_sector` in `examples/zork/instr_save/disk.c`:

```c
uint16_t sector = SCRATCH1;
while (sector >= SECTORS_PER_TRACK) {
    sector -= SECTORS_PER_TRACK;
    iob.track++;
}
iob.sector = (uint8_t)sector;
```

### Before

```
.loop@0_continue:
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_1
   SBC   #$00
   BCC   .loop@0_break
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__command
   STA   __local_do_rwts_on_sector__sector_0
   BCS   .carry_inc@0
   DEC   __local_do_rwts_on_sector__sector_1
.carry_inc@0:
   INC   iob+4
   JMP   .loop@0_continue
```

### After

```
.loop@0_continue:
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__command
   STA   __local_do_rwts_on_sector__0
   LDA   __local_do_rwts_on_sector__sector_1
   SBC   #$00
   STA   __local_do_rwts_on_sector__sector_1
   BCC   .loop@0_break
   INC   iob+4
   LDA   __local_do_rwts_on_sector__0
   STA   __local_do_rwts_on_sector__sector_0
   JMP   .loop@0_continue
```

A pass through the loop takes 42 cycles instead of 50, and the
routine is 7 bytes shorter. That's the compare and the subtract alone.
The low byte of the difference goes through a temporary, since the
loop's exit reads the old `sector`; the next section removes that.

## The old value on the loop's exit

When the minuend `a` is the value the loop carries round (a Phi in
the compare's block) and only the loop's exit reads it after the
compare, the pass gives the exit `a` as `t + b` instead, the way
Zork's code adds `SECTORS_PER_TRACK` back after its loop:

```
.loop@0_break:
    Binary(Add, %11.1, %8.1.exit, @9.sector.2.exit)
    Truncate(@9.sector.2.exit, %16.1)
```

`a` then dies at the subtract, so the loop updates it in place. When
`b` is a read of a variable in memory, as `ZeroExtend(SECTORS_PER_TRACK)`
is, and nothing after it in the compare's block writes memory, the
exit reads the variable again into a new temporary (`%8.1.exit`),
rather than keeping the loop's copy alive past the loop.

With [operands read in place](cmp_through_tmp_fold.md), the register
allocator's [carry hint](regalloc.md), [carry INC](carry_inc.md),
[branch threading](branch_thread.md), and [carry already
known](carry_known.md), the loop ends up as Zork wrote it, with the
low byte in A and the high byte in X:

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

A pass that borrows nothing from the high byte is 15 cycles.

## When it applies

- A block ends in `JumpIfCmp` with `<`, `<=`, `>`, or `>=` on integer
  or pointer operands of 2 bytes or more.
- One of its successors has that block as its only predecessor and
  computes `Binary(Subtract)` of the difference the compare looks at:
  `src1 - src2` for `<` and `>=`, `src2 - src1` for `>` and `<=`.
- The operands are SSA values or constants. The subtraction's may
  instead each be a second read of the variable in memory that the
  compare's operand reads (`ZeroExtend(SECTORS_PER_TRACK)` in both
  blocks above), with no memory write between the two reads.
  [Common subexpression elimination](cse.md) leaves such reads alone,
  since a second read of a global costs no more than holding the first
  in a temporary.
- The difference is an SSA value of the operands' type.

In `tac_to_asm` the subtract has to come right before the compare,
and its destination can't be one of the compare's operands, which the
compare reads after it.

## What it leaves alone

- 1-byte operands. A 1-byte compare is a `CMP`, which leaves A alone,
  and replacing it with the subtraction costs a store when A is read
  on the other side of the branch. [CMP and SBC
  fusion](cmp_sbc_fusion.md) handles that case in the asm, where it
  can see whether A is read.
- `==` and `!=`, whose result needs every byte's zero test.
- A signed `char` compare: the compare is of the operands widened to
  `int`, and the subtraction is narrowed back to a byte.
- The side of the branch that doesn't use the difference still
  computes it. In `do_rwts_on_sector` that costs the exit an `ADC`
  to get the old value back, 3 cycles on a call where the loop
  doesn't run.
- An exit that isn't reached only from the compare, or a minuend read
  anywhere else after the compare, for the exit's recompute. The
  subtract still moves.
