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
(see "Compared with Zork's own code"). It is 17 bytes larger than the
original, so it doesn't fit in the 26 sectors that Zork's boot loader
reads (see "Size").

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
| `*.asm` | Each file compiled on its own, and `interpreter.asm`, the listing of the program the eleven link into by the script | |

Each `.c` file's header lists where it departs from the original and
then quotes the original code it translates.

## Building

```sh
cd examples/zork/interpreter
for f in main decode objects numbers input pages zstring output disk prompts save; do
    uv run python ../../../compile.py $f.c -S -O -o $f.asm
    uv run python ../../../compile.py $f.c -c -O
done
uv run python ../../../compile.py -T interpreter.ld \
    main.o decode.o objects.o numbers.o input.o pages.o \
    zstring.o output.o disk.o prompts.o save.o -o interpreter
uv run python ../../../objdump.py -D --no-show-raw-insn --no-addresses \
    interpreter > interpreter.asm
```

Each unit is compiled twice from the same source: to its `.asm`, which
is checked in, and to its object, which the link reads. The link
writes the ELF executable `interpreter`. Neither the objects nor the
executable are checked in. `interpreter.asm` is the executable's
listing in dasm's syntax, with a `SEG` and an `ORG` for each output
section, and `sim/linked.py` runs the executable.
`tests/test_zork_interpreter.py` checks
each unit's output and the linked program against fresh compiles, and
checks that the program calls none of the runtime's helpers and
doesn't use the soft stack. With `ZORK_DSK` set to the path of `Zork I
r15 (4am crack).dsk` from the repository above, it also plays two
sessions on this interpreter and on Zork's own and compares them. The
disk image isn't part of this repository.

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
abbreviation under `#pragma c6502 call save(hardware_stack)`.

**Starting over.** Zork jumps to `main` when a page can't be read and
for the `restart` instruction. `restart()` is declared at `$0800` like
a ROM routine, so that the compiler's call graph has no cycle through
`main`, and
`main` sets each byte of the interpreter's state that's read before
it's written. The original clears `$80`-`$FF`, but the compiler keeps
its own bytes in that range too.

## Compared with Zork's own code

**Behavior.** `tests/test_zork_interpreter.py` plays two sessions on
`sim/apple2.py`, a stand-in Apple II with the Monitor's text routines,
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
file and the original code it translates:

| File | Zork | C |
|---|---:|---:|
| `main.c` | 391 | 446 |
| `decode.c` | 1859 | 1775 |
| `objects.c` | 788 | 809 |
| `numbers.c` | 436 | 419 |
| `input.c` | 1018 | 1059 |
| `pages.c` | 384 | 371 |
| `zstring.c` | 383 | 387 |
| `output.c` | 387 | 399 |
| `disk.c` | 138 | 136 |
| `prompts.c` | 534 | 527 |
| `save.c` | 325 | 332 |
| **Total** | **6643** | **6660** |

The counts include each file's constants and data, and leave out the
page tables and `__zp_init`, 41 bytes of startup code the linker
generates to clear the globals in zero page. The boot loader doesn't
run it, and the interpreter writes each of those globals before it
reads it. A function that ends by jumping to another is placed
right before that one and falls into it, as in Zork. The linker does
that for 32 functions, which saves a 3-byte `JMP` in each. Each
function is counted with the file that defines it, wherever the
linker places it. Zork's boot loader reads 26 sectors,
`$0800`-`$21FF`, so on an Apple II this program would take a loader
that also reads
`$2C00`-`$37FF`, from sectors that Zork's diskette doesn't have free.
The story starts 12 pages higher than Zork's, which leaves the page
cache 64 slots in 48K where Zork has 76.

The four dispatch switches take 778 bytes with the handlers written
into their cases, not counting the tails they share with other
functions (`do_2op` alone is 408), where Zork's four tables
take 126 bytes and the handlers are counted with their own files.
Each switch now dispatches through a table too, and a case that only
calls a handler is the handler's address in the table, as in Zork's.
`divu16` is 61 bytes against Zork's 78, `match_dictionary_word` 181
against 190, and `mulu16` 41 against 69, since it computes only the
low 16 bits of the product.

**Speed.** The first session takes 35,510,387 cycles against Zork's
38,490,260, 7.7% fewer, and the second 6,913,737 against 7,434,428,
7.0% fewer. Before the compiler changes listed below from the 1-byte
right shift on, the two took 35,373,347 and 6,888,843. Of those
changes, the shared tails and the outlined runs add a `JMP`, or a
`JSR` and an `RTS`, where they share code. Most of the difference is the jump tables, which reach
each instruction's case in the same few cycles where the compare
chains had passed one compare after another. `print_number` divides
0 by 10 once, where Zork's prints it without dividing. The stand-ins
for the Monitor and RWTS take no cycles. Where a function is placed matters to these counts, since a taken
branch that crosses a page takes a cycle more. When `objects.c`
shrank by 24 bytes, `branch` and `print_zstring`, which the linker
places after functions of `objects.c` that fall into them, moved,
and that cost the first session 16,534 cycles in the two, more than
the smaller code saved.

## Compiler changes this needed

- `from_ssa` put the copies for a conditional jump's taken edge on the
  fall-through edge when the fall-through edge had copies too, which
  miscompiled `get_next_prop` (`passes/optimization/ssa_destruction.py`,
  and the case in `tests/test_sim_miscompile_regressions.py`).
- A compare narrows through a `ZeroExtend` or `SignExtend` whose
  result has other uses, when the result is SSA-renamed, so that a
  switch on a byte compares bytes
  ([cmp_zero_jump_fold.md](../../../docs/optimizations/cmp_zero_jump_fold.md)).
  `do_1op` went from 183 instructions to 118.
- The linker gives parameters and locals the free bytes below `$80`
  when `$80`-`$FF` is full, before it spills them out of zero page.
  Zork's state takes most of `$80`-`$FF`, and `(zp),Y` pointers
  can't spill ([linker_scripts.md](../../../docs/linker_scripts.md)).
- The linker writes its output with LF line endings on every
  platform, as `compile.py` does.
- A function that ends with a `JMP` to another function of its output
  section goes right before that one, and the `JMP` is dropped, so
  `return_value` falls into `store_result` and `branch` into
  `branch_to_offset`, as Zork's routines fall into each other
  ([linker_scripts.md](../../../docs/linker_scripts.md)).
- `==` and `!=` compare only the low bytes either operand can have
  nonzero, and a comparison truncated to a byte, as in `bool on_true
  = (b & 0x80) != 0;`, has a 1-byte result
  ([narrow_widened_arith.md](../../../docs/optimizations/narrow_widened_arith.md)).
  `branch` had compared a high byte known to be 0 with 0.
- A read of a slot that holds a copy of a zero-page byte at a fixed
  address reads that byte, when the slot's store then goes, and the
  pass that does it runs on functions that end in a tail call
  ([memory_value_propagation.md](../../../docs/optimizations/memory_value_propagation.md)).
  `text = Z_HEADER_ADDR + OPERANDS[0];` had copied `OPERANDS` into
  two slots first.
- A pointer plus a constant byte offset that several 1-byte loads
  and stores read is a `(p),Y` access in each of them
  ([recognize_indirect_indexed.md](../../../docs/optimizations/recognize_indirect_indexed.md)).
  `parse[1]++` in `instr_sread` had built `parse + 1` in a
  zero-page pair.
- A loop's test goes to its bottom, and the loop is entered with a
  `JMP` to it, or at its body when the test is known to pass the
  first time ([rotate_loop_test.md](../../../docs/optimizations/rotate_loop_test.md)).
  The 30-byte clear in `main` and the 6-byte clears in `instr_sread`
  and `ascii_to_zchar` had tested and jumped back on every pass.
- A `static` function returning `bool` returns it in the carry flag,
  without `return_bool_in_carry`, when every call branches on it or
  passes it on ([carry_return_choice.md](../../../docs/optimizations/carry_return_choice.md)).
  `input.c`'s separator tests return in the carry as Zork's do, and
  each call had followed its `JSR` with `ORA #$00`. Every `bool`
  function with external linkage returns in the carry too, so
  `disk.c`'s sector routines need no annotation. The `static`
  `do_rwts_on_sector` keeps it, because `write_to_sector` stores its
  result while it restores the motor's spin-up time, and the carry
  survives the `LDA` and `STA` in between.
- A function returning a pointer returns it in A and X, as an
  `unsigned int` comes back, where it had used `HARGS`
  ([calling_conventions.md](../../../docs/calling_conventions.md)).
  Each call to `get_object_addr` and `get_dictionary_addr` had
  copied the pointer out of `HARGS`.
- `STA t; INC t; LDA t` is `CLC; ADC #1`
  ([shift_in_a.md](../../../docs/optimizations/shift_in_a.md)), a value
  pushed around `TXA; TAY` is loaded again instead
  ([dead_pha_pla.md](../../../docs/optimizations/dead_pha_pla.md)), and
  `LDA M; CLC; ADC #1; TAY` is `INY` when Y holds M
  ([transfer_pm1_store.md](../../../docs/optimizations/transfer_pm1_store.md)).
- A parameter passed on to two callees shares both callees' bytes
  ([slot_sharing.md](../../../docs/optimizations/slot_sharing.md)).
  `is_separator` had copied its `c` from `is_dict_separator`'s byte to
  `is_std_separator`'s.
- A store through a pointer leaves the register copies of a
  function's own zero-page slots, when the unit never takes their
  address ([redundant_load.md](../../../docs/optimizations/redundant_load.md)).
  `read_line` had loaded `c` again after storing it into the line.
- A 2-byte unsigned `<` and `==` of the same values compare the high
  bytes and then the low ones
  ([three_way_compare.md](../../../docs/optimizations/three_way_compare.md)),
  a branch to a copy of a return tail goes to the copy the function
  keeps ([tail_merge.md](../../../docs/optimizations/tail_merge.md)),
  and a compare right after the branches of an identical one goes
  ([redundant_compare.md](../../../docs/optimizations/redundant_compare.md)).
  `match_dictionary_word` had subtracted the two bytes of each half
  of the word from the entry's and then compared them again for
  equality.
- `CLC; ADC #1; TAY` is `TAY; INY`, and `TYA; CLC; ADC #1` is `INY;
  TYA`, when the value left behind in the other register isn't read
  ([transfer_pm1_store.md](../../../docs/optimizations/transfer_pm1_store.md)).
  `match_dictionary_word` steps its index through the dictionary's
  header with `y++`, as the original steps Y.
- A compare with a constant of a register that holds a constant is
  `SEC` or `CLC` when only C is read after
  ([carry_return.md](../../../docs/optimizations/carry_return.md)).
  `return false;` in `is_dict_separator` had become `CPY #1`.
- A 2-byte negate is `0 - x`, `SEC; LDA #0; SBC`, 3 bytes shorter
  than complementing and adding 1
  (`tac_to_asm.py`). `check_sign`, `store_with_sign`, and
  `print_number` negate.
- A `CMP #$00` right after the instruction that set N and Z from the
  register goes, when C is dead
  ([redundant_compare.md](../../../docs/optimizations/redundant_compare.md)),
  and `AND #$01; BEQ` is `LSR; BCC`
  ([and_sign_bit_branch.md](../../../docs/optimizations/and_sign_bit_branch.md)).
  `instr_mul` and `instr_div` had compared each `b`'s high byte with 0.
- The peepholes and the counter promotion run again after a loop's
  test moves to its bottom
  ([counter_to_index_reg.md](../../../docs/optimizations/counter_to_index_reg.md)),
  so `divu16`'s counter is `DEY; BNE`. A counter whose initial value
  comes from the other index register isn't moved, since the 6502 has
  no `TYX`: the move had become `TYA; TAX` before the loop, and the
  loop's first load then went as redundant, which left the
  interpreter looping forever. A move between X and Y is now split
  into its two transfers for the passes that follow A
  ([split_mem_to_mem.md](../../../docs/optimizations/split_mem_to_mem.md)).
- A local that copies a parameter, or that's copied into a callee's
  parameter slot for the call, is that slot when the two are never
  live with different values
  ([slot_coalescing.md](../../../docs/optimizations/slot_coalescing.md)).
  `mulu16` had copied both its parameters, `divu16` its dividend, and
  `check_sign`, `store_with_sign`, and `print_number` the parameter
  they change; `instr_mul` and `instr_div` now compute the product
  and quotient in `store_with_sign`'s bytes.
- A `(zp),Y` pointer that holds a label's address is `label,Y`
  ([memory_value_propagation.md](../../../docs/optimizations/memory_value_propagation.md)),
  so `print_number` indexes its `digits` array directly.
- A compare branches on the flags of the subtract before it with
  copies in between
  ([compare_subtract.md](../../../docs/optimizations/compare_subtract.md)),
  and `(x << 1) | b` for a `b` of 0 or 1 is `LSR` and a `ROL` chain,
  with no `LSR` when C already holds `b`
  ([rotate_in.md](../../../docs/optimizations/rotate_in.md)).
  `divu16`'s loop is now one subtract and the `ROL`s of the quotient
  and remainder, as Zork's is. The C sets `fits` on each side of the
  `if`, which makes the subtract the `if`'s body.
- `LDA M; ASL; STA M` is `ASL M` when A is dead after
  ([shift_in_a.md](../../../docs/optimizations/shift_in_a.md)).
  `instr_random` shifts `RND`'s high byte through a byte view of it,
  which keeps the shift in A, since `RND` is `volatile`.
- `asm_liveness.sets_flags` no longer takes `STX` / `STY` for an
  instruction that sets the flags, and `asm_aliasing.may_alias` takes
  an indexed access to a zero-page local to reach its bytes. Each had
  let a peephole miscompile a program once the changes above gave it
  the shape (`tests/test_divide_shapes.py`).
- `check_sign` negates `a` when the signs of `a` and `b` differ and
  keeps the XOR of their high bytes in `sign_bit`, whose bit 7 is
  then the result's sign. Zork negates `a` once for each negative
  operand and counts them, which comes to the same. A compare whose
  result goes to a static or an address-taken variable no longer
  folds into the jump that tests it
  ([framework/rules.py](../../../passes/optimization/framework/rules.py)
  `single_use`). With `sign_bit` a `bool` set by `(int16_t)(a ^ b) <
  0`, the store had been dropped, and `store_with_sign` never
  negated (`tests/test_cmp_zero_jump_fold.py`). A signed `x < 0` or
  `x >= 0` kept as a value is now `LDA x+1; ASL; LDA #0; ROL`, where
  it had been a 2-byte subtract with the overflow correction
  (`tests/test_sign_test_lowering.py`). The `bool` is still 4 bytes
  longer than keeping the high byte, so `sign_bit` is an `int8_t`.
- A branch goes past the tests at its target that its path has
  already decided
  ([branch_value_thread.md](../../../docs/optimizations/branch_value_thread.md)),
  so `instr_mul` and `instr_div` test `b`'s high byte once, as Zork's
  do, where the `b == 4` test had loaded and tested it again. Two
  blocks that end in the same two or more instructions and a jump to
  the same place share one copy
  ([cross_jump.md](../../../docs/optimizations/cross_jump.md)), so
  the `* 4` and `/ 4` paths shift once and join the `* 2` and `/ 2`
  ones, as Zork's fall into them. `do_0op`, `do_2op`, and
  `do_instruction` get shorter the same way.
- `asm_liveness.kills_a` counts a move from memory or a constant into
  memory, and a move between X and Y, as writing A, since `asm_emit`
  writes them through it. `divu16` had kept a dead `LDA dividend` at
  its entry, before the `LDA #$00` of the store that clears
  `partial`.
- `print_number` divides at least once, so 0 prints as `0` with no
  test of its own, 12 bytes shorter. Zork's tests for 0 after the
  loop, and pushes the digits on the 6502's stack, which C can't.
- A switch's chain of compares dispatches through a table of its
  cases' addresses, `ASL; TAY; LDA t+1,Y; PHA; LDA t,Y; PHA; RTS`
  ([jump_table.md](../../../docs/optimizations/jump_table.md)), as
  Zork's dispatch does, 151 bytes shorter in `decode.c`'s four
  switches. A `JMP` to an `RTS` is that `RTS`, so a case that ends in
  a call and `break` is `JMP f`
  ([tail_call.md](../../../docs/optimizations/tail_call.md)), 174
  bytes there. And a signed or unsigned compare kept as a value is
  `ASL; LDA #0; ROL` or `LDA #0; ROL` where it had branched to `LDA
  #0` or `LDA #1`, 32 bytes in `do_2op`'s `jl`, `jg`, `dec_chk`, and
  `inc_chk`. A tail call no longer counts as reading C, which the
  tables needed at the cases that end in one.
- A constant added to a byte index of a static array goes in the
  address, so `LOCAL_ZVARS[x + 1]` is `LDA LOCAL_ZVARS+1,X` and
  `OPERANDS[i + 1]` is `LDA OPERANDS+2,X`
  ([static_indexed_access.md](../../../docs/optimizations/static_indexed_access.md)).
  CSE matches a shift by an `unsigned char` 1 with a shift by an `int`
  1 ([cse.md](../../../docs/optimizations/cse.md)), so `instr_call`
  shifts `i` once for both `OPERANDS[i + 1]` and the `i << 1` it
  indexes `LOCAL_ZVARS` with. Two stores through `name,X` and `name+1,X` swap places when A
  already holds the second value
  ([redundant_load.md](../../../docs/optimizations/redundant_load.md)).
  Together they take 32 bytes off `instr_call`, which copies a
  routine's locals and arguments with them.
- A value each arm of a `?:` leaves in a slot, which the join copies
  into memory, is stored there by each arm
  ([join_store.md](../../../docs/optimizations/join_store.md)), 16
  bytes in `do_instruction`'s operands.
- A unit compiled on its own puts a slot that doesn't fit in `$80`-`$FF`
  in the free bytes below `$80`, as the linker does, where it had put it
  at `$0200`
  ([zp_local_pools.md](../../../docs/optimizations/zp_local_pools.md)).
  `decode.c`'s zero-page globals fill most of `$80`-`$FF`, and memory
  value propagation, which follows only zero-page bytes, then reads
  `Z_PC` where `branch_to_offset` had copied it into slots, 24 bytes
  in it and `set_nonstack_var`.
- `AND #$00` is `LDA #$00`, and `LDA #k1; CLC; ADC #k2` is `LDA` of the
  sum when C and V aren't read after
  ([const_arith_fold.md](../../../docs/optimizations/const_arith_fold.md)),
  22 bytes in `branch_to_offset`.
- `zork.h` declares `internal_error`, `instr_quit`, `instr_restart`,
  and `restart` `[[noreturn]]`, and nothing after a call to one of
  them has to survive the call
  ([noreturn_call.md](../../../docs/optimizations/noreturn_call.md)).
  `do_2op` reads `a` from `OPERANDS`, where it had copied it on entry
  because `je` calls `internal_error` before reading it.
- `x -= 2` on an index that's then loaded into X is `LDX x; DEX; DEX;
  STX x`
  ([transfer_pm1_store.md](../../../docs/optimizations/transfer_pm1_store.md)),
  7 bytes in `return_value` with the store swap it allows. `if (new_lo
  < lo)` after `new_lo = lo + offset` branches on the add's carry
  without a compare
  ([redundant_compare.md](../../../docs/optimizations/redundant_compare.md)),
  2 bytes in `branch_to_offset`. And `je`'s `!equal && --left != 0`
  branches directly, where the copy of `left` on one arm had kept it
  a 0 or 1
  ([short_circuit_jump_fold.md](../../../docs/optimizations/short_circuit_jump_fold.md)),
  6 bytes.
- A test of bit 6 of a byte in memory is `BIT m; BVC`
  ([and_sign_bit_branch.md](../../../docs/optimizations/and_sign_bit_branch.md)),
  4 bytes in `branch`.
- A jump table's entry for a case that only jumps to another function
  is that function, and the case goes, as does a default that only
  calls `internal_error`. `LDA #0; JMP end` is `LDA #0; BEQ end` when
  `end` is in reach
  ([short_jumps.md](../../../docs/optimizations/short_jumps.md)). The
  jump table takes in the case for 0 after the `ORA #$00` that tests a
  switch's register parameter
  ([jump_table.md](../../../docs/optimizations/jump_table.md)).
  Together they take 112 bytes off the four dispatch switches and 9
  off the rest.
- Dead-A elimination checks each flag an `ADC` writes on its own, so a
  `BEQ` after a later `LDA` no longer keeps it
  ([dead_a_arith.md](../../../docs/optimizations/dead_a_arith.md)), 12
  bytes in `remove_obj` and `instr_insert_obj`.
- A byte copied while A is busy goes through X
  ([via_a_store_fold.md](../../../docs/optimizations/via_a_store_fold.md)),
  4 bytes at `do_var`'s entry.
- `<apple2/monitor_io.h>` declares `VTAB` and `GETLN1`, and names the
  random seed at `$4E` `RND`.
- A right shift of a byte promoted to `int` and truncated back is a
  1-byte shift
  ([narrow_widened_arith.md](../../../docs/optimizations/narrow_widened_arith.md)).
  `get_property_len`'s `props[y] >> 5` had shifted the promoted high
  byte, known to be 0, left by three and ORed it in, 26 bytes in
  `objects.c` and `input.c`. A 1-byte `x > k` is `x >= k + 1` and `x
  <= k` is `x < k + 1`, so `x` stays in A for `CMP #k+1`
  ([cmp_zero_jump_fold.md](../../../docs/optimizations/cmp_zero_jump_fold.md)),
  10 bytes in the range tests of `read_line` and
  `get_alphabet_for_char`.
- `CLC; ADC #$00` is `CLC` when V is dead after
  ([const_arith_fold.md](../../../docs/optimizations/const_arith_fold.md)),
  8 bytes in the 2-byte adds of a page number shifted left by 8 in
  `main` and `pages.c`.
- A `static` function returning a pointer returns it in a zero-page
  pointer of its own, and a read through the result before the next
  call reads through that pointer
  ([return_in_zero_page.md](../../../docs/optimizations/return_in_zero_page.md)),
  as Zork's `get_object_addr` leaves the address in `SCRATCH2`.
  `get_object_addr` is `static` in `objects.c` for this, since no other
  unit calls it, and `get_dictionary_addr` in `input.c` already was.
  Each call had stored the result from A and X into a zero-page pair,
  22 bytes in the two units.
- When parameters that a function with external linkage passes on
  can't share their callees' bytes, slot sharing drops those first and
  checks the other values again
  ([slot_sharing.md](../../../docs/optimizations/slot_sharing.md)).
  `push`, `pop`, and `var_put` are `static` in `decode.c`, since no
  other unit calls them. Then `var_get` keeps the value it pops in
  `push`'s bytes across `push(value)`, and `var_put`'s value is in
  `push`'s bytes, where its callers store it, 26 bytes in `decode.c`.
  `store_result` still copies its value, since other units call it,
  and its value is live across `get_next_code_byte`, which another
  unit defines.
- `LDA #>page_tables; CLC; ADC #$01` is `LDA #>(page_tables+256)`, and
  `ASL; CLC; ADC #1` is `SEC; ROL`, when C and V are dead after
  ([const_arith_fold.md](../../../docs/optimizations/const_arith_fold.md)),
  and a run of constant stores loads each value once
  ([constant_store_order.md](../../../docs/optimizations/constant_store_order.md)),
  9 bytes in `main` and `get_property_ptr`.
- A 2-byte decrement in place tests the low byte and decrements the
  high one before the low one, `LDA lo; BNE; DEC hi; DEC lo`
  ([dec_peephole.md](../../../docs/optimizations/dec_peephole.md)),
  and an `ORA #$00` goes when every way into the label before it set
  N and Z from A
  ([redundant_compare.md](../../../docs/optimizations/redundant_compare.md)),
  11 bytes in `push`, `je`, and `please_insert_save_diskette`.
- Code that several functions of a unit end with alike goes in a
  function of its own, which each of them jumps to, and the linker
  puts one of them right before it
  ([shared_tails.md](../../../docs/optimizations/shared_tails.md)), 79
  bytes in 15 tails, such as the store of a byte with `store_result`
  that four of `objects.c`'s property routines end with, and the
  message and endless loop `quit` and the internal error end with.
- `p[i + k]` through a pointer is `(p),Y` with Y = `i + k` when `i`'s
  largest value plus `k` fits in a byte, as for an `i` shifted left
  by 1
  ([recognize_indirect_indexed.md](../../../docs/optimizations/recognize_indirect_indexed.md)),
  so `print_zstring` reads the abbreviation table's second byte with
  `INY`, as Zork's does, 14 bytes shorter.
- A call to a function of the unit whose writes are known, directly or
  through what it calls, ends only the facts about those bytes, and a
  call to a `[[noreturn]]` function adds nothing
  ([memory_value_propagation.md](../../../docs/optimizations/memory_value_propagation.md)).
  The property instructions, `remove_obj`, `instr_insert_obj`, and
  `do_var` read `OPERANDS` after their calls, where they had copied
  it into a slot first, 28 bytes.
- `AND #$80; BEQ` is `BPL` after any instruction that set N from A,
  with stores between
  ([and_sign_bit_branch.md](../../../docs/optimizations/and_sign_bit_branch.md)),
  8 bytes in `ascii_to_zchar` and `get_alphabet_for_char`.
- A run of instructions repeated in a unit goes in a function of its
  own, which each place calls, ending with a jump to its last call
  ([outline_runs.md](../../../docs/optimizations/outline_runs.md)), 75
  bytes, such as the 19 that pass both operands to `check_sign` in
  `mul`, `div`, and `mod`. Each place pays 12 cycles.
- `instr_get_prop` and `instr_put_prop` read and write the second byte
  of a word property at `props[(uint8_t)(y + 1)]`, a byte offset like
  the original's Y, where `props[y + 1]` had made C add `y` to `props`
  as 16 bits. That's a change to `objects.c`, not to the compiler,
  and with the tails and runs it lets go, it takes 52 bytes off.
- A jump on a value right after its last step tests the stepped value
  ([sink_increment.md](../../../docs/optimizations/sink_increment.md)),
  so `search_nonalpha_table`'s `while (x-- != 0)` is `DEX; CPX #$FF;
  BNE`, 9 bytes shorter.
- `attr_ptr_and_mask` shifts the static `attr_mask` in place, as Zork
  shifts `SCRATCH3`, where it had built the mask in a local and copied
  it, 18 bytes. That's a change to `objects.c`.
