/*
    disk.c -- reading and writing 256-byte sectors through DOS's RWTS.

    Sectors are numbered from the start of track 3, SECTORS_PER_TRACK
    to a track. On the game diskette, sector n holds page n of the story
    file. A save diskette holds eight saves, 64 sectors apart.

    Departures from the original:

    - The sector and the buffer are in disk_sector and disk_buffer,
      where the original keeps them in SCRATCH1 and SCRATCH2. Those
      are scratch bytes that other routines use too, and these two
      hold only the disk routines' arguments. do_rwts_on_sector counts
      the tracks in a local where the original subtracts from A and X.
    - The original's read_next_sector falls into inc_sector_and_read,
      which falls into read_from_sector, and its write routines work
      the same way. Here each ends by jumping to the next.
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

uint16_t disk_sector [[c6502::zeropage]];
uint8_t *disk_buffer [[c6502::zeropage]];

/* Reads or writes (`command`, 1 or 2) sector disk_sector to or from
 * disk_buffer. Returns true if the operation failed. The annotation
 * keeps the result in the carry: write_to_sector stores it in
 * `failed` while it restores the motor's spin-up time, which the
 * compiler's choice for a static function takes for a value it keeps
 * (docs/optimizations/carry_return_choice.md). */
[[c6502::return_bool_in_carry]]
static bool do_rwts_on_sector(register uint8_t command) {
    iob.command = command;
    iob.buffer_addr = disk_buffer;
    iob.track = 3;
    uint16_t s = disk_sector;
    while (s >= SECTORS_PER_TRACK) {
        s -= SECTORS_PER_TRACK;
        iob.track++;
    }
    iob.sector = (uint8_t)s;
    return RWTS(&iob);
}

bool read_from_sector(void) {
    return do_rwts_on_sector(1);
}

bool inc_sector_and_read(void) {
    disk_sector++;
    return read_from_sector();
}

bool read_next_sector(void) {
    disk_buffer = BUFF_AREA;
    return inc_sector_and_read();
}

/* Writes sector disk_sector from disk_buffer, with the motor's spin-up
 * time set for a write. Returns true if the write failed. */
bool write_to_sector(void) {
    uint16_t motor = dct.motor_on_time_count;
    dct.motor_on_time_count = 0xD8EF;
    bool failed = do_rwts_on_sector(2);
    dct.motor_on_time_count = motor;
    return failed;
}

bool inc_sector_and_write(void) {
    disk_sector++;
    return write_to_sector();
}

bool write_next_sector(void) {
    disk_buffer = BUFF_AREA;
    return inc_sector_and_write();
}
