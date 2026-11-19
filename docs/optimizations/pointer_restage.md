# Each byte of a wide access through the pointer

`tac_to_asm` lowers a load or store of a value wider than a byte,
through a pointer at a constant offset, by copying the pointer into
DPTR and reading or writing each byte through `(DPTR),Y`. Under `-O`,
when the pointer is in zero page, it copies the pointer into DPTR
again before each byte after the first (`Translator._restage`, with
the rules in
[passes/pointer_restage.py](../../passes/pointer_restage.py)).
[Memory value propagation](memory_value_propagation.md) then reads or
writes every byte through the pointer itself, and the copies into
DPTR go. It doesn't run in the unoptimized pipeline.

With one copy, the second byte was left going through DPTR for two
reasons. A store through a pointer ends every fact memory value
propagation holds, since the stored byte could be one of the
pointer's, so after the first byte of `P->gold = 0` nothing showed
that DPTR still copied `P`. And the register allocator gave a pointer
parameter's bytes to another value once the copy into DPTR had read
them, so the low byte of `r->gold` went into `r`'s own byte. A copy
before each byte reads the pointer after the earlier bytes, which
keeps it alive to the last one and gives each byte a fact of its own.

## Example

```c
#include <stdint.h>
struct s { uint8_t a[13]; uint16_t g; };
extern struct s *P [[c6502::zeropage(0xF4)]];
void f(void) { P->g = 0; }
uint16_t get(struct s *r) { return r->g; }
```

### Before

```
f:
   LDA   P
   STA   DPTR
   LDA   P+1
   STA   DPTR+1
   LDA   #$00
   LDY   #$0D
   STA   (P),Y
   INY
   STA   (DPTR),Y
   RTS

get:
   STA   __local_get__r
   STX   __local_get__0
   STA   DPTR
   STX   DPTR+1
   LDY   #$0D
   LDA   (__local_get__r),Y
   STA   __local_get__r
   INY
   LDA   (DPTR),Y
   TAX
   LDA   __local_get__r
   RTS
```

### After

```
f:
   LDA   #$00
   LDY   #$0D
   STA   (P),Y
   INY
   STA   (P),Y
   RTS

get:
   STA   __local_get__r
   STX   __local_get__1
   LDY   #$0E
   LDA   (__local_get__r),Y
   TAX
   DEY
   LDA   (__local_get__r),Y
   RTS
```

In `get`, `r` stays in its two bytes to the second read, so the high
byte can be read first, straight into X.

## Why a store may read the pointer again

A load writes nothing between the copies, so the pointer has the value
it had. A store writes its earlier bytes between them, and those could
change the pointer only if the store's bytes overlapped the object
that holds the pointer. C's effective-type rule (C23 §6.5p7) makes
that overlap undefined in the cases the lowering accepts.

- The pointer is a TAC variable of pointer type. Its bytes are an
  object of that type, or a temporary that no C lvalue reaches.
- The store writes the scalar that the pointer's referenced type has
  at exactly those bytes: a member, an element of an array member, or
  the whole referenced object (`scalar_at`).
- That scalar has no character type, which may write any object, and
  its type isn't compatible with the pointer's own type. An integer
  or floating type never is. A pointer type is compared with every
  qualifier removed, and a pointer to an array or a function counts
  as compatible.

The lowering applies the rule to the bytes of one store, whose type
it reads from the pointer's referenced type. A byte store through a
`uint8_t *` may write any object, so a store of a single byte between
two wide ones still ends every fact.

## When it applies

- Under `-O`, to a `Load` / `Store` and to an `IndirectIndexedLoad` /
  `IndirectIndexedStore` with a constant index, of 2 bytes or more.
- The pointer is a TAC local or temporary, a parameter's or local's
  zero-page slot, or a zero-page object (`pointer_in_zero_page`).
  Memory value propagation can remove only a copy of zero-page bytes,
  and a copy it leaves costs 8 bytes.
- A store is non-volatile and meets the type conditions above.

## What it leaves alone

- `N->next = q` on a `struct node *N`, whose member has the pointer's
  own type. `N = (struct node *)&N; N->next = q;` is defined C, and the
  second byte has to go to `N`'s second byte, where the first byte has
  already changed `N`. The copy into DPTR stays.
- A store through a pointer whose referenced type is a union, `void`,
  or a structure without a member at exactly those bytes, and a store
  of one byte.
- A pointer in a static outside zero page or on the soft stack.

A store through a pointer that the program casts to another structure
type (`((struct t *)p)->m`) is judged by the type TAC gives the
pointer, which copy propagation may make `p`'s own. A program that
writes `p`'s bytes through a member of a structure that isn't there is
outside these rules, and the lowering assumes it doesn't occur.

## Related

[Memory value propagation](memory_value_propagation.md) makes each
access go through the pointer, and [a byte written back through its
pointer](pointer_self_store.md) removes the store of a byte that
`P->buf += 0x100` leaves unchanged.
