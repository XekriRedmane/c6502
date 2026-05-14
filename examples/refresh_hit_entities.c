#include <stdint.h>

/* Per-slot hit-entity state (12 slots; runtime-modified by the game).
* Indexed by X = 0 .. hit_max (<= 11). */
uint8_t entity_hit_y[12];      /* world-Y per slot          */
uint8_t entity_hit_row[12];    /* screen row per slot       */
uint8_t entity_hit_state[12];  /* bit 7 = facing; $FF=inactive */

/* Hit-entity sprite-pointer tables (7 walking frames each).
* Indexed by sprite_xref (0..6).  Pairs decode to:
*   POS frames -> $8AC0 $8AE8 $8B10 $8B38 $8B60 $8B88 $8BB0
*   NEG frames -> $7AD4 $7AFC $7B24 $7B4C $7B74 $7B9C $7BC4
*/
const uint8_t hit_spr_pos_lo[7] = { 0xC0, 0xE8, 0x10, 0x38, 0x60, 0x88, 0xB0 };
const uint8_t hit_spr_pos_hi[7] = { 0x8A, 0x8A, 0x8B, 0x8B, 0x8B, 0x8B, 0x8B };
const uint8_t hit_spr_neg_lo[7] = { 0xD4, 0xFC, 0x24, 0x4C, 0x74, 0x9C, 0xC4 };
const uint8_t hit_spr_neg_hi[7] = { 0x7A, 0x7A, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B };

__attribute__((zp_abi))
extern void draw_sprite_opaque(
    uint8_t width, uint8_t height,
    uint8_t sprite_x, uint8_t sprite_y,
    const uint8_t *tile_src);

__attribute__((zp_abi))
void refresh_hit_entities(uint8_t hit_max,
                        uint8_t player_y,
                        uint8_t sprite_xref)
{
    uint8_t x = hit_max;                            
    do {                                            
        uint8_t hy = entity_hit_y[x];
        if (hy >= player_y) {                       
            uint8_t delta = (uint8_t)(hy - player_y);
            if (delta < 0x2F) {                     
                uint8_t lo;
                uint8_t hi;
                if (entity_hit_state[x] & 0x80) {   
                    hi = hit_spr_neg_hi[sprite_xref];
                    lo = hit_spr_neg_lo[sprite_xref];
                } else {
                    hi = hit_spr_pos_hi[sprite_xref];
                    lo = hit_spr_pos_lo[sprite_xref];
                }
                const uint8_t *src =
                    (const uint8_t *)(((uint16_t)hi << 8) | lo);

                draw_sprite_opaque(
                    0x07,                /* 7 bytes/row */
                    0x05,                /* 5 rows      */
                    delta,               /* sprite_x: right-edge column */
                    entity_hit_row[x],   /* sprite_y: screen row        */
                    src);
            }
        }
        x--;                       
    } while ((x & 0x80) == 0);                      
}