# c6502

c6502 is an experimental optimizing C99 compiler for the MOS 6502,
with some C23 features, written in Python. It writes assembly in
[dasm](https://dasm-assembler.github.io/)'s syntax, and it has its own
assembler and linker, which write ELF objects and executables.

## Status

It isn't finished, and the gaps listed below matter for a program of
any size. It's far enough along to compile two whole programs, each
linked from several source files. They are Zork I's interpreter
([examples/zork](examples/zork/README.md)) and Broderbund's Lode
Runner ([examples/lode_runner](examples/lode_runner/README.md)). The
tests run each one on a simulated Apple II beside the original and
compare what the two do with the Monitor, DOS, and the hardware, call
for call. Those runs use files that aren't in this repository (see
"Running the tests").

These parts work.

- Most of C99 compiles. That includes every integer type, `float`
  and `double` as types, pointers, arrays, structs and unions (passed
  and returned by value), enums, typedefs, `switch`, `goto`, function
  pointers, compound literals, `static` and `extern` across source
  files, `const`, and `volatile`.
- C23's attribute syntax, `[[...]]`, is accepted, with the standard
  attributes and c6502's own. The compiler's attributes put objects
  and parameters in zero page, bind names to the addresses of ROM
  routines and I/O registers, pass parameters in registers, return a
  `bool` in the carry flag, place code in sections, and unroll loops.
- C23's `bool`, `true`, and `false` are keywords. `true` and `false`
  are constants of type `bool`, so `sizeof(true)` is 1, and they
  work in constant expressions and in `#if`, where `true` is 1 and
  `false` is 0. `<stdbool.h>` is C23's, which defines only
  `__bool_true_false_are_defined`. The older spelling `_Bool` still
  compiles, with a warning at each use, since C23 makes it
  obsolescent. A program that names something `bool`, `true`, or
  `false`, as C99 allowed, no longer compiles.
- C23's `nullptr` is a keyword, a null pointer constant of type
  `nullptr_t`, which `<stddef.h>` declares. It converts to any
  pointer type and to `bool`, and it compares equal to `0`, to
  `NULL`, and to every null pointer. Converting it to an integer, or
  comparing it with `<`, is an error, as C23 specifies. The compiler
  treats it as the constant 0 from the type checker on.
- C23's binary constants, such as `0b1010'0101`, and digit
  separators, such as `1'000'000` and `0xFF'FF`, work in C and in
  `#if`. A separator anywhere but between two digits is an error
  that says where it is.
- C23's bit-precise integer types, `_BitInt(N)` and `unsigned
  _BitInt(N)`, work for N up to `BITINT_MAXWIDTH`, which is 64, with
  the `wb` and `uwb` constant suffixes, in C and in `#if`. They aren't
  promoted, so `_BitInt(5) + _BitInt(5)` is a `_BitInt(5)`, and
  arithmetic on them wraps at N bits. c6502 wraps a signed one too,
  where C leaves overflow undefined. A value takes ceil(N/8)
  bytes, so an `unsigned _BitInt(24)` is 3 bytes, and adding,
  subtracting, comparing, and the bitwise operators work on those 3
  bytes directly. A multiply, divide, or shift by a variable of a 3-,
  5-, 6-, or 7-byte value uses the runtime's helper for the next of 4
  and 8 bytes.
- C23's `<stdbit.h>` has all 14 of its operations, from
  `stdc_leading_zeros` to `stdc_bit_ceil`, in the forms of one type
  and the type-generic forms, and its endian and version macros. The
  compiler folds a call of a constant, which makes it a constant
  expression, and writes a short loop in place for any other call
  ([docs/stdbit.md](docs/stdbit.md)).
- The optimizer, `-O`, keeps values in zero page and in the A, X, and
  Y registers, gives each function that can have one a frame in fixed
  zero-page bytes, and runs more than a hundred smaller rewrites.
- The linker links several objects and reads GNU ld-style linker
  scripts.

These parts are missing.

- There's no runtime library. Multiplying, dividing, and taking a
  remainder by a value that isn't a constant, shifting by a variable
  count, every floating-point operation, and calling through a
  function pointer all compile to calls of helper routines (`mul16`,
  `udivmod16`, `asl16`, `fadd`, `icall`, and so on), and no library
  to link them from is in the repository. A program that uses them
  links only when `--defsym` gives each helper's address, so you have
  to write the helpers. The simulator has 6502 versions of the
  integer helpers (`sim/runtime_helpers.py`) and Python stand-ins
  for the floating-point ones, which is how the tests run such
  programs.
- There's no startup code. Something has to set the soft stack
  pointer `SSP`, call `__zp_init`, and then call `main` (see
  [Zero page](#zero-page) below).
- There's no standard library. The only headers are `<stdint.h>`,
  `<stddef.h>`, `<stdbool.h>`, `<stdbit.h>`, `<limits.h>`, and two
  that declare the Apple II Monitor's routines,
  `<apple2/monitor_io.h>` and `<apple2/interactive_monitor.h>`.
- Floating point has no 6502 code at all, as described above.
  `long double` and hexadecimal floating constants are rejected, and
  `==` on `float` or `double` compares bit patterns, so `0.0 == -0.0`
  is false.
- Variadic functions, bit-fields, designated initializers,
  variable-length arrays, `inline`, and `_Complex` are rejected. So
  are a few initializer forms, such as the flat list for a nested
  array (`int a[2][3] = {1, 2, 3, 4, 5, 6};`).

## Installation

1. Install Python 3.11 or later.
2. Install [uv](https://docs.astral.sh/uv/getting-started/installation/).
3. Clone the repository with
   `git clone https://github.com/XekriRedmane/c6502.git`, and `cd`
   into it.
4. Run `uv sync`. It creates a virtual environment in `.venv` and
   installs the dependencies, including the preprocessor,
   [pcpp](https://github.com/ned14/pcpp), and the development
   dependencies pytest and pytest-xdist.
5. Check that it works with
   `uv run python -m unittest tests.test_example_outputs`. It
   compiles every example in `examples/` and compares the output
   with the checked-in `.asm` files, and it should print `OK`.
6. Optionally, install dasm and put it on your `PATH`. Compiling,
   linking, and running programs in the simulator don't use it, but
   the tests that compare c6502's assembler with dasm run only where
   dasm is installed.

`requirements.txt` lists the same packages for a plain `pip`
environment. It's kept by hand and may be behind `uv.lock`.

## Usage

`compile.py`'s flags follow gcc's where the two have the same
options. Every command below runs from the repository's directory.

```sh
uv run python compile.py prog.c -S             # assembly in prog.s
uv run python compile.py prog.c -S -O          # the same, optimized
uv run python compile.py prog.c -S -O -o -     # to stdout
uv run python compile.py -O -c a.c b.c         # objects a.o and b.o
uv run python compile.py a.o b.o -o prog       # link them
uv run python compile.py -O -T prog.ld a.c b.o -o prog   # both, with a linker script
uv run python compile.py a.o -o prog --defsym mul16=0x9000   # give a helper's address
uv run python nm.py a.o                        # an object's symbols
uv run python objdump.py -d a.o                # disassembly, in dasm's syntax
```

Without `-S` or `-c`, the inputs (C sources, `.asm` files, and
objects) are linked into an ELF executable, `a.out` unless `-o` names
it. Without `-T`, the code starts at `$0800`, and `-Ttext=ADDR` moves
it. Flags that `compile.py` doesn't recognize go to the preprocessor,
so `-D`, `-U`, and `-I` work as usual. `--lex`, `--parse`,
`--resolve`, and `--tac` stop after the stage they name and print
that stage's output.

An error stops the compile with one line on stderr, `<file>: error:
<message>`, and exit status 1. A warning, such as the one for each
use of `_Bool`, is a line of the form `<file>: warning: line N:
<message>`, and the compile goes on.

### An example

```c
#include <stdint.h>

uint8_t table[16];

uint8_t sum(void) {
    uint8_t total = 0;
    for (uint8_t i = 0; i < 16; i++)
        total += table[i];
    return total;
}
```

`uv run python compile.py sum.c -S -O -o -` prints this.

```
SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def sum params= locals=__local_sum__0,__local_sum__total indirect=false in_cycle=false
; item sum section=.text local=false zero=false
; item table section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

sum:
   SUBROUTINE

   LDA   #$00
   LDX   #$00
   BEQ   .loop@0_start
.sum@rot@0:
   ADC   table,X
   INX
.loop@0_start:
   CPX   #$10
   BCC   .sum@rot@0
   RTS

table:
   DS.B  16
```

`total` stays in A and `i` in X for the whole loop, and the result
comes back in A. The `ADC` has no `CLC` in front of it because the
only way to reach it is the `BCC`, which is taken only when C is
clear. The comment block between the `EQU` lines and the code is
read by the linker, which uses it to place the function's zero-page
bytes when it links several files. The function is 15 bytes. Without
`-O` it's 228 bytes, since every variable and temporary then lives on
the soft stack. With `int` in place of both `uint8_t`s, `-O` makes it
48 bytes, for the reason the next section gives.

### How C maps onto the 6502

The C types have these sizes on the 6502, which differ from their
sizes on 32- and 64-bit machines.

| Type | Size | Range |
|---|---|---|
| `char` | 1 byte | 0 to 255. Plain `char` is unsigned. |
| `signed char` | 1 byte | -128 to 127 |
| `short`, `int` | 2 bytes | -32768 to 32767 |
| `long` | 4 bytes | -2^31 to 2^31 - 1 |
| `long long` | 8 bytes | -2^63 to 2^63 - 1 |
| a pointer | 2 bytes | `$0000` to `$FFFF` |
| `float`, `double` | 4 and 8 bytes | IEEE 754, with no arithmetic code (see "Status") |
| `bool` | 1 byte | 0 (`false`) or 1 (`true`). `_Bool` is the same type. |
| `nullptr_t` | 2 bytes | Only `nullptr`, stored as 2 zero bytes. |
| `_BitInt(N)`, `unsigned _BitInt(N)` | ceil(N/8) bytes | -2^(N-1) to 2^(N-1) - 1, or 0 to 2^N - 1, for N up to 64. The bits above the value hold its sign (or 0). |

`<stdint.h>` defines `uint8_t` as `unsigned char`, `int16_t` as
`int`, `int32_t` as `long`, and so on. `sizeof` gives an `unsigned
long`, and the difference of two pointers is an `int`, which
`<stddef.h>` names `size_t` and `ptrdiff_t`. `NULL` is `((void
*)0)`. Since `int` is
16 bits, `40000` is a `long` constant, and `1 << 16` is undefined.
A binary constant takes the types a hexadecimal one does, so
`0b1111'1111'1111'1111` is an `unsigned int`, and a digit separator
`'` can go between any two digits of a constant.

The 6502 does arithmetic a byte at a time, so every byte of a value
costs code and time. C promotes `char` operands to `int` before any
arithmetic, which makes `a + b` on two `uint8_t`s a 16-bit add. `-O`
narrows such arithmetic back to one byte when only the low byte is
used, as when the sum is stored in a `uint8_t`, but a value kept in
an `int` stays two bytes everywhere it goes. Declaring a counter, an
index, or a flag `uint8_t` wherever its values fit saves code, as the
example above shows.

A multiplication by a constant power of two becomes a shift, and so
do the division and remainder of an unsigned value by one. A
multiplication by a constant such as 3, 5, 10, or 12, which is the
sum or difference of two powers of two, becomes two shifts and an add
or a subtract. Every other multiplication, division, remainder, and
shift by a variable count calls a helper, which the program has to
supply.

#### Zero page

The first 256 bytes of memory have shorter, faster instructions, and
the 6502 can follow a pointer only if it's in zero page. c6502 keeps its runtime's bookkeeping in `$00`-`$25` (the soft
stack pointer `SSP`, the frame pointer `FP`, the helper exchange
block `HARGS`, and a pointer scratch `DPTR`), and with `-O` it keeps
the functions' parameters and locals in `$80`-`$FF`. The bytes
between, `$26`-`$7F`, are for globals marked
`[[c6502::zeropage]]`, and with `-O` the compiler moves the busiest
scalar globals there itself. Since zero page isn't part of the
program's image, the compiler writes a routine `__zp_init` that
stores those globals' initial values, and the startup code has to
call it before `main`. A program that shares zero page with a ROM or
an operating system declares the bytes those use, and the runtime's
block moves out of their way.

#### Calling conventions

There are two. Without `-O`, every function keeps its arguments and
locals on a *soft stack*, a stack in main memory reached through
`SSP` and `FP` with `(FP),Y` instructions, which costs a prologue and
an epilogue on each call. With `-O`, each
function that qualifies gets its arguments and locals in fixed
zero-page bytes instead, and has no frame at all. Among other
conditions, a function that qualifies isn't recursive, doesn't call
through a pointer, and doesn't have its address taken. Two functions
that are never active at the same time share those bytes. `register`
on a parameter passes it in A or X, and `[[c6502::in_y]]` passes it
in Y. [docs/calling_conventions.md](docs/calling_conventions.md)
traces a call through each convention.

#### Recursion

Recursion works, on the soft stack. A soft-stack frame holds at most
253 bytes of locals, because its offsets are one byte, and
nothing checks that the soft stack stays clear of the program. Each
call in progress also keeps a 2-byte return address on the 6502's own
256-byte stack, which limits recursion to about 120 calls deep.

#### I/O registers

Declare an I/O register as a `volatile` object at its address.

```c
extern volatile uint8_t KBD [[c6502::address(0xC000)]];      /* bit 7 set: a key is waiting */
extern volatile uint8_t KBDSTRB [[c6502::address(0xC010)]];  /* touch it to clear the key */
extern volatile uint8_t SPKR [[c6502::address(0xC030)]];     /* touch it to click the speaker */
```

A read of a soft switch like `SPKR` generally interacts with
hardware, and usually its value doesn't matter and can be thrown
away. Without `volatile`, the
optimizer deletes a read whose value isn't used, so `(void)SPKR;`
compiles to nothing. A loop that waits for a key, `while (!(KBD &
0x80)) {}`, changes nothing the optimizer can see either, so it's
deleted too, and the program goes on without waiting. With
`volatile`, each read in the source is one `LDA` and each write one
`STA`, in the order the source has them: `(void)SPKR;` is `LDA SPKR`,
and `KBDSTRB = 0;` is a store to `$C010`.

`volatile` doesn't keep a loop that only spends time. A delay loop
with nothing in its body is deleted, as C allows. Put
`[[c6502::keep]]` before it to keep it, and it compiles to a count
down in X or Y.

```c
[[c6502::keep]] for (uint8_t i = 0xB4; --i;) {
}
```

## Documentation

- [docs/attributes.md](docs/attributes.md) describes every attribute,
  where each one goes, and the errors the compiler reports, and walks
  through declaring and calling the Apple II Monitor's routines.
- [docs/builtins.md](docs/builtins.md) describes
  `__c6502_bcd_add` and `__c6502_bcd_sub`, which add and subtract
  packed BCD numbers with the 6502's decimal mode, for scores and
  other decimal counters.
- [docs/stdbit.md](docs/stdbit.md) describes C23's `<stdbit.h>`, the
  bit counts, the bit searches, and the powers of 2 it rounds to,
  what each returns, and the loops they compile to.
- [docs/calling_conventions.md](docs/calling_conventions.md) traces a
  call through the soft stack and through zp_abi, says which
  functions qualify for zp_abi, and gives the costs and limits of
  each.
- [docs/linker_scripts.md](docs/linker_scripts.md) covers sections,
  the subset of GNU ld's script language the linker reads, the
  symbols it defines, and the copies and clears that `__zp_init` does
  at startup.
- [docs/optimizations/README.md](docs/optimizations/README.md) shows
  how `-O` is organized and lists every optimization, alphabetically
  and in the order they run, each with a before-and-after example.

## Working on the compiler

[docs/architecture.md](docs/architecture.md) describes the pipeline,
the four intermediate representations, and the runtime conventions.
Each directory's `CLAUDE.md` describes the modules in it.

### Running the tests

```sh
uv run python -m unittest                          # every test
uv run python -m unittest tests.test_asm_emit      # one module
C6502_CHECK_IR=1 uv run python -m unittest         # check the IR after every pass
C6502_CHECK_IR=1 uv run pytest -n auto --dist worksteal -q   # the same, in parallel
```

The parallel run takes about 70 seconds, against about 4 minutes in
one process. `C6502_CHECK_IR=1` checks the IR after every optimizer
pass and at each stage boundary, and adds about 3% to the time.

The runs of the two example programs read files from each program's
reverse-engineering project, which aren't in this repository. Without
them, the runs are skipped, and the comparisons of each unit's output
with its checked-in `.asm` file still run.

- `ZORK_DSK` names the disk image of Zork I r15, `Zork I r15 (4am
  crack).dsk` from
  [zorki-r15-reveng](https://github.com/XekriRedmane/zorki-r15-reveng).
  `tests/test_zork_interpreter.py` then plays two sessions on the
  compiled interpreter and on Zork's own, read from that image.
- `LODE_RUNNER_DIR` names the directory of
  [lode_runner_reveng](https://github.com/XekriRedmane/lode_runner_reveng),
  which holds `main.bin`, `lode_runner.do`, and `main.asm`.
  `tests/test_lode_runner.py` then plays three scripts of keys on the
  compiled game and on the original. The demo takes about four
  minutes.

[tests/CLAUDE.md](tests/CLAUDE.md) describes how the tests are
organized.

### The fuzzers

The fuzzers generate random C programs and check that the TAC
interpreter, the unoptimized code, and the optimized code all give
the same result. They're worth running after changing a pass, the ABI
selection, or the register allocator.

```sh
uv run python -m fuzz.exprs --start 0 --count 300 -j 12   # expressions, in 12 processes
uv run python -m fuzz.calls --start 0 --count 300         # calling conventions
uv run python -m fuzz.stmts --start 0 --count 300         # control flow and storage
uv run python -m fuzz.build_yarpgen                       # once, before the next line
uv run python -m fuzz.yarpgen --start 0 --count 100       # YARPGen's programs
uv run python -m fuzz.reduce fuzz_out/calls/calls534.c    # shrink a failing program
```

Failing programs land in `fuzz_out/`. [fuzz/CLAUDE.md](fuzz/CLAUDE.md)
describes each tool.

### Regenerating the AST modules

Each `*_ast.py` module is generated from the matching `.asdl` file by
`asdl.py`. After editing an ASDL file, regenerate its module.

```sh
uv run python asdl.py c99.asdl c99_ast.py
uv run python asdl.py tac.asdl tac_ast.py
uv run python asdl.py asm.asdl asm_ast.py
uv run python asdl.py asm2.asdl asm2_ast.py
```
