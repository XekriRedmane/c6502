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

## After any instruction that set N from A

The `LDA` needn't be right before the `AND`. When the instruction that
last wrote A set N from it, with only stores and conditional branches
between, and a label in between is reached that way on every path in
(`redundant_compare._flags_from`), `AND #$80; BEQ` / `BNE` is `BPL` /
`BMI`. A must be dead on both paths, and N and Z too, since without the
`AND`, Z comes from all of A. In Zork's `ascii_to_zchar`
(examples/zork/interpreter/input.c), `a = get_alphabet_for_char(c) -
1; if (a & 0x80)` was `SEC; SBC #$01; STA a; AND #$80; BEQ`, and
`get_alphabet_for_char` tests `c & 0x80` after an `ORA #$00` that tests
`c == 0`, 8 bytes in the two with the reloads of A that then go.

## The low bit

Bit 0 maps onto C through `LSR A`, which is 1 byte to the `AND`'s 2
and takes the same 2 cycles:

```
   LDA   sign_bit                      LDA   sign_bit
   AND   #$01                    ->    LSR
   BEQ   .if_else                      BCC   .if_else
```

`BEQ` becomes `BCC` and `BNE` becomes `BCS`. The `LSR` leaves A
shifted where the `AND` left it masked, writes C, which the `AND`
left alone, and sets N and Z from the shifted value, so A, C, N, and
Z must all be dead on both paths after the branch. Any instruction
may put the value in A. That's the test of `sign_bit & 0x01` in
Zork's `store_with_sign` (examples/zork/interpreter/numbers.c), of
the transcript bit in `read_line` and twice in `output.c`, and of a
header flag in `main`.

## Bit 6

`BIT m` copies bit 6 of `m` into V, so a test of bit 6 of a byte in
memory needn't load it:

```
   LDA   __local_branch__offset_0      BIT   __local_branch__offset_0
   AND   #$40                    ->    BVC   .if_else@16
   BEQ   .if_else@16
```

`BEQ` becomes `BVC` and `BNE` becomes `BVS`. `m` is a `Data` or `ZP`
byte, which `BIT` can address, and isn't volatile. `BIT` leaves A as
it was, sets N from bit 7 of `m` and Z from A AND `m`, and writes V,
where the load and the `AND` left V alone, so A, N, Z, and the earlier
V must all be dead on both paths after the branch. It saves 2 bytes
and 2 cycles. Zork's `branch` (examples/zork/interpreter/decode.c)
tests bit 6 of its offset byte twice this way.

## What it leaves alone

- A mask other than `$80`, `$40`, or `$01`, as in `flags & 0x20`,
  since only bits 7, 6, and 0 map onto a flag.
- A test of bit 6 of a value that isn't a byte in memory, since `BIT`
  has no immediate or register form on the 6502.
- A read of the masked value after the branch on the fall-through
  path. `unsigned char t = flags & 0x80; if (t) hits = t;` keeps its
  `AND #$80`, because the `STA hits` after the `BEQ` reads A.
- A branch on any condition other than Z.
- Any other instruction between the load and the `AND`.

## Related

[Constant arithmetic fold](const_arith_fold.md) folds the high byte
of the promoted `int` AND, which leaves this pass the low-byte
`AND #$80`.
