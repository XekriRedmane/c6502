# Carry INC

`apply_carry_inc_peephole` in
[passes/carry_inc.py](../../passes/carry_inc.py) rewrites the bytes
of an add that only take in the carry, `LDA M; ADC #0; STA M`, as a
`BCC` around an `INC M`. It does the same for the top byte of a
subtract, which becomes a `BCS` around a `DEC M`. It runs in the
peephole fixed point, in both pipelines, but in practice only the
optimized pipeline produces the shape it matches.

When a value of one byte is added to a wider one, as in
`t + (uint16_t)c` or `p + 53`, each byte above the lowest has a zero
for its other operand, so `tac_to_asm` lowers it as an `ADC #0` that
adds the carry out of the byte below. When that byte is updated in
place, the carry can be added with `BCC skip; INC M; skip:` instead.
In zero page that's 4 bytes instead of 6, and 3 cycles when the
branch is taken or 7 when it isn't, against 8. At an absolute address
it's 5 bytes instead of 8, and 3 or 8 cycles against 10. A subtract's
`SBC #0` takes the borrow out of the top byte, and `BCS skip; DEC M;
skip:` does the same.

A wider add has a run of carry bytes. A byte that `INC` wraps to zero
is the only case that carries into the next byte, so a run in place
becomes one chain of `INC`s, with a `BNE` after each one that skips
the rest. This is the chain that [multi-byte INC](inc_peephole.md)
uses for `x += 1`.

## Example

```c
#include <stdint.h>
static uint16_t total;
void add(uint8_t c) { total += c; }
```

### Before

```
   LDA   total
   CLC
   ADC   __zpabi_add__c
   STA   total
   LDA   total+1
   ADC   #$00
   STA   total+1
   RTS
```

### After

```
   LDA   total
   CLC
   ADC   __zpabi_add__c
   STA   total
   BCC   .carry_inc@0
   INC   total+1
.carry_inc@0:
   RTS
```

With a `uint32_t big`, `big += c` has three carry bytes, and they
become `BCC .carry_inc@1; INC big+1; BNE .carry_inc@1; INC big+2; BNE
.carry_inc@1; INC big+3; .carry_inc@1:`. With `total -= c`, the
`LDA total+1; SBC #$00; STA total+1` becomes `BCS .carry_inc@0; DEC
total+1; .carry_inc@0:`.

## When it applies

Each byte must be the three atoms `Mov(M, A)`, `Add(Imm(0), A)` or
`Sub(Imm(0), A)`, and `Mov(A, M)`, where the store writes the same
byte the load reads. M must be a `Data` or `ZP` operand that `INC`
can address, or X or Y, where the rewrite produces `BCC skip; INX`.
Neither the load nor the store may be volatile, since `INC` is a
single read-modify-write access where the `ADC` form reads and writes
separately.

The two forms leave A and the flags in different states, so A, N, Z,
C, and V must all be dead after the last store, along every path
(`asm_liveness.a_dead_at`, `flags_dead_at`, `c_dead_at`, and
`v_dead_at`). The `ADC` form leaves the new top byte in A, while the
`INC` form leaves A as it was before the load, which in the example
is the low byte of the sum. The `ADC` form
sets C from the carry out of M, while the `INC` form passes the
incoming C through.

An add takes every carry byte in a row as one run. A subtract only
has its top byte rewritten. A byte that `DEC` wraps to `$FF` is the
one that borrows from the next, and one branch after the `DEC` can't
detect that. The lower `SBC #0` bytes also fail the check that C is
dead, since the next byte's `SBC` reads it.

Move coalescing assigns both ends of a carry byte to one slot when it
can ([move coalescing](coalescing.md)), so a sum kept in a local
reaches this pass in place. An add to a global, `g += c`, has the
shape already.

## What it leaves alone

- A sum stored somewhere other than its source. For `total = other +
  c`, the high byte is `LDA other+1; ADC #$00; STA total+1`, and the
  pass reports 0 changes.
- A byte on the soft stack or behind a pointer, because `INC` has no
  `(zp),Y` mode.
- A carry byte followed by a read of A, C, or V, or by a branch on
  N or Z. A carry byte in the middle of a subtract is one case.
- The unoptimized pipeline's adds. There, the zero high byte of the
  widened operand sits in a soft-stack temporary, so the high byte is
  an `ADC (FP),Y`, which the pass doesn't match. For `total += c` with an
  `unsigned char c`, the pass reports 0 changes under `--unopt`.

A volatile variable updated in place is read into a temporary first,
and the carry byte of that temporary still qualifies. For
`vt += c` with a `volatile uint16_t vt`, the pass rewrites the
temporary's high byte to `BCC .carry_inc@0; INC __local_addv__1`,
and the two stores to `vt` stay.

## Related

[Multi-byte INC](inc_peephole.md) handles `x += 1`, where the lowest
byte is also an add of a constant 1.
