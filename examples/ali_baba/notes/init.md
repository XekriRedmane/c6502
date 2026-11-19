# init.c

## What was translated

Every routine UNITS.md gives init, except the three it drops.

| Original | C |
|---|---|
| GAME_INIT, INIT_HRCG | `game_init` (calls `hrcg_init` directly) |
| SELECT_INPUT_MODE | `select_input_mode` |
| INIT_MOB_COUNT | `init_mob_count` |
| INIT_GAME_STATE | `init_game_state` |
| GAME_ACTION_HANDLER, BORDER_HANDLER_A/_B/_C | `border_anim_step` (the handlers are the cases of a switch) |
| `.restart` and TICK_BORDER_ANIM | `start_border_passes` |
| DRAW_BORDER_ICON_LIST | `move_border_icon_list` |
| DRAW_BORDER_ICON_ENTRY | `move_border_icons` |
| DRAW_BORDER_CHAR_AT | `draw_border_cell` |
| ANIM_BORDER_ICONS_A / _B | `anim_border_icons(limit, from, to)` |
| INIT_GAME_DISPLAY | `init_game_display`, with `print_at` |
| DRAW_BORDER_CHAR, DRAW_BORDER_ICONS, PLOT_CHAR | `plot_title_glyphs` over the table `TITLE_GLYPH` / `_COL` / `_ROW` |
| DRAW_BORDER_LABELS | `draw_border_labels`, with `draw_border_strings` |
| INIT_DISK_IOB | `init_disk_iob` |
| INIT_STUB, INIT_STUB2, RELOCATE_HIRES_STAGING | dropped |

Tables: BORDER_POS_TABLE is `border_glyph` (written at run time),
`border_row`, and `border_col`. BORDER_POS_HORIZ, _VERT_L, and _VERT_R
are `BORDER_LABEL_COL` and `BORDER_LABEL_ROW`. UI_TEXT_STREAM,
BORDER_STR_HORIZ, _VERT_L, _VERT_R, S_COPYRIGHT, S_PRESS_SPACE_BOOT, and
S_DECOR_ROW1-3 keep their names. Each initial byte was checked against
main.bin. BORDER_INITIAL_CHAR, BORDER_ANIM_INDEX, BORDER_ANIM_SAVE,
BORDER_ICON_CHARS (with BORDER_ICON_OFFSET, BORDER_LOOP_CTR, and
BORDER_SWAP_FLAG), BORDER_HANDLER_TABLE, ICON_TABLE_PTR, and
DISPATCH_VEC are statics, locals, and the switch. `BORDER_ANIM_ON` is
defined here, initially true.

## Departures

The file's header lists them. In short:

- No relocation, and no calls of the two RTS stubs.
- `game_init` calls `hrcg_init` in place of INIT_HRCG, since hrcg.c
  translates the HRCG as INIT_HRCG patches it.
- The border animation is a state machine that `tune_player` calls
  after each note while `BORDER_ANIM_ON` is true. `border_next` holds
  the handler that the original keeps in DISPATCH_VEC, and
  `border_anim_step` returns where the original jumps to TUNE_ADVANCE.
  `init_game_state` clears `BORDER_ANIM_ON` in place of the three
  stores that write JMP TUNE_ADVANCE at `$7A49`.
- The border's records are three arrays indexed by cell, and the
  handlers name cell ranges where the original passes pointers into
  BORDER_POS_TABLE. The nine glyphs that DRAW_BORDER_CHAR and
  DRAW_BORDER_ICONS draw from immediate operands are a table.
- `render_font_char_direct` gets 0 as its font selector from the
  border code. The selector is read only for glyphs of 24 and up, and
  the border holds only glyphs $00-$15: the initial $05, $0B, $0E, $0F,
  and $14, the 0 of an empty cell, and the 5-$15 that TICK_BORDER_ANIM
  stores. The draws therefore use the `$B1` that `init_game_state`
  stores in CHAR_CHARSET, as the original's do.

Behavior kept exactly:

- The order of the soft-switch accesses. SELECT_INPUT_MODE's loop reads
  BUTN[0] once, then writes KBDSTRB and reads KBD once per pass and
  BUTN[0] once per pass with no key, as the original does. The value
  the original writes to KBDSTRB (whatever is in A) differs from the
  C's 0, and the harness doesn't record written values.
- INIT_MOB_COUNT's carry. The original adds 9 to CHAR_PTR with the
  carry out of the sum into TOTAL_MOB_COUNT, so a total past 255 would
  advance the pointer by 10. `init_mob_count` does the same (11 bytes).
  The total can't pass 128 with consistent records, since there are
  128 mobs, so this could be dropped to save the bytes.
- `print_at(S_DECOR_ROW2, 0x1E, TEXT_ROW + 1)` keeps the original's
  `INC TEXT_ROW`, which reads the row that the previous print left.
- The order of `random_in_range` calls and draws in the animation, and
  the four passes per cycle: a cycle starts by ticking the glyph in
  cell 9, the end of the second pass runs ANIM_BORDER_ICONS_B, the end
  of the third runs ANIM_BORDER_ICONS_A, and the end of the fourth
  starts the next cycle.

## Where the original's comments and code disagree

The C follows the code in each case.

- INIT_GAME_STATE calls `$B1` a "LDA (zp),Y opcode". It is the font
  selector '1' in s_PRINT_FONT_CHAR. It also says "tune 6", where X = 6
  is tune 3 (TUNE_ATTRACT).
- SELECT_INPUT_MODE calls `$C061` paddle button 1. It is button 0.
- DRAW_BORDER_CHAR_AT and ANIM_BORDER_ICONS label the record's byte 1
  "column" and byte 2 "row", and store byte 1 in TEXT_ROW and byte 2 in
  TEXT_COL. BORDER_POS_TABLE's comments ("Bottom border (row $13,
  columns $09-$12)") read the bytes the same wrong way round. In C the
  arrays are named for what the code stores them in.
- GAME_ACTION_HANDLER's comments say BORDER_ANIM_SAVE = 1 means "pass
  0" and runs ANIM_BORDER_ICONS_B, and that 0 means "pass 1" and runs
  ANIM_BORDER_ICONS_A, and the header says "3-pass loop". The code
  makes four passes per cycle, as above.
- ANIM_BORDER_ICONS_A's comment says random(2) picks "table A or B".
  RANDOM_IN_RANGE(2) returns 0-2, and 2 skips the move. B's limit of 4
  skips it for 2-4.

## Header changes needed

None.

## Compiler gaps

1. **Comparing an `int8_t` with 0 after another comparison sign-extends
   it.**

   ```c
   #include <stdint.h>
   void g(void);
   void h(void);
   int8_t n;
   void f(void)
   {
       if (--n < 0)
           return;
       if (n == 1)
           g();
       else if (n == 0)
           h();
   }
   ```

   compiles the `n == 0` test to

   ```
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
   .sx_neg@2:
   LDA   #$FF
   .sx_done@3:
   ORA   n
   BNE   .if_end@3
   ```

   where `BNE` alone would do, since A already holds `n` after the
   `CMP #$01`. It tests both bytes of the promoted `int`. init.c keeps
   `border_pass` in a `uint8_t`, which avoids it.

2. **The carry out of an 8-bit sum is computed with a second add.**
   `uint16_t sum = TOTAL_MOB_COUNT + CHAR_PTR->pc_count;` followed by
   `TOTAL_MOB_COUNT = (uint8_t)sum;` and an add of `(uint8_t)(sum >> 8)`
   to a pointer compiled to two complete additions (one to get the
   carry with `LDA #0; ADC #0`, one for the low byte) and a 16-bit add
   of the carry, 80 bytes for INIT_MOB_COUNT. Adding a `bool carry =
   TOTAL_MOB_COUNT < count` to the pointer is no better, since the bool
   is made with `CLC; SBC; LDA #0; ROL` and added in 16 bits (73 bytes).
   init.c tests `TOTAL_MOB_COUNT < count` in an `if`, which compiles to
   a `CMP` and `BCS` and brings the routine to 62 bytes. The original's
   45 bytes reuse the carry flag directly in the pointer's `ADC #9`.

3. **A `bool` from a comparison is materialized before it's tested.**
   An earlier version of `border_anim_step` began with `bool restart =
   border_next == BORDER_RESTART; if (!restart) ...`, which compiled to
   `CMP #3; BEQ; LDA #0; BEQ; LDA #1; STA restart; BEQ; JMP`. It's
   restructured now.

4. **A post-decrement in a loop's condition goes through a temporary.**

   ```c
   #include <stdint.h>
   void g(uint8_t i);
   void f(uint8_t first, uint8_t last) {
       uint8_t i = last;
       do g(i); while (i-- != first);
   }
   ```

   compiles the step and the test to `LDA i; SEC; SBC #1; STA t; LDA
   i; CMP first; BNE more; RTS; more: LDA t; STA i; JMP loop`, 21
   bytes. The same loop written `for (uint8_t i = last;; i--) { g(i);
   if (i == first) break; }` compiles to `LDA i; CMP first; BNE dec;
   RTS; dec: DEC i` and a `JMP` into the loop at the start, 12 bytes.
   `draw_border_strings` (35 to 44 bytes) and `move_border_icons` (64
   to 70) pay it.

   Fixed on 19 Nov 2026. The step now goes in place of its copy on the
   loop's back edge, where it's `DEC i`
   (docs/optimizations/step_on_edge.md). `draw_border_strings` is 35
   bytes and `move_border_icons` 61.

5. **A loop over a fixed-address array by a 16-bit index adds the
   index to the base each time.**

   ```c
   #include <stdint.h>
   extern uint8_t HIRES_PAGE_1 [[c6502::address(0x2000)]][0x2000];
   void a(void) {
       for (uint16_t i = 0; i < sizeof HIRES_PAGE_1; i++)
           HIRES_PAGE_1[i] = 0;
   }
   ```

   compiles to a 16-bit add of `i` to `#<HIRES_PAGE_1` and
   `#>HIRES_PAGE_1` into a pointer, `LDA #0; LDY #0; STA (p),Y`, a
   16-bit increment of `i`, and a 16-bit compare, 43 bytes. The loop
   the C had before, a pointer stepped a page at a time and a byte
   index, took 32. A loop that keeps the byte in Y and steps the
   pointer's high byte (`LDA #0; TAY; loop: STA (p),Y; INY; BNE loop;
   INC p+1; LDX p+1; CPX #$40; BNE loop`, after storing $2000 in `p`)
   takes about 25, counted by hand. `clear_hires_page1` pays 11 bytes
   over the old C.

6. **A test of bit 7 written as a comparison with $80 is a `CMP`.**
   `(key = KBD) < 0x80` compiles to `LDA KBD; CMP #$80; BCS`, where
   `LDA KBD; BMI` does, and `(BUTN[0] & 0x80) != button`, with `button`
   already masked, compiles to `AND #$80; CMP button; BEQ`, where the
   old `(BUTN[0] ^ button) & 0x80` gave `EOR button; BPL`. Together they
   cost `select_input_mode` 4 bytes.

## Size

Bytes of code in the `-c -O` object (`nm.py -S`) against main.lst's
bytes for the same routines (`__zp_init`, 11 bytes, is the linker's and
not counted). The last column is the C before it was rewritten as plain
C on 18 Nov 2026.

| Original | Bytes | C | Bytes | Before |
|---|---|---|---|---|
| SELECT_INPUT_MODE | 54 | `select_input_mode` | 56 | 52 |
| INIT_MOB_COUNT | 45 | `init_mob_count` | 62 | 63 |
| INIT_GAME_STATE | 42 | `init_game_state` | 30 | 30 |
| GAME_ACTION_HANDLER through ANIM_BORDER_ICONS_B | 323 | `border_anim_step`, `start_border_passes`, `move_border_icon_list`, `move_border_icons`, `draw_border_cell`, `anim_border_icons` | 293 | 295 |
| INIT_GAME_DISPLAY, DRAW_BORDER_CHAR, DRAW_BORDER_LABELS, DRAW_BORDER_ICONS, PLOT_CHAR | 338 | `init_game_display` (and its outlined run), `print_at`, `plot_title_glyphs`, `draw_border_labels`, `draw_border_strings` | 287 | 275 |
| INIT_DISK_IOB | 9 | `init_disk_iob` | 9 | 9 |
| GAME_INIT, INIT_HRCG | 101 | `game_init`, `clear_hires_page1` | 121 | 112 |
| total | 912 | | 858 | 836 |

The rewrite made `border_anim_step` keep the next handler in
`border_next` alone, which took 8 bytes off it and
`start_border_passes`. The loops that count down with `while (i-- !=
first)` and the clear of hi-res page 1 by a 16-bit index cost 9, 6, and
11 bytes (compiler gaps 4 and 5).

RELOCATE_HIRES_STAGING (51), INIT_STUB and INIT_STUB2 (1 each) aren't
translated. INIT_WORLD (7) moved to input.c.

Tables: the original's `$5DBF-$5F18` is 346 bytes. The C has 315
bytes of `.rodata` and 47 of `.data` (`border_glyph` and
`BORDER_ANIM_ON`), 362 in all, which include the 27 bytes of
`TITLE_GLYPH*` that were immediate operands in the original and leave
out the 12 bytes of variables and BORDER_HANDLER_TABLE. Three statics
(`border_anim_index`, `border_next`, `border_pass`) go wherever the
compiler puts them, and `border_anim_index`'s initial 6 is stored by
`__zp_init` when `-O` promotes it to zero page.

## For the integrator

- `border_next` is 0 (handler A) until `init_game_state` sets it to
  `BORDER_RESTART`, as the original's DISPATCH_VEC holds whatever is at
  `$F6/$F7` until INIT_GAME_STATE sets it. Only the title tune, which
  plays after that, calls `border_anim_step`. A call before it would
  run handler A and then start the passes, where the original would
  jump through whatever DISPATCH_VEC held.
- `BORDER_ANIM_ON` is never set true again, as the original never
  restores JMP (DISPATCH_VEC) at `$7A49`.
- The harness has to run `__zp_init` before `main` for
  `border_anim_index`.
