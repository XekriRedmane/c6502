# ai.c

## What it translates

Every routine UNITS.md gives the unit, from `main.asm` lines 3434-3528,
11724-12116, and 13470-13826.

| Original | C |
|---|---|
| FIND_NEAREST_EVENT $138D | `find_nearest_event()` |
| MANHATTAN_DISTANCE $13DB | `manhattan_distance(pos, from)` |
| NPC_MOVE_AI $6742 | `npc_move_ai()` |
| EVALUATE_DIRECTION $6815, INC_MOB_COUNTERS $689F | `evaluate_direction(dir, test_pos)` and `event_penalty(ev)`, both static |
| AI_CHOOSE_TARGET $68AB | `ai_choose_target()`, with the party join in the static `join_party()` |
| DISPLAY_SHOP $70DF | `display_shop(ev)` |
| BROWSE_HANDLER $7194 | `browse_handler()` |
| the body DISPLAY_SHOP and BROWSE_HANDLER share | the static `show_item()`, `next_slot()`, and `current_slot()` |
| SHOP_BUY_HANDLER $71AC | `shop_buy_handler()` |
| ADVANCE_SHOP_PTR $71F0 | the index into SHOP_TABLE in `display_shop` |
| ADD_PRICE_COMPONENT $71FC | the multiplication of the price in `show_item` |
| SHOP_BUY_EFFECT_0-4, APPLY_SHOP_EFFECT $720B-$7248 | the static `apply_shop_effect(n)` |
| XP_AWARD_0-4, AWARD_XP, EXTRACT_PENDING_XP $7249-$72A6 | the static `award_xp(n)` and `pending_xp(armor)` |

The unit defines no movable table of the original's. It reads
SHOP_TABLE, SHOP_EFFECT_TABLE, and XP_AWARD_TABLE where the header puts
them, and adds three tables of its own. `WEAPON_EFFECT_ADDRS` and
`ARMOR_EFFECT_ADDRS` hold the ten handler addresses that
shop_buy_handler compares a slot's word with, and `REVERSE_DIR` holds
the direction back from each direction, which NPC_MOVE_AI computes
with an XOR. Every routine is
translated, and none is left incomplete.

## Departures

The file's opening comment lists them. In short:

- `manhattan_distance` takes the position to measure from, so
  EVALUATE_DIRECTION's store of the test square into `ENTITY_PTR->pos`
  and its restore from AI_SAVED_POS ($5A69) are gone, as UNITS.md
  specifies. The stores to FONT_COL and FONT_ROW are kept.
- The scratch bytes AI_DIRECTION, AI_TEST_POS, NEAREST_DIST, $BA, $BB,
  $BE, $BF, and NUM_VALUE are parameters and locals.
- UNITS.md proposes `static uint8_t shop_number` in place of
  SHOP_DATA_PTR. The C keeps the pointer as the static `shop`, which
  display_shop sets to `&SHOP_TABLE[type - $D5]`, so that the address
  isn't computed again on every menu round trip. Types $D7-$DA point
  past the table's two records as in the original.
- The slot is SHOP_ITEM_OFFSET bytes into the record, and the C adds
  that byte offset to the record's address, since the header keeps
  SHOP_ITEM_OFFSET at $5A85 as the original's byte offset.
- The price, which ADD_PRICE_COMPONENT adds up once for each tier and
  once more, is `slot->price * (tier + 1)`. The multiplication calls
  `mul16` (see below).
- SHOP_BUY_HANDLER's `JMP (NUM_VALUE)` is a loop over the two tables of
  addresses that calls `apply_shop_effect(n)` or `award_xp(n)`. A junk
  slot's word, which only the slots that no shipped mask enables hold,
  calls neither effect, where the original would jump into data.
- INC_MOB_COUNTERS's loop of INCs is one addition. A score never comes
  near $80, so its fall into AI_CHOOSE_TARGET can't happen.
- `npc_move_ai` starts its best direction at DIR_NONE. The original
  leaves $BB as it was, but every score is below $7F, so some
  direction always replaces it.
- The direction back is `REVERSE_DIR[best]`, where the original takes
  `best ^ 3` for directions 0-2 and `best ^ 7` for 3-4. The table keeps
  the XOR's DIR_LEFT for DIR_NONE.
- award_xp lowers the armor in a local and stores it once, where
  AWARD_XP stores each value it tries. apply_shop_effect stores the
  weapon once, where SHOP_BUY_EFFECT stores it and then adds the bonus.

The quirks UNITS.md lists are kept. FIND_NEAREST_EVENT returns the
previous NEAREST_POS when nothing qualifies, the shop's slot search
never ends with an empty mask, AWARD_XP never ends when the level plus
the pending amount is 0, the group walks are do-while loops that call
CHECK_HOSTILE once even on an empty list, and they test only MOB_PTR's
high byte (`IN_PAGE_0`). The name copy in `show_item` runs down to a
negative index as the original's does, so a length of 0 copies one
byte to `S_PAY_FOR.item[255]`.

## Where the original's comments and code disagree

The C follows the code in each case.

- DISPLAY_SHOP's comment gives the tier as LOCATION_STYLE bits 7-6.
  Four ROLs through the carry and `AND #3` give bits 6-5.
- APPLY_SHOP_EFFECT's comment gives the bonus as strength bits 2-1.
  Three RORs and `AND #3` give bits 4-3.
- AWARD_XP's comment says it adds the award to byte 8. It stores the
  award over the whole byte.
- EVALUATE_DIRECTION's comment says +9 when already at the target. The
  code adds 9 when the test square differs from AI_TARGET_POS.
- AI_CHOOSE_TARGET's comment says mode 2 seeks an encounter when the
  strength exceeds the life. The code seeks one when the life is at
  least the strength.
- EVALUATE_DIRECTION's comment calls a result with bit 15 set a player
  character. FIND_ENTITY_AT_POS sets that bit for any mob.

## Header

No change is needed. The unit uses the header's declarations as they
are.

## Compiler bugs and gaps

Items 1, 3, and 4 of the earlier list were fixed on 18 Nov 2026: a
crash on `keep` before a `for (;;)` with a `break`, a `uint16_t` member
read through a pointer that built the member's address, and a
comparison of two masked bytes made as a signed 16-bit comparison. The
earlier item 2, `&SHOP_TABLE[n]` calling `mul8`, compiles inline now,
as `n * 41` in shifts and adds, so display_shop indexes the table.

None of the gaps below is a miscompile.

1. **A byte times a small byte plus 1 calls `mul16`.** show_item's
   price is a byte times 1-4, whose result takes 16 bits.

   ```c
   #include <stdint.h>
   extern uint16_t PRICE [[c6502::address(0x5A81)]];
   void f(uint8_t price, uint8_t tier) { PRICE = price * (tier + 1); }
   ```

   compiles to `STA HARGS; TXA; CLC; ADC #1; STA HARGS+2; LDA #0; ADC
   #0; STA HARGS+3; LDA #0; STA HARGS+1; JSR mul16` and two copies out
   of HARGS+4 and HARGS+5. The old C added the price tier + 1 times in
   a loop of 16-bit adds, as the original does, and show_item is 4
   bytes larger with the call. The call also makes the linked program
   depend on a runtime helper, which this repository doesn't have as
   6502 code (UNITS.md, "Compiler gaps", item 6). With `price * (((style
   >> 5) & 3) + 1)`, where `style` is a parameter, the shift also calls
   `asr16`, where the shift alone compiles to five `LSR`s.

2. **A pointer to one of two members of ENTITY_PTR's record is
   computed as a 16-bit address**, as in notes/combat.md, gap 3.
   apply_shop_effect's `n < 3 ? &ENTITY_PTR->sword : &ENTITY_PTR->dagger`
   costs 7 bytes against the old C's field index.

3. **A compound assignment to a member through a zero-page pointer,
   with a call on its right side, computes the member's address before
   the call.**

   ```c
   #include <stdint.h>
   struct mob { uint8_t pad[11]; uint8_t level; };
   extern struct mob *E [[c6502::zeropage(0xF8)]];
   uint8_t h(uint8_t x);
   void f(uint8_t x) { E->level |= h(x); }
   ```

   compiles to `LDA E; CLC; ADC #$0B; STA t; LDA E+1; ADC #0; STA t+1;
   LDY #0; LDA (t),Y; ...; JSR h; ORA ...; LDY #0; STA (t),Y`, where
   calling h first and then `LDY #$0B; ORA (E),Y; STA (E),Y` would do,
   since C leaves the order of the two sides open. award_xp's
   `ENTITY_PTR->level |= total - pending_xp(armor)` has this shape, and
   award_xp is 21 bytes larger than the old C, which read the pending
   amount into a local in its loop.

4. **The static `shop` stays in `.bss`.** With the loop that stepped it
   gone, `-O` no longer promotes it into zero page, and `shop->scene`
   copies it into DPTR for the read. display_shop is 11 bytes larger,
   most of it the inline multiplication by 41 and the copy.

5. **A `switch` on a `uint16_t` with ten cases that share a high byte**
   compiles to a chain that tests the high byte again before every low
   byte. With the switch, shop_buy_handler was 357 bytes. With a loop
   over two tables of the addresses it is 140 bytes and the tables 20.

6. Smaller things seen in the listing: `STA AI_BEST_DIST; ORA #$00;
   BEQ` sets the flags again after a store that already set them, and
   a `for (i = n; i-- != 0;)` loop whose index lives in zero page
   compiles to `DEC; LDA; CMP #$FF; BNE`.

## Size

Code bytes of the `-S -O` output, from the object's symbol sizes,
against the original's from `main.lst`.

| Original | Bytes | C | Bytes |
|---|---|---|---|
| FIND_NEAREST_EVENT | 78 | find_nearest_event | 82 |
| MANHATTAN_DISTANCE | 43 | manhattan_distance | 66 |
| NPC_MOVE_AI | 211 | npc_move_ai | 217 |
| EVALUATE_DIRECTION, INC_MOB_COUNTERS | 150 | evaluate_direction, event_penalty | 174 |
| AI_CHOOSE_TARGET | 285 | ai_choose_target, join_party | 289 |
| DISPLAY_SHOP, BROWSE_HANDLER, ADVANCE_SHOP_PTR, ADD_PRICE_COMPONENT | 232 | display_shop, browse_handler, show_item, next_slot (and @run0), current_slot | 321 |
| SHOP_BUY_HANDLER | 68 | shop_buy_handler | 140 |
| SHOP_BUY_EFFECT_0-4, APPLY_SHOP_EFFECT | 62 | apply_shop_effect | 63 |
| XP_AWARD_0-4, AWARD_XP, EXTRACT_PENDING_XP | 94 | award_xp, pending_xp | 107 |
| **Total** | **1223** | | **1459** |

The C also has 25 bytes of read-only data, the two tables of handler
addresses and REVERSE_DIR, and the 2 bytes of `shop` in `.bss`. It is
236 bytes (19%) larger in code. The largest losses are in show_item
and shop_buy_handler, where much of the difference is the copies of
zero-page parameters around calls and the zero-page loop counters of
gap 6.

Before the rewrite of 18 Nov 2026 the unit's code was 1427 bytes, with
7 bytes of `__zp_init` for `shop` beside it. award_xp grew by 21 bytes
(gap 3), display_shop by 11 (gap 4), apply_shop_effect by 7 (gap 2),
and show_item by 4 (gap 1), and npc_move_ai shrank by 13 with
REVERSE_DIR's 5 bytes in place of the XORs.

## For the integrator

- `shop` is a static in the unit's `.bss`, and ai.c has no `__zp_init`.
- show_item calls the runtime helper `mul16`, so the program links only
  with `--defsym` for it (the tests use `sim.linked.defsym_args()`).
- display_shop, browse_handler, and shop_buy_handler return
  `CHAIN_START_GAME_INPUT`, `CHAIN_START_GAME_INPUT`, and
  `CHAIN_BROWSE_HANDLER` (or `CHAIN_RETURN` from display_shop for an AI
  mob), so run_chain has to map `CHAIN_BROWSE_HANDLER` and
  `CHAIN_SHOP_BUY_HANDLER` to these functions.
- `find_nearest_event` reads `ENTITY_PTR->pos`, so map's ENTER_LOCATION
  has to leave ENTITY_PTR at CANDIDATE_POS before calling it.
