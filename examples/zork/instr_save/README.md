# Zork's `save` instruction, in C

This is the Z-machine's `save` instruction from Zork I (r15) for the
Apple II, `instr_save`, and the routines it reaches, rewritten in C as
three source files and linked with a linker script into the addresses
the original occupies. The original is in `output/main.asm` of
https://github.com/XekriRedmane/zorki-r15-reveng, lines 3359-3699.

It makes a good test of the compiler because it touches everything
around Zork as well as Zork itself:

- **DOS.** Sectors are written with DOS 3.3's `RWTS`, which takes the
  address of its parameter block in A and Y and reports failure in the
  carry flag.
- **The Monitor ROM.** The prompts read keys with `RDKEY`, clear the
  line with `CLREOL`, and set the cursor column `CH` and the inverse
  mask `INVFLG` directly (`<apple2/monitor_io.h>`).
- **The rest of Zork.** Text goes through Zork's line buffer
  (`buffer_char`, `dump_buffer_with_more`, ...), and the instruction
  ends by jumping to Zork's `branch` or `negated_branch`. Those
  routines aren't part of this example. They're declared at their
  addresses in Zork, the way a ROM routine is.
- **Zork's zero page.** Zork keeps its interpreter state in `$7C`-`$F2`,
  and DOS and the Monitor use most of `$1F`-`$4F`. The compiler has to
  fit its own bytes around all of that.

## Files

| File | What's in it | Original |
|---|---|---|
| `zork.h` | Zork's zero page and memory, the IOB and DCT structures, and the routines outside this example, at their addresses | |
| `disk.c` | The IOB and DCT, `do_rwts_on_sector`, `write_next_sector`, `inc_sector_and_write` | lines 3359-3448 |
| `prompts.c` | `print_ascii_string`, `please_insert_save_diskette`, `get_prompted_number_from_user`, `please_reinsert_game_diskette`, and their strings | lines 3452-3632 |
| `save.c` | `instr_save` and `copy_data_to_buff` | lines 3634-3699 |
| `instr_save.ld` | The linker script: everything in one region from `$1DAC`, where the original starts, to `$2200` | |
| `disk.asm`, `prompts.asm`, `save.asm` | Each file compiled on its own | |
| `instr_save.asm` | The three linked by the script | |

Each `.c` file's header quotes or locates the original code and lists
where it departs from it.

## Building

```sh
cd examples/zork/instr_save
uv run python ../../../compile.py disk.c --codegen --optimize -o disk.asm
uv run python ../../../compile.py prompts.c --codegen --optimize -o prompts.asm
uv run python ../../../compile.py save.c --codegen --optimize -o save.asm
uv run python ../../../compile.py --link -T instr_save.ld \
    disk.asm prompts.asm save.asm -o instr_save.asm
```

`instr_save.asm` assembles with dasm. It has no `main`, since it's a
piece of Zork. `tests/test_zork_instr_save.py` links it with a `main`
that calls `instr_save`, runs it, and checks the outputs above
against fresh compiles.

## How it's put together

**Zero page.** `zork.h` declares every byte Zork and DOS own as an
`extern` `zeropage(addr)` object: the ones the example uses by name
(`Z_PC`, `LOCAL_ZVARS`, `Z_HEADER_ADDR`, `SCRATCH1`, `SCRATCH2`, ...),
and the rest as reservations such as `__zork_zp_80[10]`. The compiler
keeps everything of its own off them, which leaves it `$00`-`$1E` and
`$50`-`$7B` for the runtime's block, and `$F3`-`$FF`, 13 bytes, for
the parameters and locals of the eight functions. When those run out,
the linker puts the rest in `__spill`, three bytes laid out with the
data.

**Calling convention.** Every function is `zp_abi`. Parameters arrive
in registers where the original passes them that way:
`print_ascii_string`'s count in X, `copy_data_to_buff`'s index in X
and count in Y, and `do_rwts_on_sector`'s command in A. Functions
that return "failed" in the carry are `return_bool_in_carry`, so
`if (write_next_sector()) goto fail;` is a `JSR` and a `BCS`, as in
the original. Zork passes pointers in `SCRATCH2`, and so does this
version, since the routines outside it expect them there.

**Overlapping memory.** Zork's line buffer is `$0200`-`$02FF`, and its
stack is `$0280`-`$03E7`, so the second half of the buffer is the
bottom of the stack. The compiler takes two different objects to be
different bytes, so `Z_STACK` is an address, `(uint8_t *)0x0280`,
rather than an object. An access through a pointer may reach any
object, so the compiler doesn't reorder it past `BUFF_AREA`'s.

**Sections.** The code goes in `.text`, the strings in `.rodata`, the
IOB, DCT, and prompts in `.data`, and the spilled slots in `.bss`. The
script puts all four in one region, which is how Zork's listing mixes
code and data. dasm stops with a message if they don't fit before
`$2200`.

## Compared with Zork's own code

**Behavior.** `tests/test_zork_instr_save.py` runs two saves: one to
slot 6, drive 1 (the game's own drive) at position 3, with a wrong key
and a default answer on the way, which then asks for the game diskette
back; and one to slot 5, drive 2 whose fifth write fails. Zork's other
routines, `RDKEY`, `CLREOL`, and `RWTS` are stand-ins that record every
call: each character added to the line buffer, each prompt printed and
the cursor column and inverse mask it's printed with, each key read,
and each sector written, with its slot, drive, track, sector, and
256 bytes. With `ZORK_MAIN_BIN` set to Zork's `main.bin`, the test runs
Zork's own `instr_save` the same way. The two record the same 319
calls in the first save and the same 143 in the second, and leave the
same state.

**Size.** In bytes, measured from the assembled programs:

| Routine | Zork | C |
|---|---:|---:|
| `do_rwts_on_sector` | 51 | 74 |
| `write_next_sector` | 8 | 11 |
| `inc_sector_and_write` | 38 | 42 |
| `print_ascii_string` | 20 | 25 |
| `please_insert_save_diskette` | 167 | 168 |
| `get_prompted_number_from_user` | 106 | 151 |
| `please_reinsert_game_diskette` | 70 | 70 |
| `instr_save` | 141 | 154 |
| `copy_data_to_buff` | 12 | 12 |
| **Code** | **613** | **707** |

The data is the same apart from where it sits: 17 bytes of IOB, 4 of
DCT, the five strings, and 48 bytes of prompts, plus the C version's 3
spilled bytes. In Zork, `write_next_sector` falls into
`inc_sector_and_write`, and `inc_sector_and_write` into the code that
writes; the sizes above count each one to the next routine.

**Speed.** Counting only the cycles spent in this code (the stand-ins
take none), the first save takes 21,901 cycles against Zork's 18,631,
and the second 12,552 against 12,316.

**Where the difference comes from.** Looking at `instr_save.asm`:

- `copy_data_to_buff` is the same loop as the original, down to the
  byte count, with `TYA` where the original has `CPY #$00`.
- `print_ascii_string` is the original's loop too, but its count and
  index are in the spill area, absolute addresses, where Zork uses its
  zero-page SCRATCH3. That's a byte and a cycle per access, in the
  loop that prints every character.
- `get_prompted_number_from_user` indexes the 16-byte prompt records,
  and computes `which * 16` as a 16-bit shift, three times, where
  Zork keeps a byte offset in `prompt_offset`.
- `do_rwts_on_sector` copies `SECTORS_PER_TRACK` into a local before
  each subtraction instead of subtracting it directly, and compares
  and subtracts in two passes where Zork's loop does both at once. It
  ends in `JMP RWTS`, which Zork could have used too.
- `instr_save`'s `SCRATCH2 += 0x100` is a 16-bit add where Zork has
  `INC SCRATCH2+1`.
- `inc_sector_and_write` saves and restores the motor count through
  locals where Zork pushes it, which is the whole difference.

## Compiler changes this example led to

Building it turned up gaps that are now fixed:

- A quoted `#include "zork.h"` now looks in the including file's
  directory.
- A `const char` array can be initialized from a string literal.
- `--link` chooses the runtime's zero-page block for the whole
  program, so a unit that sizes HARGS differently, or includes
  `<apple2/monitor_io.h>` when another doesn't, still links.
- `--link` gives the parameter and local slots the whole zero-page
  pool, `$80`-`$FF`, since every linked function has a private pool.
  Zork leaves only `$F3`-`$FF` free, all in the half that was unused.
- With a script, a slot that doesn't fit in zero page goes in a
  `__spill` block the script places, instead of at `$0200`, which
  here is Zork's line buffer.
- A store to `DPTR` before a call is dead, unless the call is the
  `icall` trampoline. The print loop used to copy `SCRATCH2` to `DPTR`
  for every character.
- A body-local slot is dead when its function returns even when it
  was spilled out of zero page, so stores to it before the return go.
- A carry-flag function returning a `bool` variable returns it as is
  instead of testing it against 0 again, and a carry that's stored,
  kept through loads and stores, and loaded for `CMP #1` stays in C.
  `inc_sector_and_write` (`bool failed = ...; return failed;`) had a
  16-bit compare there, and `do_rwts_on_sector` a capture of RWTS's
  carry where it now jumps to RWTS.
