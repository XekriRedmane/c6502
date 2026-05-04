/* interlace_fill_p1.c
 *
 * C99 port of Drol's INTERLACE_FILL_P1 helper ($0A0F).
 *
 * Broadcasts a single byte across 105 hi-res page-1 row positions at
 * a given column.  The painted rows form three 35-row bands
 * (72-106, 112-146, 152-186) with the three text-strip boundary rows
 * (67/107/147) and the top-HUD / bottom-text areas deliberately left
 * untouched.  Each group of 3 stores covers the same relative row in
 * each of the three bands, weaving them together over 35 groups.
 *
 * The original 6502 routine unrolls the 105 stores to avoid a row-base
 * lookup; this C version does the opposite, since on any modern host
 * the table-and-loop form is cheaper and the unroll buys nothing.
 */

#include <stdint.h>

/* Apple II hi-res page 1 framebuffer: $2000-$3FFF. */
#define HGR_PAGE1_ADDR 0x2000
static uint8_t* hires_page1 = (uint8_t*)HGR_PAGE1_ADDR;

/* Offsets into hires_page1 (i.e. effective_addr - $2000) for the 105
* painted rows, in the same band-interlaced order as the 6502 unroll.
* Each line is one row group: { band 0 (rows 72-106),
*                               band 1 (rows 112-146),
*                               band 2 (rows 152-186) }. */
static const uint16_t interlace_p1_offsets[105] = {
    0x00A8, 0x0328, 0x01D0,  /* rows  72 / 112 / 152 */
    0x04A8, 0x0728, 0x05D0,  /* rows  73 / 113 / 153 */
    0x08A8, 0x0B28, 0x09D0,  /* rows  74 / 114 / 154 */
    0x0CA8, 0x0F28, 0x0DD0,  /* rows  75 / 115 / 155 */
    0x10A8, 0x1328, 0x11D0,  /* rows  76 / 116 / 156 */
    0x14A8, 0x1728, 0x15D0,  /* rows  77 / 117 / 157 */
    0x18A8, 0x1B28, 0x19D0,  /* rows  78 / 118 / 158 */
    0x1CA8, 0x1F28, 0x1DD0,  /* rows  79 / 119 / 159 */
    0x0128, 0x03A8, 0x0250,  /* rows  80 / 120 / 160 */
    0x0528, 0x07A8, 0x0650,  /* rows  81 / 121 / 161 */
    0x0928, 0x0BA8, 0x0A50,  /* rows  82 / 122 / 162 */
    0x0D28, 0x0FA8, 0x0E50,  /* rows  83 / 123 / 163 */
    0x1128, 0x13A8, 0x1250,  /* rows  84 / 124 / 164 */
    0x1528, 0x17A8, 0x1650,  /* rows  85 / 125 / 165 */
    0x1928, 0x1BA8, 0x1A50,  /* rows  86 / 126 / 166 */
    0x1D28, 0x1FA8, 0x1E50,  /* rows  87 / 127 / 167 */
    0x01A8, 0x0050, 0x02D0,  /* rows  88 / 128 / 168 */
    0x05A8, 0x0450, 0x06D0,  /* rows  89 / 129 / 169 */
    0x09A8, 0x0850, 0x0AD0,  /* rows  90 / 130 / 170 */
    0x0DA8, 0x0C50, 0x0ED0,  /* rows  91 / 131 / 171 */
    0x11A8, 0x1050, 0x12D0,  /* rows  92 / 132 / 172 */
    0x15A8, 0x1450, 0x16D0,  /* rows  93 / 133 / 173 */
    0x19A8, 0x1850, 0x1AD0,  /* rows  94 / 134 / 174 */
    0x1DA8, 0x1C50, 0x1ED0,  /* rows  95 / 135 / 175 */
    0x0228, 0x00D0, 0x0350,  /* rows  96 / 136 / 176 */
    0x0628, 0x04D0, 0x0750,  /* rows  97 / 137 / 177 */
    0x0A28, 0x08D0, 0x0B50,  /* rows  98 / 138 / 178 */
    0x0E28, 0x0CD0, 0x0F50,  /* rows  99 / 139 / 179 */
    0x1228, 0x10D0, 0x1350,  /* rows 100 / 140 / 180 */
    0x1628, 0x14D0, 0x1750,  /* rows 101 / 141 / 181 */
    0x1A28, 0x18D0, 0x1B50,  /* rows 102 / 142 / 182 */
    0x1E28, 0x1CD0, 0x1F50,  /* rows 103 / 143 / 183 */
    0x02A8, 0x0150, 0x03D0,  /* rows 104 / 144 / 184 */
    0x06A8, 0x0550, 0x07D0,  /* rows 105 / 145 / 185 */
    0x0AA8, 0x0950, 0x0BD0,  /* rows 106 / 146 / 186 */
};

/* Broadcast `value` to column `col` across the 105 interlaced rows of
* hi-res page 1.  Inputs match the 6502 entry contract: A=value, X=col.
* Caller must hold col in 0..39. */
__attribute__((zp_abi)) void interlace_fill_p1(uint8_t value, uint8_t col)
{
    for (uint8_t i = 0; i < 105; i++)
        hires_page1[interlace_p1_offsets[i] + col] = value;
}
