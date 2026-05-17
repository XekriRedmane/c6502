#include <stdint.h>

/* === 20-slot rescue-child state tables (shared with the draw side) === */
extern uint8_t entity_active[20];      /* 0=idle, +ve=walking, $FE=exit, other -ve=drift */
extern uint8_t entity_floor_col[20];   /* world-X low byte */
extern uint8_t entity_xoff_idx[20];    /* world-X high byte (0..3) */
extern uint8_t entity_floor_pos[20];   /* row (screen Y) */
extern uint8_t rescue_dir[20];         /* $01 = right, $FF = left */
extern uint8_t rescue_anim[20];        /* step-anim counter (0..8) */
extern uint8_t rescue_floor[20];       /* assigned floor index */
extern uint8_t rescue_countdown[20];   /* drift-mode rearm counter */

/* === Per-level tables === */
extern const uint8_t floor_thresh[];           /* per-floor row anchor */
extern const uint8_t floor_base_row[];         /* per-(floor_col) screen Y base */
extern const uint8_t perspective_xoff_byte[]; /* byte-domain perspective X offsets */
extern const uint8_t rescue_bobble[];          /* 7 signed step-Y deltas (bit 7 = sign) */

/* Apply a bobble-table delta to the slot's screen row.
* Bit 7 set = descend (row += magnitude); bit 7 clear = ascend
* (row -= magnitude).  Index is the post-decrement anim counter. */
__attribute__((zp_abi))
static void apply_bobble(uint8_t slot, uint8_t bobble_idx)
{
    uint8_t bobble    = rescue_bobble[bobble_idx];
    uint8_t magnitude = bobble & 0x7F;
    if (bobble & 0x80) {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] + magnitude);
    } else {
        entity_floor_pos[slot] = (uint8_t)(entity_floor_pos[slot] - magnitude);
    }
}