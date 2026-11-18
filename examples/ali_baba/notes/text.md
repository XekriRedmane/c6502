# text.c

## What it translates

Every routine UNITS.md gives the unit, `main.asm` lines 2434-2497,
12698-12786, 14362-14469, 14475-14499, 14539-14582, 14600-14717, and
15011-15036.

| Original | C |
|---|---|
| NUM_TO_DECIMAL $0DA1, NUM_TO_DECIMAL_CONT $0DA6 | `num_to_decimal(value, out)`, with the table `DECIMAL_PLACES` |
| TEXT_RENDERER $6C8E | `text_renderer`, `text_renderer_no_pause`, and the static `render_stream(i)` |
| DISPLAY_MESSAGE $761E | `display_message(index)` |
| DISPLAY_MESSAGE_LOOP $764A | `display_message_loop(msg)` |
| SCROLL_STATUS_LINE $769B | `scroll_status_line` |
| SET_CURSOR_ROW21 $76BE, SET_TEXT_WINDOW_BOTTOM $76C2 | `set_cursor_row21`, `set_text_window_bottom`, and the static `set_cursor_row(row)` |
| PRINT_BOTTOM_CENTERED $7705, PRINT_CENTERED_PLUS_COL $7710 | `print_bottom_centered(s)`, `print_centered_plus_col(s)` |
| SET_INVERSE_VIDEO $7730, PRINT_CTRL_N $7735 | `set_inverse_video`, `print_ctrl_n` |
| SET_TEXT_WINDOW_WRAP $773A, SET_TEXT_WINDOW_SCROLL $7744 | `set_text_window_wrap`, the static `set_text_window_scroll` |
| PRINT_CTRL_AB $774E | `print_ctrl_ab` |
| PRINT_STRING_AT_ADDR $7758, PRINT_STRING $775C | `print_string(s)` |
| SET_TEXT_WINDOW_UPPER_LEFT_ALL $792D, _LOW $7932, SET_TEXT_WINDOW_UPPER_LEFT $7934 | the static `set_text_window_upper_left(row)` |

The unit defines `DECIMAL_PLACES` (10 bytes) and moves no table of the
original. Nothing is left incomplete. NUM_TO_DECIMAL's entries at +3,
+$30, and +$3E, which only the dead ORPHANED_ENCOUNTER_HANDLER calls,
aren't translated.

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
- NUM_TO_DECIMAL divides with a local value and digit, and writes
  NUM_DIVISOR, NUM_DIGIT, and NUM_LEADING with the original's values
  at each digit. It writes NUM_DIVISOR's high byte for 10 and 1 too,
  where the original writes only the low byte, which holds 0 already.
- TEXT_STREAM_IDX is still written before each glyph, though the C
  keeps the index in a local.

## Header changes needed

None.

## Compiler bugs and gaps

None hit. These cost bytes against the original, for phase 5:

- `TEXT_COL += (space >> 1) + (space & 1);` in
  print_centered_plus_col compiles to an LSR, an AND, and two adds,
  42 bytes for the routine, where the original's `LSR; ADC
  TEXT_COL` takes in the shifted-out bit as the carry, 19 bytes.
- `msg += *msg + 1;` on a `const uint8_t *` compiles to an add of the
  byte, a carry into the high byte, an add of 1, and a second carry.
  The original sets the carry and does one `ADC`.
- print_string and display_message_loop copy their pointer parameter
  into a local pair before the loop that steps it. print_string is 41
  bytes against PRINT_STRING's 18.

## Size

From `objdump.py -t` of `text.o` (before linking), against `main.lst`.

| Original | Bytes | C | Bytes |
|---|---:|---|---:|
| NUM_TO_DECIMAL | 132 | num_to_decimal | 107 + 10 table |
| TEXT_RENDERER | 145 | render_stream, text_renderer, text_renderer_no_pause | 191 |
| DISPLAY_MESSAGE, DISPLAY_MESSAGE_LOOP | 125 | display_message, display_message_loop | 190 |
| SCROLL_STATUS_LINE | 21 | scroll_status_line | 31 |
| SET_CURSOR_ROW21, SET_TEXT_WINDOW_BOTTOM | 20 | the same and set_cursor_row | 32 |
| PRINT_BOTTOM_CENTERED | 11 | print_bottom_centered | 11 |
| PRINT_CENTERED_PLUS_COL | 19 | print_centered_plus_col | 42 |
| SET_INVERSE_VIDEO, PRINT_CTRL_N | 10 | the same | 14 |
| SET_TEXT_WINDOW_WRAP, SET_TEXT_WINDOW_SCROLL | 20 | the same, and an outlined ctrl-O | 27 |
| PRINT_CTRL_AB | 10 | print_ctrl_ab | 14 |
| PRINT_STRING_AT_ADDR, PRINT_STRING | 22 | print_string | 41 |
| SET_TEXT_WINDOW_UPPER_LEFT_ALL, _LOW, SET_TEXT_WINDOW_UPPER_LEFT | 18 | set_text_window_upper_left | 15 |
| **Total code** | **553** | | **715** |

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
`JMP TUNE_ADVANCE` in both programs, as INIT_GAME_STATE leaves it.

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
