
# examples/CLAUDE.md

Real-world C sources used to exercise the compiler end-to-end. The
expected `-S -O` output of each `<name>.c` is
golden/O/examples/<name>.s, which `c6502cpp check-golden` compares
with the compiler's output. This command writes the same text.

```sh
cpp/build/bin/c6502cpp compile examples/<name>.c -S -O -o <name>.s
```

Each `<name>.c` had a matching `<name>.asm` here until group R of
step 9 of the port deleted them on 27 Nov 2026. Each was byte for
byte its golden copy, and nothing had compared them with the
compiler's output since 26 Nov (docs/cpp_port.md, "Open questions for
Robert", question 5). The golden copies are short enough (typically
under 500 lines) that a reader can read one end-to-end and see what
`-O` produces on representative game code. Loop unrolling for
`[[c6502::unroll]]` loops is part of `-O`, and there is no separate
`--unroll` flag.

The example corpus is biased toward Apple II / game-engine workloads:
HUD strip painting, sprite blitting, audio tone generation, enemy AI
advancement, hit-entity refresh. Many use `[[c6502::zp_abi]]`
on the leaf helpers to exercise the call-graph-disjoint ZP allocator.

`zork/` is all of `output/main.asm` of
https://github.com/XekriRedmane/zorki-r15-reveng, Zork I's (r15) whole
interpreter, in eleven C files and a shared header, linked by a linker
script into a program that runs from `$0800` around DOS's RWTS and
loads the story after itself. Its README compares it with Zork's code,
and cpp/tests/test_zork_interpreter.cpp checks that the listing of the
linked executable, `c6502cpp objdump -D`, is interpreter.asm, which
is the one `.asm` file left in examples/. When `ZORK_DSK`
names the game's disk image, `Zork I r15 (4am crack).dsk` from that
repository, cpp/tests/test_zork_interpreter_sim.cpp plays two
sessions on it and on Zork's own interpreter (read from the same
image) and compares every Monitor and RWTS call.
Each `.c` file's header quotes the lines of `main.asm` it translates.
The interpreter grew out of smaller examples of single routines
(`zork_<routine>.c`) and of Zork's `save` instruction and z-string
printer (`zork/instr_save/`, `zork/print_zstring/`), which were removed
on 16 Nov 2026. Some documents in `docs/optimizations/` show listings
captured from them.

`lode_runner/` is `main.asm` of
https://github.com/XekriRedmane/lode_runner_reveng, Broderbund's Lode
Runner for the Apple II, in twelve C files and a shared header,
`lode_runner.h`, which names the original's zero-page bytes and the
tables its memory image keeps in place. The linker script,
`lode_runner.ld`, puts the program where the original's code was,
from `$6000`, with `format_patch` (editor.c's section `format_patch`)
at `$8E00`, where the game's DOS jumps when the editor formats a
disk. What doesn't fit there yet goes in `$9000`-`$9AFF`, which the
original never touches. Each `.c` file's header quotes the lines of
`main.asm` it translates and lists its departures from the original.
golden/O.txt checks each unit's output, and
cpp/tests/test_lode_runner_sim.cpp, when `LODE_RUNNER_DIR` names the
reverse-engineering project's directory (which holds `main.bin`,
`lode_runner.do`, and `main.asm`), plays three scripts on the linked
program and on the original, on the stand-in Apple II in
cpp/src/sim/lode_runner.h, and compares every soft-switch access and
RWTS call. There's no checked-in
listing of the linked program. Its README describes how the program
is built and compared, and each file's size against the original's.

`ali_baba/` is the literate `main.nw` of
https://github.com/XekriRedmane/ali_baba_reveng, Electronic Arts' Ali
Baba and the Forty Thieves for the Apple II, in fourteen C files and a
shared header, `ali_baba.h`. The game's code and its HRCG text engine
are C, and the copy-protection interpreter at `$0500-$07FF`, the disk
code at `$B300-$BFFF`, and the boot loader stay the original's, called
at their addresses. The tables among the original's code moved into
the units, so `ali_baba.ld` puts the program in the spans the original's
code and tables filled, plus `$A900-$B2FF`, which the original never
touches. `UNITS.md` records which routines and tables each unit has and
the decisions behind the header, and `notes/` has each unit's
departures and size by routine. golden/O.txt checks each unit's
output, and cpp/tests/test_ali_baba_sim.cpp, when `ALI_BABA_DIR` names
the reverse-engineering project's directory (after its `build.py` has
made `output/main.bin`, `main.asm`, and `main.sym`), plays six scripts
on the linked program and on the original, on the stand-in Apple II in
cpp/src/sim/ali_baba.h with the Disk II of cpp/src/sim/disk2.h, and
compares every soft-switch access, Monitor call, and call into the
disk code, and the disks they leave. Its README describes the comparison and each
file's size against the original's.

The program has `static` variables with initial values that `-O`
promotes into zero page, such as editor.c's `kbd_entry_index`, and
their initial bytes are stored by the `__zp_init` the linker writes.
The game starts from the reset vector at `cold_start`, or from `main`,
and neither calls `__zp_init`, so the program is correct only when
something runs `__zp_init` first. The harness calls it before `main`
(`lode_runner::program_machine` in cpp/src/sim/lode_runner.h).

## Translating Zork's code

Zork's program counters are 24-bit byte addresses, and the interpreter
declares them as `uint24_t`, a `struct { uint16_t lo; uint8_t hi; }`
in its `zork.h`, rather than as the separate bytes
`output/main.asm` names. Keep that in mind when translating more of
Zork, and declare these the same way in a new example, so that units
agree and code that reads them stays plain C:

- `Z_PC` (`$8A`) keeps Zork's byte order, low, high, top, so its
  addresses are Zork's. `Z_PC` / `Z_PC+1` are the low and high bytes
  of `Z_PC.lo`, and `Z_PC+2` is `Z_PC.hi`. A save file holds these
  three bytes (`instr_save`, `instr_restore`), unchanged.
- `Z_PC2` (`$91`) is in a different order from Zork's. Zork keeps it
  high byte first: `Z_PC2_H` at `$91`, `Z_PC2_HH` at `$92`, and
  `Z_PC2_L` at `$93`. The interpreter keeps it low byte first like
  `Z_PC`: low at `$91`, high at `$92`, top at `$93`. So:
  - `Z_PC2_L` is `(uint8_t)Z_PC2.lo`,
  - `Z_PC2_H` is `(uint8_t)(Z_PC2.lo >> 8)`,
  - `Z_PC2_HH` is `Z_PC2.hi`,
  - the story page, which Zork copies out as the word at `Z_PC2_H`
    (`MOVW Z_PC2_H, SCRATCH2`), is `(Z_PC2.lo >> 8) | ((uint16_t)
    Z_PC2.hi << 8)`, and
  - the three `MOVB`s between `Z_PC` and `Z_PC2` are one assignment,
    `Z_PC2 = Z_PC;`.

  Zork's other code that touches `Z_PC2` (`load_address` at line
  2428, which `print_addr`, `print_paddr`, and the object printer go
  through) does so by name, a byte at a time, so it works with this
  order once its three `EQU`s are changed, but Zork's binary as it is
  doesn't: its `load_address` would write the bytes where the C
  doesn't look for them. `Z_PC2` isn't in a save file.
- `TMP_Z_PC` (`$F0`) is copied from `Z_PC` in the same order (`MOVW
  Z_PC, TMP_Z_PC; MOVB Z_PC+2, TMP_Z_PC+2`), so it would be a
  `uint24_t` with Zork's addresses. The interpreter only reserves it.

Zork's `INY; STY Z_PC2_L; BEQ` to step the low byte and carry into
the rest is `Z_PC2.lo++; if ((uint8_t)Z_PC2.lo == 0) { ...; if
(Z_PC2.lo == 0) Z_PC2.hi++; }`, which compiles to `INC Z_PC2; BNE`
(see docs/optimizations/in_place_member_update.md and
inc_low_byte_test.md).

## The game ports' READMEs

A port of a game (Zork, Lode Runner, Ali Baba) has a README that
compares the C with the original. Robert set two conventions for it:

- It doesn't list the compiler changes the port needed. The commits
  and docs/ record those.
- Its size table has the original's code, the original's data, the C
  program's code, the C program's data, and the difference of the
  totals (code plus data) from the original's, in percent. To split
  the original into code and data, assemble it with dasm and read the
  listing. HEX, DC, DS, and BYTE lines count as data, and a macro's
  bytes count with the line that invokes it.

## Adding a new example

The `compile-example` skill (project-local) wraps the canonical
compile command. Type `/compile-example <name>` to compile
`examples/<name>.c` with `-O` and compare the text with
golden/O/examples/<name>.s. The golden files hold a job for each
example in each mode (cpp/src/golden.h), and an example that has none
isn't checked by `check-golden`.
