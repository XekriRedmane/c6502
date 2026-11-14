/*
    save.c -- the save and restore instructions.

    A save takes the sectors of one of eight positions on a save
    diskette. The first holds the story's version byte, Z_PC, the
    routine's local variables, and the stack's state; the next two the
    Z-machine's stack; and the rest the story's dynamic memory, a page
    to a sector. Each block is copied last byte first.

    Departures from the original:

    - The sector number is save_sector, where the original keeps it in
      SCRATCH1, and the routines that step it to the next sector,
      read_next_sector, inc_sector_and_read, write_next_sector, and
      inc_sector_and_write, are here. They take the buffer as an
      argument, where the original passes it in SCRATCH2.
    - The copy routines take the address of the bytes as an argument
      and return the index after the bytes they copied, where the
      original passes the address in SCRATCH2 and leaves the index in
      X.
    - instr_restore keeps the transcript bit in a local where the
      original uses SIGN_BIT.
    - Both end by taking the branch when they succeed, branch(true),
      and not taking it when they fail, branch(false), where the
      original jumps to branch and negated_branch.

    Zork I (r15), output/main.asm lines 3634-3772:

    instr_save:
        JSR      please_insert_save_diskette
        LDX      #$00
        LDY      #HEADER_VERSION
        LDA      (Z_HEADER_ADDR),Y
        STA      BUFF_AREA,X
        INX
        STOW     #Z_PC, SCRATCH2
        LDY      #$03
        JSR      copy_data_to_buff
        STOW     #LOCAL_ZVARS, SCRATCH2
        LDY      #30
        JSR      copy_data_to_buff
        STOW     #STACK_COUNT, SCRATCH2
        LDY      #6
        JSR      copy_data_to_buff
        JSR      write_next_sector
        BCS      .fail
        LDX      #$00
        STOW     #$0280, SCRATCH2
        LDY      #$00
        JSR      copy_data_to_buff
        JSR      write_next_sector
        BCS      .fail
        LDX      #$00
        STOW     #$0380, SCRATCH2
        LDY      #$68
        JSR      copy_data_to_buff
        JSR      write_next_sector
        BCS      .fail
        MOVW     Z_HEADER_ADDR, SCRATCH2
        LDY      #HEADER_STATIC_MEM_BASE
        LDA      (Z_HEADER_ADDR),Y
        STA      SCRATCH3                   ; big-endian!
        INC      SCRATCH3
    .loop:
        JSR      inc_sector_and_write
        BCS      .fail
        INC      SCRATCH2+1
        DEC      SCRATCH3
        BNE      .loop
        JSR      inc_sector_and_write
        BCS      .fail
        JSR      please_reinsert_game_diskette
        JMP      branch
    .fail:
        JSR      please_reinsert_game_diskette
        JMP      negated_branch
    copy_data_to_buff:
        DEY
        LDA      (SCRATCH2),Y
        STA      BUFF_AREA,X
        INX
        CPY      #$00
        BNE      copy_data_to_buff
        RTS
    instr_restore:
        JSR      please_insert_save_diskette
        JSR      read_next_sector
        BCC      .continue
        JMP      .fail
    .continue:
        LDX      #$00
        LDY      #HEADER_VERSION
        LDA      (Z_HEADER_ADDR),Y
        CMP      BUFF_AREA,X
        BEQ      .continue2
        JMP      .fail
    .continue2:
        LDY      #HEADER_FLAGS2+1
        LDA      (Z_HEADER_ADDR),Y
        STA      SIGN_BIT
        INX
        STOW     #Z_PC, SCRATCH2
        LDY      #3
        JSR      copy_data_from_buff
        LDA      #$00
        STA      ZCODE_PAGE_VALID
        STOW     #LOCAL_ZVARS, SCRATCH2
        LDY      #30
        JSR      copy_data_from_buff
        STOW     #STACK_COUNT, SCRATCH2
        LDY      #6
        JSR      copy_data_from_buff
        JSR      read_next_sector
        BCS      .fail
        LDX      #$00
        STOW     #$0280, SCRATCH2
        LDY      #$00
        JSR      copy_data_from_buff
        JSR      read_next_sector
        BCS      .fail
        LDX      #$00
        STOW     #$0380, SCRATCH2
        LDY      #$68
        JSR      copy_data_from_buff
        MOVW     Z_HEADER_ADDR, SCRATCH2
        LDY      #HEADER_STATIC_MEM_BASE
        LDA      (Z_HEADER_ADDR),Y
        STA      SCRATCH3               ; big-endian!
        INC      SCRATCH3
    .loop:
        JSR      inc_sector_and_read
        BCS      .fail
        INC      SCRATCH2+1
        DEC      SCRATCH3
        BNE      .loop
        LDA      SIGN_BIT
        LDY      #HEADER_FLAGS2+1
        STA      (Z_HEADER_ADDR),Y
        JSR      please_reinsert_game_diskette
        JMP      branch
    .fail:
        JSR      please_reinsert_game_diskette
        JMP      negated_branch
    copy_data_from_buff:
        DEY
        LDA      BUFF_AREA,X
        STA      (SCRATCH2),Y
        INX
        CPY      #$00
        BNE      copy_data_from_buff
        RTS
*/
#include "zork.h"

/* The save's sector last read or written. */
static uint16_t save_sector;

/* Writes the next sector of the save from `buffer`. Returns true if
 * the write failed. */
[[c6502::return_bool_in_carry]]
static bool inc_sector_and_write(uint8_t *const buffer) {
    save_sector++;
    return write_to_sector(save_sector, buffer);
}

/* Reads the next sector of the save into `buffer`. Returns true if
 * the read failed. */
[[c6502::return_bool_in_carry]]
static bool inc_sector_and_read(uint8_t *const buffer) {
    save_sector++;
    return read_from_sector(save_sector, buffer);
}

/* Copies the `count` bytes at `src` into BUFF_AREA from index `x` on,
 * last byte first, and returns the index after them. A count of 0
 * copies 256. */
static uint8_t copy_data_to_buff(
    const uint8_t *const src, uint8_t x [[c6502::in_x]],
    uint8_t count [[c6502::in_y]]) {
    do {
        count--;
        BUFF_AREA[x] = src[count];
        x++;
    } while (count != 0);
    return x;
}

/* Copies `count` bytes from BUFF_AREA, from index `x` on, to `dest`,
 * last byte first, and returns the index after them. A count of 0
 * copies 256. */
static uint8_t copy_data_from_buff(
    uint8_t *const dest, uint8_t x [[c6502::in_x]],
    uint8_t count [[c6502::in_y]]) {
    do {
        count--;
        dest[count] = BUFF_AREA[x];
        x++;
    } while (count != 0);
    return x;
}

void instr_save(void) {
    save_sector = please_insert_save_diskette();
    uint8_t x = 0;
    BUFF_AREA[x] = Z_HEADER_ADDR[HEADER_VERSION];
    x++;
    x = copy_data_to_buff((const uint8_t *)&Z_PC, x, sizeof Z_PC);
    x = copy_data_to_buff(LOCAL_ZVARS, x, 30);
    copy_data_to_buff(STACK_STATE, x, 6);
    if (inc_sector_and_write(BUFF_AREA))
        goto fail;
    copy_data_to_buff(Z_STACK, 0, 0);
    if (inc_sector_and_write(BUFF_AREA))
        goto fail;
    copy_data_to_buff(Z_STACK + 0x100, 0, 0x68);
    if (inc_sector_and_write(BUFF_AREA))
        goto fail;
    /* One page more than restore reads back. */
    uint8_t *page = Z_HEADER_ADDR;
    uint8_t pages = Z_HEADER_ADDR[HEADER_STATIC_MEM_BASE] + 1;
    do {
        if (inc_sector_and_write(page))
            goto fail;
        page += 0x100;
    } while (--pages != 0);
    if (inc_sector_and_write(page))
        goto fail;
    please_reinsert_game_diskette();
    branch(true);
    return;
fail:
    please_reinsert_game_diskette();
    branch(false);
}

void instr_restore(void) {
    save_sector = please_insert_save_diskette();
    if (inc_sector_and_read(BUFF_AREA))
        goto fail;
    uint8_t x = 0;
    if (Z_HEADER_ADDR[HEADER_VERSION] != BUFF_AREA[x])
        goto fail;
    /* The restored header would replace the transcript bit. */
    uint8_t flags2 = Z_HEADER_ADDR[HEADER_FLAGS2 + 1];
    x++;
    x = copy_data_from_buff((uint8_t *)&Z_PC, x, sizeof Z_PC);
    ZCODE_PAGE_VALID = 0;
    x = copy_data_from_buff(LOCAL_ZVARS, x, 30);
    copy_data_from_buff(STACK_STATE, x, 6);
    if (inc_sector_and_read(BUFF_AREA))
        goto fail;
    copy_data_from_buff(Z_STACK, 0, 0);
    if (inc_sector_and_read(BUFF_AREA))
        goto fail;
    copy_data_from_buff(Z_STACK + 0x100, 0, 0x68);
    uint8_t *page = Z_HEADER_ADDR;
    uint8_t pages = Z_HEADER_ADDR[HEADER_STATIC_MEM_BASE] + 1;
    do {
        if (inc_sector_and_read(page))
            goto fail;
        page += 0x100;
    } while (--pages != 0);
    Z_HEADER_ADDR[HEADER_FLAGS2 + 1] = flags2;
    please_reinsert_game_diskette();
    branch(true);
    return;
fail:
    please_reinsert_game_diskette();
    branch(false);
}
