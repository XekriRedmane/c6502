# entities.c

## What was translated

Every routine UNITS.md gives entities. The unit defines no tables or
variables.

| Original | C |
|---|---|
| LOAD_CHAR_NAME_PTR | `load_char_name_ptr` |
| SETUP_CHAR_SPRITE | `setup_char_sprite` |
| FIND_ENTITY_AT_POS | `find_entity_at_pos` |
| GET_ENTITY_FONTCHAR | `get_entity_fontchar` |
| APPEARANCE_TO_FONTCHAR | `appearance_to_fontchar` |
| PRINT_MOB_NAME | `print_mob_name` |
| REORDER_CHAR | `reorder_char` |
| DECR_GROUP_COUNT, INCR_GROUP_COUNT | `adjust_pc_count(loc, m, 0xFF)` and `(loc, m, 1)`, static |
| PRINT_ENTITY_NAME | `print_entity_name` |
| DRAW_CHAR_AT_POS | `draw_char_at_pos` |
| CLASSIFY_ENTITY_TYPE | `classify_entity_type` |
| FIRST_GROUP_MEMBER, NEXT_GROUP_MEMBER | `first_group_member`, `next_group_member`, and the static `follow_link` |
| PRINT_MEMBER_NAME | `print_member_name` |
| REORDER_CURRENT_CHAR | `reorder_current_char` |
| SET_BLINK_TARGET | `set_blink_target` |
| LOAD_CHAR_TO_FA | `load_char_to_fa` |
| RENDER_FONT_CHAR, PRINT_FONTCHAR_AT_TEXT_POS, RENDER_FONT_CHAR_DIRECT | `render_font_char`, `print_fontchar_at_text_pos`, `render_font_char_direct` |
| DRAW_BLINK_ALT | `draw_blink_alt` |
| DRAW_BLINK_NORMAL, DRAW_BLINK_CHAR | `draw_blink_normal`, and the static `draw_blink_char` |
| FONT_POS_TO_TEXT_POS | `font_pos_to_text_pos` |

Each routine that falls into the next calls it in C. Nothing was left
out or left incomplete.

## Departures

The file's header lists them. In short:

- FIND_ENTITY_AT_POS's self-modified operands are local pointers.
- DECR/INCR_GROUP_COUNT take the location, the mob, and the delta as
  parameters, where the original has them in $BE, $BC, and
  GROUP_COUNT_DELTA ($5A2C).
- APPEARANCE_TO_FONTCHAR's two results come back in one `uint16_t`,
  and CLASSIFY_ENTITY_TYPE's string is a return value, as the header
  declares them.
- DRAW_CHAR_AT_POS's call of COLROW_TO_POS is dropped.
  POS_TO_COLROW gives pos % 20 and pos / 20 for every byte, and
  COLROW_TO_POS adds 20 per row without a carry for any such pair, so
  the round trip gives back pos. COLROW_TO_POS writes no memory.
- DRAW_BLINK_CHAR's save of PRINT_STRING_ADDR is dropped, as UNITS.md
  says, since text.c keeps the string pointer in a local.
- The scratch results in $BA-$BF (ENTITY_REC, CHAR_REC, MOB_DATA_PTR,
  PRINT_STRING_ADDR, TARGET_REC, HANDLER_PTR) and $5A2A-$5A2C are
  locals, parameters, and return values. SET_BLINK_TARGET's store of
  MOB_PTR into ENTITY_REC has no counterpart.

Behavior kept exactly:

- GET_ENTITY_FONTCHAR, CLASSIFY_ENTITY_TYPE, and SET_BLINK_TARGET's
  untagged MOB_PTR all read byte 1 of whatever record they're given
  as an event's type.
- FIRST_GROUP_MEMBER with an empty list continues from the old
  MOB_PTR's next link, which reads `$0002` when MOB_PTR is 0.
- REORDER_CHAR's source walk runs away when the mob isn't in the
  list, through GET_MOB_DATA(0) = MOB_BASE - 16 and on.
- RENDER_FONT_CHAR_DIRECT writes CHAR_CHARSET only for sprite numbers
  of 24 and up.

DECR/INCR_GROUP_COUNT test for record 0 by the low byte of the
record's address, and adjust_pc_count compares the pointer with
`&LOCATIONS[0]`. GET_CHAR_RECORD returns one of records 0-128, and of
those only record 0's address has a low byte of 0, so the two tests
agree.

## Where the original's comments and code disagree

The C follows the code in each case.

- GET_ENTITY_FONTCHAR's comments give event types `$80-$BF` the
  sprite `$1B` and types `$00-$3F` the empty cell. After `CMP #$40`,
  the BPL to the empty cell is taken for `$80` and not for `$00`, so
  `$00-$3F` draw `$1B` and `$80-$BF` draw BLINK_ALT_CHAR. UNITS.md
  says the same.
- CLASSIFY_ENTITY_TYPE's comment on `BANE #FLAGS_CLASS_MASK` says
  "both set -> check further". The branch is taken when bits 7-6 of
  an object's value are not both set, which names it an illusion, and
  with both set the code goes on to the sprite number.
- REORDER_CHAR's header comment says TOTAL_MOB_COUNT goes down when
  the source is record 0 and up when the destination is, for an
  "NPC". The code decrements it when ACTIVE_CHAR is 0 and increments
  it when SOURCE_CHAR is 0, for a mob with appearance below `$15`,
  which is a player character.
- REORDER_CHAR's comment at `.random` says it inserts when
  PRNG_OUTPUT is 0. The test is `CMP #$01` and BMI, which inserts
  when the output less 1 is negative as a signed byte, that is for
  0 and for `$81-$FF`. report_entities repeats the comment's reading.
  The C tests `r == 0 || r > 0x80`.
- DECR/INCR_GROUP_COUNT's comments describe byte 8 of a mob record
  and a group count in its low nibble. The pointer is GET_CHAR_RECORD's
  location record, and the byte is the location's pc_count.

## Header changes needed

None. Every routine keeps the header's declaration.

## Compiler gaps

The gaps listed here before, an address-attributed object plus a
constant and a comparison of a masked byte, were fixed on 18 Nov 2026,
and the C no longer works around them. classify_entity_type returns
each name's address as `LDX #>S_A_DOOR; LDA #<S_A_DOOR; RTS`, and
reorder_char compares the masked level without a cast. These remain.

1. **A division or remainder of a byte by a constant calls the
   runtime helper `sdivmod16`.**

   ```c
   extern uint8_t V [[c6502::address(0x5A00)]];
   extern uint8_t Q [[c6502::address(0x5A01)]];
   extern uint8_t R [[c6502::address(0x5A02)]];
   void f(void) { Q = V / 21; R = V % 21; }
   ```

   compiles to two calls of a fragment that stores V and 21 in HARGS
   as 16-bit operands and jumps to `sdivmod16`, which this program
   doesn't have (README, "Building"). appearance_to_fontchar and
   render_font_char_direct therefore divide by repeated subtraction,
   as the original does, in a `while` loop. Both operands are
   unsigned bytes, so an 8-bit unsigned division would do, and a
   division of a byte by a constant can be done inline with a multiply
   by the reciprocal or, for a quotient this small, by the loop.

2. **Stores to different globals are reordered.** setup_char_sprite
   stores FONT_COL, FONT_ROW, BLINK_COL, and BLINK_ROW in the
   original's order, and the listing stores FONT_COL, BLINK_COL,
   FONT_ROW, BLINK_ROW, so that each value is stored twice from A.
   C allows this for objects that aren't volatile, and the bytes are
   the same once the routine returns. A harness that compares the
   order of RAM writes would report the difference.

No compiler crash or wrong code.

## Size

The bytes of code in `entities.c -c -O` (the `.text` sections, from
`nm.py -S`), against the original's code bytes in `main.lst`. Before
the rewrite to plain C the unit took 1,049 bytes with the same
compiler, and the only routine whose size changed is
adjust_pc_count, by 2 bytes, for the comparison of both bytes of the
record's address.

| Routine | Original | C |
|---|---|---|
| LOAD_CHAR_NAME_PTR | 14 | 15 |
| SETUP_CHAR_SPRITE | 37 | 32 + 8 |
| FIND_ENTITY_AT_POS | 152 | 91 |
| GET_ENTITY_FONTCHAR | 121 | 125 |
| APPEARANCE_TO_FONTCHAR | 25 | 37 |
| PRINT_MOB_NAME | 14 | 15 |
| REORDER_CHAR | 171 | 237 |
| DECR/INCR_GROUP_COUNT | 36 | 29 |
| PRINT_ENTITY_NAME | 20 | 33 |
| DRAW_CHAR_AT_POS | 21 | 22 |
| CLASSIFY_ENTITY_TYPE | 145 | 124 |
| FIRST/NEXT_GROUP_MEMBER | 29 | 11 + 7 + 19 |
| PRINT_MEMBER_NAME | 14 | 15 |
| REORDER_CURRENT_CHAR | 17 | 13 |
| SET_BLINK_TARGET | 61 | 54 |
| LOAD_CHAR_TO_FA | 12 | 8 |
| RENDER_FONT_CHAR group | 66 | 6 + 19 + 66 |
| DRAW_BLINK_ALT | 18 | 20 |
| DRAW_BLINK_NORMAL/CHAR | 47 | 6 + 26 |
| FONT_POS_TO_TEXT_POS | 13 | 13 |
| Total | 1033 | 1051 |

The `+` terms are the C's separate functions, and for
SETUP_CHAR_SPRITE the 8 bytes of `setup_char_sprite@tail0`, a tail the
optimizer outlined. Most of the difference is the copying of pointer
arguments into the callee's fixed zero-page slots (reorder_char's
calls of adjust_pc_count and get_mob_data), and appearance_to_fontchar
and render_font_char_direct keeping their counters in memory where the
original keeps them in A and Y.
