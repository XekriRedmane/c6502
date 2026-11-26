# A byte written back through its pointer

`apply_pointer_self_store_drop` in
[cpp/src/passes/pointer_self_store.h](../../cpp/src/passes/pointer_self_store.h)
follows, within a basic block, which bytes hold a byte read through a
zero-page pointer, `(N),Y` with `Y = k`. It makes two rewrites.

- A store of A through `(N),Y` with `Y = k`, when A holds the byte
  read from there, goes. The memory already holds that value.
- A read of one of the function's slots `t` that holds such a byte
  becomes a read through `(N),Y` again, when the byte was copied into
  `t` right after it was read, with A dead after the copy, nothing
  has read `t` since, and `t` and Y are dead after the read. The read
  that filled `t` and the store into `t` are then dead, and later
  sweeps remove them, so the byte is read once, where it's used.

It runs in the peephole fixed point right after the
[self-store drop](self_store_drop.md), which handles `LDA M; STA M`
on a byte with an address. It finds nothing in the unoptimized
pipeline, where a pointer isn't in zero page.

## Example

```c
#include <stdint.h>
struct iob { uint8_t pad[8]; uint8_t *buffer; };
extern struct iob *P [[c6502::zeropage(0x08)]];
void step(void) { P->buffer += 0x100; }

struct mob { uint8_t b[13]; uint16_t gold; };
extern struct mob *E [[c6502::zeropage(0xF8)]];
extern uint16_t W [[c6502::address(0x5A02)]];
uint8_t c(void) { return W <= E->gold; }
```

### Before

```
step:
   LDY   #$08
   LDA   (P),Y
   STA   __local_step__1
   INY
   LDA   (P),Y
   STA   __local_step__0
   INC   __local_step__0
   LDA   __local_step__1
   DEY
   STA   (P),Y
   LDA   __local_step__0
   INY
   STA   (P),Y
   RTS

c:
   LDY   #$0D
   LDA   (E),Y
   STA   __local_c__1
   INY
   LDA   (E),Y
   STA   __local_c__0
   LDA   __local_c__1
   SEC
   SBC   W
   LDA   __local_c__0
   SBC   W+1
   LDA   #$00
   ROL
   RTS
```

### After

```
step:
   CLC
   LDY   #$09
   LDA   (P),Y
   ADC   #$01
   STA   (P),Y
   RTS

c:
   LDY   #$0D
   LDA   (E),Y
   SEC
   SBC   W
   INY
   LDA   (E),Y
   SBC   W+1
   LDA   #$00
   ROL
   RTS
```

In `step`, the low byte of the pointer member is stored back as it was
read. Once that store goes, the read of the low byte and its copy are
dead, and [shift in A](shift_in_a.md) makes the `INC` of the copy an
`ADC #1` on A. In `c`, both bytes of `E->gold` are read again where
the subtraction reads them, and the copies go.

## What ends a fact

A fact that A or a byte holds the byte at `(N)+k` ends at

- a write of `N` or `N + 1`, which moves the pointer,
- a write that `asm_aliasing.may_alias` says may reach `(N)+k`, with
  the unit's bytes that no pointer reaches installed by
  `pointer_proof`. A store through another pointer, or to a static
  outside zero page or a zero-page object, may reach it. A store to
  one of the function's slots whose address the unit never takes, or
  through `N` with another `Y`, doesn't, since `(N)+k'` differs from
  `(N)+k` while `N` is unchanged,
- a call, a compound atom, a label, or a branch.

A write of a byte ends what that byte held, and a write of A ends what
A held. A volatile access makes no fact, and a volatile store stays.

## Soundness

A store dropped by the first rewrite writes the value the byte already
holds, and `STA` writes no register or flag. A read made by the second
rewrite reads a byte whose value the copy in `t` still holds, and sets
N and Z from it as the read of `t` did. Its `LDY #k` writes Y, which
must be dead after the read.

## Related

[Each byte of a wide access through the
pointer](pointer_restage.md) makes the `(N),Y` operands of a wide
member, and [memory value propagation](memory_value_propagation.md)
makes them from `(DPTR),Y`.
