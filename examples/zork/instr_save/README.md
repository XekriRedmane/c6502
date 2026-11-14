# Zork's `save` instruction, in C

This is the Z-machine's `save` instruction from Zork I (r15) for the
Apple II, `instr_save`, and the routines it reaches, rewritten in C as
three source files and linked with a linker script into the addresses
the original occupies. The original is in `output/main.asm` of
https://github.com/XekriRedmane/zorki-r15-reveng, lines 3359-3699, and
`cout_string` at lines 3282-3292.

It makes a good test of the compiler because it touches everything
around Zork as well as Zork itself:

- **DOS.** Sectors are written with DOS 3.3's `RWTS`, which takes the
  address of its parameter block in A and Y and reports failure in the
  carry flag.
- **The Monitor ROM.** The prompts read keys with `RDKEY`, print with
  `COUT1`, clear the line with `CLREOL`, and set the cursor column
  `CH` and the inverse mask `INVFLG` directly
  (`<apple2/monitor_io.h>`).
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
| `prompts.c` | `print_ascii_string`, `cout_string`, `please_insert_save_diskette`, `get_prompted_number_from_user`, `please_reinsert_game_diskette`, and their strings | lines 3452-3632, and 3282-3292 for `cout_string` |
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
(`Z_PC`, `LOCAL_ZVARS`, `Z_HEADER_ADDR`, `SCRATCH1`, ...), and the
rest as reservations such as `__zork_zp_80[10]`. `SCRATCH2`'s bytes
are a reservation too, since Zork's other routines still use them,
but nothing here names them. The compiler keeps everything of its own
off these bytes, which leaves it `$00`-`$1E` and `$50`-`$7B` for the
runtime's block, and `$F3`-`$FF`, 13 bytes, for the parameters and
locals of the ten functions. They all fit.

**Calling convention.** Every function is `zp_abi`. Parameters arrive
in registers where the original passes them that way:
`print_ascii_string`'s and `cout_string`'s count in X,
`copy_data_to_buff`'s index in X and count in Y, and
`do_rwts_on_sector`'s command in A. A register parameter takes no
zero-page byte of its own. Functions that return "failed" in the carry
are `return_bool_in_carry`, so `if (write_next_sector()) goto fail;`
is a `JSR` and a `BCS`, as in the original.

**Pointers as arguments.** Zork passes the address of the bytes to
print, copy, or write in its global `SCRATCH2`. Here each routine
takes it as a parameter, `print_ascii_string(sDefault, 10)`, and the
compiler works out from the call graph where the pointer can live:

- `print_ascii_string`, `cout_string`, and `copy_data_to_buff` never
  run at the same time, so their pointer parameters share `$F3`-`$F4`.
  That pair is the compiler's own `SCRATCH2`. Every caller stores the
  address there, and the loops read `(__zpabi_cout_string__s_0),Y`
  as Zork's read `(SCRATCH2),Y`.
- `inc_sector_and_write` passes its `buffer` on to `do_rwts_on_sector`
  unchanged, so the two parameters get the same bytes, `$F5`-`$F6`,
  and it copies nothing.
- `instr_save`'s loop keeps its page pointer in `inc_sector_and_write`'s
  parameter, which that routine and everything it calls leave alone,
  and advances it there, as Zork advances `SCRATCH2`. Since
  `inc_sector_and_write` is in another file, its declaration in
  `zork.h` makes the parameter `const`
  (`bool inc_sector_and_write(uint8_t *const buffer)`), which is the
  promise that lets `save.c` do this. `--link` checks it against the
  whole program.

[docs/optimizations/slot_sharing.md](../../../docs/optimizations/slot_sharing.md)
describes the rules.

**Overlapping memory.** Zork's line buffer is `$0200`-`$02FF`, and its
stack is `$0280`-`$03E7`, so the second half of the buffer is the
bottom of the stack. The compiler takes two different objects to be
different bytes, so `Z_STACK` is an address, `(uint8_t *)0x0280`,
rather than an object. An access through a pointer may reach any
object, so the compiler doesn't reorder it past `BUFF_AREA`'s.

**Sections.** The code goes in `.text`, the strings in `.rodata`, and
the IOB, DCT, and prompts in `.data`. The script puts them in one
region, which is how Zork's listing mixes code and data. dasm stops
with a message if they don't fit before `$2200`. The script also
places `.bss`, where slots that don't fit in zero page would go; here
none are left over.

## Compared with Zork's own code

**Behavior.** `tests/test_zork_instr_save.py` runs two saves: one to
slot 6, drive 1 (the game's own drive) at position 3, with a wrong key
and a default answer on the way, which then asks for the game diskette
back; and one to slot 5, drive 2 whose fifth write fails. Zork's other
routines, `RDKEY`, `COUT1`, `CLREOL`, and `RWTS` are stand-ins that
record every call: each character added to the line buffer, each
character printed to the screen and the cursor column and inverse mask
it's printed with, each key read, and each sector written, with its
slot, drive, track, sector, and 256 bytes. With `ZORK_MAIN_BIN` set to
Zork's `main.bin`, the test runs Zork's own `instr_save` (and its own
`cout_string`) the same way. The two record the same 355 calls in the
first save and the same 170 in the second, and leave the same state.

**Size.** In bytes, measured from the assembled programs:

| Routine | Zork | C |
|---|---:|---:|
| `do_rwts_on_sector` | 51 | 50 |
| `write_next_sector` | 8 | 11 |
| `inc_sector_and_write` | 38 | 42 |
| `print_ascii_string` | 20 | 20 |
| `cout_string` | 14 | 14 |
| `please_insert_save_diskette` | 167 | 162 |
| `get_prompted_number_from_user` | 106 | 103 |
| `please_reinsert_game_diskette` | 70 | 70 |
| `instr_save` | 141 | 142 |
| `copy_data_to_buff` | 12 | 12 |
| **Code** | **627** | **626** |

The data is the same apart from where it sits: 17 bytes of IOB, 4 of
DCT, the five strings, and 48 bytes of prompts. In Zork,
`write_next_sector` falls into `inc_sector_and_write`, and
`inc_sector_and_write` into the code that writes; the sizes above
count each one to the next routine.

**Speed.** Counting only the cycles spent in this code (the stand-ins
take none), the first save takes 19,386 cycles against Zork's 19,561,
and the second 12,982 against 13,012.

**Where the difference comes from.** Looking at `instr_save.asm`:

- `copy_data_to_buff` is the same loop as the original, down to the
  byte count, with `TYA` where the original has `CPY #$00`, and so is
  `cout_string`, instruction for instruction.
- `print_ascii_string` is the original's loop too, the same size, with
  its count and index in zero-page locals where Zork uses SCRATCH3.
- `get_prompted_number_from_user` shifts `which` left four times in A
  to get the byte offset of its prompt record, where Zork's callers
  pass the offset in `prompt_offset`. The pointer arguments are each
  one add straight into the callee's parameter bytes,
  `#<(prompts+15)` plus the offset for the answer, and the default
  answer is read with `LDA prompts+15,X`. It's 3 bytes shorter than
  Zork's.
- `do_rwts_on_sector`'s loop is Zork's: the sector number in A and
  X, `SBC SECTORS_PER_TRACK; BCS` when nothing is borrowed from the
  high byte, and `ADC SECTORS_PER_TRACK` after the loop to get the
  last value back. Where Zork does `DEX; BMI` on a borrow and then
  sets the carry again, it does `CPX #$01; DEX`, which leaves the
  carry as the loop's test needs it, so it's a little faster
  (1,666 cycles in the first save against 1,729). It ends in `JMP
  RWTS`, which Zork could have used too.
- `instr_save`'s `page += 0x100` is `INC` of the pointer's high byte,
  as Zork's `INC SCRATCH2+1` is, in the parameter's bytes where Zork
  has it.
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

Passing pointers as arguments instead of in `SCRATCH2` led to more:

- A value can stay in a callee's parameter bytes across calls: a
  parameter passed on unchanged shares its callee's bytes, and a local
  passed and still needed afterwards lives in them, including across
  files through a `const` declaration
  (`passes/slot_sharing.py`).
- A register parameter takes no zero-page byte. It used to have one
  that was written on entry and then usually never read again.
- A unit whose functions all have private pools takes its slots from
  `$80`-`$FF`, as `--link` does. Compiled alone, each unit here used to
  get only `$80`-`$BF`, all of it Zork's, so its pointer parameters
  were outside zero page, and its loops went through `DPTR` even after
  linking put the pointers in zero page.
- `--link` leaves out the local bytes no code names any more, which
  had kept `print_ascii_string`'s count and index out of zero page.
- Loop-invariant store hoisting leaves a copy of a zero-page pointer
  into `DPTR` in its loop, where it becomes a `(zp),Y` access.
- A function can take a parameter at a fixed zero-page address,
  `[[c6502::zeropage(addr)]]`, as Zork's own `cout_string` does
  with `SCRATCH2`. This example doesn't need it, since its routines
  only call each other, but a C `cout_string` declared that way could
  be called from Zork's other code.

Comparing its code with Zork's then led to:

- A compare and a subtract of the same operands are one subtraction
  and a branch on its flags
  ([compare_subtract.md](../../../docs/optimizations/compare_subtract.md)).
  `do_rwts_on_sector`'s loop went from 50 cycles a pass to 42.
- An `ADC`, `SBC`, `AND`, `ORA`, or `EOR` reads its operand where it
  is instead of from a copy
  ([cmp_through_tmp_fold.md](../../../docs/optimizations/cmp_through_tmp_fold.md)),
  which made it 36: `SBC SECTORS_PER_TRACK`.
- The loop's exit gets the old value as `t + b` instead of keeping it
  alive across the loop
  ([compare_subtract.md](../../../docs/optimizations/compare_subtract.md)),
  so the loop updates the sector number in place.
- The upper byte of an add or subtract of a byte that a loop carries
  round goes in X or Y ([regalloc.md](../../../docs/optimizations/regalloc.md)),
  where [carry_inc.md](../../../docs/optimizations/carry_inc.md) makes
  its borrow `BCS; CPX #$01; DEX`, keeping the carry for the loop's
  `BCC`. The low byte then stays in A by itself.
- A branch to a branch on the same flag goes where that one goes
  ([branch_thread.md](../../../docs/optimizations/branch_thread.md)),
  and a `SEC` or `CLC` the carry already matches goes, or moves in
  front of the loop when only the way in needs it
  ([carry_known.md](../../../docs/optimizations/carry_known.md)).
  That made the loop 15 cycles a pass, as Zork's is.
- An index into an array object of at most 255 bytes is scaled at byte
  width, `o[i].m` on a static array is read and written as `LDA
  o+m,X` ([static_indexed_access.md](../../../docs/optimizations/static_indexed_access.md)),
  a label's address plus a constant is one immediate
  ([label_offset_fold.md](../../../docs/optimizations/label_offset_fold.md)),
  and a value computed only to be passed is stored straight into the
  callee's parameter bytes
  ([backward_copy_propagation.md](../../../docs/optimizations/backward_copy_propagation.md)).
  `get_prompted_number_from_user` went from 150 bytes to 103.
- An add or subtract of a constant copies the bytes where the
  constant's byte is zero, so `page += 0x100` is `INC page+1`, and a
  load and shift that only set C to what it already holds goes
  ([carry_recompute.md](../../../docs/optimizations/carry_recompute.md)).
  `instr_save` went from 152 bytes to 142.
- A store of an immediate A already holds is the `STA` alone, so
  `SCRATCH1 = 0xFFFF` is `LDA #$FF; STA SCRATCH1; STA SCRATCH1+1`
  ([redundant_load.md](../../../docs/optimizations/redundant_load.md)).
  `please_insert_save_diskette` went from 168 bytes to 166.
- A left shift of a value with one significant byte, such as `64 *
  (answer & 0x07)`, is two shifts of that byte in A instead of a call
  to `asl16`
  ([strength_reduction.md](../../../docs/optimizations/strength_reduction.md)).
  `please_insert_save_diskette` sets `SCRATCH1` to `64 * position -
  1` in one statement, where it used to add 64 in a loop: 162 bytes,
  and a constant 47 cycles where the loop took 23 plus 26 a position.
