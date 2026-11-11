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
   LDA   __zpabi_classify__key
   CMP   __zpabi_classify__entry
   BCS   .if_end@0
   LDA   #$01
   RTS
.if_end@0:
   CMP   __zpabi_classify__entry
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
   LDA   __zpabi_classify__key
   CMP   __zpabi_classify__entry
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
  in the run. It must be a `Branch`, and the instruction just before
  it must be a compare equal to the one being deleted, with the same
  register and the same operand.
- No label in the run has its address taken by an `ImmLabelLow` or
  `ImmLabelHigh` operand.

The pass doesn't check for a volatile operand. According to the
module docstring, none can reach it, because `volatile_void_read_cmp`
turns a volatile read into a compare only when the flags are dead, so
a compare of a volatile operand is never followed by a branch.

## What it leaves alone

- A label that the code above falls into. In `if (key < entry) g =
  1; if (key == entry) g = 2;` the `STA g` falls through into
  `.if_end@0` with 1 in A, so the code reloads `key` there and the
  second compare stays.
- A label reached by a second branch or jump, which could arrive with
  other values in A or the flags.
- Two compares with different operands or registers.

## Related

[Redundant load](redundant_load.md) drops the second `LDA key` that
would otherwise sit between the label and the compare.
