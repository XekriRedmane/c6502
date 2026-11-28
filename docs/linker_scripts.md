# Linker scripts and sections

A program compiled from several source files is linked by
`c6502cpp compile` from their objects. Given a linker script with `-T`, the
linker places the program in memory: which code and data go in ROM
and which in RAM, at what addresses, with what alignment. The script
language is a subset of GNU ld's, so a script written for a GCC
toolchain mostly reads the same.

```sh
cpp/build/bin/c6502cpp compile -O -c main.c io.c   # main.o and io.o
cpp/build/bin/c6502cpp compile -T prog.ld main.o io.o -o prog
cpp/build/bin/c6502cpp objdump -h prog             # where each section went
```

The output is an ELF executable with a loadable segment for each output
section, at its load address. Without `-T`, `.text`, `.rodata`,
`.data`, and `.bss` follow each other from `$0800`, and any other
section comes after them; `-Ttext=ADDR` starts `.text` at `ADDR`
instead.

## Sections

Every function and every object with static storage is placed in a
*section*, named the way GCC names them:

| What | Section |
|---|---|
| a function | `.text` |
| a `const` object, and a string literal | `.rodata` |
| an object whose initial value is all zero, such as `int count;` | `.bss` |
| any other object | `.data` |

`[[c6502::section("name")]]` puts a function or object in a
section of your own:

```c
[[c6502::section(".text.boot")]] void reset(void);
const uint8_t font [[c6502::section(".rodata.font")]][768] = { ... };
uint8_t buffer [[c6502::section(".page")]][256];
```

A section name is letters, digits, `_`, and `.`. Every declaration
that names a section has to name the same one. The attribute goes on
a function or an object with static storage, not on an automatic
local, and not with `zeropage` or `address`, which place a name
themselves. A zero-page object has no section; the linker places
those separately ([attributes.md](attributes.md)).

## A script

```
MEMORY
{
    ram (rwx) : ORIGIN = 0x0800, LENGTH = 0x5800
    rom (rx)  : ORIGIN = 0x8000, LENGTH = 16K
}

SECTIONS
{
    .text : { *(.text.boot) *(.text .text.*) } > rom
    .rodata : { *(.rodata .rodata.*) } > rom
    .page ALIGN(256) : { *(.page) } > rom
    .data : { *(.data .data.*) } > ram AT> rom
    .bss (NOLOAD) : { *(.bss .bss.*) } > ram
    .vectors 0xFFFA : { KEEP(*(.vectors)) }
    __free_ram = ADDR(.bss) + SIZEOF(.bss);
}
```

`MEMORY` names regions with their first address and size. `SECTIONS`
lists the *output sections* in order. Each one collects the input
sections whose names match its patterns, in the order the patterns
are written, and within one pattern in the order of the input files
and of the items in each file. A function or object goes in the first
output section that matches it, so `*(.text.boot)` before
`*(.text .text.*)` puts the boot code first. An item that no output
section matches is a link error.

An output section goes:

- at the address written after its name (`.vectors 0xFFFA`),
- or else, with `> region`, right after the region's previous output
  section, starting at the region's origin,
- or else right after the previous output section.

`ALIGN(n)` after the colon rounds that address up to a multiple of
`n`, a power of two. `AT> region` gives the section a *load address*
in another region: its bytes are stored there, and its code runs at
the first address. `(NOLOAD)` puts no bytes in the program; such a
section can only hold `.bss`-style items, whose initial value is all
zero.

## Functions that fall into each other

Hand-written 6502 code often ends one routine where the next begins,
so that the first runs into the second without a `JMP`. Zork's
`read_next_sector` sets the buffer and runs into
`inc_sector_and_read`, which steps the sector and runs into
`read_from_sector`. In C each one is a function that ends by calling
the next, `return inc_sector_and_read();`, which the compiler turns
into `JMP inc_sector_and_read`.

Within an output section, the linker puts each function that ends
with a `JMP` to another function of the section right before that
one, and drops the `JMP`. When several functions end by jumping to
the same one, the first of them in the section's order is the one
placed in front of it, and the others keep their `JMP`s. A chain of
such functions is laid down where its first function would have
been, and everything else keeps the script's order. A function
whose jump would close a loop, such as two functions that each end
by jumping to the other, keeps its `JMP`. No item moves across an
assignment inside the section (`. = ...` or `sym = .;`), since an
assignment marks an address.

In Zork's interpreter (examples/zork) this removes 28
`JMP`s, 84 bytes. Two of the pairs are functions of different units:
`print_obj` in `objects.c` falls into `print_zstring` in `zstring.c`,
and `store_and_branch_on@tail0`, the tail that `store_and_branch_on`
in `objects.c` shares with another function and falls into, falls
into `branch` in `decode.c`.

Moving a function changes its address. For a section whose order
matters, `(NOREORDER)` after the section's name keeps the order, and
then only a function already right before the one it jumps to loses
its `JMP`:

```
.text (NOREORDER) : { *(.text .text.*) } > rom
```

`(NOREORDER)` isn't part of GNU ld's script language.

## What's supported

- `MEMORY` regions, `name [(attrs)] : ORIGIN = n, LENGTH = n`, also
  spelled `org`, `o`, `len`, `l`. The attributes are ignored.
- Output sections, `name [address] [(NOLOAD)] : [AT(address)]
  [ALIGN(n)] { ... } [> region] [AT> region]`, with `(NOREORDER)` in
  place of `(NOLOAD)` to keep the order of its functions.
- Inside one: `file(pattern ...)`, `KEEP(file(pattern ...))` (the same,
  since nothing is discarded), symbol assignments, and `. = expr`,
  `. += n` to skip ahead. Patterns use `*`, `?`, and `[...]`. `file`
  is `*` for every input, or a pattern matched against an input
  file's name without its directory, so `zstring.o(.text)` takes
  only that unit's code. A C source or `.asm` file given to the link
  goes by its object's name, `zstring.o` for `zstring.c`.
  examples/zork/interpreter.ld puts the six units that run
  on every Z-machine instruction in the low region this way.
- Symbol assignments `sym = expr;` and `PROVIDE(sym = expr);`, inside
  output sections or between them.
- Expressions: numbers (`0x` hex, decimal, `K` and `M` suffixes),
  symbols, `.`, `+ - * / & | ^ << >>`, unary `-` and `~`, parentheses,
  `ALIGN(n)`, `ALIGN(expr, n)`, `ORIGIN(region)`, `LENGTH(region)`,
  `ADDR(section)`, `LOADADDR(section)`, and `SIZEOF(section)`.
- Comments `/* ... */` and `//`.

`ENTRY`, `INCLUDE`, `/DISCARD/`, `FILL`, `BYTE`, `SORT`, and `. =`
between output sections are errors.

Every output section's bytes, at its run address and at its load
address, must lie in the 6502's 64K, $0000-$FFFF. A section that
starts below $0000 or runs past $FFFF is an error, such as `section
.bss runs at $1AF1 with 480000 bytes, outside the 6502's address space
($0000-$FFFF)`. An empty section may start at $10000, so the sections
after one that fills memory to the top are no error. Without a script,
an origin (`-Ttext=ADDR`) outside $0000-$FFFF is an error too.

## The runtime library

A multiplication, a division or remainder, and a shift by a variable
count compile to calls of the runtime's helpers, such as `mul16`,
`sdivmod16`, and `asl8`. The linker adds the helpers a program calls
from the runtime library (cpp/src/passes/runtime_library.cpp), as a GCC
toolchain adds routines from libgcc. Only those go in the program,
together with the helpers they call (`sdivmod16` calls `udivmod16`).
The library is one more input file to the link, named `runtime.o`,
with each helper a function in `.text`, so a script places the
helpers by that name:

```
    .runtime : { runtime.o(.text) } > rom
```

A pattern that takes every file's code, such as `*(.text .text.*)`,
takes them too, and a script with neither fails the link on the
first helper it leaves out:

```
c6502: link error: no output section in the script takes `udivmod16` (section .text, runtime.o)
```

Without a script, the helpers follow the program's code in `.text`. A
function that ends with a jump to a helper falls into it when the two
end up next to each other, as with any two functions.

A helper that a file defines, or whose address `--defsym` gives, isn't
taken from the library. That's how the tests run a program on the
simulator's copies of the helpers (`sim::defsym_args`). The
helpers for floating point, for 64-bit integers, and for calls
through a pointer have no 6502 code yet, so a program that calls one
links only when `--defsym` gives its address:

```
c6502: link error: a.o: undefined symbol `fadd` (a runtime helper that has no 6502 code in the runtime library yet, so give its address with --defsym)
```

The helpers work in `HARGS`, at the address the linker gives it, and
the linker makes the block as large as the largest of the helpers in
the program needs.

## Symbols

Each output section defines `__name_start` and `__name_end` (its run
addresses), `__name_load` and `__name_load_end` (its load addresses),
and `__name_size`, with the section's dots turned into underscores:
`.text.boot` gives `__text_boot_start`.
`ADDR`, `LOADADDR`, and `SIZEOF` in the script use them, and so can
assembly or C code that declares them `extern`.

## Startup

The boot code calls `__zp_init` before `main` (it initializes the
zero-page globals). With a script, `__zp_init` also copies each
section with `AT>` or `AT(...)` from its load address to its run
address, and fills each `NOLOAD` section with zeros, so `.data` in
ROM and `.bss` in RAM hold the values C expects when `main` starts.
The copy uses `DPTR` and the first four bytes of `HARGS`.

## How it works

Each function and object is a section of its own in its object, with
its size, so the linker computes every address itself: a section in a
region starts at the end of the region's previous section, and its
items follow each other. A section that runs past the end of its
region, or whose load bytes run past the end of its load region,
fails the link:

```
c6502: link error: section .text runs past the end of region rom by 12 bytes
```

Before laying the program out, the linker gives each instruction the
addressing mode the program's addresses call for. An absolute operand
the program puts in zero page, such as a global that another unit
moved into zero page, gets its zero-page form, one byte shorter
(`ld::relax` in cpp/src/binutils/ld.h), and a unit that reached a name in zero page
that the program puts above $FF is assembled again from the text the
object keeps. Then it applies each relocation. A segment of the
executable runs at its section's run address and is loaded at its
load address, which is how `.data` goes in ROM to be copied to RAM.

`c6502cpp objdump -D` writes the executable as dasm source, each
output section a `SEG` (`SEG.U` for `NOLOAD`) with its `ORG`, and a
`RORG` for a section with a load address. examples/zork/interpreter.asm
is that listing of Zork's linked interpreter.

## Zero page

The linker lays out zero page for the whole program too. It puts the
runtime's block (`SSP`, `FP`, `HARGS`, `DPTR`) around every file's
fixed zero-page objects, with `HARGS` as large as the largest any file
needs, then the zero-page globals, then the functions' parameter and
local bytes, which may use any free byte from `$80` up, and then any
free byte below `$80`, around the runtime's block and every
zero-page object. A program whose headers reserve most of `$80`-`$FF`,
as examples/zork's does for Zork's own zero page, keeps
its parameters and locals in zero page that way. A parameter or
local that doesn't fit in either goes in `__spill`, a block in `.bss`, which the
script places with the rest of the data. Without a script it goes at
`$0200` up instead. A local byte that no file's code names any more
(the peepholes emptied it after the file sized its pool) gets no
address. Parameter bytes that hold one value for several functions,
possibly in different files, get one address, after the linker checks
that nothing in the program writes them while a value is kept there
([optimizations/slot_sharing.md](optimizations/slot_sharing.md)).

## Names

An internal name, a `static` function or object or a string literal,
belongs to its own source file, and two files may each have one
called `count`. The linker renames each of them after the file it came
from, `count.0` in the first and `count.1` in the second. Two files
defining the same external name is an error.

Each file names a function's zero-page parameter and local bytes after
the function (`__zpabi_f__x`, `__local_f__t`), so when two files each
have a `static` function `f`, the linker renames those bytes with it
(`__zpabi_f__x.0`, `__zpabi_f__x.1`). It renames the function in the
file's call graph too, so the two functions get their bytes as two
functions do. A function that the compiler makes from code several
functions of a file share, a shared tail (`f@tail0`) or an outlined run
(`f@run0`), is an internal name of its file as well.

## Tables several files make

Under `-O`, a file that indexes a 2-D array makes tables of the
array's row addresses, one of their low bytes (`A@lo`) and one of
their high bytes (`A@hi`)
([optimizations/row_address_tables.md](optimizations/row_address_tables.md)).
Every file that indexes the array makes the same tables, so the linker
keeps one copy of each. Two tables are the same when they have the
same bytes and their relocations name the same things, an external
name by its spelling and an internal one only within its own file. So
two files' tables of one `extern` array become one table, and the
tables of two files' `static` arrays of one name stay two. The copy
the linker keeps is the first, in the order of the files on the
command line, and it stays where the script puts that file's items.
Every other file's references to its own copy go to the kept one,
with their offsets (`A@hi+1,X`).

Only tables the compiler marks in the file's link metadata
(`merge=true` on the `item` line) are merged. C requires distinct
objects to have distinct addresses, so the linker never merges a
program's own objects, even ones with the same bytes. Nothing takes
the address of a table of row addresses, and nothing writes one, so a
program behaves the same whichever copy its lookups read.

Lode Runner (examples/lode_runner) indexes `LEVEL_MAP` and `LEVEL_BG`
in five files, which made 238 bytes of these tables, and the linked
program keeps the 51 bytes of `level.c`'s.

## Related

[cpp/src/passes/link_script.h](../cpp/src/passes/link_script.h) parses
scripts, [cpp/src/passes/sections.h](../cpp/src/passes/sections.h)
picks each item's section, and
[cpp/src/passes/link_layout.h](../cpp/src/passes/link_layout.h) writes
the layout. [calling_conventions.md](calling_conventions.md) covers
what else the linker does: the zero-page parameter and local bytes of
every function.
