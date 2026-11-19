# main.c

## What it translates

All of UNITS.md's main.c routines, from `main.asm` lines 1232-1287,
1329-1431, 1503-1625, and 1642-1663:

| Original | Address | C |
|---|---|---|
| MAIN_ENTRY | $0800 | `main`, section `.main` |
| PROCESS_MOB_GROUPS | $0884 | `process_mob_groups` (static) |
| SHOW_IDLE_STATUS | $08D1 | `show_idle_status` (static) |
| START_MOB_TURN | $08DF | `start_mob_turn` (static) |
| CHECK_MOB_NEEDS_TURN | $0958 | `check_mob_needs_turn` (static, bool) |
| TOGGLE_SOUND | $0984 | `toggle_sound` (chain) |
| SETTINGS_DISPLAY_INIT | $09AC | `settings_display_init` |
| DEC_THRESHOLD_WRAP, INC_THRESHOLD_WRAP | $09CB, $09CF | `dec_threshold_wrap`, `inc_threshold_wrap` |
| DEC_EVENT_THRESHOLD, INC_EVENT_THRESHOLD | $09D3, $09DC | `dec_event_threshold`, `inc_event_threshold` (static) |
| ATTRACT_SCREEN | $09F4 | `attract_screen` (static) |

Main defines no tables or variables. `cold_restart` isn't here. UNITS.md
lists it under util.c, `ali_baba.ld` takes `.cold_restart` from
`util.o`, the header declares it in util.c's part, and util.c defines
it.

## Departures

- `start_mob_turn` calls `run_chain(CHAIN_GAME_TURN_LOOP)` where
  START_MOB_TURN jumps to GAME_TURN_LOOP. The chain's last RTS returns
  to PROCESS_MOB_GROUPS in the original, and `run_chain` returns there
  in C.
- `toggle_sound` flips and tests `SOUND_ON` where TOGGLE_SOUND tests
  and patches the first bytes of PLAY_HIGH_TONE and PLAY_LOW_TONE. It
  writes `S_SOUND_IS_ON.state`, the original's S_SOUND_CHAR1 and
  S_SOUND_CHAR2, as the original does, and
  returns `CHAIN_MENU_GAME_SETTINGS` for the JMP MENU_GAME_SETTINGS.
- `dec_threshold_wrap` and `inc_threshold_wrap` are each a tail jump to
  the static routine, where the original has JSR and RTS.

Kept as the original has them:

- DEC_EVENT_THRESHOLD decrements and increments again when the result
  has bit 7 set, so at 0 it writes $FF and then 0. `dec_event_threshold`
  does the same, and compiles to the same DEC, BPL, INC.
- PROCESS_MOB_GROUPS's comment calls its test before VERIFY_DISK_IO a
  1-in-24 chance. The test is `PRNG_OUTPUT & $17`, which is 0 for one
  value in 16. The C makes the same test and the call, although
  `verify_disk_io` does nothing.
- MAIN_ENTRY ends a pass when byte 1 of the next record is $FF. The C
  reads that byte through CHAR_PTR, which the routines called in the
  loop may move, as in the original.

## Header changes

None.

## Compiler gaps

Items 1 and 2 were fixed by 18 Nov 2026. main.c now writes the mask's
comparison without a cast and reads the name's high byte as
`(uint16_t)CHAR_PTR->name >> 8`, and both compile as the workarounds
did.

1. A mask of a byte compared with a small constant is a signed 16-bit
   comparison.

   ```c
   extern uint8_t X1 [[c6502::address(0x5A00)]];
   void f(void);
   void c(void) { if ((X1 & 0x3F) < 3) f(); }
   ```

   compiled to `AND #$3F; LDY #0; SEC; SBC #3; TYA; SBC #0; BVC; EOR
   #$80; BPL`, where `CMP #3; BCS` does, since the value is 0-63.

2. Reading the high byte of a pointer member reads the low byte too.

   ```c
   struct loc { const uint8_t *name; uint8_t a[7]; };
   extern struct loc *P [[c6502::zeropage(0xFA)]];
   extern uint8_t X1 [[c6502::address(0x5A00)]];
   void e(void) { X1 = (uint8_t)((uint16_t)P->name >> 8); }
   ```

   compiled to `LDY #0; LDA (P),Y; INY; LDA (P),Y; STA X1`, and now
   compiles to `LDY #1; LDA (P),Y`.

3. `check_mob_needs_turn` has an `ORA #$00` after `JSR check_encounter`
   to set the flags from A, which the original's `LDA
   ENCOUNTER_RESULT` also spends.

## Size

The original's routines take 381 bytes (main.lst, the sizes in
UNITS.md's table). The C takes 396 bytes in `main.asm`.

| Routine | Original | C |
|---|---|---|
| MAIN_ENTRY | 78 | 78 |
| PROCESS_MOB_GROUPS | 77 | 72 |
| SHOW_IDLE_STATUS | 14 | 16 |
| START_MOB_TURN | 34 | 40 |
| CHECK_MOB_NEEDS_TURN | 44 | 47 |
| TOGGLE_SOUND | 40 | 40 |
| SETTINGS_DISPLAY_INIT | 31 | 33 |
| DEC_/INC_THRESHOLD_WRAP | 8 | 6 |
| DEC_/INC_EVENT_THRESHOLD | 20 | 29 |
| ATTRACT_SCREEN | 35 | 35 |
| Total | 381 | 396 |

The C's extra bytes are the zp_abi argument stores (`STA
__zpabi_...`) before calls that take A in the original, and
START_MOB_TURN's JMP to GAME_TURN_LOOP becoming a store of the chain
state and a JMP to `run_chain`.
