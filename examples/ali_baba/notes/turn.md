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

Item 1 was fixed by 18 Nov 2026, and turn.c now writes the comparison
without a cast.

1. **A comparison of a masked byte is done in 16 bits, signed.**
   `(ENTITY_PTR->life & HP_MASK) < 3` compiled to `AND #$3F; LDY #0;
   SEC; SBC #3; TYA; SBC #0; BVC; EOR #$80; BPL`, and now compiles to
   `AND #$3F; CMP #3; BCS`. notes/entities.md has the minimal example.
2. **A chained assignment reads its source again for a later use.**

   ```c
   #include <stdint.h>
   extern uint8_t A1 [[c6502::address(0x5A0C)]], A2 [[c6502::address(0x5AA2)]], C1 [[c6502::address(0x5A63)]];
   extern uint8_t B1 [[c6502::address(0x5A0D)]], B2 [[c6502::address(0x5AA3)]], C2 [[c6502::address(0x5A64)]];
   uint8_t pos(uint8_t c, uint8_t r);
   uint8_t f(void) {
       A1 = A2 = C1;
       B1 = B2 = C2;
       return pos(C1, C2);
   }
   ```

   compiles to `LDA C1; STA A2; STA A1; LDA C2; STA B2; STA B1; LDX
   C2; LDA C1; JMP pos`. The stores go to other objects at fixed
   addresses, so C1 and C2 are unchanged, and `TAX` and a reload of
   C1 alone, or `LDX C2` before the second pair of stores, would do.
   `commit_move` writes FONT_COL, BLINK_COL, FONT_ROW, and BLINK_ROW
   this way, and it is 6 bytes larger than with locals for the column
   and row (50 to 56 bytes). The stores also come out in the order
   BLINK_COL, FONT_COL, BLINK_ROW, FONT_ROW, where the original stores
   FONT_COL first, and the bytes are the same once the routine
   returns.

   Partly fixed on 19 Nov 2026. The `LDX` of the row after its stores
   is `TAX` (docs/optimizations/redundant_load.md), and commit_move is
   54 bytes. The column is still loaded again, since A holds the row
   by then.
3. **A test that repeats one the code has already made is made
   again.** `move_loop` tests `ENTITY_IS_MOB(e)` and returns, and then
   `!ENTITY_IS_EVENT(e)`, which is `!(e > 0x00FF && !ENTITY_IS_MOB(e))`.

   ```c
   #include <stdint.h>
   uint16_t find(void);
   uint8_t f(void) {
       uint16_t e = find();
       if (e & 0x8000) return 1;
       if (!(e > 0xFF && !(e & 0x8000))) return 2;
       return 3;
   }
   ```

   compiles the second test to `LDA #$FF; SEC; SBC e; LDA #0; SBC
   e+1; BCS two; TXA; BPL three`. The bit-15 test is known false
   there, and `e > 0xFF` is `e+1 != 0`, so `TXA; BNE three` would do.
   The old C's `(e >> 8) == 0` gave `TXA; BNE`. In `move_loop` the
   compiler also keeps the bit-15 test as a byte (`AND #$80; STA t`)
   for the second test. The two tests take 17 bytes more than the old
   C's, and `type < EVENT_ACTIVATED`, which replaced `(type &
   EVENT_ACTIVATED) != EVENT_ACTIVATED`, saved 6, so move_loop grew 11
   bytes (316 to 327).

   Partly fixed on 19 Nov 2026. `e > 0xFF` is a test of the high byte,
   `TXA; BEQ two` (docs/optimizations/cmp_zero_jump_fold.md), and
   move_loop is 319 bytes. The bit-15 test is still made again. In
   the minimal example above it's a `BPL three` after the `BEQ two`,
   whose `TXA` goes (docs/optimizations/redundant_load.md), and in
   move_loop it's the byte `AND #$80; STA t` described above.
4. **The chain costs a dispatcher.** The original's JMPs between
   chain routines cost nothing beyond the JMP. run_chain is 461 bytes,
   a 118-byte table of case addresses and 6 to 13 bytes a case for
   the JSR, the state, and the JMP back to the switch. A table of the
   routines' addresses less 1, with an RTS trick in place of the JSR,
   would be about 120 bytes plus the loop, but c6502 can't call
   through a function pointer (UNITS.md, gap 1). Every chain exit
   that returns a constant state also costs `LDA #n; RTS` where the
   original has `JMP x`, and every call of a handler with `ev` copies
   the pointer into the callee's zero-page parameter. The five cases
   that call an ordinary routine return from run_chain, which took 41
   bytes off it.

No compiler crash or wrong code.

## Size

The bytes of code in `turn.c -c -O` (`nm.py -S`), against the
original's code bytes in `main.lst`. The last column is the C before
it was rewritten as plain C on 18 Nov 2026.

| Routine | Original | C | Before |
|---|---|---|---|
| TURN_DISPATCH | 20 | 37 | 37 |
| GAME_TURN_LOOP and MOVE_LOOP | 421 | 145 + 327 | 145 + 316 |
| COMMIT_MOVE and RESET_TURN_POS | 85 | 56 + 42 | 50 + 42 |
| END_TURN | 31 | 34 | 34 |
| SET_STATE_FIGHT | 5 | 5 | 5 |
| CHECK_AND_RESTRICT | 21 | 22 | 22 |
| SET_CHAR_STATE | 15 | 13 | 13 |
| CLEAR_RESTRICTED | 9 | 9 | 9 |
| SET_RESTRICTED | 9 | 9 | 9 |
| Total of the original's routines | 616 | 699 | 682 |
| run_chain | | 461 | 502 |
| `__zp_init` for TURN_ACTIVE | | 5 | 5 |
| Total | 616 | 1165 | 1189 |

move_loop loses most of its bytes to the handlers' parameters and to
`JSR x; LDA #0; RTS` in place of `JMP x`.
