# Ali Baba, in C

This is Electronic Arts' Ali Baba and the Forty Thieves (1982) for the
Apple II, rewritten in C as fourteen source files and a shared header,
and linked with a linker script into a program that runs where the
original's code ran. The original is the literate program `main.nw` of
https://github.com/XekriRedmane/ali_baba_reveng, whose `build.py`
tangles it into `output/main.asm`. The files below translate the game's
code and its HRCG text engine. The copy-protection interpreter in the
resident page `$0500-$07FF`, the disk code at `$B300-$BFFF` (a sector
reader and DOS 3.3's RWTS), and the boot loader stay the original's,
and the C program calls the disk code at its addresses.

It plays Ali Baba. On six scripted runs, the attract mode, a game, a
save to a new scenario disk, a load from it, a visit to the forest's
shops, and a game with a second character, it makes the same
soft-switch accesses, Monitor calls, and disk calls as the original,
with the same hi-res page 1 at each one, and leaves the same disks
(see "How it's compared"). Its code is about 2.3% larger than the code
it translates, counting the runtime helper the linker adds (see
"Size"), and it uses `$A900-$B2FF` too, which the original never
touches.

## Files

| File | What's in it | Chapters of main.nw |
|---|---|---|
| `ali_baba.h` | The game's zero page and memory, the records and tables the original's image keeps in place, the soft switches, the Monitor and disk routines, and each unit's routines | |
| `main.c` | The main loop, the mob groups' turns, and the attract screen | The Main Loop, The Attract Screen |
| `init.c` | Starting the game, the title screen and its border animation | Initialization |
| `turn.c` | A character's turn, and `run_chain`, which runs the original's chains of jumps | The Game Turn |
| `input.c` | The menus, the keyboard and joystick, and saving and loading | Input System |
| `combat.c` | Attacks, damage, encounters, and healing | Combat |
| `ai.c` | The mobs' moves, the shops, and experience | NPC AI |
| `events.c` | A location's events, treasure, gold, and random events | Events and Scripting |
| `entities.c` | Characters and mobs on the map, and their groups | Entities |
| `map.c` | Scenes, the disk calls, entering a location, and drawing the map | Map and Scene, Game Data |
| `display.c` | The status bar and a character's stats | Game Display |
| `util.c` | The random numbers, waits, record lookups, the integrity checks, and the cold restart | Utility Routines |
| `text.c` | Messages, numbers, and the text renderer | Text and Display Primitives |
| `sound.c` | The tune player and its tones | Sound |
| `hrcg.c` | The HRCG, which draws text on the hi-res screen | The HRCG Text Engine |
| `ali_baba.ld` | The linker script | |
| `*.asm` | Each file compiled on its own | |
| `UNITS.md` | Which routines and tables each unit has, and the decisions behind the header | |
| `notes/*.md` | Each unit's departures from the original, the places its comments and code disagree, and its size by routine | |

Each `.c` file's header lists where it departs from the original and
then quotes the lines of `main.asm` it translates.

## Building

```sh
cd examples/ali_baba
for f in main init turn input combat ai events entities map display util text sound hrcg; do
    uv run python ../../compile.py $f.c -S -O -o $f.asm
    uv run python ../../compile.py $f.c -c -O
done
uv run python ../../compile.py -T ali_baba.ld \
    main.o init.o turn.o input.o combat.o ai.o events.o entities.o \
    map.o display.o util.o text.o sound.o hrcg.o -o ali_baba
```

Each unit is compiled twice from the same source: to its `.asm`,
which is checked in, and to its object, which the link reads. Neither
the objects nor the executable are checked in. The link needs no
`--defsym`. c6502 compiles most multiplications, divisions, and shifts
by a variable count inline, and calls a runtime helper for the rest,
and for the builtin `__c6502_bin_to_bcd`. The linker adds each helper
the program calls from the runtime library, as `runtime.o`, and the
script's last pattern in `.spare`, `*(.text ...)`, puts it in
`$A900-$B2FF` after map's, util's, and text's code. Ali Baba calls
one, `bin16_to_bcd`, 46 bytes, which converts a number to decimal
digits for `num_to_decimal` in text.c.

The original's code shares its spans of memory with tables, which the
C program defines where it uses them, so the linker script's regions
are those spans: `$0800-$1FFF`, `$5B24-$5BFF`, `$5D00-$777F`,
`$7780-$7D82`, `$92A5-$9AA4`, `$A441-$A56E`, and `$A56F-$A805`, with
the dead code inside them reused. The script also puts map's,
util's, and text's code and the runtime library's helper in
`$A900-$B2FF`, which holds zeros in the image and which no
instruction of the original addresses. `main`
is at `$0800`, where the loader enters the game, and `cold_restart` at
`$A56F`, where the reset vector points. The rest of memory is read
where the original's image puts it: the game's records at
`$4000-$5AA8`, the scene buffer at `$5C00`, the fonts, the tunes, the
shop table, and the disk code.

The original starts with the HRCG and the disk code staged in hi-res
page 1 and copies them to `$9600` and `$B300`. The C program is loaded
with them in place, so `game_init` only clears page 1, as the copy
leaves it. The program has `static` variables with initial values that
`-O` promotes into zero page, and their initial bytes are stored by the
`__zp_init` the linker writes, which the harness runs before `main`.

## How it's compared

golden/O.txt checks each unit's output against a fresh compile. With `ALI_BABA_DIR` set to the reverse-engineering project's
directory, which holds the game's disk image and, after `build.py`,
`output/main.bin` (the original's memory image, `$0500-$BFFF`),
`output/main.asm`, and `output/main.sym`, it also plays six scripts
on the C program and on the original, on the stand-in Apple II in
cpp/src/sim/ali_baba.h (cpp/tests/test_ali_baba_sim.cpp):

- The attract mode. Nothing is pressed, and the title tune and the
  border animation play.
- A game. Ali Baba walks onto the dusty road, opens the menu, and is
  killed by a hostile group, and the reincarnation scene loads from
  the disk.
- A save. The settings change and change back, and the game is saved
  on a blank disk, which the game formats.
- A load. The save is made again, and the game continues from it.
- The forest. Ali Baba picks up two chests of gold, buys an oak club
  and leather armor, drops some of his gold, turns the armor's
  experience into a level, and attacks a tiger twice.
- A party. The slot number at the start goes up and down, Ali Baba
  attacks the rune on the dusty road, Abou Hassan joins him, a save is
  begun and cancelled, Ali Baba retires, and Abou Hassan steps on the
  rune.

A script is the keys, each pressed before a numbered `KBD` read, and
the disk swaps, found by running the original. Both programs start
from the image as the loader leaves it, and the C program is loaded
over it with the linker script's regions cleared. The Apple II's Disk
II is emulated a nibble at a time (cpp/src/sim/disk2.h), so the game's own
disk code runs as it would on the machine, copy-protection check
included, and the Monitor routines the game calls are stand-ins with
the ROM's results. The simulator records every soft-switch access,
Monitor call, and call into the disk code, with a checksum of hi-res
page 1, and the two streams must be the same up to the event at which
the original's run ends, as must the disks afterwards. `COUT` isn't
recorded, since the C program calls the HRCG directly where the
original goes through `COUT`.

The six scripts run on the program linked with the simulator's runtime
helpers (`sim::defsym_args`), and the game also runs on the
program linked by the command above, which holds its own copy of
`bin16_to_bcd` from the runtime library.

The six scripts run 337 of the 387 code labels in the spans the C
program replaces (`c6502cpp game-run ali_baba coverage`
lists the rest). The code that no script reaches includes the menu
that picks one of several mobs in a cell, attacks that open or topple an object,
the weapons and armor other than the oak club and the leather armor,
the trapped and special chests, the story scenes that move groups of
mobs, the joystick, the integrity checks' range counters, and the
HRCG's entry points and control characters that the game never
uses.

## Size

In bytes, for each file and the original's code and tables that it
translates (the chunks of `main.asm` that `UNITS.md` gives it, with
the tables as the original's data). The difference is that of the C
file's code and data together against the original's.

| File | Original code | Original data | C code | C data | Difference |
|---|---:|---:|---:|---:|---:|
| `main.c` | 381 | 2 | 347 | 0 | -9.4% |
| `init.c` | 972 | 355 | 806 | 362 | -12.0% |
| `turn.c` | 616 | 6 | 1111 | 0 | +78.6% |
| `input.c` | 1639 | 1313 | 1675 | 1256 | -0.7% |
| `combat.c` | 2856 | 23 | 2924 | 0 | +1.6% |
| `ai.c` | 1223 | 18 | 1346 | 27 | +10.6% |
| `events.c` | 1542 | 8 | 1584 | 0 | +2.2% |
| `entities.c` | 1033 | 8 | 1016 | 0 | -2.4% |
| `map.c` | 993 | 43 | 963 | 8 | -6.3% |
| `display.c` | 399 | 0 | 437 | 0 | +9.5% |
| `util.c` | 866 | 119 | 773 | 13 | -20.2% |
| `text.c` | 553 | 8 | 605 | 0 | +7.8% |
| `sound.c` | 337 | 64 | 436 | 0 | +8.7% |
| `hrcg.c` | 1141 | 907 | 813 | 790 | -21.7% |
| **Total** | **14551** | **2874** | **14836** | **2456** | **-0.8%** |

The C counts are the symbol sizes of each unit's `-c -O` object
(`nm.py -S`), and they leave out the `__zp_init` the linker writes.
The link also adds the runtime library's `bin16_to_bcd`, 46 bytes of
code, which brings the C program's code to 14,882 bytes, 2.3% more
than the original's. `hrcg.c` leaves out the HRCG's entry points and control
characters that the game never uses.

The largest difference is `turn.c`'s `run_chain`, 461 bytes. The
original's turn and menus are chains of `JMP`s that can lead back to
where they started, and c6502 rejects recursion at the link, so each
routine of a chain returns the next one's number and `run_chain` calls
them in a loop. The rest comes mostly from code the compiler could
write better, which the notes list unit by unit. The largest kinds
left are the stores of each argument into its callee's zero-page
parameters, where the original passes it in a register or leaves it
where the callee reads it (combat.c, events.c, entities.c, main.c, and
ai.c), a chain routine's `JSR x; LDA #state; RTS` where the original
jumps (turn.c and events.c), pointers to a record's members built as
16-bit addresses in temporaries (ai.c and combat.c), and map.c's
16-bit copies of the saved pointers, which the original makes in
SAVE_ZP_CONTEXT, outside the code counted. The gap was 11% before 18
Nov 2026, when c6502 began passing a function's leading arguments in A
and X, which took 742 bytes off the units, and narrowed comparisons of
masked bytes. The rewrite of the units as plain C later that day added
469 bytes, which each unit's notes give by routine. c6502's inline
byte division, shifts by a variable count, and byte products then took
96 bytes off, and rewriting the four routines that still divided by
subtraction with `/` and `%` added 17.
