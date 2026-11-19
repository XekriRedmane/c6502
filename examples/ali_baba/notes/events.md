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
  count in MOB_PTR's low byte, and `pick_random_mob` does too.
- DROP_GOLD_EXEC stores 0 in DROP_STEP's high byte before calling
  MODIFY_CURRENT_STATS and the random step's low byte after it, then
  increments it. `drop_gold_exec` stores the step, both bytes, once
  after the call. Nothing reads DROP_STEP in between.
- INIT_DROP_GOLD stores DROP_AMOUNT's low byte (0) and then its high
  byte. `init_drop_gold` stores the word, high byte first in the
  listing, and when the high byte is 0 it stores the word again with
  the low byte's mask, which writes 0 to the high byte a second time.
- The chain exits follow the header's rules. FLUSH_PENDING_XP's JMP
  DISPLAY_MESSAGE is a call and `return CHAIN_RETURN`, and ADJ_DROP's
  JMP INIT_DROP_GOLD is `return init_drop_gold();`.
- ENTITY_PTR's gold and CHAR_PTR's event pointer are read a byte at a
  time through the function-like macros `ENTITY_BYTE` and `CHAR_BYTE`,
  as a workaround for compiler gap 1 below. In
  `adj_drop`, `pickup_treasure`, `update_event_list`, and
  `process_char_events` that saved 64 bytes.

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

Item 1 was fixed on 18 Nov 2026 for a read. A store still computes the
address. Items 2, 3, and 4 were fixed on the same day, and events.c
keeps its workarounds.

1. A 2-byte member at a nonzero offset, read or written through a
   zero-page pointer, has its address computed first.

   ```c
   struct mob { const uint8_t *name; uint8_t b[11]; uint16_t gold; uint8_t flags; };
   extern struct mob *ENTITY_PTR [[c6502::zeropage(0xF8)]];
   extern uint16_t X2 [[c6502::address(0x5A02)]];
   void b(void) { X2 = ENTITY_PTR->gold; }
   ```

   compiles to `LDA ENTITY_PTR; CLC; ADC #$0D; STA t; LDA ENTITY_PTR+1;
   ADC #0; STA t+1; LDY #0; LDA (t),Y; STA X2; INY; LDA (t),Y; STA
   X2+1`, where `LDY #$0D; LDA (ENTITY_PTR),Y; STA X2; INY; LDA
   (ENTITY_PTR),Y; STA X2+1` does. Reading the bytes by offset through a
   `uint8_t *` cast gives the short form, and that is the workaround
   here. A 1-byte member and a 2-byte member at offset 0 compile to the
   short form already. Storing to such a member (`ENTITY_PTR->gold =
   0;`) also computes the address, and copies it to DPTR as well.

2. The low byte of a constant address is computed at run time.

   ```c
   struct loc { const uint8_t *name; uint8_t a[7]; };
   extern struct loc LOCS [[c6502::address(0x4000)]][63];
   extern struct loc *P [[c6502::zeropage(0xFA)]];
   extern uint8_t X1 [[c6502::address(0x5A00)]];
   void a(void) { if ((uint8_t)(uint16_t)P == (uint8_t)(uint16_t)&LOCS[1]) X1 = 0; }
   ```

   compiles to `LDA #<LOCS; CLC; ADC #$09; STA t; LDA P; CMP t`, where
   `LDA P; CMP #<(LOCS+9)` does. `event_faction_scene_cont` compares
   with the constant `LOCATION_1_LO`.

3. Storing an array's address in a pointer adds 0 with carry.

   ```c
   void b(void) { P = &LOCS[0]; }
   ```

   compiles to `LDA #<LOCS; CLC; STA P; LDA #>LOCS; ADC #0; STA P+1`.
   `P = LOCS;` compiles to the four instructions without the CLC and
   ADC, and `event_faction_scene_cont` writes it that way.

4. `(int16_t)w <= 0` compiles to a 16-bit subtraction from 0 with an
   overflow fix-up.

   ```c
   extern uint16_t W [[c6502::address(0x5A02)]];
   void f(void);
   void d(void) { if ((int16_t)W <= 0) f(); }
   ```

   gives `LDA #0; SEC; SBC W; LDA #0; SBC W+1; BVC; EOR #$80; BMI`.
   `drop_gold_exec` writes `(int16_t)DROP_AMOUNT < 0 || DROP_AMOUNT ==
   0`, which compiles to a BMI on the subtraction's high byte and `LDA;
   ORA; BNE`, as the original tests it.

5. After a call that returns a byte in A, a test of the byte is `ORA
   #$00` (in `pick_random_mob`, `pickup_treasure`, and `init_drop_gold`),
   because the flags after the call aren't known. The original has the
   same kind of byte (`CMP #$00`) at PICK_RANDOM_MOB and PICKUP_TREASURE.

## Size

The original's routines take 1,542 bytes (main.lst, the sizes in
UNITS.md's table). The C takes 1,667 bytes in `events.asm`, 125 more.

| Routine | Original | C |
|---|---|---|
| CHECK_CAN_LEAVE | 47 | 40 |
| UPDATE_EVENT_LIST | 112 | 128 |
| TRIGGER_SCENE_EVENT | 73 | 68 |
| PICKUP_TREASURE | 272 | 313 |
| INC_STRENGTH, INC_LEVEL | 24 | 18 |
| CLEAR_TREASURE_DIGITS | 14 | 19 |
| CLEAR_AND_DISPLAY | 8 | 16 |
| RANDOM_EVENTS and the three EVENT_ stubs | 115 | 88 |
| EVENT_FACTION_SCENE_CONT | 50 | 42 |
| EVENT_SPAWN_MOB | 59 | 48 |
| PICK_RANDOM_MOB, COPY_BE_TO_FA | 49 | 65 |
| PICK_RANDOM_CHAR | 20 | 28 |
| PROCESS_CHAR_EVENTS | 56 | 62 |
| FIND_MOB_BY_FACTION | 40 | 38 |
| CHECK_LOCATION_MATCH | 20 | 19 |
| CHECK_SCENE_EVENTS | 96 | 119 |
| SCAN_FOR_HOSTILE | 68 | 67 |
| PROCESS_SCENE_GROUPS | 43 | 46 |
| SCAN_AND_REORDER | 20 | 12 |
| FLUSH_PENDING_XP | 46 | 61 |
| INIT_DROP_GOLD | 59 | 73 |
| INIT_DROP_DISPLAY | 32 | 36 |
| ADJ_DROP_INC, ADJ_DROP_DEC | 67 | 98 |
| DROP_GOLD_EXEC | 80 | 87 |
| FIND_MSB | 16 | 22 |
| ADD_DROP, SUB_DROP, HALVE_STEP | 56 | 20 |
| Fragments the optimizer outlined and shared | | 34 |
| Total | 1,542 | 1,667 |

The C counts include the outlined fragments of one routine
(`check_can_leave@run3`, `random_events@run1`, `init_drop_gold@run6`)
in that routine's row. ADD_DROP and HALVE_STEP are inline in C, and
the 20 bytes are `adj_drop@run0`, the outlined `DROP_AMOUNT -=
DROP_STEP`. The shared row is `trigger_scene_event@run4` and `@run5`
and `event_spawn_mob@run2` and `@run7`.

The largest losses are the zp_abi argument stores before each call
that takes A or a word in the original, CHECK_SCENE_EVENTS's
comparisons (the original keeps 1 in A across four of them),
PICKUP_TREASURE's gold word kept in two temporaries across
MODIFY_CURRENT_STATS, and the chain routines' `LDA #state; RTS` in
place of a JMP.
