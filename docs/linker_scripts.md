# Linker scripts and sections

A program compiled from several source files is joined with
`compile.py --link`. Given a linker script with `-T`, the linker also
places the program in memory: which code and data go in ROM and which
in RAM, at what addresses, with what alignment. The script language
is a subset of GNU ld's, so a script written for a GCC toolchain
mostly reads the same.

```sh
uv run python compile.py main.c --codegen --optimize -o main.asm
uv run python compile.py io.c --codegen --optimize -o io.asm
uv run python compile.py --link -T prog.ld main.asm io.asm -o prog.asm
dasm prog.asm -f2 -oprog.bin
```

Without `-T`, the linked output has no addresses, as before, and
whatever assembles it decides where it goes.

## Sections

Every function and every object with static storage is placed in a
*section*, named the way GCC names them:

| What | Section |
|---|---|
| a function | `.text` |
| a `const` object, and a string literal | `.rodata` |
| an object whose initial value is all zero, such as `int count;` | `.bss` |
| any other object | `.data` |

`__attribute__((section("name")))` puts a function or object in a
section of your own:

```c
__attribute__((section(".text.boot"))) void reset(void);
const uint8_t font[768] __attribute__((section(".rodata.font"))) = { ... };
uint8_t buffer[256] __attribute__((section(".page")));
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

## What's supported

- `MEMORY` regions, `name [(attrs)] : ORIGIN = n, LENGTH = n`, also
  spelled `org`, `o`, `len`, `l`. The attributes are ignored.
- Output sections, `name [address] [(NOLOAD)] : [AT(address)]
  [ALIGN(n)] { ... } [> region] [AT> region]`.
- Inside one: `file(pattern ...)`, `KEEP(file(pattern ...))` (the same,
  since nothing is discarded), symbol assignments, and `. = expr`,
  `. += n` to skip ahead. Patterns use `*`, `?`, and `[...]`. `file`
  is `*` for every input, or a pattern matched against an input
  file's name without its directory, so `zstring.asm(.text)` takes
  only that unit's code. examples/zork/print_zstring/print_zstring.ld
  places each unit where Zork keeps the original routines this way.
- Symbol assignments `sym = expr;` and `PROVIDE(sym = expr);`, inside
  output sections or between them.
- Expressions: numbers (`0x` hex, decimal, `K` and `M` suffixes),
  symbols, `.`, `+ - * / & | ^ << >>`, unary `-` and `~`, parentheses,
  `ALIGN(n)`, `ALIGN(expr, n)`, `ORIGIN(region)`, `LENGTH(region)`,
  `ADDR(section)`, `LOADADDR(section)`, and `SIZEOF(section)`.
- Comments `/* ... */` and `//`.

`ENTRY`, `INCLUDE`, `/DISCARD/`, `FILL`, `BYTE`, `SORT`, and `. =`
between output sections are errors.

## Symbols

Each output section defines `__name_start` and `__name_end` (its run
addresses) and `__name_load` (its load address), with the section's
dots turned into underscores: `.text.boot` gives `__text_boot_start`.
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

Section sizes aren't known until the program is assembled, so the
linker writes the layout as dasm directives and leaves the arithmetic
to dasm. Each output section is a `SEG` (`SEG.U` for `NOLOAD`) with
an `ORG` at its address, which for a section in a region is the end
label of the region's previous section. A section with a load address
puts its bytes at the `ORG` and runs at a `RORG`. After each section,
an `IF` / `ERR` block stops dasm with a message if the section runs
past the end of its region:

```
section .text runs past the end of region rom by 12 bytes
```

dasm's `-f2` format keeps each segment at its own address, which a
program with code in ROM and data in RAM needs.

## Zero page

The linker lays out zero page for the whole program too. It puts the
runtime's block (`SSP`, `FP`, `HARGS`, `DPTR`) around every file's
fixed zero-page objects, with `HARGS` as large as the largest any file
needs, then the zero-page globals, then the functions' parameter and
local bytes, which may use any free byte from `$80` up. A parameter or
local that doesn't fit goes in `__spill`, a block in `.bss`, which the
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
called `count`. The linker renames one of them `count.1` in the file
it came from. Two files defining the same external name is an error.

## Related

[passes/link_script.py](../passes/link_script.py) parses scripts,
[passes/sections.py](../passes/sections.py) picks each item's
section, and [passes/link_layout.py](../passes/link_layout.py) writes
the layout. [calling_conventions.md](calling_conventions.md) covers
what else the linker does: the zero-page parameter and local bytes
of every function.
