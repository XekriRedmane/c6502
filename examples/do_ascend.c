#include <stdint.h>

/* === Player position / motion state === */
extern uint8_t player_col;       /* ZP_PLAYER_COL: vertical screen column */
extern uint8_t move_dir;          /* ZP_MOVE_DIR: $00 idle, $01 right, $FF left */

/* === Floor mirrors (four parallel ZP bytes; each holds the current
*     floor index, read by a different subsystem) === */
extern uint8_t beam_seed_floor;  /* ZP_BEAM_SEED_FLOOR */
extern uint8_t floor_mirror;     /* ZP_FLOOR_MIRROR */
extern uint8_t dsc_floor;        /* ZP_DSC_FLOOR */

/* === Other game state === */
extern uint8_t ent_rescued;      /* ZP_ENT_RESCUED: $FF after rescue/hit */
extern uint8_t beam_tick;        /* ZP_BEAM_TICK: beam pre-activation tick */

/* === 12-slot hit-entity active flag table (shared with
*     refresh_hit_entities and the rest of the engine) === */
extern uint8_t entity_hit_state[];

/* === Per-floor tables (per-level data) === */
extern const uint8_t floor_ceil[];    /* per-floor ceiling Y */
extern const uint8_t floor_thresh[];  /* per-floor floor Y */

/* === Speaker-click helpers === */
extern void snd_delay_up(uint8_t pitch, uint8_t clicks);
extern void sfx_tone(uint8_t pitch, uint8_t duration);


/**
* Per-frame ascent step.  Called from MAIN_LOOP via the SMC_ASCEND
* slot while the player holds the 'A' (ascend) key.
*
* Steps player_col vertically by -4 toward the ascent-target floor's
* ceiling and dispatches on what (if anything) was landed on:
*
*   - Already at the ceiling row on entry: plays the landing tone
*     (sfx_tone($05, $04)) and ticks the beam subsystem; player_col
*     does not change.
*   - The -4 step lands exactly on the ceiling row: clears move_dir
*     and plays the landing tone + beam tick (same as the
*     already-at-ceiling case, with move_dir := 0 prepended).
*   - The -4 step lands exactly on the same slot's floor-side
*     threshold (the "other side" of the floor pair): propagates
*     asc_floor into the three other floor mirrors (beam_seed_floor,
*     floor_mirror, dsc_floor), sets ent_rescued := $FF, wipes all
*     hit-entity slots [0..hit_max] to $FF (inactive, bit 7 set), and
*     emits the rising slide (snd_delay_up($04, $08)).
*   - Otherwise the step is mid-air: just emits the rising slide.
*
* @param asc_floor  Ascent-target floor index (was ZP_ASC_FLOOR; the
*                   input handler seeds it).  Indexes floor_ceil and
*                   floor_thresh.
* @param hit_max    Top hit-entity slot index for the wipe pass (was
*                   ZP_HIT_MAX).  The wipe walks slots hit_max..0.
*/
void do_ascend(uint8_t asc_floor, uint8_t hit_max)
{
    uint8_t col = player_col;

    if (col == floor_ceil[asc_floor]) {
        /* already at the ceiling */
        sfx_tone(0x05, 0x04);
        beam_tick--;
        return;
    }

    col = (uint8_t)(col - 0x04);
    player_col = col;

    if (col == floor_ceil[asc_floor]) {
        /* landed on ceiling: clear motion, then landing tone + beam tick */
        move_dir = 0x00;
        sfx_tone(0x05, 0x04);
        beam_tick--;
        return;
    }

    if (col == floor_thresh[asc_floor]) {
        /* landed on the opposite-side floor threshold: propagate the
        * floor index into the three other mirrors, mark rescue/hit
        * seen, wipe the entire hit-entity table */
        beam_seed_floor = asc_floor;
        floor_mirror    = asc_floor;
        dsc_floor       = asc_floor;
        ent_rescued     = 0xFF;
        for (int8_t i = (int8_t)hit_max; i >= 0; i--) {
            entity_hit_state[i] = 0xFF;
        }
        /* fall through to the rising slide */
    }

    /* mid-air step OR landed-on-floor: rising slide */
    snd_delay_up(0x04, 0x08);
}