#include <stdint.h>

/* === Beam subsystem state (shared with BEAM_TARGET_TICK / BEAM_UPDATE) === */
extern uint8_t beam_state;        /* $00 idle, $0X chase, $FX attack */
extern uint8_t beam_y;             /* current beam scan row */
extern uint8_t beam_snd_ctr;       /* post-hit jingle counter */

/* === 4-slot floor-enemy state (shared with floor_enemy_advance) === */
extern uint8_t enemy_flag[4];      /* ZP_ENEMY_FLAG ($9B) */
extern uint8_t enemy_col[4];       /* ZP_ENEMY_COL ($9F) -- perspective-grid index */
extern uint8_t enemy_y[4];         /* ZP_ENEMY_Y ($A3) -- hi-res row */

/* === Score-addend ZP bytes (BCD; consumed by score_add) === */
extern uint8_t score_add_lo;       /* ZP_SCORE_ADD_LO */
extern uint8_t score_add_hi;       /* ZP_SCORE_ADD_HI */

/* === Per-level beam sprite ($B529; sprite id 0 of SPRITE_TABLE family) === */
extern const uint8_t beam_spr[];

/* === External routines === */
extern void score_add(void);
extern void draw_sprite(uint8_t width, uint8_t height,
                        uint8_t sprite_x, uint8_t sprite_y,
                        const uint8_t *tile_src,
                        uint8_t page_flag);


/* FLOOR_Y_TABLE: per-floor walk-row Y anchors.  Five entries indexed
* by floor 0..4 (low nibble of beam_state); entry 0 is unused. */
static const uint8_t floor_y_table[5] = {
    0x00, 0x43, 0x6B, 0x93, 0xBB,
};

/* PROJ_SCREEN_COL: 132-byte "horizontal residue -> screen column"
* table.  Plateau widths cycle 4,3,4,3,...  Shared with the floor-enemy
* and perspective draw routines, but the beam consumes it locally to
* test enemy column against the fixed beam column $24. */
static const uint8_t proj_screen_col[132] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02,
    0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04,
    0x04, 0x04, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06,
    0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08,
    0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B,
    0x0B, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D,
    0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F,
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x12,
    0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x14, 0x14,
    0x14, 0x14, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16,
    0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x18,
    0x19, 0x19, 0x19, 0x1A, 0x1A, 0x1A, 0x1A, 0x1B,
    0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1C, 0x1D, 0x1D,
    0x1D, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F,
    0x20, 0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x22,
    0x22, 0x22, 0x22, 0x23, 0x23, 0x23, 0x24, 0x24,
    0x24, 0x24, 0x25, 0x25,
};


/**
* Render the beam trail and run the 4-slot enemy-collision check.
*
* Three independent passes per call:
*
*   1. Gate.  beam_state == 0 -> return immediately (idle).
*   2. Trail.  Compute height = floor_y_table[floor_idx] - beam_y
*      where floor_idx is the low nibble of beam_state.  A zero delta
*      means the beam has reached its target this frame -- skip the
*      draw and the collision scan.  Otherwise clamp height to a
*      maximum of 4 rows and draw beam_spr (2 bytes/row, `height'
*      rows) at column $24, row beam_y, via draw_sprite.
*   3. Collision.  Walk enemy slots 3..0.  Each active slot
*      (enemy_flag[slot] != 0) whose screen column
*      (proj_screen_col[enemy_col[slot]]) lies in the
*      {$24, $25} band AND whose row sits in (beam_y, beam_y + 3] is
*      a hit: zero the enemy flag, return beam_state to idle ($00),
*      arm the post-hit jingle (beam_snd_ctr := $0A), and award
*      $0100 BCD via score_add.  Only the first hit is processed;
*      subsequent slots are skipped by the early return.
*
* Modifies (on hit): beam_state, beam_snd_ctr, enemy_flag[slot],
*                    score_add_lo / score_add_hi, plus whatever the
*                    framebuffer side of draw_sprite touches.
*
* @param page_flag  Bit 7 selects the hidden hi-res draw page;
*                   forwarded unchanged to draw_sprite.
*/
void beam_target_draw(uint8_t page_flag)
{
    /* --- Gate: idle beam does nothing --- */
    uint8_t state = beam_state;
    if (state == 0) return;

    /* --- Height computation, clamped to 4 rows --- */
    uint8_t floor_idx = state & 0x0F;
    uint8_t delta = (uint8_t)(floor_y_table[floor_idx] - beam_y);
    if (delta == 0) return;
    uint8_t height = (delta < 0x05) ? delta : 0x04;

    /* --- Draw the beam trail --- */
    draw_sprite(0x02, height, 0x24, beam_y, beam_spr, page_flag);

    /* --- 4-slot enemy collision scan (walks slots 3..0) --- */
    for (int8_t slot = 3; slot >= 0; slot--) {
        if (enemy_flag[slot] == 0) continue;

        /* Column test: PROJ_SCREEN_COL[enemy_col] must be in {$24, $25}. */
        uint8_t screen_col = proj_screen_col[enemy_col[slot]];
        if (screen_col > 0x25)  continue;
        if (screen_col <= 0x23) continue;

        /* Row test: enemy_y must be in (beam_y, beam_y + 3]. */
        uint8_t ey = enemy_y[slot];
        if (beam_y >= ey) continue;
        if ((uint8_t)(beam_y + 0x03) < ey) continue;

        /* HIT */
        enemy_flag[slot] = 0x00;
        beam_state       = 0x00;
        beam_snd_ctr     = 0x0A;
        score_add_lo     = 0x00;
        score_add_hi     = 0x01;
        score_add();
        return;
    }
}