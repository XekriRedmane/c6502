# Attributes, and how to call ROM routines

c6502 has a few attributes, written in C23's attribute syntax, that
tell the compiler things C itself has no way to say. These include where an object
lives, how a function takes its arguments, and how it returns its
result. A few more go before a statement and change how a loop or a
call compiles. This document explains each attribute, and then shows
how to use the declaration attributes together to call routines that
are already in ROM, such as the Apple II Monitor's `COUT`.

The examples use `uint8_t` and friends from `<stdint.h>`, and `bool`,
which is a keyword in C23.

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
are the same. The
[statement attributes](#statement-attributes) go in front of a
statement, as in `[[c6502::unroll]] for (...)`.

C23 also lets attributes follow a function's parameter list, a `*`, an
array's `]`, or the type specifiers, where they apply to that type.
c6502 doesn't accept them there, and reports a syntax error.

The compiler rejects a `c6502::` name it doesn't know, the same
attribute twice in one declaration, an attribute used where it doesn't
apply, such as `zeropage` on a function, and a c6502 attribute written
without its prefix. Each of these is a compile error that names the
declaration. GCC's `__attribute__((...))` isn't accepted; the error
for it shows the C23 form.

C23's standard attributes are accepted where C23 allows them:
`deprecated`, `maybe_unused`, `nodiscard`, `noreturn`, and
`fallthrough`, the last as `[[fallthrough]];` in a switch. Only
`noreturn` (or `_Noreturn`) changes anything. Under `-O`, the
code after a call to a function that any declaration marks with it is
dropped, and the call becomes a `JMP`
([docs/optimizations/noreturn_call.md](optimizations/noreturn_call.md)).
A `[[noreturn]]` function that returns anyway, which C23 leaves
undefined, returns to code that may not be there. `unsequenced` and `reproducible` go after a parameter list,
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
| `section("name")` | a function, or an object with static storage | Puts it in a section of that name, for a linker script to place. |

### `zp_abi`

By default, a function takes its arguments on c6502's *soft stack*, a
stack in main memory that the compiler manages with the `SSP` and `FP`
pointers. That works for every function, but every argument costs
several instructions to write and to read.

A `zp_abi` function takes its arguments in the A and X registers and
in zero page instead. When its declaration names no register and no
address for any parameter, its leading integer and pointer parameters
arrive in A and then X, as many whole parameters as fit in the two
registers, and the rest in zero page. So `f(uint8_t a, uint8_t b)`
takes `a` in A and `b` in X, and `f(const char *s)` takes the low byte
of `s` in A and the high byte in X. `main` takes its parameters in
zero page.

When the declaration has `register` parameters, they alone go in
registers, and the others in zero page. The first `register` byte
goes in A and the second in X, so `register uint8_t a, register
uint8_t b` puts `a` in A and `b` in X, and `register uint16_t v` puts
the low byte in A and the high byte in X. A parameter can name its
register instead, with `in_a`, `in_x`, or `in_y` (see below), and
then the other parameters go in zero page unless they name a register
too. A pointer can't be a `register` parameter by position, but it can
name its registers.

The return value doesn't depend on the attribute. A `char`-sized
result comes back in A, and an `int`-sized one or a pointer comes back
with its low byte in A and its high byte in X. Anything wider comes
back in the zero-page block called `HARGS`.

With `-O`, every function that can use `zp_abi` gets it, so
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
needs the function to use `zp_abi` (with `-O`), so a function
that can't, such as a recursive one, is an error. Without `-O`,
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
function has to return `bool`; an `unsigned char` or
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

Under `-O`, a `static` function returning `bool` gets the same
convention without the attribute when every call to it branches on
the result or returns it from another such function, and its address
isn't taken
([carry_return_choice.md](optimizations/carry_return_choice.md)). A
`bool` function with external linkage gets it whenever it's zp_abi,
since every unit can make that choice from the declaration, and
The linker checks that the units agree. Such a function returns
`unsigned char` to keep its result in A.

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

The value member can't be a pointer. A function that returns
only a pointer returns it in A and X, but the carry-flag struct's
value member is limited to integers.

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

With `-O`, the compiler also moves the busiest scalar globals
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

A file that only declares `hits` doesn't give it an address. The linker
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
`dump_buffer_to_screen` in `examples/zork/output.c`. It
keeps the index in X.

```c
for (uint8_t x = 0; x != BUFF_END; x++)
    COUT1(BUFF_AREA[x]);
```

Without `preserves_x`, the loop saves X before each call and reloads
it after.

```
   LDX   #$00
   BEQ   .loop@0_start
.dump_buffer_to_screen@rot@0:
   LDA   BUFF_AREA,X
   STX   __local_dump_buffer_to_screen__x
   JSR   COUT1
   LDX   __local_dump_buffer_to_screen__x
   INX
.loop@0_start:
   CPX   BUFF_END
   BNE   .dump_buffer_to_screen@rot@0
```

With it, the save and reload are gone. The loop then has the
instructions of Zork's hand-written one, which tests at the top with
`CPX BUFF_END; BEQ` and jumps back with `JMP`. The compiler moves the
test to the bottom, so each pass through the loop takes one branch.

```
   LDX   #$00
   BEQ   .loop@0_start
.dump_buffer_to_screen@rot@0:
   LDA   BUFF_AREA,X
   JSR   COUT1
   INX
.loop@0_start:
   CPX   BUFF_END
   BNE   .dump_buffer_to_screen@rot@0
```

Only a routine with `address` can carry these attributes, because the
compiler can't check that a function it compiles keeps the promise.
The compiler trusts the declaration. Calling a routine declared
`preserves_x` that changes X anyway is undefined behavior, so declare
only what the routine's code shows. `<apple2/monitor_io.h>` declares
what the ROM listings show for each Monitor routine. A routine that
returns an `int` can't be `preserves_x`, because the high byte of
its result comes back in X.

### One-byte enums

An enum's type is `int`, two bytes, which makes every variable,
parameter, and struct member of that type cost two bytes and every
operation on it two bytes' work. C23's fixed underlying type gives an
enum the size of the type after its `:`, and its constants that type
too, so `sizeof RUN` is 1 here:

```c
typedef enum : uint8_t { IDLE, RUN, STOP } state;
enum dir : int8_t { LEFT = -1, NONE, RIGHT };

struct actor { state s; enum dir d; uint8_t x; };   // 3 bytes
```

A value the type can't hold is an error. c6502 had an attribute for
this, `[[c6502::packed]]`, which picked the smallest type that held an
enum's values. It was removed on 19 Nov 2026, and using it is an error
whose message points to the fixed type.

### `section("name")`

`section` puts a function or an object with static storage in a
named section instead of the one it would go in (`.text`, `.rodata`,
`.data`, or `.bss`). A linker script given to `compile.py -T`
then decides where the section goes in memory:

```c
[[c6502::section(".text.boot")]] void reset(void);
uint8_t buffer [[c6502::section(".page")]][256];
```

Every declaration that names a section has to name the same one, and
the attribute can't go with `zeropage` or `address`. Without `-T` it
has no effect. [linker_scripts.md](linker_scripts.md) describes
sections and scripts.

## Statement attributes

Three of c6502's attributes go before a statement inside a function,
where C23 §6.8 allows an attribute list.

| Attribute | Goes before | What it does |
|---|---|---|
| `unroll` | a `for` loop | Unrolls the loop completely. |
| `keep` | a `for` loop | Keeps a loop that computes nothing, such as a delay loop. |
| `save_hardware_stack` | an expression statement, a `return`, or a declaration | Lets a recursive call save its caller's locals on the 6502's hardware stack. |

```c
[[c6502::unroll]] for (uint8_t i = 0; i < 4; i++)
    row[i] = v + i;
```

The attribute can share the loop's line, as above, or sit on the line
before it. `unroll` and `save_hardware_stack` take effect only under
`-O`, and without it the code compiles as if they weren't there. A
misplaced one is an error either way. `keep` matters only under `-O`
too, since dead-loop elimination runs only there.

A statement attribute anywhere else is an error whose message names
the place it goes. That covers `unroll` or `keep` before a `while`
loop, any of the three on a function or at file scope, and any of them
before a label.
C23 gives the attributes before a label to the label, so `L:
[[c6502::keep]] for (...)` is the way to mark a labeled loop. No
standard attribute applies to a statement, and the other `c6502::`
attributes don't either.

These used to be `#pragma c6502` directives. A `#pragma c6502` line is
now an error whose message names the attribute that replaces it.
`#pragma c6502 loop unroll(disable)` never did anything and has no
replacement. Any other `#pragma`, such as `#pragma once` or one meant
for another compiler, is ignored, as C99 §6.10.6 allows.

### `unroll`

```c
uint8_t row[4];

void fill(uint8_t v) {
    [[c6502::unroll]] for (uint8_t i = 0; i < 4; i++)
        row[i] = v + i;
}
```

The loop is replaced by one copy of its body per iteration, with `i`
replaced by that iteration's value in each copy. The copies need no
compare, branch, or increment, and a constant index lets `row[i]`
use the element's address directly. The cost is code size, which
grows with the number of iterations.

The loop has to have a simple shape the compiler can count:

- The `for` declares one integer variable, initialized to a
  constant.
- The condition compares that variable with a constant using `<`,
  `<=`, `>`, or `>=`.
- The step is `++`, `--`, `+= K`, or `-= K`, with `K` a positive
  constant.
- The body has no `break`, `continue`, `goto`, or label, and doesn't
  change the variable or take its address.
- The loop ends within 256 iterations.

A marked loop that doesn't fit this shape is a compile error, since
the attribute states that the loop is to be unrolled. [Loop
unrolling](optimizations/unroll.md) has the full rules, including
which constants count.

### `keep`

Under `-O`, dead-loop elimination deletes a loop whose only effect is
on variables that nobody reads afterwards. A delay loop is exactly
that, and its effect is the time it takes:

```c
void sound_delay1(uint8_t count) {
    do {
        [[c6502::keep]] for (uint8_t i = 0xB4; --i;) {
        }
    } while (--count);
}
```

`keep` makes dead-loop elimination leave the loop alone. The other
optimizations still apply to it, so the loop may compile to fewer
instructions than a straightforward translation, and the time it takes
can change from one version of the compiler to the next. A delay that
has to be exact belongs in assembly.

### `save_hardware_stack`

This attribute is for recursive functions. Use it only after reading
the cautions below.

#### What it does

A function that calls itself, directly or through other functions,
normally keeps its locals in zero-page bytes that it shares with
other functions on the same terms as everyone else: it saves the
ones it uses when it starts and restores them when it returns. It
does that on every call, including the calls that don't recurse,
and it saves them in a soft-stack frame.

The attribute marks calls that recurse. When every call that can
recurse is marked, the function instead gets zero-page bytes of its
own, as a non-recursive function does. Around each marked call, the
caller pushes each local that's still used after the call onto the
hardware stack, and pulls them back after the call:

```c
void print_zstring(void) {
    ...
            uint8_t locked = LOCKED_ALPHABET;
            uint8_t state = ZDECOMPRESS_STATE;
            load_packed_address();
            [[c6502::save_hardware_stack]] print_zstring();
            ZDECOMPRESS_STATE = state;
            LOCKED_ALPHABET = locked;
    ...
}
```

compiles to

```
   LDA   LOCKED_ALPHABET
   PHA
   LDA   ZDECOMPRESS_STATE
   PHA
   JSR   load_packed_address
   JSR   print_zstring
   PLA
   STA   ZDECOMPRESS_STATE
   PLA
   STA   LOCKED_ALPHABET
```

and the function no longer saves anything when it starts or ends. In
Zork's `print_zstring` (examples/zork/zstring.c) this made
the function 180 bytes smaller, measured in the example it came from,
and faster on every call.

#### Where it goes

The attribute goes inside a function, just before one of these:

- an expression statement, such as `f(x);` or `y = f(x) + 1;`,
- a `return` statement, such as `return a + f(x);`,
- a declaration in a block, such as `int r = f(x);`.

On a declaration it goes at the front, where a declaration's leading
attributes go: `[[c6502::save_hardware_stack]] int r = f(x);`. C23
says those apply to the names the declaration declares, but c6502
reads this one as applying to the calls in the declaration's
initializers. After a declaration's name, on a function declaration,
on a typedef, at file scope, and in the first clause of a `for`, it's
an error.

It marks every call in that statement or declaration, and each of
them has to be a direct call that can recurse, that is, a call to a
function that can call the caller back. A call to any other function,
a call through a function pointer, or a statement with no call at
all is an error. Before an `if`, a loop, or a block, the attribute is
also an error, because it would cover the calls in the statements
nested inside.

The attribute takes effect only when every call that can recurse is
marked. If a function `f` calls `g` and `g` calls `f`, both calls
need it. If one is unmarked, the functions compile as they would
without the attribute. The attribute also has no effect on a function
that couldn't get its own zero-page bytes anyway, such as one that
calls through a function pointer.

Parameters, and locals whose address is taken, stay on the soft
stack, where each call has its own copy. They aren't pushed.

The calls have to be in one source file. The linker accepts
such a cycle, which the link metadata marks call by call, and reports
any other recursion as an error. examples/zork links
`print_zstring` this way.

#### Cautions

**The compiler can't check that the hardware stack is big enough.**
How deep your recursion goes isn't fixed at compile time, so the
attribute is your promise that it's shallow enough. That's why it isn't on by
default.

The 6502's hardware stack is a single 256-byte page, `$0100`-`$01FF`.
Every call in progress already keeps a 2-byte return address there.
With the attribute, every call in progress also keeps the bytes it
pushed until the call returns. A recursive call that saves 7 bytes
uses 9 bytes of hardware stack per level, so 20 levels take 180
bytes. The rest of the program uses the stack too:

- the return addresses of the calls that led to the recursion,
- the runtime helpers and the soft-stack epilogue, which push a few
  bytes for a moment,
- an interrupt handler, which can run at any point and pushes at
  least 3 bytes, plus whatever it pushes itself,
- ROM routines you call, such as the Monitor's, which may use the
  stack too.

**Running out of hardware stack fails silently.** The stack pointer
wraps around within page 1, and the next push overwrites the oldest
bytes on the stack, usually the return addresses of the outermost
calls. Nothing reports it. The program returns to the wrong place,
often much later, and the crash points nowhere near its cause.

**You may push more than you expect.** Every local that's still
needed after the call is pushed, along with any value the compiler
keeps in a temporary across the call, and a `long` local takes 4
bytes. To see the exact number, compile with `-S -O`
and count the `PHA`s before the marked `JSR`.

Use the attribute when you know the recursion is shallow.

Without the attribute, recursion keeps everything but return addresses
on the soft stack, which can grow much larger than 256 bytes. That's
slower, but it doesn't depend on how deep the recursion goes. [The
calling conventions guide](calling_conventions.md#limits) covers the
limits of both stacks.

[Saves around a recursive call](optimizations/recursion_saves.md)
describes how the compiler decides what to save, for readers who
want the details.

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

With `-O`, the output starts with an `EQU` line for each
name the header declares, and `main` compiles to the following.

```
main:
   SUBROUTINE

   JSR   HOME
   LDX   #>str@0
   LDA   #<str@0
   JSR   print
   JSR   RDKEY
   JSR   COUT
   LDX   #$00
   TXA
   RTS
```

`RDKEY` leaves the key in A and `COUT` reads it from A, so the
compiler passes it along without storing it anywhere. The calls work
the same way without `-O`, although the code around them is
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
`-O`, `wait_key` compiles to the following.

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
When several source files are compiled separately and linked,
each routine's name is bound once, and the link fails if two
files give a name different addresses.

## What the compiler rejects

Each of these stops the compile with a message that names the
declaration and the reason. `compile.py` prints it on one line, as
`<file>: error: <message>`, and exits with status 1.

```
$ compile.py rom.c -S
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
| A `zp_abi` routine with `address` and a parameter that has neither `register`, `in_*`, nor `zeropage(addr)`, when another parameter has one of them or the parameters don't all fit in A and X | That parameter would go in a zero-page byte the compiler picks per program, and that address isn't in the routine's code. |
| A `zeropage(addr)` parameter on a function that can't use `zp_abi` with `-O`, such as a recursive one | Only `zp_abi` passes arguments in zero page. |
| Declarations of a function that give a parameter different addresses, or `zeropage(addr)` on only some of them | A caller would store the argument where the function doesn't read it. The linker checks this across source files too. |
| `zeropage(addr)` on a `register` parameter, without an address, or past `$FF` | The parameter arrives either in a register or in zero page, at one address. |
| Two parameters that name the same register | A register holds one value. |
| A pair on a 1-byte parameter, or one register on a 2-byte one | Each byte needs its own register. |
| `in_*` on a struct, a union, or an array | Only an integer or a pointer is passed in registers. |
| A pointer declared `register` without `in_*` | A `register` parameter by position takes A, then X, for numbers; name the registers for a pointer. |
| A `zp_abi` routine with `address` that returns more than 2 bytes | Those come back in `HARGS`, where the routine doesn't put them. A pointer comes back in A and X, as a 2-byte integer does. |
| Taking the address of a `zp_abi` routine with `address` | A call through a pointer passes its arguments on the soft stack. |
| `preserves_x` or `preserves_y` on a function without `address`, or on a function definition | The compiler can't check that code it compiles keeps the promise. |
| `preserves_x` on a routine that returns an `int` or `unsigned int` | The high byte of the result comes back in X. |
| `return_bool_in_carry` on some declarations of a function but not all | A caller compiled without it would read A instead of the carry flag. |
| `return_bool_in_carry` on a function returning a struct that isn't one value of 1 or 2 bytes followed by one `bool` | Only that shape fits in A, X, and the carry flag. |
| `preserves_x` on a routine that returns a struct whose value is an `int` or `unsigned int` | The high byte of the value comes back in X. |
| `packed` on an enum | It was removed. `enum e : uint8_t { ... }` gives an enum one byte. |
| `unroll` or `keep` before anything but a `for` loop, or both on one loop | They describe a `for` loop, and an unrolled loop leaves no loop to keep. |
| `save_hardware_stack` before an `if`, a loop, or a block | It would cover the calls in the statements nested inside. |
| `save_hardware_stack` on a statement or declaration without a call, or on a call that can't recurse | There's nothing for it to save around. |
| A statement attribute on a declaration other than one in a block, or before a label | It applies to a statement, and C23 gives the attributes before a label to the label. |
| A `#pragma c6502` line | The statement attributes replaced the pragmas, and the error names the attribute to write instead. |

## Limits

**The 80-column firmware.** On an Apple IIe, `HOME`, `CLREOL`,
scrolling, and keyboard input continue into the 80-column firmware
at `$C100`. The Monitor headers cover the bytes the Monitor ROM
listing shows, and any zero page that firmware uses isn't covered.

**Linking.** The linker chooses where the runtime's zero-page block
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
