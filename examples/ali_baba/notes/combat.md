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
event_prng_check, set_target_restricted, and modify_char_stats.
ADD_WILLINGNESS is the `+=` on COMBAT_WILLINGNESS where it is called.
WAIT_LOOP_INIT is `#define WAIT_LOOP_INIT 0x20`.

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
  enough that the result is the same, the C compares unsigned. The
  comparisons with PRNG_OUTPUT or STEP_PRNG's result keep the signed
  test, because those bytes take every value (AUTO_HEAL's loop,
  HANDLE_ENCOUNTER's roll against $74, HANDLE_MOB_ENCOUNTER's roll
  against the courage limit, and EVENT_PRNG_CHECK).
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

Items 1 and 2 were fixed on 18 Nov 2026, and item 3 for a read. A
store still computes the address. combat.c keeps its workarounds for
the first two.

None of these is a miscompile. Each makes code larger than it has to
be, and the first two have workarounds in combat.c.

1. A comparison of a masked byte with a constant, and a signed byte's
   `> 0`, are made at 16 bits with the overflow test.

   ```c
   void f(void);
   void g(uint8_t *p) { if ((p[6] & 0x3F) >= 40) f(); }
   void h(uint8_t a, uint8_t b) { int8_t d = (int8_t)(a - b); if (d > 0) f(); }
   ```

   g compiles to `AND #$3F; LDY #0; SEC; SBC #$28; TYA; SBC #0; BVC;
   EOR #$80; BMI`, where `uint8_t v = p[6] & 0x3F; if (v >= 40)` gives
   `AND #$3F; CMP #$28; BCC`. h sign-extends d and subtracts it from 0
   at 16 bits. combat.c puts each masked byte in a `uint8_t` local, or
   casts it to `uint8_t`, before comparing it (eight places), and writes `d > 0` as `d != 0 && (int8_t)d >=
   0` (mob_attack), which compiles to byte tests.

2. Storing a constant to a zero-page pointer global makes every field
   access through that pointer in the same function go through a
   computed copy of the pointer.

   ```c
   struct mob { uint8_t a, b, c, d, look; };
   extern struct mob *P [[c6502::zeropage(0xF4)]];
   void next(void);
   void walk(uint8_t n) {
       if (n == 0) { P = 0; return; }
       while (P->look >= 0x15) next();
   }
   ```

   The loop compiles to `LDA P; CLC; ADC #4; STA t; LDA P+1; ADC #0;
   STA t+1; LDY #0; LDA (t),Y` in place of `LDY #4; LDA (P),Y`.
   Storing a variable that holds 0 (`P = (struct mob *)(uint16_t)n;`)
   gives `LDY #4; LDA (P),Y`. pick_random_member stores its count,
   which is 0 there, as the original does with `STA MOB_PTR`.

3. A 16-bit member read or written through a pointer parameter is
   reached through a computed pointer to the member, and a second copy
   of it in DPTR, where `LDY #13; LDA (r),Y; ... INY` would do.

   ```c
   struct rec { uint8_t pad[13]; uint16_t gold; };
   uint16_t get(struct rec *r) { return r->gold; }
   ```

   compiles to 19 bytes of address arithmetic and copies before the
   two loads. This costs modify_char_stats and apply_damage about 20
   bytes each. No workaround is in combat.c.

## Size

combat.c's code is 3146 bytes in the `-O` output, against the
original's 2856 bytes for the same routines (UNITS.md's sizes, from
main.lst), 290 bytes or 10% more. The compiler factors code shared by
several functions into `name@runN` pieces, and the table counts each
piece under the function whose name it carries, so a function that
lends its pieces to others looks larger than it is.

| Routine | Original | C |
|---|---|---|
| check_encounter | 71 | 88 |
| find_mob_at_same_pos | 69 | 73 |
| check_hostile | 50 | 60 |
| get_hostility | 14 | 16 |
| check_adjacent_threats | 88 | 75 |
| sum_hostile_at_pos | 59 | 64 |
| apply_damage | 211 | 302 |
| map_value_to_string | 53 | 72 |
| pick_random_member | 42 | 46 |
| apply_damage_to_current | 13 | 11 |
| apply_damage_to_mob | 13 | 11 |
| auto_heal | 67 | 63 |
| increment_hp | 33 | 36 |
| enter_combat_state | 22 | 24 |
| start_combat | 11 | 13 |
| attack_right, _left, _up, _down | 18 | 39 |
| find_attack_target | 77 | 61 |
| init_attack_display | 39 | 44 |
| select_someone_else | 6 | 6 |
| check_attack_eligible | 38 | 39 |
| next_colocated_mob | 33 | 33 |
| resolve_attack and add_willingness | 248 | 278 |
| calc_combat_strength | 113 | 143 |
| player_attack | 248 | 314 |
| process_entity_script | 23 | 23 |
| mob_attack | 132 | 117 |
| select_combat_target | 149 | 145 |
| get_entity_threat | 36 | 43 |
| load_target_ptr | 16 | 16 |
| handle_encounter | 240 | 253 |
| show_locked_msg | 19 | 17 |
| handle_event_slot and event_prng_check | 269 | 268 |
| handle_mob_encounter | 169 | 150 |
| set_target_restricted | 9 | 9 |
| check_threats_here | 12 | 21 |
| modify_char_stats | 135 | 162 |
| modify_current_stats | 11 | 11 |
| Total | 2856 | 3146 |

The ATTACK_ entries are 11 bytes each in C against the original's 5,
since each is an increment, a branch, and two jumps. Much of
apply_damage's and player_attack's excess is the stores of each
argument into its callee's zero-page slot, where the original passes
it in A or leaves it in EFFECT_REC, and apply_damage also pays for
gap 3.

## For the integrator

- find_attack_target, the ATTACK_ entries, select_someone_else,
  handle_event_slot, and handle_mob_encounter return CHAIN_ states as
  UNITS.md lists. handle_event_slot returns CHAIN_MOVE_LOOP only from
  its first branch (`$1D85`) and CHAIN_RETURN everywhere else.
- PLAYER_ATTACK and PICK_RANDOM_MEMBER reach $0000-$0004 through
  MOB_PTR = 0 or 1, as the header's NULL_PAGE_LOW reservation expects.
- combat.c uses DPTR (`$24`/`$25` in the classic layout) through the
  compiler's own code for gap 3.
