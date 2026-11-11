# Branch on INC or DEC flags

`apply_dec_inc_branch_fold` in
[passes/dec_inc_branch_fold.py](../../passes/dec_inc_branch_fold.py)
deletes the `LDA M` in `DEC M; LDA M; Bcc` and `INC M; LDA M; Bcc`
when the branch reads only N or Z. It runs in the peephole fixed
point, which both pipelines run, but no program in the test corpus
makes it fire in the unoptimized pipeline.

`INC` and `DEC` set N and Z from the byte they write back, and an
`LDA` of the same byte sets them to the same values. So a `BEQ`,
`BNE`, `BMI`, or `BPL` after the load branches the same way without
it, and the load's only remaining effect is the value it puts in A.
When nothing reads that value, deleting the load saves 2 bytes and 3
cycles for a zero-page `M`, or 3 bytes and 4 cycles for an absolute
one. A countdown loop that tests its counter at the bottom gains the
most, since the load runs on every iteration.

[Redundant load after RMW](redundant_load_after_rmw.md) makes the
same deletion when the `INC` or `DEC` and the load are adjacent, and
it runs earlier in each sweep. This pass also steps over labels
between the two, and it catches the shapes that other passes
complete later in the same sweep. In the example below, the load is
first followed by `AND #$80; BNE`, which reads A, so the earlier
pass keeps it. `and_sign_bit_branch` then turns the pair into a
`BMI`, and this pass deletes the load in the same sweep.

## Example

```c
unsigned char timer;
unsigned char tick(void) {
    timer--;
    if ((timer & 0x80) == 0)
        return timer;
    return 0;
}
```

### Before

```
tick:
   DEC   timer
   LDA   timer
   BMI   .if_end@0
   LDA   timer
   RTS
.if_end@0:
   LDA   #$00
   RTS
```

### After

```
tick:
   DEC   timer
   BMI   .if_end@0
   LDA   timer
   RTS
.if_end@0:
   LDA   #$00
   RTS
```

## When it applies

- An `INC` or `DEC` of a `Data` or `ZP` operand is followed by a
  non-volatile `LDA` of the same operand. Labels may sit between the
  two, as long as no branch or jump in the function targets them.
- Right after the load comes a `BEQ`, `BNE`, `BMI`, or `BPL`.
- A is dead from the instruction after the branch, by `a_dead_at`.

The liveness check starts at the instruction after the branch, so it
only follows the fall-through path. The branch target isn't checked.
The function

```c
unsigned char timer, hits;
unsigned char tick(void) {
    unsigned char t = --timer;
    if (t == 0) { hits = 1; return 7; }
    return t;
}
```

compiles to `DEC timer; BNE .if_end@0; ... .if_end@0: RTS`, which
returns whatever A held on entry instead of `timer`. With `timer = 5`
the simulator returns 5 where the unoptimized build returns 4. This
is a bug in the current code.

## What it leaves alone

- A volatile `M`. With `volatile unsigned char timer`, the same
  function keeps its `LDA timer` before the `BMI`, because a volatile
  read has to happen.
- A `BCC`, `BCS`, `BVC`, or `BVS`. The pass accepts only the four
  branches on N or Z.
- A label between the two that some branch or jump targets, since a
  path arriving there didn't run the `INC` or `DEC`.
- An `M` on the soft stack or behind a pointer.

## Related

[Sign-bit test without AND](and_sign_bit_branch.md) produces the
`BPL` and `BMI` this pass often matches, and
[DEC](dec_peephole.md) produces the `DEC M`.
