# Lode Runner, in C

This is Broderbund's Lode Runner (1983) for the Apple II, rewritten in
C as twelve source files and a shared header, and linked with a linker
script into a program that runs where the original's code ran, from
`$6000`. The original is `main.asm` of
https://github.com/XekriRedmane/lode_runner_reveng, and the files
below translate its code from line 303 to line 8199, apart from the
loader that relocates the game when it boots.

It plays Lode Runner. On three scripted runs, the attract mode's demo,
a game that ends in the high-score table, and a session in the level
editor, it makes the same soft-switch accesses and RWTS calls as the
original, with the same two high-resolution pages at each one, and
leaves the same disk (see "How it's compared"). It is about 540 bytes
larger than the code it translates (see "Size"), so it doesn't fit in
the original's `$6000`-`$8E52`, and the linker script puts what
doesn't fit in `$9000`-`$9AFF`, which the original never touches.

## Files

| File | What's in it | Original lines |
|---|---|---|
| `lode_runner.h` | The game's zero page and memory, the tables its memory image keeps in place, the soft switches, DOS's IOB and RWTS, and each unit's routines | |
| `main.c` | Starting the game, the splash screen, the wait between screens, and the loop that plays a game | 303-731 |
| `level.c` | Reading, unpacking, drawing, and saving a level, the high-score sector, and the hidden ladders | 735-1210, 6767-6840 |
| `player.c` | Moving the player one step each turn: falling, running, climbing, swinging on ropes, digging, and picking up gold | 1211-2022, 2423-2610 |
| `input.c` | The keyboard, the joystick, and the demo's moves | 2023-2422, 4600-4729, 6721-6766, 6841-6858, 6931-6984, 7120-7221 |
| `guards.c` | Moving the guards, the AI that picks each guard's move, and the gold guards pick up, carry, and drop | 2611-4317 |
| `timers.c` | The holes filling back in and the dead guards coming back | 4318-4599 |
| `screen.c` | The status line, text, and numbers on the high-resolution screen | 4865-5214, 6902-6930 |
| `hiscore.c` | Showing the high-score table, and putting a new score in it | 4730-4864, 6543-6720 |
| `editor.c` | The level editor, and the patch to DOS that formats a data disk | 5215-6131, 8147-8199 |
| `sprites.c` | Drawing and erasing sprites on the high-resolution screen | 6132-6542, 7346-7439 |
| `sound.c` | The speaker: the queue of notes, the falling tone, the beep, and the delays that pace the game | 6859-6901, 7222-7345 |
| `wipe.c` | The iris wipe between levels, and the turning GAME OVER | 7440-8130 |
| `lode_runner.ld` | The linker script | |
| `*.asm` | Each file compiled on its own | |

Each `.c` file's header lists where it departs from the original and
then quotes the original code it translates.

## Building

```sh
cd examples/lode_runner
for f in main level player input guards timers screen hiscore editor sprites sound wipe; do
    uv run python ../../compile.py $f.c -S -O -o $f.asm
    uv run python ../../compile.py $f.c -c -O
done
uv run python ../../compile.py -T lode_runner.ld \
    main.o level.o player.o input.o guards.o timers.o screen.o \
    hiscore.o editor.o sprites.o sound.o wipe.o -o lode_runner
```

Each unit is compiled twice from the same source: to its `.asm`, which
is checked in, and to its object, which the link reads. The link
writes the ELF executable `lode_runner`. Neither the objects nor the
executable are checked in, and there's no checked-in listing of the
linked program.

`lode_runner.ld` puts `main.o` and nine other units in `$6000`-`$8DFF`,
`editor.c`'s `format_patch` at `$8E00`, where the game's DOS jumps
when the editor formats a disk, and `hiscore.o`'s data after it. What
the script doesn't name, which is `editor.o` and the code of
`hiscore.o`, goes in `$9000`-`$9AFF`. The program reads the rest of
memory where the original's image puts it: the level maps, the sprite
and pixel-shift tables, the splash screen, the demo's levels and
moves, the high-score sector, and DOS.

The program has `static` variables with initial values that `-O`
promotes into zero page, and their initial bytes are stored by the
`__zp_init` the linker writes. Neither the reset vector's `cold_start`
nor `main` calls it, so something has to run `__zp_init` first. The
test harness does (`sim.lode_runner.program_machine`).

## How it's compared

`tests/test_lode_runner.py` checks each unit's output against its
`.asm` file. With `LODE_RUNNER_DIR` set to the reverse-engineering
project's directory, which holds `main.bin` (the original's 48K memory
image), `lode_runner.do` (the game's disk), and `main.asm`, it also
plays three scripts on the C program and on the original, on the
stand-in Apple II in `sim/lode_runner.py`.

- The demo. Nothing is pressed, the attract mode plays its three
  levels, and the high scores show.
- A game. The player picks up a piece of gold, is caught, digs a hole
  that a guard falls into, loses the last lives on purpose, and types
  initials into the high-score table, which is written to the disk.
- The editor. It edits level 1 and saves it, quits a second edit
  without saving, clears level 2, and plays level 1 from the editor.

A script is the keys and the number of the `KBD` read before which
each is pressed, found by running the original. The C program is
loaded over the same image with the original's code cleared and starts
at `main`. The simulator records every soft-switch access and RWTS
call, with a checksum of each high-resolution page, and the two
streams must be the same up to the event at which the original's run
ends, as must the two disks afterwards. The demo takes about four
minutes on the C program, so each script is a test method of its own.

## Size

In bytes, measured from the linked program, for each file and the
lines of `main.asm` it translates:

| File | Original | C code | C data | C total |
|---|---:|---:|---:|---:|
| `main.c` | 699 | 615 | 71 | 686 |
| `level.c` | 755 | 855 | 11 | 866 |
| `player.c` | 1528 | 1299 | 93 | 1392 |
| `input.c` | 918 | 959 | 11 | 970 |
| `guards.c` | 2417 | 2303 | 65 | 2368 |
| `timers.c` | 440 | 505 | 9 | 514 |
| `screen.c` | 503 | 514 | 36 | 550 |
| `hiscore.c` | 621 | 610 | 99 | 709 |
| `editor.c` | 1898 | 1318 | 740 | 2058 |
| `sprites.c` | 629 | 602 | 24 | 626 |
| `sound.c` | 192 | 210 | 16 | 226 |
| `wipe.c` | 1201 | 920 | 371 | 1291 |
| **Total** | **11801** | **10710** | **1546** | **12256** |

The original's count is every byte its lines assemble to, code and
data alike, since the original keeps its tables and messages among its
routines. `main.c`'s lines include the 98-byte table MAIN relocates
the game with, which the C program has no use for. The C counts leave
out `__zp_init`, 47 bytes, and the 51 bytes of tables of row
addresses that the linker keeps one copy of for the units that share
them. A function that ends by jumping to another is placed right
before that one and falls into it, as in the original.

The C code's largest differences from the original come in a few
places. The guards' move routines take 60 bytes more, mostly in the
two-dimensional lookups of `LEVEL_MAP` and `LEVEL_BG`, where the
original keeps a row's two pointers in `PTR1` and `PTR2` and the C
sets up a lookup's pointer again after a call or a change to the
row. `editor.c` keeps its messages as
data, where the original puts them after the `JSR` that prints them,
so its code is smaller and its data larger.
`add_and_update_score` adds a decimal digit at a time, where the
original sets the 6502's decimal mode, and takes 65 bytes more.
