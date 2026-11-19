# util.c

util.c translates the Utility Routines chapter, STEP_PRNG, and the
integrity check and restart at `$A4E2`-`$A58F`.

## What is translated

| Routine | C | Notes |
|---|---|---|
| CLAMP_CHAR_FIELD | `clamp_char_field` | |
| FIND_FIRST_PC | `find_first_pc` | true when found |
| COPY_PC_NAME_TO_BUF | static `copy_pc_name_to_buf(m)` | |
| FIND_NEXT_PC_WRAP | `find_next_pc_wrap` | |
| FIND_NEXT_PC | static `find_next_pc` | its branch to its own entry is a loop |
| FIND_PREV_PC | `find_prev_pc` | INPUT_BUFFER[0..1] are locals |
| INIT_CHAR_SELECT | `init_char_select` | chain |
| POS_TO_COLROW | `pos_to_colrow` | column in the low byte, row in the high byte |
| GET_MOB_DATA | `get_mob_data` | |
| DELAY_WITH_ANIMATION | `delay_with_animation` | |
| TICK_ANIM_COUNTER | `tick_anim_counter` | |
| ANIM_TICK_AND_WAIT, ANIM_CHECK_FRAME | `anim_tick_and_wait`, `anim_check_frame` | both return ANIM_FRAME_CTR |
| TIMED_WAIT, CALL_ROM_WAIT | `timed_wait`, `WAIT(WAIT_DURATION)` | |
| IDLE_ANIM_HANDLER, DEC_WAIT_IF_NONZERO | `idle_anim_handler` | subtracts the 12 calls at once |
| COLROW_TO_POS | `colrow_to_pos` | |
| GET_CHAR_RECORD | `get_char_record` | |
| RANDOM_IN_RANGE | `random_in_range` | |
| DISPATCH_ON_MODE | `dispatch_on_mode` | tests SOUND_ON |
| STEP_PRNG | `step_prng` | |
| VERIFY_INTEGRITY | `verify_integrity` | `.text.integrity` |
| VERIFY_DISK_IO | `verify_disk_io` | empty, `.text.integrity` |
| COMPUTE_CHECKSUM | static `compute_checksum` | `.text.integrity` |
| COLD_RESTART | `cold_restart` | `.cold_restart` |

The tables: `ANIM_INDEX` (`$7ABD`, the header's extern, initially 2),
`ANIM_FRAME_CTR` (static, initially 3, which `-O` puts in zero page),
`ANIM_WAIT_TABLE`, and `ANIM_FRAME_TABLE` (static const).
WAIT_DURATION and DELAY_COUNT are `constexpr uint8_t` constants,
because nothing writes them.

Not translated: SAVE_ZP_CONTEXT and RESTORE_ZP_CONTEXT (map's
`scene_loop` saves what is needed), and LOAD_END_OFFSET,
ADVANCE_CKSUM_START, and ADVANCE_RANGE_CTR, which have no callers.
RANDOM_IN_RANGE's entries at +42 and +48 are reached only from the
dead ORPHANED_ENCOUNTER_HANDLER and aren't separate entries.

Every routine of the unit is translated, and none is incomplete.

## Departures

- `dispatch_on_mode` tests sound.c's `SOUND_ON`, where the original
  compares PLAY_HIGH_TONE's first byte with `$60`.
- `find_prev_pc` keeps its two indexes in locals, where the original
  writes them to INPUT_BUFFER[0] and [1] (`$5AAC`, `$5AAD`). Its only
  caller, CHAR_SELECT_PREV, goes on to ENTER_CHAR_SELECT, whose
  SETUP_INPUT runs CLEAR_INPUT_BUFFER before anything reads the
  buffer, so no reader of those bytes sees a difference. A harness that
  compares memory after FIND_PREV_PC and before the next menu is drawn
  will see the two bytes differ.
- `verify_disk_io` does nothing. The original checksums `$6DD8`-`$6DF9`,
  its own LOAD_SCENE_DATA, which nothing writes, so the check always
  passes.
- `verify_integrity` compares the checksum with the constant `0x7AAE`,
  so INTEGRITY_DATA isn't kept and section `.rodata.integrity` is
  empty. The linker script's pattern for it matches nothing.
- The scratch bytes RNG_LIMIT (`$5A11`), RNG_MASK (`$5A12`), PRNG_TEMP
  (`$5A1A`), CKSUM_ACC (`$A527`), and `$BA`-`$BF` are locals, so the C
  never writes them. SAVE_ZP_CONTEXT's `$5A9A`-`$5A9B` and
  `$5A9E`-`$5A9F` aren't written either. A harness comparing memory
  should leave all of these out.
- `colrow_to_pos` wraps a sum past 255, where the original falls into
  NUM_TO_DECIMAL. No caller passes such a column and row.
- `idle_anim_handler` subtracts 12 from WAIT_LOOP_COUNT, or sets it to
  0 when it holds 12 or less, where the original calls
  DEC_WAIT_IF_NONZERO 12 times. The byte ends the same.
- `cold_restart` fills `$0800`-`$A4FF` and then `$A501`-`$A56F` from the
  lowest address up. The original fills each page from offset 0 and
  then from `$FF` down, and the tail from `$A56F` down. The bytes are
  the same. The one byte of cold_restart's own code in the range, at
  `$A56F`, has already run when it is stored.
- The C is written as plain arithmetic where the original loops:
  `pos_to_colrow` divides with `/` and `%`, `colrow_to_pos` multiplies,
  `get_char_record` indexes LOCATIONS, `step_prng` multiplies by 69,
  and `random_in_range` shifts by its count. Every limit, sample, and
  index gives the original's result (checked for all 256 limits and 128
  samples of random_in_range). The division and the shifts by a
  variable count are inline loops, and the unit calls no runtime
  helper.
- `cold_restart` uses only zero-page locals, no soft stack, and calls
  nothing but DISK_BOOT_ROM, which the listing shows.

## Where the original's comments and code disagree

The C follows the code in each case.

- RANDOM_IN_RANGE's comments say it returns 0 to limit - 1. The code
  accepts a sample equal to the limit, so it returns 0 to limit.
- CLAMP_CHAR_FIELD's header comment says it caps strength at 31 and HP
  at 63, with ENTITY_PTR as its input. The code raises MOB_PTR's life
  (bits 5-0) to its strength (bits 4-0).
- COLD_RESTART's comment says it zeros memory. It stores each page's
  own number in the page, and `$A5` in `$A501`-`$A56F`.
- FIND_FIRST_PC's comment gives CHAR_PTR as its input. It reads
  CHAR_LIST_HEAD (`$4002`).

## Changes needed in ali_baba.h

None.

## Compiler gaps

These don't stop the unit from compiling. Each costs bytes. The first
item of the earlier list, a byte's `/` and `%` by a constant calling
`sdivmod16` twice, was fixed on 18 Nov 2026. pos_to_colrow is now one
loop of subtractions, 11 bytes, where the original's is 13.

1. **A shift by a variable count is a loop on the value in memory, and
   a shifted byte compared with a constant is a 16-bit shift and a
   signed 16-bit comparison.** random_in_range is 120 bytes, where the
   old C, with loops of one-bit shifts, was 89.

   ```c
   uint8_t f(uint8_t x, uint8_t n) { return x >> n; }
   uint8_t g(uint8_t limit) {
       uint8_t shift = 0;
       while (limit << shift < 0x40)
           shift++;
       return shift;
   }
   ```

   f compiles to `STA x; CPX #0; BEQ done; loop: LSR x; DEX; BNE loop;
   done: LDA x`, where `LSR A` in the loop would save the store and the
   load. g copies `limit` and a 0 into a 16-bit temporary on each pass,
   shifts both bytes, and compares with `SEC; SBC #$40; LDA hi; SBC
   #0; BVC; EOR #$80; BPL`. For a limit of 1 or more the loop ends by
   a shift of 6, so the shifted value fits in a byte and an unsigned
   8-bit comparison would do, but the compiler has no bound on `shift`
   and keeps the high byte and the signed comparison. random_in_range
   also loads the count into X again for each of its four shift loops.

   Since 19 Nov 2026, f shifts A in its loop
   (docs/optimizations/shift_in_a.md), `CPX #0; BEQ; LSR A; DEX; BNE`.
   random_in_range's loops shift bytes that aren't loaded again right
   after the loop, and they still shift memory. The comparison is still
   16 bits. A narrower one needs a bound on the shifted value, which
   comes only from C's rule that a signed shift may not overflow, and
   a new lowering of a comparison of a shifted byte, which wasn't done.

2. A small static function isn't inlined. `is_player_character` is 16
   bytes, and each caller sets up a pointer in A and X for it, where
   `LDY #4; LDA (m),Y; CMP #$15` in place would do.

   ```c
   struct mob { uint8_t a[4]; uint8_t appearance; };
   extern struct mob *M [[c6502::zeropage(0xF4)]];
   void hit(void);
   static bool is_pc(const struct mob *m) { return m->appearance < 0x15; }
   void f(void) { if (is_pc(M)) hit(); }
   ```

3. A loop that counts up from 0 and doesn't use its index keeps the
   count-up: `LDA #0; STA n; ...; INC n; LDA n; CMP #4; BCC`. Counting
   down to 0 would save the compare. `delay_with_animation` is 16 bytes,
   where the old do-while that counted down was 12.

   Fixed on 19 Nov 2026 (docs/optimizations/count_down.md).
   `delay_with_animation` is 12 bytes.

   ```c
   void tick(void);
   void f(void) { for (uint8_t n = 0; n < 4; n++) tick(); }
   ```

4. `cold_restart`'s loops over a pointer compare both bytes of the
   pointer on each pass, and the `p <= (uint8_t *)0xA56F` test of the
   tail is a 16-bit subtraction. It is 70 bytes, where loops over a page
   number and a byte index were 40.

5. A 16-bit store to a member at an offset through a zero-page
   pointer builds the member's address in a temporary and in DPTR.

   ```c
   struct s { uint8_t a[13]; uint16_t g; };
   extern struct s *P [[c6502::zeropage(0xF4)]];
   void f(void) { P->g = 0; }
   ```

   is 26 bytes, where `LDA #0; LDY #13; STA (P),Y; INY; STA (P),Y` is
   9. map.c hits it twice. This item was here before the rewrite and
   wasn't measured again.

## Size

Code bytes in the `-S -O` output, from `objdump.py -t util.o`, against
the original's bytes for the same routines (from the label to the next
entry, as main.lst lays them out).

| Original | Bytes | C | Bytes |
|---|---|---|---|
| CLAMP_CHAR_FIELD | 27 | clamp_char_field | 26 |
| FIND_FIRST_PC | 32 | find_first_pc, is_player_character | 54 |
| COPY_PC_NAME_TO_BUF | 48 | copy_pc_name_to_buf | 55 |
| FIND_NEXT_PC_WRAP | 4 | find_next_pc_wrap | 3 |
| FIND_NEXT_PC | 38 | find_next_pc | 31 |
| FIND_PREV_PC | 39 | find_prev_pc | 34 |
| INIT_CHAR_SELECT | 19 | init_char_select | 19 |
| POS_TO_COLROW | 13 | pos_to_colrow | 11 |
| GET_MOB_DATA | 31 | get_mob_data | 32 |
| DELAY_WITH_ANIMATION | 14 | delay_with_animation | 16 |
| TICK_ANIM_COUNTER | 18 | tick_anim_counter | 17 |
| ANIM_TICK_AND_WAIT, ANIM_CHECK_FRAME | 25 | anim_tick_and_wait, anim_check_frame | 25 |
| TIMED_WAIT, CALL_ROM_WAIT | 21 | timed_wait | 17 |
| IDLE_ANIM_HANDLER, DEC_WAIT_IF_NONZERO | 44 | idle_anim_handler | 47 |
| COLROW_TO_POS | 9 | colrow_to_pos | 16 |
| GET_CHAR_RECORD | 28 | get_char_record | 47 |
| RANDOM_IN_RANGE | 118 | random_in_range | 120 |
| DISPATCH_ON_MODE | 13 | dispatch_on_mode | 11 |
| STEP_PRNG | 31 | step_prng | 29 |
| VERIFY_INTEGRITY | 34 | verify_integrity | 27 |
| VERIFY_DISK_IO | 31 | verify_disk_io | 1 |
| COMPUTE_CHECKSUM | 70 | compute_checksum | 83 |
| COLD_RESTART | 33 | cold_restart | 70 |
| Total | 740 | | 791 |

The C's bytes are the symbol sizes of `nm.py -S util.o`. Before the
rewrite into plain C on 18 Nov 2026, the same routines came to 722
bytes. They came to 824 after the rewrite, and to 791 once c6502
divided a byte by a constant and shifted by a variable count inline.

By section, the C has 610 bytes in `.text`
 (and the linker's 5-byte
`__zp_init` for ANIM_FRAME_CTR), 111 in `.text.integrity`, which the
original's `$A4E6`-`$A56E` (135 bytes) held, and 70 in `.cold_restart`
at `$A56F`. The data is 12 bytes of `.rodata`, 1 of `.data`
(ANIM_INDEX), and 1 byte of zero page (ANIM_FRAME_CTR).

PLAN.md gives the unit 582 bytes. The 740 above is the sum of the
routines util.c translates, which include STEP_PRNG and the code at
`$A4E6`-`$A58F`, and leave out SAVE_ZP_CONTEXT, RESTORE_ZP_CONTEXT, and
the dead code at `$A590`-`$A5BB`.
