#include <stdint.h>

/*
    get_object_addr, from the Zork I (r15) Z-machine interpreter for
    the Apple II, as reverse-engineered in
    https://github.com/XekriRedmane/zorki-r15-reveng (main.nw,
    "Getting an object's address").

    The object table starts with 31 words of property defaults,
    followed by up to 255 objects of 9 bytes each, numbered from 1.
    So object n is at object_table + 62 + 9 * (n - 1), which is
    object_table + 53 + 9 * n. The story file's header (page-aligned
    in RAM) holds the object table's offset, big-endian, at bytes
    $0A-$0B.

    The original:

        STA  SCRATCH2               ; SCRATCH2 = 9 * A
        LDA  #$00
        STA  SCRATCH2+1
        LDA  SCRATCH2
        ASL  SCRATCH2
        ROL  SCRATCH2+1
        ASL  SCRATCH2
        ROL  SCRATCH2+1
        ASL  SCRATCH2
        ROL  SCRATCH2+1
        CLC
        ADC  SCRATCH2
        BCC  .continue
        INC  SCRATCH2+1
        CLC
    .continue:
        ADC  #FIRST_OBJECT_OFFSET   ; + 53
        STA  SCRATCH2
        BCC  .continue2
        INC  SCRATCH2+1
    .continue2:
        LDY  #HEADER_OBJECT_TABLE_ADDR+1
        LDA  (Z_HEADER_ADDR),Y      ; + table offset, low byte
        CLC
        ADC  SCRATCH2
        STA  SCRATCH2
        DEY
        LDA  (Z_HEADER_ADDR),Y      ; high byte
        ADC  SCRATCH2+1
        ADC  Z_HEADER_ADDR+1        ; + the header's page
        STA  SCRATCH2+1
        RTS

    It takes the object number in A and leaves the address in the
    zero-page word SCRATCH2, where callers use it as the base of
    `(SCRATCH2),Y`. Here the object number is the zp_abi parameter
    and the address comes back as a pointer (in HARGS, also zero
    page). The original keeps the header pointer Z_HEADER_ADDR in
    zero page at $BA, and `z_header_addr` is placed there too, so
    it's read as the `(zp),Y` base directly.
*/

#define HEADER_OBJECT_TABLE_ADDR 0x0A
#define FIRST_OBJECT_OFFSET 53

uint8_t *z_header_addr [[c6502::zeropage(0xBA)]];

uint8_t *get_object_addr(uint8_t obj) {
    uint16_t table = ((uint16_t)z_header_addr[HEADER_OBJECT_TABLE_ADDR] << 8)
                     | z_header_addr[HEADER_OBJECT_TABLE_ADDR + 1];
    return z_header_addr + table + obj * 9 + FIRST_OBJECT_OFFSET;
}

static uint8_t story[0x200];

int main(void) {
    /* A header at story[0], with the object table at offset $0100. */
    z_header_addr = story;
    story[HEADER_OBJECT_TABLE_ADDR] = 0x01;
    story[HEADER_OBJECT_TABLE_ADDR + 1] = 0x00;
    /* Object 3 is at story + $0100 + 53 + 27. */
    return (int)(get_object_addr(3) - story);
}
