# events.c

## What it translates

All of UNITS.md's events.c routines, from `main.asm` lines 3158-3266,
5106-5151, 11215-11723, 12933-13049, and 13070-13361:

| Original | Address | C |
|---|---|---|
| CHECK_CAN_LEAVE | $1231 | `check_can_leave` |
| UPDATE_EVENT_LIST | $1260 | `update_event_list(pos, value)` |
| TRIGGER_SCENE_EVENT | $1D04 | `trigger_scene_event(ev)` |
| PICKUP_TREASURE | $646B | `pickup_treasure(ev)` |
| INC_STRENGTH, INC_LEVEL | $657B, $6580 | `inc_stat(offset)` (static) |
| CLEAR_TREASURE_DIGITS | $6593 | `clear_treasure_digits` (static) |
| CLEAR_AND_DISPLAY | $65A1 | `clear_and_display(strings)` (static) |
| RANDOM_EVENTS | $65A9 | `random_events` |
| EVENT_PICK_MOB, EVENT_PICK_CHAR, EVENT_FACTION_SCENE | $660D, $6613, $6619 | written out in `random_events` |
| EVENT_FACTION_SCENE_CONT | $661C | `event_faction_scene_cont(index)` (static) |
| EVENT_SPAWN_MOB | $664E | `event_spawn_mob` (static) |
| PICK_RANDOM_MOB, COPY_BE_TO_FA | $6689, $66B1 | `pick_random_mob` (static) |
| PICK_RANDOM_CHAR | $66BA | `pick_random_char` (static) |
| PROCESS_CHAR_EVENTS | $66CE | `process_char_events(index)` (static) |
| FIND_MOB_BY_FACTION | $6706 | `find_mob_by_faction(faction)` (static, bool) |
| CHECK_LOCATION_MATCH | $672E | `check_location_match` (static) |
| CHECK_SCENE_EVENTS (SCENE_LOADER_END) | $6DFA | `check_scene_events` |
| SCAN_FOR_HOSTILE | $6E5A | `scan_for_hostile` (static) |
| PROCESS_SCENE_GROUPS | $6E9E | `process_scene_groups` (static) |
| SCAN_AND_REORDER | $6ED5 | `scan_and_reorder` (static) |
| FLUSH_PENDING_XP | $6EE9 | `flush_pending_xp` (chain) |
| INIT_DROP_GOLD | $6F17 | `init_drop_gold` (chain) |
| INIT_DROP_DISPLAY | $6F52 | `init_drop_display` |
| ADJ_DROP_INC, ADJ_DROP_DEC | $6F72, $6F76 | `adj_drop_inc`, `adj_drop_dec` (chain), sharing `adj_drop(down)` |
| DROP_GOLD_EXEC | $6FB5 | `drop_gold_exec` (chain) |
| FIND_MSB | $7005 | `find_msb(v)` (static) |
| ADD_DROP, SUB_DROP, HALVE_STEP | $7015, $7029, $703D | `+=`, `-=`, and `>>= 1` on DROP_AMOUNT and DROP_STEP |

The unit defines no tables or variables. Everything is translated.

## Departures

- The handlers GAME_TURN_LOOP reaches with the entry in EVENT_PTR take
  it as `ev`. `trigger_scene_event` keeps TRIGGER_SCENE_EVENT's store
  of the pointer to `SAVED_EVENT_PTR` ($5A5B), which nothing reads,
  and doesn't reload it.
- PICKUP_TREASURE copies EVENT_PTR to MOB_PTR and reads the entry
  through MOB_PTR. `pickup_treasure` makes the copy and reads through
  `ev`. Nothing PICKUP_TREASURE calls after the copy writes MOB_PTR in
  the original (a search of every store to $F4/$F5 in main.asm finds
  none on those paths, and SCENE_LOOP restores it), so the two
  pointers stay equal. The C units that `pickup_treasure` calls need to
  keep that true.
- The scratch bytes are locals, parameters, and return values:
  NUM_VALUE, DECIMAL_OUT_PTR, EVENT_PTR, DATA_PTR, CHAR_INDEX ($F6),
  FACTION_MASK ($F7), and TRAP_TYPE ($5A58). PICK_RANDOM_MOB keeps its
  count in MOB_PTR's low byte and stores it there after each location.
  `pick_random_mob` keeps it in a local and stores its last value in
  MOB_PTR's low byte once, which leaves the same byte there.
- INC_STRENGTH and INC_LEVEL pass the field's offset in Y to their
  shared body. `inc_stat` takes a pointer to the field.
- DROP_GOLD_EXEC stores 0 in DROP_STEP's high byte before calling
  MODIFY_CURRENT_STATS and the random step's low byte after it, then
  increments it. `drop_gold_exec` stores the step, both bytes, once
  after the call. Nothing reads DROP_STEP in between.
- INIT_DROP_GOLD runs FIND_MSB on the gold's high byte, and on its
  low byte when the high byte is 0, and stores DROP_AMOUNT a byte at a
  time. `init_drop_gold` runs `find_msb` on the gold as a word and
  stores DROP_AMOUNT once. Both give the highest power of 2 in the
  gold, or 0.
- UPDATE_EVENT_LIST adds the gold's low byte to the entry's value and
  bits 8-10 with the carry to its type. `update_event_list` adds the
  gold to the word whose high byte is the type and low byte the value,
  which gives the same two bytes.
- EVENT_FACTION_SCENE_CONT tests for location 1 by CHAR_PTR's low
  byte, $09. `event_faction_scene_cont` compares CHAR_PTR with
  `&LOCATIONS[1]`. CHAR_PTR holds a record from GET_CHAR_RECORD there,
  which is one of records 0-128, and of those only record 1's address
  has a low byte of $09.
- The chain exits follow the header's rules. FLUSH_PENDING_XP's JMP
  DISPLAY_MESSAGE is a call and `return CHAIN_RETURN`, and ADJ_DROP's
  JMP INIT_DROP_GOLD is `return init_drop_gold();`.

## Where the original's comments and code disagree

The C follows the code in each case.

- CHECK_CAN_LEAVE's header says its input is MOB_PTR. PRINT_MOB_NAME
  reads ENTITY_PTR. Its pass, PRNG_OUTPUT of $40 or more, is one time
  in two, and its comment says about 75%.
- RANDOM_IN_RANGE returns 0 to its limit inclusive, where the comments
  of its callers here say 0 to limit - 1. So a $D4 scene does 1-4
  damage, the traps 1-6 and 2-10, the special chest's bonus comes one
  time in four, and a gold drop loses 1-26 ducats.
- RANDOM_EVENTS' comment calls the burst's test inverted. Its BMI takes
  the burst when the PRNG's output is below EVENT_THRESHOLD, the same
  test as the other five steps.
- CHECK_SCENE_EVENTS's comments call the path with scenes 6, 7, and 8
  the hostile encounter. The code takes it when SCAN_FOR_HOSTILE found
  no mob hostile to mob $40, and plays scene 4 when it found one.
- EVENT_SPAWN_MOB moves a mob of an empty location's faction from that
  location to location 0, the list of mobs in no location, and heals
  it. The name and comment say it activates a character. It's
  EVENT_FACTION_SCENE_CONT that brings a mob from location 0 into a
  location.
- ADJ_DROP_INC moves the amount down and ADJ_DROP_DEC moves it up.
- The SUBROUTINE directives at EVENT_PICK_MOB and EVENT_PICK_CHAR name
  the next routine, which affects only the scope of local labels.

## Header changes

None. One note for the integrator: the task description put
`cold_restart` in main.c, but UNITS.md, `ali_baba.ld`
(`util.o(.cold_restart)`), and the header put it in util.c, and util.c
defines it. main.c doesn't.

## Compiler gaps

The routines were rewritten as plain C on 18 Nov 2026, after the
gaps listed here before were fixed for a read of a 2-byte member
through a zero-page pointer, the low byte of a constant address, `P =
&LOCS[0]`, and `(int16_t)w <= 0`. The C no longer works around them.
These remain.

1. A store to a 2-byte member through a zero-page pointer copies the
   pointer to DPTR and stores the high byte through DPTR.

   ```c
   struct mob { const uint8_t *name; uint8_t b[11]; uint16_t gold; uint8_t flags; };
   extern struct mob *ENTITY_PTR [[c6502::zeropage(0xF8)]];
   void z(void) { ENTITY_PTR->gold = 0; }
   ```

   compiles to `LDA ENTITY_PTR; STA DPTR; LDA ENTITY_PTR+1; STA DPTR+1;
   LDA #0; LDY #$0D; STA (ENTITY_PTR),Y; INY; STA (DPTR),Y`, where `LDA
   #0; LDY #$0D; STA (ENTITY_PTR),Y; INY; STA (ENTITY_PTR),Y` does. It
   costs 8 bytes in `pickup_treasure`'s fourth trap. Fixed on 19 Nov
   2026 (docs/optimizations/pointer_restage.md).

2. A comparison of a 16-bit global with a 2-byte member through a
   zero-page pointer copies both bytes of the member to temporaries.

   ```c
   extern uint16_t W [[c6502::address(0x5A02)]];
   uint8_t c(void) { return W <= ENTITY_PTR->gold; }
   ```

   gives `LDY #$0D; LDA (ENTITY_PTR),Y; STA t1; INY; LDA (ENTITY_PTR),Y;
   STA t0; LDA t1; SEC; SBC W; LDA t0; SBC W+1`, where `LDY #$0D; LDA
   (ENTITY_PTR),Y; CMP W; INY; LDA (ENTITY_PTR),Y; SBC W+1` does. It
   costs 9 bytes in `adj_drop`. Fixed on 19 Nov 2026 except for the
   `CMP`: each byte is read where the subtraction uses it, `LDY #$0D;
   LDA (ENTITY_PTR),Y; SEC; SBC W; INY; LDA (ENTITY_PTR),Y; SBC W+1`
   (docs/optimizations/pointer_self_store.md), and adj_drop is 8 bytes
   smaller.

3. `(int16_t)w <= 0` right after a store to `w` copies both bytes to
   temporaries before the test.

   ```c
   extern uint16_t W [[c6502::address(0x5A02)]];
   extern uint16_t D [[c6502::address(0x5A04)]];
   void f(void); void g(void);
   void d(void) { W -= D; if ((int16_t)W <= 0) f(); else g(); }
   ```

   gives `STA W+1; STA t0; LDA W; STA t1; LDA t0; BMI; ORA t1; BNE`,
   where `STA W+1; BMI; ORA W; BNE` does. It costs 5 bytes in
   `drop_gold_exec`. The old C wrote `(int16_t)W < 0 || W == 0`, which
   compiles to `BMI; LDA W; ORA W+1; BNE`.

4. After a call that returns a byte in A, a test of the byte is `ORA
   #$00` (in `pick_random_mob` and `pickup_treasure`), because the
   flags after the call aren't known. The original has the same kind
   of byte (`CMP #$00`) at PICK_RANDOM_MOB and PICKUP_TREASURE.

Some of the growth comes from the C itself. `inc_stat` takes a
pointer, and each call computes `&ENTITY_PTR->strength` or `->level`
at run time (16 bytes more than passing the offset). `find_msb`
scans a word (17 bytes more, and `init_drop_gold` 16 bytes less).
`DROP_STEP = random_in_range(25) + 1` adds the carry into the high
byte, since the compiler can't know that the sum is below 256, where
the old C truncated it to a byte (2 bytes). The test for location 1
compares both bytes of CHAR_PTR (6 bytes).

## Size

The original's routines take 1,542 bytes (main.lst, the sizes in
UNITS.md's table). The C takes 1,630 bytes in `events.asm`, 88 more.
Before the rewrite to plain C it took 1,561 bytes with the same
compiler.

| Routine | Original | C |
|---|---|---|
| CHECK_CAN_LEAVE | 47 | 38 |
| UPDATE_EVENT_LIST | 112 | 121 |
| TRIGGER_SCENE_EVENT | 73 | 68 |
| PICKUP_TREASURE | 272 | 315 |
| INC_STRENGTH, INC_LEVEL | 24 | 24 |
| CLEAR_TREASURE_DIGITS | 14 | 16 |
| CLEAR_AND_DISPLAY | 8 | 8 |
| RANDOM_EVENTS and the three EVENT_ stubs | 115 | 83 |
| EVENT_FACTION_SCENE_CONT | 50 | 57 |
| EVENT_SPAWN_MOB | 59 | 55 |
| PICK_RANDOM_MOB, COPY_BE_TO_FA | 49 | 65 |
| PICK_RANDOM_CHAR | 20 | 24 |
| PROCESS_CHAR_EVENTS | 56 | 63 |
| FIND_MOB_BY_FACTION | 40 | 40 |
| CHECK_LOCATION_MATCH | 20 | 17 |
| CHECK_SCENE_EVENTS | 96 | 109 |
| SCAN_FOR_HOSTILE | 68 | 59 |
| PROCESS_SCENE_GROUPS | 43 | 43 |
| SCAN_AND_REORDER | 20 | 16 |
| FLUSH_PENDING_XP | 46 | 57 |
| INIT_DROP_GOLD | 59 | 52 |
| INIT_DROP_DISPLAY | 32 | 29 |
| ADJ_DROP_INC, ADJ_DROP_DEC | 67 | 124 |
| DROP_GOLD_EXEC | 80 | 89 |
| FIND_MSB | 16 | 41 |
| ADD_DROP, SUB_DROP, HALVE_STEP | 56 | 17 |
| Total | 1,542 | 1,630 |

The C counts include the outlined fragments of one routine
(`random_events@run1`, `adj_drop@run2`) in that routine's row.
ADD_DROP and HALVE_STEP are inline in C, and the 17 bytes are
`adj_drop@run0`, the outlined `DROP_AMOUNT -= DROP_STEP`, which
`drop_gold_exec` calls too.

The largest losses are the zp_abi argument stores before each call
that takes A or a word in the original, CHECK_SCENE_EVENTS's
comparisons (the original keeps 1 in A across four of them),
PICKUP_TREASURE's gold word kept in two temporaries across
MODIFY_CURRENT_STATS, ADJ_DROP's `DROP_AMOUNT -= 2 * DROP_STEP`
computed in temporaries, and the chain routines' `LDA #state; RTS` in
place of a JMP.
