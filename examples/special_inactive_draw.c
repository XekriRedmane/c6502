#include <stdint.h>

__attribute__((zp_abi))
extern void draw_sprite(uint8_t width,
                        uint8_t height,
                        uint8_t sprite_x,
                        uint8_t sprite_y,
                        const uint8_t *tile_src,
                        uint8_t page_flag);

/* Per-level sprite data for the 2x6-byte "peek" marker.  Lives at
* SPECIAL_PEEK_DATA ($8CE3 in the asm) inside the swappable level-data
* region; the asm reads a fixed 16-bit pointer from SPECIAL_PEEK_LO /
* SPECIAL_PEEK_HI, which in C collapses to a single array reference. */
extern const uint8_t special_peek_sprite[];

/* PROJ_SCREEN_COL: 132-byte "horizontal residue -> screen column
* 0..$25" table.  Plateau widths cycle 4,3,4,3,... so two source-column
* slots feed each output column, giving sub-cell horizontal precision.
* Shared with the floor-enemy and other perspective draw routines. */
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
* Draw the small 2x6-byte "peek" marker for the special slot while
* it is in its inactive countdown phase ($AB == 0 in the asm).
*
* The peek marker is the static placeholder shown while the special
* slot is rearming.  Its screen position is fixed by the slot's
* row anchor (caller-supplied) plus the perspective projection of the
* slot's position high byte; the inactive-phase tick varies the
* position low byte but holds the high byte constant via
* SPECIAL_REARM, so the marker does not visibly drift while ticking.
*
* Always blits the same SPECIAL_PEEK_DATA frame -- the pointer is
* fixed in the level data, not indexed.
*
* @param special_row     Screen row anchor for the marker.  Used
*                        directly as the sprite Y coordinate.
* @param special_pos_hi  High byte of the special slot's 16-bit
*                        position.  Indexes proj_screen_col to produce
*                        the on-screen column (right-edge X) for the
*                        marker.
* @param page_flag       Bit 7 selects the hidden hi-res draw target
*                        (0 = page 1 at $2000, 1 = page 2 at $4000);
*                        forwarded unchanged to draw_sprite.
*/
__attribute__((zp_abi))
void special_inactive_draw(uint8_t special_row,
                            uint8_t special_pos_hi,
                            uint8_t page_flag)
{
    uint8_t sprite_x = proj_screen_col[special_pos_hi];
    draw_sprite(0x02, 0x06, sprite_x, special_row,
                special_peek_sprite, page_flag);
}
