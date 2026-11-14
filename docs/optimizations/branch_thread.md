# Branch threading on a known flag

`apply_branch_thread` in
[passes/branch_thread.py](../../passes/branch_thread.py) points a
branch whose target is another branch on the same flag at where that
second branch goes. It runs in the peephole fixed point, which both
pipelines run.

A path that takes `BCS L` arrives at `L` with C set. If the first
instruction at `L` is `BCC M`, that path never takes it, so the `BCS`
can go to the instruction after the `BCC` instead. If it's `BCS M`,
the path always takes it, so the first `BCS` can go to `M`. Each pair
of opposite conditions (`BCC`/`BCS`, `BEQ`/`BNE`, `BMI`/`BPL`,
`BVC`/`BVS`) tests one flag, and only labels may come between `L` and
the second branch, since they don't change the flags. The path that
took the first branch then skips the second, which is 2 cycles.

## Example

From `do_rwts_on_sector` in `examples/zork/instr_save/disk.c`, after
[carry INC](carry_inc.md) has rewritten the borrow out of the high
byte, which is in X:

### Before

```
.loop@0_continue:
   SBC   SECTORS_PER_TRACK
   BCS   .carry_inc@0
   CPX   #$01
   DEX
.carry_inc@0:
   BCC   .loop@0_break
   INC   iob+4
   JMP   .loop@0_continue
```

### After

```
.loop@0_continue:
   SBC   SECTORS_PER_TRACK
   BCS   .thread@0
   CPX   #$01
   DEX
   BCC   .loop@0_break
.thread@0:
   INC   iob+4
   JMP   .loop@0_continue
```

`.carry_inc@0` lost its last branch, and [dead label
drop](dead_label_drop.md) removes it. A pass through the loop that
borrows nothing from the high byte is `SBC; BCS; INC; JMP`, the same
as Zork's code.

## When it applies

- A branch's target label is followed, after any further labels, by a
  branch on the same flag.
- With the same condition, the first branch takes the second one's
  target, unless that's its own target already.
- With the opposite condition, the first branch goes to the
  instruction after the second. When that isn't already a label, the
  pass adds one, `.thread@N`, numbered across the program.

## What it leaves alone

- A branch whose target starts with anything other than labels and a
  branch, even an instruction that sets no flag, such as `DEX` before
  a `BEQ`, which does.
- A branch on a different flag at the target.

## Related

[Branch through jump](branch_through_jump.md) does the same for a
target that holds only a `JMP`. [Carry already known](carry_known.md)
uses the flags the same way to drop a `SEC` or `CLC`.
