# Attributes, and how to call ROM routines

c6502 accepts a few GCC-style attributes that tell the compiler
things C itself has no way to say. These include where an object
lives, how a function takes its arguments, and how it returns its
result. This document explains each attribute, and then shows how to
use them together to call routines that are already in ROM, such as
the Apple II Monitor's `COUT`.

The examples use `uint8_t` and friends from `<stdint.h>`, and `bool`
from `<stdbool.h>`, where it's `unsigned char`.

## Writing an attribute

An attribute goes inside a double pair of parentheses after the
keyword `__attribute__`. Several attributes can share one clause,
separated by commas, and an attribute can take an integer argument.

```c
__attribute__((zp_abi, address(0xFDED))) void COUT(register uint8_t c);
```

A function's attributes go at the front of its declaration. An
object's attributes can go at the front, or right after its name.

```c
__attribute__((zeropage)) uint8_t count;
uint8_t count __attribute__((zeropage));
```

The compiler rejects a name it doesn't know, the same attribute
twice in one declaration, and an attribute used where it doesn't
apply, such as `zeropage` on a function. Each of these is a compile
error that names the declaration.

## The attributes

| Attribute | Goes on | What it does |
|---|---|---|
| `zp_abi` | a function | Passes arguments in zero page and in A and X instead of on the soft stack. |
| `return_bool_in_carry` | a function returning `bool` | Returns the result in the carry flag. |
| `zeropage` | an object with static storage | Places the object in zero page at an address the compiler picks. |
| `zeropage(addr)` | an object with static storage | Places the object in zero page at `addr`. |
| `address(addr)` | a function, or an `extern` object | Names code or memory that's already at `addr`. |

### `zp_abi`

By default, a function takes its arguments on c6502's *soft stack*, a
stack in main memory that the compiler manages with the `SSP` and `FP`
pointers. That works for every function, but every argument costs
several instructions to write and to read.

A `zp_abi` function takes its arguments in zero page instead, and its
`register` parameters in the A and X registers. The first `register`
byte goes in A and the second in X, so `register uint8_t a, register
uint8_t b` puts `a` in A and `b` in X, and `register uint16_t v` puts
the low byte in A and the high byte in X. There can be at most two
`register` bytes, and a pointer can't be a `register` parameter.

The return value doesn't depend on the attribute. A `char`-sized
result comes back in A, and an `int`-sized one comes back with its low
byte in A and its high byte in X. A pointer, and anything wider, comes
back in the zero-page block called `HARGS`.

With `--optimize`, every function that can use `zp_abi` gets it, so
you rarely need to write it on a function you define. Writing it
anyway makes the compiler report an error when the function can't
use it, for instance because it's recursive. On a ROM routine, though,
the attribute is required, as explained below.

### `return_bool_in_carry`

A function declared with this attribute returns its result in the
carry flag, set for true and clear for false, instead of in A. The
function has to return `bool` (or `char`). When a caller tests the
result with `if`, the compiler branches on the flag with `BCC` or
`BCS` instead of first turning it into a number.

```c
__attribute__((return_bool_in_carry)) bool is_digit(uint8_t c) {
    return c >= '0' && c <= '9';
}
```

Every declaration of the function has to carry the attribute, since a
caller compiled without it would read A. You can't take the
function's address, because a call through a pointer reads A.

### `zeropage` and `zeropage(addr)`

An object in zero page takes a 2-byte instruction to read or write
instead of a 3-byte one, and a pointer in zero page can be used
directly in an `(zp),Y` access.

```c
uint8_t num_entries __attribute__((zeropage));            // compiler picks, in $26-$7F
__attribute__((zeropage(0xBA))) uint8_t *z_header_addr;   // $BA and $BB
```

The bare form takes the lowest free bytes from `$26` to `$7F`. The
address form can be anywhere from `$26` to `$FF`. The bytes below
`$26` belong to the runtime (see "Limits" below), and a byte from
`$80` up is taken out of the compiler's own zero-page pool.

Zero page isn't part of the program file that gets loaded, so the
compiler writes a routine called `__zp_init` that stores each
zero-page object's initial value. The program's startup code has to
call `__zp_init` before `main`.

### `address(addr)`

`address` says that a name refers to something that's already at a
fixed address and that the program doesn't define. It can name a ROM
routine, or a piece of memory such as an I/O register, a Monitor
variable, or a table in ROM.

```c
__attribute__((zp_abi, address(0xFDED))) void COUT(register uint8_t c);
extern volatile uint8_t KBD __attribute__((address(0xC000)));
```

The compiler writes `COUT EQU $FDED` and `KBD EQU $C000` at the top of
its output and nothing else for them. A call is then just `JSR COUT`,
and a read is just `LDA KBD`.

On an object, `address` requires `extern`, and no declaration of the
object may give it an initial value, because the program doesn't own
that memory. An object below `$100` is treated like an `extern`
`zeropage(addr)` object, so its bytes are kept out of the compiler's
pool. Declare memory that the hardware changes, such as `KBD`, as
`volatile`, so that every read in the source becomes a read in the
output.

## Calling a ROM routine

A ROM routine was written long before your program, so the compiler
has to be told three things about it.

1. **Where it is.** That's `address(addr)`.
2. **How it takes its arguments and returns its result.** The
   Monitor routines in this document take their argument in A, and
   return their result in A. Some routines use X as well, or report
   their result in the carry flag. That's `zp_abi` with `register`
   parameters, and `return_bool_in_carry` for a carry result.
3. **What else it changes.** The compiler assumes a call changes A, X,
   Y, and every flag, so nothing needs to be declared about those.
   The zero-page bytes the ROM uses do need declaring, as "Sharing
   zero page with the Monitor" below explains.

### Example: `COUT`

`COUT` prints the character in A. In the Apple IIe ROM, `COUT` is a
jump through the output hook `CSW` at `$36`, which normally points at
`COUT1`. Here is `COUT1`.

```
COUT1   CMP #$A0        ; below $A0?
        BCC COUTZ
        AND INVFLG      ; apply the inverse / flash mask
COUTZ   STY YSAV1
        PHA
        JSR VIDWAIT     ; put it on the screen
        PLA
        LDY YSAV1
        RTS
```

The character arrives in A, and nothing comes back, which gives
this declaration.

```c
__attribute__((zp_abi, address(0xFDED))) void COUT(register uint8_t c);
```

`register` puts `c` in A, `zp_abi` makes the compiler honor
`register`, and `address` says where `COUT` is.

### A small program

`<apple2/monitor.h>` declares `COUT` the same way, along with `HOME`,
`RDKEY`, and `COUT1`, and the Monitor's zero page.

```c
#include <apple2/monitor.h>

void print(const char *s) {
    while (*s)
        COUT(*s++ | 0x80);      // the Apple II screen wants the high bit set
}

int main(void) {
    HOME();                     // clear the screen
    print("PRESS A KEY");
    uint8_t key = RDKEY();      // wait for a key; it comes back in A
    COUT(key);
    return 0;
}
```

With `--optimize`, the output starts with an `EQU` line for each
name the header declares, and `main` compiles to the following.

```
main:
   SUBROUTINE

   JSR   HOME
   LDA   #<.str@0
   STA   __zpabi_print__s_0
   LDA   #>.str@0
   STA   __zpabi_print__s_1
   JSR   print
   JSR   RDKEY
   JSR   COUT
   LDX   #$00
   TXA
   RTS
```

`RDKEY` leaves the key in A and `COUT` reads it from A, so the
compiler passes it along without storing it anywhere. The calls work
the same way without `--optimize`, although the code around them is
longer.

`HOME` and `RDKEY` take no arguments, but they still get `zp_abi`.
Without it, the compiler assumes they follow its soft-stack
convention. A soft-stack function may use the compiler's zero-page
scratch bytes, so any function that calls one can't use `zp_abi`
itself, and it gets a full soft-stack prologue and epilogue instead
of a bare `RTS`.

### Status in the carry flag

Some ROM and DOS routines report success or failure in the carry
flag. For such a routine, add `return_bool_in_carry` and declare the
result `bool`.

```c
// A routine at $6000 that sets the carry flag when it fails.
__attribute__((zp_abi, return_bool_in_carry, address(0x6000)))
bool disk_read(register uint8_t track, register uint8_t sector);
void report_error(void);

void load_catalog(void) {
    if (disk_read(17, 0))
        report_error();
}
```

### Memory locations

Hardware registers and Monitor variables are `extern` objects with an
address.

```c
extern volatile uint8_t KBD     __attribute__((address(0xC000)));  // last key, high bit = new
extern volatile uint8_t KBDSTRB __attribute__((address(0xC010)));  // any access clears the strobe
extern uint16_t CSW             __attribute__((address(0x36)));    // output hook

uint8_t wait_key(void) {
    while (!(KBD & 0x80))       // wait for a key
        ;
    uint8_t k = KBD;
    KBDSTRB;                    // any access clears the strobe
    return k & 0x7F;
}
```

Because `KBD` is `volatile`, the loop reads it on every pass, and
`KBDSTRB;` reads `KBDSTRB` even though the value isn't used. With
`--optimize`, `wait_key` compiles to the following.

```
wait_key:
   SUBROUTINE

.loop@0_continue:
   LDA   KBD
   BPL   .loop@0_continue
   LDA   KBD
   STA   __local_wait_key__0
   LDA   KBDSTRB
   LDA   __local_wait_key__0
   AND   #$7F
   RTS
```

Without `volatile`, the compiler could read `KBD` once and reuse the
value, and the loop would keep testing that one value after a key
arrives.

### Sharing zero page with the Monitor

c6502's runtime keeps four things of its own in zero page. `SSP` and
`FP` are the soft stack and frame pointers, `HARGS` is where the
runtime's helpers for multiplication, division, shifts, and floating
point take their arguments, and `DPTR` is a pointer the compiler
uses to reach memory through a pointer variable. Normally they sit
at `$00` to `$25`.

The Monitor uses some of those same bytes. It keeps its text window
in `$20` to `$23` and the cursor position, `CH` and `CV`, in `$24`
and `$25`. Every time compiled code wrote `DPTR`, the cursor would
move. In the `print` function above, each pass through the loop
stores the string pointer into `DPTR`, so the next character would
appear somewhere else.

To prevent that, the program declares the bytes the Monitor owns,
and the compiler moves its own block out of their way.
`<apple2/monitor.h>` declares every zero-page byte the Monitor ROM
uses, `$00`-`$01`, `$1F`, and `$20`-`$49`, as `extern` objects with
`address`. Including it is how a source file says it runs alongside
the Monitor. For the program above, the runtime's block moves to
`$02`-`$08`, clear of the cursor.

```
HARGS	EQU	$02
SSP	EQU	$03
FP	EQU	$05
DPTR	EQU	$07
CH	EQU	$24
CV	EQU	$25
```

`HARGS` is one byte here because the program calls no helper. When
the block moves, the compiler sizes `HARGS` to what the program
uses, so a program with a 32-bit division gets 22 bytes. Zero-page
globals without an address go in the bytes left over.

The header names the Monitor variables a program is likely to use:
the window (`WNDLFT`, `WNDWDTH`, `WNDTOP`, `WNDBTM`), the cursor
(`CH`, `CV`), `BAS`, `GBAS`, `COLOR`, `INVFLG`, `PROMPT`, and the
I/O hooks `CSW` and `KSW`. For instance, `CH = 0;` moves the cursor
to the left edge of the window. The other bytes are covered by
arrays named `__monitor_*`.

The same approach works for other system software. A header for DOS
or Applesoft would declare the zero-page bytes that software uses,
and a program that includes it keeps them free. The bytes are free
for the whole program or for none of it, because the Monitor writes
`CH` no matter which source file called `COUT`. So every source file
in a program that calls the Monitor should include the header.

A header also lets every source file declare the routines the same
way, and the compiler checks that all declarations of a name agree.
When several source files are compiled separately and joined with
`compile.py --link`, each routine's name is bound once, and the link
fails if two files give a name different addresses.

## What the compiler rejects

Each of these stops the compile with a message that names the
declaration and the reason.

| Mistake | Why it's an error |
|---|---|
| A function with `address` that also has a body | The code is already at that address. |
| An object with `address` that isn't `extern`, or has an initial value | The program doesn't own that memory, so it can't define it. |
| Two different addresses for the same name | The compiler can't tell which one is right. |
| Two `address` objects that overlap | The optimizer assumes different names are different memory. |
| Reserved zero-page bytes that leave no room below `$80` for the runtime's block | The runtime needs `HARGS` and three 2-byte pointers in zero page, and the compiler's own pools use `$80`-`$FF`. |
| A `register` parameter on a function with `address` but no `zp_abi` | Without `zp_abi`, arguments go on the soft stack, which the ROM routine doesn't read. |
| A `zp_abi` routine with `address` and a parameter that isn't `register` | That parameter would go in a zero-page byte the compiler picks per program, and that address isn't in the routine's code. |
| A `zp_abi` routine with `address` that returns a pointer, or more than 2 bytes | Those come back in `HARGS`, where the routine doesn't put them. Declare a pointer result as `uint16_t`. |
| Taking the address of a `zp_abi` routine with `address` | A call through a pointer passes its arguments on the soft stack. |
| `return_bool_in_carry` on some declarations of a function but not all | A caller compiled without it would read A instead of the carry flag. |

## Limits

**The 80-column firmware.** On an Apple IIe, `HOME`, `CLREOL`,
scrolling, and keyboard input continue into the 80-column firmware
at `$C100`. `<apple2/monitor.h>` covers the bytes the Monitor ROM
listing shows, and any zero page that firmware uses isn't covered.

**Linking.** A source file with zero-page objects, including one that
includes `<apple2/monitor.h>`, can't be linked with `--link` yet.

**Registers.** The compiler assumes every call changes A, X, Y, and
the flags. `COUT1` preserves Y, but there's no way to declare that
yet, so the compiler doesn't keep a value in X or Y across the call.

**Arguments.** A ROM routine can take at most 2 bytes of arguments,
in A and X. A routine that takes an argument in Y, or in a zero-page
location, needs a small wrapper written in assembly, or the argument
stored into an `address` object before the call.
