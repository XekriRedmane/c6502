# combat.c

combat.c translates the Combat chapter of main.nw, every routine that
UNITS.md's section "combat.c" lists. Its header comment quotes
main.asm lines 2498-2719, 2886-3000, 3109-3157, 3267-3306, 3568-3675,
3700-4086, 4127-4563, 4974-5105, 5152-5386, 11202-11214, 13362-13469,
and 15335-15337.

## What is translated

All 42 entries in the table are translated. The header's routines are
check_encounter, check_hostile, check_adjacent_threats,
check_threats_here, pick_random_member, check_attack_eligible,
apply_damage_to_current, auto_heal, enter_combat_state, start_combat,
attack_right, attack_left, attack_up, attack_down, find_attack_target,
select_someone_else, init_attack_display, resolve_attack,
select_combat_target, handle_encounter, show_locked_msg,
handle_event_slot, handle_mob_encounter, and modify_current_stats,
with the header's prototypes unchanged. The statics are
find_mob_at_same_pos, get_hostility, sum_hostile_at_pos,
next_colocated_mob, apply_damage, apply_damage_to_mob, increment_hp,
load_target_ptr, calc_combat_strength, map_value_to_string,
player_attack, process_entity_script, mob_attack, get_entity_threat,
event_prng_check, set_target_restricted, modify_char_stats, and
defense, which has no routine of its own in the original.
ADD_WILLINGNESS is the `+=` on resolve_attack's local `willingness`
where it is called.
WAIT_LOOP_INIT is `constexpr uint8_t WAIT_LOOP_INIT = 0x20;`.

No routine is left out or incomplete.

## Departures

- The scratch bytes are parameters, locals, and return values.
  EFFECT_REC is the `rec` of apply_damage and modify_char_stats,
  NUM_VALUE is modify_char_stats' `amount`, and EVENT_PTR is the
  handlers' `ev`. TARGET_REC is check_hostile's `other`. ENTITY_REC,
  LEVEL_THRESHOLD, SAVED_CHAR, and the values the original keeps in X
  and Y across calls (APPLY_DAMAGE's tune, HANDLE_MOB_ENCOUNTER's
  limit across STEP_PRNG, CALC_COMBAT_STRENGTH's field index on the
  stack) are locals.
- sum_hostile_at_pos takes only the position, and tests hostility
  against ENTITY_PTR. UNITS.md proposed `sum_hostile_at_pos(pos, ref)`.
  Both callers (CHECK_ADJACENT_THREATS and HANDLE_MOB_ENCOUNTER) copy
  ENTITY_PTR into TARGET_REC just before the call, and nothing in
  SUM_HOSTILE_AT_POS changes ENTITY_PTR, so the result is the same, and
  each call is 10 bytes shorter.
- The ATTACK_ entries call the next entry where the original falls
  into it, and attack_down calls find_attack_target. They return its
  state directly, which UNITS.md allows, since nothing they call leads
  back to them.
- EVENT_PRNG_CHECK is the static event_prng_check, which
  handle_event_slot and player_attack call and then return.
- RESOLVE_ATTACK's jumps to PLAYER_ATTACK and MOB_ATTACK, and the other
  tail jumps, are calls.
- tune_player gets a strength of 0 for every tune other than 1 and 2,
  which ignore it. The original passes whatever Y holds.
- Where the original compares as signed and both operands are small
  enough that the result is the same, the C compares unsigned. That
  includes the comparisons with STEP_PRNG's result (AUTO_HEAL's loop,
  HANDLE_ENCOUNTER's roll against $74, HANDLE_MOB_ENCOUNTER's roll
  against the courage limit, and EVENT_PRNG_CHECK), since STEP_PRNG
  returns the state shifted right by one, 0-127. MOB_ATTACK's test of
  the strength left after the armor is `COMBAT_STRENGTH > MOB_DEFENSE`,
  since the strength is 1-31 and the armor 1-15.
- RESOLVE_ATTACK's willingness and HANDLE_MOB_ENCOUNTER's courage are
  `int8_t` locals, which the C stores in COMBAT_WILLINGNESS once it is
  held within 0-30. The willingness runs from -22 to 111 and the
  courage from -111 to 47 before they are held, so neither wraps.
- APPLY_DAMAGE's message for the size of the blow, which the original
  counts with a loop of RORs, is `3 + stdc_bit_width_uc(damage | 1)`.
- CALC_COMBAT_STRENGTH's field index is a pointer to the sword or the
  dagger member. The defense, bits 4-1 of the strength byte, is the
  static `defense(m)`, which check_adjacent_threats and resolve_attack
  call.
- get_entity_threat returns 0 for a distance of 2 or more. The original
  returns 2 for 129 or more, which MANHATTAN_DISTANCE can't return
  (column 19 and row 12 are the largest).
- EVENT_PRNG_CHECK's damage, `ROL; ROL; LDA #1; ADC #0`, is
  `(step_prng() & 0x40) ? 2 : 1`, which is the same value.

Every write of the original to a page $5A variable that the header
declares is kept, including the ones only combat reads (HOSTILE_APP,
HOSTILE_A, HOSTILE_Y, COLOCATE_*, SAVED_MOB_LINK, TARGET_DEFENSE,
MOB_DEFENSE, STAT_SCALE, STAT_LEVEL_BITS, EVENT_SLOT_BYTE,
EVENT_SUB_VALUE, ADJACENT_POS), so that page $5A ends each routine as
the original's does. That includes the stores of RANDOM_IN_RANGE's
results into PRNG_OUTPUT at $1C13 and $1DBB.

## Where the original's comments and code disagree

The C follows the code in each case.

- ENTER_COMBAT_STATE's header says the restricted flag is set when no
  hostile is in the cell. The code (`LDA #2; CMP ENCOUNTER_RESULT; BNE
  skip; JSR SET_RESTRICTED`) sets it when ENCOUNTER_RESULT is 2.
- PROCESS_ENTITY_SCRIPT's comment says it restores the entity. It
  stores $FE at the entry's byte 0, which deletes the entry, and never
  writes the saved byte back. It only passes it to DRAW_CHAR_AT_POS as
  the position.
- RESOLVE_ATTACK's "consume rage" ANDs the level byte with
  LEVEL_CLEAR_BIT4 ($EF), which clears bit 4, the top bit of the level,
  and leaves the rage bit 5 set.
- AUTO_HEAL's comments say the loop goes on while the PRNG's output is
  at least $50, and, at the branch, while it is below $50. `CMP #$50;
  BPL` goes on while the output is from $50 to $CF.
- MOB_ATTACK's header calls ENTITY_PTR the target and MOB_PTR the
  attacker. The strength comes from ENTITY_PTR and the damage goes to
  MOB_PTR.
- CALC_COMBAT_STRENGTH's comment at `BAEQ #$01,use_ranged` says "not
  player turn", and the branch is taken when IS_PLAYER_TURN is 1. Its
  header gives the fumble odds as 1 in 13 or 80, and RANDOM_IN_RANGE
  returns 0 to its limit inclusive, so they are 1 in 14 or 81.
- PLAYER_ATTACK's comment at `BAMI #STRENGTH_MASK,take_item` says
  "sub-type = max: take item". The branch takes the item when the
  sub-type is below $1F, and the sub-type $1F counts down.
- MAP_VALUE_TO_STRING's comment says the high range uses bits 4-3. Two
  RORs and `AND #$03` take bits 3-2. Its header maps values 0-7 to
  strings 0-7, and the loop prints string n - 1 (0-based) for index n.
- GET_HOSTILITY's comment calls groups 3-6 hostile and the rest
  friendly. Groups 8 and up are hostile too, since only 0-2 and 7
  branch to friendly.
- PICK_RANDOM_MEMBER's header reads the group count from bits 3-0 of
  byte 8. The code uses the whole byte.

## Changes needed in ali_baba.h

None. Every prototype in the header's combat section fits the code as
it is.

## Compiler gaps

None of these is a miscompile. Each makes the code larger than it has
to be, and combat.c works around none of them. The comparisons of
masked bytes, the signed byte's `> 0`, and the computed copy of a
pointer after a constant store, which combat.c worked around until 18
Nov 2026, compile to byte tests and `LDY #k; LDA (P),Y` now, so the C
compares masked bytes in place, writes `COMBAT_STRENGTH > MOB_DEFENSE`,
and stores `nullptr` in pick_random_member.

1. A 16-bit member read or written through a pointer parameter takes
   the second byte through a copy of the pointer in DPTR, where `INY`
   and the same pointer would do.

   ```c
   struct rec { uint8_t pad[13]; uint16_t gold; };
   uint16_t get(struct rec *r) { return r->gold; }
   ```

   compiles to `STA r; STX r+1; STA DPTR; STX DPTR+1; LDY #$0D; LDA
   (r),Y; STA r; INY; LDA (DPTR),Y`. modify_char_stats and apply_damage
   pay for it. Fixed on 19 Nov 2026: both bytes go through `(r),Y`
   (docs/optimizations/pointer_restage.md), and modify_char_stats is 6
   bytes smaller.

2. The test that a pointer is out of page 0, `(uint16_t)p >= 0x0100`
   (`IN_PAGE_0`), compiles to `SEC; LDA p+1; SBC #1; BCS`, 3 bytes
   more than `LDA p+1; BNE`.

   ```c
   struct mob { uint8_t a, b; };
   extern struct mob *P [[c6502::zeropage(0xF4)]];
   void next(void);
   void walk(void) { do next(); while ((uint16_t)P >= 0x0100); }
   ```

   This costs select_combat_target 6 bytes and handle_mob_encounter
   3.

3. A pointer to one of two members of the record a zero-page pointer
   points at is computed as a 16-bit address, where the member's
   offset in Y would do.

   ```c
   struct mob { uint8_t pad[7]; uint8_t sword, armor, dagger; };
   extern struct mob *E [[c6502::zeropage(0xF8)]];
   void set(uint8_t n, uint8_t v) {
       uint8_t *w = n < 3 ? &E->sword : &E->dagger;
       *w = v;
   }
   ```

   compiles to an `LDA E; CLC; ADC #k; STA w; LDA E+1; ADC #0; STA
   w+1` for each member and `LDY #0; STA (w),Y`. The field index of the
   old C (`LDA #7` or `LDA #9` into a byte, then `LDY k; STA (E),Y`)
   was 5 bytes shorter in calc_combat_strength. Since 19 Nov 2026 this
   example is `LDY #7` or `LDY #9` and `STA (E),Y`
   (docs/optimizations/phi_offset.md), but calc_combat_strength isn't
   changed. It reads `*weapon` before and after a call to
   `random_in_range`, which may move ENTITY_PTR, so C requires the
   address from before the call, and the pointer is still computed.

4. `stdc_bit_width_uc(x | 1)` compiles to a loop with its test at the
   top and a separate count, and the `3 +` around it is a separate add.

   ```c
   #include <stdbit.h>
   void show(uint8_t m);
   void f(uint8_t d) { show(3 + stdc_bit_width_uc(d | 1)); }
   ```

   gives `ORA #1; STA t; LDA #0; STA n; JMP test; body: LSR t; INC n;
   test: LDA t; BNE body; LDA #3; CLC; ADC n`, 24 bytes. The operand
   is nonzero, so a loop with its test at the bottom and the count
   starting at 3 (`ORA #1; STA t; LDA #3; STA n; loop: INC n; LSR t;
   BNE loop; LDA n`) gives the same result in 16. The old C's loop was
   that one without the `ORA`.

5. An `int8_t` that a branch has shown to be nonnegative is compared
   with a positive constant with the overflow test.

   ```c
   void f(void);
   void g(int8_t c) { if (c < 0) return; if (c > 0x1C) f(); }
   ```

   The second test compiles to `SEC; SBC #$1D; BVC; EOR #$80; BMI`,
   where `CMP #$1D; BCC` gives the same result. This costs
   handle_mob_encounter 5 bytes.

   Fixed on 19 Nov 2026. The compare after a `BPL` on the value is
   `CMP #$1D; BCC` (docs/optimizations/redundant_compare.md), and
   handle_mob_encounter is 155 bytes, where it was 163, since another
   compare in it became a branch on Z. resolve_attack, which tests a
   nonnegative byte the same way, went from 250 bytes to 238.

6. `(int8_t)(length - i) >= 0` in a loop over `i` is computed afresh
   in each pass, where a counter stepped down with `i` (the old C's
   `DEC left; BPL`) is 5 bytes shorter in init_attack_display, and the
   `?:` that picks the byte jumps over its second arm.

   ```c
   extern uint8_t OUT [[c6502::address(0x7000)]][21];
   void f(const uint8_t *name) {
       uint8_t length = name[0];
       for (uint8_t i = 1; i <= 20; i++)
           OUT[i] = (int8_t)(length - i) >= 0 ? name[i] : 0x20;
   }
   ```

## Size

combat.c's code is 2992 bytes in the `-O` output, from the object's
symbol sizes, against the original's 2856 bytes for the same routines
(UNITS.md's sizes, from main.lst), 136 bytes or 4.8% more. The
compiler factors code shared by several functions into `name@runN`
pieces, and the table counts each piece under the function whose name
it carries, so a function that lends its pieces to others looks larger
than it is.

| Routine | Original | C |
|---|---|---|
| check_encounter | 71 | 77 |
| find_mob_at_same_pos | 69 | 71 |
| check_hostile | 50 | 58 |
| get_hostility | 14 | 14 |
| check_adjacent_threats | 88 | 70 |
| sum_hostile_at_pos | 59 | 58 |
| apply_damage | 211 | 276 |
| map_value_to_string | 53 | 68 |
| pick_random_member | 42 | 44 |
| apply_damage_to_current | 13 | 9 |
| apply_damage_to_mob | 13 | 9 |
| auto_heal | 67 | 58 |
| increment_hp | 33 | 36 |
| enter_combat_state | 22 | 22 |
| start_combat | 11 | 11 |
| attack_right, _left, _up, _down | 18 | 39 |
| find_attack_target | 77 | 64 |
| init_attack_display | 39 | 49 |
| select_someone_else | 6 | 6 |
| check_attack_eligible | 38 | 35 |
| next_colocated_mob | 33 | 33 |
| resolve_attack and add_willingness | 248 | 259 |
| defense | | 12 |
| calc_combat_strength | 113 | 135 |
| player_attack | 248 | 274 |
| process_entity_script | 23 | 25 |
| mob_attack | 132 | 124 |
| select_combat_target | 149 | 145 |
| get_entity_threat | 36 | 40 |
| load_target_ptr | 16 | 16 |
| handle_encounter | 240 | 228 |
| show_locked_msg | 19 | 13 |
| handle_event_slot and event_prng_check | 269 | 264 |
| handle_mob_encounter | 169 | 163 |
| set_target_restricted | 9 | 9 |
| check_threats_here | 12 | 16 |
| modify_char_stats | 135 | 151 |
| modify_current_stats | 11 | 11 |
| Total | 2856 | 2992 |

The original computes the defense inline in CHECK_ADJACENT_THREATS and
RESOLVE_ATTACK. The ATTACK_ entries are 11 bytes each in C against the
original's 5, since each is an increment, a branch, and two jumps.
Much of apply_damage's and player_attack's excess is the stores of
each argument into its callee's zero-page slot, where the original
passes it in A or leaves it in EFFECT_REC, and apply_damage also pays
for gaps 1 and 4.

Before the rewrite of 18 Nov 2026, which removed the workarounds and
wrote the comparisons, the clamps, and the field accesses as plain C,
the unit's code was 2973 bytes. The routines that grew most are
init_attack_display (+5, gap 6), calc_combat_strength (+5, gap 3),
handle_mob_encounter (+8, gap 5), select_combat_target (+6, gap 2), and
apply_damage (+4, gap 4), and resolve_attack shrank by 13.

## For the integrator

- find_attack_target, the ATTACK_ entries, select_someone_else,
  handle_event_slot, and handle_mob_encounter return CHAIN_ states as
  UNITS.md lists. handle_event_slot returns CHAIN_MOVE_LOOP only from
  its first branch (`$1D85`) and CHAIN_RETURN everywhere else.
- PLAYER_ATTACK and PICK_RANDOM_MEMBER reach $0000-$0004 through
  MOB_PTR = 0 or 1, as the header's NULL_PAGE_LOW reservation expects.
- combat.c uses DPTR (`$24`/`$25` in the classic layout) through the
  compiler's own code for gap 1.
