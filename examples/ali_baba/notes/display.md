# display.c

## What it translates

Every routine UNITS.md gives the unit, from `main.asm` lines
12787-12828, 14500-14538, and 14740-14895.

| Original | C |
|---|---|
| DISPLAY_STATUS_BAR $6D1F | `display_status_bar()` |
| SETUP_TEXT_POS $76D2, SETUP_TEXT_POS_COL20 $76D8 | `setup_text_pos(name)`, `setup_text_pos_col20(name)`, which call the static `print_panel_name(col, name)` |
| DISPLAY_CHAR_STATS $7780 | `display_char_stats()` |

The unit defines no tables. Every routine is translated, and none is
left incomplete.

## Departures

The file's opening comment lists them. In short:

- `display_status_bar` renders STATUS_BORDER_DATA with
  `text_renderer_no_pause()`, which keeps the call graph free of the
  cycle between TEXT_RENDERER and DISPLAY_STATUS_BAR.
- The two entries of SETUP_TEXT_POS are one function with the column as
  a parameter, and the name, which the original passes in
  PRINT_STRING_ADDR, is a parameter.
- `print_panel_name` computes `TEXT_COL = 10 - TEXT_COL` from the
  column the HRCG left after the 20 spaces, as the original does. That
  column depends on hrcg_cout's handling of the row's end, so the C
  reads TEXT_COL there rather than computing a constant.

## Where the original's comments and code disagree

The C follows the code in each case.

- SETUP_TEXT_POS's header says it sends "ctrl-AB and a carriage
  return". The second character is `$8C`, HRCG_CTRL_L, which selects
  lower case, and the routine ends with `$8B`, HRCG_CTRL_K, upper
  case. UNITS.md repeats "a carriage return".
- DISPLAY_STATUS_BAR's comments call its ctrl-P "toggle inverse", and
  its stores to TEXT_ROW and TEXT_COL "WNDBTM = 23" and "WNDLFT = 4".
  Ctrl-P clears the window and homes the cursor, and the stores set the
  cursor.
- DISPLAY_CHAR_STATS's comments put the cursor at "column 21, row 0"
  and "column 20, row 0". The code stores 21 and 20 to CV ($25) and 0
  to CH ($24), which is row 21 or 20 at column 0.
- The same routine's comments name the armor field (byte 8) "group
  count", the dagger (byte 9) "ducats", the speed (byte 12) "armor",
  and the gold (bytes 13-14) "experience". The C names each by the
  header's field and the stats screen's label.
- Its "cosmetic" test is described as blanking a "0" tens digit when
  the hundreds digit is blank. STATS_DUCATS is one place after
  STATS_DUCATS_OUT, so STATS_DUCATS+2 and +3
  (`STATS_TEMPLATE.ducats[3]` and `[4]`) are the gold's tens and ones
  places. The test blanks the ones digit when the gold is 0, so no
  gold shows no digits.
- UNITS.md says the routine stores `anim_check_frame()`'s value to
  KBDSTRB. The code calls ANIM_TICK_AND_WAIT, which falls into
  ANIM_CHECK_FRAME, and stores its result. The C stores
  `anim_tick_and_wait()`'s.

## Header

No change to `ali_baba.h` is needed.

## Compiler gaps

`print_panel_name` is larger than the original's body because each
`hrcg_cout` call stores its argument in a zero-page parameter where the
original's COUT takes A, and the 20-space loop keeps its counter in
memory across the call where the original keeps it in X, which COUT
preserves. Two more gaps come from the rewrite as plain C.

1. **A loop that counts up with an unused index isn't turned to count
   down.**

   ```c
   #include <stdint.h>
   void out(uint8_t c);
   void f(void) { for (uint8_t i = 0; i < 20; i++) out(0xA0); }
   ```

   compiles the step to `INC i; LDA i; CMP #$14; BCC`, where the loop
   counting from 20 to 0 compiles to `DEC i; BNE`, 3 bytes fewer.
   `print_panel_name` pays them (50 to 53 bytes).

2. **A static function called once isn't inlined.**

   ```c
   #include <stdint.h>
   #include <stdbool.h>
   extern volatile uint8_t KBD [[c6502::address(0xC000)]];
   extern volatile uint8_t BUTN [[c6502::address(0xC061)]][2];
   extern uint8_t MODE [[c6502::address(0x5A00)]];
   void done(void);
   static bool ready(void) {
       return (KBD & 0x80) != 0
           || (MODE != 0 && ((BUTN[0] & 0x80) != 0 || (BUTN[1] & 0x80) != 0));
   }
   void f(void) { while (!ready()) ; done(); }
   ```

   compiles `ready` to the tests and `CLC; RTS` or `SEC; RTS`, and the
   loop to `JSR ready; BCC`. Written in the loop, as the old C had
   them, the tests branch straight back to the loop or out of it, as
   the original's do. `key_or_button` costs 9 bytes this way (24
   bytes, against the 15 it took off `display_char_stats`).

## Size

Code bytes, from the `-c -O` object (`nm.py -S`), against the
original's instruction bytes, counted from `main.lst`. The last column
is the C before it was rewritten as plain C on 18 Nov 2026.

| Routine | Original | C | Before |
|---|---|---|---|
| DISPLAY_STATUS_BAR | 52 | 46 | 46 |
| SETUP_TEXT_POS, SETUP_TEXT_POS_COL20 | 51 | 71 (`print_panel_name` 53 and two entries of 9) | 68 |
| DISPLAY_CHAR_STATS | 296 | 332 (308 and `key_or_button` 24) | 323 |
| total | 399 | 449 | 437 |

The C is 50 bytes, 13%, larger.
