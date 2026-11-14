# Members of a static object computed in place

`plan_in_place_updates` in
[passes/in_place_member_update.py](../../passes/in_place_member_update.py)
finds the values that can be computed straight into a member of a
static struct, or into an element of a static array at a constant
index, and `tac_to_asm` writes them there instead of into a temporary
that's then copied in. For `s.m++` it reads the member in place too,
so the add becomes `INC`. It applies only under `--optimize`, since
the unoptimized pipeline reaches a member through a pointer.

The TAC reads such a member with `IndexedLoad` and writes it with
`IndexedSymbolStore`, so `P.lo++` is a load into a temporary, an add
into another, and a store back. Lowered as written, the add works on
temporaries, and the [INC peephole](inc_peephole.md), which needs
`LDA M; ADC #1; STA M` on one byte, never sees its pattern: a 2-byte
`P.lo++` costs 25 bytes where `INC P; BNE; INC P+1` takes 8. A value
computed from elsewhere, such as `P.lo = W << 1`, at least computes
into a temporary and copies it into the member.

`tac_to_asm` names the temporaries by the member's bytes
(`Data(P, k)`, through the same renaming it uses for values kept in
a callee's parameter bytes) and leaves out the store, and for an
update the load as well.

## Example

```c
#include <stdint.h>
struct pc { uint16_t lo; uint8_t hi; } P;
uint16_t W;
void step(void) { P.lo++; }
void load(void) { P.lo = W << 1; }
```

The TAC of the two functions:

```
IndexedLoad(P, 0, %1.1, 0)
Binary(+, %1.1, 1, %2.1)
IndexedSymbolStore(P, 0, %2.1, 0)

Binary(<<, W, 1, %3.1)
IndexedSymbolStore(P, 0, %3.1, 0)
```

These listings compare two complete compiles.

### Before

```
step:
   LDA   P
   STA   __local_step__1
   LDA   P+1
   STA   __local_step__0
   LDA   __local_step__1
   CLC
   ADC   #$01
   STA   P
   LDA   __local_step__0
   ADC   #$00
   STA   P+1
   RTS

load:
   LDA   W
   ASL
   STA   __local_load__0
   LDA   W+1
   ROL
   STA   P+1
   LDA   __local_load__0
   STA   P
   RTS
```

### After

```
step:
   INC   P
   BNE   .inc_done@0
   INC   P+1
.inc_done@0:
   RTS

load:
   LDA   W
   ASL
   STA   P
   LDA   W+1
   ROL
   STA   P+1
   RTS
```

## When it applies

The value `u` is stored by an `IndexedSymbolStore` with a constant
index, right after the instruction that defines it, which is a
`Binary`, `Unary`, `Copy`, extend, or truncate. Moving the member's
write from the store back to that instruction is safe when:

- the store isn't volatile, and the object has static storage and
  isn't already kept somewhere else;
- `u` is a local of the function, defined only there and read only
  by the store, not a parameter, and its address isn't taken;
- every byte of `u` is computed (`significant_bytes` and
  `low_byte_only` don't narrow it);
- the instruction reads nothing of the object directly, so writing
  the member while computing `u` can't change what it reads. A
  temporary loaded from the object before it holds a copy, which is
  fine.

It's an update, with the member read in place as well, when the
instruction is `t + c` or `t - c` with `c` a constant and:

- `t` was loaded from the bytes the store writes, by a load that
  isn't volatile, and is the same size as `u`;
- `t` is a local defined only by that load and read only after it,
  up to the add;
- between the load and the add there's no label, jump, call, store,
  or other write to the object, only instructions that compute into
  locals and read memory, so the member still holds `t` wherever `t`
  is read.

## What it leaves alone

- A loaded value read after the store, as in `uint16_t old = P.lo;
  P.lo = old + 1; return old;`, which needs the old value.
- A store through a pointer between the load and the add, which
  might write the member.
- A volatile member, and a member of an automatic struct, which
  [direct member access](direct_member_access.md) already reads and
  writes in place.

## Related

The [INC peephole](inc_peephole.md) and [DEC](dec_peephole.md) turn
the in-place add or subtract into `INC` or `DEC`, and [testing the
low byte after an INC chain](inc_low_byte_test.md) folds a test of
the incremented low byte into the chain's branch.
