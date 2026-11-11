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

- The first instruction is a `Mov` from any non-register operand, or
  a `TXA` or `TYA`. Both set N from the loaded byte.
- In the second form, the next instruction is a non-volatile store of
  A to memory.
- Then comes an `AND` of A with an immediate whose low byte is
  `$80`, and then a `BEQ` or `BNE`.
- A is dead from the instruction after the branch, by `a_dead_at`.

The liveness check starts at the instruction after the branch, so it
only follows the fall-through path. The branch target isn't checked,
and code there that reads A gets the unmasked byte. The C function

```c
unsigned char flags, hits;
unsigned char top_bit(void) {
    unsigned char t = flags & 0x80;
    if (t) { hits = 1; return 7; }
    return t;
}
```

compiles to `LDA flags; BPL .if_end@0; ... .if_end@0: RTS`, and with
`flags = 0x05` it returns 5 in the simulator where it should return
0. The first instruction is also not required to load A. A
`Mov(M, Reg(X))`, which emits as `LDX M`, passes the same check, and
the rewritten branch then tests bit 7 of M instead of bit 7 of A.
Both are bugs in the current code.

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
