// `companion_update`, the two-slot companion walker's tick and draw,
// run in both pipelines from a `main` that records the state after each
// scenario, from tests/test_companion_update_sim.py.
//
// The program first showed an optimizer bug. The asm-SSA regalloc gave
// the outer loop counter `slot` both X (for `companion_state,X` and the
// loop's `DEX`) and the slot `__local_companion_update__slot`, which the
// `STX M / ... / LDX M` wrap saved across a JSR. `DEX` updated X but not
// M, so `LDA M; STA __zpabi_<callee>__slot` passed the stale value, and
// slot 0's iteration called the leaf functions with slot 1. The fix is
// src/passes/x_save_slot_load.h, which reads X in place of M when M is
// an X-save slot.
//
// The scenarios are A, the gate disabled (bit 7 set), which returns at
// once; B, both slots inactive, which activate themselves and draw; C,
// slot 1 active in the + direction midstream, which steps by 3, while
// slot 0 is clipped off screen; D, slot 1 at the + direction's turning
// point (hi == 3 and lo >= $52), which turns and is put on the row
// floor_thresh[player_floor] + $0B without a draw; E, the same for the
// - direction (hi == 0 and lo < $3E); F, a drift, where slot 1 on an
// anchor row is reactivated and slot 0 on another row moves down 4; G,
// an entity in the centre band, which starts a drift and still draws;
// and H, the player caught, which sets hit_flag.
#include <cstdint>
#include <vector>

#include "doctest.h"
#include "sim_case.h"

namespace c6502::suite {

namespace {

const char* const kProgram = R"c(
#include <stdbool.h>
#include <stdint.h>

/* ---- Storage for the externs declared in companion_update.c. ---- */
uint8_t companion_state[2];
uint8_t companion_dir[2];
uint8_t companion_pos_lo[2];
uint8_t companion_pos_hi[2];
uint8_t companion_row[2];
uint8_t hit_flag;
uint8_t entity_hit_state[16];
uint8_t entity_hit_row[16];

/* floor_thresh chosen so floor_thresh[i] + 0x0B equals a drift
 * anchor row ($63 / $8B / $B3). */
const uint8_t floor_thresh[4] = { 0x58, 0x80, 0xA8, 0x00 };

/* Perspective tables: all-zero so screen_x == companion_pos low. */
const uint8_t perspective_xoff_lo[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t perspective_xoff_hi[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* ---- Stubs for prng / draw_sprite. ---- */
uint8_t prng_value;
uint8_t prng_calls;

[[c6502::zp_abi]]
uint8_t prng(void) {
    prng_calls = (uint8_t)(prng_calls + 1);
    return prng_value;
}

uint8_t draw_calls;

[[c6502::zp_abi]]
void draw_sprite(uint8_t width, uint8_t height,
                 uint8_t sprite_x, uint8_t sprite_y,
                 const uint8_t *tile_src,
                 uint8_t page_flag) {
    draw_calls = (uint8_t)(draw_calls + 1);
}

/* ===========================================================
 * Verbatim body of examples/companion_update.c, minus the
 * extern declarations (the storage / stubs above replace them).
 * =========================================================== */

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

static const uint8_t *const companion_pos_lo_tbl[3] = {
)c" R"c(    companion_pos_pose1_lo, companion_pos_pose2_lo, companion_pos_pose3_lo,
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

static uint8_t pos_walk_next = 0;
static uint8_t neg_walk_next = 0;


[[c6502::zp_abi]]
static uint16_t compute_screen_x(uint8_t slot, uint8_t player_y, uint8_t sprite_xref)
{
    uint16_t xoff =
        ((uint16_t)perspective_xoff_hi[player_y] << 8) | perspective_xoff_lo[player_y];
    xoff = (uint16_t)(xoff - sprite_xref);
    uint16_t pos =
        ((uint16_t)companion_pos_hi[slot] << 8) | companion_pos_lo[slot];
    return (uint16_t)(pos - xoff);
}

[[c6502::zp_abi]]
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

[[c6502::zp_abi]]
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

[[c6502::zp_abi]]
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

[[c6502::zp_abi]]
static void player_catch(uint8_t slot, uint8_t screen_x, uint8_t player_col)
{
    if (screen_x < 0x40 || screen_x >= 0x50) return;
    uint8_t low_edge = (uint8_t)(player_col - 0x08);
    if (low_edge >= companion_row[slot]) return;
    uint8_t high_edge = (uint8_t)(low_edge + 0x1A);
    if (high_edge < companion_row[slot]) return;
    hit_flag = 0xFF;
}

[[c6502::zp_abi]]
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

[[c6502::zp_abi]]
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

[[c6502::zp_abi]]
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


[[c6502::zp_abi]]
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

        if (state == 0) {
            companion_state[slot] = 0x01;
        } else {
            bool draw_this_frame =
                (companion_dir[slot] & 0x80)
                    ? active_neg_step((uint8_t)slot, player_floor)
                    : active_pos_step((uint8_t)slot, player_floor);
            if (!draw_this_frame) continue;
        }

        uint16_t sx = compute_screen_x((uint8_t)slot, player_y, sprite_xref);
        if ((sx >> 8) != 0) continue;
        uint8_t screen_x = (uint8_t)sx;
        if (screen_x >= 0x9A) continue;

        entity_proximity((uint8_t)slot, screen_x, hit_max);

        uint8_t sprite_y  = companion_row[slot];
        uint8_t sprite_x  = proj_screen_col[screen_x];
        uint8_t frame_idx = proj_frame_idx[screen_x];
        smc_body_draw((uint8_t)slot, sprite_x, sprite_y, frame_idx,
                    companion_state[slot], page_flag);
        player_catch((uint8_t)slot, screen_x, player_col);
    }
}

/* =========================================================== */
/* Test harness.                                                */
/* =========================================================== */

/* 8 scenarios * 16 bytes = 128 bytes total. */
uint8_t result_log[128];
uint8_t log_idx;

void record(void) {
    uint8_t b = log_idx;
    result_log[(uint8_t)(b +  0)] = companion_state[0];
    result_log[(uint8_t)(b +  1)] = companion_state[1];
    result_log[(uint8_t)(b +  2)] = companion_dir[0];
    result_log[(uint8_t)(b +  3)] = companion_dir[1];
    result_log[(uint8_t)(b +  4)] = companion_pos_lo[0];
    result_log[(uint8_t)(b +  5)] = companion_pos_lo[1];
    result_log[(uint8_t)(b +  6)] = companion_pos_hi[0];
    result_log[(uint8_t)(b +  7)] = companion_pos_hi[1];
    result_log[(uint8_t)(b +  8)] = companion_row[0];
    result_log[(uint8_t)(b +  9)] = companion_row[1];
    result_log[(uint8_t)(b + 10)] = hit_flag;
    result_log[(uint8_t)(b + 11)] = draw_calls;
)c" R"c(    result_log[(uint8_t)(b + 12)] = prng_calls;
    result_log[(uint8_t)(b + 13)] = 0;
    result_log[(uint8_t)(b + 14)] = 0;
    result_log[(uint8_t)(b + 15)] = 0;
    log_idx = (uint8_t)(b + 16);
}

void reset_counters(void) {
    draw_calls = 0;
    prng_calls = 0;
    hit_flag = 0;
}

void clear_entity(void) {
    entity_hit_state[0] = 0x80;
    entity_hit_row[0]   = 0x00;
}

int main(void) {
    log_idx = 0;
    prng_value = 0xFF;   /* never trigger the 5/256 direction flip */

    /* === A. Gate disabled ($80 set) -> early return. === */
    companion_state[0] = 0xAA;
    companion_state[1] = 0xBB;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0x01;
    companion_pos_lo[0] = 0x40;
    companion_pos_lo[1] = 0x50;
    companion_pos_hi[0] = 0x00;
    companion_pos_hi[1] = 0x00;
    companion_row[0] = 0x60;
    companion_row[1] = 0x70;
    reset_counters();
    clear_entity();
    companion_update(0xFF, 0, 0, 0, 0, 0, 0);
    record();

    /* === B. Both slots inactive (state==0) -> self-activate + draw. === */
    companion_state[0] = 0x00;
    companion_state[1] = 0x00;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0x01;
    companion_pos_lo[0] = 0x70;
    companion_pos_lo[1] = 0x60;
    companion_pos_hi[0] = 0x00;
    companion_pos_hi[1] = 0x00;
    companion_row[0] = 0x80;
    companion_row[1] = 0x80;
    reset_counters();
    clear_entity();
    companion_update(0x00, 0, 0, 0x80, 0, 0, 0);
    record();

    /* === C. Slot 1 active +dir midstream + slot 0 off-screen. === */
    companion_state[0] = 0x01;
    companion_state[1] = 0x01;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0x01;
    companion_pos_lo[0] = 0x00;
    companion_pos_lo[1] = 0x60;
    companion_pos_hi[0] = 0x02;   /* slot 0 off-screen */
    companion_pos_hi[1] = 0x00;
    companion_row[0] = 0x60;
    companion_row[1] = 0x70;
    reset_counters();
    clear_entity();
    companion_update(0x00, 0, 0, 0x80, 0, 0, 0);
    record();

    /* === D. Slot 1 active +dir rearm boundary. === */
    companion_state[0] = 0x01;
    companion_state[1] = 0x01;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0x01;
    companion_pos_lo[0] = 0x10;
    companion_pos_lo[1] = 0x50;
    companion_pos_hi[0] = 0x05;   /* slot 0 off-screen */
    companion_pos_hi[1] = 0x03;
    companion_row[0] = 0x60;
    companion_row[1] = 0x70;
    reset_counters();
    clear_entity();
    /* player_floor=1 -> rearm row := floor_thresh[1] + 0x0B = $8B. */
    companion_update(0x00, 0, 0, 0x80, 1, 0, 0);
    record();

    /* === E. Slot 1 active -dir rearm boundary. === */
    companion_state[0] = 0x01;
    companion_state[1] = 0x01;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0xFF;
    companion_pos_lo[0] = 0x10;
    companion_pos_lo[1] = 0x3F;
    companion_pos_hi[0] = 0x05;   /* slot 0 off-screen */
    companion_pos_hi[1] = 0x00;
    companion_row[0] = 0x60;
    companion_row[1] = 0x70;
    reset_counters();
    clear_entity();
    /* player_floor=2 -> rearm row := floor_thresh[2] + 0x0B = $B3. */
    companion_update(0x00, 0, 0, 0x80, 2, 0, 0);
    record();

    /* === F. Drift: slot 1 on anchor row, slot 0 non-anchor. === */
    companion_state[0] = 0xFF;
    companion_state[1] = 0xFF;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0x01;
    companion_pos_lo[0] = 0x10;
    companion_pos_lo[1] = 0x70;
    companion_pos_hi[0] = 0x05;   /* still indexable: drift skips clip */
    companion_pos_hi[1] = 0x00;
    companion_row[0] = 0x5F;      /* non-anchor -> row += 4 */
    companion_row[1] = 0x63;      /* anchor -> reactivate, pos += 3 */
    reset_counters();
    clear_entity();
    companion_update(0x00, 0, 0, 0x80, 0, 0, 0);
    record();

    /* === G. Entity proximity in centre band -> drift transition. === */
    companion_state[0] = 0x01;
    companion_state[1] = 0x01;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0x01;
    companion_pos_lo[0] = 0x10;
    companion_pos_lo[1] = 0x40;
    companion_pos_hi[0] = 0x05;   /* slot 0 off-screen */
    companion_pos_hi[1] = 0x00;
    companion_row[0] = 0x60;
    companion_row[1] = 0x68;      /* matches entity_row */
    reset_counters();
    /* Active entity at row $70 -> entity_row = $70 - $08 = $68. */
    entity_hit_state[0] = 0x00;
    entity_hit_row[0]   = 0x70;
    /* player_col=$80 keeps the player-catch from firing here. */
    companion_update(0x00, 0, 0, 0x80, 0, 0, 0);
    record();

    /* === H. Player-catch hit. === */
    companion_state[0] = 0x01;
    companion_state[1] = 0x01;
    companion_dir[0]   = 0x01;
    companion_dir[1]   = 0x01;
    companion_pos_lo[0] = 0x10;
    companion_pos_lo[1] = 0x40;
    companion_pos_hi[0] = 0x05;   /* slot 0 off-screen */
    companion_pos_hi[1] = 0x00;
    companion_row[0] = 0x60;
    companion_row[1] = 0x70;
    reset_counters();
    clear_entity();
    /* player_col=$70 -> low_edge=$68 < row=$70 and high_edge=$82 >= row. */
    companion_update(0x00, 0, 0, 0x70, 0, 0, 0);
    record();

    return (int)log_idx;
}
)c";

// Each row is state0, state1, dir0, dir1, pos_lo0, pos_lo1, pos_hi0,
// pos_hi1, row0, row1, hit_flag, draw_calls, prng_calls, and three bytes
// of padding.
const std::vector<std::uint8_t> kExpected = {
    // A. The gate is disabled, and nothing changes.
    0xAA, 0xBB, 0x01, 0x01, 0x40, 0x50, 0x00, 0x00, 0x60, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // B. Both slots activate themselves and draw. entity_proximity
    // returns at the inactive entity, and the player-catch is skipped
    // since screen_x >= $50 for both slots.
    0x01, 0x01, 0x01, 0x01, 0x70, 0x60, 0x00, 0x00, 0x80, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    // C. Slot 1 steps from $00:$60 to $00:$63 and draws. Slot 0 steps
    // from $02:$00 to $02:$03 and is clipped. prng is called once for
    // each active step.
    0x01, 0x01, 0x01, 0x01, 0x03, 0x63, 0x02, 0x00, 0x60, 0x70, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00,
    // D. Slot 1 steps from $03:$50 to $03:$53 and turns: dir := $FF and
    // row := floor_thresh[1] + $0B = $8B, with no draw. Slot 0 is off
    // screen, and prng is still called.
    0x01, 0x01, 0x01, 0xFF, 0x13, 0x53, 0x05, 0x03, 0x60, 0x8B, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    // E. Slot 1 steps from $00:$3F to $00:$3C and turns: dir := $01 and
    // row := floor_thresh[2] + $0B = $B3, with no draw.
    0x01, 0x01, 0x01, 0x01, 0x13, 0x3C, 0x05, 0x00, 0x60, 0xB3, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    // F. Slot 1, drifting on the anchor row $63, is reactivated and
    // steps from $00:$70 to $00:$73. Slot 0, on row $5F, moves to $63
    // and keeps state $FF. Both draw, since a drift skips the clip, and
    // a drift doesn't call prng.
    0xFF, 0x01, 0x01, 0x01, 0x10, 0x73, 0x05, 0x00, 0x63, 0x63, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    // G. Slot 1 steps to $00:$43, so screen_x = $43, in [$40, $47). The
    // entity's row $70 - $08 = $68 is row[1], so state[1] := $FF and
    // row[1] := $6C, and the body still draws. The player-catch's low
    // edge $80 - 8 = $78 >= $6C, so there is no hit. Slot 0 is off
    // screen.
    0x01, 0xFF, 0x01, 0x01, 0x13, 0x43, 0x05, 0x00, 0x60, 0x6C, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00,
    // H. Slot 1 steps to $00:$43, the entity is inactive, and the body
    // draws. The low edge $70 - 8 = $68 < row $70 and the high edge $68 +
    // $1A = $82 >= $70, so hit_flag := $FF.
    0x01, 0x01, 0x01, 0x01, 0x13, 0x43, 0x05, 0x00, 0x60, 0x70, 0xFF, 0x01, 0x02, 0x00, 0x00, 0x00,
};

struct Run {
  sim::SimResult result;
  std::vector<std::uint8_t> log;
};

Run run(bool optimize) {
  sim::Simulation s = build_sim(kProgram, optimize);
  Run out{s.run(5'000'000), {}};
  INFO("optimize=" << optimize);
  CHECK_FALSE(out.result.timed_out);
  const std::int64_t at = s.symbol("result_log");
  out.log.assign(out.result.memory.begin() + at, out.result.memory.begin() + at + 16 * 8);
  return out;
}

}  // namespace

TEST_CASE("companion_update_sim: the unoptimized program records each scenario") {
  const Run r = run(false);
  // log_idx is 16 bytes for each of the 8 scenarios.
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 16);
  CHECK(r.log == kExpected);
}

TEST_CASE("companion_update_sim: the optimized program records each scenario") {
  const Run r = run(true);
  CHECK((r.result.return_int() & 0xFFFF) == 8 * 16);
  CHECK(r.log == kExpected);
}

TEST_CASE("companion_update_sim: both pipelines record the same state") {
  CHECK(run(false).log == run(true).log);
}

}  // namespace c6502::suite
