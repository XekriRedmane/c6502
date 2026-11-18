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
| ADVANCE_SHOP_PTR $71F0 | the loop in `display_shop` that steps `shop` |
| ADD_PRICE_COMPONENT $71FC | the loop in `show_item` that multiplies the price |
| SHOP_BUY_EFFECT_0-4, APPLY_SHOP_EFFECT $720B-$7248 | the static `apply_shop_effect(n)` |
| XP_AWARD_0-4, AWARD_XP, EXTRACT_PENDING_XP $7249-$72A6 | the static `award_xp(n)` and `pending_xp(armor)` |

The unit defines no movable table of the original's. It reads
SHOP_TABLE, SHOP_EFFECT_TABLE, and XP_AWARD_TABLE where the header puts
them, and adds two tables of its own, `WEAPON_EFFECT_ADDRS` and
`ARMOR_EFFECT_ADDRS`, which hold the ten handler addresses that
shop_buy_handler compares a slot's word with. Every routine is
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
  display_shop steps one record at a time from SHOP_TABLE, as
  ADVANCE_SHOP_PTR does. `&SHOP_TABLE[n]` multiplies by the record's
  41 bytes, which calls `mul8` (see below), and a pointer saves
  computing the address on every menu round trip. Types $D7-$DA step
  past the table's two records as in the original.
- The slot is SHOP_ITEM_OFFSET bytes into the record, which the C reads
  through a byte pointer, because `SHOP_ITEM_OFFSET / 5` would call a
  runtime helper.
- SHOP_BUY_HANDLER's `JMP (NUM_VALUE)` is a loop over the two tables of
  addresses that calls `apply_shop_effect(n)` or `award_xp(n)`. A junk
  slot's word, which only the slots that no shipped mask enables hold,
  calls neither effect, where the original would jump into data.
- INC_MOB_COUNTERS's loop of INCs is one addition. A score never comes
  near $80, so its fall into AI_CHOOSE_TARGET can't happen.
- `npc_move_ai` starts its best direction at DIR_NONE. The original
  leaves $BB as it was, but every score is below $7F, so some
  direction always replaces it.

The quirks UNITS.md lists are kept. FIND_NEAREST_EVENT returns the
previous NEAREST_POS when nothing qualifies, the shop's slot search
never ends with an empty mask, AWARD_XP never ends when the level plus
the pending amount is 0, the group walks are do-while loops that call
CHECK_HOSTILE once even on an empty list, and they test only MOB_PTR's
high byte (`IN_PAGE_0`). The name copy in `show_item` runs down to a
negative index as the original's does, so a length of 0 copies one
byte to `SHOP_ITEM_NAME[255]`.

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

1. **A crash on `keep` before a `for (;;)` with a `break`.** This
   fails in `cfg.build_cfg` with `KeyError: '.loop@0_break'`:

   ```c
   #include <stdint.h>
   void f(uint8_t x) {
       [[c6502::keep]] for (;;) {
           if (--x == 0) break;
       }
   }
   ```

   Without `keep` it compiles. Sound.c needed a loop of that shape and
   used `for (; tone_padding++, --x;)` instead (see notes/sound.md).

2. **`&SHOP_TABLE[n]` calls `mul8`.** The record is 41 bytes, which is
   32 + 8 + 1, and the index is a byte, but the multiplication isn't
   done inline. display_shop steps a pointer instead.

3. **A `uint16_t` member read through a pointer builds the member's
   address.** `ENTITY_PTR->gold < SHOP_PRICE` compiles to an addition
   of 13 to ENTITY_PTR into two zero-page pairs and two reads through
   them, 30 bytes, where `LDY #$0D; LDA (ENTITY_PTR),Y; ... INY; LDA
   (ENTITY_PTR),Y` would do.

   ```c
   struct mob { uint8_t pad[13]; uint16_t gold; uint8_t flags; };
   extern struct mob *E [[c6502::zeropage(0xF8)]];
   extern uint16_t PRICE [[c6502::address(0x5A81)]];
   uint8_t gold_ok(void) { return E->gold < PRICE; }
   ```

4. **A comparison of two masked bytes is done as a signed 16-bit
   comparison.** `(a & 0x3F) >= (b & 0x1F)` compiles to `SEC; SBC; LDA
   #0; SBC #0; BVC; EOR #$80; BMI`, although both operands are known
   to be in 0-63 after the masks and `CMP; BCS` gives the same result.
   ai_choose_target's mode 2 test has this shape.

5. **A `switch` on a `uint16_t` with ten cases that share a high byte**
   compiles to a chain that tests the high byte again before every low
   byte. With the switch, shop_buy_handler was 357 bytes. With a loop
   over two tables of the addresses it is 167 bytes and the tables 20.

6. Smaller things seen in the listing: `STA AI_BEST_DIST; ORA #$00;
   BEQ` sets the flags again after a store that already set them, and
   a `for (i = n; i-- != 0;)` loop whose index lives in zero page
   compiles to `DEC; LDA; CMP #$FF; BNE`.

## Size

Code bytes of the `-S -O` output, from the object's symbol sizes,
against the original's from `main.lst`. `__zp_init` (7 bytes) stores
`shop`'s initial value and isn't counted.

| Original | Bytes | C | Bytes |
|---|---|---|---|
| FIND_NEAREST_EVENT | 78 | find_nearest_event | 96 |
| MANHATTAN_DISTANCE | 43 | manhattan_distance, manhattan_distance@run2 | 71 |
| NPC_MOVE_AI | 211 | npc_move_ai | 256 |
| EVALUATE_DIRECTION, INC_MOB_COUNTERS | 150 | evaluate_direction (and @run1), event_penalty | 166 |
| AI_CHOOSE_TARGET | 285 | ai_choose_target, join_party | 317 |
| DISPLAY_SHOP, BROWSE_HANDLER, ADVANCE_SHOP_PTR, ADD_PRICE_COMPONENT | 232 | display_shop, browse_handler, show_item, next_slot (and @run0), current_slot | 315 |
| SHOP_BUY_HANDLER | 68 | shop_buy_handler | 167 |
| SHOP_BUY_EFFECT_0-4, APPLY_SHOP_EFFECT | 62 | apply_shop_effect | 59 |
| XP_AWARD_0-4, AWARD_XP, EXTRACT_PENDING_XP | 94 | award_xp, pending_xp | 88 |
| **Total** | **1223** | | **1535** |

The C also has 20 bytes of read-only data, the two tables of handler
addresses. It is 312 bytes (26%) larger in code. The largest losses
are in show_item and shop_buy_handler, where much of the difference is
gap 3, the copies of zero-page parameters around calls, and the
zero-page loop counters of gap 6.

## For the integrator

- `shop` is a zero-initialized static that `-O` promotes into zero page,
  so `__zp_init` has to run, as for every unit.
- display_shop, browse_handler, and shop_buy_handler return
  `CHAIN_START_GAME_INPUT`, `CHAIN_START_GAME_INPUT`, and
  `CHAIN_BROWSE_HANDLER` (or `CHAIN_RETURN` from display_shop for an AI
  mob), so run_chain has to map `CHAIN_BROWSE_HANDLER` and
  `CHAIN_SHOP_BUY_HANDLER` to these functions.
- `find_nearest_event` reads `ENTITY_PTR->pos`, so map's ENTER_LOCATION
  has to leave ENTITY_PTR at CANDIDATE_POS before calling it.
