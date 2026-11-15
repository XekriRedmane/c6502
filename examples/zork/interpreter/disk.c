/*
    disk.c -- reading and writing 256-byte sectors through DOS's RWTS.

    Sectors are numbered from the start of track 3, SECTORS_PER_TRACK
    to a track. On the game diskette, sector n holds page n of the story
    file. A save diskette holds eight saves, 64 sectors apart.

    Departures from the original:

    - The routines take the sector and the buffer as arguments, where
      the original passes them in SCRATCH1 and SCRATCH2.
    - read_next_sector, inc_sector_and_read, write_next_sector, and
      inc_sector_and_write, which step the save's sector number, are
      in save.c with the code that keeps that number. They're quoted
      here, where the original has them.
    - The original's write path, from the label .write_next_sector, is
      write_to_sector, which writes one sector.

    Zork I (r15), output/main.asm lines 3359-3448:

    iob:
        DC      #$01            ; table_type (must be 1)
    iob.slot_times_16:
        DC      #$60            ; slot_times_16
    iob.drive:
        DC      #$01            ; drive_number
        DC      #$00            ; volume
    iob.track:
        DC      #$00            ; track
    iob.sector:
        DC      #$00            ; sector
        DC.W    #dct            ; dct_addr
    iob.buffer:
        DC.W    #$0000          ; buffer_addr
        DC      #$00            ; unused
        DC      #$00            ; partial_byte_count
    iob.command:
        DC      #$00            ; command
        DC      #$00            ; ret_code
        DC      #$00            ; last_volume
        DC      #$60            ; last_slot_times_16
        DC      #$01            ; last_drive_number
    dct:
        DC      #$00            ; device_type (0 for DISK II)
        DC      #$01            ; phases_per_track (1 for DISK II)
    dct.motor_count:
        DC.W    #$D8EF          ; motor_on_time_count ($EFD8 for DISK II)
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
    read_next_sector:
        STOW     #BUFF_AREA, SCRATCH2
    inc_sector_and_read:
        INCW     SCRATCH1
    read_from_sector:
        LDA      #$01
        JSR      do_rwts_on_sector
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
*/
#include "zork.h"

struct dct dct = { 0x00, 0x01, 0xD8EF };

struct iob iob = {
    0x01, 0x60, 0x01, 0x00, 0x00, 0x00, &dct, 0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x60, 0x01,
};

/* Reads or writes (`command`) sector `sector` to or from `buffer`.
 * Returns true if the operation failed. */
[[c6502::return_bool_in_carry]]
static bool do_rwts_on_sector(register uint8_t command, uint16_t sector,
                              uint8_t *const buffer) {
    iob.command = command;
    iob.buffer_addr = buffer;
    iob.track = 3;
    while (sector >= SECTORS_PER_TRACK) {
        sector -= SECTORS_PER_TRACK;
        iob.track++;
    }
    iob.sector = (uint8_t)sector;
    return RWTS(&iob);
}

[[c6502::return_bool_in_carry]]
bool read_from_sector(uint16_t sector, uint8_t *buffer) {
    return do_rwts_on_sector(1, sector, buffer);
}

/* Writes sector `sector` from `buffer`, with the motor's spin-up time
 * set for a write. Returns true if the write failed. */
[[c6502::return_bool_in_carry]]
bool write_to_sector(uint16_t sector, uint8_t *buffer) {
    uint16_t motor = dct.motor_on_time_count;
    dct.motor_on_time_count = 0xD8EF;
    bool failed = do_rwts_on_sector(2, sector, buffer);
    dct.motor_on_time_count = motor;
    return failed;
}
