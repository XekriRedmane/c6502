# The units of the Ali Baba port

This file divides `output/main.asm` of
https://github.com/XekriRedmane/ali_baba_reveng among fourteen C
files, and settles everything one file's translator needs to know
about the others. `ali_baba.h` declares what the units share, and
`ali_baba.ld` places them. The translator of a unit reads this file's
section on the unit, the header, and the original's code for each
routine listed, and needs nothing from the other translators.

In the routine tables, the size runs from the label to the next entry
label or the end of its chunk, so a routine with several entries is
split across rows. The scope is "header" when another unit calls the
routine, jumps to it, names it in a table, or reaches it through the
chain, and the header then has its prototype. "static" routines are
private to the unit. "dropped" routines aren't translated, and the
notes say why. A routine marked "chain" returns the CHAIN_ state that
runs next.

## Rules for every unit

- Each `.c` file opens with a comment that says what it does, lists
  its departures from the original, and quotes the lines of
  `main.asm` it translates, as `examples/lode_runner/*.c` do. Compile
  it with `uv run python ../../compile.py <unit>.c -c -O` from
  `examples/ali_baba`, and write its listing with `-S -O -o
  <unit>.asm`.
- A chain routine translates its exits this way. A JMP, branch, or
  fall into another chain routine is `return CHAIN_X;`. `LDX #>T; LDY
  #<T; JMP INPUT_LOOP` is `return input_loop(&T);`. A JMP to an
  ordinary routine is a call followed by `return CHAIN_RETURN;`. An
  RTS is `return CHAIN_RETURN;`. A chain routine may return another
  chain routine's result directly (`return find_attack_target();`)
  only when the callee can't lead back to the caller through direct
  calls. Nothing a chain routine calls may call `run_chain`.
- Output goes through `hrcg_cout(c)` wherever the original has `JSR
  ROM_COUT1` or `JMP ROM_COUT1`. The original's callers rely on COUT
  preserving A, X, and Y, which a C caller doesn't need.
- The original's scratch bytes become locals, parameters, and return
  values. Those are `$BA-$BF` under all their names (DATA_PTR,
  MOB_DATA_PTR, TARGET_REC, PRINT_STRING_ADDR, NUM_VALUE,
  HANDLER_PTR, DECIMAL_OUT_PTR, EVENT_PTR, ENTITY_REC, CHAR_REC,
  ICON_TABLE_PTR, ...), `$E1-$E3`, `$E6/$E7`, `$F6/$F7` (EFFECT_REC,
  CHAR_INDEX, FACTION_MASK, DISPATCH_VEC), and these bytes of page
  `$5A`, which the header doesn't declare:

  | Address | Name | Unit | Routine it is scratch in |
  |---|---|---|---|
  | $5A11 | RNG_LIMIT | util | RANDOM_IN_RANGE |
  | $5A12 | RNG_MASK | util | RANDOM_IN_RANGE |
  | $5A14 | NEAREST_DIST | ai | FIND_NEAREST_EVENT |
  | $5A1A | PRNG_TEMP | util | STEP_PRNG |
  | $5A2A | REORDER_LINK | entities | REORDER_CHAR |
  | $5A2B | REORDER_SORT_KEY | entities | REORDER_CHAR |
  | $5A2C | GROUP_COUNT_DELTA | entities | REORDER_CHAR's DECR/INCR_GROUP_COUNT, a parameter |
  | $5A58 | TRAP_TYPE | events | PICKUP_TREASURE |
  | $5A60 | RESIDUAL_5A_5A60 | turn | SET_CHAR_STATE |
  | $5A69 | AI_SAVED_POS | ai | EVALUATE_DIRECTION (unneeded in C) |
  | $5A6F | AI_DIRECTION | ai | NPC_MOVE_AI and EVALUATE_DIRECTION, a parameter |
  | $5A70 | AI_TEST_POS | ai | NPC_MOVE_AI and EVALUATE_DIRECTION, a parameter |

  `$5A2D-$5A50`, `$5A95`, `$5AA5`, and `$5AA8` hold leftover
  assembler text. Only the dead SHUFFLE_ENTITIES uses `$5AA5`.
  `SAVED_TARGET_REC` ($5A9A) and `SAVED_DATA_PTR` ($5A9E) were
  written only by SAVE_ZP_CONTEXT, which isn't translated.
- The event handlers that MOVE_LOOP reaches with EVENT_PTR in `$BE`
  take it as a parameter `struct event *ev`: handle_encounter,
  handle_event_slot, pickup_treasure, trigger_scene_event, and
  display_shop. DECISIONS.md wrote the parameter as `uint8_t *ev`,
  and the header gives it the event entry's type, which is the same
  address.
- Several routines reach memory through a null or small pointer, and
  the C does the same through the same pointer. `$00-$05` and
  `$0A-$0F` are reserved for that (header, "Zero page").
- c6502 has no runtime helpers in this repository yet. A
  multiplication by a constant that isn't a sum of a few shifts, a
  division or remainder, and a shift by a variable count each call
  one, and the link then fails (see "Compiler gaps"). Write them as
  the original does, with repeated subtraction, shifts by constants,
  and loops.
- `[[c6502::keep]]` on a `for` keeps a delay loop that the optimizer
  would otherwise drop. A loop that times something says what it
  times in a comment.

## main.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| MAIN_ENTRY | $0800 | 78 | header | `main` |
| PROCESS_MOB_GROUPS | $0884 | 77 | static | `process_mob_groups` |
| SHOW_IDLE_STATUS | $08D1 | 14 | static | `show_idle_status` |
| START_MOB_TURN | $08DF | 34 | static | `start_mob_turn` |
| CHECK_MOB_NEEDS_TURN | $0958 | 44 | static | `check_mob_needs_turn` (bool) |
| TOGGLE_SOUND | $0984 | 40 | header, chain | `toggle_sound` |
| SETTINGS_DISPLAY_INIT | $09AC | 31 | header | `settings_display_init` |
| DEC_THRESHOLD_WRAP | $09CB | 4 | header | `dec_threshold_wrap` |
| INC_THRESHOLD_WRAP | $09CF | 4 | header | `inc_threshold_wrap` |
| DEC_EVENT_THRESHOLD | $09D3 | 9 | static | `dec_event_threshold` |
| INC_EVENT_THRESHOLD | $09DC | 11 | static | `inc_event_threshold` |
| ATTRACT_SCREEN | $09F4 | 35 | static | `attract_screen` |

Main defines no movable tables or variables.

Kept objects it uses: `LOCATIONS` (from `$4009`, ending where byte 1
of the next record is `$FF`, which is `LOCATIONS_END`'s high byte at
`$4238`), `EVENT_THRESHOLD`, `SCENE_EVENT_RESULT`, `TOTAL_MOB_COUNT`,
`CURRENT_PLAYER`, `CURRENT_ROOM`, `CHAR_SPRITE`, `CHAR_AI_MODE`,
`SAVED_AI_MODE` ($5AA6), `ACTIVE_CHAR`, `LOCATION_POS`, `BLINK_COL`,
`ENCOUNTER_RESULT`, `ADJACENT_THREAT`, `PRNG_OUTPUT`, `ENTITY_PTR`,
`CHAR_PTR`, `MOB_PTR`, and input's `S_SOUND_IS_ON` and `S_DANGER`.

Notes:

- `main` is in section `.main`, which the linker script puts at
  `$0800`. It never returns. The original's first instruction is `JSR
  GAME_INIT`.
- `start_mob_turn` is an ordinary function in C. Its RTS at `$08F1`
  becomes a return, and its `JMP GAME_TURN_LOOP` at `$08FE` becomes
  `run_chain(CHAIN_GAME_TURN_LOOP);` followed by a return. The chain's
  RTS comes back to PROCESS_MOB_GROUPS, which then stores `$14` in
  BLINK_COL. START_MOB_TURN stores CHAR_AI_MODE in `$5AA6`
  (`SAVED_AI_MODE`), which nothing reads. Keep the store.
- APPEARANCE_TO_FONTCHAR's two results are A (stored in CHAR_SPRITE)
  and Y (stored in CHAR_AI_MODE). `appearance_to_fontchar` returns
  them as the low and high byte of a `uint16_t`.
- TOGGLE_SOUND tests whether PLAY_HIGH_TONE's first byte is `$60`
  (RTS), which means sound is off. In C it tests `!SOUND_ON`. Sound
  off turns on: `S_SOUND_CHAR1 = 0xCE` ('N'), `S_SOUND_CHAR2 = 0xA0`,
  `SOUND_ON = true`. Sound on turns off: both characters `0xC6` ('F'),
  `SOUND_ON = false`. Both paths return `CHAIN_MENU_GAME_SETTINGS`.
  The `BNE .patch_tones` after `LDA #$60` is always taken.
- SETTINGS_DISPLAY_INIT stores `$20` in `S_DANGER_OUT[2..4]`, then
  `num_to_decimal(EVENT_THRESHOLD, S_DANGER_OUT)` as a tail call.
- DEC/INC_THRESHOLD_WRAP clamp at 0 and at `$63` (MAX_THRESHOLD, 99).
  They don't wrap, whatever the names say. The dossier lists their
  callers as SETTINGS_SPEED_UP at `$6239` and `$6230`, which are
  SETTINGS_DANGER_DOWN and SETTINGS_DANGER_UP.
- PROCESS_MOB_GROUPS calls VERIFY_DISK_IO on a 1-in-24 PRNG test.
  `verify_disk_io` does nothing in C (util), so the test and the call
  may stay or go. The test reads PRNG_OUTPUT without stepping it.
- SHOW_IDLE_STATUS ends with `JMP DISPLAY_MESSAGE`, and ATTRACT_SCREEN
  with `JMP SCENE_LOOP` with scene `$26`. Both are tail calls.

## init.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| SELECT_INPUT_MODE | $084E | 54 | static | `select_input_mode` |
| INIT_MOB_COUNT | $0C51 | 45 | static | `init_mob_count` |
| INIT_GAME_STATE | $5B00 | 42 | static | `init_game_state` |
| GAME_ACTION_HANDLER | $5B2A | 67 | header | `border_anim_step` |
| BORDER_HANDLER_A / _C / _B | $5B6D / $5B7E / $5B8F | 17 / 17 / 12 | static | cases of a switch in `border_anim_step`, or static functions |
| DRAW_BORDER_ICON_LIST | $5B9B | 2 | static | `draw_border_icon_list(table)`, which calls the next with offset `$18` |
| DRAW_BORDER_ICON_ENTRY | $5B9D | 59 | static | `draw_border_icon_entry(table, offset)` |
| DRAW_BORDER_CHAR_AT | $5BD8 | 16 | static | `draw_border_char_at` |
| TICK_BORDER_ANIM | $5BE8 | 38 | static | `tick_border_anim` |
| ANIM_BORDER_ICONS_A | $5C0E | 12 | static | `anim_border_icons(...)`, one body with B's |
| ANIM_BORDER_ICONS_B | $5C1A | 83 | static | the same |
| INIT_GAME_DISPLAY | $5C6D | 127 | static | `init_game_display` |
| DRAW_BORDER_CHAR | $5CEC | 17 | static | `draw_border_char` |
| DRAW_BORDER_LABELS | $5CFD | 125 | static | `draw_border_labels` |
| DRAW_BORDER_ICONS | $5D7A | 54 | static | `draw_border_icons` |
| PLOT_CHAR | $5DB0 | 15 | static | `plot_char(c, col, row)` |
| INIT_STUB2 | $6B94 | 1 | dropped | a bare RTS, whose call is dropped |
| INIT_DISK_IOB | $6C2E | 9 | static | `init_disk_iob` |
| RELOCATE_HIRES_STAGING | $6DA5 | 51 | dropped | see notes |
| GAME_INIT | $78C7 | 82 | header | `game_init` |
| INIT_STUB | $7919 | 1 | dropped | a bare RTS, whose call is dropped |
| INIT_HRCG | $791A | 19 | static | `init_hrcg`, which is `hrcg_init()` |

INIT_WORLD ($62E5) moved to input.c.

Tables and variables init defines (`$5DBF-$5F18`, 346 bytes). Copy the
initial bytes from main.bin at each address, since several are
written at run time and the border animation reads what earlier steps
left.

| Label | Address | Size | Contents and use |
|---|---|---|---|
| BORDER_INITIAL_CHAR | $5DBF | 1 | `$A0` at first. ANIM_BORDER_ICONS stores its maximum index here. |
| BORDER_ANIM_INDEX | $5DC0 | 1 | `$06` at first. TICK_BORDER_ANIM's position, 5-`$15`. |
| BORDER_ANIM_SAVE | $5DC1 | 1 | the pass count, 3 down to 0 |
| BORDER_ICON_CHARS | $5DC2 | 3 | also BORDER_ICON_OFFSET ($5DC2), BORDER_LOOP_CTR ($5DC3), and BORDER_SWAP_FLAG ($5DC4), which are its three bytes used as variables. Use one array or three statics. |
| BORDER_HANDLER_TABLE | $5DC5 | 6 | the addresses of BORDER_HANDLER_A, _B, _C. In C an array of handler numbers 0, 1, 2 (A, B, C), indexed as the original does (4, 2, 0 mean C, B, A). |
| BORDER_POS_TABLE | $5DCB | 138 | records of 3 bytes (glyph, row, column), written by the animation. The handlers use the sub-tables at `$5DCB`, `$5DE9`, `$5E04`, `$5E1F`, and `$5E3A`, which are BORDER_POS_TABLE + 0, `$1E`, `$39`, `$54`, and `$6F`. |
| UI_TEXT_STREAM | $5E55 | 38 | the title screen's text stream, for TEXT_RENDERER. It has no `$7E`. |
| BORDER_POS_HORIZ, _VERT_L, _VERT_R | $5E7B, $5E87, $5E9B | 12, 20, 20 | the border labels' positions |
| BORDER_STR_HORIZ, _VERT_L, _VERT_R | $5EAF, $5EC1, $5EC4 | 18, 3, 3 | the border frame's strings |
| S_COPYRIGHT | $5EC7 | 28 | length-prefixed, high-bit ASCII |
| S_PRESS_SPACE_BOOT | $5EE3 | 28 | the same |
| S_DECOR_ROW1, 2, 3 | $5EFF, $5F07, $5F10 | 8, 9, 9 | the same |
| (new) BORDER_ANIM_ON | | 1 | header `bool`, initially true |

DISPATCH_VEC (`$F6/$F7`) becomes init's private state, the handler
`border_anim_step` runs next, kept as a handler number.

Kept objects it uses: `SOFTEV`, `INPUT_MODE`, `TOTAL_MOB_COUNT`,
`CURRENT_PLAYER`, `FONT_CHARNUM`, `FONT_CHARSET`, `LOCATION_POS`,
`LOCATION_STYLE`, `BLINK_COL`, `ENTITY_PTR`, `CHAR_PTR`, `LOCATIONS`,
`CHAR_CHARSET` ($80A6), `TEXT_COL`/`TEXT_ROW`, `TEXT_STREAM_PTR`,
`RWTS_IOB_PTR`, `RWTS_IOB`, `KBD`, `KBDSTRB`, `BUTN`, and the soft
switches `TXTCLR`, `MIXCLR`, `TXTPAGE1`, `HIRES`.

Notes:

- GAME_INIT in C, in the original's order: `SOFTEV = 0xA56F;
  SETPWRC(); BLINK_COL = 0x14;` (no relocation), then reads of
  `TXTCLR`, `MIXCLR`, `TXTPAGE1`, `HIRES`, then `init_disk_iob();
  init_hrcg(); set_text_window_wrap(); init_game_state(); fill_map();
  set_text_window_bottom();`, then `LOCATION_POS = LOCATION_STYLE =
  0`, the high byte of ENTITY_PTR = 0, `select_input_mode();
  run_chain(CHAIN_INIT_WORLD); init_mob_count(); CURRENT_PLAYER = 1;`.
  Store the constant `0xA56F` in SOFTEV. Taking `cold_restart`'s
  address would make it ineligible for zp_abi, and then it could use
  the soft stack, which it must not (util).
- RELOCATE_HIRES_STAGING copies `$2000-$32FF` to `$9600` and `$B300`.
  The C starts from main.bin, where that has already happened, and
  the source pages hold zeros there, so running it would destroy the
  HRCG and the disk reader.
- INIT_GAME_STATE: `init_game_display(); DISPATCH_VEC =
  GAME_ACTION_HANDLER; INPUT_MODE = 0; CHAR_CHARSET = 0xB1;
  tune_player(TUNE_ATTRACT, 0);` and then `BORDER_ANIM_ON = false;` in
  place of the three stores to `$7A49-$7A4B`. The original's comment
  "LDA (zp),Y opcode" on the `$B1` is wrong: `$B1` is the font
  selector '1'. The bytes `$5B24-$5B27` of this routine's last two
  instructions are also the variables INPUT_SCAN_OFFSET, UNDER_LEVEL,
  SAFE_TO_REST, and TURN_ACTIVE, which input.c and turn.c define in
  C.
- The border animation runs one step per note of the title tune.
  sound.c's `tune_player` calls `border_anim_step()` after each note's
  tone while BORDER_ANIM_ON is true. The original reaches
  GAME_ACTION_HANDLER through `JMP (DISPATCH_VEC)` at `$7A49`, and each
  handler ends with `JMP TUNE_ADVANCE` (a return to the tune player in
  C) after it stores the next handler in DISPATCH_VEC. The first call
  runs `.restart` (BORDER_ANIM_SAVE = 3, TICK_BORDER_ANIM, then the
  handler loop at X = 4), and each later call runs the stored handler
  and then `.check_pass` up to the next `JMP TUNE_ADVANCE`. Keep the
  order of RANDOM_IN_RANGE calls, drawing, and returns exactly.
- RENDER_FONT_CHAR_DIRECT reads its X (the font selector) only when
  FONT_CHARNUM is 24 or more. In DRAW_BORDER_CHAR_AT (`$5BE5`, JMP),
  X is the high byte of the table pointer the handler passed (`$5D` or
  `$5E`). In ANIM_BORDER_ICONS (`$5C55` and `$5C6A`), X is
  BORDER_INITIAL_CHAR (6 or `$15`). In TICK_BORDER_ANIM (`$5BFB`), X is
  whatever reached GAME_ACTION_HANDLER from the tune player. Pass the
  original's value wherever the glyph can be 24 or more, and say in a
  comment why it doesn't matter elsewhere.
- PLOT_CHAR and DRAW_BORDER_CHAR enter entities'
  PRINT_FONTCHAR_AT_TEXT_POS by JMP. INIT_GAME_DISPLAY renders
  UI_TEXT_STREAM with `text_renderer()`.
- The border code at `$5C00-$5CFF` is overwritten by the first scene
  the game loads, as main.asm's comment at GAME_ACTION_HANDLER says.
  It runs only before that, so the C behaves the same.
- SELECT_INPUT_MODE polls the keyboard and button 0 (`$C061`, BUTN[0])
  and ends with `JMP DISPLAY_MESSAGE`.
- INIT_MOB_COUNT adds byte 8 of records 1 on into TOTAL_MOB_COUNT
  until byte 1 is `$FF`, and the carry out of each add goes into the
  pointer's advance, as DECISIONS.md notes. It leaves CHAR_PTR changed.

## turn.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| TURN_DISPATCH | $14A3 | 20 | static, chain | `turn_dispatch` |
| GAME_TURN_LOOP | $19AD | 132 | static, chain | `game_turn_loop` |
| MOVE_LOOP | $1A31 | 289 | static, chain | `move_loop` |
| COMMIT_MOVE | $1B52 | 47 | header | `commit_move` |
| RESET_TURN_POS | $1B81 | 38 | header | `reset_turn_pos` |
| END_TURN | $1BA7 | 31 | static, chain | `end_turn` |
| SET_STATE_FIGHT | $1BC6 | 5 | static | `set_state_fight` |
| CHECK_AND_RESTRICT | $1BCB | 21 | static | `check_and_restrict` |
| SET_CHAR_STATE | $1BE0 | 15 | header | `set_char_state` |
| CLEAR_RESTRICTED | $1BEF | 9 | header | `clear_restricted` |
| SET_RESTRICTED | $1BF8 | 9 | header | `set_restricted` |
| (new) | | | header | `run_chain` |

Variables turn defines: `TURN_ACTIVE` (header), the code byte `$5B27`
of INIT_GAME_STATE in the original, initially `0x4B`, which nothing
reads before turn writes it.

Kept objects it uses: `TURN_START_COL/ROW`, `CURRENT_COL/ROW`,
`MOVE_POINTS`, `STEPS_TAKEN`, `INPUT_DIR`, `TURN_TUNE_GUARD`,
`ENCOUNTER_RESULT`, `CHAR_SPRITE`, `CHAR_AI_MODE`, `FONT_COL/ROW/
CHARNUM/CHARSET`, `BLINK_COL/ROW/CHAR`, `ENTITY_PTR`.

Notes:

- `run_chain` is the dispatcher, a loop with a switch on the state.
  Every chain routine of the header is a case `s = name();`. These
  states name ordinary routines, and each case calls the routine and
  then sets `s = CHAIN_RETURN`: `CHAIN_SET_STATE_FIGHT`
  (set_state_fight), `CHAIN_AUTO_HEAL` (auto_heal),
  `CHAIN_ENTER_COMBAT_STATE` (`enter_combat_state(0x14)`, because the
  menu's `JMP (INPUT_JMP_VEC)` leaves the handler's high byte `$14` in
  A), `CHAIN_RESOLVE_ATTACK` (resolve_attack), and
  `CHAIN_RETIRE_CHARACTER` (retire_character).
- GAME_TURN_LOOP falls into MOVE_LOOP on every path, so it ends
  `return CHAIN_MOVE_LOOP;` or `return move_loop();`.
- MOVE_LOOP's exits, from report_turn's table, are below. An AI's
  commands 6, 7, 8, and 10 and up first run CHECK_AND_RESTRICT, with A
  kept across it by PHA/PLA.

  | Case | C in move_loop |
  |---|---|
  | out of points, or CHECK_CAN_LEAVE says no | `return CHAIN_END_TURN;` |
  | a player with MOVE disabled, or the fire key | `return CHAIN_TEARDOWN_INPUT;` |
  | AI command 5, or a move to an empty cell | loop back to the top |
  | AI command 9 | `set_restricted(); set_char_state(0); auto_heal(); return CHAIN_RETURN;` |
  | AI command 7 | `enter_combat_state(7); return CHAIN_RETURN;` |
  | AI command 6 | `start_combat(); return CHAIN_RETURN;` |
  | AI command 8 or 10 and up | `set_state_fight(); return CHAIN_RETURN;` |
  | a blocked cell (an edge) | `tune_player(TUNE_ENTITY_INTERACTION, 0); show_locked_msg(); return CHAIN_RETURN;` (X = `$0E`) |
  | a mob | `return CHAIN_HANDLE_MOB_ENCOUNTER;` |
  | an event, (type & `$C0`) != `$C0` | `handle_encounter(ev); return CHAIN_RETURN;` |
  | an event, type `$C0-$D1` | `pickup_treasure(ev); return CHAIN_RETURN;` |
  | an event, type `$D2-$D4` | `trigger_scene_event(ev); return CHAIN_RETURN;` |
  | an event, type `$D5-$DA` | `commit_move(); return display_shop(ev);` |
  | an event, type `$FE` | `return handle_event_slot(ev);` |
  | an event, type `$DB-$FD` or `$FF` | `return CHAIN_TEARDOWN_INPUT;` |

  MOVE_LOOP keeps `$BE` across COMMIT_MOVE with PSHW/PULW, which `ev`,
  a local, covers.
- An AI's command 0 moves up, because the up test is `CMP #2; BPL`.
- MOVE_LOOP reads the MOVE option's enable byte, GAME_INPUT_TABLE+4,
  right after CONFIGURE_INPUT. In C that byte is
  `configure_input()`'s return value.
- END_TURN computes `(entity[12] >> 1) & 7` (a ROR whose carry the AND
  drops) and compares it with STEPS_TAKEN by `CMP; BMI`, a signed
  test. Below goes into SET_STATE_FIGHT and ends the chain. The
  header comment in main.asm states the comparison the other way
  round.
- COMMIT_MOVE falls into RESET_TURN_POS, so `commit_move()` ends with
  `reset_turn_pos()`. RESET_TURN_POS plays tune `$16 / 2 = 11`
  (TUNE_MOVEMENT) when TURN_TUNE_GUARD is `$16`, and decrements the
  guard first. COMMIT_MOVE stores `$16` in it, so the tune plays once
  per commit. The original's `JMP TUNE_PLAYER` loads X from
  TURN_TUNE_GUARD before the decrement.
- SET_CHAR_STATE's `$5A60` is a local.

## input.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| RETIRE_CHARACTER | $09E7 | 13 | header | `retire_character` |
| INPUT_LOOP | $5F19 | 69 | header | `input_loop` |
| SETUP_INPUT | $5F60 | 16 | header | `setup_input` |
| DRAW_INPUT_OPTIONS | $5F70 | 28 | static | `draw_input_options` |
| CALC_RECORD_PTR | $5F8C | 22 | static | `calc_record_ptr`, or `&m->options[i - 1]` |
| SETUP_CURSOR_EXT | $5FA2 | 3 | static | `setup_cursor_ext`, falls into the next |
| SETUP_CURSOR | $5FA5 | 23 | static | `setup_cursor` |
| SET_CURSOR_POS | $5FBC | 33 | static | `set_cursor_pos` |
| VALIDATE_INPUT | $5FDD | 45 | static | `validate_input` |
| READ_INPUT_CHAR | $600A | 10 | static | `o->next[key - 1]` |
| CALL_INPUT_INIT | $6014 | 16 | static | `call_input_init(init)`, a switch on MENU_INIT_ |
| CLEAR_INPUT_BUFFER | $6024 | 11 | static | `clear_input_buffer` |
| RENDER_INPUT_BUFFER | $602F | 29 | static | `render_input_buffer` |
| COPY_RECORD_DATA | $604C | 37 | static | `copy_record_data` |
| STORE_SELECTION_TO_RECORD | $6071 | 12 | static | `store_selection_to_record` |
| TEARDOWN_INPUT | $60C7 | 6 | header, chain | `teardown_input` |
| GET_INPUT_TABLE_PTR | $60CD | 5 | header | `get_input_table_ptr` |
| CONFIGURE_INPUT | $60D2 | 47 | header | `configure_input` |
| MENU_SEE_YOUR_LOT | $6101 | 6 | header, chain | `menu_see_your_lot` |
| MENU_CHECK_ATTACK | $6107 | 8 | header, chain | `menu_check_attack` |
| ENTER_ATTACK_MENU | $6145 | 7 | static | part of `menu_check_attack` |
| MENU_OTHER_OPTIONS | $618C | 7 | header, chain | `menu_other_options` |
| MENU_GAME_SETTINGS | $61F1 | 5 | header, chain | `menu_game_settings` |
| MENU_GAME_SETTINGS_ENTER | $61F6 | 7 | header, chain | `menu_game_settings_enter` |
| MENU_SETTINGS_INIT | $61FD | 12 | static | `menu_settings_init` |
| SETTINGS_SPEED_UP | $6209 | 23 | header, chain | `settings_speed_up` |
| SETTINGS_SPEED_DOWN | $6220 | 13 | header, chain | `settings_speed_down` |
| SETTINGS_DANGER_UP | $622D | 9 | header, chain | `settings_danger_up` |
| SETTINGS_DANGER_DOWN | $6236 | 9 | header, chain | `settings_danger_down` |
| MENU_ATTACK_TARGET | $6261 | 7 | header, chain | `menu_attack_target` |
| MENU_ATTACK_INIT | $6268 | 4 | static | `menu_attack_init` (its RTS at `$626B` is also the "no init" and "return" exit) |
| MENU_DROP_GOLD | $62A2 | 7 | header, chain | `menu_drop_gold` |
| MENU_DROP_GOLD_INIT | $62A9 | 4 | static | `menu_drop_gold_init` |
| MENU_DROP_INC | $62AD | 6 | header, chain | `menu_drop_inc` |
| MENU_DROP_DEC | $62B3 | 6 | header, chain | `menu_drop_dec` |
| INIT_WORLD | $62E5 | 7 | header, chain | `init_world` (moved here from init) |
| SAVE_LOAD_INIT | $62EC | 4 | static | `save_load_init` |
| SLOT_INC | $62F0 | 17 | static | `slot_inc` |
| SLOT_DEC | $6301 | 8 | static | `slot_dec`, falls into SLOT_UPDATE |
| SLOT_UPDATE | $6309 | 6 | static | `slot_update` |
| SLOT_INC_ENTER / SLOT_DEC_ENTER | $630F / $6315 | 6 / 6 | header, chain | `slot_inc_enter`, `slot_dec_enter` |
| FORMAT_SLOT_NUMBER | $631B | 37 | static | `format_slot_number` |
| ENTER_SAVE_GAME_SLOT | $636C | 7 | header, chain | `enter_save_game_slot` |
| SLOT_DEC_REENTER / SLOT_INC_REENTER | $6373 / $6379 | 6 / 6 | header, chain | `slot_dec_reenter`, `slot_inc_reenter` |
| ENTER_SCENARIO_DISK | $63A1 | 7 | header, chain | `enter_scenario_disk` |
| ENTER_CHAR_SELECT | $63E8 | 7 | header, chain | `enter_char_select` |
| CHAR_SELECT_INIT | $63EF | 16 | header, chain | `char_select_init` |
| CHAR_SELECT_NEXT / PREV | $63FF / $6408 | 9 / 9 | header, chain | `char_select_next`, `char_select_prev` |
| CHAR_SELECT_HERE | $6411 | 9 | header, chain | `char_select_here` |
| CHAR_SELECT_HOME | $641A | 28 | header, chain | `char_select_home`, with a static `char_select_patch(opt6, opt5)` for the shared tail |
| START_GAME_INPUT | $6458 | 7 | header, chain | `start_game_input` |
| CONTINUE_GAME | $69C8 | 53 | header, chain | `continue_game` |
| SHUFFLE_ENTITIES | $69FD | 74 | dropped | no callers and no table holds it |
| SAVE_GAME | $6A47 | 17 | header, chain | `save_game` |
| SCENARIO_INIT_FLAG | $6A58 | 22 | static | a data byte and SAVE_GAME's tail |
| CANCEL_SCENARIO | $6A6E | 6 | header, chain | `cancel_scenario` |
| SCENARIO_INSERT_PROMPT | $6A74 | 11 | static | part of `format_scenario_disk` |
| FORMAT_SCENARIO_DISK | $6A7F | 34 | header, chain | `format_scenario_disk` |
| USE_SCENARIO_DISK | $6AA1 | 22 | header, chain | `use_scenario_disk` |
| DISPLAY_INSERT_MSG | $6AB7 | 8 | static | `display_insert_msg` |
| ENSURE_GAME_DISK | $6ABF | 12 | static | `ensure_game_disk` |
| PROMPT_INSERT_DISK | $6ACB | 8 | static | `prompt_insert_disk`, falls into the next |
| WAIT_KEYPRESS | $6AD3 | 14 | static | `wait_keypress` |
| FORMAT_DISK_TRACK | $6AE1 | 7 | static | `format_disk_track` (bool) |
| WRITE_DISK_SIGNATURE | $6AE8 | 30 | static | `write_disk_signature` (bool) |
| VERIFY_DISK_SIGNATURE | $6B06 | 33 | static | `verify_disk_signature` (bool) |
| SETUP_DATA_IOB | $6B27 | 21 | static | `setup_data_iob` |
| VERIFY_GAME_DISK | $6B3C | 54 | static | `verify_game_disk` (bool) |
| WRITE_GAME_DATA | $6B72 | 34 | static | `write_game_data` |
| READ_GAME_DATA | $6B95 | 42 | static | `read_game_data` |
| SETUP_SLOT_IOB | $6BBF | 47 | static | `setup_slot_iob` |
| ADVANCE_SECTOR | $6BEE | 36 | static | `advance_sector` |
| CHECK_COMBAT_ELIGIBILITY | $750C | 82 | static | `check_combat_eligibility` |
| CHECK_INPUT | $755E | 138 | static | `check_input`: 36 bytes, the key tables, then 102 more |
| JOY_DELAY | $75F7 | 7 | static | `joy_delay` |
| JOY_TO_ZONE | $7606 | 24 | static | `joy_to_zone(old_zone, paddle)` |
| READ_KEYBOARD | $A44C | 44 | header | `read_keyboard` |

Tables and variables input defines:

| Label | Address | Size | Contents | Users |
|---|---|---|---|---|
| the 11 menus | $607D-$6457 | 593 in the original | `turninput/c_tables.txt` has the exact C. The header declares the `struct menu`s, and the options arrays are static. | input; turn reads GAME_INPUT_TABLE's MOVE enable through `configure_input` |
| the menu strings | $7ADC-$7D82 | 679 | `turninput/c_tables.txt`. Length byte, then high-bit ASCII, no terminator. Some start with leftover bytes, which the file keeps. | the header lists the strings other units write; the rest are static |
| DIR_KEYS_UPPER | $7582 | 5 | A0 C9 CD CA CB (space I M J K), indexed by DIR_ | CHECK_INPUT |
| DIR_KEYS_LOWER | $7587 | 5 | 8D E9 ED EA EB (CR i m j k) | CHECK_INPUT |
| DIR_KEYS_CTRL | $758C | 5 | 9B 8B 8A 88 95 (ESC ^K ^J ^H ^U) | CHECK_INPUT |
| JOY_AXIS_X, JOY_AXIS_Y | $75FE, $75FF | 1, 1 | 01, 01, written | CHECK_INPUT |
| JOY_THRESH_LO | $7600 | 3 | 1A 1D 1D | JOY_TO_ZONE |
| JOY_THRESH_HI | $7603 | 3 | 03 03 06 | JOY_TO_ZONE |
| SCENARIO_INIT_FLAG | $6A58 | 1 | 00, written | SAVE_GAME |
| UNDER_LEVEL, SAFE_TO_REST | $5B25, $5B26 | 1, 1 | code bytes of INIT_GAME_STATE, written by CHECK_COMBAT_ELIGIBILITY before CONFIGURE_INPUT reads them | input |
| INPUT_SCAN_OFFSET | $5B24 | 1 | becomes a local of `validate_input` | |
| JOY_SETTLE_DELAY | $7AAE | 1 | C8 | JOY_DELAY |
| MAX_GAME_SPEED | $7ABC | 1 | 05 | SETTINGS_SPEED_UP |
| SLOT_NUMBER | $7AC0 | 1 | 01, written | the slot routines |
| GAME_DISK_SLOTS | $7AC5 | 1 | 01 | CONTINUE_GAME, SAVE_GAME, SETUP_SLOT_IOB |
| MAX_SLOT_NUMBER | $7AC6 | 1 | 11 | SLOT_INC |
| SAVE_TRACK_BASE | $7AC7 | 2 | 13 01 | SETUP_SLOT_IOB |
| SAVE_SECTOR_COUNT | $7AC9 | 1 | 1A | WRITE/READ_GAME_DATA |
| GAME_SIG_TRACK, GAME_SIG_SECTOR | $7ACA, $7ACB | 1, 1 | 11, 00 (header `extern const`) | VERIFY_GAME_DISK |
| DISK_SIGNATURE | $7ACC | 3 | C1 CC C9 ("ALI") | the signature routines |

INPUT_JMP_VEC ($5F5E) and SHUFFLE_COUNT ($7ACF) go away.

Kept objects it uses: `INPUT_BUFFER` ($5AAC, 120 bytes), `CURRENT_SELECTION`,
`INPUT_OPTION_COUNT`, `MENU_DESC_PTR`, `MENU_OPTION_PTR`, `INPUT_MODE`,
`CURRENT_PLAYER`, `ACTIVE_CHAR`, `PRNG_OUTPUT`, `ENCOUNTER_RESULT`,
`ADJACENT_THREAT`, `MOVE_POINTS`, `STEPS_TAKEN`, `ENTITY_PTR`,
`SCENE_TEXT_DATA` (the signature sector), `LOCATIONS`/`MOBS` (the save
buffer `$4000-$59FF`), `RWTS_IOB_PTR`, `TEXT_COL`/`TEXT_ROW`, `KBD`,
`KBDSTRB`, `BUTN`, `PADDL`, `PTRIG`, util's `ANIM_INDEX`, turn's
`TURN_ACTIVE`.

Notes:

- `input_loop(m)` sets MENU_DESC_PTR, draws the menu, loops on
  READ_KEYBOARD, and returns the chosen option's `exit` without
  running it. DECISIONS.md declares it with `const struct menu *`. The
  header drops the `const`, because STORE_SELECTION_TO_RECORD writes
  the descriptor through MENU_DESC_PTR.
- An invalid move plays `tune_player(TUNE_MENU_SELECTION, 0)` (X =
  `$12`).
- READ_KEYBOARD returns DIR_NONE for fire and DIR_UP..DIR_RIGHT, with
  the Z flag of its final PLA. Its steps: write `$FF` to KBDSTRB;
  `anim_tick_and_wait()`; `verify_integrity()` when `(PRNG_OUTPUT &
  0x7E) == 0`; `tick_anim_counter()`; then loop `step_prng();
  r = check_input();` until bit 7 of r is clear, calling
  `idle_anim_handler()` on each miss; write r to KBDSTRB;
  `anim_check_frame()`; return r. Its comment "key code (high bit
  set)" is wrong.
- CHECK_INPUT reads KBD first. In joystick mode it reads both paddles
  with JOY_DELAY (`WAIT(JOY_SETTLE_DELAY)`, which lets the paddle
  timers run out between reads) before each ROM PREAD. PREAD returns
  its count in Y, which C can't read (see "Compiler gaps"), so the C
  reads the paddle itself: a read of PTRIG, then a count while bit 7
  of `PADDL[n]` is set, with the same delay per pass as PREAD's loop
  (`LDA PADDL0,X; BPL; INY; BNE`, 11 cycles a pass, counting from 0
  and stopping at 255). The harness compares soft-switch accesses, so
  the number of reads of PADDL must match what PREAD would make on the
  harness's paddle model. Check that with the harness. JOY_TO_ZONE's
  comment "(horiz, vert, vert)" is wrong.
- CONFIGURE_INPUT patches GAME_INPUT_TABLE: selection =
  UNDER_LEVEL + 3; MOVE enable = MOVE_POINTS, then 0 if TURN_ACTIVE is
  0; ATTACK and DEFEND enables = UNDER_LEVEL ^ 1; STOP enable =
  UNDER_LEVEL; REST enable = SAFE_TO_REST. It returns the MOVE enable
  byte.
- MENU_DROP_INC ('@') goes to ADJ_DROP_INC, which subtracts, and
  MENU_DROP_DEC ('#') to ADJ_DROP_DEC, which adds. Keep the original's
  crossing.
- CHAR_SELECT_HERE's `BEQ .patch` is always taken into
  CHAR_SELECT_HOME's tail, which writes option 6's enable (A), option
  5's enable (X), and selection = 2, then returns
  `CHAIN_ENTER_CHAR_SELECT`.
- FORMAT_SCENARIO_DISK: the source comment on VERIFY_DISK_SIGNATURE's
  carry has the sense reversed. Carry clear means the signature
  matched, which goes to `.write`.
- WAIT_KEYPRESS stores to KBDSTRB, then waits for bit 7 of KBD, and
  doesn't clear the strobe afterwards.
- READ_GAME_DATA's final failure prints message `$3B` and jumps to
  COLD_RESTART, which is `cold_restart()` (noreturn).
- FIND_PREV_PC (util) used INPUT_BUFFER[0..1] as scratch. In C those
  are util's locals, so INPUT_BUFFER is input's alone.
- VALIDATE_INPUT steps over disabled options in the same direction
  with nothing to stop a cycle of them, as in the original.

## combat.c

combat has no full report. DECISIONS.md gives its prototypes and its
statics, and the translator reads main.asm for the rest.

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| CHECK_ENCOUNTER | $0E25 | 71 | header | `check_encounter` |
| FIND_MOB_AT_SAME_POS | $0E6C | 69 | static | `find_mob_at_same_pos` |
| CHECK_HOSTILE | $0EB1 | 50 | header | `check_hostile` |
| GET_HOSTILITY | $0EE3 | 14 | static | `get_hostility` |
| CHECK_ADJACENT_THREATS | $0EF1 | 88 | header | `check_adjacent_threats` |
| SUM_HOSTILE_AT_POS | $0F49 | 59 | static | `sum_hostile_at_pos(pos, ref)` |
| APPLY_DAMAGE | $106F | 211 | static | `apply_damage(rec, damage)` |
| MAP_VALUE_TO_STRING | $11FC | 53 | static | `map_value_to_string` |
| PICK_RANDOM_MEMBER | $12D0 | 42 | header | `pick_random_member` |
| APPLY_DAMAGE_TO_CURRENT | $1425 | 13 | header | `apply_damage_to_current` |
| APPLY_DAMAGE_TO_MOB | $1432 | 13 | static | `apply_damage_to_mob` |
| AUTO_HEAL | $143F | 67 | header | `auto_heal` |
| INCREMENT_HP | $1482 | 33 | static | `increment_hp` |
| ENTER_COMBAT_STATE | $14B7 | 22 | header | `enter_combat_state(pos)` |
| START_COMBAT | $14CD | 11 | header | `start_combat` |
| ATTACK_RIGHT / LEFT / UP / DOWN | $14D8 / $14DD / $14E2 / $14E7 | 5 / 5 / 5 / 3 | header, chain | `attack_right` ... `attack_down` |
| FIND_ATTACK_TARGET | $14EA | 77 | header, chain | `find_attack_target` |
| INIT_ATTACK_DISPLAY | $1537 | 39 | header | `init_attack_display` |
| SELECT_SOMEONE_ELSE | $155E | 6 | header, chain | `select_someone_else` |
| CHECK_ATTACK_ELIGIBLE | $1564 | 38 | header | `check_attack_eligible` (bool, the carry) |
| NEXT_COLOCATED_MOB | $158A | 33 | static | `next_colocated_mob` |
| RESOLVE_ATTACK | $15AB | 240 | header | `resolve_attack` |
| ADD_WILLINGNESS | $169B | 8 | static | `add_willingness` |
| CALC_COMBAT_STRENGTH | $16E0 | 113 | static | `calc_combat_strength` |
| PLAYER_ATTACK | $1751 | 248 | static | `player_attack` |
| PROCESS_ENTITY_SCRIPT | $1849 | 23 | static | `process_entity_script` |
| MOB_ATTACK | $1860 | 132 | static | `mob_attack` |
| SELECT_COMBAT_TARGET | $18E4 | 149 | header | `select_combat_target` |
| GET_ENTITY_THREAT | $1979 | 36 | static | `get_entity_threat` |
| LOAD_TARGET_PTR | $199D | 16 | static | `load_target_ptr` |
| HANDLE_ENCOUNTER | $1C01 | 240 | header | `handle_encounter(ev)` |
| SHOW_LOCKED_MSG | $1CF1 | 19 | header | `show_locked_msg` |
| HANDLE_EVENT_SLOT | $1D4D | 143 | header, chain | `handle_event_slot(ev)` |
| EVENT_PRNG_CHECK | $1DDC | 126 | static | `event_prng_check` |
| HANDLE_MOB_ENCOUNTER | $1E5A | 169 | header, chain | `handle_mob_encounter` |
| SET_TARGET_RESTRICTED | $1F03 | 9 | static | `set_target_restricted` |
| CHECK_THREATS_HERE | $645F | 12 | header | `check_threats_here` |
| MODIFY_CHAR_STATS | $704D | 135 | static | `modify_char_stats(rec, amount, add)` |
| MODIFY_CURRENT_STATS | $70D4 | 11 | header | `modify_current_stats(amount, add)` |

Variables combat defines: WAIT_LOOP_INIT ($7ABE, `$20`), read-only,
becomes `#define WAIT_LOOP_INIT 0x20`.

Kept objects it uses: the page `$5A` variables DAMAGE_AMOUNT,
HEAL_NOTIFIED, ENCOUNTER_RESULT, COLOCATE_LINK/POS/WRAPPED,
HOSTILE_APP/A/Y, ADJACENT_THREAT, ADJACENT_POS, EVENT_SLOT_BYTE,
EVENT_SUB_VALUE, EVENT_DEST_POS, SAVED_MOB_LINK, COMBAT_WILLINGNESS,
TARGET_DEFENSE, COMBAT_STRENGTH, MOB_DEFENSE, STAT_SCALE,
STAT_LEVEL_BITS, TARGET_PTR, BEST_THREAT, IS_PLAYER_TURN,
CURRENT_COL/ROW, TURN_START_COL/ROW, MOVE_POINTS, STEPS_TAKEN,
WAIT_LOOP_COUNT, PRNG_OUTPUT; `SCENE_COMBAT_FLAG`, `ATTACK_VERB_STRINGS`,
`MOBS`, `ENTITY_PTR`, `MOB_PTR`, `CHAR_PTR`, and input's
`S_ATTACK_TARGET` (`ATTACK_NAME_BUF`) and `S_MISSES`.

Notes:

- The chain exits, from report_turn's table: ATTACK_RIGHT/LEFT/UP
  `BPL FIND_ATTACK_TARGET`, otherwise each falls into the next entry.
  RIGHT's fall into LEFT undoes its INC, but LEFT's fall into UP
  leaves the column at `$FF` and steps the row, and UP's fall into
  DOWN undoes UP's DEC. Translate the fall-throughs literally.
  FIND_ATTACK_TARGET's exits are `JMP RESOLVE_ATTACK` (three places),
  which is `resolve_attack(); return CHAIN_RETURN;`, and `JMP
  MENU_ATTACK_TARGET`, `return CHAIN_MENU_ATTACK_TARGET;`.
  SELECT_SOMEONE_ELSE returns `CHAIN_MENU_ATTACK_TARGET`.
  HANDLE_MOB_ENCOUNTER returns `CHAIN_END_TURN` (three places) or
  `CHAIN_MOVE_LOOP`. HANDLE_EVENT_SLOT returns `CHAIN_MOVE_LOOP` at
  `$1D85`, and its other exits (`JMP SHOW_LOCKED_MSG`, `JMP
  APPLY_DAMAGE_TO_CURRENT`, and EVENT_PRNG_CHECK) end the chain.
- EVENT_PRNG_CHECK is entered by falling in from HANDLE_EVENT_SLOT and
  by JMP from PLAYER_ATTACK (`$1823`, `$183B`) and HANDLE_EVENT_SLOT
  (`$1E57`). It becomes a static function that each calls and then
  returns.
- RESOLVE_ATTACK's exits are its RTS and `JMP PLAYER_ATTACK` / `JMP
  MOB_ATTACK`, tail calls.
- The stores of RANDOM_IN_RANGE's results into PRNG_OUTPUT at `$1C13`
  and `$1DBB` stay, because READ_KEYBOARD and LOAD_SCENE_DATA read
  PRNG_OUTPUT later.
- PLAYER_ATTACK on an empty square writes through MOB_PTR = 0 or 1
  into `$00-$03`, and PICK_RANDOM_MEMBER reads `$02-$0C` through
  MOB_PTR = 0. Keep both. The header reserves those bytes.
- PLAYER_ATTACK and SET_BLINK_TARGET point MOB_PTR at an event entry
  and pass it to GET_ENTITY_FONTCHAR and CLASSIFY_ENTITY_TYPE as an
  untagged reference. When MOB_PTR is a mob, the type read is the high
  byte of its name pointer (`$54-$5A`).
- APPLY_DAMAGE plays tune 4 or 5 with X from a PLA of `$08` or `$0A`,
  and tune 2 with `strength` = DAMAGE_AMOUNT. MOB_ATTACK and
  PLAYER_ATTACK play tune 1 with `strength` = COMBAT_STRENGTH.
- EFFECT_REC (`$F6`) becomes the `rec` parameter of apply_damage and
  modify_char_stats, and the argument of entities' print_entity_name.
- HANDLE_EVENT_SLOT reads EVENT_PTR at `$1E43` after `SCENE_LOOP(#$13)`.
  With `ev` a parameter, the value is the one MOVE_LOOP passed.
- HANDLE_ENCOUNTER reaches COMMIT_MOVE and RESET_TURN_POS by JSR and
  by tail JMP (`$1C9C`, `$1CAB`), and plays tune 10 by tail JMP at
  `$1C3B`. PROCESS_ENTITY_SCRIPT, PLAYER_ATTACK, and MOB_ATTACK call
  util's `dispatch_on_mode()`. PROCESS_ENTITY_SCRIPT keeps SAVED_CHAR
  in `$BA` across it, a local in C.
- HANDLE_MOB_ENCOUNTER relies on X surviving STEP_PRNG (`$1E96`), which
  a C local covers.
- The labels ROL2 inside combat's chunks are macro labels inside routines.

## ai.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| FIND_NEAREST_EVENT | $138D | 78 | header | `find_nearest_event` |
| MANHATTAN_DISTANCE | $13DB | 43 | header | `manhattan_distance(pos, from)` |
| NPC_MOVE_AI | $6742 | 211 | header | `npc_move_ai` |
| EVALUATE_DIRECTION | $6815 | 138 | static | `evaluate_direction(dir, test_pos)` |
| INC_MOB_COUNTERS | $689F | 12 | static | `DIR_SCORES[dir] += n` |
| AI_CHOOSE_TARGET | $68AB | 285 | header | `ai_choose_target` |
| DISPLAY_SHOP | $70DF | 181 | header, chain | `display_shop(ev)` |
| BROWSE_HANDLER | $7194 | 24 | header, chain | `browse_handler` |
| SHOP_BUY_HANDLER | $71AC | 68 | header, chain | `shop_buy_handler` |
| ADVANCE_SHOP_PTR | $71F0 | 12 | static | indexing |
| ADD_PRICE_COMPONENT | $71FC | 15 | static | the price multiplication |
| SHOP_BUY_EFFECT_0-4 | $720B-$7223 | 6 each | static | `apply_shop_effect(n)` |
| APPLY_SHOP_EFFECT | $7229 | 32 | static | the same |
| XP_AWARD_0-4 | $7249-$7259 | 4 each | static | `award_xp(n)` |
| AWARD_XP | $725D | 67 | static | the same |
| EXTRACT_PENDING_XP | $72A0 | 7 | static | `(a >> 4) & 7` |

Variables ai defines: `static uint8_t shop_number`, which replaces
SHOP_DATA_PTR (`$E6/$E7`) and carries the shop across the menu's round
trips. DISPLAY_SHOP sets it to `ev->type - 0xD5`.

Kept objects it uses: `SHOP_TABLE` ($7EDD), `SHOP_EFFECT_TABLE`,
`XP_AWARD_TABLE`, `SHOP_ITEM_MASK`, `SHOP_PRICE`, `SHOP_VALID_MASK`,
`LOCATION_STYLE_BITS`, `SHOP_ITEM_OFFSET`, `LOCATION_STYLE`,
`NEAREST_POS`, `CANDIDATE_POS`, `AI_PREV_DIR`, `EVENT_POS`,
`AI_BEST_DIST`, `DIR_SCORES`, `AI_TARGET_POS/COL/ROW`, `FONT_COL/ROW`,
`TOTAL_MOB_COUNT`, `CHAR_AI_MODE`, `ENTITY_PTR`, `MOB_PTR`, `CHAR_PTR`,
and input's `S_PAY_FOR` (`SHOP_PRICE_DIGITS`, `SHOP_ITEM_NAME`).

Notes:

- `find_nearest_event()` measures from `ENTITY_PTR->pos`, as the
  original does. map's ENTER_LOCATION points ENTITY_PTR at
  CANDIDATE_POS (`$5A15`), so the position is PRNG_OUTPUT (`$5A18`),
  and the store of each candidate's position to CANDIDATE_POS lands on
  the record's byte 0. When no entry qualifies it returns NEAREST_POS
  unchanged. AI_CHOOSE_TARGET calls it with ENTITY_PTR on the mob.
- `manhattan_distance(pos, from)` takes the reference position as a
  parameter, which removes EVALUATE_DIRECTION's store of the test
  position into `E[3]` and its restore from AI_SAVED_POS. Keep its
  stores of pos's column and row into FONT_COL and FONT_ROW. Callers
  pass `ENTITY_PTR->pos` as `from`, except EVALUATE_DIRECTION, which
  passes the test position, and combat's GET_ENTITY_THREAT, which
  passes what it read from `E[3]`.
- NPC_MOVE_AI's steps are in report_ai. Their order fixes the order of
  the PRNG calls. The ROLs that multiply the scores by 4 shift in a
  carry that is always clear. A tie keeps the higher index.
- Several comments in the source disagree with the code, and the code
  governs: the shop tier is LOCATION_STYLE bits 6-5, the shop bonus is
  strength bits 4-3, AWARD_XP overwrites armor where its comment says
  it adds, EVALUATE_DIRECTION adds 9 when the test square isn't the
  target, AI_CHOOSE_TARGET's mode 2 condition is the reverse of its
  comment, and a mob reference from FIND_ENTITY_AT_POS isn't a
  "player".
- DISPLAY_SHOP returns `CHAIN_RETURN` for an AI and otherwise
  `CHAIN_START_GAME_INPUT`. BROWSE_HANDLER enters its body at
  `.next_item`, so the two share a static helper. With
  SHOP_VALID_MASK = 0 the item loop never ends, as in the original.
- SHOP_BUY_HANDLER, when the gold covers the price:
  `modify_current_stats(SHOP_PRICE, false); set_cursor_row21();
  display_message(MSG_SHUFFLES_OFF);` then reads the slot's handler
  word from `SHOP_TABLE[shop_number].slot[SHOP_ITEM_OFFSET / 5]` and
  compares it with the ten `SHOP_*_ADDR` constants: SHOP_BUY_EFFECT_n
  calls `apply_shop_effect(n)` and XP_AWARD_n calls `award_xp(n)`. Any
  other word (a junk slot, which the shipped data never selects) calls
  neither. Then it returns `CHAIN_BROWSE_HANDLER`. With too little
  gold it prints message `$31` and returns `CHAIN_BROWSE_HANDLER`.
- AWARD_XP never ends when the level plus the pending amount is 0, as
  in the original. Keep that.
- UPDATE_EVENT_LIST (events) can carry a gold entry's type past `$D0`
  up to `$D8`. A type of `$D7` or `$D8` makes DISPLAY_SHOP index a
  record past SHOP_TABLE's two, which the C reproduces by reading
  `SHOP_TABLE[shop_number]` past the end of the declared array.

## events.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| CHECK_CAN_LEAVE | $1231 | 47 | header | `check_can_leave` |
| UPDATE_EVENT_LIST | $1260 | 112 | header | `update_event_list(pos, value)` |
| TRIGGER_SCENE_EVENT | $1D04 | 73 | header | `trigger_scene_event(ev)` |
| PICKUP_TREASURE | $646B | 272 | header | `pickup_treasure(ev)` |
| INC_STRENGTH | $657B | 5 | static | `inc_strength` |
| INC_LEVEL | $6580 | 19 | static | `inc_level` (shared body) |
| CLEAR_TREASURE_DIGITS | $6593 | 14 | static | `clear_treasure_digits` |
| CLEAR_AND_DISPLAY | $65A1 | 8 | static | `clear_and_display(strings)` |
| RANDOM_EVENTS | $65A9 | 100 | header | `random_events` |
| EVENT_PICK_MOB | $660D | 6 | static | `event_pick_mob` |
| EVENT_PICK_CHAR | $6613 | 6 | static | `event_pick_char` |
| EVENT_FACTION_SCENE | $6619 | 3 | static | `event_faction_scene`, falls into the next |
| EVENT_FACTION_SCENE_CONT | $661C | 50 | static | `event_faction_scene_cont(char_index)` |
| EVENT_SPAWN_MOB | $664E | 59 | static | `event_spawn_mob` |
| PICK_RANDOM_MOB | $6689 | 40 | static | `pick_random_mob` |
| COPY_BE_TO_FA | $66B1 | 9 | static | its tail, shared with PICK_RANDOM_CHAR |
| PICK_RANDOM_CHAR | $66BA | 20 | static | `pick_random_char` |
| PROCESS_CHAR_EVENTS | $66CE | 56 | static | `process_char_events(char_index)` |
| FIND_MOB_BY_FACTION | $6706 | 40 | static | `find_mob_by_faction(faction)` (bool) |
| CHECK_LOCATION_MATCH | $672E | 20 | static | `check_location_match` |
| CHECK_SCENE_EVENTS | $6DFA | 96 | header | `check_scene_events` (also SCENE_LOADER_END) |
| SCAN_FOR_HOSTILE | $6E5A | 68 | static | `scan_for_hostile` |
| PROCESS_SCENE_GROUPS | $6E9E | 43 | static | `process_scene_groups` |
| SCAN_AND_REORDER | $6ED5 | 20 | static | `scan_and_reorder` |
| FLUSH_PENDING_XP | $6EE9 | 46 | header, chain | `flush_pending_xp` |
| INIT_DROP_GOLD | $6F17 | 59 | header, chain | `init_drop_gold` |
| INIT_DROP_DISPLAY | $6F52 | 32 | header | `init_drop_display` |
| ADJ_DROP_INC | $6F72 | 4 | header, chain | `adj_drop_inc` |
| ADJ_DROP_DEC | $6F76 | 63 | header, chain | `adj_drop_dec` (shared body) |
| DROP_GOLD_EXEC | $6FB5 | 80 | header, chain | `drop_gold_exec` |
| FIND_MSB | $7005 | 16 | static | `find_msb` |
| ADD_DROP / SUB_DROP / HALVE_STEP | $7015 / $7029 / $703D | 20 / 20 / 16 | static | 16-bit `+=`, `-=`, `>>= 1` |

Events defines no movable tables or variables. TRAP_TYPE (`$5A58`) is
a local of `pickup_treasure`.

Kept objects it uses: `EVENT_THRESHOLD`, `SCENE_COMBAT_FLAG`,
`SCENE_P3_DONE`, `SCENE_P1_DONE`, `SCENE_HOSTILE_DONE`,
`SCENE_EVENT_RESULT`, `TOTAL_MOB_COUNT`, `CURRENT_PLAYER`,
`CURRENT_ROOM`, `PRNG_OUTPUT`, `SOURCE_CHAR`, `ACTIVE_CHAR`,
`LOCATION_POS`, `MSG_LINE_COUNT`, `SAVED_EVENT_PTR` ($5A5B),
`DROP_AMOUNT`, `DROP_STEP`, `SCENE_FOUND_FLAG`, `SCENE_HOSTILE_FLAG`,
`SCENE_GROUP_IDX`, `LOCATIONS`, `ENTITY_PTR`, `MOB_PTR`, `CHAR_PTR`,
and input's `S_CHEST_MESSAGE` (`S_CHEST_OPENS`, `S_REVEAL_DIGITS`,
`S_NOW_HAVE_DIGITS`) and `S_DUCATS_TO_DROP` (`DROP_DISPLAY_BUF`).

Notes:

- CHECK_CAN_LEAVE prints ENTITY_PTR's name through PRINT_MOB_NAME.
  report_events says MOB_PTR, which main.asm doesn't bear out.
- TRIGGER_SCENE_EVENT and PICKUP_TREASURE are ordinary functions. Their
  sketches in report_events are exact. TRIGGER_SCENE_EVENT saves
  EVENT_PTR in `$5A5B` across COMMIT_MOVE. With `ev` a parameter the
  save is unneeded, and the C may keep the store to `SAVED_EVENT_PTR`
  or drop it.
- PICKUP_TREASURE stores the entry pointer in MOB_PTR, and leaves it
  there. PICK_RANDOM_MOB uses MOB_PTR's low byte as its counter
  (`MOB_PTR_LO`) and leaves it. Keep both stores, since other units
  read MOB_PTR.
- PICK_RANDOM_MOB branches into PICK_RANDOM_CHAR when TOTAL_MOB_COUNT
  is 0, and both end with COPY_BE_TO_FA (`CHAR_PTR = record`).
- EVENT_FACTION_SCENE_CONT returns when CHAR_PTR's low byte is `$09`
  (record 1 at `$4009`), and sets `CHAR_PTR = LOCATIONS` (`$4000`,
  the header record) before FIND_MOB_BY_FACTION. CHECK_LOCATION_MATCH
  compares a record pointer's low byte with LOCATION_POS. Record
  addresses are kept, so these comparisons work unchanged.
- RANDOM_EVENTS' last step is a tail call
  `event_faction_scene_cont(pick_random_char())`. Its burst fires on
  the same test as the other steps, although its comment says
  inverted.
- CHECK_SCENE_EVENTS takes its "hostile" path when SCENE_HOSTILE_FLAG
  is 0, against its comment. Its scene_loop calls at the ends of paths
  are tail calls.
- CLEAR_AND_DISPLAY sets MSG_LINE_COUNT = 0 and calls text's
  `display_message_loop(S_CHEST_OPENS)`, which prints the three
  strings up to the `$00`.
- UPDATE_EVENT_LIST adds to a gold entry's type with an 8-bit add and
  carry, so a type can pass `$D0`. Keep it. Its end of list prints
  message `$2A` (MSG_ARMOR_WORTHLESS by name).
- INIT_DROP_GOLD's second test is the Z flag of FIND_MSB's last ROL,
  which is clear exactly when the result is nonzero. ADJ_DROP_INC's
  `BNE` into the shared body is always taken. An ADJ_DROP with step 0
  is `return init_drop_gold();`.
- DROP_GOLD_EXEC compares DROP_AMOUNT as a signed 16-bit number.

## entities.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| LOAD_CHAR_NAME_PTR | $0925 | 14 | header | `load_char_name_ptr` |
| SETUP_CHAR_SPRITE | $0933 | 37 | header | `setup_char_sprite` |
| FIND_ENTITY_AT_POS | $0B62 | 152 | header | `find_entity_at_pos` |
| GET_ENTITY_FONTCHAR | $0C7E | 121 | header | `get_entity_fontchar` |
| APPEARANCE_TO_FONTCHAR | $0CF7 | 25 | header | `appearance_to_fontchar` |
| PRINT_MOB_NAME | $0D8A | 14 | header | `print_mob_name` |
| REORDER_CHAR | $0F84 | 171 | header | `reorder_char(m)` |
| DECR_GROUP_COUNT | $102F | 4 | static | `adjust_pc_count(loc, m, -1)` |
| INCR_GROUP_COUNT | $1033 | 32 | static | `adjust_pc_count(loc, m, +1)` |
| PRINT_ENTITY_NAME | $1142 | 20 | header | `print_entity_name(m)` |
| DRAW_CHAR_AT_POS | $1156 | 21 | header | `draw_char_at_pos` |
| CLASSIFY_ENTITY_TYPE | $116B | 145 | header | `classify_entity_type` |
| FIRST_GROUP_MEMBER | $12FA | 6 | header | `first_group_member`, falls into the next |
| NEXT_GROUP_MEMBER | $1300 | 23 | header | `next_group_member` |
| PRINT_MEMBER_NAME | $1406 | 14 | header | `print_member_name` |
| REORDER_CURRENT_CHAR | $1414 | 17 | header | `reorder_current_char` |
| SET_BLINK_TARGET | $16A3 | 61 | header | `set_blink_target` |
| LOAD_CHAR_TO_FA | $6EC9 | 12 | header | `load_char_to_fa` |
| RENDER_FONT_CHAR | $7489 | 3 | header | `render_font_char`, falls into the next |
| PRINT_FONTCHAR_AT_TEXT_POS | $748C | 14 | header | `print_fontchar_at_text_pos`, falls into the next |
| RENDER_FONT_CHAR_DIRECT | $749A | 49 | header | `render_font_char_direct(charset_sel)` |
| DRAW_BLINK_ALT | $74CB | 18 | header | `draw_blink_alt` |
| DRAW_BLINK_NORMAL | $74DD | 3 | header | `draw_blink_normal`, falls into the next |
| DRAW_BLINK_CHAR | $74E0 | 44 | static | `draw_blink_char` |
| FONT_POS_TO_TEXT_POS | $7723 | 13 | header | `font_pos_to_text_pos` |

Entities defines no movable tables or variables.

Kept objects it uses: `LOCATIONS`, `MOBS`, `MAP_OBJECT_NAMES`,
`s_PRINT_FONT_CHAR` (with `CHAR_CHARSET` and the four glyph bytes),
`TOTAL_MOB_COUNT`, `CURRENT_PLAYER`, `CURRENT_ROOM`, `CHAR_SPRITE`,
`FONT_COL/ROW/CHARNUM/CHARSET`, `ENTITY_SEARCH_POS`, `PRNG_OUTPUT`,
`SOURCE_CHAR`, `ACTIVE_CHAR`, `CURRENT_COL/ROW`, `IS_PLAYER_TURN`,
`BLINK_COL/ROW/CHAR`, `TEXT_COL`/`TEXT_ROW`, `ENTITY_PTR`, `MOB_PTR`,
`CHAR_PTR`, and map's `DEFAULT_CHAR` and `BLINK_ALT_CHAR`.

Notes:

- FIND_ENTITY_AT_POS modifies its own operands (`+25/+26`, `+30`,
  `+36/+37`, `+91/+92`, `+103/+104`) with CHAR_PTR, the event pointer,
  and each mob record. In C those are pointer variables. With no match
  it jumps to map's CHECK_AT_BOUNDARY, `return check_at_boundary(pos);`.
  A mob comes back as `MOB_REF(m)`, an event entry as its address.
- GET_ENTITY_FONTCHAR maps an event's type class `$00` to glyph `$1B`
  and `$80` to BLINK_ALT_CHAR. The comments have these swapped. Its
  full table is in report_entities.
- REORDER_CHAR walks from the location record as a head node, since
  the link is at offset 2 in both kinds of record. Keep the walks that
  run away when the mob isn't in the list, as the original does. With
  ACTIVE_CHAR = 0 it inserts at a random place (a STEP_PRNG per link).
  It fixes CURRENT_ROOM when it unlinks that mob, and adjusts
  TOTAL_MOB_COUNT for a player character.
- DECR/INCR_GROUP_COUNT change the location's pc_count (byte 8), and
  do nothing for record 0 (low byte of the record pointer 0) or for a
  mob with appearance `$15` or more. The original's comments say mob
  byte 8.
- FIRST_GROUP_MEMBER with an empty list falls into NEXT_GROUP_MEMBER,
  which continues from the old MOB_PTR's next link, and reads `$0002`
  when MOB_PTR is 0. Keep the fall-through.
- CLASSIFY_ENTITY_TYPE returns a string (one of `S_A_WALL` ...), which
  the original leaves in HANDLER_PTR (`$BC`). Its comments call it a
  handler address.
- RENDER_FONT_CHAR_DIRECT reads its selector only for glyphs 24 and
  up, and otherwise leaves CHAR_CHARSET as it was. It writes the four
  glyphs `g = n * 4 + $20` .. `g + 3` into s_PRINT_FONT_CHAR and prints
  it with `print_string(s_PRINT_FONT_CHAR)`. The highest selector seen
  in a run of the original was `$B5`. A selector above that reads past
  FONT_DATA, as the HRCG does in the original.
- DRAW_BLINK_CHAR returns at once when BLINK_COL is 20 or more. It
  saves PRINT_STRING_ADDR around RENDER_FONT_CHAR because
  DISPLAY_MESSAGE reads it afterwards. In C text keeps it in a local,
  so the save is unneeded.
- PRINT_ENTITY_NAME's argument is combat's EFFECT_REC.
- SET_BLINK_TARGET stores MOB_PTR into ENTITY_REC untagged before
  `get_entity_fontchar`, when IS_PLAYER_TURN is 1.
- SETUP_CHAR_SPRITE's TAX has no effect.

## map.c

map has no full report. DECISIONS.md gives its prototypes and statics.

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| SETUP_SCENE_DISPLAY | $0901 | 36 | header | `setup_scene_display` |
| SCENE_SETUP | $0A17 | 124 | header | `scene_setup` |
| CHECK_AT_BOUNDARY | $0BFA | 43 | header | `check_at_boundary(pos)` |
| DISK_READ | $6C12 | 4 | header | `disk_read` |
| DISK_WRITE | $6C16 | 2 | header | `disk_write` |
| RWTS_DISPATCH | $6C18 | 22 | header | `rwts_dispatch(command)` |
| SCENE_LOOP | $6C37 | 87 | header | `scene_loop(scene)` |
| LOAD_SCENE_DATA (SCENE_LOADER_START) | $6DD8 | 21 | static | `load_scene_data` |
| INTEGRITY_FAIL_ADDR | $6DED | 13 | header | `integrity_fail` |
| ENTER_LOCATION | $72A7 | 124 | header | `enter_location` |
| DRAW_MAP_COL | $7323 | 24 | static | `draw_map_col` |
| DRAW_MAP_ENTITIES | $733B | 208 | static | `draw_map_entities` |
| DRAW_MAP_ICON_B | $740B | 48 | static | `draw_map_icon_b` |
| FILL_MAP | $743B | 49 | header | `fill_map` |
| LOAD_MAP_TILES | $746C | 29 | static | `load_map_tiles` |
| CLEAR_MAP | $76B0 | 14 | header | `clear_map` |
| CLEAR_STRENGTH_FLAGS | $793F | 28 | dropped | no callers |
| LOAD_SCENE_DISK_ENTRY | $A441 | 7 | dropped | no callers |
| DISK_READ_JUMP | $A448 | 3 | dropped | `JMP DISK_READ`, folded into its callers |
| DISK_RTS | $A44B | 1 | dropped | an RTS that LOAD_SCENE_FROM_DISK JSRs, folded |
| LOAD_SCENE_FROM_DISK | $A478 | 70 | static | `load_scene_from_disk(scene)` (bool, true = error), in section `.text.scene_disk` |
| SETUP_AND_READ_DISK | $A4BE | 36 | dropped | reached only from LOAD_SCENE_DISK_ENTRY |

Tables and variables map defines (from main.bin):

| Label | Address | Size | Contents | Users |
|---|---|---|---|---|
| BLINK_DELAY | $7AAC | 1 | 2D | DRAW_MAP_ICON_B |
| MAP_FILL_CHAR | $7AC1 | 1 | 25, written by LOAD_MAP_TILES | FILL_MAP |
| DEFAULT_CHAR | $7AC2 | 1 | 2C, written by LOAD_MAP_TILES (header) | entities |
| BLINK_ALT_CHAR | $7AC3 | 1 | 00, written by LOAD_MAP_TILES (header) | entities |
| MAP_REDRAW_FLAG | $7AC4 | 1 | 00, written | DRAW_MAP_ENTITIES |
| TILE_DEFAULT_TAB | $7AD0 | 4 | 2C 2C 2C 2C | LOAD_MAP_TILES |
| TILE_FILL_TAB | $7AD4 | 4 | 25 2A 20 29 | LOAD_MAP_TILES |
| TILE_BLINK_TAB | $7AD8 | 4 | 00 00 00 00 | LOAD_MAP_TILES |

TUNE_END_MARKER (`$A440`) is the END byte of tune 11, kept with the
tunes.

Kept objects it uses: `SCENE_TEXT_DATA` and `SCENE_TRANSITION`,
`GAME_MESSAGE_STRINGS` (integrity_fail), `RWTS_IOB_PTR`, `DOS_ZP`
(`$48`), `LOCATIONS`, `MOBS`, `CHAR_AI_MODE`, `AI_WAIT_FLAG`,
`VIEW_MIN/MAX_COL/ROW`, `FONT_COL/ROW`, `CANDIDATE_POS`, `LOCATION_ID`,
`SOURCE_CHAR`, `ACTIVE_CHAR`, `BOUNDS_*`, `LOCATION_POS`,
`LOCATION_STYLE`, `EVENT_POS`, `WAIT_LOOP_COUNT`, `SCENE_NUMBER`,
`BLINK_ROW/CHAR`, `SAVED_ENTITY_PTR`, `SAVED_MOB_PTR`,
`SCENE_EVENT_RESULT`, `PRNG_OUTPUT`, `ENTITY_PTR`, `MOB_PTR`,
`CHAR_PTR`, `DISK_MOTOR_ON`, `RWTS_ENTRY`, and `DISK_READ_FIELD`.

Notes:

- `load_scene_from_disk` is in section `.text.scene_disk`, which the
  linker script puts in `$A441-$A56E` with util's integrity code:
  `[[c6502::section(".text.scene_disk")]]` on its definition.
- SCENE_LOOP saves ENTITY_PTR and MOB_PTR in SAVED_ENTITY_PTR and
  SAVED_MOB_PTR on entry and restores them on exit, which is what was
  left of util's SAVE_ZP_CONTEXT and RESTORE_ZP_CONTEXT. The `$BC` and
  `$BE` that those also saved are locals and parameters in C. The
  original's exit is `JMP RESTORE_ZP_CONTEXT`.
- SCENE_LOOP calls `text_renderer()`, which may pause at a `$7E` and
  call `display_status_bar()`. TEXT_RENDERER's `JMP MOVE_LOOP` on a
  stream 256 bytes long with no `$7F` isn't reproduced (see text.c).
- ENTER_LOCATION sets `ENTITY_PTR = (struct mob *)&CANDIDATE_POS`
  before `find_nearest_event()`, so that the reference position is
  PRNG_OUTPUT, and leaves it there.
- CHECK_AT_BOUNDARY reads the position from LOCATION_ID (`$5A17`),
  which FIND_ENTITY_AT_POS stored. In C it takes `pos`, returns
  ENTITY_EDGE or ENTITY_NONE, and the store to ENTITY_SEARCH_POS
  stays in FIND_ENTITY_AT_POS.
- RWTS_DISPATCH stores the command at IOB+`$0C` and 0 at IOB+3 through
  RWTS_IOB_PTR, calls `RWTS_ENTRY(RWTS_IOB_PTR)`, stores 0 to `$48`
  (`DOS_ZP[0x48 - 0x2A]`), and returns the carry. DISK_READ loads
  command 1 and DISK_WRITE command 2 and fall into it. Its `LDY #$0C`
  makes FORMAT_DISK_TRACK's own load unnecessary.
- INTEGRITY_FAIL_ADDR is the error tail of LOAD_SCENE_DATA: `y =
  PRNG_OUTPUT; step_prng(); GAME_MESSAGE_STRINGS[1 + y] = PRNG_OUTPUT;`
  and RTS. util's VERIFY_INTEGRITY jumps to it, which in C is a call.
  LOAD_SCENE_DATA itself checks PRNG_OUTPUT before stepping it.
- DRAW_MAP_ENTITIES zeroes the location's mob head during its scan and
  restores it, sets the style's bit 7, and can reach FIRST_GROUP_MEMBER
  with an empty list, which reads `$02-$0C` through MOB_PTR = 0.
- CLEAR_MAP prints CHAR_PTR's name through SETUP_TEXT_POS. Its name
  and its comment in main.asm (which belongs to SET_CURSOR_ROW21)
  don't describe it.
- SETUP_SCENE_DISPLAY: a player's turn (CHAR_AI_MODE == 0) gets
  TUNE_AI_TURN and a wait, and an AI's gets neither, which is the
  reverse of the original's comments. Its ROL at `$091E` takes its
  carry from the HRCG's ctrl-K handler, which leaves it clear here.
- LOAD_SCENE_DATA reads DISK_MOTOR_ON (`$C0E9`), and
  LOAD_SCENE_FROM_DISK runs RWTS and DISK_READ_FIELD and reads the
  scene's sector into SCENE_TEXT_DATA.

## display.c

display has no full report.

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| DISPLAY_STATUS_BAR | $6D1F | 52 | header | `display_status_bar` |
| SETUP_TEXT_POS | $76D2 | 6 | header | `setup_text_pos(name)` |
| SETUP_TEXT_POS_COL20 | $76D8 | 45 | header | `setup_text_pos_col20(name)` (one body) |
| DISPLAY_CHAR_STATS | $7780 | 296 | header | `display_char_stats` |

Display defines no movable tables or variables.

Kept objects it uses: `STATS_TEMPLATE` and its fields,
`STATS_PROMPT_KEYBOARD`, `STATS_PROMPT_JOYSTICK`, `STATUS_BORDER_DATA`,
`S_PRESS_SPACE`, `S_PRESS_BUTTON`, `INPUT_MODE`, `ENTITY_PTR`,
`TEXT_STREAM_PTR`, `TEXT_COL`/`TEXT_ROW`, `KBD`, `KBDSTRB`, `BUTN`.

Notes:

- DISPLAY_STATUS_BAR sends HRCG_CTRL_P, renders STATUS_BORDER_DATA
  with `text_renderer_no_pause()` (it has no `$7E`, and the call keeps
  text_renderer and display_status_bar from calling each other), sets
  row 23 and column 4, and prints S_PRESS_SPACE or S_PRESS_BUTTON by
  INPUT_MODE. DISPLAY_STATUS_BAR+34 is an internal target.
- DISPLAY_CHAR_STATS fills the STATS_TEMPLATE fields with
  `num_to_decimal`, prints it, waits for a key or the button, stores
  `anim_check_frame()`'s value to KBDSTRB, and ends with `JMP
  CLEAR_MAP`, which is `clear_map()`.
- SETUP_TEXT_POS and SETUP_TEXT_POS_COL20 share one body: column 0 or
  20, row 20, HRCG_CTRL_A '0' and a carriage return, 20 spaces, the
  name centered, and HRCG_CTRL_K.

## util.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| CLAMP_CHAR_FIELD | $0A93 | 27 | header | `clamp_char_field` |
| FIND_FIRST_PC | $0AAE | 32 | header | `find_first_pc` (true = found) |
| COPY_PC_NAME_TO_BUF | $0ACE | 48 | static | `copy_pc_name_to_buf(m)` |
| FIND_NEXT_PC_WRAP | $0AFE | 4 | header | `find_next_pc_wrap` |
| FIND_NEXT_PC | $0B02 | 38 | static | `find_next_pc` |
| FIND_PREV_PC | $0B28 | 39 | header | `find_prev_pc` |
| INIT_CHAR_SELECT | $0B4F | 19 | header, chain | `init_char_select` |
| POS_TO_COLROW | $0C25 | 13 | header | `pos_to_colrow` |
| GET_MOB_DATA | $0C32 | 31 | header | `get_mob_data` |
| DELAY_WITH_ANIMATION | $0D10 | 14 | header | `delay_with_animation` |
| TICK_ANIM_COUNTER | $0D1E | 18 | header | `tick_anim_counter` |
| ANIM_TICK_AND_WAIT | $0D30 | 6 | header | `anim_tick_and_wait`, falls into the next |
| ANIM_CHECK_FRAME | $0D36 | 19 | header | `anim_check_frame` |
| TIMED_WAIT | $0D49 | 15 | header | `timed_wait` |
| CALL_ROM_WAIT | $0D58 | 6 | static | `WAIT(WAIT_DURATION)` |
| IDLE_ANIM_HANDLER | $0D5E | 35 | header | `idle_anim_handler` |
| DEC_WAIT_IF_NONZERO | $0D81 | 9 | static | `dec_wait_if_nonzero` |
| COLROW_TO_POS | $0D98 | 9 | header | `colrow_to_pos` |
| GET_CHAR_RECORD | $1053 | 28 | header | `get_char_record` |
| RANDOM_IN_RANGE | $1317 | 118 | header | `random_in_range` |
| DISPATCH_ON_MODE | $1F0C | 13 | header | `dispatch_on_mode` |
| SAVE_ZP_CONTEXT | $6D53 | 41 | dropped | map's scene_loop saves what is needed |
| RESTORE_ZP_CONTEXT | $6D7C | 41 | dropped | the same |
| STEP_PRNG | $78A8 | 31 | header | `step_prng` |
| VERIFY_INTEGRITY | $A4E6 | 34 | header | `verify_integrity`, section `.text.integrity` |
| VERIFY_DISK_IO | $A508 | 31 | header | `verify_disk_io`, empty, section `.text.integrity` |
| COMPUTE_CHECKSUM | $A529 | 70 | static | `compute_checksum(start, end)`, section `.text.integrity` |
| COLD_RESTART | $A56F | 33 | header | `cold_restart`, section `.cold_restart` |
| LOAD_END_OFFSET, ADVANCE_CKSUM_START, ADVANCE_RANGE_CTR | $A590-$A5BB | 44 | dropped | no callers |

Tables and variables util defines (from main.bin):

| Label | Address | Size | Contents | Users |
|---|---|---|---|---|
| WAIT_DURATION | $7AAD | 1 | 50 | CALL_ROM_WAIT |
| ANIM_FRAME_CTR | $7AAF | 1 | 03, written | the animation routines |
| ANIM_WAIT_TABLE | $7AB0 | 6 | 00 32 23 17 0C 05 | TICK_ANIM_COUNTER |
| ANIM_FRAME_TABLE | $7AB6 | 6 | 00 02 02 04 09 13 | ANIM_CHECK_FRAME |
| ANIM_INDEX | $7ABD | 1 | 02, written by input (header) | util, input |
| DELAY_COUNT | $7ABF | 1 | 04 | DELAY_WITH_ANIMATION |
| INTEGRITY_DATA | $A4E2 | 4 | AE 7A 4F 87, section `.rodata.integrity` | VERIFY_INTEGRITY |
| CKSUM_ACC | $A527 | 2 | code bytes used as the checksum's accumulators, which become locals | COMPUTE_CHECKSUM |

Kept objects it uses: `LOCATIONS` (`MOB_BASE`, `CHAR_LIST_HEAD`),
`MOBS`, `SCENE_EVENT_RESULT`, `INPUT_MODE`, `BROWSE_CHAR_INDEX`,
`PRNG_OUTPUT`, `PRNG_STATE`, `WAIT_LOOP_COUNT`, `MOB_PTR`,
`DISK_IO_START`/`DISK_IO_END`, input's `CHAR_SELECT_NAME`, and sound's
`SOUND_ON`.

Notes:

- `cold_restart` is `[[noreturn]]` and in section `.cold_restart`,
  which the linker script puts at `$A56F`, where SOFTEV points. RESET
  enters it with any state, and it overwrites `$0800-$A56E`, which
  holds C code and data, so it must use no soft stack, call nothing in
  that range, and keep its pointer in zero page. Check the listing.
  It fills page p with the byte p for pages `$08-$A4`, storing offset
  0 and then `$FF` down to 1, then `$A501-$A56F` with `$A5` from the
  top down (`$A500` isn't written), then jumps to `DISK_BOOT_ROM`
  (`$C600`). The stop constants encode its own address, so the C uses
  the constants `0xA5` and `0x6F`. Its last store overwrites its own
  first byte, which has already run. No unit takes its address (init
  stores the constant `0xA56F` in SOFTEV), so it stays eligible for
  zp_abi.
- VERIFY_DISK_IO checksums 34 bytes of the original's LOAD_SCENE_DATA
  (`$6DD8-$6DF9`), which nothing writes, so it always passes. In C it
  does nothing. VERIFY_INTEGRITY checksums the kept `$B300-$B442` and
  compares with `$AE $7A`, and on a mismatch calls map's
  `integrity_fail()`. COMPUTE_CHECKSUM's algorithm is in
  report_util. Put these three and INTEGRITY_DATA in sections
  `.text.integrity` and `.rodata.integrity` with
  `[[c6502::section(...)]]`.
- RANDOM_IN_RANGE returns 0..limit inclusive. Its range tests are
  signed (`CMP #n; BPL`), and the sample is accepted on `d == 0 ||
  (d & 0x80)`. Report_util gives the masks and shifts. A limit of 0
  returns 0 without stepping the PRNG.
- STEP_PRNG computes `69 * state + $53`. Write the multiplication as
  shifts and adds, since `* 69` calls the missing `mul8` helper.
- POS_TO_COLROW divides by repeated subtraction, giving column and row
  for any byte. It returns the column in the low byte and the row in
  the high byte.
- GET_MOB_DATA adds `16 * (index - 1)` to the pointer at `$4000`
  (`MOB_BASE`). An index of 0 gives `MOB_BASE + $FF0`, which no
  caller passes.
- DISPATCH_ON_MODE tests `SOUND_ON` where the original compares
  PLAY_HIGH_TONE's first byte with `$60`: sound off tail-calls
  `delay_with_animation()`, and sound on `anim_tick_and_wait()`.
- FIND_PREV_PC keeps BROWSE_CHAR_INDEX's two copies in locals where
  the original used INPUT_BUFFER[0..1]. If the starting mob is no
  longer a player character it loops forever, as in the original.
- FIND_FIRST_PC branches into COPY_PC_NAME_TO_BUF on a find.
  COPY_PC_NAME_TO_BUF pads CHAR_SELECT_NAME[1..20] with `$20`, a
  low-ASCII space. FIND_NEXT_PC loops to its own entry over non-player
  mobs and calls FIND_FIRST_PC at the end of the list.
- INIT_CHAR_SELECT: `SCENE_EVENT_RESULT = 1; scene_setup();` then
  `find_first_pc()`, and returns `CHAIN_MOVE_LOOP` when none is found
  (carry set) and `CHAIN_ENTER_CHAR_SELECT` otherwise.
- IDLE_ANIM_HANDLER loops to its own entry while WAIT_LOOP_COUNT is 0.
  In keyboard mode it calls `WAIT($50)` once and decrements
  WAIT_LOOP_COUNT. In joystick mode it calls DEC_WAIT_IF_NONZERO 12
  times without a delay of its own.
- TIMED_WAIT calls `WAIT(0x50)` WAIT_LOOP_COUNT times, leaving it at 0.
  One call is 17,093 cycles, the game's unit of time. The harness
  hooks WAIT, so each call must stay a call.
- ANIM_TICK_AND_WAIT and ANIM_CHECK_FRAME return ANIM_FRAME_CTR in A,
  which DISPLAY_CHAR_STATS writes to KBDSTRB.
- CLAMP_CHAR_FIELD works on MOB_PTR. Its header comment describes caps
  at 31 and 63, which the code doesn't apply.

## text.c

text has no full report.

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| NUM_TO_DECIMAL | $0DA1 | 5 | header | `num_to_decimal(value, out)` |
| NUM_TO_DECIMAL_CONT | $0DA6 | 127 | static | its body, without the entries only the dead ORPHANED_ENCOUNTER_HANDLER calls |
| TEXT_RENDERER | $6C8E | 145 | header | `text_renderer`, `text_renderer_no_pause` |
| DISPLAY_MESSAGE | $761E | 44 | header | `display_message(index)` |
| DISPLAY_MESSAGE_LOOP | $764A | 81 | header | `display_message_loop(msg)` |
| SCROLL_STATUS_LINE | $769B | 21 | header | `scroll_status_line` |
| SET_CURSOR_ROW21 | $76BE | 4 | header | `set_cursor_row21` |
| SET_TEXT_WINDOW_BOTTOM (SET_TEXT_WINDOW) | $76C2 | 16 | header | `set_text_window_bottom` |
| PRINT_BOTTOM_CENTERED | $7705 | 11 | header | `print_bottom_centered(s)`, falls into the next |
| PRINT_CENTERED_PLUS_COL | $7710 | 19 | header | `print_centered_plus_col(s)` |
| SET_INVERSE_VIDEO | $7730 | 5 | header | `set_inverse_video` |
| PRINT_CTRL_N | $7735 | 5 | header | `print_ctrl_n` |
| SET_TEXT_WINDOW_WRAP | $773A | 10 | header | `set_text_window_wrap` |
| SET_TEXT_WINDOW_SCROLL | $7744 | 10 | static | `set_text_window_scroll` |
| PRINT_CTRL_AB | $774E | 10 | header | `print_ctrl_ab` |
| PRINT_STRING_AT_ADDR | $7758 | 4 | header | the same as `print_string` |
| PRINT_STRING | $775C | 18 | header | `print_string(s)` |
| SET_TEXT_WINDOW_UPPER_LEFT_ALL | $792D | 5 | static | `set_text_window_upper_left(0)` |
| SET_TEXT_WINDOW_UPPER_LEFT_LOW | $7932 | 2 | static | `set_text_window_upper_left(21)` |
| SET_TEXT_WINDOW_UPPER_LEFT | $7934 | 11 | static | `set_text_window_upper_left(row)` |

Text defines no movable tables or variables.

Kept objects it uses: `MSG_TABLE_PTR` and `GAME_MESSAGE_STRINGS`,
`NUM_DIGIT`, `NUM_DIVISOR`, `NUM_LEADING`, `MSG_LINE_COUNT`,
`TEXT_LEFT_MARGIN`, `TEXT_RIGHT_MARGIN`, `TEXT_STREAM_IDX`,
`FONT_COL/ROW/CHARNUM/CHARSET`, `TEXT_STREAM_PTR`,
`TEXT_COL`/`TEXT_ROW`.

Notes:

- TEXT_RENDERER calls DISPLAY_STATUS_BAR at a `$7E` pause, and
  DISPLAY_STATUS_BAR calls TEXT_RENDERER, which would be recursion in
  C. `text_renderer()` handles the pause (`tune_player(TUNE_ATTRACT,
  0)`, `display_status_bar()`, then it starts the stream's layout
  again), and `text_renderer_no_pause()` is the same renderer without
  the `$7E` case, for display_status_bar. Share the body through a
  static helper whose pause branch only text_renderer reaches, so that
  the call graph has no cycle. STATUS_BORDER_DATA and UI_TEXT_STREAM
  contain no `$7E`.
- TEXT_RENDERER's `JMP MOVE_LOOP` at `$6CDB` runs only when a stream
  goes 256 bytes with no `$7F`. No stream on this disk does that, and
  C can't reproduce a jump into the chain from inside calls, so it is
  dropped. Say so in the file's departures.
- NUM_TO_DECIMAL writes digits `$B0-$B9` into out[0..4], skipping
  leading zeros, which keep whatever the caller stored (`$20`).
- DISPLAY_MESSAGE reads PRINT_STRING_ADDR after DELAY_WITH_ANIMATION,
  which entities' DRAW_BLINK_CHAR preserves in the original. In C the
  pointer is a local of display_message.
- PRINT_STRING prints nothing for a length of 0 or above 128.
- SCROLL_STATUS_LINE sends SET_TEXT_WINDOW_SCROLL's codes, sets the
  window to rows 21-23 through SET_TEXT_WINDOW_UPPER_LEFT_LOW, sends
  HRCG_CTRL_C, and restores the window with
  SET_TEXT_WINDOW_UPPER_LEFT_ALL and SET_TEXT_WINDOW_WRAP.

## sound.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| TUNE_PLAYER | $795B | 57 | header | `tune_player(tune, strength)` |
| TUNE_PLAYER_FETCH | $7994 | 184 | static | the top of `tune_player`'s loop, through the note action and `$7A49` |
| TUNE_ACTION_INDIRECT | $7A4C | 3 | static | an `if` on TUNE_ACTION_VEC |
| TUNE_ADVANCE | $7A4F | 40 | static | the bottom of the loop |
| TUNE_INC_PC | $7A77 | 8 | static | `TUNE_PC++` |
| PLAY_HIGH_TONE | $7A7F | 18 | static | `play_high_tone(freq, duration)` |
| PLAY_LOW_TONE | $7A91 | 27 | static | `play_low_tone(freq, duration)` |

Variables sound defines: `SOUND_ON` (header), initially true. It
replaces the RTS/LDY bytes that TOGGLE_SOUND writes over the first
byte of both tone routines.

Kept objects it uses: `TUNE_TABLE`, `TUNE_1_COMBAT_HIT`,
`TUNE_2_COMBAT_DAMAGE`, the tunes at `$A300-$A440` through TUNE_TABLE,
`TUNE_PC`, `TUNE_LOOP_CTR`, `TUNE_LOOP_ADDR`, `TUNE_GOSUB_ADDR`,
`TUNE_PRNG_MASK`, `TUNE_ACTION_VEC`, `ATTRACT_FLAG`, `WAIT_LOOP_COUNT`,
`INPUT_MODE`, `PRNG_OUTPUT`, `KBD`, `KBDSTRB`, `SPKR`, `BUTN`, and
init's `BORDER_ANIM_ON`.

Notes:

- `tune` is the original's X divided by 2. main.asm's `TUNE_ATTRACT
  EQU $06` is the offset in X, and the header's `TUNE_ATTRACT` is the
  tune number 3. `strength` is the original's Y, read only for tunes
  1 and 2.
- TUNE_PLAYER's setup runs in this order.
  1. A store to KBDSTRB, a write.
  2. `ATTRACT_FLAG = (tune == TUNE_ATTRACT)`.
  3. For tune 1, `TUNE_1_COMBAT_HIT[14] = strength - 1`, and for tune
     2, `TUNE_2_COMBAT_DAMAGE[12] = strength - 1`, with no special case
     for values that turn the byte into another opcode.
  4. `TUNE_PC = TUNE_TABLE[tune]`, `TUNE_LOOP_CTR = 0`,
     `TUNE_PRNG_MASK = 1`.

  The bytecode is in report_sound section 2.
- A note runs in this order.
  1. SOUND_DURATION = op, then `step_prng()`.
  2. freq = `(PRNG_OUTPUT & TUNE_PRNG_MASK) ^ op`, then `TUNE_PC++`
     and duration = `*TUNE_PC`.
  3. The tone that TUNE_ACTION_VEC selects.
  4. `border_anim_step()`, while BORDER_ANIM_ON is true.
  5. The step at TUNE_ADVANCE.

  STEP_PRNG runs for every note even with sound off, which advances
  the game's random numbers.
- TUNE_ACTION_VEC holds `$7A7F` or `$7A91` after a HIGH or LOW
  command. Store those values and compare with them. Every tune sets
  one before its first note.
- TUNE_ADVANCE does `TUNE_PC++`. In attract mode it then reads KBD and
  returns if bit 7 is set, and with INPUT_MODE != 0 reads BUTN[0] and
  BUTN[1] and returns if either has bit 7 set. Then it does
  `WAIT_LOOP_COUNT >>= 1`. The
  control commands MARK1, MARK2, GOTO1, and a LOOP that repeats go
  back to the fetch without this step. The harness counts KBD reads,
  so the reads must match the original's exactly.
- The tone routines click SPKR 10 times per unit of duration (0 means
  256). The gap between clicks is 5X+11 cycles for the high tone and
  about 10X+28 for the low tone, plus 9 once per 10 clicks. A
  `[[c6502::keep]]` loop that counts a byte down compiles to DEX/BNE,
  5 cycles a pass, which matches the high tone. The low tone's pass is
  10 cycles in the original, and the C loop takes a padding instruction in the
  loop. Check the listing's cycle counts. The harness compares the
  order of clicks among the other events. It doesn't compare their timing. The first
  gap of a note starts from the X TUNE_PLAYER leaves, the previous
  note's frequency except for a tune's first note.
- With sound off the tone routines return at once.

## hrcg.c

| Label | Address | Size | Scope | C name |
|---|---|---|---|---|
| HRCG_ORIGINAL_ENTRY | $92A5 | 3 | dropped | no callers |
| HRCG_INIT | $92A8 | 3 | header | `hrcg_init` |
| HRCG_ENTRY2, HRCG_ENTRY3 | $92AE, $92B1 | 3, 3 | dropped | reached only from shifted ctrl-Y and ctrl-Z |
| HRCG_PRINT_VERSION | $92B4 | 17 | dropped | reached only from HRCG_ORIGINAL_ENTRY |
| HRCG_INIT_BODY | $92C5 | 35 | header | the body of `hrcg_init` |
| HRCG_GETKEY | $931A | 164 | dropped | KSW points at it, and nothing reads KSW |
| HRCG_COUT | $93BE | 16 | header | `hrcg_cout` |
| HRCG_PROCESS_CHAR | $93CE | 32 | static | inside `hrcg_cout` |
| HRCG_CTRL_A, HRCG_CHARSET_SELECT, HRCG_SET_STD_FONT | $9424, $942C, $943F | 8, 19, 11 | static | `hrcg_charset_select`, `hrcg_set_std_font` |
| HRCG_SET_REPLACE | $944A | 6 | static | inline |
| HRCG_CTRL_B_SAVE, HRCG_CTRL_C, HRCG_CTRL_D | $9450, $9478, $9493 | 40, 27, 32 | static | cases or static functions |
| HRCG_DISPLAY_PAGE | $94B3 | 14 | static | `(void)TXTPAGE1;` as patched |
| HRCG_CTRL_F, _I, _K, _L, _N, _O, _P, _S, _V, _W | $94C9-$95AA | | static | the live handlers |
| HRCG_CTRL_E, _H, _M, _Q, _R, _T, _Y, _Z, HRCG_CLREOL, HRCG_CR_ENTRY | | | dropped | the game never sends them |
| HRCG_RESET_WINDOW, HRCG_RESET | $95D3, $95FA | 37, 39 | static | `hrcg_reset_window`, `hrcg_reset` |
| HRCG_DRAW_CHAR | $9621 | 125 | static | `hrcg_draw_char(c, raw)`, falls into the next |
| HRCG_ADVANCE_CURSOR | $969E | 8 | static | `hrcg_advance_cursor` |
| HRCG_LINE_FEED | $96AA | 29 | static | `hrcg_line_feed` |
| HRCG_SCROLL_UP | $96C7 | 79 | static | `hrcg_scroll_up` |
| HRCG_HOME, HRCG_CLRLINE, HRCG_FILL_LINE | $9716, $9720, $9737 | 10, 19, 58 | static | `hrcg_home`, `hrcg_clrline`, `hrcg_fill_line(row, col)` |
| HRCG_CALC_HIRES_ADDR, HRCG_CALC_HIRES_ROW | $9771, $9773 | 2, 18 | static | `hrcg_row_address(row)` |

Tables and variables hrcg defines:

| Label | Address | Size | Contents | Users |
|---|---|---|---|---|
| the parameter block | $9305-$9319 | 21 | report_hrcg section 3; HRCG_RESET sets all but `$9305` and `$9306`, which are 0 in the image | hrcg only |
| HRCG_DISPATCH_TBL | $93EE | 54 | becomes a switch | |
| HRCG_ROW_TABLE | $9785 | 12 | 00 00 00 00 50 50 50 50 A0 A0 A0 A0, or the equivalent arithmetic | CALC_HIRES_ROW |
| STD_FONT_DATA | $97A5 | 768 | from main.bin; `static const` | the standard font, selector '0' |
| HRCG_FONT_BASE | $92AC | 2 | `$83A5`; use FONT_DATA | |

Kept objects it uses: `FONT_DATA`, `WNDLFT`, `WNDWDTH`, `WNDTOP`,
`WNDBTM`, `CH`, `CV`, `BASL`, the Monitor routines `VTAB`, `VTABZ`,
`CLREOP`, `HOME`, `SCROLL`, and the soft switches `MIXCLR`, `HIRES`,
`TXTPAGE1`, `TXTCLR`.

Notes:

- Translate the behavior after INIT_HRCG's patches: HRCG_DISPLAY_PAGE
  is `BIT $C054; RTS`, and the `JSR $03EA` at `$92D5` is gone.
  `hrcg_init` reads MIXCLR, HIRES, calls `hrcg_reset()` (whose window
  reset ends with `VTAB()`), reads TXTPAGE1, then TXTCLR. CSW and KSW
  aren't set, because nothing reads them in this program.
- A selector `c` after ctrl-A picks font `c - '1'` from FONT_DATA by
  the original's arithmetic (`$83 + 3i` for the high byte, `$A5` for
  the low byte) for i = 0 to 8, and the standard font otherwise. Keep
  the arithmetic for i up to 8, so that '6' to '9' read the memory
  that follows FONT_DATA, as the original does. STD_FONT_DATA moves
  into hrcg.c, and nothing else computes its address.
- DRAW_CHAR stores the raw character at `BASL[CH]` as well as drawing
  it, with whatever BASL the last VTAB, VTABZ, HOME, CLREOP, or SCROLL
  left. Game code often moves CV without a VTAB, so the text-page
  write can land on another row. Keep it as it is.
- The order of hi-res drawing and the ROM call: ctrl-F runs
  HRCG_CLRLINE and then CLREOP, ctrl-P runs HRCG_HOME and then HOME,
  and a line feed in scroll mode runs HRCG_SCROLL_UP and then SCROLL.
  LINE_FEED calls `VTABZ(CV)`, which the harness compares with A.
- A printable character that follows ctrl-O is dropped. HRCG_MODE_FLAGS
  is `$00`, `$40`, or `$80`, and two shifts leave it 0 after every
  character.
- DRAW_MODE stays 0 and VIDEO_MODE `$20` in this game. The OR, AND, and
  XOR draw modes and INVERT_FLAG `$C0` never run, and may be left out
  with a comment.

## Fixed addresses and kept data that point into the program

- `SHOP_TABLE` (`$7EDD`, kept) holds the addresses of SHOP_BUY_EFFECT_0-4
  (`$720B`, `$7211`, `$7217`, `$721D`, `$7223`) and XP_AWARD_0-4 (`$7249`,
  `$724D`, `$7251`, `$7255`, `$7259`) as each slot's handler word. The
  table stays at its address and nothing writes it.
  `shop_buy_handler` compares the word with the header's `SHOP_*_ADDR`
  constants, so no code has to sit at those addresses, and the C
  places the shop's code wherever the linker puts ai.o.
- `TUNE_TABLE` (`$80AE`, kept) points at TUNE_0-2 (`$80C6-$80E4`) and
  TUNE_3-11 (`$A300-$A440`), all kept. Its pointers reach no code.
- `cold_restart` is at `$A56F`, because GAME_INIT stores `$A56F` in
  SOFTEV and calls SETPWRC, so RESET jumps there. Section
  `.cold_restart` holds it, and the linker script puts it at `$A56F`.
  The function also hard-codes its own address in its stop constants.
- `SCENE_TEXT_DATA` (`$5C00-$5CFF`) is a buffer at a fixed address. The
  scene loader's IOB points at it, and the disk-signature routines use
  its first bytes. The C places nothing there.
- `main` is at `$0800` in section `.main`, where the loader enters the
  original. The ELF entry is `main` too.
- Nothing else that the C keeps holds a program address. The location
  records point at their names (`$4240`) and event lists (`$451E`),
  all kept. The mob records point at their names (`$5480`). The event
  lists and the scene data the game reads from the disk hold text,
  positions, and types. The resident copy-protection bytecode
  (`$0500-$07FF`) and the disk reader's bytecode (`$B300-$B5FF`) reach
  only kept code. Searching the image for words equal to a chain
  routine's address, or that address minus 1, found only the menu
  tables (which move into C) and SHOP_TABLE.

## Compiler gaps

Each gap below was tried on c6502 with the example shown. The header
and the units follow the workaround given for each.

1. **A call through a function pointer can't be linked.**

   ```c
   static void (*const handler[3])(void) = { a, b, c };
   void run(uint8_t n) { handler[n](); }
   ```

   The link fails with `compile.py: link error: function(s) contain
   IndirectCall and can't be linked under the private-pool model`.
   Every table of routine addresses becomes a table of small numbers
   and a switch: the menu exits (`CHAIN_` states and `run_chain`), the
   menu inits (`enum menu_init` and `call_input_init`), the border
   handlers (init.c), the HRCG's dispatch table, the tune player's
   tone vector, and the shop's handler words.

2. **A function with register parameters can't have its address
   taken.**

   ```c
   extern uint16_t CSW [[c6502::address(0x36)]];
   void out(register uint8_t c) { ... }
   void hook(void) { CSW = (uint16_t)&out; }
   ```

   compiles to `error: function 'out' declares a register parameter
   or local but isn't eligible for the ZP-passing ABI: ... its
   address is taken`. So a C function can't be the CSW output hook.
   Even without register parameters, a function reached only through
   CSW would be missing from the call graph that the zero-page
   allocator uses, so its parameter and local bytes could overlap its
   callers'. The units call `hrcg_cout(c)` directly where the original
   calls ROM_COUT1. For the same reason init stores the constant
   `0xA56F` in SOFTEV and doesn't take `cold_restart`'s address.

3. **Two objects may not share an address.**

   ```c
   extern uint8_t LOCATION_ID [[c6502::address(0x5A17)]];
   extern uint8_t ENTITY_SEARCH_POS [[c6502::address(0x5A17)]];
   ```

   is `error: ... overlaps`. The header declares one object per
   address and gives the other EQU names as `#define`s
   (`ENTITY_SEARCH_POS`, `WORLD_INIT_DATA`, `TEXT_COL`, ...). Record 0
   of LOCATIONS is also MOB_BASE, CHAR_LIST_HEAD, and MSG_TABLE_PTR,
   which are fields of `struct location_header` through a cast of
   LOCATIONS. The disk reader's code range is a pair of address
   constants, because DISK_READ_FIELD sits at its first byte. Fields
   inside strings (`S_REVEAL_DIGITS`, `ATTACK_NAME_BUF`, the STATS_
   fields, `CHAR_CHARSET`) are macros on their string.

4. **Recursion is a link error** unless every call in the cycle is
   marked `[[c6502::save_hardware_stack]]`.

   ```c
   void text_renderer(void) { ... display_status_bar(); ... }
   void display_status_bar(void) { ... text_renderer(); ... }
   ```

   fails with `link error: function(s) on a cycle in their own TU's
   call graph ...` followed by the functions' names, and the same
   check runs across units at the link. The chain
   is a dispatcher loop, so chain routines never call each other in a
   cycle, and text.c provides `text_renderer_no_pause` for
   display_status_bar. TEXT_RENDERER's jump into MOVE_LOOP from inside
   calls is dropped.

5. **A ROM routine that returns its result in Y can't be declared.**
   c6502 returns a value in A (and X) or a flag in the carry, and has
   no attribute for a return in Y.

   ```c
   [[c6502::zp_abi, c6502::address(0xFB1E)]]
   uint8_t PREAD(register uint8_t x [[c6502::in_x]]);
   ```

   compiles, but the caller stores A, which PREAD leaves holding the
   last read of PADDL0. input.c reads the paddle itself (PTRIG, then a
   count while bit 7 of `PADDL[n]` is set) with the same soft-switch
   accesses.

6. **The runtime helpers aren't in the repository**, so a program that
   calls one doesn't link without `--defsym`.

   ```c
   a = (uint8_t)(b * 69 + 0x53);    /* JSR mul8 */
   a = b / 20;  a = b % 20;         /* JSR sdivmod16 */
   a = b << n;  a = b >> n;         /* JSR asl16, JSR asr16 */
   ```

   The link fails with `undefined symbol 'mul8' (a runtime helper; the
   runtime library isn't in the repository yet, so give its address
   with --defsym)`. Multiplications by 3, 4, and 20 compile inline.
   The simulator has the helpers as Python hooks, but a program
   relying on them wouldn't run on an Apple II, and the original has
   no such calls. Units write these operations as the original does:
   STEP_PRNG's `* 69` as shifts and adds, POS_TO_COLROW's division by
   repeated subtraction, and shifts by a variable count as loops of
   shifts by one, in `uint8_t` so that integer promotion doesn't make
   them 16-bit signed shifts.

## For the harness

- The C program starts from main.bin as it is, with the C build's
  sections loaded over it. main.bin is the image after GAME_INIT's
  relocation, so the C doesn't relocate. The original must start from
  the image before the relocation (the boot loader's output), because
  its GAME_INIT copies `$2000-$32FF` to `$9600` and `$B300`, and in
  main.bin those source pages hold zeros, which would erase the HRCG
  and the disk reader.
- The C has static variables with initial values that `-O` promotes
  into zero page, and the linker's `__zp_init` stores them. Neither
  `main` nor `cold_restart` calls it, so the harness calls `__zp_init`
  before `main`, as `sim.lode_runner.program_machine` does.
- Don't compare COUT (`$FDED`) events. The C calls `hrcg_cout`
  directly, so it makes none, where the original makes one per
  character. The harness's `$FDED` must behave as `JMP (CSW)` for the
  original, so that its characters reach HRCG_COUT. Compare what the
  HRCG draws through the soft-switch and Monitor events it makes, and
  through the screen memory if the harness checks it.
- A is meaningful only at VTABZ, WAIT, and COUT calls. VTAB, HOME,
  CLREOP, and SCROLL don't read it, and the C doesn't reproduce the
  original's A there.
- The harness's stand-ins for the Monitor routines must preserve the
  registers the header promises: X and Y for WAIT, VTAB, and VTABZ,
  and X for CLREOP, HOME, SCROLL, and CLREOL.
- The C reads the paddles itself and never calls PREAD (`$FB1E`). It
  makes PREAD's accesses, a read of PTRIG (`$C070`) and reads of
  PADDL0/1 (`$C064`/`$C065`) until bit 7 clears, so a harness that
  hooks PREAD in the original and doesn't model the paddle timer will
  see different events. Model the paddles at the soft switches, or
  compare only the button and keyboard events in joystick mode.
- `cold_restart` fills `$0800-$A56E` and jumps to `$C600`, the slot 6
  boot ROM, which reboots from the disk. RESET goes there too through
  SOFTEV.
- The resident routines at `$0500-$07FF` sit on the text page, and the
  HRCG's text-page writes (`BASL[CH]`) and the Monitor's HOME, CLREOP,
  and SCROLL overwrite them. Both programs do this.
- The linker script puts map, util, display, and everything it
  doesn't name in `$A900-$B2FF` (RUNTIME_DATA_AREA, zeros in the
  image). No absolute operand of the original addresses that range,
  and no immediate operand names a page in `$A9-$B2`, so the original
  neither reads nor writes it. The harness should still check that a
  run of the original leaves it zero, since a pointer built some
  other way would escape that search.

## Decisions where the reports disagreed

- The five event handlers take `struct event *ev` (DECISIONS.md:
  `uint8_t *ev`; the same address).
- `input_loop` takes `struct menu *` without `const` (DECISIONS.md:
  `const struct menu *`), because the menu is written through
  MENU_DESC_PTR.
- `find_nearest_event` takes no parameter and measures from
  ENTITY_PTR->pos, per DECISIONS.md, which keeps ENTER_LOCATION's
  pointer at CANDIDATE_POS. report_ai proposed a parameter.
  `manhattan_distance` takes `(pos, from)` as report_ai proposed,
  because DECISIONS.md gives no signature for it and EVALUATE_DIRECTION
  measures from a position other than `E[3]`.
- `clamp_char_field` takes no parameter and works on MOB_PTR, the
  original's interface, where report_util proposed a parameter. Both
  callers pass MOB_PTR.
- `configure_input` returns the MOVE option's enable byte (report_input),
  so turn doesn't read the static options.
- `anim_tick_and_wait` and `anim_check_frame` return ANIM_FRAME_CTR,
  which DISPLAY_CHAR_STATS writes to KBDSTRB (report_util).
- report_events says CHECK_CAN_LEAVE prints MOB_PTR's name. main.asm's
  PRINT_MOB_NAME reads ENTITY_PTR, and the header follows the code.
- `$5A5B` stays declared (`SAVED_EVENT_PTR`), as DECISIONS.md lists it
  among the kept variables, though report_events treats it as scratch.
  `$5A60` is a local, as DECISIONS.md says. `INPUT_OPTION_COUNT`
  (`$5AAA`) stays declared, as DECISIONS.md keeps it, though
  report_input calls it a loop counter that could be a local.
- The header keeps the menu tables `extern`, so that any unit can name
  one, though DECISIONS.md lists only six that other units name.
- MENU_DESC_PTR and MENU_OPTION_PTR are zero-page globals at `$C0` and
  `$C2` (DECISIONS.md), where report_input proposed statics.
- sound's border flag is init's `BORDER_ANIM_ON` (DECISIONS.md), where
  report_sound proposed `TUNE_BORDER_ANIM` in sound.c.
- util and map put the routines from `$A441-$A56E` in sections of their
  own (`.text.integrity`, `.rodata.integrity`, `.text.scene_disk`), so
  that the linker script can use that small region. That is an
  addition to DECISIONS.md.
