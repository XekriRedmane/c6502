# text.c

## What it translates

Every routine UNITS.md gives the unit, `main.asm` lines 2434-2497,
12698-12786, 14362-14469, 14475-14499, 14539-14582, 14600-14717, and
15011-15036.

| Original | C |
|---|---|
| NUM_TO_DECIMAL $0DA1, NUM_TO_DECIMAL_CONT $0DA6 | `num_to_decimal(value, out)` |
| TEXT_RENDERER $6C8E | `text_renderer`, `text_renderer_no_pause`, and the static `render_stream(i)` |
| DISPLAY_MESSAGE $761E | `display_message(index)`, and the static `next_string(s)` it shares with display_message_loop |
| DISPLAY_MESSAGE_LOOP $764A | `display_message_loop(msg)` |
| SCROLL_STATUS_LINE $769B | `scroll_status_line` |
| SET_CURSOR_ROW21 $76BE, SET_TEXT_WINDOW_BOTTOM $76C2 | `set_cursor_row21`, `set_text_window_bottom`, and the static `set_cursor_row(row)` |
| PRINT_BOTTOM_CENTERED $7705, PRINT_CENTERED_PLUS_COL $7710 | `print_bottom_centered(s)`, `print_centered_plus_col(s)` |
| SET_INVERSE_VIDEO $7730, PRINT_CTRL_N $7735 | `set_inverse_video`, `print_ctrl_n` |
| SET_TEXT_WINDOW_WRAP $773A, SET_TEXT_WINDOW_SCROLL $7744 | `set_text_window_wrap`, the static `set_text_window_scroll` |
| PRINT_CTRL_AB $774E | `print_ctrl_ab` |
| PRINT_STRING_AT_ADDR $7758, PRINT_STRING $775C | `print_string(s)` |
| SET_TEXT_WINDOW_UPPER_LEFT_ALL $792D, _LOW $7932, SET_TEXT_WINDOW_UPPER_LEFT $7934 | the static `set_text_window_upper_left(row)` |

The unit has no tables and moves none of the original's. Nothing is
left incomplete. NUM_TO_DECIMAL's entries at +3, +$30, and +$3E, which
only the dead ORPHANED_ENCOUNTER_HANDLER calls, aren't translated.

## Departures

The file's opening comment lists them. In short:

- TEXT_RENDERER's recursion through DISPLAY_STATUS_BAR is split as
  UNITS.md says. `render_stream(i)` renders from index i to the end or
  to a $7E and returns STREAM_END ($100) or the $7E's index.
  `text_renderer` plays TUNE_ATTRACT, saves TEXT_STREAM_PTR across
  `display_status_bar()`, and calls render_stream again from the byte
  after the $7E, which sets the scene's margins again as the original's
  `JMP .init` does. `text_renderer_no_pause` stops at a $7E. The
  original passes the stream index in Y to TUNE_PLAYER, which only
  tunes 1 and 2 read, and the C passes 0.
- TEXT_RENDERER's `JMP MOVE_LOOP`, when the index passes 255 after a
  glyph, is a return. A position command that passes 255 wraps to
  index 0 and goes on, as in the original.
- num_to_decimal converts the value to packed BCD with
  `__c6502_bin_to_bcd` and reads the digits from its nibbles, and its
  digit and leading-zero flag are locals, so it doesn't write
  NUM_DIVISOR, NUM_DIGIT, and NUM_LEADING (`$5A1B`-`$5A1E`). Only NUM_TO_DECIMAL
  reads those bytes in the original, and a save holds `$4000`-`$59FF`,
  so no reader sees the difference. The declarations stay in
  ali_baba.h, which other units share.
- render_stream doesn't write TEXT_STREAM_IDX (`$5A98`), where the
  original saves its index there across RENDER_FONT_CHAR. Nothing else
  reads it.
- num_to_decimal's conversion calls the runtime helper
  `bin16_to_bcd`, which the linker adds from the runtime library. The
  original calls no helper.

## Header changes needed

None.

## Compiler bugs and gaps

No bug was hit. These cost bytes against the original:

1. **`place /= 10` calls `udivmod16`.** num_to_decimal's `value /
   place` and `value % place` share one call, and dividing the place
   by 10 makes a second, so a number takes 10 helper calls.
   num_to_decimal is 125 bytes, where the original's repeated
   subtraction is 132 bytes and the old C's was 111 and a 10-byte
   table of the places. A 16-bit value divided by the constant 10 could
   be done inline, and a loop over the five places, which are
   constants, could be unrolled into five divisions by constants.

   On 19 Nov 2026 num_to_decimal was rewritten with the new builtin
   `__c6502_bin_to_bcd`, which converts the value to five packed BCD
   digits with one call of the runtime helper `bin16_to_bcd` (46
   bytes, docs/builtins.md). The loop reads each digit from a nibble,
   and the function is 96 bytes. It wrote the same five bytes as the
   version with `/` and `%` for all 65536 values, in the simulator,
   in both pipelines. A division of a 16-bit value by a constant is
   still a helper call.

   ```c
   #include <stdint.h>
   void f(uint16_t v, uint8_t *out) {
       for (uint16_t p = 10000; p != 0; p /= 10, out++) {
           *out = v / p;
           v %= p;
       }
   }
   ```

2. **`(space + 1) / 2`, with `space` a `uint8_t`, is a 9-bit shift
   right**, an add of 1 with the carry into a high byte, `LSR` of the
   high byte, and `ROR` of the low. The original's `LSR; ADC TEXT_COL`
   takes in the rounding as the carry, which is the same sum.
   print_centered_plus_col is 42 bytes, against the original's 19.

   This was fixed on 19 Nov 2026 (docs/optimizations/carry_halve.md):
   the add's carry is the sum's ninth bit, so `CLC; ADC #1; ROR`
   computes the quotient. The same day, `40 - s[0]` subtracted the
   byte through the pointer instead of a copy of it
   (docs/optimizations/cmp_through_tmp_fold.md), and the function is
   30 bytes. It still adds 1 where the original's `ADC` takes in the
   bit its `LSR` shifts out.

   ```c
   #include <stdint.h>
   extern uint8_t COL [[c6502::zeropage(0x24)]];
   void f(uint8_t space) { COL += (space + 1) / 2; }
   ```

3. `s + s[0] + 1` on a `const uint8_t *` adds the byte, carries into
   the high byte, adds 1, and carries again, where the original sets
   the carry and does one `ADC`. next_string is 27 bytes.

   This was fixed on 19 Nov 2026 (docs/optimizations/carry_in_add.md),
   and next_string is the original's `SEC; ADC`, 17 bytes.

   The same day next_string was inlined into its two callers
   (docs/optimizations/inline_functions.md). display_message is 58
   bytes and display_message_loop 95, 5 fewer than the 17-byte
   next_string and callers of 52 and 89 bytes that call it. The
   inlined copy first came out 11 bytes longer, because
   loop_counter_to_x took msg's high byte, which `carry_in_add`
   steps with one `INC`, for a loop counter and kept it in X around
   each of the four calls. The tests of `*msg` after the step then
   read through the old high byte, 256 bytes below the string, and
   the forest script's run differed from the original's. That was
   fixed the same day (docs/optimizations/loop_counter_to_x.md).

4. print_string and display_message_loop copy their pointer parameter
   into a local pair before the loop that steps it. print_string is 38
   bytes against PRINT_STRING's 18.

   On 19 Nov 2026 display_message_loop came to keep its pointer in
   `next_string@ret`, where next_string returns the next one
   (docs/optimizations/ret_slot_coalescing.md), so the copy after each
   call of next_string went, and so did display_message's. The copy of
   the argument into that pair at entry stays, since the loop reads
   through it. print_string is unchanged.

These were fixed on 19 Nov 2026 too, and weren't listed:

- print_bottom_centered kept its argument's low byte in a local
  across its two constant stores. The constants go through Y now,
  since the tail call reads X (docs/optimizations/via_a_store_fold.md),
  and the function is the original's 11 bytes.
- display_message copied `msg` into a second local pair at the top of
  each pass and stepped it through a third. Move coalescing gives the
  copy one pair (docs/optimizations/coalescing.md), and the step is
  `INC`.

## Size

From `objdump.py -t` of `text.o` (before linking), against `main.lst`.

| Original | Bytes | C | Bytes |
|---|---:|---|---:|
| NUM_TO_DECIMAL | 132 | num_to_decimal | 96 |
| TEXT_RENDERER | 145 | render_stream, text_renderer, text_renderer_no_pause | 180 |
| DISPLAY_MESSAGE, DISPLAY_MESSAGE_LOOP | 125 | display_message, display_message_loop (next_string inlined) | 153 |
| SCROLL_STATUS_LINE, SET_TEXT_WINDOW_SCROLL | 31 | scroll_status_line (set_text_window_scroll inlined) | 32 |
| SET_CURSOR_ROW21, SET_TEXT_WINDOW_BOTTOM | 20 | the same and set_cursor_row | 24 |
| PRINT_BOTTOM_CENTERED | 11 | print_bottom_centered | 11 |
| PRINT_CENTERED_PLUS_COL | 19 | print_centered_plus_col | 30 |
| SET_INVERSE_VIDEO, PRINT_CTRL_N | 10 | the same | 10 |
| SET_TEXT_WINDOW_WRAP | 10 | set_text_window_wrap | 10 |
| PRINT_CTRL_AB | 10 | print_ctrl_ab | 10 |
| PRINT_STRING_AT_ADDR, PRINT_STRING | 22 | print_string | 38 |
| SET_TEXT_WINDOW_UPPER_LEFT_ALL, _LOW, SET_TEXT_WINDOW_UPPER_LEFT | 18 | set_text_window_upper_left | 11 |
| **Total code** | **553** | | **605** |

The C's bytes are the symbol sizes of `nm.py -S text.o`. Before the
rewrite into plain C on 18 Nov 2026, the unit was 668 bytes of code
and 10 of data. It was 729 bytes after the rewrite, and 692 once c6502
shared one call of `udivmod16` between a `/` and a `%` and divided a
byte by 2 without `sdivmod16`. On 19 Nov 2026 it was 642, after the
fixes listed under "Compiler bugs and gaps", 613 once
num_to_decimal used `__c6502_bin_to_bcd`, and 605 once c6502
inlined next_string and set_text_window_scroll into their callers. The linker adds
`bin16_to_bcd`'s 46 bytes to the program from the runtime library,
where it added `udivmod16`'s 49 before.

## Checked against the original

The differential test described in notes/hrcg.md also calls each text
routine on both programs with the same arguments and compares events
and memory: num_to_decimal for 13 values, display_message for every
message 0-$3B, print_string, print_bottom_centered, and
print_centered_plus_col on five strings, the window and control
routines, scroll_status_line, text_renderer and text_renderer_no_pause
on STATUS_BORDER_DATA and UI_TEXT_STREAM, and text_renderer on a
stream with every kind of byte and a $7E pause, which runs the title
tune and the status bar. Every case matched. The test patches $7A49 to
`JMP TUNE_ADVANCE` in both programs, as INIT_GAME_STATE leaves it. The test ran on the C
as it was before the rewrite into plain C on 18 Nov 2026, and the
rewrite was checked only with the six scripts of TestRuns.

## Where the original's comments and code disagree

- PRINT_CENTERED_PLUS_COL's comment gives (40 - length) / 2 +
  TEXT_COL. Its ADC adds the bit the LSR shifts out, so the quotient
  is rounded up. The C follows the code.
- DISPLAY_MESSAGE's `BALT MSG_LINE_COUNT,.pause` is commented "printed
  <= 1 lines?", and it branches when more than one line has been
  printed. The header comment says the message pauses "between groups".
  It pauses after every line but a first one with more after it.
- SET_CURSOR_ROW21's comments call its stores "WNDBTM = 21" and
  "WNDLFT = 0". They store TEXT_ROW (CV) and TEXT_COL (CH).
- TEXT_RENDERER's `STOB #$02,FONT_CHARSET` is commented "character
  width = 2". FONT_CHARSET selects the second pair of fonts.
- SCROLL_STATUS_LINE's comment says the $83 "triggers a scroll-up".
  $83 is ctrl-C, which restores the saved column and does a line feed
  when that column is at or left of the cursor. The line feed on row
  23 scrolls the window. The saved column is 0 whenever the game sends
  it, because the game's only ctrl-B is in s_PRINT_FONT_CHAR, which
  ends with a ctrl-D that zeroes it (report_hrcg's scan of the
  strings).
