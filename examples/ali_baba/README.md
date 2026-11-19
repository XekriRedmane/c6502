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
(see "How it's compared"). It is about 4.5% larger than the code it
translates (see "Size"), and it fits by using `$A900-$B2FF`, which the
original never touches.

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
`--defsym`. A unit that multiplies, divides, or shifts by a variable
count calls one of the runtime's helpers, and the linker adds each
helper the program calls from the runtime library, as `runtime.o`,
which the script's `*(.text ...)` puts in `$A900-$B2FF`. The units
write those operations as the original does, so today the program
calls none (UNITS.md, "Compiler gaps", item 6).

The original's code shares its spans of memory with tables, which the
C program defines where it uses them, so the linker script's regions
are those spans: `$0800-$1FFF`, `$5B24-$5BFF`, `$5D00-$777F`,
`$7780-$7D82`, `$92A5-$9AA4`, `$A441-$A56E`, and `$A56F-$A805`, with
the dead code inside them reused. The C program is larger than the
original, so the script also uses `$A900-$B2FF`, which holds zeros in
the image and which no instruction of the original addresses. `main`
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

`tests/test_ali_baba.py` checks each unit's output against its `.asm`
file. With `ALI_BABA_DIR` set to the reverse-engineering project's
directory, which holds the game's disk image and, after `build.py`,
`output/main.bin` (the original's memory image, `$0500-$BFFF`),
`output/main.asm`, and `output/main.sym`, it also plays six scripts
on the C program and on the original, on the stand-in Apple II in
`sim/ali_baba.py`:

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
II is emulated a nibble at a time (`sim/disk2.py`), so the game's own
disk code runs as it would on the machine, copy-protection check
included, and the Monitor routines the game calls are stand-ins with
the ROM's results. The simulator records every soft-switch access,
Monitor call, and call into the disk code, with a checksum of hi-res
page 1, and the two streams must be the same up to the event at which
the original's run ends, as must the disks afterwards. `COUT` isn't
recorded, since the C program calls the HRCG directly where the
original goes through `COUT`.

The six scripts run on the program linked with the simulator's runtime
helpers (`sim.linked.defsym_args`), and the game also runs on the
program linked by the command above, which holds any helper it calls
from the runtime library.

The six scripts run 337 of the 387 code labels in the spans the C
program replaces (`uv run python -m tests.test_ali_baba coverage`
lists the rest). The code that no script reaches includes the menu
that picks one of several mobs in a cell, attacks that open or topple an object,
the weapons and armor other than the oak club and the leather armor,
the trapped and special chests, the story scenes that move groups of
mobs, the joystick, the integrity checks' range counters, and the
HRCG's entry points and control characters that the game never
uses.

## Size

In bytes, for each file and the original's code and tables that it
translates (the chunks of `main.asm` that `UNITS.md` gives it):

| File | Original code | Original tables | C code | C data |
|---|---:|---:|---:|---:|
| `main.c` | 381 | 2 | 368 | 0 |
| `init.c` | 972 | 355 | 836 | 362 |
| `turn.c` | 616 | 6 | 1184 | 0 |
| `input.c` | 1639 | 1313 | 1707 | 1256 |
| `combat.c` | 2856 | 23 | 2973 | 0 |
| `ai.c` | 1223 | 18 | 1427 | 20 |
| `events.c` | 1542 | 8 | 1561 | 0 |
| `entities.c` | 1033 | 8 | 1049 | 0 |
| `map.c` | 993 | 43 | 1001 | 8 |
| `display.c` | 399 | 0 | 437 | 0 |
| `util.c` | 866 | 119 | 722 | 13 |
| `text.c` | 553 | 8 | 668 | 10 |
| `sound.c` | 337 | 64 | 436 | 0 |
| `hrcg.c` | 1141 | 907 | 835 | 781 |
| **Total** | **14551** | **2874** | **15204** | **2450** |

The C counts are of each unit's object, and they leave out the
`__zp_init` the linker writes. `hrcg.c` leaves out the HRCG's entry
points and control characters that the game never uses.

The largest difference is `turn.c`'s `run_chain`, 502 bytes. The
original's turn and menus are chains of `JMP`s that can lead back to
where they started, and c6502 rejects recursion at the link, so each
routine of a chain returns the next one's number and `run_chain` calls
them in a loop. The rest comes mostly from code the compiler could
write better, which the notes list unit by unit. The largest kinds
left are tail calls between a unit's functions, which the original
arranges to fall through or branch into each other, copies that the
compiler puts after a function's end with a `JMP` back, a value
chosen by an `if` that goes through a temporary at the join, and
switches on 16-bit values. The gap was 11% before 18 Nov 2026, when
c6502 began passing a function's leading arguments in A and X, which
took 742 bytes off the units, and narrowed comparisons of masked
bytes.
