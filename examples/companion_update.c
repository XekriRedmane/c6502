#include <stdbool.h>
#include <stdint.h>

/* === 2-slot companion state (shared with DRAW_ENTITIES phase 4) === */
extern uint8_t companion_state[2];     /* $00 idle, +ve active, $FF drift */
extern uint8_t companion_dir[2];       /* $01 right, $FF left, $00=right */
extern uint8_t companion_pos_lo[2];    /* world-X low byte */
extern uint8_t companion_pos_hi[2];    /* world-X high byte */
extern uint8_t companion_row[2];       /* screen row */

/* === Cross-subsystem state === */
extern uint8_t hit_flag;               /* $FF = player was hit this frame */
extern uint8_t entity_hit_state[];     /* hit-entity active flags */
extern uint8_t entity_hit_row[];       /* hit-entity row attributes */

/* === Per-level tables === */
extern const uint8_t floor_thresh[];          /* per-floor row anchor */
extern const uint8_t perspective_xoff_lo[256];
extern const uint8_t perspective_xoff_hi[256];

__attribute__((zp_abi))
extern uint8_t prng(void);

__attribute__((zp_abi))
extern void draw_sprite(uint8_t width, uint8_t height,
                        uint8_t sprite_x, uint8_t sprite_y,
                        const uint8_t *tile_src,
                        uint8_t page_flag);

/* PROJ_SCREEN_COL: 132-byte "horizontal residue -> screen column" table. */
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

/* PROJ_FRAME_IDX: 165-byte (X mod 7) table cycling 0..6. */
static const uint8_t proj_frame_idx[165] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x00, 0x01, 0x02, 0x03,
};

/* Companion body sprite pointer tables: 7 perspective frames per pose,
* 3 poses per direction.
*   +dir POSE1..3 -> $A051..$A2D1
*   -dir POSE1..3 -> $9DB1..$A031
* Walk cycle visits 1 -> 2 -> 3 -> 1 on successive draws. */
static const uint8_t companion_pos_pose1_lo[7] = { 0x51, 0x71, 0x91, 0xB1, 0xD1, 0xF1, 0x11 };
static const uint8_t companion_pos_pose1_hi[7] = { 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA1 };
static const uint8_t companion_pos_pose2_lo[7] = { 0x31, 0x51, 0x71, 0x91, 0xB1, 0xD1, 0xF1 };
static const uint8_t companion_pos_pose2_hi[7] = { 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1 };
static const uint8_t companion_pos_pose3_lo[7] = { 0x11, 0x31, 0x51, 0x71, 0x91, 0xB1, 0xD1 };
static const uint8_t companion_pos_pose3_hi[7] = { 0xA2, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2 };

static const uint8_t companion_neg_pose1_lo[7] = { 0xB1, 0xD1, 0xF1, 0x11, 0x31, 0x51, 0x71 };
static const uint8_t companion_neg_pose1_hi[7] = { 0x9D, 0x9D, 0x9D, 0x9E, 0x9E, 0x9E, 0x9E };
static const uint8_t companion_neg_pose2_lo[7] = { 0x91, 0xB1, 0xD1, 0xF1, 0x11, 0x31, 0x51 };
static const uint8_t companion_neg_pose2_hi[7] = { 0x9E, 0x9E, 0x9E, 0x9E, 0x9F, 0x9F, 0x9F };
static const uint8_t companion_neg_pose3_lo[7] = { 0x71, 0x91, 0xB1, 0xD1, 0xF1, 0x11, 0x31 };
static const uint8_t companion_neg_pose3_hi[7] = { 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0xA0, 0xA0 };

/* Pose-indexed dispatch: companion_{pos,neg}_{lo,hi}_tbl[pose][frame_idx]. */
static const uint8_t *const companion_pos_lo_tbl[3] = {
    companion_pos_pose1_lo, companion_pos_pose2_lo, companion_pos_pose3_lo,
};
static const uint8_t *const companion_pos_hi_tbl[3] = {
    companion_pos_pose1_hi, companion_pos_pose2_hi, companion_pos_pose3_hi,
};
static const uint8_t *const companion_neg_lo_tbl[3] = {
    companion_neg_pose1_lo, companion_neg_pose2_lo, companion_neg_pose3_lo,
};
static const uint8_t *const companion_neg_hi_tbl[3] = {
    companion_neg_pose1_hi, companion_neg_pose2_hi, companion_neg_pose3_hi,
};

/* SMC walk-cycle state: which pose to draw next for each direction.
* Persists across calls (mirrors the asm's self-modified trampoline
* operand bytes).  0 = POSE1, 1 = POSE2, 2 = POSE3.  Initialised to 0
* to match the on-disk trampoline target of POSE1. */
static uint8_t pos_walk_next = 0;
static uint8_t neg_walk_next = 0;


/* Perspective transform: world-X -> 16-bit screen-X.  Caller checks the
* hi byte for off-screen (non-zero = off the left/right of the playfield). */
__attribute__((zp_abi))
static uint16_t compute_screen_x(uint8_t slot, uint8_t player_y, uint8_t sprite_xref)
{
    uint16_t xoff =
        ((uint16_t)perspective_xoff_hi[player_y] << 8) | perspective_xoff_lo[player_y];
    xoff = (uint16_t)(xoff - sprite_xref);
    uint16_t pos =
        ((uint16_t)companion_pos_hi[slot] << 8) | companion_pos_lo[slot];
    return (uint16_t)(pos - xoff);
}

/* Scan the hit-entity table top-down for the first positive (active)
* state slot; on success returns true with *out_row = ENTITY_HIT_ROW - 8. */
__attribute__((zp_abi))
static bool find_active_entity(uint8_t hit_max, uint8_t *out_row)
{
    for (int8_t i = (int8_t)hit_max; i >= 0; i--) {
        if ((entity_hit_state[i] & 0x80) == 0) {
            *out_row = (uint8_t)(entity_hit_row[i] - 0x08);
            return true;
        }
    }
    return false;
}

/* Entity-proximity check.  If an active entity shares this companion's
* row and the companion is in the centre band [$40,$47), transition to
* drift (state := $FF, row += 4).  Outside the centre, the
* direction-specific deactivation window ([$30,$38) for +dir,
* [$50,$58) for -dir) sets state := 0. */
__attribute__((zp_abi))
static void entity_proximity(uint8_t slot, uint8_t screen_x, uint8_t hit_max)
{
    uint8_t entity_row;
    if (!find_active_entity(hit_max, &entity_row)) return;
    if (entity_row != companion_row[slot]) return;

    if (screen_x >= 0x40 && screen_x < 0x47) {
        companion_state[slot] = 0xFF;
        companion_row[slot]   = (uint8_t)(companion_row[slot] + 0x04);
        return;
    }
    if ((companion_dir[slot] & 0x80) == 0) {
        if (screen_x >= 0x30 && screen_x < 0x38) companion_state[slot] = 0x00;
    } else {
        if (screen_x >= 0x50 && screen_x < 0x58) companion_state[slot] = 0x00;
    }
}

/* Body draw: pick the current SMC pose for the slot's direction, fetch
* the frame's sprite-data pointer, and draw.  state == 0 (deactivated
* by entity_proximity this frame) draws POSE1 without advancing the
* walk cycle; non-zero state both draws the current pose and advances
* the cycle to the next. */
__attribute__((zp_abi))
static void smc_body_draw(uint8_t slot,
                        uint8_t sprite_x, uint8_t sprite_y,
                        uint8_t frame_idx, uint8_t state,
                        uint8_t page_flag)
{
    bool is_neg = (companion_dir[slot] & 0x80) != 0;
    uint8_t pose;
    if (state == 0) {
        pose = 0;
    } else {
        uint8_t *next = is_neg ? &neg_walk_next : &pos_walk_next;
        pose  = *next;
        *next = (uint8_t)((*next + 1) % 3);
    }
    uint8_t lo, hi;
    if (is_neg) {
        lo = companion_neg_lo_tbl[pose][frame_idx];
        hi = companion_neg_hi_tbl[pose][frame_idx];
    } else {
        lo = companion_pos_lo_tbl[pose][frame_idx];
        hi = companion_pos_hi_tbl[pose][frame_idx];
    }
    const uint8_t *src = (const uint8_t *)(((uint16_t)hi << 8) | lo);
    draw_sprite(0x03, 0x08, sprite_x, sprite_y, src, page_flag);
}

/* Player-catch hit-box: writes $FF to hit_flag if the companion is in
* the screen-X band [$40,$50) and its row lies in
* (player_col - $08, player_col + $12]. */
__attribute__((zp_abi))
static void player_catch(uint8_t slot, uint8_t screen_x, uint8_t player_col)
{
    if (screen_x < 0x40 || screen_x >= 0x50) return;
    uint8_t low_edge = (uint8_t)(player_col - 0x08);
    if (low_edge >= companion_row[slot]) return;
    uint8_t high_edge = (uint8_t)(low_edge + 0x1A);
    if (high_edge < companion_row[slot]) return;
    hit_flag = 0xFF;
}

/* +dir active step: PRNG 5/256 flips direction, then pos += 3.  Re-arms
* when pos crosses hi==3 && lo>=$52: flip to -dir, reseat row to the
* player's current floor threshold + $0B.  Returns true if the slot
* should continue to draw, false if it re-armed (skip to next slot). */
__attribute__((zp_abi))
static bool active_pos_step(uint8_t slot, uint8_t player_floor)
{
    if (prng() < 0x05) {
        companion_dir[slot] = 0xFF;
    }
    uint16_t pos =
        ((uint16_t)companion_pos_hi[slot] << 8) | companion_pos_lo[slot];
    pos = (uint16_t)(pos + 3);
    companion_pos_lo[slot] = (uint8_t)pos;
    companion_pos_hi[slot] = (uint8_t)(pos >> 8);

    if ((pos >> 8) == 0x03 && (uint8_t)pos >= 0x52) {
        companion_dir[slot] = 0xFF;
        companion_row[slot] = (uint8_t)(floor_thresh[player_floor] + 0x0B);
        return false;
    }
    return true;
}

/* -dir active step: mirror of active_pos_step.  Re-arms when pos
* crosses hi==0 && lo<$3E. */
__attribute__((zp_abi))
static bool active_neg_step(uint8_t slot, uint8_t player_floor)
{
    if (prng() < 0x05) {
        companion_dir[slot] = 0x01;
    }
    uint16_t pos =
        ((uint16_t)companion_pos_hi[slot] << 8) | companion_pos_lo[slot];
    pos = (uint16_t)(pos - 3);
    companion_pos_lo[slot] = (uint8_t)pos;
    companion_pos_hi[slot] = (uint8_t)(pos >> 8);

    if ((pos >> 8) == 0x00 && (uint8_t)pos < 0x3E) {
        companion_dir[slot] = 0x01;
        companion_row[slot] = (uint8_t)(floor_thresh[player_floor] + 0x0B);
        return false;
    }
    return true;
}

/* Drift step: row += 4 each frame until the row matches one of the
* three floor anchors ($63 / $8B / $B3), at which point the slot
* re-activates (state := $01) and nudges its world-X by +/- 3 per
* direction.  Writes the sprite Y for this frame's draw to *out_sprite_y
* -- on the rearm transition the sprite is drawn one row back from the
* anchor while companion_row stays parked on the anchor itself. */
__attribute__((zp_abi))
static void drift_step(uint8_t slot, uint8_t *out_sprite_y)
{
    uint8_t row = companion_row[slot];
    if (row == 0x63 || row == 0x8B || row == 0xB3) {
        *out_sprite_y = (uint8_t)(row - 0x04);
        companion_state[slot] = 0x01;
        uint16_t pos =
            ((uint16_t)companion_pos_hi[slot] << 8) | companion_pos_lo[slot];
        if (companion_dir[slot] & 0x80) {
            pos = (uint16_t)(pos - 3);
        } else {
            pos = (uint16_t)(pos + 3);
        }
        companion_pos_lo[slot] = (uint8_t)pos;
        companion_pos_hi[slot] = (uint8_t)(pos >> 8);
    } else {
        row = (uint8_t)(row + 0x04);
        companion_row[slot] = row;
        *out_sprite_y = row;
    }
}


/**
* Per-frame tick + draw for the two-slot "companion" walker subsystem.
*
* Iterates slot index 1, then 0.  Each slot runs a three-state machine:
*
*   state == 0   Inactive.  The slot self-activates (state := 1) and
*                falls through to draw.
*   state >  0   Active.  Advances the 16-bit world-X by +/- 3 per
*                frame; PRNG (5/256) may flip direction.  On reaching
*                the world-X boundary the slot "re-arms" -- direction
*                flips and the row reseats to floor_thresh[player_floor]
*                + $0B.
*   state <  0   Drift (climbing).  Row += 4 per frame until reaching
*                a floor-anchor row ($63 / $8B / $B3), at which point
*                reactivate (state := 1) with a small horizontal nudge.
*
* The draw stage perspective-transforms the world position into a
* screen X, clips at $9A and against off-screen, runs the entity-
* proximity check that can flip the slot into drift or deactivate it,
* draws the body through a direction-specific SMC-chained 3-pose walk
* cycle, and then runs the player-catch hit-box.  Drift draws skip the
* $9A clip but otherwise share the SMC walk cycle and the player-catch.
*
* The whole routine is gated on `gate` (ZP_COMPANION_GATE): a negative
* value (bit 7 set) disables the subsystem and the routine returns
* immediately.  The minimum difficulty tier sets gate to $FF.
*
* @param gate          Subsystem gate.  Bit 7 set ($FF) disables the
*                      routine; non-negative values allow it to run.
* @param player_y      Player world-Y (ZP_PLAYER_Y).  Indexes the
*                      256-entry perspective_xoff tables for the
*                      world-X-to-screen-X transform.
* @param sprite_xref   World-X reference (ZP_SPRITE_XREF).  Subtracted
*                      from the perspective offset during transform.
* @param player_col    Player screen row (ZP_PLAYER_COL).  Used by the
*                      player-catch hit-box test.
* @param player_floor  Player's current floor index (ZP_PLAYER_FLOOR).
*                      Indexes floor_thresh to reseat the companion's
*                      row on re-arm.
* @param hit_max       Top hit-entity slot index (ZP_HIT_MAX).  Bounds
*                      the entity_proximity scan.
* @param page_flag     Bit 7 selects the hidden hi-res draw page;
*                      forwarded unchanged to draw_sprite.
*/
__attribute__((zp_abi))
void companion_update(uint8_t gate,
                    uint8_t player_y,
                    uint8_t sprite_xref,
                    uint8_t player_col,
                    uint8_t player_floor,
                    uint8_t hit_max,
                    uint8_t page_flag)
{
    if (gate & 0x80) return;

    for (int8_t slot = 1; slot >= 0; slot--) {
        uint8_t state = companion_state[slot];

        /* --- Drift path (bypasses the off-screen / clip / proximity stages) */
        if (state & 0x80) {
            uint8_t sprite_y;
            drift_step((uint8_t)slot, &sprite_y);

            uint16_t sx = compute_screen_x((uint8_t)slot, player_y, sprite_xref);
            uint8_t screen_x  = (uint8_t)sx;
            uint8_t sprite_x  = proj_screen_col[screen_x];
            uint8_t frame_idx = proj_frame_idx[screen_x];

            smc_body_draw((uint8_t)slot, sprite_x, sprite_y, frame_idx,
                        companion_state[slot], page_flag);
            player_catch((uint8_t)slot, screen_x, player_col);
            continue;
        }

        /* --- Inactive / active dispatch */
        if (state == 0) {
            companion_state[slot] = 0x01;
        } else {
            bool draw_this_frame =
                (companion_dir[slot] & 0x80)
                    ? active_neg_step((uint8_t)slot, player_floor)
                    : active_pos_step((uint8_t)slot, player_floor);
            if (!draw_this_frame) continue;
        }

        /* --- Draw preamble: transform, off-screen / right-edge clip */
        uint16_t sx = compute_screen_x((uint8_t)slot, player_y, sprite_xref);
        if ((sx >> 8) != 0) continue;
        uint8_t screen_x = (uint8_t)sx;
        if (screen_x >= 0x9A) continue;

        /* --- Entity proximity may flip the slot into drift or deactivate it */
        entity_proximity((uint8_t)slot, screen_x, hit_max);

        /* --- Body draw + player-catch */
        uint8_t sprite_y  = companion_row[slot];
        uint8_t sprite_x  = proj_screen_col[screen_x];
        uint8_t frame_idx = proj_frame_idx[screen_x];
        smc_body_draw((uint8_t)slot, sprite_x, sprite_y, frame_idx,
                    companion_state[slot], page_flag);
        player_catch((uint8_t)slot, screen_x, player_col);
    }
}