# Element stepped in place

`apply_element_in_place` in
[cpp/src/passes/element_in_place.h](../../cpp/src/passes/element_in_place.h)
rewrites a step of a slot that holds a copy of an array element, `DEC
t; LDA t; STA tab,X`, as a step of the element, `DEC tab,X; LDA
tab,X`, and makes each read of the slot that the new value reaches
read the element. The step may be `INC`, `DEC`, `ASL`, `LSR`, `ROL`,
or `ROR`. It runs in the peephole fixed point under `-O`, after
[DEC](dec_peephole.md) and [multi-byte INC](inc_peephole.md) have
made the step, and before [load after
read-modify-write](redundant_load_after_rmw.md) and [asm dead-store
elimination](asm_dead_store.md), which remove the load and the copy
it leaves unused.

## Example

`uint8_t timer = --BRICK_FILL_TIMERS[i];` in Lode Runner's
`handle_timers` (examples/lode_runner/timers.c) tests the timer for
zero first, and compares it with 20 and 10 later. The allocator kept
`timer` in a slot, which the code loaded from the element, stepped,
and stored back.

### Before

```
   LDX   __local_handle_timers__i
   LDA   BRICK_FILL_TIMERS,X
   STA   __local_handle_timers__0
   BNE   .lb_skip@3
   JMP   .loop@3_continue
.lb_skip@3:
   DEC   __local_handle_timers__0
   LDA   __local_handle_timers__0
   STA   BRICK_FILL_TIMERS,X
   BEQ   .lb_skip@2
   JMP   .if_end@21
   ...
.if_end@21:
   LDA   BRICK_DIG_COLS,X
   STA   GAME_COLNUM
   LDA   BRICK_DIG_ROWS,X
   STA   GAME_ROWNUM
   LDA   __local_handle_timers__0
   CMP   #$14
```

### After

```
   LDX   __local_handle_timers__i
   LDA   BRICK_FILL_TIMERS,X
   BNE   .lb_skip@3
   JMP   .loop@3_continue
.lb_skip@3:
   DEC   BRICK_FILL_TIMERS,X
   BEQ   .lb_skip@2
   JMP   .if_end@21
   ...
.if_end@21:
   LDA   BRICK_DIG_COLS,X
   STA   GAME_COLNUM
   LDA   BRICK_DIG_ROWS,X
   STA   GAME_ROWNUM
   LDA   BRICK_FILL_TIMERS,X
   CMP   #$14
```

This is the original's code. The `LDA` after the `DEC` went because A
is dead on both of its paths, and the `STA` into the slot went
because nothing reads the slot. `handle_timers` is 5 bytes shorter.

## When it applies

- The slot is one of the function's own `__local_<fn>__*` bytes, its
  address is never formed, and the slot map gives its address. The
  map isn't passed in the unoptimized pipeline, whose locals are on
  the soft stack, so the pass does nothing there.
- The step, the load of the slot, and the store of A into the element
  are adjacent, and the store isn't volatile. The element is indexed
  by X, since the 6502 has no abs,Y form of a step.
- The slot holds the element's value before the step, on every path.
  A forward dataflow follows which of A, the slot, and the element
  hold one value. A non-volatile load of the element or the slot
  makes A hold that byte's value, and a store of A makes the stored
  byte hold whatever A holds. A write of X ends what's known of the
  element, since the element is then another byte, and so do a call
  and a store that may reach it. A store to a zero-page object, such
  as `GAME_COLNUM` above, can't reach an element of an array outside
  zero page indexed from its start, which is at `$100` or above.
- Every read of the slot that the stepped value reaches before the
  slot is next written is another such step, or a load (into A or Y,
  or through A into memory), a `CMP`, or an `ADC`, `SBC`, `AND`,
  `ORA`, or `EOR` with the slot as the operand, before which the slot
  holds the element's value. Those read the element instead. A read
  into X or a `CPX` can't, since there's no `LDX abs,X` or `CPX
  abs,X`, and neither can a read of the slot as a byte of a `(zp),Y`
  pointer.
- The rewrite saves bytes. The step and its load are a byte shorter,
  and 3 bytes more when A is dead after them and the load goes. Each
  read that becomes a read of the element is a byte longer. The store
  into the slot that dies isn't counted.

## Why the rewrite gives the same results

The element ends up with the value the store gave it, and the load
after the step loads the value the slot's load did. X doesn't change
within the three instructions. The step sets N and Z (and for a
shift, C) from the same value as the step of the slot did, and V and
C (for `INC` and `DEC`) are left alone either way. Every other
instruction runs as before, and the reads that now name the element
read the value the slot held there. Afterwards the slot keeps the
value it had before the step, and no read of the slot is reached
before it's written again or the function ends.

## What it leaves alone

- A slot copied from the element but stepped through A, `LDA t; SEC;
  SBC #1; STA t; STA tab,X`, until [DEC](dec_peephole.md) has made
  the step a `DEC`.
- A store to another array or to a static outside zero page between
  the step and a read. `may_alias` takes two names that aren't
  compiler slots to be possibly the same memory, since a fixed
  address can put one inside another.
- A 2-byte element, `w[i]++`, whose bytes are stepped in two slots
  with a branch between them.

## Related

[DEC](dec_peephole.md), [multi-byte INC](inc_peephole.md), and [shift
in A](shift_in_a.md) step an element in place when the code loads and
stores it in adjacent instructions.
[Rematerialization of staged values](asm_remat.md) reads a staged
value's source again in one block, for a source the function never
writes.
