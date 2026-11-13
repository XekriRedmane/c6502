/*
    disk.c -- writing a save file's sectors through DOS's RWTS.

    Zork I (r15), output/main.asm lines 3359-3448: the IOB and DCT
    (3359-3386), do_rwts_on_sector (3387-3415), and write_next_sector
    and inc_sector_and_write (3432-3448):

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
    write_next_sector:
        STOW     #BUFF_AREA, SCRATCH2
    inc_sector_and_write:
        INCW     SCRATCH1
    .write_next_sector:
        PSHW     dct.motor_count
        STOW2    #$D8EF, dct.motor_count
        LDA      #$02
        JSR      do_rwts_on_sector
        PULW     dct.motor_count
        RTS

    write_next_sector falls into inc_sector_and_write; here it calls it.
*/
#include "zork.h"

struct dct dct = { 0x00, 0x01, 0xD8EF };

struct iob iob = {
    0x01, 0x60, 0x01, 0x00, 0x00, 0x00, &dct, 0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x01,
};

/* Reads or writes (`command`) the 256-byte sector numbered SCRATCH1,
 * counting from the start of track 3, to or from the buffer at
 * SCRATCH2. Returns true if the operation failed. */
__attribute__((return_bool_in_carry))
bool do_rwts_on_sector(register uint8_t command) {
    iob.command = command;
    iob.buffer_addr = SCRATCH2;
    iob.track = 3;
    uint16_t sector = SCRATCH1;
    while (sector >= SECTORS_PER_TRACK) {
        sector -= SECTORS_PER_TRACK;
        iob.track++;
    }
    iob.sector = (uint8_t)sector;
    return RWTS((uint8_t)((uint16_t)&iob >> 8), (uint8_t)(uint16_t)&iob);
}

/* Writes the sector after SCRATCH1 from SCRATCH2, with the motor's
 * spin-up time set for a write. Returns true if the write failed. */
__attribute__((return_bool_in_carry))
bool inc_sector_and_write(void) {
    SCRATCH1++;
    uint16_t motor = dct.motor_on_time_count;
    dct.motor_on_time_count = 0xD8EF;
    bool failed = do_rwts_on_sector(2);
    dct.motor_on_time_count = motor;
    return failed;
}

/* Writes the sector after SCRATCH1 from the line buffer. */
__attribute__((return_bool_in_carry))
bool write_next_sector(void) {
    SCRATCH2 = BUFF_AREA;
    return inc_sector_and_write();
}
