# Attributes, and how to call ROM routines

c6502 has a few attributes, written in C23's attribute syntax, that
tell the compiler things C itself has no way to say. These include where an object
lives, how a function takes its arguments, and how it returns its
result. This document explains each attribute, and then shows how to
use them together to call routines that are already in ROM, such as
the Apple II Monitor's `COUT`.

The examples use `uint8_t` and friends from `<stdint.h>`, and `bool`
from `<stdbool.h>`, where it's `_Bool`.

## Writing an attribute

Attributes go inside double square brackets, as C23 writes them
(§6.7.13). c6502's own carry the prefix `c6502::`, the way GCC's carry
`gnu::`, so another compiler can tell they aren't its own. Several
attributes can share one pair of brackets, separated by commas, and an
attribute can take an integer or string argument in parentheses.

```c
[[c6502::zp_abi, c6502::address(0xFDED)]] void COUT(register uint8_t c);
```

Attributes at the front of a declaration apply to every name it
declares. Attributes right after a name apply to that name alone. The
two places mean the same thing, and a declaration can use both.

```c
[[c6502::zeropage]] uint8_t count;
uint8_t count [[c6502::zeropage]];
uint8_t table [[c6502::zeropage]][16];     // after the name, before [16]
void f [[c6502::zp_abi]] (void);
```

A parameter's attributes go after its name, or in front of the
parameter: `uint8_t n [[c6502::in_x]]` and `[[c6502::in_x]] uint8_t n`
are the same. An enum's `packed` goes right after `enum`.

C23 also lets attributes follow a function's parameter list, a `*`, an
array's `]`, or the type specifiers, where they apply to that type.
c6502 doesn't accept them there, and reports a syntax error.

The compiler rejects a `c6502::` name it doesn't know, the same
attribute twice in one declaration, an attribute used where it doesn't
apply, such as `zeropage` on a function, and a c6502 attribute written
without its prefix. Each of these is a compile error that names the
declaration. GCC's `__attribute__((...))` isn't accepted; the error
for it shows the C23 form.

C23's standard attributes are accepted where C23 allows them, and
otherwise change nothing: `deprecated`, `maybe_unused`, `nodiscard`,
`noreturn`, and `fallthrough`, the last as `[[fallthrough]];` in a
switch. `unsequenced` and `reproducible` go after a parameter list,
so c6502 rejects them. An attribute with any other prefix, such as
`[[gnu::cold]]`, is ignored, as C23 says it should be, and so are its
arguments. Those arguments can't contain braces.

## The attributes

| Attribute | Goes on | What it does |
|---|---|---|
| `zp_abi` | a function | Passes arguments in zero page and in A and X instead of on the soft stack. |
| `return_bool_in_carry` | a function returning `bool`, or a struct of a value and a `bool` | Returns the `bool` in the carry flag. |
| `zeropage` | an object with static storage | Places the object in zero page at an address the compiler picks. |
| `zeropage(addr)` | an object with static storage | Places the object in zero page at `addr`. |
| `address(addr)` | a function, or an `extern` object | Names code or memory that's already at `addr`. |
| `preserves_x`, `preserves_y` | a function with `address` | Says the routine leaves X (Y) as it found it. |
| `in_a`, `in_x`, `in_y`, and pairs such as `in_ay` | a parameter | Names the register (or two, low byte first) the parameter arrives in. |
| `zeropage(addr)` | a parameter that isn't `register` | Passes the argument in zero page at `addr`. |
| `packed` | an enum's definition | Makes the enum one byte when its values fit. |
| `section("name")` | a function, or an object with static storage | Puts it in a section of that name, for a linker script to place. |

### `zp_abi`

By default, a function takes its arguments on c6502's *soft stack*, a
stack in main memory that the compiler manages with the `SSP` and `FP`
pointers. That works for every function, but every argument costs
several instructions to write and to read.

A `zp_abi` function takes its arguments in zero page instead, and its
`register` parameters in the A and X registers. The first `register`
byte goes in A and the second in X, so `register uint8_t a, register
uint8_t b` puts `a` in A and `b` in X, and `register uint16_t v` puts
the low byte in A and the high byte in X. A parameter can name its
register instead, with `in_a`, `in_x`, or `in_y` (see below). A
pointer can't be a `register` parameter by position, but it can name
its registers.

The return value doesn't depend on the attribute. A `char`-sized
result comes back in A, and an `int`-sized one comes back with its low
byte in A and its high byte in X. A pointer, and anything wider, comes
back in the zero-page block called `HARGS`.

With `--optimize`, every function that can use `zp_abi` gets it, so
you rarely need to write it on a function you define. Writing it
anyway makes the compiler report an error when the function can't
use it, for instance because it's recursive. On a ROM routine, though,
the attribute is required, as explained below.
[calling_conventions.md](calling_conventions.md) describes both
conventions in detail.

### `in_a`, `in_x`, and `in_y`

These go on a parameter, after its name, and say which register it
arrives in. A 2-byte parameter names two, low byte first: `in_xy`
puts the low byte in X and the high byte in Y. The pairs are `in_ax`,
`in_ay`, `in_xa`, `in_xy`, `in_ya`, and `in_yx`. The parameter is then
a register parameter, so `register` is optional, and it can be any
integer or pointer with one byte for each register named.

```c
// DOS's RWTS takes its parameter block's address with the low byte
// in Y and the high byte in A.
[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0x2900)]]
bool RWTS(struct iob *iob [[c6502::in_ya]]);

bool read_sector(void) { return RWTS(&iob); }
```

```
read_sector:
   LDA   #>iob
   LDY   #<iob
   JMP   RWTS
```

Y is the one register that only a parameter naming it gets. A
`register` parameter without the attribute still goes in A, then X,
skipping a register another parameter names, so `register uint8_t k`
next to an `in_xy` parameter arrives in A. A function the compiler
compiles can take parameters this way too. It copies each one into a
local as it starts, which stays in the register when the body allows
and is in its zero-page pool otherwise, so a register parameter takes
no zero-page byte of its own. A pointer always goes in the pool, as
two adjacent bytes, since the 6502 reads through a pointer only in
zero page.

### `zeropage(addr)` on a parameter

Some routines take an argument in a zero-page location rather than in
a register. Zork's `cout_string`, for instance, prints the string
whose address is in `SCRATCH2`, at `$E6`. On a parameter,
`zeropage(addr)` says the argument is passed there:

```c
[[c6502::zp_abi, c6502::address(0x1D34)]]
void cout_string(const char *s [[c6502::zeropage(0xE6)]],
                 uint8_t count [[c6502::in_x]]);

void show(void) { cout_string("DEFAULT = ", 10); }
```

```
show:
   LDA   #<str@0
   STA   __zpabi_cout_string__s_0
   LDA   #>str@0
   STA   __zpabi_cout_string__s_1
   LDX   #$0A
   JMP   cout_string
```

where `__zpabi_cout_string__s_0` and `__zpabi_cout_string__s_1` are
bound to `$E6` and `$E7`. A call stores the argument there and jumps,
so the program needs no global for it. The compiler keeps its own
zero-page bytes, and the runtime's block, off the bytes such
parameters use. Two functions may take parameters at the same bytes,
and a zero-page object may cover them too. The attribute goes on a
parameter that isn't `register` and doesn't name a register. On a routine with `address` the
routine has to be `zp_abi`, as with `register` parameters.

A function you define can take a parameter this way too, and then
assembly code that calls it knows where to put the argument. Written
in C, Zork's `cout_string` keeps its convention, and with `COUT1`
declared `preserves_x, preserves_y` it compiles to Zork's own code:

```c
void cout_string(const char *s [[c6502::zeropage(0xE6)]],
                 uint8_t count [[c6502::in_x]]) {
    uint8_t i = 0;
    do { COUT1(s[i] | 0x80); i++; } while (--count != 0);
}
```

```
cout_string:
   LDY   #$00
.loop@0_start:
   LDA   (__zpabi_cout_string__s_0),Y
   ORA   #$80
   JSR   COUT1
   INY
   DEX
   BNE   .loop@0_start
   RTS
```

The function reads the parameter where it arrives, as it would one in
bytes the compiler picked. If something the function calls might
store an argument at the same bytes (a function with a `zeropage(addr)`
parameter there, or a function defined in another source file, which
might call one), the function first copies the parameter into a local
of its own. Like a `register` parameter, a `zeropage(addr)` parameter
needs the function to use `zp_abi` (with `--optimize`), so a function
that can't, such as a recursive one, is an error. Without `--optimize`,
calls between functions the compiler compiles pass every argument on
the soft stack, as they do for `register` parameters. Each declaration
of the function has to give the parameter the same address.

The compiler doesn't see a zero-page object that covers the bytes as
the same memory as the parameter, so the function shouldn't read or
write such an object while it still needs the parameter.

To share bytes between C functions, you don't need the attribute. Give
two functions that never run at the same time a pointer parameter
each, and the compiler gives both the same bytes: callers write the
pointer where the callee reads it, as Zork's code does with
`SCRATCH2`. A function that passes its own parameter on to another
shares that one's bytes, and a caller that passes a local and still
needs it afterwards can keep it in the callee's bytes when the callee
leaves them alone. See
[optimizations/slot_sharing.md](optimizations/slot_sharing.md).

### `return_bool_in_carry`

A function declared with this attribute returns its result in the
carry flag, set for true and clear for false, instead of in A. The
function has to return `bool` (`_Bool`); an `unsigned char` or
`char` return is an error. When a caller tests the
result with `if`, the compiler branches on the flag with `BCC` or
`BCS` instead of first turning it into a number.

```c
[[c6502::return_bool_in_carry]] bool is_digit(uint8_t c) {
    return c >= '0' && c <= '9';
}
```

Every declaration of the function has to carry the attribute, since a
caller compiled without it would read A. You can't take the
function's address, because a call through a pointer reads A.

#### A value and a flag

Many assembly routines return a value in A and a flag in the carry,
such as a character and "end of string", or a table index and "not
found". C has one return value, so such a function returns a struct
with exactly two members. The first is the value, a `char`, `unsigned
char`, `signed char`, `int`, or `unsigned int`. The second is the
flag, a `bool`. The value comes back in A (an `int` in A and X) and
the flag in the carry. No memory is involved, so the function can
still use `zp_abi`.

```c
struct zchar { uint8_t code; bool end; };

[[c6502::return_bool_in_carry]] struct zchar next_zchar(void) {
    struct zchar r;
    r.code = buffer[pos++];
    r.end = r.code == 0xFF;
    return r;
}

void print_all(void) {
    for (;;) {
        struct zchar c = next_zchar();
        if (c.end)
            break;
        print_code(c.code);
    }
}
```

The function sets the carry from the flag and then loads the value,
because loading A (and X) leaves the carry alone:

```
   LDA   r+1      ; the flag
   CMP   #1       ; carry set if the flag is 1
   LDA   r        ; the value
   RTS
```

The value member can't be a pointer. A pointer result comes
back in `HARGS`, in zero page, because a pointer is only usable from
zero page, as "Arguments in registers" in
[calling_conventions.md](calling_conventions.md) explains.

### `zeropage` and `zeropage(addr)`

An object in zero page takes a 2-byte instruction to read or write
instead of a 3-byte one, and a pointer in zero page can be used
directly in an `(zp),Y` access.

```c
uint8_t num_entries [[c6502::zeropage]];            // compiler picks, in $26-$7F
[[c6502::zeropage(0xBA)]] uint8_t *z_header_addr;   // $BA and $BB
```

The bare form takes the lowest free bytes from `$26` to `$7F`. The
address form can be anywhere from `$26` to `$FF`. The bytes below
`$26` belong to the runtime (see "Limits" below), and a byte from
`$80` up is taken out of the compiler's own zero-page pool.

Zero page isn't part of the program file that gets loaded, so the
compiler writes a routine called `__zp_init` that stores each
zero-page object's initial value. The program's startup code has to
call `__zp_init` before `main`.

With `--optimize`, the compiler also moves the busiest scalar globals
into the zero-page bytes left below `$80`, without the attribute
([zero-page promotion](optimizations/zp_promotion.md)). The attribute
is still the way to be sure a variable gets one of them.

A program in several source files declares a shared object in a
header and defines it in one file, as with any global:

```c
// counters.h
extern uint8_t hits [[c6502::zeropage]];

// counters.c
#include "counters.h"
uint8_t hits [[c6502::zeropage]] = 0;
```

A file that only declares `hits` doesn't give it an address. `--link`
places every file's bare `zeropage` objects for the whole program,
keeps each `zeropage(addr)` object where it was declared, and writes
one `__zp_init` that stores them all. It's an error to define the
object in two files, to declare it with different sizes or addresses,
or to declare it `extern` with no address in a program that never
defines it.

### `address(addr)`

`address` says that a name refers to something that's already at a
fixed address and that the program doesn't define. It can name a ROM
routine, or a piece of memory such as an I/O register, a Monitor
variable, or a table in ROM.

```c
[[c6502::zp_abi, c6502::address(0xFDED)]] void COUT(register uint8_t c);
extern volatile uint8_t KBD [[c6502::address(0xC000)]];
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

### `preserves_x` and `preserves_y`

The compiler assumes a call changes X and Y, so a value it keeps in
X across a call has to be saved before the call and reloaded after
it. Many ROM routines leave X or Y alone, though. `preserves_x` and
`preserves_y` say so, and the compiler then keeps the value in the
register across the call.

```c
[[c6502::zp_abi, c6502::preserves_x, c6502::preserves_y, c6502::address(0xFDF0)]]
void COUT1(register uint8_t c);
```

Here is Zork's loop that prints its output buffer, from
`examples/zork_dump_buffer.c`. It keeps the index in X.

```c
for (uint8_t x = 0; x != BUFF_END; x++)
    COUT1(BUFF_AREA[x]);
```

Without `preserves_x`, the loop saves X before each call and reloads
it after.

```
.loop@0_start:
   CPX   BUFF_END
   BEQ   .loop@0_break
   LDA   BUFF_AREA,X
   STX   __local_dump_buffer_to_screen__x
   JSR   COUT1
   LDX   __local_dump_buffer_to_screen__x
   INX
   JMP   .loop@0_start
```

With it, the save and reload are gone, and the loop is the same as
Zork's hand-written one.

```
.loop@0_start:
   CPX   BUFF_END
   BEQ   .loop@0_break
   LDA   BUFF_AREA,X
   JSR   COUT1
   INX
   JMP   .loop@0_start
```

Only a routine with `address` can carry these attributes, because the
compiler can't check that a function it compiles keeps the promise.
The compiler trusts the declaration. Calling a routine declared
`preserves_x` that changes X anyway is undefined behavior, so declare
only what the routine's code shows. `<apple2/monitor_io.h>` declares
what the ROM listings show for each Monitor routine. A routine that
returns an `int` can't be `preserves_x`, because the high byte of
its result comes back in X.

### `packed`

An enum's type is `int`, two bytes, which makes every variable,
parameter, and struct member of that type cost two bytes and every
operation on it two bytes' work. With `packed`, which does what GCC's
`packed` does on an enum, the type is the smallest that holds the
enum's values:

| Values | Type |
|---|---|
| all in 0..255 | `unsigned char` |
| all in -128..127, some negative | `signed char` |
| anything else | `int` |

```c
typedef enum [[c6502::packed]] { IDLE, RUN, STOP } state;
enum [[c6502::packed]] dir { LEFT = -1, NONE, RIGHT };

struct actor { state s; enum dir d; uint8_t x; };   // 3 bytes
```

The attribute goes right after `enum` in the definition, not on a
later `enum dir` that refers to it. GCC also took it after the closing
`}`, but C23 has no place for an attribute there. The
enumeration constants are still `int` constants, as C99 says, so
`RUN + 1` is an `int`; storing one in a packed variable takes a
byte.

### `section("name")`

`section` puts a function or an object with static storage in a
named section instead of the one it would go in (`.text`, `.rodata`,
`.data`, or `.bss`). A linker script given to `compile.py --link -T`
then decides where the section goes in memory:

```c
[[c6502::section(".text.boot")]] void reset(void);
uint8_t buffer [[c6502::section(".page")]][256];
```

Every declaration that names a section has to name the same one, and
the attribute can't go with `zeropage` or `address`. Without `-T` it
has no effect. [linker_scripts.md](linker_scripts.md) describes
sections and scripts.

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
   Y, and every flag, so nothing needs to be declared about those,
   though declaring `preserves_x` or `preserves_y` lets the compiler
   keep a value in that register across the call. The zero-page
   bytes the ROM uses do need declaring, as "Sharing zero page with
   the Monitor" below explains.

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
[[c6502::zp_abi, c6502::address(0xFDED)]] void COUT(register uint8_t c);
```

`register` puts `c` in A, `zp_abi` makes the compiler honor
`register`, and `address` says where `COUT` is.

### A small program

`<apple2/monitor_io.h>` declares `COUT` the same way, along with `HOME`,
`RDKEY`, `COUT1`, and `WAIT`, and the Monitor's zero page.

```c
#include <apple2/monitor_io.h>

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
   LDA   #<str@0
   STA   __zpabi_print__s_0
   LDA   #>str@0
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
result `bool`. A routine that also returns a value in A, such as a
byte read from disk, returns a struct of the value and the `bool`
(see "A value and a flag" above).

```c
// A routine at $6000 that sets the carry flag when it fails.
[[c6502::zp_abi, c6502::return_bool_in_carry, c6502::address(0x6000)]]
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
extern volatile uint8_t KBD     [[c6502::address(0xC000)]];  // last key, high bit = new
extern volatile uint8_t KBDSTRB [[c6502::address(0xC010)]];  // any access clears the strobe
extern uint16_t CSW             [[c6502::address(0x36)]];    // output hook

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
`<apple2/monitor_io.h>` declares, as `extern` objects with `address`,
every zero-page byte that the Monitor routines a program calls use.
Those are the text, keyboard, line-input, lo-res graphics, and delay
routines, and the interrupt entry. A source file that calls the
Monitor includes it. For the program above, the runtime's
block moves to `$00`-`$06`, clear of the cursor.

```
HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
DPTR	EQU	$05
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

The Monitor's interactive part, which is its command loop and
memory tools, the disassembler, register display and `BRK`
handling, cassette I/O, and the power-up slot scan, uses more zero
page: `$00`-`$01`, `$2F`, `$31`, `$34`, `$3A`-`$44`, `$46`-`$47`, and
`$49`. A program that calls those routines, or stops at a `BRK` and
then continues, also includes `<apple2/interactive_monitor.h>`, which
reserves those bytes as well. A program that only calls the routines
in `<apple2/monitor_io.h>` never reaches that code, so those bytes are
free for its own use.

The disassembler has no zero-page bytes to itself. It shares `$2C`
to `$2E` with the lo-res graphics routines and `$2F` with the
cassette routines.

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
declaration and the reason. `compile.py` prints it on one line, as
`<file>: error: <message>`, and exits with status 1.

```
$ compile.py rom.c --codegen
rom.c: error: function 'f' has `preserves_x` but returns an int, whose high byte comes back in X
```

| Mistake | Why it's an error |
|---|---|
| A function with `address` that also has a body | The code is already at that address. |
| An object with `address` that isn't `extern`, or has an initial value | The program doesn't own that memory, so it can't define it. |
| Two different addresses for the same name | The compiler can't tell which one is right. |
| Two `address` objects that overlap | The optimizer assumes different names are different memory. |
| Reserved zero-page bytes that leave no room below `$80` for the runtime's block | The runtime needs `HARGS` and three 2-byte pointers in zero page, and the compiler's own pools use `$80`-`$FF`. |
| A `register` parameter on a function with `address` but no `zp_abi` | Without `zp_abi`, arguments go on the soft stack, which the ROM routine doesn't read. |
| A `zp_abi` routine with `address` and a parameter that has neither `register`, `in_*`, nor `zeropage(addr)` | That parameter would go in a zero-page byte the compiler picks per program, and that address isn't in the routine's code. |
| A `zeropage(addr)` parameter on a function that can't use `zp_abi` with `--optimize`, such as a recursive one | Only `zp_abi` passes arguments in zero page. |
| Declarations of a function that give a parameter different addresses, or `zeropage(addr)` on only some of them | A caller would store the argument where the function doesn't read it. `--link` checks this across source files too. |
| `zeropage(addr)` on a `register` parameter, without an address, or past `$FF` | The parameter arrives either in a register or in zero page, at one address. |
| Two parameters that name the same register | A register holds one value. |
| A pair on a 1-byte parameter, or one register on a 2-byte one | Each byte needs its own register. |
| `in_*` on a struct, a union, or an array | Only an integer or a pointer is passed in registers. |
| A pointer declared `register` without `in_*` | A `register` parameter by position takes A, then X, for numbers; name the registers for a pointer. |
| A `zp_abi` routine with `address` that returns a pointer, or more than 2 bytes | Those come back in `HARGS`, where the routine doesn't put them. Declare a pointer result as `uint16_t`. |
| Taking the address of a `zp_abi` routine with `address` | A call through a pointer passes its arguments on the soft stack. |
| `preserves_x` or `preserves_y` on a function without `address`, or on a function definition | The compiler can't check that code it compiles keeps the promise. |
| `preserves_x` on a routine that returns an `int` or `unsigned int` | The high byte of the result comes back in X. |
| `return_bool_in_carry` on some declarations of a function but not all | A caller compiled without it would read A instead of the carry flag. |
| `return_bool_in_carry` on a function returning a struct that isn't one value of 1 or 2 bytes followed by one `bool` | Only that shape fits in A, X, and the carry flag. |
| `preserves_x` on a routine that returns a struct whose value is an `int` or `unsigned int` | The high byte of the value comes back in X. |
| `packed` on `enum tag` without its list of constants | The definition decides the type, so a later use can't change it. |

## Limits

**The 80-column firmware.** On an Apple IIe, `HOME`, `CLREOL`,
scrolling, and keyboard input continue into the 80-column firmware
at `$C100`. The Monitor headers cover the bytes the Monitor ROM
listing shows, and any zero page that firmware uses isn't covered.

**Linking.** `--link` chooses where the runtime's zero-page block
goes for the whole program, around every file's zero-page objects, so
a file that includes `<apple2/monitor_io.h>` links with one that
doesn't. Files compiled before the linker recorded how much of `HARGS`
each one uses have to agree on the block instead.

**Registers.** The loop-counter optimizations, the register
allocator (so a `register` local can stay in Y across a call to
`COUT1`), and the redundant-load pass (an `LDX #k` repeated after the
call) keep a value in X or Y across a call declared `preserves_x` or
`preserves_y`. Other optimizations still treat the call as changing
the register, which is correct but may leave a reload in.

**Arguments.** A ROM routine can take at most 3 bytes of arguments,
in A, X, and Y. A routine that takes an argument in a zero-page
location needs the argument stored into an `address` object before
the call.
