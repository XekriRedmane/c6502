# A choice of offsets from one pointer

`PhiOffset` in
[cpp/src/passes/optimization/phi_offset.h](../../cpp/src/passes/optimization/phi_offset.h)
rewrites a join that chooses between `p + k1` and `p + k2`, for one
pointer `p` and constants from 0 to 255, as a choice between `k1` and
`k2` that the join adds to `p`. It runs once after the TAC fixed
point, before [indexed access to a static array of
structs](static_indexed_access.md) and [indirect-indexed
access](recognize_indirect_indexed.md), which make `p +
ZeroExtend(o)` an indexed access. It doesn't run in the unoptimized
pipeline.

A pointer to one of two members of the record a zero-page pointer
points at, `n < 3 ? &E->sword : &E->dagger`, was a 16-bit add of a
constant to both bytes of `E` in each arm, and a store through the
sum. After the rewrite each arm loads its member's offset into Y, and
the store goes through `(E),Y`.

## Example

```c
#include <stdint.h>
struct mob { uint8_t pad[7]; uint8_t sword, armor, dagger; };
extern struct mob *E [[c6502::zeropage(0xF8)]];
void set(uint8_t n, uint8_t v) {
    uint8_t *w = n < 3 ? &E->sword : &E->dagger;
    *w = v;
}
```

In SSA form the arms compute `Binary(+, E, 7, %t1)` and `Binary(+, E,
9, %t2)`, and the join is `%w = Phi(%t1, %t2)`. The pass makes the
join `%w.o = Phi(7, 9)`, `ZeroExtend(%w.o, %w.e)`, and `Binary(+, E,
%w.e, %w)`, and drops the two adds.

### Before

```
set:
   CMP   #$03
   BCS   .cond_else@0
   LDA   E
   ADC   #$07
   STA   __local_set__n
   LDA   E+1
   ADC   #$00
   STA   __local_set__0
   JMP   .cond_end@1
.cond_else@0:
   LDA   E
   CLC
   ADC   #$09
   STA   __local_set__n
   LDA   E+1
   ADC   #$00
   STA   __local_set__0
.cond_end@1:
   TXA
   LDY   #$00
   STA   (__local_set__n),Y
   RTS
```

### After

```
set:
   CMP   #$03
   BCS   .cond_else@0
   LDY   #$07
   BNE   .cond_end@1
.cond_else@0:
   LDY   #$09
.cond_end@1:
   TXA
   STA   (E),Y
   RTS
```

## When it applies

- Every argument of the Phi is an SSA temporary that only the Phi
  reads (on one or more of its edges), defined before it by `p + k` or
  `k + p` with the same variable `p` in each and an integer constant
  `k` from 0 to 255.
- `p` has the same value at the join as at each add. An SSA-renamed
  `p` has one value. For any other `p`, such as a zero-page pointer
  global, the instructions from the first add to the Phi write no
  memory, call nothing, and don't write `p`, and each jump among them
  goes to a label among them, so no path from an add to the join
  leaves that range.
- The Phi's result and `p` are 2 bytes wide, `p`'s type isn't
  `volatile`, whose read would move, and the result's one reader is a
  load or store through it, which becomes `(p),Y`.

A 16-bit add wraps the same way whichever operand is added first, so
`p + ZeroExtend(k)` is `p + k`.

## What it leaves alone

- Arms with different bases, such as `c ? &E->a : &F->b`, and an arm
  whose value isn't an add of a constant.
- An offset above 255, or one that isn't constant.
- A call, a store, or a write of `p` between an add and the join, as
  in `w = &E->a; if (n > 5) { E = q; w = &E->b; }`.
- A pointer read more than once. `calc_combat_strength` in
  examples/ali_baba/combat.c reads `*weapon` on either side of a call
  that may move `ENTITY_PTR`, so the pointer is computed in 16 bits
  anyway, and a choice of an offset would only add a step.

## Related

[Indirect-indexed access through a
pointer](recognize_indirect_indexed.md) turns the store through `p +
ZeroExtend(o)` into `(p),Y`, and [indexed access to a static array of
structs](static_indexed_access.md) turns it into `name,X` when `p` is
a static array's address.
