# Carry-flag return peepholes

[passes/carry_return.py](../../passes/carry_return.py) contains three
peepholes for functions declared
`__attribute__((return_bool_in_carry))`, which return their result in
the carry flag ([attributes.md](../attributes.md) describes the
attribute). `apply_carry_return_tails`, `apply_carry_const_fold`, and
`apply_carry_branch_fold` run one after another in the peephole fixed
point, right after [constant AND and ORA fold](const_arith_fold.md).
They run in both pipelines but only fire with `--optimize`. Without it
every function returns through the soft-stack `Ret`, which the tail
copy doesn't match, and both the callee's `LDA` and the caller's `ROL
A` are followed by a store of A to the soft stack.

`tac_to_asm` lowers both sides of the convention plainly. The callee
ends in `LDA value; CMP #1; RTS`, which sets C when the value is
nonzero, and the caller captures C with `JSR f; LDA #0; ROL A`, which
leaves 0 or 1 in A. The three passes clean up after that.

- `apply_carry_return_tails` copies a shared `CMP #c; RTS` tail into
  each path that loads a constant into A and then jumps or falls into
  the tail. A copy that replaces a `JMP` is the same size, and a copy
  in front of the label adds 3 bytes. Either way the constant ends up
  next to the compare.
- `apply_carry_const_fold` turns `LDA #k; CMP #c` into `SEC` when `k
  >= c` (unsigned) and `CLC` otherwise, when only C is read after it.
  That saves 3 bytes and 2 cycles, and makes `return true;` a `SEC;
  RTS`.
- `apply_carry_branch_fold` turns the caller's `LDA #0; ROL A; BEQ L`
  into `BCC L`, and `BNE L` into `BCS L`, saving 3 bytes and 4
  cycles. It also deletes `LDA #0; ROL A; CMP #1`, which turns C into
  a number and straight back into C. When the caller does other work
  between the call and the test, as with a value-and-flag struct whose
  value it uses first, the capture is stored and loaded back for the
  test. The pass then deletes the capture and its store and branches
  on C at the test, which saves 7 bytes and 10 cycles.

## Branching on a struct's flag

Zork's `get_next_code_byte2` stores the value from a value-and-flag
struct before it tests the flag:

```c
struct page_slot found = find_index_of_page_table();
PAGE_TABLE_INDEX2 = found.slot;
if (found.not_found) {
    if (found.slot == PAGE_TABLE_INDEX)
        ZCODE_PAGE_VALID = 0;
```

### Before

```
   JSR   find_index_of_page_table
   STA   __local_get_next_code_byte2__page
   LDA   #$00
   ROL
   STA   __local_get_next_code_byte2__found_1
   LDA   __local_get_next_code_byte2__page
   STA   PAGE_TABLE_INDEX2
   LDA   __local_get_next_code_byte2__found_1
   BEQ   .if_end@13
   LDA   __local_get_next_code_byte2__page
   CMP   PAGE_TABLE_INDEX
```

### After

```
   JSR   find_index_of_page_table
   STA   PAGE_TABLE_INDEX2
   BCC   .if_end@13
   CMP   PAGE_TABLE_INDEX
```

With the capture gone, A still holds the slot at both loads of
`page`, so [redundant load elimination](redundant_load.md) deletes
them, and [dead store elimination](asm_dead_store.md) the store.

## Example

```c
#include <stdbool.h>
#include <stdint.h>
__attribute__((return_bool_in_carry)) bool is_three(uint8_t x)
{
    return x == 3;
}
__attribute__((return_bool_in_carry)) bool is_three_too(uint8_t x)
{
    return is_three(x);
}
uint8_t g;
void bump(void) { if (is_three(g)) g = 7; }
```

### Before

This listing shows `is_three` as `apply_carry_return_tails` receives
it, and `bump` as `apply_carry_branch_fold` receives it.

```
is_three:
   LDA   #$00
   CMP   #$00
   BNE   .cmp_differ@2
   LDA   __zpabi_is_three__x
   CMP   #$03
.cmp_differ@2:
   BEQ   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   CMP   #$01
   RTS

bump:
   LDA   g
   STA   __zpabi_is_three__x
   JSR   is_three
   LDA   #$00
   ROL
   BEQ   .if_end@0
   LDA   #$07
   STA   g
.if_end@0:
   RTS
```

### After

This listing shows `is_three` after `apply_carry_return_tails` and
then `apply_carry_const_fold` in the same round, and `bump` after
`apply_carry_branch_fold`.

```
is_three:
   LDA   #$00
   CMP   #$00
   BNE   .cmp_differ@2
   LDA   __zpabi_is_three__x
   CMP   #$03
.cmp_differ@2:
   BEQ   .cmp_true@0
   CLC
   RTS
.cmp_true@0:
   SEC
   RTS
.cmp_end@1:
   CMP   #$01
   RTS

bump:
   LDA   g
   STA   __zpabi_is_three__x
   JSR   is_three
   BCC   .if_end@0
   LDA   #$07
   STA   g
.if_end@0:
   RTS
```

The tail at `.cmp_end@1` is left with no path into it.
[Dead label drop](dead_label_drop.md) deletes the label and
[unreachable code drop](unreachable_drop.md) deletes the tail in the
next round. In `is_three_too`, `JSR is_three; LDA #$00; ROL A; CMP
#$01; RTS` becomes `JSR is_three; RTS`, which [tail call](tail_call.md)
then turns into `JMP is_three`.

## When it applies

### Tail copies

- An `LDA #k` followed by a `JMP` to a label in the function. After
  any further labels, the code there is `CMP #c` and a bare `Return`
  that returns C and returns nothing in A or X. The `JMP` is replaced
  by a copy of the two instructions.
- An `LDA #k` followed by such a label. A copy goes in front of the
  label, and the label and tail stay for the other paths.

### Constant compares

- `LDA #k` immediately followed by `CMP #c`, with A dead after them
  (`asm_liveness.a_dead_at`) and N and Z dead (`flags_dead_at`). The
  pass doesn't check that the function returns in C. A `BCC`, `BCS`,
  `BVC`, or `BVS` doesn't read N or Z, so the check follows both of
  its paths.

### Caller captures

- `LDA #0; ROL A`, then `BEQ` or `BNE` to a label in the function,
  with A, N, Z, and C dead both on the fall-through and at the label.
  After `ROL A`, A holds the old C, so `BEQ` is taken exactly when C
  was clear. The rewrite leaves C as the callee set it, where the
  original left C clear and N and Z set from the 0 or 1, which is why
  all of them have to be dead on both paths.
- `LDA #0; ROL A; CMP #1`, with A and N and Z dead after it. The
  `CMP #1` on the 0 or 1 in A sets C to the value `ROL` read.
- `LDA #0; ROL A; STA flag`, then instructions that don't read or
  write C (loads, stores, transfers, `INC`, `DEC`, `AND`, `ORA`,
  `EOR`, `BIT`) and don't touch `flag`, then `LDA flag` and `BEQ` or
  `BNE`, or `LDA flag` and `CMP #1`, which turns the flag back into C
  as `return failed;` does when `failed` holds a carry-flag call's
  result. Then the reload and the compare go too. A and N and Z have to be dead after the store, since they
  held the capture; A, N, Z, and C dead on both sides of the branch,
  as above; and `flag` dead after the load
  (`asm_dead_store.mem_dead_after`), since its store goes.

## What it leaves alone

- A return through the soft-stack frame (`Ret`), as in a recursive
  carry-flag function. Copying the tail would copy the whole frame
  teardown.
- A return that also carries a value in A or X, such as a function
  returning a value-and-flag struct. The tail copy doesn't match such
  a `Return`, and the constant fold leaves `LDA #k` alone while A is
  live there.
- A captured flag that is used as a number, as in `g = is_three(h);`,
  where `LDA #$00; ROL A; STA g` stays.
- A capture whose C a later instruction reads. After `ROL A` the
  original has C clear, and an `ADC` on the fall-through path would
  add that 0, where after the rewrite it would add the callee's C.
- A stored capture with a call, a branch, a label, or anything that
  changes C between it and the test, such as `put(c.v); if (c.end)`.
  The flag has to wait in memory there.

## Related

[Constant AND and ORA fold](const_arith_fold.md) runs just before
these passes. [Unreachable code drop](unreachable_drop.md) removes
the original tail once every path has its own copy.
