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

__attribute__((zp_abi))
extern void apply_bobble(uint8_t slot, uint8_t bobble_idx);

/* +dir step: anim_in is the pre-decrement anim counter (8 on the
* fresh-step path, 7..1 on the mid-step path). */
__attribute__((zp_abi))
static void step_pos(uint8_t slot, uint8_t anim_in)
{
    uint8_t new_anim = (uint8_t)(anim_in - 1);
    rescue_anim[slot] = new_anim;
    uint16_t world_x =
        ((uint16_t)entity_xoff_idx[slot] << 8) | entity_floor_col[slot];
    world_x = (uint16_t)(world_x + 3);
    entity_floor_col[slot] = (uint8_t)world_x;
    entity_xoff_idx[slot]  = (uint8_t)(world_x >> 8);
    apply_bobble(slot, new_anim);
}
