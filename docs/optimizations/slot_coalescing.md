# A parameter's copy in the parameter's slot

`apply_slot_coalescing` in
[passes/slot_coalescing.py](../../passes/slot_coalescing.py) renames
a zero-page local that holds a copy of a parameter to the parameter's
own slot, when the two are never live at once with different values.
It runs once under `-O`, after the third peephole fixed point
and `carry_known`, and before [loop-invariant compare key in
A](loop_invariant_a.md).

A function that changes a parameter works on a copy of it. The asm
SSA gives each assignment of the parameter a new version, and the
register allocator puts a version that doesn't go in a register in a
byte of the function's pool. So the function starts by copying the
parameter's bytes into its pool, and the parameter's slot is never
read again. After the rename, the copy stores a byte into itself, and
the pass drops the store. A version the allocator put in a register
stays there. In Zork's `numbers.c` (examples/zork/interpreter), this
took 16 bytes of copies out of `mulu16`, the copy of `dividend` into
`quotient` out of `divu16`, and the blocks that copied the parameter
on the path that leaves it alone out of `check_sign` and
`print_number`.

## Example

```c
#include <stdint.h>
static uint16_t mul(uint16_t a, uint16_t b) {
    uint16_t product = 0;
    while (b != 0) {
        if (b & 1)
            product += a;
        a <<= 1;
        b >>= 1;
    }
    return product;
}
```

### Before

```
mul:
   LDA   #$00
   STA   __local_mul__product
   LDX   #$00
   LDA   __zpabi_mul__a_0
   STA   __local_mul__a_0
   LDA   __zpabi_mul__a_1
   STA   __local_mul__a_1
   LDA   __zpabi_mul__b_0
   STA   __local_mul__b_0
   LDA   __zpabi_mul__b_1
   STA   __local_mul__b_1
   JMP   .loop@0_continue
   ...
   ASL   __local_mul__a_0
   ROL   __local_mul__a_1
   LSR   __local_mul__b_1
   ROR   __local_mul__b_0
```

### After

```
mul:
   LDA   #$00
   STA   __local_mul__product
   LDX   #$00
   JMP   .loop@0_continue
   ...
   ASL   __zpabi_mul__a_0
   ROL   __zpabi_mul__a_1
   LSR   __zpabi_mul__b_1
   ROR   __zpabi_mul__b_0
```

## When it applies

- The function is zp_abi, and the slot is one of its own parameter
  bytes. It isn't a register parameter's byte or a `zeropage(addr)`
  one.
- The slot isn't in a share ([values kept in a callee's parameter
  bytes](slot_sharing.md)). A share keeps a caller's value in the
  slot across the call, which the function's copy leaves alone, and
  saves the caller a copy before each call. In
  `tests/test_slot_sharing.py`, `main` passes one pointer to `sum`
  twice and keeps it in `sum`'s slot, so `sum` keeps its copy.
- The local is one of the function's `__local_F__*` bytes. Neither
  byte's address is taken in the unit
  (`asm_aliasing.unreachable_by_pointer`), and neither is reached by
  address, by a `ZP` operand or a `(zp),Y` pointer base, which is how
  a local used as a pointer is spelled. Either could change behind a
  rename of its name otherwise. A callee's zero page is disjoint from
  its caller's, so only the function's own instructions read or write
  them.
- Each is named only at offset 0, never by an `IndexedData`, a
  `LoadAddress`, or a volatile access, and only by instructions whose
  reads and writes the pass models. A `volatile` local keeps bytes of
  its own.
- The local is written somewhere with the slot's value, through a
  register or by a memory-to-memory move.
- They don't interfere. No instruction writes one while the other is
  live after it, except a write of one with the other's value.
  Nothing is live at a return or a tail call, since the caller
  doesn't read its arguments back.

Which register holds which byte's value is followed forward through
the function, keeping at a label what is the same on every path into it.
That finds the copies, and after the rename, the stores of a byte into
itself, such as `check_sign`'s `STA a_1` on the path where `a` was
already positive, after the `LDA a_1; BPL` before the branch.

Several locals can join one slot, one at a time, each checked against
everything already in it.

The link metadata lists each slot the pass writes as one its function
assigns (`changes=`), so the linker rejects a unit that declares the
function with a `const` parameter and keeps a value of its own in the
slot (`slot_sharing.check_linked_shares`).

## A callee's parameter slot

The same goes for the bytes a function writes a callee's argument
into. A local copied into a callee's parameter slot for the call can
be that slot from where it's computed. `instr_mul` in Zork's
`numbers.c` passes the product to `store_with_sign`:

```
   JSR   check_sign                        JSR   check_sign
   STA   __local_instr_mul__product        STA   __zpabi_store_with_sign__n_0
   STX   __local_instr_mul__0              STX   __zpabi_store_with_sign__n_1
   ...                               ->    ...
   ASL   __local_instr_mul__product        ASL   __zpabi_store_with_sign__n_0
   ROL   __local_instr_mul__0              ROL   __zpabi_store_with_sign__n_1
   ...                                     ...
   LDA   __local_instr_mul__product        JMP   store_with_sign
   STA   __zpabi_store_with_sign__n_0
   LDA   __local_instr_mul__0
   STA   __zpabi_store_with_sign__n_1
   JMP   store_with_sign
```

A call reads its callee's slots, and so does a tail call's `JMP`. A
call may also write any callee's slots, since functions that are
never active together can share bytes, so the local can't be live
across a call. For the same reason, a store into another function's
slot may write the same byte. The linker allocates the slots again
for the whole program, so two that are apart in this unit can share
a byte there, and only one function's own slots are certainly apart.
So the local can't be live across a store into another function's
slot either. In `tests/test_slot_coalescing.py`, `div`'s argument had
overwritten a local kept in `mul`'s slot, which in that unit was the
same byte.

That's also why a function's locals go into the slots of one callee
at most, the callee whose slots take the most locals. A local merged
into one callee's slot turns its stores into stores into that slot,
which could overwrite a local merged into another callee's. A callee's
slot takes only a local copied into it, since that copy is what goes.

The slot must be one of the parameter bytes of a function the unit
defines, other than a register or `zeropage(addr)` byte, and outside
every share.
The callee may change its own slot, since the local is dead once the
callee has it. The link metadata gets no record of these: the
function writes the callee's slot as it writes any argument.

## Where it runs

After the promotions and peepholes that look for `__local_*` slots,
such as [loop-carried value in A](promote_loop_carried_to_a.md).
When the pass ran before them, `while (s >= k) s -= k;` lost `s` in A
across its loop (`tests/test_compare_subtract.py`). The peephole fixed
points after [loop test at the bottom](rotate_loop_test.md) drop the
loads the copies leave.

## What it leaves alone

- A copy whose source or destination is in a register, which the
  register allocator chose.
- Two locals that copy each other.
- A local kept across a call, or across the store of another
  function's argument. `instr_mul`'s product is copied into `mulu16`'s
  slot on one path, but its value is also passed to `store_with_sign`
  after `mulu16` returns, so only the second copy goes.
- A function that isn't zp_abi, whose parameters are on the soft
  stack.

## Related

[Values kept in a callee's parameter bytes](slot_sharing.md) runs
first and determines which parameter slots are shared, and [move
coalescing](coalescing.md) merges pseudos before register allocation.
