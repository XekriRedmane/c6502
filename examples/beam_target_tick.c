#include <stdint.h>

/* === Beam subsystem state (shared with BEAM_TARGET_DRAW / BEAM_UPDATE) === */
extern uint8_t beam_state;       /* $00 idle, $0X chase floor X, $FX attack floor X */
extern uint8_t beam_y;            /* current beam scan Y (Apple II hi-res row) */
extern uint8_t beam_snd_ctr;      /* post-hit jingle counter ($00..$0A, $FF=idle) */

/* === Per-floor ceiling Y anchors (ZP $7E..$82, level-data) === */
extern const uint8_t floor_ceil[];   /* 5 entries; chase ends at FLOOR_CEIL[X]+1 */

/* === Speaker-click helpers === */
extern void snd_delay_up(
    uint8_t pitch __attribute__((reg("A"))),
    uint8_t clicks __attribute__((reg("X"))));
extern void snd_delay_down(
    uint8_t pitch __attribute__((reg("A"))),
    uint8_t clicks __attribute__((reg("X"))));


/* FLOOR_Y_TABLE: per-floor walk-row Y anchors.  Five entries indexed by
* floor 0..4; entry 0 is unused (idle/no-floor sentinel).  These are
* the rows the chase phase climbs toward (via FLOOR_CEIL[X]+1) and the
* attack phase descends back onto. */
static const uint8_t floor_y_table[5] = {
    0x00, 0x43, 0x6B, 0x93, 0xBB,
};

/* BEAM_JINGLE: post-hit jingle pitches.  11 bytes ($0241..$024B in the
* asm).  Indexed by ZP_BEAM_SND_CTR after a successful beam hit; the
* counter ticks 10 -> 0 -> $FF and the routine plays one note per
* frame until the counter underflows. */
static const uint8_t beam_jingle[11] = {
    0x1E, 0x09, 0x16, 0x18, 0x1B, 0x0A, 0x07, 0x05,
    0x1E, 0x1B, 0x19,
};


/**
* One-frame advance of the beam target state machine.
*
* Two independent passes per call:
*
*   1. Jingle tick.  If beam_snd_ctr >= 0 (armed by a recent beam
*      hit), play beam_jingle[beam_snd_ctr] via snd_delay_up and
*      decrement the counter.  Runs regardless of beam_state.
*   2. State-machine dispatch on beam_state:
*
*      Attack ($FX, bit 7 set).  Target floor = state & $0F.  If
*        beam_y is already at floor_y_table[floor], clear state to
*        $00 (idle).  Otherwise beam_y += 2 (descend on-screen) and
*        play a pitch-$10 descending click.
*      Chase ($01..$7F).  Target floor = state (low nibble).  If
*        floor_ceil[floor]+1 == beam_y, OR state with $F0 to flip
*        into attack phase.  Otherwise beam_y -= 2 (ascend) and play
*        a pitch-$10 ascending click.
*      Idle ($00).  If beam_tick == 0, seed a fresh beam:
*        beam_y := floor_y_table[beam_seed_floor],
*        beam_state := beam_seed_floor.  Otherwise return -- the
*        beam stays dormant while the tick counter winds down
*        elsewhere (DO_ASCEND and DO_DESCEND DEC it on landings).
*
* @param beam_tick         ZP_BEAM_TICK: idle-phase seed gate.  Must
*                          be 0 for the beam to seed; non-zero values
*                          keep the beam idle this frame.  Decremented
*                          by the player-movement landing paths.
* @param beam_seed_floor   ZP_BEAM_SEED_FLOOR: floor index captured by
*                          DO_ASCEND / DO_DESCEND when the player last
*                          anchored on a floor.  Indexes
*                          floor_y_table and seeds beam_state at the
*                          start of each chase.
*/
void beam_target_tick(uint8_t beam_tick, uint8_t beam_seed_floor)
{
    /* --- Post-hit jingle tick (independent of beam_state) --- */
    int8_t snd_ctr = (int8_t)beam_snd_ctr;
    if (snd_ctr >= 0) {
        uint8_t pitch = beam_jingle[snd_ctr];
        beam_snd_ctr = (uint8_t)(snd_ctr - 1);
        snd_delay_up(pitch, 0x0A);
    }

    /* --- Dispatch on beam_state --- */
    uint8_t state = beam_state;

    if (state & 0x80) {
        /* Attack: descend toward FLOOR_Y_TABLE[floor] */
        uint8_t floor_idx = state & 0x0F;
        if (beam_y == floor_y_table[floor_idx]) {
            beam_state = 0x00;
            return;
        }
        beam_y = (uint8_t)(beam_y + 0x02);
        snd_delay_down(0x10, 0x0A);
        return;
    }

    if (state != 0) {
        /* Chase: ascend toward FLOOR_CEIL[floor] + 1 */
        uint8_t floor_idx = state;
        uint8_t target    = (uint8_t)(floor_ceil[floor_idx] + 0x01);
        if (target == beam_y) {
            beam_state = (uint8_t)(state | 0xF0);
            return;
        }
        beam_y = (uint8_t)(beam_y - 0x02);
        snd_delay_up(0x10, 0x0A);
        return;
    }

    /* Idle: gated on the tick counter winding down to zero */
    if (beam_tick == 0) {
        beam_y     = floor_y_table[beam_seed_floor];
        beam_state = beam_seed_floor;
    }
}