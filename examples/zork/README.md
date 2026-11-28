# Zork's interpreter, in C

This is the whole Z-machine interpreter of Zork I (r15) for the Apple
II, rewritten in C as eleven source files and linked with a linker
script into a program that runs where the original does, from `$0800`.
The original is `output/main.asm` of
https://github.com/XekriRedmane/zorki-r15-reveng, and every one of its
lines from 249 to 3796 is translated in one of the files below.

It plays Zork. On two test sessions it makes the same calls to the
Monitor and to DOS's RWTS as Zork's own interpreter, with the same
characters at the same cursor positions, and writes the same sectors
(see "Compared with Zork's own code"). It is 84 bytes smaller than the
original, but the linker script puts part of it past the 26 sectors
that Zork's boot loader reads (see "Size").

Two earlier examples, since removed, translated pieces of the same
code, Zork's z-string printer and its `save` instruction. This example
started from their `print_zstring`, page cache, line buffer, prompts,
and `instr_save`, and passes values as arguments where those pieces
had to keep Zork's `SCRATCH1` and `SCRATCH2`, since here no assembly
code calls them. The disk routines are the
exception. They read the sector and the buffer from two globals,
`disk_sector` and `disk_buffer`, as Zork's read them from `SCRATCH1`
and `SCRATCH2`, so that `save` and `restore` step the sector inside
`inc_sector_and_read` and `inc_sector_and_write` as Zork does, where
a sector number passed as an argument has to be stepped at each
call.

## Files

| File | What's in it | Original lines |
|---|---|---|
| `zork.h` | The interpreter's zero page and memory, `uint24_t`, the IOB and DCT structures, `RWTS`, and each unit's routines | |
| `main.c` | `main`, `locate_last_ram_page`, `instr_restart`, `instr_quit`, and the internal error | 249-403, 3025-3049, 3773-3790 |
| `decode.c` | The instruction decoder and its four dispatch switches, operands, variables, the stack, branches, calls and returns, and the instructions of a few lines | 404-907, 922-963, 1019-1057, 1126-1227, 1238-1269, 1310-1318, 1341-1359, 1464-1473, 1538-1703, 2020-2031, 2084-2095, 2306-2357 |
| `objects.c` | The object tree, attributes, and properties | 964-1018, 1058-1125, 1228-1237, 1270-1309, 1319-1340, 1360-1463, 1704-1740, 2191-2305 |
| `numbers.c` | `mul`, `div`, `mod`, `random`, and printing numbers | 1474-1537, 2032-2083, 2096-2190, 3791-3796 |
| `input.c` | `sread`: the status line, reading a line, the tokenizer, and the dictionary search | 1741-2019, 2810-3024, 3234-3281, 3295-3347 |
| `pages.c` | The page cache and the two readers of the story file | 2358-2597 |
| `zstring.c` | The z-string decoder and `print_string_literal` | 908-921, 2598-2809 |
| `output.c` | The line buffer, `[MORE]`, the printer, `home`, `reset_window`, and printing ASCII strings | 3050-3233, 3282-3294, 3348-3358, 3449-3466 |
| `disk.c` | The IOB and DCT, and reading and writing a sector | 3359-3448 |
| `prompts.c` | Asking for the save diskette and giving the game diskette back | 3467-3633 |
| `save.c` | `save` and `restore` | 3634-3772 |
| `interpreter.ld` | The linker script | |
| `interpreter.asm` | The listing of the program the eleven link into by the script | |

Each `.c` file's header lists where it departs from the original and
then quotes the original code it translates.

## Building

```sh
cd examples/zork
for f in main decode objects numbers input pages zstring output disk prompts save; do
    ../../cpp/build/bin/c6502cpp compile $f.c -c -O
done
../../cpp/build/bin/c6502cpp compile -T interpreter.ld \
    main.o decode.o objects.o numbers.o input.o pages.o \
    zstring.o output.o disk.o prompts.o save.o -o interpreter
../../cpp/build/bin/c6502cpp objdump -D --no-show-raw-insn --no-addresses \
    interpreter > interpreter.asm
```

Each unit is compiled to its object, which the link reads. The link
writes the ELF executable `interpreter`. Neither the objects nor the
executable are checked in. Each unit's `-S -O` text is in
golden/O/examples/zork/, and `c6502cpp compile $f.c -S -O -o $f.s`
writes it. `interpreter.asm` is the executable's
listing in dasm's syntax, with a `SEG` and an `ORG` for each output
section, and cpp/src/sim/harness.h's `build_linked_sim` loads the
executable for the simulator. golden/O.txt and golden/links.txt check
each unit's output and the linked program against fresh compiles, and
cpp/tests/test_zork_interpreter.cpp checks that the program calls none
of the runtime's helpers and doesn't use the soft stack. With
`ZORK_DSK` set to the path of `Zork I r15 (4am crack).dsk` from the
repository above, cpp/tests/test_zork_interpreter_sim.cpp also plays
two sessions on this interpreter and on Zork's own and compares them.
The disk image isn't part of this repository.

## How it's put together

**Memory.** Zork's boot loader leaves DOS 3.3's RWTS at `$2400`-`$2BFF`,
and the interpreter has to work around it, as the original does. The
original fills `$0800`-`$21FF`, keeps its page tables at `$2200`-`$23FF`,
and loads the story from `$2C00`. `interpreter.ld` has two regions.
`main` goes first at `$0800`, since the boot loader jumps there and
Zork goes back there to start over, followed by the six units that run
on every instruction, up to `$1AAD`. The other units, the constants,
the data, and the page tables go from `$2C00` to `$37FF`. The page
tables start on a page, `$3600`, as Zork's do, because
`find_index_of_page_table` reads them with `(zp),Y` on every page
lookup, and a read that crosses a page takes an extra cycle. The
script defines `__story`, the first page after them, `$3800`, and
`main` loads the story there. The screen at `$0400`-`$07FF` and the
line buffer at `$0200`, which is the Monitor's input buffer, are
where the Monitor keeps them.

**Zero page.** `zork.h` declares the interpreter's state at Zork's
addresses in `$7F`-`$EF`, so that a save holds the same bytes as
Zork's: `Z_PC`, the 30 bytes of locals, and the six bytes of the
stack's state. It leaves out the bytes Zork uses only for scratch
(`SCRATCH1`-`SCRATCH3`, `SIGN_BIT`, `TOKEN_IDX`, `INPUT_PTR`,
`TMP_Z_PC`) and `DEBUG_JUMP`. `<apple2/monitor_io.h>` and the `__dos_zp_*`
reservations in `zork.h` cover the bytes the Monitor and RWTS use. The
linker puts the runtime's block at `$00`-`$07`, `disk.c`'s two
zero-page globals and the globals it promotes into zero page at
`$08`-`$1A`, and the functions' parameters and locals in what's left
of `$80`-`$FF` and then at `$1B`-`$1E` and `$50`-`$65`.

**The program counters.** `Z_PC` and the string reader's `Z_PC2` are
`uint24_t`, `struct { uint16_t lo; uint8_t hi; }`. `Z_PC` keeps Zork's
byte order. `Z_PC2` is low byte first at `$91`, where Zork keeps it
high byte first ([../../CLAUDE.md](../../CLAUDE.md) maps its bytes).

**Dispatch.** The original jumps through four tables of handler
addresses. `decode.c` has a switch for each, and the handlers of a few
lines are written into their cases. A table of function pointers would
make every handler's address taken, which keeps it off the zero-page
calling convention, and a call through a pointer goes through the
runtime's `icall` trampoline. A switch on a byte compiles to `CMP #k;
BEQ` for each case.

**No runtime.** The repository doesn't have the runtime's helpers or
the startup code that sets up the soft stack, so the program uses
neither. `mul`, `div`, `mod`, and printing a number use the original's
shift-and-subtract loops, written in C. No function uses the soft
stack, `print_zstring` included, which calls itself for an
abbreviation in a call marked `[[c6502::save_hardware_stack]]`.

**Starting over.** Zork jumps to `main` when a page can't be read and
for the `restart` instruction. `restart()` is declared at `$0800` like
a ROM routine, so that the compiler's call graph has no cycle through
`main`, and
`main` sets each byte of the interpreter's state that's read before
it's written. The original clears `$80`-`$FF`, but the compiler keeps
its own bytes in that range too.

## Compared with Zork's own code

**Behavior.** cpp/tests/test_zork_interpreter_sim.cpp plays two
sessions on cpp/src/sim/apple2.h, a stand-in Apple II with the Monitor's text routines,
the keyboard and line input, a printer, and RWTS over the disk image.
The first walks into the cellar, saves, fights the troll until it
kills the player, which moves the player with random numbers,
restores, types words that aren't in the dictionary and sentences the
parser rejects, and quits.
The second turns the transcript on, saves at position 3 by answering
the prompts, fails to restore from the empty position 5, restores
from 3, fills the screen so that Zork stops for `[MORE]`, and
restarts. In both, this interpreter and Zork's make the same calls to
the Monitor and RWTS (8,813 in the first session and 5,572 in the
second), write the same sectors, and leave the same screen and
printer output. Traced outside the test, the first session runs the
same 39,112 instructions of the story on both, with the same locals
and stack depth before each. Its save files differ in two bytes, both
in the sectors that hold the Z-machine's stack. Zork pushes a
routine's count of locals as a word whose high byte is whatever was
in `SCRATCH2+1`, and this pushes 0 there. Nothing reads that byte.

**Size.** In bytes, measured from the assembled programs, for each
file and the original code it translates.

The original's bytes come from dasm's listing of its source. A line of
`HEX`, `DC`, `DS`, or `BYTE` counts as data, and every other line that
assembles to bytes counts as code, with a macro's expansion counted
with the line that invokes it. The C program's data is each file's
constants, strings, and variables. The difference is that of the C
file's code and data together against the original's.

| File | Original code | Original data | C code | C data | Difference |
|---|---:|---:|---:|---:|---:|
| `main.c` | 351 | 40 | 401 | 40 | +12.8% |
| `decode.c` | 1733 | 126 | 1711 | 0 | -8.0% |
| `objects.c` | 788 | 0 | 803 | 0 | +1.9% |
| `numbers.c` | 436 | 0 | 429 | 0 | -1.6% |
| `input.c` | 1004 | 14 | 1011 | 14 | +0.7% |
| `pages.c` | 384 | 0 | 374 | 0 | -2.6% |
| `zstring.c` | 359 | 24 | 358 | 24 | -0.3% |
| `output.c` | 380 | 7 | 392 | 7 | +3.1% |
| `disk.c` | 117 | 21 | 115 | 21 | -1.4% |
| `prompts.c` | 343 | 191 | 337 | 190 | -1.3% |
| `save.c` | 325 | 0 | 332 | 0 | +2.2% |
| **Total** | **6220** | **423** | **6263** | **296** | **-1.3%** |

The C counts leave out the page tables and `__zp_init`, 45 bytes of
startup code the linker generates to clear the globals in zero page.
The boot loader doesn't
run it, and the interpreter writes each of those globals before it
reads it. A function that ends by jumping to another is placed
right before that one and falls into it, as in Zork. The linker does
that for 26 functions, which saves a 3-byte `JMP` in each. Each
function is counted with the file that defines it, wherever the
linker places it. Zork's boot loader reads 26 sectors,
`$0800`-`$21FF`, so on an Apple II this program would take a loader
that also reads
`$2C00`-`$37FF`, from sectors that Zork's diskette doesn't have free.
The story starts 12 pages higher than Zork's, which leaves the page
cache 64 slots in 48K where Zork has 76.

The four dispatch switches take 900 bytes with the handlers written
into their cases, not counting the tails they share with other
functions (`do_2op` alone is 399), where Zork's four tables
take 126 bytes and the handlers are counted with their own files.
Each switch now dispatches through a table too, and a case that only
calls a handler is the handler's address in the table, as in Zork's.
`divu16` is 61 bytes against Zork's 78, and `match_dictionary_word`
181 against 190. `mulu16`, which computes only the low 16 bits of
the product, is inlined into `instr_mul`, its one caller.

**Speed.** The first session takes 35,308,334 cycles against Zork's
38,490,260, 8.3% fewer, and the second 6,879,995 against 7,434,428,
7.5% fewer. The shared tails and the outlined runs add a `JMP`, or a
`JSR` and an `RTS`, where they share code. `print_number` divides
0 by 10 once, where Zork's prints it without dividing. The stand-ins
for the Monitor and RWTS take no cycles. Where a function is placed matters to these counts, since a taken
branch that crosses a page takes a cycle more. When `objects.c`
shrank by 24 bytes, `branch` and `print_zstring`, which the linker
places after functions of `objects.c` that fall into them, moved,
and that cost the first session 16,534 cycles in the two, more than
the smaller code saved.
