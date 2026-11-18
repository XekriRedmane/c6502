# input.c

## What it translates

Every routine UNITS.md gives the unit, from `main.asm` lines 1626-1641,
10246-11198, 12117-12154, 12213-12484, 12488-12596, 14177-14361, and
17001-17039, and the tables and variables at lines 15318-15375 and
15386-15443.

| Original | C |
|---|---|
| RETIRE_CHARACTER $09E7 | `retire_character()` |
| INPUT_LOOP $5F19 | `input_loop(m)` |
| SETUP_INPUT $5F60 | `setup_input(m)` |
| DRAW_INPUT_OPTIONS $5F70 | static `draw_input_options()` |
| CALC_RECORD_PTR $5F8C | static `option(n)` |
| SETUP_CURSOR_EXT $5FA2, SETUP_CURSOR $5FA5 | `set_inverse_video()` followed by the static `draw_option(o)`, and `draw_option(o)` |
| SET_CURSOR_POS $5FBC | static `set_cursor_pos(pos)` |
| VALIDATE_INPUT $5FDD, READ_INPUT_CHAR $600A | static `validate_input(dir, o)` |
| CALL_INPUT_INIT $6014 | static `call_input_init(init)`, a switch on MENU_INIT_ |
| CLEAR_INPUT_BUFFER $6024 | static `clear_input_buffer()` |
| RENDER_INPUT_BUFFER $602F | static `render_input_buffer()` |
| COPY_RECORD_DATA $604C | static `copy_record_data(o)` |
| STORE_SELECTION_TO_RECORD $6071 | static `store_selection_to_record()` |
| TEARDOWN_INPUT $60C7 | `teardown_input()` |
| GET_INPUT_TABLE_PTR $60CD | `get_input_table_ptr()` |
| CONFIGURE_INPUT $60D2 | `configure_input()` |
| MENU_SEE_YOUR_LOT $6101 | `menu_see_your_lot()` |
| MENU_CHECK_ATTACK $6107, ENTER_ATTACK_MENU $6145 | `menu_check_attack()` |
| MENU_OTHER_OPTIONS $618C | `menu_other_options()` |
| MENU_GAME_SETTINGS $61F1, MENU_GAME_SETTINGS_ENTER $61F6 | `menu_game_settings()`, `menu_game_settings_enter()` |
| MENU_SETTINGS_INIT $61FD | static `menu_settings_init()` |
| SETTINGS_SPEED_UP/DOWN, SETTINGS_DANGER_UP/DOWN $6209-$623E | `settings_speed_up()` and the other three |
| MENU_ATTACK_TARGET $6261 | `menu_attack_target()` |
| MENU_ATTACK_INIT $6268 | the MENU_INIT_ATTACK case of `call_input_init` |
| MENU_DROP_GOLD $62A2 | `menu_drop_gold()` |
| MENU_DROP_GOLD_INIT $62A9 | the MENU_INIT_DROP_GOLD case of `call_input_init` |
| MENU_DROP_INC $62AD, MENU_DROP_DEC $62B3 | `menu_drop_inc()`, `menu_drop_dec()` |
| INIT_WORLD $62E5 | `init_world()` |
| SAVE_LOAD_INIT $62EC | the MENU_INIT_SAVE_LOAD case of `call_input_init` |
| SLOT_INC $62F0, SLOT_DEC $6301, SLOT_UPDATE $6309 | static `slot_inc()`, `slot_dec()`, `slot_update()` |
| SLOT_INC_ENTER, SLOT_DEC_ENTER $630F, $6315 | `slot_inc_enter()`, `slot_dec_enter()` |
| FORMAT_SLOT_NUMBER $631B | static `format_slot_number()` |
| ENTER_SAVE_GAME_SLOT $636C, SLOT_DEC/INC_REENTER $6373, $6379 | `enter_save_game_slot()`, `slot_dec_reenter()`, `slot_inc_reenter()` |
| ENTER_SCENARIO_DISK $63A1 | `enter_scenario_disk()` |
| ENTER_CHAR_SELECT $63E8 | `enter_char_select()` |
| CHAR_SELECT_INIT $63EF | `char_select_init()` |
| CHAR_SELECT_NEXT/PREV $63FF, $6408 | `char_select_next()`, `char_select_prev()` |
| CHAR_SELECT_HERE $6411, CHAR_SELECT_HOME $641A | `char_select_here()`, `char_select_home()`, and the static `char_select_patch(home, here)` for their shared tail |
| START_GAME_INPUT $6458 | `start_game_input()` |
| CONTINUE_GAME $69C8 | `continue_game()` |
| SAVE_GAME $6A47 and its tail after SCENARIO_INIT_FLAG | `save_game()` |
| CANCEL_SCENARIO $6A6E | `cancel_scenario()` |
| SCENARIO_INSERT_PROMPT $6A74, FORMAT_SCENARIO_DISK $6A7F | `format_scenario_disk()` |
| USE_SCENARIO_DISK $6AA1 | `use_scenario_disk()` |
| the `.cleanup` tail they share | static `scenario_done()` |
| DISPLAY_INSERT_MSG $6AB7 | static `display_insert_msg()` |
| ENSURE_GAME_DISK $6ABF | static `ensure_game_disk()` |
| PROMPT_INSERT_DISK $6ACB, WAIT_KEYPRESS $6AD3 | static `prompt_insert_disk()`, `wait_keypress()` |
| FORMAT_DISK_TRACK $6AE1 | static `format_disk_track()` |
| WRITE_DISK_SIGNATURE $6AE8 | static `write_disk_signature()` |
| VERIFY_DISK_SIGNATURE $6B06 | static `verify_disk_signature()` |
| SETUP_DATA_IOB $6B27 | static `setup_data_iob()` |
| VERIFY_GAME_DISK $6B3C | static `verify_game_disk()` |
| WRITE_GAME_DATA $6B72, READ_GAME_DATA $6B95 | static `write_game_data()`, `read_game_data()`, both through `transfer_game_data(command)` |
| SETUP_SLOT_IOB $6BBF | static `setup_slot_iob()` |
| ADVANCE_SECTOR $6BEE | static `advance_sector()` |
| CHECK_COMBAT_ELIGIBILITY $750C | static `check_combat_eligibility()` |
| CHECK_INPUT $755E | static `check_input()` and `check_joystick()` |
| ROM PREAD, which CHECK_INPUT calls | static `read_paddle(n)` |
| JOY_DELAY $75F7 | `WAIT(JOY_SETTLE_DELAY)` in `check_joystick` |
| JOY_TO_ZONE $7606 | static `joy_to_zone(old_zone, reading)` |
| READ_KEYBOARD $A44C | `read_keyboard()` |

Tables and variables:

| Original | C |
|---|---|
| the 11 menus, $607D-$6457 | the header's `struct menu`s and their static options arrays, from `turninput/c_tables.txt` |
| the menu strings, $7ADC-$7D82 | the header's eight strings, and 32 static ones. S_SPACE ($7B64), which no menu names, is left out. |
| DIR_KEYS_UPPER, _LOWER, _CTRL | `DIR_KEYS[3][5]` |
| JOY_AXIS_X, JOY_AXIS_Y | `joy_zone_x`, `joy_zone_y` |
| JOY_THRESH_LO, JOY_THRESH_HI | `ZONE_THRESH_PLUS`, `ZONE_THRESH_MINUS` |
| JOY_SETTLE_DELAY, MAX_GAME_SPEED, GAME_DISK_SLOTS, MAX_SLOT_NUMBER, SAVE_SECTOR_COUNT | `#define`s |
| SLOT_NUMBER | `slot_number` |
| SAVE_TRACK_BASE, DISK_SIGNATURE | `static const` arrays |
| GAME_SIG_TRACK, GAME_SIG_SECTOR | the header's `const uint8_t`s, defined here |
| SCENARIO_INIT_FLAG | `scenario_init_flag` |
| UNDER_LEVEL, SAFE_TO_REST ($5B25, $5B26) | `under_level`, `safe_to_rest` |
| INPUT_SCAN_OFFSET ($5B24) | validate_input's parameter `dir` |

SHUFFLE_ENTITIES and SHUFFLE_COUNT aren't translated, because nothing
calls the routine or names it in a table. INPUT_JMP_VEC is gone with
the jumps through it. Every other routine is translated, and none is
left incomplete.

## Departures

The file's opening comment lists them. In short:

- The menus' exits are CHAIN_ states, which `input_loop` returns, and
  their init routines are MENU_INIT_ numbers, which `call_input_init`
  switches on.
- A jump to a chain routine of this file that only runs a menu, and
  the branches between CHAR_SELECT_INIT, CHAR_SELECT_HOME, and
  CHAR_SELECT_HERE, are calls. They lead nowhere that could call back.
  Every jump to another unit's chain routine is `return CHAIN_X;`.
- `read_paddle` stands in for ROM PREAD (compiler gap 5 of UNITS.md).
  It makes PREAD's accesses, a read of PTRIG and then reads of PADDL0
  or PADDL1 until bit 7 clears, and counts up to 255 in Y, but its loop
  takes 14 cycles a pass to PREAD's 11. On a real Apple II its readings
  come out at about 11/14 of PREAD's, so the stick has to go further
  before the zone changes. In the harness, whose paddles never time
  out, both count to 255. The harness hooks PREAD in the original
  without recording its accesses, so in joystick mode the C makes one
  PTRIG read and 256 PADDL reads per paddle that the original's events
  don't show. UNITS.md's "For the harness" describes this.
- `wait_keypress` stores 0 to KBDSTRB where the original stores
  whatever DISPLAY_MESSAGE left in A. The harness records the write
  and not its value.
- WRITE_GAME_DATA and READ_GAME_DATA are one loop that takes the RWTS
  command. The original's DISK_READ and DISK_WRITE are RWTS_DISPATCH
  with RWTS_READ and RWTS_WRITE, so the calls and the IOB are the same.
- The disk checks return the carry as a bool, true for a failure.
- `configure_input` returns the MOVE option's enable byte, as the
  header specifies.
- MENU_OPTION_PTR ($C2) isn't written. The option pointers are locals.
  Nothing outside these routines reads $C2.
- `advance_sector` writes only the buffer address's high byte, the
  sector, and the track, as ADVANCE_SECTOR does, and writes the sector
  once. `setup_data_iob` and `setup_slot_iob` write the IOB's bytes in
  the original's order.

## Where the original's comments and code disagree

The C follows the code in each case.

- READ_KEYBOARD's header says it returns "key code (high bit set)". It
  returns a direction, 0-4, with bit 7 clear.
- CHECK_INPUT's header says a key that isn't a direction key is
  returned as it is. Such a key clears the strobe and returns $FF.
- JOY_THRESH_LO and JOY_THRESH_HI are commented "(horiz, vert, vert)".
  They're indexed by the paddle's previous zone, the same for both
  paddles, which gives the zones hysteresis. The comments in CHECK_INPUT
  name zone 2 "Y high" and zone 0 "Y low", where zone 2 is the low
  reading, the stick pushed up or left.
- FORMAT_SCENARIO_DISK's comments on its first two branches are
  reversed. VERIFY_GAME_DISK's carry is clear when the game disk is in
  the drive, which leads to the prompt for the scenario disk, and
  VERIFY_DISK_SIGNATURE's carry is clear when the signature matches,
  which leads straight to the write. FORMAT_DISK_TRACK formats the
  whole disk, since RWTS's command 4 does, whatever its name and its
  comment ("format track") say.
- GAME_INPUT_TABLE's comments call byte 1 "max_range" and say
  CONFIGURE_INPUT patches it to `UNDER_LEVEL*3+3`. It is the option
  highlighted first, and CONFIGURE_INPUT stores `UNDER_LEVEL + 3`.

## Header and UNITS.md

No change to `ali_baba.h` is needed. MENU_OPTION_PTR ($C2) is declared,
and no unit uses it now. It could be removed, or kept as a reservation
of $C2-$C3.

## Compiler gaps

Item 2 was fixed on 18 Nov 2026, and so was item 3 for a read. A store
still computes the address.

Each of these makes correct code that is larger or slower than it
needs to be. None blocked the translation.

1. **A volatile read through an index is stored to a dead local.**

   ```c
   extern volatile uint8_t PADDL [[c6502::address(0xC064)]][2];
   static uint8_t read_paddle(uint8_t n [[c6502::in_x]]) {
       register uint8_t count = 0;
       while (PADDL[n] & 0x80)
           if (++count == 0) return 0xFF;
       return count;
   }
   ```

   compiles the loop to `LDA PADDL,X; STA __local_read_paddle__0;
   BPL; INY; BNE`. Nothing reads the local. Without the store the loop
   would be PREAD's own, 11 cycles a pass, and `read_paddle` would read
   the paddle exactly as PREAD does. A read of `KBD` with no index
   (`while (!(KBD & 0x80)) {}`) compiles to `LDA KBD; BPL` without the
   store.

2. **Two masked byte fields are compared as ints.**

   ```c
   if ((E->life & 0x3F) >= (E->strength & 0x1F)) r = 1;
   ```

   compiles to a 16-bit signed comparison (`SEC; SBC; LDA #0; SBC #0;
   BVC; EOR #$80; BMI`), although both sides are 0-63. With the two
   values in `uint8_t` locals it is `CMP; BCC`.
   `check_combat_eligibility` uses the locals.

3. **A pointer member read or written through a zero-page pointer
   goes through a computed address and DPTR.**

   ```c
   struct iob { uint8_t pad[8]; uint8_t *buffer; };
   extern struct iob *P [[c6502::zeropage(0x08)]];
   uint8_t *get(void) { return P->buffer; }
   void put(void) { P->buffer = BUF; }
   ```

   Each adds 8 to P into a pair of locals, copies the pair to DPTR,
   and reads or writes the low byte through the locals and the high
   byte through DPTR, 18 instructions in all. `LDY #8; LDA (P),Y; INY;
   LDA (P),Y` is enough, as it is for a byte member. This costs about
   20 bytes in each of `setup_data_iob`, `setup_slot_iob`, and
   `option` (which reads `MENU_DESC_PTR->options`).

4. **Indexing an array of 9-byte structs multiplies in 16 bits.**
   `&MENU_DESC_PTR->options[n - 1]`, with `n` a `uint8_t`, extends
   `n - 1` to 16 bits and multiplies by 9 with 16-bit shifts and adds,
   about 30 bytes. `option` computes the offset as a `uint8_t`, which
   is correct because no menu has more than 9 options, and the
   multiplication is then `ASL; ASL; ASL; ADC`.

## Size

Code bytes, from the `-c -O` object's sections, against the original's
instruction bytes for the same routines, counted from `main.lst`
(SHUFFLE_ENTITIES and INIT_STUB2 left out):

| | Original | C |
|---|---|---|
| code | 1572 | 1885, of which `__zp_init` is 15 |
| menus | 593 | 505 (11 descriptors of 5 bytes and 50 options of 9) |
| menu strings | 679 | 677 |
| other tables and variables | 39 | 34, of which `-O` moves the 5 variables to zero page |

The code is 313 bytes, 20%, larger than the original's. The largest
differences are the menu driver (`option`, `draw_option`, and
`input_loop`, which pass option pointers in zero-page parameters where
the original keeps one in $C2), the IOB setup (gap 3), and
`check_joystick`, whose zone tests compile to separate loads and
compares where the original keeps the zone in A.

UNITS.md and PLAN.md give the unit 1414 code bytes. The count above is
every instruction in the routines UNITS.md lists for input, which also
includes the instructions inside the table chunks (ENTER_ATTACK_MENU,
MENU_ATTACK_TARGET, and the others).
