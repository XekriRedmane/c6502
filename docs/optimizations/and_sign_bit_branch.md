# Sign-bit test without AND

`apply_and_sign_bit_branch` in
[passes/and_sign_bit_branch.py](../../passes/and_sign_bit_branch.py)
turns `LDA M; AND #$80; BEQ` into `LDA M; BPL` and `LDA M; AND #$80;
BNE` into `LDA M; BMI`. It runs in the peephole fixed point, which
both pipelines run, but no program in the test corpus makes it fire
in the unoptimized pipeline. There the result of the `AND` goes to a
soft-stack temporary and is tested later, so the three instructions
are never adjacent.

A test of bit 7, such as `if (x & 0x80)`, lowers to an `AND #$80`
followed by a branch on Z. The load before the `AND` already set N
to bit 7 of the loaded byte, and after the `AND`, Z is set exactly
when that bit is clear. So `BEQ` after the `AND` branches in the same
cases as `BPL` without it, and `BNE` in the same cases as `BMI`.
Dropping the `AND` saves 2 bytes and 2 cycles. The difference is
that A keeps the whole byte instead of the masked value, so the pass
requires A to be dead after the branch.

## Example

```c
unsigned char flags;
unsigned char hits;
void check(void) {
    if (flags & 0x80)
        hits = 1;
}
```

### Before

```
check:
   LDA   flags
   AND   #$80
   BEQ   .if_end@0
   LDA   #$01
   STA   hits
.if_end@0:
   RTS
```

### After

```
check:
   LDA   flags
   BPL   .if_end@0
   LDA   #$01
   STA   hits
.if_end@0:
   RTS
```

A second form allows one store between the load and the `AND`, since
`STA` changes neither A nor the flags. With `copy = flags; if (copy &
0x80) hits = 1;` the pass turns `LDA flags; STA copy; AND #$80; BEQ
.if_end@0` into `LDA flags; STA copy; BPL .if_end@0`.

## When it applies

- The first instruction is a `Mov` from any non-register operand
  into A or into memory (which emits as `LDA src; STA dst`), or a
  `TXA` or `TYA`. All of these set N from the byte they load into A.
  A load into X or Y (`LDX M`) doesn't count, since the `AND` tests
  A.
- In the second form, the next instruction is a non-volatile store of
  A to memory.
- Then comes an `AND` of A with an immediate whose low byte is
  `$80`, and then a `BEQ` or `BNE`.
- A is dead after the branch on both paths, by `a_dead_at`: from
  the instruction after the branch, and from the branch's target
  label. A target that isn't a label in the function counts as live.

Without the `AND`, A holds the unmasked byte on both paths. In

```c
unsigned char flags, hits;
unsigned char top_bit(void) {
    unsigned char t = flags & 0x80;
    if (t) { hits = 1; return 7; }
    return t;
}
```

the branch's target returns `t` in A, so the code stays `LDA flags;
AND #$80; BEQ .if_end@0`.

## What it leaves alone

- A mask other than `$80`, as in `flags & 0x40`, since only bit 7
  maps onto a flag.
- A read of the masked value after the branch on the fall-through
  path. `unsigned char t = flags & 0x80; if (t) hits = t;` keeps its
  `AND #$80`, because the `STA hits` after the `BEQ` reads A.
- A branch on any condition other than Z.
- Any other instruction between the load and the `AND`.

## Related

[Constant arithmetic fold](const_arith_fold.md) folds the high byte
of the promoted `int` AND, which leaves this pass the low-byte
`AND #$80`.
