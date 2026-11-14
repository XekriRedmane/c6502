# Zork's z-string printer, in C

This is `print_string_literal` from Zork I (r15) for the Apple II, the
Z-machine instruction that prints the text following it, with the
z-string decoder, the page cache, the line buffer, and the disk
routines it reaches, rewritten in C as four source files and linked
with a linker script into the addresses the original occupies. The
original is in `output/main.asm` of
https://github.com/XekriRedmane/zorki-r15-reveng:
`print_string_literal` at line 908, the page cache and decoder at
lines 2431-2809, the line buffer at lines 3050-3227 and 3282-3294,
and the disk routines at lines 3359-3431.

It's a different test of the compiler from `../instr_save`:

- **Recursion.** An abbreviation is a z-string printed inside another,
  so `print_zstring` calls itself, saving the decoder's state on the
  hardware stack with `#pragma c6502 call save(hardware_stack)`, as
  Zork does with `PHA`.
- **Placement.** Zork keeps these routines in four places, and
  `print_string_literal` among its instruction handlers. The linker
  script puts each source file, and that one function, where the
  original is.
- **Hot loops.** Printing a string runs the decoder once per
  z-character and the line buffer once per character, so the code's
  speed shows, and the test compares it with Zork's cycle for cycle.

## Files

| File | What's in it | Original |
|---|---|---|
| `zork.h` | Zork's zero page and memory, the IOB and DCT structures, `RWTS` and Zork's `main`, and each unit's routines | |
| `disk.c` | The IOB and DCT, `do_rwts_on_sector`, `read_from_sector` | lines 3359-3431 |
| `pages.c` | `invalidate_zcode_page2`, `load_packed_address`, `get_next_code_word`, `get_next_code_byte2`, `set_page_first`, `find_index_of_page_table` | lines 2431-2597 |
| `output.c` | `buffer_char`, `dump_buffer_line`, `dump_buffer_to_printer`, `dump_buffer_to_screen`, `dump_buffer_with_more`, `cout_string`, and `[MORE]` | lines 3050-3227 and 3282-3294 |
| `zstring.c` | `print_zstring`, `A_mod_3`, `get_alphabet`, `get_next_zchar`, alphabet 2's table, and `print_string_literal` | lines 2598-2809 and 908-921 |
| `print_zstring.ld` | The linker script | |
| `disk.asm`, `pages.asm`, `output.asm`, `zstring.asm` | Each file compiled on its own | |
| `print_zstring.asm` | The four linked by the script | |

Each `.c` file's header quotes the original code and lists where it
departs from it.

## Building

```sh
cd examples/zork/print_zstring
for f in disk pages output zstring; do
    uv run python ../../../compile.py $f.c --codegen --optimize -o $f.asm
done
uv run python ../../../compile.py --link -T print_zstring.ld \
    disk.asm pages.asm output.asm zstring.asm -o print_zstring.asm
```

`print_zstring.asm` assembles with dasm. It has no `main`, since it's
a piece of Zork. `tests/test_zork_print_zstring.py` links it with a
`main` that calls `print_string_literal`, runs it, and checks the
outputs above against fresh compiles.

## How it's put together

**Zero page.** As in `../instr_save`, `zork.h` declares every byte
Zork and DOS own as an `extern` `zeropage(addr)` object, by name where
these routines use it and as a reservation otherwise, so the compiler
keeps its own bytes in `$00`-`$1E`, `$50`-`$7B`, and `$F3`-`$FF`.

**Placement.** `print_zstring.ld` has a region at each place Zork
keeps these routines, running to the start of the next: `$17DD` for
`pages.c` and then `zstring.c`, `$1B90` for `output.c`, and `$1DAC`
for `disk.c`, its IOB and DCT first. Input descriptions name the
file, `zstring.asm(.text .text.* .rodata .rodata.*)`, so each unit's
code goes in its region. `print_string_literal` has
`__attribute__((section(".text.literal")))`, which the script puts at
`$0C5B`. The C version of a region is larger than Zork's, so it runs
over Zork's routines that this example leaves out, which don't exist
here.

**Recursion.** `print_zstring` is on a call-graph cycle, which a
private zero-page pool normally rules out. The pragma on its
recursive call makes each call push the caller's live values. The
link metadata marks that call edge `saved`, and `--link` accepts the
cycle the way the compile did.

**Arguments in registers and in zero page.** `RWTS` takes its
parameter block in Y and A, declared `struct iob *iob
__attribute__((in_ya))`. `cout_string` takes its string at `SCRATCH2`,
`$E6`, as a `zeropage(0xE6)` parameter, so Zork's other routines can
still call it.

## Compared with Zork's own code

**Behavior.** The test runs three scenarios on a five-page story: the
string, which crosses from a resident page into the cache, evicts a
cache slot, uses every kind of z-character and two abbreviations,
wraps between words in a 20-column window, and stops twice for
[MORE]; the same with a transcript, which copies each line to the
printer; and the same with every disk read failing, which ends at
Zork's `main`. With `ZORK_MAIN_BIN` set to Zork's `main.bin`, the test
runs Zork's own `print_string_literal` the same way. The two print the
same characters to the screen and the printer and leave the same
interpreter state in every scenario.

**Size.** In bytes, measured from the assembled programs:

| Routine | Zork | C |
|---|---:|---:|
| `print_string_literal` | 44 | 44 |
| `invalidate_zcode_page2` | 5 | 5 |
| `load_packed_address` | 18 | 18 |
| `get_next_code_word` | 13 | 15 |
| `get_next_code_byte2` | 127 | 126 |
| `set_page_first` | 53 | 58 |
| `find_index_of_page_table` | 29 | 37 |
| `print_zstring` | 216 | 242 |
| `A_mod_3` | 11 | 10 |
| `get_alphabet` | 12 | 12 |
| `get_next_zchar` | 76 | 78 |
| `buffer_char` | 82 | 88 |
| `dump_buffer_line` | 15 | 14 |
| `dump_buffer_to_printer` | 78 | 88 |
| `dump_buffer_to_screen` | 21 | 21 |
| `dump_buffer_with_more` | 110 | 117 |
| `cout_string` | 14 | 14 |
| `do_rwts_on_sector` | 51 | 50 |
| `read_from_sector` | 6 | 5 |
| **Code** | **981** | **1042** |

Zork's `buffer_char` counts its `buffer_char_set_buffer_end` label,
which `dump_buffer_with_more` jumps to. The data is the same: the IOB
and DCT, the printer flag, `[MORE]`, and alphabet 2's 24 characters.

**Speed.** Counting the cycles from the call to `print_string_literal`
to its return (the stand-ins take none), the string takes 41,567
cycles against Zork's 39,980, 4.0% more; with a transcript, 46,022
against 43,687; and to the failed read, 2,373 against 2,283. When the
example was first linked, the string took 55,570 cycles, 39% more than
Zork's, and the code was 1,122 bytes.

**Where the difference comes from.** Per routine, in the first
scenario:

- `get_alphabet`, `get_next_zchar`, `get_next_code_word`, `A_mod_3`,
  `dump_buffer_to_screen`, `cout_string`, and the disk routines take
  as many cycles as Zork's or fewer. `get_alphabet` and
  `get_next_code_byte2`'s path for a byte in the current page are
  Zork's own code.
- `print_zstring` takes 1,427 more, about 3 per z-character. Its
  `if` / `else if` chain branches past each case that doesn't match,
  where Zork falls through to the next test and puts the rare cases
  out of line, and a letter jumps to the shared `JSR buffer_char`
  that Zork places right after it.
- `buffer_char` takes 258 more and `get_next_code_byte2` 116 more,
  in the code that wraps a line and that maps a new page.

## Compiler changes this example led to

Linking it needed:

- An input description in a linker script can name its file,
  `zstring.asm(.text)` (passes/link_script.py, passes/link_layout.py).
- `--link` accepts a recursive cycle whose every call is under
  `#pragma c6502 call save(hardware_stack)`: the link metadata marks
  such call edges `saved` (passes/zp_link_metadata.py,
  passes/linker.py).

Comparing it with Zork's code then led to, roughly in order of how
many cycles each saved:

- A right shift whose result is only truncated to a byte computes
  just that byte, from two bytes of the source
  ([narrow_widened_arith.md](../../../docs/optimizations/narrow_widened_arith.md)).
  `get_next_zchar`'s `(uint8_t)(ZCHARS >> 5)` called `lsr16`, which
  isn't in Zork at all: 5,933 cycles.
- A short-circuit `&&` tested by an `if` branches directly even when
  SSA destruction puts a copy between them
  ([short_circuit_jump_fold.md](../../../docs/optimizations/short_circuit_jump_fold.md)),
  and `LDA M; STA t; CPX t` is `CPX M` even for the Monitor's volatile
  `WNDWDTH` ([cpx_cpy_peephole.md](../../../docs/optimizations/cpx_cpy_peephole.md)).
  `buffer_char`'s character path became Zork's: 4,500 cycles.
- A mask on a wide value that falls in one byte tests that byte
  ([and_zero_jump_fold.md](../../../docs/optimizations/and_zero_jump_fold.md)):
  `(ZCHARS & 0x8000)` is `LDA ZCHARS+1; BPL`.
- `TYA; CLC; ADC #1; STA M` is `INY; STY M` when a branch reads N or
  Z after it ([transfer_pm1_store.md](../../../docs/optimizations/transfer_pm1_store.md)),
  and a constant store goes through Y when A holds a value that's
  loaded again after it ([redundant_load.md](../../../docs/optimizations/redundant_load.md)),
  which made `get_next_code_byte2`'s fast path and `get_alphabet`
  Zork's own code.
- A reload of a value A already holds, kept for a branch on its flags,
  is `ORA #$00` ([redundant_load.md](../../../docs/optimizations/redundant_load.md),
  [round_trip_load.md](../../../docs/optimizations/round_trip_load.md)),
  `ADC`, `AND`, and `ORA` read their operand in place without a store
  of the result after them
  ([adc_commute.md](../../../docs/optimizations/adc_commute.md)), and
  a compare reads a volatile byte in place
  ([cmp_through_tmp_fold.md](../../../docs/optimizations/cmp_through_tmp_fold.md)),
  which made `dump_buffer_with_more`'s `++CURR_LINE >= WNDBTM` Zork's
  `INC CURR_LINE; LDA CURR_LINE; CMP WNDBTM`.

The fuzzers then found that rematerialization took a constant store
for something that leaves A alone, which these changes exposed
([asm_remat.md](../../../docs/optimizations/asm_remat.md)).
