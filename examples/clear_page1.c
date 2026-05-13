#include <stdint.h>

__attribute__((zp_abi)) extern void          interlace_fill_p1(uint8_t col, uint8_t paint);

uint8_t* const TEXT_STRIP_SRC = (uint8_t*)0x0300;   /* 40-byte dither source    */

/* Base addresses of the 34 hi-res rows the inner stripe writes hit (page 1). */
static uint8_t* const STRIPE_ROW[34] = {
    (uint8_t*)0x2600, (uint8_t*)0x2A00, (uint8_t*)0x2E00, (uint8_t*)0x3200,
    (uint8_t*)0x3600, (uint8_t*)0x3A00, (uint8_t*)0x3E00,
    (uint8_t*)0x2280, (uint8_t*)0x2680, (uint8_t*)0x2A80, (uint8_t*)0x2E80,
    (uint8_t*)0x3280, (uint8_t*)0x3680, (uint8_t*)0x3A80, (uint8_t*)0x3E80,
    (uint8_t*)0x2300, (uint8_t*)0x2700, (uint8_t*)0x2B00, (uint8_t*)0x2F00,
    (uint8_t*)0x3300, (uint8_t*)0x3700, (uint8_t*)0x3B00, (uint8_t*)0x3F00,
    (uint8_t*)0x2380, (uint8_t*)0x2780, (uint8_t*)0x2B80, (uint8_t*)0x2F80,
    (uint8_t*)0x3380, (uint8_t*)0x3780, (uint8_t*)0x3B80, (uint8_t*)0x3F80,
    (uint8_t*)0x2028, (uint8_t*)0x2428, (uint8_t*)0x2828,
};

/* The 12 interlaced rows of the bottom-4 text lines. */
static uint8_t* const TEXT_ROW[12] = {
    (uint8_t*)0x3028, (uint8_t*)0x3428, (uint8_t*)0x3828, (uint8_t*)0x3C28,
    (uint8_t*)0x32A8, (uint8_t*)0x36A8, (uint8_t*)0x3AA8, (uint8_t*)0x3EA8,
    (uint8_t*)0x3150, (uint8_t*)0x3550, (uint8_t*)0x3950, (uint8_t*)0x3D50,
};

/*
    Zero-paints hi-res page 1 ($2000-$3FFF) across the
    playfield column range [zp_clear_col_end+1 .. zp_clear_col]
    and then copies the 40-byte TEXT_STRIP_SRC dither buffer into
    12 interlaced hi-res rows.  Used to clear the game framebuffer
    between level transitions and after full redraws.
    Inputs:
      zp_clear_col --- high (starting) column.
      zp_clear_col_end --- low terminator (inclusive: stop when X==this).
      TEXT_STRIP_SRC[X] --- 40-byte ($28) bottom-text-row dither source
                            (column-indexed alternating $55/$2A).
*/
__attribute__((zp_abi))
void clear_page1(uint8_t zp_clear_col, uint8_t zp_clear_col_end)
{
    /* Phase 1: walk columns from zp_clear_col down to zp_clear_col_end
        (inclusive) and zero each of 34 explicit rows + 63 helper rows. */
    uint8_t x = zp_clear_col;
    for (;;) {
        #pragma c6502 loop unroll(enable)
        for (unsigned i = 0; i < 34; i++)
            STRIPE_ROW[i][x] = 0x00;
        interlace_fill_p1(x, 0x00);
        if (x == zp_clear_col_end) break;   /* CPX/BEQ before the DEX */
        x--;
    }

    /* Phase 2: re-lay the gray-dither strip into 12 bottom rows,
        indexed [39 .. 0] inclusive.                                   */
    uint8_t y = 39;
    for (;;) {
        uint8_t v = TEXT_STRIP_SRC[y];
        #pragma c6502 loop unroll(enable)
        for (unsigned i = 0; i < 12; i++)
            TEXT_ROW[i][y] = v;
        if (y == 0) break;
        y--;
    }
}