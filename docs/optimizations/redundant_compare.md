# Redundant compare

`apply_redundant_compare` in
[passes/redundant_compare.py](../../passes/redundant_compare.py)
deletes a compare at a label when the only way to reach the label is
a branch right after an identical compare. It runs in the peephole
fixed point, which both pipelines run, but no program in the test
corpus makes it fire in the unoptimized pipeline. That pipeline
computes each comparison into a 0 or 1 value on the soft stack and
tests that value, so two compares of the same operands in a row
don't arise there.

Two tests of the same operands in a row, such as

```c
if (key < entry) return 1;
if (key == entry) return 2;
```

lower to two `CMP entry` instructions with A holding `key` both
times, once `redundant_load` has dropped the second `LDA key`. A
branch changes no register or flag, so when the second compare can
only be reached by the branch after the first, A and the operand are
what the first compare saw and every flag is already set the way the
second compare would set it. Deleting it saves 2 bytes and 3 cycles
for a zero-page operand, or 3 bytes and 4 cycles for an absolute one.

## Example

```c
unsigned char classify(unsigned char key, unsigned char entry) {
    if (key < entry)
        return 1;
    if (key == entry)
        return 2;
    return 3;
}
```

### Before

```
classify:
   STX   __local_classify__entry
   CMP   __local_classify__entry
   BCS   .if_end@0
   LDA   #$01
   RTS
.if_end@0:
   CMP   __local_classify__entry
   BNE   .if_end@1
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$03
   RTS
```

### After

```
classify:
   STX   __local_classify__entry
   CMP   __local_classify__entry
   BCS   .if_end@0
   LDA   #$01
   RTS
.if_end@0:
   BNE   .if_end@1
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$03
   RTS
```

## When it applies

- The compare is preceded by a run of one or more labels, and the
  instruction before that run is a `Jump`, `Return`, or `Ret`, so
  nothing falls through into it.
- Exactly one `Branch` or `Jump` in the function targets any label
  in the run. It must be a `Branch`, and the instruction before it,
  past any other branches right before it, must be a compare equal
  to the one being deleted, with the same register and the same
  operand.
- No label in the run has its address taken by an `ImmLabelLow` or
  `ImmLabelHigh` operand.

The pass also deletes a compare that follows one or more branches
right after an identical compare, with no label in between, so it's
reached only by falling through them:

```
   CMP   (entry),Y
   BCC   .tail@0
   BNE   .if_end@41
   CMP   (entry),Y          <- deleted
```

[Byte-by-byte three-way compare](three_way_compare.md) writes a `<`
and a `!=` of the same bytes in a row, which lower to that shape.

## A compare with 0

A compare of a register with 0 sets N and Z from the register's
value, and sets C. When the instruction that wrote the register set N
and Z from the same value, and C is dead after the compare, the
compare goes too, saving 2 bytes and 2 cycles:

```
   LDA   __zpabi_check_sign__b_1       LDA   __zpabi_check_sign__b_1
   CMP   #$00                    ->    BNE   .if_else@6
   BNE   .if_else@6
```

That's the high byte of `b == 2` in Zork's `instr_mul`
(examples/zork/numbers.c). The instructions that count
are a load or transfer into the register, an `ADC`, `SBC`, `AND`,
`ORA`, `EOR`, or shift on A, an `INX` / `DEX` style step, and a
`PLA`. Stores may come between that instruction and the compare,
except a store lowered with an `LDY #off` setup, since the `LDY` sets
N and Z from Y. These compares go in a sweep of their own, which
returns before the other rule runs, since a compare with 0 that one
rule drops may be the one an identical compare after it relies on
for C.

An `ORA #$00`, `AND #$FF`, or `EOR #$00` goes under the same rule,
without the condition on C, which none of them changes. The `EOR
#$00` comes from the low bytes of the `^` that restores the padding
bits of a signed `_BitInt(N)` (bit_precise.py), whose sign bit is in
the top byte. `round_trip_load_drop` leaves
`ORA #$00` where a load of a byte just stored comes after an
instruction that doesn't set N and Z from A.

The walk back from the compare also passes over conditional branches
and follows a label to every way in: each branch or jump to it, and
the fall-through from the instruction before it. Each must have set
N and Z from the register. In `je`, in Zork's `do_2op`
(examples/zork/decode.c), `equal = a == b;` ends

```
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_do_2op__equal
   ORA   #$00
   BNE   .if_end@25
```

and both loads into A set the flags the `ORA #$00` tests, so it goes,
for 6 bytes in the three such tests. A label the walk meets twice
(a loop), a label a `JumpTable` or a label operand names, and the
start of the function stop the walk, and the compare stays.

## A compare with the byte an add started from

After `LDA m; CLC; ADC x`, the sum wraps below `m` exactly when the
add carries out, so `CMP m` sets C to the opposite of the add's
carry. A `BCC` or `BCS` right after the compare takes the opposite
condition, and the compare goes:

```
   LDA   Z_PC                       LDA   Z_PC
   CLC                              CLC
   ADC   offset_0          ->       ADC   offset_0
   STA   new_lo                     STA   new_lo
   CMP   Z_PC                       BCC   .if_end@11
   BCS   .if_end@11
```

That's `if (new_lo < lo) pages++;` after `new_lo = lo + offset`, the
C for the carry of a byte add, in Zork's `branch_to_offset`
(examples/zork/decode.c). Stores from A may come between
the add and the compare, as long as none writes `m`. The add sets N
and Z from the sum where the compare set them from the difference, so
N and Z must be dead after the branch on both paths
(`_carry_of_add`). V comes out the same, since `CMP` leaves it alone.
It saves 2 bytes and 3 cycles with `m` in zero page.

The pass doesn't check for a volatile operand. According to the
module docstring, none can reach it, because `volatile_void_read_cmp`
turns a volatile read into a compare only when the flags are dead, so
a compare of a volatile operand is never followed by a branch.

## What it leaves alone

- A label that the code above falls into. In `if (key < entry) g =
  1; if (key == entry) g = 2;` the `LDY #$01; STY g` falls through
  into `.if_end@0`, so the second compare stays, even though A holds
  `key` on both ways in.
- A label reached by a second branch or jump, which could arrive with
  other values in A or the flags.
- Two compares with different operands or registers.

## Related

[Redundant load](redundant_load.md) drops the second `LDA key` that
would otherwise sit between the label and the compare.
