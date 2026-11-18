# map.c

map.c translates the Map and Scene routines, the RWTS calls, and the
code in the Game Data chapter (LOAD_SCENE_FROM_DISK). Its analysis was
in condensed form only, so each routine below was translated from
main.asm directly.

## What is translated

| Routine | C | Notes |
|---|---|---|
| SETUP_SCENE_DISPLAY | `setup_scene_display` | see the disagreement below |
| SCENE_SETUP | `scene_setup` | |
| CHECK_AT_BOUNDARY | `check_at_boundary(pos)` | |
| DISK_READ, DISK_WRITE, RWTS_DISPATCH | `disk_read`, `disk_write`, `rwts_dispatch(command)` | true on an error |
| SCENE_LOOP | `scene_loop(scene)` | |
| LOAD_SCENE_DATA | static `load_scene_data` | |
| INTEGRITY_FAIL_ADDR | `integrity_fail` | |
| ENTER_LOCATION | `enter_location` | |
| DRAW_MAP_COL | static `draw_map_col` | |
| DRAW_MAP_ENTITIES | static `draw_map_entities` | |
| DRAW_MAP_ICON_B | static `draw_map_icon_b` | |
| FILL_MAP | `fill_map` | |
| LOAD_MAP_TILES | static `load_map_tiles` | |
| CLEAR_MAP | `clear_map` | |
| LOAD_SCENE_FROM_DISK | static `load_scene_from_disk(scene)` | `.text.scene_disk`, true on an error |
| DISK_READ_JUMP, DISK_RTS | folded | a JMP to DISK_READ and an RTS |

The variables: `DEFAULT_CHAR` (initially `$2C`) and `BLINK_ALT_CHAR`
(initially 0), which the header declares, and the statics
`MAP_FILL_CHAR` (initially `$25`, which `fill_map` uses when GAME_INIT
calls it before any location has loaded its tiles), `MAP_REDRAW_FLAG`,
and `TILE_FILL_TAB`. BLINK_DELAY is a `#define`, because nothing writes
it.

Not translated: CLEAR_STRENGTH_FLAGS, LOAD_SCENE_DISK_ENTRY, and
SETUP_AND_READ_DISK, which have no callers that run.
TUNE_END_MARKER (`$A440`) stays with the tunes.

Every routine of the unit is translated, and none is incomplete.

## Departures

- `load_map_tiles` stores `$2C` in DEFAULT_CHAR and 0 in
  BLINK_ALT_CHAR as constants. The original reads them from
  TILE_DEFAULT_TAB and TILE_BLINK_TAB, which hold those bytes for all
  four styles. The stores are in the original's order.
- `draw_map_col` computes the cell's position with `colrow_to_pos`.
  DRAW_MAP_COL's own loop stops adding 20 when the sum passes 255, and
  no location's corner (the largest is position 196) is below row 9,
  so the two never differ.
- `check_at_boundary` takes the position as a parameter, where
  CHECK_AT_BOUNDARY reads LOCATION_ID, which FIND_ENTITY_AT_POS stores
  just before. It returns ENTITY_EDGE or ENTITY_NONE where the original
  stores 1 or 0 in `$BE`/`$BF`.
- `scene_loop` copies ENTITY_PTR and MOB_PTR to SAVED_ENTITY_PTR and
  SAVED_MOB_PTR and back, where the original calls SAVE_ZP_CONTEXT and
  RESTORE_ZP_CONTEXT, which also copy `$BC` and `$BE` to `$5A9A` and
  `$5A9E` and back. Those two are locals and parameters in C. One
  caller depends on `$BE` surviving SCENE_LOOP: HANDLE_EVENT_SLOT
  reads EVENT_PTR at `$1E43` after `SCENE_LOOP(#$13)`. In C that is
  combat.c's parameter `ev`, which survives the call as any local
  does.
- `load_scene_from_disk` takes the scene as a parameter where the
  original reads SCENE_NUMBER (scene_loop passes SCENE_NUMBER), keeps
  the track and the count of tries in locals where the original uses
  `$BE`, and drops the three calls of DISK_RTS, an RTS.
- `integrity_fail` is a function. In the original it is the error tail
  of LOAD_SCENE_DATA, which VERIFY_INTEGRITY reaches with a JMP.
- `tune_player` is passed 0 as the strength. The original's Y at its
  calls is what the previous routine left, and only tunes 1 and 2 read
  it.
- `scene_loop` doesn't reproduce TEXT_RENDERER's jump to MOVE_LOOP on
  a stream of 256 bytes with no `$7F`, which text.c leaves out.

## Where the original's comments and code disagree

The C follows the code in each case.

- SETUP_SCENE_DISPLAY. Its code is `STOB #$00,AI_WAIT_FLAG; BANE
  CHAR_AI_MODE,.not_ai`, which compares CHAR_AI_MODE with the 0 in A
  and skips the tune when CHAR_AI_MODE is nonzero. So the turn of a
  mob with CHAR_AI_MODE 0, a player character, gets AI_WAIT_FLAG = 10,
  TUNE_AI_TURN, and 20 WAIT calls, and an AI's turn gets none of them.
  The comments ("AI turns get a 10-tick wait and the AI tune", "branch
  if not AI") say the opposite, and so do the header's comment on
  `setup_scene_display` and UNITS.md.
- SETUP_SCENE_DISPLAY's ROL takes its carry from the end of
  HRCG_COUT, which LOAD_CHAR_NAME_PTR reaches last with ctrl-K
  (`$8B`). HRCG_PROCESS_CHAR's second `ASL HRCG_MODE_FLAGS` puts the
  flags' bit 6 in the carry, and HRCG_CTRL_K returns with the carry
  set only when it is set. Bit 6 is set only by HRCG_CTRL_O (`STOB
  #$40`), so the carry is set only when the character before the ctrl-K
  was ctrl-O. The character before it is the last character of the
  mob's name (or the `'0'` of ctrl-A when the name is empty), so the
  carry is clear and the wait is 0 or 20 WAIT calls.
- LOAD_MAP_TILES's comment says the style index is bits 7-6. Four ROLs
  and `AND #$03` give bits 6-5.
- DRAW_MAP_ENTITIES's comment on `BYEQ #$00` says "char 0 -> always
  blink". The Y it tests is APPEARANCE_TO_FONTCHAR's font group, so a
  player character (group 0) always blinks.
- UNITS.md says LOAD_SCENE_FROM_DISK reads DISK_MOTOR_ON. The `BIT
  $C0E9` is in LOAD_SCENE_DATA, and `load_scene_data` reads it.

## Changes needed in ali_baba.h

Only a comment. The comment on `setup_scene_display` should read
"with TUNE_AI_TURN and a wait of 20 WAIT calls for a player character's
turn (CHAR_AI_MODE == 0)". UNITS.md's note on SETUP_SCENE_DISPLAY says
the same thing the wrong way round. The declaration is right.

## Compiler gaps

These don't stop the unit from compiling. Each costs bytes.

1. A 16-bit store to a member at an offset through a pointer builds
   the member's address in a temporary and in DPTR.

   ```c
   struct s { uint8_t a[13]; uint16_t g; };
   extern struct s *P [[c6502::zeropage(0xF4)]];
   void f(void) { P->g = 0; }
   ```

   is 26 bytes, where `LDA #0; LDY #13; STA (P),Y; INY; STA (P),Y` is
   9. `scene_setup` (`MOB_PTR->gold = 0`) and `load_scene_from_disk`
   (`iob->buffer_addr = SCENE_TEXT_DATA`) each pay it.

2. A pre-increment of a global in a loop's condition goes through a
   temporary.

   ```c
   extern uint8_t X [[c6502::address(0x5A0C)]];
   extern uint8_t M [[c6502::address(0x5A08)]];
   void g(void);
   void loop_pre(void) { do { g(); } while (++X <= M); }
   ```

   compiles to `LDA X; CLC; ADC #1; STA tmp; STA X; LDA M; CMP tmp`,
   where the same loop with `X++;` as a statement before the test
   compiles to `INC X; LDA M; CMP X`. The unit's loops are written the
   second way.

## Size

Code bytes in the `-S -O` output, from `objdump.py -t map.o`, against
the original's bytes for the same routines. The compiler moved code
that several functions share into helpers (`draw_map_col@run0` and
`@run4`, `draw_map_entities@run1`, `setup_scene_display@run3`,
`scene_setup@run2`), and each is counted with the routine it came from.

| Original | Bytes | C | Bytes |
|---|---|---|---|
| SETUP_SCENE_DISPLAY | 36 | setup_scene_display (and @run3) | 45 |
| SCENE_SETUP | 124 | scene_setup (and @run2) | 159 |
| CHECK_AT_BOUNDARY | 43 | check_at_boundary | 35 |
| DISK_READ, DISK_WRITE, RWTS_DISPATCH | 28 | disk_read, disk_write, rwts_dispatch | 51 |
| SCENE_LOOP | 87 | scene_loop | 130 |
| LOAD_SCENE_DATA, INTEGRITY_FAIL_ADDR | 34 | load_scene_data, integrity_fail | 42 |
| ENTER_LOCATION | 124 | enter_location (and @run1) | 134 |
| DRAW_MAP_COL | 24 | draw_map_col (and @run0, @run4) | 34 |
| DRAW_MAP_ENTITIES | 208 | draw_map_entities | 224 |
| DRAW_MAP_ICON_B | 48 | draw_map_icon_b | 47 |
| FILL_MAP | 49 | fill_map | 57 |
| LOAD_MAP_TILES | 29 | load_map_tiles | 28 |
| CLEAR_MAP | 14 | clear_map | 14 |
| LOAD_SCENE_FROM_DISK, DISK_READ_JUMP, DISK_RTS | 74 | load_scene_from_disk | 84 |
| Total | 922 | | 1084 |

The C's 1084 bytes are 1000 in `.text` and 84 in `.text.scene_disk`.
The data is 4 bytes of `.rodata` (TILE_FILL_TAB), 2 of `.data`
(DEFAULT_CHAR and MAP_FILL_CHAR), and 2 of `.bss` (BLINK_ALT_CHAR and
MAP_REDRAW_FLAG). The original's tables were 3 + 1 + 1 + 12 = 17 bytes
at `$7AAC`-`$7ADB`.

The largest losses are `scene_loop` (43 bytes, most of them the
16-bit copies to and from SAVED_ENTITY_PTR and SAVED_MOB_PTR, which the
original makes in SAVE_ZP_CONTEXT and RESTORE_ZP_CONTEXT, outside
SCENE_LOOP's 87 bytes), `scene_setup` (35, of which gap 1 is 17), and
the disk routines (23, the carry that `rwts_dispatch` turns into a
`bool` and back with `LDA #0; ROL; ...; CMP #1`).
