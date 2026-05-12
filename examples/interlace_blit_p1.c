#include <stdint.h>

/* Per-row framebuffer base for each of the 35 painted rows, for all
* three perspective bands (rows 72-106, 112-146, 152-186).  Add X to
* each entry to get the byte address.  Order is the source-stream
* order: row index k consumes (ZP_BLIT_SRC)[Y+k]. */
static uint8_t* const HIRES_P1_ROW[35][3] = {
    { 0x20A8, 0x2328, 0x21D0 }, { 0x24A8, 0x2728, 0x25D0 },
    { 0x28A8, 0x2B28, 0x29D0 }, { 0x2CA8, 0x2F28, 0x2DD0 },
    { 0x30A8, 0x3328, 0x31D0 }, { 0x34A8, 0x3728, 0x35D0 },
    { 0x38A8, 0x3B28, 0x39D0 }, { 0x3CA8, 0x3F28, 0x3DD0 },
    { 0x2128, 0x23A8, 0x2250 }, { 0x2528, 0x27A8, 0x2650 },
    { 0x2928, 0x2BA8, 0x2A50 }, { 0x2D28, 0x2FA8, 0x2E50 },
    { 0x3128, 0x33A8, 0x3250 }, { 0x3528, 0x37A8, 0x3650 },
    { 0x3928, 0x3BA8, 0x3A50 }, { 0x3D28, 0x3FA8, 0x3E50 },
    { 0x21A8, 0x2050, 0x22D0 }, { 0x25A8, 0x2450, 0x26D0 },
    { 0x29A8, 0x2850, 0x2AD0 }, { 0x2DA8, 0x2C50, 0x2ED0 },
    { 0x31A8, 0x3050, 0x32D0 }, { 0x35A8, 0x3450, 0x36D0 },
    { 0x39A8, 0x3850, 0x3AD0 }, { 0x3DA8, 0x3C50, 0x3ED0 },
    { 0x2228, 0x20D0, 0x2350 }, { 0x2628, 0x24D0, 0x2750 },
    { 0x2A28, 0x28D0, 0x2B50 }, { 0x2E28, 0x2CD0, 0x2F50 },
    { 0x3228, 0x30D0, 0x3350 }, { 0x3628, 0x34D0, 0x3750 },
    { 0x3A28, 0x38D0, 0x3B50 }, { 0x3E28, 0x3CD0, 0x3F50 },
    { 0x22A8, 0x2150, 0x23D0 }, { 0x26A8, 0x2550, 0x27D0 },
    { 0x2AA8, 0x2950, 0x2BD0 },
};

__attribute__((zp_abi))
void interlace_blit_p1(uint8_t* zp_blit_src, uint8_t zp_blit_x_start, uint8_t zp_blit_x_end)
{
    uint8_t y = 0;
    uint8_t x = zp_blit_x_start;

    do {
        if (x >= 0x28) {
            /* Off-screen column: skip paint but keep the source
            * stream aligned by advancing Y past this column's 35
            * bytes.  Y wraps mod 256, matching (zp),Y semantics. */
            y += (uint8_t)35;
        } else {
            #pragma c6502 loop unroll(enable)
            for (uint8_t k = 0; k < 35; k++) {
                uint8_t b = zp_blit_src[y];
                HIRES_P1_ROW[k][0][x] = b;   /* band 0 */
                HIRES_P1_ROW[k][1][x] = b;   /* band 1 */
                HIRES_P1_ROW[k][2][x] = b;   /* band 2 */
                y++;
            }
        }
        x--;
    } while (x != zp_blit_x_end);
}
