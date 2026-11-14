/*
    disk.c -- reading a story page through DOS's RWTS.

    Zork I (r15), output/main.asm lines 3359-3431: the IOB and DCT
    (3359-3386), do_rwts_on_sector (3387-3415), and read_from_sector
    (3426-3431):

    do_rwts_on_sector:
        STA      iob.command
        MOVW     SCRATCH2, iob.buffer
        STOB     #$03, iob.track
        LDA      SCRATCH1
        LDX      SCRATCH1+1
        SEC
    .adjust_track:
        SBC      SECTORS_PER_TRACK
        BCS      .inc_track
        DEX
        BMI      .do_read
        SEC
    .inc_track:
        INC      iob.track
        JMP      .adjust_track
    .do_read:
        CLC
        ADC      SECTORS_PER_TRACK
        STA      iob.sector
        LDA      #$1D
        LDY      #$AC
        JSR      RWTS
        RTS
    read_from_sector:
        LDA      #$01
        JSR      do_rwts_on_sector
        RTS
*/
#include "zork.h"

struct iob iob = {
    0x01, 0x60, 0x01, 0x00, 0x00, 0x00, &dct, 0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x01,
};

struct dct dct = { 0x00, 0x01, 0xD8EF };

/* Reads or writes (`command`) the 256-byte sector numbered SCRATCH1,
 * counting from the start of track 3, to or from the buffer at
 * SCRATCH2. Returns true if the operation failed. */
__attribute__((return_bool_in_carry))
bool do_rwts_on_sector(register uint8_t command) {
    iob.command = command;
    iob.buffer_addr = (uint8_t *)SCRATCH2;
    iob.track = 3;
    uint16_t sector = SCRATCH1;
    while (sector >= SECTORS_PER_TRACK) {
        sector -= SECTORS_PER_TRACK;
        iob.track++;
    }
    iob.sector = (uint8_t)sector;
    return RWTS(&iob);
}

__attribute__((return_bool_in_carry)) bool read_from_sector(void) {
    return do_rwts_on_sector(1);
}
