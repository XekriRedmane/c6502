 #include <stdint.h>

/* Apple II hi-res page 1 row base addresses, rows 0..31.
* The famously interleaved layout: row r maps to
*   0x2000 + (r & 7)*0x400 + ((r>>3) & 7)*0x80
* Page 2 is identical with +0x2000.                                */
static uint8_t* const HGR1_ROW[32] = {
    0x2000, 0x2400, 0x2800, 0x2C00, 0x3000, 0x3400, 0x3800, 0x3C00,
    0x2080, 0x2480, 0x2880, 0x2C80, 0x3080, 0x3480, 0x3880, 0x3C80,
    0x2100, 0x2500, 0x2900, 0x2D00, 0x3100, 0x3500, 0x3900, 0x3D00,
    0x2180, 0x2580, 0x2980, 0x2D80, 0x3180, 0x3580, 0x3980, 0x3D80,
};

/* Each of the 7 source bytes per column fans out into a row group.
* Row 0 is deliberately skipped.                                   */
static const uint8_t HUD_ROW_GROUPS[7][7] = {
    { 1,  2,  3,  4,  5,  6,  7 },   /* b0 -> rows 1..7  */
    { 8 },                           /* b1 -> row  8     */
    { 9, 10, 11, 12, 13, 14, 15 },   /* b2 -> rows 9..15 */
    {16 },                           /* b3 -> row 16     */
    {17, 18, 19, 20, 21, 22, 23 },   /* b4 -> rows 17..23*/
    {24 },                           /* b5 -> row 24     */
    {25, 26, 27, 28, 29, 30, 31 },   /* b6 -> rows 25..31*/
};
static const uint8_t HUD_ROW_COUNT[7] = { 7, 1, 7, 1, 7, 1, 7 };

#define HUD_COL_BASE 0x0C    /* hi-res byte column $0C..$1B = middle 16 of 40 */

__attribute__((zp_abi))
void paint_hud_strip_p1(const uint8_t *hud_strip_src /* 112 bytes @ $A30D */) {
    uint8_t y = 0;                          /* monotonic, NOT reset per column */
    for (int8_t x = 0x0F; x >= 0; x--) {       /* right-to-left, 16 columns */

        #pragma c6502 loop unroll(enable)
        for (uint8_t b = 0; b < 7; b++) {
            uint8_t pixels = hud_strip_src[y++];

            #pragma c6502 loop unroll(enable)
            for (uint8_t i = 0; i < HUD_ROW_COUNT[b]; i++) {
                *(HGR1_ROW[HUD_ROW_GROUPS[b][i]] + HUD_COL_BASE + x) = pixels;
            }
        }
    }
}
