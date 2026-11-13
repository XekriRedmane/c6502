# Values kept in a callee's parameter bytes

`find_shares` in [passes/slot_sharing.py](../../passes/slot_sharing.py)
runs under `--optimize` after ABI selection and before the parameter
slots get addresses. It finds values that can live in the zero-page
bytes of a callee's parameter instead of in the caller's own pool:

- A parameter that a function passes on unchanged to another
  function's parameter. The two slots get the same address, so the
  function's callers write the value where the callee reads it, and
  the function copies nothing.
- A local that a function passes to a callee and still needs after
  the call, when the callee leaves its parameter alone. The function
  keeps the local in the callee's bytes, so it doesn't copy it there
  before each call. A local that a copy links to it, and that's never
  live at the same time, goes there too, so `p += 0x100` in a loop
  updates the bytes in place.

This is what Zork's code does by hand with its global `SCRATCH2`: the
routines it calls read the pointer there and leave it alone, so a
loop advances the pointer in `SCRATCH2` and passes it again without
copying. With the pointer passed as an argument, the compiler finds
the same arrangement, and checks that it's safe.

`tac_to_asm` writes each such variable as the slot's `Data` operand,
so the argument store before a call becomes a store of a byte to
itself, which it drops. The allocators then give each set of shared
slots one address, kept off the bytes of every function that can be
on the call stack with any function using it.

## Example

```c
#include <stdint.h>

uint16_t sum;

static void add_page(const uint8_t *p) {
    uint8_t i = 0;
    do { sum += p[i]; i++; } while (i != 0);
}

static void forward(const uint8_t *p) { add_page(p); }

void add_pages(const uint8_t *p, uint8_t pages) {
    do {
        forward(p);
        p += 0x100;
    } while (--pages != 0);
}
```

The "Before" listing is this program compiled with `--optimize` and
`find_shares` replaced by one that finds nothing. The "After" listing
is the normal compile. `add_page` is the same in both, so it's left
out.

### Before

```
__zpabi_add_pages__p_0	EQU	$80
__zpabi_add_pages__p_1	EQU	$81
__zpabi_add_pages__pages	EQU	$82
__zpabi_forward__p_0	EQU	$83
__zpabi_forward__p_1	EQU	$84
__zpabi_add_page__p_0	EQU	$85
__zpabi_add_page__p_1	EQU	$86
__local_add_pages__pages	EQU	$87
__local_add_pages__p_1	EQU	$88
__local_add_pages__p_0	EQU	$89

forward:
   SUBROUTINE

   LDA   __zpabi_forward__p_0
   STA   __zpabi_add_page__p_0
   LDA   __zpabi_forward__p_1
   STA   __zpabi_add_page__p_1
   JMP   add_page

add_pages:
   SUBROUTINE

   LDA   __zpabi_add_pages__p_0
   STA   __local_add_pages__p_0
   LDA   __zpabi_add_pages__p_1
   STA   __local_add_pages__p_1
   LDA   __zpabi_add_pages__pages
   STA   __local_add_pages__pages
.loop@1_start:
   LDA   __local_add_pages__p_0
   STA   __zpabi_forward__p_0
   LDA   __local_add_pages__p_1
   STA   __zpabi_forward__p_1
   JSR   forward
   LDA   __local_add_pages__p_0
   CLC
   ADC   #$00
   STA   __local_add_pages__p_0
   LDA   __local_add_pages__p_1
   ADC   #$01
   STA   __local_add_pages__p_1
   DEC   __local_add_pages__pages
   BNE   .loop@1_start
   RTS
```

### After

```
__zpabi_add_pages__p_0	EQU	$80
__zpabi_add_pages__p_1	EQU	$81
__zpabi_add_pages__pages	EQU	$82
__zpabi_add_page__p_0	EQU	$83
__zpabi_forward__p_0	EQU	$83
__zpabi_add_page__p_1	EQU	$84
__zpabi_forward__p_1	EQU	$84
__local_add_pages__pages	EQU	$85

forward:
   SUBROUTINE

   JMP   add_page

add_pages:
   SUBROUTINE

   LDA   __zpabi_add_pages__p_0
   STA   __zpabi_forward__p_0
   LDA   __zpabi_add_pages__p_1
   STA   __zpabi_forward__p_1
   LDA   __zpabi_add_pages__pages
   STA   __local_add_pages__pages
.loop@1_start:
   JSR   forward
   LDA   __zpabi_forward__p_0
   CLC
   ADC   #$00
   STA   __zpabi_forward__p_0
   LDA   __zpabi_forward__p_1
   ADC   #$01
   STA   __zpabi_forward__p_1
   DEC   __local_add_pages__pages
   BNE   .loop@1_start
   RTS
```

`forward` passes its parameter on in `add_page`'s bytes and shrinks
from 11 bytes to 3. `add_pages` keeps `p` in `forward`'s bytes, which
`forward` and `add_page` leave alone, so the loop loses the 8-byte,
12-cycle copy before each call, and the program uses 6 zero-page
bytes instead of 10. `add_pages` can't share its own parameter `p`
with `forward`'s, since it changes it, so it copies it once before
the loop.

## When it applies

Every function involved has a [private local pool](zp_local_pools.md)
and isn't on a cycle. The callee whose parameter holds the value never
assigns that parameter or takes its address, and isn't a routine with
a fixed address. The parameter is an integer or a pointer, isn't
passed in a register or at a fixed address, and has the variable's
size. The variable isn't `volatile` or `register`, and its address
isn't taken.

A function *writes* the bytes when it calls one of the functions whose
parameter is in them and passes anything but its own value there, or
when it assigns the value it keeps there. For each variable kept in
the bytes:

1. A call to one of those functions passes the variable, or the
   variable is dead after the call and isn't one of its arguments.
2. No call the variable is live across reaches a function that writes
   the bytes, other than the variable's own.
3. When one of the functions whose parameter is in the bytes has
   external linkage, no call the variable is live across reaches a
   function defined in another unit, since that function might call
   it.

A variable that breaks a rule isn't kept there, and the rules are
checked again, since dropping a shared parameter makes its function
write the bytes. Each function keeps at most one value in a set of
shared bytes.

## Across units

A callee defined in another unit qualifies when its declaration makes
the parameter `const`:

```c
bool inc_sector_and_write(uint8_t *const buffer);
```

c6502 requires a definition to match its declarations exactly, so the
definition can't assign the parameter either. The compiler takes the
declaration as a promise that nothing the callee calls writes the
bytes, which it can't check in the caller's unit. Each unit's `share`
link-metadata records list, for each function using the bytes, the
calls its value is live across, and the callers whose every call
passes their own value. The linker joins the records of all units and
checks rule 2 against the whole program's call graph. A `--link` that
finds the promise broken stops with an error naming the function that
writes the bytes. The fix is to declare the parameter without `const`.

## What it leaves alone

- A value used only up to the call. Copy folding already computes it
  into the callee's slot.
- A callee that takes the parameter in a register, or at a fixed
  address (`zeropage(addr)`).
- Functions on a cycle under `#pragma c6502 call save(hardware_stack)`,
  and functions without private pools.
- `p += 0x100` stays a 16-bit add of 0 and 1 instead of an `INC` of the
  high byte.
