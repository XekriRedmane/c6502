# turn.c

## What was translated

Every routine UNITS.md gives turn, and the new dispatcher.

| Original | C |
|---|---|
| TURN_DISPATCH | `turn_dispatch`, static, chain |
| GAME_TURN_LOOP | `game_turn_loop`, static, chain, returns CHAIN_MOVE_LOOP |
| MOVE_LOOP | `move_loop`, static, chain |
| COMMIT_MOVE | `commit_move`, which ends with `reset_turn_pos()` |
| RESET_TURN_POS | `reset_turn_pos` |
| END_TURN | `end_turn`, static, chain |
| SET_STATE_FIGHT | `set_state_fight`, static |
| CHECK_AND_RESTRICT | `check_and_restrict`, static |
| SET_CHAR_STATE | `set_char_state` |
| CLEAR_RESTRICTED, SET_RESTRICTED | `clear_restricted`, `set_restricted` |
| (new) | `run_chain` |

`TURN_ACTIVE` is defined here, initially `0x4B`. `-O` promotes it into
zero page, so the linker's `__zp_init` stores its initial value. Nothing
reads it before turn writes it.

`run_chain` is a loop over a switch with a case for every `CHAIN_`
state in the header. The five states that name ordinary routines
(CHAIN_SET_STATE_FIGHT, CHAIN_AUTO_HEAL, CHAIN_ENTER_COMBAT_STATE with
position `0x14`, CHAIN_RESOLVE_ATTACK, CHAIN_RETIRE_CHARACTER) call
the routine and stop the chain. An unknown state stops the chain too.
c6502 compiles the switch to a table of 59 case addresses.

Every exit follows UNITS.md's table for MOVE_LOOP and the report's
tables for TURN_DISPATCH and END_TURN. Nothing was left out or left
incomplete.

## Departures

The file's header lists them. In short:

- The chain's JMPs are returned states, and JMPs to ordinary routines
  are calls followed by CHAIN_RETURN.
- The entry MOVE_LOOP walked into is the handlers' `ev` parameter,
  where the original passes it in EVENT_PTR ($BE/$BF) and saves it
  across COMMIT_MOVE with PSHW/PULW.
- The MOVE option's enable byte is configure_input's return value.
- MOVE_LOOP's PHA/PLA around CHECK_AND_RESTRICT and SET_CHAR_STATE's
  `$5A60` are locals and parameters.
- `tune_player` gets 0 as its repeat count for TUNE_ENTITY_INTERACTION
  and TUNE_MOVEMENT. TUNE_PLAYER reads Y only for tunes 1 and 2.

Two tests are written for the values that reach them, where the
original's signed test would differ only for values that never occur.

- TURN_DISPATCH's `CMP #2; BMI` on SELECT_COMBAT_TARGET's result is
  `action < 2`. SELECT_COMBAT_TARGET returns only 1, 2, or 4.
- MOVE_LOOP's `CMP #2; BMI` on ENCOUNTER_RESULT is `ENCOUNTER_RESULT
  >= 2` for the other branch. CHECK_ENCOUNTER stores only 0, 1, or 2,
  and MOVE_LOOP stores 0.

END_TURN's `CMP STEPS_TAKEN; BMI` keeps its signed test, since
STEPS_TAKEN can grow past `$80` when a player's MOVE_POINTS counts down
from 255. The AI's up test (`CMP #2; BPL` with a command of 0 to 4) is
`dir <= DIR_UP`, which is the same for every command that reaches it.

## Where the original's comments and code disagree

The C follows the code in each case.

- GAME_TURN_LOOP's comment at `BAEQ #$01` says "exactly 1 threat ->
  keep full". CHECK_THREATS_HERE returns 1 when the mob's defense is
  at least the hostile strength around it, and 0 when it is
  outmatched, so 1 means safe.
- TURN_DISPATCH's header comment says "negative commits an attack via
  START_COMBAT". The BMI follows `CMP #2`, so it is taken for a result
  of 1 (an attack), and SELECT_COMBAT_TARGET returns no negative value.
- END_TURN's header comment says that a level above the steps taken
  grants another turn. The code computes `(speed >> 1) & 7`, and when
  that is below STEPS_TAKEN (signed) it falls into SET_STATE_FIGHT and
  ends the chain. Otherwise it goes to TURN_DISPATCH. UNITS.md says
  the same.

## Header changes needed

None. Every routine keeps the header's declaration.

A suggestion that needs no change now: CHECK_ENCOUNTER's results
0, 1, and 2 appear as bare numbers in combat, events, ai, and turn.
turn.c names them `ENCOUNTER_NONE`, `ENCOUNTER_FRIENDLY`, and
`ENCOUNTER_HOSTILE` locally, and the header could define them for
every unit.

## Compiler gaps

1. **A comparison of a masked byte is done in 16 bits, signed.**
   `(ENTITY_PTR->life & HP_MASK) < 3` compiled to `AND #$3F; LDY #0;
   SEC; SBC #3; TYA; SBC #0; BVC; EOR #$80; BPL`. game_turn_loop casts
   the masked byte to `uint8_t`, which gives `AND #$3F; CMP #3; BCS`.
   notes/entities.md has the minimal example.
2. **Stores to different globals are reordered.** commit_move stores
   FONT_COL, BLINK_COL, FONT_ROW, and BLINK_ROW in the original's
   order, and the listing stores FONT_COL, FONT_ROW, BLINK_COL,
   BLINK_ROW. The bytes are the same once the routine returns.
3. **The chain costs a dispatcher.** The original's JMPs between
   chain routines cost nothing beyond the JMP. run_chain is 506 bytes,
   a 118-byte table of case addresses and 6 to 13 bytes a case for
   the JSR, the state, and the JMP back to the switch. A table of the
   routines' addresses less 1, with an RTS trick in place of the JSR,
   would be about 120 bytes plus the loop, but c6502 can't call
   through a function pointer (UNITS.md, gap 1). Every chain exit
   that returns a constant state also costs `LDA #n; RTS` where the
   original has `JMP x`, and every call of a handler with `ev` copies
   the pointer into the callee's zero-page parameter.

No compiler crash or wrong code.

## Size

The bytes of code in `turn.c -c -O` (the `.text` sections, from
`objdump.py -h`), against the original's code bytes in `main.lst`:

| Routine | Original | C |
|---|---|---|
| TURN_DISPATCH | 20 | 39 |
| GAME_TURN_LOOP and MOVE_LOOP | 421 | 129 + 14 + 9 + 348 + 5 + 7 |
| COMMIT_MOVE and RESET_TURN_POS | 85 | 58 + 46 |
| END_TURN | 31 | 34 |
| SET_STATE_FIGHT | 5 | 7 |
| CHECK_AND_RESTRICT | 21 | 18 |
| SET_CHAR_STATE | 15 | 11 |
| CLEAR_RESTRICTED | 9 | 9 |
| SET_RESTRICTED | 9 | 9 |
| Total of the original's routines | 616 | 743 |
| run_chain | | 506 |
| `__zp_init` for TURN_ACTIVE | | 5 |
| Total | 616 | 1254 |

The `+` terms are pieces the optimizer outlined into shared
subroutines, counted under the routine they are named after
(`move_loop@run2` is the call of colrow_to_pos that commit_move and
reset_turn_pos share too). move_loop loses most of its bytes to the
handlers' parameters and to `JSR x; LDA #0; RTS` in place of `JMP x`.
